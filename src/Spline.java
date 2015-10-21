

public class Spline {
	double[]  knotvectors;
	int order;
	int controlpoints;
	public Spline(int order,int controlpoints)
	{
		this.order = order;
		this.controlpoints = controlpoints;
		this.knotvectors = new double[order + controlpoints];
	}
	public void getKnotvectors(double[] x)
	{
		for(int i = 0 ;i< this.order+ this.controlpoints;i++)
		{
			this.knotvectors[i] = x[i];
		}
	}
	
	public double basisSpline(int i,int k , double d)
	{
		
		if(k == 1)
		{
			if( d >= this.knotvectors[i] && d < this.knotvectors[i+1])
			{
				return 1.00 ;
			}
			else
			{
				return 0.00;
			}
		}
		else
		{
			

			if(d >= this.knotvectors[i] && d < this.knotvectors[i+k])
			{
				return this.functionF(i, k, d)*basisSpline(i,k-1,d) 
					+ this.functonG( i+1, k, d)*basisSpline(i+1,k-1,d);
		
			}
			else
			{
				return 0.000;
			}
		}	
	}
	private double functionF(int i,int k,double d)
	{
		return (d - this.knotvectors[i])/(this.knotvectors[i+ k-1] - this.knotvectors[i]) ;
	}
	private double functonG(int i, int k, double d)
	{
		return (this.knotvectors[i + k -1] -d)/(this.knotvectors[i+ k-1] - this.knotvectors[i]) ;
	}

}
