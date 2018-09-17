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
#include "DbAssocArrayItem.h"
#include "DbImpAssocArrayParameters.h"

//////////////////////////////////////////////////////////////////////////

OdDbItemLocator::OdDbItemLocator()
{
  m_Indices[kItemIndex] = 0;
  m_Indices[kRowIndex] = 0;
  m_Indices[kLevelIndex] = 0;
}

OdDbItemLocator::OdDbItemLocator(int itemIndex, int rowIndex, int levelIndex)
{
  m_Indices[kItemIndex] = itemIndex;
  m_Indices[kRowIndex] = rowIndex;
  m_Indices[kLevelIndex] = levelIndex;
}

OdDbItemLocator::OdDbItemLocator(const OdDbItemLocator& other)
{
  OdDbItemLocator::operator =(other);
}

int OdDbItemLocator::operator [](IndexType i) const
{
  return m_Indices[i];
}

OdDbItemLocator& OdDbItemLocator::operator =(const OdDbItemLocator& other)
{
  if (this == &other)
    return *this;

  m_Indices[kItemIndex] = other.m_Indices[kItemIndex];
  m_Indices[kRowIndex] = other.m_Indices[kRowIndex];
  m_Indices[kLevelIndex] = other.m_Indices[kLevelIndex];
  return *this;
}

OdDbItemLocator OdDbItemLocator::operator -(const OdDbItemLocator& other) const
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
}

OdDbItemLocator OdDbItemLocator::operator +(const OdDbItemLocator& other) const
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
}

bool OdDbItemLocator::operator ==(const OdDbItemLocator& other) const
{
  return other.m_Indices[0] == this->m_Indices[0]
      && other.m_Indices[1] == this->m_Indices[1]
      && other.m_Indices[2] == this->m_Indices[2];
}

bool OdDbItemLocator::operator !=(const OdDbItemLocator& other) const
{
  return !this->operator==(other);
}

bool OdDbItemLocator::operator <(const OdDbItemLocator& other) const
{
  return compareWith(other) < 0; 
}

bool OdDbItemLocator::operator >(const OdDbItemLocator& other) const
{ 
  return compareWith(other) > 0; 
}

int OdDbItemLocator::compareWith(const OdDbItemLocator& other) const
{
  int ve = this->m_Indices[kItemIndex] - other.m_Indices[kItemIndex];
  if ( ve )
    return ve;

  ve = this->m_Indices[kRowIndex] - other.m_Indices[kRowIndex];
  if ( ve )
    return ve;

  ve = this->m_Indices[kLevelIndex] - other.m_Indices[kLevelIndex];
  return ve;
}

//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocArrayItem, OdRxObject, L"AcDbAssocArrayItem", RXIMPL_CONSTR)

OdDbAssocArrayItem::OdDbAssocArrayItem()
  : m_pImpl(new OdDbImpAssocArrayItem())
{
}

OdDbAssocArrayItem::OdDbAssocArrayItem(OdDbItemLocator& locator, const OdGeMatrix3d& transform)
  : m_pImpl(NULL)
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
}


OdSmartPtr<OdDbAssocArrayItem> OdDbAssocArrayItem::createObject( OdDbItemLocator& locator, const OdGeMatrix3d& transform )
{
  OdDbAssocArrayItemPtr res = OdDbAssocArrayItem::createObject();
  res->m_pImpl->m_locator = locator;
  res->m_pImpl->m_matrix = transform;
  return res;
}

OdSmartPtr<OdDbAssocArrayItem> OdDbAssocArrayItem::createObject( const OdDbAssocArrayItem *pItem, OdDbObjectId modifyBodyId )
{
  OdDbAssocArrayItemPtr res = OdDbAssocArrayItem::createObject();
  res->m_pImpl->m_locator = pItem->locator();
  res->setEntityProvider( pItem->entityProvider() );
  pItem->getTransform( res->m_pImpl->m_matrix, false );
  res->m_pImpl->m_repBodyId = modifyBodyId;
  return res;
}

OdDbAssocArrayItem::~OdDbAssocArrayItem()
{
  if (m_pImpl)
    delete m_pImpl;
}

OdResult OdDbAssocArrayItem::getTransform(OdGeMatrix3d& transform,
                                          bool bCompounded) const // = true
{
  return m_pImpl->getTransform( transform, bCompounded );
}

bool OdDbAssocArrayItem::hasRelativeTransform() const
{
  return m_pImpl->hasRelativeTransform();
}

OdResult OdDbAssocArrayItem::transformBy(const OdGeMatrix3d& transform)
{
  return m_pImpl->transformBy( transform );
}

OdResult OdDbAssocArrayItem::setDefaultTransform(const OdGeMatrix3d& transform)
{
  return m_pImpl->setDefaultTransform( transform );
}

OdResult OdDbAssocArrayItem::setErased(bool bErased)
{
  return m_pImpl->setErased( bErased );
}

bool OdDbAssocArrayItem::isErased() const
{
  ODA_ASSERT_ONCE(m_pImpl);
  return m_pImpl->bErased;
}

const OdDbItemLocator& OdDbAssocArrayItem::locator() const
{
  ODA_ASSERT_ONCE(m_pImpl);
  return m_pImpl->m_locator;
}

const OdDbObjectId& OdDbAssocArrayItem::entityProvider() const
{
  ODA_ASSERT_ONCE(m_pImpl);
  return m_pImpl->m_entityProviderId;
}

void OdDbAssocArrayItem::setEntityProvider(OdDbObjectId provider)
{
  ODA_ASSERT_ONCE(m_pImpl);
  m_pImpl->m_entityProviderId = provider; 
}

void OdDbAssocArrayItem::copyFrom(const OdRxObject* other)
{
  ODA_ASSERT_ONCE(m_pImpl);
  OdDbAssocArrayItemPtr src = OdDbAssocArrayItem::cast(other);
  if (src.isNull())
  {
    ODA_FAIL_ONCE();
  }

  m_pImpl->m_entityProviderId     = src->m_pImpl->m_entityProviderId;
  m_pImpl->m_locator              = src->m_pImpl->m_locator;
  m_pImpl->m_matrix               = src->m_pImpl->m_matrix; 
  m_pImpl->m_relative             = src->m_pImpl->m_relative;
  m_pImpl->m_repBodyId            = src->m_pImpl->m_repBodyId;
}

OdResult OdDbAssocArrayItem::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpl);
  if (!m_pImpl)
    return eMakeMeProxy;
  return m_pImpl->dwgInFields(pFiler);
}

void OdDbAssocArrayItem::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  ODA_ASSERT_ONCE(m_pImpl);
  if (!m_pImpl)
    throw OdError(eNotImplementedYet);
  m_pImpl->dwgOutFields(pFiler);
}

OdResult OdDbAssocArrayItem::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpl);
  if (!m_pImpl)
    return eMakeMeProxy;
  return m_pImpl->dxfInFields(pFiler);
}

void OdDbAssocArrayItem::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  ODA_ASSERT_ONCE(m_pImpl);
  if (!m_pImpl)
    throw OdError(eNotImplementedYet);
  m_pImpl->dxfOutFields(pFiler);
}
