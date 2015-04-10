
#include <vector>
namespace nurbs{

    class Basis{
        // Knot Index
        int i;
        // Basis Order
        int n;

        std::vector<long double> *vKnots;
        
        Basis* bPlus;
        Basis* bMinus;

    public:
        Basis( int i, int n ):i(i),n(n){
            if( n != 0 ){
                bPlus = new Basis( i + 1, n - 1 );
                bMinus = new Basis( i, n - 1 );
            }else{
                bPlus = NULL;
                bMinus = NULL;
            }
        }

        long double at( long double u ){

            if( n == 0 ){
                return box( u );
            }

            long double Ni0 = bPlus->at(u);
            long double Ni1 = bPlus->at(u);
            
            vKnots = new vector<int>();

            // Calculate linear interpolation between Ni0 and Ni1 
            return 0.0;
        }

        long double box( long double u ){
            if( vKnots->at(i) < u && vKnots->at(i+1) >= u ){
                return 1.0f;
            }else{
                return 0.0f;
            }
        }

    }
}
