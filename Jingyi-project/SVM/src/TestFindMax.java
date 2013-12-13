// This class is the first demo for our method,
// which checks a very tough task in debugging area.
// That is to check whether a program of returning 
// the largest element in an array with a mistake of the
// loop number.
// @ author: Wang Jingyi
// last modified: 8/12/2013

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;


public class TestFindMax{
	
	public static int vecNum = 0;		// # variable to do SVM
	public static int rowNum = 0;		// # test cases
	public static int[] index= new int[vecNum];	// 
	public static List<int[]> varValue = new ArrayList<int[]>(vecNum);
	public static int[] lable = new int[rowNum];
	public static StringBuilder dataSb = new StringBuilder();
	
	
	
	static int length = 4;
	int[] arr = new int[length];
	int maxNum = -1;
	
	public void testIt() throws IOException{

		TestFindMax newTFM=new TestFindMax();	
		List<int[]> res= newTFM.generateSmsArr(0, new int[length]);

		// initiate arrays to store variables
		for(int i=0;i<=length;i++){
			int[] tmp = new int[res.size()];
			lable = new int[res.size()];
			varValue.add(i, tmp);
		}

		int s = 0;

		for(int[] arr:res){

			for(int j=0;j<arr.length;j++){
				varValue.get(j)[s] = arr[j];
			}

			varValue.get(length)[s] = -1;
			for(int j=0;j<arr.length-1;j++){
				if(arr[j]>varValue.get(length)[s]){
					varValue.get(length)[s] = arr[j];
				}
			}

			int maxNum1 = -1;
			for(int j=0;j<arr.length;j++){
				if(arr[j]>maxNum1){
					maxNum1 = arr[j];
				}
			}
			if(varValue.get(length)[s]!=maxNum1){
				lable[s] = -1;
			}
			else{
				lable[s] = 1;
			}	
			s++;
		}
		int[] index = new int[length+1];
		for(int i=0;i<length+1;i++)
		{
			index[i] = i;
		}
		fileFactory.preFile(index, lable, varValue);

	}
	
	public static void main(String[] args){
		TestFindMax newTFM=new TestFindMax();
		List<int[]> res= newTFM.generateSmsArr(0, new int[length]);
		StringBuilder sb = new StringBuilder();
		for(int[] arr: res){
			for(int i=0;i<arr.length;i++){
				sb.append(arr[i]);
			}
			sb.append('\n');
		}
		System.out.println(sb.toString());
	}
	
	
	private List<int[]> generateSmsArr(int idx, int[] arrs){
		List<int[]> innerArr=new ArrayList<int[]>();
			for(int i=0;i<=5;i++){
				int[] arr1=arrs.clone();			
				arr1[idx]=i;
				if(idx<length-1){
					innerArr.addAll(generateSmsArr(idx+1,arr1));
				}else{
					innerArr.add(arr1);
				}
				
			}
		return innerArr;		
	}
	
	
	private List<int[]> generateRdArr(int length) {
		List<int[]> innArr=new ArrayList<int[]>();
		int[] randomData = new int[length];
		for(int i=0;i<1000;i++){
			for(int j=0;j<length;j++){
				Random randomGnt = new Random();
				randomData[j] = randomGnt.nextInt(10);
			}
			innArr.add(i, randomData);
		}
		return innArr;
	}

	
}
