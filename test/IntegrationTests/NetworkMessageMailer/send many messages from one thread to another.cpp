#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Thread.hpp"
#include "..\..\..\src\NetworkMessage.hpp"
#include "..\..\..\src\NetworkMessages.hpp"
#include "..\..\..\src\NetworkMessageMailer.hpp"

class AThread : public Thread
{
private:
    NetworkMessageMailer& inMailer;
    NetworkMessageMailer& outMailer;

public:

    AThread(NetworkMessageMailer& inMailer, NetworkMessageMailer& outMailer) : inMailer(inMailer), outMailer(outMailer) {}

    void task()
    {
        deque<NetworkMessage*> outMessages;
        outMessages.push_back(new PlainTextMessage("From"));
        outMessages.push_back(new PlainTextMessage("       A"));
        outMessages.push_back(new PlainTextMessage("Thread"));
        outMailer.push(0, outMessages);

        deque<NetworkMessage*> inMessages = inMailer.dump();
        for (int i = 0; i < inMessages.size(); i++)
        {
            NetworkMessage& inMessage = *inMessages[i];
            cout << inMessage.type() << endl;
            if (inMessage.type() == "PlainTextMessage")
            {
                PlainTextMessage& m = (PlainTextMessage&)inMessage;
                cout << m.text << endl;
                delete &m;
            }
        }
    }
};

class BThread : public Thread
{
private:
    NetworkMessageMailer& inMailer;
    NetworkMessageMailer& outMailer;

public:

    BThread(NetworkMessageMailer& inMailer, NetworkMessageMailer& outMailer) : inMailer(inMailer), outMailer(outMailer) {}

    void task()
    {
        deque<NetworkMessage*> outMessages;
        outMessages.push_back(new PlainTextMessage("From"));
        outMessages.push_back(new PlainTextMessage("                          B"));
        outMessages.push_back(new PlainTextMessage("Thread"));
        outMailer.push(0, outMessages);

        deque<NetworkMessage*> inMessages = inMailer.dump();
        for (int i = 0; i < inMessages.size(); i++)
        {
            NetworkMessage& inMessage = *inMessages[i];
            cout << inMessage.type() << endl;
            if (inMessage.type() == "PlainTextMessage")
            {
                PlainTextMessage& m = (PlainTextMessage&)inMessage;
                cout << m.text << endl;
                delete &m;
            }
        }
    }
};

int main()
{
    NetworkMessageMailer mailer[2];
    AThread a(mailer[0], mailer[1]);
    BThread b(mailer[1], mailer[0]);

    a.startLooping(1);
    b.startLooping(1);

    cin.get();
    a.stop();
    b.stop();
}
