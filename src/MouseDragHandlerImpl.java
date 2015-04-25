import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;


public class MouseDragHandlerImpl implements MouseDragHandler {
	
	public ArrayList<Point> ptList;
	public float radius;
	
	public Point current; 
	public int index;
	
	private Point detectHit(Point p){
		Comparator<Point> c = new Comparator<Point>(){

			@Override
			public int compare(Point p1, Point p2) {
				// TODO Auto-generated method stub
				return (int)Math.signum(p1.distanceTo(p2));
			}
			
		};
		ArrayList<Point> list = new ArrayList<Point>( ptList );
		Collections.sort( list, c );
		
		Iterator<Point> i = list.iterator();
		while( i.hasNext() ){
			Point p1 = i.next();
			if( p1.distanceTo(p) < radius )
				return p1;
		}
		return null;
	}
	@Override
	public void onDrag(Point p) {
		// TODO Auto-generated method stub
		if( current != null ){
			current.x = p.x;
			current.y = p.y;
		}
		System.out.println("Drag to:  " + p);

	}

	@Override
	public void onSelect(Point p) {
		// TODO Auto-generated method stub
		Point x = detectHit(p);
		if( x == null )
			return;
		current = ptList.get(ptList.indexOf(x));
		System.out.println("Detected select at: " + p);
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
