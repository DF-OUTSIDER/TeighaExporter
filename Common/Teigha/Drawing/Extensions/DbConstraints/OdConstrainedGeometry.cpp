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
#include "OdConstrainedGeometry.h"

#include "RxObjectImpl.h"

#include "DbAssocEdgePersSubentId.h"

#include "DbImpAssoc2dConstraintGroup.h"
#include "OdConstraintGroupNodeImpl.h"
#include "OdConstrainedPointImpl.h"
#include "OdConstrainedImplicitPointImpl.h"
#include "OdConstrainedCurveImpl.h"
#include "OdConstrainedCircleImpl.h"
#include "OdConstrainedArcImpl.h"
#include "OdConstrainedLineImpl.h"
#include "OdConstrainedConstructionLineImpl.h"
#include "OdConstrainedBoundedLineImpl.h"
#include "OdConstrainedEllipseImpl.h"
#include "OdConstrainedBoundedEllipseImpl.h"
#include "OdConstrainedSplineImpl.h"
#include "OdConstrainedRigidSetImpl.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedGeometry, OdConstraintGroupNode, L"AcConstrainedGeometry");
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedPoint, OdConstrainedGeometry, L"AcConstrainedPoint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedImplicitPoint, OdConstrainedPoint,
                                 L"AcConstrainedImplicitPoint", RXIMPL_CONSTR);

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedCurve, OdConstrainedGeometry, L"AcConstrainedCurve");

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedCircle, OdConstrainedCurve,
                                 L"AcConstrainedCircle", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedArc, OdConstrainedCircle,
                                 L"AcConstrainedArc", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedLine, OdConstrainedCurve,
                                 L"AcConstrainedLine", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedBoundedLine, OdConstrainedLine,
                                 L"AcConstrainedBoundedLine", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedConstructionLine, OdConstrainedLine,
                                 L"AcConstrainedConstructionLine", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrained2PointsConstructionLine, OdConstrainedConstructionLine,
                                 L"AcConstrained2PointsConstructionLine", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedDatumLine, OdConstrainedConstructionLine,
                                 L"AcConstrainedDatumLine", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedEllipse, OdConstrainedCurve,
                                 L"AcConstrainedEllipse", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedBoundedEllipse, OdConstrainedEllipse,
                                 L"AcConstrainedBoundedEllipse", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedSpline, OdConstrainedCurve,
                                 L"AcConstrainedSpline", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConstrainedRigidSet, OdConstrainedGeometry,
                                 L"AcConstrainedRigidSet", RXIMPL_CONSTR);
///////////////////////////////////////////////////////////////////////////////////////////////////

OdConstrainedGeometry::OdConstrainedGeometry()
{}


bool OdConstrainedGeometry::operator==(const OdConstrainedGeometry& geom) const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->operator ==(*(OdConstrainedGeometryImpl*)(geom.m_pImpl));
}

bool OdConstrainedGeometry::operator!=(const OdConstrainedGeometry& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedGeometry::setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                             const OdGeMatrix3d& matrFromWrd)
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->setDataFromGeomDependency(geomDependencyId, matrFromWrd);
}

bool OdConstrainedGeometry::isModified() const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->isModified();
}

OdResult OdConstrainedGeometry::init(const OdDbObjectId geomDependencyId)
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->init(geomDependencyId);
}

bool OdConstrainedGeometry::isReadOnly() const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->isReadOnly();
}

OdDbObjectId OdConstrainedGeometry::geomDependencyId() const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->geomDependencyId();
}

OdConstrainedRigidSet* OdConstrainedGeometry::getOwningRigidSet() const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->getOwningRigidSet();
}

OdResult OdConstrainedGeometry::getConnectedConstraints(OdArray<OdGeomConstraint*>& apConstraints) const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->getConnectedConstraints(apConstraints);
}

