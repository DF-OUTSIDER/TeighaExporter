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

#ifndef __OD_ROTATED_DIM_RECOMPUTOR__
#define __OD_ROTATED_DIM_RECOMPUTOR__

#include "TD_PackPush.h"

#include "OdAlignedDimRecomputor.h"

/** \details
  This class implements a Rotate Dimension Block Recomputor.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class OdRotatedDimRecomputor : public OdAlignedDimRecomputor
{
public:
  void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

  void preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars);
  void updateMeasurement(OdDbDimStyleTableRecord* pDimVars);

  double m_dRotAngle;

protected:
  OdRotatedDimRecomputor();
  virtual ~OdRotatedDimRecomputor();

  virtual OdUInt8 getDimTypeFlag();
  virtual void calculateExtLinePoints(OdGePoint3d& extLine1Start, OdGePoint3d& extLine1End,
    OdGePoint3d& extLine2Start, OdGePoint3d& extLine2End,
    const OdGePoint3d& dimLineDefPt, const OdGePoint3d& defPoint1, const OdGePoint3d& defPoint2);
};

/** \details
  This class implements a Rotated Dimension Block Recomputor Protocol Extension.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDbRotatedDimRecomputePEImpl : public OdDbDimRecomputePEImpl
{
protected:
  virtual OdDimRecomputorPtr createRecomputor();
};

#include "TD_PackPop.h"

#endif // __OD_ROTATED_DIM_RECOMPUTOR__
