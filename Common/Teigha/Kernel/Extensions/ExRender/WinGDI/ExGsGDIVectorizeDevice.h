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

#ifndef EXGSGDIVECTORIZEDEVICE
#define EXGSGDIVECTORIZEDEVICE

#include "TD_PackPush.h"

#include "../win/ExGsWin32Device.h"
#include "Gs/GsBaseInclude.h"
#include "Gs/GsBaseModel.h"
#include "IntArray.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeInterval.h"
#include "Gi/GiXform.h"
#include "Gi/GiOrthoClipper.h"
#include "Gi/GiPerspectivePreprocessor.h"
#include "Gi/GiMetafiler.h"
#include "Gi/GiHLRemover.h"
#include "Gi/GiPlotGenerator.h"
#include "Gi/GiConveyorPassingImpl.h"
#include "ExGiGDIViewportClipper.h"
#include "ExGiGDIGeometry.h"
#include "../MetafileTransformStack.h"
#include "../TtfFontsCache.h"
#include "../WinComposite/ExGsCompositeModuleInterface.h"
#include <memory>

class ExGDIVectorizeView;

typedef OdArray<POINT, OdMemoryAllocator<POINT> > POINTArray;

/** \details
  This class is the base class for Win32 GDI Vectorizer Device objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGsGDISimpleVectorizeDevice : public OdGsBaseVectorizeDevice
                                   , public ExGiGDIGeometry
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);

  OdGiPlotGeneratorPtr  m_pPlotstyler;
  OdGiConveyorConnector m_toGDI;
  OdGiConveyorConnector m_toPlotGen;
public:
  ExGsGDISimpleVectorizeDevice();
  void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors);
  bool setBackgroundColor(ODCOLORREF color);
  void onSize(const OdGsDCRect& outputRect);

  OdGiConveyorInput& input();
  OdGiConveyorInput& plotGenInput();
  OdGiPlotGenerator& plotGenerator();
};

typedef ExGsCompositeClientDevice<ExGsGDISimpleVectorizeDevice> ExGsBaseGDIVectorizeDevice;

/** \details
  This class implements Win32 GDI Vectorizer Device objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGsGDIVectorizeDevice : public ExGsBaseGDIVectorizeDevice
                             , public ExGsWin32Device
                             , public OdTtfFontsCacheCallback
{
  class HDCStore
  {
    HDC m_hdc;
  public:
    operator HDC() const { return m_hdc; }
    HDC& operator=(const HDC &hdc) { m_hdc = hdc; return m_hdc; }
    HDCStore() : m_hdc(0) { }
    HDCStore(HDC hdc) : m_hdc(hdc) { }
    virtual ~HDCStore() { m_hdc = 0; };
  };

  class HDCAutoDestroyStore : public HDCStore
  {
  public:
    operator HDC() const { return HDCStore::operator HDC(); }
    HDC& operator=(const HDC &hdc) { return HDCStore::operator=(hdc);}
    HDCAutoDestroyStore(HDC hdc) : HDCStore(hdc) { }
    virtual ~HDCAutoDestroyStore() 
    {
      ::DeleteDC( *this );
    };
  };

  void beginPaint();
  void endPaint();
  POINTArray              m_dcPoints;
  std::auto_ptr<HDCStore> m_hOffscreenDC;
  HBITMAP                 m_hbmScreen;
  bool                    m_bClearScreen;
  bool                    m_bUseSoftwareHLR;
  bool                    m_bUseTextOut;
  bool                    m_bUseTtfCache;
  bool                    m_bContextValid;
  double                  m_dMinTTFTextSize;
  OdGsBaseDeviceMTHelpers m_deviceSync;
  OdTtfFontsCachePtr      m_pTtfCache;

  HDC createDrawDc();
  void deleteDrawDc();
  void flushDrawDc();
protected:
  virtual void HDCWasChanged();
  bool supportPartialUpdate() const;

  bool supportMultithreading() const
  {
    return checkMtRegenEnabled(userGiContext());
  }
public:
  /** \details
    Returns the offscreen device context for the Vectorizer Device object.
  */
  inline HDC drawDc() { return (m_hOffscreenDC.get()) ? (HDC)(*m_hOffscreenDC) : (HDC)NULL; }

  OdRxDictionaryPtr properties() { return this; }
  OD_COMMONDEVPROP_BlocksCacheEnabled_DevRedirect();
  OD_COMMONDEVPROP_DynamicSubEntHltEnabled_DevRedirect();
  void setColorConverter(const OdColorConverterCallback *pCC)
  {
    OdCommonDeviceProps::setColorConverter(pCC);
    ExGiGDIGeometry::m_pColorConverter = getColorConverter();
  }

  OdGsBaseDeviceMTHelpers &mtDeviceSync() { return m_deviceSync; }

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsGDIVectorizeDevice);
public:
  using ExGsBaseGDIVectorizeDevice::m_logPalette;
  ExGsGDIVectorizeDevice();
  ~ExGsGDIVectorizeDevice();

  void onSize(const OdGsDCRect& outputRect);

  /** \details
    Returns the handle to the device context associated with this Vectorizer Device object.
  */
  HDC hdc() const { return m_hDC; }
  /** \details
    Returns true if and only if the ClearScreen flag is set for this Vectorizer Device object.
  */
  bool clearScreen() const { return m_bClearScreen; }
  /** \details
    Controls the ClearScreen flag for this Vectorizer Device object.
    \param clearScreen [in]  Controls flag.
  */
  void setClearScreen(bool clearScreen) { m_bClearScreen = clearScreen; }

  /** \details
    Returns true if and only if the UseSoftwareHLR (Hidden Line Renderer) flag is set for this Vectorizer Device object.
  */
  bool useSoftwareHLR() const { return m_bUseSoftwareHLR; }
  /** \details
    Controls the UseSoftwareHLR (Hidden Line Renderer) flag for this Vectorizer Device object.
    \param useSoftwareHLR [in]  Controls flag.
  */
  void setUseSoftwareHLR(bool useSoftwareHLR) { m_bUseSoftwareHLR = useSoftwareHLR; }

  /** \details
    Returns true if and only if the UseTextOut flag is set for this Vectorizer Device object.
  */
  bool useTextOut() const { return m_bUseTextOut; }
  /** \details
    Controls the UseTextOut flag for this Vectorizer Device object.
    \param useTextOut [in]  Controls flag.
  */
  void setUseTextOut(bool useTextOut) { m_bUseTextOut = useTextOut; }

  /** \details
    Returns true if and only if the UseTtfCache flag is set for this Vectorizer Device object.
  */
  bool useTtfCache() const { return m_bUseTtfCache; }
  /** \details
    Controls the UseTtfCache flag for this Vectorizer Device object.
    \param useTtfCache [in]  Controls flag.
  */
  void setUseTtfCache(bool useTtfCache) { m_bUseTtfCache = useTtfCache; }

  /** \details
    Returns minimal TrueType Text size with which text still not rendered as simplified geometry.
  */
  double minimalTTFTextSize() const { return m_dMinTTFTextSize; }
  /** \details
    Controls the minimal TrueType Text size for this Vectorizer Device object.
    \param useTtfCache [in]  Controls flag.
  */
  void setMinimalTTFTextSize(double dMinTTFTextSize) { m_dMinTTFTextSize = dMinTTFTextSize; }

  /** \details
    Returns true if and only if the GradientsAsBitmap flag is set for this Vectorizer Device object.
  */
  bool gradientsAsBitmap() const { return m_bGradientAsBitmap; }
  /** \details
    Controls the GradientsAsBitmap flag for this Vectorizer Device object.
    \param bSet [in]  Controls flag.
  */
  void setGradientsAsBitmap(bool bSet) { m_bGradientAsBitmap = bSet; }
  /** \details
    Returns true if and only if the GradientsAsPolys flag is set for this Vectorizer Device object.
  */
  bool gradientsAsPolygons() const { return m_bGradientAsPolys; }
  /** \details
    Controls the GradientsAsPolys flag for this Vectorizer Device object.
    \param bSet [in]  Controls flag.
  */
  void setGradientsAsPolygons(bool bSet) { m_bGradientAsPolys = bSet; }
  /** \details
    Returns the GradientsAsPolys threshold for this Vectorizer Device object.
  */
  OdUInt8 gradientsAsPolygonsThreshold() const { return m_nGradientAsPolysSubdivideTreshold; }
  /** \details
    Controls the GradientsAsPolys threshold for this Vectorizer Device object.
    \param nThreshold [in]  New threshold for GradientsAsPolys mode.
  */
  void setGradientsAsPolygonsThreshold(OdUInt8 nThreshold) { m_nGradientAsPolysSubdivideTreshold = nThreshold; }

  void updateScreen();
  // old style update is used when gs model is disabled
  void update(OdGsDCRect* pUpdatedRect = 0);

  bool isSupportPartialUpdate() const {return supportPartialUpdate();}

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView);
  virtual OdGiRasterImagePtr snapshotImageParams() const;
  virtual OdGiRasterImagePtr snapshotRegion(const OdGsDCRect &area, bool bCrop) const;

  // TtfFonts cache support
  OdTtfFontsCache *ttfFontsCache() { return m_pTtfCache.get(); }
  OdRxObjectPtr tfcNewMetafile(void *pSessionId);
  OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId);
  void tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId);
  void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const;
