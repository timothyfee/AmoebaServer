class KeyValuePairLogger : public PairWare
{
private:
    const string logfile;
    const bool toCout;
    const bool logInput;
    const bool logOutput;

public:

    KeyValuePairLogger(string logfile = "", bool toCout = false, bool logInput = true, bool logOutput = true)
    :   logfile(logfile), toCout(toCout), logInput(logInput), logOutput(logOutput) {}

    deque<KeyValuePair> processInput(deque<KeyValuePair>& pairs, int connectionId)
    {
        if (logInput && pairs.size() != 0)
        {
            string banner = "Received " + dateTimeStamp() + " from connection " + toString(connectionId);
            if (logfile != "")
            {
                ofstream file(toCString(logfile), ios::app);
                file << banner << endl;
                for (int i = 0; i < pairs.size(); i++)
                {
                    file << pairs[i].field << ' ' << pairs[i].value << endl;
                }
                file << endl;
                file.close();
            }
            if (toCout)
            {
                cout << banner << endl;
                for (int i = 0; i < pairs.size(); i++)
                {
                    cout << pairs[i].field << ' ' << pairs[i].value << endl;
                }
                cout << endl;
            }
        }
        return pairs;
    }

    deque<KeyValuePair> processOutput(deque<KeyValuePair>& pairs, int connectionId)
    {
        if (logOutput && pairs.size() != 0)
        {
            string banner = "Sent " + dateTimeStamp() + " to connection " + toString(connectionId);
            if (logfile != "")
            {
                ofstream file(toCString(logfile), ios::app);
                file << banner << endl;
                for (int i = 0; i < pairs.size(); i++)
                {
                    file << pairs[i].field << ' ' << pairs[i].value << endl;
                }
                file << endl;
                file.close();
            }
            if (toCout)
            {
                cout << banner << endl;
                for (int i = 0; i < pairs.size(); i++)
                {
                    cout << pairs[i].field << ' ' << pairs[i].value << endl;
                }
                cout << endl;
            }
        }
        return pairs;
    }
};
