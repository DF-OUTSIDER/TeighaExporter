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

#ifndef ODOPENGLSTREAMVECTORIZER_INCLUDED
#define ODOPENGLSTREAMVECTORIZER_INCLUDED

#include "TD_PackPush.h"

// Comment to disable metafile stream implementation
#define OD_OGL_ENABLEMETASTREAM

#include "GsOpenGLVectorizer.h"
#include "ExOpenGLMetafileStream.h"
#include "ExOpenGLMetafileWriter.h"
#include "ExOpenGLMetafileReader.h"
#include "TtfFontsCache.h"

// Enable/disable textures/raster images binding
#ifdef GL_VERSION_1_1
#define OD_OGL_ENABLETEXBINDING
#endif

// Ability to share textures data between OpenGL contexts.
// Internal OpenGL mechanisms (like wglShareLists) have some limitations, so do all job manually.
#define OD_OGL_TEXTURESHARING

#ifdef OD_OGL_TEXTURESHARING

#include "OdList.h"

/** \details
    
    \sa

    <group !!RECORDS_tkernel_apiref> 
*/
struct OdOpenGLArrayWrapperShared : public OdRxObject, public OdOpenGLArrayWrapper
{
  static OdSmartPtr<OdOpenGLArrayWrapperShared> createSharedArray();
  ~OdOpenGLArrayWrapperShared()
  {
    clearArray();
  }
};
typedef OdSmartPtr<OdOpenGLArrayWrapperShared> OdOpenGLArrayWrapperSharedPtr;

/** \details
    
    \sa

    <group ExRender_Classes> 
*/
class OdOpenGLResourceSharingProvider
{
  public:
    struct OdOpenGLResourceShareRef;
    struct OdOpenGLResourceShareEntry
    {
      const void *m_pBasePtr;
      OdRxObjectPtr m_pLock; // Optionally lock m_pBasePtr memory block
      const void *m_pSecondPtr;
      OdRxObjectPtr m_pSecondLock; // Optionally lock m_pSecondPtr memory block
#ifdef OD_OGL_ENABLETEXBINDING
      GLuint m_uBinding;
      GLuint m_uSecondBinding;
#endif
      OdList<OdOpenGLResourceShareRef*> m_refs;
      OdOpenGLResourceSharingProvider *m_pProv;

      OdOpenGLResourceShareEntry()
        : m_pBasePtr(NULL)
        , m_pSecondPtr(NULL)
#ifdef OD_OGL_ENABLETEXBINDING
        , m_uBinding(0xFFFFFFFF)
        , m_uSecondBinding(0xFFFFFFFF)
#endif
      { }
      ~OdOpenGLResourceShareEntry();

      bool operator ==(const OdOpenGLResourceShareEntry& e2) const {
        return m_pBasePtr == e2.m_pBasePtr;
      }
    };
  protected:
    typedef OdList<OdOpenGLResourceShareEntry> shareEntries;
    shareEntries m_sharedEntries;
    OdGsOpenGLVectorizeDevice *m_pDevice;
  public:
    explicit OdOpenGLResourceSharingProvider() : m_pDevice(NULL) { }
    ~OdOpenGLResourceSharingProvider() { onManagerDestroy(); }

    void setDevice(OdGsOpenGLVectorizeDevice *pDevice) { m_pDevice = pDevice; }

    struct OdOpenGLResourceShareRef
    {
      typedef std::map<OdGsOpenGLVectorizeDevice*, OdOpenGLResourceShareEntry*> sharedDefs;
      mutable sharedDefs m_sharedDefs;

      OdOpenGLResourceShareRef() { }
      ~OdOpenGLResourceShareRef() { unlinkShared(); }

      OdOpenGLResourceShareEntry *getShared(OdGsOpenGLVectorizeDevice* pCurDevice) const;
      OdOpenGLResourceShareEntry *createShared(OdGsOpenGLVectorizeDevice* pCurDevice, /*this*/ OdOpenGLResourceSharingProvider *pProv) const;
      void unlinkShared(OdGsOpenGLVectorizeDevice* pDevice);
      void unlinkShared();

