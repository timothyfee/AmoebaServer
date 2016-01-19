class ScoreKeepingThread : public Thread
{
private:
    GameWorld& gameWorld;
    NetworkMessageMailer& mailer;

    ScoreKeepingThread(GameWorld& gameWorld, NetworkMessageMailer& mailer) : gameWorld(gameWorld), mailer(mailer)
    {
        start(Thread::infiniteLoop, 1000);
    }

    ~ScoreKeepingThread(){}

    static ScoreKeepingThread* scoreKeeper;

public:

    void task()
    {
        gameWorld.scoreBoard.updateHighScores();
        string highScoreText = gameWorld.scoreBoard.presentHighScores();
        string currentScoreText = gameWorld.scoreBoard.presentCurrentScores();
        deque<NetworkMessage*> messages;
        for (int i = 0; i < gameWorld.players.size(); i++)
        {
            HighScoreMessage* hm = new HighScoreMessage(highScoreText);
            CurrentScoreMessage* cm = new CurrentScoreMessage(currentScoreText);
            hm->connectionId = gameWorld.players[i]->connectionId;
            cm->connectionId = hm->connectionId;
            messages.push_back(hm);
            messages.push_back(cm);
        }
        mailer.push(messages);
    }

    static void instantiate(GameWorld& gameWorld, NetworkMessageMailer& mailer)
    {
        scoreKeeper = new ScoreKeepingThread(gameWorld, mailer);
    }

    static void deleteThread()
    {
        delete scoreKeeper;
    }
};
ScoreKeepingThread* ScoreKeepingThread::scoreKeeper;
