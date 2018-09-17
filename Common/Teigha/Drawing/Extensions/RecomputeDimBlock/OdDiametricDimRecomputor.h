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


#ifndef __OD_DIAMETRIC_DIM_RECOMPUTOR__
#define __OD_DIAMETRIC_DIM_RECOMPUTOR__

#include "OdRadialDimRecomputor.h"

/** \details
  This class implements a Diametric Dimension Block Recomputor.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class OdDiametricDimRecomputor : public OdRadialDimRecomputor
{
public:
  void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

protected:
  OdDiametricDimRecomputor();
  virtual ~OdDiametricDimRecomputor();

  virtual OdUInt8 getDimTypeFlag();
};

/** \details
  This class implements a Diametric Dimension Block Recomputor Protocol Extension.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDbDiametricDimRecomputePEImpl : public OdDbDimRecomputePEImpl
{
protected:
  virtual OdDimRecomputorPtr createRecomputor();
};

#endif // __OD_DIAMETRIC_DIM_RECOMPUTOR__
