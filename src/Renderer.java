import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;



public class Renderer implements Runnable {
	
	private ArrayList<ArrayList<Point>> pList;
	private boolean changed;
	private OutputStream serviceOut;
	private InputStream serviceIn;
	private PrintWriter serviceWriter;
	private Scanner serviceScanner;
	
	private MouseDragHandler handler;
	
	private int ptListPtr;
	
	public MouseDragHandler getHandler() {
		return handler;
	}


	public void setHandler(MouseDragHandler handler) {
		this.handler = handler;
	}
	private Process service;
	
	
	public Renderer( ){
		pList = new ArrayList<ArrayList<Point>>();
		changed = false;
		ptListPtr = 0;
	}
	
	
	public void initialize() throws IOException{
		service = Runtime.getRuntime().exec("./nurbs/mygl");
		serviceIn = service.getInputStream();
		serviceOut = service.getOutputStream();
		serviceWriter = new PrintWriter( serviceOut );
		serviceScanner = new Scanner( serviceIn );
	}
	
	public int trackList( ArrayList<Point> al ){
		pList.add(al);
		return pList.size() - 1;
	}
	
	public void stateChanged(){
		changed = true;
	}
	
	public void terminate(){
		service.destroy();
	}
	
	public String makeChangeString( int x ){
		String s = " " + pList.get(x).size();
		for( int i = 0; i < pList.get(x).size(); i++ ){
			s += " " + pList.get(x).get(i).x + " " + pList.get(x).get(i).y + " " + pList.get(x).get(i).z;
		}
		
		return s;
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		System.out.println("Started Comms with renderer");
		try {
			while( true ){
				byte[] b = new byte[10];
				//char a = (char)serviceIn.read();
				String input = serviceScanner.nextLine();
				System.out.println("received: " + input);
					// Requesting response.
					System.out.println("Wait signal received, processing");
					ArrayList<Point> pList = this.pList.get(ptListPtr);
					if( !changed || pList.size() < 2 ){
						System.out.println("Data Unchanged");
						
						//Thread.sleep(10);
						serviceOut.write((int)'B');
						serviceOut.write((int)'\n');
						serviceOut.flush();
					}else{
						synchronized( pList ){
							String outString = "A " + ptListPtr + makeChangeString( ptListPtr );
							serviceWriter.println( outString );
							serviceWriter.flush();
							System.out.println("Out: " + outString);

							ptListPtr++;

							if( ptListPtr >= this.pList.size() ){
								System.out.println("All buffers transmitted");
								changed = false;
								ptListPtr = 0;
							}

						}
					}
				
				//String input = "" + a;
				System.out.println( input );
				String[] instr = input.split(" ");
				
				if( instr[0].equals("Y") || instr[0].equals("Z") ){
					System.out.println( input );
					input.split(" ");
					float val[] = new float[9];
					for( int i = 0; i < 9; i ++ ){
						val[i] = Float.valueOf( instr[i + 1] );
					}
					
					if( instr[0].equals("Y") )
						handler.onSelect(new Point( val[0], val[1], val[2], 1.0f ), new Point( val[3], val[4], val[5], 1.0f ) );
					else{
						handler.onDrag(new Point( val[0], val[1], val[2], 1.0f ), new Point( val[3], val[4], val[5], 1.0f ), new Point( val[6], val[7], val[8], 1.0f ));
						this.stateChanged();
					}
				}else{
					handler.onReset();
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;

		}
	}

}
