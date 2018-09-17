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
#include "DbCenterLineActionBody.h"
#include "DbImpCenterMarkActionBody.h"
#include "DbImpCenterLineActionBody.h"
#include "DbSmartCenterObjectsParameters.h"
///////////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbCenterLineActionBody,      // ClassName
  OdDbSmartCenterActionBody, // ParentClass
  DBOBJECT_CONSTR,               // DOCREATE
  OdDb::kDHL_1027,               // DwgVer
  OdDb::kMRelease125,              // MaintVer
  1153,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbCenterLineActionBody",   // DWG class name
  L"AcDbCenterLineActionBody",   // DxfName
  L"AcDbSmartCenter",         // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbCenterLineActionBody::OdDbCenterLineActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpCenterLineActionBody());
  else
    m_pImpObj = NULL;
}

OdDbCenterLineActionBody::~OdDbCenterLineActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbSmartCenterLineParametersPtr OdDbCenterLineActionBody::parameters() const
//{
//  return static_cast<OdDbImpCenterLineActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterLineParametersPtr& OdDbCenterLineActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterLineActionBody *>(m_pImpObj)->parameters();
//}

void OdDbCenterLineActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  static_cast<OdDbImpCenterLineActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
}

enum OdResult OdDbCenterLineActionBody::setEndOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("EndOvershoot"));
}

enum OdResult OdDbCenterLineActionBody::setStartOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("StartOvershoot"));
}

double OdDbCenterLineActionBody::endOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("EndOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterLineActionBody::startOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("StartOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

OdResult OdDbCenterLineActionBody::createInstance(
  const OdDbFullSubentPathArray entPathArray,
  OdArray<int> inputArr,
  OdDbObjectId& centerLineId,
  OdDbObjectId& actionBodyId)
{
  return OdDbImpCenterLineActionBody::createInstance(entPathArray, inputArr, centerLineId, actionBodyId);
}

OdResult OdDbCenterLineActionBody::createInstance(
  const OdDbFullSubentPathArray entPathArray,
  //OdGePoint3dArray ptsSelected,
  OdDbObjectId& centerLineId,
  OdDbObjectId& actionBodyId)
{
  return OdDbImpCenterLineActionBody::createInstance(entPathArray, /*ptsSelected,*/ centerLineId, actionBodyId);
}

OdResult OdDbCenterLineActionBody::getCenterLineGeometry(OdGeLineSeg3d & segm3d, bool boolCondition)
{
  return static_cast<OdDbImpCenterLineActionBody *>
    (m_pImpObj)->getCenterLineGeometry(segm3d, boolCondition, parentAction());
}

void OdDbCenterLineActionBody::modified(const OdDbObject* pObj)
{
  if (!pObj->isUndoing())
  {
    static_cast<OdDbImpCenterLineActionBody *>
      (m_pImpObj)->evaluateOverride(parentAction());
  }
}

OdResult OdDbCenterLineActionBody::edgeRefDirEndingArray(OdArray<int> & outputArr)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentAction().safeOpenObject(OdDb::kForWrite));
  
  return static_cast<OdDbImpCenterLineActionBody *>
    (m_pImpObj)->edgeRefDirEndingArray(pAction, outputArr);
}

OdResult OdDbCenterLineActionBody::setEdgeRefDirEndingArray(OdArray<int> & inputArr)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentAction().safeOpenObject(OdDb::kForWrite));

  return static_cast<OdDbImpCenterLineActionBody *>
    (m_pImpObj)->setEdgeRefDirEndingArray(pAction, inputArr);
  return eOk;
}
