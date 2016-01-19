class Interpreter
{
public:

    virtual deque<NetworkMessage*> parse(deque<KeyValuePair>& pairs) = 0;
    virtual deque<KeyValuePair> compile(deque<NetworkMessage*>& messages) = 0;
};
