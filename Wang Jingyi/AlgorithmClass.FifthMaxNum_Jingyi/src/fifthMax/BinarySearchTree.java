package fifthMax;

public class BinarySearchTree {
	
	private Node root = null;
	private int nodeNum;
	private int count=0;
	
	
	// Create
	public BinarySearchTree(int[] array){
		
		nodeNum = array.length;
		for(int i = 0; i < nodeNum; i++){
			insert(array[i]);
		}	
		findFifthMax(5,root);
		System.out.println("The fifth maximum number in the array is " + findFifthMax(5,root).key + "  !!!");
	}
		
	// is the tree empty?
	public boolean isEmpty(){
		if(root == null){
			return true;
		}
		else{
			return false;
		}
	}
	
	// Insert a node
	public void insert(int key){
		Node parentNode = null;
		Node newNode = new Node(key,null,null,null);
		Node pNode = root;
		
		if(root==null){
			root = newNode;
			return;
		}
		
		while(pNode!=null){
			parentNode = pNode;
			if(key<pNode.key){
				pNode=pNode.leftChild;		
			}
			else if(key>pNode.key){
				pNode=pNode.rightChild;
			}
			else{
				return;
			}
		}
		
		if (key < parentNode.key) {
			parentNode.leftChild = newNode;
			newNode.parent = parentNode;
			} 
		else {
			parentNode.rightChild = newNode;
			newNode.parent = parentNode;
		}
		
		return;
	}
	
	
	// The recursive solution to find the fifth maximum number of the binary search tree
	private  Node findFifthMax(int k,Node node){
			
		int rightSons = getRightSons(node);
			
		if(rightSons>=k){
			count = 0;
			return findFifthMax(k,node.rightChild); 
		}
			
		else if(rightSons==k-1){
			return node;
		}
			
		else{
			count = 0;
			return findFifthMax(k-rightSons-1,node.leftChild);
		}
	}

	// Count the nodes number of a node's right subtree
	private int getRightSons(Node node) {
		if(node.rightChild==null){
			return 0;
		}
		else{
			countSons(node.rightChild);
			return 1+count;
		}
	}
	
	// Count the nodes number of a node
	private void countSons(Node node){
		
		if(node.leftChild!=null){
			count++;
			countSons(node.leftChild);
		}
		
		if(node.rightChild!=null){
			count++;
			countSons(node.rightChild);
		}
	}
	
}

