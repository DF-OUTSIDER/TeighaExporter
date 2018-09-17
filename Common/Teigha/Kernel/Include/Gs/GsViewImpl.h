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

#ifndef ODGSVIEWIMPL_INC
#define ODGSVIEWIMPL_INC

#include "TD_PackPush.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseModule.h"
#include "Gs/GsDCRectArray.h"
#include "DbStubPtrArray.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"
#include "UInt8Array.h"
#include "SharedPtr.h"
#include "Gs/GsDrawableHolder.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/GsProperties.h"
#include "Gs/GsCullingVolume.h"
#include "Gs/GsViewLocalId.h"
#include "Gs/GsOverlayDefs.h"
#include "Gi/GiAbstractClipBoundary.h"
#include "OdMutex.h"

class OdGsBaseVectorizeDevice;
class OdGeBoundBlock3d;
class OdGiPathNode;

/** \details
    This class implements Viewport objects in GUI display windows.
    
    Corresponding C++ library: Gs
    
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsViewImpl : public OdGsView
{
public:
  ODRX_DECLARE_MEMBERS(OdGsViewImpl);

  OdGsViewImpl();
  ~OdGsViewImpl();

  // OdGsView methods

  /** \details
    Initializes this Viewport object.
    
    \param pDevice [in]  Pointer to the OdGsBaseVectorizeDevice associated with this Viewport object.
    \param pViewInfo [in]  Pointer to the Client View Information for this Viewport object.
    \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.
  */
  virtual void init(OdGsBaseVectorizeDevice* pDevice, 
    const OdGsClientViewInfo* pViewInfo = 0, 
    bool enableLayerVisibilityPerView = false);

  virtual OdGsDevice* device() const;

  virtual void clientViewInfo(OdGsClientViewInfo& viewInfo) const;

  virtual RenderMode mode() const;
  bool setModeOverride(RenderMode mode);
  virtual void setMode(RenderMode mode);

  virtual void freezeLayer(OdDbStub* layerID);
  virtual void thawLayer(OdDbStub* layerID);
  virtual void clearFrozenLayers();

  virtual void setViewportBorderProperties(ODCOLORREF color, int width);
  virtual void getViewportBorderProperties(ODCOLORREF& color, int& width) const;
  virtual void setViewportBorderVisibility(bool visible);
  virtual bool isViewportBorderVisible() const;

  virtual void setEnableFrontClip(bool enable);
  virtual bool isFrontClipped() const;
  virtual void setFrontClip(double distance);
  virtual double frontClip() const;

  virtual void setEnableBackClip(bool enable);
  virtual bool isBackClipped() const;
  virtual void setBackClip(double distance);
  virtual double backClip() const;

  virtual OdGePoint3d position() const;
  virtual OdGePoint3d target() const;
  virtual OdGeVector3d upVector() const;
  virtual double fieldWidth() const;
  virtual double fieldHeight() const;
  /** \details
    Returns the width to height ratio for this view object.
  */
  double windowAspect() const;

  virtual double lensLength() const;
  virtual void setLensLength(double lensLength);

  virtual bool isPerspective() const;

  virtual void beginInteractivity(double frameRateInHz);
  virtual bool isInInteractivity() const;
  virtual double interactivityFrameRate() const;
  virtual void endInteractivity();
  virtual void flush();

  virtual void hide();
  virtual void show();
  virtual bool isVisible();

  virtual OdGsViewPtr cloneView(bool cloneViewParameters = true, bool cloneGeometry = false);
  virtual bool exceededBounds();
  virtual void enableStereo(bool enabled);
  virtual bool isStereoEnabled() const;
  virtual void setStereoParameters(double magnitude, double parallax);
  virtual void getStereoParameters(double& magnitude, double& parallax) const;
  virtual void initLights(OdRxIterator* pLightsIterator);
  virtual void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  virtual void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight);
  virtual void setViewport(const OdGsDCRect& screenRect);
  virtual void setViewport(const OdGsDCRectDouble& screenRec);

  virtual void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const;
  virtual void getViewport(OdGsDCRect& screenRect) const;
  virtual void getViewport(OdGsDCRectDouble& screenRec) const;

  virtual OdGeMatrix3d screenMatrix() const;
  virtual OdGeMatrix3d worldToDeviceMatrix() const;
  virtual OdGeMatrix3d objectToDeviceMatrix() const;
  virtual OdGeMatrix3d viewingMatrix() const;
  virtual OdGeMatrix3d projectionMatrix() const;

  virtual void viewParameters(OdGsView* pView) const;

  virtual double lineweightToDcScale() const;
  virtual void setLineweightToDcScale(double scale);
  virtual void setLineweightEnum(int numLineweights, const OdUInt8* lineweights);
  virtual void setLinetypeScaleMultiplier(double linetypeScaleMultiplier);
  virtual void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier);
  virtual double linetypeScaleMultiplier() const;

  virtual void dolly(const OdGeVector3d& dollyVector);
  virtual void dolly(double xDolly, double yDolly, double zDolly);
  virtual void roll(double rollAngle);
  virtual void orbit(double xOrbit, double yOrbit);
  virtual void zoom(double zoomFactor);
  virtual void pan(double xPan, double yPan);
  virtual void setView(const OdGePoint3d & position,
    const OdGePoint3d& target,
    const OdGeVector3d& upVector,
    double fieldWidth,
    double fieldHeight,
    Projection projection = kParallel);

  virtual void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt);
  virtual void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight);
  virtual bool pointInView(const OdGePoint3d &pt) const;
  virtual bool extentsInView(const OdGePoint3d &minPt, const OdGePoint3d &maxPt) const;

  virtual void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const;
  virtual void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& dcPts) const;
  virtual void setViewportClipRegion(int numContours, const int* numVertices, const OdGePoint2d* vertices);
  virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices);

  bool isDependentViewportView() const;
  bool isDependentGeometryView() const;
  bool isHelperView() const;

  const OdGsBaseModule *getGsModulePtr() const;

  const OdGiContextualColors *contextualColors() const;
  void setContextualColors(OdGiContextualColors *pColors);

  virtual bool pointInViewport(const OdGePoint2d& screenPoint) const;

  virtual void setClearColor(ClearColor color);
  virtual void invalidate();
  virtual void invalidate(const OdGsDCRect &rect);
  virtual void invalidateCachedViewportGeometry();
  /** \details
    Evokes the specified OdGsSelectionReactor object for the specified polygon in this VectorizeView object.
    \param points [in]  Array of points.
    \param numPoints [in]  Number of points.
    \param pReactor [in]  Pointer to the reactor. 
  */
  virtual void select(const OdGsDCPoint* points, int numPoints, OdGsSelectionReactor* pReactor,
    SelectionMode mode = kCrossing);

  virtual bool add(OdGiDrawable* sceneGraph, OdGsModel* model);
  virtual int numRootDrawables() const;
  virtual OdDbStub* rootDrawableIdAt(int i, OdGsModelPtr* pModelReturn = 0) const;
  virtual OdGiDrawablePtr rootDrawableAt(int i, OdGsModelPtr* pModelReturn = 0) const;
  virtual bool erase(OdGiDrawable* sceneGraph);
  virtual void eraseAll();

  virtual OdGsModel *getModel(const OdGiDrawable *pDrawable) const;
  virtual OdGsModelArray getModelList() const;

  virtual bool isValid() const;
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt,
    OdGePoint2d& pixelArea, bool includePerspective = true) const;

  virtual void setBackground(OdDbStub *backgroundId);
  virtual OdDbStub *background() const;
  virtual void setVisualStyle(OdDbStub *visualStyleId);
  virtual OdDbStub *visualStyle() const;
  virtual void setVisualStyle(const OdGiVisualStyle &visualStyle);
  virtual bool visualStyle(OdGiVisualStyle &vs) const;
  virtual void enableDefaultLighting(bool bEnable, DefaultLightingType lightType = kTwoLights);
  bool defaultLightingEnabled() const;
  DefaultLightingType defaultLightingType() const;

  // OdGsViewImpl methods

  /** \details
    Returns the specified OdGsView pointer 
    as a pointer to an OdGsViewImpl object.
    \param pView [in]  Pointer to an OdGsView object.
  */
  static OdGsViewImpl* safeCast(OdGsView* pView);
  static const OdGsViewImpl* safeCast(const OdGsView* pView);

  OdUInt32 viewportId() const { return m_giViewportId; }
  bool isValidViewportId(const OdUInt32 vpId) const;

  //NS TODO: rename to viewportObjectId()
  OdDbStub* getViewportObjectId() const { return m_viewInfo.viewportObjectId; }

  /** \details
    Returns the OdGsBaseVectorizeDevice associated with this Viewport object.
  */
  OdGsBaseVectorizeDevice* baseDevice() { return m_pDevice; }
  const OdGsBaseVectorizeDevice* baseDevice() const { return m_pDevice; }

  OdDbStub* annotationScaleId() const;
  OdIntPtr drawablesFilterFunction() const;

  /** \details
    Returns the eye coordinate system extents of this view object.
    
    \param extents [out]  Receives the extents.

    \returns
    Returns true if and only if the extents are defined.
  */
  virtual bool viewExtents(OdGeBoundBlock3d& extents) const = 0;

  // Own methods
  virtual void select(const OdGePoint2d* aPtDc, int numPoints, OdGsSelectionReactor* pReactor,
    SelectionMode mode = kCrossing) = 0;

  bool isSnapping() const;
  void setSnapping(bool val);
  bool isRegenOnDrawForbidden() const;
  void setRegenOnDrawForbidden(bool val);
  bool isAccumulateVpChanges() const;
  virtual void clearLinetypeCache() = 0;
  double linetypeAlternateScaleMultiplier() const;

  // Legacy plot modes (#12611)
  // Legacy Wireframe
  virtual bool isSupportLegacyWireframeMode() const;
  virtual bool isLegacyWireframeMode() const;
  virtual void setLegacyWireframeMode(bool bSet);
  // Legacy Hidden
  virtual bool isSupportLegacyHiddenMode() const;
  virtual bool isLegacyHiddenMode() const;
  virtual void setLegacyHiddenMode(bool bSet);
  // Returns true in case if current view supports plot styles rendering
  bool isSupportPlotStyles() const;
  // Transparencies plotting
  virtual bool isPlotTransparency() const;
  virtual void setPlotTransparency(bool bSet);

  /** \details
    Returns the frozen layers for this view object.
    
  
    \remarks
    Returns true if and only if there are any frozen layers.    
  */
  OdUInt32 frozenLayers() const { return m_frozenLayers.size(); }
  
  /** \details
    Returns the frozen layers for this view object.
    
    \param frozenLayers [out]  Receives an array of frozen layer Object IDs.
    
    \remarks
    If called without the frozenLayers argument, returns true if and only if there
    are any frozen layers.    
  */
  void frozenLayers(OdDbStubPtrArray& frozenLayers) const { frozenLayers = m_frozenLayers; }
  bool isLayerVisible(OdDbStub* layerId) const;

  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  int lineweightToPixels(OdDb::LineWeight lineweight) const;
  
  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  double lineweightToPixels(double lineweight) const;

  /** \details
    Introduces a DCS rectangle into this vectorization context.
    \param lowerLeft [in]  Lower-left corner.
    \param upperRight [in]  Upper-right corner.
  */
  void screenRect(OdGsDCPoint &lowerLeft, OdGsDCPoint &upperRight) const;
  
  /** \details
    Introduces a DCS rectangle into this vectorization context.
    \param lowerLeft [in]  Lower-left corner.
    \param upperRight [in]  Upper-right corner.
  */
  void screenRect(OdGePoint2d &lowerLeft, OdGePoint2d &upperRight) const;
  void screenRectNorm(OdGsDCRect& normalizedRect) const;
  void viewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const;
  bool doPerspectivePt(OdGePoint3d& point) const;
  bool doInversePerspectivePt(OdGePoint3d& point) const;

  const OdGeMatrix3d& eyeToWorldMatrix() const;
  const OdGeMatrix3d& worldToEyeMatrix() const;
  OdGeMatrix3d eyeToScreenMatrix(OdGsOverlayId nOverlay = kGsMainOverlay) const;
  OdGeMatrix3d worldToDeviceMatrix(OdGsOverlayId nOverlay) const;
  OdGeMatrix3d objectToDeviceMatrix(OdGsOverlayId nOverlay) const;
  enum ProjectionMatrixFlags
  {
    kProjectionIncludeDept        = (1 << 0),
    kProjectionIncludeClip        = (1 << 1),
    kProjectionIncludePerspective = (1 << 2),
    kProjectionIncludeRotation    = (1 << 3),
    kProjectionIncludeAll         = 0x0F,
    kProjectionIncludeAllNoDept   = kProjectionIncludeAll & ~kProjectionIncludeDept
  };
  OdGeMatrix3d projectionMatrix(OdUInt32 nIncludes, OdGsOverlayId nOverlay = kGsMainOverlay) const;

  ClearColor clearColor() const;

  virtual void invalidate(const OdGeExtents3d& worldExt, const OdGsModel *pModel, OdDb::LineWeight extendByLineweight = OdDb::kLnWt000);
  void invalidate(const OdGsUpdateExtents& extents, const OdGsModel *pModel);
  void invalidateRegion(const OdGiPathNode& path);

  OdGsDCRectArray invalidRects(OdGsOverlayId nOverlay = kGsMainOverlay) const;
  /** \details
    Returns true if and only if this view object is not off-screen.
  */
  bool isViewportOnScreen() const;

  bool isCompletelyVisible() const;
  void setCompletelyVisible(bool val);

  // Since layout view can be rotated to 90, 180 or 270 (for printing)
  // views linked to viewports of this layout must be rotated accordingly.
  // To determine viewport's rotation angle by it's corners coordinates 
  // it's necessary to know if overall layout's window coordinates inverted. 
  //
  // For example overall layout's output rectangle can be (0,0)-(100,100)
  // or (0,100)-(100,0)
  // This means invertedX == invertedY == false in the first case and
  // invertedX = false, invertedY == true in the second.
  // Vieport with coordinates (10,10)-(90,90) inside this layout will be 
  // considered unrotated in the first case or rotated to 270 degrees 
  // in the second case.
  void setInversion(bool invertedX, bool invertedY);

  OdGiRegenType getRegenType() const { return m_regenerationType; }

  /** \details
    Returns the WCS (ECS) focal length of the lens for this view object.
  */
  double focalLength() const;

  const OdGeVector3d& eyeVector() const { return m_eyeVector; }
  const OdGeVector3d& xVector() const { return m_xVector; }
  const OdGeVector3d& getUpVector() const { return m_upVector; }

  double getLinetypeScaleMultiplier() const;
  double getLinetypeGenerationCriteria() const;

  virtual double getDeviation(const OdGiDeviationType deviationType,
      const OdGePoint3d& pointOnCurve, bool bRecalculate = false) const;
  void initDeviation();

  /** \details
    Returns the number of cached drawables for this view object.
  */
  int cachedDrawables() const { return m_nCachedDrawables!=0; }

