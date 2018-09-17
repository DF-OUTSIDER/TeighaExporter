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
#include "RxDynamicModule.h"

#include "DbAlignedDimension.h"
#include "DbRotatedDimension.h"
#include "Db2LineAngularDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbRadialDimension.h"
#include "DbDiametricDimension.h"
#include "DbOrdinateDimension.h"
#include "DbArcDimension.h"
#include "DbRadialDimensionLarge.h"

#include "OdRecomputeDimBlockModule.h"

//DD:EXPORT_ON
ODRX_DEFINE_DYNAMIC_MODULE(OdRecomputeDimBlockModule);
//DD:EXPORT_OFF

OdRecomputeDimBlockModule::OdRecomputeDimBlockModule()
{
}

OdRecomputeDimBlockModule::~OdRecomputeDimBlockModule()
{
}

void OdRecomputeDimBlockModule::initApp()
{
  if ( !OdDbDimension::desc() )
  {
    throw OdError(eNotInitializedYet);
  }
  OdDbDimension::desc()->module()->addRef();

  OdDbAlignedDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_alignedDimRecomputor);
  OdDbRotatedDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_rotatedDimRecomputor);
  OdDb2LineAngularDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_2LineAngularDimRecomputor);
  OdDb3PointAngularDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_3PointAngularDimRecomputor);
  OdDbRadialDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_radialDimRecomputor);
  OdDbDiametricDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_diametricDimRecomputor);
  OdDbOrdinateDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_ordinateDimRecomputor);
  OdDbArcDimension::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_arcDimRecomputor);
  OdDbRadialDimensionLarge::desc()->addX(OdDbDimensionRecomputePE::desc(), &m_radialDimLargeRecomputor);
  OdDbMLeader::desc()->addX(OdDbMLeaderRecomputePE::desc(), &m_mleaderRecomputor);
}

void OdRecomputeDimBlockModule::uninitApp()
{
  OdDbMLeader::desc()->delX(OdDbMLeaderRecomputePE::desc());
  OdDbAlignedDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDbRotatedDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDb2LineAngularDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDb3PointAngularDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDbRadialDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDbDiametricDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDbOrdinateDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDbArcDimension::desc()->delX(OdDbDimensionRecomputePE::desc());
  OdDbRadialDimensionLarge::desc()->delX(OdDbDimensionRecomputePE::desc());

  OdDbDimension::desc()->module()->release();
}
