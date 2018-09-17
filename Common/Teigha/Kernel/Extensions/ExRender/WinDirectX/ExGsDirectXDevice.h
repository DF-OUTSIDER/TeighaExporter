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

#ifndef __OD_EX_DIRECT_X_DEVICE__
#define __OD_EX_DIRECT_X_DEVICE__

#include "TD_PackPush.h"

#include "ExGsWin32Device.h"
#include "ExGiDirectXGeometry.h"

#include "Gi/GiGeometrySimplifier.h"
#include "Gs/GsBaseInclude.h"
#include "Gs/GsBaseMaterialView.h"
#include "Gs/GsBaseModel.h"
#include "Gs/GsProperties.h"
#include "../ExFPPrecisionFix.h"
#include "../TtfFontsCache.h"
#include "../WinComposite/ExGsCompositeModuleInterface.h"

//#include <atlbase.h> // included in ./ExGiDirectXGeometry.h
#include "D3DInclude.h"

#include "OdErrorContext.h"

class OdGiLightTraitsData;

/** \details
  This class defines a OdError class for the ExGsDirectXDevice class. 
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class DXError : public OdError
{
  class DXErrorCtx : public OdErrorContext
  {
  protected:
    bool m_bDXAPI;
    HRESULT m_hrCode;
  public:
    DXErrorCtx() : m_bDXAPI(false), m_hrCode(0) { }

    OdString description() const { return formatHRESULTErrorInfo(); }
    OdResult code() const { return eExtendedError; }

    DXErrorCtx *setErrorCode(bool bDXAPI, HRESULT hrCode) { m_bDXAPI = bDXAPI; m_hrCode = hrCode; return this; }

    OdString formatHRESULTErrorInfo() const;
  };
public:
  DXError() : OdError(OdRxObjectImpl<DXErrorCtx>::createObject()) {}
  /** \details
    Throws a DirectX API error if and only if the specified value is less than zero.
    \param hr [in]  Result value.
  */
  static void check(HRESULT hr)
  {
    if(FAILED(hr))
      throw OdError(OdRxObjectImpl<DXErrorCtx>::createObject()->setErrorCode(true, hr));
  }
  /** \details
    Throws a DirectX API error.
  */
  static void fail(HRESULT hr = S_OK)
  {
    if (hr != S_OK)
      throw OdError(OdRxObjectImpl<DXErrorCtx>::createObject()->setErrorCode(false, hr));
    throw DXError();
  }
};

typedef ExGsCompositeClientDevice<OdGsBaseVectorizeDevice> ExGsBaseDirectXDevice;

