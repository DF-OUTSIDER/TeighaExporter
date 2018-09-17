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
// Composite view

#include "OdaCommon.h"
#include "ExGsCompositeDevice.h"
#include "Gs/GsBaseModel.h"
#include "ExGsCompositeModuleInterface.h"
#include "ExGsCompositeView.h"

#if (_MSC_VER >= 1400)
#define ODGSVIEW_SUPER __super
#else
#define ODGSVIEW_SUPER OdGsBaseVectorizeView
#endif

ExGsCompositeView::ExGsCompositeView()
  : OdGsBaseVectorizeViewDef()
  , ExGsCompositeSyncFlags()
  , m_compositeFlags(0)
  , m_pCompModule(NULL)
  , m_pCompDevice(NULL)
  , m_pCompView(NULL)
  , m_factoryFlags(0)
{
}

ExGsCompositeView::~ExGsCompositeView()
{
  if ((device() != NULL) && (device()->isRedirection()))
    detachRedirectionView();
  else
    detachCompositeView();
}

ExGsCompositeDevice* ExGsCompositeView::device()
{
  return static_cast<ExGsCompositeDevice*>(OdGsBaseVectorizeView::device());
}

void ExGsCompositeView::detachCompositeView()
{
  if (isCompositeViewAttached())
  {
    device()->compositeViewDetached(*this);
    // Destroy all dependent metafiles
    invalidateCachedViewportGeometry();
    invalidateCachedViewportMaterials();
    // Remove holder to prevent additional detach
    ExGsProtectedViewParamsAccessor::castEx(m_pCompView)->m_drawables.clear();
    ExGsProtectedViewParamsAccessor::castEx(m_pCompView)->nCachedDrawables() = 0;
    // Release dependent device
    m_pRealDevice.release();
    m_pCompModule = NULL;
    m_pCompDevice = NULL;
    m_pCompView = NULL;
    m_factoryFlags = 0;
    setCompositeViewAttached(false);
  }
}

void ExGsCompositeView::attachCompositeView()
{
  if (!isCompositeViewAttached())
  {
    m_pCompModule = device()->m_moduleFac.moduleForState(mode());
    m_factoryFlags = device()->m_moduleFac.flagsForState(mode());
    //if (!device()->isBitmapDevice())
      m_pRealDevice = m_pCompModule->createDevice();
    //else
    //  m_pRealDevice = m_pCompModule->createBitmapDevice();
    m_pCompDevice = static_cast<OdGsBaseVectorizeDevice*>(m_pRealDevice.get());
    { // If device doesn't support composition, we could work, but behavior is unexpected
      if (!m_pCompDevice->supportComposition())
      {
        throw OdError(eLibIntegrityBroken);
      }
    }
    m_pCompDevice->enableComposition(true);
    OdGsClientViewInfo cliInfo;
    clientViewInfo(cliInfo);
    cliInfo.viewportFlags = 0; // don't need such kind of information
    OdGsViewPtr pView = m_pCompDevice->createView(&cliInfo);
    m_pCompView = static_cast<OdGsBaseVectorizeView*>(pView.get());
    ExGsProtectedViewParamsAccessor::castEx(m_pCompView)->m_giViewportId = m_giViewportId;
    m_pCompDevice->addView(m_pCompView);
    device()->compositeViewAttached(*this);
    setCompositeViewAttached(true);
  }
}

void ExGsCompositeView::detachRedirectionView()
{
  if (isCompositeViewAttached())
  {
    //device()->compositeViewDetached(*this);
    // Remove holder to prevent additional detach
    ExGsProtectedViewParamsAccessor::castEx(m_pCompView)->m_drawables.clear();
    ExGsProtectedViewParamsAccessor::castEx(m_pCompView)->nCachedDrawables() = 0;
    // Erase view
    if (m_pCompDevice->numViews() == 1)
      device()->redirectionDeviceDetached(*device());
    m_pCompDevice->eraseView(m_pCompView);
    // Release dependent device
    m_pRealDevice.release();
    m_pCompModule = NULL;
    m_pCompDevice = NULL;
    m_pCompView = NULL;
    m_factoryFlags = 0;
    setCompositeViewAttached(false);
  }
}

inline int findViewIndex(OdGsDevice *pDevice, OdGsView *pView)
{
  for (int i = 0; i < pDevice->numViews(); i++)
  {
    if (pDevice->viewAt(i) == pView)
      return i;
  }
  return -1;
}

