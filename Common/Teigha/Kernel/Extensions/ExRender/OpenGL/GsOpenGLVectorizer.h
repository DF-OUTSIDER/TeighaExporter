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

#ifndef ODOPENGLVECTORIZER_INCLUDED
#define ODOPENGLVECTORIZER_INCLUDED

#include "TD_PackPush.h"

#include "Gs/GsBaseInclude.h"
#include "Gs/GsBaseMaterialView.h"
#include "Gs/GsProperties.h"
#include "RxObjectImpl.h"
#include "Gi/GiPlotStyleDefs.h"
#include "Gi/GiGeometrySimplifier.h"
#include "ExFPPrecisionFix.h"
#include "../ExColorConverterCallback.h"
#include "GsOpenGLInclude.h"

class OdGiLightTraitsData;
class OdGeQuaternion;

#ifdef OD_OGL_ES
#include "OdStack.h"

/** \details
  <group ExRender_Classes> 
*/
struct OdGlEsTexGenSaver
{
  double sPlane[4];
  double tPlane[4];
};

/** \details
  <group ExRender_Classes> 
*/
class OdGlEsStencilBitSaver
{
  protected:
    struct Entry
    {
      GLboolean m_enabled;
      GLenum m_func;
      GLint m_ref;
      GLuint m_mask;
      GLenum m_fail;
      GLenum m_zfail;
      GLenum m_zpass;
      GLuint m_writemask;
    };
    OdStack<Entry> m_stack;
  public:
    OdGlEsStencilBitSaver() { }

    void pushAttrib();
    void popAttrib();
};
#endif

/** \details
  This class implements device -independent OpenGL Vectorizer devices.
  Library: Source code provided.

  <group ExRender_Classes> 
*/
class OdGsOpenGLVectorizeDevice : public OdGsBaseVectorizeDevice
{
  friend class OdGsOpenGLVectorizeView;

  bool m_bDiscardBackFaces;
  bool m_bClearScreen;
  bool m_bForceAlternativeHlt;
  OdUInt32 m_alternativeHltColor;
  bool m_bUseTTFCache;
  OdUInt32 m_bUseLutPalette;
  bool m_bLinesRepMode;
  OdUInt32 m_lineweightOptimization;
  OdGiMaterialTextureManagerPtr m_pTexManager;
  OdGsBaseDeviceMTHelpers m_deviceSync;
  GLint m_maxTextureSize;
public:
  // List of OpenGL extensions useable by device
  enum OpenGLExtension
  {
    OpenGLExtension_EXT_bgra = 0,  // GL_EXT_bgra OpenGL extension
    OpenGLExtension_ARB_NPOT,      // GL_ARB_texture_non_power_of_two
    OpenGLExtension_ARB_Mirror,    // GL_ARB_texture_mirrored_repeat
    OpenGLExtension_ARB_Transpose, // GL_ARB_transpose_matrix
    OpenGLExtension_EXT_BlendClr,  // GL_EXT_blend_color
    OpenGLExtension_EXT_Dxt,       // GL_EXT_texture_compression_s3tc
    OpenGLExtension_EXT_EdgeClamp, // GL_EXT_texture_edge_clamp
    OpenGLExtension_ARB_VertexBuf, // GL_ARB_vertex_buffer_object
    OpenGLExtension_ARB_PixelBuf,  // GL_ARB_pixel_buffer_object
    OpenGLExtension_ARB_FragProg,  // GL_ARB_fragment_program
#if defined(ODA_WINDOWS)
    OpenGLExtension_ARB_PixelFormat, // WGL_ARB_pixel_format
    OpenGLExtension_ARB_PBuffer,     // WGL_ARB_pbuffer
#endif

