#include "object.h"

#include <iostream>


using namespace std;

Object::Object(){
    vBuf = new vector<Vertex>;
    iBuf = new vector<int>;
    pTexs = new vector<Texture*>;
    mWorld = glm::mat4();
    pProps = new glm::vec4[MAX_PROPERTY_COUNT];

    for( int i =0; i < MAX_TEXTURES; i++ )
      pTexs->push_back(NULL);

}

void Object::reset(){
    vBuf->clear();
    iBuf->clear();
}

void Object::clearIndices(){
    iBuf->clear();
}
int Object::getVertexCount(){
    return vBuf->size();
}

Object::~Object(){
  delete vBuf;
  delete iBuf;

  for(size_t i = 0; i < pTexs->size(); i++)
  {
    delete pTexs->at(i);
  }

  delete pTexs;
  delete[] pProps;

  if( glVBuf ){
    std::cout<<"deleting vertex buf: "<<glVBuf;
    glDeleteBuffers( 1, &glVBuf );
  }

  if( glIBuf ){
    std::cout<<"deleting index buf: "<<glIBuf;
    glDeleteBuffers( 1, &glIBuf );
  }
}

glm::vec4 Object::getProperty( int index ){
  return pProps[index];
}

void Object::setProperty( int index, glm::vec4 prop ){
  pProps[index] = prop;
}

int Object::addVertex( glm::vec3 pos, glm::vec2 texColor ){
    addVertex( pos, texColor, glm::vec3(0.0,0.0,0.0) );
}

int Object::addVertex( glm::vec3 pos, glm::vec2 texColor, glm::vec3 normal ){
    Vertex v;
    v.pos = pos;
    v.texColor = texColor;
    v.normal = normal;
    vBuf->push_back( v );
    return vBuf->size()-1;
}

int Object::addVertex( Vertex v ){
  vBuf->push_back( v );
  return vBuf->size()-1;
}

// This function also autocomputes normal for this surface and aggregates with the three vertices. Designed for Goraud-shading.

void Object::setDrawMode( GLenum drawMode ){
  this->glDrawMode = drawMode;
}


glm::vec3 computeNormal( glm::vec3 va, glm::vec3 vb, glm::vec3 vc ){
    glm::vec3 dab = glm::vec3( va - vb );
    glm::vec3 dbc = glm::vec3( vb - vc );

    glm::vec3 normal = glm::normalize( glm::cross( dab, dbc ) );
}

int Object::addSurfaceV( Vertex a, Vertex b, Vertex c ){
  return addSurfaceI( addVertex(a), addVertex(b), addVertex(c) );
}

int Object::addSurfaceVN( Vertex a, Vertex b, Vertex c ){
  glm::vec3 normal = glm::cross((a.pos - b.pos),(b.pos - c.pos));
  a.normal = b.normal = c.normal = normal;
  return addSurfaceV( a, b, c );
}

int Object::addSurfaceI( int a, int b, int c ){
    iBuf->push_back( a );
    iBuf->push_back( b );
    iBuf->push_back( c );

    /*glm::vec3 normal = computeNormal( vBuf->at(a), vBuf->at(b), vBuf->at(c) );

    vBuf->at(a).normal += normal;
    vBuf->at(b).normal += normal;
    vBuf->at(c).normal += normal;*/

    return iBuf->size()/3 - 1;
}

int Object::getIndexCount(){
  return iBuf->size();
}

void Object::glSetTexture( int i, const std::string& filename ){
  pTexs->at(i) = new FileTexture( filename, "RGBA", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE );
  //(pTexs->at(i))->glInit();
  //(pTexs->at(i))->load( filename );
}

void Object::glSetTexture( int i, Texture* pTex ){
  pTexs->at(i) = pTex;
}

void Object::glTransfer(){

    glGenBuffers( 1, &glVBuf );
    glGenBuffers( 1, &glIBuf );
    glGenVertexArrays( 1, &glArrayDef );

    glBindVertexArray( glArrayDef );
    // Bind the vertex & index buffers.
    glBindBuffer( GL_ARRAY_BUFFER, glVBuf );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIBuf );
    // Send data to bound buffers.
    glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex) * vBuf->size(), vBuf->data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * iBuf->size(), iBuf->data(), GL_STATIC_DRAW );

    // Bind the array definition.
    // Set array attributes.
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( const GLvoid*)0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( const GLvoid* )12 );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), ( const GLvoid* )20 );


}

// Setup state for draw.
void Object::glBind(){
    glBindVertexArray( glArrayDef );
    glBindBuffer( GL_ARRAY_BUFFER, glVBuf );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIBuf );
}

void Object::glDraw(){
  this->glBind();
  for( int i = 0; i < MAX_TEXTURES; i++ ){
    if( pTexs->at(i) != NULL )
      (pTexs->at(i))->glActivate( GL_TEXTURE0 + i );
  }
  glDrawElements( this->glDrawMode, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0 );
}


ScreenQuad::ScreenQuad(){
  Object::addSurfaceVN(
    Vertex( glm::vec3( -1.0, -1.0, 0.0 ), glm::vec2( 0.0, 0.0 ) ),
    Vertex( glm::vec3(  1.0, -1.0, 0.0 ), glm::vec2( 1.0, 0.0 ) ),
    Vertex( glm::vec3( -1.0,  1.0, 0.0 ), glm::vec2( 0.0, 1.0 ) )
  );

  Object::addSurfaceVN(
    Vertex( glm::vec3(  1.0,  1.0, 0.0 ), glm::vec2( 1.0, 1.0 ) ),
    Vertex( glm::vec3( -1.0,  1.0, 0.0 ), glm::vec2( 0.0, 1.0 ) ),
    Vertex( glm::vec3(  1.0, -1.0, 0.0 ), glm::vec2( 1.0, 0.0 ) )
  );

  Object::setDrawMode( GL_TRIANGLES );
}

glm::mat4 Object::getWorldMatrix(){
  return mWorld;
}

void Object::setWorldMatrix(glm::mat4 m){
  mWorld = m;
}

// Need to mke the return type const to prevent modificationsa.
void Object::getIndexList( std::vector<int>** ppIndex ){
  *ppIndex = iBuf;
}
void Object::getVertexList( std::vector<Vertex>** ppVertex ){
  *ppVertex = vBuf;
}

void Object::getSurface( int i, Vertex* v1, Vertex* v2, Vertex* v3 ){

  std::cout<<"Surf at:"<<i<<endl;fflush(stdout);
  if( i < getIndexCount()/3 ){
    *v1 = vBuf->at(iBuf->at(3*i));
    *v2 = vBuf->at(iBuf->at(3*i + 1));
    *v3 = vBuf->at(iBuf->at(3*i + 2));
  }

}

int LinearObject::addExtension( Vertex v ){
  int i = addVertex( v );
  if( i != 0 )
    return addLineI( i - 1, i );

  return 0;
}

int LinearObject::closePoly(){
  int i = getIndexCount();
  return addLineI( 0, i - 1 );
}

LinearObject::LinearObject(){
  Object::setDrawMode( GL_LINES );
}

int LinearObject::addLineI( int i1, int i2 ){
  iBuf->push_back( i1 );
  iBuf->push_back( i2 );
  return iBuf->size()/2 - 1;
}

int LinearObject::addLineV( Vertex v1, Vertex v2 ){
  return addLineI( Object::addVertex(v1), Object::addVertex(v2) );
}
