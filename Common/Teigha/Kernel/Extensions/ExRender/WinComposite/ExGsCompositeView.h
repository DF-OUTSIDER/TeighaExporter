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

#ifndef EXGSCOMPOSITEVIEW
#define EXGSCOMPOSITEVIEW

#include "TD_PackPush.h"

#include "ExGsCompositeDefs.h"
#include "Gs/GsBaseInclude.h"

class ExGsCompositeDevice;

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExGsCompositeView
  : public OdGsBaseVectorizeViewDef
  , protected ExGsCompositeSyncFlags
{
  protected:
    friend class ExGsCompositeDevice;
    enum SyncFlags
    {
      kSyncInvalidate   = (1 << 0),  // invalidate called
      kSyncGiCtx        = (1 << 1),  // gi context changed
      kSyncViewport     = (1 << 2),  // viewport region changed
      kSyncViewportClip = (1 << 3),  // viewport clip region modified
      kSyncBorder       = (1 << 4),  // viewport border changed
      kSyncViewParam    = (1 << 5),  // view parameters changed (transform/lens length/clip)
      kSyncGraph        = (1 << 6),  // scene graph modified (added/removed drawables)
      kSyncVisibility   = (1 << 7),  // hide or show called
      kSyncLayer        = (1 << 8),  // frozen layers list modified
      kSyncLinetype     = (1 << 9),  // linetype settings changed
      kSyncLineweight   = (1 << 10), // lineweight settings changed
      kSyncLwdEnum      = (1 << 11), // lineweight enum changed
      kSyncClearColor   = (1 << 12), // clear color changed
      kSyncBackground   = (1 << 13), // background changed
      kSyncVisualStyle  = (1 << 14), // visual style changed
      kSyncRenderMode   = (1 << 15), // render mode was changed
      kSyncClip3d       = (1 << 16)  // viewport 3d clipping
    };
    enum CompositionFlags
    {
      kCompViewAttached = (1 << 0) // real device view is attached
    };
    OdUInt32 m_compositeFlags;
    OD_GS_COMPOSITE_PROP(kCompViewAttached, isCompositeViewAttached, setCompositeViewAttached);
    // Real device
    OdGsDevicePtr            m_pRealDevice; // smart pointer to keep it alive
    OdGsModule              *m_pCompModule;
    OdGsBaseVectorizeDevice *m_pCompDevice;
    OdGsBaseVectorizeView   *m_pCompView;
    OdUInt32                 m_factoryFlags;
  public:
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeViewDef);

    ExGsCompositeView();
    ~ExGsCompositeView();

    ExGsCompositeDevice* device();

    OdGsModule *compositeModule() const
    {
      return m_pCompModule;
    }
    OdGsBaseVectorizeDevice *compositeDevice() const
    {
      return m_pCompDevice;
    }
    OdGsBaseVectorizeView *compositeView() const
    {
      return m_pCompView;
    }
    OdUInt32 moduleFactoryFlags() const
    {
      return m_factoryFlags;
    }

    void detachCompositeView();
    void attachCompositeView();

    void detachRedirectionView();
    void attachRedirectionView(OdGsDevice *pDevice, OdUInt32 flags);

    void syncMe() const;

    void invalidateCachedViewportMaterials();
    void invalidateAllModelCaches();

    // GsView overrides

    virtual void setUserGiContext(OdGiContext* pUserGiContext);

    virtual void setLineweightToDcScale(double scale);
    virtual void setLineweightEnum(int numLineweights, const OdUInt8* lineweights);

    virtual void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight);
    virtual void setViewport(const OdGsDCRect& screenRect);
    virtual void setViewport(const OdGsDCRectDouble& screenRect);

    virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices);
    virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices);

    virtual void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL);

    virtual void setViewportBorderProperties(ODCOLORREF color, int width);
    virtual void setViewportBorderVisibility(bool visible);

    virtual void setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                         double fieldWidth, double fieldHeight, Projection projectionType = kParallel);
    virtual void setLensLength(double lensLength);

    virtual void setEnableFrontClip(bool enable);
    virtual void setFrontClip(double frontClip);
    virtual void setEnableBackClip(bool enable);
    virtual void setBackClip(double backClip);

    virtual void setMode(RenderMode mode);

    virtual bool add(OdGiDrawable* pSceneGraph, OdGsModel* pModel);
    virtual bool erase(OdGiDrawable* sceneGraph);
    virtual void eraseAll();

    virtual void invalidate();
    virtual void invalidate(const OdGsDCRect &screenRect);

    virtual void flush();

    virtual void hide();
    virtual void show();

    virtual void freezeLayer(OdDbStub* layerID);
    virtual void thawLayer(OdDbStub* layerID);
    virtual void clearFrozenLayers();

    virtual void select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, SelectionMode mode = kCrossing);
    virtual bool viewExtents(OdGeBoundBlock3d& extents) const;

    virtual void dolly(const OdGeVector3d& dollyVector);
    virtual void dolly(double xDolly, double yDolly, double zDolly);
    virtual void roll(double rollAngle);
    virtual void orbit(double xOrbit, double yOrbit);
    virtual void zoom(double zoomFactor);
    virtual void pan(double xPan, double yPan);
    virtual void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt);
    virtual void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight);

    virtual bool exceededBounds();

    virtual void initLights(OdRxIterator* pLightsIterator);

    virtual void setLinetypeScaleMultiplier(double linetypeScaleMultiplier);
    virtual void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier);

    virtual void setClearColor(ClearColor color);

    virtual void setBackground(OdDbStub *backgroundId);

    virtual void setVisualStyle(OdDbStub *visualStyleId);
    virtual void setVisualStyle(const OdGiVisualStyle &visualStyle);

    virtual void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

    // GsView overrides which controls output geometry generation/rendering
    virtual void propagateLayersChanges();
    virtual void propagateInvalidVpFlag();
    virtual void updateViewProps();
    virtual void beginViewVectorization();
    virtual void endViewVectorization();
    virtual void loadViewport();
    virtual void update();
    virtual void updateExtents(bool buildCache);
    virtual void display(bool update);
    virtual void drawViewportFrame();

    // GsBaseVectorizeView overrides
    bool isLocalViewportIdCompatible(const OdGsViewImpl *pView) const;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExGsProtectedViewParamsAccessor
  : public OdGsBaseVectorizeView
{
  friend class ExGsCompositeDevice;
  friend class ExGsCompositeView;
  public:
    static ExGsProtectedViewParamsAccessor *castEx(OdGsBaseVectorizeView *pView)
    {
      return reinterpret_cast<ExGsProtectedViewParamsAccessor*>(pView);
    }
    static const ExGsProtectedViewParamsAccessor *castEx(const OdGsBaseVectorizeView *pView)
    {
      return reinterpret_cast<const ExGsProtectedViewParamsAccessor*>(pView);
    }

    int &nCachedDrawables();
};

#include "TD_PackPop.h"

#endif // EXGSCOMPOSITEVIEW
