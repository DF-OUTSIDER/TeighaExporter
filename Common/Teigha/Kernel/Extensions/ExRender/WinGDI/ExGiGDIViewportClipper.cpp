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

#include "OdaCommon.h"
#include "RxObjectImpl.h"
#include "Gi/GiGeometryRecorderPlayer.h"
#include "Gi/GiTextStyle.h"
#include "ExGiGDIViewportClipper.h"

#ifndef GDI_USE_ENHANCED_CLIPPER

// OdGiGDIViewportClippingEntry

OdGiGDIViewportClippingEntry::OdGiGDIViewportClippingEntry()
  : m_pDrawContext(NULL)
{
  m_pClipper = OdGiOrthoPrismIntersector::createObject();
  m_pClipper->input().addSourceNode(m_clipperEntryPoint);
}

OdSmartPtr<OdGiGDIViewportClippingEntry> OdGiGDIViewportClippingEntry::createObject()
{
  return OdRxObjectImpl<OdGiGDIViewportClippingEntry>::createObject();
}

OdGiConveyorGeometry* OdGiGDIViewportClippingEntry::optionalGeometry()
{
  return this;
}

void OdGiGDIViewportClippingEntry::set(const OdGsDCPoint* points, const int *pointCounts, int /*numPolygons*/)
{
  OdGePoint2dArray pts;
  pts.resize((OdUInt32)*pointCounts);
  for (int i = 0; i < *pointCounts; i++)
    pts[i].set((double)points[i].x, (double)points[i].y);
  m_pClipper->set(pts);
}

void OdGiGDIViewportClippingEntry::set(const OdGsDCPoint& min, const OdGsDCPoint& max)
{
  const OdGePoint2d pts[4] = { OdGePoint2d((double)min.x, (double)min.y),
                               OdGePoint2d((double)max.x, (double)min.y),
                               OdGePoint2d((double)max.x, (double)max.y),
                               OdGePoint2d((double)min.x, (double)max.y) };
  m_pClipper->set(4, pts);
}

void OdGiGDIViewportClippingEntry::setDrawContext(OdGiConveyorContext* pDrawContext)
{
  m_pClipper->setDrawContext(m_pDrawContext = pDrawContext);
}

OdGiConveyorGeometry& OdGiGDIViewportClippingEntry::geometry()
{
  return *this;
}

void OdGiGDIViewportClippingEntry::setDeviation(const OdGeDoubleArray& deviations)
{
  m_pClipper->setDeviation(deviations);
}

void OdGiGDIViewportClippingEntry::setDeviation(const OdGiDeviation* pDeviation)
{
  m_pClipper->setDeviation(pDeviation);
}

OdGiConveyorInput& OdGiGDIViewportClippingEntry::input()
{
  return *this;
}

OdGiConveyorOutput& OdGiGDIViewportClippingEntry::output()
{
  return *this;
}

#define OD_GVC_PROCESS(func) \
  OdGiGeometryRecorderPlayerPtr pRec = OdGiGeometryRecorderPlayer::createObject(); \
  m_pClipper->clearClipStatus(); \
  m_pClipper->output().setDestGeometry(pRec->recorderGeometry()); \
  m_clipperEntryPoint.geometry().func; \
  OdUInt32 clipStatus = m_pClipper->clipStatus(); \
  m_pClipper->clearClipStatus(); \
  if (clipStatus) \
    pRec->play(destGeometry()); \
  else \
    destGeometry().func; \
  m_pClipper->output().setDestGeometry(destGeometry())

#define OD_GVC_PREPAREPASSTHROUGH() \
  if (&m_pClipper->output().destGeometry() != &destGeometry()) \
    m_pClipper->output().setDestGeometry(destGeometry())

void OdGiGDIViewportClippingEntry::plineProc(const OdGiPolyline& polyline,
                                             const OdGeMatrix3d* pXfm,
                                             OdUInt32 fromIndex,
                                             OdUInt32 numSegs)
{
  //OD_GVC_PROCESS(plineProc(polyline, pXfm, fromIndex, numSegs));
  // Pass entirely through clipper since there is no any effectivity from initial primitive structure preserving
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().plineProc(polyline, pXfm, fromIndex, numSegs);
}

