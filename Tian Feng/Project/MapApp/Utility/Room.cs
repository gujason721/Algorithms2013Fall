using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geometry;

namespace MapApp.Utility
{
    class Room
    {
#region Member Variables
        private IPolygon _shape;
        private int _type;
        private int _subtype;
        private Room _neighbor;
        private List<Room> _neighbors;
        private int _neighborCount;
        private Road _road;
        private int _id;
        private int _fid;
#endregion

#region Attributes
        public IPolygon Shape
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

        public int FID
        {
            get
            {
                return _fid;
            }
            set
            {
                _fid = value;
            }
        }

        public double Area
        {
            get
            {
                if (null == _shape)
                    return 0;

                return ((IArea)_shape).Area;
            }
        }

        public double Perimeter
        {
            get
            {
                if (null == _shape)
                    return 0;

                return _shape.Length;
            }
        }

        public double Factor1
        {
            get
            {
                if (null == _shape)
                    return 0;

                double temp = 4 * Math.PI * Area / (Perimeter * Perimeter);
                return (temp > 1.0) ? 1.0 : temp;
            }
        }

        public double Factor2
        {
            get
            {
                if (null == _shape)
                    return 0;

                ITopologicalOperator pTop = _shape as ITopologicalOperator;
                double chA = ((IArea)pTop.ConvexHull()).Area;

                double temp = Area / chA;
                return (temp > 1.0) ? 1.0 : temp;
            }
        }

        public int Type
        {
            get
            {
                return _type;
            }
            set
            {
                _type = value;
            }
        }

        public int Subtype
        {
            get
            {
                return _subtype;
            }
        }

        public int SubtypeIndex
        {
            get
            {
                return DataUtility.SubtypeToIndex(_subtype);
            }
            set
            {
                _subtype = DataUtility.IndexToSubtype(value);
            }
        }

        public Room Neighbor
        {
            get
            {
                return _neighbor;
            }
            set
            {
                _neighbor = value;
            }
        }

        public List<Room> Neighbors
        {
            get
            {
                return _neighbors;
            }
            set
            {
                _neighbors = value;
            }
        }

        public int NeighborCount
        {
            get
            {
                return _neighborCount;
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
#endregion

#region Member Functions
        public Room(IPolygon pShape, int id)
        {
            _shape = pShape;
            _type = -1;
            _subtype = -1;
            _neighborCount = 0;
            _neighbor = null;
            _neighbors = new List<Room>();
            _id = id;
            _road = null;
        }
#endregion
    }
}
