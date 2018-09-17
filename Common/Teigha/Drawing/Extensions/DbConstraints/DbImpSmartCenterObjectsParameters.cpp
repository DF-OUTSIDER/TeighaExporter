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
#include "DbImpAssocArrayParameters.h"
#include "DbBlockReference.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocActionParam.h"
#include "DbAssocDependency.h"
#include "DbAudit.h"
#include "DbHostAppServices.h"
#include "DbImpSmartCenterObjectsParameters.h"

#define NEXT_CODE(code) \
  if (pFiler->nextItem() != code) \
  { \
    ODA_FAIL(); \
    return eMakeMeProxy; \
  }

//////////////////////////////////////////////////////////////////////////
OdDbImpSmartCenterCommonParameters::OdDbImpSmartCenterCommonParameters()
{
  m_pOwner = NULL;
}

OdDbImpSmartCenterCommonParameters::~OdDbImpSmartCenterCommonParameters()
{
}

OdResult OdDbImpSmartCenterCommonParameters::setOwner(class OdDbAssocActionBody* pOwner)
{
  if (m_pOwner)
  {
    return eAlreadyActive;
  }

  m_pOwner = pOwner;

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(m_pOwner->parentAction().openObject(OdDb::kForWrite));

  OdString expression;
  OdString evaluatorId;

  OdArray<OdDbImpValueParamPtr>::iterator it_Value;

  OdString errorMSG;
  OdResult res;
  OdDbEvalVariantPtr pEV = OdDbEvalVariant::createObject();

  // Synchronize values
  for (it_Value = m_arrValueParams.begin(); it_Value != m_arrValueParams.end(); it_Value++)
  {
    if (eOk == pAction->getValueParam((*it_Value)->name(),
      *pEV,
      expression,
      evaluatorId))
    {
      (*it_Value)->setValue(*pEV);
      continue;
    }

    res = pAction->setValueParam((*it_Value)->name(),
      *(*it_Value)->value(),
      expression,
      evaluatorId,
      errorMSG,
      true);
    if (eOk != res)
    {
      m_pOwner = NULL;
      return res;
    }

    res = pAction->setValueParamUnitType((*it_Value)->name(),
      (*it_Value)->unitType());

    if (eOk != res)
    {
      m_pOwner = NULL;
      return res;
    }
  }

  m_arrValueParams.clear();

  return eOk;
}

OdDbItemLocator OdDbImpSmartCenterCommonParameters::getCanonicalForm(const OdDbItemLocator& locator) const
{
  int itemIndex = locator[OdDbItemLocator::kItemIndex],
    rowIndex = locator[OdDbItemLocator::kRowIndex],
    levelIndex = locator[OdDbItemLocator::kLevelIndex];

  return OdDbItemLocator(itemIndex, rowIndex, levelIndex);
}

OdResult OdDbImpSmartCenterCommonParameters::GetItemByLocator(const OdDbItemLocator &locator, OdDbAssocArrayItemPtr &pItem) const
{

  //if (locator[OdDbItemLocator::kLevelIndex] > m_levelCount
  //  || locator[OdDbItemLocator::kRowIndex] > m_rowCount
  //  || locator[OdDbItemLocator::kItemIndex] > m_itemCount
  //  || locator[OdDbItemLocator::kLevelIndex] < 0
  //  || locator[OdDbItemLocator::kRowIndex] < 0
  //  || locator[OdDbItemLocator::kItemIndex] < 0)
  //  return eOutOfRange;

  //pItem = m_items[locator[OdDbItemLocator::kLevelIndex]
  //  + locator[OdDbItemLocator::kRowIndex] * m_levelCount
  //  + locator[OdDbItemLocator::kItemIndex] * m_levelCount * m_rowCount];
  return eOk;
}

