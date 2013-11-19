using System;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;

using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Controls;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.ADF;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.AnalysisTools;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Geoprocessing;
using ESRI.ArcGIS.Geoprocessor;
using ESRI.ArcGIS.DataManagementTools;
using ESRI.ArcGIS.Output;

using MapApp.Utility;
using Netica;

namespace MapApp
{
    public sealed partial class MainForm : Form
    {
        #region class private members
        private IMapControl3 m_mapControl = null;
        private string m_mapDocumentName = string.Empty;
        #endregion

        #region class constructor
        public MainForm()
        {
            InitializeComponent();
        }
        #endregion

        private void MainForm_Load(object sender, EventArgs e)
        {
            //get the MapControl
            m_mapControl = (IMapControl3)axMapControl1.Object;
        }

        #region Main Menu event handlers
        private void menuNewDoc_Click(object sender, EventArgs e)
        {
            //execute New Document command
            ICommand command = new CreateNewDocument();
            command.OnCreate(m_mapControl.Object);
            command.OnClick();
        }

        private void menuOpenDoc_Click(object sender, EventArgs e)
        {
            //execute Open Document command
            ICommand command = new ControlsOpenDocCommandClass();
            command.OnCreate(m_mapControl.Object);
            command.OnClick();
        }

        private void menuSaveDoc_Click(object sender, EventArgs e)
        {
            //execute Save Document command
            if (m_mapControl.CheckMxFile(m_mapDocumentName))
            {
                //create a new instance of a MapDocument
                IMapDocument mapDoc = new MapDocumentClass();
                mapDoc.Open(m_mapDocumentName, string.Empty);

                //Make sure that the MapDocument is not readonly
                if (mapDoc.get_IsReadOnly(m_mapDocumentName))
                {
                    MessageBox.Show("Map document is read only!");
                    mapDoc.Close();
                    return;
                }

                //Replace its contents with the current map
                mapDoc.ReplaceContents((IMxdContents)m_mapControl.Map);

                //save the MapDocument in order to persist it
                mapDoc.Save(mapDoc.UsesRelativePaths, false);

                //close the MapDocument
                mapDoc.Close();
            }
        }

        private void menuSaveAs_Click(object sender, EventArgs e)
        {
            //execute SaveAs Document command
            ICommand command = new ControlsSaveAsDocCommandClass();
            command.OnCreate(m_mapControl.Object);
            command.OnClick();
        }

        private void menuExitApp_Click(object sender, EventArgs e)
        {
            //exit the application
            System.Windows.Forms.Application.Exit();
        }
        #endregion

        //listen to MapReplaced evant in order to update the statusbar and the Save menu
        private void axMapControl1_OnMapReplaced(object sender, IMapControlEvents2_OnMapReplacedEvent e)
        {
            //get the current document name from the MapControl
            m_mapDocumentName = m_mapControl.DocumentFilename;

            //if there is no MapDocument, diable the Save menu and clear the statusbar
//             if (m_mapDocumentName == string.Empty)
//             {
//                 statusBarXY.Text = string.Empty;
//             }
//             else
//             {
//                 //enable the Save manu and write the doc name to the statusbar
//                 statusBarXY.Text = System.IO.Path.GetFileName(m_mapDocumentName);
//             }
        }

        private void axMapControl1_OnMouseMove(object sender, IMapControlEvents2_OnMouseMoveEvent e)
        {
            //statusBarXY.Text = string.Format("{0}, {1}  {2}", e.mapX.ToString("#######.##"), e.mapY.ToString("#######.##"), axMapControl1.MapUnits.ToString().Substring(4));
        }

        #region Member Variables
        private IWorkspace mWsp;
        private IFeatureWorkspace mFtrWsp;
        private IWorkspace mTmpWsp;
        private IFeatureWorkspace mTmpFtrWsp;

        private IMap mMap;
        private IActiveView mAV;
        private IGraphicsContainer mGC;
        private ITOCControl mTocCtrl;

        private IPolygon mCtrPlg;
        private IPolyline mCtrPll;

        private IFeatureClass mCtrFC;
        private IFeatureClass mRdsFC;
        private IFeatureClass mPrvLytFC;
        private IFeatureClass mBlkFC;
        private IFeatureClass mLytFC;

        private Random mRdm;

        private List<Room> mSltRms;
        private List<IElement> mSltRmElms;

        private string m_strGdbName = "LayoutDB.gdb";
        private string m_strTmpGdbName = "LayoutTemp.gdb";
        private string m_strCtrFCName = "L1_C";
        private string m_strRdsFCName = "L1_R";
        private string m_strOldLytFCName = "L1";
        private string m_strDataFldPath = string.Format("{0}data", AppDomain.CurrentDomain.BaseDirectory);

