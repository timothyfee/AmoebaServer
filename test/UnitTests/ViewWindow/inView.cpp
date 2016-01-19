#include "..\..\..\src\NetworkFramework\StandardFunctionsLib.hpp"
#include "..\..\..\src\MathLib.hpp"

#include "..\..\..\src\PhysicalObject.hpp"
#include "..\..\..\src\Models.hpp"

int main()
{
    ViewWindow window;
    window.centerWindow(0, 0, 0);
    window.setWindowSize(80, 30, 10);

    TestObject testObject;
    testObject.setSize(1, 1, 1);
    testObject.setCoordinates(39, 14, 0);

    if (window.inView(&testObject))
    {
        cout << "In window" << endl;
    }
    else
    {
        cout << "Out of window" << endl;
    }
}
