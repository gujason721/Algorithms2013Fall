package china.zhouyiren.red_black_tree;


public class red_black_tree {
	
	//test code here
	public static void main(String[] args) {
		red_black_tree Tree = new red_black_tree();
		
		//generate random array
        int num=10;
        int top=100;
		int [] a=new int[num];
		for(int i=0;i<num;i++){
			int j=(int)(Math.random()*top);
			a[i]=j;
			Tree.redblackInsert(new Node(a[i]));
			System.out.print(a[i]+"	");
		}
		System.out.print("\n");
		
        //print tree
        red_black_tree.printTree();
        System.out.print("\n");
        Tree.redblackDelete(findNode(a[5]));
        
        red_black_tree.printTree();
	}
	
	//left rotate and right rotate
	private void leftRotate(Node node) {
		Node right=node.getright();
		node.setright(right.getleft());
		if(right.getleft()!=NIL){
			right.getleft().setparent(node);
		}
		right.setparent(node.getparent());
		if(node.getparent()==NIL){
			root=right;
			root.setparent(NIL);
		}
		else if(node==node.getparent().getleft()){
			node.getparent().setleft(right);
		}
		else{
			node.getparent().setright(right);
		}
		right.setleft(node);
		node.setparent(right);
	}
	
	private void rightRotate(Node node) {
		Node left=node.getleft();
		node.setleft(left.getright());
		if(left.getright()!=NIL){
			left.getright().setparent(node);
		}
		left.setparent(node.getparent());
		if(node.getparent()==NIL){
			root=left;
			root.setparent(NIL);
		}
		else if(node==node.getparent().getright()){
			node.getparent().setright(left);
		}
		else{
			node.getparent().setleft(left);
		}
		left.setright(node);
		node.setparent(left);
	}
	
	//insert node
	public void redblackInsert(Node nodez){
		Node nodey=NIL;
		Node nodex=root;
		while(nodex!=NIL){
			nodey=nodex;
			if(nodez.getkey()<nodex.getkey()){
				nodex=nodex.getleft();
			}
			else{
				nodex=nodex.getright();
			}
		}
		nodez.setparent(nodey);
		if(nodey==NIL){
			root=nodez;
			root.setparent(NIL);
		}
		else if(nodez.getkey()<nodey.getkey()){
			nodey.setleft(nodez);
		}
		else{
			nodey.setright(nodez);
		}
		nodez.setleft(NIL);
		nodez.setright(NIL);
		nodez.setcolor(Color.Red);
		redblackInsertFixup(nodez);
	}
	
	public void redblackInsertFixup(Node nodez){
		while(nodez.getparent().getcolor()==Color.Red){
			if(nodez.getparent()==nodez.getparent().getparent().getleft()){
				Node nodey=nodez.getparent().getparent().getright();
				if(nodey.getcolor()==Color.Red){
					nodez.getparent().setcolor(Color.Black);
					nodey.setcolor(Color.Black);
					nodez.getparent().getparent().setcolor(Color.Red);
					nodez=nodez.getparent().getparent();
				}
				else if(nodez==nodez.getparent().getright()){
					nodez=nodez.getparent();
					leftRotate(nodez);
				}
				else{
					nodez.getparent().setcolor(Color.Black);
					nodez.getparent().getparent().setcolor(Color.Red);
					rightRotate(nodez.getparent().getparent());
				}
			}
			else{
				Node nodey=nodez.getparent().getparent().getleft();
				if(nodey.getcolor()==Color.Red){
					nodez.getparent().setcolor(Color.Black);
					nodey.setcolor(Color.Black);
					nodez.getparent().getparent().setcolor(Color.Red);
					nodez=nodez.getparent().getparent();
				}
				else if(nodez==nodez.getparent().getleft()){
					nodez=nodez.getparent();
					rightRotate(nodez);
				}
				else{
					nodez.getparent().setcolor(Color.Black);
					nodez.getparent().getparent().setcolor(Color.Red);
					leftRotate(nodez.getparent().getparent());
				}
			}
		}
		root.setcolor(Color.Black);
	}
	
	//transplant
	public void redblackTransplant(Node nodeu,Node nodev){
		if(nodeu.getparent()==NIL){
			root=nodev;
			root.setparent(NIL);
		}
		else if(nodeu==nodeu.getparent().getleft()){
			nodeu.getparent().setleft(nodev);
		}
		else{
			nodeu.getparent().setright(nodev);
		}
		nodev.setparent(nodeu.getparent());
	}
	
	//delete node
	public void redblackDelete(Node nodez){
		Node nodey=nodez;
		Color color_y_original=nodey.getcolor();
		Node nodex=NIL;
		if(nodez.getleft()==NIL){
			nodex=nodez.getright();
			redblackTransplant(nodez,nodez.getright());
		}
		else if(nodez.getright()==NIL){
			nodex=nodez.getleft();
			redblackTransplant(nodez,nodez.getleft());
		}
		else{
			nodey=treeMinimum(nodez.getright());
			color_y_original=nodey.getcolor();
			nodex=nodey.getright();
			if(nodey.getparent()==nodez){
				nodex.setparent(nodey);
			}
			else{
				redblackTransplant(nodey,nodey.getright());
				nodey.setright(nodez.getright());
				nodey.getright().setparent(nodey);
			}
			redblackTransplant(nodez,nodey);
			nodey.setleft(nodez.getleft());
			nodey.getleft().setparent(nodey);
			nodey.setcolor(nodez.getcolor());
		}
		if(color_y_original==Color.Black){
			redblackDeleteFixup(nodex);
		}
	}
	
