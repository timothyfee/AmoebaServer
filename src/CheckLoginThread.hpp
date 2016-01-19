class CheckLoginThread : public Thread
{
private:
    GameWorld& gameWorld;
    ConnectionPool& connectionPool;
    NetworkMessageMailer& mailer;

    CheckLoginThread(GameWorld& gameWorld, ConnectionPool& connectionPool, NetworkMessageMailer& mailer)
    :   gameWorld(gameWorld), connectionPool(connectionPool), mailer(mailer) {}

    ~CheckLoginThread(){}

    static CheckLoginThread* checkLoginThread;

public:

    void task()
    {
        sleep(60000);
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            if (!connection.isConnected())
            {
                continue;
            }
            if (gameWorld.findPlayerByConnectionId(i) == NULL)
            {
                mailer.push(i, new LoginFailedMessage());
                sleep(100);
                connection.disconnect();
            }
        }
    }

    static void startThread()
    {
        checkLoginThread->start();
    }

    static void deleteThread()
    {
        delete checkLoginThread;
    }

    static void instantiate(GameWorld& gameWorld, ConnectionPool& connectionPool, NetworkMessageMailer& mailer)
    {
        checkLoginThread = new CheckLoginThread(gameWorld, connectionPool, mailer);
    }
};
CheckLoginThread* CheckLoginThread::checkLoginThread;
