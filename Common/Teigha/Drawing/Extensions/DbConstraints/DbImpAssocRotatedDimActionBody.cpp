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
#include "NextCodeDefs.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbAssocDependency.h"
#include "DbImpAssocRotatedDimActionBody.h"
#include "DbObjectId.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeCircArc3d.h"
#include "DbAssocNetwork.h"
#include "DbDimStyleTable.h"
#include "DbLine.h"
#include "DbSubentId.h"
#include <typeinfo>
//////////////////////////////////////////////////////////////////////////

OdDbImpAssocAnnotationActionBody::OdDbImpAssocAnnotationActionBody()
{
  m_undefined2 = 0;
  m_ActionParamID1 = NULL;
  m_ActionParamID2 = NULL;
  m_ActionParamID3 = NULL;
  m_undefinedAnnotationActionBody = 0;
  //m_ActionBodyType = kUnDefined; TODO
  m_paramBasedActionBody = NULL;
}

OdDbImpAssocAnnotationActionBody::~OdDbImpAssocAnnotationActionBody()
{
  if (m_paramBasedActionBody == NULL)
		return;

	delete m_paramBasedActionBody;
}

OdResult OdDbImpAssocAnnotationActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	const OdResult res = OdDbImpAssocActionBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  if (pFiler->dwgVersion() > OdDb::vAC24)
  {
    m_undefined2 = pFiler->rdInt16();
    m_assocDependencyID = pFiler->rdSoftPointerId();
  }
  
  if (pFiler->dwgVersion() <= OdDb::vAC24 && m_paramBasedActionBody != NULL)
  {
      m_paramBasedActionBody->dwgInFields(pFiler);
      m_arrParams = m_paramBasedActionBody->m_arrParams;
      m_arrValueParams = m_paramBasedActionBody->m_arrValueParams;
	  m_assocDependencyID = m_paramBasedActionBody->assocDependencyID();
  }



  return eOk;
}

void OdDbImpAssocAnnotationActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocActionBody::dwgOutFields(pFiler);

  if (pFiler->dwgVersion() > OdDb::vAC24)
  {
    pFiler->wrInt16(m_undefined2);
    pFiler->wrSoftPointerId(m_assocDependencyID);
  }

  if (pFiler->dwgVersion() <= OdDb::vAC24 && m_paramBasedActionBody != NULL)
  {
      m_paramBasedActionBody->m_arrParams = m_arrParams;
      m_paramBasedActionBody->m_arrValueParams = m_arrValueParams;
	  m_paramBasedActionBody->assocDependencyID() = m_assocDependencyID;
      m_paramBasedActionBody->dwgOutFields(pFiler);
  }
}

OdResult OdDbImpAssocAnnotationActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  //OdResult res = OdDbImpAssocActionBody::dxfInFields(pFiler);

  //if (res != eOk)
  //  return res;

  if (!pFiler->atSubclassData(OdDbAssocActionBody::desc()->name()))
  {
	  ODA_FAIL_ONCE();
	  return eMakeMeProxy;
  }

  NEXT_CODE(90)
	m_dwgVersion = pFiler->rdUInt32();

  if (m_dwgVersion != (pFiler->dwgVersion() > OdDb::vAC24 ? 2 : 1))
  {
	  ODA_FAIL_ONCE();
	  return eMakeMeProxy;
  }

  if (pFiler->dwgVersion() > OdDb::vAC24)
  {
    NEXT_CODE(90)
    m_undefined2 = pFiler->rdUInt32();
    NEXT_CODE(330)
    m_assocDependencyID = pFiler->rdObjectId();
  }

  //if (pFiler->atSubclassData(OdDbAssocParamBasedActionBody::desc()->name()) && m_paramBasedActionBody != NULL)
  if (m_paramBasedActionBody != NULL)
  {
	  const OdResult res = m_paramBasedActionBody->dxfInFields(pFiler);
	  if (res != eOk)
	    return res;

      m_arrParams = m_paramBasedActionBody->m_arrParams;
      m_arrValueParams = m_paramBasedActionBody->m_arrValueParams;
	  m_assocDependencyID = m_paramBasedActionBody->assocDependencyID();
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocAnnotationActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  //OdDbImpAssocActionBody::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(OdDbAssocActionBody::desc()->name());
  pFiler->wrUInt32(90, pFiler->dwgVersion() > OdDb::vAC24 ? 2 : 1);

  if (pFiler->dwgVersion() > OdDb::vAC24)
  {
    pFiler->wrUInt32(90, m_undefined2);
    pFiler->wrObjectId(330, m_assocDependencyID);
  }
  
  if (pFiler->dwgVersion() <= OdDb::vAC24  && m_paramBasedActionBody != NULL)
  {
      //pFiler->wrSubclassMarker(OdDbAssocParamBasedActionBody::desc()->name());
      m_paramBasedActionBody->m_arrParams = m_arrParams;
      m_paramBasedActionBody->m_arrValueParams = m_arrValueParams;
	    m_paramBasedActionBody->assocDependencyID() = m_assocDependencyID;
      m_paramBasedActionBody->dxfOutFields(pFiler);
  }
}

void OdDbImpAssocAnnotationActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  //OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForWrite));

  //OdDbObjectIdArray objIDs;

  ////Array entity dependency check
  ////BEGIN
  //pAction->getDependencies(true, true, objIDs);
}