OdResult OdConstrainedGeometry::getCommonConstraints(OdConstrainedGeometry* pOtherConsGeom, OdArray<OdGeomConstraint*>& apConstraints) const
{
  return ((OdConstrainedGeometryImpl*)m_pImpl)->getCommonConstraints(pOtherConsGeom, apConstraints);
}

OdResult OdConstrainedGeometry::getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const
{
	return ((OdConstrainedGeometryImpl*)m_pImpl)->getConnectedGeometries(apConsGeoms);
}

OdResult OdConstrainedGeometry::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
	return ((OdConstrainedGeometryImpl*)m_pImpl)->getFullSubentPaths(subentPaths);
}

void OdConstrainedGeometry::DoPostEvaluateJob()
{
  ((OdConstrainedGeometryImpl*)m_pImpl)->DoPostEvaluateJob();
}
/////////////////////////////////////////////////////////////////////////////////////
OdConstrainedPoint::OdConstrainedPoint(bool bCreateImp):OdConstrainedGeometry()
{
  if ( bCreateImp )
    m_pImpl = new OdConstrainedPointImpl();
}

OdGePoint3d OdConstrainedPoint::point() const
{
  return ((OdConstrainedPointImpl*)m_pImpl)->point();
}

OdResult OdConstrainedPoint::setPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedPointImpl*)m_pImpl)->setPoint(val);
}

const OdGePoint3d OdConstrainedPoint::getOriginalPoint() const
{
  return ((OdConstrainedPointImpl*)m_pImpl)->getOriginalPoint();
}
/////////////////////////////////////////////////////////////////////////////////////

OdConstrainedImplicitPoint::OdConstrainedImplicitPoint()
  : OdConstrainedPoint(false)
{
  m_pImpl = new OdConstrainedImplicitPointImpl();
}

OdResult OdConstrainedImplicitPoint::init(OdConstraintGroupNodeId constrCurvId, ImplicitPointType ptype, int index)
{
  return ((OdConstrainedImplicitPointImpl*)m_pImpl)->init(constrCurvId, ptype, index);
}

OdConstrainedImplicitPoint::ImplicitPointType OdConstrainedImplicitPoint::pointType()  const
{
  return ((OdConstrainedImplicitPointImpl*)m_pImpl)->pointType();
}

OdInt32 OdConstrainedImplicitPoint::pointIndex() const
{
  return ((OdConstrainedImplicitPointImpl*)m_pImpl)->pointIndex();
}

OdConstraintGroupNodeId OdConstrainedImplicitPoint::constrainedCurveId() const
{
  return ((OdConstrainedImplicitPointImpl*)m_pImpl)->constrainedCurveId();
}

/////////////////////////////////////////////////////////////////////////////////////
OdConstrainedCurve::OdConstrainedCurve():OdConstrainedGeometry()
{
  
}

void OdConstrainedCurve::getConstrainedImplicitPoints(OdArray<OdConstrainedImplicitPoint*>& apImplicitPoints) const
{
  ((OdConstrainedCurveImpl*)m_pImpl)->getConstrainedImplicitPoints(apImplicitPoints);
}

OdResult OdConstrainedCurve::point(const OdConstrainedImplicitPoint* pImplicitPoint, OdGePoint3d& res) const
{
  OdConstrainedImplicitPointImpl* point =
          (OdConstrainedImplicitPointImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pImplicitPoint);
  return ((OdConstrainedCurveImpl*)m_pImpl)->point(point, res);
}

OdResult OdConstrainedCurve::setPoint(const OdConstrainedImplicitPoint* pImplicitPoint, const OdGePoint3d& val)
{
  OdConstrainedImplicitPointImpl* point =
          (OdConstrainedImplicitPointImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pImplicitPoint);
  return ((OdConstrainedCurveImpl*)m_pImpl)->setPoint(point, val);
}

OdResult OdConstrainedCurve::getOriginalPoint(const OdConstrainedImplicitPoint* pImplicitPoint, OdGePoint3d& res) const
{
  OdConstrainedImplicitPointImpl* point =
          (OdConstrainedImplicitPointImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pImplicitPoint);
  return ((OdConstrainedCurveImpl*)m_pImpl)->getOriginalPoint(point, res);
}

