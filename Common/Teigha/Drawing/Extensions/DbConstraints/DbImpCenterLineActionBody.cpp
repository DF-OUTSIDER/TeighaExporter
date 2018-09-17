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

#include "DbImpCenterLineActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbBlockReference.h"
#include "DbAssocNetwork.h"
#include "DbBlockTable.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "Ge/GeLineSeg3d.h"
#include "DbAssocActionParam.h"
#include "Ge/GePointOnCurve3d.h"
#include "Ge/GeCurve3d.h"
#include "DbLinetypeTableRecord.h"
#include "DbDimAssoc.h"
#include "SaveState.h"

#define BREAK_TOLERANCE_MIN  1e-8

OdDbImpCenterLineActionBody::OdDbImpCenterLineActionBody()
{
}

OdDbImpCenterLineActionBody::~OdDbImpCenterLineActionBody()
{
}

OdResult OdDbImpCenterLineActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	OdResult res = OdDbImpSmartCenterActionBody::dwgInFields(pFiler);

	if (res != eOk)
		return res;

  m_pParams = OdDbSmartCenterLineParameters::createObject();

	return res;
}

void OdDbImpCenterLineActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
	OdDbImpSmartCenterActionBody::dwgOutFields(pFiler);
}

OdResult OdDbImpCenterLineActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	OdResult res = OdDbImpSmartCenterActionBody::dxfInFields(pFiler);

	if (res != eOk)
		return res;

	ODA_ASSERT_ONCE(pFiler->atEOF());
  
  m_pParams = OdDbSmartCenterLineParameters::createObject();

	return eOk;
}

void OdDbImpCenterLineActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
	OdDbImpSmartCenterActionBody::dxfOutFields(pFiler);
}

//const OdDbSmartCenterLineParametersPtr OdDbImpCenterLineActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterLineParametersPtr& OdDbImpCenterLineActionBody::parameters()
//{
//  return m_pParams;
//}

void OdDbImpCenterLineActionBody::composeForLoad(OdDbObject *pObject,
	OdDb::SaveType format,
	OdDb::DwgVersion version,
	OdDbAuditInfo* pAuditInfo)
{
  OdDbCenterLineActionBody *Body = static_cast<OdDbCenterLineActionBody *>(pObject);
  m_pParams->setOwner(Body);
}

void OdDbImpCenterLineActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbImpSmartCenterActionBody::evaluateOverride(parentActionId);
  OdGePoint3d intersectPoint(INT_MIN, INT_MIN, INT_MIN);
  OdGeLineSeg3d ptLine1;
  OdGeLineSeg3d ptLine2;
  OdArray<int>  refDirEndArr;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForWrite));

  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbBlockReferencePtr bRef = OdDbBlockReference::cast(bEntRef);

  if (bRef.isNull())
    return;

  OdDbAssocGeomDependencyPtr pGeomDep0 = getAssocGeomDependencyPtr(pAction, OdDb::kForWrite);
  OdDbAssocGeomDependencyPtr pGeomDep1 = getAssocGeomDependencyPtr(pAction, OdDb::kForWrite, 1);
  OdDbViewportPtr viewportV0 = getViewPortFromCompoundObject(pGeomDep0);
  OdDbViewportPtr viewportV1 = getViewPortFromCompoundObject(pGeomDep1);

  ODA_ASSERT_ONCE(viewportV0 == viewportV1);

  OdGeMatrix3d mViewTrans;
  double  viewScale = 1.0; 
  
  if (!viewportV0.isNull())
  {
    OdSaveStateFunc<OdDbViewport, OdGsView*> saveView(viewportV0, &OdDbViewport::gsView, &OdDbViewport::setGsView, NULL);

    viewScale = viewportV0->customScale();
    mViewTrans = OdDbPointRef::mswcsToPswcs(viewportV0);
  }

  edgeRefDirEndingArray(pAction, refDirEndArr);
  ODA_ASSERT(refDirEndArr.length() == 2);

  if (getCenterLineGeometry(ptLine1, 0, parentActionId) != eOk)
    return;

  if (getCenterLineGeometry(ptLine2, 1, parentActionId) != eOk)
    return;

  // Calculate intersection of two assotiated lines 
  bool ResOfIntersect = ptLine2.intersectWith(ptLine1, intersectPoint);
  bool bParallel = !ResOfIntersect && intersectPoint == OdGePoint3d(INT_MIN, INT_MIN, INT_MIN);

  OdDbBlockTableRecordPtr pBR = bRef->blockTableRecord().openObject(OdDb::kForWrite);
  OdDbDatabase *pDb = bRef->database();

  // setting "CENTER" parameters to BlockReference
  OdString centerMarklineType = subentLineType(pAction);
  OdDbLinetypeTableRecordPtr centerLTypeTableRec = OdDbImpSmartCenterActionBody::getCenterLType(pDb, centerMarklineType);
  OdDb::LineWeight lineWeight = OdDb::LineWeight(subentLineWeight(pAction));

  bRef->setLinetype(centerLTypeTableRec->objectId());
  bRef->setLinetypeScale(subentLinetypeScale(pAction));
  bRef->setLineWeight(OdDb::LineWeight(lineWeight));

  double lengtForCheckingIntersection = ptLine1.length() > ptLine2.length() ? ptLine1.length() : ptLine2.length();

  OdGeLineSeg3d dirSegOfLine, revertSegOfLine;
  OdGePoint3d dirStartPoint, revStartPoint, dirEndPoint, revEndPoint;

  //if (!bParallel)
  {
    // ABT: the case if intersectPoint is equal to one of line point and refDirEndArr doesn't match it ( vector  vFirst = dirStartPoint - intersectPoint;
    // or vSecond = dirEndPoint - intersectPoint; is zero ) I have got from ACAD file. Look like it's mistake for Solid
    if (intersectPoint == ptLine1.startPoint())
      refDirEndArr[0] = 1;

    if (intersectPoint == ptLine1.endPoint())
      refDirEndArr[0] = 0;

    if (refDirEndArr[0] == 0)
    {
      dirStartPoint = ptLine1.startPoint();
      revStartPoint = ptLine1.endPoint();
    }
    else
    {
      dirStartPoint = ptLine1.endPoint();
      revStartPoint = ptLine1.startPoint();
    }

    if (intersectPoint == ptLine2.startPoint())
      refDirEndArr[1] = 1;

    if (intersectPoint == ptLine2.endPoint())
      refDirEndArr[1] = 0;

    if (refDirEndArr[1] == 0)
    {
      dirEndPoint = ptLine2.startPoint();
      revEndPoint = ptLine2.endPoint();
    }
    else
    {
      dirEndPoint = ptLine2.endPoint();
      revEndPoint = ptLine2.startPoint();
    }
  }
  
  dirSegOfLine.set(dirStartPoint, dirEndPoint);
  revertSegOfLine.set(revStartPoint, revEndPoint);

  // The case of not parallel lines
  if (!bParallel)
  {
    OdGeVector3d vFirst, vSecond;
    OdGeLineSeg3d segFirst, segSecond;

    vFirst = dirStartPoint - intersectPoint;
    vSecond = dirEndPoint - intersectPoint;
    vFirst.setLength(lengtForCheckingIntersection);
    vSecond.setLength(lengtForCheckingIntersection);

    segFirst.set(intersectPoint, vFirst);
    segSecond.set(intersectPoint, vSecond);

    dirSegOfLine.set(segFirst.endPoint(), segSecond.endPoint());

    vFirst = revStartPoint - intersectPoint;
    vSecond = revEndPoint - intersectPoint;
    vFirst.setLength(lengtForCheckingIntersection);
    vSecond.setLength(lengtForCheckingIntersection);

    segFirst.set(intersectPoint, vFirst * 2);
    segSecond.set(intersectPoint, vSecond * 2);

    revertSegOfLine.set(segFirst.endPoint(), segSecond.endPoint());
  }

  OdGePoint3d dirSegPoint = dirSegOfLine.midPoint();
  OdGePoint3d revertSegPoint = revertSegOfLine.midPoint();
  OdGeLineSeg3d centerLineSeg;
  OdGePoint3d startPoint = dirSegPoint;
  OdGePoint3d endPoint = revertSegPoint;
  // The case of not parallel lines
  if (!bParallel)
  {
    centerLineSeg.set(dirSegPoint, revertSegPoint);
    dirSegOfLine.set(dirStartPoint, dirEndPoint);
    // bisectors should be collinear. If not only one taken in account
    if (OdZero (centerLineSeg.getDistanceToVector(intersectPoint)))
    {
      revertSegOfLine.set(revStartPoint, revEndPoint);
      centerLineSeg.intersectWith(dirSegOfLine, startPoint);
      centerLineSeg.intersectWith(revertSegOfLine, endPoint);
    }
    else
    {
      centerLineSeg.set(dirSegPoint, intersectPoint);
      centerLineSeg.intersectWith(dirSegOfLine, startPoint);
      endPoint = intersectPoint;
    }
  }

  centerLineSeg.set(startPoint, endPoint);
  // START <Calculating center position for BlockReference and points of center line> START 
  // center position for BlockReference is the center of center line extents
  //if (!ResOfIntersect && intersectPoint.x == 0 && intersectPoint.y == 0 && intersectPoint.z == 0)
  {
    intersectPoint = centerLineSeg.midPoint();
  }

  double extension = endExtension(pAction);
  OdGeVector3d v1 = startPoint - intersectPoint;

  if (extension == OdDbSmartCenterCommonParameters::MAX_EXTENSIONS())
  {
      v1.setLength(endOvershoot(pAction) * viewScale);
      ptLine1.set(startPoint, v1);
  }
  else
  {
      v1.setLength((extension  + endOvershoot(pAction)) * viewScale);
      ptLine1.set(intersectPoint, v1);
  }
  startPoint = ptLine1.endPoint();

  extension = startExtension(pAction);
  v1 = intersectPoint - startPoint;

  if (extension == OdDbSmartCenterCommonParameters::MAX_EXTENSIONS())
  {
      v1.setLength(startOvershoot(pAction) * viewScale);
      ptLine1.set(endPoint, v1);
  }
  else
  {
      v1.setLength((extension + startOvershoot(pAction)) * viewScale);
      ptLine1.set(intersectPoint, v1);
  }

  endPoint = ptLine1.endPoint();

  centerLineSeg.set(startPoint, endPoint);
  // START <Calculating center position for BlockReference and points of center line> 
  //START  center position for BlockReference is the center of center line extents
  OdGePoint3d blPosition = intersectPoint;
  double angle = 0;
  OdGeVector3d normal(0, 0, 1);

  if (!viewportV0.isNull())
  {
    OdDbBlockReferencePtr pCenterLineBlockRef = OdDbBlockReference::createObject();

    pCenterLineBlockRef->transformBy(mViewTrans);
    //angle = pCenterMarkBlockRef->rotation();
    //normal = pCenterMarkBlockRef->normal();
  }

  if (!bRef.isNull() && viewportV0 == viewportV1)
  {
    bRef->setNormal(normal); // inside the m_Position can be changed ones or twise, depend on normal of blockRef before. 
    bRef->setPosition(blPosition);
    bRef->setRotation(angle);
  }
  else
    return;
  
  endPoint.x = (centerLineSeg.startPoint().x - intersectPoint.x) ;
  endPoint.y = (centerLineSeg.startPoint().y - intersectPoint.y) ;
  endPoint.z = (centerLineSeg.startPoint().z - intersectPoint.z) ;

  startPoint.x = (centerLineSeg.endPoint().x - intersectPoint.x) ;
  startPoint.y = (centerLineSeg.endPoint().y - intersectPoint.y) ;
  startPoint.z = (centerLineSeg.endPoint().z - intersectPoint.z) ;
  // End <Calculating center position for BlockReference and points of center line> END 

  OdString centerLinelineType = subentLineType(pAction);
  OdDbObjectIteratorPtr entityIter = pBR->newIterator();

  OdDbLinePtr pLine;

  if (!entityIter->done())
  {
    pLine = entityIter->entity(OdDb::kForWrite);
  }
  else
  {
    pLine = OdDbLine::createObject();
  }

  pLine->setStartPoint(startPoint);
  pLine->setEndPoint(endPoint);

  if (!entityIter->done())
  {
    entityIter->step();
  }
  else
  {
    pBR->appendOdDbEntity(pLine);
  }

  pAction->setStatus(kIsUpToDateAssocStatus);
}

