package sortingAlgorithms;

public class GenerateArray {
	
	int arrayLength;
	int arrayMaxNum;
	int[] generatedArray;
	
	// This constructor will take the length and the maximum number of the integer array wanted
	// and generate a random array as requested
	public GenerateArray(int Length,int MaxNum){
		arrayLength = Length;
		arrayMaxNum = MaxNum;
		System.out.println("***The length of the random array generated is " + arrayLength + ".");
		System.out.println("***The maximum number of the random array generated is " + arrayMaxNum + ".");
		generatedArray = randomArray(arrayLength,arrayMaxNum);
	}
		
	public int[] randomArray(int Length, int MaxNum){
		// we can set the length and maximum number of the array
		int[] Array = new int[Length];
		for(int i=0;i<arrayLength;i++){
			Array[i]=(int)(Math.random()*MaxNum);
		}
		// we get a random array of integers only
		return Array;
	}
	
}