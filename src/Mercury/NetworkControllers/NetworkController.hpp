class NetworkController
{
private:
    StringWareContainer stringWareContainer;
    PairWareContainer pairWareContainer;
    MessageWareContainer messageWareContainer;

protected:
    ReceiverThread receiver;
    SenderThread sender;

    void attach(StringWare* middleWare)
    {
        stringWareContainer.add(middleWare);
    }

    void attach(StringWare& middleWare)
    {
        stringWareContainer.add(middleWare);
    }

    void attach(PairWare* middleWare)
    {
        pairWareContainer.add(middleWare);
    }

    void attach(PairWare& middleWare)
    {
        pairWareContainer.add(middleWare);
    }

    void attach(MessageWare* middleWare)
    {
        messageWareContainer.add(middleWare);
    }

    void attach(MessageWare& middleWare)
    {
        messageWareContainer.add(middleWare);
    }

    NetworkController(Codec& codec, Interpreter& interpreter, int maxConnections, int connectionTimeoutCycles)
    :   connectionPool(ConnectionPool(maxConnections)),
        receiver(ReceiverThread(connectionPool, incomingMailer, messageWareContainer, interpreter, pairWareContainer, codec, stringWareContainer, connectionTimeoutCycles)),
        sender(SenderThread(connectionPool, outgoingMailer, messageWareContainer, interpreter, pairWareContainer, codec, stringWareContainer)) {}

public:
    ConnectionPool connectionPool;
    NetworkMessageMailer incomingMailer;
    NetworkMessageMailer outgoingMailer;

    virtual void start()
    {
        receiver.start(Thread::infiniteLoop);
        sender.start(Thread::infiniteLoop);
    }

    virtual void stop()
    {
        sender.stop();
        receiver.stop();
    }
};
