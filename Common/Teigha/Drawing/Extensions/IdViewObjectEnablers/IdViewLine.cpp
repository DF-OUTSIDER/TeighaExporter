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
#include "IdViewLine.h"

ODRX_DEFINE_MEMBERS_EX(OdIdViewLine,                                                                                 // ClassName
                       OdDbLine,                                                                                     // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::vAC21,                                                                                  // DwgVer
                       OdDb::kMRelease1,                                                                             // MaintVer
                       1,                                                                                            // NProxyFlags
                       L"AcIdViewLine",                                                                              // DWG class name
                       L"ACIDVIEWLINE",                                                                              // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware)                // CustomFlags

OdResult OdIdViewLine::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbLine::dwgInFields(pFiler);
  m_pVportId = pFiler->rdSoftPointerId();
  return eOk;
}

void OdIdViewLine::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbLine::dwgOutFields(pFiler);
  pFiler->wrSoftPointerId(m_pVportId);
}

bool OdIdViewLine::subWorldDraw(OdGiWorldDraw* pWd) const
{
  assertReadEnabled();
  if (ownerId() == database()->getPaperSpaceId())
    return OdDbLine::subWorldDraw(pWd);
  return false;
}

void OdIdViewLine::subViewportDraw(OdGiViewportDraw* pViewportDraw) const
{
  assertReadEnabled();
  if (OdDbObjectId(pViewportDraw->viewportObjectId()) != m_pVportId)
    return;

	OdGePoint3d pts[2];
	pts[0] = startPoint();
	pts[1] = endPoint();
  OdGeVector3d vNormal = normal();
  pViewportDraw->geometry().polyline( 2, pts, &vNormal);
}

OdResult OdIdViewLine::subExplode(OdRxObjectPtrArray& /*entitySet*/) const
{
  assertReadEnabled();
  return eOk;
}
