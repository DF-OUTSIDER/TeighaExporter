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
#include "Od3PointAngleConstraintImpl.h"

#include "DbDimension.h"
#include "Db3PointAngularDimension.h"

Od3PointAngleConstraintImpl::Od3PointAngleConstraintImpl():OdAngleConstraintImpl()
{
}

Od3PointAngleConstraintImpl::Od3PointAngleConstraintImpl(OdAngleConstraint::SectorType type):OdAngleConstraintImpl()
{
}

Od3PointAngleConstraintImpl::~Od3PointAngleConstraintImpl()
{
}

void Od3PointAngleConstraintImpl::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  OdDbObjectId id = dimDependencyId();
  if ( id.isNull() )
    return;

  OdArray<OdConstrainedGeometry*> arrGeom;
  getConnectedGeometries(arrGeom);
  
  ODA_ASSERT(arrGeom.length() == 3);

  OdDbAssocDependencyPtr assocDepPtr = id.openObject();
  id = assocDepPtr->dependentOnObject();
  if ( id.isNull() )
    return;

  OdDb3PointAngularDimensionPtr dimPtr = id.openObject(OdDb::kForWrite);
  
  OdSmartPtr<OdConstrainedPoint> centerPtr = OdConstrainedPoint::cast(arrGeom[0]);
  OdSmartPtr<OdConstrainedPoint> rightPtr = OdConstrainedPoint::cast(arrGeom[1]);
  OdSmartPtr<OdConstrainedPoint> leftPtr = OdConstrainedPoint::cast(arrGeom[2]);

  OdGeMatrix3d matrToPlane = matrToWrd.inverse();

  OdGePoint3d xline1SPt = dimPtr->centerPoint();
  OdGePoint3d xline1EPt = dimPtr->xLine1Point();

  OdGePoint3d xline2EPt = dimPtr->xLine2Point();

  OdGePoint3d arcPt = dimPtr->arcPoint();

  xline1SPt.transformBy(matrToPlane);
  xline1EPt.transformBy(matrToPlane);
  xline2EPt.transformBy(matrToPlane);
  arcPt.transformBy(matrToPlane);

  OdGeVector3d dir1Vec = xline1EPt - xline1SPt;

  OdGeVector3d arcVec = arcPt - xline1SPt;

  double arcVecLength = arcVec.length();
  double angle = 0.0;
  getMeasuredValue(angle);
  angle *= angleMultiplier();

  switch ( sectorType() )
  {
    case OdAngleConstraint::kParallelAntiClockwise:
      xline1SPt = centerPtr->point();
      xline1EPt = rightPtr->point();

      xline2EPt = leftPtr->point();
    break;
    case OdAngleConstraint::kAntiParallelClockwise:
      xline1SPt = leftPtr->point();
      xline1EPt = centerPtr->point();

      ODA_FAIL_ONCE(); // not expected sector type
      xline2EPt = rightPtr->point();
    break;
    case OdAngleConstraint::kParallelClockwise:
      xline1SPt = centerPtr->point();
      xline1EPt = rightPtr->point();

      angle = -angle;
      xline2EPt = leftPtr->point();
      break;
    case OdAngleConstraint::kAntiParallelAntiClockwise:
      xline1SPt = centerPtr->point();
      xline1EPt = rightPtr->point();

      ODA_FAIL_ONCE(); // not expected sector type
      xline2EPt = centerPtr->point();
    break;
  }
  
  OdGeVector3d arcVecNew = xline1EPt - xline1SPt;
  arcVecNew.normalize();
  arcVecNew.rotateBy(angle / 2.0, OdGeVector3d::kZAxis);
  arcPt = xline1SPt + arcVecNew * arcVecLength;

  xline1SPt.transformBy(matrToWrd);
  xline1EPt.transformBy(matrToWrd);
  xline2EPt.transformBy(matrToWrd);
  arcPt.transformBy(matrToWrd);

  dimPtr->setCenterPoint(xline1SPt);
  dimPtr->setXLine1Point(xline1EPt);

  dimPtr->setXLine2Point(xline2EPt);

  dimPtr->setArcPoint(arcPt);
  dimPtr->setTextPosition(arcPt);
}