	public void redblackDeleteFixup(Node nodex){
		while(nodex!=root && nodex.getcolor()==Color.Black){
			if(nodex==nodex.getparent().getleft()){
				Node nodew=nodex.getparent().getright();
				if(nodew.getcolor()==Color.Red){
					nodew.setcolor(Color.Black);
					nodex.getparent().setcolor(Color.Red);
					leftRotate(nodex.getparent());
					nodew=nodex.getparent().getright();
				}
				if(nodew.getleft().getcolor()==Color.Black && nodew.getright().getcolor()==Color.Black){
					nodew.setcolor(Color.Red);
					nodex=nodex.getparent();
				}
				else if(nodew.getright().getcolor()==Color.Black){
					nodew.getleft().setcolor(Color.Black);
					nodew.setcolor(Color.Red);
					rightRotate(nodew);
					nodew=nodex.getparent().getright();
				}
				else{
					nodew.setcolor(nodex.getparent().getcolor());
					nodex.getparent().setcolor(Color.Black);
					nodew.getright().setcolor(Color.Black);
					leftRotate(nodex.getparent());
					root=nodex;
					root.setparent(NIL);
				}
			}
			else{
				Node nodew=nodex.getparent().getleft();
				if(nodew.getcolor()==Color.Red){
					nodew.setcolor(Color.Black);
					nodex.getparent().setcolor(Color.Red);
					rightRotate(nodex.getparent());
					nodew=nodex.getparent().getleft();
				}
				if(nodew.getright().getcolor()==Color.Black && nodew.getleft().getcolor()==Color.Black){
					nodew.setcolor(Color.Red);
					nodex=nodex.getparent();
				}
				else if(nodew.getleft().getcolor()==Color.Black){
					nodew.getright().setcolor(Color.Black);
					nodew.setcolor(Color.Red);
					leftRotate(nodew);
					nodew=nodex.getparent().getleft();
				}
				else{
					nodew.setcolor(nodex.getparent().getcolor());
					nodex.getparent().setcolor(Color.Black);
					nodew.getleft().setcolor(Color.Black);
					rightRotate(nodex.getparent());
					root=nodex;
					root.setparent(NIL);
				}
			}
		}
		nodex.setcolor(Color.Black);
	}
	
	//find successor
	public static Node findSuccessor(Node node){
		Node right=node.getright();
		if(right!=NIL){
			Node previous=null;
			while(right!=NIL){
				previous=right;
				right=right.getleft();
			}
			return previous;
		}
		else{
			Node parent=node.getparent();
			while(parent!=NIL && node!=parent.getleft()){
				node=parent;
				parent=parent.getparent();
			}
			return parent;
		}
	}
	
	//find node
	public static Node findNode(int value){
		Node temp=root;
		while(temp!=NIL){
			if(temp.getkey()==value){
				return temp;
			}
			else if(temp.getkey()>value){
				temp=temp.getleft();
			}
			else{
				temp=temp.getright();
			}
		}
		return null;
	}
	
	//in order traverse
	private static void inOrderTraverse(Node node){
		if(node!=NIL){
			inOrderTraverse(node.getleft());
			System.out.println("Node:"+node.getkey()+"	Color:"+node.getcolor()+"	Parent:"+node.getparent().getkey()+"	Leftchild:"+node.getleft().getkey()+"	Rightchild:"+node.getright().getkey());
			inOrderTraverse(node.getright());
		}
	}
	
	//print tree
	public static void printTree(){
		inOrderTraverse(root);
	}
	
	//tree minimum
	private Node treeMinimum(Node node){
		if(node.getleft()==NIL){
			return node;
		}
		else{
			return(treeMinimum(node.getleft()));
		}
	}
	
	//define root and NIL
	private final static Node NIL=new Node(null,null,null,Color.Black,-1);
	private static Node root;
		
	public red_black_tree(){
		root=NIL;
	}
		
	public red_black_tree(Node root){
		red_black_tree.root=root;
	}

}

//structure definition
class Node {
	private int key;
	private Node left;
	private Node right;
	private Node parent;
	private Color color;
	
	public Node(Node left,Node right,Node parent,Color color,int key){
		super();
		this.left=left;
		this.right=right;
		this.parent=parent;
		this.color=color;
		this.key=key;
	}
	
	public Node(){
	}
	
	public Node(int key){
		this(null,null,null,null,key);
	}
	
	public Node getleft(){
		return left;
	}
	
	public Node getright(){
		return right;
	}
	
	public void setleft(Node left){
		this.left=left;
	}
	
	public void setright(Node right){
		this.right=right;
	}
	
	public Node getparent(){
		return parent;
	}
	
	public void setparent(Node parent){
		this.parent=parent;
	}
	
	public Color getcolor(){
		return color;
	}
	
	public void setcolor(Color color){
		this.color=color;
	}
	
	public int getkey(){
		return key;
	}
	
	public void setkey(int key){
		this.key=key;
	}
}

enum Color{
	Black,Red;
}