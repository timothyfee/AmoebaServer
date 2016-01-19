class AmoebaNetworkClientController : public NetworkClientController
{
private:
    NetworkMessageMailer& incoming;
    NetworkMessageMailer& outgoing;

public:
    AmoebaInterpreter interpreter;
    AmoebaCodec codec;

    AmoebaNetworkClientController()
    :   NetworkClientController(codec, interpreter),
        incoming(NetworkController::incomingMailer), outgoing(NetworkController::outgoingMailer)
    {
    }

    bool connect(string address, int port)
    {
        if (connectionPool.getConnection(ConnectionPool::FIRST_CONNECTION).connect(address, port, 3))
        {
            start();
            return true;
        }
        return false;
    }

    void disconnect()
    {
        connectionPool.getConnection(ConnectionPool::FIRST_CONNECTION).disconnect();
    }

    bool isConnected()
    {
        return connectionPool.getConnection(ConnectionPool::FIRST_CONNECTION).isConnected();
    }

    void sendMessage(NetworkMessage* message)
    {
        outgoing.push(ConnectionPool::FIRST_CONNECTION, message);
    }

    deque<NetworkMessage*> getMessages()
    {
        return incoming.dump();
    }
};
