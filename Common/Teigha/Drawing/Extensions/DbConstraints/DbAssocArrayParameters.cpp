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
#include "DbAssocArrayParameters.h"
#include "DbImpAssocArrayParameters.h"
#include "DbAssocArrayActionBody.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocArrayParameters, OdRxObject, L"AcDbAssocArrayParameters")

//protected
OdDbAssocArrayParameters::OdDbAssocArrayParameters(class OdDbImpAssocArrayParameters* pImpl)
{
  m_pImpl = pImpl;
}

OdDbAssocArrayParameters::~OdDbAssocArrayParameters()
{
  if (m_pImpl)
  {
    delete m_pImpl;
    m_pImpl = NULL;
  }
}

OdResult OdDbAssocArrayParameters::setValueParam(const OdString& paramName,
                                                 const OdDbEvalVariant& value,
                                                 const OdString& expression,
                                                 const OdString& evaluatorId,
                                                 OdString& errorMessage,
                                                 OdValue::UnitType* pUnitType) // = NULL
{
  return m_pImpl->setValueParam(paramName, value, expression, evaluatorId, errorMessage, pUnitType);
}

OdResult OdDbAssocArrayParameters::getValueParam(const OdString& paramName,
                                                 OdDbEvalVariant& value,
                                                 OdString& expression,
                                                 OdString& evaluatorId,
                                                 OdValue::UnitType& unitType) const
{
  return m_pImpl->getValueParam( paramName, value, expression, evaluatorId, unitType);
}

OdResult OdDbAssocArrayParameters::setGeomParam(const OdString& paramName,
                                                const OdDbGeomRef* pGeomRef,
                                                OdDbObjectId& paramId)
{
  return m_pImpl->setGeomParam( paramName, pGeomRef, paramId );
}

OdResult OdDbAssocArrayParameters::getGeomParam(const OdString& paramName,
                                                OdDbGeomRefPtr& pGeomRef,
                                                OdDbObjectId& paramId) const
{
  return m_pImpl->getGeomParam( paramName, pGeomRef, paramId );
}

void OdDbAssocArrayParameters::ownedValueParamNames(OdStringArray& paramNames) const
{
  return m_pImpl->ownedValueParamNames( paramNames );
}

void OdDbAssocArrayParameters::ownedGeomParamNames(OdStringArray& paramNames) const
{
  return m_pImpl->ownedGeomParamNames( paramNames );
}

OdResult OdDbAssocArrayParameters::setOwner(class OdDbAssocArrayActionBody* pOwner)
{  
  return m_pImpl->setOwner( pOwner );
}

const OdDbAssocArrayActionBody* OdDbAssocArrayParameters::owner() const
{
  return m_pImpl->m_pOwner;
}

OdDbAssocArrayActionBody* OdDbAssocArrayParameters::owner()
{
  return m_pImpl->m_pOwner;
}

void OdDbAssocArrayParameters::copyFrom(const OdRxObject* other)
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
}

OdResult OdDbAssocArrayParameters::getGripPointAt(unsigned int mode, OdDbGripDataPtrArray& pGrip) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbAssocArrayParameters::getGripPoints(OdArray<OdDbGripDataPtrArray>& grips) const
{
  return m_pImpl->getGripPointAt( grips );
}

OdResult OdDbAssocArrayParameters::moveGripPointAt(const OdDbGripDataPtrArray pGrip,
                                                   const OdGeVector3d &offset, 
                                                   int& modificationBit)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbAssocArrayParameters::dwgInFields(OdDbDwgFiler* pFiler)
{
  if (!m_pImpl)
  {
    ODA_FAIL_ONCE(); 
    return eMakeMeProxy;
  }
  return m_pImpl->dwgInFields(pFiler);
}

void OdDbAssocArrayParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  if (!m_pImpl)
  {
    ODA_FAIL_ONCE(); 
    throw OdError(eMakeMeProxy);
  }
  m_pImpl->dwgOutFields(pFiler);
}

OdResult OdDbAssocArrayParameters::dxfInFields(OdDbDxfFiler* pFiler)
{
  if (!m_pImpl)
  {
    ODA_FAIL_ONCE(); 
    return eMakeMeProxy;
  }
  return m_pImpl->dxfInFields(pFiler);
}

void OdDbAssocArrayParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  if (!m_pImpl)
  {
    ODA_FAIL_ONCE(); 
    throw OdError(eMakeMeProxy);
  }
  m_pImpl->dxfOutFields(pFiler);
}

void OdDbAssocArrayParameters::composeForLoad( OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo )
{
  m_pImpl->composeForLoad(this, format, version);
}


