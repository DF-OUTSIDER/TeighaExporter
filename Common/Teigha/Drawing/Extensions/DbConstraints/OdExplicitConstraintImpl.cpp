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

#include "DbAssocValueDependency.h"
#include "OdExplicitConstraintImpl.h"
#include "DbFiler.h"

OdExplicitConstraintImpl::OdExplicitConstraintImpl():OdGeomConstraintImpl()
{
  m_implied = false;
}

OdExplicitConstraintImpl::~OdExplicitConstraintImpl()
{
}

OdDbObjectId OdExplicitConstraintImpl::valueDependencyId() const
{
   return m_valueDependencyId;
}

OdDbObjectId OdExplicitConstraintImpl::dimDependencyId() const
{
  return m_dimDependencyId;
}

OdResult OdExplicitConstraintImpl::setValueDependencyId(const OdDbObjectId& valueDependencyId)
{
  m_valueDependencyId = valueDependencyId;
  return eOk;
}

OdResult OdExplicitConstraintImpl::setDimDependencyId(const OdDbObjectId& dimDependencyId)
{
  m_dimDependencyId = dimDependencyId;
  return eOk;
}

OdResult OdExplicitConstraintImpl::getMeasuredValue(double& val) const
{
  OdDbObjectId valueDepId = valueDependencyId();
  if ( valueDepId.isNull() )
    return eWrongObjectType;

  OdDbAssocValueDependencyPtr pValueDependency = valueDepId.openObject();
  OdDbEvalVariantPtr pVal = OdDbEvalVariant::createObject();
  pValueDependency->getDependentOnObjectValue(pVal);
  switch ( pVal->getType() )
  {
    case kDwgReal:
      double dVal;
      pVal->getValue(dVal);
      val = dVal;
    break;
    case kDwgInt32:
      int IVal;
      pVal->getValue(IVal);
      val = IVal;
    break;
    default:
      ODA_FAIL();
  }
  return eOk;
}

OdResult OdExplicitConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdGeomConstraintImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  m_valueDependencyId = pFiler->rdHardPointerId();
  m_dimDependencyId = pFiler->rdHardPointerId();

  return eOk;
}

void OdExplicitConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdGeomConstraintImpl::dwgOutFields(pFiler);
  pFiler->wrHardPointerId(m_valueDependencyId);
  pFiler->wrHardPointerId(m_dimDependencyId);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdExplicitConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdGeomConstraintImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  NEXT_CODE(340)
  m_valueDependencyId = pFiler->rdObjectId();
  NEXT_CODE(340)
  m_dimDependencyId = pFiler->rdObjectId();

  return eOk;
}

void OdExplicitConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdGeomConstraintImpl::dxfOutFields(pFiler);
  pFiler->wrObjectId(340, valueDependencyId());
  pFiler->wrObjectId(340, dimDependencyId());
}

#undef NEXT_CODE
