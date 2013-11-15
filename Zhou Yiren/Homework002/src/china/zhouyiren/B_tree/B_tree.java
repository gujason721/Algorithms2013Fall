package china.zhouyiren.B_tree;

import java.io.*;
import java.util.*;



//define BTNode
class BTNode {
      int number;  
      ArrayList<Integer> key;  
      ArrayList<BTNode> children;  
      boolean isLeaf;
      
      public BTNode(){
			this.key = new ArrayList<Integer>();
			this.children = new ArrayList<BTNode>();
			this.isLeaf = false;
		}
      
      public void renewnumber(){
      	this.number=key.size();
      }

      int getkey(int i){
      	return key.get(i);
      }
      
      BTNode getchildren(int i){
      	return children.get(i);
      }
      
      void setkey(int i, int k) {
      	key.set(i,k);
      }

      void setchildren(int i, BTNode c){
      	children.set(i, c);
      }
      
      void shownode(){
    	  System.out.print("[	");
    	  for(int i=0;i<key.size();i++){
    		  System.out.print(getkey(i)+"	");
    	  }
    	  System.out.print("]");
      }
}

public class B_tree{
	
	//define structure
	private static BTNode root;
	private static int t=3;
	private static int fullNum=2*t-1;
	
	private static KV NIL = new KV(null,-1);	
	
	//define KV as a pair
	private static class KV{
        BTNode x;  
        int key;  
          
        public KV(BTNode x,int i){  
             this.x=x;
             this.key=i;
        }
        
        public void tostring(){
        	x.shownode();
        	System.out.println("	No."+(key+1));
        	System.out.println();
        }  
    }
	
	public B_tree(){
		root=new BTNode();
		root.isLeaf=true;
		root.number=0;
	}
	
	//test code here
	public static void main(String[] args) {
		B_tree tree = new B_tree();
        int[] array = {39,58,82,26,47,63,27,67,51,94};
        
        //insert data
        for (int i = 0; i < array.length; i++) {  
            B_tree_insert(tree,array[i]);  
            System.out.println("After insert the: " + array[i]);
            
            //search data
            KV p=B_tree_search(root,array[i]);
            p.tostring();
        }
        
        //show one node
        System.out.println("Show one node:");
        KV p=B_tree_search(root,array[5]);
        p.tostring();
        System.out.println(p.x.getkey(p.key));
        
        //delete data
        B_tree_delete(tree,array[5]);
        
        //show node
        p=B_tree_search(root,array[4]);
        System.out.println("After delete:");
        p.tostring();
	}
	
	//search for key
	public static KV B_tree_search(BTNode x,int k){
		int i=0;
		while(i<x.number && k>x.getkey(i)){
			i++;
		}
		if(i<x.number && k==x.getkey(i)){
			KV y=new KV(x,i);
			return y;
		}
		else if(x.isLeaf){
			System.out.println("Error: BTNode not exist.");
			return NIL;
		}
		else{
			return B_tree_search(x.getchildren(i),k);
		}
	}
	
	
	//split children
	public static void B_tree_split_child(BTNode x,int i){
		BTNode z=new BTNode();
		z.number=t-1;
		BTNode y=x.getchildren(i-1);
		z.isLeaf=y.isLeaf;
		
		//set z key
		for(int j=0;j<t-1;j++){
			z.key.add(j,y.getkey(j+t));
		}
		for(int j=0;j<t-1;j++){
			y.key.remove(t);
		}
		
		//set z children
		if(!y.isLeaf){
			for(int j=0;j<t;j++){
				z.children.add(j,y.getchildren(j+t));
			}
			for(int j=0; j<t; j++){
				y.children.remove(t);
			}
		}
		
		y.number=t-1;
		
		//set x children
		x.children.add(i,z);
		x.key.add(i-1,y.getkey(t-1));
		y.key.remove(t-1);
		x.number++;
	}
	
