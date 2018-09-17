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
#include "DbImpAssocViewRepActionParam.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"

OdDbImpAssocViewRepActionParam::OdDbImpAssocViewRepActionParam()
{
  m_isBase = false;
}

OdDbImpAssocViewRepActionParam::~OdDbImpAssocViewRepActionParam()
{
}

OdResult OdDbImpAssocViewRepActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  //Handle: x2db x2fc of dataFiles/tests/bugs/11652/DetailAndSectionViews.dwg

  OdResult res = OdDbImpAssocObjectActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  OdUInt16 ver = pFiler->rdInt16(); // 1
  if (ver != 1)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }
  return eOk;
}

void OdDbImpAssocViewRepActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dwgOutFields(pFiler, objectId);

  OdUInt16 ver = 1;
  pFiler->wrInt16(ver);
}

OdResult OdDbImpAssocViewRepActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  //Handle: x2db x2fc of dataFiles/tests/bugs/11652/DetailAndSectionViews.dxf

  OdResult res = OdDbImpAssocObjectActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocViewRepActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(70)
  OdUInt16 ver = pFiler->rdUInt16(); // 1
  if (ver != 1)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocViewRepActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dxfOutFields(pFiler, objectId);
  
  pFiler->wrSubclassMarker(OdDbAssocViewRepActionParam::desc()->name());

  OdUInt16 ver = 1;
  pFiler->wrUInt16(70, ver);
}
