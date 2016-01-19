class AmoebaCodec : public Codec
{
private:
    const string beginStreamTag;
    const string endStreamTag;

public:

    AmoebaCodec() : beginStreamTag("begin:stream;"), endStreamTag("end:stream;") {}

    bool checkHeader(string& data, Connection& connection)
    {
        if (!contains(beginStreamTag, data) && data != "")
        {
            cout << "Incorrect format: connection " << connection.connectionId << endl;
            //connection.disconnect();
            return false;
        }
        return true;
    }

    bool checkFooter(string& data)
    {
        return contains(endStreamTag, data);
    }

    deque<KeyValuePair> decode(string& dataStream)
    {
        dataStream = filterDataStream(dataStream);
        deque<KeyValuePair> pairs;
        string buffer = "";
        string field = "";
        string value = "";
        for (int i = 0; i < dataStream.length(); i++)
        {
            switch(dataStream[i])
            {
            case '#':
                i++;
                buffer += dataStream[i];
                continue;
            case ':':
                field = buffer;
                buffer = "";
                break;
            case ';':
                value = buffer;
                buffer = "";
                pairs.push_back(KeyValuePair(field, value));
                break;
            default:
                buffer += dataStream[i];
            };
        }
        return pairs;
    }

    string encode(deque<KeyValuePair>& pairs)
    {
        string stream = "";
        string field = "";
        string value = "";
        for (int i = 0; i < pairs.size(); i++)
        {
            field = escapeCharacters(pairs[i].field);
            value = escapeCharacters(pairs[i].value);
            stream += field + ':' + value + ';';
        }
        return prepareDataStream(stream);
    }

private:

    inline string filterDataStream(string& dataStream)
    {
        dataStream = replace(beginStreamTag, "", dataStream);
        dataStream = replace(endStreamTag, "", dataStream);
        return dataStream;
    }

    inline string prepareDataStream(string& dataStream)
    {
        return beginStreamTag + dataStream + endStreamTag;
    }

    inline string escapeCharacters(string key)
    {
        string result = "";
        for (int i = 0; i < key.length(); i++)
        {
            switch(key[i])
            {
            case '#':
            case ':':
            case ';':
                result += "#";
            };
            result += key[i];
        }
        return result;
    }
};
