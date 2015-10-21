import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;


public class MouseDragHandlerImpl implements MouseDragHandler {
	
	public ArrayList<Point> ptList;
	public float radius;
	
	public Point current; 
	public int index;
	
	public NURBSEditor editor;
	
	public float rayToSphere( Point p, Point ray, Point c ){
		Point v1 = new Point( c.x - ray.x - p.x, c.y - ray.y - p.y, c.z - ray.z - p.z, 1.0f);
		Point v2 = new Point( c.x - p.x, c.y - p.y, c.z - p.z, 1.0f);
		
		Point fin = new Point( ( v1.y * v2.z - v1.z * v2.y), ( v1.z * v2.x - v1.x * v2.z ), ( v1.x * v2.y - v1.y * v2.x ) , 1.0f );
		return (float)Math.sqrt( fin.x*fin.x + fin.y*fin.y + fin.z*fin.z ); 
	}
	
	private Point detectHit(Point p, Point ray){
		
		final Point p0 = p;
		final Point ray0 = ray;
		Comparator<Point> c = new Comparator<Point>(){
			
			
			@Override
			public int compare(Point p1, Point p2) {
				// TODO Auto-generated method stub
				return (int) Math.signum( Math.abs( rayToSphere( p0, ray0, p1 ) ) - Math.abs( rayToSphere( p0, ray0, p2 ) ) );
			}
			
		};
		ArrayList<Point> list = new ArrayList<Point>( ptList );
		Collections.sort( list, c );
		
		Iterator<Point> i = list.iterator();
		while( i.hasNext() ){
			Point p1 = i.next();
			
			if( Math.abs( rayToSphere( p, ray, p1 ) ) < radius )
				return p1;
		}
		return null;
	}
	
	public void zeroCorrect( Point n ){
		if( n.x == 0 )
			n.x += 0.00001f;
		if( n.y == 0 )
			n.y += 0.00001f;
		if( n.z == 0 )
			n.z += 0.00001f;
		
	}
	@Override
	public void onDrag(Point p, Point r, Point n ) {
		// TODO Auto-generated method stub
		/*if( current != null ){
			current.x = p.x;
			current.y = p.y;
		}*/
		if( current == null ){
			return;
		}
		Point a = current;
		
		zeroCorrect( n );
		double t = ( ( - p.x + current.x )*n.x + ( - p.y + current.y )*n.y + ( - p.z + current.z )*n.z )/( r.x*n.x + r.y*n.y + r.z*n.z );
		
		//System.out.println( "parameter" + t );
		
		Point f = new Point( 0.0f, 0.0f, 0.0f, 1.0f );
		f.x = p.x + t * r.x;
		f.y = p.y + t * r.y;
		f.z = p.z + t * r.z;
		
		current.x = f.x;
		current.y = f.y;
		current.z = f.z;
		
		editor.updateResult();
		//editor.updateList();
		//editor.selectInList(index);
		
		//System.out.println("Drag to:  " + f);

	}
	
	public MouseDragHandlerImpl( NURBSEditor e ){
		editor = e;
	}

	@Override
	public void onSelect(Point p, Point ray) {
		// TODO Auto-generated method stub
		Point x = detectHit(p, ray);
		if( x == null )
			return;
		index = ptList.indexOf(x);
		current = ptList.get(index);
		
		//editor.selectInList(index);
		System.out.println("Detected select at: " + x);
	}
	@Override
	public void onReset() {
		// TODO Auto-generated method stub
		if( current != null )
			current = null;
	}
	@Override
	public void setPointGroup(ArrayList<Point> ptList) {
		// TODO Auto-generated method stub
		this.ptList = ptList;
	}
	
	@Override
	public void setRadius(float radius) {
		// TODO Auto-generated method stub
		this.radius = radius;
	}

}
