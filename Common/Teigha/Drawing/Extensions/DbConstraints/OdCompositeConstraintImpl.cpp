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

#include "OdCompositeConstraintImpl.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbImpAssoc2dConstraintGroup.h"
#include "DbFiler.h"

OdCompositeConstraintImpl::OdCompositeConstraintImpl():OdGeomConstraintImpl()
{
}

OdCompositeConstraintImpl::~OdCompositeConstraintImpl()
{
}

OdResult OdCompositeConstraintImpl::getOwnedConstraints(OdArray<OdGeomConstraint*>& apSubConstraints) const
{
  unsigned int size = m_ownedConstraints.length();
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdGeomConstraint* pConst;
  for ( unsigned int i = 0; i < size; i++ )
  {
    pConst = OdGeomConstraint::cast(pGr->getGroupNodePtr(m_ownedConstraints[i]));
    if (pConst)
      apSubConstraints.push_back(pConst);
  }

  return eOk;
}

OdResult OdCompositeConstraintImpl::addOwnedConstraint(const OdGeomConstraint* pCnstr)
{
  m_ownedConstraints.append(pCnstr->nodeId());
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);

  ((OdGeomConstraintImpl*)pGrNodeImpl)->setOwningCompositeConstraintId(nodeId());

  return eOk;
}

OdResult OdCompositeConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdGeomConstraintImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  int len = pFiler->rdInt32();
  m_ownedConstraints.resize(len);
  OdConstraintGroupNodeId *pConstr = m_ownedConstraints.asArrayPtr();
  while ( len-- )
    *pConstr++ = pFiler->rdInt32();

  return res;
}

void OdCompositeConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdGeomConstraintImpl::dwgOutFields(pFiler);

  int len = m_ownedConstraints.size();
  pFiler->wrInt32(len);

  const OdConstraintGroupNodeId* pConstr = m_ownedConstraints.asArrayPtr();
  while ( len-- )
    pFiler->wrInt32(*pConstr++);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdCompositeConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdGeomConstraintImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(90)
  int len = pFiler->rdInt32();
  m_ownedConstraints.resize(len);
  OdConstraintGroupNodeId *pConstr = m_ownedConstraints.asArrayPtr();
  while ( len-- )
  {
    NEXT_CODE(90)
    *pConstr++ = pFiler->rdInt32();
  }
  return res;
}

#undef NEXT_CODE

void OdCompositeConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdGeomConstraintImpl::dxfOutFields(pFiler);

  int len = m_ownedConstraints.size();
  pFiler->wrInt32(90, len);

  const OdConstraintGroupNodeId* pConstr = m_ownedConstraints.asArrayPtr();
  while ( len-- )
    pFiler->wrInt32(90, *pConstr++);
}