protected:
  virtual void createContext();

  ODRX_USING_HEAP_OPERATORS(ExGsBaseGDIVectorizeDevice);
};

#ifndef ROUND
#define ROUND(X) long((X) + .5)
#endif

/** \details
  This class is the base class for Win32 GDI Vectorizer Device objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/

class ExGDIVectorizeGeometryDestGeom : public OdGiConveyorPassingImpl<ExGDIVectorizeGeometryDestGeom>
{
  OdGiConveyorConnector*          m_pGDIViewPlayEntryPoint;

public:
  void setGDIViewPlayEntryPoint(OdGiConveyorConnector* pGDIViewPlayEntryPoint) { m_pGDIViewPlayEntryPoint = pGDIViewPlayEntryPoint; }
  OdGiConveyorGeometry& destGeometry() const
  {
    if ( m_pGDIViewPlayEntryPoint == NULL )
      return OdGiEmptyGeometry::kVoid;
    
    return m_pGDIViewPlayEntryPoint->geometry();
  }
};

/** \details
  This class implements Win32 GDI Vectorizer View objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGDIVectorizeView : public OdGsBaseVectorizeViewMT
{
  OdGiConveyorConnector          m_mfPlayEntryPoint;
  ExGDIVectorizeGeometryDestGeom m_GeometryDestForPlaying;
  OdGiXformPtr                   m_pMfPlay;
  OdGiOrthoClipperPtr            m_pZClip;
  OdGiXformPtr                   m_pPerspXform;
  OdGiPerspectivePreprocessorPtr m_pPerspPrepr;
  OdGiHLRemoverPtr               m_pHLR;
  OdGiGDIViewportClippingPtr     m_pVptClipper;
  OdGiMetafilerPtr               m_pMetafiler;
  bool                           m_bCurHLTState; // !dynamicSubEntHltEnabled
  OdGsMarker                     m_gsHLTMarker;  // dynamicSubEntHltEnabled
  bool                           m_bProcessingTTF;
  bool                           m_bCurTTFState;
  MetafileTransformStack         m_mfTransformStack;
  bool                           m_bInexactSelection;
  OdGiConveyorGeometry          *m_pPrevSelGeom;

protected:
  /** \details
    Sets the current lineweight in pixels.
    \param lineWeight [in]  Lineweight.
  */
  void setLineWeightPixels(int lineWeight);

  /** \details
    Detach or attach current geometry conveyor screen output.
    \param bEnable [in]  New geometry conveyor screen output state.
  */
  void enableScreenOutput(bool bEnable);
