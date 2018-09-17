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

#include "OdConstrainedEllipseImpl.h"
#include "DbEllipse.h"
#include "DbFiler.h"

OdConstrainedEllipseImpl::OdConstrainedEllipseImpl():OdConstrainedCurveImpl(),
                                             m_geArc(),
                                             m_originGeArc()
{
  m_implObjType = EllipseImpl;
}

OdResult OdConstrainedEllipseImpl::init(const OdDbObjectId depId)
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

bool OdConstrainedEllipseImpl::isBounded() const
{
  return false;
}

OdGeVector3d OdConstrainedEllipseImpl::direction() const
{
  return m_geArc.majorAxis();
}

double OdConstrainedEllipseImpl::majorRadius() const
{
  return m_geArc.majorRadius();
}

double OdConstrainedEllipseImpl::minorRadius() const
{
  return m_geArc.minorRadius();
}

OdGePoint3d OdConstrainedEllipseImpl::centerPoint() const
{
  return m_geArc.center();
}

void OdConstrainedEllipseImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedCurveImpl::dwgOutFields(pFiler);
  pFiler->wrPoint3d(centerPoint());

  OdGeVector3d majorAxis = direction();
  majorAxis *= majorRadius();
  pFiler->wrVector3d(majorAxis);

  pFiler->wrDouble(minorRadius()/majorRadius());
}

OdResult OdConstrainedEllipseImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  OdGePoint3d center = pFiler->rdPoint3d();
  OdGeVector3d majorAxis = pFiler->rdVector3d();
  double ratio = pFiler->rdDouble();

  OdGeVector3d normal(OdGeVector3d::kZAxis);
  double majorRadius = majorAxis.length();
  majorAxis /= majorRadius;
  m_geArc.set(center,
              majorAxis,
              normal.crossProduct(majorAxis),
              majorRadius,
              ratio*majorRadius);

  return res;
}

void OdConstrainedEllipseImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedCurveImpl::dxfOutFields(pFiler);
  
  pFiler->wrPoint3d(10, centerPoint());

  OdGeVector3d majorAxis = direction();
  majorAxis *= majorRadius();
  pFiler->wrVector3d(11, majorAxis);

  pFiler->wrDouble(40, minorRadius()/majorRadius());
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedEllipseImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  OdGePoint3d center;
  OdGeVector3d majorAxis;
  OdGeVector3d normal(OdGeVector3d::kZAxis);
  double ratio;

  NEXT_CODE(10)
  pFiler->rdPoint3d(center);

  NEXT_CODE(11)
  pFiler->rdVector3d(majorAxis);

  NEXT_CODE(40)
  ratio = pFiler->rdDouble();

  double majorRadius = majorAxis.length();
  majorAxis /= majorRadius;
  m_geArc.set(center,
              majorAxis,
              normal.crossProduct(majorAxis),
              majorRadius,
              ratio*majorRadius);

  return res;
}

#undef NEXT_CODE

bool OdConstrainedEllipseImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( getType(geom) != m_implObjType )
    return false;

  OdConstrainedEllipseImpl* sec = (OdConstrainedEllipseImpl*)&geom;

  return centerPoint().isEqualTo(sec->centerPoint()) &&
         direction().isEqualTo(sec->direction()) &&
         fabs(sec->majorRadius() - majorRadius()) <= OdGeContext::gTol.equalPoint() &&
         fabs(sec->minorRadius() - minorRadius()) <= OdGeContext::gTol.equalPoint();
}