    OpenGLExtension_NExtensions    // Count of extensions useable by device
  };
  // List of OpenGL extension functions useable by device
  enum OpenGLExtFunc
  {
    // GL_ARB_transpose_matrix functions
    OpenGLExtFunc_ARB_LoadTransposeMatrixf = 0,
    OpenGLExtFunc_ARB_LoadTransposeMatrixd,
    OpenGLExtFunc_ARB_MultTransposeMatrixf,
    OpenGLExtFunc_ARB_MultTransposeMatrixd,
    // GL_EXT_blend_color functions
    OpenGLExtFunc_EXT_BlendColor,
    // GL_EXT_texture_compression_s3tc functions
    OpenGLExtFunc_EXT_CompressedTexImage2D,
    // GL_ARB_pixel_buffer_object functions
    OpenGLExtFunc_ARB_BindBuffer,
    OpenGLExtFunc_ARB_DeleteBuffers,
    OpenGLExtFunc_ARB_GenBuffers,
    OpenGLExtFunc_ARB_IsBuffer,
    OpenGLExtFunc_ARB_BufferData,
    OpenGLExtFunc_ARB_BufferSubData,
    OpenGLExtFunc_ARB_GetBufferSubData,
    OpenGLExtFunc_ARB_MapBuffer,
    OpenGLExtFunc_ARB_UnmapBuffer,
    OpenGLExtFunc_ARB_GetBufferParameteriv,
    OpenGLExtFunc_ARB_GetBufferPointerv,
    // GL_ARB_fragment_program functions
    OpenGLExtFunc_ARB_ActiveTexture,
    OpenGLExtFunc_ARB_ProgramString,
    OpenGLExtFunc_ARB_BindProgram,
    OpenGLExtFunc_ARB_DeletePrograms,
    OpenGLExtFunc_ARB_GenPrograms,
    OpenGLExtFunc_ARB_ProgramLocalParameter4f,
    OpenGLExtFunc_ARB_ProgramLocalParameter4fv,
#if defined(ODA_WINDOWS)
    // WGL_ARB_pixel_format functions
    OpenGLExtFunc_ARB_GetPixelFormatAttribiv,
    OpenGLExtFunc_ARB_GetPixelFormatAttribfv,
    OpenGLExtFunc_ARB_ChoosePixelFormat,
    // WGL_ARB_pbuffer functions
    OpenGLExtFunc_ARB_CreatePBuffer,
    OpenGLExtFunc_ARB_GetPBufferDC,
    OpenGLExtFunc_ARB_ReleasePBufferDC,
    OpenGLExtFunc_ARB_DestroyPBuffer,
    OpenGLExtFunc_ARB_QueryPBuffer,
#endif

    OpenGLExtFunc_NFuncs
  };
protected:
  enum OpenGLExtSupport
  {
    kExtSupportUndetermined = -1,
    kExtNotSupported = 0,
    kExtSupported = 1
  };
  mutable OpenGLExtSupport m_extAvail[OpenGLExtension_NExtensions];
  mutable void*            m_extFuncs[OpenGLExtFunc_NFuncs];

  virtual OpenGLExtSupport checkExtensionSupport(const char *pExtName, OpenGLExtension extIdx) const;
  virtual const char *getExtensionNameFromList(OpenGLExtension extIdx) const;
  // This is required to be sure if platform wrapper could provide function accessors
  virtual bool checkExtensionFuncSupport(OpenGLExtension extIdx) const; // Should be overrided by platform wrapper
  virtual void *getExtensionFuncPtrFromList(OpenGLExtFunc funcIdx) const;
  virtual void *getExtensionFuncPtrFromListInt(const char *pFuncName) const; // Should be overrided by platform wrapper

  virtual void onContextCreate();
  virtual void onContextDelete();
public:
  OdGsOpenGLVectorizeDevice();
  ~OdGsOpenGLVectorizeDevice();

  virtual void preReleaseResource() { }
  virtual void postReleaseResource() { }

  void updateScreen();

  // Don't support model sharing on this level
  bool isModelCompatible(OdGsModel* pModel) const { return OdGsDevice::isModelCompatible(pModel); }

  virtual bool supportMultithreading() const { return false; }

  OdColorConverterCallback *getColorConverter();
  ODCOLORREF colorConvert(ODCOLORREF color, bool bBackground = false);

