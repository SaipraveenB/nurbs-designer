
#include "input_handler.h"

#include <stdio.h>
#include <iostream>

using namespace std;


InputHandler::InputHandler( GLFWwindow* pwindow ){
  this->pwindow = pwindow;
  iState = 0;
  glfwSetInputMode( pwindow, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE );
  glfwSetInputMode( pwindow, GLFW_STICKY_KEYS, GL_TRUE );
  //glfwSetInputMode( pwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
}
/*
  0 - no input
  1 - drag in progress
  2 - drag started
*/
int InputHandler::doFrame(){
  //cout<<"Doing frame\n";fflush(stdout);

  double x;
  double y;
  glfwGetCursorPos( pwindow, &x, &y );

  dragPos.x = x;
  dragPos.y = y;
  //cout<<"iState:"<<x<<" "<<y<<"\n";
  // Check if button down
  if( iState == 0 ){
    //std::cout<<"Not clicked yet\n";
    if( glfwGetMouseButton( pwindow, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ){
      iState = 1;
      dragStart.x = x;
      dragStart.y = y;
      //std::cout<<"Clicked now\n";

      return 2;
    }

  }else if( iState == 1 ){
    //std::cout<<"Clicked already\n";
    if( glfwGetMouseButton( pwindow, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_RELEASE ){
      iState = 0;
    }

    return 1;
  }else
    iState = 0;

  return 0;
}
glm::vec2 InputHandler::getDragPosition(){
  return dragPos;
}

glm::vec2 InputHandler::getDragStart(){
  return dragStart;
}
