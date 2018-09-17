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
// Composite device

#ifndef EXGSCOMPOSITEDEVICE
#define EXGSCOMPOSITEDEVICE

#include "TD_PackPush.h"

#include "ExGsCompositeDefs.h"
#include "ExGsCompositeModuleFactory.h"
#include "Gs/GsBaseInclude.h"

class ExGsCompositeView;

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExGsCompositeDevice
  : public OdGsBaseVectorizeDevice
  , protected ExGsCompositeSyncFlags
{
  protected:
    friend class ExGsCompositeView;
    enum SyncFlags
    {
      kSyncInvalidate = (1 << 0),  // invalidate called
      kSyncGiCtx      = (1 << 1),  // gi context changed
      kSyncResize     = (1 << 2),  // onSize called
      kSyncViews      = (1 << 3),  // views array changed
      kSyncBackground = (1 << 4),  // background color changed
      kSyncPalette    = (1 << 5)   // palette changed
    };
    enum CompositionFlags
    {
      kCompContextInitialized = (1 << 0), // Composition device context initialized
      kCompFactoryPrepared    = (1 << 1), // Modules factory prepared
      kCompPartialUpdate      = (1 << 2), // Support partial update
      kCompBitmapDevice       = (1 << 3), // Generate bitmap devices
      kCompInvalidRects       = (1 << 4), // Invalid rects available during update
      kCompRedirection        = (1 << 5)  // Redirection mode enabled
    };
    OdUInt32 m_compositeFlags;
    OD_GS_COMPOSITE_PROP(kCompContextInitialized, isContextInitialized, setContextInitialized);
    OD_GS_COMPOSITE_PROP(kCompFactoryPrepared   , isFactoryPrepared   , setFactoryPrepared   );
    OD_GS_COMPOSITE_PROP(kCompPartialUpdate     , isPartialUpdate     , setPartialUpdate     );
    OD_GS_COMPOSITE_PROP(kCompBitmapDevice      , isBitmapDevice      , setBitmapDevice      );
    OD_GS_COMPOSITE_PROP(kCompInvalidRects      , isInvalidRects      , setInvalidRects      );
    OD_GS_COMPOSITE_PROP(kCompRedirection       , isRedirection       , setRedirection       );
    ExGsCompositeModuleFactory m_moduleFac;
  public:
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);

    ExGsCompositeDevice();
    ~ExGsCompositeDevice();

    // OdGsDevice overrides

    virtual OdGsModelPtr createModel();

    virtual bool supportPartialUpdate() const;

    virtual void update(OdGsDCRect* pUpdatedRect = 0);

    virtual void updateGeometry();
    virtual void updateScreen();

    double getRegenCoef() const;
    void setRegenCoef(double) { } // formal

    void setModuleFactoryProp(OdIntPtr pEntry);
    OdIntPtr getModuleFactoryProp() const;

    // Each change in device or view requires synchronization of composition devices, so we need keep all modifications
    // information to make sychronization on next composition step (do this job inside overrides).

    virtual void setUserGiContext(OdGiContext* pUserGiContext);

    virtual void invalidate();
    virtual void invalidate(const OdGsDCRect &screenRect);

    virtual void onSize(const OdGsDCRect& outputRect);

    virtual void addView(OdGsView* pView);
    virtual void insertView(int viewIndex, OdGsView* pView);
    virtual bool eraseView(OdGsView* pView);
    virtual bool eraseView(int viewIndex);
    virtual void eraseAllViews();

    virtual bool setBackgroundColor(ODCOLORREF backgroundColor);
    virtual void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors);
  protected:
    // Initialize only our context which is attached to window
    virtual void createContext();
    virtual void deleteContext();

    // Process synchronization with all dependent devices/views/modules
    virtual void sync();

    // Make preprocessing (something like beginViewVectorization but for entire surface)
    virtual void preprocess();
    // Make postprocessing (something like endViewVectorization but for entire surface)
    virtual void postprocess();
    // Make surface presentation
    virtual void present();

    bool prepareModuleFactory();
    void syncView(int id);
    void runRedraw(OdGsDCRect* pUpdatedRect);

    // Manage composite view context
    virtual void compositeViewAttached(ExGsCompositeView &pView);
    virtual void compositeViewDetached(ExGsCompositeView &pView);
    // Manage redirection view context
    virtual void redirectionDeviceAttached(ExGsCompositeDevice &pDevice);
    virtual void redirectionDeviceDetached(ExGsCompositeDevice &pDevice);
    // Make presentation of updated viewport onto surface
    virtual void presentViewportSurface(ExGsCompositeView &pView, const OdGsDCRect &pUpdatedRect);
    // Make presentation of entire redirected surface
    virtual void presentRedirectedSurface(ExGsCompositeDevice &pDevice, const OdGsDCRect *pUpdatedRect = NULL);

    void detachAllViews();

    static bool isRectIntersectsView(const OdGsDCRect &rect, const OdGsBaseVectorizeView &view);

    bool checkIsViewModulesInSync(bool &bPointsToSimilarModule);
    void prepareFlagsForSync();
    void mergeDeviceFlags(const ExGsCompositeView &pView);

    void resetRedirectionMode(bool bRedirectionMode);

    void validateRedirectedViews();

    void syncBackDrawablesInfo();
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExGsProtectedDeviceParamsAccessor
  : public OdGsBaseVectorizeDevice
{
  friend class ExGsCompositeDevice;
  friend class ExGsCompositeView;
  public:
    static ExGsProtectedDeviceParamsAccessor *castEx(OdGsBaseVectorizeDevice *pDevice)
    {
      return reinterpret_cast<ExGsProtectedDeviceParamsAccessor*>(pDevice);
    }
    static const ExGsProtectedDeviceParamsAccessor *castEx(const OdGsBaseVectorizeDevice *pDevice)
    {
      return reinterpret_cast<const ExGsProtectedDeviceParamsAccessor*>(pDevice);
    }
};

#include "TD_PackPop.h"

#endif // EXGSCOMPOSITEDEVICE
