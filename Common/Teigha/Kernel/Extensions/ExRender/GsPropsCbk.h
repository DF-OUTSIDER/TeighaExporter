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

#ifndef __OdGsPropChangeCbk_H__
#define __OdGsPropChangeCbk_H__

#include "TD_PackPush.h"

#include "Gs/GsViewImpl.h"
#include "Gs/GsBaseVectorizeDevice.h"

/** <group OdGs_Classes>
*/
template <typename CallFor, typename PropType>
class OdGsInvalidatedCallPropModified
{
  protected:
    CallFor *m_pCallFor;
    bool *m_pControlBit;
    PropType m_nFlag;
  public:
    OdGsInvalidatedCallPropModified(CallFor *pCallFor, bool *pControlBit, PropType nFlag)
      : m_pCallFor(pCallFor), m_pControlBit(pControlBit), m_nFlag(nFlag)
    { *m_pControlBit = false; }
    ~OdGsInvalidatedCallPropModified()
    { if (*m_pControlBit) m_pCallFor->onPropertyModified(m_nFlag); }
};

/** <group OdGs_Classes>
*/
template <typename ImplBase>
class OdGsViewPropChangeCbk : public ImplBase
{
  private:
    OdGsViewImpl &thisView() { return *static_cast<OdGsViewImpl*>(this); }
    const OdGsViewImpl &thisView() const { return *static_cast<const OdGsViewImpl*>(this); }
  private:
    bool m_bInvalidated;
  public:
    enum ViewProperty
    {
      kViewViewport = 0, // viewport region changed
      kViewViewportClip, // viewport clip region modified
      kViewBorder,       // viewport border changed
      kViewOrientation,  // view parameters changed (position, fields, target)
      kViewLensLength,   // lens length changed
      kViewFrontClip,    // front clipping changed
      kViewBackClip,     // back clipping changed
      kViewVisibility,   // hide or show called
      kViewLineweight,   // lineweight settings changed
      kViewLwdEnum,      // lineweight enum changed
      kViewRenderMode,   // render mode was changed
      kView3dClip,       // viewport 3d clipping changed
      kViewFrozenLayers, // frozen layers list modified
      kViewLinetype,     // linetype scale multipliers
      kViewBackground,   // background object changed
      kViewVisualStyle,  // visual style changed
      kViewStereoParams, // stereo parameters changed
      kViewClearColor,   // clear color changed
      kViewDefLighting,  // default lighting parameters

      kNumViewProps
    };

    virtual void onPropertyModified(ViewProperty nProp) { }
  protected:
    OdUInt32 makeFlag(ViewProperty nProp) { return 1 << nProp; }
  private:
    typedef OdGsInvalidatedCallPropModified<OdGsViewPropChangeCbk<ImplBase>, ViewProperty> IsInvalidatedCallPropModified;
  public:
    OdGsViewPropChangeCbk()
      : m_bInvalidated(false)
    { }

    // OdGsView overrides

    virtual void invalidate()
    {
      m_bInvalidated = true;
      ImplBase::invalidate();
    }

