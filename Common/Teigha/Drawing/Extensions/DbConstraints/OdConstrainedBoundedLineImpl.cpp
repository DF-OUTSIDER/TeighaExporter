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
#include "OdConstrainedBoundedLineImpl.h"

#include "DbLine.h"
#include "DbRay.h"
#include "DbPolyline.h"

#include "DbFiler.h"

OdConstrainedBoundedLineImpl::OdConstrainedBoundedLineImpl():OdConstrainedLineImpl(),
                                                     m_bRay(false),
                                                     m_originLine(),
                                                     m_line(),
                                                     m_originRay(),
                                                     m_ray()
{
  m_implObjType = BoundedLineImpl;
}

OdConstrainedBoundedLineImpl::OdConstrainedBoundedLineImpl(bool bRay):OdConstrainedLineImpl(),
                                                                    m_bRay(bRay),
                                                                    m_originLine(),
                                                                    m_line(),
                                                                    m_originRay(),
                                                                    m_ray()
{
  m_implObjType = BoundedLineImpl;
}

OdResult OdConstrainedBoundedLineImpl::init(const OdDbObjectId depId, bool bRay)
{
  OdResult res = OdConstrainedGeometryImpl::init(depId);
  if ( res != eOk )
    return res;

  m_bRay = bRay;

  OdDbAssoc2dConstraintGroupPtr pGr = m_groupObjectId.openObject();
  OdGeMatrix3d matrFromWrd = OdGeMatrix3d::worldToPlane(pGr->getWorkPlane());
  setDataFromGeomDependency(depId, matrFromWrd);

  m_origGeomSet = false;
  return eOk;
}

bool OdConstrainedBoundedLineImpl::isRay() const
{
  return m_bRay;
}

OdGePoint3d OdConstrainedBoundedLineImpl::startPoint() const
{
  OdGePoint3d res;
  if ( m_bRay )
    m_ray.hasStartPoint(res);
  else
    res = m_line.startPoint();

  return res;
}

OdGePoint3d OdConstrainedBoundedLineImpl::endPoint() const
{
  OdGePoint3d res;
  if ( !m_bRay )
    res = m_line.endPoint();

  return res;
}

OdGePoint3d OdConstrainedBoundedLineImpl::midPoint() const
{
  if ( !m_bRay )
    return m_line.evalPoint(0.5);
  else
    return OdGePoint3d();
}

bool OdConstrainedBoundedLineImpl::isBounded() const
{
  return true;
}

void OdConstrainedBoundedLineImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedLineImpl::dwgOutFields(pFiler);
  pFiler->wrBool(m_bRay);
  if ( m_bRay )
  {
    pFiler->wrPoint3d(startPoint());
  }
  else
  {
    pFiler->wrPoint3d(startPoint());
    pFiler->wrPoint3d(endPoint());
  }
}

OdResult OdConstrainedBoundedLineImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedLineImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  m_bRay = pFiler->rdBool();
  OdGePoint3d ptStart = pFiler->rdPoint3d();

  if ( m_bRay )
  {
    OdConstrainedLineImpl::m_line.set(ptStart, direction());
    m_ray.set(ptStart, direction());
  }
  else
  {
    OdGePoint3d ptEnd = pFiler->rdPoint3d();
    OdConstrainedLineImpl::m_line.set(ptStart, ptEnd);
    m_line.set(ptStart, ptEnd);
  }
  return eOk;
}

void OdConstrainedBoundedLineImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedLineImpl::dxfOutFields(pFiler);
  pFiler->wrBool(290, m_bRay);
  if ( m_bRay )
  {
    pFiler->wrPoint3d(10, startPoint());
  }
  else
  {
    pFiler->wrPoint3d(10, startPoint());
    pFiler->wrPoint3d(11, endPoint());
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedBoundedLineImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedLineImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  OdGePoint3d ptStart;
  OdGePoint3d ptEnd;
  NEXT_CODE(290)
  m_bRay = pFiler->rdBool();
  NEXT_CODE(10)

  pFiler->rdPoint3d(ptStart);

  if ( m_bRay )
  {
    m_ray.set(ptStart, direction());
  }
  else
  {
    NEXT_CODE(11)
    pFiler->rdPoint3d(ptEnd);
    m_line.set(ptStart, ptEnd);
  }
  return eOk;
}

