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
#include "OdExplicitConstr.h"
#include "OdConstrainedGeometry.h"
#include "OdExplicitConstraintImpl.h"
#include "OdDistanceConstraintImpl.h"
#include "Od3PointAngleConstraintImpl.h"
#include "OdRadiusDiameterConstraintImpl.h"
#include "OdAngleConstraintImpl.h"

#include "RxObjectImpl.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdExplicitConstraint, OdGeomConstraint,
                                 L"AcExplicitConstraint");
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDistanceConstraint, OdExplicitConstraint,
                                 L"AcDistanceConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdAngleConstraint, OdExplicitConstraint,
                                 L"AcAngleConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(Od3PointAngleConstraint, OdAngleConstraint,
                                 L"Ac3PointAngleConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdRadiusDiameterConstraint, OdExplicitConstraint,
                                 L"AcRadiusDiameterConstraint", RXIMPL_CONSTR);

OdDbObjectId OdExplicitConstraint::valueDependencyId() const
{
  return ((OdExplicitConstraintImpl*)m_pImpl)->valueDependencyId();
}

OdDbObjectId OdExplicitConstraint::dimDependencyId() const
{
  return ((OdExplicitConstraintImpl*)m_pImpl)->dimDependencyId();
}

OdResult OdExplicitConstraint::setValueDependencyId(const OdDbObjectId& valueDependencyId)
{
  return ((OdExplicitConstraintImpl*)m_pImpl)->setValueDependencyId(valueDependencyId);
}

OdResult OdExplicitConstraint::setDimDependencyId(const OdDbObjectId& dimDependencyId)
{
  return ((OdExplicitConstraintImpl*)m_pImpl)->setDimDependencyId(dimDependencyId);
}

OdResult OdExplicitConstraint::getMeasuredValue(double& val) const
{
  return ((OdExplicitConstraintImpl*)m_pImpl)->getMeasuredValue(val);
}

/////////////////////////////////////////////////////////////////
OdDistanceConstraint::OdDistanceConstraint():OdExplicitConstraint()
{
  m_pImpl = new OdDistanceConstraintImpl();
}

OdResult OdDistanceConstraint::setDirection(const OdGeVector3d& direction)
{
  return ((OdDistanceConstraintImpl*)m_pImpl)->setDirection(direction);
}

OdResult OdDistanceConstraint::setDirection(const OdConstraintGroupNodeId consLineId,
                                            DirectionType type)
{
  return ((OdDistanceConstraintImpl*)m_pImpl)->setDirection(consLineId, type);
}

OdDistanceConstraint::DirectionType OdDistanceConstraint::directionType() const
{
  return ((OdDistanceConstraintImpl*)m_pImpl)->directionType();
}

OdGeVector3d OdDistanceConstraint::direction() const
{
  return ((OdDistanceConstraintImpl*)m_pImpl)->direction();
}

OdConstraintGroupNodeId OdDistanceConstraint::constrainedLineId() const
{
  return ((OdDistanceConstraintImpl*)m_pImpl)->constrainedLineId();
}

void OdDistanceConstraint::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  ((OdDistanceConstraintImpl*)m_pImpl)->updateDimDependency(matrToWrd);
}
/////////////////////////////////////////////////////////////////
OdAngleConstraint::OdAngleConstraint():OdExplicitConstraint()
{
  m_pImpl = new OdAngleConstraintImpl();
}

OdAngleConstraint::SectorType OdAngleConstraint::sectorType() const
{
  return ((OdAngleConstraintImpl*)m_pImpl)->sectorType();
}

OdResult OdAngleConstraint::setSectorType(OdAngleConstraint::SectorType type)
{
  return ((OdAngleConstraintImpl*)m_pImpl)->setSectorType(type);
}

void OdAngleConstraint::setAngleMultiplier(double multiplier)
{
  OdAngleConstraintImpl::setAngleMultiplier(multiplier);
}

double OdAngleConstraint::angleMultiplier()
{
   return OdAngleConstraintImpl::angleMultiplier();
}

void OdAngleConstraint::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  ((OdAngleConstraintImpl*)m_pImpl)->updateDimDependency(matrToWrd);
}
/////////////////////////////////////////////////////////////////
Od3PointAngleConstraint::Od3PointAngleConstraint():OdAngleConstraint()
{
  delete m_pImpl;
  m_pImpl = new Od3PointAngleConstraintImpl();
}

void Od3PointAngleConstraint::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  ((Od3PointAngleConstraintImpl*)m_pImpl)->updateDimDependency(matrToWrd);
}
/////////////////////////////////////////////////////////////////
OdRadiusDiameterConstraint::OdRadiusDiameterConstraint():OdExplicitConstraint()
{
  m_pImpl = new OdRadiusDiameterConstraintImpl();
}

OdResult OdRadiusDiameterConstraint::setConstrType(RadiusDiameterConstrType type)
{
  return ((OdRadiusDiameterConstraintImpl*)m_pImpl)->setConstrType(type);
}

OdRadiusDiameterConstraint::RadiusDiameterConstrType OdRadiusDiameterConstraint::constrType() const
{
  return ((OdRadiusDiameterConstraintImpl*)m_pImpl)->constrType();
}

void OdRadiusDiameterConstraint::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  ((OdRadiusDiameterConstraintImpl*)m_pImpl)->updateDimDependency(matrToWrd);
}
