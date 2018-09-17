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

#ifndef ODGSBASEINCLUDE_INC
#define ODGSBASEINCLUDE_INC

// Previously GsBaseVectorizer.h defines module/device/view classes
#include "Gs/GsBaseModule.h"
#include "Gs/GsBaseVectorizeDevice.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gs/GsBaseVectorizeView.h"
#include "SaveState.h"
#include "RxDictionary.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "ThreadsCounter.h"

#include "TD_PackPush.h"

/** \details
    Base template to join vectorize view and vectorizer objects with default behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
template <class BaseVectorizerObject>
class OdGsBaseVectorizeViewJoin : public OdGsBaseVectorizeView, public BaseVectorizerObject
{
  public:
    OdGsBaseVectorizeViewJoin()
    {
      OdGsBaseVectorizer::setUp(*this);
    }

    IMPL_VECTORIZER_DEFAULT()

    virtual void clearLinetypeCache() { OdGiBaseVectorizerImpl::clearLinetypeCache(); }

#if !defined(_MSC_VER) || (_MSC_VER < 1600)
    ODRX_USING_HEAP_OPERATORS(BaseVectorizerObject);
#else
    // Usage of template class as a base for new/delete operators in 'using' directive causes IntelliSense crash in VS2010
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeView);
#endif

#if 0

    // Resolve ambiguous access
    bool isPerspective() const
    {
      return OdGsViewImpl::isPerspective();
      //return OdGsBaseVectorizer::view().isPerspective();
    }
    OdGeMatrix3d objectToDeviceMatrix() const
    {
      return OdGiBaseVectorizerImpl::objectToDeviceMatrix();
    }
    void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt,
      OdGePoint2d& pixelArea, bool includePerspective = true) const
    {
      OdGsViewImpl::getNumPixelsInUnitSquare(givenWorldpt, pixelArea, includePerspective);
      //OdGsBaseVectorizer::view().getNumPixelsInUnitSquare(givenWorldpt, pixelArea, includePerspective);
    }

#else
    // Resolve ambiguous access. Typically we wants to call it from vectorizer, elsewhere it is not differs from worldToDevice.
    OdGeMatrix3d objectToDeviceMatrix() const
    {
      return OdGiBaseVectorizerImpl::objectToDeviceMatrix();
    }
    int lineweightToPixels(OdDb::LineWeight lineweight) const
    {
      return OdGiBaseVectorizerImpl::lineweightToPixels(lineweight);
    }
    double lineweightToPixels(double lineweight) const
    {
      return OdGiBaseVectorizerImpl::lineweightToPixels(lineweight);
    }
    void setVisualStyle(const OdGiVisualStyle& visualStyle)
    {
      OdGsViewImpl::setVisualStyle(visualStyle);
    }
    bool isSupportPlotStyles() const
    {
      return OdGiBaseVectorizerImpl::isSupportPlotStyles();
    }
#endif
  protected:
    void updateGeometryByDevice() { }
};

/** \details
    Enables multithreading behavior for joined view and vectorizer objects.
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
template <class BaseVectorizerObject>

class OdGsBaseVectorizeViewJoinMT : public OdGsBaseVectorizeViewJoin<BaseVectorizerObject>
{
  protected:
    OdArray<OdGsViewPtr> m_vectorizers;
    OdUInt32 m_vectUsage;
  public:
    OdGsBaseVectorizeViewJoinMT()
      : m_vectUsage(0)
    {
    }

    virtual OdUInt32 numVectorizers() const
    {
      if (OdGsBaseVectorizeView::baseDevice()->m_vectPerfData.enableParallelVectorization())
      {
        OdGsDevice *pDevice = OdGsBaseVectorizeView::device();
        if (pDevice && !pDevice->properties().isNull() &&
            pDevice->properties()->has(OD_T("MaxRegenThreads")))
          return (OdUInt32)OdRxVariantValue(pDevice->properties()->getAt(OD_T("MaxRegenThreads")))->getUInt16();
        return 4;
      }
      return 1;
    }
    virtual OdGsBaseVectorizer* getVectorizer(bool /*bDisplay*/)
    {
      m_vectUsage++;
      if (m_vectUsage == 1)
        return this;
      if (m_vectorizers.size() < m_vectUsage - 1)
      {
        OdGsViewPtr pView;
        {
          OdSaveStateFlag(OdUInt32, OdGsClientViewInfo::kSetViewportId, _ss1, this->OdGsBaseVectorizeView::m_viewInfo.viewportFlags, true);
          OdSaveState<long> _ss2(this->OdGsBaseVectorizeView::m_viewInfo.viewportId, (long)OdGsBaseVectorizeView::viewportId());
          pView = OdGsBaseVectorizeView::cloneView(false);
        }
        OdGsBaseVectorizeViewJoinMT *pThis = static_cast<OdGsBaseVectorizeViewJoinMT*>(pView.get());
        pThis->setUp(OdGsBaseVectorizer::view());
        m_vectorizers.push_back(pView);
      }
      OdGsBaseVectorizer *pVect = static_cast<OdGsBaseVectorizeViewJoinMT*>(m_vectorizers[m_vectUsage - 2].get());
      // viewParams?
      return pVect;
    }
    virtual void releaseVectorizer(OdGsBaseVectorizer* /*pVect*/)
    {
      m_vectUsage--;
    }

    bool isMTRegen() const
    {
      return odThreadsCounter();
    }
    //bool isMTView() const
    //{
    //  return (&view() != this);
    //}

    // Note: do not delete this code branch, it is useful for find places where view is called unsafe for MT.