#ifdef ODGS_LOCAL_IDS
  // Local viewport ID section
  OdUInt32 localViewportId(const OdGsBaseModel *pModel) const
  {
    return m_localId.localViewportId(pModel);
  }
  virtual bool isLocalViewportIdCompatible(const OdGsViewImpl *pView) const;
  OdGsViewLocalId &getViewLocalId()
  {
    return m_localId;
  }
#endif
  virtual bool isViewRegenerated() const;
protected:
  /** \details
    Returns the minimum and maximum z-values for this view object.
    \param zNear [out]  Receives the minimum z-value.
    \param zFar [out]  Receives the maximum z-value.
    \param nOverlay [in]  Overlay buffer index.
    \returns
    Returns true if and only if this view object has z-values associated with it.
  */
  virtual bool sceneDept(double& zNear, double& zFar, OdGsOverlayId nOverlay) const;
  // This is old version of sceneDept overrideable method, which is still provided for compatibility with
  // old vectorizers. Don't override it in new vectorizers, use new sceneDept method version even if
  // your vectorizer doesn't support overlay buffers. This method will be removed in one of next Teigha versions.
  virtual bool sceneDept(double& zNear, double& zFar) const;

  /** \details
    Returns the minimum and maximum depth of this view object.
    \param pMinDeptSupported [out]  Receives the minimum depth supported.
    \param pMaxDeptSupported [out]  Receives the maximum depth supported.
    \returns
    Returns true if and only if this view object supports depth.    

    \remarks
    The default implementation of this function does nothing but return false.
  */
  virtual bool usesDept(double* pMinDeptSupported = 0, double* pMaxDeptSupported = 0) const;
