using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    class Program
    {

        static void Main(string[] args)
        {
            ShortestTimeFinder stf = new ShortestTimeFinder();
            stf.loadAdjacencyList(@"./nb_trace.txt");
            stf.loadSchedule(@"./schedule_trace.txt");

            Console.Write(@"Please enter the source node ID followed by the return key: ");
            int sourceID = int.Parse(Console.ReadLine());

            Console.Write(@"Please enter the destination node ID followed by the return key: ");
            int destID = int.Parse(Console.ReadLine());

            int shortestTime = stf.findShortestTime(sourceID, destID);

            Console.WriteLine(@"Shortest Time: " + shortestTime);

            Console.Write(@"Press any key to quit.");
            Console.ReadKey();
        }

        class ShortestTimeFinder
        {
            Dictionary<int, HashSet<int>> forwardLinkingLookup { get; set; }
            Dictionary<int, HashSet<int>> backwardLinkingLookup { get; set; }
            Dictionary<int, HashSet<int>> schedule { get; set; }
            
            public int periodLength { get; private set; }

            public int findShortestTime(int sourceID, int destID)
            {
                // HashSet<int> visited = new HashSet<int>();
                Queue<int> scheduledToVisit = new Queue<int>();
                Dictionary<int, Dictionary<int,int>> timeToDest = new Dictionary<int, Dictionary<int,int>>();

                if (backwardLinkingLookup == null)
                    generateBackwardLookup();


                scheduledToVisit.Enqueue(destID);
                // add the zeros to the timesToDest table for the paths from destID to itself
                Dictionary<int,int> destIDTimesToDest = new Dictionary<int,int>();
                foreach (int onTime in schedule[destID]) destIDTimesToDest.Add(onTime,0);
                timeToDest.Add(destID, destIDTimesToDest);

                while (scheduledToVisit.Count != 0)
                {
                    // begin processing the next node
                    int currentNode = scheduledToVisit.Dequeue();

                    // mark the current node visited
                    // visited.Add(currentNode);

                    // for all the incoming neighbors
                    if (backwardLinkingLookup.ContainsKey(currentNode))
                    {
                        foreach (int incomingNeighborID in backwardLinkingLookup[currentNode])
                        {
                            // add this neighbor to the list scheduled to be visited
                            //if (!visited.Contains(incomingNeighborID))
                                scheduledToVisit.Enqueue(incomingNeighborID);

                            Dictionary<int, int> thisNeighborTimesToDest = new Dictionary<int, int>();

                            // compute the times from incomingNeighborID to destID
                            foreach (int neighborOnTime in schedule[incomingNeighborID])
                            {
                                int shortestTimeThisNeighborOnTime = int.MaxValue;

                                foreach (int currentNodeOnTime in schedule[currentNode])
                                {
                                    int edgeTime = waitTime(neighborOnTime, currentNodeOnTime);
                                    int totalTimeToDest = edgeTime + timeToDest[currentNode][currentNodeOnTime];
                                    if (totalTimeToDest < shortestTimeThisNeighborOnTime)
                                        shortestTimeThisNeighborOnTime = totalTimeToDest;
                                }

                                thisNeighborTimesToDest.Add(neighborOnTime, shortestTimeThisNeighborOnTime);
                            }

                            if (!timeToDest.ContainsKey(incomingNeighborID))
                                timeToDest.Add(incomingNeighborID,thisNeighborTimesToDest);
                            else {
                                foreach (KeyValuePair<int,int> keyVal in thisNeighborTimesToDest)
                                    if (timeToDest[incomingNeighborID].ContainsKey(keyVal.Key))
                                        if (timeToDest[incomingNeighborID][keyVal.Key] < keyVal.Value)
                                            timeToDest[incomingNeighborID][keyVal.Key] = keyVal.Value;
                            }
                        }
                    }
                }

                int shortestTime = Int32.MaxValue;
                foreach (KeyValuePair<int,int> timePair in timeToDest[sourceID]){
                    if (timePair.Value < shortestTime) shortestTime = timePair.Value;
                }

                return shortestTime;
            }

            /*
            // beginning at "startNode" at time "startTime", what is the shortest time to destNode?
            int shortestOneHopTravelTime(int startTime, int startNode, int destNode)
            {
                // if there is no path between the two nodes, return INT_MAX
                if (!forwardLinkingLookup[startTime].Contains(destNode))
                    return int.MaxValue;

                int shortestTime = int.MaxValue;
                foreach (int onTime in schedule[destNode])
                {
                    int wt = waitTime(startTime, onTime);
                    if (wt < shortestTime)
                        shortestTime = wt;
                }

                return shortestTime;
            }*/

            int mod(int x, int m)
            {
                return (x % m + m) % m;
            }

            // starting at "startTime", how long do we have to wait before "onTime"?
            int waitTime(int startTime, int onTime)
            {
                return mod((onTime - startTime),periodLength);
            }

            public ShortestTimeFinder()
            {
                forwardLinkingLookup = new Dictionary<int, HashSet<int>>();
                schedule = new Dictionary<int, HashSet<int>>();
                periodLength = 200;
            }

            public void generateBackwardLookup()
            {
                backwardLinkingLookup = new Dictionary<int, HashSet<int>>();

                foreach (KeyValuePair<int, HashSet<int>> destinations in forwardLinkingLookup)
                    foreach (int destID in destinations.Value)
                    {
                        if (!backwardLinkingLookup.ContainsKey(destID))
                            backwardLinkingLookup.Add(destID,new HashSet<int>());
                        backwardLinkingLookup[destID].Add(destinations.Key);
                    }
            }

            public void loadAdjacencyList(String filename)
            {
                StreamReader r = new StreamReader(File.OpenRead(filename));

                while (!r.EndOfStream)
                {
                    String line = r.ReadLine();
                    String[] lineValuesSt = line.Split(' ');
                    int nodeID = int.Parse(lineValuesSt[0]);
                    HashSet<int> neighbors = new HashSet<int>();
                    int numNeighbors = int.Parse(lineValuesSt[1]);

                    // compile the list of neighbors
                    if (numNeighbors > 0)
                       for (int i = 2; i < lineValuesSt.Length; i++) neighbors.Add(int.Parse(lineValuesSt[i]));

                    // enter the adjacency data for the current line
                    forwardLinkingLookup.Add(nodeID, neighbors);
                }
            }

            public void loadSchedule(String filename)
            {
                StreamReader r = new StreamReader(File.OpenRead(filename));

                while (!r.EndOfStream)
                {
                    String line = r.ReadLine();
                    String[] lineValuesSt = line.Split(' ');
                    int nodeID = int.Parse(lineValuesSt[0]);
                    HashSet<int> times = new HashSet<int>();
                    int numOnTimes = int.Parse(lineValuesSt[1]);

                    // compile the list of onTimes
                    if (numOnTimes > 0)
                        for (int i = 2; i < lineValuesSt.Length; i++) times.Add(int.Parse(lineValuesSt[i]));

                    // enter the adjacency data for the current line
                    schedule.Add(nodeID, times);
                }
            }
        }
    }
}
