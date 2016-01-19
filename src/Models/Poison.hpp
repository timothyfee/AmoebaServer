class Poison : PhysicalObject
{
public:
    int timer;
    double currentSize;

    Poison() : PhysicalObject("Poison")
    {
        tangibility = GHOSTLIKE;
        reset();
    }

    void reset()
    {
        timer = 2000;
        currentSize = 0;
        x = random(gameWorldProperties.width/3, -gameWorldProperties.width/3);
        y = random(gameWorldProperties.length/3, -gameWorldProperties.length/3);
    }

    void onStep()
    {
        setSize(currentSize, currentSize);
        if (timer > 1500)
        {
            currentSize += 0.5;
        }
        else if (timer < 1000 && timer > 0)
        {
            currentSize -= 0.25;
        }
        else if (timer == 0)
        {
            reset();
            return;
        }
        timer--;
    }

    void onCollision(PhysicalObject& that)
    {
        if (that.type == "Blob")
        {
            Blob& blob = (Blob&)that;
            if (blob.currentSize > Blob::minSize)
            {
                blob.currentSize--;
            }
            else
            {
                blob.kill();
            }
        }
    }
};
