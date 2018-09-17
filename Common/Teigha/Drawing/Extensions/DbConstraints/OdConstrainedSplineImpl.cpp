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

#include "OdConstrainedSplineImpl.h"

#include "DbSpline.h"
#include "DbFiler.h"

OdConstrainedSplineImpl::OdConstrainedSplineImpl():OdConstrainedCurveImpl(),
m_nurb(),
m_originalNurb(),
m_UnkBool1(false),
m_periodic(false),
m_degree(0),
m_precision(1.0e-10),
m_knots(),
m_weights(),
m_controlPoints(),
m_implicitPoints()
{
  m_implObjType = SplineImpl;
}

OdResult OdConstrainedSplineImpl::init(const OdDbObjectId depId, const OdGeNurbCurve3d& spline)
{
  OdResult res = OdConstrainedGeometryImpl::init(depId);
  if ( res != eOk )
    return res;

  OdDbAssoc2dConstraintGroupPtr pGr = m_groupObjectId.openObject();
  OdGeMatrix3d matrFromWrd = OdGeMatrix3d::worldToPlane(pGr->getWorkPlane());
  setDataFromGeomDependency(depId, matrFromWrd);

  m_origGeomSet = false;
  return eOk;
}

bool OdConstrainedSplineImpl::isBounded() const
{
  return true;
}

const OdGeNurbCurve3d OdConstrainedSplineImpl::nurbSpline() const
{
  return m_nurb;
}

const OdGeNurbCurve3d OdConstrainedSplineImpl::originalNurbSpline() const
{
  if ( m_origGeomSet )
    return m_originalNurb;
  else
    return m_nurb;
}

int OdConstrainedSplineImpl::numOfDefinePoints() const
{
  return m_nurb.numControlPoints() - (m_nurb.isClosed() ? 1 : 0);
}

OdGePoint3d OdConstrainedSplineImpl::definePointAt(int index) const
{
  return m_nurb.controlPointAt(index);
}

void OdConstrainedSplineImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedCurveImpl::dwgOutFields(pFiler);
  
  int len;

  pFiler->wrBool(m_UnkBool1);
  pFiler->wrBool(m_periodic);
  
  pFiler->wrInt32(m_degree);

  pFiler->wrDouble(m_precision);

  pFiler->wrInt32((len = m_knots.logicalLength()));
  pFiler->wrInt32(m_knots.physicalLength());
  pFiler->wrInt32(m_knots.growLength());
  
  const double *pKnots = m_knots.asArrayPtr();
  while ( len-- )
    pFiler->wrDouble(*pKnots++);

  pFiler->wrInt32((len = m_weights.logicalLength()));
  pFiler->wrInt32(m_weights.physicalLength());
  pFiler->wrInt32(m_weights.growLength());

  const double *pWeights = m_weights.asArrayPtr();
  while ( len-- )
    pFiler->wrDouble(*pWeights++);

  pFiler->wrInt32((len = m_controlPoints.logicalLength()));
  pFiler->wrInt32(m_controlPoints.physicalLength());
  pFiler->wrInt32(m_controlPoints.growLength());

  const OdGePoint3d *point = m_controlPoints.asArrayPtr();
  while ( len-- )
    pFiler->wrPoint3d(*point++);
  
  pFiler->wrInt32((len = m_implicitPoints.logicalLength()));

  const OdConstraintGroupNodeId* pImplicitPoints = m_implicitPoints.asArrayPtr();
  while (len--)
    pFiler->wrInt32(*pImplicitPoints++);
}

