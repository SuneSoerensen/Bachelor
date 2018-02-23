#include <iostream>
#include "URControl.hpp"

using namespace std;

int main()
{
  cout << "Welcome to URControlTest.cpp" << endl;

  URControl ur5("192.168.100.4",30002);

  try
  {
    ur5.connect();
    ur5.moveToInit();
    cout << "Succesfully called moveToInit()!" << endl;
  }
  catch(const char e[])
  {
    cout << e << endl;
  }
  //ur5.connect();

  return 0;
}
