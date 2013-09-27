package sortingAlgorithms;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

//This class write an array to a txt file with one
//element one line each
//
//Inputs: input array, filename
//Output: array written in the file
//
//author: Wang Jingyi

public class WriteToTxt {
	
	// This constructor takes the filename and the array that wants to be wrote into the file as input
	// and write the array into the file with one element only in one line
	public WriteToTxt(int[] Array, File filename){	
		// The method ArrayDisplay will write the input ArrayList 'Array' to the input File 'filename'
		// in the way that one line with only one element
		try{
			PrintWriter outPut = new PrintWriter(new FileWriter(filename));
			for(int i=0;i<Array.length;i++)
				outPut.println(Array[i]+"\n");
			outPut.close();
		} catch (IOException e) {
			e.printStackTrace();
		}	
		// The file is wrote with one array element in each line sequentially
	}
}
