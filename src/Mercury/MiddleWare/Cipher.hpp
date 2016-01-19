class Cipher : public StringWare
{
public:

    virtual string encrypt(string& dataStream) = 0;
    virtual string decrypt(string& dataStream) = 0;

    // Part of the MiddleWare interface
    string processInput(string& dataStream, int connectionId)
    {
        return decrypt(dataStream);
    }

    string processOutput(string& dataStream, int connectionId)
    {
        return encrypt(dataStream);
    }
};
