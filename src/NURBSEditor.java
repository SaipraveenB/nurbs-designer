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

import javax.swing.JLabel;


public class NURBSEditor extends JFrame {

	private JPanel contentPane;
	private JTextField textField;
	private ArrayList<Point> ptList;
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
	
	class Point{
		
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
		
	}
	/**
	 * Create the frame.
	 */
	public NURBSEditor() {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 450, 300);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		contentPane.setLayout(new BorderLayout(0, 0));
		setContentPane(contentPane);
		
		
		final JPanel panel = new JPanel();
		contentPane.add(panel, BorderLayout.CENTER);
		
		JList list = new JList(){
			public Dimension getPreferredSize() {
				return panel.getSize();
			};
		};
		panel.add(list);
		
		LinkedList<String> ll = new LinkedList<String>();
		
		ptList = new ArrayList<Point>();
		list.setListData(ptList.toArray());
		
		JPanel panel_1 = new JPanel();
		contentPane.add(panel_1, BorderLayout.SOUTH);
		panel_1.setLayout(new GridLayout(0, 5, 0, 0));
		
		textField = new JTextField();
		panel_1.add(textField);
		textField.setColumns(10);
		
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
				
				ptList.add( new Point(x, y, w) );
				listGUI.setListData(ptList.toArray());

			}
		});
		
		panel_1.add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("-");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Point p = (Point)listGUI.getSelectedValue();
				ptList.remove(p);
				
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
				
				ptList.remove( p );
				ptList.add(index + 1, p);
				
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
				
				listGUI.setListData(ptList.toArray());
				listGUI.setSelectedIndex(index - 1);
			}
		});
		
		
		panel_1.add(btnNewButton_2);
		
	}

	
	   
}
