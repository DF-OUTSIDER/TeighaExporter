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
#include "DbSpline.h"
#include "OdDbAssocSplinePersSubentIdPE.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeKnotVector.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocSplinePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocSplinePersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocSplinePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocSingleEdgePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbSpline::desc()) || !checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType) )
    res.attach((OdDbAssocSingleEdgePersSubentId*)NULL);
  else
  {
    res = OdDbAssocSingleEdgePersSubentId::createObject();
  }
  return res;
}

OdResult OdDbAssocSplinePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbSpline::desc()) )
    return eBadObjType;

  if ( subentType == OdDb::kEdgeSubentType )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, (OdGsMarker)1));
    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);
    
    for ( int i = 0; i < pSpline->numControlPoints(); i++ )
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, i));

    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocSplinePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                 const OdDbSubentId&    edgeSubentId,
                                                                 OdDbSubentId&          startVertexSubentId,
                                                                 OdDbSubentId&          endVertexSubentId,
                                                                 OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return otherVertexSubentIds.length() == 0 ? eBadObjType : eOk;
}

OdResult OdDbAssocSplinePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbSpline::desc()) )
  {
    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);

    int degree;
    bool rational;
    bool closed;
    bool periodic;
    OdGePoint3dArray controlPoints;
    OdGeKnotVector knots;
    OdGeDoubleArray weights;
    double controlPtTol;

    pSpline->getNurbsData(degree, rational, closed, periodic,
                              controlPoints, knots, weights, controlPtTol);

    pEdgeCurve = new OdGeNurbCurve3d(degree, knots, controlPoints, weights, pSpline->isPeriodic());

    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocSplinePersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbSpline::desc())
        && pNewEdgeCurve->isKindOf(OdGe::kCircArc3d) )
  {
    OdGeNurbCurve3d* pGeNurb = (OdGeNurbCurve3d*)pNewEdgeCurve;
    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);

    int i;
    OdGePoint3dArray points;
    OdGeDoubleArray weights;
    for ( i = 0; i < pGeNurb->numControlPoints(); i++ )
      points.append(pGeNurb->controlPointAt(i));

    for ( i = 0; i < pGeNurb->numWeights(); i++ )
      weights.append(pGeNurb->weightAt(i));

    double period;
    pSpline->setNurbsData(pGeNurb->degree(),
                          pGeNurb->isRational(),
                          pGeNurb->isClosed(),
                          pGeNurb->isPeriodic(period),
                          points,
                          pGeNurb->knots(),
                          weights,
                          pSpline-> fitTolerance());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocSplinePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(pEntity, vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbSpline::desc()) )
  {
    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kStart:
        pSpline->getControlPointAt(0, vertexPosition);
      break;
      case kEnd:
        pSpline->getControlPointAt(pSpline->numControlPoints() - 1, vertexPosition);
      break;
      default:
        pSpline->getControlPointAt((int)vertexSubentId.index(), vertexPosition);
    }

    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocSplinePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(pEntity, vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbSpline::desc()) )
  {
    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kStart:
        pSpline->setControlPointAt(0, newVertexPosition);
      break;
      case kEnd:
        pSpline->setControlPointAt(pSpline->numControlPoints() - 1, newVertexPosition);
      break;
      default:
        pSpline->setControlPointAt((int)vertexSubentId.index(), newVertexPosition);
    }
    return eOk;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocSplinePersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbSpline::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbSpline::desc()) )
    return eBadObjType;

  if ( checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType) )
  {
    OdGeCurve3d* pCurve = NULL;
    if ( getEdgeSubentityGeometry(pNewSubentityGeometry, subentId, pCurve) != eOk )
    {
      delete pCurve;
      return eBadObjType;
    }
    else
    {
      OdResult res = setEdgeSubentityGeometry(pEntity, subentId, pCurve);
      delete pCurve;
      return res;
    }
  }
  else if ( checkSubentId(pEntity, subentId, OdDb::kVertexSubentType) )
  {
    OdGePoint3d newPt;
    if ( getVertexSubentityGeometry(pNewSubentityGeometry, subentId, newPt) != eOk )
      return eBadObjType;

    setVertexSubentityGeometry(pEntity, subentId, newPt);
    return eOk;
  }
  else
    return eBadObjType;
}

void OdDbAssocSplinePersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  startVertexSubentId.setIndex(kStart);
}

void OdDbAssocSplinePersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                        const OdDbSubentId&    edgeSubentId,
                                                        OdDbSubentId& endVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, endVertexSubentId);
  endVertexSubentId.setIndex(kEnd);
}

void OdDbAssocSplinePersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                          const OdDbSubentId&    edgeSubentId,
                                                          OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);
    for ( int i = 0; i < pSpline->numControlPoints(); i++ )
      otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, i));
  }
}

bool OdDbAssocSplinePersSubentIdPE::checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( type == OdDb::kEdgeSubentType )
  {
    return subentId.index() == 1;
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    if ( !isTypeEqual(pEntity, OdDbSpline::desc()) )
      return false;

    OdDbSplinePtr pSpline = OdDbSpline::cast(pEntity);
    OdGsMarker idx = subentId.index();
    return (idx == kStart || idx == kEnd || (0 <= idx && idx < pSpline->numControlPoints()));
  }
  else
    return false;
}
