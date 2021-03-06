package skiplist;

import java.util.ArrayList;

public class SkipListTest {

	public static void main(String[] args) {
		SList L = new SList();

		// Test Insert
		// Generate an array of random numbers
		
		java.util.Random rand = new java.util.Random();
		int[] A = new int[15];
		int testRange = 5;
		for (int i = 0; i < A.length; i++) {
			int nextRandom = rand.nextInt() % testRange;
			A[i] = nextRandom;
		}
		print(A, "A");

		// Insert the entire array into the list
		System.out.println("Inserting the entire array A into the list.");
		for (int i : A){
			L.insert(i);
			ArrayList<Integer> al = L.toArrayList();
			System.out.println("The s-list after inserting " + i);
			L.print();
		}

		
		

		// Delete all the negative numbers from the tree
		System.out.println("\nDeleting all negative keys from the list");
		for (int i : A)
			if (i < 0) {
				L.delete(i);
				ArrayList<Integer> al = L.toArrayList();
				System.out.println("The s-list after deleting " + i);
				L.print();
			}
		
		// try to delete an element that is not in the list
		L.delete(3);
		L.delete(3);
		System.out.print("\nL after attempting to delete 3, which is not in the list: \n");
		L.print();
		
		
		// Put back all the negative numbers and delete all the positive numbers
		// instead
		for (int i : A)
			if (i < 0)
				L.insert(i);
		System.out
				.println("\nNow, replace negatives and delete all positive keys.");
		for (int i : A)
			if (i >= 0) {
				L.delete(i);
				System.out.print("L after deleting " + i + ": \n");
				L.print();
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
