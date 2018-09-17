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

#include "OdConstrainedArcImpl.h"

#include "DbArc.h"
#include "DbPolyline.h"
#include "DbFiler.h"

OdConstrainedArcImpl::OdConstrainedArcImpl():OdConstrainedCircleImpl(),
                                     m_start(), m_end(),
                                     m_startOrig(), m_endOrig()
{
  m_implObjType = CircleArcImpl;
}

OdResult OdConstrainedArcImpl::init(const OdDbObjectId depId)
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

OdGePoint3d OdConstrainedArcImpl::startPoint() const
{
  return m_start;
}

OdGePoint3d OdConstrainedArcImpl::endPoint() const
{
  return m_end;
}

OdGePoint3d OdConstrainedArcImpl::midPoint() const
{
  OdGeVector3d seVec = m_end - m_start;
  seVec.normalize();
  OdGeVector3d seOrtho(seVec.y, -seVec.x, 0.0);
  seOrtho *= radius();

  return centerPoint() + seOrtho;
}

bool OdConstrainedArcImpl::isBounded() const
{
  return true;
}

void OdConstrainedArcImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedCircleImpl::dwgOutFields(pFiler);
  pFiler->wrPoint3d(m_start);
  pFiler->wrPoint3d(m_end);
}

OdResult OdConstrainedArcImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedCircleImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  m_start = pFiler->rdPoint3d();
  m_end = pFiler->rdPoint3d();

  return res;
}

void OdConstrainedArcImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedCircleImpl::dxfOutFields(pFiler);
  pFiler->wrPoint3d(10, m_start);
  pFiler->wrPoint3d(11, m_end);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedArcImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedCircleImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(10)
  pFiler->rdPoint3d(m_start);
  NEXT_CODE(11)
  pFiler->rdPoint3d(m_end);

  return res;
}

#undef NEXT_CODE

bool OdConstrainedArcImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  bool res = OdConstrainedCircleImpl::operator==(geom);

  if ( !res )
    return res;

  OdConstrainedArcImpl* sec = (OdConstrainedArcImpl*)&geom;

  return startPoint().isEqualTo(sec->startPoint()) && endPoint().isEqualTo(sec->endPoint());
}

bool OdConstrainedArcImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedArcImpl::setStartPoint(const OdGePoint3d& val)
{
  if ( !m_origGeomSet )
  {
    if ( val == startPoint() )
      return eOk;

    m_originGeArc = m_geArc;
    m_startOrig.set(m_start.x, m_start.y, m_start.z);
    m_endOrig.set(m_end.x, m_end.y, m_end.z);
    m_origGeomSet = true;
  }

  m_start.set(val.x, val.y, val.z);

  return eOk;
}

OdResult OdConstrainedArcImpl::setEndPoint(const OdGePoint3d& val)
{
  if ( !m_origGeomSet )
  {
    if ( val == endPoint() )
      return eOk;

    m_originGeArc = m_geArc;
    m_startOrig.set(m_start.x, m_start.y, m_start.z);
    m_endOrig.set(m_end.x, m_end.y, m_end.z);
    m_origGeomSet = true;
  }

  m_end.set(val.x, val.y, val.z);

  return eOk;
}

OdResult OdConstrainedArcImpl::setCenterPoint(const OdGePoint3d& center)
{
  if ( center == centerPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_startOrig.set(m_start.x, m_start.y, m_start.z);
    m_endOrig.set(m_end.x, m_end.y, m_end.z);
    m_origGeomSet = true;
  }

  m_geArc.setCenter(center);

  return eOk;
}

OdResult OdConstrainedArcImpl::setRadius(const double val)
{
  if ( fabs(val - radius()) <= OdGeContext::gTol.equalPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_startOrig.set(m_start.x, m_start.y, m_start.z);
    m_endOrig.set(m_end.x, m_end.y, m_end.z);

    m_origGeomSet = true;
  }

  m_geArc.setRadius(val);

  return eOk;
}

