class ScoreMultiplier : public PhysicalObject
{
public:
    int factor;
    string color;

    ScoreMultiplier() : PhysicalObject("ScoreMultiplier")
    {
        factor = 2;
        setSize(50, 50);
        reset();
    }

    void reset()
    {
        x = random(gameWorldProperties.width/3, -gameWorldProperties.width/3);
        y = random(gameWorldProperties.length/3, -gameWorldProperties.length/3);
        if (factor == 3)
        {
            factor = 2;
            color = "silver";
        }
        else
        {
            factor = 3;
            color = "gold";
        }
    }

    void onCollision(PhysicalObject& that)
    {
        if (that.type == "Blob")
        {
            Blob& blob = (Blob&)that;
            blob.score.addScore(blob.score.score * (factor - 1));
            reset();
        }
        else if (that.type == "Pellet" || that.type == "BigPellet" || that.type == "Splitter" || that.type == "ScoreMultiplier")
        {
            that.reset();
        }
    }
};
