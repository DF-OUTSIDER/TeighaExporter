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
#include "DbAssocPersSubentId.h"
#include "OdDbAssocPersSubentIdPE.h"
#include "DbFiler.h"
#include "RxObjectImpl.h"
#include "DbEntity.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocPersSubentId, OdRxObject, L"AcDbAssocPersSubentId");

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocSingleEdgePersSubentId, OdDbAssocPersSubentId,
                                 L"AcDbAssocSingleEdgePersSubentId", RXIMPL_CONSTR);
                                   
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocSimplePersSubentId, OdDbAssocPersSubentId,
                                 L"AcDbAssocSimplePersSubentId", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocIndexPersSubentId, OdDbAssocPersSubentId,
                                 L"AcDbAssocIndexPersSubentId", RXIMPL_CONSTR);

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocAsmBasedEntityPersSubentId, OdDbAssocPersSubentId,
                                 L"AcDbAssocAsmBasedEntityPersSubentId", RXIMPL_CONSTR);


OdResult OdDbAssocPersSubentId::getTransientSubentIds(const OdDbEntity* pEntity, 
                                        OdArray<OdDbSubentId>& subents) const
{
  OdDbAssocPersSubentIdPEPtr pPE =  pEntity->isA()->getX( OdDbAssocPersSubentIdPE::desc() );
  if (!pPE.isNull())
    return pPE->getTransientSubentIds(pEntity, this, subents);

  ODA_FAIL();
  //throw OdError(eNotImplementedYet);
  return eNotImplementedYet;
}

int OdDbAssocPersSubentId::transientSubentCount(const OdDbEntity* pEntity) const
{
  OdArray<OdDbSubentId> arrSubEnts;
  if (getTransientSubentIds(pEntity, arrSubEnts) == eOk)
    return arrSubEnts.size();
  return 0;
}

OdResult OdDbAssocPersSubentId::mirror(const OdDbEntity* /*pMirroredEntity*/)
{
/*
  OdDbAssocPersSubentIdPEPtr pPE = pMirrorredEntity;
  if (!pPE.isNull())
    return pPE->mirrorPersSubent(pMirrorredEntity, *this);
*/
  ODA_FAIL();
  //throw OdError(eNotImplementedYet);
  return eNotImplementedYet;
}

OdResult OdDbAssocPersSubentId::createObjectAndDwgInFields(OdDbDwgFiler* pFiler, OdDbAssocPersSubentIdPtr& pCreatedPersSubentId)
{
  bool bClassByName = true;
  if (pFiler->dwgVersion() >= OdDb::vAC27)
  {
    bClassByName = pFiler->rdBool();
    if (bClassByName)
    {
      ODA_FAIL_ONCE(); // TODO check in debugger
      return eMakeMeProxy;
    }
  }

  OdString strClass;
  if (bClassByName)
    strClass = pFiler->rdString();
  else
  {
    OdInt32 index = pFiler->rdInt32();
    switch (index)
    {
    case 1:
      // see also OdDbAssocSingleEdgePersSubentId::dwgOutFields
      strClass = L"AcDbAssocSingleEdgePersSubentId";
      break;
    case 2:
      // see also OdDbAssocSimplePersSubentId::dwgOutFields
      strClass = L"AcDbAssocSimplePersSubentId";
      break;
    case 3:
      // see also OdDbAssocEdgePersSubentId::dwgOutFields
      strClass = L"AcDbAssocEdgePersSubentId";
      break;
    case 4:
      strClass = L"AcDbAssocIndexPersSubentId";
      break;
    case 5:
      strClass = L"AcDbAssocAsmBasedEntityPersSubentId";
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }
  }

  pCreatedPersSubentId = ::odrxCreateObject(strClass);
  if (pCreatedPersSubentId.isNull())
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }
  return pCreatedPersSubentId->dwgInFields(pFiler);
}

OdResult OdDbAssocPersSubentId::createObjectAndDxfInFields(OdDbDxfFiler* pFiler, OdDbAssocPersSubentIdPtr& pCreatedPersSubentId)
{
  int nNextCode = pFiler->nextItem();
  if (nNextCode != 1)
    return eInvalidGroupCode;

  OdString strClass = pFiler->rdString();
  pCreatedPersSubentId = ::odrxCreateObject(strClass);
  if (pCreatedPersSubentId.isNull())
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  return pCreatedPersSubentId->dxfInFields(pFiler);
}

void OdDbAssocPersSubentId::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  //in accordance with ObjectArx this function does nothing
  /*
  pFiler->wrBool(isNull());
// TODO: Filers except File & Id ones save some int16 here
//  pFiler->wrInt16(0);        // TODO
*/
}

