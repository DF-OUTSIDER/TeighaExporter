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

#ifndef _EXGIGDIVIEWPORTCLIPPER_H_INCLUDED_
#define _EXGIGDIVIEWPORTCLIPPER_H_INCLUDED_

#include "TD_PackPush.h"

#define GDI_USE_ENHANCED_CLIPPER

#ifndef GDI_USE_ENHANCED_CLIPPER
#include "Gs/GsExtAccum.h"
#include "Gi/GiOrthoPrismIntersector.h"
#include "Gi/GiConveyorEntryPoint.h"
#else // GDI_USE_ENHANCED_CLIPPER
#include "Gi/GiOrthoClipperEx.h"
#endif // GDI_USE_ENHANCED_CLIPPER

#ifndef GDI_USE_ENHANCED_CLIPPER
/** \details
  This class implements Viewport Clipping entry for GDI Vectorizer Device objects.
  <group ExRender_Windows_Classes> 
*/
class OdGiGDIViewportClippingEntry : public OdGiConveyorNode,
                                     public OdGsConveyorNodeBase,
                                     public OdGiConveyorGeometry
{
  OdGiOrthoPrismIntersectorPtr m_pClipper;
  OdGiConveyorEntryPoint m_clipperEntryPoint;
  OdGiConveyorContext *m_pDrawContext;
protected:
  OdGiGDIViewportClippingEntry();
public:
  static OdSmartPtr<OdGiGDIViewportClippingEntry> createObject();

  OdGiConveyorGeometry* optionalGeometry();

  void set(const OdGsDCPoint* points, const int *pointCounts, int numPolygons);
  void set(const OdGsDCPoint& min, const OdGsDCPoint& max);

  void setDrawContext(OdGiConveyorContext* pDrawContext);
  OdGiConveyorGeometry& geometry();
  void setDeviation(const OdGeDoubleArray& deviations);
  void setDeviation(const OdGiDeviation* pDeviation);

  // OdGiConveyorNode methods

  virtual OdGiConveyorInput& input();
  virtual OdGiConveyorOutput& output();

  // OdGiConveyorGeometry functions

  virtual void plineProc(const OdGiPolyline& polyline,
                         const OdGeMatrix3d* pXfm = 0,
                         OdUInt32 fromIndex = 0,
                         OdUInt32 numSegs = 0);
  virtual void polylineProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                            const OdGeVector3d* pNormal = 0,
                            const OdGeVector3d* pExtrusion = 0,
                            OdGsMarker baseSubEntMarker = -1);
  virtual void polygonProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                           const OdGeVector3d* pNormal = 0,
                           const OdGeVector3d* pExtrusion = 0);
  virtual void xlineProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
  virtual void rayProc(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);
  virtual void meshProc(OdInt32 numRows,
                        OdInt32 numColumns,
                        const OdGePoint3d* vertexList,
                        const OdGiEdgeData* pEdgeData = 0,
                        const OdGiFaceData* pFaceData = 0,
                        const OdGiVertexData* pVertexData = 0);
  virtual void shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                         OdInt32 faceListSize, const OdInt32* faceList,
                         const OdGiEdgeData* pEdgeData = 0,
                         const OdGiFaceData* pFaceData = 0,
                         const OdGiVertexData* pVertexData = 0);
  virtual void circleProc(const OdGePoint3d& center,
                          double radius,
                          const OdGeVector3d& normal,
                          const OdGeVector3d* pExtrusion = 0);
  virtual void circleProc(const OdGePoint3d& firstPoint,
                          const OdGePoint3d& secondPoint,
                          const OdGePoint3d& thirdPoint,
                          const OdGeVector3d* pExtrusion = 0);
  virtual void circularArcProc(const OdGePoint3d& center,
                               double radius,
                               const OdGeVector3d& normal,
                               const OdGeVector3d& startVector,
                               double sweepAngle,
                               OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  virtual void circularArcProc(const OdGePoint3d& firstPoint,
                               const OdGePoint3d& secondPoint,
                               const OdGePoint3d& thirdPoint,
                               OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  virtual void ellipArcProc(const OdGeEllipArc3d& ellipArc,
                            const OdGePoint3d* endPointOverrides = 0,
                            OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  virtual void nurbsProc(const OdGeNurbCurve3d& nurbsCurve);
  virtual void textProc(const OdGePoint3d& position,
                        const OdGeVector3d& direction,
                        const OdGeVector3d& upVector,
                        const OdChar* msg,
                        OdInt32 numBytes,
                        bool raw,
                        const OdGiTextStyle* pTextStyle,
                        const OdGeVector3d* pExtrusion = 0);
  virtual void textProc2(const OdGePoint3d& position,
                         const OdGeVector3d& direction,
                         const OdGeVector3d& upVector,
                         const OdChar* msg,
                         OdInt32 numBytes,
                         bool raw,
                         const OdGiTextStyle* pTextStyle,
                         const OdGeVector3d* pExtrusion = 0,
                         const OdGeExtents3d* extentsBox = 0);
  virtual void shapeProc(const OdGePoint3d& position,
                         const OdGeVector3d& direction,
                         const OdGeVector3d& upVector,
                         int shapeNumber,
                         const OdGiTextStyle* pTextStyle,
                         const OdGeVector3d* pExtrusion = 0);
  virtual void rasterImageProc(const OdGePoint3d& origin,
                               const OdGeVector3d& u,
                               const OdGeVector3d& v,
                               const OdGiRasterImage* pImage,
                               const OdGePoint2d* uvBoundary,
                               OdUInt32 numBoundPts,
                               bool transparency = false,
                               double brightness = 50.0,
                               double contrast = 50.0,
                               double fade = 0.0);
  virtual void metafileProc(const OdGePoint3d& origin,
                            const OdGeVector3d& u,
                            const OdGeVector3d& v,
                            const OdGiMetafile* pMetafile,
                            bool dcAligned = true,
                            bool allowClipping = false);
  virtual void xlineProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction);
  virtual void rayProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction);
  virtual void setExtentsProc(const OdGePoint3d *pPoints, bool bTransform = true);
  virtual int ttfCharProcFlags() const;
  virtual bool ttfCharProc(OdChar character, const OdGePoint3d& position,
                           const OdGeBoundBlock3d* pBoundBlock);
  virtual void ttfPolyDrawProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                               OdInt32 faceListSize, const OdInt32* faceList,
                               const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData = 0);
  virtual void conveyorBoundaryInfoProc(const OdGeBoundBlock3d &boundBlock,
                                        OdUInt32 &outputFlags);
  virtual void polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors,
                             const OdCmTransparency* pTransparency = 0, const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
                             const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0);
  virtual void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGDIViewportClippingEntry object pointers.
