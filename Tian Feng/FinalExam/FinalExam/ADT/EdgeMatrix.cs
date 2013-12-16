using System;
using System.Collections.Generic;
using System.Text;

namespace FinalExam.ADT
{
    class EdgeMatrix
    {
        #region Member Variables
        private int _dimension;
        private int[,] _data;
        #endregion

        #region Member Functions
        public EdgeMatrix(int dimension)
        {
            _dimension = dimension;
            _data = new int[dimension, dimension];

            for (int i = 0; i < _dimension; i++)
                for (int j = 0; j < _dimension; j++)
                    _data[i, j] = 0;
        }
        #endregion

        #region Attributes
        public void SetWeight(int vid0, int vid1, int weight)
        {
            _data[vid0, vid1] = weight;
        }

        public int GetWeight(int vid0, int vid1)
        {
            return _data[vid0, vid1];
        }
    
        public int Dimension
        {
            get { return _dimension; }
        }

        public int Size
        {
            get
            {
                int count = 0;
                for (int i = 0; i < _dimension; i++)
                    for (int j = 0; j < _dimension; j++)
                        if (_data[i, j] > 0)
                            count++;
                return count;
            }
        }
        #endregion
    }
}
