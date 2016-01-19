#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Thread.hpp"
#include "..\..\..\src\Network.hpp"
#include "..\..\..\src\Connection.hpp"
#include "..\..\..\src\ConnectionPool.hpp"

int size = 10;

class ReceiverThread : public Thread
{
public:
    ConnectionPool connectionPool;

    ReceiverThread()
    {
        connectionPool.maxConnections = size;
        connectionPool.add(connectionPool.maxConnections);
    }

    void setUp()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            connection.accept(8080, "127.0.0.1");
            cout << "Connected" << i << endl;
        }
    }

    void task()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            string text = "";
            text = connection.getData();
            if (text != "")
            {
                cout << text << endl;
            }
        }
    }
};

int main()
{
    ReceiverThread receiver;
    receiver.startLooping(1);

    ConnectionPool connectionPool;
    connectionPool.maxConnections = size;
    connectionPool.add(connectionPool.maxConnections);

    cout << "Connecting" << endl;
    for (int i = 0; i < connectionPool.size(); i++)
    {
        Connection& connection = connectionPool.getConnection(i);
        if (!connection.connect("127.0.0.1", 8080, 1))
        {
            cout << "Failed to connect " << i << endl;
            cin.get();
            exit(0);
            return 0;
        }
    }

    sleep(3000);

    while (true)
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            connection.sendData("This is a test. " + toString(i));
        }
    }
}
