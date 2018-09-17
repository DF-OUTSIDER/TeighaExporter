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
#include "ThreejsJSONExport.h"
#include "RxObjectImpl.h"
#include "RxDynamicModule.h"

#include "Gs/GsBaseVectorizeDevice.h" // for OdSmartPtr<OdGsBaseVectorizeDevice>

#include "Tr/vec/TrVecBaseModule.h"
#include "RxDispatchImpl.h"
#include "GlesJsonServerBaseImpl.h"
#include "GlesJsonServerImpl.h"
#include "ExGsGLES2JsonRendition.h"

namespace TD_THREEJSJSON_EXPORT
{
  class OdTrJsonDevice : public OdTrVectorizerModuleHost
  {
  public:
    ODRX_DECLARE_PROPERTY_TRV(JsonServer)
    ODRX_DECLARE_PROPERTY_TRV(ClientSettings)

    OdTrJsonDevice() {}

    OdTrVisRenditionPtr createRendition() { return OdRxObjectImpl<OdGLES2JsonRenditionRedir, OdTrVisRendition>::createObject(); }
    OdGLES2JsonRendition &rendition() const
    {
      OdTrVisRendition *pRendition = reinterpret_cast<OdTrVisRendition*>(renderClient()->getProperty(OD_T("Rendition"))->getIntPtr());
      return static_cast<OdGLES2JsonRenditionRedir*>(pRendition)->jsonRendition();
    }

    DeviceSetupType deviceSetupOverride() const { return kDtOffScreen; }
    void configureForPlatform(OdTrVisRenderClient* /*pRenderClient*/, DeviceSetupType /*devType*/) {}

    void put_JsonServer(OdIntPtr pSet) { rendition().setJsonServer(pSet); }
    OdIntPtr get_JsonServer() const { return rendition().getJsonServer(); }

    void put_ClientSettings(OdIntPtr pSet) { rendition().setClientSettings(pSet); }
    OdIntPtr get_ClientSettings() const { return rendition().getClientSettings(); }

    void generateProperties(OdRxDictionary* pInfo) const
    {
      ODRX_GENERATE_PROPERTY_TRV(JsonServer)
      ODRX_GENERATE_PROPERTY_TRV(ClientSettings)
    }
  };

  ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdTrJsonDevice::, JsonServer, OdTrJsonDevice, getIntPtr)
  ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdTrJsonDevice::, ClientSettings, OdTrJsonDevice, getIntPtr)

  class TrJsonModule : public BaseGLES2Module
  {
  protected:
    OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
    {
      return createVectorizeDevice(OdRxObjectImpl<OdTrJsonDevice, OdTrVectorizerModuleHost>::createObject(), OdTrVectorizerModuleHost::kDtOnScreen);
    }
    OdSmartPtr<OdGsViewImpl> createViewObject()
    {
      return createVectorizeView();
    }

    OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
    { // Not supported yet for this kind of device.
      return OdSmartPtr<OdGsBaseVectorizeDevice>();
    }
    OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
    { // Not supported yet for this kind of device.
      return OdSmartPtr<OdGsViewImpl>();
    }

    void addRef() {}
    void release() {}
  };

  ODRX_DEFINE_PSEUDO_STATIC_MODULE(TrJsonModule);

  struct TryToVectorizeMod
  {
    TryToVectorizeMod() { }
    virtual ~TryToVectorizeMod() { }

    virtual OdGsDevicePtr initDevice(OdDbBaseDatabase *pDb, OdGsDevice* pDevice, const ODCOLORREF &background) const;
    virtual void vectorizationTest(OdGsDevice* pDevice) const;
  };

  void tryToVectorize(OdStreamBuf *pOutStream, OdDbBaseDatabase *pDb, const ODCOLORREF &background, bool bFacesEnabled, const TryToVectorizeMod &pMod = TryToVectorizeMod());


  OdGsDevicePtr TryToVectorizeMod::initDevice(OdDbBaseDatabase *pDb, OdGsDevice* pDevice, const ODCOLORREF &background) const
  {
    OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pDb);
    ODA_ASSERT_ONCE(pDbPE.get());

    OdGiDefaultContextPtr pContext = pDbPE->createGiContext(pDb);
    pContext->enableGsModel(true);

    OdGsDevicePtr pNewDev = pDbPE->setupActiveLayoutViews(pDevice, pContext);
    pDbPE->setupPalette(pNewDev, pContext, NULL, background);

    return pNewDev;
  }

  void TryToVectorizeMod::vectorizationTest(OdGsDevice* pDevice) const
  {
    // Default implementation only make one step of vectorization
    pDevice->update();
  }

  void tryToVectorize(OdStreamBuf *pOutStream, OdDbBaseDatabase *pDb, const ODCOLORREF &background, bool bFacesEnabled, const TryToVectorizeMod &pMod)
  {
    odgsInitialize();
    OdGsModulePtr pGsModule = ODRX_STATIC_MODULE_ENTRY_POINT(TrJsonModule)(OD_T("TrJsonModule"));

    OdGsDevicePtr pDevice = pGsModule->createDevice();

    pDevice = pMod.initDevice(pDb, pDevice, background);

    OdGsDCRect screenRect(OdGsDCPoint(0, 2000), OdGsDCPoint(2000, 0));
    pDevice->onSize(screenRect);

    OdSharedPtr<OdGlesJsonServerBaseImpl> pJsonServer = new OdGlesJsonServerImpl(pDb);
    pJsonServer->setOutput(pOutStream);
    pJsonServer->setSkipShaders(true);
    pJsonServer->setEnableFaces(bFacesEnabled);

    OdRxDictionaryPtr pProperties = pDevice->properties();

    if (bFacesEnabled)
    {
      OdTrVisRendition::ClientSettings cliSets; // <<< obtained from client
      pProperties->putAt(L"ClientSettings", OdRxVariantValue((OdIntPtr)&cliSets));

      if (pProperties->has(L"UseCompositeMetafiles"))
        pProperties->putAt(L"UseCompositeMetafiles", OdRxVariantValue(true));
      if (pProperties->has(L"DiscardBackFaces"))
        pProperties->putAt(L"DiscardBackFaces", OdRxVariantValue(true));
      if (pProperties->has(L"UseTTFCache"))
        pProperties->putAt(L"UseTTFCache", OdRxVariantValue(true));
      if (pProperties->has(L"DynamicSubEntHlt"))
        pProperties->putAt(L"DynamicSubEntHlt", OdRxVariantValue(true));
    }

    pProperties->putAt(OD_T("JsonServer"), OdRxVariantValue((OdIntPtr)pJsonServer.get()));

    pMod.vectorizationTest(pDevice);
    pJsonServer->flushOut();
    pDevice.release();
    pGsModule.release();
    odgsUninitialize();
  }

  OdResult doExport(OdDbBaseDatabase *pDb, OdStreamBuf *pOutStream, const ODCOLORREF &background, bool bFacesEnabled)
  {
    OdResult ret = eOk;

    try
    {
      tryToVectorize(pOutStream, pDb, background, bFacesEnabled);
    }
    catch (const OdError& e)
    {
      ret = e.code();
    }
    catch (...)
    {
      ret = eExtendedError;
    }

    return ret;
  }

  OdResult exportThreejsJSON(OdDbBaseDatabase *pDb, OdStreamBuf *pOutStream, const ODCOLORREF &background, bool bFacesEnabled)
  {
    return doExport(pDb, pOutStream, background, bFacesEnabled);
  }
};
