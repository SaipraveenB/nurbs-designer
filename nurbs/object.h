

#include "stdgl.h"
#include <vector>
#include "texture.h"

#ifndef OBJECT_H
#define OBJECT_H
struct Vertex{
    glm::vec3 pos;
    glm::vec2 texColor;
    glm::vec3 normal;

    Vertex():pos( glm::vec3(0,0,0) ),texColor( glm::vec2(0,0) ),normal( glm::vec3(0,0,0) ){

    }

    Vertex( glm::vec3 pos, glm::vec2 texColor, glm::vec3 normal ):pos(pos),texColor(texColor),normal(normal){
    }

    Vertex( glm::vec3 pos, glm::vec2 texColor ):pos(pos),texColor(texColor),normal( glm::vec3(0,0,0) ){

    }
};


class Object{
protected:
    // Bulk data structures.
    std::vector<Vertex>* vBuf;
    std::vector<int>* iBuf;
    std::vector<Texture*>* pTexs;

    // GPU memory addresses.
    GLuint glVBuf;
    GLuint glIBuf;
    GLuint glArrayDef;

    // Draw properties.
    GLenum glDrawMode;

    // Absolute position. All primitives are transformed by this matrix before rasterizing.
    glm::mat4 mWorld;

    // Vector list to hold properties like specular coeff, diffuse coeff, etc.
    glm::vec4* pProps;

    //IntersectableObject* pIntersectable;

public:
    // Property Constant definitions.
    const static int PROP_BASE_COLOR = 0;
    const static int PROP_PHONG_COEFFS = 1;

    const static int MAX_PROPERTY_COUNT = 25;
    const static int MAX_TEXTURES = 8;
    Object();
    ~Object();

    void reset();

    int addVertex( glm::vec3 pos, glm::vec2 texColor );
    int addVertex( Vertex v );
    int addVertex( glm::vec3 pos, glm::vec2 texColor, glm::vec3 normal );

    int addSurfaceI( int a, int b, int c );
    int addSurfaceV( Vertex a, Vertex b, Vertex c );
    int addSurfaceVN( Vertex a, Vertex b, Vertex c );

    int getIndexCount();
    int getVertexCount();

    void glSetTexture( int i, const std::string& filename );
    void glSetTexture( int i, Texture* pTex );
    void setDrawMode( GLenum drawMode );

    void glTransfer();

    void glBind();
    void glDraw();

    glm::mat4 getWorldMatrix();
    void setWorldMatrix( glm::mat4 );

    glm::vec4 getProperty( int );
    void setProperty( int, glm::vec4 );

    void getIndexList( std::vector<int>** ppIndex );
    void getVertexList( std::vector<Vertex>** ppVertex );
    void getSurface( int i, Vertex* v1, Vertex* v2, Vertex* v3 );
    void clearIndices();
    //IntersectableObject* getIntersectableObject();
    //void setIntersectableObject( IntersectableObject* );
};

class LinearObject: public Object{
public:
  LinearObject();
  int addLineV( Vertex v1, Vertex v2 );
  int addLineI( int v1, int v2 );
  int addExtension( Vertex v );
  int closePoly();
};

class ScreenQuad : public Object{
public:
  ScreenQuad();
};
#endif
