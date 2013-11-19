package algorithmsClass.MergeSort;

public class IterationMerge {
	
	private int length;
	private int maxNum;
	private int[] array;
	
	public IterationMerge(int Length,int MaxNum){
		length = Length;
		maxNum = MaxNum;
		array = randomArray(length,maxNum);
		mergeSort(array);
		for(int p=0;p<Length;p++){
			System.out.println(array[p]+"\n");
		}
	}
	
	
	private void mergeSort(int[] inputArray) {
		int[] tempArray = new int[inputArray.length];
		int s = 1;
		while(s< inputArray.length ){
			mergePass(inputArray,tempArray,s);
			s=2*s;
			mergePass(tempArray,inputArray,s);
			s=2*s;
		}	
	}

	private void mergePass(int[] inputArray, int[] tempArray, int s) {
		int i = 0;
		int j = 0;
		
		while(i <= inputArray.length-2*s){
			merge(inputArray,tempArray,i,i+s-1,i+2*s-1);
			i=i+2*s;
		}

		
		if(i+s < inputArray.length){
			merge(inputArray,tempArray,i,i+s-1,inputArray.length-1);
		}
		
		else{
			for(j=i;j<inputArray.length;j++){
				tempArray[j]=inputArray[i++];
			}
		}
		
	}


	private void merge(int[] a, int[] b, int l, int m, int r) {
		int i = l;
		int j = m+1;
		int k = l;
		while((i<=m)&&(j<=r)){
			if(a[i]<=a[j]){
				b[k++]=a[i++];
				if(i>m){
					for(int q=j;j<=r;j++){
						b[k++]=a[q];
					}
				}
			}
			else{
				b[k++]=a[j++];
				if(j>r){
					for(int q=i;q<=m;q++){
						b[k++]=a[q];
					}
				}
			}				
		}
		
	}

	private int[] randomArray(int length, int maxNum){
		
		// we can set the length and maximum number of the array
		int[] Array = new int[length];
		for(int i=0;i<length;i++){
			Array[i]=(int)(Math.random()*maxNum);
		}
		// we get a random array of integers only
		return Array;
	}
}
