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

#include "OdaCommon.h"
#include "ExGsCompositeView.h"
#include "RxDictionary.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "ExGsCompositeModuleInterface.h"
#include "ExGsCompositeDevice.h"

#if (_MSC_VER >= 1400)
#define ODGSDEV_SUPER __super
#else
#define ODGSDEV_SUPER OdGsBaseVectorizeDevice
#endif

ExGsCompositeDevice::ExGsCompositeDevice()
  : OdGsBaseVectorizeDevice()
  , ExGsCompositeSyncFlags()
  , m_compositeFlags(kCompPartialUpdate)
{
  // Enable all mergeable options here, if one of devices doesn't support anything, we'll disable it for all
  SETBIT(m_flags, kSortRenderTypes    , true);
  SETBIT(m_flags, kDynamicHighlight   , true);
  //SETBIT(m_flags, kDynamicSubhighlight, true);
  SETBIT(m_flags, kSupportOverlays    , true);
}

ExGsCompositeDevice::~ExGsCompositeDevice()
{
  detachAllViews();
  if (isContextInitialized())
    deleteContext();
}

OdGsModelPtr ExGsCompositeDevice::createModel()
{
  // Create default GsModel
  return ODGSDEV_SUPER::createModel();
}

bool ExGsCompositeDevice::supportPartialUpdate() const
{
  return isPartialUpdate();
}

void ExGsCompositeDevice::update(OdGsDCRect* pUpdatedRect)
{
  if (!isContextInitialized())
    createContext();
  sync();
  if (!supportPartialUpdate() || isRedirection())
    preprocess();
  if (!isRedirection())
    runRedraw(pUpdatedRect);
  else
  {
    if (numViews() > 0)
    {
      ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(0));
      OdGsDCRect updatedRect;
      pView->compositeDevice()->update(&updatedRect);
      validateRedirectedViews();
      if (!GETBIT(pView->moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableRedirDoubleBuffer))
        presentRedirectedSurface(*this, &updatedRect);
      if (pUpdatedRect)
        *pUpdatedRect = updatedRect;
    }
  }
  syncBackDrawablesInfo();
  postprocess();
  present();
  setValid(true);
  resetSyncFlags(); // Reset accumulated invalidates
}

template <class T, class TSize>
static bool checkArraysDiff(const T& a1, const T& a2)
{
  if (a1.getPtr() == a2.getPtr())
    return true;
  if (a1.size() != a2.size())
    return false;
  //if (a1.empty() && a2.empty())
  //  return true;
  return ::memcmp(a1.getPtr(), a2.getPtr(), sizeof(TSize) * a1.size()) == 0;
}

void ExGsCompositeDevice::updateGeometry()
{
  ODGSDEV_SUPER::updateGeometry();
  int nViews = numViews(), nView;
  bool bInvalidRects = invalidRects().empty();

  for (nView = 0; nView < nViews; nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    const OdGsDCRectArray &invRects = ExGsProtectedDeviceParamsAccessor::castEx(pView->compositeDevice())->invalidRects();

    if (pView->compositeDevice()->supportComposition())
      dynamic_cast<ExGsCompositeClientDeviceIface*>(pView->compositeDevice())->enableCompositionUpdate(true);
    pView->compositeDevice()->update(NULL);
    if (pView->compositeDevice()->supportComposition())
      dynamic_cast<ExGsCompositeClientDeviceIface*>(pView->compositeDevice())->enableCompositionUpdate(false);

    if (pView->compositeView()->isVisible())
    {
      if (!invRects.empty())
      {
        if (!checkArraysDiff<OdGsDCRectArray, OdGsDCRect>(invRects, invalidRects()))
        {
          for (OdUInt32 nRect = 0; nRect < invRects.size(); nRect++)
          {
            invalidate(invRects.at(nRect));
          }
        }
      }
      else if (!pView->compositeDevice()->isValid())
      {
        invalidate();
      }
    }
  }

  // Propagate all invalidation data to all devices
  for (nView = 0; nView < nViews; nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    if (pView->isVisible())
    {
      if (!invalidRects().empty())
      {
        if (!checkArraysDiff<OdGsDCRectArray, OdGsDCRect>(ExGsProtectedDeviceParamsAccessor::castEx(pView->compositeDevice())->invalidRects(), invalidRects()))
        {
          for (OdUInt32 nRect = 0; nRect < invalidRects().size(); nRect++)
          {
            if (isRectIntersectsView(invalidRects().at(nRect), *pView))
              pView->compositeDevice()->invalidate(invalidRects().at(nRect));
          }
        }
      }
      else if (invalidRects().empty() != bInvalidRects)
      {
        pView->compositeDevice()->invalidate();
      }
    }
    pView->resetSyncFlags(); // Reset accumulated invalidates
  }
}

