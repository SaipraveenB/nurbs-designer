import java.util.ArrayList;


public interface MouseDragHandler {
	public void onDrag( Point p, Point point, Point point2 );
	public void onSelect( Point p, Point point );
	public void onReset();
	public void setPointGroup( ArrayList<Point> ptList );
	public void setRadius( float radius );
}
