using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using FinalExam.ADT;

namespace FinalExam
{
    class Program
    {
        static void Main(string[] args)
        {
             Console.WriteLine("Please move the source files to the same folder as the application exists in.");
             string strNbtPath = "nb_trace.txt";
             string strStPath = "schedule_trace.txt";
             Console.ReadKey();
 
             Graph G = null;
             if (!File.Exists(strNbtPath) || !File.Exists(strStPath))
                 Console.WriteLine("Source files do not exist! Please exit and try again.");
             else
             {
                 G = Utility.GetData(strNbtPath, strStPath);
                 if (null == G)
                     Console.WriteLine("Graph initialization failed! Please exit and try again.");
                 else
                 {
                     Console.WriteLine("Please enter the id of the source node:");
                     int sID = int.Parse(Console.ReadLine().Trim());
                     Console.WriteLine("Please enter the id of the destination node:");
                     int dID = int.Parse(Console.ReadLine().Trim());

                     if (sID < 0 || dID > 99 || sID >= dID)
                         Console.WriteLine("Node ID invalid! Please exit and try again.");
                     else
                         Utility.GetShortestTimePath(G, G.V[sID], G.V[dID], 0);
                 }
             }
            Console.ReadKey();
        }
    }
}
