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
#include "ExGsGDIVectorizeDevice.h"
#include "../ExGsHelpers.h"
#include "Gs/GsBaseModel.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GePlane.h"
#ifdef _WIN32_WCE
#define DD_IGNORE_WCEALT_NEW
#include "afxwin.h"
#if (_WIN32_WCE < 0x500 && _MSC_VER < 1300)
  #include "wcealt.h"
#endif
#endif
#include "Ge/GeScale3d.h"
#include "Gi/GiSelector.h"
#include "Gi/GiMetafiler.h"
#include "Gi/GiSelectProc.h"
#include "Gs/GsViewportProperties.h"
#include "OdRound.h"
#include "OdCharConverter.h"

//Implementation of class ExGsOpenGLBaseDevice
//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGsGDIVectorizeDevice::ExGsGDIVectorizeDevice()
  : m_hOffscreenDC(0)
  , m_hbmScreen(0)
  , m_bClearScreen(true)
  , m_bUseSoftwareHLR(false)
  , m_bUseTextOut(false)
  , m_bUseTtfCache(false)
  , m_dMinTTFTextSize(5.0) // 2d optimized mode optimization. Display small texts, using poly instead.
  , m_bContextValid(false)
{
  setSortRenderTypes(true);
#ifdef OD_GDI_DYNAMICSUBENTHLT // for compatibility with previous behavior (enable dynamicSubEntHlt via OD_GDI_DYNAMICSUBENTHLT preprocessor definition)
  setDynamicSubEntHltEnabled(true);
#endif
#ifdef _USE_TEXTOUT // for compatibility with previous behavior (enable textOut via _USE_TEXTOUT preprocessor definition)
  setUseTextOut(true);
#endif
  SETBIT_1(m_flags, kDynamicHighlight);
  setSupportInteractiveViewMode( true );
}

ExGsGDIVectorizeDevice::~ExGsGDIVectorizeDevice()
{
  deleteDrawDc();
}

ODRX_DECLARE_PROPERTY(ClearScreen)
ODRX_DECLARE_PROPERTY(EnableSoftwareHLR)
ODRX_DECLARE_PROPERTY(UseTextOut)
ODRX_DECLARE_PROPERTY(UseTTFCache)
ODRX_DECLARE_PROPERTY(MinTTFTextSize)
ODRX_DECLARE_PROPERTY(GradientsAsBitmap)
ODRX_DECLARE_PROPERTY(GradientsAsPolys)
ODRX_DECLARE_PROPERTY(GradientsAsPolysThreshold)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsGDIVectorizeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsWin32Device);
  ODRX_GENERATE_PROPERTY(ClearScreen)
  ODRX_GENERATE_PROPERTY(EnableSoftwareHLR)
  ODRX_GENERATE_PROPERTY(UseTextOut)
  ODRX_GENERATE_PROPERTY(UseTTFCache)
  ODRX_GENERATE_PROPERTY(MinTTFTextSize)
  ODRX_GENERATE_PROPERTY(GradientsAsBitmap)
  ODRX_GENERATE_PROPERTY(GradientsAsPolys)
  ODRX_GENERATE_PROPERTY(GradientsAsPolysThreshold)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsGDIVectorizeDevice);

ODRX_DEFINE_PROPERTY_METHODS(ClearScreen,               ExGsGDIVectorizeDevice, clearScreen,                  setClearScreen,                  getBool);
ODRX_DEFINE_PROPERTY_METHODS(EnableSoftwareHLR,         ExGsGDIVectorizeDevice, useSoftwareHLR,               setUseSoftwareHLR,               getBool);
ODRX_DEFINE_PROPERTY_METHODS(UseTextOut,                ExGsGDIVectorizeDevice, useTextOut,                   setUseTextOut,                   getBool);
ODRX_DEFINE_PROPERTY_METHODS(UseTTFCache,               ExGsGDIVectorizeDevice, useTtfCache,                  setUseTtfCache,                  getBool);
ODRX_DEFINE_PROPERTY_METHODS(MinTTFTextSize,            ExGsGDIVectorizeDevice, minimalTTFTextSize,           setMinimalTTFTextSize,           getDouble);
ODRX_DEFINE_PROPERTY_METHODS(GradientsAsBitmap,         ExGsGDIVectorizeDevice, gradientsAsBitmap,            setGradientsAsBitmap,            getBool);
ODRX_DEFINE_PROPERTY_METHODS(GradientsAsPolys,          ExGsGDIVectorizeDevice, gradientsAsPolygons,          setGradientsAsPolygons,          getBool);
ODRX_DEFINE_PROPERTY_METHODS(GradientsAsPolysThreshold, ExGsGDIVectorizeDevice, gradientsAsPolygonsThreshold, setGradientsAsPolygonsThreshold, getUInt8);

ExGsGDISimpleVectorizeDevice::ExGsGDISimpleVectorizeDevice()
{
  m_toGDI.setDestGeometry(*this);
}

void ExGsGDISimpleVectorizeDevice::onSize(const OdGsDCRect& outputRect)
{
  {
    OdGsDCRect orNorm(odmin(outputRect.m_min.x, outputRect.m_max.x), odmax(outputRect.m_min.x, outputRect.m_max.x),
                      odmax(outputRect.m_min.y, outputRect.m_max.y), odmin(outputRect.m_min.y, outputRect.m_max.y));
    ExGiGDIGeometry::setOutputRect(orNorm);
  }
  OdGsBaseVectorizeDevice::onSize(outputRect);
}

void ExGsGDISimpleVectorizeDevice::setLogicalPalette(const ODCOLORREF* palette, int nCount)
{
  OdGsBaseVectorizeDevice::setLogicalPalette(palette, nCount);
  ExGiGDIGeometry::setLogicalPalette(m_logPalette.asArrayPtr(), OdUInt16(nCount));
  { // Windows GDI needs 0 in alpha component
    ODCOLORREF cMask = ODRGB(255, 255, 255);
    ODCOLORREF *pArray = m_logPalette.asArrayPtr();
    for (int i = 0; i < nCount; i++)
    {
      pArray[i] &= cMask;
    }
  }
}

bool ExGsGDISimpleVectorizeDevice::setBackgroundColor(ODCOLORREF color)
{
  return OdGsBaseVectorizeDevice::setBackgroundColor(color & ODRGB(255, 255, 255));
}

OdGiConveyorInput& ExGsGDISimpleVectorizeDevice::input()
{
  return m_toGDI;
}

OdGiPlotGenerator& ExGsGDISimpleVectorizeDevice::plotGenerator()
{
  if(m_pPlotstyler.isNull())
  {
    m_pPlotstyler = OdGiPlotGenerator::createObject();
    input().addSourceNode(m_pPlotstyler->output());
    OdGeDoubleArray devs(5, 1);
    devs.insert(devs.end(), 5, .5);
    m_pPlotstyler->setDeviation(devs);
#ifndef _WIN32_WCE
    m_pPlotstyler->setExternalPolylineOut(this);
#endif
  }
  return *m_pPlotstyler;
}

void ExGsGDIVectorizeDevice::onSize(const OdGsDCRect& outputRect)
{
  ExGsGDISimpleVectorizeDevice::onSize(outputRect);
  if(m_hbmScreen) // substitute the bitmap
  {
    if (m_nColorDepth > 8)
    {
      // Work's stable only for 16, 24, 32
      m_hbmScreen = ::CreateCompatibleBitmap(hdc(), width(), height());
    }
    else
    {
      // AMark: 12.03.2008
      // For palette double buffering (true color of selected device will be converted in BitBlt)
      HBITMAP devBitmap = (HBITMAP)::GetCurrentObject(hdc(), OBJ_BITMAP);
      BITMAP bm;
      ::GetObject(devBitmap, sizeof(BITMAP), &bm);
      m_hbmScreen = ::CreateBitmap(bm.bmWidth, bm.bmHeight, ::GetDeviceCaps(hdc(), PLANES), ::GetDeviceCaps(hdc(), BITSPIXEL), NULL);
    }
    if(m_hbmScreen)
    {
      ODA_VERIFY(::DeleteObject(::SelectObject(*m_hOffscreenDC, m_hbmScreen)));
#ifndef _WIN32_WCE
      ::SetWindowOrgEx(*m_hOffscreenDC,
        OdRoundToLong(odmin(m_outputRect.m_min.x, m_outputRect.m_max.x)),
        OdRoundToLong(odmin(m_outputRect.m_min.y, m_outputRect.m_max.y)),
        NULL);
#endif
    }
    else // no space for double buffer - turning it off
    {
      std::auto_ptr<HDCStore> tmp(new HDCStore( hdc() ));
      m_hOffscreenDC = tmp;
    }
  }
}

HDC ExGsGDIVectorizeDevice::createDrawDc()
{
  if(doubleBufferEnabled() && !userGiContext()->isPlotGeneration())
  {
    std::auto_ptr<HDCStore> tmp( new HDCAutoDestroyStore( ::CreateCompatibleDC(hdc()) ) );
    m_hOffscreenDC = tmp;
    if(m_hOffscreenDC.get())
    {
      if (m_nColorDepth > 8)
      {
        // Work's stable only for 16, 24, 32
        m_hbmScreen = ::CreateCompatibleBitmap(hdc(), width(), height());
      }
      else
      {
        // AMark: 12.03.2008
        // For palette double buffering (true color of selected device will be converted in BitBlt)
        HBITMAP devBitmap = (HBITMAP)::GetCurrentObject(hdc(), OBJ_BITMAP);
        if (devBitmap != 0)
        {
          BITMAP bm;
          ::GetObject(devBitmap, sizeof(BITMAP), &bm);
          m_hbmScreen = ::CreateBitmap(bm.bmWidth, bm.bmHeight, ::GetDeviceCaps(hdc(), PLANES), ::GetDeviceCaps(hdc(), BITSPIXEL), NULL);
        } 
        else 
        {
          m_hbmScreen = 0;
        }
      }
      if(m_hbmScreen)
      {
        ::SelectObject(*m_hOffscreenDC, m_hbmScreen);
#ifndef _WIN32_WCE
      ::SetWindowOrgEx(*m_hOffscreenDC,
        OdRoundToLong(odmin(m_outputRect.m_min.x, m_outputRect.m_max.x)),
        OdRoundToLong(odmin(m_outputRect.m_min.y, m_outputRect.m_max.y)),
        NULL);
#endif
      }
      else
      {
        std::auto_ptr<HDCStore> tmp(0);//.reset(0);
        m_hOffscreenDC = tmp;
      }
    }
  }
  if(!m_hOffscreenDC.get())
  {
    std::auto_ptr<HDCStore> tmp(new HDCStore( hdc() ));
    m_hOffscreenDC = tmp;
  }
  return drawDc();
}

