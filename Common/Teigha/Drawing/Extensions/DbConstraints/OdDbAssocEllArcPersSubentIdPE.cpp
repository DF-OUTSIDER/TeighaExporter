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
#include "DbEllipse.h"
#include "OdDbAssocEllArcPersSubentIdPE.h"
#include "Ge/GeEllipArc3d.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocEllArcPersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocEllArcPersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocEllArcPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocSingleEdgePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbEllipse::desc()) || !checkSubentId(subentId, OdDb::kEdgeSubentType) )
    res.attach((OdDbAssocSingleEdgePersSubentId*)NULL);
  else
  {
    res = OdDbAssocSingleEdgePersSubentId::createObject();
  }
  return res;
}

OdResult OdDbAssocEllArcPersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbEllipse::desc()) )
    return eBadObjType;

  if ( subentType == OdDb::kEdgeSubentType )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, (OdGsMarker)1));
    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    OdDbEllipsePtr pEllipse = OdDbEllipse::cast(pEntity);

    if ( pEllipse->isClosed() )
    {
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kCenter));
    }
    else
    {
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kStart));
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kCenter));
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kEnd));
    }
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocEllArcPersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                 const OdDbSubentId&    edgeSubentId,
                                                                 OdDbSubentId&          startVertexSubentId,
                                                                 OdDbSubentId&          endVertexSubentId,
                                                                 OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return otherVertexSubentIds.length() == 0 ? eBadObjType : eOk;
}

OdResult OdDbAssocEllArcPersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbEllipse::desc()) )
  {
    OdDbEllipsePtr pEllipse = OdDbEllipse::cast(pEntity);
    OdGeVector3d majorAxis = pEllipse->majorAxis();
    OdGeVector3d minorAxis = pEllipse->minorAxis();
    if ( pEllipse->isClosed() )
      pEdgeCurve = new OdGeEllipArc3d(pEllipse->center(), majorAxis, minorAxis,
                  majorAxis.length(), minorAxis.length());
    else
      pEdgeCurve = new OdGeEllipArc3d(pEllipse->center(), majorAxis, minorAxis,
                  majorAxis.length(), minorAxis.length(), pEllipse->startAngle(), pEllipse->endAngle());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocEllArcPersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbEllipse::desc())
          && pNewEdgeCurve->isKindOf(OdGe::kEllipArc3d) )
  {
    OdGeEllipArc3d* pGeEllArc = (OdGeEllipArc3d*)pNewEdgeCurve;
    OdDbEllipsePtr pEllArc = OdDbEllipse::cast(pEntity);
    OdGeVector3d majorAxis = pGeEllArc->majorAxis();
    majorAxis.normalize();
    majorAxis *= pGeEllArc->majorRadius();
    double radiusRatio = pGeEllArc->majorRadius()/pGeEllArc->minorRadius();
    if ( radiusRatio > 1.0 )
      radiusRatio = 1.0/radiusRatio;

    if ( pEllArc->isClosed() )
    {
      pEllArc->set(pGeEllArc->center(), pEllArc->normal(), majorAxis, radiusRatio);
    }
    else
    {
      pEllArc->set(pGeEllArc->center(), pEllArc->normal(), majorAxis, radiusRatio, pGeEllArc->startAng(), pGeEllArc->endAng());
    }
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocEllArcPersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbEllipse::desc()) )
  {
    OdDbEllipsePtr pEllArc = OdDbEllipse::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kStart:
        return pEllArc->getStartPoint(vertexPosition);
      case kEnd:
        return pEllArc->getEndPoint(vertexPosition);
      case kCenter:
        vertexPosition = pEllArc->center();
      break;
      default:
        return eBadObjType;
    }
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocEllArcPersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbEllipse::desc()) )
  {
    OdDbEllipsePtr pEllArc = OdDbEllipse::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kStart:
      {
        double startParam;
        if ( pEllArc->getParamAtPoint(newVertexPosition, startParam) == eOk )
          pEllArc->setStartParam(startParam);
        else
          return eBadObjType;
      }
      break;
      case kEnd:
        double endParam;
        if ( pEllArc->getParamAtPoint(newVertexPosition, endParam) == eOk )
          pEllArc->setEndParam(endParam);
        else
          return eBadObjType;
      break;
      case kCenter:
        pEllArc->setCenter(newVertexPosition);
      break;
      default:
        return eBadObjType;
    }
    return eOk;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocEllArcPersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbEllipse::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbEllipse::desc()) )
    return eBadObjType;

  OdDbEllipsePtr pLineDst = OdDbEllipse::cast(pEntity);
  OdDbEllipsePtr pLineSrc = OdDbEllipse::cast(pNewSubentityGeometry);

  if ( checkSubentId(subentId, OdDb::kEdgeSubentType) )
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
  else if ( checkSubentId(subentId, OdDb::kVertexSubentType) )
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

void OdDbAssocEllArcPersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  if (!isTypeEqual(pEntity, OdDbEllipse::desc()) )
    return;

  OdDbEllipsePtr pEllArc = OdDbEllipse::cast(pEntity);

  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) && !pEllArc->isClosed() )
    startVertexSubentId.setIndex(kStart);
}

void OdDbAssocEllArcPersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                        const OdDbSubentId&    edgeSubentId,
                                                        OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);

  if (!isTypeEqual(pEntity, OdDbEllipse::desc()) )
    return;

  OdDbEllipsePtr pEllArc = OdDbEllipse::cast(pEntity);

  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && !pEllArc->isClosed() )
    startVertexSubentId.setIndex(kEnd);
}

void OdDbAssocEllArcPersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                          const OdDbSubentId&    edgeSubentId,
                                                          OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kCenter));
  }
}

bool OdDbAssocEllArcPersSubentIdPE::checkSubentId(const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( type == OdDb::kEdgeSubentType )
  {
    return subentId.index() == 1;
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    return subentId.index() == kStart || subentId.index() == kEnd || subentId.index() == kCenter;
  }
  else
    return false;
}
