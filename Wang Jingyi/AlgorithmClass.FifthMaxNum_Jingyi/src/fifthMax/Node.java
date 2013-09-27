package fifthMax;


public class Node {
	
	int key;
	Node leftChild;
	Node rightChild;
	Node parent;

	public Node(int key, Node leftChild, Node rightChild, Node parent) {
	this.key = key;
	this.leftChild = leftChild;
	this.rightChild = rightChild;
	this.parent = parent;
	}
	
	public int getKey(){
		return key;
	}
}
