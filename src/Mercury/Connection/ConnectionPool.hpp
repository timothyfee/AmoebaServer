class ConnectionPool
{
private:
    int nextConnectionId;
    deque<Connection> connections;

public:
    int maxConnections;

    static const int FIRST_CONNECTION = 0;
    static const int SECOND_CONNECTION = 1;
    static const int THIRD_CONNECTION = 2;

    ConnectionPool(int maxConnections)
    {
        this->maxConnections = maxConnections;
        nextConnectionId = 0;
        add(maxConnections);
    }

    int size()
    {
        return connections.size();
    }

    bool add()
    {
        if (connections.size() < maxConnections)
        {
            Connection newConnection;
            newConnection.connectionId = nextConnectionId;
            nextConnectionId++;
            connections.push_back(newConnection);
            return true;
        }
        return false;
    }

    bool add(int count)
    {
        bool ret = true;
        for (int i = 0; ret && i < count; i++)
        {
            ret = add();
        }
        return ret;
    }

    void remove(int index)
    {
        if (connections.size() != 0)
        {
            connections[index].disconnect();
            connections.erase(connections.begin() + index);
        }
    }

    Connection& getConnection(int index)
    {
        return connections[index];
    }

    Connection* getConnectionById(int connectionId)
    {
        for (int i = 0; i < connections.size(); i++)
        {
            if (connections[i].connectionId == connectionId)
            {
                return &connections[i];
            }
        }
        return NULL;
    }
};
