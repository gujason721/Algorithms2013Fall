import java.io.*;
import java.util.*;

public class mSort
{
    public static void main (String[]args) throws FileNotFoundException, IOException
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
        int array[] = new int [counter];
        for(int i=0; i<counter; i++)
        {
            array[i] = scanN.nextInt();
        }
        
        scan.close();
        scanN.close();
        
        
        long startTime = System.currentTimeMillis();
        
        int N = array.length;
        int[] aux = new int[N];
        sort(array,aux,0,N-1);
        
            
        PrintWriter writer = new PrintWriter (new FileWriter("C:\\output.txt"));
                      
        for (Integer number : array)
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

        
    public static void sort(int array[],int aux[], int lo, int hi)
    {
        if(lo >= hi) {
            return;
        }
        int mid = (lo+hi)/2;
        sort(array, aux, lo, mid);
        sort(array, aux, mid+1, hi);
        int i = lo;
        int j = mid+1;
        
        for(int k=lo; k<=hi; k++)
            aux[k] = array[k];
        
        for(int k=lo; k<=hi; k++)
        {
            if(i>mid)
                array[k]=aux[j++];
            else if(j>hi)
                array[k]=aux[i++];
            else if(aux[j] < aux[i])
                array[k]=aux[j++];
            else
                array[k]=aux[i++];
        }
    }
}

                
