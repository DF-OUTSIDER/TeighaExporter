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

#include "OdGeomConstraintImpl.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbFiler.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

OdGeomConstraintImpl::OdGeomConstraintImpl():
                      OdConstraintGroupNodeImpl(),
                      m_enabled(true),
                      m_partOfComposite(false),
                      m_owningCompositeConstraint(OdConstraintGroupNode::kNullGroupNodeId),
                      m_implied(true),
                      m_active(true)
{
  m_Unk_UInt8 = 0;
}

OdGeomConstraintImpl::~OdGeomConstraintImpl()
{
  
}

OdResult OdGeomConstraintImpl::getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const
{
  OdResult res = eOk;
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  if (pGr.isNull())
    return eInvalidObjectId;
  for (OdGroupNodeIdArray::const_iterator i = m_connections.begin(); i != m_connections.end(); ++i)
  {
    if (OdConstraintGroupNode* n = pGr->getGroupNodePtr(*i))
    {
      if (n->isKindOf(OdConstrainedGeometry::desc()))
        apConsGeoms.append((OdConstrainedGeometry*)n);
    }
  }
  return eOk;
}

OdResult OdGeomConstraintImpl::getConnectedHelpParameters(OdArray<OdHelpParameter*>& apHelpParameters) const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  if (pGr.isNull())
    return eInvalidObjectId;
  for (unsigned int i = 0; i < getConnectionQty(); i++)
	{
    OdHelpParameterPtr helpParameter = OdHelpParameter::cast(pGr->getGroupNodePtr(getConnectionAt(i)));
    if (!helpParameter.isNull())
      apHelpParameters.push_back(helpParameter);
  }
  return eOk;
}

OdHelpParameter* OdGeomConstraintImpl::getConnectedHelpParameterFor(const OdConstrainedGeometry* pConsGeom) const
{
  OdArray<OdHelpParameter*> apHelpParameters;

  OdConstrainedGeometry *pConGeom;
  getConnectedHelpParameters(apHelpParameters);
  int i, size = apHelpParameters.length();
  for ( i = 0; i < size; i++ )
  {
    if ( apHelpParameters[i]->getConnectedGeometry(pConGeom) == eOk &&
         pConGeom->nodeId() == pConsGeom->nodeId() )
      return apHelpParameters[i];
  }

  return NULL;
}

OdCompositeConstraint* OdGeomConstraintImpl::getOwningCompositeConstraint() const
{
  if ( !m_partOfComposite )
    return NULL;

  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  if (pGr.isNull())
  {
    ODA_FAIL_ONCE();
    return NULL;
  }
  OdConstraintGroupNode *node = pGr->getGroupNodePtr(m_owningCompositeConstraint);
  if ( node != NULL && node->isKindOf(OdCompositeConstraint::desc()) )
    return (OdCompositeConstraint*)node;
  else
    return NULL;
}

OdResult OdGeomConstraintImpl::setOwningCompositeConstraintId(const OdConstraintGroupNodeId id)
{
  m_owningCompositeConstraint = id;
  m_partOfComposite = m_owningCompositeConstraint != OdConstraintGroupNode::kNullGroupNodeId;

  return eOk;
}

OdResult OdGeomConstraintImpl::deactivate()
{
  m_active = false;
  return eOk;
}

OdResult OdGeomConstraintImpl::reactivate()
{
  m_active = true;
  return eOk;
}

bool OdGeomConstraintImpl::isActive() const
{
  return m_active;
}

bool OdGeomConstraintImpl::isImplied() const
{
  return m_implied;
}

bool OdGeomConstraintImpl::isInternal() const
{
  return m_implied | m_partOfComposite;
}

bool OdGeomConstraintImpl::isEnabled() const
{
  return m_enabled;
}

void OdGeomConstraintImpl::setImplied(bool implied)
{
  m_implied = implied;
}

void OdGeomConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstraintGroupNodeImpl::dwgOutFields(pFiler);
  pFiler->wrInt32(m_owningCompositeConstraint);
  pFiler->wrBool(m_implied);
  pFiler->wrBool(m_active);
}


OdResult OdGeomConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstraintGroupNodeImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  m_owningCompositeConstraint = pFiler->rdInt32();
  m_partOfComposite = m_owningCompositeConstraint != OdConstraintGroupNode::kNullGroupNodeId;

  m_implied = pFiler->rdBool();
  m_active = pFiler->rdBool();

  return eOk;
}

void OdGeomConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstraintGroupNodeImpl::dxfOutFields(pFiler);
  pFiler->wrInt32(90, m_owningCompositeConstraint);
  pFiler->wrBool(290, m_implied);
  pFiler->wrBool(290, m_active);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdGeomConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstraintGroupNodeImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  NEXT_CODE(90)
  m_owningCompositeConstraint = pFiler->rdInt32();
  m_partOfComposite = m_owningCompositeConstraint != OdConstraintGroupNode::kNullGroupNodeId;

  NEXT_CODE(290)
  m_implied = pFiler->rdBool();
  NEXT_CODE(290)
  m_active = pFiler->rdBool();
  return eOk;
}

#undef NEXT_CODE