void ExGsCompositeDevice::validateRedirectedViews()
{
  for (int nView = 0; nView < numViews(); nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    pView->setInvalid(false);
    pView->setCheckValid(false);
  }
}

void ExGsCompositeDevice::syncBackDrawablesInfo()
{
  for (int nView = 0; nView < numViews(); nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    const DrawableHolderArray &invDraws = ExGsProtectedViewParamsAccessor::castEx(pView->compositeView())->m_drawables;
    if (!checkArraysDiff<DrawableHolderArray, DrawableHolder>(pView->m_drawables, invDraws))
      pView->m_drawables = invDraws;
    ExGsProtectedViewParamsAccessor::castEx(pView)->nCachedDrawables() = ExGsProtectedViewParamsAccessor::castEx(pView->compositeView())->nCachedDrawables();
  }
}

void ExGsCompositeDevice::updateScreen()
{
  if (supportPartialUpdate())
    preprocess();
  ODGSDEV_SUPER::updateScreen();
}

void ExGsCompositeDevice::createContext()
{
  setContextInitialized(true);
}

void ExGsCompositeDevice::deleteContext()
{
  setContextInitialized(false);
}

bool ExGsCompositeDevice::prepareModuleFactory()
{
  if (!isFactoryPrepared())
  {
    if (!m_moduleFac.isValid())
    {
      if (!m_moduleFac.validate())
      {
        m_moduleFac.fillByDefault();
        if (!m_moduleFac.isValid())
        {
          ODA_FAIL();
          return false;
        }
      }
    }
    setFactoryPrepared(true);
  }
  return isFactoryPrepared();
}

bool ExGsCompositeDevice::isRectIntersectsView(const OdGsDCRect &rect, const OdGsBaseVectorizeView &view)
{
  OdGsDCRect scrRect;
  ExGsProtectedViewParamsAccessor::castEx(&view)->screenRectNorm(scrRect);
  OdGsDCRect invrc = rect;
  invrc &= scrRect;
  if (invrc.is_null())
    return false;
  return true;
}

