class Blob : public PhysicalObject, public Bindable
{
public:
    Player& player;
    Score& score;
    deque<Blob*>& blobs;
    string color;
    double currentSpeed;
    double currentSize;
    static const double maxSpeed = 10;
    static const double maxSize = 500;
    static const double minSize = 10;
    static const int maxBlobs = 20;
    int boostTimer;

    Blob(Player& player, Score& score, deque<Blob*>& blobs, string color = "black")
        :   PhysicalObject("Blob"), player(player), score(score), blobs(blobs), color(color)
    {
        reset();
    }

    void onDestroy()
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            if (blobs[i] == this)
            {
                blobs.erase(blobs.begin() + i);
                return;
            }
        }
    }

    void onStep()
    {
        if (boostTimer > 0)
        {
            boostTimer--;
        }
    }

    void synchronize()
    {
        setSize(currentSize, currentSize);
        if (boostTimer < 900)
        {
            currentSpeed = currentSpeed >= 4 ? maxSpeed - sqrt(maxSpeed * maxSpeed * (currentSize / maxSize)) : 4;
        }

        x = wrapToScreen(x, gameWorldProperties.width/2, -gameWorldProperties.width/2);
        y = wrapToScreen(y, gameWorldProperties.length/2, -gameWorldProperties.length/2);

        if (this != blobs[0])
        {
            tangibility = GHOSTLIKE;
            if (speed < currentSpeed * 0.9 && distanceBetweenPoints(getCenterPoint(), blobs[0]->getCenterPoint()) > (blobs[0]->currentSize + currentSize))
            {
                double dir = 0;
                degreeAngleBetweenPoints(getCenterPoint(), blobs[0]->getCenterPoint(), dir);
                move(dir);
            }
        }
        else
        {
            tangibility = TANGIBLE;
        }
    }

    void move(double direction)
    {
        setFacingDirection(direction);
        setVelocity(currentSpeed, direction);
    }

    void boost()
    {
        if (boostTimer == 0)
        {
            currentSpeed *= 2;
            boostTimer = 1000;
        }
    }

    void split()
    {
        if (currentSize < 20 || blobs.size() > maxBlobs)
        {
            return;
        }
        currentSize /= 2;
        blobs.push_back(new Blob(player, score, blobs, color));
        Blob& newBlob = *blobs[blobs.size() - 1];
        newBlob.currentSize = currentSize;
        newBlob.setCenterPoint(calculateVector(getCenterPoint(), currentSize + 1, facingDirectionH));
        newBlob.setVelocity(maxSpeed, facingDirectionH);
    }

    void reset()
    {
        boostTimer = 0;
        currentSpeed = maxSpeed;
        currentSize = minSize;
        x = random(gameWorldProperties.width/2, -gameWorldProperties.width/2);
        y = random(gameWorldProperties.length/2, -gameWorldProperties.length/2);
        setVelocity(0, 0);
    }

    void kill()
    {
        if (blobs.size() > 1)
        {
            destroy();
        }
        else
        {
            score.score = 0;
            reset();
        }
    }

    void onCollision(PhysicalObject& that)
    {
        if (that.type == "Pellet")
        {
            currentSize++;
            score.addScore(1);
            that.reset();
        }
        else if (that.type == "BigPellet")
        {
            currentSize += that.width;
            score.addScore(that.width);
            that.reset();
        }
        else if (that.type == "Blob")
        {
            Blob& blob = (Blob&)that;
            if (&blob.player == &player)
            {
                if (this != blobs[0])
                {
                    setMovingDirection(movingDirectionH + 30 * random(1, -1));
                }
                return;
            }
            else if (blob.currentSize < currentSize)
            {
                currentSize += blob.currentSize;
                score.addScore(blob.currentSize);
                blob.kill();
            }
        }

        if (currentSize > maxSize)
        {
            if (blobs.size() < maxBlobs)
            {
                split();
            }
            else
            {
                for (int i = 1; i < blobs.size(); i++)
                {
                    blobs[i]->destroy();
                }
                reset();
            }
        }
    }
};
