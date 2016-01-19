#include "..\..\..\..\src\Network.hpp"
#include "..\..\..\..\src\Thread.hpp"
#include "..\..\..\..\src\Connection.hpp"
#include "..\..\..\..\src\ConnectionPool.hpp"
#include "..\..\..\..\src\NetworkMessage.hpp"
#include "..\..\..\..\src\NetworkMessages.hpp"
#include "..\..\..\..\src\NetworkMessageMailer.hpp"
#include "..\..\..\..\src\ConnectorThread.hpp"

class ClientThread : public Thread
{
public:
    ClientSocket clientSocket;
    int times;

    ClientThread(int times) : times(times) {}

    void task()
    {
        times--;
        if (clientSocket.connect("127.0.0.1", 8080, 3))
        {
            cout << "Success" << endl;
            clientSocket.disconnect();
        }
        else
        {
            cout << "Fail" << endl;
        }
        if (times == 0)
        {
            cout << "Done" << endl;
            exit(0);
        }
    }
};

int main()
{
    ConnectionPool connectionPool(10);
    NetworkMessageMailer mailer;
    ConnectorThread connector(connectionPool, mailer, 8080);
    ClientThread client(connectionPool.maxConnections);

    connector.startLooping(1);
    client.startLooping(1);

    while(true)
    {
        deque<NetworkMessage*> messages = mailer.dump();
        for (int i = 0; i < messages.size(); i++)
        {
            NewConnectionMessage& message = (NewConnectionMessage&)*messages[i];
            cout << "New Connection " << message.connectionId << " " << message.ipAddress << " on port " << message.port << endl;
            delete &message;
        }
        nanosleep(1);
    }
}
