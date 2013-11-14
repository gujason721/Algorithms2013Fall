package china.zhouyiren.mergesort;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class mergesort {
	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException{
		List list = new ArrayList();
		int[] nums = null;
		String line = null;
		//BufferReader 读入缓冲区
		BufferedReader br = new BufferedReader(new FileReader("input.txt"));
		while((line = br.readLine()) != null){
		    list.add(line);
		}
		br.close();
		nums = new int[list.size()];
		for(int i=0;i<list.size();i++){
			String s = (String) list.get(i);
			nums[i] = Integer.parseInt(s);
		}
		
		long start = System.nanoTime(); //开始计时 begin timing
		MergeSort(nums);  //进行归并排序 merge sort
		long end = System.nanoTime();	 //结束计时 end timing
		long useTime = end - start;
		
		FileWriter fw=new FileWriter(new File("output_mergesort.txt"));
		//写入txt文件 write txt file
		BufferedWriter  bw=new BufferedWriter(fw);
		for(int i: nums){
            bw.write(i+"\t\n");
        }
		bw.write("UseTime="+useTime+"ns");
        bw.close();
        fw.close();
        System.out.println("UseTime="+useTime+"ns");
	}

	public static int[] MergeSort(int[] args){ //归并排序 merge sort
		if(args.length==1){ //single num
			return args;
		}
		int h=args.length/2;
        //bisect array
		int[] args1=new int[h];
		int[] args2=new int[args.length-h];
		System.arraycopy(args, 0, args1, 0, args1.length);
        System.arraycopy(args, h, args2, 0, args2.length);
        MergeSort(args1);
		MergeSort(args2);
		int i=0;
		int j=0;
		int k=0;
		while(i<args1.length&&j<args2.length){
			if(args1[i]<args2[j]){
				args[k]=args1[i];
				i++;
			}
			else{
				args[k]=args2[j];
				j++;
			}
			k++;
		}
		for(;i<args1.length;i++){
            args[k] = args1[i];
            k++;
		}
		for(;j<args2.length;j++){
            args[k] = args2[j];
            k++;
		}
		return args;
	}	
	//merge sort
}

