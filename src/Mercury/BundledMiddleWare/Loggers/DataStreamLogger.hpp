class DataStreamLogger : public StringWare
{
private:
    const string logfile;
    const bool toConsole;
    const bool logInput;
    const bool logOutput;

public:

    DataStreamLogger(string logfile = "", bool toConsole = false, bool logInput = true, bool logOutput = true)
    :   logfile(logfile), toConsole(toConsole), logInput(logInput), logOutput(logOutput) {}

    string processInput(string& dataStream, int connectionId)
    {
        if (logInput && dataStream != "")
        {
            string banner = "Received " + dateTimeStamp() + " from connection " + toString(connectionId);
            if (logfile != "")
            {
                ofstream file(toCString(logfile), ios::app);
                file << banner << endl;
                file << dataStream << endl << endl;
                file.close();
            }
            if (toConsole)
            {
                cout << banner << endl;
                cout << dataStream << endl << endl;
            }
        }
        return dataStream;
    }

    string processOutput(string& dataStream, int connectionId)
    {
        if (logOutput && dataStream != "")
        {
            string banner = "Sent " + dateTimeStamp() + " to connection " + toString(connectionId);
            if (logfile != "")
            {
                ofstream file(toCString(logfile), ios::app);
                file << "Sent " << dateTimeStamp() << endl;
                file << dataStream << endl << endl;
                file.close();
            }
            if (toConsole)
            {
                cout << banner << endl;
                cout << dataStream << endl << endl;
            }
        }
        return dataStream;
    }
};