void ExGsCompositeView::attachRedirectionView(OdGsDevice *pDevice, OdUInt32 flags)
{
  if (!isCompositeViewAttached())
  {
    m_factoryFlags = flags;
    m_pRealDevice = pDevice;
    m_pCompDevice = static_cast<OdGsBaseVectorizeDevice*>(m_pRealDevice.get());
    OdGsClientViewInfo cliInfo;
    clientViewInfo(cliInfo);
    OdGsViewPtr pView = m_pCompDevice->createView(&cliInfo);
    m_pCompView = static_cast<OdGsBaseVectorizeView*>(pView.get());
    ExGsProtectedViewParamsAccessor::castEx(m_pCompView)->m_giViewportId = m_giViewportId;
    m_pCompDevice->insertView(::findViewIndex(device(), this), m_pCompView);
    m_pCompModule = const_cast<OdGsBaseModule*>(m_pCompView->getGsModulePtr());
    //device()->compositeViewAttached(*this);
    if (m_pCompDevice->numViews() == 1)
      device()->redirectionDeviceAttached(*device());
    setCompositeViewAttached(true);
  }
}

void ExGsCompositeView::syncMe() const
{
  if (!isSynchronized() || !isCompositeViewAttached())
    const_cast<ExGsCompositeView*>(this)->device()->sync();
}

void ExGsCompositeView::invalidateCachedViewportMaterials()
{
  // @@@TODO: currently we invalidates entire cache, this maybe ineffective potentially.
  if (cachedDrawables())
  {
    for (unsigned i = 0; i < m_drawables.size(); ++i)
    {
      DrawableHolder& holder = m_drawables[i];
      OdGsBaseModel* pModel = static_cast<OdGsBaseModel*>(holder.m_pGsModel.get());
      if (pModel)
        pModel->invalidate(OdGsModel::kInvalidateMaterials);
    }
  }
}

void ExGsCompositeView::invalidateAllModelCaches()
{
  if (cachedDrawables())
  {
    for (unsigned i = 0; i < m_drawables.size(); ++i)
    {
      DrawableHolder& holder = m_drawables[i];
      OdGsBaseModel* pModel = static_cast<OdGsBaseModel*>(holder.m_pGsModel.get());
      if (pModel)
        pModel->invalidate(OdGsModel::kInvalidateAll);
    }
  }
}

// sync overrides

void ExGsCompositeView::setUserGiContext(OdGiContext* pUserGiContext)
{
  setSyncFlag(kSyncGiCtx);
  ODGSVIEW_SUPER::setUserGiContext(pUserGiContext);
}

void ExGsCompositeView::setLineweightToDcScale(double scale)
{
  setSyncFlag(kSyncLineweight);
  ODGSVIEW_SUPER::setLineweightToDcScale(scale);
}

void ExGsCompositeView::setLineweightEnum(int numLineweights, const OdUInt8* lineweights)
{
  setSyncFlag(kSyncLwdEnum);
  ODGSVIEW_SUPER::setLineweightEnum(numLineweights, lineweights);
}

void ExGsCompositeView::setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight)
{
  setSyncFlag(kSyncViewport);
  ODGSVIEW_SUPER::setViewport(lowerLeft, upperRight);
}

void ExGsCompositeView::setViewport(const OdGsDCRect& screenRect)
{
  setSyncFlag(kSyncViewport);
  ODGSVIEW_SUPER::setViewport(screenRect);
}

void ExGsCompositeView::setViewport(const OdGsDCRectDouble& screenRect)
{
  setSyncFlag(kSyncViewport);
  ODGSVIEW_SUPER::setViewport(screenRect);
}

void ExGsCompositeView::setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices)
{
  setSyncFlag(kSyncViewportClip);
  ODGSVIEW_SUPER::setViewportClipRegion(numContours, numVertices, vertices);
}

void ExGsCompositeView::setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices)
{
  setSyncFlag(kSyncViewportClip);
  ODGSVIEW_SUPER::setViewportClipRegion(numContours, numVertices, vertices);
}

void ExGsCompositeView::setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo)
{
  setSyncFlag(kSyncClip3d);
  ODGSVIEW_SUPER::setViewport3dClipping(pBoundary, pClipInfo);
}

void ExGsCompositeView::setViewportBorderProperties(ODCOLORREF color, int width)
{
  setSyncFlag(kSyncBorder);
  ODGSVIEW_SUPER::setViewportBorderProperties(color, width);
}

void ExGsCompositeView::setViewportBorderVisibility(bool visible)
{
  setSyncFlag(kSyncBorder);
  ODGSVIEW_SUPER::setViewportBorderVisibility(visible);
}

