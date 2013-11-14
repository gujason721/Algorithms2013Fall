package redblack;

enum Colour {RED,BLACK};

public class RBNode {
	protected Colour colour;
	protected int key;
	protected RBNode left;
	protected RBNode right;
	protected RBNode p;
	
	public RBNode(int key){
		this.colour = null;
		this.key = key;
		this.left = null;
		this.right = null;
		this.p = null;
	}
	
	public RBNode(){
		this.colour = null;
		this.key = Integer.MAX_VALUE;
		this.left = null;
		this.right = null;
		this.p = null;
	}
}
