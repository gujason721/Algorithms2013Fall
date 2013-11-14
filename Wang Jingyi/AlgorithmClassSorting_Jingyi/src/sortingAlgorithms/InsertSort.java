package sortingAlgorithms;

import java.io.File;


public class InsertSort {
	// This method will take the unsorted array as the input and make it sorted with insert sorting
	// and write both of them into .txt files 
	private int[] array;
	private int[] sortedArray;
	
	public InsertSort(int[] unsortedArray){
		
		array = unsortedArray;
		
		// Write the unsorted array into the .txt
		File insertArray = new File("E:\\Jy_insertSortArray.txt");
		new WriteToTxt(array,insertArray);
		
		// Sort the array using insert sorting and record the time
		long startTime = System.nanoTime();
		sortedArray = Sort(array);
		long endTime = System.nanoTime();
		
		// Write the sorted array into the .txt
		File insertedArray = new File("E:\\Jy_insertSortedArray.txt");
		new WriteToTxt(sortedArray,insertedArray);
		 
		System.out.println("***The generated random array has been sorted with insert sorting.");
		System.out.println("***Check hardware E for the stored arrays in .txt.");
		System.out.println("***The run time of insert sorting is " + (endTime-startTime) + " nanoseconds\n");
	}
	
	private int[] Sort(int[] array){
		for(int j=1;j<array.length;j++){
			for(int i=0;i<j;i++){
				if(array[i]>array[j]){
					int key=array[j];
					array[j]=array[i];
					array[i]=key;
				}
			}
		}		
		return array;		
	}
	
}
