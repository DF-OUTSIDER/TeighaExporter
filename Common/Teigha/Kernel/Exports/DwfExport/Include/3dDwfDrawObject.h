/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
//
// 3dDwfDrawObject.h: definition of the Od3dDwfDevice, Od3dDwfView classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef _3DDWFDRAWOBJECT_INCLUDED_
#define _3DDWFDRAWOBJECT_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// workaround for bug in certain versions of HP's aCC compiler
#ifdef OD_REDEFINE_STD
namespace std {} using namespace std;
#endif

//#include "dwf/Version.h"
//#include "dwf/package/Constants.h"
//#include "dwf/package/EPlotSection.h"
////#include "dwf/package/DefinedObject.h"
////#include "dwf/package/GlobalSection.h"
//#include "dwf/package/writer/PackageWriter.h"
#include "dwf/publisher/model/Model.h"
//#include "dwf/publisher/impl/DWFPackagePublisher.h"

#include "Gs/GsBaseInclude.h"
#include "Gi/GiGeometrySimplifier.h"
//#include "GiRasterImage.h"
#include "Gi/GiSpatialFilter.h"
//#include "Gi/GiPerspectivePreprocessor.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DWF_EXPORT {

class C3dDwfExportImpl;

//////////////////////////////////////////////////////////////////////

/** \details
   This class implements the DWF export device.
*/
class Od3dDwfDevice : public OdGsBaseVectorizeDevice
{
  // Members
  //

  C3dDwfExportImpl* m_3dDwf;


protected:
  ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);

public:

  // Constructor.
  Od3dDwfDevice();
  ~Od3dDwfDevice();

  // Set the target data stream and the type.  
  
  /** \details
    Sets the target data stream and the type.
  */
  static OdGsDevicePtr createObject();

  // Attention: there is a need to call own device methods for setting of context 
  //            before setupActiveLayoutViews() or setupLayoutViews() not after! 
  //            Only base interface methods can be called after them.


  // Called by the Teigha rendering framework before rendering this view.
  // The override should call the parent version of this function, OdGsBaseVectorizeDevice::update().

  /** \details
    This method is called by the Teigha rendering framework before rendering the view.
	
	\param pUpdatedRect [in] Input 2D device coordinate rectangle.
	
	\remarks The override should call the parent version of this function, OdGsBaseVectorizeDevice::update().
  */  
  void update(OdGsDCRect* pUpdatedRect);
  
  // Called by the rendering framework to create an instance 
  // of the desired OdGsView subclass. 
  
  /** \details
    This method is called by the rendering framework to create an instance of the desired OdGsView subclass.
    
	\param pInfo [in] Input client view information.
	\param bEnableLayerVisibilityPerView [in] Value is true if layer visibility per view is enabled.
  */
  OdGsViewPtr createView(const OdGsClientViewInfo* pInfo = 0, 
                         bool bEnableLayerVisibilityPerView = false);

  // output into device context

  /** \details
    Returns a pointer to the database object to be exported.
  */
  virtual OdDbBaseDatabase* getDb();


  //  ----- Od3dDwfDevice methods -----
  //

  // Attention: there is a need to call own device methods for setting of context 
  //            before setupActiveLayoutViews() or setupLayoutViews() not after! 
  //            Only base interface methords could be called after them.

  /** \details
    Sets a special OdDwfDevice context.
	
	\param dwf [in] Input OdDwfDevice context.
  */
  void setContext(C3dDwfExportImpl* dwf);             // Sets a special OdDwfDevice context

  /** \details
    Returns a special DWF export object.
  */
  C3dDwfExportImpl* getExportImpl()
  {
    return m_3dDwf;
  }

}; 



//////////////////////////////////////////////////////////////////////

