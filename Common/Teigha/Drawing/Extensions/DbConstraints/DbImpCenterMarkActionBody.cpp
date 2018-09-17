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
#include "DbImpCenterMarkActionBody.h"
#include "DbBlockReference.h"
#include "DbBlockTableRecord.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "DbAssocNetwork.h"
#include "DbBlockTable.h"
#include "OdDToStr.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "DbAssocActionParam.h"
#include "Ge/GeCircArc3d.h"
#include "DbImpSmartCenterObjectsParameters.h"
#include "DbLayerTableRecord.h"
#include "DbLinetypeTableRecord.h"
#include "DbDimAssoc.h"
#include "SaveState.h"

OdDbImpCenterMarkActionBody::OdDbImpCenterMarkActionBody()
{
}

OdDbImpCenterMarkActionBody::~OdDbImpCenterMarkActionBody()
{
}

OdResult OdDbImpCenterMarkActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	OdResult res = OdDbImpSmartCenterActionBody::dwgInFields(pFiler);

	if (res != eOk)
		return res;

  m_pParams = OdDbSmartCenterMarkParameters::createObject();

	return res;
}

void OdDbImpCenterMarkActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
	OdDbImpSmartCenterActionBody::dwgOutFields(pFiler);
}

OdResult OdDbImpCenterMarkActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	OdResult res = OdDbImpSmartCenterActionBody::dxfInFields(pFiler);
	
	if (res != eOk)
		return res;

	ODA_ASSERT_ONCE(pFiler->atEOF()); 

  m_pParams = OdDbSmartCenterMarkParameters::createObject();

  return eOk;
}

void OdDbImpCenterMarkActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
	OdDbImpSmartCenterActionBody::dxfOutFields(pFiler);
}

//const OdDbSmartCenterMarkParametersPtr OdDbImpCenterMarkActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpCenterMarkActionBody::parameters()
//{
//  return m_pParams;
//}
//
void OdDbImpCenterMarkActionBody::composeForLoad(OdDbObject *pObject,
  OdDb::SaveType format,
  OdDb::DwgVersion version,
  OdDbAuditInfo* pAuditInfo)
{
  OdDbCenterMarkActionBody *Body = static_cast<OdDbCenterMarkActionBody *>(pObject);
  m_pParams->setOwner(Body);
  //m_pParams->composeForLoad(format, version, pAuditInfo);
  //m_matrix = OdDbBlockReference::cast(getArrayEntity(Body->parentAction()).safeOpenObject())->blockTransform();
}

enum OdResult OdDbImpCenterMarkActionBody::setRotation(double rotation, const OdDbObjectId parentActionId)
{
	OdDbBlockReferencePtr bRef = OdDbBlockReference::cast(getArrayEntity(parentActionId).safeOpenObject(OdDb::kForWrite));
  OdString expression, evaluatorID, errorMSG;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForWrite));
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(0);
  
  bRef->setRotation(rotation);
  pAction->setValueParam(OD_T("AssociationToBeBroken"), *value.get(), expression, evaluatorID, errorMSG, false, 0);
  value = OdDbEvalVariant::init(rotation);
  pAction->setValueParam(OD_T("CachedRotation"), *value.get(), expression, evaluatorID, errorMSG, false, 0);

  return eOk;
}

double OdDbImpCenterMarkActionBody::rotation(const OdDbObjectId &parentActionId) const
{
	OdDbBlockReferencePtr bRef = OdDbBlockReference::cast(getArrayEntity(parentActionId).safeOpenObject());
	return bRef->rotation();
}

