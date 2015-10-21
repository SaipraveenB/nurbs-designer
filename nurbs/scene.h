#include "stdgl.h"
#include "object.h"
#include <vector>
#include "technique.h"
/*
  A Collection of objects, techniques and a sequence of drawing instructions.
*/
#ifndef SCENE_H
#define SCENE_H
class Scene{
  private:
    std::vector<Object*> *pObjects;
  public:
    Scene();
    ~Scene();
    int addObject( Object* );
    Object* getObjectRef( int );
    int getObjectCount();

    // Transfer vertex attribute arrays into the GL memory
    void glTransfer();

    // Draw objects along with the required effects. Output is rendered to the currently active Framebuffer(s).
    void glDraw();
    void glDraw( AbstractTechnique* );
};

#endif
