package sortingAlgorithms;

public class SortingNow {
		
	public static void main(String[] args){
		
		// The length and maximum number of the array shall be set here.
		int arrayLength = 3000;
		int arrayMaxNum = 5000;
		int[] inputArray;
		
		// We generate a random array store and sort it with insert sort, then store the sorted array, too
		inputArray = new GenerateArray(arrayLength,arrayMaxNum).generatedArray;
		new InsertSort(inputArray);
		
		// Similarly, with quick sort
		inputArray = new GenerateArray(arrayLength,arrayMaxNum).generatedArray;
		new QuickSort(inputArray);
		
		// Merge sort
		inputArray = new GenerateArray(arrayLength,arrayMaxNum).generatedArray;
		new MergeSort(inputArray);
		
		// Heap Sort
		inputArray = new GenerateArray(arrayLength,arrayMaxNum).generatedArray;
		new HeapSort(inputArray);
	}

}