OdResult OdDbAssocPersSubentId::dwgInFields(OdDbDwgFiler* pFiler)
{
  //in accordance with ObjectArx this function does nothing
/*
  bool bCachePresents = pFiler->rdBool();
  if (bCachePresents)
  {
    ODA_FAIL();
    // TODO: implement cache reading and storing
  }
// TODO: Filers except File & Id ones save some int16 here
//  int nUnk = pFiler->rdInt16();
//  ODA_ASSERT(nUnk == 0);
*/
  return eOk;
}

void OdDbAssocPersSubentId::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  //in accordance with ObjectArx this function does nothing
  /*
  pFiler->wrBool(290, isNull());
  */
}
OdResult OdDbAssocPersSubentId::dxfInFields(OdDbDxfFiler* pFiler)
{
  //in accordance with ObjectArx this function does nothing
  /*
  if (pFiler->nextItem() != 290)
    return eInvalidGroupCode;

  bool bCachePresents = pFiler->rdBool();
  if (bCachePresents)
  {
    ODA_FAIL();
    // TODO: implement cache reading and storing
  }
  */
  return eOk;
}

//----------------------------------------------------------------------------

OdDbAssocSingleEdgePersSubentId::OdDbAssocSingleEdgePersSubentId():OdDbAssocPersSubentId(),
                                                                   m_Unk_Bool_1(false)
{
}
#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

void OdDbAssocSingleEdgePersSubentId::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  if (pFiler->dwgVersion() < OdDb::vAC27)
    pFiler->wrString(desc()->name());
  else
  {
    pFiler->wrBool(false);
    pFiler->wrInt32(1);
  }

  pFiler->wrBool(m_Unk_Bool_1);
}

OdResult OdDbAssocSingleEdgePersSubentId::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_Unk_Bool_1 = pFiler->rdBool();
  return eOk;
}

void OdDbAssocSingleEdgePersSubentId::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrString(1, desc()->name());
  pFiler->wrBool(290, m_Unk_Bool_1);
}

OdResult OdDbAssocSingleEdgePersSubentId::dxfInFields(OdDbDxfFiler* pFiler)
{
  NEXT_CODE(290)
  m_Unk_Bool_1 = pFiler->rdBool();

  return eOk;
}

//----------------------------------------------------------------------------

OdDbAssocSimplePersSubentId::OdDbAssocSimplePersSubentId()
  : OdDbAssocPersSubentId()
  , m_Unk_Bool_1(false)
{
}

void OdDbAssocSimplePersSubentId::init(const OdDbSubentId& subentId)
{
  m_SubentId.setType(subentId.type());
  m_SubentId.setIndex(subentId.index());
}

void OdDbAssocSimplePersSubentId::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdUInt16 ver = (pFiler->dwgVersion() >= OdDb::vAC27) ? 1 : 0;
  if (!ver)
    pFiler->wrString(desc()->name());
  else
  {
    pFiler->wrBool(false);
    pFiler->wrInt32(2);
  }

  pFiler->wrInt16((OdInt16) ver);
  pFiler->wrInt32(m_SubentId.type());
  pFiler->wrInt32((OdInt32) m_SubentId.index());

  pFiler->wrBool(m_Unk_Bool_1);
  if (ver) // AC27
    pFiler->wrBool(false);
}

OdResult OdDbAssocSimplePersSubentId::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdUInt16 ver = pFiler->rdInt16();
  if (ver > 1)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }
  
  m_SubentId.setType((OdDb::SubentType)pFiler->rdInt32());
  m_SubentId.setIndex(pFiler->rdInt32());
  m_Unk_Bool_1 = pFiler->rdBool();
  
  if (ver) 
  {
    // AC27
    if (pFiler->rdBool())
    {
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }
  }
  return eOk;
}

void OdDbAssocSimplePersSubentId::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrString(1, desc()->name());

  OdUInt32 ver = (pFiler->dwgVersion() >= OdDb::vAC27) ? 1 : 0;
  pFiler->wrInt32(90, ver);
  pFiler->wrInt32(90, m_SubentId.type());
  pFiler->wrInt32(90, (OdInt32) m_SubentId.index());
  pFiler->wrBool(290, m_Unk_Bool_1);
  if (ver) // AC27
  {
    pFiler->wrBool(290, false);
  }

  // Save m_SubentId
}

OdResult OdDbAssocSimplePersSubentId::dxfInFields(OdDbDxfFiler* pFiler)
{
  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdInt32();
  if (ver > 1)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_SubentId.setType((OdDb::SubentType)pFiler->rdInt32());

  NEXT_CODE(90)
  m_SubentId.setIndex(pFiler->rdInt32());

  NEXT_CODE(290)
  m_Unk_Bool_1 = pFiler->rdBool();

  if (ver) // AC27
  {
    NEXT_CODE(290)
    if (pFiler->rdBool())
    {
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }
  }

  // Read m_SubentId
  return eOk;
}

void OdDbAssocSimplePersSubentId::audit(OdDbAuditInfo* /*pAuditInfo*/)
{
  ODA_FAIL();
  // audit m_SubentId
}