        private List<IGeometry> mPlgs;
        private IGeometry mPlgBase;
        private List<IGeometry> mRestPlgs;

        private List<double> mWids;
        private double m_minArea;
        private double m_maxArea;
        #endregion

        private void LoadData()
        {
            SwitchProgress(true);
            SetStatus("Loading Data...");

            mWsp = null;
            mFtrWsp = null;
            mTmpWsp = null;
            mTmpFtrWsp = null;
            
            mMap = null;
            mAV = null;
            mTocCtrl = null;

            mCtrPlg = null;
            mCtrPll = null;
            
            mCtrFC = null;
            mRdsFC = null;
            mPrvLytFC = null;
            mBlkFC = null;
            mPlgs = null;
            mPlgBase = null;

            mWids = null;
            m_minArea = 0.0;
            
            mSltRms = new List<Room>();
            mSltRmElms = new List<IElement>();
            
            if (null == mRdm)
                mRdm = new Random();

            mMap = m_mapControl.Map;
            mAV = mMap as IActiveView;
            mTocCtrl = axTOCControl1.Object as ITOCControl;
            mGC = mMap as IGraphicsContainer;

            mMap.ClearLayers();

            mWsp = DataUtility.LoadFileGDB(string.Format("{0}\\{1}", m_strDataFldPath, m_strGdbName));
            mFtrWsp = mWsp as IFeatureWorkspace;

            mTmpWsp = DataUtility.LoadFileGDB(string.Format("{0}\\{1}", m_strDataFldPath, m_strTmpGdbName));
            mTmpFtrWsp = mTmpWsp as IFeatureWorkspace;

            mCtrFC = DataUtility.LoadFeatureClass(mWsp, m_strCtrFCName);
            IFeatureLayer pLayer = new FeatureLayer();
            pLayer.FeatureClass = mCtrFC;
            pLayer.Name = "Contour";
            mMap.AddLayer(pLayer);

            mRdsFC = DataUtility.LoadFeatureClass(mWsp, m_strRdsFCName);
            pLayer = new FeatureLayer();
            pLayer.FeatureClass = mRdsFC;
            pLayer.Name = "Roads";
            mMap.AddLayer(pLayer);

            mPrvLytFC = DataUtility.LoadFeatureClass(mWsp, m_strOldLytFCName);
            pLayer = new FeatureLayer();
            pLayer.FeatureClass = mPrvLytFC;
            pLayer.Name = "Old Layout";
            pLayer.Visible = false;
            mMap.AddLayer(pLayer);

            IFeatureCursor pCursor = mCtrFC.Search(null, false);
            if (null == pCursor)
                return;
            IFeature pFeature = pCursor.NextFeature();
            mCtrPlg = pFeature.ShapeCopy as IPolygon;
            mCtrPll = DataUtility.PolygonToPolyline(mCtrPlg);
            if (null == mCtrPlg || null == mCtrPll)
                return;
            Marshal.ReleaseComObject(pCursor);

            mAV.Refresh();
            miBlock.Enabled = true;

            SwitchProgress(false);
            SetStatus("Data Loaded!");
        }

