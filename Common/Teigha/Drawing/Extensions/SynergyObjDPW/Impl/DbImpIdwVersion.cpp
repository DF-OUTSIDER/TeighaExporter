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
#include "DbImpIdwVersion.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"

OdDbImpIdwVersion::OdDbImpIdwVersion()
  : m_vession(1)
{
}

OdDbImpIdwVersion::~OdDbImpIdwVersion()
{
}

OdResult OdDbImpIdwVersion::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  // handle : x352 of DetailAndSectionViews.dwg

  m_vession = pFiler->rdInt32();
  ODA_ASSERT_ONCE(m_vession == 1); // test

  return eOk;
}

void OdDbImpIdwVersion::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  ODA_ASSERT_ONCE(m_vession == 1); // test
  pFiler->wrInt32(m_vession);
}

OdResult OdDbImpIdwVersion::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  // handle : x352 of DetailAndSectionViews.dxf

  if (!pFiler->atSubclassData(OdDbIdwVersion::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_vession = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(m_vession == 1); // test

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpIdwVersion::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrSubclassMarker(OdDbIdwVersion::desc()->name());

  ODA_ASSERT_ONCE(m_vession == 1); // test
  pFiler->wrUInt32(90, m_vession);
}
