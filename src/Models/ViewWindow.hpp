class ViewWindow : public Bindable
{
private:
    double left, right, top, bottom;

    void updatSides()
    {
        double ww = width/2;
        double wl = length/2;

        left = x - ww;
        right = x + ww;
        top = y + wl;
        bottom = y - wl;
    }

public:
    double x, y;
    int width, length;
    PhysicalObject* targetObject;
    BlobController* blobController;

    ViewWindow()
    {
        setSize(800, 600);
        width = 800;
        length = 600;
        targetObject = NULL;
        blobController = NULL;
    }

    void trackObject(PhysicalObject& object)
    {
        this->targetObject = &object;
    }

    void trackBlobController(BlobController& blobController)
    {
        this->blobController = &blobController;
    }

    void synchronize()
    {
        if (blobController != NULL)
        {
            PhysicalObject& blob = blobController->getBlob(0);
            centerWindow(blob.x, blob.y);
        }
        else if (targetObject != NULL)
        {
            centerWindow(targetObject->x, targetObject->y);
        }
    }

    void centerWindow(double x, double y)
    {
        this->x = x;
        this->y = y;
        updatSides();
    }

    void setSize(int width, int length)
    {
        this->width = width;
        this->length = length;
        updatSides();
    }

    inline bool inView(PhysicalObject* object)
    {
        double ox = object->x;
        double oy = object->y;
        double os = object->width;

        if (ox - os < right && ox + os > left &&
            oy - os < top && oy + os > bottom)
        {
            return true;
        }
        return false;
    }

    Point serverToClientView(Point p)
    {
        return Point((width / 2) + (p.x - x), (length / 2) + (y - p.y));
    }
};
