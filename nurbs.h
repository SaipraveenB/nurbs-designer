#include "basis.h"

using namespace std;

namespace nurbs{
    class 2DPoint{

        long double x;
        long double y;
        long double weight;
        
        public:
        2DPoint( long double x, long double y );
        2DPoint( long double x, long double y, long double weight );

    }

    class 2DNURBSCurve{
        // Knot vector.
        vector<long double> *vKnots;

        // List of points.
        vector<2DPoint> *vPoints;

        // Bases list.
        vector<Basis*>* vBases;

        // Number of points in the NURBS curve.
        int iPointCount;


        public:
        2DNURBSCurve( int i ):i(iPointCount){
            vBases = new vector<Basis*>();
            // Finish initialization.
        }

        
        2DPoint at( long double u );
    };
};
