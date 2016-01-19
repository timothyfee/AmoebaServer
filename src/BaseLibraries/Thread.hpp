#ifndef _usingStandardFunctionsLib_
#include <windows.h>
#include <process.h>
#include <exception>
#include <unistd.h>
#endif
#define _usingThread_

enum ThreadTiming {nanoSeconds, milliSeconds};

namespace ThreadingDoNotUseThisNamespaceInYourProgram
{
void task(void*);
class _Thread;
}

class Thread
{
private:
    bool running;
    bool started;
    HANDLE realThread;

protected:

    Thread()
    {
        running = false;
        started = false;
        repetitions = 0;
        threadInterval = 1;
        timing = milliSeconds;
    }

    void setRunningProperty(bool r)
    {
        running = r;
        started = true;
    }

public:
    static const int infiniteLoop = -1;
    int repetitions;
    int threadInterval;
    ThreadTiming timing;

    bool isRealThreadRunning()
    {
        DWORD code = 0;
        GetExitCodeThread(realThread, &code);
        if (code == STILL_ACTIVE)
        {
            return true;
        }
        return false;
    }

    bool isRunning()
    {
        return running;
    }

    bool hasStarted()
    {
        return started;
    }

    virtual void preStart() {}

    virtual void setUp() {}

    virtual void task() = 0;

    virtual void cleanUp() {}

    void handleException(std::exception e) {}

    virtual void stop()
    {
        repetitions = 0;
        TerminateThread(realThread, 0);
        usleep(1);
        running = false;
        cleanUp();
    }

    virtual ~Thread()
    {
        if (isRunning())
        {
            stop();
        }
        CloseHandle(realThread);
    }

    virtual bool start(int repetitions = 1, int threadInterval = -1, ThreadTiming timing = milliSeconds)
    {
        if (!running)
        {
            this->repetitions = repetitions;
            this->threadInterval = threadInterval > 0 ? threadInterval : this->threadInterval;
            this->timing = timing;
            preStart();
            realThread = (HANDLE)_beginthread(ThreadingDoNotUseThisNamespaceInYourProgram::task, 0, this);
            usleep(1);
        }
        return running;
    }

    virtual bool run(int repetitions = 1, int threadInterval = -1, ThreadTiming timing = milliSeconds)
    {
        if (running)
        {
            return false;
        }
        this->repetitions = repetitions;
        this->threadInterval = threadInterval > 0 ? threadInterval : this->threadInterval;
        this->timing = timing;
        running = true;
        started = true;
        preStart();
        try
        {
            setUp();
            while(repetitions == INFINITE || repetitions > 0)
            {
                task();
                switch(timing)
                {
                case milliSeconds:
                    Sleep(threadInterval);
                    break;
                case nanoSeconds:
                    usleep(threadInterval);
                    break;
                };
                if (repetitions != INFINITE)
                {
                    repetitions--;
                }
            }
            running = false;
        }
        catch (std::exception e)
        {
            running = false;
            handleException(e);
        }
        if (!running)
        {
            cleanUp();
        }
    }
};

class ThreadingDoNotUseThisNamespaceInYourProgram::_Thread : public Thread
{
private:

    _Thread() {}

public:

    void setRunningProperty(bool r)
    {
        Thread::setRunningProperty(r);
    }
};

void ThreadingDoNotUseThisNamespaceInYourProgram::task(void* threadPtr)
{
    _Thread &thread = *(_Thread*)threadPtr;
    thread.setRunningProperty(true);
    try
    {
        thread.setUp();
        thread.task();
        while (thread.repetitions == Thread::infiniteLoop || thread.repetitions > 0)
        {
            thread.task();
            switch(thread.timing)
            {
            case milliSeconds:
                Sleep(thread.threadInterval);
                break;
            case nanoSeconds:
                usleep(thread.threadInterval);
                break;
            };
            if (thread.repetitions != Thread::infiniteLoop)
            {
                thread.repetitions--;
            }
        }
        thread.setRunningProperty(false);
    }
    catch (std::exception e)
    {
        thread.setRunningProperty(false);
        thread.handleException(e);
    }
    if (!thread.isRunning())
    {
        thread.cleanUp();
    }
    _endthread();
}