protected:
  /** \details
    Notification function called whenever the World to Eye transformation matrix has been changed.
  */
  void onWorldToEyeChanged();

  /** \details
    Notification function called whenever this ViewImpl object is about to be released.
  */
  void onFinalRelease();

  virtual void updateViewProps(bool bFullUpdate = true);

  inline bool isCheckValid() const;
  inline void setCheckValid(bool bValue) const;
  inline bool isInvalid() const;
  inline void setInvalid(bool bValue) const;
  inline void setPerspectiveEnabled(bool bValue) const;

public:
  inline bool isNonRectClipped() const;
  inline bool hasLweights() const { return !m_lweights.empty(); }
  inline const OdUInt8Array &getLweightsEnum() const { return m_lweights; }

  const OdGePoint2d& dcScreenMin() const { return m_dcScreenMin; }
  const OdGePoint2d& dcScreenMax() const { return m_dcScreenMax; }
  const OdGePoint2d& dcLowerLeft() const { return m_dcLowerLeft; }
  const OdGePoint2d& dcUpperRight() const { return m_dcUpperRight; }

  /** \details
    Returns the perspective transformation matrix for this view object.
  */
  OdGeMatrix3d perspectiveMatrix() const;

  /** \details
    Utility function to recompute lens length into field of view angle.
    \param lensLength [in]  Lens length in millimeters.
    \returns
    Returns field of view angle in radians.
  */
  static double lensLengthToFOV(double lensLength);
  /** \details
    Utility function to recompute field of view angle into lens length.
    \param fovAngle [in]  Field of view angle in radians.
    \returns
    Returns lens length in millimeters.
    \remarks
    Input angle must be in range between 4 and 179 degrees inclusively.
  */
  static double lensLengthFromFOV(double fovAngle);

  /** \param mask [in]  Mask.
  */
  void invalidateCachedViewportGeometry(OdUInt32 mask);

  void registerOverlay(const OdGsModel *pModel);
  void unregisterOverlay(const OdGsModel *pModel);

  /** \details
    Returns partial update rectange extension in pixels.
    \remarks
    This extension required for devices which support visual styles to avoid artifacts
    caused by silhouettes or line extensions.
  */
  virtual int partialUpdateExtentsEnlargement() const;
