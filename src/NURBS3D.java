import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;


public class NURBS3D extends Spline {
/*	double[] weight;
	double[] xCordinates;
	double[] yCordinates;
	double[] zCordinates;
	*/
	ArrayList<Point> cordinates;
	ArrayList<Point> curve;
	
	private int numSamples;
	public NURBS3D(int order,int cps)
	{
		super(order, cps);
	/*	
		this.weight = new double[cps];
		this.xCordinates = new double[cps];
		this.yCordinates = new double[cps];
		this.zCordinates = new double[cps];
*/	
		this.cordinates = new ArrayList<Point>();
		this.curve = new ArrayList<Point>();
	}
/*	public void setWeights(double[] wts)
	{
		for(int i = 0;i< super.controlpoints;i++)
		{
			this.weight[i] = wts[i];
		}
	}
	*/
	public void getKnotvectors(double [] knt)
	{
		super.getKnotvectors(knt);
	}
	public void setCordinates(ArrayList<Point> cords)
	{
		this.cordinates.addAll(cords);
	}
	public boolean NurbsCurve()
	{
		double nik;
	
		double sum,Qsumx,Qsumy,Qsumz;
	try{
		BufferedWriter writer = new BufferedWriter(new FileWriter("output.dat"));
		
		BufferedWriter curve = new BufferedWriter(new FileWriter("curve.dat"));
		for(int i = 1;i<numSamples;i++)
		{
		    sum = 0.000;
			Qsumx = 0.000;
			Qsumy = 0.000;
			Qsumz = 0.000;
			
			writer.write(Double.toString((double) i/100));
			writer.write(" ");
			
			for(int j  = 0 ;j < super.controlpoints;j++)
			{	
					
				nik = super.basisSpline(j,super.order,(double) i/numSamples);
				
				sum = sum + nik*this.cordinates.get(j).wt;
				
				
				Qsumx = Qsumx + nik*this.cordinates.get(j).weightedX();
				Qsumy = Qsumy + nik*this.cordinates.get(j).weightedY();
				Qsumz = Qsumz + nik*this.cordinates.get(j).weightedZ();
				
				writer.write(Double.toString(nik));
				writer.write(" ");
			}	
				writer.newLine();
				curve.write(Double.toString(Qsumx/sum));
				curve.write(" ");
				curve.write(Double.toString(Qsumy/sum));
				curve.write(" ");
				curve.write(Double.toString(Qsumz/sum));
				curve.newLine();
				this.curve.add(i-1, new Point(Qsumx/sum,Qsumy/sum,Qsumz/sum,1.00));
		}
		
		curve.flush();
		curve.close();
		writer.flush();
		writer.close();


} catch (IOException e) {
	// TODO Auto-generated catch block
	e.printStackTrace();
}

		return true;
	}
	public ArrayList<Point> Curve()
	{
		return this.curve ;
	}
	public int getNumSamples() {
		return numSamples;
	}
	public void setNumSamples(int numSamples) {
		this.numSamples = numSamples;
	}
		
	
}
