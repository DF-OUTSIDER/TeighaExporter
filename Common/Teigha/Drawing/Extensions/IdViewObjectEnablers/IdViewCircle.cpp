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
#include "IdViewCircle.h"

ODRX_DEFINE_MEMBERS_EX(OdIdViewCircle,                                                                               // ClassName
                       OdDbCircle,                                                                                   // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::vAC21,                                                                                  // DwgVer
                       OdDb::kMRelease1,                                                                             // MaintVer
                       1,                                                                                            // NProxyFlags
                       L"AcIdViewCircle",                                                                            // DWG class name
                       L"ACIDVIEWCIRCLE",                                                                            // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware)                // CustomFlags

OdResult OdIdViewCircle::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbCircle::dwgInFields(pFiler);
  m_pVportId = pFiler->rdSoftPointerId();
  return eOk;
}

void OdIdViewCircle::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbCircle::dwgOutFields(pFiler);
  pFiler->wrSoftPointerId(m_pVportId);
}

bool OdIdViewCircle::subWorldDraw(OdGiWorldDraw* pWd) const
{
  assertReadEnabled();
  if (ownerId() == database()->getPaperSpaceId())
    return OdDbCircle::subWorldDraw(pWd);
  return false;
}

void OdIdViewCircle::subViewportDraw(OdGiViewportDraw* pViewportDraw) const
{
  assertReadEnabled();
  if (OdDbObjectId(pViewportDraw->viewportObjectId()) != m_pVportId)
    return;
  pViewportDraw->geometry().circle(center(), radius(), normal());
}

OdResult OdIdViewCircle::subExplode(OdRxObjectPtrArray& /*entitySet*/) const
{
  assertReadEnabled();
  return eOk;
}