#if 0
  private:

#define MT_VECT_VIEWMTH(mthSem, mthCall) \
    mthSem \
    { \
      if (&view() != this) \
        view().mthCall; \
      OdGsViewImpl::mthCall; \
    }
#define MT_VECT_VIEWMTH_RET(mthSem, mthCall) \
    mthSem \
    { \
      if (&view() != this) \
        return view().mthCall; \
      return OdGsViewImpl::mthCall; \
    }

    MT_VECT_VIEWMTH_RET(RenderMode mode() const, mode())
    //MT_VECT_VIEWMTH(void setMode(RenderMode mode), setMode(mode))
    //MT_VECT_VIEWMTH(void freezeLayer(OdDbStub* layerID), freezeLayer(layerID))
    //MT_VECT_VIEWMTH(void thawLayer(OdDbStub* layerID), thawLayer(layerID))
    //MT_VECT_VIEWMTH(void clearFrozenLayers(), clearFrozenLayers());
    //MT_VECT_VIEWMTH(void setViewportBorderProperties(ODCOLORREF color, int width), setViewportBorderProperties(color, width))
    MT_VECT_VIEWMTH(void getViewportBorderProperties(ODCOLORREF& color, int& width) const, getViewportBorderProperties(color, width))
    //MT_VECT_VIEWMTH(void setViewportBorderVisibility(bool visible), setViewportBorderVisibility(visible))
    MT_VECT_VIEWMTH_RET(bool isViewportBorderVisible() const, isViewportBorderVisible())
    //MT_VECT_VIEWMTH(void setEnableFrontClip(bool enable), setEnableFrontClip(enable))
    MT_VECT_VIEWMTH_RET(bool isFrontClipped() const, isFrontClipped())
    //MT_VECT_VIEWMTH(void setFrontClip(double distance), setFrontClip(distance))
    MT_VECT_VIEWMTH_RET(double frontClip() const, frontClip())
    //MT_VECT_VIEWMTH(void setEnableBackClip(bool enable), setEnableBackClip(enable))
    MT_VECT_VIEWMTH_RET(bool isBackClipped() const, isBackClipped())
    //MT_VECT_VIEWMTH(void setBackClip(double distance), setBackClip(distance))
    MT_VECT_VIEWMTH_RET(double backClip() const, backClip())
    MT_VECT_VIEWMTH_RET(OdGePoint3d position() const, position())
    MT_VECT_VIEWMTH_RET(OdGePoint3d target() const, target())
    MT_VECT_VIEWMTH_RET(OdGeVector3d upVector() const, upVector())
    MT_VECT_VIEWMTH_RET(double fieldWidth() const, fieldWidth())
    MT_VECT_VIEWMTH_RET(double fieldHeight() const, fieldHeight())
    MT_VECT_VIEWMTH_RET(double windowAspect() const, windowAspect())
    MT_VECT_VIEWMTH_RET(double lensLength() const, lensLength())
    //MT_VECT_VIEWMTH(void setLensLength(double lensLength), setLensLength(lensLength))
    MT_VECT_VIEWMTH_RET(bool isPerspective() const, isPerspective())
    //MT_VECT_VIEWMTH(void beginInteractivity(double frameRateInHz), beginInteractivity(frameRateInHz))
    //MT_VECT_VIEWMTH(void endInteractivity(), endInteractivity())
    //MT_VECT_VIEWMTH(void flush(), flush())
    //MT_VECT_VIEWMTH(void hide(), hide())
    //MT_VECT_VIEWMTH(void show(), show())
    MT_VECT_VIEWMTH_RET(bool isVisible(), isVisible())
    //MT_VECT_VIEWMTH_RET(OdGsViewPtr cloneView(bool cloneViewParameters = true, bool cloneGeometry = false),
    //                    cloneView(cloneViewParameters, cloneGeometry))
    //MT_VECT_VIEWMTH_RET(bool exceededBounds(), exceededBounds())
    //MT_VECT_VIEWMTH(void enableStereo(bool enabled), enableStereo(enabled))
    MT_VECT_VIEWMTH_RET(bool isStereoEnabled() const, isStereoEnabled())
    //MT_VECT_VIEWMTH(void setStereoParameters(double magnitude, double parallax), setStereoParameters(magnitude, parallax))
    MT_VECT_VIEWMTH(void getStereoParameters(double& magnitude, double& parallax) const, getStereoParameters(magnitude, parallax))
    //MT_VECT_VIEWMTH(void initLights(OdRxIterator* pLightsIterator), initLights(pLightsIterator))
    //MT_VECT_VIEWMTH(void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight), setViewport(lowerLeft, upperRight))
    //MT_VECT_VIEWMTH(void setViewport(const OdGsDCRect& screenRect), setViewport(screenRect))
    //MT_VECT_VIEWMTH(void setViewport(const OdGsDCRectDouble& screenRec), setViewport(screenRec))
    MT_VECT_VIEWMTH(void getViewport(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const, getViewport(lowerLeft, upperRight))
    MT_VECT_VIEWMTH(void getViewport(OdGsDCRect& screenRect) const, getViewport(screenRect))
    MT_VECT_VIEWMTH(void getViewport(OdGsDCRectDouble& screenRec) const, getViewport(screenRec))
    MT_VECT_VIEWMTH_RET(OdGeMatrix3d screenMatrix() const, screenMatrix())
    MT_VECT_VIEWMTH_RET(OdGeMatrix3d worldToDeviceMatrix() const, worldToDeviceMatrix())
    ////MT_VECT_VIEWMTH_RET(OdGeMatrix3d objectToDeviceMatrix() const, objectToDeviceMatrix())
    MT_VECT_VIEWMTH_RET(OdGeMatrix3d viewingMatrix() const, viewingMatrix())
    MT_VECT_VIEWMTH_RET(OdGeMatrix3d projectionMatrix() const, projectionMatrix())
    //MT_VECT_VIEWMTH(void viewParameters(OdGsView* pView) const, viewParameters(pView))
    MT_VECT_VIEWMTH_RET(double lineweightToDcScale() const, lineweightToDcScale())
    //MT_VECT_VIEWMTH(void setLineweightToDcScale(double scale), setLineweightToDcScale(scale))
    //MT_VECT_VIEWMTH(void setLineweightEnum(int numLineweights, const OdUInt8* lineweights), setLineweightEnum(numLineweights, lineweights))
    //MT_VECT_VIEWMTH(void setLinetypeScaleMultiplier(double linetypeScaleMultiplier), setLinetypeScaleMultiplier(linetypeScaleMultiplier))
    //MT_VECT_VIEWMTH(void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier),
    //                setAlternateLinetypeScaleMultiplier(linetypeAlternateScaleMultiplier))
    MT_VECT_VIEWMTH_RET(double linetypeScaleMultiplier() const, linetypeScaleMultiplier())
    //MT_VECT_VIEWMTH(void dolly(const OdGeVector3d& dollyVector), dolly(dollyVector))
    //MT_VECT_VIEWMTH(void dolly(double xDolly, double yDolly, double zDolly), dolly(xDolly, yDolly, zDolly))
    //MT_VECT_VIEWMTH(void roll(double rollAngle), roll(rollAngle))
    //MT_VECT_VIEWMTH(void orbit(double xOrbit, double yOrbit), orbit(xOrbit, yOrbit))
    //MT_VECT_VIEWMTH(void zoom(double zoomFactor), zoom(zoomFactor))
    //MT_VECT_VIEWMTH(void pan(double xPan, double yPan), pan(xPan, yPan))
    //MT_VECT_VIEWMTH(void setView(const OdGePoint3d & position, const OdGePoint3d& target, const OdGeVector3d& upVector,
    //                             double fieldWidth, double fieldHeight, Projection projection = kParallel),
    //                setView(position, target, upVector, fieldWidth, fieldHeight, projection))
    MT_VECT_VIEWMTH(void viewportClipRegion(OdIntArray& counts, OdGePoint2dArray& vertices) const,
                     viewportClipRegion(counts, vertices))
    MT_VECT_VIEWMTH(void viewportClipRegion(OdIntArray& counts, OdGsDCPointArray& dcPts) const,
                     viewportClipRegion(counts, dcPts))
    //MT_VECT_VIEWMTH(void setViewportClipRegion(int numContours, const int* numVertices, const OdGePoint2d* vertices),
    //                setViewportClipRegion(numContours, numVertices, vertices))
    //MT_VECT_VIEWMTH(void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices),
    //                setViewportClipRegion(numContours, numVertices, vertices))
    MT_VECT_VIEWMTH_RET(bool isDependentViewportView() const, isDependentViewportView())
    MT_VECT_VIEWMTH_RET(bool isDependentGeometryView() const, isDependentGeometryView())
    MT_VECT_VIEWMTH_RET(bool isHelperView() const, isHelperView())
    ////MT_VECT_VIEWMTH_RET(const OdGsBaseModule *getGsModulePtr() const, getGsModulePtr())
    MT_VECT_VIEWMTH_RET(const OdGiContextualColors *contextualColors() const, contextualColors())
    //MT_VECT_VIEWMTH(void setContextualColors(OdGiContextualColors *pColors), setContextualColors())
    MT_VECT_VIEWMTH_RET(bool pointInViewport(const OdGePoint2d& screenPoint) const, pointInViewport(screenPoint))
    //MT_VECT_VIEWMTH(void setClearColor(ClearColor color), setClearColor(color))
    MT_VECT_VIEWMTH(void invalidate(), invalidate())
    MT_VECT_VIEWMTH(void invalidate(const OdGsDCRect &rect), invalidate(rect))
    MT_VECT_VIEWMTH(void invalidateCachedViewportGeometry(), invalidateCachedViewportGeometry())
    //MT_VECT_VIEWMTH(void select(const OdGsDCPoint* points, int numPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode = OdGsView::kCrossing),
    //                select(points, numPoints, pReactor, mode))
    //MT_VECT_VIEWMTH_RET(bool add(OdGiDrawable* sceneGraph, OdGsModel* model), add(sceneGraph, model))
    //MT_VECT_VIEWMTH_RET(bool erase(OdGiDrawable* sceneGraph), erase(sceneGraph))
    //MT_VECT_VIEWMTH(void eraseAll(), eraseAll())
    MT_VECT_VIEWMTH_RET(bool isValid() const, isValid())
    MT_VECT_VIEWMTH(void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt, OdGePoint2d& pixelArea, bool includePerspective = true) const,
                    getNumPixelsInUnitSquare(givenWorldpt, pixelArea, includePerspective));
    //MT_VECT_VIEWMTH(void setBackground(OdDbStub *backgroundId), setBackground(backgroundId))
    MT_VECT_VIEWMTH_RET(OdDbStub *background() const, background())
    //MT_VECT_VIEWMTH(void setVisualStyle(OdDbStub *visualStyleId), setVisualStyle(visualStyleId))
    MT_VECT_VIEWMTH_RET(OdDbStub *visualStyle() const, visualStyle())
    //MT_VECT_VIEWMTH(void setVisualStyle(const OdGiVisualStyle &visualStyle), setVisualStyle(visualStyle))
    MT_VECT_VIEWMTH_RET(bool visualStyle(OdGiVisualStyle &visualStyle) const, visualStyle(visualStyle))
    MT_VECT_VIEWMTH_RET(OdUInt32 viewportId() const, viewportId())
    MT_VECT_VIEWMTH_RET(OdDbStub* getViewportObjectId() const, getViewportObjectId())
    //MT_VECT_VIEWMTH_RET(bool viewExtents(OdGeBoundBlock3d& extents) const, viewExtents(extents))
    //MT_VECT_VIEWMTH(void select(const OdGePoint2d* aPtDc, int numPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode = OdGsView::kCrossing),
    //                select(aPtDc, numPoints, pReactor, mode))
    MT_VECT_VIEWMTH_RET(bool isSnapping() const, isSnapping())
    //MT_VECT_VIEWMTH(void setSnapping(bool val), setSnapping(val))
    MT_VECT_VIEWMTH_RET(bool isRegenOnDrawForbidden() const, isRegenOnDrawForbidden())
    //MT_VECT_VIEWMTH(void setRegenOnDrawForbidden(bool val), setRegenOnDrawForbidden())
    //MT_VECT_VIEWMTH(void clearLinetypeCache(), clearLinetypeCache())
    MT_VECT_VIEWMTH_RET(double linetypeAlternateScaleMultiplier() const, linetypeAlternateScaleMultiplier())
    MT_VECT_VIEWMTH_RET(OdUInt32 frozenLayers() const, frozenLayers())
    MT_VECT_VIEWMTH(void frozenLayers(OdDbStubPtrArray& frozenLayers) const, frozenLayers(frozenLayers))
    MT_VECT_VIEWMTH_RET(bool isLayerVisible(OdDbStub* layerId) const, isLayerVisible(layerId))
    MT_VECT_VIEWMTH_RET(int lineweightToPixels(OdDb::LineWeight lineweight) const, lineweightToPixels(lineweight))
    MT_VECT_VIEWMTH_RET(double lineweightToPixels(double lineweight) const, lineweightToPixels(lineweight))
    MT_VECT_VIEWMTH(void screenRect(OdGsDCPoint &lowerLeft, OdGsDCPoint &upperRight) const, screenRect(lowerLeft, upperRight))
    MT_VECT_VIEWMTH(void screenRect(OdGePoint2d &lowerLeft, OdGePoint2d &upperRight) const, screenRect(lowerLeft, upperRight))
    MT_VECT_VIEWMTH(void screenRectNorm(OdGsDCRect& normalizedRect) const, screenRectNorm(normalizedRect))
    MT_VECT_VIEWMTH(void viewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const, viewportDcCorners(lower_left, upper_right))
    MT_VECT_VIEWMTH_RET(bool doPerspectivePt(OdGePoint3d& point) const, doPerspectivePt(point))
    MT_VECT_VIEWMTH_RET(bool doInversePerspectivePt(OdGePoint3d& point) const, doInversePerspectivePt(point))
    MT_VECT_VIEWMTH_RET(OdGeMatrix3d eyeToScreenMatrix() const, eyeToScreenMatrix())
    MT_VECT_VIEWMTH_RET(const OdGeMatrix3d& eyeToWorldMatrix() const, eyeToWorldMatrix())
    MT_VECT_VIEWMTH_RET(const OdGeMatrix3d& worldToEyeMatrix() const, worldToEyeMatrix())
    MT_VECT_VIEWMTH_RET(ClearColor clearColor() const, clearColor())
    MT_VECT_VIEWMTH(void invalidate(const OdGeExtents3d& worldExt, OdDb::LineWeight extendByLineweight = OdDb::kLnWt000),
                    invalidate(worldExt, extendByLineweight))
    MT_VECT_VIEWMTH(void invalidateRegion(const OdGiPathNode& path), invalidateRegion(path))
    MT_VECT_VIEWMTH_RET(OdGsDCRectArray invalidRects() const, invalidRects())
    MT_VECT_VIEWMTH_RET(bool isViewportOnScreen() const, isViewportOnScreen())
    MT_VECT_VIEWMTH_RET(bool isCompletelyVisible() const, isCompletelyVisible())
    //MT_VECT_VIEWMTH(void setInversion(bool invertedX, bool invertedY), setInversion(invertexX, invertexY))
    MT_VECT_VIEWMTH_RET(OdGiRegenType getRegenType() const, getRegenType())
    MT_VECT_VIEWMTH_RET(double focalLength() const, focalLength())
    MT_VECT_VIEWMTH_RET(const OdGeVector3d& eyeVector() const, eyeVector())
    MT_VECT_VIEWMTH_RET(const OdGeVector3d& xVector() const, xVector())
    MT_VECT_VIEWMTH_RET(double getLinetypeScaleMultiplier() const, getLinetypeScaleMultiplier())
    MT_VECT_VIEWMTH_RET(double getLinetypeGenerationCriteria() const, getLinetypeGenerationCriteria())
    MT_VECT_VIEWMTH_RET(double getDeviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve, bool bRecalculate = false) const,
                        getDeviation(deviationType, pointOnCurve, bRecalculate))
    //MT_VECT_VIEWMTH(void initDeviation(), initDeviation())
    MT_VECT_VIEWMTH_RET(int cachedDrawables() const, cachedDrawables())