    virtual void setLineweightToDcScale(double scale)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewLineweight);
      ImplBase::setLineweightToDcScale(scale);
    }
    virtual void setLineweightEnum(int numLineweights, const OdUInt8* lineweights)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewLwdEnum);
      ImplBase::setLineweightEnum(numLineweights, lineweights);
    }

    virtual void setViewport(const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewViewport);
      ImplBase::setViewport(lowerLeft, upperRight);
    }
    virtual void setViewport(const OdGsDCRect& screenRect)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewViewport);
      ImplBase::setViewport(screenRect);
    }
    virtual void setViewport(const OdGsDCRectDouble& screenRect)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewViewport);
      ImplBase::setViewport(screenRect);
    }

    virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGsDCPoint const* vertices)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewViewportClip);
      ImplBase::setViewportClipRegion(numContours, numVertices, vertices);
    }
    virtual void setViewportClipRegion(int numContours, int const* numVertices, OdGePoint2d const* vertices)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewViewportClip);
      ImplBase::setViewportClipRegion(numContours, numVertices, vertices);
    }

    virtual void setViewportBorderProperties(ODCOLORREF color, int width)
    {
      ODCOLORREF prevColor; int prevWidth;
      thisView().getViewportBorderProperties(prevColor, prevWidth);
      ImplBase::setViewportBorderProperties(color, width);
      if ((color != prevColor) || (width != prevWidth))
        onPropertyModified(kViewBorder);
    }
    virtual void setViewportBorderVisibility(bool visible)
    {
      bool bPrevVisible = thisView().isViewportBorderVisible();
      ImplBase::setViewportBorderVisibility(visible);
      if (visible != bPrevVisible)
        onPropertyModified(kViewBorder);
    }

    virtual void setView(const OdGePoint3d& position, const OdGePoint3d& target, const OdGeVector3d& upVector,
                         double fieldWidth, double fieldHeight, OdGsView::Projection projectionType = OdGsView::kParallel)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::setView(position, target, upVector, fieldWidth, fieldHeight, projectionType);
    }
    virtual void setLensLength(double lensLength)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewLensLength);
      ImplBase::setLensLength(lensLength);
    }

    virtual void setEnableFrontClip(bool enable)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewFrontClip);
      ImplBase::setEnableFrontClip(enable);
    }
    virtual void setFrontClip(double frontClip)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewFrontClip);
      ImplBase::setFrontClip(frontClip);
    }
    virtual void setEnableBackClip(bool enable)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewBackClip);
      ImplBase::setEnableBackClip(enable);
    }
    virtual void setBackClip(double backClip)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewBackClip);
      ImplBase::setBackClip(backClip);
    }

    virtual void setMode(OdGsView::RenderMode mode)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewRenderMode);
      ImplBase::setMode(mode);
    }
    virtual void setVisualStyle(OdDbStub *visualStyleId)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewVisualStyle);
      ImplBase::setVisualStyle(visualStyleId);
    }
    virtual void setVisualStyle(const OdGiVisualStyle &visualStyle)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewVisualStyle);
      ImplBase::setVisualStyle(visualStyle);
    }

    virtual void hide()
    {
      bool bPrevVisible = thisView().isVisible();
      ImplBase::hide();
      if (bPrevVisible)
        onPropertyModified(kViewVisibility);
    }
    virtual void show()
    {
      bool bPrevVisible = thisView().isVisible();
      ImplBase::show();
      if (!bPrevVisible)
        onPropertyModified(kViewVisibility);
    }

    virtual void dolly(const OdGeVector3d& dollyVector)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::dolly(dollyVector);
    }
    virtual void dolly(double xDolly, double yDolly, double zDolly)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::dolly(xDolly, yDolly, zDolly);
    }
    virtual void roll(double rollAngle)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::roll(rollAngle);
    }
    virtual void orbit(double xOrbit, double yOrbit)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::orbit(xOrbit, yOrbit);
    }
    virtual void zoom(double zoomFactor)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::zoom(zoomFactor);
    }
    virtual void pan(double xPan, double yPan)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::pan(xPan, yPan);
    }
    virtual void zoomExtents(const OdGePoint3d &minPt, const OdGePoint3d &maxPt)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::zoomExtents(minPt, maxPt);
    }
    virtual void zoomWindow(const OdGePoint2d &lowerLeft, const OdGePoint2d &upperRight)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewOrientation);
      ImplBase::zoomWindow(lowerLeft, upperRight);
    }

    // GsView overrides (extended)

    virtual void setViewport3dClipping(const OdGiClipBoundary* pBoundary, const OdGiAbstractClipBoundary* pClipInfo = NULL)
    {
      const OdGiClipBoundary *pPrevSet = thisView().viewport3dClipping();
      ImplBase::setViewport3dClipping(pBoundary, pClipInfo);
      if (pPrevSet != thisView().viewport3dClipping())
        onPropertyModified(kView3dClip);
    }

    virtual void freezeLayer(OdDbStub* layerID)
    {
      const OdUInt32 nPrevLayers = thisView().frozenLayers();
      ImplBase::freezeLayer(layerID);
      if (nPrevLayers != thisView().frozenLayers())
        onPropertyModified(kViewFrozenLayers);
    }
    virtual void thawLayer(OdDbStub* layerID)
    {
      const OdUInt32 nPrevLayers = thisView().frozenLayers();
      ImplBase::thawLayer(layerID);
      if (nPrevLayers != thisView().frozenLayers())
        onPropertyModified(kViewFrozenLayers);
    }
    virtual void clearFrozenLayers()
    {
      const OdUInt32 nPrevLayers = thisView().frozenLayers();
      ImplBase::clearFrozenLayers();
      if (nPrevLayers != thisView().frozenLayers())
        onPropertyModified(kViewFrozenLayers);
    }

    virtual void enableStereo(bool enabled)
    {
      const bool bPrevState = thisView().isStereoEnabled();
      ImplBase::enableStereo(enabled);
      if (bPrevState != thisView().isStereoEnabled())
        onPropertyModified(kViewStereoParams);
    }
    virtual void setStereoParameters(double magnitude, double parallax)
    {
      double fMagnitude, fMagnitudePrev, fParallax, fParallaxPrev;
      thisView().getStereoParameters(fMagnitudePrev, fParallaxPrev);
      ImplBase::setStereoParameters(magnitude, parallax);
      thisView().getStereoParameters(fMagnitude, fParallax);
      if ((fMagnitude != fMagnitudePrev) || (fParallax != fParallaxPrev))
        onPropertyModified(kViewStereoParams);
    }

    virtual void setLinetypeScaleMultiplier(double linetypeScaleMultiplier)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewLinetype);
      ImplBase::setLinetypeScaleMultiplier(linetypeScaleMultiplier);
    }
    virtual void setAlternateLinetypeScaleMultiplier(double linetypeAlternateScaleMultiplier)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewLinetype);
      ImplBase::setAlternateLinetypeScaleMultiplier(linetypeAlternateScaleMultiplier);
    }

    virtual void setClearColor(OdGsView::ClearColor color)
    {
      const OdGsView::ClearColor prevColor = thisView().clearColor();
      ImplBase::setClearColor(color);
      if (prevColor != color)
        onPropertyModified(kViewClearColor);
    }
    virtual void setBackground(OdDbStub *backgroundId)
    {
      OdDbStub *prevBkgnd = thisView().background();
      ImplBase::setBackground(backgroundId);
      if (prevBkgnd != thisView().background())
        onPropertyModified(kViewBackground);
    }

    virtual void enableDefaultLighting(bool bEnable, OdGsView::DefaultLightingType lightType = OdGsView::kTwoLights)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kViewDefLighting);
      ImplBase::enableDefaultLighting(bEnable, lightType);
    }
};