void OdDbImpAssocAnnotationActionBody::evaluateOverrideAssocDim(OdDbObjectId parentActionId, OdGePoint3d* ptPoint, OdGePoint3d* ptDimPoint, const int pointNum)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();
  OdGeCurve3d* geometry = NULL;
  //// Get viewport and customscale
  //double viewportScale = 1;

  //OdArray<OdDbViewportPtr> viewportEdgeActionArray;
  //viewportEdgeActionArray.resize(pointNum);

  //OdArray<OdDbBlockReferencePtr> blockRefArray;
  //blockRefArray.resize(pointNum);

  OdArray<OdDbAssocGeomDependencyPtr> geomDepArray;
  geomDepArray.resize(pointNum);
  //OdDbViewportPtr pViewport = objIdVp.openObject();

  //if (!pViewport.isNull())
  //  viewportScale = pViewport->customScale();

  for (int i = 0; i < pointNum; i++)
  {
    geomDepArray[i] = getAssocGeomDependencyPtr(pAction, OdDb::kForWrite, i);
    //geomDepArray[i]->getViewPortOrBlRefFromFullSubentPath(viewportEdgeActionArray[i], blockRefArray[i]);

    //if (pViewport.isNull())
    //{
    //  if (i > 0)
    //    ODA_ASSERT(viewportEdgeActionArray[0] == viewportEdgeActionArray[1]);

    //  pViewport = viewportEdgeActionArray[i];

    //  //if (!pViewport.isNull())
    //  //  viewportScale = pViewport->customScale();
    //}

    if (getGeometryAndSidePointForDimension(geometry, ptPoint[i], i, pAction) != eOk)
      return;

    OdDbViewportPtr pViewport;// TODO
    setDataToActionParam(geometry, snapPointParams[i], ptPoint[i], ptDimPoint[i], parentActionId/*, blockRefArray[i], pViewport*/);

    delete geometry;
    geometry = NULL;
  }
  
  //viewportEdgeActionArray.clear();
  //blockRefArray.clear();
  geomDepArray.clear();
}

OdDbObjectId OdDbImpAssocAnnotationActionBody::getViewPortIdFromDependency(OdDbObjectId parentActionId) const
{
  OdDbObjectIdArray objIDs;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForRead));
  pAction->getDependencies(true, false, objIDs);

	for (unsigned int i = 0; i < objIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDepArrayEntity = objIDs[i].openObject();
    if (pDepArrayEntity.isNull())
      continue;

    OdDbAssocGeomDependencyPtr depGeomPtr = OdDbAssocGeomDependency::cast(pDepArrayEntity);

    if (!pDepArrayEntity->isWriteDependency() && depGeomPtr.isNull())
    {
      OdDbObjectId objIdDep = OdDbAssocDependency::cast(pDepArrayEntity->objectId().openObject())->dependentOnObject();
      OdDbViewportPtr objIdviewPort = OdDbViewport::cast(objIdDep.openObject());

      if (!objIdviewPort.isNull())
        return objIdDep;
    }
  }

  return OdDbObjectId();
}

OdDbObjectId OdDbImpAssocAnnotationActionBody::getArrayEntity(OdDbObjectId parentActionId) const
{
  OdDbObjectIdArray objIDs;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForRead));
  pAction->getDependencies(false, true, objIDs);

  return OdDbAssocDependency::cast(getIdForFirstWriteDependency(objIDs).openObject())->dependentOnObject();
}

OdDbObjectId OdDbImpAssocAnnotationActionBody::getIdForFirstWriteDependency(OdDbObjectIdArray depIDs)
{
  for (unsigned int i = 0; i < depIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDepEntity = depIDs[i].openObject();
    if (pDepEntity.isNull())
      continue;

    if (pDepEntity->isWriteDependency())
    {
      return pDepEntity->objectId();
    }
  }

  ODA_ASSERT_ONCE(!"Entity dep not found");
  return OdDbObjectId();
}