OdResult OdConstrainedSplineImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  int logicalLen;
  int physicalLen;
  int growLen;
  
  m_UnkBool1 = pFiler->rdBool();
  m_periodic = pFiler->rdBool();
  
  m_degree = pFiler->rdInt32();

  m_precision = pFiler->rdDouble();

  logicalLen = pFiler->rdInt32();
  physicalLen = pFiler->rdInt32();
  growLen = pFiler->rdInt32();
  
  m_knots.setTolerance(m_precision);
  m_knots.setLogicalLength(logicalLen);
  m_knots.setPhysicalLength(physicalLen);
  m_knots.setGrowLength(growLen);

  double *pKnots = m_knots.asArrayPtr();
  while ( logicalLen-- )
    (*pKnots++) = pFiler->rdDouble();

  logicalLen = pFiler->rdInt32();
  physicalLen = pFiler->rdInt32();
  growLen = pFiler->rdInt32();

  m_weights.setLogicalLength(logicalLen);
  m_weights.setPhysicalLength(physicalLen);
  m_weights.setGrowLength(growLen);
  double *pWeights = m_weights.asArrayPtr();
  while ( logicalLen-- )
    (*pWeights++) = pFiler->rdDouble();

  logicalLen = pFiler->rdInt32();
  physicalLen = pFiler->rdInt32();
  growLen = pFiler->rdInt32();

  m_controlPoints.setPhysicalLength(physicalLen);
  m_controlPoints.setGrowLength(growLen);

  while ( logicalLen-- )
    m_controlPoints.append(pFiler->rdPoint3d());
  
  m_nurb.set(m_degree, m_knots, m_controlPoints, m_weights, m_periodic);

  logicalLen = pFiler->rdInt32();
  m_implicitPoints.resize(logicalLen);

  OdConstraintGroupNodeId* pImplicitPoints = m_implicitPoints.asArrayPtr();
  while (logicalLen--)
    *pImplicitPoints++ = pFiler->rdInt32();
  
  return res;
}

void OdConstrainedSplineImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedCurveImpl::dxfOutFields(pFiler);
  
  int len;

  pFiler->wrInt16(70, m_UnkBool1 ? 1 : 0);
  pFiler->wrInt16(70, m_periodic ? 1 : 0);
  pFiler->wrInt32(90, m_degree);

  pFiler->wrDouble(40, m_precision);

  pFiler->wrInt32(90, (len = m_knots.logicalLength()));
  pFiler->wrInt32(90, m_knots.physicalLength());
  pFiler->wrInt32(90, m_knots.growLength());

  const double *pKnots = m_knots.asArrayPtr();
  while ( len-- )
    pFiler->wrDouble(40, *pKnots++);

  pFiler->wrInt32(90, (len = m_weights.logicalLength()));
  pFiler->wrInt32(90, m_weights.physicalLength());
  pFiler->wrInt32(90, m_weights.growLength());

  const double *pWeights = m_weights.asArrayPtr();
  while ( len-- )
    pFiler->wrDouble(40, *pWeights++);

  pFiler->wrInt32(90, (len = m_controlPoints.logicalLength()));
  pFiler->wrInt32(90, m_controlPoints.physicalLength());
  pFiler->wrInt32(90, m_controlPoints.growLength());

  const OdGePoint3d *point = m_controlPoints.asArrayPtr();
  while ( len-- )
    pFiler->wrPoint3d(10, *point++);
  
  pFiler->wrInt32(90, (len = m_implicitPoints.logicalLength()));

  const OdConstraintGroupNodeId* pImplicitPoints = m_implicitPoints.asArrayPtr();
  while (len--)
    pFiler->wrInt32(90, *pImplicitPoints++);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedSplineImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  int logicalLen;
  int physicalLen;
  int growLen;
  
  NEXT_CODE(70)
  m_UnkBool1 = pFiler->rdInt16() == 1;

  NEXT_CODE(70)
  m_periodic = pFiler->rdInt16() == 1;
  
  NEXT_CODE(90)
  m_degree = pFiler->rdInt32();

  NEXT_CODE(40)
  m_precision = pFiler->rdDouble();

  NEXT_CODE(90)
  logicalLen = pFiler->rdInt32();

  NEXT_CODE(90)
  physicalLen = pFiler->rdInt32();

  NEXT_CODE(90)
  growLen = pFiler->rdInt32();
  
  m_knots.setTolerance(m_precision);
  m_knots.setLogicalLength(logicalLen);
  m_knots.setPhysicalLength(physicalLen);
  m_knots.setGrowLength(growLen);

  double *pKnots = m_knots.asArrayPtr();
  while ( logicalLen-- )
  {
    NEXT_CODE(40)
    (*pKnots++) = pFiler->rdDouble();
  }

  NEXT_CODE(90)
  logicalLen = pFiler->rdInt32();

  NEXT_CODE(90)
  physicalLen = pFiler->rdInt32();

  NEXT_CODE(90)
  growLen = pFiler->rdInt32();

  m_weights.setLogicalLength(logicalLen);
  m_weights.setPhysicalLength(physicalLen);
  m_weights.setGrowLength(growLen);
  double *pWeights = m_weights.asArrayPtr();
  while ( logicalLen-- )
  {
    NEXT_CODE(40)
    (*pWeights++) = pFiler->rdDouble();
  }

  NEXT_CODE(90)
  logicalLen = pFiler->rdInt32();

  NEXT_CODE(90)
  physicalLen = pFiler->rdInt32();

  NEXT_CODE(90)
  growLen = pFiler->rdInt32();

  m_controlPoints.setLogicalLength(logicalLen);
  m_controlPoints.setPhysicalLength(physicalLen);
  m_controlPoints.setGrowLength(growLen);

  OdGePoint3d *point = m_controlPoints.asArrayPtr();
  while ( logicalLen-- )
  {
    NEXT_CODE(10)
    pFiler->rdPoint3d(*point);
    *point++;
  }
  
  m_nurb.set(m_degree, m_knots, m_controlPoints, m_weights, m_periodic);

  NEXT_CODE(90)
  logicalLen = pFiler->rdInt32();
  m_implicitPoints.resize(logicalLen);

  OdConstraintGroupNodeId* pImplicitPoints = m_implicitPoints.asArrayPtr();
  while (logicalLen--)
  {
    NEXT_CODE(90)
    *pImplicitPoints++ = pFiler->rdInt32();
  }
  
  return res;
}

