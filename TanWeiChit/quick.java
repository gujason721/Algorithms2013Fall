import java.io.*;
import java.util.*;

public class quick{
    public static void main(String args[]) throws FileNotFoundException,IOException
    {
        //long startTime = System.currentTimeMillis();
        Scanner scan = new Scanner(new File("C:\\input.txt"));
        int counter = 0;
        while (scan.hasNextInt())
        {
            counter++;
            scan.nextInt();
        }
        Scanner scanN = new Scanner(new File("C:\\input.txt"));
        int array[] = new int [counter];
        for(int i=0; i<counter; i++)
        {
            array[i] = scanN.nextInt();
        }
        scan.close();
        scanN.close();
        
       	long startTime = System.currentTimeMillis();
        quick_sort(array,0,array.length-1);
	PrintWriter writer = new PrintWriter (new FileWriter("C:\\output.txt"));
        
        for (Integer number : array)
        {
            writer.println(number);
            //System.out.print(number+" ");
        }
        //System.out.println();
        writer.close();
        long endTime = System.currentTimeMillis();
        long totalTime = endTime - startTime;
        System.out.println("Running Time= "+totalTime+"ms");
    }
    
    public static void quick_sort(int array[], int lo, int hi)
    {
        if(hi <= lo){
            return;
        }
        int i = lo;
        int j = hi + 1;
        while(lo < hi)
        {
            while ( array[++i] < array[lo])
                if(i == hi) break;
            while ( array[lo] < array[--j])
                if(j == lo) break;
            if( i >=j ) break;
            int Temp = array[i];
            array[i] = array[j];
            array[j] = Temp;
        }
        int Temp = array[lo];
        array[lo] = array[j];
        array[j] = Temp;
     
        quick_sort(array, lo, j-1);
        quick_sort(array, j+1, hi);
    }   
}