OdResult OdDbImpAssocAnnotationActionBody::setDataToActionParam(OdGeCurve3d* geometry, OdDbObjectId snapPointParamId, OdGePoint3d& ptPoint, OdGePoint3d ptDimPoint,
  OdDbObjectId parentActionId/*, OdDbBlockReferencePtr pBlockRef, OdDbViewportPtr pViewport*/)
{
  OdDbAssocOsnapPointRefActionParamPtr pSnapPointParam =
    OdDbAssocOsnapPointRefActionParam::cast(snapPointParamId.openObject(OdDb::kForWrite));

  if (!pSnapPointParam.isNull())
  {
    OdGeLineSeg3d line;
    OdDbObjectIdArray edgeParams = pSnapPointParam->ownedParams();
    OdDbObjectId assocActionParam = pSnapPointParam->paramAtIndex(0);
    OdDbAssocVertexActionParamPtr pVertexParam =
      OdDbAssocVertexActionParam::cast(assocActionParam.openObject(OdDb::kForWrite));

    if (!pVertexParam.isNull())
    {
      pVertexParam->setOwnerId(parentActionId);
      line = *((OdGeLineSeg3d*)geometry);

      if (!ptPoint.isEqualTo(ptDimPoint))
        ptPoint = line.startPoint();

      pSnapPointParam->setOsnapMode(OdDb::kOsModeEnd);
      pSnapPointParam->setParam(-1);

      //if (!pBlockRef.isNull())
      //{
      //  ptPoint.set(ptPoint.x + pBlockRef->position().x, ptPoint.y + pBlockRef->position().y, ptPoint.z + pBlockRef->position().z);
      //}

      //if (!pViewport.isNull())
      //{
      //  transformPositionByViewPort(pViewport, ptPoint);
      //}
      
      pVertexParam->setVertexRef(OdDbVertexRef(ptPoint), false, false, 0);
    }
    else
    {
      OdGeCircArc3d arc;
      OdDbEdgeRef edgeRef;
      OdDbEdgeRef edgeRefNext;
      OdResult ret;
      OdSharedPtr<OdGeCurve3d> pCurve3d;

      OdDbAssocEdgeActionParamPtr pEdgeParam =
        OdDbAssocEdgeActionParam::cast(assocActionParam.openObject(OdDb::kForWrite));
      {
        OdArray<OdDbEdgeRef> edgeRefs;
        pEdgeParam->setOwnerId(parentActionId);
        OdResult res = pEdgeParam->getEdgeRef(edgeRefs);

        if (edgeRefs.length() > 0)
        {
          edgeRef = edgeRefs[0];
        }
        
        if (edgeRef.curve() == NULL)
        {
          if (geometry->isKindOf(OdGe::kLine3d))
            pCurve3d = new OdGeLineSeg3d();

          if (geometry->isKindOf(OdGe::kLineSeg3d))
            pCurve3d = new OdGeLineSeg3d();

          if (geometry->isKindOf(OdGe::kCircArc3d))
            pCurve3d = new OdGeCircArc3d();

          ODA_ASSERT(!pCurve3d.isNull());

          if (pCurve3d.isNull())
            return eInvalidDrawing;

          edgeRef.setCurve(pCurve3d);
        }
	      if (edgeRef.curve()->type() == 23 && geometry->isKindOf(OdGe::kLineSeg3d))// any point on line
        {
          line = *((OdGeLineSeg3d*)geometry);
          double ratio = 1.0;

          OdGePoint3d ptPointStGeometry = line.startPoint();
          //if (!pBlockRef.isNull())
          //{
          //  ptPointStGeometry.set(ptPointStGeometry.x + pBlockRef->position().x, ptPointStGeometry.y + pBlockRef->position().y, ptPointStGeometry.z + pBlockRef->position().z);
          //}

          OdGePoint3d ptPointEnd = line.endPoint();

          //if (!pBlockRef.isNull())
          //{
          //  ptPointEnd.set(ptPointEnd.x + pBlockRef->position().x, ptPointEnd.y + pBlockRef->position().y, ptPointEnd.z + pBlockRef->position().z);
          //}

          OdGeLineSeg3d* lineGeom = (OdGeLineSeg3d*)geometry;
          OdGeVector3d vecGeomLine = ptPointEnd - ptPointStGeometry;
          lineGeom->set(ptPointStGeometry, vecGeomLine);

          //if (!pViewport.isNull())
          //{
          //  transformPositionByViewPort(pViewport, ptPointStGeometry);
          //  transformPositionByViewPort(pViewport, ptPointEnd);
          //  vecGeomLine = ptPointEnd - ptPointStGeometry;
          //}
          
          if (!pCurve3d.isNull())
          {
            ret = resetEdgeRefs(pEdgeParam, geometry,/* ptPoint,*/ parentActionId, edgeRef);

            if (ret != eOk)
              return ret;
          }

          OdGeLineSeg3d lineRef = *(OdGeLineSeg3d*)edgeRef.curve();

          OdGePoint3d ptPointSt = lineRef.startPoint();
          ptPointEnd = lineRef.endPoint();

          //if (!pViewport.isNull())
          //{
          //  transformPositionByViewPort(pViewport, ptPointSt);
          //  transformPositionByViewPort(pViewport, ptPointEnd);
          //}

          OdGeLineSeg3d lineRefView(ptPointSt, ptPointEnd);
          ODA_ASSERT(lineRefView.isOn(ptDimPoint));

          OdGeVector3d vec = ptDimPoint - ptPointSt;
          OdGeVector3d vecLineRf = ptPointEnd - ptPointSt;
          ratio = vec.length() / vecLineRf.length();
          OdGePlane plane;
          if (lineRef.startPoint().isEqualTo(ptDimPoint))
          {
            pSnapPointParam->setOsnapMode(OdDb::kOsModeStart);
            pSnapPointParam->setParam(0);
          }
          else if (lineRef.endPoint().isEqualTo(ptDimPoint))
          {
            pSnapPointParam->setOsnapMode(OdDb::kOsModeEnd);
            pSnapPointParam->setParam(1.0);
          }
          else
          {
            pSnapPointParam->setOsnapMode(OdDb::kOsModeNear);
            pSnapPointParam->setParam(ratio);
          }

          if (pCurve3d.isNull()) // pCurve3d was set ( pCurve3d.IsNull( ) == fals ) if evaluate of existing (not created here) edgeRef
          {
            vecGeomLine.setLength(vecGeomLine.length() * ratio);
            ptPoint = ptPointStGeometry + vecGeomLine;
          }
          else
            ptPoint = ptDimPoint;
          //try
          //{
          //	//line.getPerpPlane(ptDimPoint, plane);
          //	//line.intersectWith(plane, ptPoint);
          //} 
          //catch (OdError &)
          //{
          //	ODA_ASSERT(!L"Geometry is wrong");
          //}
        }

        if (pCurve3d.isNull())
        {
          ret = resetEdgeRefs(pEdgeParam, geometry,/* ptPoint,*/ parentActionId, edgeRef);

          if (ret != eOk)
            return ret;
        }

        arc = *(OdGeCircArc3d*)edgeRef.curve();
      }
      // the case of two edgeRefs (the point on the arc and line of center mark)
      if (edgeParams.length() > 1)
      {
        OdDbObjectId assocActionParamNext = pSnapPointParam->paramAtIndex(1);

        pEdgeParam =
          OdDbAssocEdgeActionParam::cast(assocActionParamNext.openObject(OdDb::kForWrite));

        ret = resetEdgeRefs(pEdgeParam, geometry, /*ptPoint,*/ parentActionId, edgeRefNext);
        
        if (ret == eOk)
        {
          ODA_ASSERT(edgeRefNext.curve()->type() == 23);

          line = *(OdGeLineSeg3d*)edgeRefNext.curve();
          
          OdGePoint3d p2;
          int numInt = 0;
          arc.intersectWith(line, numInt, ptPoint, p2);
          ODA_ASSERT(numInt == 1);
        }
			}
      // recalculate the point of dimension for the arc segment
			else if (edgeParams.length() == 1)
			{
				if (edgeRef.curve()->type() == 11)// any point on Arc
				{
					OdGePoint3d cen = arc.center();

					if (!cen.isEqualTo(ptDimPoint))// point of dimension is not a center of arc 
					{
						OdGeVector3d vec = ptDimPoint - cen;
						OdGePoint3d p2;
						int numInt = 0;

						vec.setLength(arc.radius() * 100);
						line.set(cen, vec);
						arc.intersectWith(line, numInt, ptPoint, p2);
						ODA_ASSERT(numInt == 1);
					}
				}
			}
		}
  }

  return eOk;
}

