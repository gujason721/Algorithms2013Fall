using System;
using System.Collections.Generic;
using System.Text;

namespace FinalExam.ADT
{
    class Graph
    {
        #region Member Variables
        private bool _isDirected;
        private List<Vertex> _vertices;
        private EdgeMatrix _edges;
        #endregion

        #region Member Functions
        public Graph(bool isDirected, List<Vertex> vertices, EdgeMatrix edges)
        {
            _isDirected = isDirected;
            _vertices = vertices;
            _edges = edges;
        }

        public List<Vertex> GetAdjacentVertices(Vertex v)
        {
            List<Vertex> result = new List<Vertex>();
            for (int i = 0; i < _edges.Dimension; i++)
                if (_edges.GetWeight(v.ID, i) > 0)
                    result.Add(_vertices[i]);

            return result;
        }

        public void Print()
        {
            string strFlag = "--";
            if (IsDirectedGraph)
                strFlag = "->";

            for (int i = 0; i < _vertices.Count; i++)
            {
                Console.WriteLine("{0}", i);
                List<Vertex> adjs = GetAdjacentVertices(_vertices[i]);
                for (int j = 0; j < adjs.Count; j++)
                    Console.WriteLine("{0}{1}", strFlag, adjs[j].ID);
            }
        }
        #endregion

        #region Member Attributes
        public List<Vertex> V
        {
            get { return _vertices; }
        }

        public EdgeMatrix E
        {
            get { return _edges; }
        }

        public bool IsDirectedGraph
        {
            get { return _isDirected; }
        }
        #endregion
    }
}