protected:
  /** \details
    Returns the OdGiDrawable associated with the specified DrawableHolder object.
    \param drawableHolder [in]  Drawable holder.
  */
  OdGiDrawablePtr drawableAt(DrawableHolder& drawableHolder);
  OdGsNode* getRootNode(DrawableHolder& holder);
  virtual void propagateInvalidVpFlag();
  virtual void propagateLayersChanges();

public:
  double calcDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const;

  // Viewport properties (backgrounds, render settings, visual styles) management
  OdGsPropertiesPtr getViewportPropertiesRoot() const;
  bool updateViewportProperties(OdUInt32 incFlags);
  OdGsPropertiesPtr getViewportPropertiesForType(OdGsProperties::PropertiesType type) const;

  // Printing support methods

  const OdGeMatrix3d& rotationMatrix() const;
  short viewportRotation() const;
  inline bool isRotated() const;

  /** \details
   When viewport is not rotated (or rotated 180)

   +-----------------* m_dcScreenMax
   |                 |
   |                 |
   |                 | fieldHeight
   |                 | unrotatedFieldHeight
   |                 |
   |m_dcScreenMin    |
   *-----------------+
        fieldWidth
    unrotatedFieldWidth

   When viewport is rotated to 90 or 270:
  
   m_dcScreenMin
   *-----------------+
   |                 |
   |                 |
   |                 | fieldHeight
   |                 | unrotatedFieldWidth
   |                 |
   |                 |
   +-----------------* m_dcScreenMax
        fieldWidth
    unrotatedFieldHeight

    Returns fieldWidth and fieldHeight 
  */
  double unrotatedFieldHeight() const;
  double unrotatedFieldWidth() const;

  /** \details
    Returns actual field width variable (without inclusion if aspect ratio).
  */
  double actualFieldWidth() const;
  /** \details
    Returns actual field height variable (without inclusion if aspect ratio).
  */
  double actualFieldHeight() const;

  // Primitives culling routines

  /** \details
    Initialize culling volume, associated with current view.

    \remarks
    Default implementation initializes culling volume for overall view, and transforms overall view culling volume
    for all viewports inside overall view space (only if viewports has orthogonal projection). 
    This method can be overrided into user device for adjust described behaviour.
    If this method overrided as empty function (not create culling volume inside) - all culling functionality
    will be disabled.

    \sa
    *  isCullingVolumeInitialized
    *  cullingVolumeIntersectWithOpt
    *  cullingVolumeIntersectWith
  */
  virtual void initCullingVolume();
  /** \details
    Sets culling volume associated with current view.
    \param newVol [in]  Smart pointer to new culling volume object.
    \sa
    *  cullingVolume
    *  initCullingVolume
  */
  void setCullingVolume(OdGsCullingVolumePtr newVol);
  /** \details
    Returns culling volume associated with current view.
    \sa
    *  initCullingVolume
    *  setCullingVolume
  */
  OdGsCullingVolumePtr cullingVolume() const;
  /** \details
    Returns true if and only if culling volume associated with current view.
    \remarks
    Culling volume consider associated with current view if pointer to culling volume is not null.
    \sa
    *  initCullingVolume
    *  setCullingVolume
    *  cullingVolume
  */
  virtual bool isCullingVolumeInitialized() const;
  /** \details
    Optimized intersection test between culling volume associated with current view and culling primitive.
    \returns
    Returns true if culling volume is not initialized.
    \param prim [in]  Input primitive for culling detection.
    \sa
    *  isCullingVolumeInitialized
    *  cullingVolumeIntersectWith
  */
  virtual bool cullingVolumeIntersectWithOpt(const OdGsCullingPrimitive &prim) const;
  /** \details
    Complete intersection test between culling volume associated with current view and culling primitive.

    \returns
    Returns kIntersectIn intersection status if culling volume is not initialized.
    \remarks
    Return value intersection statuses:
    <table>
    Name               Value    Description
    kIntersectNot      0        No intersection detected.
    kIntersectOk       1        Intersects with culling volume boundary.
    kIntersectIn       2        Culling volume primitive is completely inside culling volume boundary.
    </table>

    \param prim [in]  Input primitive for culling detection.

    \sa
    *  isCullingVolumeInitialized
    *  cullingVolumeIntersectWithOpt
  */
  virtual OdGsCullingVolume::IntersectionStatus cullingVolumeIntersectWith(const OdGsCullingPrimitive &prim) const;
  /** \details
    Applies transformation for culling volume associated with current view.
    \param xfm [in]  Transformation matrix.
    \sa
    *  initCullingVolume
  */
  virtual void cullingVolumeTransformBy(const OdGeMatrix3d &xfm);

  /** \details
    Defines the 3d clipping for this Viewport object.

    \param pBoundary [in]  3d clipping boundary.
    \param pClipInfo [in]  Optional 3d clipping boundary extended data.
  */
  virtual void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL);
  /** \details
    Returns the current 3d clipping for this Viewport object.

    \param ppClipInfo [out]  Optional 3d clipping boundary extended data.
  */
  virtual const OdGiClipBoundary *viewport3dClipping(const OdGiAbstractClipBoundary** ppClipInfo = NULL) const;
  /** \details
    Returns the current 3d clipping for this Viewport object as single object.
  */
  OdGiClipBoundaryWithAbstractData *viewport3dClippingObject();
  /** \details
    Returns true if and only if 3d clipping set for this Viewport object.
  */
  bool hasViewport3dClipping() const;

  virtual bool saveViewState(OdGsFiler *pFiler) const;
  virtual bool loadViewState(OdGsFiler *pFiler);
  virtual bool saveClientViewState(OdGsFiler *pFiler) const;
  virtual bool loadClientViewState(OdGsFiler *pFiler);

  virtual const OdDbStubPtrArray& getFrozenLayersDiff() const;

  virtual void collide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, const OdGsCollisionDetectionContext* pCtx = NULL ) = 0;
