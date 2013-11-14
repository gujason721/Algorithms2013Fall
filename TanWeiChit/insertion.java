import java.io.*;
import java.util.*;

public class insertion
{
    public static void main (String[]args) throws IOException, FileNotFoundException
    {
        
        FileReader file = new FileReader("C:\\input.txt");
        Scanner input = new Scanner(file);
        List<Integer> list = new ArrayList<Integer>();
        while (input.hasNext())
        {
            list.add(input.nextInt());
        }
        input.close();
	long startTime = System.currentTimeMillis();
        for (int i=0; i < list.size(); i++)
        {
            int j=i;
            int key = list.get(i);
                
            while((j>0) && (list.get(j-1) > key)){
                list.set(j, list.get(j-1));
                j--;
            }
            list.set(j, key);
        }
            
        PrintWriter writer = new PrintWriter (new FileWriter("C:\\output.txt"));
                      
        for (Integer number : list)
        {
            writer.println(number);
           // System.out.print(number + " ");
        }
        //System.out.println();
        writer.close(); 
        long endTime = System.currentTimeMillis();
        long totalTime = endTime - startTime;
        System.out.println("Running Time= "+totalTime+"ms");
    }
}



    