        private void Subdivition(IGeometry pGeometry)
        {
            int index = mRdm.Next(mWids.Count);
            ISegment pSeg = DataUtility.GetLongestSegment((IPolygon)pGeometry);
            if (pSeg == null)
                return;

            if (pSeg.Length < mWids[index])
            {
                mRestPlgs.Add(pGeometry);
                return;
            }

            if (((IArea)pGeometry).Area < m_minArea)
            {
                mRestPlgs.Add(pGeometry);
                return;
            }
            
            ILine pNorm = new LineClass();
            pSeg.QueryNormal(esriSegmentExtension.esriNoExtension, pSeg.Length / 2.0, false, 0.1, pNorm);
            bool isNormInside = false;
            if (((IRelationalOperator)pNorm.ToPoint).Within(pGeometry))
                isNormInside = true;
            
            ITopologicalOperator pTop = pGeometry as ITopologicalOperator;
            IGeometry pLeft = null;
            IGeometry pRight = null;

            double extDist = 1000.0;
            double normDist = 10.0;

            for (int idx = index; idx < mWids.Count; idx++)
            {
                pNorm = new LineClass();
                pSeg.QueryNormal(esriSegmentExtension.esriNoExtension, mWids[idx], false, normDist, pNorm);
                IPoint pFp = new PointClass();
                IPoint pTp = new PointClass();
                pNorm.QueryPoint(esriSegmentExtension.esriExtendAtFrom, -extDist, false, pFp);
                pNorm.QueryPoint(esriSegmentExtension.esriExtendAtTo, extDist, false, pTp);
                IPolyline pNormLine = new PolylineClass();
                pNormLine.SpatialReference = mMap.SpatialReference;
                pNormLine.FromPoint = pFp;
                pNormLine.ToPoint = pTp;

                try
                {
                    pTop.Cut(pNormLine, out pLeft, out pRight);
                }
                catch (System.Exception ex)
                {
                	Console.WriteLine(ex.Message);
                    if (DataUtility.CheckRoomGeometry((IPolygon)pGeometry, DataUtility.PolygonToPolyline((IPolygon)mPlgBase), mCtrPll))
                        mPlgs.Add(DataUtility.GetSimpleGeometry(pGeometry));
                    else
                        mRestPlgs.Add(DataUtility.GetSimpleGeometry(pGeometry));
                    return;
                }

                IGeometry pClip = null;
                IGeometry pOther = null;
                if (isNormInside)
                {
                    pClip = pRight;
                    pOther = pLeft;
                }
                else
                {
                    pClip = pLeft;
                    pOther = pRight;
                }

                double clipArea = ((IArea)pClip).Area;         
                if (clipArea >= m_maxArea) // [200, inf)
                {
                    Subdivition(pClip);
                    Subdivition(pOther);
                }
                else if (clipArea >= m_minArea) // [10, 200)
                {
                    if (DataUtility.CheckRoomGeometry((IPolygon)pClip, DataUtility.PolygonToPolyline((IPolygon)mPlgBase), mCtrPll))
                        mPlgs.Add(DataUtility.GetSimpleGeometry(pClip));
                    else
                        mRestPlgs.Add(DataUtility.GetSimpleGeometry(pClip));
                    Subdivition(pOther);
                }
                else // (0, 10)
                {
                    if (idx == mWids.Count - 1)
                    {
                        mRestPlgs.Add(pClip);
                        Subdivition(pOther);
                    }
                    else if (pSeg.Length <= mWids[idx + 1])
                    {
                        if (DataUtility.CheckRoomGeometry((IPolygon)pGeometry, DataUtility.PolygonToPolyline((IPolygon)mPlgBase), mCtrPll))
                            mPlgs.Add(DataUtility.GetSimpleGeometry(pGeometry));
                        else
                            mRestPlgs.Add(DataUtility.GetSimpleGeometry(pGeometry));
                    }
                    else
                        continue;
                }

                break;
            } 
        }

        private void miLayout_Click(object sender, EventArgs e)
        {
            SwitchProgress(true);
            SetStatus("Subdividing Blocks into Rooms...");

            if (null == mPrvLytFC || null == mBlkFC)
                return;

            mWids = new List<double>() { 3, 4, 5, 6, 7 };
            m_minArea = 10;
            m_maxArea = 50;

            IFeatureCursor pCursor = mBlkFC.Search(null, false);
            if (null == pCursor)
            {
                return;
            }
            IFeature pFeature = null;
            mPlgs = new List<IGeometry>();
            mRestPlgs = new List<IGeometry>();
            while (null != (pFeature = pCursor.NextFeature()))
            {
                IGeometry pGeo = pFeature.ShapeCopy;
                ITopologicalOperator pTop = mCtrPlg as ITopologicalOperator;
                mPlgBase = pTop.Difference(pGeo);
                Subdivition(pGeo);
            }

//             for (int i = 0; i < mRestPlgs.Count - 1; i++)
//             {
//                 if (null == mRestPlgs[i])
//                     continue;
// 
//                 for (int j = i + 1; i < mRestPlgs.Count; i++)
//                 {
//                     if (null == mRestPlgs[j])
//                         continue;
// 
//                     IRelationalOperator pRop = mRestPlgs[i] as IRelationalOperator;
//                     ITopologicalOperator pTop = mRestPlgs[i] as ITopologicalOperator;
// 
//                     if (!pRop.Touches(mRestPlgs[j]))
//                         continue;
// 
//                     mRestPlgs[i] = pTop.Union(mRestPlgs[j]);
//                     mRestPlgs[j] = null;
//                 }
//             }

            for (int i = 0; i < mRestPlgs.Count; i++)
            {
                if (null == mRestPlgs[i])
                    continue;

                IRelationalOperator pRop = mRestPlgs[i] as IRelationalOperator;
                double maxLen = -1.0;
                int geoIdx = -1;
                for (int j = 0; j < mPlgs.Count; j++)
                {
                    if (!pRop.Touches(mPlgs[j]))
                        continue;

                    ITopologicalOperator pTop = mPlgs[j] as ITopologicalOperator;
                    double temp = ((IPolyline)pTop.Intersect(mRestPlgs[i], esriGeometryDimension.esriGeometry1Dimension)).Length;
                    if (geoIdx == -1)
                    {
                        geoIdx = j;
                        maxLen = temp;
                    } 
                    else if (temp > maxLen)
                    {
                        geoIdx = j;
                        maxLen = temp;
                    }
                }

                if (geoIdx == -1)
                    continue;

                ITopologicalOperator pTop2 = mPlgs[geoIdx] as ITopologicalOperator;
                mPlgs[geoIdx] = pTop2.Union(mRestPlgs[i]);
            }

            int count = mPlgs.Count;
            for (int idx = 0; idx < count; idx++)
                DataUtility.AddElementToMap(mPlgs[idx], mMap);

            mMap.DeleteLayer(DataUtility.GetLayerByName(mMap, "Block"));
            mAV.Refresh();

            SwitchProgress(false);
            SetStatus("Layout Generated!");
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            IDataset pDs = mBlkFC as IDataset;
            if (null == pDs)
            {
                return;
            }
            if (pDs.CanDelete())
            {
                pDs.Delete();
            }

            pDs = mLytFC as IDataset;
            if (null == pDs)
            {
                return;
            }
            if (pDs.CanDelete())
            {
                pDs.Delete();
            }
        }