/** \details
  This class implements Win32 DirectX Vectorizer Device objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGsDirectXDevice : public ExGsBaseDirectXDevice,
                          public ExGsWin32Device,
                          public OdTtfFontsCacheCallback
{
  bool m_bContextValid, m_bDiscardBackFaces, m_bLockableBackBuffer;
  bool m_bUseTTFCache, m_bRefDevice;
  OdUInt32 m_nUseLutPalette;
  CComPtr<IDirect3D> m_pD3D;
  CComPtr<IDirect3DDevice> m_pD3Ddevice;
  OdD3DStateManager m_stateManager;
  HINSTANCE m_d3dInst;
  D3dLostStateManager m_lsManager;
  OdGiMaterialTextureManagerPtr m_pTexManager;
  D3dResourceSharingProvider m_sharingProv;
  OdGsBaseDeviceMTHelpers m_deviceSync;
  OdTtfFontsCachePtr m_ttfFontsCache;
  // Raster images cache
public:
  struct RasterImageEntry
  {
    D3dMaterialViewRedirect::RasterImageRef m_texture;
    const OdGiRasterImage *m_pImg;
    bool m_bTransparency;
    ODCOLORREF m_fgColor;
    bool m_bSecColor;
    ODCOLORREF m_bkColor;
    double m_fBrightness;
    double m_fContrast;
    double m_fFade;
    OdUInt32 m_uRefCounter;
    //RasterImageEntry() : m_uRefCounter(0) { }
  };
private:
  OdArray<RasterImageEntry> m_RasterImages;
  //
protected:
  ExGsDirectXDevice();
  virtual ~ExGsDirectXDevice();

  void createContext();
  void deleteContext();

  ODRX_USING_HEAP_OPERATORS(ExGsBaseDirectXDevice);

public:
  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsDirectXDevice);
  /** \details
    Sets the HInstance of the request handerer DLL for this Vectorizer device.
    \param hInst [in]  Request handler DLL
  */
  void setD3DDllHInst(HINSTANCE hInst);

  OdRxDictionaryPtr properties() { return this; }
  OD_COMMONDEVPROP_BlocksCacheEnabled_DevRedirect();
  OD_COMMONDEVPROP_DynamicSubEntHltEnabled_DevRedirect();

  void update(OdGsDCRect* pUpdatedRect);
  void updateScreen();

  bool supportPartialUpdate() const;

  bool supportMultithreading() const
  {
    return checkMtRegenEnabled(userGiContext());
  }

  void onSize(const OdGsDCRect& outputRect);

  /** \details
    Returns the Direct3D device associated with this Vectorizer object.
  */
  CComPtr<IDirect3DDevice> d3dDevice();
  /** \details
    Returns the Direct3D device state manager associated with this Vectorizer object.
  */
  OdD3DStateManager &d3dStateManager();

  /** \details
    Returns true if and only if back faces are discarded by this Vectorizer device.
  */
  bool discardBackFaces() const { return m_bDiscardBackFaces; }
  /** \details
    Controls the discarding of back faces by this Vectorizer device
    \param discard [in]  Back faces are discarded.
  */
  void setDiscardBackFaces(bool discard) { m_bDiscardBackFaces = discard; }

  // lockable back buffer may decrease performance on some video-hardware
  // by default DD uses non-lockable back buffer
  // if lockable back buffer is strongly required, it can be enabled by this function

  /** \details
    Returns true if and only if a lockable back buffer is enabled for this Vectorizer device.
  */
  bool lockableBackBuffer() const { return m_bLockableBackBuffer; }
  /** \details
    Controls the lockable back buffer for this Vectorizer device.
    \note
    A lockable back buffer may decrease performance on some video-hardware
    Teigha, by default, uses a non non-lockable back buffer.
    If a lockable back buffer is required, it can be enabled by this function.
  */
  void setLockableBackBuffer(bool enable) { m_bLockableBackBuffer = enable; }

  /** \details
    Returns true if and only if TTF fonts cache is enabled for this Vectorizer device.
  */
  bool useTTFCache() const { return m_bUseTTFCache; }
  /** \details
    Controls the usage of TTF fonts cache by this Vectorizer device.
    \param use [in]  Enable TTF fonts cache.
  */
  void setUseTTFCache(bool use) { m_bUseTTFCache = use; }

  /** \details
    Returns Lut settings for this Vectorizer device.
  */
  OdUInt32 useLutPalette() const { return m_nUseLutPalette; }
  /** \details
    Controls Lut settings for this Vectorizer device.
    \param lutFlags [in]  Set of Lut flags (1 - use monochrome; 2 - use palette).
  */
  void setUseLutPalette(OdUInt32 lutFlags) { m_nUseLutPalette = lutFlags; }

  /** \details
    Returns IDirect3DSurface pointer for WPF controls.
    \remarks
    Caller is responsible to call IUnknown->Release() method after Surface interface usage.
  */
  OdIntPtr backBufferSurface() const;
  /** \details
    Does nothing.
  */
  void setBackBufferSurface(OdIntPtr /*pSurf*/) { }

  /** \details
    Computed regeneration coefficient.
  */
  double getRegenCoef() const;
  /** \details
    Does nothing.
  */
  void setRegenCoef(double /*val*/) { }

  /** \details
    Returns true if lines anti-aliasing supported on current hardware.
  */
  bool isLinesAntiAliasingSupported() const;
  /** \details
    Does nothing.
  */
  void setLinesAntiAliasingSupported(bool /*val*/) { }

  /** \details
    Returns true in case if REF device is enabled instead of HAL.
  */
  bool isRefDevice() const { return m_bRefDevice; }
  /** \details
    Enable using REF device instead of HAL.
  */
  void setRefDevice(bool bSet) { m_bRefDevice = bSet; }

  /** \details
    Returns raster images cache.
  */
  OdArray<RasterImageEntry> &rasterImageCache() { return m_RasterImages; }

  /** \details
    Returns material textures cache.
  */
  OdGiMaterialTextureManager *materialTextureManager() { return m_pTexManager; }

  /** \details
    Returns resource sharing provider.
  */
  D3dResourceSharingProvider *sharingProvider() { return &m_sharingProv; }

  /** \details
    Returns MT synchronization helpers.
  */
  OdGsBaseDeviceMTHelpers &mtDeviceSync() { return m_deviceSync; }

  /** \details
    Returns TTF fonts cache.
  */
  OdTtfFontsCache *ttfFontsCache()
  {
    if (m_ttfFontsCache.isNull())
      m_ttfFontsCache = OdTtfFontsCache::createObject(this);
    return m_ttfFontsCache.get();
  }

  bool requireWindow() const { return true; }
  void makeBitBltOp(const ExWinCompositeBitBltOp &bitBltOp);

  OdColorConverterCallback *getColorConverter();
  DWORD getBackgroundColor();

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView);
  virtual OdGiRasterImagePtr snapshotImageParams() const;
  virtual OdGiRasterImagePtr snapshotRegion(const OdGsDCRect &area, bool bCrop) const;

  // TtfFonts cache support
  OdRxObjectPtr tfcNewMetafile(void *pSessionId);
  OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId);
  void tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId);
  void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const;
};

