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

// WinCompositeModule.cpp : Defines the entry point for the DLL application.
//

#include "OdaCommon.h"
#include "Gs/Gs.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"

#include "ExWinGsCompositeView.h"
#include "ExWinGsCompositeDevice.h"

#include "ExGsScreenDevice.h"
#include "ExGsBitmapDevice.h"

/*
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  return TRUE;
}
*/

// to not use properties in templates -- this fails to compile on Borland, GCC
class ExWinGsBitmapCompositeDevice : public ExGsBitmapDevice<ExWinGsCompositeDevice>
{
ODRX_DECLARE_PROPERTY(RasterImage)
ODRX_DECLARE_PROPERTY(BitPerPixel)
public:
  ExWinGsBitmapCompositeDevice() : ExGsBitmapDevice<ExWinGsCompositeDevice>()
  {
    setBitmapDevice(true);
  }
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExWinGsBitmapCompositeDevice, OdGsDevice>::createObject();
  }
  void generatePropMap(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES(ExWinGsCompositeDevice);
    ODRX_GENERATE_PROPERTY(RasterImage)
    ODRX_GENERATE_PROPERTY(BitPerPixel)
  }
  void onSize(const OdGsDCRect& outputRect)
  {
    //if(outputRect.m_max.y > outputRect.m_min.y)
    //{
    //  OdGsDCRect rc(OdGsDCPoint(outputRect.m_min.x, outputRect.m_max.y), OdGsDCPoint(outputRect.m_max.x, outputRect.m_min.y));
    //  ExGsBitmapDevice<ExWinGsCompositeDevice>::onSize(rc);
    //}
    //else
    {
      ExGsBitmapDevice<ExWinGsCompositeDevice>::onSize(outputRect);
    }
  }
};

ODRX_DEFINE_PROPERTY_OBJECT_PREFIX(ExWinGsBitmapCompositeDevice::, RasterImage, ExWinGsBitmapCompositeDevice, getRasterImage().get, putRasterImage, OdGiRasterImage)
ODRX_DEFINE_PROPERTY_PREFIX(ExWinGsBitmapCompositeDevice::, BitPerPixel, ExWinGsBitmapCompositeDevice, getInt32)

class WinCompositeModule : public OdGsBaseModule
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
    return ExGsScreenDevice<ExWinGsCompositeDevice>::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdRxObjectImpl<ExWinGsCompositeView, OdGsViewImpl>::createObject();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    return ExWinGsBitmapCompositeDevice::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return OdRxObjectImpl<ExWinGsCompositeView, OdGsViewImpl>::createObject();
  }
};

ODRX_DEFINE_DYNAMIC_MODULE(WinCompositeModule);

// @@@ test
#include "ExGsCompositeModuleFactory.h"

void ExGsCompositeModuleFactory::fillByDefault()
{
  OdGsModulePtr pGsWinGDI = ::odrxDynamicLinker()->loadModule(OdWinGDIModuleName, false);
  OdGsModulePtr pGsWinOGL = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName, false);
#ifdef _DEBUG
  OdGsModulePtr pGsWinDRX = ::odrxDynamicLinker()->loadModule(OdWinDirectXModuleName, false);
#endif
  ODA_ASSERT(!pGsWinGDI.isNull());
  ODA_ASSERT(!pGsWinOGL.isNull());
#ifdef _DEBUG
  ODA_ASSERT(!pGsWinDRX.isNull());
#endif
  setModuleForAllStates(pGsWinOGL);
  setModuleForState(OdGsView::k2DOptimized, pGsWinGDI, ExGsCompositeModuleProperty::kDisableCompClearScreen | ExGsCompositeModuleProperty::kDisableCompDoubleBuffer);
#ifdef _DEBUG
  setModuleForState(OdGsView::kHiddenLine, pGsWinDRX);
#endif
  // for test materials compatibility
  //setModuleForState(OdGsView::kFlatShadedWithWireframe, pGsWinDRX);
  //setModuleForState(OdGsView::kGouraudShadedWithWireframe, pGsWinDRX);
}

//
