//
//  URControl.cpp
//  tutorial
//
//  Created by <author> on 21/02/2018.
//
//

#include "URControl.hpp"
#include <fstream>
#include <unistd.h>

//NOTE: The following defines shall be moved to settings.hpp
#define ACC     0.1
#define VEL     0.1
#define MOVTIME 5
#define BLENDR  0
#define UR_MAX_X  1000
#define UR_MIN_X  -1000
#define UR_MAX_Y  1000
#define UR_MIN_Y  -1000
#define UR_MAX_Z  10000
#define UR_MIN_Z  0

#define URCONTROL_MODE 0 //0=standard 1=debug

USE_ROBWORK_NAMESPACE

using namespace robwork;
using namespace rwhw;

using namespace std;

URControl::URControl()
{
  //Default settings
  port = 30002;
  ip = "192.168.100.4";
  haveBeenToInit = 0;
}

URControl::URControl(string anIp, int APort)
{
  port = APort;
  ip = anIp;
  haveBeenToInit = 0;
}

void URControl::connect()
{
  ur.connect(ip, port);
  ur.start();
}

void URControl::disconnect()
{
  ur.stop();
  ur.disconnect();
}

void URControl::sendScript(string fileName)
{
  ur.sendScriptFile(fileName);
  if(URCONTROL_MODE)
  {
    cout << "Script sent!" << endl;
  }
}

void URControl::moveToInit()
{
  sendScript("goToInit.txt");
  haveBeenToInit = 1;
}

void URControl::moveToHome()
{
  sendScript("goToHome.txt");
  haveBeenToInit = 0;
}

void URControl::moveRel(double anX, double aY, double aZ)
{
  //Convert from mm to m:
  double x = anX/1000.0;
  double y = aY/1000.0;
  double z = aZ/1000.0;

  //Security check:
  if(!haveBeenToInit)
  {
    throw("[URControl::moveRel]: Cannot use this function w/o having been to init-conf. first!");
  }

  //Security check:
  if((currToolPos[0]+x) < UR_MIN_X || (currToolPos[0]+x) > UR_MAX_X)
  {
    throw("[URControl::moveRel]: New x-coordinates are out of bounds!");
  }
  else if((currToolPos[1]+y) < UR_MIN_Y || (currToolPos[1]+y) > UR_MAX_Y)
  {
    throw("[URControl::moveRel]: New y-coordinates are out of bounds!");
  }
  else if((currToolPos[2]+z) < UR_MIN_Z || (currToolPos[2]+z) > UR_MAX_Z)
  {
    throw("[URControl::moveRel]: New z-coordinates are out of bounds!");
  }

  //Generate scriptfile:
  string fileName;

  if(URCONTROL_MODE)
  {
    fileName = "moveRelScript" + to_string(numOfMoves) + ".txt";
  }
  else
  {
    fileName = "moveRelScript.txt";
  }

  ofstream out(fileName, ofstream::out);

  out << "HOST=" << ip << "\n" << "PORT=" << port << "\n" << "def moveRel():\n";
  out << "\tpos=p[" << to_string(currToolPos[0]+x) << ", " << to_string(currToolPos[1]+y) << ", " << to_string(currToolPos[2]+z) << ", " << currToolPos[3] << ", " << currToolPos[4] << ", " << currToolPos[5] << "]\n";
  out << "\tmovel(pos," << ACC << ", " << VEL << "," << MOVTIME << "," << BLENDR << ")\n";
  out << "\ttextmsg(\"Moved to position:" << to_string(currToolPos[0]+x) << ", " << to_string(currToolPos[1]+y) << ", " << to_string(currToolPos[2]+z) << ", " << currToolPos[3] << ", " << currToolPos[4] << ", " << currToolPos[5] << "\")\n";
  out << "end\n";

  out.close();


  //Send scriptfile to peform movement:
  sendScript(fileName);

  if(URCONTROL_MODE)
  {
    cout << "Sent scriptfile: \"" << fileName << "\"!" << endl;
  }

  //Update current tool position:
  currToolPos[0] += x;
  currToolPos[1] += y;
  currToolPos[2] += z;

  usleep((MOVTIME*1000000)+100000); //Wait for movement to finish (MOVTIME) + 100000 µs (0.1 s)
}

//Helpful debug functions:
void URControl::printcurrToolPos()
{
  cout << "Current tool position (x, y, z, rotX, rotY, rotZ): ";
  for(int i = 0; i < 6; i++)
  {
    cout << currToolPos[i] << "  ";
  }
  cout << endl;
}

URControl::~URControl() {}
