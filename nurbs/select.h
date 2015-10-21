#include "stdgl.h"

class SelectHandler{
private:
  Scene* pScene;

public:
  SelectHandler( Scene* sc );
  int handleSelect( glm::vec2 pos );
  int getSelected()

};