#undef NEXT_CODE

bool OdConstrainedSplineImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( m_implObjType != getType(geom) )
    return false;

  OdConstrainedSplineImpl* spline = (OdConstrainedSplineImpl*)&geom;

  if ( m_UnkBool1 != spline->m_UnkBool1 )
    return false;

  if ( m_periodic != spline->m_periodic )
    return false;

  if ( m_degree != spline->m_degree )
    return false;

  if ( m_knots.length() != spline->m_knots.length() )
    return false;

  if ( m_weights.length() != spline->m_weights.length() )
    return false;

  if ( m_controlPoints.length() != spline->m_controlPoints.length() )
    return false;

  const double *thisKnots = m_knots.asArrayPtr();
  const double *splineKnots = spline->m_knots.asArrayPtr();

  int i, size = m_knots.length();
  for ( i = 0; i < size; i++ )
    if ( fabs(thisKnots[i] - splineKnots[i]) > m_knots.tolerance() )
      return false;

  const double *thisWeights = m_weights.asArrayPtr();
  const double *splineWeights = spline->m_weights.asArrayPtr();

  size = m_weights.length();
  for ( i = 0; i < size; i++ )
    if ( fabs(thisWeights[i] - splineWeights [i]) > 1.0e-06 )
      return false;

  size = m_controlPoints.length();
  for ( i = 0; i < size; i++ )
    if ( m_controlPoints[i] != spline->m_controlPoints[i] )
      return false;

  return true;
}

