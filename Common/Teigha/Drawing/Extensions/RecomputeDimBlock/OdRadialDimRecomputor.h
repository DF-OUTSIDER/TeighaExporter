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

#ifndef __OD_RADIAL_DIM_RECOMPUTOR__
#define __OD_RADIAL_DIM_RECOMPUTOR__

#include "TD_PackPush.h"

#include "OdDimRecomputor.h"

/** \details
  This class implements a Large Radial Dimension Block Recomputor.
  <group RecomputeDimBlock_Classes>
  Library: Source provided.
*/
class OdRadialDimRecomputor : public OdDimRecomputor
{
public:
  void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

  void buildDimension(OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
  void preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars);
  void updateMeasurement(OdDbDimStyleTableRecord* pDimVars);
  void formatMeasurement(OdString& measurement, OdDbDimStyleTableRecord* pDimVars);

protected:
  OdRadialDimRecomputor();
  virtual ~OdRadialDimRecomputor();

  OdGePoint3d m_DefPointCenter;
  double      m_dLeaderLen;

  bool m_bExtArcOn;
  double m_dExtArcStartAngle;
  double m_dExtArcEndAngle;

  bool m_bNeedCorrectTextPos;
  virtual OdUInt8 getDimTypeFlag();

  virtual void formatDimStrWithLimsAndTols(OdString& formattedString, bool hasLimits, bool hasTols,
    double measurement, OdInt16 dimlunit,
    double dimtm, double dimtp, OdInt16 dimtzin, double dimtfac,
    OdInt16 dimtdec, double dimrnd, OdChar dimdsep,
    OdInt16 dimfrac, double dimtxt, OdInt16 dimzin, OdInt16 dimdec,
    OdString dimpost, bool bPrimaryUnits);
};

/** \details
  This class implements a Radial Dimension Block Recomputor Protocol Extension.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDbRadialDimRecomputePEImpl : public OdDbDimRecomputePEImpl
{
protected:
  virtual OdDimRecomputorPtr createRecomputor();
};

#include "TD_PackPop.h"

#endif // __OD_RADIAL_DIM_RECOMPUTOR__
