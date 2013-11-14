package sortingAlgorithms;

import java.io.File;

public class HeapSort {
	
	
	private int[] array;
	private int[] sortedArray;
	private int heapSize;
	public HeapSort(int[] inputArray){
		array = inputArray;
		heapSize = array.length;
		
		// Write the unsorted array into the .txt
		File heapArray = new File("E:\\Jy_heapSortArray.txt");
		new WriteToTxt(array,heapArray);
						
		// Sort the array using insert sorting
		long startTime = System.nanoTime();
		sortedArray = heapSort(array);
		long endTime = System.nanoTime();
		
		// Write the sorted array into the .txt
		File heapedArray = new File("E:\\Jy_heapSortedArray.txt");
		new WriteToTxt(sortedArray,heapedArray);
						 
		System.out.println("***The generated random array has been sorted with heap sorting.");
		System.out.println("***Check hardware E for the stored arrays in .txt.");
		System.out.println("***The run time of heap sorting is " + (endTime-startTime) + " nanoseconds\n");
	}
	
	public int[] heapSort(int[] heap)
	{
	    BulidHeap(heap);
	    for(int i = heapSize-1;i>0;i--){
	    	int tmp=heap[0];
		    heap[0]=heap[heapSize-1];
		    heap[heapSize-1]=tmp;
		    heapSize--; 
		    maxHeapify(heap,0);
	    }    
	    return heap;
	}
	
	private void maxHeapify(int[] heap, int i) {
		int l = Left(i);
		int r = Right(i);
		int largest;
		if(l<=heapSize-1 && heap[l]>heap[i]){
			largest = l;
		}
		else{
			largest = i;
		}
		
		if(r<=heapSize-1 && heap[r]>heap[largest]){
			largest = r;
		}
		
		if(heap[i]!=heap[largest]){
			int tmp=heap[i];
		    heap[i]=heap[largest];
		    heap[largest]=tmp;
		    maxHeapify(heap,largest);
		}
		
		
	}

	private void BulidHeap(int[] heap) {
		for(int i = heap.length/2-1;i>=0;i--){
			maxHeapify(heap,i);
		}
		
	}

	
	private int Right(int i) {
		return 2*(i+1);
	}

	private int Left(int i) {
		return 2*(i+1)-1;
	}
}