  /** \details
    Clears (erases) this Vectorizer Device.
  */
  virtual void clear();
  /** \details
    Returns true if and only if back faces are discarded.
  */
  bool discardBackFaces() const { return m_bDiscardBackFaces; }
  /** \details
    Controls the discarding of back faces.
    \param discard [in]  Controls discarding of back faces.
  */
  void setDiscardBackFaces(bool discard) { m_bDiscardBackFaces = discard; }

  /** \details
    Returns common regeneration coefficient.
  */
  double getRegenCoef() const;

  /** \details
    Returns current state of ClearScreen flag.
  */
  bool isClearScreenEnabled() const { return m_bClearScreen; }
  /** \details
    Sets new state of ClearScreen flag.
    \param bSet [in]  New flag state.
  */
  void enableClearScreen(bool bSet) { m_bClearScreen = bSet; }

  /** \details
    Returns current state of AlternativeHlt flag.
  */
  bool isAlternativeHltEnabled() const { return m_bForceAlternativeHlt; }
  /** \details
    Sets new state of AlternativeHlt flag.
    \param bSet [in]  New flag state.
  */
#ifndef OD_OGL_ALTERNATIVEHLT
  void enableAlternativeHlt(bool bSet) { m_bForceAlternativeHlt = bSet; }
#else
  void enableAlternativeHlt(bool /*bSet*/) { }
#endif
  /** \details
    Returns alternative highlighting color.
  */
  OdUInt32 alternativeHltColor() const { return m_alternativeHltColor; }
  /** \details
    Sets alternative highlighting color.
    \param newColor [in]  New alternative highlighting color.
  */
  void setAlternativeHltColor(OdUInt32 newColor) { m_alternativeHltColor = newColor; }

  /** \details
    Returns current state of UseTTFCache flag.
  */
  bool isTTFCacheEnabled() const { return m_bUseTTFCache; }
  /** \details
    Sets new state of UseTTFCache flag.
    \param bSet [in]  New flag state.
  */
  void enableTTFCache(bool bSet) { m_bUseTTFCache = bSet; }

  /** \details
    Returns current state of UseLutPalette flags.
  */
  OdUInt32 useLutPalette() const { return m_bUseLutPalette; }
  /** \details
    Sets new state of UseLutPalette flags.
    \param bSet [in]  New flags state.
  */
  void setUseLutPalette(OdUInt32 bSet) { m_bUseLutPalette = bSet; }
  /** \details
    Checks does UseLutPalette flag is applicable for specified color depth.
  */
  bool canUseLutPalette(OdUInt32 colDepth) const
  {
    if (colDepth > 2) colDepth = 2;
    return GETBIT(m_bUseLutPalette, colDepth);
  }

  /** \details
    Returns current state of LinesRepMode flag.
  */
  bool isLinesRepModeEnabled() const { return m_bLinesRepMode; }
  /** \details
    Sets new state of LinesRepMode flag.
    \param bSet [in]  New flag state.
  */
  void enableLinesRepMode(bool bSet) { m_bLinesRepMode = bSet; }

  /** \details
    Returns requested point lineweight optimization state.
  */
  bool isPointLineweightOptimizationEnabled() const { return GETBIT(m_lineweightOptimization, 1); }
  /** \details
    Sets requested point lineweight optimization state.
    \ param bSet [in]  New flag state.
  */
  void enablePointLineweightOptimization(bool bSet) { SETBIT(m_lineweightOptimization, 1, bSet); }
  /** \details
    Returns requested line lineweight optimization state.
  */
  bool isLineLineweightOptimizationEnabled() const { return GETBIT(m_lineweightOptimization, 2); }
  /** \details
    Sets requested line lineweight optimization state.
    \ param bSet [in]  New flag state.
  */
  void enableLineLineweightOptimization(bool bSet) { SETBIT(m_lineweightOptimization, 2, bSet); }

  /** \details
    Returns true if and only if OpenGL extension supported by driver.
    \param ext [in]  Extension index from OpenGLExtension enum.
  */
  bool isExtensionSupported(OpenGLExtension extIdx) const;

  /** \details
    Returns extension function (should be compatible with internal protocol prototype)
    \param func [in]  Function index from OpenGLExtFunc enum.
  */
  void *getExtensionFunc(OpenGLExtFunc funcIdx) const;

