using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Windows;

using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.DataSourcesGDB;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Display;

using Netica;

namespace MapApp.Utility
{
    class DataUtility
    {
        private static double trh = 0.1;
        public static List<int> pSubCount = new List<int>() { 14, 5, 6, 11 };

        public static IWorkspace LoadFileGDB(string sFilePath)
        {
            IWorkspace pWsp = null;
            IWorkspaceFactory pWspFac = new FileGDBWorkspaceFactory();

            try
            {
                pWsp = pWspFac.OpenFromFile(sFilePath, 0);
            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.Message);
                pWsp = null;
            }

            return pWsp;
        }

        public static IFeatureClass LoadFeatureClass(IWorkspace pWsp, string sName)
        {
            IFeatureClass pFc = null;
            IFeatureWorkspace pFWsp = pWsp as IFeatureWorkspace;

            try
            {
                pFc = pFWsp.OpenFeatureClass(sName);
            }
            catch (System.Exception ex)
            {
            	Console.WriteLine(ex.Message);
                pFc = null;
            }
            
            return pFc;
        }

        public static IFeatureClass CreateStandaloneFeatureClass(IWorkspace workspace, String featureClassName, IFields fieldsCollection, String shapeFieldName)
        {
            IFeatureWorkspace featureWorkspace = (IFeatureWorkspace)workspace;
            IFeatureClassDescription fcDesc = new FeatureClassDescriptionClass();
            IObjectClassDescription ocDesc = (IObjectClassDescription)fcDesc;

            IFieldChecker fieldChecker = new FieldCheckerClass();
            IEnumFieldError enumFieldError = null;
            IFields validatedFields = null;
            fieldChecker.ValidateWorkspace = workspace;
            fieldChecker.Validate(fieldsCollection, out enumFieldError, out validatedFields);

            IFeatureClass featureClass = featureWorkspace.CreateFeatureClass(featureClassName, validatedFields, ocDesc.InstanceCLSID, ocDesc.ClassExtensionCLSID, esriFeatureType.esriFTSimple, shapeFieldName, "");
            return featureClass;
        }