/** \details
   This class implements the view for DWF export.
*/
class Od3dDwfView :   public OdGsBaseVectorizeViewDef
                    , public OdGiGeometrySimplifier  // Receives 3d data from the Teigha 
                                                     //  rendering framework. 
{
  // Members
  //

  // Context
  //
  int                     m_TKOGeometry;    // current TKO geometry (W3D)
  DWFToolkit::DWFSegment* m_pCurrentRootSubpart;

  bool                    m_bWriteToTheSameSegment;

//  OdGePoint2d           m_ptMinViewClip;
//  OdGePoint2d           m_ptMaxViewClip;

public:
//  OdGiOrthoClipperPtr   m_pViewportClip;
//  OdGiSpatialFilterPtr  m_pSpatialFilter;
//  OdGiXformPtr          m_pXYProjector;

//  OdGiXformPtr                   m_pPerspXform;
//  OdGiPerspectivePreprocessorPtr m_pPerspPrepr;

public:
  // Returns the ExGsSimpleDevice instance that owns this view. 
  
  /** \details
    Returns the ExGsSimpleDevice instance that owns this view.
  */
  Od3dDwfDevice* device()
  { 
    return (Od3dDwfDevice*)OdGsBaseVectorizeView::device();
  }

  // Constructor. 
  Od3dDwfView();

  // PseudoConstructor. 
  static OdGsViewPtr createObject()
  {
    return OdRxObjectImpl<Od3dDwfView, OdGsView>::createObject();
  }

  friend class Od3dDwfDevice;

public:
//  OdGiConveyorOutput& screenOutput() { return m_pXYProjector->output(); }

/*
  // to update conveyor if view is perspective
  virtual void setView(const OdGePoint3d & position,
    const OdGePoint3d& target,
    const OdGeVector3d& upVector,
    double fieldWidth,
    double fieldHeight,
    Projection projection = kParallel);
*/

  // Called by the Teigha rendering framework to give the 
  // client application a chance to terminate the current 
  // rendering process.  Returning true from this function will 
  // stop the current rendering operation.

  /** \details
    Called by the Teigha rendering framework to give the client application a chance to terminate the current rendering process.
  
    \remarks Returning true from this function will stop the current rendering operation.
  */
  bool regenAbort() const;

  /** \details
    Returns the deviation of the specified type for the specified point.
  
    \param OdGiDeviationType [in] Input deviation type. 
    \param OdGePoint3d [in] Input point for which deviation is needed to be returned.
  */
  double deviation(const OdGiDeviationType, const OdGePoint3d&) const
  {
    // to force Teigha rendering framework to use some reasonable value.
    return 0.0; 
  }

  /** \details
    Called by the Teigha rendering framework before rendering the view.
  */
  void update();

  // ----- The Overrides of OdGiGeometrySimplifier (OdGiConveyorGeometry) -----
  //
  // Must be overridden at least.
  
  /** \details
    Passes polyline data to this Vectorizer object.
	
	\param OdInt32 [in] Input number of points in the polyline.  
    \param OdGePoint3d* [in] Input array of vertices that make up the polyline. 
	
	\remarks Must be overridden at least.
  */
  void polylineOut( OdInt32 /*nbPoints*/, 
                    const OdGePoint3d* /*pVertexList*/)
  {}

  // Must be overridden at least.
  
  /** \details
    Passes polyline data to the Vectorizer object.
	
	\param OdInt32 [in] Input number of points in the polygon. 
	\param OdGePoint3d* [in] Input array of vertices that make up the polygon.  
    \param OdGeVector3d* [in] Input normal vector for the polygon. 

	\remarks Must be overridden at least.
  */
  void polygonOut( OdInt32 /*nbPoints*/, 
                   const OdGePoint3d* /*pVertexList*/, 
                   const OdGeVector3d* /*pNormal*/ = NULL)
  {}

  /** \details
    Adds a circle to the view (using an input center point and radius, etc.).
	
	\param center [in] Input circle center point. 
	\param radius [in] Input circle radius.  
    \param normal [in] Input normal vector. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void circleProc(
            const OdGePoint3d& center, 
            double radius, 
            const OdGeVector3d& normal, 
            const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    Adds a circle to the view (using input points of a 3-point circle).
	
	\param firstPoint [in] Input first point of a 3-point circle.
	\param secondPoint [in] Input second point of a 3-point circle.  
    \param thirdPoint [in] Input third point of a 3-point circle. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void circleProc(
            const OdGePoint3d&, 
            const OdGePoint3d&, 
            const OdGePoint3d&, 
            const OdGeVector3d* pExtrusion = 0);

  /** \details
    Adds a circular arc to the view (using an input center point, arc radius, etc.).
	
	\param center [in] Input circular arc center point. 
	\param radius [in] Input circular arc radius.  
    \param normal [in] Input normal vector. 
    \param startVector [in] Input start vector. 
    \param sweepAngle [in] Input sweep angle. 
    \param arcType [in] Input circular arc type. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void circularArcProc(
            const OdGePoint3d& center,
            double radius,
            const OdGeVector3d& normal,
            const OdGeVector3d& startVector,
            double sweepAngle,
            OdGiArcType arcType = kOdGiArcSimple, 
            const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    Adds a circular arc to the view (using input points of a 3-point arc, etc.).
	
	\param start [in] Input first point of a 3-point arc. 
	\param point [in] Input second point of a 3-point arc.   
    \param end [in] Input third point of a 3-point arc.  
    \param arcType [in] Input circular arc type. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void circularArcProc(
            const OdGePoint3d& start,
            const OdGePoint3d& point,
            const OdGePoint3d& end,
            OdGiArcType arcType = kOdGiArcSimple, 
            const OdGeVector3d* pExtrusion = 0);

  /** \details
    Adds a polyline to the view.
	
	\param nbPoints [in] Input number of points. 
	\param pVertexList [in] Input array of vertices.  
    \param pNormal [in] Input array of normals. 
    \param pExtrusion [in] Input extrusion vector. 
    \param lBaseSubEntMarker [in] Input GS marker of the subentity of this entity. 
  */
  void polylineProc(
            OdInt32 nbPoints, 
            const OdGePoint3d* pVertexList,
            const OdGeVector3d* pNormal = 0,
            const OdGeVector3d* pExtrusion = 0,
            OdGsMarker lBaseSubEntMarker = -1);
  
  /** \details
    Adds a polygon to the view.
	
	\param nbPoints [in] Input number of points. 
	\param pVertexList [in] Input array of vertices.  
    \param pNormal [in] Input array of normals. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void polygonProc(
            OdInt32 nbPoints, 
            const OdGePoint3d* pVertexList,
            const OdGeVector3d* pNormal = 0,
            const OdGeVector3d* pExtrusion = 0);

  /** \details
    Adds a mesh to the view.
	
	\param rows [in] Input number of rows. 
	\param columns [in] Input number of columns. 
	\param pVertexList [in] Input array of vertices. 
    \param pEdgeData [in] Input array of edge's data. 
    \param pFaceData [in] Input array of face's data. 
    \param pVertexData [in] Input array of vertex's data. 
  */
  void meshProc(
            OdInt32 rows,
            OdInt32 columns,
            const OdGePoint3d* pVertexList,
            const OdGiEdgeData* pEdgeData = 0,
            const OdGiFaceData* pFaceData = 0,
            const OdGiVertexData* pVertexData = 0);
  
  /** \details
    Adds a shell to the view.
	
	\param nbVertex [in] Input number of points.  
	\param pVertexList [in] Input array of vertices. 
	\param faceListSize [in] Input number of faces. 
	\param pFaceList [in] Input array of faces. 
	\param pEdgeData [in] Input array of edge's data.  
    \param pFaceData [in] Input array of face's data. 
    \param pVertexData [in] Input array of vertex's data. 
  */
  void shellProc(
            OdInt32 nbVertex,
            const OdGePoint3d* pVertexList,
            OdInt32 faceListSize,
            const OdInt32* pFaceList,
            const OdGiEdgeData* pEdgeData = 0,
            const OdGiFaceData* pFaceData = 0,
            const OdGiVertexData* pVertexData = 0);
  
  /** \details
    Adds a shape to the view.
	
	\param position [in] Input shape position.  
	\param u [in] Input u-vector. 
	\param v [in] Input v-vector. 
	\param shapeNo [in] Input shape number. 
	\param pStyle [in] Input shape text style.  
    \param pExtrusion [in] Input extrusion vector. 
  */
  void shapeProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            int shapeNo, 
            const OdGiTextStyle* pStyle,
            const OdGeVector3d* pExtrusion = 0);

  /** \details
    Adds NURBS curves to the view.
	
	\param nurbs [in] Input array of NURBS curves.  
  */
  void nurbsProc(
            const OdGeNurbCurve3d& nurbs);

  /** \details
    Adds text to the view.
	
	\param position [in] Input shape position.  
	\param u [in] Input u-vector. 
	\param v [in] Input v-vector. 
	\param msg [in] Input text. 
	\param length [in] Input text length.  
	\param raw [in] True if text is in raw state.  
    \param pTextStyle [in] Input shape text style. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void textProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool raw, 
            const OdGiTextStyle* pTextStyle,
            const OdGeVector3d* pExtrusion = 0);

  /** \details
    Adds an elliptical arc to the view.

	\param arc [in] Input elliptical arc.
	\param pEndPointsOverrides [in] Input arc's end points. 
	\param arcType [in] Input arc type. 
    \param pExtrusion [in] Input extrusion vector. 
  */
  void ellipArcProc(
            const OdGeEllipArc3d& arc,
            const OdGePoint3d* pEndPointsOverrides,
            OdGiArcType arcType,
            const OdGeVector3d* pExtrusion);

  /** \details
    Adds an x-line to the view.

	\param firstPoint [in] Input x-line's first point.
	\param secondPoint [in] Input x-line's second point. 
  */
  void xlineProc(
            const OdGePoint3d&, 
            const OdGePoint3d&);
  
  /** \details
    Adds a ray to the view.

	\param firstPoint [in] Input ray's first point.
	\param secondPoint [in] Input ray's second point. 
  */
  void rayProc(
            const OdGePoint3d&, 
            const OdGePoint3d&);
  

  // It doesn't support it for W3D yet
