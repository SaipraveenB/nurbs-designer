#include "technique.h"


class FXLineDraw : public WVPBasedTechnique{
  GLint uBaseColor;
  glm::vec4 vBaseColor;
public:
  FXLineDraw();
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
  void loadUniformsFromObject( Object* o );
};