#ifdef ODGS_LOCAL_IDS
    MT_VECT_VIEWMTH_RET(OdUInt32 localViewportId(const OdGsBaseModel *pModel) const, localViewportId(pModel))
    MT_VECT_VIEWMTH_RET(OdGsViewLocalId &getViewLocalId(), getViewLocalId())
#endif
    //MT_VECT_VIEWMTH_RET(bool sceneDept(double& zNear, double& zFar, OdGsOverlayId nOverlay = kGsMainOverlay) const, sceneDept(zNear, zFar, nOverlay))
    //MT_VECT_VIEWMTH_RET(bool usesDept(double* pMinDeptSupported = 0, double* pMaxDeptSupported = 0) const, usesDept(pMinDeptSupported, pMaxDepSupported))
    MT_VECT_VIEWMTH_RET(bool isNonRectClipped() const, isNonRectClipped())
    MT_VECT_VIEWMTH_RET(bool hasLweights() const, hasLweights())
    MT_VECT_VIEWMTH_RET(const OdGePoint2d& dcScreenMin() const, dcScreenMin())
    MT_VECT_VIEWMTH_RET(const OdGePoint2d& dcScreenMax() const, dcScreenMax())
    MT_VECT_VIEWMTH_RET(const OdGePoint2d& dcLowerLeft() const, dcLowerLeft())
    MT_VECT_VIEWMTH_RET(const OdGePoint2d& dcUpperRight() const, dcUpperRight())
    MT_VECT_VIEWMTH_RET(OdGeMatrix3d perspectiveMatrix() const, perspectiveMatrix())
    //MT_VECT_VIEWMTH(void invalidateCachedViewportGeometry(OdUInt32 mask), invalidateCachedViewportGeometry(mask))
    MT_VECT_VIEWMTH_RET(double calcDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const, calcDeviation(type, pt))
    MT_VECT_VIEWMTH_RET(OdGsPropertiesPtr getViewportPropertiesRoot() const, getViewportPropertiesRoot())
    MT_VECT_VIEWMTH_RET(bool updateViewportProperties(OdUInt32 incFlags), updateViewportProperties(incFlags))
    MT_VECT_VIEWMTH_RET(OdGsPropertiesPtr getViewportPropertiesForType(OdGsProperties::PropertiesType type) const, getViewportPropertiesForType(type))
    MT_VECT_VIEWMTH_RET(const OdGeMatrix3d& rotationMatrix() const, rotationMatrix())
    MT_VECT_VIEWMTH_RET(short viewportRotation() const, viewportRotation())
    MT_VECT_VIEWMTH_RET(bool isRotated() const, isRotated())
    MT_VECT_VIEWMTH_RET(double unrotatedFieldHeight() const, unrotatedFieldHeight())
    MT_VECT_VIEWMTH_RET(double unrotatedFieldWidth() const, unrotatedFieldWidth())
    //MT_VECT_VIEWMTH(void initCullingVolume(), initCullingVolume())
    //MT_VECT_VIEWMTH(void setCullingVolume(OdGsCullingVolumePtr newVol), setCullingVolume(newVol))
    MT_VECT_VIEWMTH_RET(OdGsCullingVolumePtr cullingVolume() const, cullingVolume())
    MT_VECT_VIEWMTH_RET(bool isCullingVolumeInitialized() const, isCullingVolumeInitialized())
    MT_VECT_VIEWMTH_RET(bool cullingVolumeIntersectWithOpt(const OdGsCullingPrimitive &prim) const, cullingVolumeIntersectWithOpt(prim))
    MT_VECT_VIEWMTH_RET(OdGsCullingVolume::IntersectionStatus cullingVolumeIntersectWith(const OdGsCullingPrimitive &prim) const,
                        cullingVolumeIntersectWith(prim))
    MT_VECT_VIEWMTH(void cullingVolumeTransformBy(const OdGeMatrix3d &xfm), cullingVolumeTransformBy(xfm))