        private void miFunan_Click(object sender, EventArgs e)
        {
            m_strCtrFCName = "Funan_L1_C";
            m_strRdsFCName = "Funan_L1_R";
            m_strOldLytFCName = "Funan_L1";
            LoadData();
        }

        private void SwitchProgress(bool start)
        {
            if (start)
                System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor;
            else
                System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default;
        }

        private void SetStatus(string status)
        {
            m_ctrlStatus.Text = status;
        }

        private void miBlock_Click(object sender, EventArgs e)
        {
            SwitchProgress(true);
            SetStatus("Generating Blocks from Roads...");

            if (null == mRdsFC || null == mCtrPlg || null == mCtrPll)
                return;

            IFeatureCursor pCursor = mRdsFC.Search(null, false);
            if (null == pCursor)
                return;

            int idx = mRdsFC.FindField("PATH_WIDTH");
            IFeature pFeature = null;
            IGeometry pBufferShape = null;
            ITopologicalOperator pTop = null;

            while (null != (pFeature = pCursor.NextFeature()))
            {
                IGeometry pBuffer = DataUtility.GetBuffer(pFeature.Shape, (double)pFeature.get_Value(idx) / 2.0) as IGeometry;

                if (null == pBufferShape)
                    pBufferShape = pBuffer;
                else
                {
                    pTop = pBufferShape as ITopologicalOperator;
                    pBufferShape = pTop.Union(pBuffer);
                }
            }
            Marshal.ReleaseComObject(pCursor);

            pTop = mCtrPlg as ITopologicalOperator;
            IGeometry pDiffShape = pTop.Difference(pBufferShape);

            IPolygon4 pDiffPolygon = pDiffShape as IPolygon4;
            IGeometryCollection pRingCol = pDiffPolygon.ExteriorRingBag as IGeometryCollection;
            int count = pRingCol.GeometryCount;
            List<IGeometry> pGeoCol = new List<IGeometry>();

            for (idx = 0; idx < count; idx++)
            {
                IRing p = pRingCol.Geometry[idx] as IRing2;
                IPointCollection pCol = p as IPointCollection;

                IPolygon p2 = new PolygonClass();
                IPointCollection pCol2 = p2 as IPointCollection;
                pCol2.AddPointCollection(pCol);

                IArea pArea = p2 as IArea;
                if (pArea.Area / p2.Length <= 1.0)
                    continue;

                //p2.Generalize(0.5);
                ISegmentCollection pSegCol2 = p2 as ISegmentCollection;
                if (pSegCol2.SegmentCount > 3)
                    pGeoCol.Add(p2);
            }

            if (pGeoCol.Count == 0)
                return;

            string sBlkFCName = string.Format("BLK_{0}{1:D2}{2:D2}{3:D2}", DateTime.Now.DayOfYear, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second);
            try
            {
                mBlkFC = DataUtility.CreateStandaloneFeatureClass(mTmpWsp, sBlkFCName, mCtrFC.Fields, "SHAPE");
            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }

            if (null == mBlkFC)
                return;

            count = pGeoCol.Count;
            for (idx = 0; idx < count; idx++)
            {
                IGeometry g = pGeoCol[idx];

                pCursor = mBlkFC.Insert(true);
                if (null == pCursor)
                    return;

                IFeatureBuffer b = mBlkFC.CreateFeatureBuffer();
                b.Shape = g;
                pCursor.InsertFeature(b);
                pCursor.Flush();
            }

            Marshal.ReleaseComObject(pCursor);

            IFeatureLayer pLayer = new FeatureLayerClass();
            pLayer.FeatureClass = mBlkFC;
            pLayer.Name = "Block";

            mMap.AddLayer(pLayer);
            mAV.Refresh();
            mTocCtrl.Update();
            miLayout.Enabled = true;

            SwitchProgress(false);
            SetStatus("Blocks Generated!");
        }
    }
}