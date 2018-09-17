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
#include "DbImpAssocViewRepHatchManager.h"
#include "DbAssocViewRepHatchActionParam.h"
#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocViewRepHatchManager,    // ClassName
                       OdDbAssocCompoundActionParam,    // ParentClass
                       DBOBJECT_CONSTR,                 // DOCREATE
                       OdDb::kDHL_1027,                 // DwgVer
                       OdDb::kMRelease8,                // MaintVer
                       1,                               // nProxyFlags (kEraseAllowed)
                       L"AcDbAssocViewRepHatchManager", // DWG class name
                       L"ACDBASSOCVIEWREPHATCHMANAGER", // DxfName
                       L"AcSynergyObjDPW",              // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocViewRepHatchManager::OdDbAssocViewRepHatchManager(bool createImpObj)
 : OdDbAssocCompoundActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocViewRepHatchManager();
}

OdResult OdDbAssocViewRepHatchManager::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dwgInFields(pFiler);

  if (res != eOk)
    return res;
  return m_pImpObj->dwgInFields(pFiler, objectId());
}

void OdDbAssocViewRepHatchManager::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocViewRepHatchManager::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dxfInFields(pFiler, objectId());
}

void OdDbAssocViewRepHatchManager::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler, objectId());
}
//---------------------------------------------------------------------------------------------------------------------------------

bool OdDbAssocViewRepHatchManager::isEmpty()  const                          
{
  return ((OdDbImpAssocViewRepHatchManager*)m_pImpObj)->m_Count_ActionParam == 0;  // Count of DbAssocViewRepHatchActionParam
}
//---------------------------------------------------------------------------------------------------------------------------------

OdResult OdDbAssocViewRepHatchManager::getShowHatch(bool& bResult) const
{
  assertReadEnabled();

  OdDbObjectIdArray hatchActionParamIds;
  viewRepHatches( hatchActionParamIds );
  if ( hatchActionParamIds.empty() )
    return eInvalidInput;

  OdDbAssocViewRepHatchActionParamPtr pViewRepHatchParam =
    OdDbAssocActionParam::cast( hatchActionParamIds.first().openObject( OdDb::kForRead ) );
  ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

  bResult = !GETBIT(pViewRepHatchParam->propertyOverrides(), 6);
  return eOk;
}

OdResult OdDbAssocViewRepHatchManager::setShowHatch( bool bShow, bool bUnknown )
{
  assertWriteEnabled();
  OdDbObjectIdArray hatchActionParamIds;
  viewRepHatches( hatchActionParamIds );

  OdDbObjectIdArray::iterator it_HatchActionParamId;
  for ( it_HatchActionParamId = hatchActionParamIds.begin();
        it_HatchActionParamId != hatchActionParamIds.end();
        it_HatchActionParamId++ )
  {
    OdDbAssocViewRepHatchActionParamPtr pViewRepHatchParam =
      OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForWrite ) );
    ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

    OdUInt32 prop = pViewRepHatchParam->propertyOverrides();
    SETBIT( prop, 6, !bShow );
    pViewRepHatchParam->setPropertyOverrides( prop );
  }
  return eOk;
}

void OdDbAssocViewRepHatchManager::viewRepHatches( OdDbObjectIdArray &hatchActionParamIds ) const
{
  hatchActionParamIds = ((OdDbImpAssocViewRepHatchManager*)m_pImpObj)->m_AssocViewRepHatchActionParam;
}

//---------------------------------------------------------------------------------------------------------------------------------