  /** \details
    Returns material textures manager.
  */
  OdGiMaterialTextureManager *materialTextureManager();

  /** \details
    Returns MT synchronization helpers.
  */
  OdGsBaseDeviceMTHelpers &mtDeviceSync() { return m_deviceSync; }

  // Extension helpers
  void glLoadTransposedMatrix(OdGeMatrix3d &x);

  GLint glMaxTextureSize() const { return m_maxTextureSize; }

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView);
  virtual OdGiRasterImagePtr snapshotImageParams() const;
  virtual OdGiRasterImagePtr snapshotRegion(const OdGsDCRect &area, bool bCrop) const;
};

/** \details
  This class implements device -independent OpenGL vectorizer views.
  Library: Source code provided.
  <group ExRender_Classes> 
*/
class OdGsOpenGLVectorizeView : public OdGsBaseMaterialViewMT
                              , public OdGiGeometrySimplifier
                              , public ExFPPrecisionFix
#ifdef GL_VERSION_1_1
                              , protected OdGsPropertiesDirectRenderOutput
#endif
{
  friend class OdGsOpenGLVectorizeDevice;
protected:
  bool                      m_bFill;
  bool                      m_bTexCoords;
  bool                      m_bIsVertsNormals;
  bool                      m_bFaceNormalsRequired;
  bool                      m_bAcceptColorChange;
  bool                      m_bRasterImage;
  bool                      m_bForceAlternativeHlt;
  bool                      m_bProcessingTTF;
  bool                      m_bDontShade;
  bool                      m_bFading;
  OdUInt8                   m_nCurFading;
  bool                      m_bBackfacesDiscarded;
  bool                      m_bDrawTransparency;
  bool                      m_bNoColorMod;
  bool                      m_bCheckFaceMaterials;
  float                     m_dLwScaler;
  OdGeVector3d              m_n1, m_n2;
  OdGeMatrix3d              m_correction;
  OdUInt8Array              m_colors;
  // number of non-default lights
  OdUInt32                  m_numLights;

  OdOpenGLLinePatt          m_lnPattState, m_lnPattPrevState;
  OdOpenGLFillPatt          m_fillPattState, m_fillPattPrevState;

  // Current recording metafile pointer
  OdRxObject*               m_pCurMetafile;
  GLfloat                   m_fCurLw;
  OdOpenGLLwdSetting        m_lineWeight;
  ODCOLORREF                m_cCurColor;

  // Color converter
  OdColorConverterCallback *m_pColorConverter;
  bool                      m_bConvertBackgroundColor;

  // Lineweights
  GLfloat                   m_fMaxLineWidth;
  mutable OdArray<OdGeVector3dArray> m_lwdCaps;

  // Transparency patterns
  mutable OdArray<OdUInt8Array> m_trpPats;

  // Array for optional texture coordinates
  OdGePoint2dArray m_texCoordsArray;
#ifdef OD_OGL_ES
  OdGlEsTexGenSaver m_esTexGenSaver;
  OdGlEsStencilBitSaver m_esStencilSaver;
#endif

  const OdGeMatrix3d& projectionCorrection() { return m_correction; }
  OdGeMatrix3d dividedDcToNormalizedDc() const;
  void getGLScreenRect(double& left, double& bottom, double& width, double& height);
  void initArrays(OdInt32 nbVertex,
                  const OdGePoint3d* pVertexList,
                  const OdGiVertexData* pVertexData);
  void uninitArrays(bool bVertexData);

  void calcFaceNormal( const OdInt32* indices );

  const OdGiPsLinetypes::PsLinetypeOGL   *m_psLinetypeDefs;
  const OdGiPsFillstyles::PsFillstyleOGL *m_psFillstyleDefs;
  static const GLubyte                    g_selPatt[128];

  virtual void beginFaces(GLenum mode, OdGsView::RenderMode rm);
  virtual void endFaces(OdGsView::RenderMode rm);
public:
  OdGsOpenGLVectorizeDevice* device() 
  {
    return static_cast<OdGsOpenGLVectorizeDevice*>(OdGsBaseVectorizeView::device()); 
  }
protected:
  /** \details
    Sets the draw color for this VectorizeView object.
    \param color [in]  Color.
    \param colorIndex [in]  Color index.
    \param transparency [in]  Transparency.
  */
  void draw_color(const OdCmEntityColor& color);
  virtual void draw_color(ODCOLORREF color);
  void draw_color(OdUInt16 colorIndex);
  void draw_color(const OdCmEntityColor& color, const OdCmTransparency& transparency);
  void draw_color(OdUInt16 colorIndex, const OdCmTransparency& transparency);

  /** \details
    Sets "no color modification" mode state.
    \param bState [in]  New state.
  */
  virtual void draw_noColorMod(bool bState);
  
  /** \details
    Sets the draw lineweight for this VectorizeView object.
    \param lwLineWeight [in]  Lineweight as enum.
    \param lineweight [in]  Lineweight in pixels.
  */
  virtual void draw_lineweight(OdDb::LineWeight lwLineWeight, GLfloat lineweight);
  /** \details
    Sets the draw lineweight for this VectorizeView object.
    \param dLineWeight [in]  Plot style Lineweight.
    \param lineweight [in]  Lineweight in pixels.
  */
  virtual void draw_lineweight(double dLineWeight, GLfloat lineweight);
  /** \details
    Sets the draw lineweight for this VectorizeView object.
    \param nLineWeight [in]  Lineweight in pixels.
    \param bReset [in]  Reset previously set lineweight override.
  */
  virtual void draw_lineweight(OdInt32 nLineWeight, bool bReset);
  /** \details
    Sets the draw linetype for this VectorizeView object.
    \param lineType [in]  Linetype.
    \param isAdaptive [in]  Controls if linetype is adaptive.
    \param scaleFactor [in]  Scale factor for linetype.
  */
  virtual void draw_linetype(OdOpenGLLinePatt::Entry entry, OdInt32 state, bool isAdaptive = true, double scaleFactor = 1.0);
  /** \details
    Sets the draw fillstyle for this VectorizeView object.
    \param fillStyle [in]  Fillstyle.
  */
  virtual void draw_fillStyle(OdOpenGLFillPatt::Entry entry, OdInt32 state);
  /** \details
    Sets the draw line caps and joints style for this VectorizeView object.
    \param bByDefault [in]  Enables default line style.
    \param capStyle [in]  Line cap style for non-default line style.
    \param joinStyle [in]  Line join style for non-default line style.
  */
  virtual void draw_lineStyle(bool /*bByDefault*/,
    OdPs::LineEndStyle /*capStyle*/ = OdPs::kLesRound, OdPs::LineJoinStyle /*joinStyle*/ = OdPs::kLjsRound) { }

  bool usesDept(double* pMinDeptSupported, double* pMaxDeptSupported) const;
  bool sceneDept(double& zNear, double& zFar) const;
  OdGeMatrix3d projectionMatrixGL(bool bNoDept);
public:
  OdGsOpenGLVectorizeView();
  ~OdGsOpenGLVectorizeView();
  
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
  bool useSharedBlockReferences() const { return false; }
  void resetFading(bool bFlag);
  void alternativeHighlight(bool bHLT);

  /** \details
    Returns true if model cache enabled.
  */
  bool isModelCacheEnabled() const;

  /** \details
    Adds a light to this VectorizeView object.
    \param traits [in]  Light traits data.
  */
  OdUInt32 addLight(const OdGiLightTraitsData& traits);
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

  void beginViewVectorization();
  void loadViewport();
  void endViewVectorization();

  bool forceMetafilesDependence() const;
  double regenCoef();

  void onTraitsModified();
  void onHighlightModified(bool bDisableLine = false, bool bDisableFill = false);
  void renderTypeOnChange(OdGsModel::RenderType renderType);

  void initLights(OdRxIterator* pLightsIterator);
  /** \details
    Updates the lights attached to this VectorizeView object.
    \note
    The default implementation of this function sets the first light to a distant, "over-the-shoulder" light,
    and disables all other lights.
  */
  void updateLights();

  OdRxObjectPtrArray m_lights;

  void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    const OdGePoint2d* uvBoundary,
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* msg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pTextStyle);

  // OdGiConveyorGeometry
  // handling primitives in eye CS

  // required by simplifier

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);

  void polylineOut(OdInt32 numPoints, const OdInt32* vertexIndexList);


  void polygonOut(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal);

  void generateMeshFaces(OdInt32 rows, OdInt32 columns, const OdGiFaceData* pFaceData);

  bool generateShellFill(OdGiHatchPatternPtr pHatch, OdDouble& fillDensity, const OdGePoint3d* pVertexList,
                         OdInt32 faceListSize, const OdInt32* pFaceList,
                         const OdGiFaceData* pFaceData = 0, OdGiMapperItemEntry* pMapper = 0);


  void generateShellFaces(OdInt32 faceListSize, const OdInt32* faceList,
                          const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData);

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

  /** \details
    Prepares this Vectorizer object for shell and mesh face generation.
    \param pFaceData [in]  Pointer to additional face data.
  */
  virtual void prepareShMFacesGeneration(const OdGiFaceData* pFaceData, OdGsView::RenderMode rm);
  bool isTextureCoordinatesEnabled(OdGsView::RenderMode rm) const;
  bool isTextureCoordinatesPredefined() const
    { return vertexData() && vertexData()->mappingCoords(OdGiVertexData::kAllChannels); }
  
  void meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal);
  void shellFaceOut(OdInt32 faceListSize, const OdInt32* faceList, const OdGeVector3d* pNormal);

  void triangleOut( const OdInt32* vertices, const OdGeVector3d* pNormal );

  void initTexture(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    bool transparency,
    double brightness,
    double contrast,
    double fade);
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
  virtual void glInitTexture(OdOpenGLAlphaTestType bEnableAlphaTest, GLint glFormat, GLsizei pxWidth, GLsizei pxHeight,
                             const void *pTexture, GLint glFilter, GLint glEnvMode,
                             const OdGeQuaternion &glSPlane, const OdGeQuaternion &glTPlane,
                             OdUInt32 bufSize, bool bBitonal, const void *pPalTexture, OdUInt32 lutSize);
  void glInitLutTexture(const void *pPalTexture, OdUInt32 lutSize);
  virtual bool canUseLut() const { return false; }
  void glInitTextureTransform(OdOpenGLAlphaTestType bEnableAlphaTest, bool bBitonal, GLint glEnvMode,
                              const OdGeQuaternion &glSPlane, const OdGeQuaternion &glTPlane);

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

  /* These are to be simplified by OdGiGeometrySimplifier

  void polygonOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal = 0);

  void polylineProc(OdInt32 nbPoints, const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker lBaseSubEntMarker = -1);

  void textProc(const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);

  void polygonOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal = 0);

  void polylineProc(OdInt32 nbPoints, const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker lBaseSubEntMarker = -1);

  void polygonProc(OdInt32 nbPoints, const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0);

  void circleProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0);

  void circleProc(const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&, const OdGeVector3d* pExtrusion = 0);

  void circularArcProc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  void circularArcProc(
    const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  void nurbsProc(const OdGeNurbCurve3d& nurbs);

  void ellipArcProc(const OdGeEllipArc3d& arc,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  void ellipArcProc(const OdGePoint3d& center,
    const OdGeVector3d& majorRadius,
    const OdGeVector3d& minorRadius,
    const OdGePoint3d& startPoint,
    const OdGePoint3d& endPoint,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  void shapeProc(const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v,
    int shapeNo, const OdGiTextStyle* pStyle,
    const OdGeVector3d* pExtrusion = 0);

  void shellProc(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void xlineProc(const OdGePoint3d&, const OdGePoint3d&);

  void rayProc(const OdGePoint3d&, const OdGePoint3d&);

  void metafileProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool bDcAligned = true,           // reserved
    bool bAllowClipping = false); // reserved
  */

  virtual void ownerDrawDc(
    const OdGePoint3d& origin, 
    const OdGeVector3d& u, 
    const OdGeVector3d& v,
    const class OdGiSelfGdiDrawable* pDrawable, 
    bool dcAligned = true, 
    bool allowClipping = false);

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  // Materials support
protected:
  // OdGsBaseMaterialView overrides
  virtual bool getDeviceMapperMatrix(OdGeMatrix3d &dm) const;
  virtual bool skipMaterialProcess(OdDbStub *materialId) const;
  virtual void nullMaterialStub();
  virtual OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData);
  virtual void renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId);
  void renderMaterial(OdGiMaterialItemPtr pMaterialItem);
  virtual OdRxClassPtr createMaterialDesc() const;
  void checkFaceMaterialChange();
  // override viewport frame rendering
  void drawViewportFrame();
  // Background rendering compatibility
