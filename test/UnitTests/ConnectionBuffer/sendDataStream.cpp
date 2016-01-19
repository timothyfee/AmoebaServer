#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Network.hpp"
#include "..\..\..\src\Thread.hpp"
#include "..\..\..\src\Connection.hpp"
#include "..\..\..\src\ConnectionPool.hpp"
#include "..\..\..\src\Codec.hpp"
#include "..\..\..\src\ConnectionBuffer.hpp"

class SenderThread : public Thread
{
private:
    Connection connection;
    DefaultCodec codec;
    ConnectionBuffer sender;

public:

    SenderThread() : sender(ConnectionBuffer(codec)) {}

    void setUp()
    {
        connection.accept(8080, "127.0.0.1");
    }

    void task()
    {
        string stream = "begin:PlainTextMessage;text:This is a test.;end:PlainTextMessage;";
        sender.sendDataStream(connection, stream);
    }

    void cleanUp()
    {
        connection.disconnect();
    }
};

int main()
{
    string stream = "";
    Connection connection;
    SenderThread senderThread;
    senderThread.start();
    cout << "Connecting" << endl;
    if (connection.connect("127.0.0.1", 8080, 3))
    {
        cout << "Connected" << endl;
        stream = connection.getData();
        if (stream != "")
        {
            cout << stream << endl;
        }
        else
        {
            cout << "No data sent." << endl;
        }
        exit(0);
    }
    else
    {
        cout << "Failed to connect." << endl;
    }
}
