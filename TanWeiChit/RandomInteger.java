import java.io.*;
import java.util.Random;

public final class RandomInteger {
    public static final void main (String[] args) throws IOException
    {
        PrintWriter writer = new PrintWriter (new FileWriter("C:\\input.txt"));
        Random randomGenerator = new Random();
        for(int idx =0; idx< 1000; idx++){
            int randomInt = randomGenerator.nextInt(10000);
            writer.println(randomInt);
        }
        writer.close();
    }
}

    
 
        