void OdGiGDIViewportClippingEntry::polylineProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                                                const OdGeVector3d* pNormal,
                                                const OdGeVector3d* pExtrusion,
                                                OdGsMarker baseSubEntMarker)
{
  //OD_GVC_PROCESS(polylineProc(numPoints, vertexList, pNormal, pExtrusion, baseSubEntMarker));
  // Pass entirely through clipper since there is no any effectivity from initial primitive structure preserving
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().polylineProc(numPoints, vertexList, pNormal, pExtrusion, baseSubEntMarker);
}

void OdGiGDIViewportClippingEntry::polygonProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                                               const OdGeVector3d* pNormal,
                                               const OdGeVector3d* pExtrusion)
{
  //OD_GVC_PROCESS(polygonProc(numPoints, vertexList, pNormal, pExtrusion));
  // Pass entirely through clipper since there is no any effectivity from initial primitive structure preserving
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().polygonProc(numPoints, vertexList, pNormal, pExtrusion);
}

void OdGiGDIViewportClippingEntry::xlineProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint)
{
  // Run always through clipper
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().xlineProc(firstPoint, secondPoint);
}

void OdGiGDIViewportClippingEntry::rayProc(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint)
{
  // Run always through clipper
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().rayProc(basePoint, throughPoint);
}

void OdGiGDIViewportClippingEntry::meshProc(OdInt32 numRows,
                                            OdInt32 numColumns,
                                            const OdGePoint3d* vertexList,
                                            const OdGiEdgeData* pEdgeData,
                                            const OdGiFaceData* pFaceData,
                                            const OdGiVertexData* pVertexData)
{
  //OD_GVC_PROCESS(meshProc(numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData));
  // Pass entirely through clipper since there is no any effectivity from initial primitive structure preserving
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().meshProc(numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData);
}

void OdGiGDIViewportClippingEntry::shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                                             OdInt32 faceListSize, const OdInt32* faceList,
                                             const OdGiEdgeData* pEdgeData,
                                             const OdGiFaceData* pFaceData,
                                             const OdGiVertexData* pVertexData)
{
  //OD_GVC_PROCESS(shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData));
  // Pass entirely through clipper since there is no any effectivity from initial primitive structure preserving
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
}

void OdGiGDIViewportClippingEntry::circleProc(const OdGePoint3d& center,
                                              double radius,
                                              const OdGeVector3d& normal,
                                              const OdGeVector3d* pExtrusion)
{
  OD_GVC_PROCESS(circleProc(center, radius, normal, pExtrusion));
}

void OdGiGDIViewportClippingEntry::circleProc(const OdGePoint3d& firstPoint,
                                              const OdGePoint3d& secondPoint,
                                              const OdGePoint3d& thirdPoint,
                                              const OdGeVector3d* pExtrusion)
{
  OD_GVC_PROCESS(circleProc(firstPoint, secondPoint, thirdPoint, pExtrusion));
}

void OdGiGDIViewportClippingEntry::circularArcProc(const OdGePoint3d& center,
                                                   double radius,
                                                   const OdGeVector3d& normal,
                                                   const OdGeVector3d& startVector,
                                                   double sweepAngle,
                                                   OdGiArcType arcType, const OdGeVector3d* pExtrusion)
{
  OD_GVC_PROCESS(circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion));
}

void OdGiGDIViewportClippingEntry::circularArcProc(const OdGePoint3d& firstPoint,
                                                   const OdGePoint3d& secondPoint,
                                                   const OdGePoint3d& thirdPoint,
                                                   OdGiArcType arcType, const OdGeVector3d* pExtrusion)
{
  OD_GVC_PROCESS(circularArcProc(firstPoint, secondPoint, thirdPoint, arcType, pExtrusion));
}

void OdGiGDIViewportClippingEntry::ellipArcProc(const OdGeEllipArc3d& ellipArc,
                                                const OdGePoint3d* endPointOverrides,
                                                OdGiArcType arcType, const OdGeVector3d* pExtrusion)
{
  OD_GVC_PROCESS(ellipArcProc(ellipArc, endPointOverrides, arcType, pExtrusion));
}

void OdGiGDIViewportClippingEntry::nurbsProc(const OdGeNurbCurve3d& nurbsCurve)
{
  OD_GVC_PROCESS(nurbsProc(nurbsCurve));
}