void ExGsGDIVectorizeDevice::deleteDrawDc()
{
  if(m_hbmScreen)
  {
    ::DeleteObject(m_hbmScreen);
    m_hbmScreen = 0;
  }
  std::auto_ptr<HDCStore> tmp(0);//.reset(0);
  m_hOffscreenDC = tmp;
}

void ExGsGDIVectorizeDevice::flushDrawDc()
{
  if(m_hbmScreen && !userGiContext()->regenAbort())
  {
#ifndef _WIN32_WCE
    ::SetWindowOrgEx(*m_hOffscreenDC, 0, 0, NULL);
#endif
    ::BitBlt(hdc(), OdRoundToLong(odmin(m_outputRect.m_min.x, m_outputRect.m_max.x)),
                    OdRoundToLong(odmin(m_outputRect.m_min.y, m_outputRect.m_max.y)),
                    width(), height(), drawDc(), 0, 0, SRCCOPY);
  }
}

void ExGsGDIVectorizeDevice::HDCWasChanged()
{
  m_bContextValid = false;
  deleteDrawDc();
}

//*******************************************************************************/
// BeginPaint
//*******************************************************************************/
void ExGsGDIVectorizeDevice::beginPaint()
{
  // for debugging
  // m_Background = ODRGB(BYTE(rand() * 255 / RAND_MAX), BYTE(rand() * 255 / RAND_MAX), BYTE(rand() * 255 / RAND_MAX));

  ExGiGDIGeometry::draw_color(m_Background);
  ExGiGDIGeometry::beginPaint(drawDc());
  ExGiGDIGeometry::setSelection(ExGsWin32Device::isDrawSelection());
  ExGiGDIGeometry::setDragging(ExGsWin32Device::isDrawDragging());

  if(invalidRects().size())
    ExGiGDIGeometry::pushViewportClip(invalidRects().size(), invalidRects().getPtr());

  if(m_bClearScreen && !isValid())
    ::Rectangle(drawDc(), m_outputRect.m_min.x, m_outputRect.m_max.y, m_outputRect.m_max.x, m_outputRect.m_min.y);
}


//*******************************************************************************/
// EndPaint
//*******************************************************************************/
void ExGsGDIVectorizeDevice::endPaint()
{
  if (invalidRects().size())
    ExGiGDIGeometry::popViewportClip();

  ExGiGDIGeometry::endPaint();
  flushDrawDc();
}

bool ExGsGDIVectorizeDevice::supportPartialUpdate() const
{
  return true;
}

//*******************************************************************************/
// update
//*******************************************************************************/
void ExGsGDIVectorizeDevice::updateScreen()
{
  if(!m_bContextValid)
    createContext();

  if(!doubleBufferEnabled() && !compositionEnabled())
    invalidate();

  beginPaint();

  ExGsBaseGDIVectorizeDevice::updateScreen();

  endPaint();
}

void ExGsGDIVectorizeDevice::update(OdGsDCRect* pUpdatedRect)
{
  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  if (useTtfCache() && m_pTtfCache.isNull())
    m_pTtfCache = OdTtfFontsCache::createObject(this);
  else if (!useTtfCache() && !m_pTtfCache.isNull())
    m_pTtfCache.release();

  if (!supportPartialUpdate())
  {
    if(!m_bContextValid)
      createContext();

    if(!doubleBufferEnabled())
      invalidate();

    beginPaint();
  }

  ExGsBaseGDIVectorizeDevice::update(pUpdatedRect);

  if (!supportPartialUpdate())
  {
    endPaint();
  }
}

//*******************************************************************************/
//  createContext()
//*******************************************************************************/
void ExGsGDIVectorizeDevice::createContext()
{
  m_bColorIndexMode = false;
  if (m_nColorDepth < 16)
  {
    m_bColorIndexMode = true;
  }
  createPalette(&m_logPalette);
  
  createDrawDc();

  m_bContextValid = true;
}

// conveyor:
// output      ->    ZClip -> PerspPrepr -> PerspXform -> HLR -> device
//     \-> metafiler -> MF  |
//   MF -> mfPlayXform  -->-/

ExGDIVectorizeView::ExGDIVectorizeView()
  : m_bCurHLTState(false) // !dynamicSubEntHltEnabled
  , m_gsHLTMarker(kNullSubentIndex) // dynamicSubEntHltEnabled
  , m_bProcessingTTF(false)
  , m_bCurTTFState(false)
  , m_bInexactSelection(true)
  , m_pPrevSelGeom(NULL)
  , m_bIsLineTyperWasAddedAfterMetafile(false) 
{
  m_GeometryDestForPlaying.setGDIViewPlayEntryPoint(&m_mfPlayEntryPoint);

  m_pMfPlay = OdGiXform::createObject();
  m_pMfPlay->input().addSourceNode(m_mfPlayEntryPoint);

  m_pMetafiler = OdGiMetafiler::createObject();

  m_pZClip = OdGiOrthoClipper::createObject();
  m_pZClip->input().addSourceNode(output());
  m_pZClip->input().addSourceNode(m_pMfPlay->output());

  m_pPerspPrepr = OdGiPerspectivePreprocessor::createObject();
  m_pPerspPrepr->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  m_pPerspPrepr->input().addSourceNode(m_pZClip->output());

  m_pPerspXform = OdGiXform::createObject();
  m_pPerspXform->input().addSourceNode(m_pPerspPrepr->output());

  m_pHLR = OdGiHLRemover::createObject();
  OdGeDoubleArray devs(5, 1);
  devs.insert(devs.end(), 5, .5);
  m_pHLR->setDeviation(devs); // always DC deviation
  m_pHLR->input().addSourceNode(m_pPerspXform->output());

  m_pVptClipper = OdGiGDIViewportClipping::createObject();
  m_pVptClipper->setDeviation(devs);
  m_pVptClipper->input().addSourceNode(m_pHLR->output());
}

void ExGDIVectorizeView::setView(const OdGePoint3d & position,
  const OdGePoint3d& target,
  const OdGeVector3d& upVector,
  double fieldWidth,
  double fieldHeight,
  Projection projection)
{
  OdGsBaseVectorizeView::setView(position, target, 
    upVector, fieldWidth, fieldHeight, projection);
}

void ExGDIVectorizeView::loadViewport()
{
  OdGsBaseVectorizer::loadViewport();
}

void ExGDIVectorizeView::pushViewportClipping(bool bForceWinClip)
{
  if(view().isNonRectClipped())
  {
    OdGsDCPointArray pts;
    OdIntArray counts;
    view().viewportClipRegion(counts, pts);
    if (bForceWinClip ||
        !giContext().isPlotGeneration() ||
        !m_pVptClipper->push(pts.getPtr(), counts.getPtr(), counts.size()))
      device()->pushViewportClip(pts.getPtr(), counts.getPtr(), counts.size());
  }
  else
  {
    OdGsDCPoint dcPoint1, dcPoint2;
    view().screenRect(dcPoint1, dcPoint2);
    if (bForceWinClip ||
        !giContext().isPlotGeneration() ||
        !m_pVptClipper->push(dcPoint1, dcPoint2))
      device()->pushViewportClip(dcPoint1, dcPoint2);
  }
}

void ExGDIVectorizeView::popViewportClipping(bool bForceWinClip)
{
  if (isViewportClippingWinClip() || bForceWinClip)
    device()->popViewportClip();
  else
    m_pVptClipper->pop();
}

bool ExGDIVectorizeView::isViewportClippingWinClip()
{
#ifndef GDI_USE_ENHANCED_CLIPPER
  return !m_pVptClipper->optionalGeometry();
#else // GDI_USE_ENHANCED_CLIPPER
  return !m_pVptClipper->isClipping();
#endif // GDI_USE_ENHANCED_CLIPPER
}