void ExGsCompositeDevice::syncView(int id)
{
  ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(id));
  bool bViewAttached = false;
  // First of all we need create or recreate real device
  if (pView->isSyncFlagSet(ExGsCompositeView::kSyncRenderMode) &&
      pView->isCompositeViewAttached() &&
      m_moduleFac.moduleForState(pView->mode()) != pView->compositeModule())
  {
    // We require recreate module
    // Detach device
    pView->detachCompositeView();
    // Mark all synchronization flags to update for new device
    pView->setSyncAll();
  }
  if (!pView->isCompositeViewAttached())
  {
    // Attach device if it is not created
    if (!isRedirection())
    {
      pView->attachCompositeView();
      bViewAttached = true;
    }
    else
    {
      for (int i = 0; i < numViews(); i++)
      {
        ExGsCompositeView *pViewAt = static_cast<ExGsCompositeView*>(viewAt(i));
        if (pViewAt->isCompositeViewAttached())
        {
          pView->attachRedirectionView(pViewAt->compositeDevice(), pViewAt->moduleFactoryFlags());
          // Mark all synchronization flags to update for new view
          pView->setSyncAll();
          break;
        }
      }
    }
    // Check device flags
    mergeDeviceFlags(*pView);
  }
  // Synchronize device changes
  if (bViewAttached || (!isSynchronized() && (!isRedirection() || id == 0)))
  {
    if (isSyncFlagSet(kSyncGiCtx) || bViewAttached)
    {
      pView->compositeDevice()->setUserGiContext(userGiContext());
    }
    // kSyncViews - nothing to do for this here
    if (isSyncFlagSet(kSyncBackground) || bViewAttached)
    {
      pView->compositeDevice()->setBackgroundColor(getBackgroundColor());
      if (!isRedirection() && pView->isDependentViewportView()) // Do this like Acad does
        pView->compositeDevice()->setBackgroundColor(userGiContext()->paletteBackground());
    }
    if (isSyncFlagSet(kSyncPalette) || bViewAttached)
    {
      pView->compositeDevice()->setLogicalPalette(m_logPalette.getPtr(), (int)m_logPalette.size());
    }
    if (isSyncFlagSet(kSyncResize) || bViewAttached)
    {
      pView->compositeDevice()->onSize(m_outputRect);
    }
#if 0
    if (isSyncFlagSet(kSyncInvalidate))
    {
      if (!m_invalidRects.empty())
      {
        OdUInt32 nRects = m_invalidRects.size();
        for (OdUInt32 n = 0; n < nRects; n++)
        {
          pView->compositeDevice()->invalidate(m_invalidRects.at(n));
        }
      }
      else
      {
        pView->compositeDevice()->invalidate();
      }
    }
#endif
  }
  // Synchronize view changes
  if (!pView->isSynchronized())
  {
    // kSyncInvalidate - view is invalidated, and this is will be solved by device invalidation.
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncGiCtx))
    {
      //pView->compositeView()->setUserGiContext(userGiContext());
      pView->compositeDevice()->setUserGiContext(userGiContext());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncViewport))
    {
      pView->compositeView()->setViewport(pView->dcLowerLeft(), pView->dcUpperRight());
      pView->compositeView()->setViewport(OdGsDCRectDouble(pView->dcScreenMin(), pView->dcScreenMax()));
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncViewportClip))
    {
      OdIntArray nrcCounts; OdGePoint2dArray nrcPoints;
      pView->viewportClipRegion(nrcCounts, nrcPoints);
      pView->compositeView()->setViewportClipRegion(nrcCounts.size(), nrcCounts.getPtr(), nrcPoints.getPtr());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncClip3d))
    {
      const OdGiAbstractClipBoundary *pClipInfo = NULL;
      const OdGiClipBoundary *pClipBoundary = pView->viewport3dClipping(&pClipInfo);
      pView->compositeView()->setViewport3dClipping(pClipBoundary, pClipInfo);
    }
    if (isRedirection() && pView->isSyncFlagSet(ExGsCompositeView::kSyncBorder))
    {
      // This is only interesting for redirection mode
      pView->compositeView()->setViewportBorderVisibility(pView->isViewportBorderVisible());
      ODCOLORREF color; int width;
      pView->getViewportBorderProperties(color, width);
      pView->compositeView()->setViewportBorderProperties(color, width);
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncViewParam))
    {
      pView->compositeView()->setView(pView->position(), pView->target(), pView->upVector(),
        pView->fieldWidth(), pView->fieldHeight(), (pView->OdGsViewImpl::isPerspective()) ? OdGsView::kPerspective : OdGsView::kParallel);
      pView->compositeView()->setLensLength(pView->lensLength());
      pView->compositeView()->setEnableFrontClip(pView->isFrontClipped());
      pView->compositeView()->setFrontClip(pView->frontClip());
      pView->compositeView()->setEnableBackClip(pView->isBackClipped());
      pView->compositeView()->setBackClip(pView->backClip());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncVisibility))
    { // ? maybe don't need this sync in composite mode?
      if (pView->isVisible())
        pView->compositeView()->show();
      else
        pView->compositeView()->hide();
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncLayer))
    {
      OdUInt32 n = pView->m_frozenLayers.size();
      OdDbStub** id = const_cast<OdDbStub**>(pView->m_frozenLayers.getPtr());
      pView->compositeView()->clearFrozenLayers();
      while (n--)
      {
        pView->compositeView()->freezeLayer(*id);
        ++id;
      }
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncLinetype))
    {
      pView->compositeView()->setLinetypeScaleMultiplier(pView->OdGsViewImpl::linetypeScaleMultiplier());
      pView->compositeView()->setAlternateLinetypeScaleMultiplier(pView->linetypeAlternateScaleMultiplier());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncLineweight))
    {
      pView->compositeView()->setLineweightToDcScale(pView->lineweightToDcScale());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncLwdEnum))
    {
      const OdUInt8Array &lweights = pView->getLweightsEnum();
      pView->compositeView()->setLineweightEnum(lweights.size(), lweights.getPtr());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncClearColor))
    {
      pView->compositeView()->setClearColor(pView->clearColor());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncBackground))
    {
      pView->compositeView()->setBackground(pView->background());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncVisualStyle))
    {
      pView->compositeView()->setVisualStyle(pView->OdGsBaseVectorizeView::visualStyle());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncRenderMode))
    {
      pView->compositeView()->setMode(pView->mode());
    }
    if (pView->isSyncFlagSet(ExGsCompositeView::kSyncGraph))
    {
      ExGsProtectedViewParamsAccessor::castEx(pView->compositeView())->m_drawables = pView->m_drawables;
      ExGsProtectedViewParamsAccessor::castEx(pView->compositeView())->nCachedDrawables() = ExGsProtectedViewParamsAccessor::castEx(pView)->nCachedDrawables();
    }
    pView->resetSyncFlags();
  }
  // Synchronize device changes
  if (/*bViewAttached ||*/ !isSynchronized() /*|| pView->isSyncFlagSet(ExGsCompositeView::kInvalid)*/)
  {
#if 0
    if (isSyncFlagSet(kSyncGiCtx) || bViewAttached)
    {
      pView->compositeDevice()->setUserGiContext(userGiContext());
    }
    // kSyncViews - nothing to do for this
    if (isSyncFlagSet(kSyncBackground) || bViewAttached)
    {
      pView->compositeDevice()->setBackgroundColor(getBackgroundColor());
    }
    if (isSyncFlagSet(kSyncPalette) || bViewAttached)
    {
      pView->compositeDevice()->setLogicalPalette(m_logPalette.getPtr(), (int)m_logPalette.size());
    }
    if (isSyncFlagSet(kSyncResize) || bViewAttached)
    {
      pView->compositeDevice()->onSize(m_outputRect);
    }
#endif
    if (isSyncFlagSet(kSyncInvalidate) /*|| pView->isSyncFlagSet(ExGsCompositeView::kInvalid)*/)
    {
      if (pView->compositeView()->isVisible())
      {
        if (!invalidRects().empty())
        {
          OdUInt32 nRects = invalidRects().size();
          for (OdUInt32 n = 0; n < nRects; n++)
          {
            if (isRectIntersectsView(invalidRects().at(n), *pView))
              pView->compositeDevice()->invalidate(invalidRects().at(n));
          }
        }
        else
        {
          pView->compositeDevice()->invalidate();
        }
      }
    }
  }
}