#include "OdDbAssocPersSubentIdPE.h"  //To be removed
OdResult OdDbImpCenterMarkActionBody::getRefCircle(OdGeCircArc3d &refCircle, const OdDbObjectId &parentActionId)
{
  OdResult res;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));

  if (pAction.isNull())
  {
    ODA_ASSERT(! L"Action is Null");
    return eNullObjectId;
  }

  OdDbAssocGeomDependencyPtr pGeomDep;
  pGeomDep = getAssocGeomDependencyPtr(pAction);

  if (pGeomDep.isNull())
  {
    //ODA_ASSERT(! L"Dependency is Null");
    // It's allowed if copy CM without object to assotiate
    return eNullObjectId;
  }

  OdDbEntityPtr pEnt = OdDbEntity::cast(pGeomDep->dependentOnObject().openObject());

  if (pEnt.isNull())
      return eNullObjectId;

  OdArray<OdGeCurve3d*> arrCurve;
  res = pGeomDep->getEdgeSubentityGeometry(arrCurve);

  if ( res != eOk )
  {
      ODA_ASSERT(! L"No edge geometry");
      return res;
  }
  ODA_ASSERT(arrCurve.length() == 1);
  OdGeCircArc3d refCircle1;
  
  if (NULL != arrCurve[0] && arrCurve[0]->isKindOf(OdGe::kCircArc3d))
  {
    refCircle = *((OdGeCircArc3d*)arrCurve[0]);
    res = eOk;
  }
  else
  {
    ODA_ASSERT_ONCE(! L"Unknown edge geometry");
    res = eBadObjType;
  }

  for(OdArray<OdGeCurve3d*>::iterator pCurve = arrCurve.begin(); pCurve != arrCurve.end(); ++pCurve)
    delete (*pCurve);

  return res;
}

void OdDbImpCenterMarkActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdGeCircArc3d arc;
  if (getRefCircle(arc, parentActionId) != eOk)
    return;

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForWrite));
  OdDbCenterMarkActionBodyPtr pActionBody = OdDbCenterMarkActionBody::cast(pAction->actionBody().openObject());
  double radius = arc.radius();

  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;
  
  OdDbBlockReferencePtr bRef = OdDbBlockReference::cast(bEntRef);
  OdDbAssocGeomDependencyPtr pGeomDep = getAssocGeomDependencyPtr(pAction, OdDb::kForWrite);

  OdGeVector3d normal = arc.normal();
  OdGePoint3d ocsCenter = arc.center();
  double angle = 0;
  
  if (!bRef.isNull())
    angle = bRef->rotation();

  OdDbViewportPtr viewportV = getViewPortFromCompoundObject(pGeomDep);

  if (!viewportV.isNull())
  {
    OdDbBlockReferencePtr pCenterMarkBlockRef = OdDbBlockReference::createObject();
    OdSaveStateFunc<OdDbViewport, OdGsView*> saveView(viewportV, &OdDbViewport::gsView, &OdDbViewport::setGsView, NULL);
    OdGeMatrix3d mViewTrans = OdDbPointRef::mswcsToPswcs(viewportV);

    pCenterMarkBlockRef->transformBy(mViewTrans);
    angle = pCenterMarkBlockRef->rotation();
  }

  if (!bRef.isNull())
  {
    bRef->setNormal(normal); // inside the m_Position can be changed ones or twise, depend on normal of blockRef before. 
    bRef->setPosition(ocsCenter);
    bRef->setRotation(angle);
  }
  else
    return;
  OdDbBlockTableRecordPtr pBR = bRef->blockTableRecord().openObject(OdDb::kForWrite);
  OdDbDatabase *pDb = bRef->database();
  /////////////////////
  // setting "CENTER" parameters to BlockReference
  OdString centerMarklineType = subentLineType(pAction);
  OdDbLinetypeTableRecordPtr centerLTypeTableRec = OdDbImpSmartCenterActionBody::getCenterLType(pDb, centerMarklineType);
  OdDb::LineWeight lineWeight = OdDb::LineWeight(subentLineWeight(pAction));

  bRef->setLinetype(centerLTypeTableRec->objectId());
  bRef->setLinetypeScale(subentLinetypeScale(pAction));
  bRef->setLineWeight(OdDb::LineWeight(lineWeight));

  if (!pBR.isNull())
  {
    //startSettingEntities(pBR);
    OdDbObjectIteratorPtr entityIter = pBR->newIterator();
    int count = 0;
    OdString  hairLengthStr = crossHairLengthExpression(pAction);
    double  hairLength = -1;
    OdChar lastCh = hairLengthStr.getAt(hairLengthStr.getLength() - 1);

    if (lastCh == 'x' || lastCh == 'X')
    {
      hairLengthStr = hairLengthStr.left(hairLengthStr.getLength() - 1);
      hairLength = odStrToD(hairLengthStr) * radius * 2;
    }
    else
      hairLength = odStrToD(hairLengthStr);

    OdString  hairGapStr = crossHairGapExpression(pAction);
    double  hairGap = -1.88;

    lastCh = hairGapStr.getAt(hairGapStr.getLength() - 1);
    if (lastCh == 'x' || lastCh == 'X')
    {
      hairGapStr = hairGapStr.left(hairGapStr.getLength() - 1);

      hairGap = odStrToD(hairGapStr) *radius * 2;
    }
    else
      hairGap = odStrToD(hairLengthStr);

    OdDb::Visibility extLinesVisibility = getExtensionLinesVisibility(pAction);

    OdResult res = eOk;
    pDb->startTransaction();

    for (count = 0; count < 6; count++)
    {
			OdDbEntityPtr pEntity;
      OdDbLinePtr pLine;
			bool newCreated = false;
      
      if (!entityIter->done())
      {
				pEntity = entityIter->entity(OdDb::kForWrite);

        if (!pEntity.isNull())
        {
          pLine = OdDbLine::cast(pEntity->objectId().openObject(OdDb::kForWrite));
        }

				if (pLine.isNull())
        {
          res = eInvalidInput;
          break;
        }
      }
      
			if (pLine.isNull())
      {
        pLine = OdDbLine::createObject();
				newCreated = true;
      }

      OdGePoint3d startPoint = pLine->startPoint();
      OdGePoint3d endPoint = pLine->endPoint();

      if (count > 1)
        pLine->setVisibility(extLinesVisibility);

      switch (count)
      {
      case 0:
        startPoint.x = -hairLength / 2;
        endPoint.x = hairLength / 2;
        break;
      case 1:
        startPoint.y = -hairLength / 2;
        endPoint.y = hairLength / 2;
        break;
      case 2:
      {
          double extension = horizontalStartExtension(pAction);
          double overSh = horizontalStartOvershoot(pAction);

          if (extension == OdDbSmartCenterCommonParameters::MAX_EXTENSIONS())
          {
              startPoint.x = - radius - horizontalStartOvershoot(pAction);
          }
          else
          {
              startPoint.x = - extension - horizontalStartOvershoot(pAction);
          }

          endPoint.x = -hairLength / 2 - hairGap;
      }
        break;
      case 3:
      {
          double extension = horizontalEndExtension(pAction);

          if (extension == OdDbSmartCenterCommonParameters::MAX_EXTENSIONS() )
          {
              endPoint.x = radius + horizontalEndOvershoot(pAction);
          }
          else
          {
              endPoint.x = extension + horizontalEndOvershoot(pAction);
          }

          startPoint.x = hairLength / 2 + hairGap;
      }
        break;
      case 4:
      {
          double extension = verticalStartExtension(pAction);

          if (extension == OdDbSmartCenterCommonParameters::MAX_EXTENSIONS() )
          {
              startPoint.y = -radius - verticalStartOvershoot(pAction);
          }
          else
          {
              startPoint.y = - extension - verticalStartOvershoot(pAction);
          }

          endPoint.y = -hairLength / 2 - hairGap;
      }
        break;
      case 5:
      {
          double extension = verticalEndExtension(pAction);

          if (extension == OdDbSmartCenterCommonParameters::MAX_EXTENSIONS() )
          {
              endPoint.y = radius + verticalEndOvershoot(pAction);
          }
          else
          {
              endPoint.y = extension + verticalEndOvershoot(pAction);
          }

          startPoint.y = hairLength / 2 + hairGap;
      }
        break;
      }

      pLine->setStartPoint(startPoint);
      pLine->setEndPoint(endPoint);

      if (!entityIter->done())
      {
        entityIter->step();
      }
      
			if (newCreated)
      {
				pBR->appendOdDbEntity(pLine);
				pLine->setDatabaseDefaults();
				////From 11941: For all entity properties of lines inside block I suggest to set ByBlock and
				//  layer "0"
				if (count > 1)
				{
  				pLine->setLinetype(pDb->getLinetypeByBlockId());
				}
				else
				{
					pLine->setLinetype(pDb->getLinetypeContinuousId());
				}

				setAddedEntityPropertiesByBlock(pLine, pAction);// 1, OdDb::kLnWtByLayer, OdCmEntityColor::kByBlock);
      }
    }

    if (res != eOk)
    {
      pDb->abortTransaction(); // Roll back changes
      pGeomDep->setStatus(kFailedToEvaluateAssocStatus); //TODO
    }
    else
    {
      pDb->endTransaction();
      pGeomDep->setStatus(kIsUpToDateAssocStatus);
    }
  }
}

