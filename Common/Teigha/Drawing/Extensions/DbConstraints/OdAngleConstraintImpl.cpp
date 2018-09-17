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
#include "OdAngleConstraintImpl.h"
#include "DbFiler.h"

#include "DbDimension.h"
#include "Db2LineAngularDimension.h"

double OdAngleConstraintImpl::ANGLE_MULTIPLIER = OdaPI/180.;

OdAngleConstraintImpl::OdAngleConstraintImpl():OdExplicitConstraintImpl(),
                                        m_secType(OdAngleConstraint::kParallelAntiClockwise)
{
}

OdAngleConstraintImpl::OdAngleConstraintImpl(OdAngleConstraint::SectorType type):OdExplicitConstraintImpl(),
                                        m_secType(type)
{
}

OdAngleConstraintImpl::~OdAngleConstraintImpl()
{
}

OdResult OdAngleConstraintImpl::setSectorType(OdAngleConstraint::SectorType type)
{
  m_secType = type;
  return eOk;
}

OdAngleConstraint::SectorType OdAngleConstraintImpl::sectorType() const
{
  return m_secType;
}

void OdAngleConstraintImpl::setAngleMultiplier(double multiplier)
{
  OdAngleConstraintImpl::ANGLE_MULTIPLIER = multiplier;
}

double OdAngleConstraintImpl::angleMultiplier()
{
  return ANGLE_MULTIPLIER;
}

OdResult OdAngleConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdExplicitConstraintImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;
   
  m_secType = static_cast<OdAngleConstraint::SectorType>(pFiler->rdUInt8());

  return res;
}

void OdAngleConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdExplicitConstraintImpl::dwgOutFields(pFiler);
  pFiler->wrUInt8((OdUInt8)m_secType);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdAngleConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdExplicitConstraintImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(280)
  m_secType = static_cast<OdAngleConstraint::SectorType>(pFiler->rdUInt8());
  return res;
}

#undef NEXT_CODE

void OdAngleConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdExplicitConstraintImpl::dxfOutFields(pFiler);
  pFiler->wrUInt8(280, (OdUInt8)m_secType);
}

static OdGePoint3d getXPoint(const OdGePoint3d& iPoint1,
                             const OdGeVector3d& iVector1,
                             const OdGePoint3d& iPoint2,
                             const OdGeVector3d& iVector2)
{
  double eps = 1e-15, det = iVector2.x * iVector1.y - iVector2.y * iVector1.x;
  OdGePoint3d xPt;
  if (fabs(det) < eps)
    xPt = iPoint1 + (iPoint2 - iPoint1) / 2;
  else
    xPt = iPoint1 + iVector1 * (iVector2.x * (iPoint2.y - iPoint1.y) - iVector2.y * (iPoint2.x - iPoint1.x)) / det;
  return xPt;
}

void OdAngleConstraintImpl::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  OdDbObjectId id = dimDependencyId();
  if ( id.isNull() )
    return;

  OdArray<OdConstrainedGeometry*> arrGeom;
  getConnectedGeometries(arrGeom);
  
  ODA_ASSERT(arrGeom.length() == 2);

  OdDbAssocDependencyPtr assocDepPtr = id.openObject();
  id = assocDepPtr->dependentOnObject();
  if ( id.isNull() )
    return;

  OdDb2LineAngularDimensionPtr dimPtr = id.openObject(OdDb::kForWrite);
  
  OdSmartPtr<OdConstrainedBoundedLine> line1Ptr = OdConstrainedBoundedLine::cast(arrGeom[0]);
  OdSmartPtr<OdConstrainedBoundedLine> line2Ptr = OdConstrainedBoundedLine::cast(arrGeom[1]);

  OdGeMatrix3d matrToPlane = matrToWrd.inverse();

  OdGePoint3d xline1SPt = dimPtr->xLine1Start();
  OdGePoint3d xline1EPt = dimPtr->xLine1End();

  OdGePoint3d xline2SPt = dimPtr->xLine2Start();
  OdGePoint3d xline2EPt = dimPtr->xLine2End();

  OdGePoint3d arcPt = dimPtr->arcPoint();

  xline1SPt.transformBy(matrToPlane);
  xline1EPt.transformBy(matrToPlane);
  xline2SPt.transformBy(matrToPlane);
  xline2EPt.transformBy(matrToPlane);
  arcPt.transformBy(matrToPlane);

  OdGeVector3d dir1Vec = xline1EPt - xline1SPt;
  OdGeVector3d dir2Vec = xline2EPt - xline2SPt;

  OdGePoint3d xPt = getXPoint(xline1SPt, dir1Vec, xline2SPt, dir2Vec);

  OdGeVector3d arcVec = arcPt - xPt;

  OdGeVector3d dir1VecNorm = dir1Vec.normal();
  OdGeVector3d dir2VecNorm = dir2Vec.normal();
  if (sectorType() == OdAngleConstraint::kAntiParallelClockwise ||
      sectorType() == OdAngleConstraint::kAntiParallelAntiClockwise)
    dir2VecNorm.negate();
  double arcVecSign = arcVec.dotProduct(dir1VecNorm + dir2VecNorm) >= 0 ? 1 : -1;

  switch ( sectorType() )
  {
    case OdAngleConstraint::kParallelAntiClockwise:
      xline1SPt = line1Ptr->startPoint();
      xline1EPt = line1Ptr->endPoint();

      xline2SPt = line2Ptr->startPoint();
      xline2EPt = line2Ptr->endPoint();
    break;
    case OdAngleConstraint::kAntiParallelClockwise:
      xline1SPt = line2Ptr->endPoint();
      xline1EPt = line2Ptr->startPoint();

      xline2SPt = line1Ptr->startPoint();
      xline2EPt = line1Ptr->endPoint();
    break;
    case OdAngleConstraint::kParallelClockwise:
      xline1SPt = line2Ptr->startPoint();
      xline1EPt = line2Ptr->endPoint();

      xline2SPt = line1Ptr->startPoint();
      xline2EPt = line1Ptr->endPoint();
    break;
    case OdAngleConstraint::kAntiParallelAntiClockwise:
      xline1SPt = line1Ptr->startPoint();
      xline1EPt = line1Ptr->endPoint();

      xline2SPt = line2Ptr->endPoint();
      xline2EPt = line2Ptr->startPoint();
    break;
  }
  
  dir1Vec = xline1EPt - xline1SPt;
  dir2Vec = xline2EPt - xline2SPt;
  xPt = getXPoint(xline1SPt, dir1Vec, xline2SPt, dir2Vec);

  dir1Vec.normalize();
  dir2Vec.normalize();

  OdGeVector3d arcVecNew = (dir1Vec + dir2Vec).normalize();
  if (arcVecNew.length() < 0.5) {
    arcVecNew.x = -dir1Vec.y;
    arcVecNew.y = dir1Vec.x;
  }
  arcVecNew *= arcVec.length()*arcVecSign;

  arcPt = xPt + arcVecNew;


  xline1SPt.transformBy(matrToWrd);
  xline1EPt.transformBy(matrToWrd);
  xline2SPt.transformBy(matrToWrd);
  xline2EPt.transformBy(matrToWrd);
  arcPt.transformBy(matrToWrd);

  dimPtr->setXLine1Start(xline1SPt);
  dimPtr->setXLine1End(xline1EPt);

  dimPtr->setXLine2Start(xline2SPt);
  dimPtr->setXLine2End(xline2EPt);

  dimPtr->setArcPoint(arcPt);
  dimPtr->setTextPosition(arcPt);
}
