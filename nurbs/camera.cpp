#include "camera.h"
// model camera written for GLFW.
// Automatically captures input from a GLFW service.
    // Two normalized vectors define direction.
    AbstractCamera::AbstractCamera( GLFWwindow* window ): glWindow(window){
    }
    ModelCamera::ModelCamera( GLFWwindow* window ):AbstractCamera( window ){
        //glfwSetInputMode( window, GLFW_CURSOR ,GLFW_CURSOR_DISABLED );
        iState = CAM_INACTIVE;
    }

    void ModelCamera::setSpeedX( float speed ){
        fSpeedX = speed;
    }

    void ModelCamera::setSpeedY( float speed ){
        fSpeedY = speed;
    }

    float ModelCamera::getSpeedX(){
        return fSpeedX;
    }

    float ModelCamera::getSpeedY(){
        return fSpeedY;
    }

    void ModelCamera::setRadius( float f ){
        fRadius = f;
    }
    void ModelCamera::setScrollSpeed( float f ){
        fScrollSpeed = f;
    }


    void AbstractCamera::setOrientation( glm::vec3 vec ){
        vOrientation = vec;
    }

    glm::mat4 AbstractCamera::getViewMatrix(){
        return glm::lookAt( vPos, vLookAt, vUp );
    }

    void ModelCamera::reset( glm::vec3 vDir ){
        vPos = glm::normalize( vDir ) * fRadius;

        vLookAt = glm::normalize( -vPos );

        // Do norm of lookAt x ( lookAt x orientation_base_vector ) to get the new Up vector.
        vUp = glm::normalize( glm::cross( vLookAt, glm::cross( vLookAt, vOrientation ) ) );

        //mView = glm::lookAt( vPos, vLookAt, vUp );
    }

    glm::vec3 ModelCamera::getEye(){
      return vPos;
    }
    glm::vec3 ModelCamera::getLookAt(){
      return vLookAt;
    }

    void ModelCamera::glfwHandleCursor( float fTimeDelta ){


      if( scrollState == SCROLL_INACTIVE ){
        if( glfwGetMouseButton( glWindow, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS ){
          glfwSetInputMode( glWindow, GLFW_CURSOR ,GLFW_CURSOR_DISABLED );
          scrollState = SCROLL_ACTIVE;
          glfwSetCursorPos( glWindow, 0, 0 );
        }

      }else if( scrollState == SCROLL_ACTIVE ){
        if( glfwGetMouseButton( glWindow, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_RELEASE ){
          glfwSetInputMode( glWindow, GLFW_CURSOR ,GLFW_CURSOR_NORMAL );
          scrollState = SCROLL_INACTIVE;

        }else{
          double x, y;
          glfwGetCursorPos( glWindow, &x, &y );
          glfwSetCursorPos( glWindow, 0, 0 );
          fRadius -= y*fScrollSpeed;
          reset( getEye() );
        }
      }

        if( iState == CAM_INACTIVE ){
          if( glfwGetMouseButton( glWindow, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS ){
            glfwSetInputMode( glWindow, GLFW_CURSOR ,GLFW_CURSOR_DISABLED );
            iState = CAM_ACTIVE;
            glfwSetCursorPos( glWindow, 0, 0 );
          }
          return;
        }else if( iState == CAM_ACTIVE ){
          if( glfwGetMouseButton( glWindow, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_RELEASE ){
            glfwSetInputMode( glWindow, GLFW_CURSOR ,GLFW_CURSOR_NORMAL );
            iState = CAM_INACTIVE;
            return;
          }
        }


        double x,y;
        glfwGetCursorPos( glWindow, &x, &y );
        glfwSetCursorPos( glWindow, 0, 0 );

        // Scale components to obtain degree space projections.
        double degX = fSpeedX * x;
        double degY = fSpeedY * y;

        // vUp is used as axis for rotating the point about virtual X
        // vUp x vLookAt isusedas pivot axis for virtual Y.

        // First compute the new differentially modified point.
        glm::vec3 newPos = vPos;
        newPos = glm::rotate( newPos, static_cast<float>(degX), vUp );
        newPos = glm::rotate( newPos, static_cast<float>(degY), glm::cross( vUp, vLookAt ) );
        vPos = newPos;

        // Now compute vUp and vLookAt.

        // just the inverse of the current position vector.
        vLookAt = glm::normalize( -vPos );

        // Do norm of lookAt x ( lookAt x orientation_base_vector ) to get the new Up vector.
        vUp = glm::normalize( glm::cross( vLookAt, glm::cross( vLookAt, vOrientation ) ) );

        //mView = glm::lookAt( vPos, vLookAt, vUp );
    }

    FirstPersonCamera::FirstPersonCamera( GLFWwindow* window ):AbstractCamera(window){
      glfwSetInputMode( window, GLFW_CURSOR ,GLFW_CURSOR_DISABLED );
    }
    void FirstPersonCamera::setOmegaX( float f ){
      fOmegaX = f;
    }
    void FirstPersonCamera::setOmegaY( float f ){
      fOmegaY = f;
    }
    void FirstPersonCamera::setSpeed( float f ){
      fSpeed = f;
    }
    //void setOrientation( glm::vec3 );
    void FirstPersonCamera::reset( glm::vec3 v ){
        vPos = v;
        vLookAt = vPos + glm::vec3( 0.0f, 0.0f, 1.0f );
        vUp = vOrientation;
    }
    //glm::mat4 getViewMatrix();
    void FirstPersonCamera::glfwHandleCursor( float fDeltaTime ){
      vUp = vOrientation;

      glm::vec3 vFront = vLookAt - vPos;
      glm::vec3 vBack = -vFront;
      glm::vec3 vRight = glm::normalize( glm::cross( vFront, vUp ) );
      glm::vec3 vLeft = -vRight;

      if( glfwGetKey( glWindow, GLFW_KEY_W ) == GLFW_PRESS ){
        vPos += vFront * fDeltaTime * fSpeed;
      }
      if( glfwGetKey( glWindow, GLFW_KEY_S ) == GLFW_PRESS ){
        vPos += vBack * fDeltaTime * fSpeed;
      }
      if( glfwGetKey( glWindow, GLFW_KEY_A ) == GLFW_PRESS ){
        vPos += vLeft * fDeltaTime * fSpeed;
      }
      if( glfwGetKey( glWindow, GLFW_KEY_D ) == GLFW_PRESS ){
        vPos += vRight * fDeltaTime * fSpeed;
      }

      double x,y;
      glfwGetCursorPos( glWindow, &x, &y );
      glfwSetCursorPos( glWindow, 0, 0 );

      // Scale components to obtain degree space projections.
      double degX = fOmegaX * x;
      double degY = fOmegaY * y;

      glm::vec3 vNewLA = glm::normalize( glm::rotate( glm::normalize(vFront), (float)degY, glm::normalize( glm::cross( vFront, vUp ) ) ) );
      vNewLA = glm::normalize( glm::rotate( vNewLA, (float)degX, vUp ) );
      vLookAt = vPos + vNewLA;
      vUp = glm::normalize( glm::cross( glm::cross( vNewLA, vOrientation ), vNewLA ) );
    }