void ExGDIVectorizeView::beginViewVectorization()
{
  OdGsBaseVectorizer::beginViewVectorization();

  if (isOutputSuppressed())
    return;
    
  device()->mtDeviceSync().lock(OdGsBaseDeviceMTHelpers::kSyncDeviceAccess);

  device()->setDrawContext(drawContext());
  updateLineweightOverride(OdGiLineweightOverride());
  m_pMetafiler->setDrawContext(drawContext());
  m_pHLR->setDrawContext(drawContext()); // to propagate renderMode
  m_pVptClipper->setDrawContext(drawContext());
  setDrawContextFlags(drawContextFlags() | kForceMarkersOnModified, true);
  m_pPrevSelGeom = NULL;

  if(giContext().isPlotGeneration())
  {
    OdGiPlotGenerator& pgen = device()->plotGenerator();
    pgen.setDrawContext(drawContext());
    OdInt32 dpi = OdRxVariantValue(device()->properties()->getAt(OD_T("DPI")))->getInt32();
    pgen.dot_per_inch(dpi);
    pgen.input().addSourceNode(screenOutput());
  }
  else
  {
    device()->input().addSourceNode(screenOutput());
  }

  pushViewportClipping();

  device()->mtDeviceSync().unlock(OdGsBaseDeviceMTHelpers::kSyncDeviceAccess);

  /*
  if(device()->clearScreen() && (clearColor() != kTransparent))
  {
    if(clearColor() == kDeviceBackground)
      device()->draw_color(device()->getBackgroundColor());
    else
      device()->draw_color(device()->getPaletteBackground());
    device()->draw_fillStyle(ExGiGDIGeometry::kFsSolid);
    device()->updatePenNBrush();

    OdGsDCPoint dcPoint1, dcPoint2;
    screenRect(dcPoint1, dcPoint2);
    ::Rectangle(device()->drawDc(), 
      odmin(dcPoint1.x, dcPoint2.x), 
      odmin(dcPoint1.y, dcPoint2.y), 
      odmax(dcPoint1.x, dcPoint2.x), 
      odmax(dcPoint1.y, dcPoint2.y));
  }
  */

  bool bFrontClp = view().isFrontClipped();
  bool bBackClp = view().isBackClipped();
  
  double frClp = view().frontClip();
  double bkClp = view().backClip();
  
  if(view().isPerspective())
  {
    if(bFrontClp)
      frClp = odmin(frClp, view().focalLength() * 0.9);
    else
      frClp = view().focalLength() * 0.9;
    bFrontClp = true;

    m_pPerspPrepr->enable(true);  
  }
  else
  {
    m_pPerspPrepr->enable(false);  
  }

  m_pZClip->set(0, NULL, bBackClp, bkClp, bFrontClp, frClp);
  m_pZClip->setDrawContext(drawContext());
  m_pZClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());

  // Disable linetyper by default in the before metafile conveyor part
  if ( isNeedActionWithLineTyper() )
  {
    removeLineTyperBeforeMetafile();

    setDrawContextFlags(drawContextFlags() | kLineTyperAfterMetafile, true);

    m_pMfPlay->input().removeSourceNode(output());
    if(view().cachedDrawables())
       m_pMfPlay->input().addSourceNode(output());
  
    //In this case the metafile will be written in WCS
    if ( giContext().isPlotGeneration() )
    {
      m_pMetafiler->setDeviation(&m_pModelToEyeProc->worldDeviation());
      m_pMetafiler->setCoordinatesType(OdGiMetafiler::kWorld);

      setEyeToOutputTransform(getEyeToWorldTransform());
      m_pMfPlay->setTransform(getWorldToEyeTransform());
      m_mfTransformStack.setOutputTransform(getWorldToEyeTransform()); 
      m_pPerspXform->setTransform(view().eyeToScreenMatrix());
    }
    //LINETYPE: CHANGE #3
    else
    {
      m_pMetafiler->setDeviation(&m_pModelToEyeProc->eyeDeviation());
      m_pMetafiler->setCoordinatesType(OdGiMetafiler::kEye);

      setEyeToOutputTransform(OdGeMatrix3d::kIdentity);
      m_pMfPlay->setTransform(OdGeMatrix3d::kIdentity);
      m_mfTransformStack.setOutputTransform(OdGeMatrix3d::kIdentity);
      m_pPerspXform->setTransform(view().eyeToScreenMatrix());
    }
  }
  else
  {
    m_pMfPlay->input().removeSourceNode(output());
    if(view().cachedDrawables())
    {
      m_pMetafiler->setDeviation(&m_pModelToEyeProc->worldDeviation());
      m_pMetafiler->setCoordinatesType(OdGiMetafiler::kWorld);

      setEyeToOutputTransform(getEyeToWorldTransform());
      m_pMfPlay->input().addSourceNode(output());
      m_pMfPlay->setTransform(getWorldToEyeTransform());
      m_mfTransformStack.setOutputTransform(getWorldToEyeTransform());
      m_pPerspXform->setTransform(view().eyeToScreenMatrix());
    }
    else
    {
      m_pMetafiler->setDeviation(&m_pModelToEyeProc->eyeDeviation());
      if(bBackClp || bFrontClp)
      {
        m_pMetafiler->setCoordinatesType(OdGiMetafiler::kEye);
        setEyeToOutputTransform(OdGeMatrix3d::kIdentity);
        m_pPerspXform->setTransform(view().eyeToScreenMatrix());
      }
      else
      {
        m_pMetafiler->setCoordinatesType(OdGiMetafiler::kDevice);
        setEyeToOutputTransform(view().eyeToScreenMatrix());
        m_pPerspXform->setTransform(OdGeMatrix3d::kIdentity);
      }
      m_pMfPlay->setTransform(OdGeMatrix3d::kIdentity);
      m_mfTransformStack.setOutputTransform(OdGeMatrix3d::kIdentity);
    }
  }

  m_pPerspPrepr->setDrawContext(drawContext());

  m_pHLR->invertZ(true);
  m_pHLR->enable(device()->useSoftwareHLR() && (view().mode() == kHiddenLine));
  m_pHLR->setDrawContext(drawContext());
}

void ExGDIVectorizeView::endViewVectorization()
{
  if (!isOutputSuppressed())
  {
    if(device()->useSoftwareHLR() && (view().mode() == kHiddenLine))
      m_pHLR->process();

    device()->mtDeviceSync().lock(OdGsBaseDeviceMTHelpers::kSyncDeviceAccess);

    if (m_pPrevSelGeom) // #15164
      enableScreenOutput(true);
    popViewportClipping();

    if(giContext().isPlotGeneration())
    {
      device()->plotGenerator().input().removeSourceNode(screenOutput());
    }
    else
    {
      device()->input().removeSourceNode(screenOutput());
    }

    device()->mtDeviceSync().unlock(OdGsBaseDeviceMTHelpers::kSyncDeviceAccess);

    m_mfTransformStack.setOutputTransform(OdGeMatrix3d::kIdentity);
    m_mfTransformStack.checkMetafileTransform();

    setSelectionMarker(0);

    // Restore linetyper in the before metafile conveyor part
    if ( isNeedActionWithLineTyper() )
      addLineTyperBeforeMetafile();
  }

  OdGsBaseVectorizer::endViewVectorization();
}

struct RecLineTyper : OdGiGeometryMetafile::Record
{
  bool m_bRemoveLineTyper;
  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    pView->updateLineTyper(m_bRemoveLineTyper);
  }
  static void updateLineTyper(OdGiMetafiler* pMetafiler, bool bRemove)
  {
    if ( pMetafiler && pMetafiler->metafile() ) // this condition is just for safety. Formally here we always should have the metafile
    {
      RecLineTyper* pRec;
      pMetafiler->add(pRec = new RecLineTyper);
      pRec->m_bRemoveLineTyper = bRemove;
    }
  }
  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

void ExGDIVectorizeView::pushClipBoundary(OdGiClipBoundary* pBoundary)
{
  bool bInitialClipped = m_pModelToEyeProc->isClipping();

  OdGiBaseVectorizer::pushClipBoundary(pBoundary);

  bool bCurrentClipped = m_pModelToEyeProc->isClipping();

  if ( (bInitialClipped != bCurrentClipped) && isNeedActionWithLineTyper() && !isOutputSuppressed() )
  {
    addLineTyperBeforeMetafile();
    RecLineTyper::updateLineTyper(m_pMetafiler.get(), true);
  }
}

void ExGDIVectorizeView::pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo)
{
  bool bInitialClipped = m_pModelToEyeProc->isClipping();

  OdGiBaseVectorizer::pushClipBoundary(pBoundary, pClipInfo);

  bool bCurrentClipped = m_pModelToEyeProc->isClipping();

  if ( (bInitialClipped != bCurrentClipped) && isNeedActionWithLineTyper() && !isOutputSuppressed() )
  {
    addLineTyperBeforeMetafile();
    RecLineTyper::updateLineTyper(m_pMetafiler.get(), true);
  }
}

void ExGDIVectorizeView::popClipBoundary()
{
  bool bInitialClipped = m_pModelToEyeProc->isClipping();

  OdGiBaseVectorizer::popClipBoundary();

  bool bCurrentClipped = m_pModelToEyeProc->isClipping();

  if ( (bInitialClipped != bCurrentClipped) && isNeedActionWithLineTyper() && !isOutputSuppressed() )
  {
    removeLineTyperBeforeMetafile();
    RecLineTyper::updateLineTyper(m_pMetafiler.get(), false);
  }
}

void ExGDIVectorizeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  if (!device()->supportDynamicSubhighlight())
  {
    if (!device()->useTextOut() && isModelCacheEnabled())
      m_bInexactSelection = !OdGiSelectProc::supportsSubentitySelection(this, mode, pReactor);
    else
      m_bInexactSelection = false;
  }
  else if (device()->useTextOut())
    m_bInexactSelection = false;
  class DetachMfOutput { OdGiConveyorInput &mfInput; OdGiConveyorOutput &vecOutput;
    public: DetachMfOutput(OdGiConveyorInput &_mfInput, OdGiConveyorOutput &_vecOutput) : mfInput(_mfInput), vecOutput(_vecOutput)
            { mfInput.removeSourceNode(vecOutput); }
            ~DetachMfOutput() { mfInput.addSourceNode(vecOutput); }
  } _detachMfOutput(m_pMfPlay->input(), output());
  m_pPerspPrepr->setDrawContext(drawContext());
  OdGsBaseVectorizeView::select(pts, nPoints, pReactor, mode);
  m_bInexactSelection = true;
}

void ExGDIVectorizeView::setMode(OdGsView::RenderMode mode)
{
  if (((mode == kHiddenLine) && (!device()->useSoftwareHLR())) || (mode > kHiddenLine))
    mode = kWireframe;
  OdGsBaseVectorizeView::setMode(mode);
}

// !dynamicSubEntHltEnabled
struct RecHighlight : OdGiGeometryMetafile::Record
{
  bool m_bState;

  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    pView->highlight(m_bState);
  }

  struct RecTM : OdGiGeometryMetafile::Record
  {
    void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
    {
      pCtx->onTraitsModified();
    }
    virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
  };

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

