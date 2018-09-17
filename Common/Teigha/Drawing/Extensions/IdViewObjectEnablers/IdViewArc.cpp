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
#include "IdViewArc.h"

ODRX_DEFINE_MEMBERS_EX(OdIdViewArc,                                                                                  // ClassName
                       OdDbArc,                                                                                      // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::vAC21,                                                                                  // DwgVer
                       OdDb::kMRelease1,                                                                             // MaintVer
                       1,                                                                                            // NProxyFlags
                       L"AcIdViewArc",                                                                               // DWG class name
                       L"ACIDVIEWARC",                                                                               // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware)                // CustomFlags

OdResult OdIdViewArc::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbArc::dwgInFields(pFiler);
  m_pVportId = pFiler->rdSoftPointerId();
  return eOk;
}

void OdIdViewArc::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbArc::dwgOutFields(pFiler);
  pFiler->wrSoftPointerId(m_pVportId);
}

bool OdIdViewArc::subWorldDraw(OdGiWorldDraw* pWd) const
{
  assertReadEnabled();
  if (ownerId() == database()->getPaperSpaceId())
    return OdDbArc::subWorldDraw(pWd);
  return false;
}

void OdIdViewArc::subViewportDraw(OdGiViewportDraw* pViewportDraw) const
{
  assertReadEnabled();
  if (OdDbObjectId(pViewportDraw->viewportObjectId()) != m_pVportId)
    return;

  OdGePoint3d ptCenter = center();
  double dRadius = radius();
  OdGeVector3d vStart;
  if (dRadius)
  {
    OdGePoint3d ptStart;
    getStartPoint(ptStart);
    vStart = (ptStart - ptCenter) / dRadius;
  }
  double dSweepAngle = endAngle() - startAngle();
  if (dSweepAngle < 0.)
    dSweepAngle += Oda2PI;
  else if (dSweepAngle > Oda2PI)
    dSweepAngle = Oda2PI;

  pViewportDraw->geometry().circularArc(ptCenter, dRadius, normal(), vStart, dSweepAngle);
}

OdResult OdIdViewArc::subExplode(OdRxObjectPtrArray& /*entitySet*/) const
{
  assertReadEnabled();
  return eOk;
}