void ExGsCompositeView::setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                                double fieldWidth, double fieldHeight, Projection projectionType)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::setView(position, target, upVector, fieldWidth, fieldHeight, projectionType);
}

void ExGsCompositeView::setLensLength(double lensLength)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::setLensLength(lensLength);
}

void ExGsCompositeView::setEnableFrontClip(bool enable)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::setEnableFrontClip(enable);
}

void ExGsCompositeView::setFrontClip(double frontClip)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::setFrontClip(frontClip);
}

void ExGsCompositeView::setEnableBackClip(bool enable)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::setEnableBackClip(enable);
}

void ExGsCompositeView::setBackClip(double backClip)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::setBackClip(backClip);
}

void ExGsCompositeView::setMode(RenderMode mode)
{
  setSyncFlag(kSyncRenderMode);
  ODGSVIEW_SUPER::setMode(mode);
}

bool ExGsCompositeView::add(OdGiDrawable* pSceneGraph, OdGsModel* pModel)
{
  setSyncFlag(kSyncGraph);
  return ODGSVIEW_SUPER::add(pSceneGraph, pModel);
}

bool ExGsCompositeView::erase(OdGiDrawable* sceneGraph)
{
  setSyncFlag(kSyncGraph);
  return ODGSVIEW_SUPER::erase(sceneGraph);
}

void ExGsCompositeView::eraseAll()
{
  setSyncFlag(kSyncGraph);
  ODGSVIEW_SUPER::eraseAll();
}

void ExGsCompositeView::invalidate()
{
  setSyncFlag(kSyncInvalidate);
  ODGSVIEW_SUPER::invalidate();
}

void ExGsCompositeView::invalidate(const OdGsDCRect &screenRect)
{
  setSyncFlag(kSyncInvalidate);
  ODGSVIEW_SUPER::invalidate(screenRect);
}

void ExGsCompositeView::flush()
{
  if (isCompositeViewAttached())
    compositeView()->flush();
}

void ExGsCompositeView::hide()
{
  setSyncFlag(kSyncVisibility);
  ODGSVIEW_SUPER::hide();
}

void ExGsCompositeView::show()
{
  setSyncFlag(kSyncVisibility);
  ODGSVIEW_SUPER::show();
}

void ExGsCompositeView::freezeLayer(OdDbStub* layerID)
{
  setSyncFlag(kSyncLayer);
  ODGSVIEW_SUPER::freezeLayer(layerID);
}

void ExGsCompositeView::thawLayer(OdDbStub* layerID)
{
  setSyncFlag(kSyncLayer);
  ODGSVIEW_SUPER::thawLayer(layerID);
}

void ExGsCompositeView::clearFrozenLayers()
{
  setSyncFlag(kSyncLayer);
  ODGSVIEW_SUPER::clearFrozenLayers();
}

void ExGsCompositeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, SelectionMode mode)
{
  syncMe();
  if (isCompositeViewAttached())
    compositeView()->select(pts, nPoints, pReactor, mode);
}

bool ExGsCompositeView::viewExtents(OdGeBoundBlock3d& extents) const
{
  syncMe();
  if (isCompositeViewAttached())
    return compositeView()->viewExtents(extents);
  return false;
}

void ExGsCompositeView::dolly(const OdGeVector3d& dollyVector)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::dolly(dollyVector);
}

void ExGsCompositeView::dolly(double xDolly, double yDolly, double zDolly)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::dolly(xDolly, yDolly, zDolly);
}

void ExGsCompositeView::roll(double rollAngle)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::roll(rollAngle);
}

void ExGsCompositeView::orbit(double xOrbit, double yOrbit)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::orbit(xOrbit, yOrbit);
}

void ExGsCompositeView::zoom(double zoomFactor)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::zoom(zoomFactor);
}

void ExGsCompositeView::pan(double xPan, double yPan)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::pan(xPan, yPan);
}

void ExGsCompositeView::zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::zoomExtents(minPt, maxPt);
}

void ExGsCompositeView::zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight)
{
  setSyncFlag(kSyncViewParam);
  ODGSVIEW_SUPER::zoomWindow(lowerLeft, upperRight);
}

bool ExGsCompositeView::exceededBounds()
{
  //syncMe();
  if (isCompositeViewAttached())
    return compositeView()->exceededBounds();
  return false;
}

void ExGsCompositeView::initLights(OdRxIterator* /*pLightsIterator*/)
{
  // @@@TODO:
  // i. e. only opengl device supports this method on our side. probably support of it isn't interesting for us at all.
}

