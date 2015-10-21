#include "stdgl.h"

class InputHandler{
private:
  char* buffer;
  int iWaiting;
  GLFWwindow* pwindow;

  int iState;
  glm::vec2 dragStart;
  glm::vec2 dragPos;

public:
  InputHandler( GLFWwindow* );
  int doFrame();
  glm::vec2 getDragStart();
  glm::vec2 getDragPosition();

};
