class DefaultPhysicsEngine : public PhysicsEngine
{
private:
    deque<PhysicalObject*> objects;
    double resistance;

public:

    DefaultPhysicsEngine(double resistance = 1) : resistance(resistance) {}

    void update(deque<PhysicalObject*> objects)
    {
        this->objects = objects;
        for (int i = 0; i < objects.size(); i++)
        {
            PhysicalObject& object = *objects[i];
            if (object.isDestroyed())
            {
                continue;
            }
            if (object.updatable)
            {
                updateObject(object);
            }
            object.onStep();
        }
        deleteDestroyedObjects();
    }

private:

    deque<PhysicalObject*> checkCollisions(PhysicalObject& object, Point &target)
    {
        deque<PhysicalObject*> collisions;
        if (object.tangibility == INTANGIBLE)
        {
            return collisions;
        }
        for (int i = 0; i < objects.size(); i++)
        {
            PhysicalObject& other = *objects[i];
            if (&object != &other && other.tangibility != INTANGIBLE)
            {
                if (distanceBetweenPoints(target, other.getCenterPoint()) <= (object.width / 2) + (other.width / 2))
                {
                    if (object.tangibility == TANGIBLE && other.tangibility == TANGIBLE)
                    {
                        target = object.getCenterPoint();
                    }
                    collisions.push_back(&other);
                }
            }
        }
        return collisions;
    }

    void updateObject(PhysicalObject& object)
    {
        applyResistance(object);
        object.momentum = object.speed * object.mass;
        Point target = calculateTargetPoint(object);
        deque<PhysicalObject*> collidingObjects = checkCollisions(object, target);
        move(object, target);
        object.collisions = collidingObjects;
        object.isColliding = false;
        for (int i = 0; i < collidingObjects.size(); i++)
        {
            handleCollision(object, *collidingObjects[i]);
        }
    }

    void deleteDestroyedObjects()
    {
        for(int i = 0; i < objects.size(); i++)
        {
            if (objects[i]->isDestroyed())
            {
                delete objects[i];
            }
        }
    }

    void handleCollision(PhysicalObject& object, PhysicalObject& other)
    {
        object.isColliding = true;
        object.onCollision(other);
        if (!other.isDestroyed())
        {
            other.isColliding = true;
            other.onCollision(object);
        }
    }

    void move(PhysicalObject& object, Point target)
    {
        object.setCenterPoint(target);
    }

    void applyResistance(PhysicalObject &object)
    {
        object.speed -= resistance + object.resistance;
        if (object.speed < 0)
        {
            object.speed = 0;
        }
    }

    Point calculateTargetPoint(PhysicalObject &object)
    {
        return calculateVector(object.getCenterPoint(), object.speed, object.movingDirectionH, object.movingDirectionV);
    }
};
