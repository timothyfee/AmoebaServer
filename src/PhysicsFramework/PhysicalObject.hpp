enum Tangible {TANGIBLE, GHOSTLIKE, INTANGIBLE};

class PhysicalObject
{
private:
    static deque<PhysicalObject*> objects;
    static long nextId;
    bool destroyed;

protected:

    PhysicalObject(string type) : type(type), objectId(nextId)
    {
        nextId++;
        destroyed = false;
        isColliding = false;
        tangibility = tangibility;
        updatable = true;
        facingDirectionH = 0;
        movingDirectionH = 0;
        facingDirectionV = 90;
        movingDirectionV = 90;
        facingDirectionS = 0;
        movingDirectionS = 0;
        x = 0;
        y = 0;
        z = 0;
        height = 0;
        width = 0;
        length = 0;
        resistance = 0;
        momentum = 0;
        speed = 0;
        objects.push_back(this);
    }

public:
    const long objectId;
    const string type;
    Tangible tangibility;
    bool updatable;
    bool isColliding;
    deque<PhysicalObject*> collisions;

    double x, y, z;
    double width, height, length;
    double facingDirectionH, facingDirectionV, facingDirectionS;
    double movingDirectionH, movingDirectionV, movingDirectionS;
    double temperature;
    double mass;
    double speed;

    double resistance;
    double momentum;

    bool isDestroyed()
    {
        return destroyed;
    }

    void destroy()
    {
        destroyed = true;
        onDestroy();
    }

    virtual void onDestroy(){}

    virtual ~PhysicalObject()
    {
        for (long i = 0; i < objects.size(); i++)
        {
            if (objects[i] == this)
            {
                objects.erase(objects.begin() + i);
                break;
            }
        }
    }

    const static deque<PhysicalObject*> getObjects()
    {
        return objects;
    }

    Point getCenterPoint()
    {
        return Point(x, y, z);
    }

    void setCenterPoint(Point p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }

    virtual void onStep() {}

    virtual void reset() {}

    long snapToGrid(long xy, int gridUnit)
    {
        long t = xy % gridUnit;
        xy = (xy / gridUnit) * gridUnit;
        if (t >= gridUnit / 2)
        {
            xy += gridUnit;
        }
        return xy;
    }

    double wrapToScreen(double xy, int screenMax, int screenMin = 0)
    {
        if (xy > screenMax)
        {
            return screenMin;
        }
        else if (xy < screenMin)
        {
            return screenMax;
        }
        return xy;
    }

    void bounce(double v = 90)
    {
        double n = movingDirectionH;
        setMovingDirection(2 * n - v - 180);
        if (n == movingDirectionH)
        {
            movingDirectionH = v;
        }
    }

    virtual void onCollision(PhysicalObject& that)
    {
        bounce(that.facingDirectionH);
    }

    void setVelocity(double speed, double hDegrees)
    {
        setMovingDirection(hDegrees);
        this->speed = speed;
    }

    void setVelocity(double speed, double hDegrees, double vDegrees, double sDegrees)
    {
        setMovingDirection(hDegrees, vDegrees, sDegrees);
        this->speed = speed;
    }

    void setCoordinates(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    void setCoordinates(double x, double y, double z)
    {
        setCoordinates(x, y);
        this->z = z;
    }

    void setFacingDirection(double hDegrees)
    {
        hDegrees = shiftIntoDegreePeriod(hDegrees);
        this->facingDirectionH = hDegrees;
    }

    void setMovingDirection(double hDegrees)
    {
        hDegrees = shiftIntoDegreePeriod(hDegrees);
        this->movingDirectionH = hDegrees;
    }

    void setFacingDirection(double hDegrees, double vDegrees, double sDegrees)
    {
        hDegrees = shiftIntoDegreePeriod(hDegrees);
        vDegrees = shiftIntoDegreePeriod(vDegrees);
        sDegrees = shiftIntoDegreePeriod(sDegrees);
        this->facingDirectionH = hDegrees;
        this->facingDirectionV = vDegrees;
        this->facingDirectionS = sDegrees;
    }

    void setMovingDirection(double hDegrees, double vDegrees, double sDegrees)
    {
        hDegrees = shiftIntoDegreePeriod(hDegrees);
        vDegrees = shiftIntoDegreePeriod(vDegrees);
        sDegrees = shiftIntoDegreePeriod(sDegrees);
        this->movingDirectionH = hDegrees;
        this->movingDirectionV = vDegrees;
        this->movingDirectionS = sDegrees;
    }

    void setSize(double width, double length)
    {
        this->width = width;
        this->length = length;
    }

    void setSize(double width, double length, double height)
    {
        setSize(width, length);
        this->height = height;
    }

    int area2d()
    {
        return width * length;
    }

    int area3d()
    {
        return width * length * height;
    }
};
deque<PhysicalObject*> PhysicalObject::objects;
long PhysicalObject::nextId = 0;
