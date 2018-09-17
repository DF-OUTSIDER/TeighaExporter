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

#include "OdHelpParameterImpl.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbFiler.h"

OdHelpParameterImpl::OdHelpParameterImpl():OdConstraintGroupNodeImpl(),
                                           m_value(0),
                                           m_UnkBool(false)
{
}
OdHelpParameterImpl::~OdHelpParameterImpl()
{
}

OdResult OdHelpParameterImpl::getConnectedGeometry(OdConstrainedGeometry*& pGeometry) const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  ODA_ASSERT(!pGr.isNull());
  OdConstraintGroupNode* pNode;
  int size = getConnectionQty();
  for ( int i = 0; i < size; i++ )
  {
    pNode = pGr->getGroupNodePtr(getConnectionAt(i));
    if ( pNode != NULL && pNode->isKindOf(OdConstrainedGeometry::desc()) )
    {
      pGeometry = (OdConstrainedGeometry*)pNode;
      return eOk;
    }
  }

  pGeometry = NULL;
  return eInvalidInput;
}

OdResult OdHelpParameterImpl::getConnectedConstraint(OdGeomConstraint*& pConstraint) const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  ODA_ASSERT(!pGr.isNull());
  OdConstraintGroupNode* pNode;
  int size = getConnectionQty();
  for ( int i = 0; i < size; i++ )
  {
    pNode = pGr->getGroupNodePtr(getConnectionAt(i));
    if ( pNode != NULL && pNode->isKindOf(OdGeomConstraint::desc()) )
    {
      pConstraint = (OdGeomConstraint*)pNode;
      return eOk;
    }
  }

  pConstraint = NULL;
  return eInvalidInput;
}

OdResult OdHelpParameterImpl::getConnectedEqualparamConstraints(OdArray<OdEqualHelpParameterConstraint*>& apEqualParamConstrs) const
{
  return eNotImplementedYet;
}

double OdHelpParameterImpl::getValue() const
{
  return m_value;
}

void OdHelpParameterImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstraintGroupNodeImpl::dwgOutFields(pFiler);
  pFiler->wrDouble(m_value);
  pFiler->wrBool(m_UnkBool);
}

OdResult OdHelpParameterImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstraintGroupNodeImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  m_value = pFiler->rdDouble();
  m_UnkBool = pFiler->rdBool();

  return res;
}

void OdHelpParameterImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstraintGroupNodeImpl::dxfOutFields(pFiler);
  pFiler->wrDouble(40, m_value);
  pFiler->wrBool(290, m_UnkBool);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdHelpParameterImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstraintGroupNodeImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(40)
  m_value = pFiler->rdDouble();

  NEXT_CODE(290)
  m_UnkBool = pFiler->rdBool();

  return res;
}

#undef NEXT_CODE

void OdHelpParameterImpl::setValue(const double &val)
{
  m_value = val;
}
