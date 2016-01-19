class BlobController
{
public:
    Player& player;
    Score& score;
    deque<Blob*> blobs;

    BlobController(Player* player, Score& score) : player(*player), score(score)
    {
        blobs.push_back(new Blob(*player, score, blobs));
    }

    ~BlobController()
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            delete blobs[i];
        }
    }

    Blob& getBlob(int index)
    {
        return *blobs[index];
    }

    void move (double direction)
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            if (blobs[i]->currentSize > blobs[0]->currentSize * 0.8)
            {
                blobs[i]->move(direction);
            }
        }
    }

    void boost()
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            blobs[i]->boost();
        }
    }

    void setColor (string color)
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            blobs[i]->color = color;
        }
    }

    void setResistance (double resistance)
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            blobs[i]->resistance = resistance;
        }
    }

    bool contains (PhysicalObject& blob)
    {
        for (int i = 0; i < blobs.size(); i++)
        {
            if (blobs[i] == &blob)
            {
                return true;
            }
        }
        return false;
    }

    void split()
    {
        int size = blobs.size();
        for (int i = 0; i < size; i++)
        {
            blobs[i]->split();
        }
    }
};
