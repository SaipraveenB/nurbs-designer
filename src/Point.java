
public class Point{
		
		float x;
		float y;
		float wt;
		
		public Point( float x, float y, float wt ){
			this.x = x;
			this.y = y;
			this.wt = wt;
		}
		
		public String toString(){
			return "(" + x + "," + y + ")#" + wt;
		}
		
		public float distanceTo( Point p ){
			return (float)Math.sqrt( (p.x - x)*(p.x - x) + ( p.y - y )*( p.y - y ) );
		}
	}
