class Splitter : public PhysicalObject
{
public:

    Splitter() : PhysicalObject("Splitter")
    {
        setSize(50, 50);
        reset();
    }

    void reset()
    {
        x = random(gameWorldProperties.width/3, -gameWorldProperties.width/3);
        y = random(gameWorldProperties.length/3, -gameWorldProperties.length/3);
    }

    void onCollision(PhysicalObject& that)
    {
        if (that.type == "Blob")
        {
            Blob& blob = (Blob&)that;
            blob.bounce();
            blob.split();
        }
        else if (that.type == "Pellet" || that.type == "BigPellet" || that.type == "Splitter" || that.type == "ScoreMultiplier")
        {
            that.reset();
        }
    }
};
