package redBlackTree;


public class RBTNode {
	int key;
	RBTNode left;
	RBTNode right;
	RBTNode parent;	
	boolean color;//True represents black and false red
	
	
	RBTNode(int key){
		this.key = key;
	}
	
	RBTNode(RBTNode left, RBTNode right, RBTNode parent, boolean color, int key) {  
        this.left = left;  
        this.right = right;  
        this.parent = parent;  
        this.color = color;  
        this.key = key;  
    }  
	
	public String getColor(RBTNode node){
		String result;
		if(node.color==true){result = "black";}
		else{result = "red";}
		return result;
	}
}
