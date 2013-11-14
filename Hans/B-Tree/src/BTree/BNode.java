package BTree;

public class BNode {
	int n;
	boolean leaf;
	BNode [] c;
	int [] key;
	
	public BNode (BTree T) {
		c = new BNode [2*T.t];
		key = new int [2*T.t - 1];
	}
	
	// returns the index of k in keys[] or -1 if not found
	public int findKey(int k){
		// TODO: replace this with a binary search
		for (int i = 0; i < n; i++) 
			if (key[i] == k) return i;
		return -1;
	}
	
	public void deleteKey(int kIdx){
		n--;
		for (int i = kIdx; i < n; i++) key[i] = key[i+1];
		key[n] = 0;
	}
	
	public void deleteKeyAndChild(int kIdx, int cIdx){
		n--;
		for (int i = cIdx; i < n+1; i++) c[i]=c[i+1];
		for (int i = kIdx; i < n; i++) key[i] = key[i+1];
		key[n] = 0;
		c[n+1]=null;
	}
	
	public int findSubTree(int k){
		// TODO: replace this with a binary search
		for (int i = 0; i < n; i++) 
			if (key[i] > k) return i;
		return n;
	}

	public int lastKey() {
		return key[n-1];
	}

	public BNode lastChild() {
		return c[n];
	}

	public void prependKeyAndChild(int key, BNode child) {
		n++;
		int i;
		for (i = n-1; i >= 1; i--){
			this.key[i] = this.key[i-1];
			c[i+1] = c[i];
		}
		c[1] = c[0]; c[0] = child;
		this.key[0] = key;
	}

	public void deleteLastChildAndKey() {
		c[n] = null;
		// we don't need to delete the key, just decrement n.
		n--;
	}

	public int firstKey() {
		return key[0];
	}

	public BNode firstChild() {
		return c[0];
	}

	public void appendKeyAndChild(int key, BNode child) {
		n++;
		this.key[n-1] = key;
		c[n] = child;
	}

	public void deleteFirstChildAndKey() {
		n--;
		int i;
		for (i=0; i < n; i++){
			key[i] = key[i+1];
			c[i] = c[i+1];
		}
		c[n] = c[n+1];
		c[n+1] = null;
	}
}
