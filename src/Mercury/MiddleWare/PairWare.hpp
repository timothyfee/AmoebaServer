class PairWare
{
public:

    virtual deque<KeyValuePair> processInput(deque<KeyValuePair>& pairs, int connectionId) = 0;
    virtual deque<KeyValuePair> processOutput(deque<KeyValuePair>& pairs, int connectionId) = 0;
};
