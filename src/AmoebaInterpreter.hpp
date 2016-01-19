class AmoebaInterpreter : public Interpreter
{
public:

    deque<NetworkMessage*> parse(deque<KeyValuePair>& pairs)
    {
        int beginSection = 0;
        int endSection = 0;
        deque<NetworkMessage*> messages;
        for (int i = 0; i < pairs.size(); i++)
        {
            KeyValuePair pair = pairs[i];
            if (pair.field == "begin")
            {
                beginSection = i;
            }
            else if (pair.field == "end")
            {
                endSection = i;
                parseSection(beginSection, endSection, pairs, messages);
            }
        }
        return messages;
    }

    deque<KeyValuePair> compile(deque<NetworkMessage*>& messages)
    {
        deque<KeyValuePair> pairs;
        for (int i = 0; i < messages.size(); i++)
        {
            compileMessage(*messages[i], pairs);
        }
        return pairs;
    }

private:

    inline void parseSection(int begin, int end, const deque<KeyValuePair>& pairs, deque<NetworkMessage*>& messages)
    {
        string messageType = pairs[begin].value;
        switchString(messageType)
        {
            primaryCaseString("MoveTowardCoordinatesMessage")
            double x = toDouble(getValue("x", begin, end, pairs));
            double y = toDouble(getValue("y", begin, end, pairs));
            messages.push_back(new MoveTowardCoordinatesMessage(x, y));
            endCase

            caseString("PingMessage")
            messages.push_back(new PingMessage());
            endCase

            caseString("BoostMessage")
            messages.push_back(new BoostMessage());
            endCase

            caseString("SplitMessage")
            messages.push_back(new SplitMessage());
            endCase

            caseString("ChatMessage")
            string username = getValue("username", begin, end, pairs);
            string text = getValue("text", begin, end, pairs);
            messages.push_back(new ChatMessage(username, text));
            endCase

            caseString("LoginMessage")
            string username = getValue("username", begin, end, pairs);
            string password = getValue("password", begin, end, pairs);
            messages.push_back(new LoginMessage(username, password));
            endCase

            caseString("SetBlobPropertiesMessage")
            deque<KeyValuePair> properties;
            properties.push_back(KeyValuePair("color", getValue("color", begin, end, pairs)));
            messages.push_back(new SetBlobPropertiesMessage(properties));
            endCase

            caseString("LogoutMessage")
            messages.push_back(new LogoutMessage());
            endCase

            caseString("PlainTextMessage")
            string text = getValue("text", begin, end, pairs);
            messages.push_back(new PlainTextMessage(text));
            endCase

            caseString("MoveInDirectionMessage")
            double direction = toDouble(getValue("direction", begin, end, pairs));
            messages.push_back(new MoveInDirectionMessage(direction));
            endCase

            caseString("PelletPositionMessage")
            double x = toDouble(getValue("x", begin, end, pairs));
            double y = toDouble(getValue("y", begin, end, pairs));
            long id = toLong(getValue("id", begin, end, pairs));
            messages.push_back(new PelletPositionMessage(x, y, id));
            endCase

            caseString("BlobStateMessage")
            deque<KeyValuePair> properties;
            properties.push_back(KeyValuePair("x", getValue("x", begin, end, pairs)));
            properties.push_back(KeyValuePair("y", getValue("y", begin, end, pairs)));
            properties.push_back(KeyValuePair("size", getValue("size", begin, end, pairs)));
            properties.push_back(KeyValuePair("color", getValue("color", begin, end, pairs)));
            properties.push_back(KeyValuePair("username", getValue("username", begin, end, pairs)));
            properties.push_back(KeyValuePair("id", getValue("id", begin, end, pairs)));
            messages.push_back(new BlobStateMessage(properties));
            endCase

            caseString("HighScoreMessage")
            string text = getValue("text", begin, end, pairs);
            messages.push_back(new HighScoreMessage(text));
            endCase

            caseString("CurrentScoreMessage")
            string text = getValue("text", begin, end, pairs);
            messages.push_back(new CurrentScoreMessage(text));
            endCase
        };
        endStringSwitch
    }

    inline void compileMessage(NetworkMessage& message, deque<KeyValuePair>& pairs)
    {
        string messageType = message.type;
        switchString(messageType)
        {
            primaryCaseString("PelletPositionMessage")
            PelletPositionMessage& m = (PelletPositionMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("x", toString(m.x)));
            pairs.push_back(KeyValuePair("y", toString(m.y)));
            pairs.push_back(KeyValuePair("id", toString(m.id)));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("BlobStateMessage")
            BlobStateMessage& m = (BlobStateMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            for (int i = 0; i < m.properties.size(); i++)
            {
                pairs.push_back(KeyValuePair(m.properties[i].field, m.properties[i].value));
            }
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("HighScoreMessage")
            HighScoreMessage& m = (HighScoreMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("text", m.text));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("CurrentScoreMessage")
            CurrentScoreMessage& m = (CurrentScoreMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("text", m.text));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("PingMessage")
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("ChatMessage")
            ChatMessage& m = (ChatMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("username", m.username));
            pairs.push_back(KeyValuePair("text", m.text));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("PlainTextMessage")
            PlainTextMessage& m = (PlainTextMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("text", m.text));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("LoginMessage")
            LoginMessage& m = (LoginMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("username", m.username));
            pairs.push_back(KeyValuePair("password", m.password));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("LogoutMessage")
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("MoveInDirectionMessage")
            MoveInDirectionMessage& m = (MoveInDirectionMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("direction", toString(m.direction)));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("MoveTowardCoordinateMessage")
            MoveTowardCoordinatesMessage& m = (MoveTowardCoordinatesMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("x", toString(m.x)));
            pairs.push_back(KeyValuePair("y", toString(m.y)));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("BoostMessage")
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase

            caseString("SplitMessage")
            SplitMessage& m = (SplitMessage&)message;
            pairs.push_back(KeyValuePair("begin", messageType));
            pairs.push_back(KeyValuePair("end", messageType));
            endCase
        };
        endStringSwitch
        delete &message;
    }

    string getValue(string field, int begin, int end, const deque<KeyValuePair>& pairs)
    {
        for (int i = begin; i < end; i++)
        {
            if (pairs[i].field == field)
            {
                return pairs[i].value;
            }
        }
        return "";
    }
};
