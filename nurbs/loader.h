#include "stdgl.h"
#include "object.h"

class ModelLoader{
public:
  ModelLoader();
  Object* load( const std::string&, bool );
  void loadLineSequence( char* stream, LinearObject* );
  void loadLineSequence( char* stream, int i );
  void load3DLineSequence( char* stream, LinearObject* lobj );
};
