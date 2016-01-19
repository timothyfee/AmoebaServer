#include <cstdlib>
#include <iostream>
#include "..\..\..\src\Thread.hpp"

using namespace std;

class AThread : public Thread
 {
     public:
         
     void task()
      {
          cout << "AAAA" << endl;
      }
 };

class BThread : public Thread
 {
     public:
         
     void task()
      {
          cout << "BB" << endl;
      }
 };

int main()
 {
     AThread a;
     BThread b;
     
     a.startLooping(100);
     b.startLooping(110);
     while (true)
      {
          cout << "C" << endl;
          Sleep(120);
      }
 }
