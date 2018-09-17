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

#ifndef __ODGICONVEYORGEOMETRY_H__
#define __ODGICONVEYORGEOMETRY_H__

#include "Gi/GiGeometry.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/GsSelectionReactor.h"
#include "Gi/GiSubEntityTraitsData.h"

class OdGiRasterImage;
class OdGiMetafile;
class OdGiViewport;
class OdGsView;
class OdGiCommonDraw;
class OdGiDeviation;
class OdGeExtents3d;
class OdGeBoundBlock3d;

/** \details
    This class implements an interface that allows conveyor nodes to interact with parent objects.
    
    \remarks
    This interface gives
    abstract access to OdGiBaseVectorizer level data and services.
    Many conveyor nodes require a pointer to an OdGiConveyorContext interface,
    in order to be initialized.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiConveyorContext
{
public:
  virtual ~OdGiConveyorContext() {}

  /** \details
    Returns the OdGiContext object associated with this ConveyorContext object.
  */
  virtual OdGiContext& giContext() const = 0;

  /** \details
    Returns the OdGiSubEntityTraits instance associated with this ConveyorContext object.
    
    \remarks
    The OdGiSubEntityTraits object is used to register attribute changes with
    the vectorization framework.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const = 0;

  /** \details
    Returns the traits currently in effect for this ConveyorContext object.
  */
  virtual const OdGiSubEntityTraitsData& effectiveTraits() const = 0;

  /** \details
    Sets the traits currently in effect for this ConveyorContext object.
    \param traits [in]  New traits data which will be used for effective traits substitution.
    \param fillNormal [in]  Optional fill normal.
  */
  virtual void setEffectiveTraits(const OdGiSubEntityTraitsData& traits,
                                  const OdGeVector3d* fillNormal = NULL) = 0;

  /** \details
    Checks the current visibility state for current traits.
  */
  virtual bool effectivelyVisible() const { return effectiveTraits().visibility(); }

  /** \details
    Returns the current OdGiDrawableDesc instance for this ConveyorContext object.
    
    \remarks
    This instance contains information about the current drawable object (the object itself, rendering flags, etc.).
  */
  virtual const OdGiDrawableDesc* currentDrawableDesc() const = 0;

  /** \details
    Returns the current OdGiDrawable object for this ConveyorContext object.
    
    \remarks
    This is the object currently being vectorized by the framework.
  */
  virtual const OdGiDrawable* currentDrawable() const = 0;

  /** \details
    Returns the OdGiViewport associated with this ConveyorContext object.
  */
  virtual const OdGiViewport* giViewport() const = 0;

  /** \details
      Returns the OdGsView associated with this ConveyorContext object.
  */
  virtual const OdGsView* gsView() const = 0;

  /** \details
    Notification function called by the vectorization framework
    whenever the rendering attributes have changed.

    \remarks
    This function is called before sending any geometry through the geometry conveyor.

    This function provides a hook
    for setting rendering attributes for the final geometry handler (e.g. Win GDI device).
  */
  virtual void onTraitsModified() = 0;

  /** \details
    Notification function called by the vectorization framework
    whenever the text is become to be rendered.

    \remarks
    This function provides a hook for setting geometry transformation before text was drawn.
  */
  virtual void onTextProcessing(const OdGePoint3d& /*position*/,
                                const OdGeVector3d& /*direction*/,
                                const OdGeVector3d& /*upVector*/) { }

  /** \details
    Returns true if and only if regen was aborted.
  */
  virtual bool regenAbort() const = 0;

  /** \details
    Returns current drawable nesting graph.
  */
  virtual const OdGiPathNode* currentGiPath() const = 0;

  /** \details
    Returns deviations array accessor for world coordinate system.
  */
  virtual const OdGiDeviation& worldDeviation() const = 0;
  /** \details
    Returns deviations array accessor for model coordinate system.
  */
  virtual const OdGiDeviation& modelDeviation() const = 0;
  /** \details
    Returns deviations array accessor for eye coordinate system.
  */
  virtual const OdGiDeviation& eyeDeviation() const = 0;

  /** \details
    Returns the model-to-world coordinate transform matrix for the entity being vectorized.
    
    \remarks
    This is the inverse of the matrix returned by getWorldToModelTransform(). 

    If an entity is in one or more blocks, this matrix can be used to
    determine the WCS coordinates of the entity.  
  */
  virtual OdGeMatrix3d getModelToWorldTransform() const = 0;

  /** \details
    Returns the world-to-model coordinate transform matrix for the entity being vectorized.
    
    \remarks
    This is the inverse of the matrix returned by getModelToWorldTransform(). 
  */
  virtual OdGeMatrix3d getWorldToModelTransform() const = 0;

  /** \details
    Returns current lineweight override (if present).
  */
  virtual const OdGiLineweightOverride *currentLineweightOverride() const { return NULL; }

  enum ConveyorContextFlags
  {
    // Spatial filter as simplifier
    kSpatialFilterSimplPline = 1, // Simplify plineProc() by spatial filter
    kSpatialFilterSimplNurbs = 2, // Simplify nurbsProc() by spatial filter
    kSpatialFilterSimplText  = 4, // Simplify textProc () by spatial filter
    kSpatialFilterSimplShape = 8, // Simplify shapeProc() by spatial filter
    kSpatialFilterSimplAll   = (kSpatialFilterSimplPline | kSpatialFilterSimplNurbs |
                                kSpatialFilterSimplText  | kSpatialFilterSimplShape), // All spatial filter options
    // Conveyor embranchment as simplifier
    kEmbranchmentSimplText   = 16, // Simplify textProc () by conveyor embranchment
    kEmbranchmentSimplNurbs  = 32, // Simplify nurbsProc() by conveyor embranchment
    kEmbranchmentSimplAll    = (kEmbranchmentSimplText | kEmbranchmentSimplNurbs), // All conveyor embranchment options
    kConveyorSimplAll        = (kSpatialFilterSimplAll | kEmbranchmentSimplAll), // All simplification options
    // Pline subentity markers
    kPlineMarkers            = 64, // Enable subentity markers set per pline() call
    kForceMarkersOnModified  = 128, // Force onTraitsModified call for each subentity marker change
    // Polyline subentity markers
    kPolylineMarkers         = 256, // Enable subentity markers set per polyline() call
    // Pline geometry
    kPlineAllowArcProc       = 512, // Vectorizer will draw arc segments of polyline as arcs (otherwise as sample points)
    // Enable test-specific functionality
    kTestMode                = 1024, // Enable test mode
    // Specific vectorization mode
    kLineTyperAfterMetafile  = 2048  // Specific mode (for example in GDI device) when the Line type node are follows by the metafile
  };
  /** \details
    Returns set of conveyor context flags.
  */
  virtual OdUInt32 drawContextFlags() const { return 0; }

  /** \details
    Returns current annotationScale value of conveyor context.
  */
  virtual double annotationScale() const { return 1.0; }
};