/////////////////////////////////////////////////////////////////////////////////////
OdConstrainedLine::OdConstrainedLine(bool bCreateImp):OdConstrainedCurve()
{
  if ( bCreateImp )
    m_pImpl = new OdConstrainedLineImpl();
}

OdGePoint3d OdConstrainedLine::pointOnLine() const
{
  return ((OdConstrainedLineImpl*)m_pImpl)->pointOnLine();
}

OdGeVector3d OdConstrainedLine::direction() const
{
  return ((OdConstrainedLineImpl*)m_pImpl)->direction();
}

bool OdConstrainedLine::isBounded() const
{
  return ((OdConstrainedLineImpl*)m_pImpl)->isBounded();
}

OdResult OdConstrainedLine::setPointOnLine(const OdGePoint3d& val)
{
  return ((OdConstrainedLineImpl*)m_pImpl)->setPointOnLine(val);
}

OdResult OdConstrainedLine::setDirection(const OdGeVector3d& val)
{
  return ((OdConstrainedLineImpl*)m_pImpl)->setDirection(val);
}

const OdGeLine3d OdConstrainedLine::getOdGeLine3d() const
{
  return ((OdConstrainedLineImpl*)m_pImpl)->getOdGeLine3d();
}

const OdGeLine3d OdConstrainedLine::getOriginalOdGeLine3d() const
{
  return ((OdConstrainedLineImpl*)m_pImpl)->getOriginalOdGeLine3d();
}

OdResult OdConstrainedLine::set(const OdGePoint3d pointOnLine, const OdGeVector3d dir)
{
  return ((OdConstrainedLineImpl*)m_pImpl)->set(pointOnLine, dir);
}

/////////////////////////////////////////////////////////////////////////////////
OdConstrainedConstructionLine::OdConstrainedConstructionLine():OdConstrainedLine(false)
{
  
}

/////////////////////////////////////////////////////////////////////////////////
OdConstrained2PointsConstructionLine::OdConstrained2PointsConstructionLine():OdConstrainedConstructionLine()
{
  m_pImpl = new OdConstrained2PointsConstructionLineImpl();
}
/////////////////////////////////////////////////////////////////////////////////
OdConstrainedDatumLine::OdConstrainedDatumLine():OdConstrainedConstructionLine()
{
  m_pImpl = new OdConstrainedDatumLineImpl();
}

/////////////////////////////////////////////////////////////////////////////////
OdConstrainedBoundedLine::OdConstrainedBoundedLine():OdConstrainedLine(false)
{
  m_pImpl = new OdConstrainedBoundedLineImpl();
}

OdResult OdConstrainedBoundedLine::init(const OdDbObjectId depId, bool bRay)
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->init(depId, bRay);
}

bool OdConstrainedBoundedLine::isRay() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->isRay();
}

OdGePoint3d OdConstrainedBoundedLine::startPoint() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->startPoint();
}

OdGePoint3d OdConstrainedBoundedLine::endPoint() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->endPoint();
}

OdGePoint3d OdConstrainedBoundedLine::midPoint() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->midPoint();
}

bool OdConstrainedBoundedLine::isBounded() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->isBounded();
}

OdResult OdConstrainedBoundedLine::setStartPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->setStartPoint(val);
}

OdResult OdConstrainedBoundedLine::setEndPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->setEndPoint(val);
}

const OdGeLineSeg3d OdConstrainedBoundedLine::getOdGeLineSeg3d() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->getOdGeLineSeg3d();
}

const OdGeLineSeg3d OdConstrainedBoundedLine::getOriginalOdGeLineSeg3d() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->getOriginalOdGeLineSeg3d();
}

const OdGeRay3d OdConstrainedBoundedLine::getOdGeRay3d() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->getOdGeRay3d();
}

const OdGeRay3d OdConstrainedBoundedLine::getOriginalOdGeRay3d() const
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->getOriginalOdGeRay3d();
}

