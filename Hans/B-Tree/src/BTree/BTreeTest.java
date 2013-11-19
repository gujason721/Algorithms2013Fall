package BTree;

import java.util.ArrayList;

public class BTreeTest {

	public static void main(String[] args) {
		BTree T = new BTree(2);

		// Test Insert
		// Generate an array of random numbers
		java.util.Random rand = new java.util.Random();
		int[] A = new int[15];
		int testRange = 5;
		for (int i = 0; i < A.length; i++) {
			int nextRandom = rand.nextInt() % testRange;
			A[i] = nextRandom;
		}
		//int [] A = {2, 1, -2, -2, 0, 0, -1, -2, 0, 0, 1, -2, 2, 0};
		//int [] A = {0,2,-2,-1}; // short array that causes an error
		print(A, "A");

		// Insert the entire array into the tree
		for (int i : A){
			T.insert(i);
			System.out.print("T after inserting " + i + ": ");
			T.printTree();
		}
		// In-order-traverse the resulting tree
		//ArrayList<Integer> iot = T.inOrderTraversal();
		//print(iot, "In-order-traversal");
		System.out.print("T:\n  ");
		T.printTree();

		// Delete all the negative numbers from the tree
		System.out.println("\nDeleting all negative keys from the tree");
		for (int i : A)
			if (i < 0) {
				//if (i == -1) {
				//	System.out.println("The next line will cause an error.");
				//}
				T.delete(i);
				System.out.print("T after deleting " + i + ": ");
				T.printTree();
			}

		// Put back all the negative numbers and delete all the positive numbers
		// instead
		for (int i : A)
			if (i < 0)
				T.insert(i);
		System.out.println("\nNow, replace negatives and delete all positive keys.");
		for (int i : A)
			if (i >= 0) {
				T.delete(i);
				System.out.print("T after deleting " + i + ": ");
				T.printTree();
			}
	}

	/**
	 * Prints the contents of an array of integers, separating elements by
	 * spaces
	 * 
	 * @param array
	 *            The array to print
	 * @param title
	 *            A title to print at the beginning of the line of output
	 */
	public static void print(int[] array, String title) {
		System.out.print(title + ":\n  ");
		for (int i : array) {
			System.out.print(i + " ");
		}
		System.out.println();
	}

	/**
	 * Prints the contents of an array of integers, separating elements by
	 * spaces
	 * 
	 * @param array
	 *            The array to print
	 * @param title
	 *            A title to print at the beginning of the line of output
	 */
	public static void print(ArrayList<Integer> array, String title) {
		System.out.print(title + ":\n  ");
		for (Integer i : array) {
			System.out.print(i + " ");
		}
		System.out.println();
	}

}