/** \details
  This class implements Win32 DirectX Vectorizer View objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGsDirectXView : public OdGsBaseMaterialViewMT, protected OdGsPropertiesDirectRenderOutput,
                        public D3dMaterialViewRedirect, public ExFPPrecisionFix
{
public:
  /** \details
    Returns the ExGsDirectXDevice object associated with this Vectorizer View object.
  */
  ExGsDirectXDevice* device()
  {
    return static_cast<ExGsDirectXDevice*>(OdGsBaseVectorizeView::device());
  }
private:
  /** \details
    Returns the DirectX screen rectangle associated with this Vectorizer View object.
    \param left [out]  Receives the left ordinate of the rectangle.
    \param bottom [out]  Receives the bottom ordinate of the rectangle.
    \param width [out]  Receives the width of the rectangle.
    \param height [out]  Receives the height of the rectangle.
  */
  void getDXScreenRect(double& left, double& bottom, double& width, double& height);
  /** \details
    Sets the pixels to DirectX transformation matrix for this Vectorizer View object.
    \param pixels2DxCoords [in]  Transformation matrix.  
  */
  void setViewportClip(const OdGeMatrix3d& pixels2DxCoords);

  ExGiDirectXGeometryPtr m_pGeometry;

  // todo -- duplicated with Gl device

  bool     m_bSelecting;

  // Number of non-default lights

  OdUInt32 m_numLights;

  // Enables geometry fading

  bool     m_bFading;
  OdUInt8  m_nCurFading;

  // Enables transparencies rendering

  bool     m_bDrawTransparency;

protected:

  /** \details
    Returns the DirectX color corresponding to the specified color.
    \param color [in]  Color.
  */
  DWORD getD3DColor(const OdCmEntityColor& color);
  DWORD getD3DColor(ODCOLORREF color);

  /** \details
    Updates the lights attached to this VectorizeView object.
  */
  void updateLights();
  
  bool usesDept(double* pMinDeptSupported, double* pMaxDeptSupported) const;
  bool sceneDept(double& zNear, double& zFar) const;
  /** \details
    Returns all transformation matrices for current viewport.
  */
  void viewportTransforms(D3DXMATRIX &matView, D3DXMATRIX &matProjection, OdGeMatrix3d &pixels2DxCoords, bool bNoDept = false);

  const OdGsHlBranch* getCurrentHLTBranch() const
  {
    return currentHighlightBranch();
  }
  virtual bool dynamicSubEntHlt() const
  {
    return const_cast<ExGsDirectXView*>(this)->device()->supportDynamicSubhighlight();
  }
  void resetFading(bool bFlag);
public:
  ExGsDirectXView();
  virtual ~ExGsDirectXView();

  /** \details
    Returns the matrix that transforms view space to normalized device space for this Vectorizer View object.
  */
  OdGeMatrix3d projectionMatrix_D3D() const;

  /** \details
    Returns a reference to the OdGiDirectXGeometry object associated with this Vectorizer device.
  */
  ExGiDirectXGeometry& geometry();

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
  void xformByMetafileTransform(OdGeMatrix3d &mtx) const;
  void xformByMetafileTransform(OdGeVector3d &vec) const;

  //SEA
protected:
  OdGiXformPtr              m_pSelectXfm;
  OdGiConveyorConnector     m_mfEntryPoint;
