package AlgorithmsSUTD;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

/**
 * @author hans anderson
 *
 */
public class SortingAssignment {

	public static void main(String[] args) {
		boolean loadFromFile = true;
		boolean outputToFile = true;
	    boolean outputToConsole = true;
	    boolean printTimingDataToConsole = true;
		
		int inputLength;
		int[] input;
		long startTime, insertionTime, heapTime, mergeTime, quickTime;
		
		if (loadFromFile) {
			input = loadInputFromFile("input.txt");
			inputLength = input.length;
		} else { // generate random input for testing
			inputLength = 60000;
			int testRange = 50;
			input = new int[inputLength];

			java.util.Random rand = new java.util.Random();
			for (int i = 0; i < inputLength; i++) {
				int nextRandom = rand.nextInt() % testRange;
				input[i] = nextRandom;
			}
		}
		
		int[] testInsertion = new int[inputLength];
		int[] testMerge = new int[inputLength];
		int[] testHeap = new int[inputLength];
		int[] testQuick = new int[inputLength];
	    
		// copy the input (from file or random generator) to 4 separate arrays, one for each sorting algorithm
		for (int i=0; i < input.length; i++) 
			testInsertion[i] = testMerge[i] = testHeap[i] = testQuick[i] = input[i];
	    
	    // print out the unsorted data
	    if (outputToConsole) print(testInsertion," Unsorted");
	    
	    // insertion sort
	    startTime = System.nanoTime();
	    insertionSort(testInsertion);
	    insertionTime = System.nanoTime() - startTime;
	    if (outputToConsole) print(testInsertion,"Insertion");
	    
	    // merge sort
	    startTime = System.nanoTime();
	    mergeSort(testMerge);
	    mergeTime = System.nanoTime() - startTime;
	    if (outputToConsole) print(testMerge,    "    Merge");

	    // heap sort
	    startTime = System.nanoTime();
	    heapSort(testHeap);
	    heapTime = System.nanoTime() - startTime;
	    if (outputToConsole) print(testHeap,     "     Heap");
	    
	    // quick sort
	    startTime = System.nanoTime();
	    quickSort(testQuick);
	    quickTime = System.nanoTime() - startTime;
	    if (outputToConsole) print(testQuick,    "    Quick");
	    
	    if (outputToFile){
	    	printToFile(testQuick,"output.txt");
	    }
	    
		if (printTimingDataToConsole) {
			System.out.println("\nTIMING IN NANOSECONDS");
			System.out.println(" Insertion: " + insertionTime);
			System.out.println("     Merge: " + mergeTime);
			System.out.println("      Heap: " + heapTime);
			System.out.println("     Quick: " + quickTime);
		}
	}

	public static void quickSort(int [] A){
		quickSort(A,0,A.length-1);
	}
	
	public static void quickSort(int []A, int start, int end){
		//if the list has only a small number of elements, use insertion sort instead of recursing
		if (end - start < 30) {
			insertionSort(A,start,end);
			return;
		}
		
		// select pivot from the middle of the list
		int pivotValue = A[start + (end - start)/2];  
		
		// partition
		int leftIdx = start;
		int rightIdx = end;
		while (leftIdx <= rightIdx) {
			while (A[leftIdx] < pivotValue) leftIdx++;
			while (A[rightIdx] > pivotValue) rightIdx--;
			if (leftIdx <= rightIdx) swap (A,rightIdx--,leftIdx++);
		}
		
		// sort on both sides of the partition
		quickSort(A,start,rightIdx);
		quickSort(A,leftIdx,end);
	}
	
	public static void heapSort(int [] A){
		// make heap
		for (int lastInHeap = 0; lastInHeap < A.length -1; lastInHeap++) heapInsertAtEnd(A,lastInHeap);
		
		// transfer from heap to sorted array
		for (int lastInHeap = A.length - 1; lastInHeap > 0;) {
			swap (A,lastInHeap,0); // put the largest element from the heap (0) at the end
			lastInHeap--; // shorten the heap
			heapInsertAtStart(A,lastInHeap);
		}
	}
	
	// insert the element at index 0 in A into the heap that ends at lastInHeap
	public static void heapInsertAtStart(int [] A, int lastInHeap){
		int newElement = 0;
		int largestChild = largestChildOf(A,newElement,lastInHeap);
		while(A[newElement] < A[largestChild])
	    {
			swap(A, newElement, largestChild);
			newElement = largestChild; // update the location of newElement
			largestChild = largestChildOf(A,newElement,lastInHeap);
		}
	}
	
	// insert the element at (lastInHeap + 1) into the heap stored in A
	public static void heapInsertAtEnd(int [] A, int lastInHeap){
		int newElement = lastInHeap + 1;
		while (A[newElement] > A[parentOf(newElement)]){
			swap(A,newElement,parentOf(newElement));
			newElement = parentOf(newElement); // update the location of newElement
		}
	}
	
	// swap the elements at locations a and b
	public static void swap(int [] A, int a, int b){
		int tmp = A[a];
		A[a] = A[b];
		A[b] = tmp;
	}
	
	// returns the largest child of the element at idx in the heap A that ends at lastInHeap
	public static int largestChildOf(int [] A, int idx, int lastInHeap){
		int leftChild = idx << 1;
		int rightChild = leftChild + 1;
		if (leftChild > lastInHeap) return idx; // if there are no children in the heap, return the original element
		if (A[leftChild] > A[rightChild] || leftChild == lastInHeap) return leftChild;
		return rightChild;
	}
	
