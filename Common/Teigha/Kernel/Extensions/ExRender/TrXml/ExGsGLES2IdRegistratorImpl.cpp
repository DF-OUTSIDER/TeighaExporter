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
// GLES2 device rendition Id registrator implementation

#include "OdaCommon.h"
#include "Gs/GsFiler.h"
#include "TrVisIdMap.h"
#include "ExGsGLES2IdRegistratorImpl.h"

#define DEBUG_IDREGISTRATOR

OdGLES2RegistratorImpl::OdGLES2RegistratorImpl()
{
}

OdGLES2RegistratorImpl::~OdGLES2RegistratorImpl()
{
  killAll();
}

// Registrator methods

void OdGLES2RegistratorImpl::reg(OdTrVisRendition::DataType type, OdTrVisId id, OdDbStub *pPersId)
{
  if (pPersId)
  {
    OdTrVisUniqueID uId;
    uId.setId(pPersId); uId.setPersistent();
    uId.setData(m_linkMap[type][RegType(uId)].m_nLowFreeId);
    reg(type, id, uId);
  }
}

void OdGLES2RegistratorImpl::reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdGiDrawable *pTransId)
{
  OdTrVisUniqueID uId;
  uId.setId(pTransId); uId.setTransient();
  uId.setData(m_linkMap[type][RegType(uId)].m_nLowFreeId);
  reg(type, id, uId);
}

void OdGLES2RegistratorImpl::reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdTrVisUniqueID &uId)
{
  OdTrVisUniqueID &re = m_regMap[type][id];
#ifdef DEBUG_IDREGISTRATOR
  ODA_ASSERT_ONCE(!re.isValid());
#endif
  re = uId;
  // Reg data id
  RegLink &rl = m_linkMap[type][RegType(re)];
#ifdef DEBUG_IDREGISTRATOR
  ODA_ASSERT_ONCE(rl.m_linkIds.find(re.getData()) == rl.m_linkIds.end());
  ODA_ASSERT_ONCE(re.getData() >= rl.m_nLowFreeId);
#endif
  IdSet::iterator it = rl.m_linkIds.insert(re.getData()).first;
  if (re.getData() <= rl.m_nLowFreeId)
  {
    OdUInt64 counter = re.getData();
    do {
      if (counter == *it)
        counter++;
      else
        break;
      it++;
    } while (it != rl.m_linkIds.end());
    rl.m_nLowFreeId = counter;
  }
}

void OdGLES2RegistratorImpl::killImpl(OdTrVisRendition::DataType type, OdTrVisId id) const
{
  RegMap::iterator itReg = m_regMap[type].find(id);
#ifdef DEBUG_IDREGISTRATOR
  ODA_ASSERT_ONCE(itReg != m_regMap[type].end());
#endif
  if (itReg != m_regMap[type].end())
  {
    LinkMap::iterator itLink = m_linkMap[type].find(RegType(itReg->second));
    ODA_ASSERT_ONCE(itLink != m_linkMap[type].end());
    if (itLink != m_linkMap[type].end())
    {
      RegLink &rl = itLink->second;
      rl.m_linkIds.erase(itReg->second.getData());
      if (rl.isEmpty())
        m_linkMap[type].erase(itLink);
      else
      {
        if (itReg->second.getData() < rl.m_nLowFreeId)
          rl.m_nLowFreeId = itReg->second.getData();
      }
    }
    m_regMap[type].erase(itReg);
  }
}

void OdGLES2RegistratorImpl::kill(OdTrVisRendition::DataType type, OdTrVisId id)
{
  killImpl(type, id);
}

OdTrVisUniqueID *OdGLES2RegistratorImpl::get(OdTrVisRendition::DataType type, OdTrVisId id, bool bKill) const
{
  if (!id)
    return NULL;
  RegMap::iterator it = m_regMap[type].find(id);
  if (it != m_regMap[type].end())
  {
    if (!bKill)
      return &it->second;
    m_tmpId = it->second;
    killImpl(type, id);
    return &m_tmpId;
  }
  return NULL;
}

