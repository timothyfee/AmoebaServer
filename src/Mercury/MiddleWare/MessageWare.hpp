class MessageWare
{
public:

    virtual deque<NetworkMessage*> processInput(deque<NetworkMessage*>& messages, int connectionId) = 0;
    virtual deque<NetworkMessage*> processOutput(deque<NetworkMessage*>& messages, int connectionId) = 0;
};
