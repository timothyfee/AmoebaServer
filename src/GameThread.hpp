class GameThread : public Thread
{
private:
    GameWorld gameWorld;
    CommandFactory commandFactory;
    GameWorldPoller poller;
    ConnectionPool& connectionPool;
    NetworkMessageMailer &incomingMailer;
    NetworkMessageMailer &outgoingMailer;
    int startingSpeed;

public:

    GameThread(NetworkMessageMailer& incomingMailer, NetworkMessageMailer& outgoingMailer, ConnectionPool& connectionPool)
    :   incomingMailer(incomingMailer), outgoingMailer(outgoingMailer), connectionPool(connectionPool),
        commandFactory(CommandFactory(gameWorld, connectionPool, outgoingMailer)), poller(GameWorldPoller(gameWorld)) {}

    void setUp()
    {
        CheckLoginThread::instantiate(gameWorld, connectionPool, outgoingMailer);
        ScoreKeepingThread::instantiate(gameWorld, outgoingMailer);
        startingSpeed = threadInterval;
    }

    void cleanUp()
    {
        CheckLoginThread::deleteThread();
        ScoreKeepingThread::deleteThread();
    }

    void task()
    {
        deque<NetworkMessage*> incomingMessages = incomingMailer.dump();
        deque<Command*> commands = commandFactory.generateCommands(incomingMessages);
        executeCommands(commands);
        gameWorld.update();
        if (outgoingMailer.messageCount() == 0)
        {
            deque<NetworkMessage*> outgoingMessages = poller.poll();
            outgoingMailer.push(outgoingMessages);
        }
        threadInterval = gameWorld.players.size() > 1 ? startingSpeed / gameWorld.players.size() : startingSpeed;
    }

private:

    inline void executeCommands(deque<Command*> commands)
    {
        for (int i = 0; i < commands.size(); i++)
        {
            commands[i]->execute();
            delete commands[i];
        }
    }
};
