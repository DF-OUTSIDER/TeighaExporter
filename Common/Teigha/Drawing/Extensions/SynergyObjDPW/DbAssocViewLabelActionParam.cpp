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
#include "DbImpAssocViewLabelActionParam.h"
#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocViewLabelActionParam,    // ClassName
                       OdDbAssocObjectActionParam,       // ParentClass
                       DBOBJECT_CONSTR,                  // DOCREATE
                       OdDb::kDHL_1027,                  // DwgVer
                       OdDb::kMRelease8,                 // MaintVer
                       1025,                             // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocViewLabelActionParam", // DWG class name
                       L"ACDBASSOCVIEWLABELACTIONPARAM", // DxfName
                       L"AcSynergyObjDPW",               // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocViewLabelActionParam::OdDbAssocViewLabelActionParam(bool createImpObj)
 : OdDbAssocObjectActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocViewLabelActionParam();
}

OdResult OdDbAssocViewLabelActionParam::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dwgInFields(pFiler);

  if (res != eOk)
    return res;
  return m_pImpObj->dwgInFields(pFiler, objectId());
}

void OdDbAssocViewLabelActionParam::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocViewLabelActionParam::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dxfInFields(pFiler, objectId());
}

void OdDbAssocViewLabelActionParam::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler, objectId());
}

OdDbObjectId OdDbAssocViewLabelActionParam::viewLabelId() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->dependentOnObjectId();
}
//---------------------------------------------------------------------------------------------------------------------------------
// Functions, reading flags

bool OdDbAssocViewLabelActionParam::isLabelOffsetOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kLabelOffsetOverriden );
}

bool OdDbAssocViewLabelActionParam::isAttachmentPointOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kAttachmentPointOverriden );
}

bool OdDbAssocViewLabelActionParam::isTextStyleOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kTextStyleOverriden );
}

bool OdDbAssocViewLabelActionParam::isColorOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kColorOverriden );
}


bool OdDbAssocViewLabelActionParam::isTextHeightOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kTextHeightOverriden );
}

bool OdDbAssocViewLabelActionParam::isTextFieldOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kTextFieldOverriden );
}

bool OdDbAssocViewLabelActionParam::isDirectionOverriden() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kDirectionOverriden );
}

bool OdDbAssocViewLabelActionParam::isLabelPlacementCustomized() const
{
  assertReadEnabled();
  return GETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
                 kLabelPlacementCustomized );
}
//---------------------------------------------------------------------------------------------------------------------------------
// Functions, changing flags

void OdDbAssocViewLabelActionParam::setAttachmentPointOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kAttachmentPointOverriden, true );
}

void OdDbAssocViewLabelActionParam::setColorOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kColorOverriden, true );
}

void OdDbAssocViewLabelActionParam::setDirectionOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kDirectionOverriden, true );
}

void OdDbAssocViewLabelActionParam::setLabelOffsetOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kLabelOffsetOverriden, true );
}

void OdDbAssocViewLabelActionParam::setLabelPlacementCustomized(bool flag)
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kLabelPlacementCustomized, flag );
}

void OdDbAssocViewLabelActionParam::setTextFieldOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kTextFieldOverriden, true );
}

void OdDbAssocViewLabelActionParam::setTextHeightOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kTextHeightOverriden, true );
}

void OdDbAssocViewLabelActionParam::setTextStyleOverride()
{
  assertWriteEnabled();
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kTextStyleOverriden, true );
}
//---------------------------------------------------------------------------------------------------------------------------------
// Other functions

OdGeVector2d OdDbAssocViewLabelActionParam::labelOffset() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_vLabelOffset;
}

void OdDbAssocViewLabelActionParam::setLabelOffset(OdGeVector2d const & Vector2d)
{
  assertWriteEnabled();
  ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_vLabelOffset = Vector2d;
}

void OdDbAssocViewLabelActionParam::setLabelOffsetOverride(OdGeVector2d const & Vector2d)
{
  assertWriteEnabled();
  ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_vLabelOffset = Vector2d;
  SETBIT( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nPropertyOverrides, 
          kLabelOffsetOverriden, true );
}

enum OdDbModelDocViewStyle::AttachmentPoint OdDbAssocViewLabelActionParam::attachmentPoint() const
{
  assertReadEnabled();
  return (OdDbModelDocViewStyle::AttachmentPoint)( ((OdDbImpAssocViewLabelActionParam*)m_pImpObj)->m_nAttachPoint );
}

void OdDbAssocViewLabelActionParam::setAttachmentPointOverride(OdDbModelDocViewStyle::AttachmentPoint attPoint, bool bRestore)
{
  assertWriteEnabled();
  OdDbImpAssocViewLabelActionParam* pImpObj = (OdDbImpAssocViewLabelActionParam*)m_pImpObj;

  SETBIT(pImpObj->m_nPropertyOverrides, kAttachmentPointOverriden, true); // flag is set

  pImpObj->m_nAttachPoint = (int)attPoint ;            // attPoint = OdDbModelDocViewStyle::kAboveView | kBelowView

  if (bRestore)
  {  // restored the initial value
    pImpObj->m_vLabelOffset = pImpObj->m_vInit;
    if (attPoint != OdDbModelDocViewStyle::kAboveView)
    {
      pImpObj->m_vLabelOffset *= -1;
    }
    SETBIT(pImpObj->m_nPropertyOverrides, kLabelOffsetOverriden, false); // canceled flags: setLabelOffsetOverride
    SETBIT(pImpObj->m_nPropertyOverrides, kDirectionOverriden,   false); //                 setDirectionOverride
  }
}
