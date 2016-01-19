class AIController : public PhysicalObject
{
public:
    Player player;
    BlobController& blobController;

    double targetSize;
    double targetDirection;
    double targetDistance;

    int debounceTimer;

    AIController() : PhysicalObject("AIController"), player(Player(9001, 9001, "AI Player")), blobController(player.blobController)
    {
        blobController.setResistance(-gameWorldProperties.resistance);
        tangibility = GHOSTLIKE;
        changeColor();
        width = Blob::maxSize;
        length = width;
        reset();
    }

    void reset()
    {
        debounceTimer = 1000;
        targetDirection = targetSize = 0;
        targetDistance = width;
        blobController.getBlob(0).move(random(360));
    }

    void onStep()
    {
        setCenterPoint(blobController.getBlob(0).getCenterPoint());

        if (debounceTimer > 0)
        {
            debounceTimer--;
        }
        else
        {
            reset();
        }

        if (blobController.getBlob(0).currentSize > Blob::maxSize)
        {
            changeColor();
            blobController.getBlob(0).move(random(360));
        }
    }

    void onCollision(PhysicalObject& that)
    {
        if (collisions.size() == 1)
        {
            int chance = random(1000, 1);
            if (chance == 1)
            {
                int dir = random(360);
                blobController.move(dir);
            }
            return;
        }
        else if (that.type == "AIController" || that.type == "Splitter" || blobController.contains(that))
        {
            return;
        }

        if (that.type == "Pellet" || that.width < blobController.getBlob(0).currentSize && that.type != "Bomb" && that.type != "Poison")
        {
            double d = distanceBetweenPoints(getCenterPoint(), that.getCenterPoint());
            if (d < targetDistance || that.width > targetSize)
            {
                targetDistance = d;
                targetSize = that.width;
                degreeAngleBetweenPoints(getCenterPoint(), that.getCenterPoint(), targetDirection);
                blobController.move(targetDirection);
            }
        }
        else
        {
            degreeAngleBetweenPoints(that.getCenterPoint(), blobController.getBlob(0).getCenterPoint(), targetDirection);
            blobController.move(targetDirection + 30);
        }
    }

private:

    void changeColor()
    {
        switch(random(5, 0))
        {
        case 0:
            blobController.setColor("red");
            break;
        case 1:
            blobController.setColor("blue");
            break;
        case 2:
            blobController.setColor("green");
            break;
        case 3:
            blobController.setColor("purple");
            break;
        case 4:
            blobController.setColor("orange");
            break;
        case 5:
            blobController.setColor("white");
            break;
        };
    }
};