OdResult OdConstrainedBoundedLine::set(const OdGePoint3d startPt, const OdGePoint3d endPt)
{
  return ((OdConstrainedBoundedLineImpl*)m_pImpl)->set(startPt, endPt);
}
///////////////////////////////////////////////////////////////////////////////
OdConstrainedCircle::OdConstrainedCircle(bool bCreateImp):OdConstrainedCurve()
{
  if ( bCreateImp )
    m_pImpl = new OdConstrainedCircleImpl();
}

OdResult OdConstrainedCircle::setCenterPoint(const OdGePoint3d& center)
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->setCenterPoint(center);
}

OdResult OdConstrainedCircle::setRadius(const double val)
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->setRadius(val);
}

double OdConstrainedCircle::radius() const
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->radius();
}

OdGePoint3d OdConstrainedCircle::centerPoint() const
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->centerPoint();
}

bool OdConstrainedCircle::isBounded() const
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->isBounded();
}

const OdGeCircArc3d OdConstrainedCircle::getOdGeCircArc3d() const
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->getOdGeCircArc3d();
}

const OdGeCircArc3d OdConstrainedCircle::getOriginalOdGeCircArc3d() const
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->getOriginalOdGeCircArc3d();
}

OdResult OdConstrainedCircle::set(const OdGePoint3d centerPt, const double rad)
{
  return ((OdConstrainedCircleImpl*)m_pImpl)->set(centerPt, rad);
}
/////////////////////////////////////////////////////////////////////////////////////


OdConstrainedArc::OdConstrainedArc():OdConstrainedCircle(false)
{
  m_pImpl = new OdConstrainedArcImpl();
}

OdGePoint3d OdConstrainedArc::startPoint() const
{
  return ((OdConstrainedArcImpl*)m_pImpl)->startPoint();
}

OdGePoint3d OdConstrainedArc::endPoint() const
{
  return ((OdConstrainedArcImpl*)m_pImpl)->endPoint();
}

OdGePoint3d OdConstrainedArc::midPoint() const
{
  return ((OdConstrainedArcImpl*)m_pImpl)->midPoint();
}

bool OdConstrainedArc::isBounded() const
{
  return ((OdConstrainedArcImpl*)m_pImpl)->isBounded();
}

OdResult OdConstrainedArc::setEndPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedArcImpl*)m_pImpl)->setEndPoint(val);
}

OdResult OdConstrainedArc::set(const OdGePoint3d centerPt, const double rad, const OdGePoint3d startPt, const OdGePoint3d endPt)
{
  return ((OdConstrainedArcImpl*)m_pImpl)->set(centerPt, rad, startPt, endPt);
}
/////////////////////////////////////////////////////////////////////////////////////

OdConstrainedEllipse::OdConstrainedEllipse(bool bCreateImp):OdConstrainedCurve()
{
  if ( bCreateImp )
    m_pImpl = new OdConstrainedEllipseImpl();
}

bool OdConstrainedEllipse::isBounded() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->isBounded();
}

OdGeVector3d OdConstrainedEllipse::direction() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->direction();
}

double OdConstrainedEllipse::majorRadius() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->majorRadius();
}

double OdConstrainedEllipse::minorRadius() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->minorRadius();
}

OdGePoint3d OdConstrainedEllipse::centerPoint() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->centerPoint();
}

OdResult OdConstrainedEllipse::setDirection(const OdGeVector3d& val)
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->setDirection(val);
}

OdResult OdConstrainedEllipse::setMajorRadius(const double val)
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->setMajorRadius(val);
}

OdResult OdConstrainedEllipse::setMinorRadius(const double val)
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->setMinorRadius(val);
}

OdResult OdConstrainedEllipse::setCenterPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->setCenterPoint(val);
}

const OdGeEllipArc3d OdConstrainedEllipse::getOdGeEllipArc3d() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->getOdGeEllipArc3d();
}

