#include "fx_combine.h"
FXCombine::FXCombine( ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    AbstractTechnique::load("fx/hdr.vs","fx/combine.fs");
    glLinkUniforms();
  }
  void FXCombine::glLinkUniforms(){
    this->glActivate();
    uTex0 = glGetUniformLocation( shader, "sampler0" );
    uTex1 = glGetUniformLocation( shader, "sampler1" );
    u0 = glGetUniformLocation( shader, "f0" );
    u1 = glGetUniformLocation( shader, "f1" );
  }

  void FXCombine::glTransferUniforms(){
    glUniform1i( uTex0, 0 );
    glUniform1i( uTex1, 1 );

    glUniform1f( u0, f0 );
    glUniform1f( u1, f1 );

  }
  void FXCombine::glActivate(){
    glUseProgram( shader );
  }

  void FXCombine::setFactors( float a, float b ){
    f0 = a;
    f1 = b;
  }

  void FXCombine::glPass( Texture* pSource0, Texture* pSource1, RenderTarget* pDest, ScreenQuad* pSQuad, float f0, float f1 ){
    this->setFactors( f0, f1 );

    pDest->glBind();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->glActivate();
    this->glTransferUniforms();
    pSQuad->glSetTexture( 0, pSource0 );
    pSQuad->glSetTexture( 1, pSource1 );
    pSQuad->glDraw();

    pDest->generateMipmaps();
  }
