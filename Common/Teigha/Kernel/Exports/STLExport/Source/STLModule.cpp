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
#include "STLModule.h"
#include "STLExport.h"

using namespace TD_STL_EXPORT;

//DD:EXPORT_ON
ODRX_DEFINE_DYNAMIC_MODULE(STLModule);
//DD:EXPORT_OFF
namespace TD_STL_EXPORT
{
void STLModule::initApp()
{
}

void STLModule::uninitApp()
{
}

OdResult STLModule::exportSTL(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, OdStreamBuf &pOutStream, bool bTextMode, double dDeviation)
{
  return TD_STL_EXPORT::exportSTL(pDb, pEntity, pOutStream, bTextMode, dDeviation);
}

OdResult STLModule::exportSTLEx(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, OdStreamBuf &pOutStream, bool bTextMode, double dDeviation)
{
  return TD_STL_EXPORT::exportSTLEx(pDb, pEntity, pOutStream, bTextMode, dDeviation);
}
}