#undef NEXT_CODE

bool OdConstrainedBoundedLineImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( !OdConstrainedLineImpl::operator==(geom) ) 
    return false;

  OdConstrainedBoundedLineImpl* sec = (OdConstrainedBoundedLineImpl*)&geom;

  if ( isRay() != sec->isRay() )
    return false;
  else if ( isRay() )
    return startPoint().isEqualTo(sec->startPoint());
  else
    return startPoint().isEqualTo(sec->startPoint()) && endPoint().isEqualTo(sec->endPoint());
}

bool OdConstrainedBoundedLineImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedBoundedLineImpl::setStartPoint(const OdGePoint3d& val)
{
  OdGePoint3d ptEnd = endPoint();

  if ( val == startPoint() && val == pointOnLine() )
    return eOk;

  if ( !m_origGeomSet )
  {
    if ( m_bRay )
      m_originRay = m_ray;
    else
      m_originLine = m_line;
  }

  OdConstrainedLineImpl::setPointOnLine(val);

  if ( m_bRay )
    m_ray.set(val, direction());
  else
  {
    m_line.set(val, endPoint());
    OdGeVector3d vec = m_line.endPoint() - m_line.startPoint();
    OdConstrainedLineImpl::setDirection(vec);
  }

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::setEndPoint(const OdGePoint3d& val)
{
  OdGePoint3d ptSt = startPoint();

  if ( m_bRay )
    return eBadObjType;

  if ( val == endPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_originLine = m_line;
  }

  m_line.set(startPoint(), val);

  OdGeVector3d vec = m_line.endPoint() - m_line.startPoint();
  OdConstrainedLineImpl::setDirection(vec);

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::setPointOnLine(const OdGePoint3d& val)
{
  return setStartPoint(val);
}

OdResult OdConstrainedBoundedLineImpl::setDirection(const OdGeVector3d& val)
{
  OdGeVector3d v;
  if ( m_bRay )
  {
    if ( !m_origGeomSet )
    {
      m_originRay = m_ray;
    }
    v = val;
    v.normalize();
    OdConstrainedLineImpl::setDirection(v);
    m_ray.set(startPoint(), direction());
  }
  else
  {
    OdConstrainedLineImpl::setDirection(val);
    m_line.set(startPoint(), val);
  } 

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbObjectPtr objPtr = geomObject(geomDependencyId);

  if ( objPtr->isKindOf(OdDbLine::desc()) )
  {
    OdDbLinePtr linePtr = OdDbLine::cast(objPtr);
    OdGePoint3d start(linePtr->startPoint());
    OdGePoint3d end(linePtr->endPoint());

    start.transformBy(blockToWrd);
    end.transformBy(blockToWrd);

    start.transformBy(matrFromWrd);
    end.transformBy(matrFromWrd);

    if ( !m_origGeomSet )
    {
      OdConstrainedLineImpl::m_originLine = OdConstrainedLineImpl::m_line;
      m_originLine = m_line;
      m_origGeomSet = true;
    }

    OdGeVector3d vec = end - start;
    OdConstrainedLineImpl::m_line.set(start, vec);

    m_line.set(start, end);
  }
  else if ( objPtr->isKindOf(OdDbRay::desc()) )
  {
    OdDbRayPtr rayPtr = OdDbRay::cast(objPtr);
    OdGePoint3d start(rayPtr->basePoint());
    OdGePoint3d end(start + rayPtr->unitDir());

    start.transformBy(blockToWrd);
    end.transformBy(blockToWrd);

    start.transformBy(matrFromWrd);
    end.transformBy(matrFromWrd);

    if ( !m_origGeomSet )
    {
      OdConstrainedLineImpl::m_originLine = OdConstrainedLineImpl::m_line;
      m_originRay = m_ray;
      m_origGeomSet = true;
    }

    OdConstrainedLineImpl::m_line.set(start, (end - start).normalize());
    m_ray.set(start, end - start);
  }
  else if ( objPtr->isKindOf(OdDbPolyline::desc()) )
  {
    OdGePoint3d wrdStart, wrdEnd;
    double bulge;
    OdDbPolylinePtr pLinePtr = OdDbPolyline::cast(objPtr);
    OdResult res = getPolylineSegmentProp(geomDependencyId, pLinePtr, wrdStart, wrdEnd, bulge);
    if ( res != eOk )
      return res;

    wrdStart.transformBy(blockToWrd);
    wrdStart.transformBy(matrFromWrd);

    wrdEnd.transformBy(blockToWrd);
    wrdEnd.transformBy(matrFromWrd);

    if ( !m_origGeomSet )
    {
      OdConstrainedLineImpl::m_originLine = OdConstrainedLineImpl::m_line;
      m_originLine = m_line;
      m_origGeomSet = true;
    }

    OdGeVector3d vec = wrdEnd - wrdStart;
    OdConstrainedLineImpl::m_line.set(wrdStart, vec);
    m_line.set(wrdStart, wrdEnd);
  }
  else if ( objPtr->isKindOf( OdDb2dPolyline::desc() ) )
  {
    OdGePoint3d wrdStart, wrdEnd;
    double bulge;
    OdDb2dPolylinePtr pLinePtr = OdDb2dPolyline::cast( objPtr );
    OdResult res = getPolylineSegmentProp( geomDependencyId, pLinePtr, wrdStart, wrdEnd, bulge );
    if ( res != eOk )
      return res;

    wrdStart.transformBy( blockToWrd );
    wrdStart.transformBy( matrFromWrd );

    wrdEnd.transformBy( blockToWrd );
    wrdEnd.transformBy( matrFromWrd );

    if ( !m_origGeomSet )
    {
      OdConstrainedLineImpl::m_originLine = OdConstrainedLineImpl::m_line;
      m_originLine = m_line;
      m_origGeomSet = true;
    }

    OdGeVector3d vec = wrdEnd - wrdStart;
    OdConstrainedLineImpl::m_line.set( wrdStart, vec );
    m_line.set( wrdStart, wrdEnd );
  }
  else
  {
    ODA_FAIL();
  }

  m_idDependency = geomDependencyId;

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbLine::desc()) )
  {
    OdDbLinePtr linePtr = OdDbLine::cast(pObj);
    OdGePoint3d start(startPoint());
    OdGePoint3d end(endPoint());

    start.transformBy(matrToWrd);
    end.transformBy(matrToWrd);

    linePtr->setStartPoint(start);
    linePtr->setEndPoint(end);
  }
  else if ( pObj->isKindOf(OdDbRay::desc()) )
  {
    OdDbRayPtr rayPtr = OdDbRay::cast(pObj);
    OdGePoint3d start(startPoint());
    OdGePoint3d end(start + direction());

    start.transformBy(matrToWrd);
    end.transformBy(matrToWrd);

    rayPtr->setBasePoint(start);
    OdGeVector3d dir = end - start;
    dir.normalize();
    rayPtr->setUnitDir(dir);
  }
  else if ( pObj->isKindOf(OdDbPolyline::desc()) )
  {
    int startIdx, endIdx;
    OdDbPolylinePtr pLinePtr = OdDbPolyline::cast(pObj);
    if (getPolylineSegmentPtIdx(geomDependencyId(), pLinePtr, startIdx, endIdx) != eOk) {
      ODA_FAIL();
      return eBadObjType;
    }

    OdGeVector3d plinedir = pLinePtr->normal();
    OdGeMatrix3d wrdToPLine = OdGeMatrix3d::worldToPlane(plinedir);

    OdGePoint3d pt = startPoint();
    pt.transformBy(matrToWrd);
    pt.transformBy(wrdToPLine);
    pLinePtr->setPointAt(startIdx, pt.convert2d());

    pt = endPoint();
    pt.transformBy(matrToWrd);
    pt.transformBy(wrdToPLine);
    pLinePtr->setPointAt(endIdx, pt.convert2d());

    /*
    if ( pLinePtr->isClosed() )
    {
      OdGePoint2d sePt;
      if ( startIdx == 0 )
      {
        pLinePtr->getPointAt(0, sePt);
        pLinePtr->setPointAt(pLinePtr->numVerts() - 1, sePt);
      }
      else if ( endIdx == pLinePtr->numVerts() - 1 )
      {
        pLinePtr->getPointAt(pLinePtr->numVerts() - 1, sePt);
        pLinePtr->setPointAt(0, sePt);
      }
    }
    */
  }
  else
  {
    ODA_FAIL();
  }

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const
{
  OdGePoint3d p;
  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kStartImplicit:
      p = startPoint();
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      p = endPoint();
    break;
    case OdConstrainedImplicitPoint::kMidImplicit:
      p = midPoint();
    break;
    default:
      return eBadObjType;
  }

  res.set(p.x, p.y, p.z);

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val)
{
  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kStartImplicit:
      setStartPoint(val);
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      setEndPoint(val);
    break;
    case OdConstrainedImplicitPoint::kMidImplicit:
      return eNotImplementedYet;
    default:
      return eBadObjType;
  }

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint,
                                                    OdGePoint3d& res) const
{
  switch ( pImplicitPoint->pointType() )
  {
    case OdConstrainedImplicitPoint::kStartImplicit:
      if ( isRay() )
        getOriginalOdGeRay3d().hasStartPoint(res);
      else
        res = getOriginalOdGeLineSeg3d().startPoint();
    break;
    case OdConstrainedImplicitPoint::kEndImplicit:
      res = getOriginalOdGeLineSeg3d().endPoint();
    break;
    case OdConstrainedImplicitPoint::kMidImplicit:
      res = getOriginalOdGeLineSeg3d().evalPoint(0.5);
    break;
    default:
      return eBadObjType;
  }


  return eOk;
}