OdResult OdDbImpAssocAnnotationActionBody::resetEdgeRefs(OdDbAssocEdgeActionParamPtr pEdgeParam, OdGeCurve3d* geometry, /*OdGePoint3d ptPoint,*/ OdDbObjectId parentActionId, OdDbEdgeRef& edgeRef)
{
  OdResult ret = eInvalidDrawing;
  OdGeCircArc3d* arc = NULL;
  OdGeLineSeg3d* line = NULL;;

  ret = eOk;
	const OdInt32 entId = edgeRef.curve()->type();

  switch (entId)
  {
    case 23: //case OdGe::kLineSeg3d: // to regenerate DbLine in m_dbEntIds 
    {
      ODA_ASSERT(geometry->isKindOf(OdGe::kLineSeg3d));
      OdGeLineSeg3d* lineGeom = (OdGeLineSeg3d*)geometry;
	    const OdGePoint3d ptSG = lineGeom->startPoint();
	    const OdGePoint3d ptEG = lineGeom->endPoint();
	    const OdGeVector3d vec = ptEG - ptSG;

      line = (OdGeLineSeg3d*)edgeRef.curve();
      line->set(ptSG, vec);
      //edgeRefs[0] = OdDbEdgeRef(lineGeom);
    }
      break;
    case 17: //case OdGe::kEllipArc3d: // to regenerate DbEllipse in m_dbEntIds // x3a8
              //OdGeDxfIO::outFields(pFiler, *(OdGeEllipArc3d*)m_edgeRef.curve());
      break;
    case 11: //case OdGe::kCircArc3d: // to regenerate DbArc in m_dbEntIds // budweiser2013.dwg
    {
      ODA_ASSERT(geometry->isKindOf(OdGe::kCircArc3d));
      OdGeCircArc3d refCircle = *((OdGeCircArc3d*)geometry);
      OdGePoint3d center = refCircle.center();
      //ODA_ASSERT(ptPoint.isEqualTo(refCircle.center()));
      //arc = (OdGeCircArc3d*)edgeRefs[0].curve();
      arc = (OdGeCircArc3d*)edgeRef.curve();
      arc->setCenter(center);
      arc->setRadius(refCircle.radius());

      //arc->setCenter(ptPoint);
      //arc->setRadius(refCircle.radius());
    }
    break;
    case 42: //case OdGe::kNurbCurve3d: // to regenerate DbSpline in m_dbEntIds // budweiser2013.dwg x12fa8
              // TODO move kSpunSurface and kSegmentChain2d to end of enum // via 
              //OdGeDxfIO::outFields(pFiler, *(OdGeNurbCurve3d*)m_edgeRef.curve());
      break;
    case 19: //case OdGe::kLine3d: // to regenerate DbLine in m_dbEntIds 
              //OdGeDxfIO::outFields(pFiler, *(OdGeLine3d*)m_edgeRef.curve());
      break;
    case 47:
      //OdGeDxfIO::outFields(pFiler, *(OdGeCompositeCurve3d*)m_edgeRef.curve());
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      ret = eInvalidDrawing;
      //throw OdError(eNotImplementedYet);
  }

  pEdgeParam->setEdgeRef(edgeRef, false, false, 0);
  
  return ret;
}
  OdResult OdDbImpAssocAnnotationActionBody::getGeometryAndSidePointForDimension(OdGeCurve3d* & geometry, OdGePoint3d & refPoint, int segNumber, OdDbAssocActionPtr pAction)
  {
	  bool secondSeg = segNumber > 0;

    if (pAction.isNull())
    {
      ODA_ASSERT(!L"Action is Null");
      return eNullObjectId;
    }
    OdDbAssocGeomDependencyPtr pGeomDep, pFirstDep;
    OdDbObjectIdArray idDependencies;

    OdResult res = pAction->getDependencies(true, false, idDependencies);

    if (res != eOk)
      return res;

    for (unsigned int i = 0; i < idDependencies.length(); i++)
    {
      pGeomDep = OdDbAssocGeomDependency::cast(idDependencies[i].openObject());

      if (!pGeomDep.isNull())
      {
        if (!secondSeg && pFirstDep.isNull())
        {
          break;
        }

        if (secondSeg && !pFirstDep.isNull())
        {
          break;
        }

        pFirstDep = pGeomDep;
      }
    }

    if (pGeomDep.isNull())
    {
      ODA_ASSERT(!L"Dependency is Null");
      return eNullObjectId;\
    }

    OdGePoint3dArray vertexPositions;

    res = pGeomDep->getVertexSubentityGeometryFromPersSubentiy(geometry, vertexPositions);

    if (res != eOk)
    {
      ODA_ASSERT(!L"No vertex geometry");
      return res;
    }

    ODA_ASSERT(vertexPositions.length() == 1);

    if (vertexPositions.length() >= 1)
    {
      refPoint = vertexPositions[0];
      res = eOk;
    }
    else
    {
      res = eBadObjType;
    }

    return res;
  }

  OdDbObjectId OdDbImpAssocAnnotationActionBody::createOdDbAssocOSnapPointRefActionParam(const OdDbObjectId actionId,const OdDbObjectId source, OdGePoint3d ptPoint1, const OdDbAssocGeomDependencyPtr pGeomDependency1, 
    const OdDbAssocGeomDependencyPtr pGeomDependency2, OdDbObjectId& anchorId, const PointPositionOnBlockLine isStartPoint, int paramIndex)
  {
    OdDbObjectPtr ptObj;
    OdDbSmartCenterActionBodyPtr smartObj;

    if (!source.isNull())
    {
      ptObj = source.openObject();
      smartObj = OdDbSmartCenterActionBody::cast(ptObj);
    }

    OdDbObjectId paramId;

	  const OdString paramName = OD_T(" ");
	//OdString paramName = OD_T("");
    OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("ACDBASSOCOSNAPPOINTREFACTIONPARAM")).get();
    OdDbAssocOsnapPointRefActionParamPtr pOSnapActionParam;
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));

    switch (pAction->addParam(paramName, pClass, paramId, paramIndex))
    {
    case eOk:
      pOSnapActionParam = OdDbAssocOsnapPointRefActionParam::cast(paramId.openObject(OdDb::kForWrite));
      break;
    case eAlreadyInDb:
      pOSnapActionParam = OdDbAssocOsnapPointRefActionParam::cast(paramId.openObject(OdDb::kForRead));
      break;
    default:
      ODA_FAIL_ONCE();
    }

    if (!pOSnapActionParam.isNull())
    {
        OdDbAssocActionParamPtr pAssocActionParam;
				int paramIndex = 0;
				OdDbObjectId paramId;
				//OdString paramName = OD_T("");
				OdString paramName = OD_T(" ");

				if (smartObj.isNull() || isStartPoint == kOnLine)
				{
					//paramName = OD_T(" ");
					pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbAssocEdgeActionParam")).get();
				}
				else
				{
					pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbAssocVertexActionParam")).get();
				}

				switch (pOSnapActionParam->addChildParam(actionId, paramName, pClass, anchorId, paramId, paramIndex))
				{
				case eOk:
				{
					pAssocActionParam = OdDbAssocActionParam::cast(paramId.openObject(OdDb::kForWrite));
				}
				break;
				case eAlreadyInDb:
					pAssocActionParam = OdDbAssocActionParam::cast(paramId.openObject(OdDb::kForRead));
					break;
				default:
					ODA_FAIL_ONCE();
				}

				if (!pAssocActionParam.isNull())
				{
					OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
					pCompoundObjectId->set(pGeomDependency1->objectId());

					OdResult res = pAssocActionParam->setObjectOnly(
					pCompoundObjectId,
					true,
					false);
        }
				
				anchorId = pOSnapActionParam->objectId();
    }

    ODA_ASSERT(!paramId.isNull());
    return paramId;
  }

  OdDbObjectId OdDbImpAssocAnnotationActionBody::getAssocDependencyForAction(const OdDbObjectId parentActionId)
  {
    OdDbObjectIdArray objIDs;
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForRead));
    pAction->getDependencies(false, true, objIDs);

    return OdDbAssocDependency::cast(getIdForFirstWriteDependency(objIDs).openObject())->dependentOnObject();
  }

  void OdDbImpAssocAnnotationActionBody::CreateGeomDependencies(const OdDbFullSubentPath entPath, const OdDbObjectId actionId, const OdDbObjectId source, 
    OdDbAssocGeomDependencyPtr& pGeomDependency1, OdDbAssocGeomDependencyPtr& pGeomDependency2, PointPositionOnBlockLine& isStartPoint)
  {
    OdDbSmartCenterActionBodyPtr smartObj;

    if (!source.isNull())
    {
      smartObj = OdDbSmartCenterActionBody::cast(source.openObject());
    }
    else
    {
      return;
    }

    OdDbDatabase *pDb = source.database();
    OdDbObjectId compoundObjID = source;// .openObject()->objectId();
    OdDbBlockReferencePtr bRef;

    if (!smartObj.isNull())
    {
      OdDbObjectId sourceActionID = smartObj->ownerId();
      OdDbAssocActionPtr pSourceAction = OdDbAssocAction::cast(sourceActionID.openObject());

      bRef = OdDbBlockReference::cast(getAssocDependencyForAction(sourceActionID).safeOpenObject());

      if (!bRef.isNull())
      {
        compoundObjID = bRef->objectId();
      }
      else
        return;
    }
    
    int paramIndex = 0;
    OdDbObjectId paramId;
  	OdString paramName = OD_T("");
    OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("ACDBASSOCOSNAPPOINTREFACTIONPARAM")).get();
    OdDbAssocOsnapPointRefActionParamPtr pOSnapActionParam;
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));

	  const OdDbObjectIdArray path = entPath.objectIds();
    pGeomDependency1 = OdDbAssocGeomDependency::createObject();
    pDb->addOdDbObject(pGeomDependency1);
    pGeomDependency1->setIsReadDependency(true);
    pGeomDependency1->setIsWriteDependency(false);
    pGeomDependency1->setOrder(2147483647u);
    OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();

    if (!smartObj.isNull())
    {
      isStartPoint = kOnLine;// kUndefined;
      pGeomDependency1->createNewAssocSimplePersSubent(bRef->objectId(), OdDb::kVertexSubentType, isStartPoint);
    }
    else
    {
      OdDbObjectPtr assocOb = source.openObject(OdDb::kForWrite);

      if (entPath.subentId().type() != OdDb::kNullSubentType)
      {
        OdUInt32 segInd = entPath.subentId().index() - 1;
        pGeomDependency1->createNewEdgePersSubent(assocOb, pAction, segInd);// , false);
      }
      else
        pGeomDependency1->createNewSingleEdgePersSubent(assocOb->objectId());
    }

    pCompoundObjectId->setFullPath(path);

    if (!pGeomDependency1.isNull() && !pCompoundObjectId.isNull())
    {
      pGeomDependency1->attachToObject(pCompoundObjectId);
      pAction->addDependency(pGeomDependency1->objectId(), true);
    }

    ODA_ASSERT(!pGeomDependency1.isNull());
  }

  OdDbObjectId OdDbImpAssocAnnotationActionBody::GetLineIDByPoint(OdDbBlockReferencePtr bRef, OdGePoint3d ptPoint, PointPositionOnBlockLine&  isStartPoint)
  {
    OdDbBlockTableRecordPtr pBR = bRef->blockTableRecord().openObject();
    OdGePoint3d blockPosition = bRef->position();
    OdGeMatrix3d matrix = bRef->blockTransform();

    OdDbObjectIteratorPtr pObjIt = pBR->newIterator();

    for (; !pObjIt->done(); pObjIt->step())
    {
      OdDbObjectId tempId = pObjIt->entity()->objectId();
      OdDbLinePtr pLineInBlock = OdDbLine::cast(tempId.openObject(OdDb::kForWrite));
      
      if (!pLineInBlock.isNull())
      {
        OdGePoint3d tempStartPoint = pLineInBlock->startPoint();
        OdGePoint3d tempEndPoint = pLineInBlock->endPoint();
      
        tempStartPoint.transformBy(matrix);
        tempEndPoint.transformBy(matrix);

        OdGeLineSeg3d segLine(tempStartPoint, tempEndPoint);

        if (tempStartPoint.isEqualTo(ptPoint))
        {
          isStartPoint = kStartPoint;
          return tempId;
        }
        else
        {
          if (tempEndPoint.isEqualTo(ptPoint))
          {
            isStartPoint = kEndPoint;
            return tempId;
          }
          else if (segLine.isOn(ptPoint))
          {
            isStartPoint = kOnLine;
            return tempId;
          }
        }
      }
    }

    return NULL;
  }

  void OdDbImpAssocAnnotationActionBody::setAssocDependencyID(OdDbObjectId assocDependencyID)
  {
    m_assocDependencyID = assocDependencyID;
  }

  void OdDbImpAssocAnnotationActionBody::setActionParamID1(OdDbObjectId actionParamID1)
  {
    m_ActionParamID1 = actionParamID1;
  }

  void OdDbImpAssocAnnotationActionBody::setActionParamID2(OdDbObjectId actionParamID1)
  {
    m_ActionParamID2 = actionParamID1;
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OdDbImpAssocRotatedDimActionBody::OdDbImpAssocRotatedDimActionBody()
{
//  m_ActionBodyType = kRotatedDim;
  m_paramBasedActionBody = new OdDbImpAssocParamBasedActionBody(true);
}

OdDbImpAssocRotatedDimActionBody::~OdDbImpAssocRotatedDimActionBody()
{
}

OdResult OdDbImpAssocRotatedDimActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	const OdResult res = OdDbImpAssocAnnotationActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_undefinedAnnotationActionBody = pFiler->rdInt16();
  m_ActionParamID1 = pFiler->rdSoftPointerId();
  m_ActionParamID2 = pFiler->rdSoftPointerId();

  return res;
}

void OdDbImpAssocRotatedDimActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dwgOutFields(pFiler);
  pFiler->wrInt16(m_undefinedAnnotationActionBody);
  pFiler->wrSoftPointerId(m_ActionParamID1);
  pFiler->wrSoftPointerId(m_ActionParamID2);
}

OdResult OdDbImpAssocRotatedDimActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	const OdResult res = OdDbImpAssocAnnotationActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocRotatedDimActionBody::desc()->dxfName()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90);
  m_undefinedAnnotationActionBody = pFiler->rdUInt32();
  NEXT_CODE(330);
  m_ActionParamID1 = pFiler->rdObjectId();
  NEXT_CODE(330);
  m_ActionParamID2 = pFiler->rdObjectId();

  return eOk;
}

void OdDbImpAssocRotatedDimActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocRotatedDimActionBody::desc()->dxfName());
  pFiler->wrUInt32(90, m_undefinedAnnotationActionBody);
  pFiler->wrObjectId(330, m_ActionParamID1);
  pFiler->wrObjectId(330, m_ActionParamID2);
}

//const OdDbSmartCenterMarkParametersPtr OdDbImpAssocRotatedDimActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpAssocRotatedDimActionBody::parameters()
//{
//  return m_pParams;
//}
//
void OdDbImpAssocRotatedDimActionBody::composeForLoad(OdDbObject *pObject,
  OdDb::SaveType format,
  OdDb::DwgVersion version,
  OdDbAuditInfo* pAuditInfo)
{
  //OdDbObjectId actionID = Body->ownerId();
  //OdDbAssocActionPtr assocAct = OdDbAssocAction::cast(actionID.openObject(OdDb::kForWrite));

  OdDbAssocOsnapPointRefActionParamPtr param1, param2;

  if (!m_ActionParamID1.isNull())
	  param1 = OdDbAssocOsnapPointRefActionParam::cast(m_ActionParamID1.openObject(OdDb::kForWrite));

  if (!m_ActionParamID2.isNull())
	  param2 = OdDbAssocOsnapPointRefActionParam::cast(m_ActionParamID2.openObject(OdDb::kForWrite));

  if (!param1.isNull())
  {
	  if ( m_arrParams.size() == 0)
	  {
		  m_arrParams.append(param1->id());
	  }
  }

  if (!param2.isNull())
  {
	  if (m_arrParams.size() < 2 )
	  {
		  m_arrParams.append(param2->id());
	  }
  }
}


void OdDbImpAssocRotatedDimActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite)); 
  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbRotatedDimensionPtr objDim = OdDbRotatedDimension::cast(bEntRef);

  if (objDim.isNull())
    return;

  int const pointNum = 2;
  OdGePoint3d ptPoint[pointNum];
  OdGePoint3d ptDimPoint[pointNum];
  ODA_ASSERT(pAction->ownedParams().length() == pointNum);
  ptDimPoint[0] = objDim->xLine1Point();
  ptDimPoint[1] = objDim->xLine2Point();

  evaluateOverrideAssocDim(parentActionId, ptPoint, ptDimPoint, pointNum);

  if (!objDim.isNull())
  {
    objDim->setXLine1Point(ptPoint[0]);
    
    if (pAction->ownedParams().length() == pointNum)
      objDim->setXLine2Point(ptPoint[1]);
    
    OdDbObjectIdArray objIDs;
    pAction->getDependencies(true, true, objIDs);

    OdDbAssocDependencyPtr pDepArrayEntity;
    for (unsigned int i = 0; i < objIDs.length(); i++)
    {
      pDepArrayEntity = objIDs[i].openObject(OdDb::kForWrite);
      pDepArrayEntity->setStatus(kIsUpToDateAssocStatus, false);
    }

  }
  else
  { 
    ODA_ASSERT(false);
    return;
  }
}