void OdGiGDIViewportClippingEntry::textProc(const OdGePoint3d& position,
                                            const OdGeVector3d& direction,
                                            const OdGeVector3d& upVector,
                                            const OdChar* msg,
                                            OdInt32 numBytes,
                                            bool raw,
                                            const OdGiTextStyle* pTextStyle,
                                            const OdGeVector3d* pExtrusion)
{
  OdGiSubEntityTraits& traits = m_pDrawContext->subEntityTraits();
  OdGiFillType fill = traits.fillType();
  OdUInt32 drawFlags = traits.drawFlags();
  bool bTraitsModified = false;
  if (pTextStyle->isTtfFont())
  {
    traits.setFillType(m_pDrawContext->giContext().fillTtf() ? kOdGiFillAlways : kOdGiFillNever);
    traits.setDrawFlags(traits.drawFlags() | OdGiSubEntityTraits::kDrawPolygonFill);
    bTraitsModified = (fill != traits.fillType());
    if (bTraitsModified)
      m_pDrawContext->onTraitsModified();
  }
  OD_GVC_PROCESS(textProc(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion));
  if (pTextStyle->isTtfFont())
  {
    traits.setFillType(fill);
    traits.setDrawFlags(drawFlags);
    if (bTraitsModified)
      m_pDrawContext->onTraitsModified();
  }
}

void OdGiGDIViewportClippingEntry::textProc2(const OdGePoint3d& position,
                                             const OdGeVector3d& direction,
                                             const OdGeVector3d& upVector,
                                             const OdChar* msg,
                                             OdInt32 numBytes,
                                             bool raw,
                                             const OdGiTextStyle* pTextStyle,
                                             const OdGeVector3d* pExtrusion,
                                             const OdGeExtents3d* extentsBox)
{
  OdGiSubEntityTraits& traits = m_pDrawContext->subEntityTraits();
  OdGiFillType fill = traits.fillType();
  OdUInt32 drawFlags = traits.drawFlags();
  bool bTraitsModified = false;
  if (pTextStyle->isTtfFont())
  {
    traits.setFillType(m_pDrawContext->giContext().fillTtf() ? kOdGiFillAlways : kOdGiFillNever);
    traits.setDrawFlags(traits.drawFlags() | OdGiSubEntityTraits::kDrawPolygonFill);
    bTraitsModified = (fill != traits.fillType());
    if (bTraitsModified)
      m_pDrawContext->onTraitsModified();
  }
  OD_GVC_PROCESS(textProc2(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion, extentsBox));
  if (pTextStyle->isTtfFont())
  {
    traits.setFillType(fill);
    traits.setDrawFlags(drawFlags);
    if (bTraitsModified)
      m_pDrawContext->onTraitsModified();
  }
}

void OdGiGDIViewportClippingEntry::shapeProc(const OdGePoint3d& position,
                                             const OdGeVector3d& direction,
                                             const OdGeVector3d& upVector,
                                             int shapeNumber,
                                             const OdGiTextStyle* pTextStyle,
                                             const OdGeVector3d* pExtrusion)
{
  OD_GVC_PROCESS(shapeProc(position, direction, upVector, shapeNumber, pTextStyle, pExtrusion));
}

void OdGiGDIViewportClippingEntry::rasterImageProc(const OdGePoint3d& origin,
                                                   const OdGeVector3d& u,
                                                   const OdGeVector3d& v,
                                                   const OdGiRasterImage* pImage,
                                                   const OdGePoint2d* uvBoundary,
                                                   OdUInt32 numBoundPts,
                                                   bool transparency,
                                                   double brightness,
                                                   double contrast,
                                                   double fade)
{
  //OD_GVC_PROCESS(rasterImageProc(origin, u, v, pImage, uvBoundary, numBoundPts, transparency, brightness, contrast, fade));
  // Pass entirely through clipper since there is no any effectivity from initial primitive structure preserving
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().rasterImageProc(origin, u, v, pImage, uvBoundary, numBoundPts, transparency, brightness, contrast, fade);
}

void OdGiGDIViewportClippingEntry::metafileProc(const OdGePoint3d& origin,
                                                const OdGeVector3d& u,
                                                const OdGeVector3d& v,
                                                const OdGiMetafile* pMetafile,
                                                bool dcAligned,
                                                bool allowClipping)
{
  // Skip clipping
  destGeometry().metafileProc(origin, u, v, pMetafile, dcAligned, allowClipping);
}

