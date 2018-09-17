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

#include "OdConstrainedLineImpl.h"
#include "Ge/GeLine3d.h"

#include "DbXline.h"

#include "DbFiler.h"

OdConstrainedLineImpl::OdConstrainedLineImpl():OdConstrainedCurveImpl(),
                                                        m_line(),
                                                        m_originLine()
{
  m_implObjType = LineImpl;
}

OdConstrainedLineImpl::~OdConstrainedLineImpl()
{
}

OdResult OdConstrainedLineImpl::init(const OdDbObjectId depId)
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

OdGePoint3d OdConstrainedLineImpl::pointOnLine() const
{
  return m_line.evalPoint(0.0);
}

OdGeVector3d OdConstrainedLineImpl::direction() const
{
  OdGeVector3d dir = m_line.evalPoint(1.0) - m_line.evalPoint(0.0);
  dir.normalizeGetLength(); // Prevent exception in normalize()
  return dir;
}

bool OdConstrainedLineImpl::isBounded() const
{
  return false;
}

void OdConstrainedLineImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedCurveImpl::dwgOutFields(pFiler);
  pFiler->wrPoint3d(pointOnLine());
  pFiler->wrVector3d(direction());
}

OdResult OdConstrainedLineImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  OdGePoint3d ptOrigin = pFiler->rdPoint3d();
  OdGeVector3d vDir = pFiler->rdVector3d();

  m_line.set(ptOrigin, vDir);

  return eOk;
}

void OdConstrainedLineImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedCurveImpl::dxfOutFields(pFiler);
  pFiler->wrPoint3d(10, pointOnLine());
  pFiler->wrVector3d(10, direction());
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedLineImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedCurveImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  OdGePoint3d ptOrigin;
  OdGeVector3d vDir;

  NEXT_CODE(10)
  pFiler->rdPoint3d(ptOrigin);
  NEXT_CODE(10)
  pFiler->rdVector3d(vDir);

  m_line.set(ptOrigin, vDir);

  return eOk;
}

#undef NEXT_CODE

bool OdConstrainedLineImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( getType(geom) != m_implObjType )
    return false;

  OdConstrainedLineImpl *sec = (OdConstrainedLineImpl*)&geom;

  return pointOnLine().isEqualTo(sec->pointOnLine()) && direction().isEqualTo(sec->direction());
}

bool OdConstrainedLineImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedLineImpl::setPointOnLine(const OdGePoint3d& val)
{
  if ( val == pointOnLine() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originLine = m_line;
    m_origGeomSet = true;
  }

  m_line.set(val, m_line.direction());

  return eOk;
}

OdResult OdConstrainedLineImpl::setDirection(const OdGeVector3d& val)
{
  if (  val == direction() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originLine = m_line;
    m_origGeomSet = true;
  }

  m_line.set(m_line.evalPoint(0.0), val);
  return eOk;
}

OdResult OdConstrainedLineImpl::set(const OdGePoint3d ptOnLine, const OdGeVector3d dir)
{
  if ( ptOnLine == pointOnLine() && dir == direction() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originLine = m_line;
    m_origGeomSet = true;
  }

  m_line.set(ptOnLine, dir);
  return eOk;
}

OdResult OdConstrainedLineImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbObjectPtr objPtr = geomObject(geomDependencyId);

  if ( objPtr->isKindOf(OdDbXline::desc()) )
  {
    OdDbXlinePtr linePtr = OdDbXline::cast(objPtr);
    OdGePoint3d start(linePtr->basePoint());
    OdGePoint3d end(start + linePtr->unitDir());

    start.transformBy(blockToWrd);
    end.transformBy(blockToWrd);

    start.transformBy(matrFromWrd);
    end.transformBy(matrFromWrd);

    if ( !m_origGeomSet )
    {
      m_originLine = m_line;
      m_origGeomSet = true;
    }

    m_line.set(start, end - start);
  }

  m_idDependency = geomDependencyId;
  
  return eOk;
}

OdResult OdConstrainedLineImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  if ( pObj->isKindOf(OdDbXline::desc()) )
  {
    OdDbXlinePtr linePtr = OdDbXline::cast(pObj);
    OdGePoint3d start(pointOnLine());
    OdGePoint3d end(start + direction());

    start.transformBy(matrToWrd);
    end.transformBy(matrToWrd);

    linePtr->setBasePoint(start);
    linePtr->setUnitDir((end - start).normalize());

    m_origGeomSet = false;
  }

  return eOk;
}

OdResult OdConstrainedLineImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  return eBadObjType;
}

OdResult OdConstrainedLineImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  return eBadObjType;
}

OdResult OdConstrainedLineImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  return eBadObjType;
}

const OdGeLine3d OdConstrainedLineImpl::getOdGeLine3d() const
{
  return m_line;
}

const OdGeLine3d OdConstrainedLineImpl::getOriginalOdGeLine3d() const
{
  if ( m_origGeomSet )
    return m_originLine;
  else
    return m_line;
}
