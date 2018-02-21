//
//  URControl.hpp
//  tutorial
//
//  Created by SLS on 21/02/2018.
//
//

#ifndef URControl_hpp
#define URControl_hpp

#include <stdio.h>
#include <string>
#include <rw/rw.hpp>
#include <rwhw/universalrobots/URCallBackInterface.hpp>
//#include <rwhw/universalrobots/UniversalRobotsRTLogging.hpp>

USE_ROBWORK_NAMESPACE

using namespace robwork;
using namespace rwhw;

using namespace std;

class URControl
{
private:
public:
    URControl();
    URControl(string anIp, int aPort);

    void connect();
    void sendScript(string fileName);

    void moveToInit();
    void moveRel(double x, double y, double z);

    ~URControl();
private:
  rwhw::URCallBackInterface _ur;
  //rwhw::UniversalRobotsRTLogging _urrt;
  int port;
  string ip;
  bool isConnected;
  double currToolPos[6] = {0, 0, 0, 0, 0, 0}; //TODO: correct values

};


#endif /* URControl_hpp */