OdResult OdDbImpAssocRotatedDimActionBody::createInstance(
	const OdDbFullSubentPathArray entPathArray,
	OdGePoint3dArray ptsSelected,
	OdDbObjectId& actionBodyId, OdString createAssocAlignedDim)
{
  OdDbObjectIdArray sourceEntites;
  OdDbObjectId selectedEntId1;
  OdDbSmartCenterActionBodyPtr smartBody1, smartBody2;
	OdDbObjectIdArray pathArray = entPathArray[0].objectIds();

  if (pathArray.length() < 1)
    return eInvalidInput;

  OdDbObjectId ownerBTRId = pathArray[0].openObject()->ownerId();
  OdDbBlockTableRecordPtr pOwnerSpace = ownerBTRId.safeOpenObject(OdDb::kForWrite);
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(pOwnerSpace->objectId(), true);
  OdDbViewportPtr viewportV;
  OdDbDatabase *pDb;

  if (!pathArray.isEmpty())
  {
    pDb = pathArray[0].database();

    viewportV = OdDbViewport::cast(pathArray[0].openObject());

    if (!viewportV.isNull())
    {
      pathArray.removeFirst();
    }

    selectedEntId1 = pathArray.first();
    OdDbBlockReferencePtr pBody1 = OdDbBlockReference::cast(selectedEntId1.openObject(OdDb::kForRead));

    if (!pBody1.isNull())
    {
      for (OdUInt32 i = 1; i < pathArray.length(); i++)
      {
        OdDbObjectId bodyId1 = OdDbAssocArrayActionBody::getControllingActionBody(pBody1);
       
        if (!bodyId1.isNull())
        {
          smartBody1 = OdDbSmartCenterActionBody::cast(bodyId1.openObject());
          break;
        }
        else
          pBody1 = OdDbBlockReference::cast(pathArray[i].openObject(OdDb::kForRead));
      }
    }
  }

  OdDbFullSubentPath pathDep0(pathArray, entPathArray[0].subentId());

  pathArray = entPathArray[1].objectIds();
  OdDbObjectId selectedEntId2;

  if (!pathArray.isEmpty())
  {
    viewportV = OdDbViewport::cast(pathArray[0].openObject());

    if (!viewportV.isNull())
    {
      pathArray.removeFirst();
    }

    selectedEntId2 = pathArray.first();
    OdDbBlockReferencePtr pBody2 = OdDbBlockReference::cast(selectedEntId2.openObject(OdDb::kForRead));

    if (!pBody2.isNull())
    {
      for (OdUInt32 i = 1; i < pathArray.length(); i++)
      {
        OdDbObjectId bodyId2 = OdDbAssocArrayActionBody::getControllingActionBody(pBody2);

        if (!bodyId2.isNull())
        {
          smartBody2 = OdDbSmartCenterActionBody::cast(bodyId2.openObject());
          break;
        }
        else
          pBody2 = OdDbBlockReference::cast(pathArray[i].openObject(OdDb::kForRead));
      }
    }
  }

  OdDbFullSubentPath pathDep1(pathArray, entPathArray[1].subentId());

	const OdDbObjectId transferredID1 = smartBody1.isNull() ? selectedEntId1 : smartBody1->objectId();
	const OdDbObjectId transferredID2 = smartBody2.isNull() ? selectedEntId2 : smartBody2->objectId();
  sourceEntites.append(transferredID1);
  sourceEntites.append(transferredID2);
  
///////////////////////////////////////////////////////////////////////
  OdResult res = eOk;
  OdGePoint3d dimLinePt = ptsSelected[2];

  if (sourceEntites[0].isNull() && sourceEntites[1].isNull())
    return eInvalidInput;

	//OdDbSmartCenterActionBodyPtr smartObj1, smartObj2;
  //if (!sourceEntites[0].isNull())
  //{
  //  OdDbObjectPtr ptObj1 = sourceEntites[0].openObject();
  //}

  //if (!sourceEntites[1].isNull())
  //{
  //  OdDbObjectPtr ptObj2 = sourceEntites[1].openObject();
  //}

  //if (smartObj1.isNull() && smartObj2.isNull()) TODO
  //  return eInvalidInput;
  if (pDb == NULL)
    pDb = sourceEntites[1].database();

  //create action and action body
  //BEGIN
  OdString pClassName = OD_T("AcDbAssoc") + createAssocAlignedDim + OD_T("DimActionBody");
	OdRxClass * pClass = (OdRxClass*)odrxClassDictionary()->getAt(pClassName).get();

  OdDbObjectId actionId;

  if (eOk != createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId))
  {
    return eInvalidInput;
  }

  OdDbAssocAnnotationActionBodyPtr pActionBody =
    OdDbAssocActionBody::cast(actionBodyId.openObject(OdDb::kForWrite));

  ODA_ASSERT(!pActionBody.isNull());

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  pAction->setActionBody(actionBodyId);

  /**********************************************************************/
  /* Create an rotated dimension and dimension the ends of the line     */
  /**********************************************************************/
  pClassName = OD_T("AcDb") + createAssocAlignedDim + OD_T("Dimension");
  pClass = (OdRxClass*)odrxClassDictionary()->getAt(pClassName).get();
  OdDbDimensionPtr pDimension = OdDbDimension::cast(pClass->create());
  OdDbRotatedDimensionPtr pDimensionRot = OdDbRotatedDimension::cast(pDimension);
  OdDbAlignedDimensionPtr pDimensionAlig = OdDbAlignedDimension::cast(pDimension);
  pDimension->setDatabaseDefaults(pDb);
  pOwnerSpace->appendOdDbEntity(pDimension);

  OdDbDimStyleTablePtr pTable = pDb->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);
  /**********************************************************************/
  /* Add the DimStyle                                                   */
  /**********************************************************************/
	const OdDbObjectId odaDimStyleId = pDb->getDimStyleStandardId();
  
  //transformPositionByViewPort(viewportV, ptsSelected[0]);
  //transformPositionByViewPort(viewportV, ptsSelected[1]);
  //transformPositionByViewPort(viewportV, dimLinePt);

  if (!pDimensionRot.isNull())
  {
    pDimensionRot->setDimensionStyle(odaDimStyleId);
    pDimensionRot->setXLine1Point(ptsSelected[0]);
    pDimensionRot->setXLine2Point(ptsSelected[1]);
    pDimensionRot->setDimLinePoint(dimLinePt);
    pDimensionRot->setJogSymbolHeight(1.5);
    //pDimensionRot->setRotation(1.57);
  }

  if (!pDimensionAlig.isNull())
  {
    pDimensionAlig->setDimensionStyle(odaDimStyleId);
    pDimensionAlig->setXLine1Point(ptsSelected[0]);
    pDimensionAlig->setXLine2Point(ptsSelected[1]);
    pDimensionAlig->setDimLinePoint(dimLinePt);
    pDimensionAlig->setJogSymbolHeight(1.5);
  }

  pDimension->useDefaultTextPosition();
  pDimension->subClose();

  OdDbObjectId anchorId;

  PointPositionOnBlockLine isStartPoint1, isStartPoint2;

  OdDbAssocGeomDependencyPtr pGeomDependency1, pGeomDependency2, pGeomDependency3, pGeomDependency4;
  if (!sourceEntites[0].isNull())
  {
    CreateGeomDependencies(pathDep0, actionId, sourceEntites[0], pGeomDependency1, pGeomDependency2, isStartPoint1);
  }

  if (!sourceEntites[1].isNull())
  {
    CreateGeomDependencies(pathDep1, actionId, sourceEntites[1], pGeomDependency3, pGeomDependency4, isStartPoint2);
  }

	OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject(pDependency);
  pDependency->setIsReadDependency(true);
  pDependency->setIsWriteDependency(true);

  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->set(pDimension->objectId());
  pDependency->attachToObject(pCompoundObjectId);
  pAction->addDependency(pDependency->objectId(), true);
  pActionBody->setAssocDependencyID(pDependency->objectId());

  if (!viewportV.isNull())
  {
    pDependency = OdDbAssocDependency::createObject();
    pDb->addOdDbObject(pDependency);
    pDependency->setIsReadDependency(true);
    pDependency->setIsWriteDependency(false);

    pCompoundObjectId = OdDbCompoundObjectId::createObject();
    pCompoundObjectId->set(viewportV->objectId());
    pDependency->attachToObject(pCompoundObjectId);
    pAction->addDependency(pDependency->objectId(), true);
    pActionBody->setAssocDependencyID(pDependency->objectId());
  }

  if (!sourceEntites[0].isNull())
  {
	  const OdDbObjectId snapParamID1 = createOdDbAssocOSnapPointRefActionParam(actionId, sourceEntites[0], ptsSelected[0], pGeomDependency1, pGeomDependency2, anchorId, isStartPoint1, 0);
    pActionBody->setActionParamID1(snapParamID1);
  }

  if (!sourceEntites[1].isNull())
  {
	  const OdDbObjectId snapParamID2 = createOdDbAssocOSnapPointRefActionParam(actionId, sourceEntites[1], ptsSelected[1], pGeomDependency3, pGeomDependency4, anchorId, isStartPoint2, 1);
    pActionBody->setActionParamID2(snapParamID2);
  }

  return res;
}