	// returns the index of the parentOf idx
	public static int parentOf(int idx){
		if (idx > 0) return idx >> 1;
		else return 0;
	}
	
	public static void insertionSort(int [] A){
		insertionSort(A, 0, A.length-1);
	}
	
	// insertion sort on a range of an array
	public static void insertionSort(int [] A, int start, int end){
		int elementToInsert, firstUnsorted;

		// Begin with the 1-element sub-array at the beginning of input[].
		// This sub-array is sorted.
		// Iterate through input[] element by element. Adding each one into the sorted sub-array
		// at the beginning of input[].
		for (firstUnsorted = start+1; firstUnsorted <= end; ){
			elementToInsert = firstUnsorted; // this is an unnecessary step.  We do it here because the name change makes the code easier to read.
			
			// find the position of the first element in the sorted part of the list 
			// that is larger than the elementToInsert.
			int insertionPosition = start;
			while (A[elementToInsert] > A[insertionPosition] && insertionPosition < firstUnsorted)
				insertionPosition++;
			
			// insert the next element in its proper place, keeping a copy of the element previously in that location
			int tmp = A[insertionPosition];
			A[insertionPosition] = A[elementToInsert];
			
			// shift all the larger elements back one position
			int nextShift = firstUnsorted;
			while(nextShift > insertionPosition+1) A[nextShift] = A[--nextShift];
			if(insertionPosition != elementToInsert) A[insertionPosition+1]=tmp;
			
			firstUnsorted++;
		}
	}
	
	// this allows us to sort an entire array without bothering to specify a range to sort
	public static void mergeSort(int []A){
		mergeSort (A, 0, A.length-1);
	}
	
	// sort the elements of A between start and end
	public static void mergeSort(int [] A, int start, int end){
		// if the list has only a small number of elements, use insertion sort instead of recursion
		if (end - start < 30) {
			insertionSort(A,start,end);
			return;
		}
		
		// set bounds for the range of recursion
		int leftStart = start;
		int leftEnd = start + ((end - start) / 2);
		int rightStart = leftEnd + 1;
		int rightEnd = end;
		
		// recursively sort the left and right halves of the array
		mergeSort(A,leftStart,leftEnd);
		mergeSort(A,rightStart,rightEnd);
		
		// merge the two sublists into a temporary storage array
		int leftIdx = leftStart;
		int rightIdx = rightStart;
		int tmpIdx = 0;
		int [] tmp = new int [end - start + 1];
		while (tmpIdx < tmp.length){
			if ((leftIdx <= leftEnd) && (rightIdx > rightEnd || A[leftIdx] < A[rightIdx]))
				tmp[tmpIdx++] = A[leftIdx++];
			else tmp[tmpIdx++] = A[rightIdx++];
		}
		
		// copy the temporary array back into A
		for (int i = 0; i < tmp.length; i++){
			A[start+i]=tmp[i];
		}
	}
	
	/**
	 * Prints the contents of an array of integers, separating elements by spaces
	 * 
	 * @param array The array to print
	 * @param title A title to print at the beginning of the line of output
	 */
	public static void print(int[] array, String title){
		System.out.print(title + ": ");
		for (int i : array) {
			System.out.print(i + " ");
		}
		System.out.println();
	}
	
	/**
	 * Prints the contents of an array of integers, separating elements by spaces
	 * 
	 * @param array The array to print
	 * @param title A title to print at the beginning of the line of output
	 */
	public static void print(ArrayList<Integer> array, String title){
		System.out.print(title + ": ");
		for (Integer i : array) {
			System.out.print(i + " ");
		}
		System.out.println();
	}
	
	/**
	 * Reads integers from the specified file and copies them into an array
	 * 
	 * @param filename The name of the file to read. The file must contain exactly one integer on each line
	 * @return An array where the Nth element is the integer on the Nth line of input.txt
	 */
	public static int[] loadInputFromFile(String filename){
		// read file into an ArrayList for temporary storage
		// we do this because the length of the input is unknown until
		// after we parse the file.  Later we will copy the data into
		// an array to improve the performance.
		ArrayList<Integer> inputArrayList = new ArrayList<Integer>();
		BufferedReader br = null;
		 
		try {
 
			String sCurrentLine;
 
			br = new BufferedReader(new FileReader("input.txt"));
 
			while ((sCurrentLine = br.readLine()) != null) {
				inputArrayList.add(new Integer(sCurrentLine));
			}
 
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (br != null)br.close();
			} catch (IOException ex) {
				ex.printStackTrace();
			}
		}
		
		// copy the arrayList into an array of ints
		int [] inputArray = new int [inputArrayList.size()];
		int idx = 0;
		for (Integer i : inputArrayList) inputArray[idx++] = i.intValue();
	
		return inputArray;
	}
	
	public static void printToFile(int A[], String filename){
		try {
 
			File file = new File(filename);
 
			// if the file doesn't exist, create it
			if (!file.exists()) {
				file.createNewFile();
			}
 
			FileWriter fw = new FileWriter(file.getAbsoluteFile());
			BufferedWriter bw = new BufferedWriter(fw);
			for (int i : A)	bw.write(i + "\n");
			bw.close();
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

