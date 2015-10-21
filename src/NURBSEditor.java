import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.util.ArrayList;
import java.util.LinkedList;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JList;
import javax.swing.JButton;
import javax.swing.JTextField;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.GridLayout;
import java.io.IOException;

import javax.swing.JLabel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSpinner;


public class NURBSEditor extends JFrame {

	private JPanel contentPane;
	private ArrayList<Point> ptList;
	private ArrayList<Point> nurbs;
	private ArrayList<ArrayList<Point>> ptMatrix;
	private ArrayList<ArrayList<Point>> nurbsMatrix;
	
	public Renderer renderer;
	private Thread renderThread;
	private MouseDragHandler dragHandler;
	private NURBSSurface nurbsFactory;
	private JSpinner samplesSpinner;
	private JSpinner orderSpinner;
	
	
	private ArrayList<Point> matrixToList( ArrayList<ArrayList<Point>> mat ){
		ArrayList<Point> al = new ArrayList<Point>();
		for( int i = 0; i < mat.size(); i ++ ){
			al.addAll( mat.get(i) );
		}
		return al;
	}
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					NURBSEditor frame = new NURBSEditor();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	
	public double[] getUniformKnots( int n ){
		double d[] = new double[n];
		for( int i = 0; i < n ; i++ ){
			d[i] = (( ((double)i) / (double)(n-1) ));
		}
		return d;
	}
	public void updateResult(){
		int samples = (Integer)samplesSpinner.getValue();
		int order = (Integer)orderSpinner.getValue();
		
		int sqSide = (int)Math.sqrt( ptList.size() );
		
		nurbsFactory = new NURBSSurface( 4, sqSide );
		nurbsFactory.addCoordinates( ptMatrix );
		
		System.out.println( getUniformKnots( 4 + sqSide ) );
		nurbsFactory.setNumSamples(40);
		double d[] = { 0, 0.0001, 0.0002, 0.33, 0.67, 0.99998, 0.99999, 1.0 };
		nurbsFactory.getKnotvectors( getUniformKnots( 4 + sqSide ) );
		nurbsFactory.NurbsCurve();
		
		nurbsMatrix.clear();
		nurbsMatrix.addAll( nurbsFactory.curve );
		nurbs.clear();
		nurbs.addAll(  matrixToList( nurbsMatrix ) );
	}
	/**
	 * Create the frame.
	 * @throws IOException 
	 */
	public NURBSEditor() throws IOException {
		
		System.out.println("Starting.....!!!!");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 450, 300);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		contentPane.setLayout(new BorderLayout(0, 0));
		setContentPane(contentPane);
		
		
		final JPanel panel = new JPanel();
		contentPane.add(panel, BorderLayout.CENTER);
		panel.setLayout(new BorderLayout(0, 0));
		
		LinkedList<String> ll = new LinkedList<String>();
		
		ptList = new ArrayList<Point>();
		nurbs = new ArrayList<Point>();
		ptMatrix = new ArrayList<ArrayList<Point>>();
		nurbsMatrix = new ArrayList<ArrayList<Point>>();
		
		JPanel panel_1 = new JPanel();
		contentPane.add(panel_1, BorderLayout.SOUTH);
		panel_1.setLayout(new GridLayout(0, 5, 0, 0));
		
		JScrollPane scrollPane = new JScrollPane();
		panel.add(scrollPane, BorderLayout.CENTER);
		renderer = new Renderer( );
		
		renderer.trackList( ptList );
		renderer.trackList( nurbs );

		renderer.initialize();
		
		orderSpinner = new JSpinner();
		panel_1.add(orderSpinner);
		orderSpinner.setValue(3);
		
		samplesSpinner = new JSpinner();
		panel_1.add(samplesSpinner);
		samplesSpinner.setValue(50);
		
		JButton btnTerminateRenderer = new JButton("Terminate Renderer");
		btnTerminateRenderer.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				renderer.terminate();
			}
		});
		contentPane.add(btnTerminateRenderer, BorderLayout.NORTH);
		
		dragHandler = new MouseDragHandlerImpl( this );
		renderer.setHandler( dragHandler );
		dragHandler.setPointGroup(ptList);
		dragHandler.setRadius(0.1f);
		
		renderThread = new Thread( renderer, "renderer" );
		System.out.println("starting renderer");
		renderThread.start();
		
		synchronized( ptList ){
			for( int i = 0; i < 6; i++ ){
				ptMatrix.add( new ArrayList<Point>() );
				for( int j = 0; j < 6; j++ ){
					ptMatrix.get(i).add( new Point( i/4.0, 0.0, j/4.0, 1.0 ) );
				}
			}
			//ptMatrix.get(2).add( 2, new Point( 2/3.0, 2/3.0, 0.5, 1.0 ) );
			ptList.addAll( matrixToList( ptMatrix ) );
			updateResult();
			renderer.stateChanged();
		}
		
	}
}
