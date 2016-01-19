class CompressionAlgorithm : public StringWare
{
public:

    virtual string compress(string& dataStream) = 0;
    virtual string decompress(string& dataStream) = 0;

    // Part of the MiddleWare interface
    string processInput(string& dataStream, int connectionId)
    {
        return decompress(dataStream);
    }

    string processOutput(string& dataStream, int connectionId)
    {
        return compress(dataStream);
    }
};
