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
#include "DbImpAssocViewRepHatchActionParam.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"

OdDbImpAssocViewRepHatchActionParam::OdDbImpAssocViewRepHatchActionParam()
{
}

OdDbImpAssocViewRepHatchActionParam::~OdDbImpAssocViewRepHatchActionParam()
{
}

OdResult OdDbImpAssocViewRepHatchActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocObjectActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  int ver = pFiler->rdInt16();  // Is it version?
  if (ver != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  m_vNormal = pFiler->rdVector3d();
  m_nPropertyOverrides = pFiler->rdInt32();
  m_nHatchIndex = pFiler->rdInt32();
  return eOk;
}

void OdDbImpAssocViewRepHatchActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(0); // Is it version?
  pFiler->wrVector3d(m_vNormal);
  pFiler->wrInt32(m_nPropertyOverrides);
  pFiler->wrInt32(m_nHatchIndex);
}

OdResult OdDbImpAssocViewRepHatchActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocObjectActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocViewRepHatchActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(70)
  int nVer = pFiler->rdInt16();
  if (nVer != 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  NEXT_CODE(210)
  pFiler->rdVector3d(m_vNormal);
  NEXT_CODE(90)
  m_nPropertyOverrides = pFiler->rdInt32();
  NEXT_CODE(90)
  m_nHatchIndex = pFiler->rdInt32();
  ODA_ASSERT_ONCE(pFiler->atEOF());
  return eOk;
}

void OdDbImpAssocViewRepHatchActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(OdDbAssocViewRepHatchActionParam::desc()->name());
  pFiler->wrInt16(70, 0);   // Version?
  pFiler->wrVector3d(210, m_vNormal);
  pFiler->wrInt32(90, m_nPropertyOverrides);
  pFiler->wrInt32(90, m_nHatchIndex);
}
