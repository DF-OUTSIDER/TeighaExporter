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

#include "OdConstraintGroupNodeImpl.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbFiler.h"

OdConstraintGroupNodeImpl::OdConstraintGroupNodeImpl():
                            m_Id(OdConstraintGroupNode::kNullGroupNodeId),
                            m_Unk_UInt8(0)
{
}

OdConstraintGroupNodeImpl::~OdConstraintGroupNodeImpl()
{
}

OdResult OdConstraintGroupNodeImpl::setOwnerAction(OdDbObjectId id)
{
  m_groupObjectId = id;
  return eOk;
}

OdResult OdConstraintGroupNodeImpl::addConnection(OdConstraintGroupNodeId id)
{
  m_connections.push_back(id);
  return eOk;
}

OdResult OdConstraintGroupNodeImpl::removeConnection(OdConstraintGroupNodeId id, bool &isNoConnection)
{
  for ( unsigned int i = 0; i <  m_connections.length(); i++ )
    if ( m_connections[i] == id )
    {
      m_connections.removeAt(i);
      isNoConnection = m_connections.length() == 0;
      return eOk;
    }

    return eBadObjType;
}

unsigned int OdConstraintGroupNodeImpl::getConnectionQty() const
{
  return m_connections.length();
}

OdConstraintGroupNodeId OdConstraintGroupNodeImpl::getConnectionAt(int idx) const
{
  return m_connections[idx];
}

OdConstraintGroupNodeId OdConstraintGroupNodeImpl::nodeId() const
{
  return m_Id;
}

void OdConstraintGroupNodeImpl::setNodeId(OdConstraintGroupNodeId val)
{
  m_Id = val;
}

OdDbObjectId OdConstraintGroupNodeImpl::owningConstraintGroupId()const
{
  return m_groupObjectId;
}

void OdConstraintGroupNodeImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt32(m_Id);

  if (pFiler->dwgVersion() < OdDb::vAC27)
    pFiler->wrUInt8(m_Unk_UInt8);

  OdUInt32 nLinks = m_connections.size();
  pFiler->wrInt32(nLinks);
  const OdConstraintGroupNodeId* pConnection = m_connections.asArrayPtr();
  while (nLinks--)
  {
    pFiler->wrInt32(*pConnection++);
  }

  if (pFiler->dwgVersion() >= OdDb::vAC27)
    pFiler->wrUInt8(m_Unk_UInt8);
}

OdResult OdConstraintGroupNodeImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  if (pFiler->dwgVersion() < OdDb::vAC27)
  {
    if (!m_Id)
    {
      OdConstraintGroupNodeId id = pFiler->rdInt32();
      m_Id = id;
      ODA_ASSERT_ONCE(m_Id);
      m_Unk_UInt8 = pFiler->rdUInt8();
    }
#ifdef _DEBUG
    else
      // marker for : version of parent object is 0
      // (intermediate R24 version)
      // dataFiles/dwg/ACADSamples/2010/architectural_example-imperial.dwg
      m_Id = m_Id; // brk
#endif
  }
  else
  {
    OdConstraintGroupNodeId id = pFiler->rdInt32();
    if (id != m_Id) // m_Id was already defined
    {
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }
  }

  OdUInt32 nLinks = pFiler->rdInt32();
  m_connections.resize(nLinks);
  OdConstraintGroupNodeId* pConnection = m_connections.asArrayPtr();
  while (nLinks--)
  {
    *pConnection++ = pFiler->rdInt32();
  }
  
  if (pFiler->dwgVersion() >= OdDb::vAC27)
    m_Unk_UInt8 = pFiler->rdUInt8();

  return eOk;
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

void OdConstraintGroupNodeImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  if (pFiler->dwgVersion() <= OdDb::vAC24)
  {
    pFiler->wrObjectId(330, m_groupObjectId);
    pFiler->wrUInt32(90, m_Id);
    pFiler->wrInt16(70, m_Unk_UInt8);
  }
  else
  {
    pFiler->wrUInt32(90, m_Id);
  }
  OdUInt32 nLinks = m_connections.size();
  pFiler->wrInt32(90, nLinks);
  const OdConstraintGroupNodeId* pConnection = m_connections.asArrayPtr();
  while (nLinks--)
  {
    pFiler->wrUInt32(90, *pConnection++);
  }
  if (pFiler->dwgVersion() > OdDb::vAC24)
  {
    pFiler->wrInt16(70, m_Unk_UInt8);
  }
}

OdResult OdConstraintGroupNodeImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  if (pFiler->dwgVersion() <= OdDb::vAC24)
  {
    NEXT_CODE(330)
    m_groupObjectId = pFiler->rdObjectId();
    NEXT_CODE(90)
    m_Id = pFiler->rdUInt32();
    NEXT_CODE(70)
    m_Unk_UInt8 = (OdUInt8)pFiler->rdUInt16();// This value seems to be 0 for OdGeomConstraint and 1 for OdConstrainedGeometry
  }
  else
  {
    NEXT_CODE(90)
    if (m_Id != pFiler->rdUInt32())
    {
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }
  }
  NEXT_CODE(90)
  OdUInt32 nLinks = pFiler->rdInt32();
  m_connections.resize(nLinks);
  OdConstraintGroupNodeId* pConnection = m_connections.asArrayPtr();
  while (nLinks--)
  {
    NEXT_CODE(90)
    *pConnection++ = pFiler->rdInt32();
  }

  if (pFiler->dwgVersion() > OdDb::vAC24)
  {
    NEXT_CODE(70)
    m_Unk_UInt8 = (OdUInt8)pFiler->rdUInt16();// This value seems to be 0 for OdGeomConstraint and 1 for OdConstrainedGeometry
  }

  return eOk;
}

#undef NEXT_CODE