OdResult OdConstrainedArcImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbObjectPtr objPtr = geomObject(geomDependencyId);

  if ( objPtr->isKindOf(OdDbArc::desc()) )
  {
    OdDbArcPtr arcPtr = OdDbArc::cast(objPtr);
    
    OdGePoint3d center(arcPtr->center());
    OdGePoint3d start;
    OdGePoint3d end;

    arcPtr->getStartPoint(start);
    arcPtr->getEndPoint(end);

    center.transformBy(blockToWrd);
    start.transformBy(blockToWrd);
    end.transformBy(blockToWrd);

    center.transformBy(matrFromWrd);
    start.transformBy(matrFromWrd);
    end.transformBy(matrFromWrd);

    if ( !m_origGeomSet )
    {
      m_originGeArc = m_geArc;
      m_startOrig.set(m_start.x, m_start.y, m_start.z);
      m_endOrig.set(m_end.x, m_end.y, m_end.z);
      m_origGeomSet = true;
    }

    OdGeVector3d normal = arcPtr->normal();
    normal.transformBy(blockToWrd);
    normal.transformBy(matrFromWrd);
    OdGeVector3d refVec = OdGeVector3d::kXAxis;
    if (normal.dotProduct(OdGeVector3d::kZAxis) < 0.0)
      refVec = -refVec;
    m_geArc.set(center, normal, refVec, arcPtr->radius(), 0.0, Oda2PI);
    setStartPoint(start);
    setEndPoint(end);
  }
  else if ( objPtr->isKindOf(OdDbPolyline::desc()) )
  {
    OdDbPolylinePtr pLinePtr = OdDbPolyline::cast(objPtr);

    double bulge;
    OdGePoint3d start;
    OdGePoint3d end;

    OdResult res = getPolylineSegmentProp(geomDependencyId, pLinePtr, start, end, bulge);
    if ( res != eOk )
      return res;

    start.transformBy(blockToWrd);
    start.transformBy(matrFromWrd);

    end.transformBy(blockToWrd);
    end.transformBy(matrFromWrd);

    if (OdZero(bulge))
      return eDegenerateGeometry;
    double ang = atan(fabs(bulge))*2.0;
    OdGeVector3d vec = end - start;
    double halfLen = vec.length()/2.0;
    double rad = halfLen/sin(ang);
    double lenToCenter = rad*cos(ang)*(bulge > 0.0 ? 1.0 : -1.0);
    OdGeVector3d ortho = OdGeVector3d::kZAxis.crossProduct(vec);
    OdGe::ErrorCondition ec = OdGe::kOk;
    ortho.normalize(OdGeContext::gZeroTol, ec);
    ortho *= lenToCenter;

    OdGePoint3d center((start.x + end.x)/2.0 + ortho.x, (start.y + end.y)/2.0 + ortho.y, (start.z + end.z)/2.0 + ortho.z);

    if ( !m_origGeomSet )
    {
      m_originGeArc = m_geArc;
      m_startOrig.set(m_start.x, m_start.y, m_start.z);
      m_endOrig.set(m_end.x, m_end.y, m_end.z);
      m_origGeomSet = true;
    }
    const OdGeVector3d arcNormal = bulge < 0 ? -OdGeVector3d::kZAxis : OdGeVector3d::kZAxis;
    const OdGeVector3d arcRefVector = bulge < 0 ? -OdGeVector3d::kXAxis : OdGeVector3d::kXAxis;
    m_geArc.set(center, arcNormal, arcRefVector, rad, 0.0, Oda2PI);
 
    setStartPoint(start);
    setEndPoint(end);
  }
  else
    ODA_FAIL();

  m_idDependency = geomDependencyId;
  return eOk;
}

