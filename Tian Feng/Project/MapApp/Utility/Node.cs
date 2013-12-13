using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geometry;

namespace MapApp.Utility
{
    enum NodeType
    {
        Shop = 1,
        Food = 2,
        Lifestyle = 3,
        Facility = 4,
        Road = 5,
        Entrance = 6,
        Start = 7,
        End = 8,
        Unknown = 9
    }

    class Node
    {
        private IPoint _shape;
        private int _id;
        private NodeType _type;
        private List<Edge> _edges;
        private Room _room;
        private Road _road;

        public void SetType(NodeType type)
        {
            this._type = type;
        }

        public IPoint Shape
        {
            get
            {
                return _shape;
            }
        }

        public int ID
        {
            get
            {
                return _id;
            }
        }

        public Room Room
        {
            get
            {
                return _room;
            }
            set
            {
                _room = value;
            }
        }

        public Road Road
        {
            get
            {
                return _road;
            }
            set
            {
                _road = value;
            }
        }

        public string Type
        {
            get
            {
                string strType = "Road";
                switch (this._type)
                {
                    case NodeType.Shop:
                        strType = "Shop";
                        break;
                    case NodeType.Food:
                        strType = "Food";
                        break;
                    case NodeType.Lifestyle:
                        strType = "Lifestyle";
                        break;
                    case NodeType.Facility:
                        strType = "Facility";
                        break;
                    case NodeType.Road:
                        strType = "Road";
                        break;
                    case NodeType.Entrance:
                        strType = "Entrance";
                        break;
                    case NodeType.Start:
                        strType = "Start";
                        break;
                    case NodeType.End:
                        strType = "End";
                        break;
                    case NodeType.Unknown:
                        strType = "Unknown";
                        break;
                    default:
                        break;
                }

                return strType;
            }
        }

        public int TypeIndex
        {
            get
            {
                return (int)_type;
            }
        }

        public List<Edge> Edges
        {
            get 
            {
                return _edges;
            }
            set
            {
                _edges = value;
            }
        }

        public Node(IPoint pt, int id, NodeType type)
        {
            this._shape = pt;
            this._id = id;
            this._type = type;
            this._room = null;
            this._edges = new List<Edge>();
        }
    }
}
