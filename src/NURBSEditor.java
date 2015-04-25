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


public class NURBSEditor extends JFrame {

	private JPanel contentPane;
	private JTextField textField;
	private ArrayList<Point> ptList;
	private ArrayList<Point> nurbs;
	public Renderer renderer;
	private Thread renderThread;
	private MouseDragHandler dragHandler;
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
	
	
	/**
	 * Create the frame.
	 * @throws IOException 
	 */
	public NURBSEditor() throws IOException {
		
		
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
		
		JPanel panel_1 = new JPanel();
		contentPane.add(panel_1, BorderLayout.SOUTH);
		panel_1.setLayout(new GridLayout(0, 5, 0, 0));
		
		textField = new JTextField();
		panel_1.add(textField);
		textField.setColumns(10);
		
		
		
		JList list = new JList();
		list.setVisibleRowCount(100);
		//panel.add(list);
		list.setListData(ptList.toArray());
		
		JScrollPane scrollPane = new JScrollPane();
		panel.add(scrollPane, BorderLayout.CENTER);
		scrollPane.setViewportView(list);
		
		final JList listGUI = list;
		JButton btnNewButton = new JButton("+");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				//System.out.println("Hello?");
				String text = textField.getText();
				String[] comp = text.split(",");
				float x = Float.parseFloat(comp[0]);
				float y = Float.parseFloat(comp[1]);
				float w = Float.parseFloat(comp[2]);
				
				
				renderer.stateChanged();
				synchronized( ptList ){
					ptList.add( new Point(x, y, w) );
				}
				listGUI.setListData(ptList.toArray());

			}
		});
		renderer = new Renderer( );
		renderer.trackList( ptList );
		renderer.trackList( nurbs );

		renderer.initialize();
		
		panel_1.add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("-");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Point p = (Point)listGUI.getSelectedValue();
				synchronized( ptList ){
					ptList.remove(p);
				}
				
				renderer.stateChanged();
				listGUI.setListData(ptList.toArray());
			}
		});
		panel_1.add(btnNewButton_1);
		
		JButton button = new JButton(">");
		button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Point p = (Point)listGUI.getSelectedValue();
				int index = ptList.indexOf(p);
				
				if( index == ptList.size() - 1 )
					return;
				
				synchronized( ptList ){
					ptList.remove(p);
					ptList.add(index + 1, p);
				}
				
				renderer.stateChanged();
				listGUI.setListData(ptList.toArray());
				listGUI.setSelectedIndex(index + 1);
			}
		});
		panel_1.add(button);
		
		JButton btnNewButton_2 = new JButton("<");
		btnNewButton_2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Point p = (Point)listGUI.getSelectedValue();
				int index = ptList.indexOf(p);
				
				if( index == 0 )
					return;
				
				ptList.remove( p );
				ptList.add(index - 1, p);
				
				renderer.stateChanged();
				listGUI.setListData(ptList.toArray());
				listGUI.setSelectedIndex(index - 1);
			}
		});
		
		
		panel_1.add(btnNewButton_2);
		
		JButton btnTerminateRenderer = new JButton("Terminate Renderer");
		btnTerminateRenderer.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				renderer.terminate();
			}
		});
		contentPane.add(btnTerminateRenderer, BorderLayout.NORTH);
		
		dragHandler = new MouseDragHandlerImpl();
		renderer.setHandler( dragHandler );
		dragHandler.setPointGroup(ptList);
		dragHandler.setRadius(0.1f);
		
		renderThread = new Thread( renderer, "renderer" );
		System.out.println("starting renderer");
		renderThread.start();
		/*Animator anim = new Animator();
		anim.al = ptList;
		anim.r = renderer;
		System.out.println("starting animator");
		Thread animThread = new Thread( anim, "animator" );
		animThread.start();*/
		
		synchronized( ptList ){
			for( float i = 0.0f; i < 1; i+= 0.2f ){
				ptList.add( new Point( i, i*i, 1 ) );
			}
			listGUI.setListData(ptList.toArray());
			renderer.stateChanged();
		}
		
	}
}
