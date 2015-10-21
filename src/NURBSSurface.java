import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;


public class NURBSSurface{
	/*	double[] weight;
	double[] xCordinates;
	double[] yCordinates;
	double[] zCordinates;
	*/
	ArrayList<ArrayList<Point>> coordinates;
	ArrayList<ArrayList<Point>> curve;
	int numSamples;
	
	Spline splineFactory;
	
	public int getNumSamples() {
		return numSamples;
	}
	public void setNumSamples(int numSamples) {
		this.numSamples = numSamples;
	}
	
	@SuppressWarnings("unchecked")
	public NURBSSurface(int order,int cps)
	{
		splineFactory = new Spline(order,cps);
		/*	
		this.weight = new double[cps];
		this.xCordinates = new double[cps];
		this.yCordinates = new double[cps];
		this.zCordinates = new double[cps];
*/	
		this.coordinates = new ArrayList<ArrayList<Point>>();
		this.curve = new ArrayList<ArrayList<Point>>();
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
		splineFactory.getKnotvectors(knt);
	}
	public void addCoordinates( ArrayList<ArrayList<Point>> coords)
	{
		for( int i = 0;i < coords.size(); i++ ){
			this.coordinates.add( new ArrayList<Point>( coords.get(i) ) );
		}
	}
	public boolean NurbsCurve()
	{
		double nik,nij;

		double sum,Qsumx,Qsumy,Qsumz;
		//BufferedWriter writer = new BufferedWriter(new FileWriter("output.dat"));

		//BufferedWriter curve = new BufferedWriter(new FileWriter("curve.dat"));
		for(int i = 1;i<  numSamples;i++)
		{
			

			for(int k = 1;k < numSamples;k++)
			{	
				sum = 0.000;
				Qsumx = 0.000;
				Qsumy = 0.000;
				Qsumz = 0.000;
				this.curve.add( new ArrayList<Point>() );
				for(int x  = 0 ;x < splineFactory.controlpoints;x++)
				{	
					for( int y = 0; y < splineFactory.controlpoints; y++ ){

						nik = splineFactory.basisSpline( x, splineFactory.order,(double) k/numSamples);
						nij = splineFactory.basisSpline( y, splineFactory.order,(double) i/numSamples);

						Point p = this.coordinates.get(y).get(x);
						
						sum = sum + nik * nij * p.wt;


						Qsumx = Qsumx + nik * nij * p.weightedX();
						Qsumy = Qsumy + nik * nij * p.weightedY();
						Qsumz = Qsumz + nik * nij * p.weightedZ();

					}
				}

				this.curve.get(i-1).add( new Point(Qsumx/sum,Qsumy/sum,Qsumz/sum,1.00));

			}

		}

		//curve.flush();
		//curve.close();
		//writer.flush();
		//writer.close();



		return true;
	}

	
	
}
