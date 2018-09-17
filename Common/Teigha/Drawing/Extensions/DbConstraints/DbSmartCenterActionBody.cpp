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
#include "DbImpSmartCenterActionBody.h"
#include "DbBlockReference.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbSmartCenterActionBody, OdDbSmartCenterActionBodyBase, L"AcDbSmartCenterActionBody")

OdDbSmartCenterActionBody::OdDbSmartCenterActionBody()
{
}

void OdDbSmartCenterActionBody::evaluateOverride()
{
	static_cast<OdDbImpSmartCenterActionBody *>(m_pImpObj)->evaluateOverride(parentAction());
}

enum OdResult OdDbSmartCenterActionBody::setStringGeometryParam(const OdString newValue, const OdString& paramName)
{
  ODA_ASSERT(paramName == OD_T("CrossHairGapExpression") || paramName == OD_T("CrossHairLengthExpression"));
  assertWriteEnabled();
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newValue);
  OdString emptyStr("");
  return setValueParam(paramName, *value.get(),  OD_T(""), OD_T(""), emptyStr, false, 0);
}

enum OdResult OdDbSmartCenterActionBody::getStringGeometryParam(OdString& newString, const OdString& paramName) const
{
  ODA_ASSERT(paramName == OD_T("CrossHairGapExpression") || paramName == OD_T("CrossHairLengthExpression"));
	OdDbEvalVariantPtr value = OdDbEvalVariant::init(OdString(""));
        OdString emptyStr("");
	OdResult ret = getValueParam(paramName, *value.get(), emptyStr, emptyStr, 0);

	if (ret == eOk)
		newString = value->getString();

	return ret;
}

enum OdResult OdDbSmartCenterActionBody::setGeometryParam(double newValue, const OdString& paramName, bool assertEnable)
{
  ODA_ASSERT(paramName != OD_T("CrossHairGapExpression") && paramName != OD_T("CrossHairLengthExpression") && paramName != OD_T("EdgeRefDirectionEnding") && paramName != OD_T("CenterDepId"));
  if(assertEnable)
    assertWriteEnabled();

  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newValue);
  OdString emptyStr("");
  return setValueParam(paramName, *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
  //return static_cast<OdDbImpCenterMarkActionBody *>
  //  (m_pImpObj)->setHorizontalStartOvershoot(newovershoot, parentAction());
}

enum OdResult OdDbSmartCenterActionBody::getGeometryParam(double& newValue, const OdString& paramName) const
{
  ODA_ASSERT(paramName != OD_T("CrossHairGapExpression") && paramName != OD_T("CrossHairLengthExpression") && paramName != OD_T("EdgeRefDirectionEnding") && paramName != OD_T("CenterDepId"));
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(double(1));
  OdString expression(""), evaluatorId(""); 
  OdResult ret = getValueParam(paramName, *value.get(), expression, evaluatorId, 0);

  if (ret == eOk)
    newValue = value->getDouble();

  return ret;
}

OdDbObjectId OdDbSmartCenterActionBody::getSmartCenterActionBody(const OdDbBlockReference* pBlkRef)
{
  OdDbObjectId idRet;
  OdDbObjectIdArray arrReactors = pBlkRef->getPersistentReactors();
  OdDbObjectIdArray::const_iterator iter = arrReactors.begin();
  OdDbObjectIdArray::const_iterator end = arrReactors.end();
  while (iter  != end)
  {
    OdDbObjectPtr pObj = (*iter).openObject();
    if (pObj.isNull())
      continue;
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast(pObj->ownerId().openObject());
    if (pAction.isNull())
      continue;
    idRet = pAction->actionBody();
    break;
  }
  return idRet;
}

OdResult OdDbSmartCenterActionBody::addMoreObjectsToDeepCloneOverride(OdDbIdMapping& idMap,
  OdDbObjectIdArray& additionalObjectsToClone) const
{
  assertReadEnabled();
  return static_cast<OdDbImpSmartCenterActionBody *>
    (m_pImpObj)->addMoreObjectsToDeepCloneOverride(this, idMap, additionalObjectsToClone);
}

OdResult OdDbSmartCenterActionBody::postProcessAfterDeepCloneOverride(OdDbIdMapping& idMap)
{
  return static_cast<OdDbImpSmartCenterActionBody *>
    (m_pImpObj)->postProcessAfterDeepCloneOverride(this, idMap);
}

const OdDbSmartCenterCommonParametersPtr OdDbSmartCenterActionBody::parameters() const
{
  return static_cast<OdDbImpSmartCenterActionBody *>(m_pImpObj)->parameters();
}

OdDbSmartCenterCommonParametersPtr& OdDbSmartCenterActionBody::parameters()
{
  return static_cast<OdDbImpSmartCenterActionBody *>(m_pImpObj)->parameters();
}

OdResult OdDbSmartCenterActionBody::addSubentitySpecialSettings(OdDbEntityPtr ent, const OdDbSubentId subentId, OdUInt32 idx)
{
  return static_cast<OdDbImpSmartCenterActionBody *>(m_pImpObj)->addSubentitySpecialSettings(ent, subentId, idx, this);
}
