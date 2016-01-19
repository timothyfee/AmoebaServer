class StringWareContainer
{
private:
    deque<StringWare*> middleWares;

public:

    void add(StringWare& middleWare)
    {
        middleWares.push_back(&middleWare);
    }

    void add(StringWare* middleWare)
    {
        middleWares.push_back(middleWare);
    }

    string processForInput(string& dataStream, int connectionId)
    {
        for (int i = 0; i < middleWares.size(); i++)
        {
            dataStream = middleWares[i]->processInput(dataStream, connectionId);
        }
        return dataStream;
    }

    string processForOutput(string& dataStream, int connectionId)
    {
        for (int i = middleWares.size() - 1; i >= 0; i--)
        {
            dataStream = middleWares[i]->processOutput(dataStream, connectionId);
        }
        return dataStream;
    }
};
