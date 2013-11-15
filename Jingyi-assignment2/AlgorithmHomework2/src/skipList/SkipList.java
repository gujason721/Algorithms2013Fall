package skipList;

public class SkipList{
	
	public static final double P = 1/Math.E;
	public final int MAX_LEVEL = 5;
	
	public final SkipNode header = new SkipNode(MAX_LEVEL, -1);
	public int level = 0;
	
	public void insert(int value){
		SkipNode x = header;
		SkipNode[] update = new SkipNode[MAX_LEVEL + 1];
		int lvl = randomLevel();
		
		for (int i = level; i >= 0; i--){
			while (x.forward[i] != null && x.forward[i].value < value){
				x = x.forward[i];
			}	
			update[i] = x; 
		}
		x = x.forward[0];
		if (x == null || x.value!=value){	
			if (lvl > level){
				for (int i=level+1; i<=lvl; i++){
					update[i] = header;
				}
				level = lvl;
			}
		}
		x = new SkipNode(lvl, value); 
		for (int i = 0; i <= lvl; i++){
			x.forward[i] = update[i].forward[i];
			update[i].forward[i] = x;
		}
		
		System.out.println("The value " + value + " is successfully inserted in the skip list");
	}
	
	
	
	public void delete(int value){
		SkipNode x = header;
		SkipNode[] update = new SkipNode[MAX_LEVEL + 1];
		for (int i = level; i >= 0; i--){
			while (x.forward[i] != null && x.forward[i].value<value){
				 x = x.forward[i];
			}
			update[i] = x; 
		}
		x = x.forward[0];
		if (x.value==value){
			for (int i = 0; i <= level; i++){
				if (update[i].forward[i] != x){break;}
				update[i].forward[i] = x.forward[i];
			}
			while (level > 0 && header.forward[level] == null){
				 level--;
			}
		}
		System.out.println("The value " + value + " is successfully deleted from the skip list");
	}
	
	public boolean contains(int searchValue){
		System.out.println("Try to search " + searchValue + " in the skip list...");
		SkipNode x = header;
		for (int i = level; i >= 0; i--){
			while (x.forward[i] != null && x.forward[i].value<searchValue){
				x = x.forward[i];
			}
		}
		x = x.forward[0];
		boolean result = x != null && x.value==searchValue;
		if(result){
			System.out.println("***The value " + searchValue + " is in the skip list...");
		}
		else{
			System.out.println("***The value " + searchValue + " is not in the skip list...");
		}
		return result;
	}
	
	
	public int randomLevel(){
		int lvl = (int) (Math.log(1. - Math.random()) / Math.log(1. - P));
		return Math.min(lvl, MAX_LEVEL);
	}
	
	public String toString(int level){
		StringBuilder sb = new StringBuilder();
		sb.append("{"); 
		SkipNode x = header.forward[level];
		while (x != null){
			sb.append(x.value);
			x = x.forward[level];
			if (x != null){sb.append(",");}
		}
		sb.append("}");
		return sb.toString();
	}
	
	public void displaySkipList(){
		System.out.println("\nThe current skip list looks like this(different every time due to the random level generated):");
		for(int i=MAX_LEVEL; i>-1; i--){
			String result = this.toString(i);
			System.out.println("The " + (i+1) + "th level of the skiplist is " + result + "\n");
		}
	}
}