bool OdConstrainedEllipseImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedEllipseImpl::setDirection(const OdGeVector3d& val)
{
  if ( val.normal() == direction().normal() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  OdGeVector3d majorAxis = val.normal();
  OdGeVector3d minorAxis = OdGeVector3d::kZAxis.crossProduct(majorAxis);
  majorAxis *= majorRadius();
  minorAxis *= minorRadius();
  m_geArc.setAxes(majorAxis, minorAxis);
  return eOk;
}

OdResult OdConstrainedEllipseImpl::setMajorRadius(const double val)
{
  if ( fabs(val - majorRadius()) <= OdGeContext::gTol.equalPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.setMajorRadius(val);
  return eOk;
}

OdResult OdConstrainedEllipseImpl::setMinorRadius(const double val)
{
  if ( fabs(val - minorRadius()) <= OdGeContext::gTol.equalPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.setMinorRadius(val);
  return eOk;
}

OdResult OdConstrainedEllipseImpl::setCenterPoint(const OdGePoint3d& val)
{
  if ( val == centerPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.setCenter(val);
  return eOk;
}

OdResult OdConstrainedEllipseImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbObjectPtr objPtr = geomObject(geomDependencyId);

  OdDbEllipsePtr ellipsePtr = OdDbEllipse::cast(objPtr);

  ODA_ASSERT(!ellipsePtr.isNull());
  OdGePoint3d center;
  OdGeVector3d unitNormal, majorAxis;
  double radiusRatio, startAng, endAng;

  ellipsePtr->get(center, unitNormal, majorAxis, radiusRatio, startAng, endAng);

  OdGePoint3d endMajor = center + majorAxis;

  double majorRadius = majorAxis.length();

  center.transformBy(blockToWrd);
  center.transformBy(matrFromWrd);

  endMajor.transformBy(blockToWrd);
  endMajor.transformBy(matrFromWrd);

  majorAxis = endMajor - center;

  OdGeVector3d normal(OdGeVector3d::kZAxis);
  
  majorAxis /= majorAxis.length();

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  m_geArc.set(center,
              majorAxis,
              normal.crossProduct(majorAxis),
              majorRadius,
              majorRadius*radiusRatio);

  m_idDependency = geomDependencyId;

  return eOk;
}

OdResult OdConstrainedEllipseImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbEllipse::desc()) )
  {

    OdGePoint3d centerWrd(centerPoint());
    OdGePoint3d majorEndWrd(centerWrd + direction()*majorRadius());
    OdGePoint3d minorEndWrd(centerWrd + direction()*majorRadius());

    centerWrd.transformBy(matrToWrd);
    majorEndWrd.transformBy(matrToWrd);
    minorEndWrd.transformBy(matrToWrd);

    OdDbEllipsePtr pEllipse = OdDbEllipse::cast(pObj);
    if ( minorRadius() > majorRadius() )
      pEllipse->set(centerWrd, pEllipse->normal(), minorEndWrd - centerWrd, majorRadius()/minorRadius());
    else
      pEllipse->set(centerWrd, pEllipse->normal(), majorEndWrd - centerWrd, minorRadius()/majorRadius());

    return eOk;
  }
   
  return eOk;
}

OdResult OdConstrainedEllipseImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kCenterImplicit )
  {
    OdGePoint3d p = centerPoint();
    res.set(p.x, p.y, p.z);
    return eOk;
  }

  return eBadObjType;
}

OdResult OdConstrainedEllipseImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  if ( pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kCenterImplicit )
  {
    setCenterPoint(val);
    return eOk;
  }

  return eBadObjType;
}

OdResult OdConstrainedEllipseImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( pImplicitPoint->pointType() == OdConstrainedImplicitPoint::kCenterImplicit )
  {
    res = getOriginalOdGeEllipArc3d().center();
    return eOk;
  }

  return eBadObjType;
}

const OdGeEllipArc3d OdConstrainedEllipseImpl::getOdGeEllipArc3d() const
{
  return m_geArc;
}

const OdGeEllipArc3d OdConstrainedEllipseImpl::getOriginalOdGeEllipArc3d() const
{
  if ( m_origGeomSet )
    return m_originGeArc;
  else
    return m_geArc;
}

OdResult OdConstrainedEllipseImpl::set(const OdGePoint3d centerPt, const OdGeVector3d dir, const double majorRad, const double minorRad)
{
  if ( centerPt == centerPoint() &&
        dir == direction() && 
        fabs(majorRad - majorRadius()) <= OdGeContext::gTol.equalPoint() &&
         fabs(minorRad - minorRadius()) <= OdGeContext::gTol.equalPoint() )
         return eOk;

  if ( !m_origGeomSet )
  {
    m_originGeArc = m_geArc;
    m_origGeomSet = true;
  }

  OdGeVector3d normal(OdGeVector3d::kZAxis);

  m_geArc.set(centerPt,
              dir,
              normal.crossProduct(dir),
              majorRad,
              minorRad);
  return eOk;
}
