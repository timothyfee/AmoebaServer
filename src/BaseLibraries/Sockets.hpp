#ifndef _usingStandardFunctionsLib_
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
unsigned long fileByteLength(std::string filename)
{
    unsigned long ret = 0;
    if (filename == "")
    {
        return 0;
    }
    FILE* file = fopen((char*)filename.c_str(), "rb");
    fseek (file, 0, SEEK_END);
    ret = ftell(file);
    rewind(file);
    fclose(file);
    return ret;
}
using namespace std;
#endif
#define _usingSockets_

class SocketBase
{
private:
    char* receiveBuffer;
    static bool wsaStarted;
    unsigned long receiveBufferSize;

protected:
    std::string targetAddress;
    std::string remoteAddress;
    int port;
    bool active;
    bool connected;
    int connectWaitTime;
    int sendWaitTime;

    SocketBase()
    {
        receiveBufferSize = 10000;
        receiveBuffer = new char[receiveBufferSize];
        connected = false;
        active = false;
        targetAddress = "";
        remoteAddress = "";
        port = 0;
        connectWaitTime = 0;
        sendWaitTime = 0;
        startWinsock();
    }

public:
    SOCKET rawWinsockSocket;

    static void startWinsock()
    {
        if (wsaStarted)
        {
            return;
        }
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2,2), &wsaData) == 0)
        {
            wsaStarted = true;
        }
    }

    static void stopWinsock()
    {
        WSACleanup();
        wsaStarted = false;
    }

    static void restartWinsock()
    {
        stopWinsock();
        startWinsock();
    }

    void setReceiveBufferSize(unsigned long size)
    {
        delete[] receiveBuffer;
        receiveBuffer = new char[size];
        receiveBufferSize = size;
    }

    unsigned long getReceiveBufferSize()
    {
        return receiveBufferSize;
    }

    void sendRawData(char* sendBuffer, unsigned long length, DWORD flags = 0)
    {
        send(rawWinsockSocket, sendBuffer, length, flags);
    }

    void sendBinaryFile(std::string filename, unsigned long fileLength = 0)
    {
        if (filename == "")
        {
            return;
        }
        if (fileLength == 0)
        {
            fileLength = fileByteLength(filename);
        }
        FILE* file = fopen((char*)filename.c_str(), "rb");
        char* sendbuf = new char[sizeof(char)*fileLength];
        size_t result = fread(sendbuf, 1, fileLength, file);
        if (result > 0)
        {
            sendRawData(sendbuf, result);
        }
        delete[] sendbuf;
        fclose(file);
    }

    void setConnectWaitTime(int milliseconds)
    {
        connectWaitTime = milliseconds;
    }

    void setSendWaitTime(int milliseconds)
    {
        sendWaitTime = milliseconds;
    }

    void disconnect()
    {
        if (connected)
        {
            // In accordance with the rules of tcp/ip a socket should be read from at least once before closing it.
            // Not attempting to read from a socket on the server side before disconnecting it can result in connection reset errors on many browsers.
            getCString(); // <-----
            closesocket(rawWinsockSocket);
        }
        connected = false;
        active = false;
        remoteAddress = "";
        Sleep(connectWaitTime);
    }

    virtual ~SocketBase()
    {
        disconnect();
        delete[] receiveBuffer;
    }

    static int getNetworkAdapterCount()
    {
        char ac[256];
        gethostname(ac, sizeof(ac));
        struct hostent *phe = gethostbyname(ac);
        for (int i = 0; phe->h_addr_list[i] != 0; i++)
        {
            if (phe->h_addr_list[i + 1] == 0)
            {
                return i + 1;
            }
        }
        return 0;
    }

    static std::string getLocalAddress(int adapter = 0)
    {
        char ac[256];
        gethostname(ac, sizeof(ac));
        struct hostent *phe = gethostbyname(ac);
        if (phe->h_addr_list[adapter] != 0)
        {
            struct in_addr addr;
            memcpy(&addr, phe->h_addr_list[adapter], sizeof(struct in_addr));
            return inet_ntoa(addr);
        }
        return "";
    }

    static std::string getLocalHostname()
    {
        char host[256] = "";
        gethostname(host, sizeof(host));
        return host;
    }

    std::string getRemoteAddress()
    {
        return remoteAddress;
    }

    std::string getTargetAddress()
    {
        return targetAddress;
    }

    std::string getRemoteHostname()
    {
        struct sockaddr_in saGNI;
        char hostname[256];
        char servInfo[256];
        saGNI.sin_family = AF_INET;
        saGNI.sin_addr.s_addr = inet_addr((char*)remoteAddress.c_str());
        saGNI.sin_port = htons(port);
        DWORD dwRetval;
        dwRetval = getnameinfo((struct sockaddr *) &saGNI,
                               sizeof (struct sockaddr),
                               hostname,
                               256, servInfo, 256, 0);
        return hostname;
    }

    int getPort()
    {
        return port;
    }

    bool isActive()
    {
        return active;
    }

    bool isConnected()
    {
        return connected;
    }

    bool isWsaStarted()
    {
        return wsaStarted;
    }

    virtual void sendData(char* data, int length = 0)
    {
        if (length == 0)
        {
            for (true; data[length]; length++) {}
        }
        char *sendBuffer = new char[length];
        strncpy(sendBuffer, "", length);
        strncpy(sendBuffer, data, length);
        sendRawData(sendBuffer, length, 0);
        Sleep(sendWaitTime);
        delete[] sendBuffer;
    }

    template<typename T>
    void sendData(T data)
    {
        stringstream s;
        s << data;
        sendData((char*)s.str().c_str());
    }

    char* getCString()
    {
        strncpy(receiveBuffer, "", receiveBufferSize);
        recv(rawWinsockSocket, receiveBuffer, receiveBufferSize, 0);
        active = strlen(receiveBuffer) > 0;
        return receiveBuffer;
    }

    std::string getString()
    {
        return getCString();
    }

    int getInteger()
    {
        return atoi(getCString());
    }

    double getDouble()
    {
        return atof(getCString());
    }

    float getFloat()
    {
        return atof(getCString());
    }

    long getLong()
    {
        return atol(getCString());
    }
};
bool SocketBase::wsaStarted = false;

