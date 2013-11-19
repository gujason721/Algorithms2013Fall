package redblack;

import java.util.ArrayList;

public class RedBlackTreeTest {

	public static void main(String[] args) {
		RBTree T = new RBTree();
		
		// Test Insert
		//   Generate an array of random numbers
		java.util.Random rand = new java.util.Random();
		int [] A = new int [15];
		int testRange = 500;
		for (int i = 0; i < A.length; i++) {
			int nextRandom = rand.nextInt() % testRange;
			A[i] = nextRandom;
		}
		print(A,"A");
		
		//   Insert the entire array into the tree
		for (int i : A)	T.insert(new RBNode(i));
		//   In-order-traverse the resulting tree
		ArrayList<Integer> iot = T.inOrderTraversal();
		print(iot,"In-order-traversal");
		
		// Delete all the negative numbers from the tree
		for (int i : A) if(i < 0) T.delete(i);
		iot = T.inOrderTraversal();
		print(iot,"In-order-traversal after deleteing all the negative numbers from the tree");
		
		// Put back all the negative numbers and delete all the positive numbers instead
		for (int i : A) if(i < 0) T.insert(new RBNode(i));
		for (int i : A) if(i >= 0) T.delete(i);
		iot = T.inOrderTraversal();
		print(iot,"In-order-traversal after deleteing all the positive numbers from the tree");
	}

	/**
	 * Prints the contents of an array of integers, separating elements by spaces
	 * 
	 * @param array The array to print
	 * @param title A title to print at the beginning of the line of output
	 */
	public static void print(int[] array, String title){
		System.out.print(title + ":\n  ");
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
		System.out.print(title + ":\n  ");
		for (Integer i : array) {
			System.out.print(i + " ");
		}
		System.out.println();
	}
	
}
