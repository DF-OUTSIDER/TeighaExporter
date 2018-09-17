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

// WinOpenGL.cpp : Defines the entry point for the DLL application.
//

#include "OdaCommon.h"
#include "Gs/Gs.h"
#include "RxDynamicModule.h"
#include "ExGsScreenDevice.h"
#include "ExGsOpenGLVectorizeDevice.h"
#include "ExGsBitmapDevice.h"

class ExGsOpenGLBitmapVectorizeDevice : public ExGsBitmapDevice<ExGsOpenGLVectorizeDevice>
{
ODRX_DECLARE_PROPERTY(RasterImage)
ODRX_DECLARE_PROPERTY(BitPerPixel)
public:
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsOpenGLBitmapVectorizeDevice, OdGsDevice>::createObject();
  }
  void generatePropMap(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES(ExGsOpenGLVectorizeDevice);
    ODRX_GENERATE_PROPERTY(RasterImage)
    ODRX_GENERATE_PROPERTY(BitPerPixel)
  }
};

ODRX_DEFINE_PROPERTY_OBJECT_PREFIX(ExGsOpenGLBitmapVectorizeDevice::, RasterImage, ExGsOpenGLBitmapVectorizeDevice, getRasterImage().get, putRasterImage, OdGiRasterImage)
ODRX_DEFINE_PROPERTY_PREFIX(ExGsOpenGLBitmapVectorizeDevice::, BitPerPixel, ExGsOpenGLBitmapVectorizeDevice, getInt32)

#ifdef _TOOLKIT_IN_DLL_

BOOL APIENTRY DllMain(HANDLE , DWORD /*ul_reason_for_call*/, LPVOID )
{
  return TRUE;
}

#endif

class WinOpenGLModule : public OdGsBaseModule
{
protected:
  void initApp()
  {
    OdGsBaseModule::initApp();
#ifdef _TOOLKIT_IN_DLL_
    // opengl32 adds a hook to the rendering window, and should not be unloaded,
    // when this module is unloaded, or the application will crash, so we are adding a reference to it
    ::LoadLibrary(OD_T("opengl32.dll"));
#endif

    // Add extra initializations here...
  }
  void uninitApp()
  {
    // Add extra uninitializations here...

    OdGsBaseModule::uninitApp();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  {
    return ExGsScreenDevice<ExGsOpenGLVectorizeDevice>::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdRxObjectImpl<ExGsOpenGLVectorizeView, OdGsViewImpl>::createObject();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    return ExGsOpenGLBitmapVectorizeDevice::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return OdRxObjectImpl<ExGsOpenGLVectorizeView, OdGsViewImpl>::createObject();
  }
};

ODRX_DEFINE_DYNAMIC_MODULE(WinOpenGLModule);
