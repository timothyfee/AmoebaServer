#define _usingStandardFunctionsLib_
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <conio.h>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <deque>
#include <sstream>
#include <tchar.h>
#include <ctime>
#include <typeinfo>
#include <process.h>
#include <exception>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif
#include <ws2tcpip.h>
#include <windows.h>

#define sleep(long) Sleep(long)
#define nanoSleep(long) usleep(long)

using namespace std;

inline string toLower(string in)
{
    string ret = "";
    for (int i = 0; i < in.length(); i++)
    {
        char c = in[i];
        c = tolower(c);
        ret += c;
    }
    return ret;
}

inline char toLower(char in)
{
    return tolower(in);
}

template<typename T>
inline string toString(T var)
{
    stringstream s;
    s << var;
    return s.str();
}

template<typename T>
inline char* toCString(T var)
{
    return (char*)toString(var).c_str();
}

template<typename T>
inline int toInt(T var)
{
    return atoi(toCString(var));
}

template<typename T>
inline long toLong(T var)
{
    return atol(toCString(var));
}

template<typename T>
inline double toDouble(T var)
{
    return atof(toCString(var));
}

template<typename T>
inline float toFloat(T var)
{
    return atof(toCString(var));
}

inline bool toBool(string in)
{
    in = toLower(in);
    if (in == "1" || in == "true" || "t")
    {
        return true;
    }
    return false;
}

inline bool toBool(char in)
{
    in = toLower(in);
    if (in == '1' || in == 't')
    {
        return true;
    }
    return false;
}

inline int timestampMilliseconds()
{
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}

inline string dateTimeStamp()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buf[100];
    time (&rawtime);
    timeinfo=localtime (&rawtime);
    strftime (buf,100,"%I:%M %p - %x",timeinfo);
    return buf;
}

string replace(string originalPattern, string replacementPattern, string text)
{
    string ret = text;
    deque<int> points;
    int currPoint = text.find(originalPattern);

    while (currPoint != string::npos)
    {
        points.push_back(currPoint);
        currPoint = text.find(originalPattern, ++currPoint);
    }

    for (int i = points.size() - 1; i >= 0; i--)
    {
        ret.replace(points[i], originalPattern.length(), replacementPattern);
    }
    return ret;
}

string replace(char originalChar, char replacementChar, string text)
{
    string ret = text;
    int pos = text.find(originalChar);
    int lastPos = string::npos;
    while (pos != string::npos && pos != lastPos)
    {
        ret[pos] = replacementChar;
        lastPos = pos;
        pos = ret.find(originalChar);
    }
    return ret;
}

inline bool contains(string pattern, string text)
{
    if (text.find(pattern) != string::npos)
    {
        return true;
    }
    return false;
}

inline bool contains(char c, string text)
{
    if (text.find(c) != string::npos)
    {
        return true;
    }
    return false;
}

inline string reverse(string s)
{
    string ret = "";
    for (int i = s.length() - 1; i > -1; i--)
    {
        ret += s[i];
    }
    return ret;
}

inline bool fileExists(string filename)
{
    ifstream file(toCString(filename));
    if (file)
    {
        file.close();
        return true;
    }
    return false;
}

unsigned long fileByteLength(string filename)
{
    unsigned long ret = 0;
    if (filename == "")
    {
        return 0;
    }
    FILE* file = fopen(toCString(filename), "rb");
    fseek (file, 0, SEEK_END);
    ret = ftell(file);
    rewind(file);
    fclose(file);
    return ret;
}

deque<string> readDir(string path)
{
    deque<string> ret;
    path += "/*";
    _finddata_t data;
    int ff = _findfirst (toCString(path), &data);
    if (ff != -1)
    {
        int res = 0;
        while (res != -1)
        {
            if (strcmp(data.name, ".") && strcmp(data.name, ".."))
            {
                ret.push_back(data.name);
            }
            res = _findnext(ff, &data);
        }
        _findclose(ff);
    }
    return ret;
}

string readTextFile(string filename)
{
    string data = "";
    string line = "";
    ifstream file(toCString(filename));
    if (!file)
    {
        return "";
    }
    getline(file, data);
    while (getline(file, line))
    {
        data += "\n" + line;
    }
    file.close();
    return data;
}

inline void startProcess(string path)
{
    popen(toCString(path), "r");
}

void startBackgroundProcess(string command)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    si.cb = sizeof(si);
    CreateProcess(NULL, //full path to exe
                  (char*)command.c_str(), //cmd line options
                  NULL, //process security attr
                  NULL, //thread security attr
                  false, //inherit handles flag
                  CREATE_NO_WINDOW,//process creation flags
                  NULL,//environment
                  NULL, //current directory
                  &si,//startup info
                  &pi//process info
                 );
}

struct KeyValuePair
{
    string field;
    string value;

    KeyValuePair(string field = "", string value = "") : field(field), value(value) {}
};

#define switchString(s) { string qwerasdf = s;
#define primaryCaseString(s) if (qwerasdf == s){
#define caseString(s) else if (qwerasdf == s){
#define endCase }
#define endStringSwitch }