      // Client overrides
      virtual const void* rsrAccessPtr(int /*n*/) const { return NULL; }
      virtual OdRxObject *rsrAccessObj(int /*n*/) const { return NULL; }
#ifdef OD_OGL_ENABLETEXBINDING
      virtual void rsrRenderToBind(const void* /*pPtr*/, const void* /*pSecPtr*/, GLint /*secBind*/) { }
#endif
    };

    void onManagerDestroy();

    // Managing methods
    OdOpenGLResourceShareEntry *appendEntry(OdOpenGLResourceShareRef *pRef, OdGsOpenGLVectorizeDevice* pCurDevice /*this*/);
    void unlinkEntry(OdOpenGLResourceShareRef *pRef, OdGsOpenGLVectorizeDevice* pCurDevice, OdOpenGLResourceShareEntry *pEntry);
};

#endif

/** \details
    
    \sa

    <group ExRender_Classes> 
*/
class OdOpenGLFragmentProgram
{
  protected:
    int m_bTried;
    GLuint m_progBind;
  public:
    OdOpenGLFragmentProgram() : m_bTried(0), m_progBind(0) { }

    bool isInited() const { return m_bTried > 0; }
    bool initShad(OdGsOpenGLVectorizeDevice *pDevice, const char *pAsm);
    void uninitShad(OdGsOpenGLVectorizeDevice *pDevice);

    void bind(OdGsOpenGLVectorizeDevice *pDevice) const;
    void unbind(OdGsOpenGLVectorizeDevice *pDevice) const;
    bool canBind() const { return m_bTried > 1; }

    void setParam(OdGsOpenGLVectorizeDevice *pDevice, GLuint nParam, GLfloat p1, GLfloat p2, GLfloat p3, GLfloat p4) const;
    void setParam(OdGsOpenGLVectorizeDevice *pDevice, GLuint nParam, const GLfloat *ps) const;
};

