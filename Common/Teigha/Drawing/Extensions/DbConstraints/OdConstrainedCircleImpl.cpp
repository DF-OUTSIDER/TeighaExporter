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
#include "OdConstrainedCircleImpl.h"

#include "DbFiler.h"
#include "DbCircle.h"

OdConstrainedCircleImpl::OdConstrainedCircleImpl():OdConstrainedCurveImpl(),
                                           m_geArc(),
                                           m_originGeArc()
{
  m_implObjType = CircleImpl;
}

OdConstrainedCircleImpl::~OdConstrainedCircleImpl()
{
}

OdResult OdConstrainedCircleImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbObjectPtr objPtr = geomObject(geomDependencyId);

  OdDbCirclePtr circlePtr = OdDbCircle::cast(objPtr);
  if (circlePtr.isNull() && !objPtr.isNull()) // it may become OdDbArc after TRIM command
    throw OdError(eBadObjType);
  
  ODA_ASSERT(!circlePtr.isNull());

  OdGePoint3d center(circlePtr->center());
  center.transformBy(blockToWrd);
  center.transformBy(matrFromWrd);

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.set(center, m_originGeArc.normal(), m_originGeArc.refVec(), circlePtr->radius(), 0.0, Oda2PI);

  m_idDependency = geomDependencyId;

  return eOk;
}

OdResult OdConstrainedCircleImpl::setCenterPoint(const OdGePoint3d& center)
{
  if ( center == centerPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.setCenter(center);

  return eOk;
}

OdResult OdConstrainedCircleImpl::setRadius(const double val)
{
  if ( fabs(val - radius()) <= OdGeContext::gTol.equalPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.setRadius(val);

  return eOk;
}

bool OdConstrainedCircleImpl::operator==(const OdConstrainedGeometryImpl & geom) const
{
  if ( getType(geom) != m_implObjType )
    return false;

  OdConstrainedCircleImpl* sec = (OdConstrainedCircleImpl*)&geom;

  bool res = sec->centerPoint() == centerPoint();
  res &= fabs(sec->radius() - radius()) <= OdGeContext::gTol.equalPoint();
  res &= sec->m_geArc.normal().isCodirectionalTo(m_geArc.normal());
  res &= sec->m_geArc.refVec().isCodirectionalTo(m_geArc.refVec());

  return res;
}

bool OdConstrainedCircleImpl::operator!=(const OdConstrainedGeometryImpl & geom) const
{
  return !operator ==(geom);
}

OdResult OdConstrainedCircleImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbCircle::desc()) )
  {
    OdGePoint3d centerWrd(centerPoint());
    centerWrd.transformBy(matrToWrd);

    OdDbCirclePtr pCircle = OdDbCircle::cast(pObj);
    pCircle->setRadius(radius());
    pCircle->setCenter(centerWrd);
  }
  return eOk;
}

OdResult OdConstrainedCircleImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& val) const
{
  if ( pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kCenterImplicit )
  {
    OdGePoint3d center = centerPoint();
    val.set(center.x, center.y, center.z);
    return eOk;
  }

  return eBadObjType;
}

OdResult OdConstrainedCircleImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  if ( pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kCenterImplicit )
   {
     setCenterPoint(val);
     return eOk;
   }

   return eBadObjType;
}

OdResult OdConstrainedCircleImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kCenterImplicit )
  {
    res = getOriginalOdGeCircArc3d().center();
    return eOk;
  }

  return eBadObjType;
}

OdResult OdConstrainedCircleImpl::init(const OdDbObjectId depId)
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

double OdConstrainedCircleImpl::radius() const
{
  return m_geArc.radius();
}

OdGePoint3d OdConstrainedCircleImpl::centerPoint() const
{
  return m_geArc.center();
}

bool OdConstrainedCircleImpl::isBounded() const
{
  return false;
}

void OdConstrainedCircleImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedCurveImpl::dwgOutFields(pFiler);
  pFiler->wrPoint3d(m_geArc.center());
  pFiler->wrVector3d(m_geArc.normal());
  pFiler->wrVector3d(m_geArc.refVec());
  pFiler->wrDouble(m_geArc.radius());
  pFiler->wrDouble(0.0/*m_geArc.startAng()*/);
  pFiler->wrDouble(Oda2PI/*m_geArc.endAng()*/);
  if (pFiler->dwgVersion() >= OdDb::vAC27)
    pFiler->wrDouble(0.0);
}

OdResult OdConstrainedCircleImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  OdGePoint3d ptC = pFiler->rdPoint3d();
  OdGeVector3d vNormal = pFiler->rdVector3d();
  OdGeVector3d vRefVect = pFiler->rdVector3d();
  double dR = pFiler->rdDouble();
  double dS = pFiler->rdDouble();
  double dE = pFiler->rdDouble();
  if (pFiler->dwgVersion() >= OdDb::vAC27)
  {
    double Unk_Double = pFiler->rdDouble();
    if (!OdZero(Unk_Double))
    {
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }
  }
  m_geArc.set(ptC, vNormal, vRefVect, dR, dS, dE);
  return eOk;
}

void OdConstrainedCircleImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedCurveImpl::dxfOutFields(pFiler);
  pFiler->wrPoint3d(10, m_geArc.center());
  pFiler->wrVector3d(10, m_geArc.normal());
  pFiler->wrVector3d(10, m_geArc.refVec());
  pFiler->wrDouble(40, m_geArc.radius());
  pFiler->wrDouble(40, 0.0/*m_geArc.startAng()*/);
  pFiler->wrDouble(40, Oda2PI/*m_geArc.endAng()*/);
  if (pFiler->dwgVersion() >= OdDb::vAC27)
    pFiler->wrDouble(40, 0.0);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedCircleImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  OdGePoint3d ptC;
  OdGeVector3d vNormal;
  OdGeVector3d vRefVect;

  NEXT_CODE(10)
  pFiler->rdPoint3d(ptC);
  NEXT_CODE(10)
  pFiler->rdVector3d(vNormal);
  NEXT_CODE(10)
  pFiler->rdVector3d(vRefVect);
  NEXT_CODE(40)
  double dR = pFiler->rdDouble();
  NEXT_CODE(40)
  double dS = pFiler->rdDouble();
  NEXT_CODE(40)
  double dE = pFiler->rdDouble();
  if (pFiler->dwgVersion() >= OdDb::vAC27)
  {
    NEXT_CODE(40)
    double Unk_Double = pFiler->rdDouble();
    if (!OdZero(Unk_Double))
    {
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }
  }
  m_geArc.set(ptC, vNormal, vRefVect, dR, dS, dE);
  return eOk;
}

#undef NEXT_CODE

const OdGeCircArc3d OdConstrainedCircleImpl::getOdGeCircArc3d() const
{
  return m_geArc;
}

const OdGeCircArc3d OdConstrainedCircleImpl::getOriginalOdGeCircArc3d() const
{
  if ( m_origGeomSet )
    return m_originGeArc;
  else
    return m_geArc;
}

OdResult OdConstrainedCircleImpl::set(const OdGePoint3d centerPt, const double rad)
{
  if ( centerPt == centerPoint() && fabs(radius() - rad) <= OdGeContext::gTol.equalPoint() )
    return eOk;
  
  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.set(centerPt, m_originGeArc.normal(), m_originGeArc.refVec(), rad, 0.0, Oda2PI);

  return eOk;
}