public:
  ExGsGDIVectorizeDevice* device()
  {
    return static_cast<ExGsGDIVectorizeDevice*>(OdGsBaseVectorizeView::device());
  }
  ExGDIVectorizeView();

  /** \details
    Returns the output from this Vectorizer View object geometry conveyor.
  */
  OdGiConveyorOutput& screenOutput() { return m_pVptClipper->output(); }
  /** \details
    Initializes the paint process for this Vectorizer Device object.
    \param targetDC [in]  Handle to the target device context.
  */
  void beginPaint(HDC targetDC);
  /** \details
    Terminates the paint process for this Vectorize Device object.
    \remarks
    Deletes all objects and resets all settings associated with this Vectorize Device object.
  */
  void endPaint();

  // to update conveyor if view is perspective

  virtual void setView(const OdGePoint3d & position,
    const OdGePoint3d& target,
    const OdGeVector3d& upVector,
    double fieldWidth,
    double fieldHeight,
    Projection projectionType = kParallel);

  void pushViewportClipping(bool bForceWinClip = false);
  void popViewportClipping(bool bForceWinClip = false);
  bool isViewportClippingWinClip();

  void loadViewport();
  void beginViewVectorization();
  void endViewVectorization();

  void pushClipBoundary(OdGiClipBoundary* pBoundary);
  void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  void popClipBoundary();

  void select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);

  virtual void setMode(RenderMode mode);
  virtual void onTraitsModified();
  void renderTypeOnChange(OdGsModel::RenderType renderType);
  void updateLineweightOverride(const OdGiLineweightOverride &lwdOverride);
  // override viewport frame rendering
  void drawViewportFrame();

  virtual bool pushPaletteOverride(const OdGiPalette* pOverride);
  virtual void popPaletteOverride();
  virtual void updatePaletteOverride(const OdGiPalette *pPalette);

  // virtual override, to let entities know that they're drawn for dragging
  // (overrides OdGiCommonDraw::isDragging() )

  bool isDragging() const;

  void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned = true,
    bool allowClipping = false);

  // caching

  /** \details
    Creates a new GsMetafile (cache) object.
    \remarks
    Returns a SmartPointer to the newly created object.
  */
  OdRxObjectPtr newGsMetafile();
  /** \details
    Begins vectorization to the specified GsMetafile (cache) object.    
    \param pMetafile [in]  Pointer to the metafile.
  */
  void beginMetafile(OdRxObject* pMetafile);
  /** \details
    Ends vectorization to the specified GsMetafile (cache) object.    
    \param pMetafile [in]  Pointer to the metafile.
  */
  void endMetafile(OdRxObject* pMetafile);
  /** \details
    Plays (draws) the specified Metafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  void playMetafile(const OdRxObject* pMetafile);

  void pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags = 0);
  void popMetafileTransform(OdUInt32 uFlags = 0);

  bool useMetafileAsGeometry() const { return m_bInexactSelection && isModelCacheEnabled() && !const_cast<ExGDIVectorizeView*>(this)->device()->useTextOut(); }
  OdGiConveyorOutput& outputForMetafileGeometry() { return m_pMfPlay->output(); }
  void setTransformForMetafileGeometry(const OdGeMatrix3d& mtx);
  OdGeMatrix3d getTransformForMetafileGeometry() const;
  const OdGeMatrix3d &getMetafileTransform() const { return m_mfTransformStack.metafileTransform(); }

  inline bool isModelCacheEnabled() const
  {
    return view().cachedDrawables() != 0;
  }
  const OdGsHlBranch* getCurrentHLTBranch() const
  {
    return currentHighlightBranch();
  }

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* msg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pTextStyle);

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  // TtfFonts cache support
  OdRxObjectPtr tfcNewMetafile();
  OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile);
  void tfcFinalizeMetafile(OdRxObject *pMetafile);
  void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile) const;

  void updateLineTyper(bool bRemove);

  // Background rendering compatibility
protected:
  void display(bool update);

private:

  bool isNeedActionWithLineTyper();
  void addLineTyperBeforeMetafile();
  void removeLineTyperBeforeMetafile();
  
  bool m_bIsLineTyperWasAddedAfterMetafile;
  void addLineTyperAfterMetafile();
  void removeLineTyperAfterMetafile();
};

#include "TD_PackPop.h"

#endif //#ifndef EXGSGDIVECTORIZEDEVICE