private:
  void setExtents(OdGsOverlayId nOverlay, const OdGeExtents3d& ext, bool bAdd);
  void getExtents(OdGsOverlayId nOverlay, OdGeExtents3d& ext) const;

private:
  enum EGsViewImplFlags
  {
    kFirstViewImplFlag       = 1,

    kInvalid                 = kFirstViewImplFlag << 0, // device surface entirely invalid
    kCheckValid              = kFirstViewImplFlag << 1, // device surface entirely valid
    kBorderVisible           = kFirstViewImplFlag << 2,
    kDoFrontClip             = kFirstViewImplFlag << 3,
    kDoBackClip              = kFirstViewImplFlag << 4,
    kPerspectiveEnabled      = kFirstViewImplFlag << 5,
    kHidden                  = kFirstViewImplFlag << 6, // view is hidden
    kTransparentClearColor   = kFirstViewImplFlag << 7,
    kDevBgClearColor         = kFirstViewImplFlag << 8,
    kEyeToWorldValid         = kFirstViewImplFlag << 9,
    kWorldToEyeValid         = kFirstViewImplFlag << 10,
    kSnapping                = kFirstViewImplFlag << 11,
    kForbidDrawableRegenDraw = kFirstViewImplFlag << 12,
    kDepViewInside           = kFirstViewImplFlag << 13,
    kLegacyWireframe         = kFirstViewImplFlag << 14,
    // there are two modes with hidden lines removed:
    // - normal, set by SHADEMODE command in CAD for display and
    //   represented by 'Hidden' visual style in shade plot viewport/layout props for printing, and
    // - legacy, set by HIDE command in CAD for display and
    //   according to 'OdDbViewport/OdDbPlotSettings::kHidden' shade plot for printing;
    // they both use 'kHiddenLine' render mode, but have some differences, e.g. in plot style usage;
    // this flag is to distinguish these two cases
    kLegacyHidden            = kFirstViewImplFlag << 15,
    kPlotTransparency        = kFirstViewImplFlag << 16,
    kAccumulateVpChanges     = kFirstViewImplFlag << 17,
    kDefaultLightingEnabled  = kFirstViewImplFlag << 18
  };

