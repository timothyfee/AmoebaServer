class SenderThread : public Thread
{
private:
    string stream;
    NetworkMessageMailer& mailer;
    MessageWareContainer& messageWareContainer;
    Interpreter& compiler;
    PairWareContainer& pairWareContainer;
    Codec& encoder;
    StringWareContainer& stringWareContainer;
    ConnectionBuffer sender;
    ConnectionPool& connectionPool;

public:

    SenderThread(ConnectionPool& connectionPool,
                 NetworkMessageMailer& mailer,
                 MessageWareContainer& messageWareContainer,
                 Interpreter& compiler,
                 PairWareContainer& pairWareContainer,
                 Codec& encoder,
                 StringWareContainer& stringWareContainer)
        :   connectionPool(connectionPool), mailer(mailer),
            messageWareContainer(messageWareContainer), compiler(compiler),
            pairWareContainer(pairWareContainer), encoder(encoder),
            stringWareContainer(stringWareContainer), sender(ConnectionBuffer(encoder)) {}

    void task()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            deque<NetworkMessage*> messages = mailer.dump(i);
            deque<NetworkMessage*> processedMessages = messageWareContainer.processForOutput(messages, i);
            deque<KeyValuePair> pairs = compiler.compile(processedMessages);
            Connection& connection = connectionPool.getConnection(i);
            if (connection.isConnected())
            {
                deque<KeyValuePair> processedPairs = pairWareContainer.processForOutput(pairs, i);
                stream = encoder.encode(processedPairs);
                stream = stringWareContainer.processForOutput(stream, i);
                sender.sendDataStream(connection, stream);
            }
        }
    }
};
