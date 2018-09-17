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
#include "NextCodeDefs.h"

OdDbImpAssocViewLabelActionParam::OdDbImpAssocViewLabelActionParam()
{
}

OdDbImpAssocViewLabelActionParam::~OdDbImpAssocViewLabelActionParam()
{
}

OdResult OdDbImpAssocViewLabelActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocObjectActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  int nVersion  = pFiler->rdInt16();              // = 1 - version

  if (nVersion != 1)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  m_nPropertyOverrides = pFiler->rdInt16();     // flags Overrides
  m_vLabelOffset       = pFiler->rdVector2d();            
  m_nAttachPoint       = pFiler->rdUInt8();     // OdDbModelDocViewStyle::kAboveView | kBelowView

  m_vInit = m_vLabelOffset;                     // use to return the original value of the vector m_vLabelOffset after the call
                                                // setAttachmentPointOverride (AcDbModelDocViewStyle::kBelowView, true)
  return eOk;
}

void OdDbImpAssocViewLabelActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dwgOutFields(pFiler, objectId);
  pFiler->wrInt16(1);                           // Version
  pFiler->wrInt16(m_nPropertyOverrides);        // flags Overrides
  pFiler->wrVector2d(m_vLabelOffset);
  pFiler->wrUInt8(m_nAttachPoint);              // OdDbModelDocViewStyle::kAboveView | kBelowView
}

OdResult OdDbImpAssocViewLabelActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocObjectActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocViewLabelActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(70);
  int nVersion  = pFiler->rdUInt16();              // = 1 - version
  if (nVersion != 1)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(70);
  m_nPropertyOverrides = pFiler->rdUInt16();     // flags Overrides

  NEXT_CODE(210)
  pFiler->rdVector2d(m_vLabelOffset);

  NEXT_CODE(280);
  m_nAttachPoint = pFiler->rdUInt8();            // OdDbModelDocViewStyle::kAboveView | kBelowView

  m_vInit = m_vLabelOffset;

  ODA_ASSERT_ONCE(pFiler->atEOF());
  return eOk;
}

void OdDbImpAssocViewLabelActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocObjectActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(OdDbAssocViewLabelActionParam::desc()->name());    // class Marker

  pFiler->wrUInt16(70, 1);                       // Version
  pFiler->wrUInt16(70, m_nPropertyOverrides);    // flags Overrides
  pFiler->wrVector2d(210, m_vLabelOffset);
  pFiler->wrUInt8(280, m_nAttachPoint);          // OdDbModelDocViewStyle::kAboveView | kBelowView
}