*/
typedef OdSmartPtr<OdGiGDIViewportClippingEntry> OdGiGDIViewportClippingEntryPtr;

/** \details
  This class implements Viewport Clipping list entry.
  <group ExRender_Windows_Classes> 
*/
struct OdGiGDIViewportClippingListEntry
{
  OdGiGDIViewportClippingEntryPtr m_pEntry;
  OdGiGDIViewportClippingListEntry *m_pNext;
  OdGiGDIViewportClippingListEntry()
    : m_pEntry(OdGiGDIViewportClippingEntry::createObject())
    , m_pNext(NULL)
  { }
};

/** \details
  This class implements storage container for temporary conveyor node elements.
  <group ExRender_Windows_Classes> 
*/
struct OdGiGDIConvCtxStorage
{
  enum Flags
  {
    kContextSet = 1,
    kDevArraySet = 2,
    kDeviationSet = 4
  };
  OdUInt8 m_flags;
  OdGiConveyorContext *m_pDrawContext;
  OdGeDoubleArray m_deviations;
  const OdGiDeviation *m_pDeviation;
  OdGiGDIConvCtxStorage() : m_flags(0), m_pDrawContext(NULL), m_pDeviation(NULL) { }
  void setDrawContext(OdGiConveyorContext* pDrawContext, OdGiGDIViewportClippingListEntry *pEntry);
  void setDeviation(const OdGeDoubleArray& deviations, OdGiGDIViewportClippingListEntry *pEntry);
  void setDeviation(const OdGiDeviation* pDeviation, OdGiGDIViewportClippingListEntry *pEntry);
  void init(OdGiGDIViewportClippingListEntry *pEntry);
};

#endif // GDI_USE_ENHANCED_CLIPPER

/** \details
  This class implements Viewport Clipping manager for GDI Vectorizer Device objects.
  <group ExRender_Windows_Classes> 
*/
class OdGiGDIViewportClipping : public OdGiConveyorNode
#ifndef GDI_USE_ENHANCED_CLIPPER
                              , public OdGsConveyorNodeBase
#endif // GDI_USE_ENHANCED_CLIPPER
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  OdGiGDIConvCtxStorage m_ctxStorage;
  OdGiGDIViewportClippingListEntry *m_pClipList;
  OdGiGDIViewportClippingListEntry *m_pClipListTail;
#else // GDI_USE_ENHANCED_CLIPPER
  OdGiOrthoClipperExPtr m_pClipper;
#endif // GDI_USE_ENHANCED_CLIPPER
protected:
  OdGiGDIViewportClipping();
public:
  static OdSmartPtr<OdGiGDIViewportClipping> createObject();

#ifndef GDI_USE_ENHANCED_CLIPPER
  OdGiConveyorGeometry* optionalGeometry();
#else // GDI_USE_ENHANCED_CLIPPER
  bool isClipping() const { return !m_pClipper->isEmpty(); }
#endif // GDI_USE_ENHANCED_CLIPPER

  static bool acceptable(const OdGsDCPoint* points, const int *pointCounts, int numPolygons);
  bool accept(const OdGsDCPoint* points, const int *pointCounts, int numPolygons) const;
  bool push(const OdGsDCPoint* points, const int *pointCounts, int numPolygons);
  static bool acceptable(const OdGsDCPoint& min, const OdGsDCPoint& max);
  bool accept(const OdGsDCPoint& min, const OdGsDCPoint& max) const;
  bool push(const OdGsDCPoint& min, const OdGsDCPoint& max);
  bool pop();

  void setDrawContext(OdGiConveyorContext* pDrawContext);
  void setDeviation(const OdGeDoubleArray& deviations);
  void setDeviation(const OdGiDeviation* pDeviation);

  // OdGiConveyorNode methods

  virtual OdGiConveyorInput& input();
  virtual OdGiConveyorOutput& output();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGDIViewportClipping object pointers.
*/
typedef OdSmartPtr<OdGiGDIViewportClipping> OdGiGDIViewportClippingPtr;

#include "TD_PackPop.h"

#endif // _EXGIGDIVIEWPORTCLIPPER_H_INCLUDED_