bool ExGsCompositeDevice::checkIsViewModulesInSync(bool &bPointsToSimilarModule)
{
  bool bInSync = true;
  bPointsToSimilarModule = true;
  OdGsModule *pModule = NULL;
  OdUInt32 nFlags = 0;
  if (isSyncFlagSet(kSyncViews))
    bInSync = false;
  int nViews = numViews(), nView;
  for (nView = 0; nView < nViews; nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    if (!pView->isCompositeViewAttached() || (pView->isSyncFlagSet(ExGsCompositeView::kSyncRenderMode) &&
        pView->compositeModule() != m_moduleFac.moduleForState(pView->mode())))
      bInSync = false;
    if (!pModule)
    {
      pModule = m_moduleFac.moduleForState(pView->mode());
      nFlags = m_moduleFac.flagsForState(pView->mode());
    }
    else
    {
      if ((m_moduleFac.moduleForState(pView->mode()) != pModule) ||
          (m_moduleFac.flagsForState(pView->mode()) != nFlags))
        bPointsToSimilarModule = false;
    }
    if (!bInSync && !bPointsToSimilarModule)
      break; // opt
  }
  return bInSync;
}

void ExGsCompositeDevice::prepareFlagsForSync()
{
  setPartialUpdate(true);
  // Enable all mergeable options here, if one of devices doesn't support anything, we'll disable it for all
  SETBIT(m_flags, kSortRenderTypes    , true);
  SETBIT(m_flags, kDynamicHighlight   , true);
  //SETBIT(m_flags, kDynamicSubhighlight, true);
  SETBIT(m_flags, kSupportOverlays    , true);
  // Restore options from known views
  int nViews = numViews(), nView;
  for (nView = 0; nView < nViews; nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    if (pView->isCompositeViewAttached() && (!pView->isSyncFlagSet(ExGsCompositeView::kSyncRenderMode) ||
        pView->compositeModule() == m_moduleFac.moduleForState(pView->mode())))
    {
      mergeDeviceFlags(*pView);
    }
  }
}