class ServerSocket : public SocketBase
{
public:

    bool accept(int clientPort, string clientAddress = "0")
    {
        disconnect();
        targetAddress = clientAddress;
        port = clientPort;
        // Temporary istening socket
        SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        // Setup listening socket
        SOCKADDR_IN SockAddr;
        SockAddr.sin_port = htons(port);
        SockAddr.sin_family = AF_INET;
        SockAddr.sin_addr.s_addr = inet_addr((char*)clientAddress.c_str());
        bind(s, (SOCKADDR*)&SockAddr, sizeof(SockAddr));
        int addrlen = sizeof(SockAddr);
        listen(s, SOMAXCONN);
        if (rawWinsockSocket = ::accept(s, (SOCKADDR*)&SockAddr, &addrlen))
        {
            //  Non-Blocking Mode
            u_long iMode = 1;
            ioctlsocket(rawWinsockSocket, FIONBIO, &iMode);
            // Set receive timeout for socket
            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
            setsockopt((SOCKET)rawWinsockSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

            remoteAddress = inet_ntoa(SockAddr.sin_addr);
            connected = true;
            active = true;
        }
        closesocket(s);
        Sleep(connectWaitTime);
        return connected;
    }
};

class ClientSocket : public SocketBase
{
public:

    bool connect(std::string serverAddress, int serverPort)
    {
        if (connected)
        {
            disconnect();
        }
        connected = false;
        targetAddress = serverAddress;
        port = serverPort;
        rawWinsockSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        struct hostent *host;
        host = gethostbyname((char*)serverAddress.c_str());
        SOCKADDR_IN SockAddr;
        SockAddr.sin_port = htons(serverPort);
        SockAddr.sin_family = AF_INET;
        SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
        if(::connect(rawWinsockSocket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) == 0)
        {
            // Set receive timeout for socket
            struct timeval tv;
            tv.tv_sec = 1;
            setsockopt(rawWinsockSocket, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));

            remoteAddress = targetAddress;
            connected = true;
            active = true;
        }
        Sleep(connectWaitTime);
        return connected;
    }

    bool connect(std::string serverAddress, int serverPort, int attempts)
    {
        for (int i = 0; !connected && i < attempts; i++)
        {
            connect(serverAddress, serverPort);
        }
        return connected;
    }
};
