import java.util.ArrayList;


public interface MouseDragHandler {
	public void onDrag( Point p );
	public void onSelect( Point p );
	public void onReset();
	public void setPointGroup( ArrayList<Point> ptList );
	public void setRadius( float radius );
}
