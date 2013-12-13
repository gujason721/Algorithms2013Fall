import java.io.*;
import java.util.*;

public class heapSort{
    public static void main(String args[]) throws FileNotFoundException, IOException 
    {
        //long startTime = System.currentTimeMillis();
        Scanner scan = new Scanner(new File("C:\\input.txt"));
        int counter = 0;
        while(scan.hasNextInt())
        {
            counter++;
            scan.nextInt();
        }
        
        Scanner scanN = new Scanner(new File("C:\\input.txt"));
        int a[] = new int [counter];
        for (int i=0; i<counter; i++)
        {
            a[i] = scanN.nextInt();
        }
        
        scan.close();
        scanN.close();
        long startTime = System.currentTimeMillis();
        int end = a.length -1;
        heapify(a, counter);
        while (end > 0)
        {
            exch(a, end, 0);
            end = end -1;
            siftDown(a, 0, end);
        }
      
        PrintWriter writer = new PrintWriter (new FileWriter("C:\\output.txt"));
        
        for(Integer number : a)
        {
            writer.println(number);
            //System.out.print(number + " ");
        }
        //System.out.println();
        writer.close();
        long endTime = System.currentTimeMillis();
        long totalTime = endTime - startTime;
        System.out.println("Running Time= "+totalTime+"ms");     
    }
    
    public static void heapify (int[] a, int counter)
    {
        int start = (counter-2)/2;
        while (start>= 0)
        {
            siftDown(a, start, counter-1);
            start = start-1;
        }
    }
    
    public static void siftDown (int[] a, int start, int end)
    {
        int root = start;
        while (root*2+1 <= end)
        {
            int child = root*2+1;
            int temp = root;
            
            if (a[temp] < a[child])
                temp = child;
            if (child+1 <= end && a[temp] < a[child+1])
                temp = child+1;
            if (temp != root){
                exch(a,root,temp);
                root = temp;
            }
            else
                return;
        }
    }
    
    public static void exch(int[] a, int i, int j)
    {
        int Temp = a[i];
        a[i] = a[j];
        a[j] = Temp;
    }
}