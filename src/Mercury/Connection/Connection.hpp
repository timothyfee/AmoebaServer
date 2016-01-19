class Connection
{
public:
    SocketBase* socket;
    int connectionId;
    int inactiveTime;

    Connection() : socket(NULL)
    {
        inactiveTime = 0;
        connectionId = -1;
    }

    void sendData(string data)
    {
        if (socket != NULL)
        {
            socket->sendData(data);
        }
    }

    string getData()
    {
        if (socket != NULL)
        {
            return socket->getString();
        }
        return "";
    }

    bool isConnected()
    {
        if (socket != NULL)
        {
            return socket->isConnected();
        }
        return false;
    }

    bool isActive()
    {
        if (socket != NULL)
        {
            return socket->isActive();
        }
        return false;
    }

    string getIpAddress()
    {
        if (socket != NULL)
        {
            return socket->getRemoteAddress();
        }
        return "";
    }

    int getPort()
    {
        if (socket != NULL)
        {
            return socket->getPort();
        }
        return -1;
    }

    bool accept(int port, string address = "0")
    {
        inactiveTime = 0;
        ServerSocket* serverSocket = new ServerSocket();
        socket = serverSocket;
        return serverSocket->accept(port, address);
    }

    bool connect(string address, int port, int attempts = 3)
    {
        inactiveTime = 0;
        ClientSocket* clientSocket = new ClientSocket();
        socket = clientSocket;
        return clientSocket->connect(address, port, attempts);
    }

    void disconnect()
    {
        if (socket != NULL)
        {
            delete socket;
            socket = NULL;
        }
    }
};
