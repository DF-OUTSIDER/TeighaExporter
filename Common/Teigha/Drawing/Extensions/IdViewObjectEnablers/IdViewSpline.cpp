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
#include "IdViewSpline.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeKnotVector.h"

ODRX_DEFINE_MEMBERS_EX(OdIdViewSpline,                                                                               // ClassName
                       OdDbSpline,                                                                                   // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::vAC21,                                                                                  // DwgVer
                       OdDb::kMRelease1,                                                                             // MaintVer
                       1,                                                                                            // NProxyFlags
                       L"AcIdViewSpline",                                                                            // DWG class name
                       L"ACIDVIEWSPLINE",                                                                            // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware)                // CustomFlags

OdResult OdIdViewSpline::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbSpline::dwgInFields(pFiler);
  m_pVportId = pFiler->rdSoftPointerId();
  return eOk;
}

void OdIdViewSpline::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbSpline::dwgOutFields(pFiler);
  pFiler->wrSoftPointerId(m_pVportId);
}

bool OdIdViewSpline::subWorldDraw(OdGiWorldDraw* pWd) const
{
  assertReadEnabled();
  if (ownerId() == database()->getPaperSpaceId())
    return OdDbSpline::worldDraw(pWd);
  return false;
}

void OdIdViewSpline::subViewportDraw(OdGiViewportDraw* pViewportDraw) const
{
  assertReadEnabled();
  if (OdDbObjectId(pViewportDraw->viewportObjectId()) != m_pVportId)
    return;

  OdGiGeometry& geom = pViewportDraw->geometry();


  int degree;
  bool rational, closed, periodic;
  OdGePoint3dArray controlPoints;
  OdGeKnotVector knots;
  OdGeDoubleArray weights;
  double controlPtTol;
  getNurbsData( degree, rational, closed, periodic,
                controlPoints, knots, weights, controlPtTol);

  
  if (controlPoints.size())
  {
    OdGeNurbCurve3d nurb(degree, knots, controlPoints, weights, periodic);
    geom.nurbs(nurb);

    OdDbDatabasePtr pDb = database();
    if ( pDb.isNull() )
    {
      pDb = pViewportDraw->context()->database();
    }
    if ( !pDb.isNull() && pDb->getSPLFRAME())
    { // Draw spline frame
      if (closed)
      {
        controlPoints.append(controlPoints[0]);
      }
      pViewportDraw->subEntityTraits().setLineType(pDb->getLinetypeContinuousId());
     	geom.polyline(controlPoints.size(), controlPoints.getPtr());
    }
  }
}

OdResult OdIdViewSpline::subExplode(OdRxObjectPtrArray& /*entitySet*/) const
{
  assertReadEnabled();
  return eOk;
}
