package china.zhouyiren.heapsort;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class heapsort {
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
		HeapSort(nums);  //进行堆排序 heap sort
		long end = System.nanoTime();	 //结束计时 end timing
		long useTime = end - start;
		
		FileWriter fw=new FileWriter(new File("output_heapsort.txt"));
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
	//main

	public static int[] HeapSort(int[] args){ //堆排序 heap sort
		BuildMaxHeap(args);
		heapsortx(args);
		return args;
	}
	//heap sort
	
	public static void BuildMaxHeap(int[] args){ //建立最大堆 build max heap
		for(int i=args.length/2;i>=0;i--){
			MaxHeapify(args,args.length,i);
		}
	}
	//build max heap
	
	public static void MaxHeapify(int[] args,int HeapSize,int i){ //堆最大化 max heap
		if(i<HeapSize){
			int lchild=2*i+1;
			int rchild=2*i+2;
			int k=i;
			if(lchild<HeapSize&&args[lchild]>args[i]){
				k=lchild;
			}
			if(rchild<HeapSize&&args[rchild]>args[k]){
				k=rchild;
			}
			if(k!=i){
				Exchange(args,i,k);
				MaxHeapify(args,HeapSize,k);
			}
		}
	}
	//max heap
	
	public static void Exchange(int[] args, int i, int j){ //交换 exchange
		int k=args[j];
		args[j]=args[i];
		args[i]=k;
	}
	//exchange
	
	public static void heapsortx(int[] args){ //进行排序 do heap sort
		int[] temp=args;
		for(int i=temp.length-1;i>=1;i--){
			Exchange(temp,i,0);
			MaxHeapify(temp,i,0);
		}
		args=temp;
	}
	//do heap sort
}

