#include "..\..\..\src\network.hpp"

using namespace std;

int main()
 {
     string text = "This is a Client Test.";
     string address = "127.0.0.1";
     
//     cout << "Address?" << endl;
//     cin >> address;
     
     ClientSocket clientSocket;
     
     for (int i = 0; !clientSocket.isConnected() && i < 3; i++)
      {
          clientSocket.connect(address, 8080);
      }
     
     while(true)
      {
          cout << clientSocket.getString() << endl;
          clientSocket.sendData(text);
          Sleep(10);
      }
 }
