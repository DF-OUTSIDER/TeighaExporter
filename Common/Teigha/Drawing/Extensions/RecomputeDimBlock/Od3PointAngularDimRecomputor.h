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

#ifndef __OD_3POINT_ANGULAR_DIM_RECOMPUTOR__
#define __OD_3POINT_ANGULAR_DIM_RECOMPUTOR__

#include "TD_PackPush.h"

#include "OdDimRecomputor.h"

/** \details
  This class implements a 3-Point Angular Dimension Block Recomputor.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class Od3PointAngularDimRecomputor : public OdDimRecomputor
{
public:
  void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

  void buildDimension(OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
  void preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars);
  void updateMeasurement(OdDbDimStyleTableRecord* pDimVars);
  void formatMeasurement(OdString& formattedMeasurement, OdDbDimStyleTableRecord* pDimVars);

protected:
  Od3PointAngularDimRecomputor();
  virtual ~Od3PointAngularDimRecomputor();

  OdGePoint3d m_DefPoint1, m_DefPoint2, m_DefPoint3;

  // ext arc
  bool m_bExtArcOn;

  virtual OdUInt8 getDimTypeFlag();
  void formatDimStr(OdString& formattedString, double measurement, OdInt16 units, OdInt16 precision, double roundOff,
    OdInt16 tzin, OdChar separator, OdInt16 fracFormat, bool forLimits, bool forTolerances, double dimTfac = 1.0);
};

/** \details
  This class implements a 3-Point Angular Dimension Block Recomputor Protocol Extension.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDb3PointAngularDimRecomputePEImpl : public OdDbDimRecomputePEImpl
{
protected:
  virtual OdDimRecomputorPtr createRecomputor();
};

#include "TD_PackPop.h"

#endif // __OD_3POINT_ANGULAR_DIM_RECOMPUTOR__
