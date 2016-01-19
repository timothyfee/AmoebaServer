class Bomb : public PhysicalObject
{
public:
    string color;
    bool armed;
    bool exploding;
    int timer;

    Bomb() : PhysicalObject("Bomb")
    {
        reset();
    }

    void reset()
    {
        setSize(50, 50);
        x = random(gameWorldProperties.width/3, -gameWorldProperties.width/3);
        y = random(gameWorldProperties.length/3, -gameWorldProperties.length/3);
        color = "black";
        armed = false;
        exploding = false;
        timer = 0;
    }

    void onStep()
    {
        if (armed)
        {
            if (timer == 0)
            {
                explode();
            }
            timer -= timer > 0 ? 1 : 0;
        }
    }

    void explode()
    {
        exploding = true;
        color = "white";
        if (width < 1500)
        {
            width += 10;
        }
        else
        {
            reset();
        }
    }

    void arm()
    {
        if (!armed)
        {
            color = "red";
            timer = 50;
            armed = true;
        }
    }

    void onCollision(PhysicalObject& that)
    {
        if (that.type == "Blob")
        {
            if (!exploding)
            {
                arm();
                that.bounce();
            }
            else
            {
                Blob& blob = (Blob&)that;
                blob.kill();
            }
        }
        else if (that.type == "Pellet" || that.type == "BigPellet" || that.type == "Splitter" || (exploding && that.type == "Poison"))
        {
            that.reset();
        }
        else if (that.type == "Bomb")
        {
            Bomb& bomb = (Bomb&)that;
            if (bomb.exploding)
            {
                return;
            }
            else if (exploding)
            {
                bomb.explode();
                bomb.armed = true;
                bomb.timer = 0;
            }
            else
            {
                that.reset();
            }
        }
    }
};
