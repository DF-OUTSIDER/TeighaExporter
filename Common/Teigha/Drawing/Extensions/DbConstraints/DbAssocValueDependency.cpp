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
#include "DbAssocValueDependency.h"
#include "DbImpAssocValueDependency.h"
#include "DbAssocVariable.h"

#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocValueDependency,    // ClassName
                       OdDbAssocDependency,         // ParentClass
                       DBOBJECT_CONSTR,             // DOCREATE
                       OdDb::kDHL_1021,             // DwgVer
                       OdDb::kMRelease6,            // MaintVer
                       1025,                        // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocValueDependency", // DWG class name
                       L"ACDBASSOCVALUEDEPENDENCY", // DxfName
                       L"ObjectDBX Classes",        // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocValueDependency::OdDbAssocValueDependency(bool createImp) : OdDbAssocDependency(false)
{
  if ( createImp )
    m_pImpObj = new OdDbImpAssocValueDependency();
}

const OdString& OdDbAssocValueDependency::valueName() const
{
  assertReadEnabled();
  return ((OdDbImpAssocValueDependency*)m_pImpObj)->valueName();
}

OdResult OdDbAssocValueDependency::setValueName(const OdString& newValueName)
{
  assertWriteEnabled();
  return ((OdDbImpAssocValueDependency*)m_pImpObj)->setValueName(newValueName);
}

OdResult OdDbAssocValueDependency::getDependentOnObjectValue(OdDbEvalVariantPtr& objectValue) const
{
  assertReadEnabled();
  return ((OdDbImpAssocValueDependency*)m_pImpObj)->getDependentOnObjectValue(objectValue);
}

OdResult OdDbAssocValueDependency::setDependentOnObjectValue(const OdDbEvalVariantPtr& newObjectValue)
{
  assertWriteEnabled();
  return ((OdDbImpAssocValueDependency*)m_pImpObj)->setDependentOnObjectValue(newObjectValue);
}