void OdGiGDIViewportClippingEntry::xlineProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
{
  // Run always through clipper
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().xlineProc2(basePoint, direction);
}

void OdGiGDIViewportClippingEntry::rayProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
{
  // Run always through clipper
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().rayProc2(basePoint, direction);
}

void OdGiGDIViewportClippingEntry::setExtentsProc(const OdGePoint3d *pPoints, bool bTransform)
{
  // Skip clipping
  destGeometry().setExtentsProc(pPoints, bTransform);
}

int OdGiGDIViewportClippingEntry::ttfCharProcFlags() const
{
  // Can't process
  return OdGiConveyorGeometry::ttfCharProcFlags();
}

bool OdGiGDIViewportClippingEntry::ttfCharProc(OdChar character, const OdGePoint3d& position,
                                               const OdGeBoundBlock3d* pBoundBlock)
{
  // Can't process
  return OdGiConveyorGeometry::ttfCharProc(character, position, pBoundBlock);
}

void OdGiGDIViewportClippingEntry::ttfPolyDrawProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                                                   OdInt32 faceListSize, const OdInt32* faceList,
                                                   const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData)
{
  OD_GVC_PROCESS(ttfPolyDrawProc(numVertices, vertexList, faceListSize, faceList, pBezierTypes, pFaceData));
}

void OdGiGDIViewportClippingEntry::conveyorBoundaryInfoProc(const OdGeBoundBlock3d &boundBlock,
                                                            OdUInt32 &outputFlags)
{
  // Run always through clipper
  OD_GVC_PREPAREPASSTHROUGH();
  m_clipperEntryPoint.geometry().conveyorBoundaryInfoProc(boundBlock, outputFlags);
}

void OdGiGDIViewportClippingEntry::polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors,
                                                 const OdCmTransparency* pTransparency, const OdGeVector3d* pNormals, const OdGeVector3d* pExtrusions,
                                                 const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize)
{
  OD_GVC_PROCESS(polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize));
}

void OdGiGDIViewportClippingEntry::rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
{
  OD_GVC_PROCESS(rowOfDotsProc(numPoints, startPoint, dirToNextPoint));
}

// OdGiGDIConvCtxStorage

void OdGiGDIConvCtxStorage::setDrawContext(OdGiConveyorContext* pDrawContext, OdGiGDIViewportClippingListEntry *pEntry)
{
  SETBIT_1(m_flags, kContextSet); m_pDrawContext = pDrawContext;
  while (pEntry)
  {
    pEntry->m_pEntry->setDrawContext(pDrawContext);
    pEntry = pEntry->m_pNext;
  }
}

void OdGiGDIConvCtxStorage::setDeviation(const OdGeDoubleArray& deviations, OdGiGDIViewportClippingListEntry *pEntry)
{
  SETBIT_1(m_flags, kDevArraySet); m_deviations = deviations;
  while (pEntry)
  {
    pEntry->m_pEntry->setDeviation(deviations);
    pEntry = pEntry->m_pNext;
  }
}

void OdGiGDIConvCtxStorage::setDeviation(const OdGiDeviation* pDeviation, OdGiGDIViewportClippingListEntry *pEntry)
{
  SETBIT_1(m_flags, kDeviationSet); m_pDeviation = pDeviation;
  while (pEntry)
  {
    pEntry->m_pEntry->setDeviation(pDeviation);
    pEntry = pEntry->m_pNext;
  }
}

void OdGiGDIConvCtxStorage::init(OdGiGDIViewportClippingListEntry *pEntry)
{
  if (m_flags != 0)
  {
    while (pEntry)
    {
      if (GETBIT(m_flags, kContextSet)) pEntry->m_pEntry->setDrawContext(m_pDrawContext);
      if (GETBIT(m_flags, kDevArraySet)) pEntry->m_pEntry->setDeviation(m_deviations);
      if (GETBIT(m_flags, kDeviationSet)) pEntry->m_pEntry->setDeviation(m_pDeviation);
      pEntry = pEntry->m_pNext;
    }
  }
}

#endif // GDI_USE_ENHANCED_CLIPPER

// OdGiGDIViewportClipping