/** \details
  This class implements streamed device -independent OpenGL Vectorizer devices.
  Library: Source code provided.

  <group ExRender_Classes> 
*/
class OdGsOpenGLStreamVectorizeDevice
  : public OdGsOpenGLVectorizeDevice
  , public OdTtfFontsCacheCallback
  , public OdGsMaterialTextureDataFiler
{
  friend class OdGsOpenGLStreamVectorizeView;

public:
  class RasterImageCacheCallback
  {
    public:
      virtual void rasterImageCacheDestroyed() = 0;
  };
  struct RasterImageInfo
  {
    const OdGiRasterImage *m_pImg;
    bool m_bTransparency;
    ODCOLORREF m_fgColor;
    bool m_bSecColor;
    ODCOLORREF m_bkColor;
    double m_fBrightness;
    double m_fContrast;
    double m_fFade;

    bool operator ==(const RasterImageInfo &riInfo) const
    {
      return (m_pImg == riInfo.m_pImg) && (m_bTransparency == riInfo.m_bTransparency) &&
             ((m_bTransparency) ? (m_fgColor == riInfo.m_fgColor) : true) &&
             (m_bSecColor == riInfo.m_bSecColor) && ((m_bSecColor) ? (m_bkColor == riInfo.m_bkColor) : true) &&
             (m_fBrightness == riInfo.m_fBrightness) && (m_fContrast == riInfo.m_fContrast) &&
             (m_fFade == riInfo.m_fFade);
    }
    bool operator !=(const RasterImageInfo &riInfo) const
    {
      return (m_pImg != riInfo.m_pImg) || (m_bTransparency != riInfo.m_bTransparency) ||
             ((m_bTransparency) ? (m_fgColor != riInfo.m_fgColor) : false) ||
             (m_bSecColor != riInfo.m_bSecColor) || ((m_bSecColor) ? (m_bkColor != riInfo.m_bkColor) : false) ||
             (m_fBrightness != riInfo.m_fBrightness) || (m_fContrast != riInfo.m_fContrast) ||
             (m_fFade != riInfo.m_fFade);
    }
    RasterImageInfo &operator =(const RasterImageInfo &riInfo)
    {
      m_pImg = riInfo.m_pImg;
      m_bTransparency = riInfo.m_bTransparency;
      m_fgColor = riInfo.m_fgColor;
      m_bSecColor = riInfo.m_bSecColor;
      m_bkColor = riInfo.m_bkColor;
      m_fBrightness = riInfo.m_fBrightness;
      m_fContrast = riInfo.m_fContrast;
      m_fFade = riInfo.m_fFade;
      return *this;
    }
  };
  struct RasterImageCacheEntry : public RasterImageInfo
  {
#ifdef OD_OGL_TEXTURESHARING
    // The bits array must be always available to provide sharing
    OdOpenGLArrayWrapperSharedPtr m_texData;
    OdOpenGLArrayWrapperSharedPtr m_palData;
#ifdef OD_OGL_ENABLETEXBINDING
    GLuint m_uBinding;
    GLuint m_uPalBinding;
#endif
#else
#ifndef OD_OGL_ENABLETEXBINDING
    OdOpenGLArrayWrapper m_texData;
    OdOpenGLArrayWrapper m_palData;
#else
    GLuint m_uBinding;
    GLuint m_uPalBinding;
#endif
#endif
    OdUInt32 m_uLutSize;
    OdUInt32 m_uRefCounter;
    //
    OdArray<RasterImageCacheCallback*> m_callbacks;
  };
  enum FragmentProgram
  {
    kPaletteShader = 0,

    NFragmentPrograms
  };
private:
  OdArray<RasterImageCacheEntry*> m_rasterImageCache;
  OdArray<RasterImageCacheCallback*> m_resourceCallbacks;
#ifdef OD_OGL_TEXTURESHARING
  OdOpenGLResourceSharingProvider m_sharingProv;
#endif
  OdTtfFontsCachePtr m_ttfFontsCache;
  OdOpenGLFragmentProgram m_fragmentProgram[NFragmentPrograms];
public:
  // Raster images cache support
  void clearRasterImageCache();
  RasterImageCacheEntry *findRasterImage(const RasterImageInfo &riInfo) const;
#ifndef OD_OGL_ENABLETEXBINDING
  RasterImageCacheEntry *addRasterImage(const RasterImageInfo &riInfo, const void *pData, OdUInt32 uSize,
                                        const void *pPalData, OdUInt32 uLutSize);
#else
#ifndef OD_OGL_TEXTURESHARING
  RasterImageCacheEntry *addRasterImage(const RasterImageInfo &riInfo, GLuint uBinding, OdUInt32 uLutSize, GLuint uPalBinding);
#else
  RasterImageCacheEntry *addRasterImage(const RasterImageInfo &riInfo, const void *pData, OdUInt32 uSize, GLuint uBinding,
                                        const void *pPalData, OdUInt32 uLutSize, GLuint uPalBinding);
#endif
#endif
  void incrementRasterImageRef(RasterImageCacheEntry *pEntry, RasterImageCacheCallback *pCallback);
  void decrementRasterImageRef(RasterImageCacheEntry *pEntry, RasterImageCacheCallback *pCallback);
  void addResourceCallback(RasterImageCacheCallback *pCallback);
  void removeResourceCallback(RasterImageCacheCallback *pCallback);
#ifdef OD_OGL_TEXTURESHARING
  OdOpenGLResourceSharingProvider &sharingProvider() { return m_sharingProv; }
#endif
  OdTtfFontsCache *ttfFontsCache() { return m_ttfFontsCache.get(); }
  const OdTtfFontsCache *ttfFontsCache() const { return m_ttfFontsCache.get(); }

  OdOpenGLFragmentProgram &fragmentProgram(FragmentProgram nProg) { return m_fragmentProgram[nProg]; }
  const OdOpenGLFragmentProgram &fragmentProgram(FragmentProgram nProg) const { return m_fragmentProgram[nProg]; }
protected:
  void onContextDelete();
public:
  OdGsOpenGLStreamVectorizeDevice();
  ~OdGsOpenGLStreamVectorizeDevice();

  // Open sharing for this level
  bool isModelCompatible(OdGsModel* pModel) const { return OdGsBaseVectorizeDevice::isModelCompatible(pModel); }

  ODRX_USING_HEAP_OPERATORS(OdGsOpenGLVectorizeDevice);

  // TtfFonts cache support
  OdRxObjectPtr tfcNewMetafile(void *pSessionId);
  OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId);
  void tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId);
  void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const;
  bool tfcSaveMetafile(const OdRxObject* pMetafile, OdGsFiler *pFiler);
  OdRxObjectPtr tfcLoadMetafile(OdGsFiler *pFiler);

  virtual bool isSupportDeviceStateSaving() const;
  virtual bool saveClientDeviceState(OdGsFiler *pFiler) const;
  virtual bool loadClientDeviceState(OdGsFiler *pFiler);
  virtual bool saveClientMaterialTextureData(const OdGiMaterialTextureData *pTextureData, OdGsFiler *pFiler);
  virtual OdGiMaterialTextureDataPtr loadClientMaterialTextureData(OdGsFiler *pFiler);
};

