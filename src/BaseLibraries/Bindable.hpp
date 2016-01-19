class Bindable
{
private:
    static deque<Bindable*> bindings;

protected:

    Bindable()
    {
        bindings.push_back(this);
    }

public:
    static bool dirty;

    virtual void synchronize() = 0;

    static deque<Bindable*> getBindings()
    {
        dirty = false;
        return bindings;
    }

    virtual ~Bindable()
    {
        dirty = true;
        for (int i = 0; i < bindings.size(); i++)
        {
            if (bindings[i] == this)
            {
                bindings.erase(bindings.begin() + i);
                break;
            }
        }
    }
};
deque<Bindable*> Bindable::bindings;
bool Bindable::dirty = false;
