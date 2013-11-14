package skiplist;

public class SLNode {
	int key;
	SLNode [] next;
	
	public SLNode (int key, int level){
		next = new SLNode [level+1];
		this.key = key;
	}
	
	public SLNode () {
		next = null;
	}
	
	public int level(){
		return next.length - 1;
	}
}
