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
#include "DbLine.h"
#include "OdDbAssocLinePersSubentIdPE.h"
#include "Ge/GeLineSeg3d.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocLinePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocLinePersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocLinePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocPersSubentIdPtr res;
  OdDbAssocPersSubentId* ptr = NULL;
  if ( !isTypeEqual(pEntity, OdDbLine::desc()) || !checkSubentId(subentId, subentId.type() ) )
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

OdResult OdDbAssocLinePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbLine::desc()) )
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
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kMiddle));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocLinePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                               const OdDbSubentId&    edgeSubentId,
                                                               OdDbSubentId&          startVertexSubentId,
                                                               OdDbSubentId&          endVertexSubentId,
                                                               OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return startVertexSubentId.index() != kNotDefine ? eBadObjType : eOk;
}

OdResult OdDbAssocLinePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbLine::desc()) )
  {
    OdDbLinePtr pLine = OdDbLine::cast(pEntity);
    pEdgeCurve = new OdGeLineSeg3d(pLine->startPoint(), pLine->endPoint());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocLinePersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbLine::desc())
      && pNewEdgeCurve->isKindOf(OdGe::kLineSeg3d) )
  {
    OdGeLineSeg3d* pGeLine = (OdGeLineSeg3d*)pNewEdgeCurve;
    OdDbLinePtr pLine = OdDbLine::cast(pEntity);
    pLine->setStartPoint(pGeLine->startPoint());
    pLine->setEndPoint(pGeLine->endPoint());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocLinePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbLine::desc()) )
  {
    OdDbLinePtr pLine = OdDbLine::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kStart:
        vertexPosition = pLine->startPoint();
      break;
      case kEnd:
        vertexPosition = pLine->endPoint();
      break;
      case kMiddle:
      {
        OdGePoint3d startPt = pLine->startPoint();
        OdGePoint3d endPt = pLine->endPoint();

        vertexPosition.set((startPt.x + endPt.x)/2.0, (startPt.y + endPt.y)/2.0, (startPt.z + endPt.z)/2.0);
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

OdResult OdDbAssocLinePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbLine::desc()) )
  {
    OdDbLinePtr pLine = OdDbLine::cast(pEntity);
    switch ( vertexSubentId.index() )
    {
      case kStart:
        pLine->setStartPoint(newVertexPosition);
      break;
      case kEnd:
        pLine->setEndPoint(newVertexPosition);
      break;
      case kMiddle:
      {
        OdGePoint3d startPt = pLine->startPoint();
        OdGePoint3d endPt = pLine->endPoint();
        OdGePoint3d oldMidPt((startPt.x + endPt.x)/2.0, (startPt.y + endPt.y)/2.0, (startPt.z + endPt.z)/2.0);

        OdGeVector3d delta = (newVertexPosition - oldMidPt);
        startPt += delta;
        endPt += delta;
        pLine->setStartPoint(startPt);
        pLine->setEndPoint(endPt);
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


OdResult OdDbAssocLinePersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbLine::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbLine::desc()) )
    return eBadObjType;

  OdDbLinePtr pLineDst = OdDbLine::cast(pEntity);
  OdDbLinePtr pLineSrc = OdDbLine::cast(pNewSubentityGeometry);

  if ( checkSubentId(subentId, OdDb::kEdgeSubentType) )
  {
    pLineDst->setStartPoint(pLineSrc->startPoint());
    pLineDst->setEndPoint(pLineSrc->endPoint());

    return eOk;
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

void OdDbAssocLinePersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
    startVertexSubentId.setIndex(kStart);
}

void OdDbAssocLinePersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                        const OdDbSubentId&    edgeSubentId,
                                                        OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
    startVertexSubentId.setIndex(kEnd);
}

void OdDbAssocLinePersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                          const OdDbSubentId&    edgeSubentId,
                                                          OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kMiddle));
  }
}

bool OdDbAssocLinePersSubentIdPE::checkSubentId(const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( type == OdDb::kEdgeSubentType )
  {
    return subentId.index() == 1;
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    return subentId.index() == kStart || subentId.index() == kEnd || subentId.index() == kMiddle;
  }
  else
    return false;
}
