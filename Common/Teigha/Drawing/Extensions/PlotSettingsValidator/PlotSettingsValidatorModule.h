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


#ifndef __OD_PS_VALIDATOR_MODULE__
#define __OD_PS_VALIDATOR_MODULE__

#include "RxModule.h"
#include "StaticRxObject.h"

#include "PlotSettingsValidatorPEImpl.h"

#include "TD_PackPush.h"

/** \details
  library: Source code provided. 
  <group Win_Classes> 
*/
class OdPlotSettingsValidatorModule : public OdRxModule
{
  ODRX_USING_HEAP_OPERATORS(OdRxModule);

  OdStaticRxObject<OdPlotSettingsValidatorPEImpl> m_PSValidator;
  OdStaticRxObject<OdPlotSettingsValidatorCustomMediaPEImpl> m_PSCustomMedia;

protected:
  OdPlotSettingsValidatorModule();
  void initApp();
  void uninitApp();

public:
  virtual ~OdPlotSettingsValidatorModule();
};

#include "TD_PackPop.h"

#endif // __OD_PS_VALIDATOR_MODULE__
