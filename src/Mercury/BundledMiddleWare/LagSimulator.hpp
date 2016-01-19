class LagSimulator : public StringWare
{
private:
    int lag;

public:

    LagSimulator(int lag) : lag(lag) {}

    string processInput(string& dataStream, int connectionId)
    {
        sleep(lag);
        return dataStream;
    }

    string processOutput(string& dataStream, int connectionId)
    {
        sleep(lag);
        return dataStream;
    }
};
