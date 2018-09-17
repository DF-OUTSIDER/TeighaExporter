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
#include "DbEntity.h"
#include "DbPolyline.h"
#include "OdDbAssocPolylinePersSubentIdPE.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeLineSeg3d.h"
#include "DbAssocEdgePersSubentId.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocPolylinePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocPolylinePersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocPolylinePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                              const class OdDbCompoundObjectId& compId,
                                                                              const OdDbSubentId& subentId)
{
  OdDbAssocEdgePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) || !checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType) )
    res.attach((OdDbAssocEdgePersSubentId*)NULL);
  else
  {
    OdDbPolylinePtr pPolyLine = OdDbPolyline::cast(pEntity);
    int ptQty = pPolyLine->numVerts();
    int maxIdentifier = 0;
    for (int i = 0; i < ptQty; i++)
    {
        if(pPolyLine->getVertexIdentifierAt(i) > maxIdentifier)
            maxIdentifier = pPolyLine->getVertexIdentifierAt(i);
    }
    
    int startVertexIdx = (int)subentId.index() - 1;
    int endVertexIdx = startVertexIdx + 1;
    if (endVertexIdx >= ptQty)
      endVertexIdx = 0;

    if ( pPolyLine->getVertexIdentifierAt(startVertexIdx) == 0 )
    {
      pPolyLine->setVertexIdentifierAt(startVertexIdx, ++maxIdentifier);
    }

    if ( pPolyLine->getVertexIdentifierAt(endVertexIdx) == 0 )
    {
      pPolyLine->setVertexIdentifierAt(endVertexIdx, ++maxIdentifier);
    }

    res = OdDbAssocEdgePersSubentId::createObject();
    res->init(pPolyLine->getVertexIdentifierAt(startVertexIdx), pPolyLine->getVertexIdentifierAt(endVertexIdx));
  }

  return res;
}


// Should be "subents.append(OdDbSubentId(OdDb::kEdgeSubentType, edgePersSubentId->index1()));"
// see OdDbAssocEdgePersSubentId::getTransientSubentIds
// therefore this method is spare
//OdResult OdDbAssocPolylinePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity, 
//                                                                const OdDbAssocPersSubentId* pPerSubentId,
//                                                                OdArray<OdDbSubentId>& subents) const
//{
//  OdDbPolylinePtr polyLine = OdDbPolyline::cast(pEntity);
//  OdDbAssocEdgePersSubentIdPtr edgePersSubentId = OdDbAssocEdgePersSubentId::cast(pPerSubentId);
//  if (!polyLine.isNull() && !edgePersSubentId.isNull()) {
//    OdGsMarker idx = edgePersSubentId->index1();
//    unsigned int upTo = polyLine->numVerts() + (polyLine->isClosed()? 1 : 0);
//    for (unsigned int i = 1; i < upTo; i++) {
//      if (idx == polyLine->getVertexIdentifierAt(i - 1)) {
//        subents.append(OdDbSubentId(OdDb::kEdgeSubentType, i));
//        return eOk;
//      }
//    }
//  }
//  return eInvalidInput;
//}

OdResult OdDbAssocPolylinePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity, 
                                                                const OdDbAssocPersSubentId* pPerSubentId,
                                                                OdArray<OdDbSubentId>& subents) const
{
  const OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
  const OdDbAssocEdgePersSubentIdPtr pEdgePerSubentId = OdDbAssocEdgePersSubentId::cast(pPerSubentId);
  if (pPolyline.isNull() || pEdgePerSubentId.isNull())
    return eWrongObjectType;

  subents.clear();
  const OdGsMarker internalIdx = pEdgePerSubentId->index1();
  for (unsigned edgeIdx = 1; edgeIdx <= pPolyline->numVerts(); ++edgeIdx)
  {
    //const bool isLastVertex = edgeIdx == pPolyline->numVerts();
    if (internalIdx == pPolyline->getVertexIdentifierAt(edgeIdx - 1))
    {
      // AlexeyTyurin The subsequent analysis is by VertexIdentifier, not by segment index. See
      // OdDbAssocPolylinePersSubentIdPE::getEdgeSubentityGeometry below

      //subents.append(OdDbSubentId(OdDb::kEdgeSubentType, (!pPolyline->isClosed() && isLastVertex)
      //  ? edgeIdx - 1 : edgeIdx));
      subents.append(OdDbSubentId(OdDb::kEdgeSubentType, internalIdx));
    }
  }

  return (subents.isEmpty()) ? eInvalidInput : eOk;
//  TODO AlexeyTyurin 09222017
  //if (m_assocGeomDependency != NULL)
  //{
  //  OdDbAssocPersSubentIdPtr persSubEntIdPtr = m_assocGeomDependency->persistentSubentId();
  //  return persSubEntIdPtr->getTransientSubentIds(pEntity, subents);
  //}
  //else
  //{
  //  OdArray<OdDbSubentId> retArray;
  //  subents = retArray;
  //}

  //return eOk;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                            OdDb::SubentType subentType,
                                                            OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) )
    return eBadObjType;

  OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);

  if ( subentType == OdDb::kEdgeSubentType )
  {
    for ( unsigned int i = 1; i < pPolyline->numVerts(); i++ )
      allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, i));

    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    for ( unsigned int i = 1; i <= pPolyline->numVerts(); i++ )
    {
      allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, i));
      if (pPolyline->segType(i - 1) == OdDbPolyline::kArc)
        allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kCenterPLinePrefix + i));
      if (i < pPolyline->numVerts())
        allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kMiddlePLinePrefix + i));
    }

    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                   const OdDbSubentId&    edgeSubentId,
                                                                   OdDbSubentId&          startVertexSubentId,
                                                                   OdDbSubentId&          endVertexSubentId,
                                                                   OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return otherVertexSubentIds.length() == 0 ? eBadObjType : eOk;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                   const OdDbSubentId& edgeSubentId,
                                                                   OdGeCurve3d*&       pEdgeCurve)
{
  OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
  unsigned int segIdx;
  bool bLastSegment = false;

  if (pPolyline->hasVertexIdentifiers())
  {
    for (segIdx = 0; segIdx < pPolyline->numVerts(); ++segIdx)
    {
      if (pPolyline->getVertexIdentifierAt(segIdx) == edgeSubentId.index())
        break;
    }

    if (!pPolyline->isClosed() && segIdx == pPolyline->numVerts() - 1)// the case with the last vertex. It\s mean the last segment
    {
        segIdx--;
        bLastSegment = true;
    }
  }
  else
  {
    ///ODA_FAIL_ONCE();  // Have not seen yet
    segIdx = (unsigned int) edgeSubentId.index() - 1;

    if (!pPolyline->isClosed() && segIdx == pPolyline->numVerts() - 1)
    {
      segIdx--;
      bLastSegment = true;
    }
  }

  int indEnd = pPolyline->isClosed() ? 1 : 2;

  if (segIdx > pPolyline->numVerts() - indEnd)
    return eInvalidInput;

  OdDbPolyline::SegType segType = pPolyline->segType(segIdx);
  if (segType == OdDbPolyline::kLine)
  {
    OdGeLineSeg3d* pLineSeg = new OdGeLineSeg3d();
    pPolyline->getLineSegAt(segIdx, *pLineSeg);

    if (bLastSegment)
    {
        OdGePoint3d point1 = pLineSeg->startPoint();
        OdGePoint3d point2 = pLineSeg->endPoint();

        pLineSeg->set(point2, point1);
    }

    pEdgeCurve = pLineSeg;
  }
  else //if (segType == OdDbPolyline::kArc)
  {
    OdGeCircArc3d* pArcSeg = new OdGeCircArc3d();
    pPolyline->getArcSegAt(segIdx, *pArcSeg);
    pEdgeCurve = pArcSeg;
  }
  return eOk;
}

OdResult OdDbAssocPolylinePersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                   const OdDbSubentId& edgeSubentId,
                                                                   const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) )
  {
    // TODO
    return eNotImplementedYet;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPolylinePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                     const OdDbSubentId& vertexSubentId,
                                                                     OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(pEntity, vertexSubentId, OdDb::kVertexSubentType) )
  {
    OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
    unsigned int idx = (unsigned int) vertexSubentId.index() - 1;
    if (idx >= kMiddlePLinePrefix)
    {
      idx -= kMiddlePLinePrefix;
      OdDbSubentId edgeSubentId(OdDb::kEdgeSubentType, idx + 1);
      OdGeCurve3d *pCurve;
      OdResult res = getEdgeSubentityGeometry(pEntity, edgeSubentId, pCurve);
      if (res != eOk)
        return res;
      OdGeInterval interval;
      pCurve->getInterval(interval);
      double param = 0.5*(interval.lowerBound() + interval.upperBound());
      vertexPosition = pCurve->evalPoint(param);
      delete pCurve;
      return eOk;
    }
    if (idx >= kCenterPLinePrefix)
    {
      idx -= kCenterPLinePrefix;
      OdGeCircArc3d arc3d;
      pPolyline->getArcSegAt(idx, arc3d);
      vertexPosition = arc3d.center();
      return eOk;
    }
    pPolyline->getPointAt(idx, vertexPosition);
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPolylinePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                     const OdDbSubentId& vertexSubentId,
                                                                     const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(pEntity, vertexSubentId, OdDb::kVertexSubentType) )
  {
    // TODO
    return eNotImplementedYet;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocPolylinePersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                               const OdDbSubentId& subentId,
                                                               const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbPolyline::desc()) )
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

void OdDbAssocPolylinePersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                              const OdDbSubentId&    edgeSubentId,
                                                              OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  startVertexSubentId.setIndex(kStart);
}

void OdDbAssocPolylinePersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                            const OdDbSubentId&    edgeSubentId,
                                                            OdDbSubentId& endVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, endVertexSubentId);
  endVertexSubentId.setIndex(kEnd);
}

void OdDbAssocPolylinePersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                              const OdDbSubentId&    edgeSubentId,
                                                              OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    // TODO
    OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
    for ( unsigned int i = 0; i < pPolyline->numVerts(); i++ )
      otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, i));
  }
}

bool OdDbAssocPolylinePersSubentIdPE::checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( !isTypeEqual(pEntity, OdDbPolyline::desc()) )
    return false;
  OdDbPolylinePtr pPolyline = OdDbPolyline::cast(pEntity);
  unsigned int idx = (unsigned int) subentId.index();
  unsigned int numVerts = pPolyline->numVerts();
  if ( type == OdDb::kEdgeSubentType )
  {
    unsigned int maxSeg = pPolyline->isClosed() ? numVerts : numVerts - 1;
    return (idx >= 1 && idx <= maxSeg);
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    if (idx > kMiddlePLinePrefix)
      idx -= kMiddlePLinePrefix;
    if (idx > kCenterPLinePrefix)
      idx -= kCenterPLinePrefix;
    return (idx >= 1 && idx <= numVerts);
  }
  else
    return false;
}
