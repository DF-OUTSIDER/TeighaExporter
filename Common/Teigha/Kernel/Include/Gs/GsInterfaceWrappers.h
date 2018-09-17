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

#ifndef __GSINTERFACEWRAPPERS_H_INCLUDED_
#define __GSINTERFACEWRAPPERS_H_INCLUDED_

#include "Gs/Gs.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"
#include "Gs/GsModel.h"
#include "Gs/GsUpdateManager.h"

#include "UInt8Array.h"
#include "IntArray.h"
#include "Ge/GePoint2dArray.h"

#include "TD_PackPush.h"

/** \details
    Default wrapper base class for Gs interface classes.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename RedirectionBaseInterface, typename RedirectionInterface>
class OdGsDefaultRedirectionBase : public RedirectionInterface
{
  private:
    RedirectionBaseInterface *m_pRedirectionObj;
  public:
    OdGsDefaultRedirectionBase(RedirectionBaseInterface *pRedirectionObj = NULL)
      : m_pRedirectionObj(pRedirectionObj)
    {}

    bool hasRedirection() const { return m_pRedirectionObj != NULL; }
    const RedirectionBaseInterface *redirection() const { return m_pRedirectionObj; }
    RedirectionBaseInterface *redirection() { return m_pRedirectionObj; }
    void setRedirection(RedirectionBaseInterface *pRedirectionObj) { m_pRedirectionObj = pRedirectionObj; }
    void resetRedirection() { m_pRedirectionObj = NULL; }
};

/** \details
    Abstract wrapper base class for Gs interface classes.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename RedirectionBaseInterface, typename RedirectionInterface>
class OdGsAbstractRedirectionBase : public RedirectionInterface
{
  public:
    OdGsAbstractRedirectionBase(RedirectionBaseInterface * /*pRedirectionObj*/ = NULL)
    {}

    virtual bool hasRedirection() const = 0;
    virtual const RedirectionBaseInterface *redirection() const = 0;
    virtual RedirectionBaseInterface *redirection() = 0;
    virtual void setRedirection(RedirectionBaseInterface * /*pRedirectionObj*/) {}
    virtual void resetRedirection() {}
};

