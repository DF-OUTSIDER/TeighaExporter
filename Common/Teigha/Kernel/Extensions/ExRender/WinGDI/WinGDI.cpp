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

// WinGDI.cpp : Defines the entry point for the DLL application.
//

#include "OdaCommon.h"
#include "Gs/Gs.h"
#include "RxDynamicModule.h"
#include "ExGsScreenDevice.h"
#include "ExGsGDIVectorizeDevice.h"
#include "ExGsBitmapDevice.h"
#include "DynamicLinker.h"

/*
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  return TRUE;
}
*/

// to not use properties in templates -- this fails to compile on Borland
class ExGsGDIBitmapVectorizeDevice : public ExGsBitmapDevice<ExGsGDIVectorizeDevice>
{
ODRX_DECLARE_PROPERTY(RasterImage)
ODRX_DECLARE_PROPERTY(BitPerPixel)
ODRX_DECLARE_PROPERTY(EnableSoftwareHLR)
ODRX_DECLARE_PROPERTY(UseTextOut)
ODRX_DECLARE_PROPERTY(UseTTFCache)
public:
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsGDIBitmapVectorizeDevice, OdGsDevice>::createObject();
  }
  void generatePropMap(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES(ExGsWin32Device);
    ODRX_GENERATE_PROPERTY(RasterImage)
    ODRX_GENERATE_PROPERTY(BitPerPixel)
    ODRX_GENERATE_PROPERTY(EnableSoftwareHLR)
    ODRX_GENERATE_PROPERTY(UseTextOut)
    ODRX_GENERATE_PROPERTY(UseTTFCache)
  }
  void onSize(const OdGsDCRect& outputRect)
  {
    //if(outputRect.m_max.y > outputRect.m_min.y)
    //{
    //  OdGsDCRect rc(OdGsDCPoint(outputRect.m_min.x, outputRect.m_max.y), OdGsDCPoint(outputRect.m_max.x, outputRect.m_min.y));
    //  ExGsBitmapDevice<ExGsGDIVectorizeDevice>::onSize(rc);
    //}
    //else
    {
      ExGsBitmapDevice<ExGsGDIVectorizeDevice>::onSize(outputRect);
    }
  }
};

ODRX_DEFINE_PROPERTY_OBJECT_PREFIX(ExGsGDIBitmapVectorizeDevice::, RasterImage, ExGsGDIBitmapVectorizeDevice, getRasterImage().get, putRasterImage, OdGiRasterImage)
ODRX_DEFINE_PROPERTY_PREFIX(ExGsGDIBitmapVectorizeDevice::, BitPerPixel, ExGsGDIBitmapVectorizeDevice, getInt32)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsGDIBitmapVectorizeDevice::, EnableSoftwareHLR, ExGsGDIBitmapVectorizeDevice, useSoftwareHLR, setUseSoftwareHLR, getBool)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsGDIBitmapVectorizeDevice::, UseTextOut, ExGsGDIBitmapVectorizeDevice, useTextOut, setUseTextOut, getBool)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsGDIBitmapVectorizeDevice::, UseTTFCache, ExGsGDIBitmapVectorizeDevice, useTtfCache, setUseTtfCache, getBool)

class WinGDIModule : public OdGsBaseModule
{
protected:
  void initApp()
  {
    OdGsBaseModule::initApp();

    // Add extra initializations here...
  }
  void uninitApp()
  {
    // Add extra uninitializations here...

    OdGsBaseModule::uninitApp();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  {
    return ExGsScreenDevice<ExGsGDIVectorizeDevice>::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdRxObjectImpl<ExGDIVectorizeView, OdGsViewImpl>::createObject();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    return ExGsGDIBitmapVectorizeDevice::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return OdRxObjectImpl<ExGDIVectorizeView, OdGsViewImpl>::createObject();
  }
};

ODRX_DEFINE_DYNAMIC_MODULE(WinGDIModule);
