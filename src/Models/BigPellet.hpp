class BigPellet : public PhysicalObject
{
public:

    BigPellet() : PhysicalObject("BigPellet")
    {
        setSize(20, 20);
        updatable = false;
        reset();
    }

    void reset()
    {
        x = random(gameWorldProperties.width/3, -gameWorldProperties.width/3);
        y = random(gameWorldProperties.length/3, -gameWorldProperties.length/3);
    }
};