/** \details
    Base template for OdGsView class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsViewInterface = OdGsView, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsView, GsViewInterface> >
class OdGsViewWrapper : public RedirectionBase
{
  private:
    bool trHas() const { return this->hasRedirection(); }
    const OdGsView *trCall() const { return this->redirection(); }
    OdGsView *trCall() { return this->redirection(); }
  public:
    OdGsViewWrapper(OdGsView *pRedirectionView = NULL)
      : RedirectionBase(pRedirectionView)
    {}

    // OdGsView wrappers

    OdGsDevice* device() const
    {
      if (trHas())
        return trCall()->device();
      return NULL;
    }

    OdGiContext* userGiContext() const
    {
      if (trHas())
        return trCall()->userGiContext();
      return NULL;
    }
    void setUserGiContext(OdGiContext* pUserGiContext)
    {
      if (trHas())
        trCall()->setUserGiContext(pUserGiContext);
    }

    double lineweightToDcScale() const
    {
      if (trHas())
        return trCall()->lineweightToDcScale();
      return 1.0;
    }
    void setLineweightToDcScale(double scale)
    {
      if (trHas())
        trCall()->setLineweightToDcScale(scale);
    }
    void setLineweightEnum(int numLineweights, const OdUInt8* lineweights)
    {
      if (trHas())
        trCall()->setLineweightEnum(numLineweights, lineweights);
    }

    void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight)
    {
      if (trHas())
        trCall()->setViewport(lowerLeft, upperRight);
    }
    void setViewport(const OdGsDCRect& screenRect)
    {
      if (trHas())
        trCall()->setViewport(screenRect);
    }
    void setViewport(const OdGsDCRectDouble& screenRect)
    {
      if (trHas())
        trCall()->setViewport(screenRect);
    }
    void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const
    {
      if (trHas())
        trCall()->getViewport(lowerLeft, upperRight);
    }
    void getViewport(OdGsDCRect& screenRect) const
    {
      if (trHas())
        trCall()->getViewport(screenRect);
    }
    void getViewport(OdGsDCRectDouble& screenRect) const
    {
      if (trHas())
        trCall()->getViewport(screenRect);
    }

    void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices)
    {
      if (trHas())
        trCall()->setViewportClipRegion(numContours, numVertices, vertices);
    }
    void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices)
    {
      if (trHas())
        trCall()->setViewportClipRegion(numContours, numVertices, vertices);
    }
    void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& vertices) const
    {
      if (trHas())
        trCall()->viewportClipRegion(counts, vertices);
    }
    void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const
    {
      if (trHas())
        trCall()->viewportClipRegion(counts, vertices);
    }

    void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL)
    {
      if (trHas())
        trCall()->setViewport3dClipping(pBoundary, pClipInfo);
    }
    const OdGiClipBoundary *viewport3dClipping(const OdGiAbstractClipBoundary** ppClipInfo = NULL) const
    {
      if (trHas())
        return trCall()->viewport3dClipping(ppClipInfo);
      return NULL;
    }

    void setViewportBorderProperties(ODCOLORREF color, int width)
    {
      if (trHas())
        trCall()->setViewportBorderProperties(color, width);
    }
    void getViewportBorderProperties(ODCOLORREF& color, int& width) const
    {
      if (trHas())
        trCall()->getViewportBorderProperties(color, width);
    }
    void setViewportBorderVisibility(bool visible)
    {
      if (trHas())
        trCall()->setViewportBorderVisibility(visible);
    }
    bool isViewportBorderVisible() const
    {
      if (trHas())
        return trCall()->isViewportBorderVisible();
      return false;
    }

    void setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                 double fieldWidth, double fieldHeight, OdGsView::Projection projectionType = OdGsView::kParallel)
    {
      if (trHas())
        trCall()->setView(position, target, upVector, fieldWidth, fieldHeight, projectionType);
    }
    OdGePoint3d position() const
    {
      if (trHas())
        return trCall()->position();
      return OdGePoint3d::kOrigin;
    }
    OdGePoint3d target() const
    {
      if (trHas())
        return trCall()->target();
      return OdGePoint3d::kOrigin + OdGeVector3d::kZAxis;
    }
    OdGeVector3d upVector() const
    {
      if (trHas())
        return trCall()->upVector();
      return OdGeVector3d::kYAxis;
    }

    double lensLength() const
    {
      if (trHas())
        return trCall()->lensLength();
      return 50.0;
    }
    void setLensLength(double lensLength)
    {
      if (trHas())
        trCall()->setLensLength(lensLength);
    }
    bool isPerspective() const
    {
      if (trHas())
        return trCall()->isPerspective();
      return false;
    }

    double fieldWidth() const
    {
      if (trHas())
        return trCall()->fieldWidth();
      return 1.0;
    }
    double fieldHeight() const
    {
      if (trHas())
        return trCall()->fieldHeight();
      return 1.0;
    }

    void setEnableFrontClip(bool enable)
    {
      if (trHas())
        trCall()->setEnableFrontClip(enable);
    }
    bool isFrontClipped() const
    {
      if (trHas())
        return trCall()->isFrontClipped();
      return false;
    }
    void setFrontClip(double frontClip)
    {
      if (trHas())
        trCall()->setFrontClip(frontClip);
    }
    double frontClip() const
    {
      if (trHas())
        return trCall()->frontClip();
      return 0.0;
    }
    void setEnableBackClip(bool enable)
    {
      if (trHas())
        trCall()->setEnableBackClip(enable);
    }
    bool isBackClipped() const
    {
      if (trHas())
        return trCall()->isBackClipped();
      return false;
    }
    void setBackClip(double backClip)
    {
      if (trHas())
        trCall()->setBackClip(backClip);
    }
    double backClip() const
    {
      if (trHas())
        return trCall()->backClip();
      return 0.0;
    }

    OdGeMatrix3d viewingMatrix() const
    {
      if (trHas())
        return trCall()->viewingMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    OdGeMatrix3d projectionMatrix() const
    {
      if (trHas())
        return trCall()->projectionMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    OdGeMatrix3d screenMatrix() const
    {
      if (trHas())
        return trCall()->screenMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    OdGeMatrix3d worldToDeviceMatrix() const
    {
      if (trHas())
        return trCall()->worldToDeviceMatrix();
      return OdGeMatrix3d::kIdentity;
    }
    OdGeMatrix3d objectToDeviceMatrix() const
    {
      if (trHas())
        return trCall()->objectToDeviceMatrix();
      return OdGeMatrix3d::kIdentity;
    }

    void setMode(OdGsView::RenderMode mode)
    {
      if (trHas())
        trCall()->setMode(mode);
    }
    OdGsView::RenderMode mode() const
    {
      if (trHas())
        return trCall()->mode();
      return OdGsView::kNone;
    }

    bool add(OdGiDrawable* pSceneGraph, OdGsModel* pModel)
    {
      if (trHas())
        return trCall()->add(pSceneGraph, pModel);
      return false;
    }
    int numRootDrawables() const {
      if (trHas())
        return trCall()->numRootDrawables();
      return 0;
    }
    OdDbStub* rootDrawableIdAt(int i, OdGsModelPtr* pModelReturn /*= 0*/) const {
      if (trHas())
        return trCall()->rootDrawableIdAt(i, pModelReturn);
      return 0;
    }
    OdGiDrawablePtr rootDrawableAt(int i, OdGsModelPtr* pModelReturn /*= 0*/) const {
      if (trHas())
        return trCall()->rootDrawableAt(i, pModelReturn);
      return OdGiDrawablePtr();
    }
    bool erase(OdGiDrawable* sceneGraph)
    {
      if (trHas())
        return trCall()->erase(sceneGraph);
      return false;
    }
    void eraseAll()
    {
      if (trHas())
        trCall()->eraseAll();
    }

    OdGsModel *getModel(const OdGiDrawable *pDrawable) const
    {
      if (trHas())
        return trCall()->getModel(pDrawable);
      return NULL;
    }
    OdGsModelArray getModelList() const
    {
      if (trHas())
        return trCall()->getModelList();
      return OdGsModelArray();
    }

    void invalidate()
    {
      if (trHas())
        trCall()->invalidate();
    }
    void invalidate(const OdGsDCRect &screenRect)
    {
      if (trHas())
        trCall()->invalidate(screenRect);
    }
    bool isValid() const
    {
      if (trHas())
        return trCall()->isValid();
      return false;
    }

    void update()
    {
      if (trHas())
        trCall()->update();
    }

    void beginInteractivity(double frameRateInHz)
    {
      if (trHas())
        trCall()->beginInteractivity(frameRateInHz);
    }
    void endInteractivity()
    {
      if (trHas())
        trCall()->endInteractivity();
    }
    bool isInInteractivity() const
    {
      if (trHas())
        return trCall()->isInInteractivity();
      return false;
    }
    double interactivityFrameRate() const
    {
      if (trHas())
        return trCall()->interactivityFrameRate();
      return 0.0;
    }

    void flush()
    {
      if (trHas())
        trCall()->flush();
    }

    void hide()
    {
      if (trHas())
        trCall()->hide();
    }
    void show()
    {
      if (trHas())
        trCall()->show();
    }
    bool isVisible()
    {
      if (trHas())
        return trCall()->isVisible();
      return false;
    }

    void freezeLayer(OdDbStub* layerID)
    {
      if (trHas())
        trCall()->freezeLayer(layerID);
    }
    void thawLayer(OdDbStub* layerID)
    {
      if (trHas())
        trCall()->thawLayer(layerID);
    }
    void clearFrozenLayers()
    {
      if (trHas())
        trCall()->clearFrozenLayers();
    }

    void invalidateCachedViewportGeometry()
    {
      if (trHas())
        trCall()->invalidateCachedViewportGeometry();
    }

    void select(const OdGsDCPoint* pts, int nPoints, OdGsSelectionReactor* pReactor,
                OdGsView::SelectionMode mode = OdGsView::kCrossing)
    {
      if (trHas())
        trCall()->select(pts, nPoints, pReactor, mode);
    }

    void dolly(const OdGeVector3d& dollyVector)
    {
      if (trHas())
        trCall()->dolly(dollyVector);
    }
    void dolly(double xDolly, double yDolly, double zDolly)
    {
      if (trHas())
        trCall()->dolly(xDolly, yDolly, zDolly);
    }
    void roll(double rollAngle)
    {
      if (trHas())
        trCall()->roll(rollAngle);
    }
    void orbit(double xOrbit, double yOrbit)
    {
      if (trHas())
        trCall()->orbit(xOrbit, yOrbit);
    }
    void zoom(double zoomFactor)
    {
      if (trHas())
        trCall()->zoom(zoomFactor);
    }
    void pan(double xPan, double yPan)
    {
      if (trHas())
        trCall()->pan(xPan, yPan);
    }

    void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt)
    {
      if (trHas())
        trCall()->zoomExtents(minPt, maxPt);
    }
    void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight)
    {
      if (trHas())
        trCall()->zoomWindow(lowerLeft, upperRight);
    }

    bool pointInView(const OdGePoint3d &pt) const
    {
      if (trHas())
        return trCall()->pointInView(pt);
      return false;
    }
    bool extentsInView(const OdGePoint3d &minPt, const OdGePoint3d &maxPt) const
    {
      if (trHas())
        return trCall()->extentsInView(minPt, maxPt);
      return false;
    }

    OdGsViewPtr cloneView(bool cloneViewParameters = true, bool cloneGeometry = false)
    {
      if (trHas())
        return trCall()->cloneView(cloneViewParameters, cloneGeometry);
      return OdGsViewPtr();
    }
    void viewParameters(OdGsView* pView) const
    {
      if (trHas())
        trCall()->viewParameters(pView);
    }

    bool exceededBounds()
    {
      if (trHas())
        return trCall()->exceededBounds();
      return false;
    }

    void enableStereo(bool enabled)
    {
      if (trHas())
        trCall()->enableStereo(enabled);
    }
    bool isStereoEnabled() const
    {
      if (trHas())
        return trCall()->isStereoEnabled();
      return false;
    }
    void setStereoParameters(double magnitude, double parallax)
    {
      if (trHas())
        trCall()->setStereoParameters(magnitude, parallax);
    }
    void getStereoParameters(double& magnitude, double& parallax) const
    {
      if (trHas())
        trCall()->getStereoParameters(magnitude, parallax);
    }

    void initLights(OdRxIterator* pLightsIterator)
    {
      if (trHas())
        trCall()->initLights(pLightsIterator);
    }

    void setLinetypeScaleMultiplier(double linetypeScaleMultiplier)
    {
      if (trHas())
        trCall()->setLinetypeScaleMultiplier(linetypeScaleMultiplier);
    }
    double linetypeScaleMultiplier() const
    {
      if (trHas())
        return trCall()->linetypeScaleMultiplier();
      return 1.0;
    }
    void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier)
    {
      if (trHas())
        trCall()->setAlternateLinetypeScaleMultiplier(linetypeAlternateScaleMultiplier);
    }
    double linetypeAlternateScaleMultiplier() const
    {
      if (trHas())
        return trCall()->linetypeAlternateScaleMultiplier();
      return 1.0;
    }

    void clientViewInfo(OdGsClientViewInfo& clientViewInfo) const
    {
      if (trHas())
        trCall()->clientViewInfo(clientViewInfo);
    }

    void setClearColor(OdGsView::ClearColor color)
    {
      if (trHas())
        trCall()->setClearColor(color);
    }

    bool pointInViewport(const OdGePoint2d& screenPoint) const
    {
      if (trHas())
        return trCall()->pointInViewport(screenPoint);
      return false;
    }

    void getNumPixelsInUnitSquare(const OdGePoint3d& point,
                                  OdGePoint2d& pixelDensity, bool bUsePerspective = true) const
    {
      if (trHas())
        trCall()->getNumPixelsInUnitSquare(point, pixelDensity, bUsePerspective);
    }

    void setBackground(OdDbStub *backgroundId)
    {
      if (trHas())
        trCall()->setBackground(backgroundId);
    }
    OdDbStub *background() const
    {
      if (trHas())
        return trCall()->background();
      return NULL;
    }

    void setVisualStyle(OdDbStub *visualStyleId)
    {
      if (trHas())
        trCall()->setVisualStyle(visualStyleId);
    }
    OdDbStub *visualStyle() const
    {
      if (trHas())
        return trCall()->visualStyle();
      return NULL;
    }
    void setVisualStyle(const OdGiVisualStyle &visualStyle)
    {
      if (trHas())
        trCall()->setVisualStyle(visualStyle);
    }
    bool visualStyle(OdGiVisualStyle &visualStyle) const
    {
      if (trHas())
        return trCall()->visualStyle(visualStyle);
      return false;
    }

    void enableDefaultLighting(bool bEnable, OdGsView::DefaultLightingType lightType = OdGsView::kTwoLights)
    {
      if (trHas())
        trCall()->enableDefaultLighting(bEnable, lightType);
    }

    void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
    {
      if (trHas())
        trCall()->getSnapShot(pImage, region);
    }

    void collide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, const OdGsCollisionDetectionContext* pCtx = NULL )
    {
      if( trHas())
        trCall()->collide( pInputList, nInputListSize, pReactor, pCollisionWithList, nCollisionWithListSize, pCtx );
    }
};