protected:
  void display(bool update);
  void renderBackground(bool secondPass = false);
#ifdef GL_VERSION_1_1
  void directRenderOutputImage(const OdGePoint3d *pRect, const OdGiRasterImage *pImage,
                               const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &driParams);
  OdUInt32 directRenderOutputFlags() const;

  // directRenderImages cache
  struct DRICacheElement
  {
    DRICacheElement();
    DRICacheElement(const DRICacheElement &celm);
    ~DRICacheElement();

    const DRICacheElement &operator =(const DRICacheElement &celm);

    void initialize(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage, OdGsOpenGLVectorizeView &view);
    void destroy();

    bool isBackground() const;
    bool isInitialized() const;

    bool isEqual(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage) const;

    const OdGiDrawable *pDrawable; // For pointer compare only
    const OdGiRasterImage *pImage; // For pointer compare only
    OdGiDrawable::DrawableType dType;
    OdUInt32 uTexture;
    mutable OdUInt32 nRefs;
  };
  OdArray<DRICacheElement> m_driCache;
#endif
  public:
    void renderLineCap(int iPx, const OdGePoint3d *origin, const OdGeVector3d &fwd, const OdGeVector3d &side, const OdUInt8 *color,
                       OdUInt8 nColorComponents = 4, const OdUInt8 *color2 = NULL) const;
    void clearLineCaps() const;
    bool validateMetafileExtents() const;
    const OdUInt8 *renderTransparency(OdUInt8 transp) const;
    void clearTransparencies() const;
    void updateLinePattState();
    void updateFillPattState();
    inline bool forceAlternativeHlt() const { return m_bForceAlternativeHlt; }
    OdInt32 lwdSettingToPixels(const OdOpenGLLwdSetting &lineWeight) const;
};