void ExGsCompositeDevice::mergeDeviceFlags(const ExGsCompositeView &pView)
{
  if (!ExGsProtectedDeviceParamsAccessor::castEx(pView.compositeDevice())->supportPartialUpdate())
    setPartialUpdate(false);
  if (!pView.compositeDevice()->sortRenderTypes())
    SETBIT(m_flags, kSortRenderTypes, false);
  if (!pView.compositeDevice()->supportDynamicHighlight())
    SETBIT(m_flags, kDynamicHighlight, false);
  //if (!pView.compositeDevice()->supportDynamicSubhighlight())
  //  SETBIT(m_flags, kDynamicSubhighlight, false);
  if (!pView.compositeDevice()->supportOverlays())
    SETBIT(m_flags, kSupportOverlays, false);
}

void ExGsCompositeDevice::sync()
{
  // Prepare modules factory first
  if (!prepareModuleFactory())
    return;
  bool bRedirectionMode, bInSync = true;
  if (!checkIsViewModulesInSync(bRedirectionMode))
  {
    prepareFlagsForSync();
    bInSync = false;
  }
  if ((bRedirectionMode != isRedirection()) ||
      (bRedirectionMode && !bInSync && // Need to change redirection device (redirection mode doesn't changed)
      !(numViews() > 0 && static_cast<ExGsCompositeView*>(viewAt(0))->compositeModule() == m_moduleFac.moduleForState(viewAt(0)->mode()))))
    resetRedirectionMode(bRedirectionMode);
  // Run views synchronization
  for (int i = 0; i < numViews(); i++)
  {
    syncView(i);
  }
  resetSyncFlags();
}

