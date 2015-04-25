import java.util.ArrayList;


public class Animator implements Runnable {
	
	public ArrayList<Point> al;
	public Renderer r;
	@Override
	public void run() {
		// TODO Auto-generated method stub
		while( true ){
			try {
				Thread.sleep(30);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			synchronized( al ){
				if( al.size() > 0 ){
						Point p = al.get(0);
						p.x += 0.001;
						r.stateChanged();
				}
				
			}
		}
	}
	
}
