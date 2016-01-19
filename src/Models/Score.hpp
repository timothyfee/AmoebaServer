class Score
{
public:
    int score;
    int highScore;

    Score()
    {
        score = 0;
        highScore = 0;
    }

    void addScore(int points)
    {
        score += points;
        if (score > highScore)
        {
            highScore = score;
        }
    }
};