	//insert key
	public static void B_tree_insert(B_tree T,int k){
		BTNode r=root;
		if(r.number==fullNum){
			BTNode s=new BTNode();
			root=s;
			s.isLeaf=false;
			s.number=0;
			s.children.add(0,r);
			B_tree_split_child(s,1);
			B_tree_insert_nonfull(s,k);
		}
		else{
			B_tree_insert_nonfull(r,k);
		}
	}
	
	//insert key nonfull
	public static void B_tree_insert_nonfull(BTNode x,int k){
		int i=x.number-1;
		if(x.isLeaf){
			while(i>=0 && k<x.getkey(i)){
				i--;
			}
			x.key.add(i+1, k);
			x.number++;
		}
		else{
			while(i>=0 && k<x.getkey(i)){
				i--;
			}
			i++;
			if(x.getchildren(i).number==fullNum){
				B_tree_split_child(x,i+1);
				if(k>x.getkey(i)){
					i++;
				}
			}
			B_tree_insert_nonfull(x.getchildren(i),k);
		}
	}
	
	//delete
	public static void B_tree_delete(B_tree T,int k){
		BTNode r=root;
		B_tree_delete_key(r,k);
		if(r.number==0 && !r.isLeaf){
			root=r.getchildren(0);
		}
	}
	
	//delete key
	public static void B_tree_delete_key(BTNode x, int k){
		int i=0;
		while(i<x.number && k>x.getkey(i)){
			i++;
		}
		//case 1
		if(i<x.number && k==x.getkey(i) && x.isLeaf){
			x.key.remove(i);
			x.renewnumber();
		}
		//case 2
		else if(i<x.number && k==x.getkey(i)){
			BTNode y=x.getchildren(i);
			//case 2a
			if(y.number>=t){
				int k1=y.getkey(y.number-1);
				B_tree_delete_key(y,k1);
				x.setkey(i,k1);
			}
			else{
				BTNode z=x.getchildren(i+1);
				//case 2b
				if(z.number>=t){
					int k1=z.getkey(0);
					B_tree_delete_key(z,k1);
					x.setkey(i,k1);
				}
				else{
					//case 2c
					x.key.remove(i);
					x.number--;
					x.children.remove(i+1);
					x.getchildren(i).key.addAll(z.key);
					x.getchildren(i).renewnumber();
					x.getchildren(i).children.addAll(z.children);
				}
			}
		}
		//case 3
		else if(!x.isLeaf){
			BTNode s=x.getchildren(i);
			if(s.number==t-1){
				if(i>0){
					BTNode lsibling=x.getchildren(i-1);
					//case 3a left sibling
					if(lsibling.number>t-1){
						s.key.remove(s.number-1);
						s.key.add(0,x.getkey(i));
						x.setkey(i,lsibling.getkey(lsibling.number-1));
						lsibling.key.remove(lsibling.number-1);
						lsibling.renewnumber();
					}
				}
				if(x.children.size()-1>i){
					BTNode rsibling = x.getchildren(i+1);
					//case 3a right sibling
					if(rsibling.number>t-1){
						s.setkey(t-2,x.getkey(i));
						x.setkey(i,rsibling.getkey(0));
						rsibling.key.remove(0);
						rsibling.renewnumber();
					}
				}
				//case 3b
				else{
					if(x.children.size()-1>i){
						s.key.add(t-1,x.getkey(i));
						BTNode z=x.getchildren(i+1);
						x.children.remove(i+1);
						x.key.remove(i);
						x.renewnumber();
						s.key.addAll(z.key);
						s.children.addAll(z.children);
					}
					else if(i>0){
						BTNode z=x.getchildren(i-1);
						x.children.remove(i);
						x.key.remove(i);
						x.renewnumber();
						z.key.addAll(s.key);
						z.children.addAll(s.children);
					}
				}
				B_tree_delete_key(s,k);
			}
		}
	}

}