/** \details

    \sa
    TD_Gi 
    <group OdGi_Classes> 
*/
class OdGiConveyorContextWrapper : public OdGiConveyorContext
{
protected:
  OdGiConveyorContext *m_pCtx;
public:
  OdGiConveyorContext *getOriginalContext() const { return m_pCtx; }
  void setOriginalContext(OdGiConveyorContext *pCtx) { m_pCtx = pCtx; }
  explicit OdGiConveyorContextWrapper(OdGiConveyorContext *pCtx = NULL) : m_pCtx(pCtx) { }

  OdGiContext& giContext() const { return m_pCtx->giContext(); }
  OdGiSubEntityTraits& subEntityTraits() const { return m_pCtx->subEntityTraits(); }
  const OdGiSubEntityTraitsData& effectiveTraits() const { return m_pCtx->effectiveTraits(); }
  void setEffectiveTraits(const OdGiSubEntityTraitsData& traits,
                          const OdGeVector3d* fillNormal = NULL) { m_pCtx->setEffectiveTraits(traits, fillNormal); }
  bool effectivelyVisible() const { return m_pCtx->effectivelyVisible(); }
  const OdGiDrawableDesc* currentDrawableDesc() const { return m_pCtx->currentDrawableDesc(); }
  const OdGiDrawable* currentDrawable() const { return m_pCtx->currentDrawable(); }
  const OdGiViewport* giViewport() const { return m_pCtx->giViewport(); }
  const OdGsView* gsView() const { return m_pCtx->gsView(); }
  void onTraitsModified() { m_pCtx->onTraitsModified(); }
  void onTextProcessing(const OdGePoint3d& position, const OdGeVector3d& direction, const OdGeVector3d& upVector)
    { m_pCtx->onTextProcessing(position, direction, upVector); }
  bool regenAbort() const { return m_pCtx->regenAbort(); }
  const OdGiPathNode* currentGiPath() const { return m_pCtx->currentGiPath(); }
  const OdGiDeviation& worldDeviation() const { return m_pCtx->worldDeviation(); }
  const OdGiDeviation& modelDeviation() const { return m_pCtx->modelDeviation(); }
  const OdGiDeviation& eyeDeviation() const { return m_pCtx->eyeDeviation(); }
  OdGeMatrix3d getModelToWorldTransform() const { return m_pCtx->getModelToWorldTransform(); }
  OdGeMatrix3d getWorldToModelTransform() const { return m_pCtx->getWorldToModelTransform(); }
  const OdGiLineweightOverride *currentLineweightOverride() const { return m_pCtx->currentLineweightOverride(); }
  OdUInt32 drawContextFlags() const { return m_pCtx->drawContextFlags(); }
};

