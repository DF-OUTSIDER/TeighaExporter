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
#include "OdGeomConstraint.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbFiler.h"
#include "OdGeomConstraintImpl.h"
#include "OdCompositeConstraintImpl.h"
#include "OdHelpParameterImpl.h"
#include "OdImplicitConstr.h"


ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdGeomConstraint, OdConstraintGroupNode, L"AcGeomConstraint");
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdHelpParameter, OdConstraintGroupNode, L"AcHelpParameter", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdCompositeConstraint, OdGeomConstraint, L"AcCompositeConstraint", RXIMPL_CONSTR);

OdGeomConstraint::OdGeomConstraint():OdConstraintGroupNode()
{}

OdResult OdGeomConstraint::getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->getConnectedGeometries(apConsGeoms);
}

OdResult OdGeomConstraint::getConnectedHelpParameters(OdArray<OdHelpParameter*>& apHelpParameters) const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->getConnectedHelpParameters(apHelpParameters);
}

OdHelpParameter* OdGeomConstraint::getConnectedHelpParameterFor(const OdConstrainedGeometry* pConsGeom) const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->getConnectedHelpParameterFor(pConsGeom);
}

OdCompositeConstraint* OdGeomConstraint::getOwningCompositeConstraint() const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->getOwningCompositeConstraint();
}

OdResult OdGeomConstraint::deactivate()
{
  return ((OdGeomConstraintImpl*)m_pImpl)->deactivate();
}

OdResult OdGeomConstraint::reactivate()
{
  return ((OdGeomConstraintImpl*)m_pImpl)->reactivate();
}

bool OdGeomConstraint::isActive() const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->isActive();
}

bool OdGeomConstraint::isImplied() const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->isImplied();
}

bool OdGeomConstraint::isInternal() const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->isInternal();
}

bool OdGeomConstraint::isEnabled() const
{
  return ((OdGeomConstraintImpl*)m_pImpl)->isEnabled();
}


/////////////////////////////////////
OdHelpParameter::OdHelpParameter():OdConstraintGroupNode()
{
  m_pImpl = new OdHelpParameterImpl();
}

OdResult OdHelpParameter::getConnectedGeometry(OdConstrainedGeometry*& pGeometry) const
{
  return ((OdHelpParameterImpl*)m_pImpl)->getConnectedGeometry(pGeometry);
}

OdResult OdHelpParameter::getConnectedConstraint(OdGeomConstraint*& pConstraint) const
{
  return ((OdHelpParameterImpl*)m_pImpl)->getConnectedConstraint(pConstraint);
}

OdResult OdHelpParameter::getConnectedEqualparamConstraints(OdArray<OdEqualHelpParameterConstraint*>& apEqualParamConstrs) const
{
  return ((OdHelpParameterImpl*)m_pImpl)->getConnectedEqualparamConstraints(apEqualParamConstrs);
}

double OdHelpParameter::getValue() const
{
  return ((OdHelpParameterImpl*)m_pImpl)->getValue();
}

void OdHelpParameter::setValue(const double &val)
{
  ((OdHelpParameterImpl*)m_pImpl)->setValue(val);
}

//////////////////////////////////////////////////////////////////////////////
OdCompositeConstraint::OdCompositeConstraint():OdGeomConstraint()
{
  
}

OdResult OdCompositeConstraint::getOwnedConstraints(OdArray<OdGeomConstraint*>& apSubConstraints) const
{
  return ((OdCompositeConstraintImpl*)m_pImpl)->getOwnedConstraints(apSubConstraints);
}