OdGiGDIViewportClipping::OdGiGDIViewportClipping()
#ifndef GDI_USE_ENHANCED_CLIPPER
  : m_pClipList(NULL)
  , m_pClipListTail(NULL)
#endif // GDI_USE_ENHANCED_CLIPPER
{
#ifdef GDI_USE_ENHANCED_CLIPPER
  m_pClipper = OdGiOrthoClipperEx::createObject();
#endif // GDI_USE_ENHANCED_CLIPPER
}

OdSmartPtr<OdGiGDIViewportClipping> OdGiGDIViewportClipping::createObject()
{
  return OdRxObjectImpl<OdGiGDIViewportClipping>::createObject();
}

#ifndef GDI_USE_ENHANCED_CLIPPER
OdGiConveyorGeometry* OdGiGDIViewportClipping::optionalGeometry()
{
  if (m_pClipList && (m_pDestGeom != &OdGiEmptyGeometry::kVoid))
    return &m_pClipList->m_pEntry->geometry();
  return NULL;
}
#endif // GDI_USE_ENHANCED_CLIPPER

#ifndef GDI_USE_ENHANCED_CLIPPER
bool OdGiGDIViewportClipping::acceptable(const OdGsDCPoint* points, const int *pointCounts, int numPolygons)
{
  if (numPolygons != 1)
    return false;
  OdGePoint2dArray pts;
  pts.resize((OdUInt32)*pointCounts);
  for (int i = 0; i < *pointCounts; i++)
    pts[i].set((double)points[i].x, (double)points[i].y);
  return ::odgiIsValidClipBoundary(pts);
}
#else // GDI_USE_ENHANCED_CLIPPER
bool OdGiGDIViewportClipping::acceptable(const OdGsDCPoint* /*points*/, const int * /*pointCounts*/, int /*numPolygons*/)
{
  return true;
}
#endif // GDI_USE_ENHANCED_CLIPPER

bool OdGiGDIViewportClipping::accept(const OdGsDCPoint* points, const int *pointCounts, int numPolygons) const
{
  return OdGiGDIViewportClipping::acceptable(points, pointCounts, numPolygons);
}

bool OdGiGDIViewportClipping::push(const OdGsDCPoint* points, const int *pointCounts, int numPolygons)
{
  if (!accept(points, pointCounts, numPolygons))
    return false;
#ifndef GDI_USE_ENHANCED_CLIPPER
  OdGiGDIViewportClippingListEntry *pNewEntry = new OdGiGDIViewportClippingListEntry;
  if (!m_pClipListTail)
  {
    m_pClipList = m_pClipListTail = pNewEntry;
    updateLink();
  }
  else
  {
    OdGiGDIViewportClippingListEntry *pPrevEntry = m_pClipListTail;
    m_pClipListTail = m_pClipListTail->m_pNext = pNewEntry;
    pPrevEntry->m_pEntry->setDestGeometry(pNewEntry->m_pEntry->geometry());
  }
  m_ctxStorage.init(pNewEntry);
  pNewEntry->m_pEntry->set(points, pointCounts, numPolygons);
  pNewEntry->m_pEntry->setDestGeometry(destGeometry());
#else // GDI_USE_ENHANCED_CLIPPER
  OdGePoint2dArray pts;
  int numPoints = 0;
  for (int nPoly = 0; nPoly < numPolygons; nPoly++)
    numPoints += pointCounts[nPoly];
  pts.resize((OdUInt32)numPoints);
  for (int i = 0; i < numPoints; i++)
    pts[i].set((double)points[i].x, (double)points[i].y);
  m_pClipper->pushClipStage((OdUInt32)numPolygons, pointCounts, (OdUInt32)numPoints, pts.getPtr(),
                            OdGiOrthoClipperEx::kCSNoFlags, OdGiOrthoClipperEx::kCCClassifiedByInclusion);
#endif // GDI_USE_ENHANCED_CLIPPER
  return true;
}

#ifndef GDI_USE_ENHANCED_CLIPPER
bool OdGiGDIViewportClipping::acceptable(const OdGsDCPoint& min, const OdGsDCPoint& max)
{
  return (min.x != max.x) && (min.y != max.y);
}
#else // GDI_USE_ENHANCED_CLIPPER
bool OdGiGDIViewportClipping::acceptable(const OdGsDCPoint& /*min*/, const OdGsDCPoint& /*max*/)
{
  return true;
}
#endif // GDI_USE_ENHANCED_CLIPPER

