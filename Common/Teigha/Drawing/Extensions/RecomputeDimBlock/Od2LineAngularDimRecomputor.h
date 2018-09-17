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

#ifndef __OD_2LINE_ANGULAR_DIM_RECOMPUTOR__
#define __OD_2LINE_ANGULAR_DIM_RECOMPUTOR__

#include "TD_PackPush.h"

#include "Od3PointAngularDimRecomputor.h"

/** \details
  This class implements a 2-Line Angular Dimension Block Recomputor.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class Od2LineAngularDimRecomputor : public Od3PointAngularDimRecomputor
{
public:
  Od2LineAngularDimRecomputor();
  virtual ~Od2LineAngularDimRecomputor();

  void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

  void buildDimension(OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
  void updateMeasurement(OdDbDimStyleTableRecord* pDimVars);

protected:
  virtual OdUInt8 getDimTypeFlag();

  OdGePoint3d m_ArcDefPt;
  // ext arc
  bool m_bExtArcOn;
  
  /** \details
    Returns the bounding vectors, centerpoint and *radius" of the Dimension entity associated with this DimRecomputor object.
    \param firstLineVector [out]  Receives the OCS Direction of first line.
    \param secondLineVector [out]  Receives the OCS Direction of second line.
    \param centerPoint [out]  Receives the OCS intersection of firstLine and secondLine.
    \param radius [out]  Receives the radius of the dimension arc.
    \param numSwaps [out]  Returns the number of times the vectors have been swapped.
    
    \note
    The angle from firstLineVector to secondLineVector is always <= 90°. The following algorithm is used.
    
    1. If the angle from firstLineVector to secondLineVector > 180°, the vectors are swapped.
    2. If the angle from firstLineVector to secondLineVector is > 90°, firstLineVector is mirrored, and the vectors are swapped.
  */
  void calculateBoundVecsRad(OdGePoint3d& centerPoint, OdGeVector3d& firstLineVector,
    OdGeVector3d& secondLineVector, double& radius, int& numSwaps);
};

/** \details
  This class implements a 2-Line Angular Dimension Block Recomputor Protocol Extension.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDb2LineAngularDimRecomputePEImpl : public OdDbDimRecomputePEImpl
{
protected:
  virtual OdDimRecomputorPtr createRecomputor();
};

#include "TD_PackPop.h"

#endif // __OD_2LINE_ANGULAR_DIM_RECOMPUTOR__
