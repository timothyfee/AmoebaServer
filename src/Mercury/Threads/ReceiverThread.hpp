class ReceiverThread : public Thread
{
private:
    string stream;
    int connectionTimeoutCycles;
    ConnectionPool& connectionPool;
    ConnectionBuffer listener;
    StringWareContainer& stringWareContainer;
    Codec& decoder;
    PairWareContainer& pairWareContainer;
    Interpreter& parser;
    MessageWareContainer& messageWareContainer;
    NetworkMessageMailer& mailer;

public:

    ReceiverThread(ConnectionPool& connectionPool,
                   NetworkMessageMailer& mailer,
                   MessageWareContainer& messageWareContainer,
                   Interpreter& parser,
                   PairWareContainer& pairWareContainer,
                   Codec& decoder,
                   StringWareContainer& stringWareContainer,
                   int connectionTimeoutCycles)
        :   connectionTimeoutCycles(connectionTimeoutCycles), connectionPool(connectionPool), mailer(mailer),
            messageWareContainer(messageWareContainer), parser(parser),
            pairWareContainer(pairWareContainer), decoder(decoder),
            stringWareContainer(stringWareContainer), listener(ConnectionBuffer(decoder)) {}

    void task()
    {
        listenToConnections();
        cutOffDeadConnections();
    }

private:

    inline void listenToConnections()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            if (connection.isConnected())
            {
                stream = listener.getDataStream(connection);
                stream = stringWareContainer.processForInput(stream, i);
                deque<KeyValuePair> pairs = decoder.decode(stream);
                deque<KeyValuePair> processedPairs = pairWareContainer.processForInput(pairs, i);
                deque<NetworkMessage*> messages = parser.parse(processedPairs);
                deque<NetworkMessage*> processedMessages = messageWareContainer.processForInput(messages, i);
                mailer.push(i, processedMessages);
            }
        }
    }

    inline void cutOffDeadConnections()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            if (!connection.isConnected())
            {
                continue;
            }
            else if (connection.inactiveTime > connectionTimeoutCycles)
            {
                connection.disconnect();
                mailer.push(i, new ConnectionTimeoutMessage());
                connection.inactiveTime = 0;
            }
            else if (connection.isActive())
            {
                connection.inactiveTime = 0;
            }
            else if (!connection.isActive())
            {
                connection.inactiveTime++;
            }
        }
    }
};
