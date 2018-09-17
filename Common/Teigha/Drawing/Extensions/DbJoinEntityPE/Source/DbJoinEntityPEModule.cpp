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
#include "DbJoinEntityPEModule.h"
#include "DbJoinEntityPECurves.h"
#include "StaticRxObject.h"
#include "DbLine.h"
#include "DbArc.h"
#include "DbEllipse.h"
#include "Db2dPolyline.h"
#include "Db3dPolyline.h"
#include "DbPolyline.h"
#include "DbSpline.h"

using namespace joinEntityPE;
static OdStaticRxObject<OdDbLineJoinEntityPE> lineJoinEntityPE;
static OdStaticRxObject<OdDbPolylineJoinEntityPE> polylineJoinEntityPE;
static OdStaticRxObject<OdDb2dPolylineJoinEntityPE> polyline2dJoinEntityPE;
static OdStaticRxObject<OdDbSplineJoinEntityPE> splineJoinEntityPE;
static OdStaticRxObject<OdDb3dPolylineJoinEntityPE> polyline3dJoinEntityPE;
static OdStaticRxObject<OdDbArcJoinEntityPE> arcJoinEntityPE;
static OdStaticRxObject<OdDbEllipseJoinEntityPE> ellipseJoinEntityPE;

//DD:EXPORT_ON
ODRX_DEFINE_DYNAMIC_MODULE(DbJoinEntityPEModule);
//DD:EXPORT_OFF

void DbJoinEntityPEModule::initApp()
{
  OdDbLine::desc()->module()->addRef(); // lock TD_DbEntities.tx

  OdDbLine::desc()->addX(OdDbJoinEntityPE::desc(), &lineJoinEntityPE);
  OdDbPolyline::desc()->addX(OdDbJoinEntityPE::desc(), &polylineJoinEntityPE);
  OdDb2dPolyline::desc()->addX(OdDbJoinEntityPE::desc(), &polyline2dJoinEntityPE);
  OdDbSpline::desc()->addX(OdDbJoinEntityPE::desc(), &splineJoinEntityPE);
  OdDb3dPolyline::desc()->addX(OdDbJoinEntityPE::desc(), &polyline3dJoinEntityPE);
  OdDbArc::desc()->addX(OdDbJoinEntityPE::desc(), &arcJoinEntityPE);
  OdDbEllipse::desc()->addX(OdDbJoinEntityPE::desc(), &ellipseJoinEntityPE);
}

void DbJoinEntityPEModule::uninitApp()
{
  OdDbLine::desc()->delX(OdDbJoinEntityPE::desc());
  OdDbPolyline::desc()->delX(OdDbJoinEntityPE::desc());
  OdDb2dPolyline::desc()->delX(OdDbJoinEntityPE::desc());
  OdDbSpline::desc()->delX(OdDbJoinEntityPE::desc());
  OdDb3dPolyline::desc()->delX(OdDbJoinEntityPE::desc());
  OdDbArc::desc()->delX(OdDbJoinEntityPE::desc());
  OdDbEllipse::desc()->delX(OdDbJoinEntityPE::desc());

  OdDbLine::desc()->module()->release(); // unlock TD_DbEntities.tx
}
