
#include "stdgl.h"
#include "scene.h"
#include "camera.h"
#include "loader.h"
#include "render_target.h"
#include "fx_linedraw.h"
#include "protocol.h"
#include "input_handler.h"
#include "triangulator.h"


#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

// GPU Shader for rendering onto screen.
FXLineDraw* pLineDraw;

// GPU handle to hold/transfer vertex data.
Scene* pScene;

// GLFW window context
GLFWwindow* window;

// Parser for line strings.
ModelLoader loader;

// Protocol I/O handler
Protocol* pProtocol;

// Drag detect and control module.
InputHandler* pInput;

// Camera controls.
ModelCamera* pCamera;

// Triangulator to compute adjacency to complete a 3D grid mesh.
Triangulator* pTriangulator;

int numFrames = 0;

  Scene* makeScene(){
    ModelLoader loader;
    LinearObject* o = new LinearObject();
    o->setDrawMode( GL_LINES );

    LinearObject* o2 = new LinearObject();
    o2->setDrawMode( GL_LINES );

    // Rende/r mode: triangluar mesh
  /*  o->glSetTexture(0, pOcclusionCoverage->getColorTexture() );// Texture
    o->glSetTexture(1, pSkyTech->getTexture() );  // Environment map.
    o->glSetTexture(2, pShadowTarget->getColorTexture() );  // Blurred shadows-only image. for soft-shadows.
    o->glSetTexture(3, pShadowsHV->getColorTexture() );
    //o->glSetTexture(0, new Texture( GL_TEXTURE_2D, pCube->getTexture() ) );
    o->setProperty( Object::PROP_BASE_COLOR, glm::vec4(0.5,0.5,0.5,1.0) );
    o->setProperty( Object::PROP_PHONG_COEFFS, glm::vec4(0.0,1.5,0.0,0.08) );// RDSA
    o->setWorldMatrix(glm::mat4());*/

    Vertex v1;
    v1.pos = glm::vec3( 0.0f, 0.5f, 0.0f );

    Vertex v2;
    v2.pos = glm::vec3( -0.5f, 0.0f, 0.0f );

    Vertex v3;
    v3.pos = glm::vec3( 0.5f, 0.0f, 0.0f );

    o->addExtension( v1 );
    o->addExtension( v2 );
    o->addExtension( v3 );
    o->setProperty( Object::PROP_BASE_COLOR, glm::vec4(102.0/256,204/256.0,255/256.0,1.0) );

    o2->addExtension( v1 );
    o2->addExtension( v2 );
    o2->setProperty( Object::PROP_BASE_COLOR, glm::vec4(0.0,0.0,0.0,1.0) );

    LinearObject* xAxis = new LinearObject();
    xAxis->setDrawMode( GL_LINES );

    LinearObject* yAxis = new LinearObject();
    xAxis->setDrawMode( GL_LINES );

    LinearObject* zAxis = new LinearObject();
    xAxis->setDrawMode( GL_LINES );

    Vertex origin;
    origin.pos = glm::vec3( 0.0f, 0.0f, 0.0f );

    Vertex x0;
    x0.pos = glm::vec3( 1.0f, 0.0f, 0.0f );

    Vertex y0;
    y0.pos = glm::vec3( 0.0f, 1.0f, 0.0f );

    Vertex z0;
    z0.pos = glm::vec3( 0.0f, 0.0f, 1.0f );

    xAxis->addExtension( origin );
    yAxis->addExtension( origin );
    zAxis->addExtension( origin );

    xAxis->addExtension( x0 );
    yAxis->addExtension( y0 );
    zAxis->addExtension( z0 );

    xAxis->setProperty( Object::PROP_BASE_COLOR, glm::vec4(153.0/256,153/256.0,153/256.0,1.0) );

    yAxis->setProperty( Object::PROP_BASE_COLOR, glm::vec4(153.0/256,255/256.0,153/256.0,1.0) );

    zAxis->setProperty( Object::PROP_BASE_COLOR, glm::vec4(255.0/256,153/256.0,255/256.0,1.0) );


    Scene* sc = new Scene();

    sc->addObject(o);
    sc->addObject(o2);

    sc->addObject(xAxis);
    sc->addObject(yAxis);
    sc->addObject(zAxis);

    return sc;
  }


  void populateObject( Object* o, char* data ){
    loader.loadLineSequence( data, reinterpret_cast<LinearObject*>(o) );
  }

  void populateObject3D( Object* o, char* data ){
    loader.load3DLineSequence( data, reinterpret_cast<LinearObject*>(o) );
  }

  void populateObject3DGrid( Object* o, char* data ){
    loader.load3DLineSequence( data, reinterpret_cast<LinearObject*>(o) );
    pTriangulator->squareTriangulate( reinterpret_cast<LinearObject*>(o) );
  }

