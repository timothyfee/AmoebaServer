#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\NetworkMessage.hpp"
#include "..\..\..\src\NetworkMessages.hpp"
#include "..\..\..\src\Interpreter.hpp"

int main()
{
    DefaultInterpreter composer;
    deque<NetworkMessage*> messages;
    messages.push_back(new LoginMessage("Testy McTesterson", "secret"));
    messages.push_back(new PlainTextMessage("This is a test."));
    deque<KeyValuePair> pairs = composer.compile(messages);
    for (int i = 0; i < pairs.size(); i++)
    {
        KeyValuePair pair = pairs[i];
        cout << "Field: " << pair.field << endl << "Value: " << pair.value << endl << endl;
    }
}
