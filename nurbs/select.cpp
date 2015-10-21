
#include "input.h"

#include <iostream>
#include <stdio.h>

using namespace std;


SelectHandler::SelectHandler( GLFWwindow* window ){
  window = window;
}
/*
  0 - no input
  1 - drag in progress
  2 - drag started
*/
int SelectHandler::doFrame(){
  int x;
  int y;
  glfwGetCursorPos( window, &x, &y );

  dragPos.x = x;
  dragPos.y = y;

  // Check if button down
  if( iState == 0 ){
    if( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ){
      iState = 1;
      dragStart.x = x;
      dragStart.y = y;

      return 2;
    }

  }else if( iState == 1 ){
    if( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_RELEASE ){
      iState = 0;
    }

    return 1;
  }

  return 0;
}
glm::vec2 SelectHandler::getDragPosition(){
  return dragPos;
}

glm::vec2 SelectHandler::getDragStart(){
  return dragStart;
}