void ExGsCompositeView::setLinetypeScaleMultiplier(double linetypeScaleMultiplier)
{
  setSyncFlag(kSyncLinetype);
  ODGSVIEW_SUPER::setLinetypeScaleMultiplier(linetypeScaleMultiplier);
}

void ExGsCompositeView::setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier)
{
  setSyncFlag(kSyncLinetype);
  ODGSVIEW_SUPER::setAlternateLinetypeScaleMultiplier(linetypeAlternateScaleMultiplier);
}

void ExGsCompositeView::setClearColor(ClearColor color)
{
  setSyncFlag(kSyncClearColor);
  ODGSVIEW_SUPER::setClearColor(color);
}

void ExGsCompositeView::setBackground(OdDbStub *backgroundId)
{
  setSyncFlag(kSyncBackground);
  ODGSVIEW_SUPER::setBackground(backgroundId);
}

void ExGsCompositeView::setVisualStyle(OdDbStub *visualStyleId)
{
  setSyncFlag(kSyncVisualStyle);
  OdGsBaseVectorizeView::setVisualStyle(visualStyleId);
}

void ExGsCompositeView::setVisualStyle(const OdGiVisualStyle &visualStyle)
{
  setSyncFlag(kSyncVisualStyle);
  OdGsBaseVectorizeView::setVisualStyle(visualStyle);
}

void ExGsCompositeView::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  if (isCompositeViewAttached())
    compositeView()->getSnapShot(pImage, region);
}

// presentation

void ExGsCompositeView::propagateLayersChanges()
{
  // It's ok to update layer changes through composite view
#if 1
  OdGsBaseVectorizeView::propagateLayersChanges();
#endif
}

void ExGsCompositeView::propagateInvalidVpFlag()
{
  // Same with layer changes
#if 0
  OdGsBaseVectorizeView::propagateInvalidVpFlag();
#endif
}

void ExGsCompositeView::updateViewProps()
{
  // Ok to update here
  //OdGsBaseVectorizeView::updateViewProps();
#if 0
  ExGsProtectedViewParamsAccessor::castEx(compositeView())->updateViewProps();
#endif
}

void ExGsCompositeView::beginViewVectorization()
{
  // Does nothing in composite view
}

void ExGsCompositeView::endViewVectorization()
{
  // Does nothing in composite view
}

void ExGsCompositeView::loadViewport()
{
  // Does nothing in composite view
}

void ExGsCompositeView::update()
{
  syncMe(); // Has interface to call outside Gs, so we must check synchronization here
  //if (isCompositeViewAttached())
  //  compositeView()->update();
  OdGsBaseVectorizeView::update();
}

void ExGsCompositeView::updateExtents(bool /*buildCache*/)
{
  // Does nothing in composite view
}

void ExGsCompositeView::display(bool /*update*/)
{
  // This is our main override. Render through real device here and copy rendering result to rendering surface.
  // (we will never enter here if haven't initialized devices, so doesn't need to check)
  OdGsDCRect updatedRect;
  if (!device()->supportPartialUpdate() &&
      ExGsProtectedDeviceParamsAccessor::castEx(compositeDevice())->supportPartialUpdate() &&
      compositeDevice()->supportComposition())
  {
    // In this situation we need solve our composition rules
    dynamic_cast<ExGsCompositeClientDeviceIface*>(compositeDevice())->enableCompositionUpdate(true);
    compositeDevice()->update(&updatedRect);
    dynamic_cast<ExGsCompositeClientDeviceIface*>(compositeDevice())->enableCompositionUpdate(false);
    compositeDevice()->update(&updatedRect);
  }
  else
    compositeDevice()->update(&updatedRect);
  if (!GETBIT(m_factoryFlags, ExGsCompositeModuleProperty::kDisableCompDoubleBuffer))
    device()->presentViewportSurface(*this, updatedRect);
}

void ExGsCompositeView::drawViewportFrame()
{
  // Draw viewport frame here, by our rendering methods on our composition surface.
  // [must be done by platform-dependent device part] override it.
}

bool ExGsCompositeView::isLocalViewportIdCompatible(const OdGsViewImpl *pView) const
{
  return (pView == m_pCompView) || OdGsBaseVectorizeView::isLocalViewportIdCompatible(pView);
}

class GsViewImplHelper
{
public:
  static int& nCachedDrawables(OdGsViewImpl& view) { return view.m_nCachedDrawables; }
};


int &ExGsProtectedViewParamsAccessor::nCachedDrawables()
{
  return GsViewImplHelper::nCachedDrawables(*this);
}

//
