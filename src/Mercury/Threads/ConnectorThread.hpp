class ConnectorThread : public Thread
{
private:
    ConnectionPool& connectionPool;
    NetworkMessageMailer& mailer;
    int port;
    string address;

public:

    ConnectorThread(ConnectionPool &connectionPool, NetworkMessageMailer &mailer, int port, string acceptableAddress)
    :   connectionPool(connectionPool), mailer(mailer), port(port), address(acceptableAddress) {}

    void task()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            if (connection.isConnected())
            {
                continue;
            }
            connection.accept(port, address);
            mailer.push(i, new NewConnectionMessage(connection.getIpAddress(), port));
        }
    }
};
