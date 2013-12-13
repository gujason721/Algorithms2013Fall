using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geometry;

namespace MapApp.Utility
{
    class Road
    {
#region Member Variables
        private IPolyline _shape;
        private double _width;
        private bool _bIsMainPath;
        private int _id;
#endregion

#region Attributes
        public IPolyline Shape
        {
            get
            {
                return _shape;
            }
        }

        public double Length
        {
            get
            {
                if (null == _shape)
                    return 0;

                return _shape.Length;
            }
        }

        public double Width
        {
            get
            {
                return _width;
            }
        }

        public int ID
        {
            get
            {
                return _id;
            }
        }

        public bool IsMainPath
        {
            get
            {
                return _bIsMainPath;
            }
        }
#endregion

#region Member Functions
        public Road(IPolyline pShape, int id, double width, bool isMainPath)
        {
            _shape = pShape;
            _width = width;
            _bIsMainPath = isMainPath;
            _id = id;
        }

        public Road(IPolyline pShape, int id, double width, int isMainPath)
        {
            _shape = pShape;
            _width = width;

            if (isMainPath == 0)
                _bIsMainPath = false;
            else
                _bIsMainPath = true;

            _id = id;
        }
#endregion
    }
}
