package sortingAlgorithms;

import java.io.File;

public class MergeSort {
	
	// This method will take the unsorted array as the input and make it sorted with merge sorting
	// and write both of them into .txt files
	
	private int[] array;
	private int[] sortedArray;
	
	public MergeSort(int[] inputArray) {
		array = inputArray;
		// Write the unsorted array into the .txt
		File mergeArray = new File("E:\\Jy_mergeSortArray.txt");
		new WriteToTxt(array,mergeArray);
				
		// Sort the array using merge sorting
		long startTime = System.nanoTime();
		sortedArray = mergeSort(array);
		long endTime = System.nanoTime();
				
		// Write the sorted array into the .txt
		File mergedArray = new File("E:\\Jy_mergeSortedArray.txt");
		new WriteToTxt(sortedArray,mergedArray);
				 
		System.out.println("***The generated random array has been sorted with merge sorting.");
		System.out.println("***Check hardware E for the stored arrays in .txt.");
		System.out.println("***The run time of merge sorting is " + (endTime-startTime) + " nanoseconds\n");
	}
	
	private static int[] mergeSort(int[] array){
		
		// Base case for the recursion
		if (array.length<=1){
			return array;
		}
		
		int half = array.length/2;
		int[] arrayFirstHalf = new int[half]; 
		for(int i=0;i<half;i++){
			arrayFirstHalf[i]=array[i];
		}
		int[] arraySecondHalf = new int[array.length-half];
		for(int j=0;j<array.length-half;j++){
			arraySecondHalf[j]=array[j+half];
		}
				
		arrayFirstHalf = mergeSort(arrayFirstHalf);
	    arraySecondHalf = mergeSort(arraySecondHalf);
		
		return merge(arrayFirstHalf,arraySecondHalf);
	}

	private static int[] merge(int[] firstArray, int[] secondArray) {
		int i = 0;
		int firstHead = 0;
		int secondHead = 0;
		int current = 0;
		int[] result = new int[firstArray.length+secondArray.length];
        
		for(i = 0; i< result.length; i++ ){
				result[current] = getMin(firstArray[firstHead],secondArray[secondHead]);
				if(firstArray[firstHead]<=secondArray[secondHead]){
					firstHead++;
					if(firstHead==firstArray.length){
						// The first half of sorted array is empty, concatenate the remaining second here
						System.arraycopy(secondArray, secondHead, result, current+1, result.length-current-1);
						break;
					}
				}
				else{
					secondHead++;
					if(secondHead==secondArray.length){
						// The second half of sorted array is empty, concatenate the remaining first half here
						System.arraycopy(firstArray, firstHead, result, current+1, result.length-current-1);
						break;
					}
				}
				current++;
        }
        return result;
	}

	private static int getMin(int i, int j) {
		if(i<=j){
			return i;
		}
		else{
			return j;
		}
	}
	
}