/** <group OdGs_Classes>
*/
template <typename ImplBase>
class OdGsDevicePropChangeCbk : public ImplBase
{
  private:
    OdGsBaseVectorizeDevice &thisDevice() { return *static_cast<OdGsBaseVectorizeDevice*>(this); }
    const OdGsBaseVectorizeDevice &thisDevice() const { return *static_cast<const OdGsBaseVectorizeDevice*>(this); }
  private:
    bool m_bInvalidated;
  public:
    enum DeviceProperty
    {
      kDeviceSize = 0,   // device surface area changed
      kDeviceBackground, // device background color changed
      kDevicePalette,    // device palette changed

      kNumDeviceProps
    };

    virtual void onPropertyModified(DeviceProperty nProp) { }
  protected:
    OdUInt32 makeFlag(DeviceProperty nProp) { return 1 << nProp; }
  private:
    typedef OdGsInvalidatedCallPropModified<OdGsDevicePropChangeCbk<ImplBase> , DeviceProperty> IsInvalidatedCallPropModified;
  public:
    OdGsDevicePropChangeCbk()
      : m_bInvalidated(false)
    { }

    // OdGsDevice overrides

    virtual void invalidate()
    {
      m_bInvalidated = true;
      ImplBase::invalidate();
    }

    virtual void onSize(const OdGsDCRect& outputRect)
    {
      const OdGsDCRect prevOutputRect = thisDevice().outputRect();
      ImplBase::onSize(outputRect);
      if (prevOutputRect != outputRect)
        onPropertyModified(kDeviceSize);
    }

    virtual bool setBackgroundColor(ODCOLORREF backgroundColor)
    {
      ODCOLORREF prevBkgndColor = thisDevice().getBackgroundColor();
      bool bSetVal = ImplBase::setBackgroundColor(backgroundColor);
      if (prevBkgndColor != backgroundColor)
        onPropertyModified(kDeviceBackground);
      return bSetVal;
    }

    virtual void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors)
    {
      IsInvalidatedCallPropModified checkNCall(this, &m_bInvalidated, kDevicePalette);
      ImplBase::setLogicalPalette(logicalPalette, numColors);
    }
};

#include "TD_PackPop.h"

#endif // __OdGsPropChangeCbk_H__
