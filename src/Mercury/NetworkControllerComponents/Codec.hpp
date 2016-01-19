class Codec
{
public:

    virtual bool checkHeader(string& data, Connection& connection) = 0;
    virtual bool checkFooter(string& data) = 0;

    virtual deque<KeyValuePair> decode(string& dataStream) = 0;
    virtual string encode(deque<KeyValuePair>& pairs) = 0;
};
