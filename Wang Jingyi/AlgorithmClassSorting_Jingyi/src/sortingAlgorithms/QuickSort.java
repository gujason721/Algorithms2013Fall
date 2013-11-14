package sortingAlgorithms;

import java.io.File;

public class QuickSort {

	// This method will take the unsorted array as the input and make it sorted using quick sorting
	// and write both of them into .txt files 
	private int[] array;
	public QuickSort(int[] unsortedArray) {
		array = unsortedArray;
		
		
		// Write the unsorted array into the .txt
		File quickArray = new File("E:\\Jy_quickSortArray.txt");
		new WriteToTxt(array,quickArray);
				
		// Sort the array using insert sorting
		long startTime = System.nanoTime();
		Sort(array);
		long endTime = System.nanoTime();
		
				
		// Write the sorted array into the .txt
		File quickedArray = new File("E:\\Jy_quickSortedArray.txt");
		new WriteToTxt(array,quickedArray);
				 
		System.out.println("***The generated random array has been sorted with quick sorting.");
		System.out.println("***Check hardware E for the stored arrays in .txt.");
		System.out.println("***The run time of quick sorting is " + (endTime-startTime) + " nonaseconds\n");
	}
	
	
	public static int[] Sort(int[] numbers) {
        return Sort(numbers, 0, numbers.length - 1);
    }
 
    public static int[] Sort(int[] numbers, int low, int high) {
        if (low < high) {
            int pivotpos = partition(numbers, low, high);
            Sort(numbers, low, pivotpos - 1);
            Sort(numbers, pivotpos + 1, high);
        }
        return numbers;
    }
    
    // Partition 
    private static int partition(int[] numbers, int low, int high) {
        int index = low + 1;
        int pivotpos = low;
        int pivot = numbers[pivotpos];
 
        while(index < high + 1) {
            if (numbers[index] < pivot) {
                pivotpos++;
 
                if (pivotpos != index) {
                    swap(numbers, index, pivotpos);
                }
            }
            index++;
        }
        swap(numbers, low, pivotpos);
        return pivotpos;
    }
 
    private static void swap(int[] numbers, int a, int b) {
        int tmp = numbers[a];
        numbers[a] = numbers[b];
        numbers[b] = tmp;
    }
	
	 
}
