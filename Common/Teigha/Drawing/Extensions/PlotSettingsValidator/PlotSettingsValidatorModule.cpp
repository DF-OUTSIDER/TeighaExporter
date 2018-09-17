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
#include "PlotSettingsValidatorModule.h"
#include "DynamicLinker.h"
#include "DbPlotSettingsValidator.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdPlotSettingsValidatorModule);
ODRX_NO_CONS_DEFINE_MEMBERS(OdDbPlotSettingsValidatorCustomMediaPE, OdRxObject)

OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> *customMediaList = 0;

OdPlotSettingsValidatorModule::OdPlotSettingsValidatorModule()
{
}

OdPlotSettingsValidatorModule::~OdPlotSettingsValidatorModule()
{
}

void OdPlotSettingsValidatorModule::initApp()
{
  if ( !OdDbPlotSettingsValidator::desc() )
  {
    throw OdError(eNotInitializedYet);
  }
  OdDbPlotSettingsValidator::desc()->module()->addRef();

  OdDbPlotSettingsValidator::desc()->addX(OdDbPlotSettingsValidatorPE::desc(), &m_PSValidator);

  OdDbPlotSettingsValidatorCustomMediaPE::rxInit();
  OdDbPlotSettingsValidatorPE::desc()->addX(OdDbPlotSettingsValidatorCustomMediaPE::desc(), &m_PSCustomMedia);
}

void OdPlotSettingsValidatorModule::uninitApp()
{
  OdDbPlotSettingsValidatorPE::desc()->delX(OdDbPlotSettingsValidatorCustomMediaPE::desc());
  OdDbPlotSettingsValidatorCustomMediaPE::rxUninit();
  
  OdDbPlotSettingsValidator::desc()->delX(OdDbPlotSettingsValidatorPE::desc());

  OdDbPlotSettingsValidator::desc()->module()->release();

  if (customMediaList)
  {
    delete customMediaList;
    customMediaList = 0;
  }
}