/** \details
  This class implements streamed device -independent OpenGL vectorizer views.
  Library: Source code provided.
  <group ExRender_Classes> 
*/
class OdGsOpenGLStreamVectorizeView : public OdGsOpenGLVectorizeView
                                    , protected OdOpenGLMetafileReaderCallback
                                    , public OdOpenGLCheckMarkIface
{
protected:
  bool                      m_bIsVertsColors;
  OdRxObjectPtr             m_pTexObject;
  OdGsOpenGLStreamVectorizeDevice::RasterImageInfo m_procTexInfo;
  OdGiXformPtr              m_pSelectXfm;
  OdGiConveyorConnector     m_mfEntryPoint;
  bool                      m_bDynamicSubEntHlt;
  bool                      m_bSecondaryBinding;

  friend class OdOpenGLStreamList;
  OdOpenGLMetafileWriter    m_metaWrite;
  OdOpenGLMetafileReader    m_metaRead;

  struct LwdDataCache
  {
    bool m_bEnabled; // Lineweight enabled
    int m_nPix; // Number of pixels for lineweight
    bool m_bForced; // Points lineweight forced
    bool m_bExtendPoints; // Enable extended lineweights mode for points
    bool m_bExtendLines; // Enable extended lineweights mode for points
    bool m_bPerspective; // Perspective mode (coef couldn't be precomputed)
    double m_dPreCft; // Precomputed ortho coefficient
    bool m_bPrecomputed; // Enabled if parameters precomputed for current view state
    OdGeVector3d m_basis[3]; // View basis
    OdGeVector3d m_xfmBasis[3]; // Xformed basis
    OdGeMatrix3d m_xfm; // Stored transform

    LwdDataCache() : m_bEnabled(false), m_nPix(1), m_bForced(false), m_bExtendPoints(true), m_bExtendLines(true)
                   , m_bPerspective(false), m_dPreCft(0.0), m_bPrecomputed(false) { }
  } m_metaLwd;

  OdOpenGLLwdStyle m_styleLwd;

  void beginFaces(GLenum mode, OdGsView::RenderMode rm);
  void endFaces(OdGsView::RenderMode rm);

  void draw_color(ODCOLORREF color);
  void draw_noColorMod(bool bState);

  void draw_linetype(OdOpenGLLinePatt::Entry entry, OdInt32 state, bool isAdaptive = true, double scaleFactor = 1.0);
  void draw_fillStyle(OdOpenGLFillPatt::Entry entry, OdInt32 state);
  void draw_lineStyle(bool bByDefault,
    OdPs::LineEndStyle capStyle = OdPs::kLesRound, OdPs::LineJoinStyle joinStyle = OdPs::kLjsRound);

  void pushTriangle(double *texCoords, const OdInt32* indices, const OdGeVector3d *n1, const OdGeVector3d *n2, const OdGeVector3d *n3);

  void metafileLineStipple(OdOpenGLLinePatt::Entry entry, OdUInt8 state);
  void metafileFillStipple(OdOpenGLFillPatt::Entry entry, OdUInt8 state);

  const OdOpenGLLinePatt *metafileGetLinePattPtr() const;
  const OdOpenGLFillPatt *metafileGetFillPattPtr() const;
public:
  OdGsOpenGLStreamVectorizeView();
  ~OdGsOpenGLStreamVectorizeView();
  
  OdRxObjectPtr newGsMetafile();
  void beginMetafile(OdRxObject* pMetafile);
  void endMetafile(OdRxObject* pMetafile);
  void playMetafile(const OdRxObject* pMetafile);

  bool saveMetafile(const OdRxObject* pMetafile, OdGsFiler *pFiler);
  OdRxObjectPtr loadMetafile(OdGsFiler *pFiler);
  bool saveClientMaterialCache(const OdRxObject* pMtl, OdGsFiler *pFiler);
  OdRxObjectPtr loadClientMaterialCache(OdGsFiler *pFiler, OdGiMaterialItem *pMatItem);
  bool saveClientMaterialTextureData(const OdGiMaterialTextureData *pTextureData, OdGsFiler *pFiler);
  OdGiMaterialTextureDataPtr loadClientMaterialTextureData(OdGsFiler *pFiler);
  bool saveClientViewState(OdGsFiler *pFiler) const;
  bool loadClientViewState(OdGsFiler *pFiler);

  void beginViewVectorization();
  void loadViewport();
  void drawViewportFrame();
  void endViewVectorization();

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);
  void polylineOut(OdInt32 numPoints, const OdInt32* vertexIndexList);

  void polygonOut(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal);

  bool isSMIndexable(const OdGiEdgeData* edgeData, const OdGiFaceData* faceData,
                     const OdGiVertexData* vertexData, bool bShell);

  void shellProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
  void meshProc(
    OdInt32 numRows, 
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void prepareShMFacesGeneration(const OdGiFaceData* pFaceData, OdGsView::RenderMode rm);
  
  void triangleOut( const OdInt32* vertices, const OdGeVector3d* pNormal );

  void uninitTexture();

  virtual bool isTextureLoaded(const OdGiRasterImage* pImage, 
    bool transparency,
    ODCOLORREF fgColor, // only if transparency enabled
    bool secColor,
    ODCOLORREF bkColor, // only if secColor enabled
    double brightness,
    double contrast,
    double fade,
    const OdUInt8 *&pData,
    OdUInt32 &bufSize,
    const OdUInt8 *&pPalData,
    OdUInt32 &lutSize);
  void glInitTexture(OdOpenGLAlphaTestType bEnableAlphaTest, GLint glFormat, GLsizei pxWidth, GLsizei pxHeight,
                     const void *pTexture, GLint glFilter, GLint glEnvMode,
                     const OdGeQuaternion &glSPlane, const OdGeQuaternion &glTPlane,
                     OdUInt32 bufSize, bool bBitonal, const void *pPalTexture, OdUInt32 lutSize);
  virtual bool canUseLut() const;
  void glBindSecondary(GLint secondBinding, OdUInt32 lutSize);

  virtual void metafileProcessInitTexture(const OdOpenGLMetafileReaderCallback::RasterImageEntrySet &dataSet, OdRxObject *pData = NULL);
  virtual void metafileProcessUninitTexture();

  void shell(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL);

  void polypointProc(OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);

  bool useSharedBlockReferences() const { return true; }

  void onTraitsModified();
  bool metafileNeedCheckMarker() const;
  bool metafileCheckMarker(OdOpenGLGsMarker gsMarker, bool bPrevState);
  void metafileSelectMarker(OdOpenGLGsMarker gsMarker);
  bool useMetafileAsGeometry() const { return m_bDynamicSubEntHlt; }
  inline bool dynamicSubEntHlt() const { return m_bDynamicSubEntHlt; }
  OdGiConveyorOutput& outputForMetafileGeometry();
  void setTransformForMetafileGeometry(const OdGeMatrix3d& mtx);
  OdGeMatrix3d getTransformForMetafileGeometry() const;
  void select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);
  OdGiConveyorGeometry &metafileEntryPoint() { return m_mfEntryPoint.geometry(); }
  void pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags = 0);
  void popMetafileTransform(OdUInt32 uFlags = 0);