// Internal types which now moved to header

/** \details
  
  Library: Source code provided.

  <group ExRender_Classes> 
*/
class OdOpenGLDynamicSubList;

/** \details
  
  Library: Source code provided.

  <group ExRender_Classes> 
*/
class OdOpenGLCheckMarkIface
{
  public:
    virtual bool metafileCheckMarker(OdOpenGLGsMarker gsMarker, bool bPrevState) = 0;
    virtual void metafileSelectMarker(OdOpenGLGsMarker gsMarker) = 0;
};

/** \details
  
  Library: Source code provided.

  <group ExRender_Classes> 
*/
class OdOpenGLDynamicSubListProc
{
  OdOpenGLDynamicSubList *m_pDynSubList;
  public:
    OdOpenGLDynamicSubListProc();
    virtual ~OdOpenGLDynamicSubListProc();

    void clear();

    void end();

    void play(OdGsOpenGLVectorizeView *pOGLView, bool bHighlighted = false, OdOpenGLCheckMarkIface *pCheckMark = NULL) const;
    void play4Select(OdGsOpenGLVectorizeView *pOGLView, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, OdOpenGLCheckMarkIface *pCheckMark = NULL) const;

    void appendDynamicLwLines(const OdGePoint3d *pPoints, OdInt32 nPoints, const OdOpenGLLwdSetting &lineWeight, ODCOLORREF cColor, OdGsMarker gsMark, bool bSelGeom, bool bSelIgnore, bool bHighlight);
    void appendDynamicLwLines(const OdGePoint3d *pPoints, const OdInt32* vertexIndexList, OdInt32 nPoints, const OdOpenGLLwdSetting &lineWeight, ODCOLORREF cColor, OdGsMarker gsMark, bool bSelGeom, bool bSelIgnore, bool bHighlight);
};

