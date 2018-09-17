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
#include "DbArc.h"
#include "OdDbAssocArcPersSubentIdPE.h"
#include "Ge/GeCircArc3d.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocArcPersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocArcPersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocArcPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocPersSubentIdPtr res;
  OdDbAssocPersSubentId* ptr = NULL;
  if ( !isTypeEqual(pEntity, OdDbArc::desc()) || !checkSubentId(subentId, subentId.type()) )
    res.attach(const_cast<const OdDbAssocPersSubentId*>(ptr));
  else
  {
    switch ( subentId.type() )
    {
      case OdDb::kVertexSubentType:
        res = OdDbAssocSimplePersSubentId::createObject();
        OdDbAssocSimplePersSubentId::cast( res )->init( subentId );
        break;

      case OdDb::kEdgeSubentType:
        res = OdDbAssocSingleEdgePersSubentId::createObject();
        break;
    }
  }
  return res;
}

OdResult OdDbAssocArcPersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbArc::desc()) )
    return eBadObjType;

  if ( subentType == OdDb::kEdgeSubentType )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, (OdGsMarker)1));
    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kStart));
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kEnd));
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kCenter));
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kMiddle));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocArcPersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                 const OdDbSubentId&    edgeSubentId,
                                                                 OdDbSubentId&          startVertexSubentId,
                                                                 OdDbSubentId&          endVertexSubentId,
                                                                 OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return otherVertexSubentIds.length() == 0 ? eBadObjType : eOk;
}

OdResult OdDbAssocArcPersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbArc::desc()) )
  {
    OdDbArcPtr pArc = OdDbArc::cast(pEntity);
    pEdgeCurve = new OdGeCircArc3d(pArc->center(), pArc->normal(), OdGeVector3d::kXAxis ,pArc->radius(), pArc->startAngle(), pArc->endAngle());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocArcPersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbArc::desc())
        && pNewEdgeCurve->isKindOf(OdGe::kCircArc3d) )
  {
    OdGeCircArc3d* pGeArc = (OdGeCircArc3d*)pNewEdgeCurve;
    OdDbArcPtr pArc = OdDbArc::cast(pEntity);

    pArc->setNormal(pGeArc->normal());
    pArc->setCenter(pGeArc->center());
    pArc->setRadius(pGeArc->radius());
    pArc->setStartAngle(pGeArc->startAng());
    pArc->setEndAngle(pGeArc->endAng());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocArcPersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbArc::desc()) )
  {
    OdDbArcPtr pArc = OdDbArc::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kCenter:
        vertexPosition = pArc->center();
      break;
      case kStart:
        pArc->getStartPoint(vertexPosition);
      break;
      case kEnd:
        pArc->getEndPoint(vertexPosition);
      break;
      case kMiddle:
      {
        double stP = 0.0;
        double enP = 0.0;
        pArc->getStartParam(stP);
        pArc->getEndParam(enP);
        pArc->getPointAtParam((stP + enP)/2.0, vertexPosition);
      }
      break;
      default:
        return eBadObjType;
    }

    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocArcPersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbArc::desc()) )
  {
    OdDbArcPtr pArc = OdDbArc::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kCenter:
        pArc->setCenter(newVertexPosition);
      break;
      case kStart:
      {
        double param;
        pArc->getParamAtPoint(newVertexPosition, param);
        pArc->setStartAngle(param);
      }
      break;
      case kEnd:
      {
        double param;
        pArc->getParamAtPoint(newVertexPosition, param);
        pArc->setEndAngle(param);
      }
      break;
      case kMiddle:
        
      break;
      default:
        return eBadObjType;
    }
    return eOk;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocArcPersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbArc::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbArc::desc()) )
    return eBadObjType;

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

void OdDbAssocArcPersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  startVertexSubentId.setIndex(kStart);
}

void OdDbAssocArcPersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                        const OdDbSubentId&    edgeSubentId,
                                                        OdDbSubentId& endVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, endVertexSubentId);
  endVertexSubentId.setIndex(kEnd);
}

void OdDbAssocArcPersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                          const OdDbSubentId&    edgeSubentId,
                                                          OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kCenter));
    otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kMiddle));
  }
}

bool OdDbAssocArcPersSubentIdPE::checkSubentId(const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( type == OdDb::kEdgeSubentType )
  {
    return subentId.index() == 1;
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    return subentId.index() == kCenter || subentId.index() == kStart || subentId.index() == kEnd || subentId.index() == kMiddle;
  }
  else
    return false;
}
