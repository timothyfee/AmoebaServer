#include "..\..\..\src\StandardFunctionsLib.hpp"
#include "..\..\..\src\Codec.hpp"

int main()
 {
     string stream = "This:is;a:test;";
     DefaultCodec codec;
     deque<KeyValuePair> pairs = codec.decode(stream);
     for (int i = 0; i < pairs.size(); i++)
      {
         KeyValuePair pair = pairs[i];
         cout << "Field: " << pair.field << endl << "Value: " << pair.value << endl << endl;
      }
 }