/** \details
  This class represents a texture data container.

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdGiOpenGLMaterialTextureData : public OdGiMaterialTextureData
{
  protected:
    OdGiPixelBGRA32Array m_pxArray; 
    OdUInt32 m_uWidth, m_uHeight;
    bool m_bInBGRA;
    GLint m_nBorder;
    OdUInt8Array m_pxOpacity;
  public:
    OdGiOpenGLMaterialTextureData();
    ~OdGiOpenGLMaterialTextureData();
    
    void setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image);
    void textureData(OdGiPixelBGRA32Array &data, OdUInt32 &width, OdUInt32 &height) const;
    bool haveData() const;

    const OdGiPixelBGRA32 *deviceTexture() const;

    virtual void renderTexture(OdGsOpenGLVectorizeDevice *pDevice);

    void setBorder(GLint nBorder);

    void convertToOpacityPattern();
    void adjustAlphaComponent(double fValue);

    inline const OdUInt8Array &accessOpacityPattern() const { return m_pxOpacity; }

    // RxClass
    class Desc : public OdRxClass
    {
      public:
        OdRxObjectPtr create() const;
        const OdString appName() const;
        AppNameChangeFuncPtr appNameCallbackPtr() const;
        const OdString dxfName() const;
        const OdString name() const;
        OdDb::DwgVersion getClassVersion(OdDb::MaintReleaseVer* = 0) const;
        OdUInt32 proxyFlags() const;
        OdRxClass* myParent() const;
        OdRxModule* module() const;
        void setConstructor(OdPseudoConstructorType);
        OdUInt32 customFlags() const;
		    OdRxOverruleIterator* getOverrule(OverrulingType t) const;
		    void setOverrule(OdRxOverruleIterator* pIt, OverrulingType t);
    };
};

/** \details
  Structure contains cached material data.

    <group !!RECORDS_tkernel_apiref>
*/
struct OdOpenGLMaterialData
{
  GLfloat m_paAmbient[4];
  GLfloat m_paDiffuse[4];
  GLfloat m_paSpecular[4];
  GLfloat m_fSpecularPower;
  GLfloat m_paEmission[4];
  bool m_bUseDiffuseMat;
  bool m_bUseAmbientMat;
  bool m_bUseSpecularMat;
  GLenum m_nUseChannel;
  double m_fOpacityPercentage;
  double m_fDiffuseBlendFactor; // 0 - no diffuse map, 1 - opaqe diffuse map
  OdGiMapper::Tiling m_uTiling, m_vTiling;
};

