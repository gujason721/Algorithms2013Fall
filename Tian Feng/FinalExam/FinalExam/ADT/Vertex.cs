using System;
using System.Collections.Generic;
using System.Text;

namespace FinalExam.ADT
{
    enum VertexColor
    {
        White = 0,
        Gray = 1,
        Black = 2
    }

    class Vertex
    {
        #region Member Variables
        private int _id;
        private int _d;
        private int _f;
        private Vertex _predecessor;
        private VertexColor _color;
        private List<int> _t;
        #endregion

        #region Member Functions
        public Vertex(int id)
        {
            _id = id;
            _predecessor = null;
            _color = VertexColor.White;
            _d = Infinity;
            _f = Infinity;
            _t = new List<int>();
        }

        public bool IsNowOn(int time)
        {
            if (_t.Contains(time))
                return true;
            else
                return false;
        }
        #endregion

        #region Member Attributes
        public int ID
        {
            get { return _id; }
        }

        public int d
        {
            get { return _d; }
            set { _d = value; }
        }

        public int f
        {
            get { return _f; }
            set { _f = value; }
        }

        public Vertex pi
        {
            get { return _predecessor; }
            set { _predecessor = value; }
        }

        public VertexColor Color
        {
            get { return _color; }
            set { _color = value; }
        }

        public static int Infinity
        {
            get { return 65535; }
        }

        public List<int> T
        {
            get { return _t; }
            set { _t = value; }
        }
        #endregion
    }
}