bool OdConstrainedSplineImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedSplineImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbSplinePtr splinePtr = geomObject(geomDependencyId);
  if ( splinePtr.isNull() )
    return eBadObjType;

  int degree;
  bool rational;
  bool closed;
  bool periodic;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray knots;
  OdGeDoubleArray weights;
  double controlPtTol;
  double knotTol;

  splinePtr->getNurbsData(degree, rational, closed, periodic,
                          controlPoints, knots, weights, controlPtTol, knotTol);

  if ( m_controlPoints.length() == 0 )
    m_controlPoints.setLogicalLength(controlPoints.length());

  if ( controlPoints.length() != m_controlPoints.length() )
    return eBadObjType;

  if ( !m_origGeomSet )
  {
    m_originalNurb = m_nurb;
    m_origGeomSet = true;
  }

  unsigned int i;
  for ( i = 0; i < controlPoints.length(); i++ )
  {
    m_controlPoints[i].set(controlPoints[i].x, controlPoints[i].y, controlPoints[i].z);

    m_controlPoints[i].transformBy(blockToWrd);
    m_controlPoints[i].transformBy(matrFromWrd);
  }

  m_weights.clear();
  for ( i = 0; i < weights.length(); i++ )
    m_weights.push_back(weights[i]);

  m_precision = knotTol;
  m_knots.set(knots.size(), knots.asArrayPtr(), m_precision);
  
  m_degree = degree;
  m_periodic = periodic;

  m_nurb.set(degree, knots, controlPoints, weights, periodic);

  return eOk;
}

OdResult OdConstrainedSplineImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbSpline::desc()) )
  {
    OdGePoint3dArray controlPoints;
    controlPoints.setPhysicalLength(m_controlPoints.length());
    for ( unsigned int i = 0 ; i < m_controlPoints.length(); i++ )
    {
      OdGePoint3d pt(m_controlPoints[i]);
      pt.transformBy(matrToWrd);
      controlPoints.push_back(pt);
    }
    OdDbSplinePtr pSpline = OdDbSpline::cast(pObj);
    
    pSpline->setNurbsData(m_degree,
                          m_nurb.isRational(),
                          m_nurb.isClosed(),
                          m_knots.length() == (int)controlPoints.length(),
                          controlPoints,
                          m_knots,
                          m_weights,
                          m_precision);

    return eOk;
  }
   
  return eBadObjType;
}

OdResult OdConstrainedSplineImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( pImplicitPoint->pointType() != OdConstrainedImplicitPoint::kDefineImplicit )
    return eBadObjType;

  OdGePoint3d p = m_nurb.controlPointAt(pImplicitPoint->pointIndex());
  res.set(p.x, p.y, p.z);

  return eOk;
}

OdResult OdConstrainedSplineImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  if ( pImplicitPoint->pointType() != OdConstrainedImplicitPoint::kDefineImplicit )
    return eBadObjType;

  int idx = pImplicitPoint->pointIndex();

  if ( !m_origGeomSet )
  {
    if ( m_controlPoints[idx] == val )
      return eOk;

    m_originalNurb = m_nurb;
    m_origGeomSet = true;
  }

  m_controlPoints[idx].set(val.x, val.y, val.z);
  m_nurb.setControlPointAt(idx, val);

  if ( m_periodic )
  {
    if ( idx == 0 )
    {
      idx = m_controlPoints.length() - 1;
      if (m_knots.length() != (int)m_controlPoints.length() + 1)
        m_controlPoints[idx].set(val.x, val.y, val.z);
      double period;
      if (!m_nurb.isPeriodic(period))
        m_nurb.setControlPointAt(idx, val);
    }
    else if ( idx == (int)m_controlPoints.length() - 1 )
    {
      idx = 0;
      if (m_knots.length() != (int)m_controlPoints.length() + 1)
        m_controlPoints[idx].set(val.x, val.y, val.z);
      double period;
      if (!m_nurb.isPeriodic(period))
        m_nurb.setControlPointAt(idx, val);
    }
  }

  return eOk;
}

OdResult OdConstrainedSplineImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( pImplicitPoint->pointType() != OdConstrainedImplicitPoint::kDefineImplicit )
    return eBadObjType;

  OdGePoint3d p = originalNurbSpline().controlPointAt(pImplicitPoint->pointIndex());
  res.set(p.x, p.y, p.z);

  return eOk;
}
