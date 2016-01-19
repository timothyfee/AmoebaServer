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
        PlainTextMessage* outMessage = new PlainTextMessage("From A Thread");
        outMailer.push(0, outMessage);

        while(inMailer.messageCount() > 0)
        {
            NetworkMessage& inMessage = *inMailer.pop();
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
        PlainTextMessage* outMessage = new PlainTextMessage("From B Thread");
        outMailer.push(0, outMessage);

        while(inMailer.messageCount() > 0)
        {
            NetworkMessage& inMessage = *inMailer.pop();
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

    a.startLooping(10);
    b.startLooping(10);

    cin.get();
    a.stop();
    b.stop();
}
