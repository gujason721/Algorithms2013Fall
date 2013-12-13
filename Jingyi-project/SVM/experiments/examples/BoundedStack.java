package examples;

public class BoundedStack {
	 
	  private static final int MaxSize = 3;
	  private int size;
	  private Integer[] data;

	  public BoundedStack() {
	    size = 0;
	    data = new Integer[MaxSize];
	  }

	  public int size() {
	  	return size;
	  }
	  
	  public boolean push(Integer element) throws Exception {
	    if (size == MaxSize) {
	      throw new Exception("Push on full stack.");
	    }
	    data[size] = element;
	    size++;
	    return true;
	  }

	  public Integer pop() throws Exception {
	    if (size == 0) {
	      throw new Exception("Pop an empty stack.");
	    }
	    Integer ret = data[size - 1];
	    size--;
	    return ret;
	  }

}