/** \details
  This class represents a cached data container.

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLMaterialDataContainer : public OdRxObject
{
  protected:
    OdOpenGLMaterialData m_mData;
  public:
    OdOpenGLMaterialDataContainer()
    {
    }
    ~OdOpenGLMaterialDataContainer()
    {
    }

    inline const OdOpenGLMaterialData &data() const
    {
      return m_mData;
    }
    inline OdOpenGLMaterialData &data()
    {
      return m_mData;
    }
    inline void setData(const OdOpenGLMaterialData &mData)
    {
      m_mData = mData;
    }
};

/** \details
  Wraps resource releasing.

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLWrapResourceRelease
{
  protected:
    OdGsOpenGLVectorizeDevice *m_pDevice;
  public:
    OdOpenGLWrapResourceRelease(OdGsOpenGLVectorizeDevice *pDevice)
      : m_pDevice(pDevice)
    {
      if (pDevice) pDevice->preReleaseResource();
    }
    ~OdOpenGLWrapResourceRelease()
    {
      if (m_pDevice) m_pDevice->postReleaseResource();
    }
};

//

bool OpenGLParseExtension_base(const char *pExtName, const char *pExtensions = NULL);

#include "TD_PackPop.h"

#endif //ODOPENGLVECTORIZER_INCLUDED
