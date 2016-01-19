class Pellet : public PhysicalObject
{
public:

    Pellet() : PhysicalObject("Pellet")
    {
        setSize(10, 10);
        updatable = false;
        reset();
    }

    void reset()
    {
        x = random(gameWorldProperties.width/3, -gameWorldProperties.width/3);
        y = random(gameWorldProperties.length/3, -gameWorldProperties.length/3);
    }
};
