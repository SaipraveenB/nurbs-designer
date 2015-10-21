#include "scene.h"

Scene::Scene( ){
  pObjects = new std::vector<Object*>;
}

Scene::~Scene( ){
  for( int i = 0; i < this->getObjectCount(); i++ )
    delete (this->getObjectRef(i));
  delete pObjects;
}

int Scene::addObject( Object* obj ){
  pObjects->push_back( obj );
  return pObjects->size();
}

Object* Scene::getObjectRef( int i ){
  return pObjects->at(i);
}

int Scene::getObjectCount(){
  return pObjects->size();
}
// Transfer vertex attribute arrays into the GL memory
void Scene::glTransfer(){
  for( int i = 0; i < this->getObjectCount(); i++ )
    (this->getObjectRef(i))->glTransfer();

}

// Draw objects along with the required effects. Output is rendered to the currently active Framebuffer(s).
void Scene::glDraw(){
  for( int i = 0; i < this->getObjectCount(); i++ ){
    (this->getObjectRef(i))->glDraw();
  }
}

void Scene::glDraw( AbstractTechnique* t ){
  for( int i = 0; i < this->getObjectCount(); i++ ){
    t->glDrawObject( this->getObjectRef(i) ); // Perform a contextual draw. This way the technique can obtain object-specific properties and transfer them before the draw.
  }
}
