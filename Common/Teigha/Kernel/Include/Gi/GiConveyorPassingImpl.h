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

#ifndef __ODGICONVEYORPASSINGIMPL__
#define __ODGICONVEYORPASSINGIMPL__

#include "Gi/GiEmptyGeometry.h"


template<class TThisClass, class TBase = OdGiConveyorGeometry>
class OdGiConveyorPassingImpl : public TBase {
  OdGiConveyorGeometry& destGeom() {
    return static_cast<TThisClass*>(this)->destGeometry();
  }
public:
  void plineProc(const OdGiPolyline& polyline, 
    const OdGeMatrix3d* pXfm = 0, 
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0)
  {
    destGeom().plineProc(polyline, pXfm, fromIndex, numSegs);
  }

  void polylineProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList, 
    const OdGeVector3d* pNormal = 0, 
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1)
  {
    destGeom().polylineProc(numPoints, vertexList, pNormal, pExtrusion, baseSubEntMarker);
  }

  void polygonProc(OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().polygonProc(numPoints, vertexList, pNormal, pExtrusion);
  }

  void xlineProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint) {
    destGeom().xlineProc(firstPoint, secondPoint);
  }

  void rayProc(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint) {
    destGeom().rayProc(basePoint, throughPoint);
  }

  void meshProc(OdInt32 numRows, 
    OdInt32 numColumns, 
    const OdGePoint3d* vertexList, 
    const OdGiEdgeData* pEdgeData = 0, 
    const OdGiFaceData* pFaceData = 0, 
    const OdGiVertexData* pVertexData = 0)
  {
    destGeom().meshProc(numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData);
  }

  void shellProc(OdInt32 numVertices, 
    const OdGePoint3d* vertexList, 
    OdInt32 faceListSize, 
    const OdInt32* faceList, 
    const OdGiEdgeData* pEdgeData = 0, 
    const OdGiFaceData* pFaceData = 0, 
    const OdGiVertexData* pVertexData = 0)
  {
    destGeom().shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  }

  void circleProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0) {
    destGeom().circleProc(center, radius, normal, pExtrusion);
  }

  void circleProc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().circleProc(firstPoint, secondPoint, thirdPoint, pExtrusion);
  }

  void circularArcProc(const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d& startVector, 
    double sweepAngle, 
    OdGiArcType arcType = kOdGiArcSimple, 
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
  }

  void circularArcProc(const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint, 
    OdGiArcType arcType = kOdGiArcSimple, 
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().circularArcProc(firstPoint, secondPoint, thirdPoint, arcType, pExtrusion);
  }

  void ellipArcProc(const OdGeEllipArc3d& ellipArc, 
    const OdGePoint3d* endPointOverrides = 0, 
    OdGiArcType arcType = kOdGiArcSimple, 
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().ellipArcProc(ellipArc, endPointOverrides, arcType, pExtrusion);
  }

  void nurbsProc(const OdGeNurbCurve3d& nurbsCurve) {
    destGeom().nurbsProc(nurbsCurve);
  }

  void textProc(const OdGePoint3d& position, 
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector, 
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle, 
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().textProc(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion);
  }

  void textProc2(const OdGePoint3d& position, 
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector, 
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle, 
    const OdGeVector3d* pExtrusion = 0, 
    const OdGeExtents3d* extentsBox = 0)
  {
    destGeom().textProc2(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion, extentsBox);
  }

  void shapeProc(const OdGePoint3d& position, 
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector, 
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle, 
    const OdGeVector3d* pExtrusion = 0)
  {
    destGeom().shapeProc(position, direction, upVector, shapeNumber, pTextStyle, pExtrusion);
  }

  void rasterImageProc(const OdGePoint3d& origin, 
    const OdGeVector3d& u, 
    const OdGeVector3d& v, 
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts, 
    bool transparency = false, 
    double brightness = 50.0, 
    double contrast = 50.0, 
    double fade = 0.0)
  {
    destGeom().rasterImageProc(origin, u, v, pImage, uvBoundary, numBoundPts, transparency, brightness, contrast, fade);
  }

  void metafileProc(const OdGePoint3d& origin, 
    const OdGeVector3d& u, 
    const OdGeVector3d& v, 
    const OdGiMetafile* pMetafile, 
    bool dcAligned = true, 
    bool allowClipping = false)
  {
    destGeom().metafileProc(origin, u, v, pMetafile, dcAligned, allowClipping);
  }

  void xlineProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction) {
    destGeom().xlineProc2(basePoint, direction);
  }

  void rayProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction) {
    destGeom().rayProc2(basePoint, direction);
  }

  void ttfPolyDrawProc(OdInt32 numVertices, 
    const OdGePoint3d* vertexList, 
    OdInt32 faceListSize, 
    const OdInt32* faceList, 
    const OdUInt8* pBezierTypes, 
    const OdGiFaceData* pFaceData = 0)
  {
    destGeom().ttfPolyDrawProc(numVertices, vertexList, faceListSize, faceList, pBezierTypes, pFaceData);
  }

  void polypointProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList, 
    const OdCmEntityColor* pColors, 
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0, 
    const OdGeVector3d* pExtrusions = 0, 
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0)
  {
    destGeom().polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize);
  }

  void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
  {
    destGeom().rowOfDotsProc(numPoints, startPoint, dirToNextPoint);
  }

  void edgeProc(const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0)
  {
    destGeom().edgeProc(edges, pXform);
  }

  virtual void conveyorBoundaryInfoProc(
    const OdGeBoundBlock3d & boundBlock, // {I} represent block for apply check
    OdUInt32 & outputFlags) // {O} will represent set of EConveyorBoundaryInfoFlags flags on out
  {
    destGeom().conveyorBoundaryInfoProc( boundBlock, outputFlags );
  }
};


#endif // __ODGICONVEYORPASSINGIMPL__