float iFrameTime = 0;

  void mainLoop(){

    // Get frame descriptor
    float deltaTime =  - ( ( iFrameTime ) - ( iFrameTime = glfwGetTime() ) );

    glDisable(GL_CULL_FACE);
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    //std::cout<<"A"<<"\n";
    int status = pInput->doFrame();
    pCamera->glfwHandleCursor( deltaTime );
    //std::cout<<status<<":\n";


    pLineDraw->glActivate();
    glm::mat4 m = pCamera->getViewMatrix();

    pLineDraw->setView( m );
    pLineDraw->glTransferUniforms();

    glm::vec4 v = glm::vec4( 0.5f, 0.0f, -1.5f, 1.0f ) * pLineDraw->getWVP();

    //std::cout<<v.x/v.w<<" "<<v.y/v.w<<" "<<v.z/v.w<<"\n";


    pScene->glDraw( pLineDraw );



    if( status == 0 ){
        // do nothing.
        pProtocol->setSignalState( Protocol::SIGNAL_OK );
    }else if( status == 2 ){
        //std::cout<<"Clicked\n";

        glm::vec3 projPt = glm::vec3( glm::vec2(1.0f,-1.0f) * ( (pInput->getDragStart() * 2.0f/glm::vec2(640.0f,480.0f) ) - glm::vec2( 1.0f, 1.0f ) ), 0.5f );
        glm::vec4 ptTemp = glm::inverse( pLineDraw->getWVP() ) * glm::vec4( projPt , 1.0f  );
        glm::vec3 pt1 = glm::vec3( ptTemp.x/ptTemp.w, ptTemp.y/ptTemp.w, ptTemp.z/ptTemp.w );
        glm::vec3 pt0 = pCamera->getEye();

        pProtocol->setSignalState( Protocol::SIGNAL_CLICK );
        pProtocol->setEventPoint( pt0 );
        pProtocol->setEventRay( glm::normalize( pt1 - pt0 ) );
        pProtocol->setEventRef( pCamera->getLookAt() );

    }else if( status == 1 ){
        //std:cout<<"Dragged\n";
        glm::vec3 projPt = glm::vec3( glm::vec2(1.0f,-1.0f) * ( (pInput->getDragPosition() * 2.0f/glm::vec2(640.0f,480.0f) ) - glm::vec2( 1.0f, 1.0f ) ), 0.5f );
        glm::vec4 ptTemp = glm::inverse( pLineDraw->getWVP() ) * glm::vec4( projPt , 1.0f  );
        glm::vec3 pt1 = glm::vec3( ptTemp.x/ptTemp.w, ptTemp.y/ptTemp.w, ptTemp.z/ptTemp.w );
        glm::vec3 pt0 = pCamera->getEye();

        pProtocol->setSignalState( Protocol::SIGNAL_DRAG );
        pProtocol->setEventPoint( pt0 );
        pProtocol->setEventRay( glm::normalize( pt1 - pt0 ) );
        pProtocol->setEventRef( pCamera->getLookAt() );
    }

    if( !pProtocol->doFrame() ){

      //std::cout<<"Swapping frames."; fflush(stdout);

      char* data = pProtocol->flush();

      //std::cout<<data; fflush(stdout);
      // Replace current scene.
      //pScene = makeScene( data );
      pScene->getObjectRef( pProtocol->getLastIndex() )->reset();
      populateObject3DGrid( pScene->getObjectRef( pProtocol->getLastIndex() ), data );
      pScene->glTransfer();
    }

    glfwPollEvents();

    glfwSwapBuffers( window );


    //numFrames++;
    //if( !(numFrames%50) )
      //std::cout<<numFrames<<std::endl;


  }


    /*
    *  C++ Main function.
    * */

    // MAIN
    int main (int argc, char** argv) {
      Magick::InitializeMagick(argv[0]);
      // start GL context and O/S window using the GLFW helper library
      if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
      }
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    #ifdef __APPLE__
      std::cout<<"Apple FTW\n";
      glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

      window = glfwCreateWindow (1280, 720, "NURBS Renderer", NULL, NULL);
      if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
      }

      glfwSetWindowSize( window, 640, 480 );

      glfwWindowHint(GLFW_SAMPLES, 16);
      glEnable( GL_MULTISAMPLE );
      
      glfwMakeContextCurrent (window);
      // start GLEW extension handler
      glewExperimental = GL_TRUE;
      glewInit();

      // get version info
      const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
      const GLubyte* version = glGetString (GL_VERSION); // version as a string
      //printf ("Renderer: %s\n", renderer);
      //printf ("OpenGL version supported %s\n", version);


      pLineDraw = new FXLineDraw( );
      pProtocol = new Protocol( );
      pInput = new InputHandler( window );
      pCamera = new ModelCamera( window );
      pTriangulator = new Triangulator();

      pCamera->setSpeedX( -0.1f );
      pCamera->setSpeedY( 0.1f );

      pCamera->setRadius( 5.0f );
      pCamera->setOrientation( glm::vec3( 0.0f, -1.0f, 0.0f ) );
      pCamera->reset( glm::vec3( 0.0f, 0.0f, 1.0f ) );
      pCamera->setScrollSpeed( 0.05f );

      pLineDraw->setProj( glm::perspective( 30.0f, 640.0f/480.0f, 1.0f, 1000.0f ) );
      pLineDraw->setWorld( glm::mat4() );


      Scene *sc = makeScene();
      sc->glTransfer();

      pScene = sc;
      glLineWidth(2);

      //cout<<"Transferred scene\n";fflush( stdout );




      /*
      pDDTech = new FXDirectDraw(  );  // Configure direct draw shaders to average 16 samples.
      pSampleTech = new FXDirectDrawMS( 16 );
      pHDRTech = new FXHDR(  );  //  HDR rendering technique to convert a given HDR texture to an LDR output. Multisampled to 16 samples per pixel.
      pBloomTechH = new FXBloomH( 10 ); // Render bright area horizontal bloom.
      pBloomTechV = new FXBloomV( 10 ); // Render bright area vertical bloom.
      pCombineTech = new FXCombine();
      pProjectTech = new FXProject();
      pShadowTech = new FXShadow();
      pScatterTech = new FXScatter();
      pLPass = new FXLightPass( 5.0f );
      pRTResolver = new FXRTSurfaceResolve();
      pSkyTech = new Skybox();
      pLight0 = new PointLight();
      pLight1 = new PointLight();
      pSpotLight = new SpotLight();
      pAmbient = new AmbientLight();
      pSQuad = new ScreenQuad();
      pCube = new GradientCube( 50, 50, 50 );
      pTarget = new RenderTarget( 640, 480, GL_RGBA32F, GL_RGBA, GL_FLOAT, 16, false );
      pSampled = new RenderTarget( 640, 480, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pTarget2 = new RenderTarget( 640, 480, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pDownsampled = new RenderTarget( 320, 240, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, true );
      pHighlight = new RenderTarget( 320, 240, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pBloomH = new RenderTarget( 640, 480, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pBloomHV = new RenderTarget( 640, 480, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pLDR = new RenderTarget( 640, 480, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pShadowTarget = new CubemapRenderTarget( 1024, 1024, GL_RGBA32F, GL_RGBA, GL_FLOAT );
      pShadows = new RenderTarget( 640, 480, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pScatter = new RenderTarget( 640, 480, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pShadowsH = new RenderTarget( 640, 480, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pShadowsHV = new RenderTarget( 640, 480, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pSpotShadowTarget = new RenderTarget( 512, 512, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pViewShadowTarget = new RenderTarget( 512, 512, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pOcclusionQuery = new RenderTarget( 1024, 1024, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pOcclusionCoverage = new RenderTarget( 1024, 1024, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, false );
      pBackBuffer = new RenderTarget( 640, 480 );*/




      /*pScatterTech->setLight( pSpotLight );

      pSQuad->glTransfer();
      pSQuad->glSetTexture( 0, pTarget->getColorTexture() );

      matProj = glm::transpose( glm::perspective( 30.0f, 640.0f/480.0f, -1.0f, -1000.0f ) );
      matWorld = glm::mat4();

      pTech->setWorld( matWorld );
      pTech->setProj( matProj );
      pTech->setView( glm::mat4() );

      std::cout<<"Setting project tech parameters";fflush(stdout);
      pProjectTech->setWorld( glm::mat4() );
      pProjectTech->setProj( glm::transpose( glm::perspective( 90.0f, 1.0f, -0.5f, -1000.0f ) ) );
      pProjectTech->setView( glm::mat4() );

      pScatterTech->setWorld( matWorld );
      pScatterTech->setProj( matProj );
      pScatterTech->setView( glm::mat4() );

      std::cout<<"Setting Exposure";fflush(stdout);

      fExposure = 1.0f;
      pHDRTech->setExposure( 1.0f );  // Initialize Exposure.

      pSkyTech->setWorld( matWorld );
      pSkyTech->setProj( matProj );
      pSkyTech->setView( glm::mat4() );

      pShadowTech->setWorld( matWorld );
      pShadowTech->setProj( matProj );
      pShadowTech->setView( glm::mat4() );


      pTech->addLightP( pLight0 );
      //pTech->addLightP( pLight1 );
      pTech->setLightA( pAmbient );
      pTech->setSpecularExponent( 32.0f );
      pTech->setIntensityPackingConstant( 300.0f );
      pSkyTech->setTexture("skybox/ame_mudskip","front.tga","back.tga","top.tga","down.tga","right.tga","left.tga");
      ///pSkyTech->setTexture( pShadowTarget->getColorTexture() );
      pSkyTech->glTransfer();

      pScene = makeScene();
      pScene->glTransfer();

      //TriangleMeshRayTracer *rt = new TriangleMeshRayTracer( pScene );
      clock_t occBuildS = clock();
      pOcclusionTracer = new OcclusionTracer( pScene );
      clock_t occBuildE = clock();

      /*cam = new ModelCamera( window );
      cam->setSpeedX( -0.1f );
      cam->setSpeedY( 0.1f );

      cam->setRadius( 5.0f );
      cam->setOrientation( glm::vec3( 0.0f, -1.0f, 0.0f ) );
      cam->reset( glm::vec3( 0.0f, 0.0f, 1.0f ) );*/

      /*cam = new FirstPersonCamera( window );
      cam->setOmegaX( -0.1f );
      cam->setOmegaY( -0.1f );
      cam->setOrientation( glm::vec3( 0.0f, 1.0f, 0.0f ) );
      cam->reset( glm::vec3( 0.0f, 0.0f, 6.0f ) );
      cam->setSpeed( 2.5f );

      std::cout<<"Camera Ready"<<std::endl;fflush(stdout);

      toneMapCoordinates( 320, 240, &tmLvl, &tmWidth, &tmHeight );

      std::cout<<"Tone mapping references: @level:"<<tmLvl<<" @width:"<<tmWidth<<" @height:"<<tmHeight<<std::endl;fflush(stdout);

      pLight0SHM = getShadowMapWVP( pLight0 );


      pSpotLight->setPosition( lightPos );
      pSpotLight->setColor( glm::vec4(1.0f,1.0f,1.0f,0.25f) );
      pSpotLight->setConstants( glm::vec4(0.02f,0.0f,0.0f,0.0f) );
      pSpotLight->setHalfAngle( 3.1415f / 8.0f );
      pSpotLight->setDirection( glm::vec3( 0.0f, 0.0f, -1.0f ) );

      pLightScatterCone = new Scene();
      Object* slCone = pSpotLight->getBoundingGeometry( 19 );
      slCone->glSetTexture( 0, pSpotShadowTarget->getColorTexture() );
      slCone->glSetTexture( 1, pViewShadowTarget->getColorTexture() );
      pLightScatterCone->addObject( slCone );

      pLightScatterCone->glTransfer();

      glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
      glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
      glDisable(GL_CULL_FACE);


      // This step draws the query surface for the raytracer.
      // Effectively this represents the positions of all the patches
      // on a surface. This also serves to set the patch samples at a certain resolution.
      // a higher resolution receiver surface would produce better-looking lighting.

      pOcclusionQuery->glBind();
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


      pRTResolver->glActivate();
      Object* ref = pScene->getObjectRef(0);
      pRTResolver->glDrawObject( ref );
      clock_t occProcessS = clock();
      pOcclusionTracer->processTexture( pOcclusionQuery->getColorTexture(), pOcclusionCoverage->getColorTexture(), 1024, 1024 );
      clock_t occProcessE = clock();

      float buildTime = ((float)occBuildE-(float)occBuildS) / CLOCKS_PER_SEC;
      float processTime = ((float)occProcessE-(float)occProcessS) / CLOCKS_PER_SEC;
      std::cout<<"Data Structure Build Time: "<<buildTime;
      std::cout<<"Surface Process Time: "<<processTime;*/

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      while (!glfwWindowShouldClose(window)) {
        mainLoop();
      }

      glfwTerminate();
      return 0;
    }
