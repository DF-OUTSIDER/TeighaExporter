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
#include "DbImpAssocViewRepHatchManager.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"

OdDbImpAssocViewRepHatchManager::OdDbImpAssocViewRepHatchManager()
  : m_Count_ActionParam(0)
{
}

OdDbImpAssocViewRepHatchManager::~OdDbImpAssocViewRepHatchManager()
{
}

OdResult OdDbImpAssocViewRepHatchManager::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocCompoundActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  m_nVersion  = pFiler->rdInt16();               // = 0 - version

  if (m_nVersion != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_Count_ActionParam = pFiler->rdInt32();       // size array

  m_Unknown2.resize(m_Count_ActionParam);
  m_Unknown3.resize(m_Count_ActionParam);
  m_Unknown4.resize(m_Count_ActionParam);
  m_AssocViewRepHatchActionParam.resize(m_Count_ActionParam);

  for(int i=0; i<m_Count_ActionParam; i++)
  {
    DWG_IN_UINT64(m_Unknown2[i]);              // 2, 3, 4
    DWG_IN_UINT64(m_Unknown3[i]);              // 2, 3, 4
    m_Unknown4[i] = pFiler->rdInt32();         // 1, 1, 1 also?

    m_AssocViewRepHatchActionParam[i] = pFiler->rdSoftPointerId(); // DbAssocViewRepHatchActionParam 1B4, 1B7, 1BA
    ODA_ASSERT_ONCE(!m_AssocViewRepHatchActionParam[i].isNull());
  }

  return eOk;
}

void OdDbImpAssocViewRepHatchManager::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocCompoundActionParam::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(m_nVersion);                   // Version
  pFiler->wrInt32(m_Count_ActionParam);          // count of AcDbAssocViewRepHatchActionParam

  for(int i=0; i<m_Count_ActionParam; i++)
  {
    ODA_ASSERT_ONCE(m_Unknown2[i] != OdUInt64());
    DWG_OUT_UINT64(m_Unknown2[i]);
    ODA_ASSERT_ONCE(m_Unknown3[i] != OdUInt64());
    DWG_OUT_UINT64(m_Unknown3[i]);
    pFiler->wrInt32(m_Unknown4[i]);              // 1 also?

    pFiler->wrSoftPointerId(m_AssocViewRepHatchActionParam[i]); // DbAssocViewRepHatchActionParam
  }
}

OdResult OdDbImpAssocViewRepHatchManager::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocCompoundActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocViewRepHatchManager::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(70)
  m_nVersion  = pFiler->rdUInt16();              // = 0 - version
  if (m_nVersion != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_Count_ActionParam = pFiler->rdInt32();       // count of AcDbAssocViewRepHatchActionParam

  m_Unknown2.resize(m_Count_ActionParam);
  m_Unknown3.resize(m_Count_ActionParam);
  m_Unknown4.resize(m_Count_ActionParam);
  m_AssocViewRepHatchActionParam.resize(m_Count_ActionParam);

  for(int i=0; i<m_Count_ActionParam; i++)
  {
    NEXT_UINT64(160, m_Unknown2[i])
    NEXT_UINT64(160, m_Unknown3[i])
    NEXT_CODE(90)                              
    m_Unknown4[i] = pFiler->rdInt32();         // 1
    NEXT_CODE(330)
    m_AssocViewRepHatchActionParam[i] = pFiler->rdObjectId(); // DbAssocViewRepHatchActionParam 1B4, 1B7, 1BA
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocViewRepHatchManager::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocCompoundActionParam::dxfOutFields(pFiler, objectId);
  
  pFiler->wrSubclassMarker(OdDbAssocViewRepHatchManager::desc()->name()); // class Marker

  pFiler->wrUInt16(70, m_nVersion);              // Version

  pFiler->wrInt32(90, m_Count_ActionParam);      // count of AcDbAssocViewRepHatchActionParam

  for(int i=0; i<m_Count_ActionParam; i++)
  {
    DXF_OUT_UINT64(160, m_Unknown2[i]);          //
    DXF_OUT_UINT64(160, m_Unknown3[i]);          //
    pFiler->wrInt32(90, m_Unknown4[i]);          // 1 also?

    pFiler->wrObjectId(330, m_AssocViewRepHatchActionParam[i]); // DbAssocViewRepHatchActionParam
  }
}
