using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geometry;

namespace MapApp.Utility
{
    class Edge
    {
        private ILine _shape;
        private Node _fromNode;
        private Node _toNode;
        private double _width;
        private int _id;

        public ILine Shape
        {
            get
            {
                return _shape;
            }
        }

        public Node FromNode
        {
            get
            {
                return _fromNode;
            }
        }

        public Node ToNode
        {
            get
            {
                return _toNode;
            }
        }

        public double Width
        {
            get
            {
                return _width;
            }
            set
            {
                _width = value;
            }
        }

        public double Length
        {
            get
            {
                if (null == _shape)
                    return 0.0;

                return _shape.Length;
            }
        }

        public int ID
        {
            get
            {
                return _id;
            }
        }

        public double Angle
        {
            get
            {
                if (null == _shape)
                    return 0.0;

                return _shape.Angle;
            }
        }

        public Edge(Node fromNode, Node toNode, int id, double width)
        {
            this._fromNode = fromNode;
            this._toNode = toNode;
            this._id = id;
            this._shape = new LineClass();
            this._shape.SpatialReference = fromNode.Shape.SpatialReference;
            this._shape.FromPoint = this._fromNode.Shape;
            this._shape.ToPoint = this._toNode.Shape;
            this._width = width;
        }
    }
}
