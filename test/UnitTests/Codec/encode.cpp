#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Codec.hpp"

int main()
{
    DefaultCodec codec;
    deque<KeyValuePair> pairs;
    pairs.push_back(KeyValuePair("This", "is"));
    pairs.push_back(KeyValuePair("a", "test"));
    string stream = codec.encode(pairs);
    cout << stream << endl;
}
