class StringWare
{
public:

    virtual string processInput(string& dataStream, int connectionId) = 0;
    virtual string processOutput(string& dataStream, int connectionId) = 0;
};
