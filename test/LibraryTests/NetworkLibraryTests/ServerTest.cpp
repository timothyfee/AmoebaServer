#include "..\..\..\src\network.hpp"

using namespace std;

int main()
 {
     string text = "This is a Server Test.";
     ServerSocket serverSocket;
     serverSocket.accept(8080);
     
     while(true)
      {
          cout << serverSocket.getString() << endl;
          serverSocket.sendData(text);
          Sleep(10);
      }
 }