void ExGsCompositeDevice::resetRedirectionMode(bool bRedirectionMode)
{
  if (!isRedirection())
  {
    // Simply detach all modules
    int nViews = numViews(), nView;
    for (nView = 0; nView < nViews; nView++)
    {
      ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
      pView->detachCompositeView();
      pView->setSyncAll();
    }
  }
  else
  {
    // Invalidate entire cache and detach redirection module
    int nViews = numViews(), nView;
    for (nView = nViews - 1; nView >= 0; nView--)
    {
      ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
      pView->invalidateAllModelCaches();
      pView->detachRedirectionView();
      pView->setSyncAll();
    }
  }
  setRedirection(bRedirectionMode);
  if (bRedirectionMode)
  {
    OdGsDevicePtr pDevice;
    OdUInt32 nFlags;
    int nViews = numViews(), nView;
    for (nView = 0; nView < nViews; nView++)
    {
      ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
      if (pDevice.isNull())
      {
        pDevice = m_moduleFac.moduleForState(pView->mode())->createDevice();
        nFlags = m_moduleFac.flagsForState(pView->mode());
        OdGsBaseVectorizeDevice *pBaseDevice = static_cast<OdGsBaseVectorizeDevice*>(pDevice.get());
        if (pBaseDevice->supportComposition())
        {
          dynamic_cast<ExGsCompositeClientDeviceIface*>(pBaseDevice)->enableRedirectionMode(true);
        }
        setSyncAll();
      }
      pView->attachRedirectionView(pDevice, nFlags);
    }
  }
}

void ExGsCompositeDevice::runRedraw(OdGsDCRect* pUpdatedRect)
{
  // Run standart update loop into OdGsBaseVectorizeDevice (it will work via overrides)
  OdGsBaseVectorizeDevice::update(pUpdatedRect);
}

void ExGsCompositeDevice::preprocess()
{
  setInvalidRects(!invalidRects().empty() && !isRedirection());
}

void ExGsCompositeDevice::postprocess()
{
  setInvalidRects(false);
}

void ExGsCompositeDevice::present()
{
}

void ExGsCompositeDevice::compositeViewAttached(ExGsCompositeView &pView)
{
  if ((!isRedirection() && GETBIT(pView.moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableCompClearScreen)) ||
      (isRedirection() && GETBIT(pView.moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableRedirClearScreen)))
  {
    OdRxDictionaryPtr pProps = pView.compositeDevice()->properties();
    if (!pProps.isNull())
    {
      if (pProps->has(OD_T("ClearScreen")))
        pProps->putAt(OD_T("ClearScreen"), OdRxVariantValue(false));
    }
  }
}

void ExGsCompositeDevice::compositeViewDetached(ExGsCompositeView & /*pView*/)
{
}

void ExGsCompositeDevice::redirectionDeviceAttached(ExGsCompositeDevice &pDevice)
{
  if (pDevice.numViews() > 0)
  {
    compositeViewAttached(*static_cast<ExGsCompositeView*>(pDevice.viewAt(0)));
  }
}

void ExGsCompositeDevice::redirectionDeviceDetached(ExGsCompositeDevice &pDevice)
{
  if (pDevice.numViews() > 0)
  {
    compositeViewDetached(*static_cast<ExGsCompositeView*>(pDevice.viewAt(0)));
  }
}

void ExGsCompositeDevice::presentViewportSurface(ExGsCompositeView & /*pView*/, const OdGsDCRect & /*pUpdatedRect*/)
{
}

void ExGsCompositeDevice::presentRedirectedSurface(ExGsCompositeDevice &pDevice, const OdGsDCRect *pUpdatedRect)
{
  if (pDevice.numViews() > 0)
  {
    OdGsDCRect normRect = (pUpdatedRect) ? *pUpdatedRect : outputRect();
    if (normRect.m_min.y > normRect.m_max.y)
      std::swap(normRect.m_min.y, normRect.m_max.y);
    presentViewportSurface(*static_cast<ExGsCompositeView*>(pDevice.viewAt(0)), normRect);
  }
}

void ExGsCompositeDevice::detachAllViews()
{
  int nViews = numViews();
  for (int i = nViews - 1; i >= 0; i--)
  {
    if (!isRedirection())
      static_cast<ExGsCompositeView*>(viewAt(i))->detachCompositeView();
    else
      static_cast<ExGsCompositeView*>(viewAt(i))->detachRedirectionView();
  }
}