        public static ILayer GetLayerByName(IMap pMap, string sName)
        {
            ILayer pLayer = null;

            try
            {
                int count = pMap.LayerCount;
                for (int idx = 0; idx < count; idx++)
                {
                    pLayer = pMap.get_Layer(idx);
                    if (pLayer.Name != sName)
                    {
                        pLayer = null;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.Message);
                pLayer = null;
            }

            return pLayer;
        }

        public static ISegment GetLongestSegment(IPolygon pPolygon)
        {
            ISegmentCollection pSegCol = pPolygon as ISegmentCollection;
            ISegment pSeg = null;
            for (int idx = 0; idx < pSegCol.SegmentCount; idx++)
            {
                if (idx == 0)
                {
                    pSeg = pSegCol.Segment[idx];
                }
                else
                {
                    if (pSeg.Length < pSegCol.Segment[idx].Length)
                    {
                        pSeg = pSegCol.Segment[idx];
                    }
                }
            }

            return pSeg;
        }

        public static void GetArrangementData(IFeatureClass pSrc, out List<double> typeRatioList)
        {
            typeRatioList = new List<double>() { 0.0, 0.0, 0.0, 0.0 };

            IFeatureCursor pCursor = pSrc.Search(null, false);
            IFeature pFeature = null;
            int idx = pSrc.FindField("TYPE");
            int count = 0;
            while (null != (pFeature = pCursor.NextFeature()))
            {
                typeRatioList[(int)pFeature.get_Value(idx) - 1] += 1.0;
                count++;
            }
            System.Runtime.InteropServices.Marshal.ReleaseComObject(pCursor);

            typeRatioList[0] /= count;
            typeRatioList[1] /= count;
            typeRatioList[2] /= count;
            typeRatioList[3] /= count;

            return;
        }

        public static bool CheckRoomGeometry(IPolygon pRmGeo, IPolyline pLocalCtr, IPolyline pGlobalCtr)
        {
            ISegmentCollection pSegCol = pRmGeo as ISegmentCollection;
            if (pSegCol.SegmentCount < 4)
                return false;

            ITopologicalOperator pTop = pRmGeo as ITopologicalOperator;
            IPolyline pLocalPll = pTop.Intersect(pLocalCtr, esriGeometryDimension.esriGeometry1Dimension) as IPolyline;
            if (pLocalPll.Length < trh)
                return false;

            IPolyline pGlobalPll = pTop.Intersect(pGlobalCtr, esriGeometryDimension.esriGeometry1Dimension) as IPolyline;
            IRelationalOperator pRop = pGlobalPll as IRelationalOperator;
            if (pRop.Equals(pLocalPll))
                return false;

            return true;
        }

        public static IGeometry GetSimpleGeometry(IGeometry pGeometry)
        {
            IPolygon4 pPolygon = pGeometry as IPolygon4;
            IGeometryCollection pGeoCol = pPolygon.ExteriorRingBag as IGeometryCollection;
            
            if (pGeoCol.GeometryCount == 1)
            {
                return pGeometry;
            }

            IRing pRing = null;
            for (int idx = 0; idx < pGeoCol.GeometryCount; idx++)
            {
                IRing pTemp = pGeoCol.Geometry[idx] as IRing;
                if (idx == 0)
                {
                    pRing = pTemp;
                }
                else
                {
                    if (pTemp.Length > pRing.Length)
                    {
                        pRing = pTemp;
                    }
                }
            }

            IGeometry pResult = new PolygonClass();
            pResult.SpatialReference = pRing.SpatialReference;
            IPointCollection pPtCol = pResult as IPointCollection;
            pPtCol.AddPointCollection((IPointCollection)pRing);

            return pResult;
        }

        public static IElement AddElementToMap(IGeometry pGeometry, IMap pMap)
        {
            if (null == pGeometry || null == pMap)
            {
                return null;
            }

            IRgbColor rgbColor = new RgbColorClass();
            rgbColor.Red = 255;
            rgbColor.Green = 255;
            rgbColor.Blue = 0;

            IRgbColor outlineRgbColor = new RgbColorClass();
            outlineRgbColor.Red = 0;
            outlineRgbColor.Green = 0;
            outlineRgbColor.Blue = 0;

            IGraphicsContainer graphicsContainer = (IGraphicsContainer)pMap; // Explicit Cast
            IElement element = null;
            if (pGeometry.GeometryType == ESRI.ArcGIS.Geometry.esriGeometryType.esriGeometryPoint)
            {
                // Marker symbols
                ISimpleMarkerSymbol simpleMarkerSymbol = new SimpleMarkerSymbolClass();
                simpleMarkerSymbol.Color = rgbColor;
                simpleMarkerSymbol.Outline = true;
                simpleMarkerSymbol.OutlineColor = outlineRgbColor;
                simpleMarkerSymbol.Size = 5;
                simpleMarkerSymbol.Style = esriSimpleMarkerStyle.esriSMSCircle;

                IMarkerElement markerElement = new MarkerElementClass();
                markerElement.Symbol = simpleMarkerSymbol;
                element = (IElement)markerElement; // Explicit Cast
            }
            else if (pGeometry.GeometryType == ESRI.ArcGIS.Geometry.esriGeometryType.esriGeometryPolyline || pGeometry.GeometryType == ESRI.ArcGIS.Geometry.esriGeometryType.esriGeometryLine)
            {
                //  Line elements
                ISimpleLineSymbol simpleLineSymbol = new SimpleLineSymbolClass();
                outlineRgbColor.Blue = 255;
                simpleLineSymbol.Color = outlineRgbColor;
                simpleLineSymbol.Style = esriSimpleLineStyle.esriSLSSolid;
                simpleLineSymbol.Width = 1;

                ILineElement lineElement = new LineElementClass();
                lineElement.Symbol = simpleLineSymbol;
                element = (IElement)lineElement; // Explicit Cast
            }
            else if (pGeometry.GeometryType == ESRI.ArcGIS.Geometry.esriGeometryType.esriGeometryPolygon)
            {
                // Polygon elements
                ISimpleFillSymbol simpleFillSymbol = new SimpleFillSymbolClass();
                simpleFillSymbol.Color = rgbColor;
                simpleFillSymbol.Style = esriSimpleFillStyle.esriSFSSolid;
                IFillShapeElement fillShapeElement = new PolygonElementClass();
                fillShapeElement.Symbol = simpleFillSymbol;
                element = (IElement)fillShapeElement; // Explicit Cast
            }
            if (!(element == null))
            {
                element.Geometry = pGeometry;
                graphicsContainer.AddElement(element, 0);
            }

            return element;
        }

        public static void AddTextToMap(IGeometry pGeometry, IMap pMap, string strTxt)
        {
            if (null == pGeometry || null == pMap)
            {
                return;
            }

            IRgbColor rgbColor = new RgbColorClass();
            rgbColor.Red = 0;
            rgbColor.Green = 0;
            rgbColor.Blue = 0;

            IRgbColor outlineRgbColor = new RgbColorClass();
            outlineRgbColor.Red = 0;
            outlineRgbColor.Green = 0;
            outlineRgbColor.Blue = 0;

            IGraphicsContainer graphicsContainer = (IGraphicsContainer)pMap; // Explicit Cast
            IElement element = null;
            if ((pGeometry.GeometryType) == ESRI.ArcGIS.Geometry.esriGeometryType.esriGeometryPoint)
            {
                ITextSymbol pTS = new TextSymbolClass();
                pTS.Color = rgbColor;
                pTS.Size = 10;

                ITextElement pTE = new TextElementClass();
                pTE.ScaleText = true;
                pTE.Symbol = pTS;
                pTE.Text = strTxt;
                element = (IElement)pTE; // Explicit Cast
            }
        
            if (!(element == null))
            {
                element.Geometry = pGeometry;
                graphicsContainer.AddElement(element, 0);
            }
        }

        public static double GetDistance(IPoint pt1, IPoint pt2)
        {
            if (null == pt1 || null == pt2)
                return -1.0;

            ILine pLine = new LineClass();
            pLine.SpatialReference = pt1.SpatialReference;
            pLine.FromPoint = pt1;
            pLine.ToPoint = pt2;

            return pLine.Length;
        }

        public static IPolygon GetBuffer(IGeometry pGeo, double dist)
        {
            if (null == pGeo)
                return null;
            IBufferConstruction pBfrCst = new BufferConstructionClass();
            IBufferConstructionProperties2 pBfrCstPpt = pBfrCst as IBufferConstructionProperties2;
            pBfrCstPpt.EndOption = esriBufferConstructionEndEnum.esriBufferFlat;
            pBfrCstPpt.SideOption = esriBufferConstructionSideEnum.esriBufferFull;

            IGeometryCollection pInGeo = new GeometryBagClass() as IGeometryCollection;
            pInGeo.AddGeometry(pGeo);
            IEnumGeometry pEnumInGeo = pInGeo as IEnumGeometry;
            IGeometryCollection pOutGeo = new GeometryBagClass() as IGeometryCollection;
            pBfrCst.ConstructBuffers(pEnumInGeo, dist, pOutGeo);

            if (pOutGeo.GeometryCount < 1)
                return null;

            return pOutGeo.Geometry[0] as IPolygon;
        }

        public static void GetRoad(List<Room> pRooms, List<Road> pRoads)
        {
            if (null == pRooms || 0 == pRooms.Count)
                return;

            for (int i = 0; i < pRooms.Count; i++)
            {                
                ITopologicalOperator pTop = pRooms[i].Shape as ITopologicalOperator;
                IRelationalOperator pRop = pRooms[i].Shape as IRelationalOperator;
                double maxArea = -1.0;
                for (int j = 0; j < pRoads.Count; j++)
                {
                    IPolygon pBfr = GetBuffer(pRoads[j].Shape, pRoads[j].Width / 2.0 + trh);
                    if (!pRop.Overlaps(pBfr))
                        continue;

                    if (!pRoads[j].IsMainPath && null != pRooms[i].Road)
                        continue;

                    double temp = ((IArea)pTop.Intersect(pBfr, esriGeometryDimension.esriGeometry2Dimension)).Area;
                    if (null == pRooms[i].Road)
                    {
                        pRooms[i].Road = pRoads[j];
                        maxArea = temp;
                    }
                    else if (temp > maxArea)
                    {
                        pRooms[i].Road = pRoads[j];
                        maxArea = temp;
                    }
                }
            }
        }

        public static void GetNeighbor(List<Room> pRooms)
        {
            if (null == pRooms || 0 == pRooms.Count)
                return;

            int count = pRooms.Count;
            for (int i = 0; i < count; i++)
            {
                Room pRoom = pRooms[i];
                IRelationalOperator pRop = pRoom.Shape as IRelationalOperator;

                Room pNeighbor = null;
                for (int j = 0; j < count; j++)
                {
                    if (j == i)
                        continue;

                    Room pTemp = pRooms[j];
                    if (!pRop.Touches(pTemp.Shape))
                        continue;

                    if (null == pNeighbor)
                        pNeighbor = pTemp;
                    else
                        if (pNeighbor.Area < pTemp.Area)
                            pNeighbor = pTemp;

                    pRoom.Neighbors.Add(pTemp);
                }
                pRoom.Neighbor = pNeighbor;
            }
        }

        public static int GetNeighborIndex(IPolygon pPlg, List<IGeometry> pGeoList, out int neighborCount)
        {
            IRelationalOperator pRop = pPlg as IRelationalOperator;
            neighborCount = 0;

            if (null == pRop)
            {
                return -1;
            }

            int result = -1;
            int count = pGeoList.Count;
            for (int idx = 0; idx < count; idx++)
            {
                IPolygon pTempPlg = pGeoList[idx] as IPolygon;

                if (!pRop.Touches(pTempPlg))
                {
                    continue;
                }

                neighborCount++;

                if (-1 == result)
                {
                    result = idx;
                }
                else
                {
                    if (((IArea)pTempPlg).Area > ((IArea)pGeoList[result]).Area)
                    {
                        result = idx;
                    }
                }
            }

            return result;
        }

        public static int SubtypeToIndex(int sub)
        {
            if (0 == sub)
                return 0;

            sub++;
            int hValue = sub / 100;
            int result = sub % 100;
            for (int idx = 0; idx < hValue - 1; idx++)
                result += pSubCount[idx];

            return result;
        }

        public static int IndexToSubtype(int index)
        {
            if (0 == index)
                return 0;

            int count = 0;
            int idx = 0;
            for (idx = 0; idx < pSubCount.Count; idx++)
            {
                count += pSubCount[idx];
                if (index <= count)
                    break;    
            }
 
            return (idx + 1) * 100 + pSubCount[idx] - 1 - (count - index);
        }

        public static int IndexToType(int index)
        {
            if (0 == index)
                return 0;

            int count = 0;
            int idx = 0;
            for (idx = 0; idx < pSubCount.Count; idx++)
            {
                count += pSubCount[idx];
                if (index <= count)
                    break;
            }

            return idx + 1;
        }

        public static double GetMinDistance(IPoint pt1, IPoint pt2, IPoint pt3)
        {
            double dis = 0;

            if (pt1.X == pt2.X)
            {
                dis = Math.Abs(pt3.X - pt1.X);
            }
            else
            {
                double k1 = (pt2.Y - pt1.Y) / (pt2.X - pt1.X);
                double c1 = (pt2.X * pt1.Y - pt1.X * pt2.Y) / (pt2.X - pt1.X);
                dis = Math.Abs(c1 * pt3.X - pt3.Y + c1) / (Math.Sqrt(k1 * k1 + 1));
            }

            return dis;
        }

        public static string TypeToText(int type, Random pRdm)
        {
            if (4 == type)
                type = pRdm.Next(3) + 1;

            string result = string.Empty;
            switch(type)
            {
                case 1:
                    result = "Shop";
                    break;
                case 2:
                    result = "Food";
                    break;
                case 3:
                    result = "Lifestyle";
                    break;
                default:
                    break;
            }

            return result;
        }

        public static IPolyline PolygonToPolyline(IPolygon pPlg)
        {
            if (null == pPlg)
                return null;

            IPolyline pPll = new PolylineClass();
            pPll.SpatialReference = pPlg.SpatialReference;

            ISegmentCollection pPlgSegCol = pPlg as ISegmentCollection;
            ISegmentCollection pPllSegCol = pPll as ISegmentCollection;
            pPllSegCol.AddSegmentCollection(pPlgSegCol);

            return pPll;
        }

        public static IPolyline SegmentToPolyline(ISegment pSeg)
        {
            if (null == pSeg)
                return null;

            IPolyline pPll = new PolylineClass();
            pPll.SpatialReference = pSeg.SpatialReference;
            pPll.FromPoint = pSeg.FromPoint;
            pPll.ToPoint = pSeg.ToPoint;

            return pPll;
        }
    }
}

