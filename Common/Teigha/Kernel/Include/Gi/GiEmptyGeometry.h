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




#ifndef __ODGIEMPTYGEOMETRY_H__
#define __ODGIEMPTYGEOMETRY_H__


#include "Gi/GiExport.h"
#include "Gi/GiConveyorGeometry.h"

#include "TD_PackPush.h"

/** \details
  This class is an implementation of OdGiConveyorGeometry that provides no-ops for the all functions therein.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiEmptyGeometry : public OdGiConveyorGeometry
{
public:
  ODGI_EXPORT_STATIC static OdGiConveyorGeometry& kVoid;

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* xfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void circleProc(
    const OdGePoint3d& center, double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);
  
  void circleProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);
  
  void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0, OdGsMarker baseSubEntMarker = -1);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void polygonProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, const OdGeVector3d* pExtrusion = 0);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void meshProc(
    OdInt32 rows,
    OdInt32 columns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void shellProc(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, const OdGeVector3d& upVector,
    const OdChar* msg, OdInt32 numChars, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void shapeProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, const OdGeVector3d& upVector,
    int shapeNumber, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void xlineProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void rayProc( const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);
  
  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void nurbsProc( const OdGeNurbCurve3d& nurbsCurve);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void rasterImageProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary,
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
	void metafileProc(
    const OdGePoint3d& origin,
		const OdGeVector3d& u,
		const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,           
    bool allowClipping = false);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void polypointProc(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  void edgeProc(const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0);
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIEMPTYGEOMETRY_H__
