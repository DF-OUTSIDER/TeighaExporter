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

#include "OdParallelConstraintImpl.h"
#include "DbFiler.h"

OdParallelConstraintImpl::OdParallelConstraintImpl():OdGeomConstraintImpl(),
                           m_datumLineIdx(OdConstraintGroupNode::kNullGroupNodeId)
{
}

OdParallelConstraintImpl::~OdParallelConstraintImpl()
{
}

OdResult OdParallelConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdGeomConstraintImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  m_datumLineIdx = pFiler->rdInt32();
  return res;
}

void OdParallelConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdGeomConstraintImpl::dwgOutFields(pFiler);
  pFiler->wrInt32(m_datumLineIdx);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdParallelConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdGeomConstraintImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(90)
  m_datumLineIdx = pFiler->rdInt32();

  return res;
}

#undef NEXT_CODE

void OdParallelConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdGeomConstraintImpl::dxfOutFields(pFiler);
  pFiler->wrInt32(90, m_datumLineIdx);
}

void OdParallelConstraintImpl::setDatumLineIdx(OdConstraintGroupNodeId id)
{
  m_datumLineIdx = id;
}
