package bTree;

import java.util.ArrayList;

public class BTree{
	
	private pairs nil = new pairs(null,-1); 
	public BTNode root = new BTNode();
	private int t = 3;
	
	private class BTNode {
		ArrayList<Integer> keys;
		ArrayList<BTNode> children; // The children of the node: n+1
		boolean leaf; 
		int n; // The number of keys stored in the node 
		
		public BTNode(){
			this.keys = new ArrayList<Integer>();
			this.children = new ArrayList<BTNode>();
			this.leaf = false;
		}
		
		public void updateN(){
			this.n = keys.size();
		}
		public void printNode(){
			for(int i=0; i<keys.size(); i++){
				System.out.println(keys.get(i).intValue());
			}
		}
	}
	
	// class to return the search result
	private class pairs{
		BTNode foundNode;
		int location;
		
		public pairs(BTNode foundNode, int location){
			this.foundNode = foundNode;
			this.location = location;
		}
		
		public void printPairs(){
			System.out.println("&&&The found key is in the node containing the following keys:");
			foundNode.printNode();
			System.out.println("&&&And the key is the " + (location+1) + "th key of the node.");
		}
	}
	
	public BTree(){
		root.leaf = true;
		root.n = 0;
	}
	
	public pairs BTreeSearch(BTNode x, int k){
		int i = 0;
		System.out.println("***Try to search value " + k + " in the current B-tree...");
		while(i<x.n && k>x.keys.get(i)){
			i++;
		}
		if(i<x.n && k==x.keys.get(i)){
			pairs result = new pairs(x,i);
			result.printPairs();
			return result;
		}
		else if(x.leaf){
			System.out.println("&&&The key doesn't exist in the current B tree...");
			return nil;
		}
		else{
			return BTreeSearch(x.children.get(i),k);
		}
	}
	
	public void displayBTree(BTNode x){
		x = root;
		if(x.children!=null){
			for(BTNode y : x.children){
				displayBTree(y);
			}
			x.printNode();
		}
	}
	
	public void BTreeSplitChild(BTNode x, int i){ // parameter i is the location to split, not the index
		BTNode z = new BTNode();
		BTNode y = x.children.get(i-1);
		z.leaf = y.leaf;
		z.n = t-1;
		// give the largest t-1 keys to node z
		for(int j=0;j<t-1;j++){
			z.keys.add(j, y.keys.get(j+t));
		}
		for(int j=0;j<t-1;j++){
			y.keys.remove(t);
		}
		// give children to the new node z
		if(!y.leaf){
			for(int p=0; p<t; p++){
				z.children.add(p, y.children.get(p+t));
			}
			for(int p=0; p<t; p++){
				y.children.remove(t);
			}	
		}
		y.n = t-1;// update n count for node y
		// update x's children and add z to x's i+1th children
		x.children.add(i, z);
		x.keys.add(i-1, y.keys.get(t-1));
		y.keys.remove(t-1);
		x.n++;
	}
	
	public void BTreeInsert(BTree T, int key){
		BTNode r = root;
		if(r.n==2*t-1){
			BTNode s = new BTNode();
			root = s;
			s.leaf = false;
			s.n = 0;
			s.children.add(0,r);
			BTreeSplitChild(s,1);
			BTreeInsertNonfull(s,key);
		}
		else{
			BTreeInsertNonfull(r,key);
		}
		System.out.println("The value " + key + " has been successfully inserted");
	}

	public void BTreeInsertNonfull(BTNode x, int key) {
		int i = x.n-1;
		if(x.leaf){
			while(i>=0 && key<x.keys.get(i)){
				i--;
			}
			x.keys.add(i+1, key);
			x.n++;
		}
		else{
			while(i>=0 && key<x.keys.get(i)){
				i--;
			}
			i++;
			if(x.children.get(i).n==2*t-1){
				BTreeSplitChild(x,i+1);
				if(key>x.keys.get(i)){
					i++;
				}
			}
			BTreeInsertNonfull(x.children.get(i),key);
		}	
	}
	
	
	public void BTreeDelete(BTree T, int key){
		BTNode r = root;
		BTreeDeleteKey(r,key);
		if(r.n==0 && !r.leaf){
			root = r.children.get(0);
		}	
		System.out.println("The value " + key + " has been successfully deleted...");
	}

	public void BTreeDeleteKey(BTNode x, int key) {
		int i = 0;
		while(i<x.n && key>x.keys.get(i)){
			i++;
		}
		if(i<x.n && key==x.keys.get(i) && x.leaf){	// case 1, x is leaf and key is in x
			x.keys.remove(i);
			x.updateN();
		}
		else if(i<x.n && key==x.keys.get(i)){	// case 2
			BTNode y = x.children.get(i);
			if(y.n>=t){	// case 2a
				int tKey = y.keys.get(y.n-1);
				BTreeDeleteKey(y,tKey);
				x.keys.set(i, tKey);
			}
			
			else{
				BTNode z = x.children.get(i+1);
				if(z.n>=t){	// case 2b
					int tKey = z.keys.get(0);
					BTreeDeleteKey(z,tKey);
					x.keys.set(i, tKey);
				}			
				else{	// case 2c
					x.keys.remove(i);
					x.n--;
					x.children.remove(i+1);
					x.children.get(i).keys.addAll(z.keys);
					x.children.get(i).updateN();
					x.children.get(i).children.addAll(z.children);	
				}				
			}			
		}		
		else if(!x.leaf){	// case 3
			BTNode s = x.children.get(i);
			if(s.n==t-1){
				if(i-1>-1){
					BTNode leftSibling = x.children.get(i-1);
					if(leftSibling.n>t-1){	//	case 3a-left sibling					
						s.keys.remove(t-2);
						s.keys.add(0,x.keys.get(i));
						x.keys.set(i, leftSibling.keys.get(leftSibling.n-1));
						leftSibling.keys.remove(leftSibling.n-1);
						leftSibling.updateN();
					}
				}			
				if(i<x.children.size()-1){
					BTNode rightSibling = x.children.get(i+1);
					if(rightSibling.n>t-1){	// case 3a-right sibling
						s.keys.set(t-2, x.keys.get(i));
						x.keys.set(i, rightSibling.keys.get(0));
						rightSibling.keys.remove(0);
						rightSibling.updateN();			
					}
				}		
				else{	// case 3b
					if(i<x.children.size()-1){
						s.keys.add(t-1, x.keys.get(i));
						BTNode z = x.children.get(i+1);
						x.children.remove(i+1);
						x.keys.remove(i);
						x.updateN();
						s.keys.addAll(z.keys);
						s.children.addAll(z.children);
					}					
					else if(i-1>-1){
						BTNode z = x.children.get(i-1);
						x.children.remove(i);
						x.keys.remove(i);
						x.updateN();
						z.keys.addAll(s.keys);
						z.children.addAll(s.children);						
					}
				}
				BTreeDeleteKey(s,key);
			}						
		}	
	}
	
}