void OdGLES2RegistratorImpl::killAll()
{
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNDataTypes; nDataType++)
  {
    m_regMap[nDataType].clear();
    m_linkMap[nDataType].clear();
  }
}

void OdGLES2RegistratorImpl::traverse(Traverser *pTraverser)
{
  if (!pTraverser) return;
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNDataTypes; nDataType++)
  {
    RegMap::iterator it = m_regMap[nDataType].begin();
    while (it != m_regMap[nDataType].end())
    {
      if (!pTraverser->idRegTraverseCallback((OdTrVisRendition::DataType)nDataType, it->first, &it->second))
        return;
      it++;
    }
  }
}

bool OdGLES2RegistratorImpl::save(OdGsFiler *pFiler) const
{
  // Save only persistent and utilitary id's
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNDataTypes; nDataType++)
  {
    RegMap::const_iterator itReg = m_regMap[nDataType].begin();
    while (itReg != m_regMap[nDataType].end())
    {
      if (!itReg->second.isTransient())
      {
        pFiler->wrBool(true);
        pFiler->wrUInt64((OdUInt64)itReg->first);
        pFiler->wrUInt32(itReg->second.getType());
        if (itReg->second.isPersistent())
          pFiler->wrHandle(itReg->second.persistentId());
        else
          pFiler->wrUInt64(itReg->second.getId());
        pFiler->wrUInt64(itReg->second.getData());
      }
      itReg++;
    }
    pFiler->wrBool(false);
    LinkMap::const_iterator itLink = m_linkMap[nDataType].begin();
    while (itLink != m_linkMap[nDataType].end())
    {
      OdTrVisUniqueID uId = itLink->first.asUID();
      if (!uId.isTransient())
      {
        pFiler->wrBool(true);
        pFiler->wrUInt32(uId.getType());
        if (uId.isPersistent())
          pFiler->wrHandle(uId.persistentId());
        else
          pFiler->wrUInt64(uId.getId());
        pFiler->wrUInt64(itLink->second.m_nLowFreeId);
        pFiler->wrUInt32((OdUInt32)itLink->second.m_linkIds.size());
        IdSet::const_iterator itSet = itLink->second.m_linkIds.begin();
        while (itSet != itLink->second.m_linkIds.end())
        {
          pFiler->wrUInt64(*itSet);
          itSet++;
        }
      }
      itLink++;
    }
    pFiler->wrBool(false);
  }
  return true;
}

bool OdGLES2RegistratorImpl::load(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
{
  for (OdUInt32 nDataType = 0; nDataType < OdTrVisRendition::kNDataTypes; nDataType++)
  {
    while (pFiler->rdBool())
    {
      OdTrVisId id = (OdTrVisId)pFiler->rdUInt64();
      pIdMap->get((OdTrVisRendition::DataType)nDataType, id, id);
      OdTrVisUniqueID uID;
      uID.setUtilitary(pFiler->rdUInt32());
      if (uID.isPersistent())
        uID.setId(pFiler->rdHandle());
      else
        uID.setId(pFiler->rdUInt64());
      uID.setData(pFiler->rdUInt64());
      m_regMap[nDataType][id] = uID;
    }
    while (pFiler->rdBool())
    {
      OdTrVisUniqueID uID;
      uID.setUtilitary(pFiler->rdUInt32());
      if (uID.isPersistent())
        uID.setId(pFiler->rdHandle());
      else
        uID.setId(pFiler->rdUInt64());
      RegLink &rl = m_linkMap[nDataType][RegType(uID)];
      rl.m_nLowFreeId = pFiler->rdUInt64();
      const OdUInt32 nSets = pFiler->rdUInt32();
      for (OdUInt32 nSet = 0; nSet < nSets; nSet++)
        rl.m_linkIds.insert(pFiler->rdUInt64());
    }
  }
  return true;
}

//