/** \details
    This class defines the interface through which nodes in the Teigha vectorization
    pipeline transfer geometric data.
    \sa
    TD_Gi
    <group OdGi_Classes> 
*/
class OdGiConveyorGeometry
{
public:
  virtual ~OdGiConveyorGeometry() {}

  /** \details
    Processes OdGiPolyline data.

    \param polyline [in]  Polyline data to be processed.
    \param pXfm [in]  Pointer to a transformation matrix.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed.
    \remarks
    numSegs == 0 indicates all segments starting at fromIndex are to be processed. 
      
  */
  virtual void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* pXfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0) = 0;

  /** \details
    Processes simple polyline data.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \param baseSubEntMarker [in]  Currently unused.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
  */
  virtual void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1) = 0;


#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning (push)
#pragma warning (disable: 4481) // Non-standard extension used

// The right function prototype was changed (see above). Old one left to cause compilation error
  virtual void polylineProc(
    OdInt32 /*numPoints*/, const OdGePoint3d* /*vertexList*/,
    const OdGeVector3d* /*pNormal*/,
    const OdGeVector3d* /*pExtrusion*/,
    OdInt32 /*baseSubEntMarker*/) sealed {ODA_FAIL();}
#pragma warning (pop)
#endif

  /** \details
    Processes polygon data.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
  */
  virtual void polygonProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes Xline data.
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \remarks
    An Xline is an infinite line passing through the 
    specified points.
  */
  virtual void xlineProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint) = 0;

  /** \details
    Processes Ray data.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point
    
    \remarks
    A Ray is a semi-infinite line that starts at the basePoint, and passes 
    through the throughPoint.
  */
  virtual void rayProc(
    const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint) = 0;

  /** \details
    Processes mesh data.

    \param numRows [in]  Number of rows.
    \param numColumns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 

    \sa
    OdGiEdgeData
    OdGiFaceData
    OdGiVertexData
    Vectorization Primitives
  */
  virtual void meshProc(
    OdInt32 numRows, 
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0) = 0;

  /** \details
    Processes shell data.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  Array of numbers that define the faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 

    \sa
    Vectorization Primitives      
  */
  virtual void shellProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0) = 0;

  /** \details
    Process circle data. 

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
  */
  virtual void circleProc(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d* pExtrusion = 0) = 0;

   /** \details
    Process circle data. 
    \param firstPoint [in]  First point of a 3-point circle.
    \param secondPoint [in]  Second point of a 3-point circle.
    \param thirdPoint [in]  Third point of a 3-point circle.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    
    \remarks
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident.
  */
  virtual void circleProc(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint, 
    const OdGeVector3d* pExtrusion = 0) = 0;
  
  /** \details
    Processes circular arc data.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Defines the start of this arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>

    The extrusion vector specifies the direction and distance of the extrusion.
    
    \remarks
    All angles are expressed in radians.
  */
  virtual void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0) = 0;
  
  /** \details
    Processes circular arc data.

    \param firstPoint [in]  First point of a 3-point arc.
    \param secondPoint [in]  Second point of a 3-point arc.
    \param thirdPoint [in]  Third point of a 3-point arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    
    \remarks
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident.
  */
  virtual void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes OdGeEllipArc3d data.

    \param ellipArc [in]  Elliptical arc.
    \param endPointOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
        
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     3         Filled area bounded by the arc and its end points
    </table>

    The extrusion vector specifies the direction and distance of the extrusion.

  */
  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes OdGeNurbCurve3d data.
    \param nurbsCurve [in]  NURBS curve data.
  */
  virtual void nurbsProc(
    const OdGeNurbCurve3d& nurbsCurve) = 0;

  /** \details
    Processes text data.

    \param position [in]  Position of the text.
    \param direction [in]  Baseline direction of the text.
    \param upVector [in]  Up vector for the text.
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg.
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for the text.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
  */
  virtual void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0) = 0;

  virtual void textProc2(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0,
    const OdGeExtents3d* /*extentsBox*/ = 0)
  {
    textProc(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion);
  }

  /** \details
    Processes shape data.

    \param position [in]  Position of the shape.
    \param direction [in]  Baseline direction of the shape.
    \param upVector [in]  Up vector for the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
  */
  virtual void shapeProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes raster images data.

    \param origin [in]  Lower-left corner. 
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    \param pImage [in]  Pointer to the RasterImage object.
    \param uvBoundary [in]  Array of image boundary points (may not be null).
    \param numBoundPts [in]  Number of boundary points.
    \param transparency [in]  True if and only if image transparency is on.
    \param brightness [in]  Image brightness [0.0 .. 100.0].
    \param contrast [in]  Image contrast [0.0 .. 100.0].
    \param fade [in]  Image fade value [0.0 .. 100.0].
  */
  virtual void rasterImageProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) = 0;

  /** \details
    Processes metafile data.

    \param origin [in]  Metafile origin.
    \param u [in]  Metafile width vector.
    \param v [in]  Metafile height vector.
    \param pMetafile [in]  Pointer to the metafile object.
    \param dcAligned [in]  reserved.
    \param allowClipping [in]  reserved.
  */
  virtual void metafileProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,
    bool allowClipping = false) = 0;

  virtual void xlineProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
  {
    xlineProc(basePoint, basePoint + direction);
  }
  virtual void rayProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
  {
    rayProc(basePoint, basePoint + direction);
  }

  virtual void setExtentsProc(const OdGePoint3d * /*pPoints*/, bool /*bTransform*/ = true) { }

  enum ETtfCharProcFlags
  {
    kTtfCharProcEnable     = 1,
    kTtfCharProcBoundBlock = 2
  };
  virtual int ttfCharProcFlags() const { return 0; }
  virtual bool ttfCharProc(OdChar /*character*/, const OdGePoint3d& /*position*/,
    const OdGeBoundBlock3d* /*pBoundBlock*/) { return false; }

  virtual void ttfPolyDrawProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdUInt8* /*pBezierTypes*/, const OdGiFaceData* pFaceData = 0)
  {
    shellProc(numVertices, vertexList, faceListSize, faceList, 0, pFaceData);
  }

  enum EConveyorBoundaryInfoFlags
  {
    kBoundaryProcXform       = 1, // Non-identity Xform available at least once
    kBoundaryProcXformNonUni = 2, // Non-uni-scale Xform available at least once
    kBoundaryProcProjection  = 4, // Projective Xform available at least once
    kBoundaryProcClip        = 8, // Boundary clipped partially or completely at least once
    kBoundaryProcClipFull   = 16 // Boundary completely clipped at least once
  };
  virtual void conveyorBoundaryInfoProc(
    const OdGeBoundBlock3d & /*boundBlock*/, // {I} represent block for apply check
    OdUInt32 & /*outputFlags*/) // {O} will represent set of EConveyorBoundaryInfoFlags flags on out
  {
  }

  void polypointProc2(OdGiConveyorContext* pContext,
    OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors = 0,
    const OdCmTransparency *pTransparency = 0, const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0);
  void rowOfDotsProc2(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \details
    Processes array of points.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormals [in]  Array of normal vectors.
    \param pExtrusions [in]  Array of extrusion vectors.
    \param pSubEntMarkers [in]  Array of subentity markers.
    \remarks
    The extrusion vectors specifies the direction and distance of the extrusion.
  */
  virtual void polypointProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0, const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0) = 0;

  /** \details
    Processes row of points.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  virtual void rowOfDotsProc(
    OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint) = 0;

  /** \details
    Processes hatch boundary.

    \param edges [in]  2d curves array.
    \param pXform [in]  Optional boundary transform matrix.
  */
  virtual void edgeProc(
    const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0) = 0;
};

