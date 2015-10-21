
#include "triangulator.h"

#include <stdio.h>
#include <iostream>

using namespace std;


Triangulator::Triangulator(){

}

void Triangulator::squareTriangulate( LinearObject* lobj ){
  lobj->clearIndices();
  int count = lobj->getVertexCount();
  int side = sqrt(count);
  for( int i = 0; i < side - 1; i++ ){
    for( int j = 0; j < side - 1; j++ ){
      lobj->addLineI( i*side + j, (i + 1)*side + j );
      lobj->addLineI( i*side + j, (i)*side + j + 1 );
    }
  }

  for( int i = 0;i < side - 1; i++ ){
    lobj->addLineI( i*side + side - 1, (i + 1)*side + side - 1 );
  }

  for( int j = 0;j < side - 1; j++ ){
    lobj->addLineI( (side - 1)*side + j, (side - 1)*side + j + 1 );
  }
}