private:
  OdGsBaseModulePtr m_pModule;
  OdDbStubPtrArray m_lastFrozenLayers;
  OdDbStubPtrArray m_frozenLayersDiff;

  mutable OdUInt32 m_gsViewImplFlags;
  ODCOLORREF m_borderColor;
  int m_borderWeight;
  double m_frontClipDist;
  double m_backClipDist;
  bool m_screenInvertedX;
  bool m_screenInvertedY;
  OdGePoint2d m_dcScreenMin;
  OdGePoint2d m_dcScreenMax;
  OdGePoint2d m_dcLowerLeft;
  OdGePoint2d m_dcUpperRight;

  double m_fieldWidth;
  double m_fieldHeight;
  double m_lensLength;
  double m_eyeVecLength;

  OdGePoint3d m_position;
  OdGePoint3d m_target;
  OdGeVector3d m_upVector;
  OdGeVector3d m_eyeVector;
  OdGeVector3d m_xVector;
  mutable OdGeMatrix3d m_eyeToWorld;
  mutable OdGeMatrix3d m_worldToEye;

  OdIntArray m_nrcCounts;
  OdGePoint2dArray m_nrcPoints;
  double m_linetypeScaleMultiplier;
  double m_linetypeAlternateScaleMultiplier;
  double m_lineweightToDcScale;
  OdUInt8Array m_lweights;
  OdGsCullingVolumePtr m_pCullingVolume;
  OdSharedPtr<OdGiClipBoundaryWithAbstractData> m_p3dClip;

  double m_deviation[5];

  int m_nCachedDrawables;
  mutable OdMutex m_mtExtents;

  OdGsPropertiesPtr m_pProperties;
  OdDbStub *m_pBackgroundId;
  OdDbStub *m_pVisualStyleId;
  OdGiDrawablePtr m_pViewportTrans;
  DefaultLightingType m_defaultLightingType;

