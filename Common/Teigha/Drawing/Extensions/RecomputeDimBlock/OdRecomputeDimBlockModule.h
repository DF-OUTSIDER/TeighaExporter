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

#ifndef __OD_RECOMPUTE_DIM_BLOCK_MODULE__
#define __OD_RECOMPUTE_DIM_BLOCK_MODULE__

#include "TD_PackPush.h"

#include "RxModule.h"
#include "StaticRxObject.h"

#include "OdAlignedDimRecomputor.h"
#include "OdRotatedDimRecomputor.h"
#include "Od2LineAngularDimRecomputor.h"
#include "Od3PointAngularDimRecomputor.h"
#include "OdRadialDimRecomputor.h"
#include "OdDiametricDimRecomputor.h"
#include "OdOrdinateDimRecomputor.h"
#include "OdRadialDimLargeRecomputor.h"
#include "OdArcDimRecomputor.h"
#include "OdMLeaderRecomputor.h"

/** \details
  This class Dimension Block Recomputor Module.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdRecomputeDimBlockModule : public OdRxModule
{
  OdStaticRxObject<OdDbAlignedDimRecomputePEImpl>       m_alignedDimRecomputor;
  OdStaticRxObject<OdDbRotatedDimRecomputePEImpl>       m_rotatedDimRecomputor;
  OdStaticRxObject<OdDb2LineAngularDimRecomputePEImpl>  m_2LineAngularDimRecomputor;
  OdStaticRxObject<OdDb3PointAngularDimRecomputePEImpl> m_3PointAngularDimRecomputor;
  OdStaticRxObject<OdDbRadialDimRecomputePEImpl>        m_radialDimRecomputor;
  OdStaticRxObject<OdDbDiametricDimRecomputePEImpl>     m_diametricDimRecomputor;
  OdStaticRxObject<OdDbOrdinateDimRecomputePEImpl>      m_ordinateDimRecomputor;
  OdStaticRxObject<OdDbArcDimRecomputePEImpl>           m_arcDimRecomputor;
  OdStaticRxObject<OdDbRadialDimLargeRecomputePEImpl>   m_radialDimLargeRecomputor;
  OdStaticRxObject<OdDbMLeaderRecomputePEImpl>          m_mleaderRecomputor;

protected:
  OdRecomputeDimBlockModule();
  void initApp();
  void uninitApp();

public:
  virtual ~OdRecomputeDimBlockModule();
};

#include "TD_PackPop.h"

#endif // __OD_RECOMPUTE_DIM_BLOCK_MODULE__
