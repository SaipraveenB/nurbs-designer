
public class Point{
		
		double x;
		double y;
		double z;
		double wt;
		
		public Point( double x, double y, double z, double wt ){
			this.x = x;
			this.y = y;
			this.z = z;
			this.wt = wt;
		}
		
		public String toString(){
			return "(" + x + "," + y + "," + z + ")#" + wt;
		}
		
		public double distanceTo( Point p ){
			return (double)Math.sqrt( (p.x - x)*(p.x - x) + ( p.y - y )*( p.y - y ) + ( p.z - z )*(p.z - z ) );
		}
		
		public double weightedX(){
			return this.x * this.wt;
		}
		
		public double weightedY(){
			return this.y * this.wt;
		}
		
		public double weightedZ(){
			return this.z * this.wt;
		}
	}
