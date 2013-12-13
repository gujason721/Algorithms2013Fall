 /*This class is to wrap up the result of SVM to predicates
 @ author: Wang Jingyi
 Last modified: 9/12/2013*/


import libsvm.*;

public class svm_wapper{
	
	static double bias;
	static int pSize;
	static double mean;
	static svm_model model ;
	static double[] wVec;
	static int[] index;
	static int loopNum = 0;
	
	// constructor
	public svm_wapper(svm_model model){
		this.model = model;
		pSize = TestFindMax.length+1;
		wVec = new double[pSize];
		index = new int[pSize];
		for(int i=0;i<pSize;i++){
			index[i]=i;
		}
	}
	
	// update model after every level of svm
	public void updateInfo(svm_model model){
		this.model = model;
	}
	
	// Caculate the threshold for the stopping condition
	// of finding significant varibles
	public static double calThre(double[] arr, double bias){
		double sum = 0;
		for(int i=0;i<arr.length;i++){
			mean = mean + Math.abs(arr[i]);
		}
		mean = (mean+bias)/(arr.length+1);
		for ( int i=0; i<arr.length; i++ ) 
		{ 
		final double v = Math.abs(arr[i]) - mean;  
		sum += v * v; 
		} 
		sum += (Math.abs(bias) - mean)*(Math.abs(bias) - mean);
		// set the threshold to be 0.45*deviation below the mean
		// it's a result of trying, and I'll do more systematic
		// analysis later to choose the best threshold.
		return mean-0.45*Math.sqrt(sum/(arr.length+1));
		
	}
	
	// Check if we have found the final vector for co-efficient
	public static boolean isFinal(double[] arr){
		double thre = calThre(arr,bias);
		for(double vec : arr ){
			if(Math.abs(vec)<thre){
				return false;
			}
		}
		return true;
	}
	
	// select coefficients with more significance and update the data information
	public static void selNewVec(double[] arr){
		
		pSize = 0;
		double threshold = calThre(wVec,bias);
		for(double vec : arr){
			if(Math.abs(vec) >= threshold){	
				pSize++;
			}
		}
		wVec = new double[pSize];
		index = new int[pSize];
		int i = 0;
		for(int j=0;j<arr.length;j++){
			if(Math.abs(arr[j]) >= threshold){	
				index[i] = j;
				i++;
			}
		}	
		loopNum++;
	}
	
	// This method caculate the co-efficients for the divider
	// from the output of libSVM
	public void generateDivider(svm_model model){
		
		if(model.nr_class!=2){
			System.out.println("Can not be binary divided...");
		}
		
		
		int[] sIndex = new int[model.nr_class];	
		for (int i = 1; i < model.nr_class; i++) {
		      sIndex[i] = sIndex[i - 1] + model.nSV[i - 1];
		}
		
		int p = 0;
	    for (int i = 0; i < model.nr_class; i++) {
	    	
	      for (int j = i + 1; j < model.nr_class; j++) {
	        int si = sIndex[i];
	        int sj = sIndex[j];
	        double[] coef1 = model.sv_coef[j - 1];
	        double[] coef2 = model.sv_coef[i];

	        for (int k = 0; k < model.nSV[i]; k++) {
	          for (int m = 0; m < pSize; m++) {
	            wVec[m] += coef1[si + k] * model.SV[si + k][m].value;
	          }
	        }

	        for (int k = 0; k < model.nSV[j]; k++) {
	          for (int m = 0; m < pSize; m++) {
	            wVec[m] += coef2[sj + k] * model.SV[sj + k][m].value;
	          }
	        }

	        bias = model.rho[p];
	        p++;
	      }
	      
	    }
			
	}

	public void printPredicate() {
		System.out.println("The " + (loopNum+1) + " round of generated predicate is: ");
		StringBuilder pdString = new StringBuilder();
		for(int i=0;i<pSize;i++){
			String app = String.format("(%f)*x[%d]+",wVec[i],index[i]);
			pdString.append(app);
		}
		pdString.append(String.format("(%f) = 0", bias));
		System.out.printf(pdString.toString());
	}
	
}