// dynamicSubEntHltEnabled
struct RecSelectionMarker : OdGiGeometryMetafile::Record
{
  OdGsMarker m_gsMarker;

  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    switch (pView->metafilePlayMode())
    {
      case kMfDisplay:
        {
          pView->setSelectionMarker(m_gsMarker);
          pView->onTraitsModified();
        }
      break;
      case kMfSelect:
        {
          pView->setSelectionMarker(m_gsMarker);
        }
      break;
      // does nothing for kMfExtents
    }
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

struct RecTTF : OdGiGeometryMetafile::Record
{
  bool m_bProcessTTF;
  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    pView->device()->enableTTFProcessing(m_bProcessTTF);
  }
  static void saveTraits(OdGiMetafiler* pMetafiler, bool bFlag)
  {
    RecTTF* pRec;
    pMetafiler->add(pRec = new RecTTF);
    pRec->m_bProcessTTF = bFlag;
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

inline bool fillMode(OdGsView::RenderMode mode, const OdGiSubEntityTraitsData &effTraits)
{
  return (effTraits.fillType() == kOdGiFillAlways) &&
    ((mode != OdGsView::kWireframe) || GETBIT(effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill | OdGiSubEntityTraits::kDrawSolidFill | OdGiSubEntityTraits::kDrawPolygonFill));
}

void ExGDIVectorizeView::onTraitsModified()
{
  OdGsBaseVectorizer::onTraitsModified();

  if (isOutputSuppressed())
    return;

  ExGsGDIVectorizeDevice* pDevice = device();
  if(m_pMetafiler->metafile())
  {
    if (!pDevice->supportDynamicSubhighlight())
    {
      RecHighlight *pRHLT = NULL;
      if (m_bCurHLTState != isHighlighted())
      {
        m_pMetafiler->flush(false);
        m_pMetafiler->add(pRHLT = new RecHighlight);
        pRHLT->m_bState = m_bCurHLTState = isHighlighted();
      }
      if (m_pMetafiler->saveTraits(effectiveTraits(), byBlockTraits()))
      {
        if (pRHLT != NULL)
        {
          m_pMetafiler->add(new RecHighlight::RecTM);
        }
      }
    }
    else
    {
      if (gsWriter().gsModel()) // Skip if cache disabled
      {
        OdGsMarker curMarker = selectionMarker();
        if (m_gsHLTMarker != curMarker)
        {
          bool bSkipNested = false;
          if (isNestedPersistent(currentGiPath()) && !device()->supportBlocks())
          {
            if (m_gsHLTMarker != kNullSubentIndex)
              curMarker = kNullSubentIndex;
            else
              bSkipNested = true;
          }
          if (!bSkipNested)
          {
            RecSelectionMarker *pRHLT = NULL;
            m_pMetafiler->flush(false);
            m_pMetafiler->add(pRHLT = new RecSelectionMarker);
            pRHLT->m_gsMarker = curMarker;
          }
          m_gsHLTMarker = curMarker;
        }
      }
      m_pMetafiler->saveTraits(effectiveTraits(), byBlockTraits());
    }
    if (m_bCurTTFState != this->m_bProcessingTTF)
    {
      RecTTF::saveTraits(m_pMetafiler.get(), m_bCurTTFState = this->m_bProcessingTTF);
    }
    // All next code is used during display only. It's redundant to call it if we process metafile.
    return;
  }
  else if (pDevice->supportDynamicSubhighlight())
  {
    const OdGsHlBranch *pBranch = getCurrentHLTBranch();
    if (pBranch && !pBranch->markers().isEmpty() && pBranch->id() == currentGiPath()->persistentDrawableId())
    {
      bool bPrevState = isHighlighted();
      bool bNewState = false;
      if (selectionMarker() != kNullSubentIndex)
        bNewState = pBranch->hasMarker(selectionMarker());
      if (bNewState != bPrevState)
      {
        highlight(bNewState);
      }
    }
  }

  if (!!m_pPrevSelGeom != GETBIT(OdGiBaseVectorizerImpl::m_flags, kSelectionGeometry))
    enableScreenOutput(!GETBIT(OdGiBaseVectorizerImpl::m_flags, kSelectionGeometry));

  bool bDrawDragging = pDevice->isDrawDragging();
  bool bDrawSelection = pDevice->isDrawSelection();
  if (isFaded())
    pDevice->draw_globalAlpha(true, 255 - (int)(2.55 * fadingIntensity()), pDevice->getPaletteBackground());
  else
    pDevice->draw_globalAlpha(false);
  if(bDrawSelection)
  {
    pDevice->draw_bkMode(OPAQUE); // selection is drawn always opaque
    const OdGiSubEntityTraitsData& entTraits = effectiveTraits();
    pDevice->draw_lineweight(0);
    pDevice->draw_color(entTraits.trueColor());
    pDevice->draw_pslinetype(ExGiGDIGeometry::kLtpForSelection);
    pDevice->draw_fillStyle(fillMode(view().mode(), entTraits) ?
                            ExGiGDIGeometry::kFsForSelection : ExGiGDIGeometry::kFsHollow);
  }
  else if (!isSupportPlotStyles() ||
           bDrawDragging) // needn't to apply plotstyles when dragging
  {
    const OdGiSubEntityTraitsData& entTraits = effectiveTraits();
    if(bDrawDragging)
    {
      OdCmEntityColor col = entTraits.trueColor();
      ODCOLORREF rgb;
      if(col.isByACI() || col.isByDgnIndex())
        rgb = paletteColor(col.colorIndex());
      else
        rgb = ODRGB(col.red(), col.green(), col.blue());
      rgb ^= pDevice->getPaletteBackground();
      pDevice->draw_color(rgb);
    }
    else
    {
      pDevice->draw_color(entTraits.trueColor());
    }
    pDevice->draw_lineweight(lineweightToPixels(entTraits.lineWeight()));
    if(!isHighlighted())
    {
      pDevice->draw_pslinetype(ExGiGDIGeometry::kLtpSolid);
      pDevice->draw_fillStyle(fillMode(view().mode(), entTraits) ?
                              ExGiGDIGeometry::kFsSolid : ExGiGDIGeometry::kFsHollow);
    }
    else
    {
      bDrawSelection = true;
      pDevice->draw_pslinetype(ExGiGDIGeometry::kLtpForSelection);
      pDevice->draw_fillStyle(fillMode(view().mode(), entTraits) ?
                              ExGiGDIGeometry::kFsForSelection : ExGiGDIGeometry::kFsHollow);
    }
  }
  else
  {
    const OdPsPlotStyleData& plotStyle = effectivePlotStyle();
    pDevice->draw_color(plotStyle.color());
    if(giContext().isPlotGeneration())
    {
      pDevice->draw_lineweight(1);
      pDevice->draw_pslinetype(ExGiGDIGeometry::kLtpSolid);
      pDevice->draw_fillStyle(ExGiGDIGeometry::kFsSolid);
      pDevice->plotGenerator().setPlotStyle(plotStyle);
    }
    else
    {
      pDevice->draw_lineweight(OdTruncateToLong(lineweightToPixels(plotStyle.lineweight())));
      pDevice->draw_pslinetype(ExGiGDIGeometry::LineType(plotStyle.linetype()));
      pDevice->draw_endStyle(plotStyle.endStyle());
      pDevice->draw_joinStyle(plotStyle.joinStyle());
      pDevice->draw_fillStyle(ExGiGDIGeometry::FillStyle(plotStyle.fillStyle()));
    }
    pDevice->draw_bkMode(TRANSPARENT); // plotstyles are drawn always using transparent background mode
  }
  pDevice->updatePenNBrush(bDrawSelection);
}

void ExGDIVectorizeView::enableScreenOutput(bool bEnable)
{
  if (!bEnable)
  {
    m_pPrevSelGeom = &screenOutput().destGeometry();
    screenOutput().setDestGeometry(OdGiEmptyGeometry::kVoid);
  }
  else
  {
    screenOutput().setDestGeometry(*m_pPrevSelGeom);
    m_pPrevSelGeom = NULL;
  }
}

void ExGDIVectorizeView::renderTypeOnChange(OdGsModel::RenderType renderType)
{
  OdGsBaseVectorizer::renderTypeOnChange(renderType);
  if (device() && device()->useSoftwareHLR() && (view().mode() == kHiddenLine))
  {
    const OdUInt32 overlayFlags = OdGsOverlayMapping::overlayFlags(
      OdGsOverlayMapping::overlayRenderingOrderToIndex(device()->renderTypeWeight(renderType)));
    if (GETBIT(overlayFlags, OdGsOverlayMapping::kNoDepth))
    {
      if (m_pHLR->enabled())
      {
        m_pHLR->process();
        m_pHLR->enable(false);
      }
    }
  }
}

struct RecLineweightOverride : OdGiGeometryMetafile::Record
{
  OdGiLineweightOverride m_lwdOverride;
  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    const bool bPushOverride = m_lwdOverride.hasOverrides();
    const bool bPrevOverride = pView->hasLineweightOverride() || pView->hasLinestyleOverride();
    if (bPrevOverride)
      pView->popLineweightOverride();
    if (bPushOverride)
    {
      if (m_lwdOverride.hasScaleOverride())
      {
        OdGiLineweightOverride lwdOverride = m_lwdOverride;
        OdGePoint2d nMul;
        pCtx->giViewport()->getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, nMul, false);
        lwdOverride.setScaleOverride(lwdOverride.scaleOverride() * nMul.y);
        pView->pushLineweightOverride(&lwdOverride);
      }
      else
        pView->pushLineweightOverride(&m_lwdOverride);
    }
  }
  static void saveTraits(OdGiMetafiler* pMetafiler, const OdGiLineweightOverride &lwdOverride, OdGiBaseVectorizerImpl *pVect)
  {
    RecLineweightOverride* pRec;
    pMetafiler->add(pRec = new RecLineweightOverride);
    pRec->m_lwdOverride = lwdOverride;
    if (lwdOverride.hasScaleOverride())
    {
      OdGePoint2d nDiv;
      pVect->getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, nDiv, false);
      pRec->m_lwdOverride.setScaleOverride(lwdOverride.scaleOverride() / nDiv.y);
    }
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

void ExGDIVectorizeView::updateLineweightOverride(const OdGiLineweightOverride &lwdOverride)
{
  if (isOutputSuppressed())
    return;

  if (!m_pMetafiler->metafile() && gsWriter().isRecordingMetafile())
    onTraitsModified();

  if (m_pMetafiler->metafile())
    RecLineweightOverride::saveTraits(m_pMetafiler.get(), lwdOverride, this);
  else
  {
    OdGiBaseVectorizerImpl::updateLineweightOverride(lwdOverride);
    if (!hasLinestyleOverride())
      device()->setLineWeightSettings(giContext().lineWeightConfiguration(OdGiContext::kPointLineWeight) > 0,
                  (OdPs::LineEndStyle)giContext().lineWeightConfiguration(OdGiContext::kLineCapStyle),
                 (OdPs::LineJoinStyle)giContext().lineWeightConfiguration(OdGiContext::kLineJoinStyle));
    else
      device()->setLineWeightSettings(device()->isPointsLineWeightEnabled(), lwdOverride.endStyleOverride(), lwdOverride.joinStyleOverride());
  }
}

struct RecPaletteOverride : OdGiGeometryMetafile::Record
{
  OdGiPalettePtr m_pPalette;
  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    pView->pushPaletteOverride(m_pPalette);
  }
  OdUInt64 palSize() const { return (m_pPalette.isNull()) ? 0 : sizeof(OdGiPalette); }
  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this) + palSize(); }
};
struct RecPaletteOverrideEOF : OdGiGeometryMetafile::Record
{
  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    pView->popPaletteOverride();
  }
  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