OdResult OdDbImpSmartCenterCommonParameters::setCenterDepId(const OdDbObjectId input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CenterDepId", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterCommonParameters::setSubentLineWeight(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"SubentLineWeight", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterCommonParameters::setSubentLinetypeScale(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"SubentLinetypeScale", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterCommonParameters::setCachedAssociateStatus(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CachedAssociateStatus", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterCommonParameters::setAssociationToBeBroken(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"AssociationToBeBroken", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterCommonParameters::setSubentLineType(const OdString input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"SubentLineType", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

double OdDbImpSmartCenterCommonParameters::subentLineWeight(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"SubentLineWeight", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterCommonParameters::subentLinetypeScale(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"SubentLinetypeScale", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterCommonParameters::cachedAssociateStatus(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CachedAssociateStatus", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterCommonParameters::associationToBeBroken(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"AssociationToBeBroken", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

OdDbObjectId OdDbImpSmartCenterCommonParameters::centerDepId(
  const OdDbDatabase* pDb,
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(OdDbObjectId());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CenterDepId", *value.get(), expression, evaluatorId, unitType);
  return value->getObjectId(pDb);
}

OdString OdDbImpSmartCenterCommonParameters::subentLineType(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(OdString());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"SubentLineType", *value.get(), expression, evaluatorId, unitType);
  return value->getString();
}

//////////////////////////////////////////////////////////////////////////


OdDbImpSmartCenterMarkParameters::OdDbImpSmartCenterMarkParameters(
    double HorizontalStartExtension,
    double HorizontalEndExtension,
    double VerticalStartExtension,
    double VerticalEndExtension,
    double HorizontalStartOvershoot,
    double HorizontalEndOvershoot,
    double VerticalStartOvershoot,
    double VerticalEndOvershoot,
    OdString CrossHairLengthExpression,
    OdString CrossHairGapExpression,
    OdDbObjectId CenterDepId,
    OdDb::Visibility extensionLinesVisibility,
    double CachedCircleRadius,
    OdString SubentLineType,
    double SubentLineWeight,
    double SubentLinetypeScale,
    double CachedAssociateStatus
  )
{
  OdString expression, evaluatorID, errorMSG;
  setHorizontalStartExtension(HorizontalStartExtension, expression, evaluatorID, errorMSG);
  setHorizontalEndExtension(HorizontalStartExtension, expression, evaluatorID, errorMSG);
  setVerticalStartExtension(VerticalStartExtension, expression, evaluatorID, errorMSG);
  setVerticalEndExtension(VerticalEndExtension, expression, evaluatorID, errorMSG);
  setHorizontalStartOvershoot(HorizontalStartOvershoot, expression, evaluatorID, errorMSG);
  setHorizontalEndOvershoot(HorizontalEndOvershoot, expression, evaluatorID, errorMSG);
  setVerticalStartOvershoot(VerticalStartOvershoot, expression, evaluatorID, errorMSG);
  setVerticalEndOvershoot(VerticalEndOvershoot, expression, evaluatorID, errorMSG);
  setCrossHairLengthExpression(CrossHairLengthExpression, expression, evaluatorID, errorMSG);
  setCrossHairGapExpression(CrossHairGapExpression, expression, evaluatorID, errorMSG);
  setCenterDepId(CenterDepId, expression, evaluatorID, errorMSG);
  setextensionLinesVisibility(extensionLinesVisibility, expression, evaluatorID, errorMSG);
  setCachedCircleRadius(CachedCircleRadius, expression, evaluatorID, errorMSG);
  setSubentLineType(SubentLineType, expression, evaluatorID, errorMSG);
  setSubentLineWeight(SubentLineWeight, expression, evaluatorID, errorMSG);
  setSubentLinetypeScale(SubentLinetypeScale, expression, evaluatorID, errorMSG);
  setCachedAssociateStatus(CachedAssociateStatus, expression, evaluatorID, errorMSG);
}

OdDbImpSmartCenterMarkParameters::~OdDbImpSmartCenterMarkParameters()
{
}

////////////////////////////////////////////////////////
double OdDbImpSmartCenterMarkParameters::horizontalStartExtension(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"HorizontalStartExtension", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::horizontalEndExtension(
   OdString& expression,
   OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"HorizontalEndExtension", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::verticalStartExtension(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"VerticalStartExtension", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::verticalEndExtension(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"VerticalEndExtension", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::horizontalStartOvershoot(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"HorizontalStartOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::horizontalEndOvershoot(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"HorizontalEndOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::verticalStartOvershoot(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"VerticalStartOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::verticalEndOvershoot(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"VerticalEndOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::cachedCircleRadius(
  OdString& expression,
  OdString& evaluatorId
) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CachedCircleRadius", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterMarkParameters::cachedRotation(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CachedRotation", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

OdString OdDbImpSmartCenterMarkParameters::crossHairLengthExpression(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(OdString());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CrossHairLengthExpression", *value.get(), expression, evaluatorId, unitType);
  return value->getString();
}

OdString OdDbImpSmartCenterMarkParameters::crossHairGapExpression(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(OdString());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CrossHairGapExpression", *value.get(), expression, evaluatorId, unitType);
  return value->getString();
}

OdDb::Visibility OdDbImpSmartCenterMarkParameters::extensionLinesVisibility(
  OdString& expression,
  OdString& evaluatorId
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"eLeftLine_visibility", *value.get(), expression, evaluatorId, unitType);
  double vis1 = value->getDouble();
  getValueParam(L"eRightLine_visibility", *value.get(), expression, evaluatorId, unitType);
  double vis2 = value->getDouble();
  getValueParam(L"eBottomLine_visibility", *value.get(), expression, evaluatorId, unitType);
  double vis3 = value->getDouble();
  getValueParam(L"eTopLine_visibility", *value.get(), expression, evaluatorId, unitType);
  double vis4 = value->getDouble();

  if (vis1 == 0.0 && vis2 == 0.0 && vis3 == 0.0 && vis4 == 0.0)
    return OdDb::kInvisible;

  return OdDb::kVisible;
}
/////////////////////////////////////////////////////////////////////
OdResult OdDbImpSmartCenterMarkParameters::setHorizontalStartExtension(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"HorizontalStartExtension", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setHorizontalEndExtension(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"HorizontalEndExtension", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setVerticalStartExtension(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"VerticalStartExtension", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setVerticalEndExtension(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"VerticalEndExtension", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setHorizontalStartOvershoot(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"HorizontalStartOvershoot", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setHorizontalEndOvershoot(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"HorizontalEndOvershoot", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setVerticalStartOvershoot(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"VerticalStartOvershoot", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setVerticalEndOvershoot(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"VerticalEndOvershoot", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setCachedCircleRadius(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CachedCircleRadius", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setCachedRotation(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CachedRotation", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setCrossHairLengthExpression(const OdString input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CrossHairLengthExpression", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setCrossHairGapExpression(const OdString input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CrossHairGapExpression", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::setextensionLinesVisibility(const OdDb::Visibility input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  double doubleInput = input == OdDb::kVisible ? 1.0 : 0.0;
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(doubleInput);
  setValueParam(L"eLeftLine_visibility", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
  setValueParam(L"eRightLine_visibility", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
  setValueParam(L"eBottomLine_visibility", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
  return setValueParam(L"eTopLine_visibility", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterMarkParameters::dwgInFields(OdDbDwgFiler* pFiler)
{
  //OdResult res = OdDbImpAssocArrayCommonParameters::dwgInFields(pFiler);
  //if (res != eOk)
  //  return res;

  //ODA_FAIL_ONCE(); // TODO
  return eMakeMeProxy;
}

void OdDbImpSmartCenterMarkParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  //OdDbImpAssocArrayCommonParameters::dwgOutFields(pFiler);
  //ODA_FAIL_ONCE(); // TODO
  //throw OdError(eNotImplementedYet);
}

OdResult OdDbImpSmartCenterMarkParameters::dxfInFields(OdDbDxfFiler* pFiler)
{
  //OdResult res = OdDbImpAssocArrayCommonParameters::dxfInFields(pFiler);
  //if (res != eOk)
  //  return res;
  //ODA_FAIL_ONCE(); // TODO
  return eMakeMeProxy;
}

void OdDbImpSmartCenterMarkParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  //OdDbImpAssocArrayCommonParameters::dxfOutFields(pFiler);
  //ODA_FAIL_ONCE(); // TODO
  //throw OdError(eNotImplementedYet);
}

OdResult OdDbImpSmartCenterMarkParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items)
{
  return eOk;
}

OdGeMatrix3d OdDbImpSmartCenterMarkParameters::CalculateDefaultMatrix(const OdDbItemLocator &locator)
{
  OdGeVector3d vPos;// = x*vXaxis() + y*vY + z*vZaxis();
  OdGeVector3d vRot;

  double angleXaxisRowProfile = 0;// vXaxis().angleTo(m_vRowProfile, OdGeVector3d(0, 0, 1));

  //vPos.rotateBy(angleXaxisRowProfile, vZaxis());
  vPos.rotateBy(angleXaxisRowProfile, vRot);
  return OdGeMatrix3d().translation(vPos);
}

//////////////////////////////////////////////////////////////////////////
OdDbImpSmartCenterLineParameters::OdDbImpSmartCenterLineParameters(
  double StartExtLength,
  double EndExtLength,
  double StartOvershoot,
  double EndOvershoot,
  int EdgeRefDirectionEnding,
  int EdgeRefDirectionEnding2,
  OdDbObjectId CenterDepId,
  double CachedCenterLineLength,
  double CachedCenterLineDirectionX,
  double CachedCenterLineDirectionY,
  double CachedCenterLineDirectionZ,
  OdString SubentLineType,
  double SubentLineWeight,
  double SubentLinetypeScale,
  double CachedAssociateStatus
  )
{
  OdString expression, evaluatorID, errorMSG;
  setStartExtension(StartExtLength, expression, evaluatorID, errorMSG);
  setEndExtension(EndExtLength, expression, evaluatorID, errorMSG);
  setStartOvershoot(StartOvershoot, expression, evaluatorID, errorMSG);
  setEndOvershoot(EndOvershoot, expression, evaluatorID, errorMSG);
  //setEdgeRefDirEnding(EdgeRefDirectionEnding, EdgeRefDirectionEnding2, expression, evaluatorID, errorMSG);
  //setEdgeRefDirEndingArray(OdArray<int, OdArrayMemCopyReallocator<int> > &, expression, evaluatorID, errorMSG);
  OdArray<int> refDirarray;
  refDirarray.append(EdgeRefDirectionEnding);
  refDirarray.append(EdgeRefDirectionEnding2);
  setEdgeRefDirEndingArray(refDirarray, expression, evaluatorID, errorMSG);

  setCenterDepId(CenterDepId, expression, evaluatorID, errorMSG);
  setCachedCenterLineLength(CachedCenterLineLength, expression, evaluatorID, errorMSG);

  OdGeVector3d CachedVector(CachedCenterLineDirectionX, CachedCenterLineDirectionY, CachedCenterLineDirectionZ);
  setCachedCenterLineDirection(CachedVector, expression, evaluatorID, errorMSG);

  setSubentLineType(SubentLineType, expression, evaluatorID, errorMSG);
  setSubentLineWeight(SubentLineWeight, expression, evaluatorID, errorMSG);
  setSubentLinetypeScale(SubentLinetypeScale, expression, evaluatorID, errorMSG);
  setCachedAssociateStatus(CachedAssociateStatus, expression, evaluatorID, errorMSG);
}

OdDbImpSmartCenterLineParameters::~OdDbImpSmartCenterLineParameters()
{
}

OdResult OdDbImpSmartCenterLineParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items)
{
  return eOk;
}

OdGeMatrix3d OdDbImpSmartCenterLineParameters::CalculateDefaultMatrix(const OdDbItemLocator &locator)
{
  OdGeVector3d vPos;// = x*vXaxis() + y*vY + z*vZaxis();
  OdGeVector3d vRot;

  double angleXaxisRowProfile = 0;// vXaxis().angleTo(m_vRowProfile, OdGeVector3d(0, 0, 1));

                                  //vPos.rotateBy(angleXaxisRowProfile, vZaxis());
  vPos.rotateBy(angleXaxisRowProfile, vRot);
  return OdGeMatrix3d().translation(vPos);
}
/////////////////////////////////////////////////////////////////////
OdResult OdDbImpSmartCenterLineParameters::setStartExtension(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"StartExtLength", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterLineParameters::setEndExtension(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"EndExtLength", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterLineParameters::setStartOvershoot(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"StartOvershoot", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterLineParameters::setEndOvershoot(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"EndOvershoot", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterLineParameters::setCachedCenterLineLength(const double input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  return setValueParam(L"CachedCenterLineLength", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterLineParameters::setCachedCenterLineDirection(OdGeVector3d const& input,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input.x);
  if (setValueParam(L"CachedCenterLineDirectionX", *value.get(), expression, evaluatorId, pErrorMessage, &unitType) == eOk)
  {
    value = OdDbEvalVariant::init(input.y);
    
    if (setValueParam(L"CachedCenterLineDirectionY", *value.get(), expression, evaluatorId, pErrorMessage, &unitType) == eOk)
    {
      value = OdDbEvalVariant::init(input.z);

      return setValueParam(L"CachedCenterLineDirectionZ", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
    }
  }

  return eMakeMeProxy;
}

OdResult OdDbImpSmartCenterLineParameters::edgeRefDirEndingArray(OdArray<int> & outputArr)
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(-1);
  OdString expression(""), evaluatorId("");
  OdValue::UnitType unitType = OdValue::kUnitless;

  getValueParam(L"EdgeRefDirectionEnding", *value.get(), expression, evaluatorId, unitType);

  while (!value.isNull())
  {
    int out = value->getInt32();
    outputArr.append(out);

    value = value->next();
  }

  return eOk;
}

OdResult OdDbImpSmartCenterLineParameters::setEdgeRefDirEndingArray(const OdArray<int> & inputArr,
  const OdString& expression, // = OdString::kEmpty
  const OdString& evaluatorId, // = OdString::kEmpty
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  const int input = ((int)inputArr[0]);
  const int input2 = ((int)inputArr[1]);
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  OdDbEvalVariantPtr value2 = OdDbEvalVariant::init(input2);
  value->setNext(value2);

  return  setValueParam(L"EdgeRefDirectionEnding", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpSmartCenterLineParameters::setEdgeRefDirEnding(const int input, const int input2,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(input);
  OdDbEvalVariantPtr value2 = OdDbEvalVariant::init(input2);
  value->setNext(value2);
  
  return setValueParam(L"EdgeRefDirectionEnding", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

double OdDbImpSmartCenterLineParameters::startExtension(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"StartExtLength", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterLineParameters::endExtension(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"EndExtLength", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterLineParameters::startOvershoot(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"StartOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpSmartCenterLineParameters::endOvershoot(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"EndOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

int OdDbImpSmartCenterLineParameters::edgeRefDirEnding(
  int,
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(int());
  OdValue::UnitType unitType = OdValue::kUnitless;
  //getValueParam(L"HorizontalEndOvershoot", *value.get(), expression, evaluatorId, unitType);
  return value->getInt32();
}

double OdDbImpSmartCenterLineParameters::cachedCenterLineLength(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CachedCenterLineLength", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

OdGeVector3d OdDbImpSmartCenterLineParameters::cachedCenterLineDirection(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"CachedCenterLineDirectionX", *value.get(), expression, evaluatorId, unitType);
  double xx = value->getDouble();

  getValueParam(L"CachedCenterLineDirectionY", *value.get(), expression, evaluatorId, unitType);
  double yy = value->getDouble();

  getValueParam(L"CachedCenterLineDirectionZ", *value.get(), expression, evaluatorId, unitType);
  double zz = value->getDouble();
  
  return OdGeVector3d(xx, yy, zz);
}