protected:
  struct GsViewOverlayData
  {
    enum OverlayFlags
    {
      kWorldToDeviceValid = (1 << 0),
      kSceneDeptInvalid   = (1 << 1)
    };
    mutable OdUInt32 m_nOverlayFlags;
    mutable OdGeMatrix3d m_worldToDeviceMatrix;
    OdGeExtents3d m_extents;
    GsViewOverlayData() : m_nOverlayFlags(0) {}
  };
  class GS_TOOLKIT_EXPORT GsViewOverlayDataContainer : public OdGsOverlayDataContainer<GsViewOverlayData>
  {
    public:
      void setWorldToDeviceInvalid();
      void setSceneDeptInvalid(OdGsOverlayId nOverlay, bool bSet) const;
  };
  GsViewOverlayDataContainer m_overlayData;
protected:
  OdGsBaseVectorizeDevice* m_pDevice;
  OdGsClientViewInfo m_viewInfo;
  OdUInt32 m_giViewportId;
#ifdef ODGS_LOCAL_IDS
  OdGsViewLocalId m_localId;
#endif
  OdGiRegenType m_regenerationType;
  // Reserved pointer for internal use (binary incompatible changes), do not delete.
  OdRxObjectPtr m_pGsReserved;

  // Made protected for access in OdGiViewport methods
  RenderMode m_renderMode;

  DrawableHolderArray m_drawables;

  OdDbStubPtrArray m_frozenLayers;

  bool m_bInInteractivityMode;
  double m_dInteractivityFrameRate;

  friend class GsViewImplHelper;
  friend class OdGiBaseVectorizerImpl;
  friend class OdGsBaseVectorizer;
  friend class OdGsBaseModule;
  friend class OdGsBaseVectorizeDevice;
};

inline bool OdGsViewImpl::isDependentViewportView() const
{
  return GETBIT(m_viewInfo.viewportFlags, OdGsClientViewInfo::kDependentViewport);
}

inline bool OdGsViewImpl::isDependentGeometryView() const
{
  return GETBIT(m_viewInfo.viewportFlags, OdGsClientViewInfo::kDependentGeometry);
}

inline bool OdGsViewImpl::isHelperView() const
{
  return GETBIT(m_viewInfo.viewportFlags, OdGsClientViewInfo::kHelperView);
}

inline const OdGsBaseModule *OdGsViewImpl::getGsModulePtr() const
{
  return m_pModule.get();
}

inline const OdGiContextualColors *OdGsViewImpl::contextualColors() const
{
  return m_viewInfo.contextColors.get();
}

inline void OdGsViewImpl::setContextualColors(OdGiContextualColors *pColors)
{
  m_viewInfo.contextColors = pColors;
  invalidate();
}