OdResult OdDbImpCenterLineActionBody::edgeRefDirEndingArray(OdDbAssocActionPtr pAction, OdArray<int> & outputArr)
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(-1);

  OdString expression(""), evaluatorId("");
  
  if (pAction->getValueParam(OD_T("EdgeRefDirectionEnding"), *value.get(), expression, evaluatorId, 0) != eOk)
    return eMakeMeProxy;
  
  while (!value.isNull())
  {
    int out = value->getInt32();
    outputArr.append(out);

    value = value->next();
  }

  return eOk;
}

OdResult OdDbImpCenterLineActionBody::setEdgeRefDirEndingArray(OdDbAssocActionPtr pAction, OdArray<int> & inputArr)
{
  OdString errorMessages;
  OdString expression(""), evaluatorId("");
  OdValue::UnitType unitType = OdValue::kUnitless;
  const int input = ((int)inputArr[0]);
  const int input2 = ((int)inputArr[1]);
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  OdDbEvalVariantPtr value2 = OdDbEvalVariant::init(input2);
  value->setNext(value2);
  
  return pAction->setValueParam(OD_T("EdgeRefDirectionEnding"), *value.get(), expression, evaluatorId, errorMessages, false);
}
// OVERSHOOTS REGION
enum OdResult OdDbImpCenterLineActionBody::setEndOvershoot(double newOvershoot, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("EndOvershoot"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterLineActionBody::setStartOvershoot(double newOvershoot, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("StartOvershoot"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

double OdDbImpCenterLineActionBody::endOvershoot(OdDbAssocActionPtr pAction) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");
  
  if (pAction->getValueParam(OD_T("EndOvershoot"), *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}

double OdDbImpCenterLineActionBody::startOvershoot(OdDbAssocActionPtr pAction) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("StartOvershoot"), *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}
// OVERSHOOTS REGION END
// EXTENSIONS REGION
enum OdResult OdDbImpCenterLineActionBody::setEndExtension(double newExtension, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newExtension);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("EndExtLength"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterLineActionBody::setStartExtension(double newExtension, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newExtension);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("StartExtLength"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

double OdDbImpCenterLineActionBody::endExtension(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("EndExtLength"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}

double OdDbImpCenterLineActionBody::startExtension(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("StartExtLength"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}
// OVERSHOOTS REGION END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OdResult OdDbImpCenterLineActionBody::createInstance(
  const OdDbFullSubentPathArray entPathArray,
  //OdGePoint3dArray ptsSelected,
  OdDbObjectId& centerLineId,
  OdDbObjectId& actionBodyId)
{
  OdArray<int> inputArr;

  OdDbDatabasePtr pDb;
  OdDbAssocActionPtr pAction;
  OdDbCenterLineActionBodyPtr pActionBody;
  OdDbAssocDependencyPtr pDependency;
  OdArray<OdDbAssocGeomDependencyPtr> pGeomDependencyArr;
  OdDbBlockTableRecordPtr pDestinationBTR;
  OdDbLinetypeTableRecordPtr centerLTypetableRec;
  OdDbBlockReferencePtr pCenterLineBlock;
  OdArray<OdGeLineSeg3d> segLineArr;

  OdResult res = getPtrsForTree(entPathArray, pDb, pAction, pActionBody, pDependency, pGeomDependencyArr, 
    pDestinationBTR, centerLTypetableRec, pCenterLineBlock, segLineArr, actionBodyId);

  if (res != eOk)
    return res;

  OdGePoint3d ptsSelected0 = segLineArr[0].midPoint();
  OdGePoint3d ptsSelected1 = segLineArr[1].midPoint();
  OdDbCompoundObjectIdPtr pCompoundObjectId;
  res = pGeomDependencyArr[0]->getDependentOnCompoundObject(pCompoundObjectId);

  //if (!pCompoundObjectId.isNull() && !pCompoundObjectId->isSimpleObjectId())
  //{
  //  OdGeMatrix3d mTrans;
  //  res = pCompoundObjectId->getTransform(mTrans);

  //  if (res != eOk)
  //    return res;

  //  ptsSelected0.transformBy(mTrans);
  //}

  //res = pGeomDependencyArr[1]->getDependentOnCompoundObject(pCompoundObjectId);

  //if (!pCompoundObjectId.isNull() && !pCompoundObjectId->isSimpleObjectId())
  //{
  //  OdGeMatrix3d mTrans;
  //  res = pCompoundObjectId->getTransform(mTrans);

  //  if (res != eOk)
  //    return res;

  //  ptsSelected1.transformBy(mTrans);
  //}

  OdGePoint3d intersectPoint(INT_MIN, INT_MIN, INT_MIN);
  OdGeLineSeg3d ptLineStartIntersect;
  OdGeLineSeg3d ptLineEndIntersect;
  OdGePointOnCurve3d closestPointN;
  bool ResOfIntersect = segLineArr[1].intersectWith(segLineArr[0], intersectPoint);

  inputArr.append(0);
  inputArr.append(1);
  // parallel case
  bool bParallel = !ResOfIntersect && intersectPoint == OdGePoint3d(INT_MIN, INT_MIN, INT_MIN);

  if (!bParallel)
  {
    ptLineStartIntersect.set(segLineArr[0].startPoint(), intersectPoint);
    ptLineEndIntersect.set(segLineArr[0].endPoint(), intersectPoint);

    if (ptLineEndIntersect.length() > ptLineStartIntersect.length())
    {
      if (ptLineEndIntersect.getNormalPoint(ptsSelected0, closestPointN))
      {
        inputArr[0] = 1;
      }
      else if (ptLineStartIntersect.getNormalPoint(ptsSelected0, closestPointN))
      {
        inputArr[0] = 0;
      }
    }
    else
    {
      if (ptLineStartIntersect.getNormalPoint(ptsSelected0, closestPointN))
      {
        inputArr[0] = 0;
      }
      else if (ptLineEndIntersect.getNormalPoint(ptsSelected0, closestPointN))
      {
        inputArr[0] = 1;
      }
    }

    //OdGePoint3d p0s = ptsSelected[0];
    //OdGePoint3d p1s = ptsSelected[1];

    //OdGePoint3d psls0 = segLineArr[0].startPoint();
    //OdGePoint3d psle0 = segLineArr[0].endPoint();

    //OdGePoint3d psls = segLineArr[1].startPoint();
    //OdGePoint3d psle = segLineArr[1].endPoint();

    ptLineStartIntersect.set(segLineArr[1].startPoint(), intersectPoint);
    ptLineEndIntersect.set(segLineArr[1].endPoint(), intersectPoint);

    if (ptLineEndIntersect.length() > ptLineStartIntersect.length())
    {
      if (ptLineEndIntersect.getNormalPoint(ptsSelected1, closestPointN))
      {
        inputArr[1] = 1;
      }
      else if (ptLineStartIntersect.getNormalPoint(ptsSelected1, closestPointN))
      {
        inputArr[1] = 0;
      }
    }
    else
    {
      if (ptLineStartIntersect.getNormalPoint(ptsSelected1, closestPointN))
      {
        inputArr[1] = 0;
      }
      else if (ptLineEndIntersect.getNormalPoint(ptsSelected1, closestPointN))
      {
        inputArr[1] = 1;
      }
    }
  }
  //Calculate EdgeRefDirEndingArray END
  //////////////////////////////////////////////////////////////////////
  return setParametersAndCreateCL(inputArr, pDb, pAction, pActionBody, pDependency,
    pDestinationBTR, centerLTypetableRec, pCenterLineBlock, centerLineId);
}

//////////////////////////////////
OdResult OdDbImpCenterLineActionBody::getPtrsForTree(
  const OdDbFullSubentPathArray entPathArray,
  OdDbDatabasePtr& pDb,
  OdDbAssocActionPtr& pAction,
  OdDbCenterLineActionBodyPtr& pActionBody,
  OdDbAssocDependencyPtr& pDependency,
  OdArray<OdDbAssocGeomDependencyPtr> & pGeomDependencyArr,
  OdDbBlockTableRecordPtr& pDestinationBTR,
  OdDbLinetypeTableRecordPtr& centerLTypetableRec,
  OdDbBlockReferencePtr& pCenterLineBlock,
  OdArray<OdGeLineSeg3d>& segLineArr,
  OdDbObjectId& actionBodyId
  )
{
  OdDbObjectIdArray sourceEntites;
  OdResult res = eOk;
  OdGeLineSeg3d segLine;
  OdDbObjectIdArray pathArray = entPathArray[0].objectIds();

  if (pathArray.length() < 1)
    return eInvalidInput;

  sourceEntites.append(pathArray[pathArray.length() - 1]);
  pDb = pathArray[0].database();

  pathArray.clear();
  pathArray = entPathArray[1].objectIds();

  if (pathArray.length() < 1)
    return eInvalidInput;

  sourceEntites.append(pathArray[pathArray.length() - 1]);

  OdDbBlockTablePtr pMainBT = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId ownerBTRId = pathArray[0].openObject()->ownerId();
  OdDbBlockTableRecordPtr pOwnerSpace = ownerBTRId.safeOpenObject(OdDb::kForWrite);
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(pOwnerSpace->objectId(), true);

  //create destination BTR
  pDestinationBTR = OdDbBlockTableRecord::createObject();
  OdString destinationBTRname = OD_T("*U");

  pDestinationBTR->setName(destinationBTRname);
  OdDbObjectId interID = pDb->addOdDbObject(pDestinationBTR, pMainBT->objectId());

  //create center line block reference
  pCenterLineBlock = OdDbBlockReference::createObject();
  //From 11941: For properties of Block Reference :
  //While creating new : setDatabaseDefaults() plus
  //  CENTERLAYER, CENTERLTSCALE, CENTERLTYPE
  pCenterLineBlock->setDatabaseDefaults(pDb);

  centerLTypetableRec = OdDbImpSmartCenterActionBody::getCenterLType(pDb);
  OdDbObjectId layer = getCenterLayerId(pDb);
  double scale = pDb->getCENTERLTSCALE();

  pCenterLineBlock->setLinetype(centerLTypetableRec->objectId());
  pCenterLineBlock->setLayer(layer);
  pCenterLineBlock->setLinetypeScale(scale);
  //End first part of "From 11941" //////////////////////////////////////////////////////////////////////
  pCenterLineBlock->setBlockTableRecord(pDestinationBTR->objectId());
  pDb->addOdDbObject(pCenterLineBlock, ownerBTRId);

  //create action and action body
  //BEGIN
  OdRxClass* pClass;
  pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbCenterLineActionBody")).get();

  OdDbObjectId actionId;

  if (eOk != createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId))
  {
    return eInvalidInput;
  }

  //append BTR and block ref
  pMainBT->add(pDestinationBTR);
  pOwnerSpace->appendOdDbEntity(pCenterLineBlock);

  pActionBody =
    OdDbAssocActionBody::cast(actionBodyId.openObject(OdDb::kForWrite));

  pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  pAction->setActionBody(actionBodyId);
  //END

  //create dependency
  //BEGIN
  //geometry reference
  //OdDbAssocGeomDependencyPtr pGeomDependency[2];
  pGeomDependencyArr.append(OdDbAssocGeomDependency::createObject());
  pDb->addOdDbObject(pGeomDependencyArr[0]);
  pGeomDependencyArr[0]->setIsReadDependency(true);
  pGeomDependencyArr[0]->setIsWriteDependency(false);

  OdDbObjectPtr assocOb0 = sourceEntites[0].openObject(OdDb::kForWrite);
  OdDbFullSubentPath entPath = entPathArray[0];
  OdUInt32 subentIndex = entPath.subentId().index();
  
  if (entPath.subentId().type() != OdDb::kNullSubentType)
  {
    pGeomDependencyArr[0]->createNewEdgePersSubent(assocOb0, pAction, subentIndex);//, bNewPersSubentIndex);//, false);
  }
  else
    pGeomDependencyArr[0]->createNewSingleEdgePersSubent(assocOb0->objectId());

  pActionBody->addSubentitySpecialSettings(assocOb0, entPathArray[0].subentId(), subentIndex);
  //pDependency->setOrder(0 - 2147483648u);
  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->setFullPath(entPath.objectIds());//, OdDbDatabase* pHostDatabase = NULL)
  pGeomDependencyArr[0]->attachToObject(pCompoundObjectId);
  pAction->addDependency(pGeomDependencyArr[0]->objectId(), true);

  //the next geometry reference
  pGeomDependencyArr.append(OdDbAssocGeomDependency::createObject());
  pDb->addOdDbObject(pGeomDependencyArr[1]);
  pGeomDependencyArr[1]->setIsReadDependency(true);
  pGeomDependencyArr[1]->setIsWriteDependency(false);
  OdDbObjectPtr assocOb1 = sourceEntites[1].openObject(OdDb::kForWrite);
  
  entPath = entPathArray[1];
  subentIndex = entPath.subentId().index();

  if (entPath.subentId().type() != OdDb::kNullSubentType)
  {
    pGeomDependencyArr[1]->createNewEdgePersSubent(assocOb1, pAction, subentIndex);//, false);
  }
  else
    pGeomDependencyArr[1]->createNewSingleEdgePersSubent(assocOb1->objectId());

  pActionBody->addSubentitySpecialSettings(assocOb1, entPathArray[1].subentId(), subentIndex);
  //pDependency->setOrder(0 - 2147483648u);
  pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->setFullPath(entPath.objectIds());//, OdDbDatabase* pHostDatabase = NULL)
  pGeomDependencyArr[1]->attachToObject(pCompoundObjectId);
  pAction->addDependency(pGeomDependencyArr[1]->objectId(), true);

  //center line block reference
  pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject(pDependency);
  pDependency->setIsReadDependency(true);
  pDependency->setIsWriteDependency(true);
  //pDependency->setOrder(0 - 2147483648u);
  pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->set(pCenterLineBlock->objectId());
  pDependency->attachToObject(pCompoundObjectId);
  pAction->addDependency(pDependency->objectId(), true);

  if (pActionBody->getCenterLineGeometry(segLine, 0) != eOk)
  {
    return eInvalidInput;
  }

  segLineArr.append(segLine);

  if (pActionBody->getCenterLineGeometry(segLine, 1) != eOk)
  {
    return eInvalidInput;
  }

  segLineArr.append(segLine);
  // EdgeAction references
  for (int i = 0; i < 2; i++)
  {
    int paramIndex = i;
    OdDbObjectId paramId;
    OdString paramName = OD_T("InputEdge");
    pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbAssocEdgeActionParam")).get();
    OdDbAssocEdgeActionParamPtr pEdgeActionParam;
    OdDbVertexRefPtr pNormalVertexRef;
    switch (pAction->addParam(paramName, pClass, paramId, paramIndex))
    {
    case eOk:
      pEdgeActionParam = OdDbAssocEdgeActionParam::cast(paramId.openObject(OdDb::kForWrite));
      break;
    case eAlreadyInDb:
      pEdgeActionParam = OdDbAssocEdgeActionParam::cast(paramId.openObject(OdDb::kForRead));
      break;
    default:
      ODA_FAIL_ONCE();
    }

    if (!pEdgeActionParam.isNull())
    {
      pCompoundObjectId = OdDbCompoundObjectId::createObject();
      pCompoundObjectId->setEmpty();
      pCompoundObjectId->set(pGeomDependencyArr[i]->objectId());

      OdResult res = pEdgeActionParam->setObject(//actionId,
        pCompoundObjectId,
        true,
        true);

      OdDbCurvePtr refCurve = OdDbCurve::cast(sourceEntites[i].safeOpenObject());

      if (!refCurve.isNull())
      {
        pEdgeActionParam->setEdgeSubentityGeometry(&(segLineArr[i]));
      }
    }
  }

  return res;
}

OdResult OdDbImpCenterLineActionBody::setParametersAndCreateCL(
  OdArray<int> inputArr,
  OdDbDatabasePtr pDb,
  OdDbAssocActionPtr pAction,
  OdDbCenterLineActionBodyPtr pActionBody,
  OdDbAssocDependencyPtr pDependency,
  OdDbBlockTableRecordPtr pDestinationBTR,
  OdDbLinetypeTableRecordPtr centerLTypetableRec,
  OdDbBlockReferencePtr pCenterLineBlock,
  OdDbObjectId& centerLineId
  )
{
  ODA_ASSERT(inputArr.length() == 2);
  OdResult res = eSubSelectionSetEmpty;

  if (inputArr.length() == 2)
  {
    double scale = pDb->getCENTERLTSCALE();
    OdString expression, evaluatorId, pErrorMessage;
    OdDbSmartCenterLineParametersPtr pParameters = OdDbSmartCenterLineParameters::createObject();

    pParameters->setEdgeRefDirEndingArray(inputArr, expression, evaluatorId, pErrorMessage);
    pParameters->setCenterDepId(pDependency->objectId(), expression, evaluatorId, pErrorMessage);
    ////From 11941: While recalculating values from Parameters should be used.
    OdDbImpSmartCenterLineParameters*  pImpParameters = (OdDbImpSmartCenterLineParameters*)(pParameters->impSmartCenterParameters());
    setSubentLineWeight(pCenterLineBlock->lineWeight(), pAction);
    OdString centerLineLineType = centerLTypetableRec->getName();
    pImpParameters->setSubentLineType(centerLineLineType, expression, evaluatorId, pErrorMessage);
    pImpParameters->setSubentLinetypeScale(scale, expression, evaluatorId, pErrorMessage);

    double exe = pDb->getCENTEREXE();
    pImpParameters->setStartOvershoot(exe, expression, evaluatorId, pErrorMessage);
    pImpParameters->setEndOvershoot(exe, expression, evaluatorId, pErrorMessage);
    // End the part of 11941
    pActionBody->parameters() = pParameters;
    const_cast<OdDbSmartCenterLineParametersPtr &>(pParameters)->setOwner(pActionBody);

    OdDbLinePtr pLine = OdDbLine::createObject();

    pDestinationBTR->appendOdDbEntity(pLine);
    ////From 11941: For all entity properties of lines inside block I suggest to set ByBlock and
    //  layer "0"
    pLine->setDatabaseDefaults(pDb);
    pLine->setLinetype(pDb->getLinetypeByBlockId());
    pLine->setLinetypeScale(scale);
    pLine->setLineWeight(OdDb::kLnWtByBlock);
    pLine->setColor(OdCmEntityColor::kByBlock);
    pLine->setLayer(pDb->getLayerZeroId());

    centerLineId = pLine->objectId();
    res = eOk;
    //End the part of "From 11941" //////////////////////////////////////////////////////////////////////
  }

  return res;
}
//////////////////////////////////

OdResult OdDbImpCenterLineActionBody::getCenterLineGeometry(OdGeLineSeg3d & refLineSegment, bool secondSeg, OdDbObjectId parentActionId)
{
  OdResult res;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));

  if (pAction.isNull())
  {
    ODA_ASSERT(!L"Action is Null");
    return eNullObjectId;
  }
  OdDbAssocGeomDependencyPtr pGeomDep, pFirstDep;
  OdDbObjectIdArray idDependencies;

  res = pAction->getDependencies(true, false, idDependencies);

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
    return eNullObjectId;
  }
  
  OdArray<OdGeCurve3d*> arrCurve;
  res = pGeomDep->getEdgeSubentityGeometry(arrCurve);
  if (res != eOk)
  {
    ODA_ASSERT(!L"No edge geometry");
    return res;
  }
  
  ODA_ASSERT(arrCurve.length() == 1 && arrCurve[0] != NULL);

  res = eBadObjType;

  if (arrCurve[0] != NULL)
  {
    if (arrCurve[0]->isKindOf(OdGe::kLineSeg3d))
    {
      refLineSegment = *((OdGeLineSeg3d*)arrCurve[0]);
      res = eOk;
    }
    else
    {
      ODA_ASSERT_ONCE(!L"Unknown edge geometry");
    }
  }
  for (OdArray<OdGeCurve3d*>::iterator pCurve = arrCurve.begin(); pCurve != arrCurve.end(); ++pCurve)
    delete (*pCurve);

  return res;
}

OdResult OdDbImpCenterLineActionBody::createInstance(
  const OdDbFullSubentPathArray entPathArray,
  OdArray<int> inputArr,
  OdDbObjectId& centerLineId,
  OdDbObjectId& actionBodyId)
{
  ODA_ASSERT(inputArr.length() == 2);

  OdDbDatabasePtr pDb;
  OdDbAssocActionPtr pAction;
  OdDbCenterLineActionBodyPtr pActionBody;
  OdDbAssocDependencyPtr pDependency;
  OdArray<OdDbAssocGeomDependencyPtr> pGeomDependencyArr;
  OdDbBlockTableRecordPtr pDestinationBTR;
  OdDbLinetypeTableRecordPtr centerLTypetableRec;
  OdDbBlockReferencePtr pCenterLineBlock;
  OdArray<OdGeLineSeg3d> segLineArr;
   
  OdResult res = getPtrsForTree(entPathArray, pDb, pAction, pActionBody, pDependency, pGeomDependencyArr,
    pDestinationBTR, centerLTypetableRec, pCenterLineBlock, segLineArr, actionBodyId);

  if (res != eOk)
    return res;

  return setParametersAndCreateCL(inputArr, pDb, pAction, pActionBody, pDependency,
    pDestinationBTR, centerLTypetableRec, pCenterLineBlock, centerLineId);
}