OdResult OdConstrainedArcImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbArc::desc()) )
  {
    OdGePoint3d centerWrd(centerPoint());
    centerWrd.transformBy(matrToWrd);

    OdDbArcPtr pArc = OdDbArc::cast(pObj);
    pArc->setRadius(radius());
    pArc->setCenter(centerWrd);

    OdGePlane ocsPlane;
    OdDb::Planarity flag;
    pArc->getPlane(ocsPlane, flag);
    OdGeVector3d normal = OdGeVector3d::kZAxis;
    OdGeVector3d refVec = OdGeVector3d::kXAxis;
    normal.transformBy(matrToWrd);
    refVec.transformBy(matrToWrd);
    if (normal.dotProduct(ocsPlane.normal()) < 0.0)
      refVec = -refVec;
    double ang = refVec.angleOnPlane(ocsPlane);

    double startAng = m_geArc.paramOf(startPoint());
    double endAng = m_geArc.paramOf(endPoint());
    pArc->setStartAngle(startAng + ang);
    pArc->setEndAngle(endAng + ang);

    m_start = m_geArc.evalPoint(startAng);
    m_end = m_geArc.evalPoint(endAng);
  }
  else if ( pObj->isKindOf(OdDbPolyline::desc()) )
  {
    double angStart = m_geArc.paramOf(startPoint());
    double angEnd = m_geArc.paramOf(endPoint());
    while ( angEnd < angStart )
      angEnd += Oda2PI;

    double ang = angEnd - angStart;
    while ( ang > Oda2PI )
      ang -= Oda2PI;

    double bulge = 0.0, newBulge = tan(ang/4);

    int startIdx, endIdx;
    OdDbPolylinePtr pLinePtr = OdDbPolyline::cast(pObj);
    if (getPolylineSegmentPtIdx(geomDependencyId(), pLinePtr, startIdx, endIdx) != eOk)
    {
      ODA_FAIL();
      return eBadObjType;
    }

    OdGeVector3d plinedir = pLinePtr->normal();
    OdGeMatrix3d wrdToPLine = OdGeMatrix3d::worldToPlane(plinedir);

    OdGePoint3d p1 = startPoint();
    OdGePoint3d p2 = endPoint();

    p1.transformBy(matrToWrd);
    p1.transformBy(wrdToPLine);

    p2.transformBy(matrToWrd);
    p2.transformBy(wrdToPLine);

    bulge = pLinePtr->getBulgeAt(startIdx);
    pLinePtr->setPointAt(startIdx, p1.convert2d());
    pLinePtr->setBulgeAt(startIdx, newBulge * (bulge > 0 ? 1 : -1));
    pLinePtr->setPointAt(endIdx, p2.convert2d());
  }
  else
    ODA_FAIL();

  return eOk;
}

OdResult OdConstrainedArcImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( OdConstrainedCircleImpl::point(pImplicitPoint, res) == eOk )
    return eOk;

  OdGePoint3d p;
  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kMidImplicit:
      p = midPoint();
    break;
    case OdConstrainedImplicitPoint::kStartImplicit:
      p = startPoint();
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      p = endPoint();
    break;
    default:
      return eBadObjType;
  }

  res.set(p.x, p.y, p.z);

  return eOk;
}

OdResult OdConstrainedArcImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  if ( OdConstrainedCircleImpl::setPoint(pImplicitPoint, val) == eOk )
    return eOk;

  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kMidImplicit:
      return eNotImplementedYet;
    case OdConstrainedImplicitPoint::kStartImplicit:
      setStartPoint(val);
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      setEndPoint(val);
    break;
    default:
      return eBadObjType;
  }

  return eOk;
}

OdResult OdConstrainedArcImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( OdConstrainedCircleImpl::getOriginalPoint(pImplicitPoint, res) == eOk )
    return eOk;

  OdGeCircArc3d arc = getOriginalOdGeCircArc3d();
  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kMidImplicit:
    {
      double startAng = arc.paramOf(m_origGeomSet ? m_startOrig : m_start);
      double endAng = arc.paramOf(m_origGeomSet ? m_endOrig: m_end);

      res = arc.evalPoint(startAng + (endAng - startAng)/2.0);
    }
    break;
    case OdConstrainedImplicitPoint::kStartImplicit:
      res = m_origGeomSet ? m_startOrig : m_start;
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      res = m_origGeomSet ? m_endOrig: m_end;
    break;
    default:
      return eBadObjType;
  }

  return eOk;
}

const OdGeCircArc3d OdConstrainedArcImpl::getOdGeCircArc3d() const
{
  OdGeCircArc3d res = m_geArc;
  res.setAngles(res.paramOf(m_start), res.paramOf(m_end));
  return res;
}

