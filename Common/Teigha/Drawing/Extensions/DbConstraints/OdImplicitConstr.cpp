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
#include "OdImplicitConstr.h"
#include "DbFiler.h"
#include "RxObjectImpl.h"

#include "OdCompositeConstraintImpl.h"
#include "OdParallelConstraintImpl.h"
//#include "OdConstrainedGeometry.h"

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdPerpendicularConstraint, OdGeomConstraint,
                                 L"AcPerpendicularConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdNormalConstraint, OdGeomConstraint,
                                 L"AcNormalConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdPointCurveConstraint, OdGeomConstraint,
                                 L"AcPointCurveConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdColinearConstraint, OdGeomConstraint,
                                 L"AcColinearConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdPointCoincidenceConstraint, OdGeomConstraint,
                                 L"AcPointCoincidenceConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdConcentricConstraint, OdGeomConstraint,
                                 L"AcConcentricConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdCenterPointConstraint, OdConcentricConstraint,
                                 L"AcCenterPointConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdTangentConstraint, OdGeomConstraint,
                                 L"AcTangentConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdEqualRadiusConstraint, OdGeomConstraint,
                                 L"AcEqualRadiusConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdEqualDistanceConstraint, OdGeomConstraint,
                                 L"AcEqualDistanceConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdEqualLengthConstraint, OdGeomConstraint,
                                 L"AcEqualLengthConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdParallelConstraint, OdGeomConstraint,
                                 L"AcParallelConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdHorizontalConstraint, OdParallelConstraint,
                                 L"AcHorizontalConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdVerticalConstraint, OdParallelConstraint,
                                 L"AcVerticalConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdEqualCurvatureConstraint, OdGeomConstraint,
                                 L"AcEqualCurvatureConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdSymmetricConstraint, OdGeomConstraint,
                                 L"AcSymmetricConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdMidPointConstraint, OdGeomConstraint,
                                 L"AcMidPointConstraint", RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdFixedConstraint, OdGeomConstraint,
                                 L"AcFixedConstraint", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdG2SmoothConstraint, OdCompositeConstraint,
                                 L"AcG2SmoothConstraint", RXIMPL_CONSTR);
#if 0 // TODO:
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdEqualHelpParameterConstraint, OdGeomConstraint,
                                 L"AcEqualHelpParameterConstraint", RXIMPL_CONSTR);

OdEqualHelpParameterConstraint::OdEqualHelpParameterConstraint()
OdResult OdEqualHelpParameterConstraint::getEqualHelpParameters(OdHelpParameter*& pHelpParameter1, OdHelpParameter*& pHelpParameter2) const
#endif

OdPerpendicularConstraint::OdPerpendicularConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdNormalConstraint::OdNormalConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdPointCurveConstraint::OdPointCurveConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdColinearConstraint::OdColinearConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdPointCoincidenceConstraint::OdPointCoincidenceConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdConcentricConstraint::OdConcentricConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdTangentConstraint::OdTangentConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdEqualRadiusConstraint::OdEqualRadiusConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdEqualDistanceConstraint::OdEqualDistanceConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdEqualLengthConstraint::OdEqualLengthConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdParallelConstraint::OdParallelConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

OdParallelConstraint::OdParallelConstraint(bool createImpl):OdGeomConstraint()
{
  if ( createImpl )
    m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdHorizontalConstraint::OdHorizontalConstraint():OdParallelConstraint(false)
{
  m_pImpl = new OdParallelConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdVerticalConstraint::OdVerticalConstraint():OdParallelConstraint(false)
{
  m_pImpl = new OdParallelConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdEqualCurvatureConstraint::OdEqualCurvatureConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdSymmetricConstraint::OdSymmetricConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdMidPointConstraint::OdMidPointConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdFixedConstraint::OdFixedConstraint():OdGeomConstraint()
{
  m_pImpl = new OdGeomConstraintImpl();
}

//////////////////////////////////////////////////////////////////////////////

OdG2SmoothConstraint::OdG2SmoothConstraint():OdCompositeConstraint()
{
  m_pImpl = new OdCompositeConstraintImpl();
}