inline bool OdGsViewImpl::isSupportPlotStyles() const
{
  return (mode() == k2DOptimized) || isLegacyHiddenMode() || isLegacyWireframeMode();
}

inline bool OdGsViewImpl::isCheckValid() const
{
  return GETBIT(m_gsViewImplFlags, kCheckValid);
}

inline void OdGsViewImpl::setCheckValid(bool bValue) const
{
  SETBIT(m_gsViewImplFlags, kCheckValid, bValue);
}

inline bool OdGsViewImpl::isInvalid() const
{
  return GETBIT(m_gsViewImplFlags, kInvalid);
}

inline void OdGsViewImpl::setInvalid(bool bValue) const
{
  SETBIT(m_gsViewImplFlags, kInvalid, bValue);
}

inline void OdGsViewImpl::setPerspectiveEnabled(bool bValue) const
{
  SETBIT(m_gsViewImplFlags, kPerspectiveEnabled, bValue);
}

inline bool OdGsViewImpl::defaultLightingEnabled() const
{
  return GETBIT(m_gsViewImplFlags, kDefaultLightingEnabled);
}

inline OdGsView::DefaultLightingType OdGsViewImpl::defaultLightingType() const
{
  return m_defaultLightingType;
}

inline bool OdGsViewImpl::isNonRectClipped() const
{
  return m_nrcCounts.size() != 0;
}

inline bool OdGsViewImpl::isRotated() const
{
  short rotation = viewportRotation();
  return rotation == 90 || rotation == 270;
}

inline void OdGsViewImpl::invalidate(const OdGsUpdateExtents& extents, const OdGsModel *pModel)
{
  invalidate(extents.getExtents(), pModel, extents.getLineweight());
}

inline const OdGeMatrix3d& OdGsViewImpl::eyeToWorldMatrix() const
{
  if(!GETBIT(m_gsViewImplFlags, kEyeToWorldValid))
  {
    m_eyeToWorld.setCoordSystem(target(), xVector(), upVector(), eyeVector());
    SETBIT(m_gsViewImplFlags, kEyeToWorldValid, true);
  }
  return m_eyeToWorld;
}

inline const OdGeMatrix3d& OdGsViewImpl::worldToEyeMatrix() const
{
  if(!GETBIT(m_gsViewImplFlags, kWorldToEyeValid))
  {
    m_worldToEye = eyeToWorldMatrix().inverse();
    SETBIT(m_gsViewImplFlags, kWorldToEyeValid, true);
  }
  return m_worldToEye;
}

inline OdGsViewImpl* OdGsViewImpl::safeCast(OdGsView* pView)
{
  return OdSmartPtr<OdGsViewImpl>(pView).get();
}

inline const OdGsViewImpl* OdGsViewImpl::safeCast(const OdGsView* pView)
{
  return OdSmartPtr<OdGsViewImpl>(pView).get();
}

inline bool OdGsViewImpl::isSnapping() const
{
  return GETBIT(m_gsViewImplFlags, kSnapping);
}

inline void OdGsViewImpl::setSnapping(bool val)
{
  SETBIT(m_gsViewImplFlags, kSnapping, val);
}

inline bool OdGsViewImpl::isRegenOnDrawForbidden() const
{
  return GETBIT(m_gsViewImplFlags, kForbidDrawableRegenDraw);
}

inline void OdGsViewImpl::setRegenOnDrawForbidden(bool val)
{
  SETBIT(m_gsViewImplFlags, kForbidDrawableRegenDraw, val);
}

inline bool OdGsViewImpl::isAccumulateVpChanges() const
{
  return GETBIT(m_gsViewImplFlags, kAccumulateVpChanges);
}

inline bool OdGsViewImpl::isCompletelyVisible() const
{
  return GETBIT(m_gsViewImplFlags, kDepViewInside);
}

inline void OdGsViewImpl::setCompletelyVisible(bool val)
{
  SETBIT(m_gsViewImplFlags, kDepViewInside, val);
}

inline OdGiClipBoundaryWithAbstractData *OdGsViewImpl::viewport3dClippingObject()
{
  return m_p3dClip.get();
}

inline bool OdGsViewImpl::hasViewport3dClipping() const
{
  return !m_p3dClip.isNull();
}

inline double OdGsViewImpl::actualFieldWidth() const
{
  return m_fieldWidth;
}

inline double OdGsViewImpl::actualFieldHeight() const
{
  return m_fieldHeight;
}

#include "TD_PackPop.h"

#endif // ODGSVIEWIMPL_INC
