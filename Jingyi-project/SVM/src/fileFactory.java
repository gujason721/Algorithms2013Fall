import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class fileFactory{
	
	public static int vecNum = 0;		// # variable to do SVM
	public static int rowNum = 0;		// # test cases
	public static int[] index= new int[vecNum];	// 
	public static List<int[]> varValue = new ArrayList<int[]>(vecNum);
	public static int[] lable = new int[rowNum];
	public static StringBuilder dataSb = new StringBuilder();
	
	// update StringBuilder after each round of svm
	public static void updateDataSb(){
		dataSb = new StringBuilder();
	}
	
	// take the selected index of data array, lable, data array list as input
	// and output a txt file with specific format that svm is able to
	// process.
	public static void preFile(int[] index,int[] lable, List<int[]> varValue) throws IOException{
		vecNum = index.length;
		rowNum = varValue.get(0).length;
		String toApp = "";
		for(int p=0;p<rowNum;p++){
			toApp +=lable[p]+"";
			for(int i=0;i<vecNum;i++){
				toApp +=" "+(i+1)+":"+varValue.get(index[i])[p];		
			}
			toApp +="\n";
		}
		dataSb.append(toApp);
		File demo = new File("C:\\Users\\Jingyi\\workspace\\SVM\\trainfile\\demo.txt");
		BufferedWriter bwr = new BufferedWriter(new FileWriter(demo));
		bwr.write(dataSb.toString());
		bwr.flush();
		bwr.close();
	}
	
}
