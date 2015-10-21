#include "stdgl.h"

class Protocol{
private:
  char* buffer;
  int iState;
  int iWaiting;
  glm::vec3 evtPt;
  glm::vec3 evtRay;
  glm::vec3 evtRef;

  int lastIndex;
  int lastCount;

public:
  static const int SIGNAL_OK = 0;
  static const int SIGNAL_DRAG = 1;
  static const int SIGNAL_CLICK = 2;

  Protocol();
  int doFrame();

  char* flush();
  int getLastIndex();
  int getLastcount();

  void setEventPoint( glm::vec3 );
  void setEventRay( glm::vec3 );
  void setEventRef( glm::vec3 );


  void setSignalState( int );

};