// OVERSHOOT REGION
enum OdResult OdDbImpCenterMarkActionBody::setHorizontalEndOvershoot(double newOvershoot, OdDbAssocActionPtr pAction)
{ 
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
  OdString emptyStr("");
  return pAction->setValueParam(OD_T("HorizontalEndOvershoot"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setHorizontalStartOvershoot(double newOvershoot, OdDbAssocActionPtr pAction)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
  OdString emptyStr("");
  return pAction->setValueParam(OD_T("HorizontalStartOvershoot"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setVerticalEndOvershoot(double newOvershoot, OdDbAssocActionPtr pAction)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
  OdString emptyStr("");
  return pAction->setValueParam(OD_T("VerticalEndOvershoot"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setVerticalStartOvershoot(double newOvershoot, OdDbAssocActionPtr pAction)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
  OdString emptyStr("");
  return pAction->setValueParam(OD_T("VerticalStartOvershoot"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

double OdDbImpCenterMarkActionBody::horizontalEndOvershoot(OdDbAssocActionPtr pAction) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");

 if(pAction->getValueParam(OD_T("HorizontalEndOvershoot"), *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;
  
  return value->getDouble();
}

double OdDbImpCenterMarkActionBody::horizontalStartOvershoot(OdDbAssocActionPtr pAction) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("HorizontalStartOvershoot"), *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}

double OdDbImpCenterMarkActionBody::verticalEndOvershoot(OdDbAssocActionPtr pAction) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("VerticalEndOvershoot"), *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}

double OdDbImpCenterMarkActionBody::verticalStartOvershoot(OdDbAssocActionPtr pAction) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("VerticalStartOvershoot"), *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}
//// overshoot region END
// EXTENSIONS REGION
enum OdResult OdDbImpCenterMarkActionBody::setHorizontalEndExtension(double newExtension, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newExtension);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("HorizontalEndExtension"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setHorizontalStartExtension(double newExtension, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newExtension);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("HorizontalStartExtension"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setVerticalEndExtension(double newExtension, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newExtension);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("VerticalEndExtension"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setVerticalStartExtension(double newExtension, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newExtension);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("VerticalStartExtension"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

double OdDbImpCenterMarkActionBody::horizontalEndExtension(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("HorizontalEndExtension"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}

double OdDbImpCenterMarkActionBody::horizontalStartExtension(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("HorizontalStartExtension"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}

double OdDbImpCenterMarkActionBody::verticalEndExtension(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("VerticalEndExtension"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}

double OdDbImpCenterMarkActionBody::verticalStartExtension(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("VerticalStartExtension"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}
//// EXTENSION region END

OdString OdDbImpCenterMarkActionBody::crossHairGapExpression(OdDbAssocActionPtr pAction)const
{
  OdString ret = OdString("-1");
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(ret);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("CrossHairGapExpression"), *value.get(), expression, evaluatorId, 0) != eOk)
    return OdString("-1");

  return value->getString();
}

OdString OdDbImpCenterMarkActionBody::crossHairLengthExpression(OdDbAssocActionPtr pAction)const
{
  OdString ret = OdString("-1");
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(ret);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("CrossHairLengthExpression"), *value.get(), expression, evaluatorId, 0) != eOk)
    return OdString("-1");

  return value->getString();
}

enum OdResult OdDbImpCenterMarkActionBody::setCrossHairGapExpression(OdString const & str, OdDbAssocActionPtr pAction)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(str);
  OdString emptyStr("");
  return pAction->setValueParam(OD_T("CrossHairGapExpression"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}
enum OdResult OdDbImpCenterMarkActionBody::setCrossHairLengthExpression(OdString const & str, OdDbAssocActionPtr pAction)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(str);
  OdString emptyStr("");
  return pAction->setValueParam(OD_T("CrossHairLengthExpression"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbImpCenterMarkActionBody::setExtensionLinesVisibility(OdDb::Visibility visible, OdDbAssocActionPtr pAction)
{
  double dVis = visible == OdDb::kVisible ? 1.0 : 0.0;

  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(visible);
  OdString emptyStr("");
  OdResult ret = pAction->setValueParam(OD_T("eLeftLine_visibility"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
  OdResult ret1 = pAction->setValueParam(OD_T("eRightLine_visibility"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
  OdResult ret2 = pAction->setValueParam(OD_T("eBottomLine_visibility"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
  OdResult ret3 = pAction->setValueParam(OD_T("eTopLine_visibility"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
  ODA_ASSERT(ret == ret1 && ret == ret2 && ret == ret3);
  return ret;
}

//OdDb::Visibility  OdDbImpCenterMarkActionBody::leftLine_visibility(OdDbAssocActionPtr pAction) const
//{
//  OdDb::Visibility bRet = OdDb::kVisible;
//  double retD = -1.0;
//  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
//  OdString expression(""), evaluatorId("");
//
//  if (pAction->getValueParam(OD_T("eLeftLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
//    return -1.0;
//
//  if (value->getDouble() == 0.0)
//    bRet = OdDb::kInvisible;
//
//  return bRet;
//}
//
//OdDb::Visibility  OdDbImpCenterMarkActionBody::rightLine_visibility(OdDbAssocActionPtr pAction) const
//{
//  OdDb::Visibility bRet = OdDb::kVisible;
//  double retD = -1.0;
//  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
//  OdString expression(""), evaluatorId("");
//
//  if (pAction->getValueParam(OD_T("eRightLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
//    return -1.0;
//
//  if (value->getDouble() == 0.0)
//    bRet = OdDb::kInvisible;
//
//  return bRet;
//}
//
//OdDb::Visibility  OdDbImpCenterMarkActionBody::bottomLine_visibility(OdDbAssocActionPtr pAction) const
//{
//  OdDb::Visibility bRet = OdDb::kVisible;
//  double retD = -1.0;
//  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
//  OdString expression(""), evaluatorId("");
//
//  if (pAction->getValueParam(OD_T("eBottomLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
//    return -1.0;
//
//  if (value->getDouble() == 0.0)
//    bRet = OdDb::kInvisible;
//
//  return bRet;
//}
//
//OdDb::Visibility  OdDbImpCenterMarkActionBody::topLine_visibility(OdDbAssocActionPtr pAction) const
//{
//  OdDb::Visibility bRet = OdDb::kVisible;
//  double retD = -1.0;
//  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
//  OdString expression(""), evaluatorId("");
//
//  if (pAction->getValueParam(OD_T("eTopLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
//    return -1.0;
//
//  if (value->getDouble() == 0.0)
//    bRet = OdDb::kInvisible;
//
//  return bRet;
//}
OdDb::Visibility OdDbImpCenterMarkActionBody::getExtensionLinesVisibility(OdDbAssocActionPtr pAction) const// default return value is true
{
  OdDb::Visibility bRet = OdDb::kVisible;
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
  OdString expression(""), evaluatorId("");

  if (pAction->getValueParam(OD_T("eLeftLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return OdDb::kVisible;
  }

  retD = value->getDouble();

  if (pAction->getValueParam(OD_T("eRightLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return OdDb::kVisible;
  }

  double retD1 = value->getDouble();

  if (pAction->getValueParam(OD_T("eBottomLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return OdDb::kVisible;
  }

  double retD2 = value->getDouble();

  if (pAction->getValueParam(OD_T("eTopLine_visibility"), *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return OdDb::kVisible;
  }

  double retD3 = value->getDouble();

  ODA_ASSERT(retD == retD1 && retD == retD2 && retD == retD3);

  if (retD == 0.0)
    bRet = OdDb::kInvisible;

  return bRet;
}

OdResult OdDbImpCenterMarkActionBody::createInstance(
  const OdDbFullSubentPath subentPath,
  OdDbObjectId& centerMarkId,
  OdDbObjectId& actionBodyId)
{
  OdDbSmartCenterMarkParametersPtr pParameters = OdDbSmartCenterMarkParameters::createObject();
  OdResult res = eInvalidInput;
  OdDbObjectIdArray pathArray = subentPath.objectIds();

  if (pathArray.length() < 1)
    return eInvalidInput;

  OdDbObjectId sourceEntity = pathArray[pathArray.length() - 1];

  OdDbDatabase *pDb = pathArray[0].database();
  //OdDbDatabase *pDb = sourceEntity.database();
  OdDbBlockTablePtr pMainBT = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId ownerBTRId = pathArray[0].openObject()->ownerId();
  OdDbBlockTableRecordPtr pOwnerSpace = ownerBTRId.safeOpenObject(OdDb::kForWrite);
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(pOwnerSpace->objectId(), true);

  //create destination BTR
  OdDbBlockTableRecordPtr pDestinationBTR = OdDbBlockTableRecord::createObject();
  OdString destinationBTRname = OD_T("*U");
  
  pDestinationBTR->setName(destinationBTRname);
  pDb->addOdDbObject(pDestinationBTR, pMainBT->objectId());

  //create center mark block reference
  OdDbBlockReferencePtr pCenterMarkBlockRef = OdDbBlockReference::createObject();
  //From 11941: For properties of Block Reference :
  //While creating new : setDatabaseDefaults() plus
  //  CENTERLAYER, CENTERLTSCALE, CENTERLTYPE
  pCenterMarkBlockRef->setDatabaseDefaults(pDb);
  
	OdDbObjectId layer = getCenterLayerId(pDb);
  double scale = pDb->getCENTERLTSCALE();
  OdDbLinetypeTableRecordPtr centerLTypeTableRec = OdDbImpSmartCenterActionBody::getCenterLType(pDb);

  pCenterMarkBlockRef->setLinetype( centerLTypeTableRec->objectId() );
	pCenterMarkBlockRef->setLayer(layer);
  pCenterMarkBlockRef->setLinetypeScale(scale);
  //End first part of "From 11941" //////////////////////////////////////////////////////////////////////
  pCenterMarkBlockRef->setBlockTableRecord(pDestinationBTR->objectId());
  pDb->addOdDbObject(pCenterMarkBlockRef, ownerBTRId);
  //create action and action body
  //BEGIN
  OdRxClass* pClass;
  pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbCenterMarkActionBody")).get();

  OdDbObjectId actionId;

  if (eOk != createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId))
  {
    return eInvalidInput;
  }
  //END
  //append BTR and block ref
  pMainBT->add(pDestinationBTR);
  pOwnerSpace->appendOdDbEntity(pCenterMarkBlockRef);
  centerMarkId = pCenterMarkBlockRef->objectId();

  OdDbCenterMarkActionBodyPtr pActionBody =
    OdDbAssocActionBody::cast(actionBodyId.openObject(OdDb::kForWrite));
  
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  pAction->setActionBody(actionBodyId);

  OdString expression, evaluatorID, errorMSG;
  OdDbImpSmartCenterMarkParameters*  pImpParameters = (OdDbImpSmartCenterMarkParameters*)(pParameters->impSmartCenterParameters());

  ////From 11941: While recalculating values from Parameters should be used.
  setSubentLineWeight(pCenterMarkBlockRef->lineWeight(), pAction);
	OdString centerMarklineType = centerLTypeTableRec->getName();
  pImpParameters->setSubentLineType(centerMarklineType, expression, evaluatorID, errorMSG);
  pImpParameters->setSubentLinetypeScale(scale, expression, evaluatorID, errorMSG);
  pImpParameters->setCrossHairGapExpression( pDb->getCENTERCROSSGAP(), expression, evaluatorID, errorMSG);
  pImpParameters->setCrossHairLengthExpression( pDb->getCENTERCROSSSIZE(), expression, evaluatorID, errorMSG);

  double exe = pDb->getCENTEREXE();
  bool isVisible = pDb->getCENTERMARKEXE();
  OdDb::Visibility newValVis = isVisible ? OdDb::kVisible : OdDb::kInvisible;

  pImpParameters->setHorizontalEndOvershoot(exe, expression, evaluatorID, errorMSG);
  pImpParameters->setVerticalEndOvershoot(exe, expression, evaluatorID, errorMSG);
  pImpParameters->setHorizontalStartOvershoot(exe, expression, evaluatorID, errorMSG);
  pImpParameters->setVerticalStartOvershoot(exe, expression, evaluatorID, errorMSG);
  pImpParameters->setextensionLinesVisibility( newValVis, expression, evaluatorID, errorMSG);
  // End the part of 11941
  //create dependency
  //BEGIN
  //geometry reference
  OdDbAssocGeomDependencyPtr pGeomDependency = OdDbAssocGeomDependency::createObject();
  pDb->addOdDbObject(pGeomDependency);
  pGeomDependency->setIsReadDependency(true);
  pGeomDependency->setIsWriteDependency(false);

  OdDbObjectPtr assocOb = sourceEntity.openObject(OdDb::kForWrite);
  OdGeCircArc3d arc;
  OdUInt32 subentIndex = subentPath.subentId().index();

  if (subentPath.subentId().type() != OdDb::kNullSubentType)
  {
    try
    {
      res = pGeomDependency->createNewEdgePersSubent(assocOb, pAction, subentIndex);/// , false);
    }
    catch (...)
    {
      ODA_ASSERT(!L"SubEntityPath is wrong");
    }
  }
  else
  {
    res = pGeomDependency->createNewSingleEdgePersSubent(assocOb->objectId());
  }

  if (res != eOk)
    return res;

  pActionBody->addSubentitySpecialSettings(assocOb, subentPath.subentId(), subentIndex);

  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();

  pCompoundObjectId->setFullPath(pathArray);//, OdDbDatabase* pHostDatabase = NULL)
  pGeomDependency->attachToObject(pCompoundObjectId);
  pAction->addDependency(pGeomDependency->objectId(), true);

  //center mark block reference
  OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject(pDependency);
  pDependency->setIsReadDependency(true);
  pDependency->setIsWriteDependency(true);

  pCompoundObjectId = OdDbCompoundObjectId::createObject();

  pCompoundObjectId->set(pCenterMarkBlockRef->objectId());
  pDependency->attachToObject(pCompoundObjectId);
  pAction->addDependency(pDependency->objectId(), true);

  if (pActionBody->getRefCircle(arc) != eOk)
  {
    return eInvalidInput;
  }
  
  double radius = arc.radius();
  pParameters->setCachedCircleRadius(radius, expression, evaluatorID, errorMSG);
  pParameters->setCenterDepId(pDependency->objectId(), expression, evaluatorID, errorMSG);
  pActionBody->parameters() = pParameters;
  const_cast<OdDbSmartCenterMarkParametersPtr &>(pParameters)->setOwner(pActionBody);

  //END
  int paramIndex = 0;
  OdDbObjectId paramId;

  OdString paramName = OD_T("InputEdge");
  pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbAssocEdgeActionParam")).get();
  OdDbAssocEdgeActionParamPtr pEdgeActionParam;

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
    pCompoundObjectId->set(pGeomDependency->objectId());

    OdResult res = pEdgeActionParam->setObject(//actionId,
      pCompoundObjectId,
      true,
      true);

    pEdgeActionParam->setEdgeSubentityGeometry(&arc);
  }
  
  OdDbObjectId centerLTypetableRecId = pDb->getLinetypeContinuousId();

  for (int i = 0; i < 6; i++)
  {
    OdDbLinePtr pLine = OdDbLine::createObject();

    pDestinationBTR->appendOdDbEntity(pLine);
    pLine->setDatabaseDefaults(pDb);
    ////From 11941: For all entity properties of lines inside block I suggest to set ByBlock and
    //  layer "0"
    if (i > 1)
    {
  		pLine->setLinetype(pDb->getLinetypeByBlockId());

      pLine->setVisibility(newValVis);// TODO: byBlock? It doesn,t make sense
    }
		else
		{
  		pLine->setLinetype(centerLTypetableRecId);
		}

		setAddedEntityPropertiesByBlock(pLine, pAction);
		//End the part of "From 11941" //////////////////////////////////////////////////////////////////////
  }
  return res;
}