const OdGeEllipArc3d OdConstrainedEllipse::getOriginalOdGeEllipArc3d() const
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->getOriginalOdGeEllipArc3d();
}

OdResult OdConstrainedEllipse::set(const OdGePoint3d centerPt, const OdGeVector3d dir, const double majorRad, const double minorRad)
{
  return ((OdConstrainedEllipseImpl*)m_pImpl)->set(centerPt, dir, majorRad, minorRad);
}
/////////////////////////////
OdConstrainedBoundedEllipse::OdConstrainedBoundedEllipse():OdConstrainedEllipse(false)
{
  m_pImpl = new OdConstrainedBoundedEllipseImpl();
}

OdGePoint3d OdConstrainedBoundedEllipse::startPoint() const
{
  return ((OdConstrainedBoundedEllipseImpl*)m_pImpl)->startPoint();
}

OdGePoint3d OdConstrainedBoundedEllipse::endPoint() const
{
  return ((OdConstrainedBoundedEllipseImpl*)m_pImpl)->endPoint();
}

OdResult OdConstrainedBoundedEllipse::setStartPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedBoundedEllipseImpl*)m_pImpl)->setStartPoint(val);
}

OdResult OdConstrainedBoundedEllipse::setEndPoint(const OdGePoint3d& val)
{
  return ((OdConstrainedBoundedEllipseImpl*)m_pImpl)->setEndPoint(val);
}

OdResult OdConstrainedBoundedEllipse::set(const OdGePoint3d centerPt, const OdGeVector3d dir, const double majorRad, const double minorRad, const OdGePoint3d startPt, const OdGePoint3d endPt)
{
  return ((OdConstrainedBoundedEllipseImpl*)m_pImpl)->set(centerPt, dir, majorRad, minorRad, startPt, endPt);
}
/////////////////////////////

OdConstrainedSpline::OdConstrainedSpline():OdConstrainedCurve()
{
  m_pImpl = new OdConstrainedSplineImpl();
}

OdResult OdConstrainedSpline::init(const OdDbObjectId depId, const OdGeNurbCurve3d& spline)
{
  return ((OdConstrainedSplineImpl*)m_pImpl)->init(depId, spline);
}

bool OdConstrainedSpline::isBounded() const
{
  return ((OdConstrainedSplineImpl*)m_pImpl)->isBounded();
}

const OdGeNurbCurve3d OdConstrainedSpline::nurbSpline() const
{
  return ((OdConstrainedSplineImpl*)m_pImpl)->nurbSpline();
}

const OdGeNurbCurve3d OdConstrainedSpline::originalNurbSpline() const
{
  return ((OdConstrainedSplineImpl*)m_pImpl)->originalNurbSpline();
}

int OdConstrainedSpline::numOfDefinePoints() const
{
  return ((OdConstrainedSplineImpl*)m_pImpl)->numOfDefinePoints();
}

OdGePoint3d OdConstrainedSpline::definePointAt(int index) const
{
  return ((OdConstrainedSplineImpl*)m_pImpl)->definePointAt(index);
}

/////////////////////////////

OdConstrainedRigidSet::OdConstrainedRigidSet():OdConstrainedGeometry()
{
  m_pImpl = new OdConstrainedRigidSetImpl();
}

int OdConstrainedRigidSet::numOfConstrainedGeoms() const
{
  return ((OdConstrainedRigidSetImpl*)m_pImpl)->numOfConstrainedGeoms();
}

OdConstrainedGeometry* OdConstrainedRigidSet::getConstrainedGeomAt(int index) const
{
  return ((OdConstrainedRigidSetImpl*)m_pImpl)->getConstrainedGeomAt(index);
}

OdGeMatrix3d OdConstrainedRigidSet::transform() const
{
  return ((OdConstrainedRigidSetImpl*)m_pImpl)->transform();
}

void OdConstrainedRigidSet::setTransform(const OdGeMatrix3d &val)
{
  ((OdConstrainedRigidSetImpl*)m_pImpl)->setTransform(val);
}
