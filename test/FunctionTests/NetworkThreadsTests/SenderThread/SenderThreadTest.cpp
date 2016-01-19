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
#include "..\..\..\..\src\SenderThread.hpp"

int size = 10;

class ReceiverThread : public Thread
{
public:
    ConnectionPool connectionPool;

    ReceiverThread() : connectionPool(ConnectionPool(size)) {connectionPool.add(size);}

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
            string text = connection.getData();
            if (text != "")
            {
                cout << i << endl << text << endl << endl;
            }
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

    SenderThread sender(connectionPool, mailer, interpreter, codec, compressor, cipher);
    ReceiverThread receiver;

    receiver.startLooping(1);

    for (int i = 0; i < connectionPool.size(); i++)
    {
        if (!connectionPool.getConnection(i).connect("127.0.0.1", 8080, 3))
        {
            cout << "Failed to connect" << endl;
            exit(0);
        }
    }

    sender.startLooping(10);

    for (int c = 0; c < 10; c++)
    {
        for (int i = 0; i < size; i++)
        {
            mailer.push(i, new PlainTextMessage("Hello world!"));
        }
        Sleep(100);
    }
    cout << "Done" << endl;
}
