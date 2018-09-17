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
#include "DbCenterMarkActionBody.h"
#include "DbImpCenterMarkActionBody.h"
#include "DbImpCenterLineActionBody.h"
#include "DbSmartCenterObjectsParameters.h"

ODRX_DEFINE_MEMBERS_EX(OdDbCenterMarkActionBody,      // ClassName
  OdDbSmartCenterActionBody, // ParentClass
  DBOBJECT_CONSTR,               // DOCREATE
  OdDb::kDHL_1027,               // DwgVer
  OdDb::kMRelease125,              // MaintVer
  1153,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbCenterMarkActionBody",   // DWG class name
  L"AcDbCenterMarkActionBody",   // DxfName
  L"AcDbSmartCenter",         // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbCenterMarkActionBody::OdDbCenterMarkActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpCenterMarkActionBody());
  else
    m_pImpObj = NULL;
}

OdDbCenterMarkActionBody::~OdDbCenterMarkActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbSmartCenterMarkParametersPtr OdDbCenterMarkActionBody::parameters() const
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbCenterMarkActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}
//
void OdDbCenterMarkActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
}

enum OdResult OdDbCenterMarkActionBody::setHorizontalEndOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("HorizontalEndOvershoot"));
}

enum OdResult OdDbCenterMarkActionBody::setHorizontalStartOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("HorizontalStartOvershoot"));
}

enum OdResult OdDbCenterMarkActionBody::setVerticalEndOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("VerticalEndOvershoot"));
}

enum OdResult OdDbCenterMarkActionBody::setVerticalStartOvershoot(double newOvershoot)
{
  return setGeometryParam(newOvershoot, OD_T("VerticalStartOvershoot"));
}

double OdDbCenterMarkActionBody::horizontalEndOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("HorizontalEndOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterMarkActionBody::horizontalStartOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("HorizontalStartOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterMarkActionBody::verticalEndOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("VerticalEndOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

double OdDbCenterMarkActionBody::verticalStartOvershoot() const
{
  double ret = -1.0;

  if (getGeometryParam(ret, OD_T("VerticalStartOvershoot")) != eOk)
    ret = -1.0;

  return ret;
}

OdString OdDbCenterMarkActionBody::crossHairGapExpression(void)const
{
  OdString ret = OdString("-1");

  if (getStringGeometryParam(ret, OD_T("CrossHairGapExpression")) != eOk)
    ret = OdString("-1");;

  return ret;
}

OdString OdDbCenterMarkActionBody::crossHairLengthExpression(void)const
{
  OdString ret = OdString("-1");

  if (getStringGeometryParam(ret, OD_T("CrossHairLengthExpression")) != eOk)
    ret = OdString("-1");;

  return ret;
}

enum OdResult OdDbCenterMarkActionBody::setCrossHairGapExpression(OdString const & str)
{
  return setStringGeometryParam(str, OD_T("CrossHairGapExpression"));
}
enum OdResult OdDbCenterMarkActionBody::setCrossHairLengthExpression(OdString const & str)
{
  return setStringGeometryParam(str, OD_T("CrossHairLengthExpression"));
}

enum OdResult OdDbCenterMarkActionBody::setExtensionLinesVisibility(OdDb::Visibility visible)
{
  double dVis = visible == OdDb::kVisible ? 1.0 : 0.0;

  OdResult ret = setGeometryParam(dVis, OD_T("eLeftLine_visibility"));
  OdResult ret1 = setGeometryParam(dVis, OD_T("eRightLine_visibility"), false);
  OdResult ret2 = setGeometryParam(dVis, OD_T("eBottomLine_visibility"), false);
  OdResult ret3 = setGeometryParam(dVis, OD_T("eTopLine_visibility"), false);
  ODA_ASSERT(ret == ret1 && ret == ret2 && ret == ret3);
  return ret;
}

//OdDb::Visibility OdDbCenterMarkActionBody::getExtensionLinesVisibility() const// default return value is true
//{
//  double ret = -1.0;
//  OdDb::Visibility bRet = OdDb::kVisible;
//
//  if (getGeometryParam(ret, OD_T("eLeftLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (getGeometryParam(ret, OD_T("eRightLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (getGeometryParam(ret, OD_T("eBottomLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (getGeometryParam(ret, OD_T("eTopLine_visibility")) != eOk)
//  {
//    ret = -1.0;
//    return OdDb::kVisible;
//  }
//
//  if (ret = 0.0)
//    bRet = OdDb::kInvisible;
//
//  return bRet;
//}

enum OdResult OdDbCenterMarkActionBody::setRotation(double rotation)// Radian
{
  return static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->setRotation(rotation, parentAction());
}

double OdDbCenterMarkActionBody::rotation(void)	const// Radian
{
  return static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->rotation(parentAction());
}

OdResult OdDbCenterMarkActionBody::createInstance(
  const OdDbFullSubentPath subentPath,
  OdDbObjectId& centerMarkId,
  OdDbObjectId& actionBodyId)
{
  return OdDbImpCenterMarkActionBody::createInstance(subentPath, centerMarkId, actionBodyId);
}


OdResult OdDbCenterMarkActionBody::getRefCircle(OdGeCircArc3d & arc)const
{
  return static_cast<OdDbImpCenterMarkActionBody *>
    (m_pImpObj)->getRefCircle(arc, parentAction());
}

void OdDbCenterMarkActionBody::modified(const OdDbObject* pObj)
{
  if (!pObj->isUndoing())
  {
    static_cast<OdDbImpCenterMarkActionBody *>
      (m_pImpObj)->evaluateOverride(parentAction());
  }
}
