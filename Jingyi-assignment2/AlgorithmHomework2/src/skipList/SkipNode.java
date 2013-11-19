package skipList;

public class SkipNode{
	
	public int value;
	public SkipNode[] forward;
	
	public SkipNode(int level, int value){
		forward = new SkipNode[level+1]; // For node of n level, forward has n+1 nodes, level starts at 0
		this.value = value;
	}	
}