#ifdef OD_OGL_ES
  void metafilePushStencilAttrib();
  void metafilePopStencilAttrib();
#endif
  bool isMappingDelayed() const;

  bool metafileForceAlternativeHlt() const { return forceAlternativeHlt(); }

  // Lineweights
  void draw_lineweight(OdDb::LineWeight lwLineWeight, GLfloat pixels);
  void draw_lineweight(double dLineWeight, GLfloat pixels);
  void draw_lineweight(OdInt32 nLineWeight, bool bReset);

  double computeLineweightCft(const OdGePoint3d *pOrigin);
  bool metafileProcessLineweight(const OdOpenGLLwdSetting &lwd);
  bool metafilePointsLineweight() const;
  bool metafileDrawExtendedPointLineweight() const;
  bool metafileDrawExtendedLineLineweight() const;
  void metafileDrawExtendedLine(const OdGePoint3d *origin, const OdUInt8 *color = NULL);
  void metafileDrawExtendedPoint(const OdGePoint3d *origin, const OdUInt8 *color = NULL);
  void metafileDrawExtendedLineStrip(const OdGePoint3d *pPoints, OdUInt32 nPoints, const OdUInt8 *color = NULL);
  void metafileGetBackgroundColor(ODCOLORREF &bkgndColor);
  OdColorConverterCallback *metafileColorConverter();

  void metafileLineweightStyle(bool bByDefault,
    OdPs::LineEndStyle lesStyle = OdPs::kLesRound, OdPs::LineJoinStyle ljsStyle = OdPs::kLjsRound);
  const OdOpenGLLwdStyle *metafileGetLwdStylePtr() const;

  // TtfFonts cache support
  OdRxObjectPtr tfcNewMetafile();
  OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile);
  void tfcFinalizeMetafile(OdRxObject *pMetafile);
  void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile) const;
  void metafileTtfText(OdUInt64 fontKeyA, OdUInt64 fontKeyB, const OdUInt32 *pText, OdUInt32 nText,
                       const OdGeMatrix3d &baseTransform, const OdGeVector3d &stepDir);
  void textProc(const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                const OdChar* msg, OdInt32 nLength, bool raw, const OdGiTextStyle* pStyle, const OdGeVector3d* pExtrusion);

  // Materials support
protected:
  // OdGsBaseMaterialView overrides
  virtual void renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId);
  virtual void metafileProcessMaterial(OdRxObject *pMetafile);
#ifdef OD_OGL_ENABLETEXBINDING
  virtual OdRxClassPtr createMaterialDesc() const;
#endif
  virtual void mapperChangedForDelayCache();
  virtual void playDelayCacheEntry(const DelayCacheEntry *pEntry);
  virtual void delayCacheProcessed(const OdGiDrawable *pDrawable);

  void display(bool update);
};

#include "TD_PackPop.h"

#endif //ODOPENGLSTREAMVECTORIZER_INCLUDED
