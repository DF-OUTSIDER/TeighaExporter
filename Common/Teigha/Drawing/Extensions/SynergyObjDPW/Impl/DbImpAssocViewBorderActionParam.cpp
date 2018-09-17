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
#include "DbImpAssocViewBorderActionParam.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"

OdDbImpAssocViewBorderActionParam::OdDbImpAssocViewBorderActionParam()
{
}

OdDbImpAssocViewBorderActionParam::~OdDbImpAssocViewBorderActionParam()
{
}

OdResult OdDbImpAssocViewBorderActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocObjectActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  int ver = pFiler->rdInt16();
  if (ver != 1)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  return eOk;
}

void OdDbImpAssocViewBorderActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dwgOutFields(pFiler, objectId);
  pFiler->wrInt16(1); // Version
}

OdResult OdDbImpAssocViewBorderActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocObjectActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocViewBorderActionParam::desc()->name()))
    return eMakeMeProxy;

  NEXT_CODE(70);
  if (pFiler->rdInt16() != 1)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  return eOk;
}

void OdDbImpAssocViewBorderActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(OdDbAssocViewBorderActionParam::desc()->name());
  pFiler->wrInt16(70, 1); // Version
}
