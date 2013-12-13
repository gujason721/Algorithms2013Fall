import java.util.ArrayList;
import java.util.List;


public interface svmInputFormat {
	
	public int vecNum = 0;		// # variable to do SVM
	public int rowNum = 0;		// # test cases
	public int[] index= new int[vecNum];	// 
	public List<int[]> varValue = new ArrayList<int[]>(vecNum);
	public int[] lable = new int[rowNum];
	public StringBuilder dataSb = new StringBuilder();
	
}
