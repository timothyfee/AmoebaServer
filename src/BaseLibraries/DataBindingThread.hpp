class DataBindingThread : public Thread
{
private:

    DataBindingThread()
    {
        start(Thread::infiniteLoop);
    }

    ~DataBindingThread(){}

    static DataBindingThread dataBinder;

public:

    void task()
    {
        deque<Bindable*> bindings = Bindable::getBindings();
        for (int i = 0; !Bindable::dirty && i < bindings.size(); i++)
        {
            bindings[i]->synchronize();
        }
    }
};
DataBindingThread DataBindingThread::dataBinder;
