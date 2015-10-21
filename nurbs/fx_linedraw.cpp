#include "fx_linedraw.h"
  FXLineDraw::FXLineDraw( ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    AbstractTechnique::load("fx/linedraw.vs","fx/linedraw.fs");
    glLinkUniforms();
  }
  void FXLineDraw::glLinkUniforms(){
    this->glActivate();
    this->WVPBasedTechnique::glLinkUniforms();
    uBaseColor = glGetUniformLocation( shader, "col" );

  }

  void FXLineDraw::loadUniformsFromObject( Object* o ){
    vBaseColor = o->getProperty( Object::PROP_BASE_COLOR );
  }


  void FXLineDraw::glTransferUniforms(){
    this->WVPBasedTechnique::glTransferUniforms();
    if( uBaseColor!= -1 ){
      glUniform4fv( uBaseColor, 1, (const float*)&vBaseColor );
    }
  }
  void FXLineDraw::glActivate(){
    glUseProgram( shader );
  }
