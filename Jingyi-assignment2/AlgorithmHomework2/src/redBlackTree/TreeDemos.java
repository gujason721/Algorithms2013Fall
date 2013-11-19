package redBlackTree;

import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

import bTree.BTree;
import skipList.SkipList;

public class TreeDemos {

	public static void main(String[] args) throws IOException{
		
		while(true){
			System.out.println("Please choose a data structure to display:");
			System.out.println("Enter 1 for red-black tree, 2 for B-tree, 3 for skip list and any other keys of numbers to exit...");
			@SuppressWarnings("resource")
			Scanner displayOption = new Scanner(System.in);
			int dOption = displayOption.nextInt();
			System.out.println("You have chosen " + dOption + "...");
			
			if(dOption == 1){ // demo for Red-Black Tree
				System.out.println("**********************************Demo for RB-Tree begins**********************************");
				RBTree RBT = new RBTree();
				ArrayList<RBTNode> RBTArray = new ArrayList<RBTNode>();
				int[] array = {6,7,30,3,14,8,9,20,84,90,17,5,13,78,89,103,99,80,42};
				for(int i=0;i<array.length;i++){
					RBTArray.add(new RBTNode(array[i])); 
					RBT.RBInsert(RBTArray.get(i));
				}
				System.out.println("*********The created Red-Black Tree after inserting some nodes into an empty tree is:*********");
				System.out.println("*********Note: We demonstrate the RB-Tree by in-order traverse it...*********");
				RBT.printRBTree();
				System.out.println("*********Now we try to delete some nodes from the RB-Tree...*********");
				RBT.RBDelete(RBTArray.get(3)); // refers to value 3
				RBT.RBDelete(RBTArray.get(5)); // refers to value 8
				RBT.RBDelete(RBTArray.get(10));	// refers to value 17
				System.out.println("*********The Red-Black Tree after deleting some nodes is:*********");
				RBT.printRBTree();
				System.out.println("*********I didn't implement search operation as it's no difference with binary search tree...*********");
				System.out.println("**********************************Demo for RB-Tree ends**********************************");
			}	
			
			else if(dOption==2){	//demo for B-Tree
				System.out.println("**********************************Demo for B-Tree begins**********************************");
				BTree BT = new BTree();
				int[] array = {6,7,30,3,14,8,9,20,84,90,17,5,13,78,89,103,99,80,42};
				for(int i=0;i<array.length;i++){ 
					BT.BTreeInsert(BT,array[i]);
				}
				System.out.println("***The B-tree has been created, now we try some operation on it***");			
				BT.BTreeSearch(BT.root, 14);
				BT.BTreeDelete(BT, 14); 
				BT.BTreeSearch(BT.root, 14);
				BT.BTreeDelete(BT, 9);	
				BT.BTreeSearch(BT.root, 9);
				BT.BTreeDelete(BT, 20); 
				BT.BTreeSearch(BT.root, 20);
				BT.BTreeDelete(BT, 30); 
				BT.BTreeSearch(BT.root, 30);
				BT.BTreeDelete(BT, 7);
				BT.BTreeSearch(BT.root, 7);
				BT.BTreeSearch(BT.root, 100);
				BT.BTreeInsert(BT, 100);
				BT.BTreeSearch(BT.root, 100);
				System.out.println("**********************************Demo for B-Tree ends**********************************");
			}	
			
			else if(dOption==3){	// Demo for Skip List
				System.out.println("**********************************Demo for Skip List begins**********************************");
				SkipList SL = new SkipList();
				int[] array = {6,7,30,3,14,8,9,20,84,2,17,70,67,1,19,24,57,63,42,80};
				for(int i=0;i<array.length;i++){ 
					SL.insert(array[i]);
				}
				SL.contains(30);
				SL.contains(100);
				SL.delete(30);
				SL.contains(30);
				SL.insert(100);
				SL.contains(100);
				SL.displaySkipList();
				System.out.println("**********************************Demo for Skip List begins**********************************");
			}			
			else{
				displayOption.close();
				System.out.println("**********************************This is the end of the demos, thank you**********************************");
				System.out.println("**********************************To see the demos again, please rerun the program**********************************");
				break;	
			}
		}
	} 
}