double ExGsCompositeDevice::getRegenCoef() const
{
  int nViews = numViews();
  double regenCoef = 0.;
  for (int nView = 0; nView < nViews; nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(const_cast<ExGsCompositeDevice*>(this)->viewAt(nView));
    if (pView->isCompositeViewAttached())
    {
      OdRxDictionaryPtr pProps = pView->compositeDevice()->properties();
      if (!pProps.isNull())
      {
        if (pProps->has(OD_T("RegenCoef")))
        {
          double curCoef = OdRxVariantValue(pProps->getAt(OD_T("RegenCoef")))->getDouble();
          if (curCoef > regenCoef)
            regenCoef = curCoef;
        }
      }
    }
    if (isRedirection())
      break; // One access is enough
  }
  return regenCoef;
}

void ExGsCompositeDevice::setModuleFactoryProp(OdIntPtr pEntry)
{
  ExGsCompositeModuleProperty *pProp = (ExGsCompositeModuleProperty*)pEntry;
  m_moduleFac.setModuleForState(pProp->m_renderMode, pProp->m_pRenderModule, pProp->m_renderFlags);
}

OdIntPtr ExGsCompositeDevice::getModuleFactoryProp() const
{
  return (OdIntPtr)m_moduleFac.directPropsPtr();
}

void ExGsCompositeDevice::setUserGiContext(OdGiContext* pUserGiContext)
{
  setSyncFlag(kSyncGiCtx);
  ODGSDEV_SUPER::setUserGiContext(pUserGiContext);
}

void ExGsCompositeDevice::invalidate()
{
  setSyncFlag(kSyncInvalidate);
  ODGSDEV_SUPER::invalidate();
}

void ExGsCompositeDevice::invalidate(const OdGsDCRect &screenRect)
{
  setSyncFlag(kSyncInvalidate);
  ODGSDEV_SUPER::invalidate(screenRect);
}

void ExGsCompositeDevice::onSize(const OdGsDCRect& outputRect)
{
  setSyncFlag(kSyncResize);
  ODGSDEV_SUPER::onSize(outputRect);
}

void ExGsCompositeDevice::addView(OdGsView* pView)
{
  setSyncFlag(kSyncViews);
  ODGSDEV_SUPER::addView(pView);
}

void ExGsCompositeDevice::insertView(int viewIndex, OdGsView* pView)
{
  setSyncFlag(kSyncViews);
  ODGSDEV_SUPER::insertView(viewIndex, pView);
}

bool ExGsCompositeDevice::eraseView(OdGsView* pView)
{
  setSyncFlag(kSyncViews);
  if (isRedirection())
  {
    static_cast<ExGsCompositeView*>(pView)->detachRedirectionView();
  }
  return ODGSDEV_SUPER::eraseView(pView);
}

bool ExGsCompositeDevice::eraseView(int viewIndex)
{
  setSyncFlag(kSyncViews);
  if (isRedirection())
  {
    static_cast<ExGsCompositeView*>(viewAt(viewIndex))->detachRedirectionView();
  }
  return ODGSDEV_SUPER::eraseView(viewIndex);
}

void ExGsCompositeDevice::eraseAllViews()
{
  setSyncFlag(kSyncViews);
  ODGSDEV_SUPER::eraseAllViews();
}

bool ExGsCompositeDevice::setBackgroundColor(ODCOLORREF backgroundColor)
{
  setSyncFlag(kSyncBackground);
  return ODGSDEV_SUPER::setBackgroundColor(backgroundColor);
}

void ExGsCompositeDevice::setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors)
{
  setSyncFlag(kSyncPalette);
  ODGSDEV_SUPER::setLogicalPalette(logicalPalette, numColors);
}

//