const OdGeCircArc3d OdConstrainedArcImpl::getOriginalOdGeCircArc3d() const
{
  OdGeCircArc3d res;

  if ( m_origGeomSet )
  {
    res = m_originGeArc;
    res.setAngles(res.paramOf(m_startOrig), res.paramOf(m_endOrig));
  }
  else
  {
    res = m_geArc;
    res.setAngles(res.paramOf(m_start), res.paramOf(m_end));
  }

  return res;
}

OdResult OdConstrainedArcImpl::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
  OdResult res = OdConstrainedCircleImpl::getFullSubentPaths(subentPaths);
  if ( res != eOk )
    return res;

  OdDbFullSubentPath *path = subentPaths.asArrayPtr();
  const OdDbObjectIdArray aObjId = path->objectIds();
  OdDbObjectId objId = aObjId[aObjId.length() - 1];
  if ( !objId.isValid() ) {
    ODA_FAIL_ONCE();
    return eInvalidObjectId;
  }
  OdDbObjectPtr pObjPtr = objId.openObject();
  if ( !pObjPtr->isKindOf(OdDbPolyline::desc()) )
    return eOk;
  OdDbPolylinePtr pLinePtr = OdDbPolyline::cast(pObjPtr);

  int startIdx = 0, endIdx = 0;
  if ( getPolylineSegmentPtIdx(geomDependencyId(), pLinePtr, startIdx, endIdx) == eOk )
    path->subentId().setIndex((endIdx == 0) ? pLinePtr->numVerts() : endIdx);

  return eOk;
}

OdResult OdConstrainedArcImpl::set(const OdGePoint3d centerPt, const double rad, const OdGePoint3d startPt, const OdGePoint3d endPt)
{
  setStartPoint(startPt);
  setEndPoint(endPt);

  OdConstrainedCircleImpl::set(centerPt, rad);

  return eOk;
}

void OdConstrainedArcImpl::audit(OdConstrainedGeometry* pConsGeom, OdAuditInfo* pAuditInfo)
{
  OdSmartPtr<OdConstrainedArc> pThisConsArc = OdConstrainedArc::cast(pConsGeom);
  OdSmartPtr<OdConstrainedArc> pRefConsArc = OdConstrainedArc::createObject();
  if (pThisConsArc.isNull())
    return;

  const OdDbObjectId geomDepId = pThisConsArc->geomDependencyId();
  if (geomDepId.isNull() || pRefConsArc->init(geomDepId) != eOk)
    return;

  //previous version of Teigha had different from object arx representation for bulged polyline edges
  //acad sets negated normal to define clockwise arc segment, Teigha swapped end points to make it counterclockwise.
  //To recognize Teigha's old representation following checks are used
  {
    if (*pThisConsArc == *pRefConsArc || !geomObject(geomDepId)->isKindOf(OdDbPolyline::desc()))
      return;

    const OdGeVector3d thisNormal = pThisConsArc->getOdGeCircArc3d().normal();
    const OdGeVector3d refNormal = pRefConsArc->getOdGeCircArc3d().normal();

    if (thisNormal.isCodirectionalTo(refNormal))
      return;

    if (   pThisConsArc->startPoint() != pRefConsArc->endPoint()
        || pThisConsArc->endPoint() != pRefConsArc->startPoint())
      return;
  }

  pAuditInfo->errorsFound(1);
  if (!pAuditInfo->fixErrors())
    return;

  pThisConsArc->init(geomDepId);

  OdArray<OdConstrainedImplicitPoint*> aImplicitPts;
  getConstrainedImplicitPoints(aImplicitPts);
  for (unsigned pointIdx = 0; pointIdx < aImplicitPts.length(); ++pointIdx)
  {
    OdConstrainedImplicitPoint* pImplicitPoint = aImplicitPts[pointIdx];
    if (pImplicitPoint == NULL)
      continue;

    OdConstrainedImplicitPoint::ImplicitPointType pointTypeToSet;
    if (pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kStartImplicit)
      pointTypeToSet = OdConstrainedImplicitPoint::kEndImplicit;
    else if (pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kEndImplicit)
      pointTypeToSet = OdConstrainedImplicitPoint::kStartImplicit;
    else
      continue;

    pImplicitPoint->init(pImplicitPoint->constrainedCurveId(), pointTypeToSet, pImplicitPoint->pointIndex());
  }

  pAuditInfo->errorsFixed(1);
}
