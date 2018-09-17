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

#include "OdConstrainedBoundedEllipseImpl.h"
#include "DbEllipse.h"
#include "DbFiler.h"

OdConstrainedBoundedEllipseImpl::OdConstrainedBoundedEllipseImpl():OdConstrainedEllipseImpl(),
                                                                      m_start(),
                                                                      m_end(),
                                                                      m_startOrig(),
                                                                      m_endOrig()
                                    
{
  m_implObjType = BoundedEllipseImpl;
}

OdResult OdConstrainedBoundedEllipseImpl::init(const OdDbObjectId depId)
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

OdGePoint3d OdConstrainedBoundedEllipseImpl::startPoint() const
{
  return m_start;
}

OdGePoint3d OdConstrainedBoundedEllipseImpl::endPoint() const
{
  return m_end;
}

void OdConstrainedBoundedEllipseImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedEllipseImpl::dwgOutFields(pFiler);
  pFiler->wrPoint3d(m_start);
  pFiler->wrPoint3d(m_end);
}
OdResult OdConstrainedBoundedEllipseImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedEllipseImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  m_start = pFiler->rdPoint3d();
  m_end = pFiler->rdPoint3d();

  return res;
}

void OdConstrainedBoundedEllipseImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedEllipseImpl::dxfOutFields(pFiler);
  pFiler->wrPoint3d(10, m_start);
  pFiler->wrPoint3d(11, m_end);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedBoundedEllipseImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedEllipseImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(10)
  pFiler->rdPoint3d(m_start);
  NEXT_CODE(11)
  OdGePoint3d end;
  pFiler->rdPoint3d(m_end);

  return res;
}

#undef NEXT_CODE

bool OdConstrainedBoundedEllipseImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( !OdConstrainedEllipseImpl::operator==(geom) )
    return false;

    OdConstrainedBoundedEllipseImpl* arc = (OdConstrainedBoundedEllipseImpl*)&geom;
    return m_start.isEqualTo(arc->m_start) && m_end.isEqualTo(arc->m_end);
}

bool OdConstrainedBoundedEllipseImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}


OdResult OdConstrainedBoundedEllipseImpl::setStartPoint(const OdGePoint3d& val)
{
  if ( !m_origGeomSet )
  {
    if ( val == startPoint() )
      return eOk;

    m_originGeArc = m_geArc;
    m_startOrig = m_start;
    m_endOrig = m_end;

    m_origGeomSet = true;
  } 

  m_start = val;
  return eOk;
}

OdResult OdConstrainedBoundedEllipseImpl::setEndPoint(const OdGePoint3d& val)
{
  if ( !m_origGeomSet )
  {
    if ( val == endPoint() )
      return eOk;

    m_originGeArc = m_geArc;
    m_startOrig = m_start;
    m_endOrig = m_end;

    m_origGeomSet = true;
  } 

  m_end = val;
  return eOk;
}


OdResult OdConstrainedBoundedEllipseImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdResult res = OdConstrainedEllipseImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);
  if ( res != eOk )
    return res;

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbEllipsePtr ellipsePtr = geomObject(geomDependencyId);

  ellipsePtr->getStartPoint(m_start);
  ellipsePtr->getEndPoint(m_end);
  
  m_start.transformBy(blockToWrd);
  m_end.transformBy(blockToWrd);

  m_start.transformBy(matrFromWrd);
  m_end.transformBy(matrFromWrd);

  return res;
}

OdResult OdConstrainedBoundedEllipseImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbEllipse::desc()) )
  {

    OdGePoint3d centerWrd(centerPoint());
    OdGePoint3d majorEndWrd(centerWrd + direction()*majorRadius());

    centerWrd.transformBy(matrToWrd);
    majorEndWrd.transformBy(matrToWrd);

    double startAng = m_geArc.paramOf(m_start);
    double endAng = m_geArc.paramOf(m_end);

    OdDbEllipsePtr pEllipse = OdDbEllipse::cast(pObj);
    pEllipse->set(centerWrd, pEllipse->normal(), majorEndWrd - centerWrd, minorRadius()/majorRadius());
    pEllipse->setStartParam(startAng);
    pEllipse->setEndParam(endAng);

    m_start = m_geArc.evalPoint(startAng);
    m_end = m_geArc.evalPoint(endAng);
  }
   
  return eOk;
}

OdResult OdConstrainedBoundedEllipseImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( OdConstrainedEllipseImpl::point(pImplicitPoint, res) == eOk )
    return eOk;

  OdGePoint3d p;
  switch ( pImplicitPoint->pointType() )
  {
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

OdResult OdConstrainedBoundedEllipseImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  if ( OdConstrainedEllipseImpl::setPoint(pImplicitPoint, val) == eOk )
    return eOk;

  switch ( pImplicitPoint->pointType() )
  {
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

OdResult OdConstrainedBoundedEllipseImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  if ( OdConstrainedEllipseImpl::getOriginalPoint(pImplicitPoint, res) == eOk )
    return eOk;

  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kStartImplicit:
      res = m_origGeomSet ? m_startOrig : m_start;
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      res = m_origGeomSet ? m_endOrig : m_end;
    break;
    default:
      return eBadObjType;
  }

  return eOk;
}

const OdGeEllipArc3d OdConstrainedBoundedEllipseImpl::getOdGeEllipArc3d() const
{
  OdGeEllipArc3d res = m_geArc;
  res.setAngles(res.paramOf(m_start), res.paramOf(m_end));
  return res;
}

const OdGeEllipArc3d OdConstrainedBoundedEllipseImpl::getOriginalOdGeEllipArc3d() const
{
  OdGeEllipArc3d res;

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

OdResult OdConstrainedBoundedEllipseImpl::set(const OdGePoint3d centerPt, const OdGeVector3d dir, const double majorRad, const double minorRad, const OdGePoint3d startPt, const OdGePoint3d endPt)
{
  setStartPoint(startPt);
  setEndPoint(endPt);

  OdConstrainedEllipseImpl::set(centerPt, dir, majorRad, minorRad);
  return eOk;
}
