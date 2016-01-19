#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\NetworkMessage.hpp"
#include "..\..\..\src\NetworkMessages.hpp"
#include "..\..\..\src\Interpreter.hpp"

int main()
{
    deque<KeyValuePair> pairs;
    DefaultInterpreter parser;

    pairs.push_back(KeyValuePair("begin", "LoginMessage"));
    pairs.push_back(KeyValuePair("username", "Testy McTesterson"));
    pairs.push_back(KeyValuePair("password", "something very secret"));
    pairs.push_back(KeyValuePair("end", "LoginMessage"));

    pairs.push_back(KeyValuePair("begin", "PlainTextMessage"));
    pairs.push_back(KeyValuePair("text", "This is a test."));
    pairs.push_back(KeyValuePair("end", "PlainTextMessage"));

    deque<NetworkMessage*> messages = parser.parse(pairs);

    for (int i = 0; i < messages.size(); i++)
    {
        NetworkMessage& message = *messages[i];
        cout << message.type() << endl;
        if (message.type() == "LoginMessage")
        {
            LoginMessage m = (LoginMessage&)message;
            cout << "User Name: " << m.username << endl << "Password: " << m.password << endl << endl;
        }
        else if (message.type() == "PlainTextMessage")
        {
            PlainTextMessage m = (PlainTextMessage&)message;
            cout << m.text << endl;
        }
    }
}