bool OdGiGDIViewportClipping::accept(const OdGsDCPoint& min, const OdGsDCPoint& max) const
{
  return OdGiGDIViewportClipping::acceptable(min, max);
}

bool OdGiGDIViewportClipping::push(const OdGsDCPoint& min, const OdGsDCPoint& max)
{
  if (!accept(min, max))
    return false;
#ifndef GDI_USE_ENHANCED_CLIPPER
  OdGiGDIViewportClippingListEntry *pNewEntry = new OdGiGDIViewportClippingListEntry;
  if (!m_pClipListTail)
  {
    m_pClipList = m_pClipListTail = pNewEntry;
    updateLink();
  }
  else
  {
    OdGiGDIViewportClippingListEntry *pPrevEntry = m_pClipListTail;
    m_pClipListTail = m_pClipListTail->m_pNext = pNewEntry;
    pPrevEntry->m_pEntry->setDestGeometry(pNewEntry->m_pEntry->geometry());
  }
  m_ctxStorage.init(pNewEntry);
  pNewEntry->m_pEntry->set(min, max);
  pNewEntry->m_pEntry->setDestGeometry(destGeometry());
#else // GDI_USE_ENHANCED_CLIPPER
  const OdGePoint2d pts[4] = { OdGePoint2d((double)min.x, (double)min.y),
                               OdGePoint2d((double)max.x, (double)min.y),
                               OdGePoint2d((double)max.x, (double)max.y),
                               OdGePoint2d((double)min.x, (double)max.y) };
  const int ptsCounts = 4;
  m_pClipper->pushClipStage(1, &ptsCounts, 4, pts);
#endif // GDI_USE_ENHANCED_CLIPPER
  return true;
}

bool OdGiGDIViewportClipping::pop()
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  if (!m_pClipListTail)
    return false;
  if (m_pClipList == m_pClipListTail)
  {
    delete m_pClipList;
    m_pClipList = m_pClipListTail = NULL;
    updateLink();
  }
  else
  {
    OdGiGDIViewportClippingListEntry *pPrevEntry = m_pClipList;
    while (pPrevEntry->m_pNext != m_pClipListTail)
      pPrevEntry = pPrevEntry->m_pNext;
    delete m_pClipListTail;
    m_pClipListTail = pPrevEntry;
    m_pClipListTail->m_pNext = NULL;
    m_pClipListTail->m_pEntry->setDestGeometry(destGeometry());
  }
  return true;
#else // GDI_USE_ENHANCED_CLIPPER
  return m_pClipper->popClipStage();
#endif // GDI_USE_ENHANCED_CLIPPER
}

void OdGiGDIViewportClipping::setDrawContext(OdGiConveyorContext* pDrawContext)
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  m_ctxStorage.setDrawContext(pDrawContext, m_pClipList);
#else // GDI_USE_ENHANCED_CLIPPER
  m_pClipper->setDrawContext(pDrawContext);
#endif // GDI_USE_ENHANCED_CLIPPER
}

void OdGiGDIViewportClipping::setDeviation(const OdGeDoubleArray& deviations)
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  m_ctxStorage.setDeviation(deviations, m_pClipList);
#else // GDI_USE_ENHANCED_CLIPPER
  m_pClipper->setDeviation(deviations);
#endif // GDI_USE_ENHANCED_CLIPPER
}

void OdGiGDIViewportClipping::setDeviation(const OdGiDeviation* pDeviation)
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  m_ctxStorage.setDeviation(pDeviation, m_pClipList);
#else // GDI_USE_ENHANCED_CLIPPER
  m_pClipper->setDeviation(pDeviation);
#endif // GDI_USE_ENHANCED_CLIPPER
}

OdGiConveyorInput& OdGiGDIViewportClipping::input()
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  return *this;
#else // GDI_USE_ENHANCED_CLIPPER
  return m_pClipper->input();
#endif // GDI_USE_ENHANCED_CLIPPER
}

OdGiConveyorOutput& OdGiGDIViewportClipping::output()
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  return *this;
#else // GDI_USE_ENHANCED_CLIPPER
  return m_pClipper->output();
#endif // GDI_USE_ENHANCED_CLIPPER
}

//
