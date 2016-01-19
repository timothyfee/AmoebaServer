struct NetworkMessage
{
    int connectionId;
    const string type;

    virtual ~NetworkMessage(){}

protected:

    NetworkMessage(string type) : type(type) {}
};
