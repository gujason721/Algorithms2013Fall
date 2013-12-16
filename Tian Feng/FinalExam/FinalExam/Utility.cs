using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using FinalExam.ADT;

namespace FinalExam
{
    class Utility
    {
        private static void DfsVisit(Graph G, Vertex u, int time, List<Vertex> result)
        {
            time++;
            u.d = time;
            u.Color = VertexColor.Gray;

            List<Vertex> pAdjs = G.GetAdjacentVertices(u);
            foreach (Vertex v in pAdjs)
            {
                if (v.Color == VertexColor.White)
                {
                    v.pi = u;
                    DfsVisit(G, v, time, result);
                }
            }

            u.Color = VertexColor.Black;
            time++;
            u.f = time;

            result.Insert(0, u);
        }

        private static List<Vertex> TopologicalSort(Graph G)
        {
            List<Vertex> pResult = new List<Vertex>();

            foreach (Vertex u in G.V)
            {
                u.Color = VertexColor.White;
                u.pi = null;
            }

            int time = 0;
            foreach (Vertex u in G.V)
            {
                if (u.Color == VertexColor.White)
                    DfsVisit(G, u, time, pResult);
            }

            return pResult;
        }

        public static void GetShortestTimePath(Graph G, Vertex s, Vertex t, int startTime)
        {
            int[] time = new int[G.V.Count];
            int[] delay = new int[G.V.Count];
            List<Vertex> pTopoList = TopologicalSort(G);

            for (int i = 0; i < G.V.Count; i++)
            {
                delay[i] = Vertex.Infinity;
                G.V[i].pi = null;
            }
            time[s.ID] = startTime;
            delay[s.ID] = 0;

            bool bFindS = false;
            foreach (Vertex u in pTopoList)
            {
                if (u == s)
                    bFindS = true;
                if (u != s && !bFindS)
                    continue;

                List<Vertex> pAdjs = G.GetAdjacentVertices(u);
                foreach (Vertex v in pAdjs)
                {
                    int vTime = 0;
                    int w = FindShortestDelay(v, time[u.ID], out vTime);
                    if (delay[u.ID] + w < delay[v.ID])
                    {
                        delay[v.ID] = delay[u.ID] + w;
                        time[v.ID] = vTime;
                        v.pi = u;
                    }
                }
            }

            Console.WriteLine("Source: {0} Destination: {1} Delay: {2}", s.ID, t.ID, delay[t.ID]);
            Stack<int> pPathStk = new Stack<int>();
            Vertex p = t;
            while (p.ID != s.ID)
            {
                pPathStk.Push(p.ID);
                p = p.pi;
            }

            Console.Write("Path: {0}", s.ID);
            while (pPathStk.Count > 0)
                Console.Write("->{0}", pPathStk.Pop());
            Console.WriteLine();
        }

        private static int FindShortestDelay(Vertex v, int time, out int nearestTime)
        {
            nearestTime = Vertex.Infinity;
            int result = Vertex.Infinity;

            for (int i = 0; i < v.T.Count; i++)
            {
                int temp = Vertex.Infinity;
                if (v.T[i] >= time)
                    temp = v.T[i] - time;
                else
                    temp = v.T[i] + 200 - time;

                if (temp < result)
                {
                    result = temp;
                    nearestTime = v.T[i];
                }
            }

            return result;
        }

        public static Graph GetData(string strNbtPath, string strStPath)
        {
            Graph G = null;
            StreamReader pNbtSR = new StreamReader(strNbtPath);
            StreamReader pStSR = new StreamReader(strStPath);

            try
            {
                List<Vertex> vertices = new List<Vertex>();
                EdgeMatrix edges = new EdgeMatrix(100);

                string strNbt = string.Empty;
                string strSt = string.Empty;

                while (null != (strNbt = pNbtSR.ReadLine()) && null != (strSt = pStSR.ReadLine()))
                {
                    string[] sNbt = strNbt.Trim().Split(' ');
                    string[] sSt = strSt.Trim().Split(' ');

                    int idNbt = int.Parse(sNbt[0]);
                    int idSt = int.Parse(sSt[0]);

                    Vertex v = new Vertex(idNbt);
                    for (int i = 2; i < sNbt.Length; i++)
                        edges.SetWeight(v.ID, int.Parse(sNbt[i]), 1);
                    for (int j = 2; j < sSt.Length; j++)
                        v.T.Add(int.Parse(sSt[j]));
                    vertices.Add(v);
                }
                pNbtSR.Close();
                pStSR.Close();

                G = new Graph(true, vertices, edges);
            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.Message);
                G = null;

                if (null != pNbtSR)
                    pNbtSR.Close();
                if (null != pStSR)
                    pStSR.Close();
            }

            return G;
        }
    }
}
