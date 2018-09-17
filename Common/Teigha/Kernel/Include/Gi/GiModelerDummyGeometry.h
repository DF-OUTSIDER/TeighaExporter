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

#ifndef _ODGIMODELERDUMMYGEOM_INCLUDED_
#define _ODGIMODELERDUMMYGEOM_INCLUDED_

#include "Gi/GiDummyGeometry.h"

#include "DbBaseSubentId.h" 

#include "ModelerMarkers.h"

#include "TD_PackPush.h"

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T, class DRAW, class WR, class WRA> class OdGeomGrabber : public T
{
  OdGsMarker m_curSelMarker;
  WRA &wires;

  OdGeomGrabber();
protected:
  virtual OdGsMarker getCurSelMarker() const {
    return m_curSelMarker;
  }

public:

  OdGeomGrabber(WRA &pW, DRAW &pVd)
    : T(pVd, pVd.geometry(), pVd.subEntityTraits())
    , m_curSelMarker(0)
    , wires(pW)
  { }

  virtual void setSelectionMarker(OdGsMarker n) { m_curSelMarker = n; T::setSelectionMarker(n); }

  virtual WR *appendWire()
  {
    WR *pWire = wires.append();
    pWire->selMarker = OdIntToGsMarker(-getFaceIndex(m_curSelMarker));
    pWire->color = T::subEntityTraits().trueColor();
    return pWire;
  }

  virtual void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
  {
    appendWire()->circle(center, radius, normal);
    T::circle(center, radius, normal);
  }

  virtual void circle(const OdGePoint3d& p1, const OdGePoint3d& p2, const OdGePoint3d& p3)
  {
    appendWire()->circle(p1, p2, p3);
    T::circle(p1, p2, p3);
  }

  virtual void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    appendWire()->circularArc(center, radius, normal, startVector, sweepAngle);
    T::circularArc(center, radius, normal, startVector, sweepAngle, arcType);
  }

  virtual void circularArc(const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    appendWire()->circularArc(start, point, end);
    T::circularArc(start, point, end, arcType);
  }

  virtual void polyline(OdInt32 nbPoints,
    const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1)
  {
    appendWire()->polyline(nbPoints, pVertexList);
    T::polyline(nbPoints, pVertexList, pNormal, lBaseSubEntMarker);
  }

  virtual void nurbs(const OdGeNurbCurve3d& nurbs)
  {
    appendWire()->nurbs(nurbs);
    T::nurbs(nurbs);
  }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T, class DRAW, class WR, class WRA> class OdShellGrabber : public T
{
  OdGsMarker m_curSelMarker;
  WRA &wires;

  OdShellGrabber();
public:

  OdShellGrabber(WRA &pW, DRAW &pVd) : T(pVd, pVd.geometry(), pVd.subEntityTraits())
    , m_curSelMarker(0)
    , wires(pW)
  { }

  virtual OdGiRegenType regenType() const              { return kOdGiHideOrShadeCommand; }

  virtual void setSelectionMarker(OdGsMarker n) { m_curSelMarker = n; T::setSelectionMarker(n); }

  virtual void shell(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0)
  {
    WR *pWire = wires.append();
    pWire->selMarker = m_curSelMarker;
    pWire->color = T::subEntityTraits().trueColor();
    pWire->shell(
      numVertices,
      vertexList,
      faceListSize,
      faceList,
      pEdgeData,
      pFaceData,
      pVertexData);

    T::shell(
      numVertices,
      vertexList,
      faceListSize,
      faceList,
      pEdgeData,
      pFaceData,
      pVertexData);
  }
};

#include "TD_PackPop.h"

#include "ModelerMarkersUndef.h"

#endif // #ifndef _ODGIMODELERDUMMYGEOM_INCLUDED_
