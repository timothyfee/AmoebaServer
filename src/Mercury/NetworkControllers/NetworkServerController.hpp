class NetworkServerController : public NetworkController
{
protected:
    ConnectorThread connector;

    NetworkServerController(Codec& codec, Interpreter& interpreter, int maxConnections, int port, string acceptableAddress = "0", int connectionTimeoutCycles = 1000)
    :   NetworkController(codec, interpreter, maxConnections, connectionTimeoutCycles),
        connector(ConnectorThread(connectionPool, incomingMailer, port, acceptableAddress)) {}

public:

    void start()
    {
        connector.start(Thread::infiniteLoop);
        NetworkController::start();
    }

    void stop()
    {
        connector.stop();
        NetworkController::stop();
    }
};
