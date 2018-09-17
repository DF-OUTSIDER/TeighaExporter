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
#include "NextCodeDefs.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbAssocDependency.h"
#include "DbObjectId.h"
#include "DbPolyline.h"
#include "DbAssocTrimSurfaceActionBody.h"
#include "DbExtrudedSurface.h"
#include "DbImpAssocTrimSurfaceActionBody.h"

OdDbImpAssocTrimSurfaceActionBody::OdDbImpAssocTrimSurfaceActionBody()
{
  m_undefined = 0;
}

OdDbImpAssocTrimSurfaceActionBody::~OdDbImpAssocTrimSurfaceActionBody()
{
}

OdResult OdDbImpAssocTrimSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_undefined = pFiler->rdInt32();
  m_unknownBool1 = pFiler->rdBool();
  m_unknownBool2 = pFiler->rdBool();
  m_unknownDouble = pFiler->rdDouble();  // 0
  return res;
}

void OdDbImpAssocTrimSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dwgOutFields(pFiler);
  pFiler->wrInt32(m_undefined);
  pFiler->wrBool(m_unknownBool1);
  pFiler->wrBool(m_unknownBool2);
  pFiler->wrDouble(m_unknownDouble);
}

OdResult OdDbImpAssocTrimSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocTrimSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_undefined = pFiler->rdUInt32();
  NEXT_CODE(290)
    m_unknownBool1 = pFiler->rdBool();
  NEXT_CODE(290)
    m_unknownBool2 = pFiler->rdBool();
  NEXT_CODE(40)
    m_unknownDouble = pFiler->rdDouble();
  return eOk;
}

void OdDbImpAssocTrimSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocTrimSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_undefined);
  pFiler->wrBool(290, m_unknownBool1);
  pFiler->wrBool(290, m_unknownBool2);
  pFiler->wrDouble(40, m_unknownBool1);
}

void OdDbImpAssocTrimSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));

  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbExtrudedSurfacePtr pSurface = OdDbExtrudedSurface::cast(bEntRef);
}
