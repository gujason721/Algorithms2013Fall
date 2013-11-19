package redBlackTree;

public class RBTree{
	
	private static final RBTNode nil = new RBTNode(null,null,null,true,-1);
	RBTNode root;
	
	public RBTree(){
		this.root = nil;
	}
	public RBTree(RBTNode root){
		this.root = root;
	}
	
	public void RBInsert(RBTNode z){
		RBTNode y = nil;
		RBTNode x = root;
		
			while(x!=nil){
				y = x;
				if(z.key<x.key){
					x = x.left;
				}
				else{
					x = x.right;
				}
			}
			
			z.parent = y;
			if(y == nil){
				root = z;
			}
			else if(z.key < y.key){
				y.left = z;
			}
			else{y.right = z;}
			
			z.left = nil;
			z.right = nil;
			z.color = false;
			RBInsertFixup(z);
	}

	public void RBInsertFixup(RBTNode z) {
		while(z.parent.color==false){
				
			if(z.parent==z.parent.parent.left){
				RBTNode y = z.parent.parent.right;
				if(y.color==false){
					z.parent.color = true;
					y.color = true;
					z.parent.parent.color = false;
					z = z.parent.parent;
				}
				else if(z == z.parent.right){
					z = z.parent;
					leftRotate(z);
				}
				else{
				z.parent.color = true;
				z.parent.parent.color = false;
				rightRotate(z.parent.parent);
				}
			}
			
			else{				
				RBTNode y = z.parent.parent.left;
				if(y.color==false){
					z.parent.color = true;
					y.color = true;
					z.parent.parent.color = false;
					z = z.parent.parent;
				}
				else if(z == z.parent.left){
					z = z.parent;
					rightRotate(z);
				}
				else{
				z.parent.color = true;
				z.parent.parent.color = false;
				leftRotate(z.parent.parent);
				}
			}
		}
		root.color = true;	
	}

	private void rightRotate(RBTNode x) {
		RBTNode y = x.left;
		x.left = y.right;
		if(y.right!=nil){
			y.right.parent = x;
		}
		y.parent = x.parent;
		if(x.parent==nil){
			root = y;
		}
		else if(x==x.parent.left){
			x.parent.left = y;
		}
		else{
			x.parent.right = y;
		}
		y.right = x;
		x.parent = y;
		
	}

	private void leftRotate(RBTNode x) {
		RBTNode y = x.right;
		x.right = y.left;
		if(y.left!=nil){
			y.left.parent = x;
		}
		y.parent = x.parent;
		if(x.parent==nil){
			root = y;
		}
		else if(x==x.parent.left){
			x.parent.left = y;
		}
		else{
			x.parent.right = y;
		}
		y.left = x;
		x.parent = y;
	}
	
	public void RBDelete(RBTNode z){
		RBTNode y = z;
		RBTNode x;
		boolean yOriginalColor = y.color;
		if(z.left==nil){
			x = z.right;
			RBTransplant(z,z.right);
		}
		else if(z.right==nil){
			x = z.left;
			RBTransplant(z,z.left);
		}
		else{
			y = TreeMinimum(z.right);
			yOriginalColor = y.color;
			x = y.right;
			if(y.parent==z){
				x.parent = y;
			}
			else{
				RBTransplant(y,y.right);
				y.right = z.right;
				y.right.parent = y;
			}
			RBTransplant(z,y);
			y.left = z.left;
			y.left.parent = y;
			y.color = z.color;
		}
		if(yOriginalColor==true){
			RBDeleteFixup(x);
		}
		System.out.println("*********The key " + z.key + " has been deleted*********");
	}

	private RBTNode TreeMinimum(RBTNode p) {
		while(p.left!=null){
			p = p.left;
		}
		return p;
	}

	public void RBDeleteFixup(RBTNode x) {
		while(x!=root && x.color==true){
			if(x==x.parent.left){
				RBTNode w = x.parent.right;
				if(w.color==false){
					w.color = true;
					x.parent.color = false;
					leftRotate(x.parent);
					w = x.parent.right;
				}
				if(w.left.color==true && w.right.color==true){
					w.color = false;
					x = x.parent;
				}
				else if(w.right.color==true){
					w.left.color = true;
					w.color = false;
					rightRotate(w);
					w = x.parent.right;
				}
				w.color = x.parent.color;
				x.parent.color = true;
				w.right.color = true;
				leftRotate(x.parent);
				x = root;
			}
			else{
				
			}
		}
		x.color = true;
	}

	public void RBTransplant(RBTNode u, RBTNode v) {
		if(u.parent==nil){
			root = v;
		}
		else if(u==u.parent.left){
			u.parent.left = v;
		}
		else{
			u.parent.right = v;
		}
		v.parent = u.parent;
	}
	
	public void printRBTree(){
		inOrderTraverse(root);
	}
	
	public void inOrderTraverse(RBTNode node) {		
		if(node!=nil){	
			inOrderTraverse(node.left);
			System.out.println("***Red-Black Tree***, Node: " + node.key + " and its color is: " + node.getColor(node));
			inOrderTraverse(node.right);
		}
	}
}
