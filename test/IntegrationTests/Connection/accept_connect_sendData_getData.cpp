#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Thread.hpp"
#include "..\..\..\src\Network.hpp"
#include "..\..\..\src\Connection.hpp"
#include "..\..\..\src\ConnectionPool.hpp"

class ReceiverThread : public Thread
{
public:
    Connection connection;

    void setUp()
    {
        connection.accept(8080, "127.0.0.1");
        cout << "Connected" << endl;
    }

    void task()
    {
        string text = connection.getData();
        if (text != "")
        {
            cout << text << endl;
        }
    }
};

int main()
{
    ReceiverThread receiver;
    receiver.startLooping(1);

    Connection connection;
    cout << "Connecting" << endl;
    if (!connection.connect("127.0.0.1", 8080, 3))
    {
        cout << "Failed to connect" << endl;
        exit(0);
    }
    while (true)
    {
        connection.sendData("This is a test.");
    }
}