//----------------------------------------------------------------------------

OdDbAssocIndexPersSubentId::OdDbAssocIndexPersSubentId()
  : OdDbAssocPersSubentId()
  , m_Unk_Bool_1(false)
{
}

void OdDbAssocIndexPersSubentId::init(const OdDbSubentId& subentId)
{
  m_SubentId.setType(subentId.type());
  m_SubentId.setIndex(subentId.index());
}

void OdDbAssocIndexPersSubentId::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdUInt16 ver = (pFiler->dwgVersion() >= OdDb::vAC27) ? 1 : 0;
  if (pFiler->dwgVersion() < OdDb::vAC27)
    pFiler->wrString(desc()->name());
  else
  {
    pFiler->wrBool(false);
    pFiler->wrInt32(4);
  }

  pFiler->wrInt16(0);
  pFiler->wrInt32(m_SubentId.type());
  pFiler->wrInt32((OdInt32) m_SubentId.index());

  pFiler->wrBool(m_Unk_Bool_1);
}

OdResult OdDbAssocIndexPersSubentId::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdUInt16 ver = pFiler->rdInt16();
  if (ver > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  
  m_SubentId.setType((OdDb::SubentType)pFiler->rdInt32());
  m_SubentId.setIndex(pFiler->rdInt32());
  m_Unk_Bool_1 = pFiler->rdBool();
  return eOk;
}

void OdDbAssocIndexPersSubentId::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrString(1, desc()->name());

  pFiler->wrInt32(90, 0);
  pFiler->wrInt32(90, m_SubentId.type());
  pFiler->wrInt32(90, (OdInt32) m_SubentId.index());
  pFiler->wrBool(290, m_Unk_Bool_1);
}

OdResult OdDbAssocIndexPersSubentId::dxfInFields(OdDbDxfFiler* pFiler)
{
  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdInt32();
  if (ver > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_SubentId.setType((OdDb::SubentType)pFiler->rdInt32());

  NEXT_CODE(90)
  m_SubentId.setIndex(pFiler->rdInt32());

  NEXT_CODE(290)
  m_Unk_Bool_1 = pFiler->rdBool();
  return eOk;
}

void OdDbAssocIndexPersSubentId::audit(OdDbAuditInfo* /*pAuditInfo*/)
{
  ODA_FAIL();
  // audit m_SubentId
}

OdDbAssocAsmBasedEntityPersSubentId::OdDbAssocAsmBasedEntityPersSubentId()
  : OdDbAssocPersSubentId()
  , m_Unk_Bool_1(false)
{
}

void OdDbAssocAsmBasedEntityPersSubentId::init(const OdDbSubentId& subentId)
{
  m_SubentId.setType(subentId.type());
  m_SubentId.setIndex(subentId.index());
}

void OdDbAssocAsmBasedEntityPersSubentId::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdUInt16 ver = (pFiler->dwgVersion() >= OdDb::vAC27) ? 1 : 0;
  if (pFiler->dwgVersion() < OdDb::vAC27)
    pFiler->wrString(desc()->name());
  else
  {
    pFiler->wrBool(false);
    pFiler->wrInt32(4);
  }

  pFiler->wrInt16(0);
  pFiler->wrInt32(m_SubentId.type());
  pFiler->wrInt32((OdInt32)m_SubentId.index());

  pFiler->wrBool(m_Unk_Bool_1);
}

OdResult OdDbAssocAsmBasedEntityPersSubentId::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdUInt16 ver = pFiler->rdInt16();
  if (ver > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_SubentId.setType((OdDb::SubentType)pFiler->rdInt32());
  m_SubentId.setIndex(pFiler->rdInt32());
  m_Unk_Bool_1 = pFiler->rdBool();
  return eOk;
}

void OdDbAssocAsmBasedEntityPersSubentId::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrString(1, desc()->name());

  pFiler->wrInt32(90, 0);
  pFiler->wrInt32(90, m_SubentId.type());
  pFiler->wrInt32(90, (OdInt32)m_SubentId.index());
  pFiler->wrBool(290, m_Unk_Bool_1);
}

OdResult OdDbAssocAsmBasedEntityPersSubentId::dxfInFields(OdDbDxfFiler* pFiler)
{
  NEXT_CODE(90)
    OdUInt32 ver = pFiler->rdInt32();
  if (ver > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_SubentId.setType((OdDb::SubentType)pFiler->rdInt32());

  NEXT_CODE(90)
    m_SubentId.setIndex(pFiler->rdInt32());

  NEXT_CODE(290)
    m_Unk_Bool_1 = pFiler->rdBool();
  return eOk;
}

void OdDbAssocAsmBasedEntityPersSubentId::audit(OdDbAuditInfo* /*pAuditInfo*/)
{
  ODA_FAIL();
  // audit m_SubentId
}
