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
// GLES2Module.cpp : Defines the initialization routines for the DLL.

#include "OdaCommon.h"
#include "RxDispatchImpl.h"
#include "RxDynamicModule.h"
#include "Gs/GsBaseVectorizeDevice.h" // for OdSmartPtr<OdGsBaseVectorizeDevice>
#include "Gs/GsViewImpl.h"            // for OdSmartPtr<OdGsViewImpl>
#include "TrVecBaseModule.h"
#include "ExGsGLES2XmlRendition.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}

#endif //_TOOLKIT_IN_DLL_

class OdTrXmlDevice : public OdTrVectorizerModuleHost
{
public:
  ODRX_DECLARE_PROPERTY_TRV(XmlServer)
  ODRX_DECLARE_PROPERTY_TRV(ClientSettings)

  OdTrXmlDevice() {}

  OdTrVisRenditionPtr createRendition() { return OdRxObjectImpl<OdGLES2XmlRenditionRedir, OdTrVisRendition>::createObject(); }
  OdGLES2XmlRendition &rendition() const
  { OdTrVisRendition *pRendition = reinterpret_cast<OdTrVisRendition*>(renderClient()->getProperty(OD_T("Rendition"))->getIntPtr());
    return static_cast<OdGLES2XmlRenditionRedir*>(pRendition)->xmlRendition(); }

  DeviceSetupType deviceSetupOverride() const { return kDtOffScreen; }
  void configureForPlatform(OdTrVisRenderClient* /*pRenderClient*/, DeviceSetupType /*devType*/) {}

  void put_XmlServer(OdIntPtr pSet) { rendition().setXmlServer(pSet); }
  OdIntPtr get_XmlServer() const { return rendition().getXmlServer(); }

  void put_ClientSettings(OdIntPtr pSet) { rendition().setClientSettings(pSet); }
  OdIntPtr get_ClientSettings() const { return rendition().getClientSettings(); }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_GENERATE_PROPERTY_TRV(XmlServer)
    ODRX_GENERATE_PROPERTY_TRV(ClientSettings)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdTrXmlDevice::, XmlServer     , OdTrXmlDevice, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdTrXmlDevice::, ClientSettings, OdTrXmlDevice, getIntPtr)

class TrXmlModule : public BaseGLES2Module
{
protected:
  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  { return createVectorizeDevice(OdRxObjectImpl<OdTrXmlDevice, OdTrVectorizerModuleHost>::createObject(), OdTrVectorizerModuleHost::kDtOnScreen); }
  OdSmartPtr<OdGsViewImpl> createViewObject() { return createVectorizeView(); }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  { // Not supported yet for this kind of device.
    return OdSmartPtr<OdGsBaseVectorizeDevice>(); }
  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  { // Not supported yet for this kind of device.
    return OdSmartPtr<OdGsViewImpl>(); }
};

ODRX_DEFINE_DYNAMIC_MODULE(TrXmlModule);

//