bool ExGDIVectorizeView::pushPaletteOverride(const OdGiPalette* pOverride)
{
  if (isOutputSuppressed())
    return false;

  if (!m_pMetafiler->metafile() && gsWriter().isRecordingMetafile())
    onTraitsModified();

  if (m_pMetafiler->metafile())
  {
    RecPaletteOverride* pRec;
    m_pMetafiler->add(pRec = new RecPaletteOverride);
    if (pOverride)
      pRec->m_pPalette = pOverride->cloneIfNeed();
    return true;
  }
  else
    return OdGiBaseVectorizerImpl::pushPaletteOverride(pOverride);
}

void ExGDIVectorizeView::popPaletteOverride()
{
  if (isOutputSuppressed())
    return;

  if (m_pMetafiler->metafile())
    m_pMetafiler->add(new RecPaletteOverrideEOF);
  else
    OdGiBaseVectorizerImpl::popPaletteOverride();
}

void ExGDIVectorizeView::updatePaletteOverride(const OdGiPalette *pPalette)
{
  OdGiBaseVectorizerImpl::updatePaletteOverride(pPalette);
  int numColors = 0; const ODCOLORREF *pColor = getPalette(&numColors);
  device()->ExGiGDIGeometry::setLogicalPalette(pColor, OdUInt16(numColors));
}

void ExGDIVectorizeView::drawViewportFrame()
{
  if(context()->isPlotGeneration() || !view().isViewportBorderVisible())
    return;
  if (device() && device()->useSoftwareHLR() && (view().mode() == kHiddenLine) && m_pHLR->enabled())
  {
    m_pHLR->process();
    m_pHLR->enable(false);
  }
  device()->popViewportClip();
  const OdGsDCRect &dcRect = baseDevice()->outputRect();
  device()->pushViewportClip(dcRect.m_min, dcRect.m_max);
  OdGsBaseVectorizer::drawViewportFrame();
}

// virtual override, to let entitys know that they're drawn for dragging
// (overrides OdGiCommonDraw::isDragging() )
bool ExGDIVectorizeView::isDragging() const
{
  return const_cast<ExGDIVectorizeView*>(this)->device()->isDrawDragging();
}

// OLE caching support
struct RecOwnerDrawDc : OdGiGeometryMetafile::Record
{
  OdGePoint3d             origin;
  OdGeVector3d            u, v;
  OdSmartPtr<OdGiSelfGdiDrawable>  pDrawable;
  bool                    bDcAligned;
  bool                    bAllowClipping;

  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    if (!pView->isViewportClippingWinClip()) pView->pushViewportClipping(true);
    if (pView->isModelCacheEnabled())
    {
      OdGeMatrix3d x = pView->objectToDeviceMatrix();
      pView->device()->ownerDrawDc(
        *static_cast<OdGiViewportDraw*>(pView), x * origin, x * u, x * v, pDrawable, bDcAligned, bAllowClipping
        );
    }
    else
    {
      pView->device()->ownerDrawDc(*static_cast<OdGiViewportDraw*>(pView), origin, u, v, pDrawable, bDcAligned, bAllowClipping);
    }
    if (!pView->isViewportClippingWinClip()) pView->popViewportClipping(true);
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

void ExGDIVectorizeView::ownerDrawDc(const OdGePoint3d& origin,
                                     const OdGeVector3d& u,
                                     const OdGeVector3d& v,
                                     const OdGiSelfGdiDrawable* pDrawable,
                                     bool bDcAligned,
                                     bool bAllowClipping)
{
  if(device()->isDrawSelection())
    return;

  OdGeMatrix3d x;
  OdGsBaseVectorizer::onTraitsModified();
  if(m_pMetafiler->metafile())
  {
    if (isModelCacheEnabled())
    {
      x = eyeToOutputTransform() * getModelToEyeTransform();
    }
    else
    {
      x = objectToDeviceMatrix();
    }
    RecOwnerDrawDc* pRec;
    m_pMetafiler->add(pRec = new RecOwnerDrawDc);
    pRec->origin = x * origin;
    pRec->u = x * u;
    pRec->v = x * v;
    pRec->pDrawable = pDrawable;
    pRec->bDcAligned = bDcAligned && (getRenderMode() == k2DOptimized);
    pRec->bAllowClipping = bAllowClipping;
  }
  else
  {
    x = view().eyeToScreenMatrix() * getModelToEyeTransform();
    if (!isViewportClippingWinClip()) pushViewportClipping(true);
    device()->ownerDrawDc(*static_cast<OdGiViewportDraw*>(this), x * origin, x * u, x * v, pDrawable,
                          bDcAligned && (getRenderMode() == k2DOptimized), bAllowClipping);
    if (!isViewportClippingWinClip()) popViewportClipping(true);
  }
}

// caching
OdRxObjectPtr ExGDIVectorizeView::newGsMetafile()
{
  return OdGiGeometryMetafile::createObject().get();
}

void ExGDIVectorizeView::beginMetafile(OdRxObject* pMetafile)
{
  OdGsBaseVectorizer::beginMetafile(pMetafile);
  OdGiGeometryMetafile* pMFer = static_cast<OdGiGeometryMetafile*>(pMetafile);
  pMFer->clear();
  
  m_pMfPlay->input().removeSourceNode(output());
  m_pMetafiler->input().addSourceNode(output());

  m_pMetafiler->setMetafile(pMFer);

  m_bCurHLTState = isHighlighted(); // !dynamicSubEntHltEnabled
  m_gsHLTMarker = kNullSubentIndex; // dynamicSubEntHltEnabled

  m_bCurTTFState = this->m_bProcessingTTF;
}

void ExGDIVectorizeView::endMetafile(OdRxObject* /*pMetafile*/)
{
  m_nMetafileSize = (OdUInt64)(-1);
  if((m_pMetafiler->metafile() == 0) || m_pMetafiler->metafile()->isEmpty())
  {
    m_nMetafileSize = 0;
  }
  else
  {
    if( !baseDevice()->getUpdateManager().isNull() )
    {
      m_nMetafileSize = m_pMetafiler->metafile()->metafileSize();
    }
  }
  m_pMetafiler->setMetafile(0);
  m_pMetafiler->input().removeSourceNode(output());
  m_pMfPlay->input().addSourceNode(output());
}

void ExGDIVectorizeView::playMetafile(const OdRxObject* pMetafile)
{
  if (metafilePlayMode() != kMfExtents) // dynamicSubEntHltEnabled
    m_nSelectionMarker = kNullSubentIndex;

  addLineTyperAfterMetafile();
  static_cast<const OdGiGeometryMetafile*>(pMetafile)->play(&m_GeometryDestForPlaying,
    drawContext());
  removeLineTyperAfterMetafile();
}

void ExGDIVectorizeView::pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags)
{
  if (GETBIT(uFlags, kSharedRefUpdate)) return;
  if (!m_bInexactSelection && GETBIT(uFlags, kSharedRefSelect)) return;
  m_mfTransformStack.pushMetafileTransform(mtx);
  m_pMfPlay->setTransform(m_mfTransformStack.getCompositeTransform());
}

void ExGDIVectorizeView::popMetafileTransform(OdUInt32 uFlags)
{
  if (GETBIT(uFlags, kSharedRefUpdate)) return;
  if (!m_bInexactSelection && GETBIT(uFlags, kSharedRefSelect)) return;
  m_mfTransformStack.popMetafileTransform();
  m_pMfPlay->setTransform(m_mfTransformStack.getCompositeTransform());
}

void ExGDIVectorizeView::setTransformForMetafileGeometry(const OdGeMatrix3d& mtx)
{
  m_mfTransformStack.setOutputTransform(mtx);
  m_pMfPlay->setTransform(mtx);
}

OdGeMatrix3d ExGDIVectorizeView::getTransformForMetafileGeometry() const
{
  return m_mfTransformStack.outputTransform();
}

struct RecTTFTextJumper;

struct RecTTFTextBegin : public OdGiGeometryMetafile::Record
{
  //OdGePoint3d origin; // Origin for perspective calculation (WCS)
  OdGeVector3d v; // Text height vector (WCS)
  RecTTFTextJumper *pJumper; // Pointer to second jumper, first jumper is in the next record
  OdGiGeometryMetafile::Record *pMarker; // Last known record

  void play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

struct RecTTFTextJumper : public OdGiGeometryMetafile::Record
{
  bool bRestoreTail;
  OdGiGeometryMetafile::Record *pPlaceToRestore;
  
  RecTTFTextJumper() : OdGiGeometryMetafile::Record(), bRestoreTail(false), pPlaceToRestore(NULL) { }