/*  
  void rasterImageProc(
            const OdGePoint3d& origin,
            const OdGeVector3d& u,
            const OdGeVector3d& v,
            const OdGiRasterImage* pImg, // image object
            const OdGePoint2d* uvBoundary, // may not be null
            OdUInt32 numBoundPts,
            bool transparency = false,
            double brightness = 50.0,
            double contrast = 50.0,
            double fade = 0.0);

	void metafileProc(
            const OdGePoint3d& origin,
		        const OdGeVector3d& u,
		        const OdGeVector3d& v,
            const OdGiMetafile* pMetafile,
            bool bDcAligned = true,       // reserved
            bool bAllowClipping = false); // reserved
*/
  // --------  Od3dDwfView  --------
  //

  // Retrieves the current rendering traits from Teigha (color, linewidth, etc.) 
  //  and sets these properties in this device. 
  
  /** \details
    Retrieves the current rendering traits from Teigha (color, linewidth, etc.) and sets these properties into this device.

	\param currTraits [in] Input current rendering traits.
  */
  virtual void setCurrTraits(const OdGiSubEntityTraitsData& currTraits);
  
  /** \details
    Sets current rendering traits into this device.

	\param currTraits [in] Input current rendering traits.
  */
  virtual void setCurrTraits();

  /** \details
    Starts view vectorization.
  */
  virtual void beginViewVectorization();
  
  /** \details
    Ends view vectorization.
  */
  virtual void endViewVectorization();

  // Called by each associated view object to set the current draw color (as RGB). 
  
  /** \details
    This method is called by each associated view object to set the current draw color (as RGB). 
  */
  void set_Color(ODCOLORREF rgb);

  // Called by each associated view object to set the current draw color (as index). 
  
  /** \details
    This method is called by each associated view object to set the current draw color (as index). 
  */
  void set_ColorIndex(OdUInt16 colorIndex);

  // Context accessors / transformers
  //
  
  /** \details
    Sets current TKO geometry.
	
	\param tko [in] Input current TKO geometry.
  */
  void setTKOGeometry( int tko )
  {
    m_TKOGeometry = tko;
  }
  
  /** \details
    Returns current TKO geometry.
  */
  int getTKOGeometry()
  {
    return m_TKOGeometry;
  }

  /** \details
    Returns current root subpart.
  */
  DWFToolkit::DWFSegment* getRootSubpart() const
  {
    return m_pCurrentRootSubpart;
  }

  /** \details
    Sets current root subpart.
	
	\param pSubpart [in] Input current root subpart.
  */
  DWFToolkit::DWFSegment* setRootSubpart( DWFToolkit::DWFSegment* pSubpart )
  {
    DWFToolkit::DWFSegment* pTempSubpart = m_pCurrentRootSubpart;
    m_pCurrentRootSubpart = pSubpart;

    return pTempSubpart;
  }

  /** \details
    Sets the new value to the "write to the same segment" flag and returns the previous state.
  */
  bool WriteToTheSameSegment( bool state )
  {
    bool bPrevious = m_bWriteToTheSameSegment;
    m_bWriteToTheSameSegment = state;

    return bPrevious;
  }

  // Additional geometry processing
  //
  //void setCircle(const OdGePoint3dArray& pointArray);
  
  /** \details
    Adds a circular arc to the view.
	
	\param pPoint1 [in] Input first point of a circular arc.
	\param pPoint2 [in] Input second point of a circular arc.  
    \param pPoint3 [in] Input third point of a circular arc. 
  */
  void setCircleArc(const OdGePoint3d& pPoint1, const OdGePoint3d& pPoint2, const OdGePoint3d& pPoint3);

  /** \details
    Adds a shell to the view.
	
	\param nbVertex [in] Input number of points.  
	\param pVertexList [in] Input array of vertices. 
	\param faceListSize [in] Input number of faces. 
	\param pFaceList [in] Input array of faces. 
	\param pEdgeData [in] Input array of edge's data.  
    \param pFaceData [in] Input array of face's data. 
    \param pVertexData [in] Input array of vertex's data. 
	\param pNormal [in] Input normal vector. 
  */
  void setShell(
            OdInt32 nbVertex,
            const OdGePoint3d* pVertexList,
            OdInt32 faceListSize,
            const OdInt32* pFaceList,
            const OdGiEdgeData* pEdgeData,
            const OdGiFaceData* pFaceData,
            const OdGiVertexData* pVertexData,
            const OdGeVector3d* pNormal = 0 );

  //=============
  void circleProcEx(
            const OdGePoint3d& center, 
            double radius, 
            const OdGeVector3d& normal, 
            const OdGeVector3d* pExtrusion = 0);
  
  void circleProcEx(
            const OdGePoint3d&, 
            const OdGePoint3d&, 
            const OdGePoint3d&, 
            const OdGeVector3d* pExtrusion = 0);

  void circularArcProcEx(
            const OdGePoint3d& center,
            double radius,
            const OdGeVector3d& normal,
            const OdGeVector3d& startVector,
            double sweepAngle,
            OdGiArcType arcType = kOdGiArcSimple, 
            const OdGeVector3d* pExtrusion = 0);
  
  void circularArcProcEx(
            const OdGePoint3d& start,
            const OdGePoint3d& point,
            const OdGePoint3d& end,
            OdGiArcType arcType = kOdGiArcSimple, 
            const OdGeVector3d* pExtrusion = 0);
  
  void polylineProcEx(
            OdInt32 nbPoints, 
            const OdGePoint3d* pVertexList,
            const OdGeVector3d* pNormal = 0,
            const OdGeVector3d* pExtrusion = 0,
            OdGsMarker lBaseSubEntMarker = -1);

    void polygonProcEx(
            OdInt32 nbPoints, 
            const OdGePoint3d* pVertexList,
            const OdGeVector3d* pNormal = 0,
            const OdGeVector3d* pExtrusion = 0);

  void meshProcEx(
            OdInt32 rows,
            OdInt32 columns,
            const OdGePoint3d* pVertexList,
            const OdGiEdgeData* pEdgeData = 0,
            const OdGiFaceData* pFaceData = 0,
            const OdGiVertexData* pVertexData = 0);

  void shellProcEx(
            OdInt32 nbVertex,
            const OdGePoint3d* pVertexList,
            OdInt32 faceListSize,
            const OdInt32* pFaceList,
            const OdGiEdgeData* pEdgeData = 0,
            const OdGiFaceData* pFaceData = 0,
            const OdGiVertexData* pVertexData = 0);

  void shapeProcEx(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            int shapeNo, 
            const OdGiTextStyle* pStyle,
            const OdGeVector3d* pExtrusion = 0);

  void nurbsProcEx(
            const OdGeNurbCurve3d& nurbs);

  void textProcEx(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool raw, 
            const OdGiTextStyle* pTextStyle,
            const OdGeVector3d* pExtrusion = 0);

  void ellipArcProcEx(
            const OdGeEllipArc3d& arc,
            const OdGePoint3d* pEndPointsOverrides,
            OdGiArcType arcType,
            const OdGeVector3d* pExtrusion);  

}; 
}

#endif  // _3DDWFDRAWOBJECT_INCLUDED_
