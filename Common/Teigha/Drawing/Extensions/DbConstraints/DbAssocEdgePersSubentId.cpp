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
#include "DbAssocEdgePersSubentId.h"
#include "DbFiler.h"
#include "RxObjectImpl.h"
#include "DbSubentId.h"

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocEdgePersSubentId, OdDbAssocPersSubentId,
                                 L"AcDbAssocEdgePersSubentId", RXIMPL_CONSTR);

OdDbAssocEdgePersSubentId::OdDbAssocEdgePersSubentId()
  :OdDbAssocPersSubentId()
  , m_index1(0)
  , m_index2(0)
  , m_isMidPoint(false)
{
}

void OdDbAssocEdgePersSubentId::init(int vertexIdentifier1, int vertexIdentifier2)
{
  m_index1 = OdIntToGsMarker(vertexIdentifier1);
  m_index2 = OdIntToGsMarker(vertexIdentifier2);
}

OdDb::SubentType OdDbAssocEdgePersSubentId::subentType(const OdDbEntity* pEntity) const
{
  if (m_index1 == 0)
    return OdDb::kNullSubentType;

  return (m_index2 == 0)
            ? OdDb::kVertexSubentType
            : OdDb::kEdgeSubentType;
}

void OdDbAssocEdgePersSubentId::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  if (pFiler->dwgVersion() < OdDb::vAC27)
    pFiler->wrString(desc()->name());
  else
  {
    pFiler->wrBool(false);
    pFiler->wrInt32(3);
  }

  pFiler->wrInt16(0);
  pFiler->wrInt32((int)m_index1);
  pFiler->wrInt32((int)m_index2);

  pFiler->wrBool(m_isMidPoint);
}

OdResult OdDbAssocEdgePersSubentId::dwgInFields(OdDbDwgFiler* pFiler)
{
  int rdVal = pFiler->rdInt16();
  ODA_ASSERT(rdVal == 0);

  m_index1 = OdIntToGsMarker(pFiler->rdInt32());
  m_index2 = OdIntToGsMarker(pFiler->rdInt32());

  m_isMidPoint = pFiler->rdBool();

  return eOk;
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

void OdDbAssocEdgePersSubentId::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrString(1, desc()->name());

  pFiler->wrInt32(90, 0);
  pFiler->wrInt32(90, (int)m_index1);
  pFiler->wrInt32(90, (int)m_index2);

  pFiler->wrBool(290, m_isMidPoint);
}

OdResult OdDbAssocEdgePersSubentId::dxfInFields(OdDbDxfFiler* pFiler)
{
  NEXT_CODE(90)
  if ( pFiler->rdInt32() != 0 )
    return eMakeMeProxy;

  NEXT_CODE(90)
  m_index1 = OdIntToGsMarker(pFiler->rdInt32());

  NEXT_CODE(90)
  m_index2 = OdIntToGsMarker(pFiler->rdInt32());

  NEXT_CODE(290)
  m_isMidPoint = pFiler->rdBool();

  return eOk;
}

OdResult OdDbAssocEdgePersSubentId::getTransientSubentIds(const OdDbEntity*  pEntity, 
                                        OdArray<OdDbSubentId>& subents) const
{
  if (m_index1 == 0)
    return eTooFewVertices;

  OdDb::SubentType type = (m_index2 == 0) ? OdDb::kVertexSubentType : OdDb::kEdgeSubentType;
  
  subents.push_back(OdDbSubentId(type, m_index1));
  return eOk;
}