  void play(OdGiConveyorGeometry* /*pGeom*/, OdGiConveyorContext* /*pCtx*/) const
  {
    if (bRestoreTail)
    {
      RecTTFTextJumper *pRec = const_cast<RecTTFTextJumper*>(this);
      pRec->setTail(pPlaceToRestore);
      pRec->bRestoreTail = false;
    }
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

void RecTTFTextBegin::play(OdGiConveyorGeometry* /*pGeom*/, OdGiConveyorContext* pCtx) const
{
  ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
  OdGeMatrix3d mtx = pView->view().worldToDeviceMatrix();
  OdGeVector2d yDir;
  OdGeVector3d vv = v;
  vv.transformBy(mtx);
  yDir.x = vv.x;
  yDir.y = vv.y;
  double height = yDir.length();
  if (height < pView->device()->minimalTTFTextSize() && !pCtx->giContext().isPlotGeneration())
  {
    // Render optimization block and skip text rendering block
    pJumper->bRestoreTail = true;
    pJumper->pPlaceToRestore = pJumper->tail();
    pJumper->setTail(pMarker->tail());
  }
  else
  {
    // Skip optimization block and render text
    RecTTFTextJumper *pJumper = const_cast<RecTTFTextJumper*>(static_cast<const RecTTFTextJumper*>(tail()));
    pJumper->bRestoreTail = true;
    pJumper->pPlaceToRestore = pJumper->tail();
    pJumper->setTail(this->pJumper->tail());
  }
}

#if !defined(_WIN32_WCE)
struct RecTTFTextDc : OdGiGeometryMetafile::Record
{
  OdGePoint3d origin;
  OdString message;
  OdIntArray ulData;
  OdGiTextStyle style;
  OdGeVector3d u, v;
  OdGeVector3d originalSize;
  double extraSizes[3]; // minX, minY, maxY

  void play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    OdGeMatrix3d x = pView->view().worldToDeviceMatrix();
    x.postMultBy(pView->getMetafileTransform());

    OdGeVector3d uNorm = u.normal(), vNorm = v.normal();
    OdGePoint3d textOrigin(origin);
    /*if (!pView->isModelCacheEnabled())
    {
      uNorm.transformBy(x); vNorm.transformBy(x);
      textOrigin.transformBy(x);
    }*/
    double extraSize0 = originalSize.length();
    OdGeBoundBlock3d bbChk(textOrigin + uNorm * extraSizes[0] + vNorm * extraSizes[1],
                           uNorm * (extraSize0 - extraSizes[0]),
                           vNorm * (extraSizes[2] - extraSizes[1]),
                           OdGePoint3d::kOrigin.asVector());
#if 0 // test
    OdGePoint3d tst[5] = { textOrigin + uNorm * extraSizes[0] + vNorm * extraSizes[1],
                           textOrigin + uNorm * extraSize0 + vNorm * extraSizes[1],
                           textOrigin + uNorm * extraSize0 + vNorm * extraSizes[2],
                           textOrigin + uNorm * extraSizes[0] + vNorm * extraSizes[2],
                           textOrigin + uNorm * extraSizes[0] + vNorm * extraSizes[1] };
    pGeom->polylineProc(5, tst);
#endif
    OdUInt32 bbStatus = 0; pGeom->conveyorBoundaryInfoProc(bbChk, bbStatus);
    if (!pView->isModelCacheEnabled())
    {
      uNorm.transformBy(x); vNorm.transformBy(x);
      textOrigin.transformBy(x);
    }
    if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClipFull))
      return; // Text is fully clipped, skip vectorization
    else if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip) || // Text is partially clipped, vectorize through simplifier
             pView->isHighlighted()) // // Text is highlighted, vectorize through simplifier
    {
      bool raw = true; OdString rawMsg = ExGiGDIGeometry::textProcExEmbedRawData(message, ulData, NULL, &raw);
      if (pView->isModelCacheEnabled())
        pGeom->textProc(origin, u, v, rawMsg.c_str(), rawMsg.getLength(), raw, &style);
      else
        pGeom->textProc(textOrigin, x * u, x * v, rawMsg.c_str(), rawMsg.getLength(), raw, &style);
    }
    else
      pView->device()->textProcEx(x * origin, x * u, x * v, message, ulData, &style, x * originalSize);
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};
#endif

struct RecTTFTextCsh : OdGiGeometryMetafile::Record
{
  OdTtfFontsCache::TextInfo textInfo;

  void play(OdGiConveyorGeometry*, OdGiConveyorContext* pCtx) const
  {
    ExGDIVectorizeView* pView = static_cast<ExGDIVectorizeView*>(pCtx);
    pView->device()->ttfFontsCache()->playText(textInfo, pView);
  }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

inline OdUInt32 removeSequence(OdChar *pStr, OdUInt32 nChars)
{
  OdChar *pStrFrom = pStr + nChars;
  while (*pStrFrom != 0)
  {
    *pStr = *pStrFrom;
     pStr++; pStrFrom++;
  }
  *pStr = 0;
  return nChars;
}
static void updateULRange(OdString &inStr, OdChar *pStr, OdIntArray &ulRanges, OdUInt32 nToggle)
{
  if (ulRanges.isEmpty())
    ulRanges.push_back(0), ulRanges.push_back(0);
  const int nCurPos = int(pStr - inStr.c_str());
  const int nPrevPos = *(ulRanges.getPtr() + ulRanges.length() - 2);
  const OdUInt32 nCurRange = OdUInt32(ulRanges.last()) ^ nToggle;
  if (nCurPos == nPrevPos)
    ulRanges.last() = (int)nCurRange;
  else
    ulRanges.push_back(nCurPos), ulRanges.push_back((int)nCurRange);
}
static bool containsSpecialSequence(OdString &inStr, OdFont *pFont, OdIntArray &ulRanges)
{
  // Look for following sequences: "%%", "\\U+", "\\u+", "\\M+", "\\m+"
  OdChar *pStr = inStr.getBuffer(0);
  OdUInt32 nSub = 0;
  while (*pStr)
  {
    switch (*pStr)
    {
      case '%':
        if (pStr[1] == '%')
        {
          OdChar subChar = 0;
          OdUInt32 nEliminate = 2;
          switch (pStr[2])
          {
            case 'd': case 'D': subChar = 0x00B0; break;
            case 'p': case 'P': subChar = 0x00B1; break;
            case 'c': case 'C':
              if (pFont->hasCharacter(0x2205))
                subChar = 0x2205;
              else
                subChar = 0xD8;
              break;
            case '%': subChar = pStr[2]; break;
            case 'u': case 'U': ::updateULRange(inStr, pStr, ulRanges, 1); subChar = (OdChar)-1; pStr--; nEliminate++; break;
            case 'o': case 'O': ::updateULRange(inStr, pStr, ulRanges, 2); subChar = (OdChar)-1; pStr--; nEliminate++; break;
            case 'k': case 'K': ::updateULRange(inStr, pStr, ulRanges, 4); subChar = (OdChar)-1; pStr--; nEliminate++; break;
            default:
              { OdUInt32 nChr; OdChar digits[3] = { '0', '0', '0' };
                for (nChr = 2; nChr < 2 + 3; digits[0] = digits[1], digits[1] = digits[2], digits[2] = pStr[nChr++])
                { if (!OdCharConverter::isDigit(pStr[nChr])) break; }
                if (nChr > 2)
                  subChar = ((digits[0] - '0') * 100) + ((digits[1] - '0') * 10) + (digits[2] - '0'), nEliminate += nChr - 3;
              }
          }
          if (!subChar)
            pStr--; // We can simply remove "%%" sequence, as Acad does.
          else if (subChar != (OdChar)-1)
            *pStr = subChar;
          nSub += ::removeSequence(pStr + 1, nEliminate);
        }
      break;
      case '\\':
        switch (pStr[1])
        {
          case 'U':
          case 'u':
            if (OdCharConverter::isCIF(pStr))
            {
              if (!OdCharConverter::parseCIFString(pStr, *pStr))
                return true;
              nSub += ::removeSequence(pStr + 1, 6);
            }
          break;
          case 'M':
          case 'm':
            if (OdCharConverter::isMIF(pStr))
            {
              OdCodePageId stub;
              if (!OdCharConverter::parseMIFString(pStr, *pStr, stub))
                return true;
              if (OdCharMapper::codepageToUnicode(*pStr, stub, *pStr) != eOk)
                return true;
              nSub += ::removeSequence(pStr + 1, 7);
            }
          break;
        }
      break;
    }
    pStr++;
  }
  if (!ulRanges.isEmpty())
    ::updateULRange(inStr, pStr, ulRanges, (OdUInt32)ulRanges.last());
  inStr.releaseBuffer(inStr.getLength() - nSub);
  return false;
}

void ExGDIVectorizeView::text(const OdGePoint3d& position,
                              const OdGeVector3d& normal,
                              const OdGeVector3d& direction,
                              const OdChar* msg,
                              OdInt32 length,
                              bool raw,
                              const OdGiTextStyle* pStyle)
{
  onTraitsModified(); // First call is required to begin metafile record
  bool m_bMetafile = m_pMetafiler->metafile() != NULL;
  if (m_bMetafile)
    this->m_bProcessingTTF = pStyle->isTtfFont();
  else
    device()->enableTTFProcessing(pStyle->isTtfFont());
  onTraitsModified(); // Second call is required to apply ProcessingTTF flag state
  RecTTFTextBegin *pEndTTFMarker = NULL;
  bool bNeedRemoveLineTyperAfter = false;
  if (m_bMetafile && !pStyle->isShxFont() && view().mode() == OdGsView::k2DOptimized &&
      !giContext().quickTextMode() && OdZero(subEntityTraits().thickness()))
  {
    RecTTFTextBegin* pRec = new RecTTFTextBegin();
    pEndTTFMarker = pRec;
    OdGePoint3d extMin, extMax;
    OdGeVector3d u, v;
    OdGiTextStyle prepStyle;
    // @@@TODO: this data calculated two times if _USE_TEXTOUT defined
    ::odgiCalculateTextBasis(u, v, normal, direction, pStyle->textSize(), pStyle->xScale(),
                             pStyle->obliquingAngle(), pStyle->isBackward(), pStyle->isUpsideDown());
    ::odgiPrepareTextStyle(pStyle, prepStyle);
    OdGiExtAccum::textExtents(drawContext(), prepStyle, msg, (int)length, raw ? kOdGiRawText : 0, extMin, extMax);
    OdGeVector3d uVector = u * (extMax.x - extMin.x);
    OdGeVector3d vVector = v * (extMax.y - extMin.y);
    OdGePoint3d minPos = position + (u * extMin.x + v * extMin.y);
    pRec->v = (!pStyle->isVertical()) ? vVector : uVector;
    pRec->v.transformBy(m_pModelToEyeProc->modelToWorldTransform()); // AMark : #7215
    m_pMetafiler->flush();
    m_pMetafiler->add(pRec);
    m_pMetafiler->add(new RecTTFTextJumper()); // Add first jumper
    // Render optimization polygone
    OdGePoint3d poly[4] =
    {
      minPos,
      minPos + vVector,
      minPos + vVector + uVector,
      minPos + uVector
    };
    OdGiFillType fillMode = subEntityTraits().fillType();
    if (fillMode != kOdGiFillAlways)
    {
      subEntityTraits().setFillType(kOdGiFillAlways);
      onTraitsModified();
    }
    OdGsBaseVectorizer::polygon(4, poly);
    if (fillMode != kOdGiFillAlways)
    {
      subEntityTraits().setFillType(fillMode);
      onTraitsModified();
    }
    m_pMetafiler->flush();
    pRec->pJumper = new RecTTFTextJumper(); // Second jumper after polygone
    m_pMetafiler->add(pRec->pJumper);
  }
#if !defined(_WIN32_WCE)
  if (device()->useTextOut())
  {
    //onTraitsModified();
    OdString sMsg = (length == -1) ? OdString(msg) : OdString(msg, length);
    OdIntArray ulRanges;
    if (m_bMetafile 
      && pStyle->isTtfFont() // only true type fonts are supported
      && OdZero(effectiveTraits().thickness()) // extruded text is exploded to lines
      && !(!raw && ::containsSpecialSequence(sMsg, pStyle->getFont(), ulRanges))
      && GetVersion() < 0x80000000) // oblique text cannot be drawn on Win98, because GM_ADVANCED mode is not supported
    {
      RecTTFTextDc* pRec = new RecTTFTextDc();
      OdGeMatrix3d m2w(m_pModelToEyeProc->modelToWorldTransform());
      pRec->origin = m2w * position;
      pRec->message = sMsg;
      pRec->ulData = ulRanges;
      pRec->style = *pStyle;
      // convert style properties to text OCS vectors
      ::odgiCalculateTextBasis(pRec->u, pRec->v, normal, direction,
        pStyle->textSize(), pStyle->xScale(),
        pStyle->obliquingAngle(),
        pStyle->isBackward(),
        pStyle->isUpsideDown()
        );
      OdGePoint3d pMin, pMax;
      device()->userGiContext()->textExtentsBox(*pStyle, msg, length, 0, pMin, pMax, (OdGePoint3d*)&pRec->originalSize);
      pRec->extraSizes[2] = pMax.y; pRec->extraSizes[0] = pMin.x; pRec->extraSizes[1] = pMin.y;

      OdGeVector3d uNorm(pRec->u.normal()), vNorm(pRec->v.normal());
      OdGeBoundBlock3d bbChk(position + uNorm * pMin.x + vNorm * pMin.y,
                             uNorm * (pRec->originalSize.x - pMin.x),
                             vNorm * (pMax.y - pMin.y),
                             OdGePoint3d::kOrigin.asVector());
      OdUInt32 bbStatus = 0;
      m_modelEntryPoint.geometry().conveyorBoundaryInfoProc(bbChk, bbStatus);

      if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClipFull))
      { // Text is fully clipped, skip vectorization
        delete pRec;
      }
      else
      {
        if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip))
        { // Text is partially clipped, vectorize through simplifier
          delete pRec;
          OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
        }
        else
        { // Text doesn't clipped, vectorize through WinAPI
          m_pMetafiler->flush();
          m_pMetafiler->add(pRec);

          // reset OCS related style properties
          pRec->style.setBackward(false);
          pRec->style.setObliquingAngle(0.);
          pRec->style.setTextSize(1.);
          pRec->style.setUpsideDown(false);
          pRec->style.setXScale(1.);

          pRec->u.transformBy(m2w);
          pRec->v.transformBy(m2w);
          pRec->originalSize.transformBy(m2w);

          secondaryOutput().destGeometry().textProc(
            pRec->origin, pRec->u, pRec->v, msg, length, raw, &pRec->style);
        }
      }
    }
    else
      OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
  }
  else