OdStringArray  OdDbImpAssocRotatedDimActionBody::compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
	OdStringArray retArr;
  OdDbImpAssocRotatedDimActionBody* comparedBody = static_cast<OdDbImpAssocRotatedDimActionBody *>(bodyIDToCompare);
  ODA_ASSERT(comparedBody != NULL);

  if (comparedBody != NULL)
  {
    //OdDbImpAssocRotatedDimActionBody start
    if (m_undefinedAnnotationActionBody != comparedBody->m_undefinedAnnotationActionBody)
      retArr.append(CS_UNDEFINED_ANNOTATION_ACTION_BODY);

    if (typeid(m_paramBasedActionBody) != typeid(comparedBody->m_paramBasedActionBody))
      retArr.append(CS_PARAM_BASED_ACTION_BODY);
    //OdDbImpAssocRotatedDimActionBody end

    //DbImpAssocAnnotationActionBody
    if (typeid(*(m_ActionParamID1.openObject())) != typeid(*(comparedBody->m_ActionParamID1.openObject())))
      retArr.append(CS_DIFFERENT_OBJECTS);

    if (typeid(*(m_ActionParamID2.openObject())) != typeid(*(comparedBody->m_ActionParamID2.openObject())))
      retArr.append(CS_DIFFERENT_OBJECTS);

    if (typeid(*(m_assocDependencyID.openObject())) != typeid(*(comparedBody->m_assocDependencyID.openObject())))
      retArr.append(CS_DIFFERENT_OBJECTS);

    if (m_undefined2 != comparedBody->m_undefined2)
      retArr.append(CS_UNDEFINED_PROPERTY);
    //OdDbImpAssocAnnotationActionBody
  }

	return retArr;
}
