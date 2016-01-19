class ScoreBoard
{
public:
    deque<Player*>& players;
    int highScores[10];
    string highNames[10];
    deque<int> currentHighScores;
    deque<int> currentScores;
    deque<string> names;

    ScoreBoard(deque<Player*>& players) : players(players)
    {
        for (int i = 0; i < 10; i++)
        {
            highNames[i] = "";
            highScores[i] = 0;
        }
    }

    void updateHighScores()
    {
        currentHighScores.clear();
        currentScores.clear();
        names.clear();

        for (int i = 0; i < players.size(); i++)
        {
            currentHighScores.push_back(players[i]->score.highScore);
            currentScores.push_back(players[i]->score.score);
            names.push_back(players[i]->username);

            for (int c = 0; c < 10; c++)
            {
                if (players[i]->score.highScore >= highScores[c])
                {
                    if (players[i]->score.highScore == highScores[c])
                    {
                        break;
                    }
                    if (c != 9 && highNames[c] != players[i]->username)
                    {
                        highScores[c + 1] = highScores[c];
                        highNames[c + 1] = highNames[c];
                    }
                    highScores[c] = players[i]->score.highScore;
                    highNames[c] = players[i]->username;
                    break;
                }
            }

            for (int i = 0; i < currentHighScores.size(); i++)
            {
                for (int c = i; c < currentHighScores.size(); c++)
                {
                    if (currentHighScores[c] > currentHighScores[i])
                    {
                        int hs = currentHighScores[i];
                        int cs = currentScores[i];
                        string n = names[i];

                        currentHighScores[i] = currentHighScores[c];
                        currentScores[i] = currentScores[c];
                        names[i] = names[c];

                        currentHighScores[c] = hs;
                        currentScores[c] = cs;
                        names[c] = n;
                    }
                }
            }
        }
    }

    string presentHighScores()
    {
        string text = "";
        for (int i = 0; i < 10; i++)
        {
            text += "\n" + toString(i +1) + " " + highNames[i] + " " + toString(highScores[i]);
        }
        return text;
    }

    string presentCurrentScores()
    {
        string text = "";
        for (int i = 0; i < currentHighScores.size(); i++)
        {
            text += "\n" + toString(i +1) + " " + names[i] + " " + toString(currentHighScores[i]) + " " + toString(currentScores[i]);
        }
        return text;
    }
};
