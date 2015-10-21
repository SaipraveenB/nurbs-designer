
#include "protocol.h"

#include <iostream>
#include <stdio.h>

using namespace std;



Protocol::Protocol(){
  buffer = new char[10];
}

int Protocol::getLastIndex(){
  return lastIndex;
}

int Protocol::getLastcount(){
  return lastCount;
}

int Protocol::doFrame(){
    // First send the signal to Controller asking for the all clear.
    if( iState == SIGNAL_OK ){
      std::cout<<"X\n";fflush(stdout);
    }else if( iState == SIGNAL_CLICK ){
      std::cout<<"Y "<<evtPt.x<<" "<<evtPt.y<<" "<<evtPt.z<<" "<<evtRay.x<<" "<<evtRay.y<<" "<<evtRay.z<<" "<<evtRef.x<<" "<<evtRef.y<<" "<<evtRef.z<<"\n";fflush(stdout);
    }else if( iState == SIGNAL_DRAG ){
      std::cout<<"Z "<<evtPt.x<<" "<<evtPt.y<<" "<<evtPt.z<<" "<<evtRay.x<<" "<<evtRay.y<<" "<<evtRay.z<<" "<<evtRef.x<<" "<<evtRef.y<<" "<<evtRef.z<<"\n";fflush(stdout);
    }

    // We use 'A' to signal a vertex list change.
    // 'B' to signal all OK.
    char c[2];

    cin >> c;
    if( c[0] == 'A' ){
      //cin.read(c, 1);
      int index;
      cin >> index;
      this->lastIndex = index;

      int n;
      cin >> n;
      this->lastCount = n;

      //std::cout<<"Detected "<<n<<std::endl;fflush(stdout);
      char* cbuf = new char[120 * n];
      cin.getline( cbuf, 120 * n );
      delete[] buffer;
      buffer = cbuf;
      return 0;
    }else if( c[0] == 'B' ){
      return 1;
    }

    return 1;

}
char* Protocol::flush(){
  return buffer;
}

void Protocol::setEventPoint( glm::vec3 evtPt ){
  this->evtPt = evtPt;
}
void Protocol::setEventRay( glm::vec3 evtRay ){
  this->evtRay = evtRay;
}
void Protocol::setEventRef( glm::vec3 evtRef ){
  this->evtRef = evtRef;
}

void Protocol::setSignalState( int state ){
  iState = state;
}
