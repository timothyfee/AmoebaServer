class ConnectionBuffer
{
private:
    Codec& codec;
    string dataStream;

public:

    ConnectionBuffer(Codec& codec) : codec(codec), dataStream("") {}

    string getDataStream(Connection &connection)
    {
        string data = connection.getData();
        dataStream = data;

        if (!codec.checkHeader(data, connection))
        {
            return "";
        }

        for (int i = 0; !codec.checkFooter(data); i++)
        {
            data = connection.getData();
            if (i == 1000 && data == "")
            {
                return "";
            }
            else if (i == 1000)
            {
                i = 0;
            }
            dataStream += data;
        }

        return dataStream;
    }

    void sendDataStream(Connection& connection, string& dataStream)
    {
        const static int characterLimit = 10000;
        if (dataStream.length() < characterLimit)
        {
            connection.sendData(dataStream);
            return;
        }
        string segment = "";
        for (long i = 0; i < dataStream.length();)
        {
            for (long c = 0; c < characterLimit && i < dataStream.length(); c++, i++)
            {
                segment += dataStream[i];
            }
            connection.sendData(segment);
            segment = "";
        }
    }
};
