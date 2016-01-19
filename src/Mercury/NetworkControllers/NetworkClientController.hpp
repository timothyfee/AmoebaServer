class NetworkClientController : public NetworkController
{
protected:
    NetworkClientController(Codec& codec, Interpreter& interpreter, int maxConnections = 1, int connectionTimeoutCycles = 1000)
    :   NetworkController(codec, interpreter, maxConnections, connectionTimeoutCycles) {}

public:

    virtual bool connect(string address, int port) = 0;

    virtual void disconnect() = 0;

    virtual bool isConnected() = 0;
};