const OdGeLineSeg3d OdConstrainedBoundedLineImpl::getOdGeLineSeg3d() const
{
  return m_line;
}

const OdGeLineSeg3d OdConstrainedBoundedLineImpl::getOriginalOdGeLineSeg3d() const
{
  if ( m_origGeomSet )
    return m_originLine;
  else
    return m_line;
}

const OdGeRay3d OdConstrainedBoundedLineImpl::getOdGeRay3d() const
{
  return m_ray;
}

const OdGeRay3d OdConstrainedBoundedLineImpl::getOriginalOdGeRay3d() const
{
  if ( m_origGeomSet )
    return m_originRay;
  else
    return m_ray;
}

OdResult OdConstrainedBoundedLineImpl::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
  OdResult res = OdConstrainedLineImpl::getFullSubentPaths(subentPaths);
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
  {
    if (endIdx == 0)
      endIdx = pLinePtr->numVerts();
    path->subentId().setIndex(endIdx);
  }

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::set(const OdGePoint3d pointOnLine, const OdGeVector3d dir)
{
  if ( !isRay() )
    return eBadObjType;

  if ( startPoint() == pointOnLine && direction() == dir )
    return eOk;

  if ( !m_origGeomSet )
  {
    OdConstrainedLineImpl::m_originLine = OdConstrainedLineImpl::m_line;
    m_originRay = m_ray;
    m_origGeomSet = true;
  }

  OdConstrainedLineImpl::m_line.set(pointOnLine, dir.normal());
  m_ray.set(pointOnLine, dir);

  return eOk;
}

OdResult OdConstrainedBoundedLineImpl::set(const OdGePoint3d startPt, const OdGePoint3d endPt)
{
  if ( isRay() )
    return eBadObjType;

  if ( startPt == startPoint() && endPt == endPoint() )
    return eOk;

  if ( !m_origGeomSet )
  {
    OdConstrainedLineImpl::m_originLine = OdConstrainedLineImpl::m_line;
    m_originLine = m_line;
    m_origGeomSet = true;
  }

  OdGeVector3d vec = endPt - startPt;
  OdConstrainedLineImpl::m_line.set(startPt, vec);

  m_line.set(startPt, endPt);

  return eOk;
}