/** \details
    Base template for OdGsDevice class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsDeviceInterface = OdGsDevice, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsDevice, GsDeviceInterface> >
class OdGsDeviceWrapper : public RedirectionBase
{
  private:
    bool trHas() const { return this->hasRedirection(); }
    const OdGsDevice *trCall() const { return this->redirection(); }
    OdGsDevice *trCall() { return this->redirection(); }
  public:
    OdGsDeviceWrapper(OdGsDevice *pRedirectionDevice = NULL)
      : RedirectionBase(pRedirectionDevice)
    {}

    // OdGsDevice wrappers

    OdRxDictionaryPtr properties()
    {
      if (trHas())
        return trCall()->properties();
      return OdRxDictionaryPtr();
    }

    OdGiContext* userGiContext() const
    {
      if (trHas())
        return trCall()->userGiContext();
      return NULL;
    }
    void setUserGiContext(OdGiContext* pUserGiContext)
    {
      if (trHas())
        trCall()->setUserGiContext(pUserGiContext);
    }

    void invalidate()
    {
      if (trHas())
        trCall()->invalidate();
    }
    void invalidate(const OdGsDCRect &screenRect)
    {
      if (trHas())
        trCall()->invalidate(screenRect);
    }
    bool isValid() const
    {
      if (trHas())
        return trCall()->isValid();
      return false;
    }

    void update(OdGsDCRect* pUpdatedRect = 0)
    {
      if (trHas())
        trCall()->update(pUpdatedRect);
    }

    void onSize(const OdGsDCRect& outputRect)
    {
      if (trHas())
        trCall()->onSize(outputRect);
    }
    void onSize(const OdGsDCRectDouble& outputRect)
    {
      if (trHas())
        trCall()->onSize(outputRect);
    }

    void getSize(OdGsDCRect& outputRect) const
    {
      if (trHas())
        trCall()->getSize(outputRect);
    }
    void getSize(OdGsDCRectDouble& outputRect) const
    {
      if (trHas())
        trCall()->getSize(outputRect);
    }

    void onRealizeForegroundPalette()
    {
      if (trHas())
        trCall()->onRealizeForegroundPalette();
    }
    void onRealizeBackgroundPalette()
    {
      if (trHas())
        trCall()->onRealizeBackgroundPalette();
    }
    void onDisplayChange(int bitsPerPixel, int xPixels, int yPixels)
    {
      if (trHas())
        trCall()->onDisplayChange(bitsPerPixel, xPixels, yPixels);
    }

    OdGsViewPtr createView(const OdGsClientViewInfo* pViewInfo = 0, bool enableLayerVisibilityPerView = false)
    {
      if (trHas())
        return trCall()->createView(pViewInfo, enableLayerVisibilityPerView);
      return OdGsViewPtr();
    }
    void addView(OdGsView* pView)
    {
      if (trHas())
        trCall()->addView(pView);
    }

    OdGsModelPtr createModel()
    {
      if (trHas())
        return trCall()->createModel();
      return OdGsModelPtr();
    }
    bool isModelCompatible(OdGsModel* pModel) const
    {
      if (trHas())
        return trCall()->isModelCompatible(pModel);
      return false;
    }

    bool saveDeviceState(OdGsFiler *pFiler) const
    {
      if (trHas())
        return trCall()->saveDeviceState(pFiler);
      return false;
    }
    bool loadDeviceState(OdGsFiler *pFiler)
    {
      if (trHas())
        return trCall()->loadDeviceState(pFiler);
      return false;
    }

    void insertView(int viewIndex, OdGsView* pView)
    {
      if (trHas())
        trCall()->insertView(viewIndex, pView);
    }
    bool eraseView(OdGsView* pView)
    {
      if (trHas())
        return trCall()->eraseView(pView);
      return false;
    }
    bool eraseView(int viewIndex)
    {
      if (trHas())
        return trCall()->eraseView(viewIndex);
      return false;
    }
    void eraseAllViews()
    {
      if (trHas())
        trCall()->eraseAllViews();
    }
    int numViews() const
    {
      if (trHas())
        return trCall()->numViews();
      return 0;
    }
    OdGsView* viewAt(int viewIndex)
    {
      if (trHas())
        return trCall()->viewAt(viewIndex);
      return NULL;
    }

    bool setBackgroundColor(ODCOLORREF backgroundColor)
    {
      if (trHas())
        return trCall()->setBackgroundColor(backgroundColor);
      return false;
    }
    ODCOLORREF getBackgroundColor()
    {
      if (trHas())
        return trCall()->getBackgroundColor();
      return ODRGB(0, 0, 0);
    }

    void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors)
    {
      if (trHas())
        trCall()->setLogicalPalette(logicalPalette, numColors);
    }
    const ODCOLORREF* getLogicalPalette(int &numColors) const
    {
      if (trHas())
        return trCall()->getLogicalPalette(numColors);
      return NULL;
    }

    void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
    {
      if (trHas())
        trCall()->getSnapShot(pImage, region);
    }

    OdGsUpdateManagerPtr getUpdateManager( bool createIfNotExist )
    {
      if( trHas())
      {
        return trCall()->getUpdateManager( createIfNotExist );
      }
      OdGsUpdateManagerPtr res;
      return res;
    }

    void setUpdateManager( OdGsUpdateManagerPtr pManager )
    {
      if( trHas() )
      {
        trCall()->setUpdateManager( pManager );
      }
    }
};

/** \details
    Minimal implementation for OdGsView class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsViewInterface = OdGsView, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsView, GsViewInterface> >
class OdGsViewWrapperMinimalImpl : public OdGsViewWrapper<GsViewInterface, RedirectionBase>
{
  protected:
    OdGsDevice *m_pDevice;
    OdGiContext *m_pContext;
    double m_lwdDcScale;
    OdGePoint2d m_llVpt, m_urVpt;
    OdGsDCRectDouble m_dcRect;
    OdIntArray m_nrcClipCounts;
    OdGePoint2dArray m_nrcClipPoints;
    ODCOLORREF m_borderColor;
    int m_borderWidth;
    bool m_bBorderVisible;
    OdGePoint3d m_position, m_target;
    OdGeVector3d m_upVector;
    double m_fieldsWidth, m_fieldsHeight;
    OdGsView::Projection m_projection;
    double m_lensLength;
    bool m_bFrontClip, m_bBackClip;
    double m_frontClip, m_backClip;
    OdGsView::RenderMode m_renderMode;
    bool m_bHidden;
    double m_ltpMult1, m_ltpMult2;
    OdGsClientViewInfo m_viewInfo;
    OdDbStub *m_bkgnd, *m_visualStyle;
  public:
    OdGsViewWrapperMinimalImpl(OdGsView *pRedirectionView = NULL)
      : OdGsViewWrapper<GsViewInterface, RedirectionBase>(pRedirectionView)
      , m_pDevice(NULL)
      , m_pContext(NULL)
      , m_lwdDcScale(0.0)
      , m_borderColor(0)
      , m_borderWidth(1)
      , m_bBorderVisible(false)
      , m_target(OdGePoint3d::kOrigin + OdGeVector3d::kZAxis)
      , m_upVector(OdGeVector3d::kYAxis)
      , m_fieldsWidth(1.0)
      , m_fieldsHeight(1.0)
      , m_projection(OdGsView::kParallel)
      , m_lensLength(50.0)
      , m_bFrontClip(false)
      , m_bBackClip(false)
      , m_frontClip(0.0)
      , m_backClip(0.0)
      , m_renderMode(OdGsView::k2DOptimized)
      , m_bHidden(false)
      , m_ltpMult1(1.0)
      , m_ltpMult2(1.0)
      , m_bkgnd(NULL)
      , m_visualStyle(NULL)
    {}

    // OdGsView wrappers

    OdGsDevice* device() const { return m_pDevice; }
    void setDevice(OdGsDevice *pDevice) { m_pDevice = pDevice; }

    OdGiContext* userGiContext() const { return m_pContext; }
    void setUserGiContext(OdGiContext* pUserGiContext) { m_pContext = pUserGiContext; }

    double lineweightToDcScale() const { return m_lwdDcScale; }
    void setLineweightToDcScale(double scale) { m_lwdDcScale = scale; }

    void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight)
    { m_llVpt = lowerLeft; m_urVpt = upperRight; }
    void setViewport(const OdGsDCRect& screenRect) { m_dcRect = OdGsDCRectDouble(screenRect); }
    void setViewport(const OdGsDCRectDouble& screenRect) { m_dcRect = screenRect; }
    void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const
    { lowerLeft = m_llVpt; upperRight = m_urVpt; }
    void getViewport(OdGsDCRect& screenRect) const { screenRect = m_dcRect.round(); }
    void getViewport(OdGsDCRectDouble& screenRect) const { screenRect = m_dcRect; }

    void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices)
    {
      m_nrcClipCounts.resize(numContours);
      int numVertexes = 0;
      for (int numContour = 0; numContour < numContours; numContour++)
        numVertexes += numVertices[numContour], m_nrcClipCounts[numContour] = numVertices[numContour];
      m_nrcClipPoints.resize(numVertexes);
      for (int numVertex = 0; numVertex < numVertexes; numVertex++)
        m_nrcClipPoints[numVertex].set(vertices[numVertex].x, vertices[numVertex].y);
    }
    void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices)
    {
      m_nrcClipCounts.resize(numContours);
      int numVertexes = 0;
      for (int numContour = 0; numContour < numContours; numContour++)
        numVertexes += numVertices[numContour], m_nrcClipCounts[numContour] = numVertices[numContour];
      m_nrcClipPoints.resize(numVertexes);
      for (int numVertex = 0; numVertex < numVertexes; numVertex++)
        m_nrcClipPoints[numVertex] = vertices[numVertex];
    }
    void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& vertices) const
    {
      counts = m_nrcClipCounts;
      const OdUInt32 nPoints = m_nrcClipPoints.size();
      vertices.resize(nPoints);
      OdGsDCPoint *pPt = vertices.asArrayPtr();
      for (OdUInt32 nPt = 0; nPt < nPoints; nPt++)
        pPt[nPt].x = (long)m_nrcClipPoints[nPt].x, pPt[nPt].y = (long)m_nrcClipPoints[nPt].y;
    }
    void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const
    { counts = m_nrcClipCounts; vertices = m_nrcClipPoints; }

    void setViewportBorderProperties(ODCOLORREF color, int width)
    { m_borderColor = color; m_borderWidth = width; }
    void getViewportBorderProperties(ODCOLORREF& color, int& width) const
    { color = m_borderColor; width = m_borderWidth; }
    void setViewportBorderVisibility(bool visible) { m_bBorderVisible = visible; }
    bool isViewportBorderVisible() const { return m_bBorderVisible; }

    void setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                 double fieldWidth, double fieldHeight, OdGsView::Projection projectionType = OdGsView::kParallel)
    {
      m_position = position; m_target = target; m_upVector = upVector;
      m_fieldsWidth = fieldWidth; m_fieldsHeight = fieldHeight; m_projection = projectionType;
    }
    OdGePoint3d position() const { return m_position; }
    OdGePoint3d target() const { return m_target; }
    OdGeVector3d upVector() const { return m_upVector; }
    double lensLength() const { return m_lensLength; }
    void setLensLength(double lensLength) { m_lensLength = lensLength; }
    bool isPerspective() const { return m_projection == OdGsView::kPerspective; }
    double fieldWidth() const { return m_fieldsWidth; }
    double fieldHeight() const { return m_fieldsHeight; }

    void setEnableFrontClip(bool enable) { m_bFrontClip = enable; }
    bool isFrontClipped() const { return m_bFrontClip; }
    void setFrontClip(double frontClip) { m_frontClip = frontClip; }
    double frontClip() const { return m_frontClip; }
    void setEnableBackClip(bool enable) { m_bBackClip = enable; }
    bool isBackClipped() const { return m_bBackClip; }
    void setBackClip(double backClip) { m_backClip = backClip; }
    double backClip() const { return m_backClip; }

    void setMode(OdGsView::RenderMode mode) { m_renderMode = mode; }
    OdGsView::RenderMode mode() const { return m_renderMode; }

    void hide() { m_bHidden = true; }
    void show() { m_bHidden = false; }
    bool isVisible() { return !m_bHidden; }

    void setLinetypeScaleMultiplier(double linetypeScaleMultiplier) { m_ltpMult1 = linetypeScaleMultiplier; }
    double linetypeScaleMultiplier() const { return m_ltpMult1; }
    void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier) { m_ltpMult2 = linetypeAlternateScaleMultiplier; }
    double linetypeAlternateScaleMultiplier() const { return m_ltpMult2; }

    void clientViewInfo(OdGsClientViewInfo& clientViewInfo) const { clientViewInfo = m_viewInfo; }
    void setViewInfo(const OdGsClientViewInfo& clientViewInfo) { m_viewInfo = clientViewInfo; }

    void setBackground(OdDbStub *backgroundId) { m_bkgnd = backgroundId; }
    OdDbStub *background() const { return m_bkgnd; }

    void setVisualStyle(OdDbStub *visualStyleId) { m_visualStyle = visualStyleId; }
    OdDbStub *visualStyle() const { return m_visualStyle; }
};

/** \details
    Base template for OdGsDevice class wrappers.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <typename GsDeviceInterface = OdGsDevice, typename RedirectionBase = OdGsDefaultRedirectionBase<OdGsDevice, GsDeviceInterface> >
class OdGsDeviceWrapperMinimalImpl : public OdGsDeviceWrapper<GsDeviceInterface, RedirectionBase>
{
  protected:
    mutable OdSmartPtr<OdGiContext> m_pContext;
    ODCOLORREF m_backColor;
    OdArray<OdGsViewPtr> m_views;
    OdGsDCRectDouble m_size;
    OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > m_palette;
  public:
    OdGsDeviceWrapperMinimalImpl(OdGsDevice *pRedirectionDevice = NULL)
      : OdGsDeviceWrapper<GsDeviceInterface, RedirectionBase>(pRedirectionDevice)
      , m_backColor(0)
    {}

    // OdGsDevice wrappers

    OdGiContext* userGiContext() const { return m_pContext.get(); }
    void setUserGiContext(OdGiContext* pUserGiContext)
    { m_pContext = pUserGiContext;
      for (OdUInt32 nView = 0; nView < m_views.size(); nView++)
        m_views[nView]->setUserGiContext(pUserGiContext);
    }

    void onSize(const OdGsDCRect& outputRect) { m_size = OdGsDCRectDouble(outputRect); }
    void onSize(const OdGsDCRectDouble& outputRect) { m_size = outputRect; }
    void getSize(OdGsDCRect& outputRect) const { outputRect = m_size.round(); }
    void getSize(OdGsDCRectDouble& outputRect) const { outputRect = m_size; }

    OdGsViewPtr createView(const OdGsClientViewInfo* pViewInfo = 0, bool /*enableLayerVisibilityPerView*/ = false)
    { OdSmartPtr<OdGsViewWrapperMinimalImpl<> > pView = OdRxObjectImpl<OdGsViewWrapperMinimalImpl<> >::createObject();
      if (pViewInfo) pView->setViewInfo(*pViewInfo);
      pView->setUserGiContext(m_pContext.get());
      pView->setDevice(this); return pView;
    }

    void addView(OdGsView* pView) { m_views.push_back(pView); }
    void insertView(int viewIndex, OdGsView* pView) { m_views.insertAt(viewIndex, pView); }
    bool eraseView(OdGsView* pView) { m_views.remove(pView); return true; }
    bool eraseView(int viewIndex) { m_views.removeAt(viewIndex); return true; }
    void eraseAllViews() { m_views.clear(); }
    int numViews() const { return m_views.size(); }
    OdGsView* viewAt(int viewIndex) { return m_views.at(viewIndex); }

    bool setBackgroundColor(ODCOLORREF backgroundColor) { m_backColor = backgroundColor; return true; }
    ODCOLORREF getBackgroundColor() { return m_backColor; }

    void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors)
    {
      m_palette.setPhysicalLength(0);
      if (numColors)
        m_palette.insert(m_palette.end(), logicalPalette, logicalPalette + numColors);
    }
    const ODCOLORREF* getLogicalPalette(int &numColors) const
    {
      numColors = (int)m_palette.size();
      return m_palette.getPtr();
    }
};

#include "TD_PackPop.h"

#endif // __GSINTERFACEWRAPPERS_H_INCLUDED_