inline
void OdGiConveyorGeometry::polypointProc2(OdGiConveyorContext* pContext,
  OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors,
  const OdCmTransparency* pTransparency, const OdGeVector3d* pNormals, const OdGeVector3d* pExtrusions,
  const OdGsMarker* pSubEntMarkers, OdInt32 /*nPointSize*/)
{
  OdGePoint3d tmpVerts[2];
  for (OdInt32 nPoint = 0; nPoint < numPoints; nPoint++)
  {
    OdGsMarker baseSubEntMarker = -1;
    if ((pColors || pTransparency || pSubEntMarkers) && pContext)
    {
      if (pColors)
        pContext->subEntityTraits().setTrueColor(pColors[nPoint]);
      if (pTransparency)
        pContext->subEntityTraits().setTransparency(pTransparency[nPoint]);
      if (pSubEntMarkers)
        pContext->subEntityTraits().setSelectionMarker(pSubEntMarkers[nPoint]);
      pContext->onTraitsModified();
    }
    else if (pSubEntMarkers)
      baseSubEntMarker = pSubEntMarkers[nPoint];
    const OdGeVector3d *pNormal = (pNormals) ? (pNormals + nPoint) : NULL;
    const OdGeVector3d *pExtrusion = (pExtrusions) ? (pExtrusions + nPoint) : NULL;
    tmpVerts[1] = tmpVerts[0] = vertexList[nPoint];
    polylineProc(2, tmpVerts, pNormal, pExtrusion, baseSubEntMarker);
  }
}

inline
void OdGiConveyorGeometry::rowOfDotsProc2(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
{
  OdGePoint3d tmpVerts[2];
  for (OdInt32 nPoint = 0; nPoint < numPoints; nPoint++)
  {
    tmpVerts[1] = tmpVerts[0] = startPoint + dirToNextPoint * double(nPoint);
    polylineProc(2, tmpVerts);
  }
}

#endif //#ifndef __ODGICONVEYORGEOMETRY_H__
