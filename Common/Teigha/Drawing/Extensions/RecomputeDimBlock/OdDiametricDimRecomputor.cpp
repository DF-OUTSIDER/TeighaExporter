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
#include "OdDiametricDimRecomputor.h"
#include "DbDiametricDimension.h"
#include "RxObjectImpl.h"
#include "DbObjectContextData.h"

OdDiametricDimRecomputor::OdDiametricDimRecomputor()
{
}

OdDiametricDimRecomputor::~OdDiametricDimRecomputor()
{
}

OdUInt8 OdDiametricDimRecomputor::getDimTypeFlag()
{
  return DimDiametric;
}
  
OdDimRecomputorPtr OdDbDiametricDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdDiametricDimRecomputor>::createObject();
}

void OdDiametricDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbDiametricDimensionPtr pDimPtr(pDim);
  m_DefPointCenter = ctx ? ((OdDbDiametricDimensionObjectContextData*)ctx)->chordPoint() : pDimPtr->chordPoint();
  m_DimLineDefPt = ctx ? ((OdDbDiametricDimensionObjectContextData*)ctx)->farChordPoint() : pDimPtr->farChordPoint();
  m_dLeaderLen = pDimPtr->leaderLength();
  m_bExtArcOn = pDimPtr->extArcOn();
  m_dExtArcStartAngle = pDimPtr->extArcStartAngle();
  m_dExtArcEndAngle = pDimPtr->extArcEndAngle();
}

void OdDiametricDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDbDiametricDimensionPtr pDimPtr(pDim);
  // pDimPtr->setChordPoint(m_DefPointCenter);
  // pDimPtr->setFarChordPoint(m_DimLineDefPt);
  if ( m_bNeedCorrectTextPos )
  {
    pDimPtr->useSetTextPosition();
    pDimPtr->setLeaderLength(m_dLeaderLen);
  }
}


////

// None