#undef MT_VECT_VIEWMTH_RET
#undef MT_VECT_VIEWMTH

#endif
};

/** \details
    Joins OdGsBaseVectorizeView and OdGsBaseVectorizer classes with default behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class OdGsBaseVectorizeViewDef : public OdGsBaseVectorizeViewJoin<OdGsBaseVectorizer>
{
  public:
    OdGsBaseVectorizeViewDef() { }
};

/** \details
    Joins OdGsBaseVectorizeView and OdGsBaseVectorizer classes with multithread behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class OdGsBaseVectorizeViewMT : public OdGsBaseVectorizeViewJoinMT<OdGsBaseVectorizer>
{
  public:
    OdGsBaseVectorizeViewMT() { }

    bool isMTView() const
    {
      return (&view() != this);
    }
};

/** \details
    Template class for helpers which provide base synchronization routines for device resources access.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
template <int SyncCount = 4>
class OdGsBaseDeviceMTHelpersTemplate
{
  protected:
    OdMutexPtr m_locker[SyncCount];
    bool m_bEnabled;
  public:
    OdGsBaseDeviceMTHelpersTemplate() : m_bEnabled(false) { }
    void setEnabled(bool bEnable) { m_bEnabled = bEnable; }
    bool isEnabled() const { return m_bEnabled; }
    void lock(OdUInt32 nSync)
    {
      //if (odThreadsCounter())
      if (m_bEnabled)
        m_locker[nSync].get()->lock();
    }
    void unlock(OdUInt32 nSync)
    {
      //if (odThreadsCounter())
      if (m_bEnabled)
        m_locker[nSync].get()->unlock();
    }
  protected:
    void check(int nSyncObjects) { if (nSyncObjects != SyncCount) throw OdError(eOutOfRange); }
  public:
    class SyncHelper
    {
      OdGsBaseDeviceMTHelpersTemplate &m_syncPoint;
      OdUInt32 m_syncType;
      public:
        SyncHelper(OdGsBaseDeviceMTHelpersTemplate &syncPoint, OdUInt32 syncType)
          : m_syncPoint(syncPoint), m_syncType(syncType)
        {
          m_syncPoint.lock(m_syncType);
        }
        ~SyncHelper()
        {
          m_syncPoint.unlock(m_syncType);
        }
    };
};

/** \details
    Helper class provide base synchronization routines for device resources access.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class OdGsBaseDeviceMTHelpers : public OdGsBaseDeviceMTHelpersTemplate<>
{
  public:
    enum SyncType
    {
      kSyncDeviceAccess = 0,
      kSyncDeviceResource,
      kSyncRasterCache,
      kSyncTextureCache,

      kSyncCount
    };
  public:
    OdGsBaseDeviceMTHelpers() : OdGsBaseDeviceMTHelpersTemplate<>() { check(kSyncCount); }
    void lock(SyncType nSync) { OdGsBaseDeviceMTHelpersTemplate<>::lock(nSync); }
    void unlock(SyncType nSync) { OdGsBaseDeviceMTHelpersTemplate<>::unlock(nSync); }
};

/** \details
    Provides AutoLock functionality for device resources access.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class OdGsBaseDeviceMTSyncHelper : public OdGsBaseDeviceMTHelpers::SyncHelper
{
  public:
    OdGsBaseDeviceMTSyncHelper(OdGsBaseDeviceMTHelpers &syncPoint, OdGsBaseDeviceMTHelpers::SyncType syncType)
      : OdGsBaseDeviceMTHelpers::SyncHelper(syncPoint, syncType) {}
};

#include "TD_PackPop.h"

#endif // ODGSBASEINCLUDE_INC
