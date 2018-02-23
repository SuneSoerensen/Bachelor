#include <iostream>
#include "URControl.hpp"
#include <string>
#include <unistd.h>


using namespace std;

int main()
{
  cout << "Welcome to URControlTest.cpp" << endl;

  string ip = "192.168.100.4";
  int port = 30002;

  URControl ur5(ip, port);
  ur5.printcurrToolPos();
  ur5.connect();
  ur5.moveToInit();
  ur5.moveRel(0.3,0,0);
  ur5.moveRel(-0.3,0,0);
  ur5.moveRel(0,0.3,0);
  ur5.moveRel(0,-0.3,0);
  ur5.moveRel(0,0.3,0.2);
  ur5.moveRel(0,-0.3,-0.2);
  ur5.moveToHome();


  return 0;
}
