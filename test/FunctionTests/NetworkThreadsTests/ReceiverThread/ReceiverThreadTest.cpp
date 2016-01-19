#include "..\..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\..\src\Network.hpp"
#include "..\..\..\..\src\Thread.hpp"
#include "..\..\..\..\src\Connection.hpp"
#include "..\..\..\..\src\ConnectionPool.hpp"
#include "..\..\..\..\src\Cipher.hpp"
#include "..\..\..\..\src\CompressionAlgorithm.hpp"
#include "..\..\..\..\src\Codec.hpp"
#include "..\..\..\..\src\NetworkMessage.hpp"
#include "..\..\..\..\src\NetworkMessages.hpp"
#include "..\..\..\..\src\Interpreter.hpp"
#include "..\..\..\..\src\ConnectionBuffer.hpp"
#include "..\..\..\..\src\NetworkMessageMailer.hpp"
#include "..\..\..\..\src\ReceiverThread.hpp"

int size = 10;

class SenderThread : public Thread
{
public:
    ConnectionPool connectionPool;

    SenderThread() : connectionPool(ConnectionPool(size)) {connectionPool.add(size);}

    void setUp()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            connection.accept(8080, "127.0.0.1");
            cout << "Connected " << i << endl;
        }
    }

    void task()
    {
        for (int i = 0; i < connectionPool.size(); i++)
        {
            Connection& connection = connectionPool.getConnection(i);
            connection.sendData("begin:stream;begin:PlainTextMessage;text:Success!!!;end:PlainTextMessage;end:stream;");
        }
    }
};

int main()
{
    ConnectionPool connectionPool(size);
    connectionPool.add(size);
    NoCompression compressor;
    NoEncryption cipher;
    DefaultCodec codec;
    DefaultInterpreter interpreter;
    NetworkMessageMailer mailer;

    ReceiverThread receiver(connectionPool, mailer, interpreter, codec, compressor, cipher);
    SenderThread sender;

    sender.startLooping(1);

    for (int i = 0; i < connectionPool.size(); i++)
    {
        if (!connectionPool.getConnection(i).connect("127.0.0.1", 8080, 3))
        {
            cout << "Failed to connect" << endl;
            exit(0);
        }
    }

    receiver.startLooping(1);
    sleep(3000);
    sender.stop();
    receiver.stop();

    deque<NetworkMessage*> messages = mailer.dump();
    for (int i = 0; i < messages.size(); i++)
    {
        PlainTextMessage& message = (PlainTextMessage&)*messages[i];
        cout << message.type() << endl;
        cout << message.connectionId << " " << message.text << endl;
        delete &message;
    }
    cout << "Done" << endl;
}
