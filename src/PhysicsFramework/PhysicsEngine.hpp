class PhysicsEngine
{
protected:
    PhysicsEngine() {}

public:

    virtual void update(deque<PhysicalObject*> objects) = 0;
};