#endif
  if (device()->useTtfCache() && pStyle->isTtfFont())
  {
    OdTtfFontsCache::TextInfo ttfInfo;
    if (OdNonZero(effectiveTraits().thickness()) || !m_bMetafile ||
        (view().mode() == OdGsView::k2DOptimized && giContext().quickTextMode()))
    {
      OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
    }
    else
    {
      OdGiFillType fill;
      { // Emulate GeometrySimplifier behavior
        fill = subEntityTraits().fillType();
        subEntityTraits().setFillType(giContext().fillTtf() ? kOdGiFillAlways : kOdGiFillNever);
        subEntityTraits().setDrawFlags(subEntityTraits().drawFlags() | OdGiSubEntityTraits::kDrawPolygonFill);
        onTraitsModified();
      }
      // Process text string
      bool bProcessed = true;
      { OdGiGeometryMetafilePtr pSavedMf = m_pMetafiler->metafile();
        m_pMetafiler->flush();
        OdGeMatrix3d m2w(m_pModelToEyeProc->modelToWorldTransform());
        OdGePoint3d origin = m2w * position;
        OdGeVector3d u, v;
        // convert style properties to text OCS vectors
        ::odgiCalculateTextBasis(u, v, normal, direction,
                                 pStyle->textSize(), pStyle->xScale(),
                                 pStyle->obliquingAngle(),
                                 pStyle->isBackward(),
                                 pStyle->isUpsideDown());

        OdGePoint3d pMin, pMax, originalSize;
        device()->userGiContext()->textExtentsBox(*pStyle, msg, length, 0, pMin, pMax, &originalSize);

        OdGeVector3d uNorm(u.normal()), vNorm(v.normal());
        OdGeBoundBlock3d bbChk(position + uNorm * pMin.x + vNorm * pMin.y,
                               uNorm * (originalSize.x - pMin.x),
                               vNorm * (pMax.y - pMin.y),
                               OdGePoint3d::kOrigin.asVector());
        OdUInt32 bbStatus = 0;
        m_modelEntryPoint.geometry().conveyorBoundaryInfoProc(bbChk, bbStatus);

        if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClipFull))
        { // Text is fully clipped, skip vectorization
          bProcessed = false;
        }
        else
        {
          if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip))
          { // Text is partially clipped, vectorize through simplifier
            bProcessed = false;
          }
          else
          { // Text doesn't clipped, vectorize to cache
            if (!device()->ttfFontsCache()->processText(position, u, v,
                msg, length, raw, pStyle, drawContext(), ttfInfo, this))
            {
              m_pMetafiler->exchangeMetafile(pSavedMf);
              bProcessed = false;
            }
            else
              secondaryOutput().destGeometry().textProc(origin, u, v, msg, length, raw, pStyle);
            m_pMetafiler->exchangeMetafile(pSavedMf);
          }
        }
      }
      if (bProcessed)
      {
        // Pack text
        RecTTFTextCsh *pRec = new RecTTFTextCsh;
        pRec->textInfo = ttfInfo;
        m_pMetafiler->add(pRec);
      }
      else
        OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
      { // Restore changed traits
        subEntityTraits().setFillType(fill);
        subEntityTraits().setDrawFlags(subEntityTraits().drawFlags() & ~OdGiSubEntityTraits::kDrawPolygonFill);
        onTraitsModified();
      }
    }
  }
  else
  {
    //LINETYPE: CHANGE #2
    if ( GETBIT(drawContextFlags(), OdGiConveyorContext::kEmbranchmentSimplText) &&  GETBIT(drawContextFlags(), OdGiConveyorContext::kLineTyperAfterMetafile) )
    {
      if ( pEndTTFMarker == 0 ) //this condition is very important, becuase in opposite case important linetyper node will be removed from the metafile and we will have problems with 
                                 // add/remove linetyper order
        bNeedRemoveLineTyperAfter = true;
    }

    if ( bNeedRemoveLineTyperAfter )
    {
      RecLineTyper::updateLineTyper(m_pMetafiler.get(), true);
    }

    OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
  }
  if (pEndTTFMarker)
  {
    m_pMetafiler->flush();
    OdGiGeometryMetafile::Record *pSearchEnd = pEndTTFMarker;
    // Search last known record to skip text rendering up to it
    while (pSearchEnd->tail())
      pSearchEnd = pSearchEnd->tail();
    pEndTTFMarker->pMarker = pSearchEnd;
  }
  if (m_bMetafile)
    this->m_bProcessingTTF = false;
  else
    device()->enableTTFProcessing(false);
  onTraitsModified(); // Last call is required to apply ProcessingTTF flag state
  if ( bNeedRemoveLineTyperAfter )
  {
    RecLineTyper::updateLineTyper(m_pMetafiler.get(), false);
  }
}

// Snapshot support

void ExGDIVectorizeView::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  device()->getSnapShot(pImage, region, this);
}

void ExGsGDIVectorizeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  getSnapShot(pImage, region, NULL);
}

#include "../SnapshotSupport.h"
#include "Gi/GiRasterWrappers.h"
#include "ExGsBitmapDevice.h"

void ExGsGDIVectorizeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView)
{
  struct SnapshotCallback : public OdGiSnapshotImageCallback
  {
    OdGiRasterImagePtr snapshotImageParams(OdGsBaseVectorizeDevice &runDevice) const
    {
      return static_cast<ExGsGDIVectorizeDevice&>(runDevice).snapshotImageParams();
    }
    OdGiRasterImagePtr snapshotRegion(OdGsBaseVectorizeDevice &runDevice, const OdGsDCRect &area, bool bCrop) const
    {
      return static_cast<ExGsGDIVectorizeDevice&>(runDevice).snapshotRegion(area, bCrop);
    }
  } _cbk;
  OdGiSnapshotImageImpl::getSnapshot(*this, &_cbk, pImage, region, pView);
}

OdGiRasterImagePtr ExGsGDIVectorizeDevice::snapshotImageParams() const
{
  OdSmartPtr<OdGiRasterImageDesc> pInfo = OdGiRasterImageDesc::createObject((OdUInt32)width(), (OdUInt32)height());
  HDC hDc = const_cast<ExGsGDIVectorizeDevice*>(this)->drawDc();
  if (!hDc)
    return OdGiRasterImagePtr();
  pInfo->setColorDepth(::GetDeviceCaps(hDc, BITSPIXEL));
  pInfo->pixelFormat().setBGR();
  pInfo->setScanLinesAlignment(4);
  if (pInfo->colorDepth() <= 8)
    pInfo->setPalette(m_logPalette.size() * sizeof(ODCOLORREF), (OdUInt8*)m_logPalette.asArrayPtr());
  else if (pInfo->colorDepth() == 32)
    pInfo->pixelFormat().setBGRA();
  return pInfo;
}

OdGiRasterImagePtr ExGsGDIVectorizeDevice::snapshotRegion(const OdGsDCRect &area, bool /*bCrop*/) const
{
  HDC hDc = const_cast<ExGsGDIVectorizeDevice*>(this)->drawDc();
  if (!hDc || !width() || !height())
    return OdGiRasterImagePtr();
  OdGiRasterImagePtr pInfo = snapshotImageParams();
  OdSmartPtr<OdGiSnapshotImageImpl> pImage = OdRxObjectImpl<OdGiSnapshotImageImpl>::createObject();
  pImage->setupForOriginal(snapshotImageParams());
  pImage->setupSurface(OdUInt32(area.m_max.x - area.m_min.x), OdUInt32(area.m_max.y - area.m_min.y), pImage->colorDepth());
  HDC copyDc = ::CreateCompatibleDC(hDc);
  {
    GsDIBSection dibSec; dibSec.setHDC(copyDc);
    dibSec.createDib((int)pImage->pixelWidth(), (int)pImage->pixelHeight(), (int)pImage->colorDepth());
    ::BitBlt(copyDc, 0, 0, (int)dibSec.getDibWidth(), (int)dibSec.getDibHeight(), hDc, area.m_min.x, height() - area.m_max.y, SRCCOPY);
    ::memcpy(pImage->scanLines(), dibSec.m_pBits, pImage->scanLineSize() * pImage->pixelHeight());
  }
  ::DeleteDC(copyDc);
  return pImage;
}

// TtfFonts cache support

OdRxObjectPtr ExGsGDIVectorizeDevice::tfcNewMetafile(void *pSessionId)
{
  return reinterpret_cast<ExGDIVectorizeView*>(pSessionId)->tfcNewMetafile();
}

OdGiConveyorGeometry *ExGsGDIVectorizeDevice::tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId)
{
  return reinterpret_cast<ExGDIVectorizeView*>(pSessionId)->tfcBeginMetafile(pMetafile);
}

void ExGsGDIVectorizeDevice::tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId)
{
  reinterpret_cast<ExGDIVectorizeView*>(pSessionId)->tfcFinalizeMetafile(pMetafile);
}

void ExGsGDIVectorizeDevice::tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const
{
  reinterpret_cast<ExGDIVectorizeView*>(pSessionId)->tfcPlayMetafile(xForm, pMetafile);
}

OdRxObjectPtr ExGDIVectorizeView::tfcNewMetafile()
{
  return newGsMetafile();
}

OdGiConveyorGeometry *ExGDIVectorizeView::tfcBeginMetafile(OdRxObject *pMetafile)
{
  OdGsBaseVectorizer::beginMetafile(pMetafile);
  OdGiGeometryMetafile* pMFer = static_cast<OdGiGeometryMetafile*>(pMetafile);
  pMFer->clear();
  m_pMetafiler->exchangeMetafile(pMFer);
  return &output().destGeometry();
}

void ExGDIVectorizeView::tfcFinalizeMetafile(OdRxObject * /*pMetafile*/)
{
  // Do nothing
}

void ExGDIVectorizeView::tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile) const
{
  ExGDIVectorizeView* pThis = const_cast<ExGDIVectorizeView*>(this);
  pThis->pushMetafileTransform(xForm, kSharedRefTransform);

  pThis->addLineTyperAfterMetafile();
  static_cast<const OdGiGeometryMetafile*>(pMetafile)->play(&pThis->m_GeometryDestForPlaying, pThis->drawContext());
  pThis->removeLineTyperAfterMetafile();

  pThis->popMetafileTransform(kSharedRefTransform);
}

void ExGDIVectorizeView::updateLineTyper(bool bRemove)
{
  if ( bRemove )
    removeLineTyperAfterMetafile();
  else
    addLineTyperAfterMetafile();
}

// Background rendering

void ExGDIVectorizeView::display(bool update)
{
  // Draw viewport background and etc.
  if (viewportObjectId() != NULL)
  {
    // Prepare for background drawing
    RenderMode rmState = m_renderMode;
    m_renderMode = k2DOptimized;
    device()->setDrawContext(drawContext());
    bool hlrState = m_pHLR->enabled();
    m_pHLR->enable(false);
    OdGiFillType ftState = subEntityTraits().fillType();
    subEntityTraits().setFillType(kOdGiFillAlways);
    // Transformation setup if no model cache enabled
    OdGeMatrix3d tmState(eyeToOutputTransform());
    OdGeMatrix3d mfState; m_pMfPlay->transform(mfState);
    OdGeMatrix3d pxState; m_pPerspXform->transform(pxState);
    {
      setEyeToOutputTransform(getEyeToWorldTransform());
      if (!cachedDrawables())
        m_pMfPlay->input().addSourceNode(output());
      m_pMfPlay->setTransform(getWorldToEyeTransform());
      m_pPerspXform->setTransform(view().eyeToScreenMatrix());
    }
    if (view().isPerspective())
      m_pPerspPrepr->enable(false);
    //
    updateViewportProperties(OdGsProperties::kDeviceBackground);
    if (rmState != OdGsView::k2DOptimized)
    {
      OdGiDrawFlagsHelper saveDrawFlag(subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
      displayViewportProperties(NULL, OdGsProperties::kDeviceBackground);
    }
    if (view().isPerspective())
      m_pPerspPrepr->enable(true);
    {
      if (!cachedDrawables())
        m_pMfPlay->input().removeSourceNode(output());
      setEyeToOutputTransform(tmState);
      m_pMfPlay->setTransform(mfState);
      m_pPerspXform->setTransform(pxState);
    }
    // Resore render settings
    subEntityTraits().setFillType(ftState);
    m_renderMode = rmState;
    device()->setDrawContext(drawContext());
    m_pHLR->enable(hlrState);
    //
  }
  OdGsBaseVectorizer::display(update);
}

// set of methods for the actions with the line typer conveyor node
bool ExGDIVectorizeView::isNeedActionWithLineTyper()
{
  //return false;
  return (!view().cachedDrawables() || giContext().isPlotGeneration()) && device()->isSupportPartialUpdate();
}

void ExGDIVectorizeView::addLineTyperBeforeMetafile()
{
   if (isOutputSuppressed())
    return;

  if ( isNeedActionWithLineTyper() )
  {
    // add line typer from the previous place in the conveyor
    m_eyeEntryPoint.removeSourceNode(m_pXToLtp->output());
    m_eyeEntryPoint.removeSourceNode(m_pXlineNRayClipper->output());
    m_eyeEntryPoint.addSourceNode(m_pLinetyper->output());

    m_pLinetyper->input().addSourceNode(m_pXToLtp->output());
    m_pLinetyper->input().addSourceNode(m_pXlineNRayClipper->output());
  }
}

void ExGDIVectorizeView::removeLineTyperBeforeMetafile()
{
   if (isOutputSuppressed())
    return;

  if ( isNeedActionWithLineTyper() )
  {
    //Remove linetyper from the preMetafile conveyor
    m_pLinetyper->input().removeSourceNode(m_pXToLtp->output());
    m_pLinetyper->input().removeSourceNode(m_pXlineNRayClipper->output());
  
    m_eyeEntryPoint.removeSourceNode(m_pLinetyper->output());
    m_eyeEntryPoint.addSourceNode(m_pXToLtp->output());
    m_eyeEntryPoint.addSourceNode(m_pXlineNRayClipper->output());
  }
}

void ExGDIVectorizeView::addLineTyperAfterMetafile()
{
   if (isOutputSuppressed())
    return;

   if ( m_bIsLineTyperWasAddedAfterMetafile )
     return;

  if ( isNeedActionWithLineTyper() )
  {
    //Add linetyper to the afterMetafile conveyor (when playing)
    m_pZClip->input().removeSourceNode(m_pMfPlay->output()); 
    m_pLinetyper->input().addSourceNode(m_pMfPlay->output());

    m_pZClip->input().addSourceNode(m_pLinetyper->output());

    m_bIsLineTyperWasAddedAfterMetafile = true;
  }
}

void ExGDIVectorizeView::removeLineTyperAfterMetafile()
{
   if (isOutputSuppressed())
    return;

   if ( !m_bIsLineTyperWasAddedAfterMetafile )
     return;

  if ( isNeedActionWithLineTyper() )
  {
    //Remove linetyper to the conveyor after metafile playing
    m_pZClip->input().removeSourceNode(m_pLinetyper->output());
    m_pLinetyper->input().removeSourceNode(m_pMfPlay->output());
 
    m_pZClip->input().addSourceNode(m_pMfPlay->output()); 

    m_bIsLineTyperWasAddedAfterMetafile = false;
  }
}