public:
  bool useMetafileAsGeometry() const;
  OdGiConveyorOutput& outputForMetafileGeometry();
  void setTransformForMetafileGeometry(const OdGeMatrix3d& mtx);
  OdGeMatrix3d getTransformForMetafileGeometry() const;
  OdGiConveyorGeometry &metafileEntryPoint() { return m_mfEntryPoint.geometry(); }

  void beginViewVectorization();
  void loadViewport();
  void endViewVectorization();

  void ownerDrawDc(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v, 
    const OdGiSelfGdiDrawable* pDrawable, bool dcAligned, bool allowClipping);
  void onTraitsModified();
  void renderTypeOnChange(OdGsModel::RenderType renderType);
  void select(const OdGePoint2d* points, int numPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);

  bool forceMetafilesDependence() const;
  bool metafileTransformOverrided() const;
  const OdGeMatrix3d &metafileToWorldMatrix() const;
  const OdGeMatrix3d &worldToMetafileMatrix() const;
  const OdGeMatrix3d &viewportToOutputMatrix() const;
  const OdGeMatrix3d &outputToViewportMatrix() const;
  double regenCoef();

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* msg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pTextStyle);

  void shell(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  /** \details
    Adds a point light to this VectorizeView object.
    \param traits [in]  Light traits data.
  */
  void addPointLight(const OdGiPointLightTraitsData& traits);
  /** \details
    Adds a spotlight to this VectorizeView object.
    \param traits [in]  Light traits data.
  */
  void addSpotLight(const OdGiSpotLightTraitsData& traits);
  /** \details
    Adds a distant light to this VectorizeView object.
    \param traits [in]  Light traits data.
  */
  void addDistantLight(const OdGiDistantLightTraitsData& traits);

  /** \details
    Appends a light with the specified traits to this VectorizeView object.
    \param traits [in]  Light traits data.
    \param light [in]  Receives the specified traits.
  */
  void appendLight(const OdGiLightTraitsData& traits, D3DLIGHT& light);

  // Raster image caching (returns 0xFFFFFFFF if raster image is not cached)
  OdUInt32 findRasterImage(const OdGiRasterImage *pImg, bool bTransparency, ODCOLORREF fgColor, bool bSecColor, ODCOLORREF bkColor, double fBrightness, double fContrast, double fFade) const;
  OdUInt32 addRasterImage(RasterImageRef &pTexture, const OdGiRasterImage *pImg, bool bTransparency, ODCOLORREF fgColor, bool bSecColor, ODCOLORREF bkColor, double fBrightness, double fContrast, double fFade, bool bIncrement = true);
  RasterImageRef incrementRasterImageRef(OdUInt32 uNum);
  bool incrementRasterImageRef(CComPtr<IDirect3DTexture> pTexture);
  bool decrementRasterImageRef(OdUInt32 uNum);
  bool decrementRasterImageRef(CComPtr<IDirect3DTexture> pTexture);
  bool isSecondaryColorNeed() const;
  ODCOLORREF getSecondaryColor() const;

  OdGiMaterialTextureManager *materialTextureManager();
  D3dResourceSharingProvider *sharingProvider();
  OdGsBaseDeviceMTHelpers *mtDeviceSync();

  // VisualStyle
  void setVisualStyle(const OdGiVisualStyle &visualStyle);

  // Materials support
  bool skipMaterialProcess(OdDbStub *materialId) const;
  OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData);
  void renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId);
  void playDelayCacheEntry(const DelayCacheEntry *pEntry);
  void mapperChangedForDelayCache();

  OdGiMaterialItemPtr geometryCurrentMaterial() const;
  void geometryResetCurrentMaterial(OdGiMaterialItemPtr pMaterial);
  bool geometryIsMaterialEnabled() const;
  bool geometryIsMaterialAvailable() const;
  void geometryResetCurrentMaterial();
  OdGiMapperItemPtr geometryCurrentMapper(bool bForCoords = true) const;
  void geometryResetCurrentMapper(OdGiMapperItemPtr pMapper);
  bool geometryIsMapperEnabled() const;
  bool geometryIsMapperAvailable() const;
  bool geometryIsMappingDelayed() const;
  void geometryAppendDelayCacheEntry(void *pEntry);

  // override viewport frame rendering
  void drawViewportFrame();

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  // TtfFonts cache support
  OdRxObjectPtr tfcNewMetafile();
  OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile);
  void tfcFinalizeMetafile(OdRxObject *pMetafile);
  void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile) const;
  OdTtfFontsCache *getTtfFontsCache();

  OdColorConverterCallback *getColorConverter();
  DWORD getBackgroundColor();

  virtual void updatePaletteOverride(const OdGiPalette *pPalette);

  bool useLutPaletteMonochrome() const;
  bool useLutPaletteColor() const;

  // Background rendering compatibility
protected:
  void display(bool update);
  void setupDepthBias(bool bEnable);
  void setupDisplayOpts(bool bEnable);
  void renderBackground(bool secondPass = false);
  void directRenderOutputImage(const OdGePoint3d *pRect, const OdGiRasterImage *pImage,
                               const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &driParams);
  OdUInt32 directRenderOutputFlags() const;

  // directRenderImages cache
  struct DRICacheElement
  {
    DRICacheElement();
    ~DRICacheElement();

    void initialize(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage, ExGsDirectXView &view);
    void destroy();

    bool isBackground() const;
    bool isInitialized() const;

    bool isEqual(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage) const;

    const OdGiDrawable *pDrawable; // For pointer compare only
    const OdGiRasterImage *pImage; // For pointer compare only
    OdGiDrawable::DrawableType dType;
    CComPtr<IDirect3DTexture> pTexture;
  };
  OdArray<DRICacheElement> m_driCache;
};

#include "TD_PackPop.h"

#endif // __OD_EX_DIRECT_X_DEVICE__
