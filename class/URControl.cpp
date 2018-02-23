//
//  URControl.cpp
//  tutorial
//
//  Created by <author> on 21/02/2018.
//
//

#include "URControl.hpp"
#include <fstream>

USE_ROBWORK_NAMESPACE

using namespace robwork;
using namespace rwhw;

using namespace std;

URControl::URControl()
{
  //Default settings
  port = 30002;
  ip = "192.168.100.4";
  isConnected = 0;
}

URControl::URControl(string anIp, int APort)
{
  port = APort;
  ip = anIp;
  isConnected = 0;
}

void URControl::connect()
{
  _ur.connect(ip, port); //Note: default port is 30002
  _urrt.connect(ip, 30003);
  isConnected = true;
}

void URControl::sendScript(string fileName)
{
  if(isConnected)
  {
    _ur.startCommunication(port,URCallBackInterface::CB2, fileName);
    _urrt.start();
    _ur.stopCommunication();
  }
  else
  {
    throw("[URControl::sendScript()]: Robot is not connected! Use connect()!");
  }
}

void URControl::moveToInit()
{
  ifstream f("goToInit.txt", ifstream::in);
  if(f.good())
  {
    sendScript("goToInit.txt");
  }
  else
  {
    throw("[URControl::moveToInit()]: Script ''goToInit.txt'' is not present!");
  }
}

void URControl::moveRel(double x, double y, double z)
{
  ofstream out("moveRelScript.txt", ofstream::out);

  out << "HOST=" << ip << endl << "PORT=" << port << endl << "def moveRel():" << endl;
  out << "pos=p[" << to_string(currToolPos[0]+x) << ", " << to_string(currToolPos[1]+y) << ", " << to_string(currToolPos[2]+z) << ", " << currToolPos[3] << ", " << currToolPos[4] << ", " << currToolPos[5] << endl;
  //TODO:Finish

  out.close();
}

URControl::~URControl() {}
