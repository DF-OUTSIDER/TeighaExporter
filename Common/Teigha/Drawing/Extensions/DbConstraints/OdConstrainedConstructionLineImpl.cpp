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

#include "OdConstrainedConstructionLineImpl.h"

OdConstrainedConstructionLineImpl::OdConstrainedConstructionLineImpl():OdConstrainedLineImpl()
{
  
}

OdResult OdConstrainedConstructionLineImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  return eOk;
}

OdResult OdConstrainedConstructionLineImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  return eOk;
}
/////////////////////////////////////////////////////////////////////////////////
OdConstrained2PointsConstructionLineImpl::OdConstrained2PointsConstructionLineImpl():OdConstrainedConstructionLineImpl()
{
}

OdConstrained2PointsConstructionLineImpl::OdConstrained2PointsConstructionLineImpl(const OdGePoint3d& point1, const OdGePoint3d& point2)
{
  set(point1, point2);
}

void OdConstrained2PointsConstructionLineImpl::set(const OdGePoint3d& point1, const OdGePoint3d& point2)
{
  bool origGeomSet = m_origGeomSet;
  setPointOnLine(point1);
  setDirection(point2 - point1);
  m_origGeomSet = origGeomSet;
}

void OdConstrained2PointsConstructionLineImpl::DoPostEvaluateJob()
{
  OdArray<OdConstrainedGeometry*> aPGeoms;
  getConnectedGeometries(aPGeoms);
  
  OdConstrainedPoint* p1 = NULL;
  OdConstrainedPoint* p2 = NULL;
  
  bool p1Set = false;
  bool p2Set = false;

  for ( unsigned int i = 0; i < aPGeoms.length(); i++ )
  {
    if ( aPGeoms[i]->isKindOf(OdConstrainedPoint::desc()) )
    {
      if ( p1Set )
      {
        p2 = OdConstrainedPoint::cast(aPGeoms[i]);
        p2Set = true;
      }
      else
      {
        p1 = OdConstrainedPoint::cast(aPGeoms[i]);
        p1Set = true;
      }
    }

    if ( p1Set && p2Set )
      break;
  }

  if ( p1Set && p2Set )
    set(p1->point(), p2->point());
}
/////////////////////////////////////////////////////////////////////////////////
OdConstrainedDatumLineImpl::OdConstrainedDatumLineImpl():OdConstrainedConstructionLineImpl()
{
  init(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis);
}

OdConstrainedDatumLineImpl::OdConstrainedDatumLineImpl(const OdGePoint3d& pointOnLine, const OdGeVector3d& direction):OdConstrainedConstructionLineImpl()
{
  init(pointOnLine, direction);
}

void OdConstrainedDatumLineImpl::init(const OdGePoint3d& pointOnLine, const OdGeVector3d& direction)
{
  setPointOnLine(pointOnLine);
  setDirection(direction);
  m_origGeomSet = false;
}
