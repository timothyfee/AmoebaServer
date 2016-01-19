#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Network.hpp"
#include "..\..\..\src\Thread.hpp"
#include "..\..\..\src\Connection.hpp"
#include "..\..\..\src\ConnectionPool.hpp"
#include "..\..\..\src\Codec.hpp"
#include "..\..\..\src\ConnectionBuffer.hpp"

class ListenerThread : public Thread
 {
 private:
    Connection connection;
    DefaultCodec codec;
    ConnectionBuffer listener;

 public:

    ListenerThread() : listener(ConnectionBuffer(codec)) {}

    void setUp()
     {
         connection.accept(8080, "127.0.0.1");
         nanosleep(1);
     }

    void task()
     {
         string stream = listener.getDataStream(connection);
         if (stream != "")
          {
             cout << stream << endl;
          }
         else
          {
             cout << "No Data Received!!!" << endl;
          }
     }

    void cleanUp()
     {
         connection.disconnect();
     }
 };

int main()
 {
     Connection connection;
     ListenerThread listenerThread;
     listenerThread.start();
     cout << "Connecting" << endl;
     if (connection.connect("127.0.0.1", 8080, 3))
      {
         cout << "Connected" << endl;
         connection.sendData("begin:stream;begin:PlainTextMessage;text:This is a test.;end:PlainTextMessage;end:stream;");
      }
     else
      {
         cout << "Failed to connect." << endl;
      }
     sleep(10);
 }
