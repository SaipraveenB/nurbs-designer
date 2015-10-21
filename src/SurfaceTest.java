import java.util.*;

public class SurfaceTest {
	public static void main( String args[] ){
		NURBSSurface surf = new NURBSSurface( 3, 4 );
		
		ArrayList<ArrayList<Point>> plist = new ArrayList<ArrayList<Point>>();
		for( int i = 0; i < 4; i++ ){
			plist.add( new ArrayList<Point>() );
			for( int j = 0; j < 4; j++ ){
				plist.get(i).add( new Point( i/3.0, j/3.0 ,0.0, 1.0 ) );
			}
		}
		plist.get(2).add( 2, new Point( 2/3.0, 2/3.0, 0.5, 1.0 ) );
		
		double d[] = { 0.0, 0.0001, 0.0002, 0.5, 0.9998, 0.9999, 1.0 };
		surf.getKnotvectors( d );
		surf.addCoordinates(plist);

		surf.NurbsCurve();
		System.out.println( surf.curve );
	}
}
