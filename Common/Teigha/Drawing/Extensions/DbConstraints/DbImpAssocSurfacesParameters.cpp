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
#include "DbAssocActionParam.h"
#include "DbAudit.h"
#include "DbHostAppServices.h"
#include "DbImpSmartCenterObjectsParameters.h"
#include "DbImpAssocSurfacesParameters.h"
#include "DbImpAssocExtrudedSurfaceActionBody.h"
#include "DbImpAssocActionBody.h"
#include "DbAssocExtrudedSurfaceActionBody.h"
#include "DbAssocMLeaderActionBody.h"

#define NEXT_CODE(code) \
  if (pFiler->nextItem() != code) \
  { \
    ODA_FAIL(); \
    return eMakeMeProxy; \
  }

OdDbImpAssocLoftParameters::OdDbImpAssocLoftParameters(
  short continuity,
  double bulge
  )
{
  m_pOwner = NULL;
  OdString expression, evaluatorID, errorMSG;
  setContinuity(kStartCrossSection, continuity, expression, evaluatorID, errorMSG);
  setBulge(kStartCrossSection, bulge, expression, evaluatorID, errorMSG);
}
OdResult OdDbImpAssocLoftParameters::setContinuity(ProfileType type, short cont,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(cont);
  return setValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocLoftParameters::setBulge(ProfileType type, double distance,
  const OdString& expression,
  const OdString& evaluatorId,
  OdString& pErrorMessage)
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(distance);
  return setValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

void OdDbImpAssocLoftParameters::setInputVariables(const OdDbObjectIdArray& sourceEntites, short continuity, double bulge)
{
  OdArray<OdDbImpValueParamPtr>::iterator it_Value;

  for (it_Value = m_arrValueParams.begin(); it_Value != m_arrValueParams.end(); it_Value++)
  {
    OdDbEvalVariantPtr m_value;

    if (kBulgeParamName == (*it_Value)->name())
      m_value = OdDbEvalVariant::init(bulge);

    if (kContinuityParamName == (*it_Value)->name())
      m_value = OdDbEvalVariant::init( continuity );

    m_value->setNext(NULL);

    OdDbEvalVariant* pLast = m_value;
    for (unsigned int i = 1; i < sourceEntites.length()  ; ++i)
    {
      OdDbEvalVariantPtr pNewEvVar;
      if (kBulgeParamName == (*it_Value)->name())
        pNewEvVar = OdDbEvalVariant::init(bulge);

      if (kContinuityParamName == (*it_Value)->name())
        pNewEvVar = OdDbEvalVariant::init( continuity );
      pLast->setNext(pNewEvVar);
      pLast = pNewEvVar;
    }

    (*it_Value)->setValue(*m_value);
    //(*it_Value)->setInputVariablesIds(sourceEntites);
  }
}

OdResult OdDbImpAssocLoftParameters::GetItemByLocator(const OdDbItemLocator &locator, OdDbAssocArrayItemPtr &pItem) const
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

OdResult OdDbImpAssocLoftParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items)
{
  return eOk;
}

OdGeMatrix3d OdDbImpAssocLoftParameters::CalculateDefaultMatrix(const OdDbItemLocator &locator)
{
  OdGeVector3d vPos;// = x*vXaxis() + y*vY + z*vZaxis();
  OdGeVector3d vRot;

  double angleXaxisRowProfile = 0;// vXaxis().angleTo(m_vRowProfile, OdGeVector3d(0, 0, 1));

                                  //vPos.rotateBy(angleXaxisRowProfile, vZaxis());
  vPos.rotateBy(angleXaxisRowProfile, vRot);
  return OdGeMatrix3d().translation(vPos);
}

OdDbItemLocator OdDbImpAssocLoftParameters::getCanonicalForm(const OdDbItemLocator& locator) const
{
  int itemIndex = locator[OdDbItemLocator::kItemIndex],
    rowIndex = locator[OdDbItemLocator::kRowIndex],
    levelIndex = locator[OdDbItemLocator::kLevelIndex];

  return OdDbItemLocator(itemIndex, rowIndex, levelIndex);
}

OdResult OdDbImpAssocLoftParameters::setOwner(class OdDbAssocActionBody* pOwner)
{
  //OdDbEvalVariantPtr m_value = OdDbEvalVariant::init(int());
  //OdDbObjectIdArray m_InputVariablesIds;
  //m_InputVariablesIds.push_back(OdDbObjectId::kNull);

  //m_InputVariablesIds.resize(nEvalVars);

  //m_value->setNext(NULL);
  //OdResult res = m_value->dwgInFields(pFiler);
  //if (res != eOk)
  //  return eMakeMeProxy;

  //m_InputVariablesIds[0] = pFiler->rdSoftPointerId();
  //ODA_ASSERT_ONCE(m_InputVariablesIds[0].isNull()); // TODO

  //OdDbEvalVariant* pLast = m_value;
  //for (int i = 1; i < nEvalVars; ++i)
  //{
  //  OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
  //  if (pNewEvVar->dwgInFields(pFiler) != eOk)
  //    return eMakeMeProxy;
  //  pLast->setNext(pNewEvVar);
  //  pLast = pNewEvVar;
  //  m_InputVariablesIds[i] = pFiler->rdSoftPointerId();
  //  ODA_ASSERT_ONCE(m_InputVariablesIds[i].isNull()); // TODO
  //}

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
  //OdDbEvalVariantPtr pEV = OdDbEvalVariant::createObject();
  OdDbEvalVariantPtr pEV = OdDbEvalVariant::init(int());
  //pEV->setNext(NULL);
  //OdResult res = m_value->dwgInFields(pFiler);
  //if (res != eOk)
  //  return eMakeMeProxy;

  //m_InputVariablesIds[0] = pFiler->rdSoftPointerId();
  //ODA_ASSERT_ONCE(m_InputVariablesIds[0].isNull()); // TODO

  //OdDbEvalVariant* pLast = pEV;
  //for (int i = 1; i < 3; ++i)
  //{
  //   OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
  //   pLast->setNext(pNewEvVar);
  //   pLast = pNewEvVar;
  ////  m_InputVariablesIds[i] = pFiler->rdSoftPointerId();
  ////  ODA_ASSERT_ONCE(m_InputVariablesIds[i].isNull()); // TODO
  //}


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

    //(*it_Value)->setValue(*pEV);

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

    res = pAction->setValueParamUnitType((*it_Value)->name(), (*it_Value)->unitType());

    if (eOk != res)
    {
      m_pOwner = NULL;
      return res;
    }

  }

  m_arrValueParams.clear();

  return eOk;
}


