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

#ifndef __OD_EX_GI_DIRECT_X_GEOMETRY__
#define __OD_EX_GI_DIRECT_X_GEOMETRY__

#include "TD_PackPush.h"

#if defined(__BORLANDC__)
#include <atl\atlvcl.h> // CComPtr<>
#include "D3DInclude.h"
#else
#include <atlbase.h>
#include "D3DInclude.h"
#endif

#define STL_USING_MAP
#define STL_USING_LIST
#include "OdaSTL.h"

#include "UInt16Array.h"
#include "OdList.h"

#include "Gi/GiGeometrySimplifier.h"
#include "Ge/GeMatrix3d.h"
#include "Ps/PlotStyles.h"
#include "Gi/GiPlotStyleDefs.h"

#include "Gi/GiMaterialItem.h"
#include "Gi/GiMapperItem.h"

#include "ChunkAllocator.h"

#include "Gs/GsHighlightData.h"
#include "Gs/GsBaseVectorizer.h"

#include "D3DStateManager.h"
#include "../TtfFontsCache.h"
#include "../ExColorConverterCallback.h"

#define SWAPBYTES(a) ODRGB(ODGETBLUE(a), ODGETGREEN(a), ODGETRED(a))

// Enables dynamic subentity highlighting mode (should be optimized #7706)
//#define OD_D3D_DYNAMICSUBENTHLT // (moved to device properties #12382)

// Maximum vertices for line cap
#define OD_D3D_MAXLINECAPVERTS   15

// Keep temporary lwd buffer not larger this limit
#define OD_D3D_LWDBUFKEEPLIMIT   256

class ExGiDirectXGeometry;

class OdGiVisualStyle;
struct OdGiMaterialTraitsData;
class OdGsMaterialNode;
class OdGsBaseDeviceMTHelpers;

// A structure for our vertexes (vertex+color)

/** \details
  This structure represents vertices with color for ExGiDirectXGeometry classes.
*/
struct OdD3DVertexCol
{
  FLOAT x, y, z; // Transformed position of the vertex.
  DWORD color;   // Color of the vertex.
};

typedef OdArray<OdD3DVertexCol, OdMemoryAllocator<OdD3DVertexCol> > OdD3DVertexColArray;

/** \details
  This structure represents vertices with texture coordinates for ExGiDirectXGeometry classes.
*/
struct OdD3DVertexTexCol
{
  D3DXVECTOR3 position;   // Position of the vertex.
  DWORD color;            // Color of the vertex.
  FLOAT s, t;                // Texture coordinates.
};

typedef OdArray<OdD3DVertexTexCol, OdMemoryAllocator<OdD3DVertexTexCol> > OdD3DVertexTexColArray;

/** \details
  This structure represents vertices with normals for ExGiDirectXGeometry classes.
*/
struct OdD3DVertexNormalCol
{
  D3DXVECTOR3 position;   // Position of the vertex.
  D3DXVECTOR3 normal;     // Surface normal at the vertex.
  DWORD color;            // Color of the vertex.
};

typedef OdArray<OdD3DVertexNormalCol, OdMemoryAllocator<OdD3DVertexNormalCol> > OdD3DVertexNormalColArray;

/** \details
  This structure represents vertices with normals and texture coordinates for ExGiDirectXGeometry classes.
*/
struct OdD3DVertexNormalTexCol
{
  D3DXVECTOR3 position;   // Position of the vertex.
  D3DXVECTOR3 normal;     // Surface normal at the vertex.
  DWORD color;            // Color of the vertex.
  FLOAT s, t;             // Texture coordinates.
};

typedef OdArray<OdD3DVertexNormalTexCol, OdMemoryAllocator<OdD3DVertexNormalTexCol> > OdD3DVertexNormalTexColArray;

union OdD3DVertexUnionPtr
{
  const OdD3DVertexCol *m_pVC;
  const OdD3DVertexNormalCol *m_pVNC;
  const OdD3DVertexNormalTexCol *m_pVNTC;
};

struct OdD3DSelMark
{
  OdGsMarker gsMark;
  OdUInt32 uFrom;
};

typedef OdArray<OdD3DSelMark, OdMemoryAllocator<OdD3DSelMark> > OdD3DSelMarkArray;

/** \details
   This class store resources which must be recreated after D3D device lost state reset.
   Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dLostStateManager //: public OdRxObject
{
  public:
    class D3dLostStateEntry
    {
      public:
        D3dLostStateEntry() { }
        virtual ~D3dLostStateEntry() { }

        virtual void recreate(D3dLostStateManager *pManager) = 0;

        virtual void onManagerDestroy(D3dLostStateManager * /*pManager*/) { }
    };
  public:
    //ODRX_USING_HEAP_OPERATORS(OdRxObject);

    D3dLostStateManager();
    ~D3dLostStateManager();

    void clearEntriesList();

    bool isManagerEnabled() const;
    void enableManager(bool bEnable);

    void setD3DDevice(IDirect3DDevice* pD3Ddevice);
    IDirect3DDevice* d3dDevice() const;

    void appendEntry(D3dLostStateEntry *pNewEntry);
    void removeEntry(D3dLostStateEntry *pEntry);

    void executeRecreation();
  protected:
    bool m_bEnableManager;
    CComPtr<IDirect3DDevice> m_pD3Ddevice;
    OdList<D3dLostStateEntry*> m_entries;
};

/** \details
    This class provides texture resources sharing between DX devices.

    Library: Source code provided.

    <group ExRender_Windows_Classes>
*/
class D3dResourceSharingProvider : public D3dLostStateManager::D3dLostStateEntry
{
  public:
    struct D3dResourceShareRef;
  protected:
    struct D3dResourceShareEntry
    {
      IDirect3DTexture *m_pBasePtr;
      CComPtr<IDirect3DTexture> m_pEntry;
      OdList<D3dResourceShareRef*> m_refs;
      D3dResourceSharingProvider *m_pProv;

      bool operator ==(const D3dResourceShareEntry& e2) {
        return m_pBasePtr == e2.m_pBasePtr;
      }
    };
    typedef OdList<D3dResourceShareEntry> shareEntries;
    shareEntries m_sharedEntries;
  public:
    D3dResourceSharingProvider() { }
    ~D3dResourceSharingProvider() { onManagerDestroy(NULL); }

    struct D3dResourceShareRef
    {
      typedef std::map<IDirect3DDevice*, D3dResourceShareEntry*> sharedDefs;
      mutable sharedDefs m_sharedDefs;

      D3dResourceShareRef() { }
      ~D3dResourceShareRef() { unlinkShared(); }

      IDirect3DTexture *getShared(IDirect3DDevice* pCurDevice) const;
      IDirect3DTexture *createShared(IDirect3DDevice* pCurDevice, IDirect3DTexture* pBaseResource, D3dResourceSharingProvider *pProv) const;
      void unlinkShared(IDirect3DDevice* pDevice);
      void unlinkShared();
    };

    // D3dLostStateManager::D3dLostStateEntry interface
    void recreate(D3dLostStateManager *pManager);
    void onManagerDestroy(D3dLostStateManager *pManager);

    // Managing methods
    D3dResourceShareEntry *appendEntry(D3dResourceShareRef *pRef, IDirect3DDevice* pCurDevice, IDirect3DTexture* pBaseResource);
    void unlinkEntry(D3dResourceShareRef *pRef, IDirect3DDevice* pCurDevice, D3dResourceShareEntry *pEntry);
};

/** \details
    This class redirects current OdGsBaseMaterialView functions to geometry.

	Library: Source code provided.

    <group ExRender_Windows_Classes>
*/
class D3dMaterialViewRedirect
{
  public:
    virtual OdGiMaterialItemPtr geometryCurrentMaterial() const = 0;
    virtual void geometryResetCurrentMaterial(OdGiMaterialItemPtr pMaterial) = 0;
    virtual bool geometryIsMaterialEnabled() const = 0;
    virtual bool geometryIsMaterialAvailable() const = 0;
    virtual void geometryResetCurrentMaterial() = 0;
    virtual OdGiMapperItemPtr geometryCurrentMapper(bool bForCoords = true) const = 0;
    virtual void geometryResetCurrentMapper(OdGiMapperItemPtr pMapper) = 0;
    virtual bool geometryIsMapperEnabled() const = 0;
    virtual bool geometryIsMapperAvailable() const = 0;
    virtual bool geometryIsMappingDelayed() const = 0;
    virtual void geometryAppendDelayCacheEntry(void *pEntry) = 0;

    struct RasterImageRef
    {
      CComPtr<IDirect3DTexture> m_pTexture;
      int m_nTexLut;
      CComPtr<IDirect3DTexture> m_pPalette;
    };
    virtual OdUInt32 findRasterImage(const OdGiRasterImage *pImg, bool bTransparency, ODCOLORREF fgColor, bool bSecColor, ODCOLORREF bkColor, double fBrightness, double fContrast, double fFade) const = 0;
    virtual OdUInt32 addRasterImage(RasterImageRef &pTexture, const OdGiRasterImage *pImg, bool bTransparency, ODCOLORREF fgColor, bool bSecColor, ODCOLORREF bkColor, double fBrightness, double fContrast, double fFade, bool bIncrement = true) = 0;
    virtual RasterImageRef incrementRasterImageRef(OdUInt32 uNum) = 0;
    virtual bool incrementRasterImageRef(CComPtr<IDirect3DTexture> pTexture) = 0;
    virtual bool decrementRasterImageRef(OdUInt32 uNum) = 0;
    virtual bool decrementRasterImageRef(CComPtr<IDirect3DTexture> pTexture) = 0;
    virtual bool isSecondaryColorNeed() const = 0;
    virtual ODCOLORREF getSecondaryColor() const = 0;

    virtual bool metafileTransformOverrided() const = 0;
    virtual const OdGeMatrix3d &metafileToWorldMatrix() const = 0;
    virtual const OdGeMatrix3d &worldToMetafileMatrix() const = 0;
    virtual const OdGeMatrix3d &viewportToOutputMatrix() const = 0;
    virtual const OdGeMatrix3d &outputToViewportMatrix() const = 0;
    virtual void xformByMetafileTransform(OdGeMatrix3d &mtx) const = 0;
    virtual void xformByMetafileTransform(OdGeVector3d &vec) const = 0;

    virtual OdGiMaterialTextureManager *materialTextureManager() = 0;
    virtual D3dResourceSharingProvider *sharingProvider() = 0;
    virtual OdGsBaseDeviceMTHelpers *mtDeviceSync() = 0;
    virtual OdTtfFontsCache *getTtfFontsCache() = 0;

    virtual OdColorConverterCallback *getColorConverter() = 0;
    virtual DWORD getBackgroundColor() = 0;

    virtual const OdGsHlBranch* getCurrentHLTBranch() const = 0;
    virtual bool dynamicSubEntHlt() const = 0;
    virtual bool useLutPaletteMonochrome() const = 0;
    virtual bool useLutPaletteColor() const = 0;
};

// Material caching data

struct D3dMaterialRecordData {
  D3DMATERIAL m_d3dMat;
  bool m_bUseDiffuseMat;
  bool m_bUseAmbientMat;
  bool m_bUseSpecularMat;
  double m_fOpacityPercentage;
  double m_fDiffuseBlendFactor; // 0 - no diffuse map, 1 - opaqe diffuse map
  OdGiMapper::Tiling m_uTiling, m_vTiling;
};

// Material cache entry

/** \details
    This class defines the material cache for DirectX 3D devices.

    <group ExRender_Windows_Classes> 
*/
class ExD3dMaterialCacheEntry : public OdRxObject
{
public:
  //Contains cached material information for a DirectX device.
  D3dMaterialRecordData m_mat;

  ODRX_DECLARE_MEMBERS(ExD3dMaterialCacheEntry);
public:
  ExD3dMaterialCacheEntry()
  {
    //m_entry = OdGiMaterialItem::createObject();
  }
  virtual ~ExD3dMaterialCacheEntry() {
  }
};

typedef OdSmartPtr<ExD3dMaterialCacheEntry> ExD3dMaterialCacheEntryPtr;

// Material texture data container for D3D device

class D3dMetafilePlayContext;

/** \details
    This class defines the material texture data for DirectX 3D devices.

    <group ExRender_Windows_Classes> 
*/
class ExGiD3dMaterialTextureData : public OdGiMaterialTextureData, protected D3dLostStateManager::D3dLostStateEntry, protected D3dResourceSharingProvider::D3dResourceShareRef
{
  protected:
    CComPtr<IDirect3DTexture> m_pTexture;
    OdGiPixelBGRA32Array m_pLSBits;
    OdUInt32 m_uLSWidth, m_uLSHeight;
    D3dLostStateManager *m_pLSManager;
  public:
    ExGiD3dMaterialTextureData();
    ~ExGiD3dMaterialTextureData();

    ODRX_DECLARE_MEMBERS(ExGiD3dMaterialTextureData);
    
    void setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image);
    void textureData(OdGiPixelBGRA32Array &data, OdUInt32 &width, OdUInt32 &height) const;
    
    bool haveData() const;

    CComPtr<IDirect3DTexture> deviceTexture() const;
    CComPtr<IDirect3DTexture> deviceTexture(const D3dMetafilePlayContext &ctx) const;

  protected:
    void recreate(D3dLostStateManager *pManager);
    void onManagerDestroy(D3dLostStateManager *pManager);
};

typedef OdSmartPtr<ExGiD3dMaterialTextureData> ExGiD3dMaterialTextureDataPtr;

//

//typedef HRESULT (CALLBACK*D3DXCreateMeshFVF_PROC)(DWORD, DWORD, DWORD, DWORD, 
//                                                  LPDIRECT3DDEVICE8, LPD3DXMESH*);

/** \details
    Lineweight style parameters.

    <group ExRender_Windows_Classes> 
*/
struct D3dLwdStyle
{
  bool                m_bPointsLwd;
  bool                m_bCurLwdDef;
  bool                m_bCurLwdUsesDef;
  OdPs::LineEndStyle  m_curLwdLes;
  OdPs::LineJoinStyle m_curLwdLjs;
  bool                m_bDefLwdDef;
  OdPs::LineEndStyle  m_defLwdLes;
  OdPs::LineJoinStyle m_defLwdLjs;

  D3dLwdStyle()
    : m_bPointsLwd(true)
    , m_bCurLwdDef(true)
    , m_bCurLwdUsesDef(true)
    , m_curLwdLes(OdPs::kLesRound)
    , m_curLwdLjs(OdPs::kLjsRound)
    , m_bDefLwdDef(true)
    , m_defLwdLes(OdPs::kLesRound)
    , m_defLwdLjs(OdPs::kLjsRound)
  { }
};

/** \details
    Lineweight settings.

    <group ExRender_Windows_Classes> 
*/
struct D3dLwdSetting
{
  enum LwdType
  { kModel = 0, kPlot, kPoint, kAbsolute };
  OdUInt8            m_type;
  void setType(LwdType typ) { m_type = (OdUInt8)typ; }
  LwdType type() const      { return (LwdType)m_type; }
  bool isForcedLineweight() const { return m_type == kPoint; }
  union {
    double           m_dLineWeight;
    OdDb::LineWeight m_lwLineWeight;
    OdInt32          m_pixLineWeight;
  } m_val;
  D3dLwdSetting() : m_type(0) { m_val.m_dLineWeight = 0.0; }
  bool operator ==(const D3dLwdSetting &sec) const
  { if (m_type == sec.m_type)
    { switch (m_type)
      { case kModel: return m_val.m_lwLineWeight == sec.m_val.m_lwLineWeight;
        case kAbsolute:
        case kPlot:  return OdEqual(m_val.m_dLineWeight, sec.m_val.m_dLineWeight);
        case kPoint: return m_val.m_pixLineWeight == sec.m_val.m_pixLineWeight;
    } }
    return false;
  }
  bool operator !=(const D3dLwdSetting &sec) const
  { if (m_type == sec.m_type)
    { switch (m_type)
      { case kModel: return m_val.m_lwLineWeight != sec.m_val.m_lwLineWeight;
        case kAbsolute:
        case kPlot:  return !OdEqual(m_val.m_dLineWeight, sec.m_val.m_dLineWeight);
        case kPoint: return m_val.m_pixLineWeight != sec.m_val.m_pixLineWeight;
    } }
    return true;
  }
  bool hasLineweight() const
  { switch (m_type)
    { case kModel: return m_val.m_lwLineWeight != OdDb::kLnWt000;
      case kAbsolute:
      case kPlot:  return OdNonZero(m_val.m_dLineWeight);
      case kPoint: return m_val.m_pixLineWeight > 1;
    }
    return false;
  }
  OdInt32 toPixels(const OdGsViewImpl &view) const
  { switch (m_type)
    { case kModel: return view.lineweightToPixels(m_val.m_lwLineWeight);
      case kPlot:  return OdInt32(OdTruncateToLong(view.lineweightToPixels(m_val.m_dLineWeight)));
      case kPoint: return m_val.m_pixLineWeight;
      case kAbsolute: { OdGePoint2d pt; view.getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, pt, false);
                   return OdInt32(OdTruncateToLong(m_val.m_dLineWeight * pt.y)); }
    }
    return 1;
  }
};

class OdGsBaseVectorizeView;

/** \details
  This class implements D3d Metafile PlayContext objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dMetafilePlayContext
{
  IDirect3DDevice*                     m_pDev;
  mutable OdD3DStateManager*           m_pDevSM;
  OdGsBaseVectorizer*                  m_pVectorizer;
  D3dMaterialViewRedirect*             m_pMVRedirect;
  bool                                 m_bReverseNormals;
  bool                                 m_bForceFill;
  bool                                 m_bDynamicSubEntHlt;
  //mutable std::map<DWORD, CComPtr<IDirect3DTexture> > m_linePattTextures;
  mutable CComPtr<IDirect3DTexture>    m_linePattTexture;
  //mutable std::map<DWORD, CComPtr<IDirect3DTexture> > m_fillPattTextures;
  mutable CComPtr<IDirect3DTexture>    m_fillPattTexture;
  // Lines highlighting
  mutable std::map<DWORD,
           CComPtr<IDirect3DTexture> > m_bkgndPattTexture;
  mutable OdD3DVertexTexColArray       m_lineHltMutation; 
  // Line caps
  mutable OdArray<OdD3DVertexColArray> m_lineCaps;
  mutable OdD3DVertexColArray          m_capRender;
  // Transparency textures
  mutable OdArray<
           CComPtr<IDirect3DTexture> > m_transpTexture;
  mutable bool                         m_bTextureEnabled, m_bSkipHLT;
  mutable OdUInt8                      m_transparency[4];
  // Vertex arrays
  OdD3DVertexUnionPtr                  m_vPtr1, m_vPtr2;
  // Selection markers
  const OdD3DSelMark                  *m_pSelMarks;
  // Lineweight styles
  mutable D3dLwdStyle                  m_lwdStyle;
  mutable OdGePoint3dArray             m_lwdBuffer;
  // Contains pointer to ExGiDirectXGeometry::m_intMapper element; used internally for rendering process optimization.
  OdGiMaterialItemPtr                 *m_pIntMtlPtr;
  // Lineweight override
  mutable bool                         m_bLwdOverride;
  mutable D3dLwdSetting                m_overLineWeight;
  // Color override
  mutable bool                         m_bColorOverride;
  mutable DWORD                        m_nColorOverride;
  mutable OdArray<OdUInt8>             m_colorMutation;
  mutable bool                         m_bNoColorMod;
          bool                         m_bProcessBackgroundColors;
          OdColorConverterCallback    *m_pColorConverter;
  // Shaders
#ifdef OD_USE_DIRECT3D9
  mutable int                          m_bPaletteShaderState;
  mutable CComPtr<
                IDirect3DPixelShader9> m_paletteImageShader;
#endif

  mutable OdUInt8                      m_subSelectionFlags;
  bool                                 m_bContextModified;
public:
  bool isContextModified() const { return m_bContextModified; }
  void setContextModified( bool m ) { m_bContextModified = m; }
public:
  D3dMetafilePlayContext()
    : m_pDev(0)
    , m_pDevSM(0)
    , m_pVectorizer(0)
    , m_pMVRedirect(NULL)
    , m_bReverseNormals(false)
    , m_bForceFill(false)
    , m_bDynamicSubEntHlt(false)
    , m_bTextureEnabled(false)
    , m_bSkipHLT(false)
    , m_pSelMarks(NULL)
    , m_pIntMtlPtr(NULL)
    , m_bLwdOverride(false)
    , m_bColorOverride(false)
    , m_nColorOverride(0)
    , m_bNoColorMod(false)
    , m_bProcessBackgroundColors(true)
    , m_pColorConverter(NULL)
#ifdef OD_USE_DIRECT3D9
    , m_bPaletteShaderState(0)
#endif
  {
    m_vPtr1.m_pVC = NULL;
    m_vPtr2.m_pVC = NULL;
    m_transparency[0] = m_transparency[1] = m_transparency[2] = m_transparency[3] = 0;
    m_subSelectionFlags = 0;
    m_bContextModified = false;
  }

  /** \param pDevice [in]  Pointer to the device.
    \param pDevSM [in]  Pointer to the state manager.
    \param pVectorizer [in]  Pointer to the vectorizer.
  */
  D3dMetafilePlayContext(IDirect3DDevice* pDevice,
    OdD3DStateManager* pDevSM,
    OdGsBaseVectorizer* pVectorizer)
    : m_pDev(pDevice)
    , m_pDevSM(pDevSM)
    , m_pVectorizer(pVectorizer)
    , m_pMVRedirect(NULL)
    , m_bReverseNormals(false)
    , m_bForceFill(false)
    , m_bTextureEnabled(false)
    , m_bSkipHLT(false)
    , m_pSelMarks(NULL)
    , m_pIntMtlPtr(NULL)
    , m_bLwdOverride(false)
    , m_bColorOverride(false)
    , m_nColorOverride(0)
    , m_bNoColorMod(false)
    , m_bProcessBackgroundColors(true)
    , m_pColorConverter(NULL)
#ifdef OD_USE_DIRECT3D9
    , m_bPaletteShaderState(0)
#endif
  {
    m_vPtr1.m_pVC = NULL;
    m_vPtr2.m_pVC = NULL;
    m_transparency[0] = m_transparency[1] = m_transparency[2] = m_transparency[3] = 0;
    m_subSelectionFlags = 0;
    m_bContextModified = false;
  }

  /** \details
    Returns the device associated with this PlayContext object.
  */
  IDirect3DDevice* device() const { return m_pDev; }
  /** \details
    Returns the state manager associated with this PlayContext object.
  */
  OdD3DStateManager &stateManager() const { return *m_pDevSM; }

  /** \details
    Sets the device associated with this PlayContext object.
    \param pDevice [in]  Pointer to the device.
  */
  void setDevice(IDirect3DDevice* pDevice) { m_pDev = pDevice; }
  /** \details
    Sets the state manager associated with this PlayContext object.
    \param pDevSM [in]  Pointer to the state manager.
  */
  void setStateManager(OdD3DStateManager* pDevSM) { m_pDevSM = pDevSM; }
  /** \details
    Sets the vectorizer associated with this PlayContext object.
    \param pVectorizer [in]  Pointer to the vectorizer.
    \param bUpdateVectorizationContext [in]  If true, calls update of vectorization context.
  */
  void setVectorizer(OdGsBaseVectorizer* pVectorizer, bool bUpdateVectorizationContext = true)
  { m_pVectorizer = pVectorizer; if (bUpdateVectorizationContext) updateVectorizerContext(); }
  /** \details
    Returns the vectorizer associated with this PlayContext object.
  */
  OdGsBaseVectorizer* vectorizer() const { return m_pVectorizer; }
  /** \details
    Returns the view associated with this PlayContext object.
  */
  OdGsViewImpl& view() const { return m_pVectorizer->view(); }
  /** \details
    Sets vectorizer interface for material/mapper functionality.
    \param pVPRedirect [in]  Pointer to D3dMaterialViewRedirect interface.
  */
  void setMaterialViewRedirect(D3dMaterialViewRedirect *pMVRedirect);
  /** \details
    Returns vectorizer interface for material/mapper functionality.
  */
  D3dMaterialViewRedirect *materialViewRedirect() const { return m_pMVRedirect; }

  /** \details
    Returns the render mode associated with this PlayContext object.
  */
  OdGsView::RenderMode renderMode() const;

  /** \details
    Returns the eye to world transformation matrix of this PlayContext object.
  */
  OdGeMatrix3d eyeToWorld() const;

  /** \details
    Returns the reverse normals flag of this PlayContext object.
  */
  bool reverseNormals() const { return m_bReverseNormals; }
  /** \details
    Sets the reverse normals flag of this PlayContext object.
    \param reverseNormals [in]  Contols flag.
  */
  void setReverseNormals(bool reverseNormals) { m_bReverseNormals = reverseNormals; }

  /** \details
    Returns the force fill flag of this PlayContext object.
  */
  bool forceFill() const { return m_bForceFill; }
  /** \details
    Sets the force fill flag of this PlayContext object.
    \param forceFill [in]  Contols flag.
  */
  void setForceFill(bool forceFill) { m_bForceFill = forceFill; }

  /** \details
    Clears the pattern texture cache of this PlayContext object.
  */
  void clearPattTextureCache() const;
  /** \details
    Returns the line pattern texture associated with the specified color for this PlayContext Object.
    \param textureStage [in]  Texture channel to set blending parameters.
    \param bSetChan [in]  Set blending parameters to channel.
  */
  IDirect3DTexture* linePattTexture(DWORD textureStage = 0, bool bSetChan = true) const;
  /** \details
    Returns the fill pattern texture associated with the specified color for this PlayContext Object.
    \param textureStage [in]  Texture channel to set blending parameters.
    \param bSetChan [in]  Set blending parameters to channel.
  */
  IDirect3DTexture* fillPattTexture(DWORD textureStage = 0, bool bSetChan = true) const;
  /** \details
    Returns the background fill pattern texture assicuated with the specified color of background.
    \param textureStage [in]  Texture channel to set blending parameters.
    \param bSetChan [in]  Set blending parameters to channel.
  */
  IDirect3DTexture* lineBkgndTexture(DWORD textureStage = 0, bool bSetChan = true) const;
  /** \details
    Stop's using of highlighting pattern.
    \param textureStage [in]  Texture channel to set blending parameters.
    \param bSetChan [in]  Set blending parameters to channel.
  */
  void closePattTextureUsage(DWORD textureStage = 0, bool bSetChan = true) const;

  /** \details
    Render wide line with rounded end-caps.
    \param iPx [in]  Line width in pixels.
    \param origin [in]  Start line point.
    \param fwd [in]  Line direction vector multiplied by line length.
    \param side [in]  Line perpendicular vector, defines line plane with direction vector.
  */
  void renderLineCap(int iPx, const OdD3DVertexCol *origin, const OdGeVector3d &fwd, const OdGeVector3d &side) const;
  /** \details
    Clears array with pregenerated line end-caps.
  */
  void clearLineCaps() const;

  /** \details
    Gets pointer to texture of specified transparency level.
    \param alpha [in]  Alpha level.
  */
  IDirect3DTexture *getTransparencyLevelTexture(OdUInt8 alpha) const;
  /** \details
    Clears array of transparency textures.
  */
  void clearTransparencyLevels() const;

  /** \details
    Render simple 1px line with background color.
    \param points [in]  Line vertices array.
    \param nPoints [in]  Count of points into points array.
    \param bLines [in]  If true, points in points array defines not linked lines list (2 points for each line), elsewhere
               points array defines linked line vertices list (end vertex of previous line is equal to start vertex of the next line).
  */
  void renderBkgndLine(const OdD3DVertexCol *points, OdUInt32 nPoints, bool bLines = false) const;

  /** \details
    Render highlighted lines using mutation array.
    \param points [in]  Line vertices array.
    \param nPoints [in]  Count of points into points array.
    \param bBackFill [in]  Enables back filling.
    \param bLines [in]  If true, points in points array defines not linked lines list (2 points for each line), elsewhere
               points array defines linked line vertices list (end vertex of previous line is equal to start vertex of the next line).
  */
  void renderHighlightedLine(const OdD3DVertexCol *points, OdUInt32 nPoints, bool bBackFill = false, bool bLines = false) const;

  /** \details
    Render highlighted triangles.
    \param PrimitiveType [in]  Type of primitive.
    \param PtimitiveCount [in]  Count of primitives.
    \param pVertexStreamZeroData [in]  Primitive array pointer.
    \param VertexStreamZeroStride [in]  Size of primitive array entry.
    \param textureStage [in]  Texture channel to set blending parameters.
  */
  void renderHighlightedTriangle(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride, DWORD textureStage = 0) const;

  /** \details
    Update material texture set.
    \param bSet [in]  Flag.
  */
  void updateMaterialTexture(bool bSet, bool bSkipHLT = false) const;

  enum TransparencySet
  {
    Transparency_Material = 0,
    Transparency_Color,
    Transparency_PSel,
    Transparency_LSel,

    Transparency_Force,
    Transparency_Reset
  };

  /** \details
    Update transparency state.
    \param nSet [in]  Transparency entry.
    \param nVal [in]  New state.
  */
  void updateTransparencyState(TransparencySet nSet, OdUInt8 nVal) const;

  /** \details
    Reset current texture safely.
  */
  void resetTextureState() const;

  /** \details
    Set pointer to vertex array (1-st slot).
  */
  inline void setVPtr1(const OdD3DVertexUnionPtr &vtPtr)
  {
    m_vPtr1 = vtPtr;
  }
  /** \details
    Set pointer to vertex array (2-nd slot).
  */
  inline void setVPtr2(const OdD3DVertexUnionPtr &vtPtr)
  {
    m_vPtr2 = vtPtr;
  }
  /** \details
    Return pointer to vertex array (1-st slot).
  */
  inline const OdD3DVertexUnionPtr &vtPtr1() const
  {
    return m_vPtr1;
  }
  /** \details
    Return pointer to vertex array (2-nd slot).
  */
  inline const OdD3DVertexUnionPtr &vtPtr2() const
  {
    return m_vPtr2;
  }
  /** \details
    Set selection markers array.
  */
  inline void setSelMarks(const OdD3DSelMark *pSM)
  {
    m_pSelMarks = pSM;
  }
  /** \details
    Return selection markers array.
  */
  inline const OdD3DSelMark *selMarks() const
  {
    return m_pSelMarks;
  }
  /** \details
    Change current selection marker.
  */
  void setSelectionMarker(OdGsMarker gsMarker) const;
  /** \details
    Checks is selection markers check is required.
  */
  bool isSubselectionRequired() const;

  /** \details
    Returns current lineweight style state.
  */
  inline D3dLwdStyle &lwdStyle() const
  {
    return m_lwdStyle;
  }
  /** \details
    Temporary lineweights generator buffer.
  */
  inline OdGePoint3dArray &lwdBuf() const
  {
    return m_lwdBuffer;
  }

  /** \details
    Link with geometry for some optimization purposes.
  */
  inline void setIntMtlPtr(OdGiMaterialItemPtr* pIntMtlPtr)
  {
    m_pIntMtlPtr = pIntMtlPtr;
  }
  /** \details
    Link with geometry for some optimization purposes.
  */
  inline OdGiMaterialItemPtr* intMtlPtr() const
  {
    return m_pIntMtlPtr;
  }

  /** \details
    Returns dynamic subentities highlighting mode.
  */
  inline bool dynamicSubEntHlt() const { return m_bDynamicSubEntHlt; }

  /** \details
    Sets lineweight override mode.
  */
  inline void setLineweightOverride(bool bSet) const { m_bLwdOverride = bSet; }
  /** \details
    Sets lineweight override mode settings.
  */
  inline void setLineweightOverride(const D3dLwdSetting &set) const { m_bLwdOverride = true; m_overLineWeight = set; }
  /** \details
    Returns lineweight override mode.
  */
  inline bool isLineweightOverride() const { return m_bLwdOverride; }
  /** \details
    Applies lineweight overrides.
  */
  inline bool applyLineweightOverride(D3dLwdSetting &lwd) const
  {
    if (m_overLineWeight != lwd)
    {
      lwd = m_overLineWeight;
      return true;
    }
    return false;
  }

  /** \details
    Sets color override mode.
  */
  inline void setColorOverride(bool bSet) const { m_bColorOverride = bSet; }
  /** \details
    Sets color override mode settings.
  */
  inline void setColorOverride(DWORD nColor) const { m_nColorOverride = nColor; m_bColorOverride = true; }
  /** \details
    Returns color override mode.
  */
  inline bool isColorOverride() const { return m_bColorOverride; }
  /** \details
    Returns current color override.
  */
  inline DWORD colorOverride() const { return m_nColorOverride; }
  /** \details
    Returns true if and only if requested color must be overriden.
  */
  inline bool isNeedOverride(DWORD color) const { return (m_bColorOverride && (m_nColorOverride != color)) ||
                                                         (m_pColorConverter && (m_bProcessBackgroundColors || !m_bNoColorMod)); }
  /** \details
    Applies color override for single color.
  */
  inline DWORD makeColorOverride(DWORD color, bool bIfNeed = true) const;
  /** \details
    Applies color override for data array.
  */
  const void* makeColorOverride(const void *pInData, OdUInt32 nOffsetToClr, OdUInt32 nEntrySize, OdUInt32 nEntries) const;

  /** \details
    Sets "no color modification" mode.
  */
  inline void setNoColorModificationMode(bool bSet) const { m_bNoColorMod = bSet; }

#ifdef OD_USE_DIRECT3D9
  /** \details
    Requests for palette shader.
  */
  IDirect3DPixelShader9 *paletteImageShader() const;
#endif

  void setSubselectionFlags( OdUInt8 flags ) const { m_subSelectionFlags = flags; }
  OdUInt8 getSubselectionFlags() const { return m_subSelectionFlags; }
  bool isDisplayHighlighted() const;
  bool isDisplayUnhighlighted() const;
  bool isSelectHighlighted() const;
  bool isSelectUnhighlighted() const;
  bool isDisplayWithHighlight() const;
  bool isSelectWithHighlight() const;

protected:
  void updateVectorizerContext();
};


//A structure for play4Select parameters
struct D3dPlay4SelectContext
{
  OdGiConveyorGeometry*         pGeometry;
  OdGiConveyorContext*          pGeometryContext;
  bool                          bCheckMarkFlag;
  OdGePoint3dArray              ptsArray;
};


/** \details
  This class is the base class for D3dRecord objects.
  Library: Source code provided.
  \remarks
  Record objects are linked to form a Record List.
  <group ExRender_Windows_Classes> 
*/
class D3dRecord
{
protected:
  D3dRecord* m_pTail;

  ODCA_HEAP_ALLOCATOR();
public:
  ODCA_HEAP_OPERATORS();

  D3dRecord() : m_pTail(0) {}
  virtual ~D3dRecord() {}

  /** \details
    Destroys this Record object and all Record objects linked to it.
  */
  void destroy();
  /** \details
    Adds the specified Record to the tail of this Record object.
    \param pTail [in]  Pointer to the new tail.
  */
  void setTail(D3dRecord* pTail) { m_pTail = pTail; }

  /** \details
    Returns or references the tail of this Record object.
  */
  const D3dRecord* tail() const { return m_pTail; }
 
  /** \details
    Returns or references the tail of this Record object.
  */
  D3dRecord* tail() { return m_pTail; }
  /** \details
    Renders this Record object.
    \param pContext [in]  Pointer to the Play context.
  */
  virtual void play(const D3dMetafilePlayContext& pContext) const = 0;

  virtual void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const = 0;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements D3d Head Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dHeadRecord : public D3dRecord
{
public:
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;
  /** \details
    Deletes the Record objects linked to this HeaderRecord object.
  */
  void deleteList();
  ~D3dHeadRecord();

  virtual OdUInt64 recordSize() const;
};

/** \details
  This class is implements D3d Point Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPointRecord : public D3dRecord
{
  OdD3DVertexCol m_pos;
public:
  /** \param position [in]  Position of the point.
    \param color [in]  Color of the point.
  */
  D3dPointRecord(const OdGePoint3d& position, DWORD color);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PointRecord object. 
    \param position [in]  Position of the point.
    \param color [in]  Color of the point.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPointRecord* createObject(const OdGePoint3d& position, DWORD color);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements packed D3d Point Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedPointRecord : public D3dRecord
{
protected:
  OdInt32 m_nInitialID;
  OdInt32 m_nbPoints;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numPoints [in]  Number of points.
  */
  D3dPackedPointRecord(OdInt32 nInitialID, OdInt32 numPoints);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedPointRecord object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numPoints [in]  Number of points.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedPointRecord* createObject(OdInt32 nInitialID, OdInt32 numPoints);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  Extends D3dPackedPointRecord by selection markers array processing.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedPointRecordWSel : public D3dPackedPointRecord
{
  OdInt32 m_nInitialSelMark;
  OdInt32 m_nbSelMarks;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numPoints [in]  Number of points.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
  */
  D3dPackedPointRecordWSel(OdInt32 nInitialID, OdInt32 numPoints, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedPointRecordWSel object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numPoints [in]  Number of points.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedPointRecordWSel* createObject(OdInt32 nInitialID, OdInt32 numPoints, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements D3d Polyline Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPolylineRecord : public D3dRecord
{
  OdInt32        m_nPoints;
  OdD3DVertexCol m_points[1];
public:
  /** \param numPoints [in]  Number of points.
    \param points [in]  Array of points.
    \param color [in]  Color of the polyline.
  */
  D3dPolylineRecord(OdInt32 numPoints, const OdGePoint3d* points, DWORD color);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;
  /** \details
    Creates the specified PolylineRecord object.
    \param numPoints [in]  Number of points.
    \param points [in]  Array of points.
    \param color [in]  Color of the polyline.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPolylineRecord* createObject(OdInt32 numPoints, const OdGePoint3d* points, DWORD color);

  virtual OdUInt64 recordSize() const;
};

/** \details
  This class is implements packed D3d Polyline Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedPolylineRecord : public D3dRecord
{
protected:
  OdInt32 m_nInitialID;
  OdInt32 m_nbLines;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
  */
  D3dPackedPolylineRecord(OdInt32 nInitialID, OdInt32 numLines);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedPolylineRecord object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedPolylineRecord* createObject(OdInt32 nInitialID, OdInt32 numLines);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  Extends D3dPackedPolylineRecord by selection markers array processing.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedPolylineRecordWSel : public D3dPackedPolylineRecord
{
  OdInt32 m_nInitialSelMark;
  OdInt32 m_nbSelMarks;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
  */
  D3dPackedPolylineRecordWSel(OdInt32 nInitialID, OdInt32 numLines, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedPolylineRecordWSel object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedPolylineRecordWSel* createObject(OdInt32 nInitialID, OdInt32 numLines, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements packed D3d Polyline Strip Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedPolylineStripRecord : public D3dRecord
{
  OdInt32 m_nInitialID;
  OdInt32 m_nbVertices;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numVertices [in]  Number of vertices.
  */
  D3dPackedPolylineStripRecord(OdInt32 nInitialID, OdInt32 numVertices);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedPolylineStripRecord object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numVertices [in]  Number of vertices.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedPolylineStripRecord* createObject(OdInt32 nInitialID, OdInt32 numLines);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

#ifndef OD_USE_DIRECT3D9
/** \details
  This class is implements D3d Polyline with Linetype Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dLtpPolylineRecord : public D3dRecord
{
  OdInt32 m_nbPoints;
  WORD m_wLinePattern, m_wRepeatFactor;
  OdD3DVertexCol m_pPts[1];
public:
  /** \param numPoints [in]  Number of points.
    \param points [in]  Array of points.
    \param color [in]  Color of the polyline.
    \param linePattern [in]  Line pattern of the polyline.
    \param repeatFactor [in]  Repeat factor of the linetype.
  */
  D3dLtpPolylineRecord(OdInt32 numPoints, const OdGePoint3d* points, DWORD color, WORD linePattern, WORD repeatFactor);
  void play(const D3dMetafilePlayContext& pContext) const;
  /** \details
    Creates the specified LtpPolylineRecord object.
    \param numPoints [in]  Number of points.
    \param points [in]  Array of points.
    \param color [in]  Color of the polyline.
    \param linePattern [in]  Line pattern of the polyline.
    \param repeatFactor [in]  Repeat factor of the polyline.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dLtpPolylineRecord* createObject(OdInt32 numPoints, const OdGePoint3d* points, 
    DWORD color, WORD linePattern, WORD repeatFactor);
  /** \details
    Returns the number of points in this LtpPolylineRecord object.
  */
  OdInt32 pointsNum() const { return m_nbPoints; }
  /** \details
    Returns a pointer to the point array of this LtpPolylineRecord object.
  */
  const OdD3DVertexCol* pPoints() const { return m_pPts; }
  /** \details
    Returns the line pattern of this LtpPolylineRecord object.
  */
  WORD linePattern() const { return m_wLinePattern; }
  /** \details
    Returns the repeat factor of this LtpPolylineRecord object.
  */
  WORD repeatFactor() const { return m_wRepeatFactor; }
};
#endif

/** \details
  This class is implements D3d Polyline with Linewidth Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class D3dLwdPolylineRecord : public D3dRecord
{
  OdInt32 m_nbPoints, m_nbRPoints;
  // WORD m_wLinePattern, m_wRepeatFactor; // May be used in future for patterned lines
  mutable OdGePoint3d m_vcn[2];
  double m_lwd;
  OdD3DVertexCol m_pPts[1];
public:
  /** \param numPoints [in]  Number of points.
    \param points [in]  Array of points.
    \param color [in]  Color of the polyline.
    \param lwd [in]  Width of the polyline.
    \param nprec [in]  Precision of generated line caps.
  */
  D3dLwdPolylineRecord(OdInt32 numPoints, const OdGePoint3d* points, DWORD color, double lwd);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;
  /** \details
    Creates the specified LwdPolylineRecord object.
    \param numPoints [in]  Number of points.
    \param points [in]  Array of points.
    \param color [in]  Color of the polyline.
    \param lwd [in]  Width of the polyline.
    \param nprec [in]  Precision of generated line caps.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dLwdPolylineRecord* createObject(OdInt32 numPoints, const OdGePoint3d* points, DWORD color, double lwd);

  virtual OdUInt64 recordSize() const;
protected:
  static OdInt32 calculateVerticesCount(OdInt32 numPoints, int nprec);
  static int calculateCapsSize(double lwd);
  void regeneratePrimitive(OdInt32 numPoints, DWORD color, const OdGeVector3d &vcn, double lwd, int nprec);
  void generateMesh(OdD3DVertexCol *verts, const OdD3DVertexCol *p1, const OdD3DVertexCol *p2, DWORD color, const OdGeVector3d &vcn, double lwd, int nprec);
  bool isNeedRebuild(const D3dMetafilePlayContext& pContext) const;
};

/** \details
  This class is implements packed D3d Polyline with Lineweight Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedLwdPolylineRecord : public D3dRecord
{
protected:
  bool m_bPoints;
  OdInt32 m_nInitialID;
  OdInt32 m_nbLines;
  D3dLwdSetting m_lineWeight;
  mutable OdGeVector3d m_viewDirection; // Differ instructs to recalc line cache
  mutable OdInt32 m_nPxSize;
  mutable OdUInt32 m_nView;
  mutable double m_dSize; // Size in depth / 2
  mutable double m_dSize2; // Size in depth
  mutable double m_lwScaleCoef; // Used to detect MS or PS lineweights vectorization style
  mutable double m_lwCoef; // MS coefficient
  mutable OdUInt32 m_flags;
  struct LwdCache
  {
    //double len; // Size in depth
    OdGeVector3d dir; // Direction * len
    OdGeVector3d side; // Side * len
  };
  mutable LwdCache m_pCache[1]; // Cache of line data
public:
  /** \param bPoints [in]  Render as points or as lines.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \param lineWeight [in]  Lienweight settings.
  */
  D3dPackedLwdPolylineRecord(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedLwdPolylineRecord object.
    \param bPoints [in]  Render as points or as lines.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \param lineWeight [in]  Lineweight settings.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedLwdPolylineRecord* createObject(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
protected:
  OdInt32 computePxSize(const D3dMetafilePlayContext& pContext) const;
  bool isNeedCacheRebuilding(const D3dMetafilePlayContext& ctx) const;
  void regenCache(const D3dMetafilePlayContext& ctx) const;
  void regenLine(LwdCache *outCache, const OdD3DVertexCol *inVerts, const OdGeVector3d &vcn) const;
};

/** \details
  Extends D3dPackedLwdPolylineRecord by selection markers array processing.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedLwdPolylineRecordWSel : public D3dPackedLwdPolylineRecord
{
  OdInt32 m_nInitialSelMark;
  OdInt32 m_nbSelMarks;
public:
  /** \param bPoints [in]  Render as points or as lines.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \param lineWeight [in]  Lineweight settings.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
  */
  D3dPackedLwdPolylineRecordWSel(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified D3dPackedLwdPolylineRecordWSel object.
    \param bPoints [in]  Render as points or as lines.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numLines [in]  Number of line segments.
    \param lineWeight [in]  Lineweight settings.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedLwdPolylineRecordWSel* createObject(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements D3d Triangle Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dTriangleRecord : public D3dRecord
{
  OdD3DVertexCol m_points[3];
public:
  /** \param points [in]  Array of three points.
  */
  D3dTriangleRecord(const OdD3DVertexCol* points);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements D3d Triangle with Normals Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dTriangleNormalRecord : public D3dRecord
{
  mutable OdD3DVertexNormalCol points[3]; // in case bReverseNormals we have to modify this field
public:
  /** \param points [in]  Array of three points.
  */
  D3dTriangleNormalRecord(OdD3DVertexNormalCol* points);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements D3d Triangle Texture with Normals Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dTriangleNormalTextureRecord : public D3dRecord
{
  mutable OdD3DVertexNormalTexCol points[3]; // in case bReverseNormals we have to modify this field
public:
  /** \param points [in]  Array of three points.
  */
  D3dTriangleNormalTextureRecord(OdD3DVertexNormalTexCol* points);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements packed D3d Triangle Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedTriangleRecord : public D3dRecord
{
protected:
  OdInt32 m_nInitialID;
  OdInt32 m_nbTriangles;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
  */
  D3dPackedTriangleRecord(OdInt32 nInitialID, OdInt32 numTriangles);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PackedTriangleRecord object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedTriangleRecord* createObject(OdInt32 nInitialID, OdInt32 numTriangles);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  Extends D3dPackedTriangleRecord by selection markers array processing.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedTriangleRecordWSel : public D3dPackedTriangleRecord
{
  OdInt32 m_nInitialSelMark;
  OdInt32 m_nbSelMarks;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
  */
  D3dPackedTriangleRecordWSel(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PackedTriangleRecordWSel object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedTriangleRecordWSel* createObject(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements packed D3d Triangle with Normals Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedTriangleNormalRecord : public D3dRecord
{
protected:
  OdInt32 m_nInitialID;
  OdInt32 m_nbTriangles;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
  */
  D3dPackedTriangleNormalRecord(OdInt32 nInitialID, OdInt32 numTriangles);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PackedTriangleNormalRecord object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedTriangleNormalRecord* createObject(OdInt32 nInitialID, OdInt32 numTriangles);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  Extends D3dPackedTriangleNormalRecord by selection markers array processing.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedTriangleNormalRecordWSel : public D3dPackedTriangleNormalRecord
{
  OdInt32 m_nInitialSelMark;
  OdInt32 m_nbSelMarks;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
  */
  D3dPackedTriangleNormalRecordWSel(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PackedTriangleNormalRecordWSel object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedTriangleNormalRecordWSel* createObject(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements packed D3d Triangle Texture with Normals Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedTriangleNormalTextureRecord : public D3dRecord
{
protected:
  OdInt32 m_nInitialID;
  OdInt32 m_nbTriangles;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
  */
  D3dPackedTriangleNormalTextureRecord(OdInt32 nInitialID, OdInt32 numTriangles);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PackedTriangleNormalTextureRecord object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedTriangleNormalTextureRecord* createObject(OdInt32 nInitialID, OdInt32 numTriangles);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  Extends D3dPackedTriangleNormalTextureRecord by selection markers array processing.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dPackedTriangleNormalTextureRecordWSel : public D3dPackedTriangleNormalTextureRecord
{
  OdInt32 m_nInitialSelMark;
  OdInt32 m_nbSelMarks;
public:
  /** \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
  */
  D3dPackedTriangleNormalTextureRecordWSel(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  /** \details
    Creates the specified PackedTriangleNormalTextureRecordWSel object.
    \param nInitialID [in]  Index of first vertex in vertex buffer.
    \param numTriangles [in]  Number of triangles.
    \param nInitialSelMark [in]  Index of first selection marker in selection markers array.
    \param nbSelMarks [in]  Count of selection markers in selection markers array.
    \remarks
    Returns a pointer to the newly created object.
  */
  static D3dPackedTriangleNormalTextureRecordWSel* createObject(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class is implements D3d Initialized Texture Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dInitTextureRecord : public D3dRecord, protected D3dLostStateManager::D3dLostStateEntry, protected D3dResourceSharingProvider::D3dResourceShareRef
{
  D3dMaterialViewRedirect::RasterImageRef m_texture;
  D3dResourceSharingProvider::D3dResourceShareRef m_palRef;
  OdGeMatrix3d m_w2TxXform;
  enum ITFlags
  {
    kTransp  = 1, // True for transparent
    kBitonal = 2, // True for bitonal
    kDraft   = 4, // True for draft quality
    kAlpha1  = 8, // True if transparency mode 1bit
    kAlpha8  = 16 // True if transparency mode 8bit
  };
  OdUInt8 m_uFlags;
  D3dMaterialViewRedirect *m_pUninitIface;
  D3dLostStateManager *m_pLSManager;
public:
  /** \param pTexture [in]  Pointer to the texture.
    \param w2TxXform [in]  World to texture transformation matrix.
    \param transparency [in]  True if and only if texture is transparent.
    \param bitonal [in]  True if and only if texture is bitonal.
    \param draft [in]  True if and only if texture has draft quality.
    \param bAlpha1Bit [in]  True if and only if 32bit image have 1bit transparency mode.
    \param bAlpha8Bit [in]  True if and only if 32bit image have 8bit transparency mode.
    \param pUninitIface [in]  Raster image cache uninitialization interface.
    \param pLSManager [in]  Resources recovering interface.
  */
  D3dInitTextureRecord(D3dMaterialViewRedirect::RasterImageRef &pTexture, 
    const OdGeMatrix3d& w2TxXform, bool transparency, bool bitonal, bool draft,
    bool bAlpha1Bit, bool bAlpha8Bit,
    D3dMaterialViewRedirect *pUninitIface, D3dLostStateManager *pLSManager);
  ~D3dInitTextureRecord();

  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const;

  protected:
    void recreate(D3dLostStateManager *pManager);
    void onManagerDestroy(D3dLostStateManager *pManager);
};

/** \details
  This class implements D3d Uninitialized Texture Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dUninitTextureRecord : public D3dRecord
{
public:
  D3dUninitTextureRecord();
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements base class for empty D3d Record which represent Begin or End for some functionality.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dBaseBeginOrEndRecord : public D3dRecord
{
public:
  D3dBaseBeginOrEndRecord();
  /** \details
    Returns true if and only if this record is begin highlighting record.
  */
  virtual bool isBegin() const = 0;
  /** \details
    Returns true if and only if this record is end highlighting record.
  */
  virtual bool isEnd() const { return !isBegin(); }

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements base class for Begin and End Highlighting Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dBaseHighlightingRecord : public D3dBaseBeginOrEndRecord
{
public:
  D3dBaseHighlightingRecord();
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements D3d Begin Highlighting Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dBeginHighlightingRecord : public D3dBaseHighlightingRecord
{
public:
  D3dBeginHighlightingRecord();
  /** \details
    Returns true if and only if this record is begin highlighting record.
  */
  bool isBegin() const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements D3d End Highlighting Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dEndHighlightingRecord : public D3dBaseHighlightingRecord
{
public:
  D3dEndHighlightingRecord();
  /** \details
    Returns true if and only if this record is begin highlighting record.
  */
  bool isBegin() const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements Selection Marker Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dSelectionMarkerRecord : public D3dRecord
{
  OdGsMarker m_gsMarker; // Subentity selection marker
public:
  D3dSelectionMarkerRecord(OdGsMarker gsMarker);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements Transparency Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class D3dTransparencyRecord : public D3dRecord
{
  OdUInt8 m_transparency; // inverted alpha
public:
  D3dTransparencyRecord(OdUInt8 invAlpha);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements Lineweight Style Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class D3dLwdStyleRecord : public D3dRecord
{
  OdUInt8 m_lwdStyle[3];
public:
  D3dLwdStyleRecord(); // for def
  D3dLwdStyleRecord(OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle); // for non-def
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements base class for Begin and End No Color Modification Record objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dBaseNoColorModRecord : public D3dBaseBeginOrEndRecord
{
public:
  D3dBaseNoColorModRecord();
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements D3d Begin Highlighting Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dBeginNoColorModRecord : public D3dBaseNoColorModRecord
{
public:
  D3dBeginNoColorModRecord();
  /** \details
    Returns true if and only if this record is begin highlighting record.
  */
  bool isBegin() const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements D3d End Highlighting Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dEndNoColorModRecord : public D3dBaseNoColorModRecord
{
public:
  D3dEndNoColorModRecord();
  /** \details
    Returns true if and only if this record is begin highlighting record.
  */
  bool isBegin() const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  This class implements TTF Text Record object.
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class D3dTtfTextRecord : public D3dRecord
{
  OdTtfFontsCache::TextInfo m_textInfo;
  D3dLwdSetting m_lineWeight;
  DWORD m_nColor;
public:
  D3dTtfTextRecord(const OdTtfFontsCache::TextInfo &textInfo, const D3dLwdSetting &lineWeight, DWORD m_nColor);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

// Material record for enabled/disabled GsModel

/** \details
  This class implements D3d Material Record objects.
  <group ExRender_Windows_Classes>
*/
class D3dMaterialRecord : public D3dRecord
{
  OdGiMaterialItemPtr m_pMatItem; // Material data pointer
public:
  D3dMaterialRecord(OdGiMaterialItemPtr pMatItem);
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  static D3dMaterialRecord* createObject(OdGiMaterialItemPtr pMatItem);

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

enum OdDirectXSelectionFlags
{
  OdDirectXSelFlag_DontDisplayUnhighlighted = (1 << 0),
  OdDirectXSelFlag_DontSelectUnhighlighted  = (1 << 1),
  OdDirectXSelFlag_DontDisplayHighlighted   = (1 << 2),
  OdDirectXSelFlag_DontSelectHighlighted    = (1 << 3),
  OdDirectXSelFlag_FlagsMask                = 15
};
/** \details
  This class implements D3d Selection Mode Record objects.
  <group ExRender_Windows_Classes>
*/
class D3dSelectionModeRecord : public D3dRecord
{
  OdUInt8 m_flags;
public:
  D3dSelectionModeRecord( OdUInt8 flags );
  void play(const D3dMetafilePlayContext& pContext) const;
  void play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;

  static D3dSelectionModeRecord* createObject( OdUInt8 flags );

  virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
};

/** \details
  Sometimes object mapping may be delayed up to object will be completely cached. This structure
  defines coordinates set for one mapper texture coordinates recalculation.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
struct ExGsDelayedMappingEntry;

/** \details
  This class is implements DirectX Metafile objects.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGiDirectXMetafile : public OdRxObject
{
  // line segments
  D3dHeadRecord                             m_lineSegsCache;
  D3dRecord*                                m_pLineSegsTail;

  // no light triangles
  D3dHeadRecord                             m_noLightCache;
  D3dRecord*                                m_pNoLightTail;

  // light triangles
  D3dHeadRecord                             m_lightCache;
  D3dRecord*                                m_pLightTail;

  OdD3DVertexColArray                       m_LineSegArray;     // Array of line segment vertices.
  OdD3DVertexColArray                       m_NoLightTriArray;  // Array of no-light vertices.
  OdD3DVertexNormalColArray                 m_LightTriArray;    // Vertices with normals.
  OdD3DVertexNormalTexColArray              m_TexturedTriArray; // Vertices with normals and texture coordinates.
  OdD3DSelMarkArray                         m_SelMarks;         // Selection markers array.

  ODRX_DECLARE_MEMBERS(ExGiDirectXMetafile);
public:
  ExGiDirectXMetafile();
  virtual ~ExGiDirectXMetafile();

  /** \details
    Begins vectorization of this Metafile object.
  */
  void begin();

  /** \details
    Clear Metafile object.
  */
  void clear();
  
  /** \details
    Appends the specified record chain to the line segments cache of this Metafile object. 
    \param pRecord [in]  Pointer to the record chain.
  */
  void appendLineSegsRecord(D3dRecord* pRecord)
  {
    m_pLineSegsTail->setTail(pRecord);
    m_pLineSegsTail = pRecord;
  }
  /** \details
    Appends the specified record chain to the no-light cache of this Metafile object. 
    \param pRecord [in]  Pointer to the record chain.
  */
  void appendNoLightRecord(D3dRecord* pRecord)
  {
    m_pNoLightTail->setTail(pRecord);
    m_pNoLightTail = pRecord;
  }
  /** \details
    Appends the specified record chain to the light triangles cache of this Metafile object. 
    \param pRecord [in]  Pointer to the record chain.
  */
  void appendLightRecord(D3dRecord* pRecord)
  {
    m_pLightTail->setTail(pRecord);
    m_pLightTail = pRecord;
  }

  void setLineSegArray(const OdD3DVertexColArray &vertices)
  {
    m_LineSegArray = vertices;
    m_LineSegArray.setPhysicalLength(vertices.size());
  }
  OdD3DVertexColArray &getLineSegArray() { return m_LineSegArray; }
  const OdD3DVertexColArray &getLineSegArray() const { return m_LineSegArray; }
  void setNoLightTriArray(const OdD3DVertexColArray &vertices)
  {
    m_NoLightTriArray = vertices;
    m_NoLightTriArray.setPhysicalLength(vertices.size());
  }
  OdD3DVertexColArray &getNoLightTriArray() { return m_NoLightTriArray; }
  const OdD3DVertexColArray &getNoLightTriArray() const { return m_NoLightTriArray; }
  void setLightTriArray(const OdD3DVertexNormalColArray &vertices)
  {
    m_LightTriArray = vertices;
    m_LightTriArray.setPhysicalLength(vertices.size());
  }
  OdD3DVertexNormalColArray &getLightTriArray() { return m_LightTriArray; }
  const OdD3DVertexNormalColArray &getLightTriArray() const { return m_LightTriArray; }
  void setTexturedTriArray(const OdD3DVertexNormalTexColArray &vertices)
  {
    m_TexturedTriArray = vertices;
    m_TexturedTriArray.setPhysicalLength(vertices.size());
  }
  OdD3DVertexNormalTexColArray &getTexturedTriArray() { return m_TexturedTriArray; }
  const OdD3DVertexNormalTexColArray &getTexturedTriArray() const { return m_TexturedTriArray; }

  void setSelMarksArray(const OdD3DSelMarkArray &selMarks)
  {
    m_SelMarks = selMarks;
    m_SelMarks.setPhysicalLength(selMarks.size());
  }
  OdD3DSelMarkArray &getSelMarksArray() { return m_SelMarks; }
  const OdD3DSelMarkArray &getSelMarksArray() const { return m_SelMarks; }

  /** \details
    Renders this Metafile object.
    \param pContext [in]  Pointer to the Play context.
  */
  void play(D3dMetafilePlayContext& pContext) const;

  virtual OdUInt64 metafileSize() const;

  /** \details
    Play this Metafile into geometry conveyor
    \param pContext [in]  Pointer to the Play context.
  */
  void play4Select( D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const;
  
  /** \details
    Ends vectorization of this Metafile object.
    \param pGeometry [in]  Receives a pointer to the vectorized geometry.
    \note
    The default implementaion of this function does nothing but return.
  */
  void end(ExGiDirectXGeometry* pGeometry);
};

typedef OdSmartPtr<ExGiDirectXMetafile> ExGiDirectXMetafilePtr;

/** \details
   Metafile packager. Initializes vertex arrays for all metafile types and manages metafile
   records creation.
   Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class D3dMetafilePackager
{
  public:
    /** \details
      Types of metafile arrays.
    */
    enum ArrayType
    {
      kUnassignedArray = -1, // Array type is not assigned
      kLSArray = 0, // Line segments (1 step)
      kVCArray, // No-light triangles (2 step)
      kVNCArray, // Light triangles (3 step)
      kVNTCArray // Light triangles with textures (3 step)
    };
    /** \details
      Special record types (not produceable by metafile packager).
      \remarks
      Each of them stops current packaging process (calls flush() internally).
    */
    enum RecordType
    {
      kPointRec = 0, // Single point record
      kPolylineRec, // Independent stripped polyline record
      kLtpPolylineRec, // Polyline with template record
      kLwdPolylineRec, // Polyline with width record
      kTriangleRec, // Single triangle record
      kTriangleNormalRec, // Single triangle with normal record
      kTriangleNormalTexRec, // Single triangle with normal and texture coordinates record
      kInitTextureRec, // Initialize texture record (called before raster image triangles)
      kUninitTextureRec, // Uninitialize texture record (called after raster image triangles)
      kMaterialRec, // Material record
      kBeginHighlightingRec, // Begin highlighting record (called for selection mark of subentities)
      kEndHighlightingRec, // End highlighting record (called for end subentity selection mark)
      kSelectionMarkerRec, // Selection marker record
      kTransparencyRec, // Transparency level
      kLwdStyleRec, // Lineweight style
      kTtfTextRec, // TTF text
      kBeginNoColorModRec, // Begin "no color modification" mode.
      kSelectionModeRec,  //Selection mode record
      kEndNoColorModRec // End "no color modification" mode.
    };
    /** \details
      Type of packaging primitive.
    */
    enum PackPrimitive
    {
      kRecordPrim = -1, // Unassigned or record primitive (packaging skipped)
      kPointPrim = 0, // Points primitive
      kPolylinePrim, // Polylines primitive
      kTrianglePrim, // Triangles primitive
      kLwdPointPrim, // Points with lineweight
      kLwdPolylinePrim // Polylines with lineweight
    };
    /** \details
      Packager entry type.
      \remarks
      Don't change entry identifiers - this is array indices.
    */
    enum PackEntryType
    {
      kLSEntry = 0, // Line segment entry
      kNLEntry = 1, // No-light entry
      kLTEntry = 2, // Light&&Textured entry
      kLastPackEntry // Number of separated entry arrays
    };
  public: // Constants
    // Maximum vertices in polyline to add into array without stripping
    static const OdInt32 MaxPolylineNoStrip;  /* default: 12 */ // 12 * 2 - 2 == 22pls, 10 verts added
    static const OdInt32 MaxPolylineNoStripPack; /* default: 32 */
  protected:
    bool                     m_bInitialized;  // true between beginMetafile()/endMetafile() calls
    bool                     m_bDynamicSubEntHlt; // Dynamic subentities highlighting enabled
    bool                     m_bNestingMode;  // Nested metafiles processing (reuse same registers)
    OdGsBaseVectorizer*      m_pVectorizer;   // Current render view
    IDirect3DDevice*         m_d3dDevice;     // Current D3D device
    ExGiDirectXMetafilePtr   m_pMetafile;     // Current recording metafile
    // SubEnts highlighting
    bool                     m_bControlHLT;   // Enable subentities highlighting support
    bool                     m_bLastHLT;      // Last highlighting state (summary for all entries)
    // Dynamic subentities highlighting
    OdGsMarker               m_gsLastMarker;  // Last highlighting marker (summary for all entries)
    //
    OdUInt8                  m_transparency;  // Current transparency level
    D3dLwdStyle              m_lwdStyle;      // Lineweight style
    bool                     m_bNoColorMod;   // "no color modification" mode state
    OdUInt8                  m_selectionFlag; // Selection flag
    struct PackEntry
    {
      ArrayType              m_arrayType;     // Type of packaging array
      PackPrimitive          m_packPrimitive; // Type of currently pack primitive
      OdInt32                m_nInitialID;    // Initial index for current packaging primitive
      OdInt32                m_nVertices;     // Count of packed vertices for current primitive
      // Lineweights
      D3dLwdSetting          m_lineWeight;    // Current lineweight
      // SubEnts highlighting
      bool                   m_bLastHLTState; // Last highlight state in entry
      // Dynamic SubEnts highlighting
      OdGsMarker             m_gsPrevMarker;  // Last selection marker in entry
      OdD3DSelMarkArray      m_SelMarks;      // Selection markers array
      //
      OdUInt8                m_transparency;  // Last transparency level
      D3dLwdStyle            m_lwdStyle;      // Last lineweight style
      bool                   m_bNoColorMod;   // "no color modification" mode
      OdUInt32               m_uMtfCount;     // Count of metafiles, added to metafile entry
      OdUInt8                m_selectionFlag; // Last selection flag

      PackEntry()
        : m_arrayType(kUnassignedArray)
        , m_packPrimitive(kRecordPrim)
        , m_nInitialID(0)
        , m_nVertices(0)
        , m_bLastHLTState(false)
        , m_gsPrevMarker(kNullSubentIndex)
        , m_uMtfCount(0)
        , m_selectionFlag( 0 )
      {
      }
    } m_packEntry[kLastPackEntry];
    OdD3DVertexColArray              m_LSArray;         // Array of line segments vertices.
    OdD3DVertexColArray              m_VCArray;         // Array of no-light vertices.
    OdD3DVertexNormalColArray        m_VNCArray;        // Vertices with normals.
    OdD3DVertexNormalTexColArray     m_VNTCArray;       // Vertices with normals and texture coords.
    ExGsDelayedMappingEntry         *m_pCurDelMapEntry; // Current entry for delayed mapping.
  public:
    D3dMetafilePackager();
    ~D3dMetafilePackager();

    /** \details
      Starts packaging for specified metafile.
      \param renderMode [in]  Metafile render mode.
      \param d3dDevice [in]  Pointer to IDirect3DDevice.
      \param pMetafile [in]  Pointer to active metafile.
    */
    void beginMetafile(OdGsBaseVectorizer* pVectorizer, IDirect3DDevice *d3dDevice, ExGiDirectXMetafilePtr pMetafile);
    /** \details
      End packaging for specified metafile.
    */
    void endMetafile();
    /** \details
      Returns currently processing metafile.
    */
    ExGiDirectXMetafilePtr &metafile() { return m_pMetafile; }
    const ExGiDirectXMetafilePtr &metafile() const { return m_pMetafile; }
    /** \details
      Starts nested metafiles packaging mode.
    */
    void runNestingMode();
    /** \details
      Ends nested metafiles packaging mode.
    */
    void stopNestingMode();
    /** \details
      Checks does nested metafiles packaging mode enabled.
    */
    bool isNestingMode() const { return m_bNestingMode; }

    /** \details
      Manual data flush (called automaticly in addRecord() and endMetafile()).
      \param entryType [in]  Specifies entry to be flushed.
    */
    void flush(PackEntryType entryType);

    /** \details
      Add's new record to metafile entry.
      \param entryType [in]  Entry type for which specified record will be added.
      \param recType [in]  Specifies record type.
      \param pRecord [in]  Pointer to newly created metafile record.
    */
    void addRecord(PackEntryType entryType, RecordType recType, D3dRecord* pRecord);

    /** \details
      Pack point into specified metafile entry.
      \remarks
      entryType can be only kLSEntry or kNLEntry.
      \param entryType [in]  Metafile entry type.
      \param pos [in]  Point position.
      \param color [in]  Point color.
    */
    void addPoint(PackEntryType entryType, const OdGePoint3d& pos, DWORD color);

    /** \details
      Pack polyline into specified metafile entry.
      \remarks
      entryType can be only kLSEntry or kNLEntry.
      \param entryType [in]  Metafile entry type.
      \param nPoint [in]  Count of points into points array.
      \param points [in]  Pointer to polyline points array.
      \param color [in]  Polyline color for all points.
    */
    void addPolyline(PackEntryType entryType, OdInt32 nPoints, const OdGePoint3d* points, DWORD color);

    /** \details
      Pack triangle into metafile.
      \remarks
      Entry type depend from pVertices type.
      \param pVertices [in]  Pointer to three triangle vertices.
    */
    void addTriangle(const OdD3DVertexCol *pVertices);
    /** \details
      Pack triangle into metafile.
      \remarks
      Entry type depend from pVertices type.
      \param bTwoSidesMode [in]  true if two sides mode enabled (add's one additional triangle in opposite vertices order).
      \param pVertices [in]  Pointer to three triangle vertices.
    */
    void addTriangle(bool bTwoSidesMode, const OdD3DVertexNormalCol *pVertices);
    /** \details
      Pack triangle into metafile.
      \remarks
      Entry type depend from pVertices type.
      \param bTwoSidesMode [in]  true if two sides mode enabled (add's one additional triangle in opposite vertices order).
      \param pVertices [in]  Pointer to three triangle vertices.
    */
    void addTriangle(bool bTwoSidesMode, const OdD3DVertexNormalTexCol *pVertices);

    /** \details
      Pack polyline with lineweight into specified metafile entry.
      \remarks
      entryType can be only kLSEntry or kNLEntry.
      \param entryType [in]  Metafile entry type.
      \param nPoints [in]  Count of points into points array.
      \param points [in]  Pointer to polyline points array.
      \param color [in]  Polyline color for all points.
      \param lineWeight [in]  Lineweight settings.
    */
    void addLwdPolyline(PackEntryType entryType, bool bPoint, OdInt32 nPoints, const OdGePoint3d* points, DWORD color, const D3dLwdSetting &lineWeight);

    /** \details
      Returns current highlight state for specified metafile entry type.
      \param entryType [in]  Metafile entry type.
    */
    bool curHLTState(PackEntryType entryType) const;

    /** \details
      Returns current selection marker specified metafile entry type.
      \param entryType [in]  Metafile entry type.
    */
    OdGsMarker curSelectionMarker(PackEntryType entryType) const;

    /** \details
      Resets current value of transparency level.
      \param alpha [in]  Inverted opacity (transparency).
    */
    inline void setTransparency(OdUInt8 alpha)
    {
      m_transparency = alpha;
    }

    /** \details
      Resets current value of selection flag.
      \param flag [in]  Selection flag.
    */
    inline void setSelectionFlag( OdUInt8 flag )
    {
      m_selectionFlag = flag;
    }

    /** \details
      Resets current lineweight style.
      \param bUseDef [in]  Use default style.
      \param lesStyle [in]  Line end style.
      \param ljsStyle [in]  Line join style.
    */
    inline void setLwdStyle(bool bUseDef, OdPs::LineEndStyle lesStyle = OdPs::kLesRound, OdPs::LineJoinStyle ljsStyle = OdPs::kLjsRound)
    {
      m_lwdStyle.m_bCurLwdUsesDef = bUseDef;
      m_lwdStyle.m_curLwdLes = lesStyle;
      m_lwdStyle.m_curLwdLjs = ljsStyle;
    }

    /** \details
      Resets "no color modification" mode.
      \param bState [in]  New state for "no color modification" mode.
    */
    inline void setNoColorMod(bool bState)
    {
      m_bNoColorMod = bState;
    }

    /** \details
      Returns true if specified entry is a last entry with data into entries array.
      \param entryType [in]  Metafile entry type.
    */
    bool isLastDataEntry(PackEntryType entryType) const;

    /** \details
      Returns last entry type with data into entries array.
      \param entryType [out]  Metafile entry type.
    */
    bool lastDataEntry(PackEntryType &entryType) const;

    /** \details
      Returns OdGsBaseVectorizeView for current metafile.
    */
    const OdGsBaseVectorizer *metafileVectorizer() const;

    /** \details
      Starts delay cache entry construction.
      \param pEntry [in]  Delay cache entry.
    */
    void startDelayMapping(void *pEntry);

    /** \details
      Add's triangle to delay mapping cache entry.
      \param bTwoSidesMode [in]  true if two sides mode enabled (add's one additional triangle in opposite vertices order).
    */
    void addTriangleToDelayMapping(bool bTwoSidesMode);

    /** \details
      Stops delay cache entry construction.
    */
    void flushDelayMapping();

    inline OdD3DVertexColArray &getLSArray() { return m_LSArray; }
    inline OdD3DVertexColArray &getVCArray() { return m_VCArray; }
    inline OdD3DVertexNormalColArray &getVNCArray() { return m_VNCArray; }
    inline OdD3DVertexNormalTexColArray &getVNTCArray() { return m_VNTCArray; }
    inline const OdD3DVertexColArray &getLSArray() const { return m_LSArray; }
    inline const OdD3DVertexColArray &getVCArray() const { return m_VCArray; }
    inline const OdD3DVertexNormalColArray &getVNCArray() const { return m_VNCArray; }
    inline const OdD3DVertexNormalTexColArray &getVNTCArray() const { return m_VNTCArray; }

    /** \details
      Returns true after beginMetafile() call, and before endMetafile().
    */
    bool isInitialized() const;

#ifdef DEBUG
    // Statistical information
    struct Stats
    {
      OdUInt32 m_nLSVertices;
      OdUInt32 m_nVCVertices;
      OdUInt32 m_nVNCVertices;
      OdUInt32 m_nVNTCVertices;

      OdUInt32 m_nLSMetafiles;
      OdUInt32 m_nNLMetafiles;
      OdUInt32 m_nLTMetafiles;
    };
    Stats statInfo() const;
#endif

    /** \details
      Enable dynamic subentities highlighting.
      \param bSet [in]  New flag.
    */
    void enableDynamicSubEntHlt(bool bSet) { m_bDynamicSubEntHlt = bSet; }

    /** \details
      Setups appripriate array parameters for current rendering options.
      \param b4Cache [in]  Enables arrays setup for cache.
    */
    void setupArrays(bool b4Cache);
  protected:
    void clear();
    void testHLTState(PackEntryType entryType);
};

/** \details
   This class provides tessellation functionality for the Teigha vectorization framework  
   Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGiDirectXGeometry : public OdGiGeometrySimplifier,
                            public OdRxObject
{
  bool                      m_bIsVertsNormals;
  bool                      m_bIsFaceNormals;
  bool                      m_bIsVertsColors;
  bool                      m_bFaceNormalsRequired;
  DWORD                     m_bgColor;
  
  bool                      m_bDoubleBufferEnabled;
  bool                      m_bTwoSidesMode;
  
  DWORD                     m_color;
  int                       m_pixLineWeight;
  D3dLwdSetting             m_lineWeight;
  OdPs::LineType            m_linetype;

  CComPtr<IDirect3DDevice>  m_pD3Ddevice;
  OdD3DStateManager*        m_pD3DSM;
  const ODCOLORREF*         m_paletteColors;
  OdUInt16                  m_numPaletteColors;

  enum
  {
    eNotInit = 0,
    eMaterialWay = 1,
    eNotMaterialWay = 2
  } m_MaterialWay;

  CComPtr<IDirect3DDevice> d3dDevice();
  OdD3DStateManager &d3dStateManager();

  ExGiDirectXMetafilePtr m_pMetafile;
  D3dMetafilePlayContext m_mfPlayCtx;

  D3dMetafilePackager    m_rPackager;

  void prepareShMFacesGeneration(const OdGiFaceData* pFaceData);

  const OdGiPsLinetypes::PsLinetypeOGL *m_psLinetypeDefs;

  OdGiMaterialItemPtr   m_pIntMtl; // Material rendering optimization
  D3dMaterialViewRedirect *m_pMViewRedirect;
public:
  ExGiDirectXGeometry();
  virtual ~ExGiDirectXGeometry();
  bool m_bProcessingTTF;
  bool m_bShadingDisabled;
  bool m_bPlayMetafile;

  ODRX_DECLARE_MEMBERS(ExGiDirectXGeometry);

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
  void beginMetafile(OdRxObject* pMetafile, OdGsBaseVectorizer *pVectorizer);
  /** \details
    Ends vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
    \param pMetafileSize [in] Pointer to the starage for metafile size, if NULL size will not be calculated
  */
  void endMetafile(OdRxObject* pMetafile, OdUInt64* pMetafileSize );
  /** \details
    Plays (draws) the specified Metafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  void playMetafile(const OdRxObject* pMetafile);

  /** \details
    Plays (draws) the specified Metafile (cache) object for Select or Extent
  */
  void play4Select(const OdRxObject* pMetafile, D3dPlay4SelectContext& context );// OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, bool bCheckMark = true);

  /** \details
    Clears Metafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  void clearMetafile(OdRxObject* pMetafile);
 
  /** \details
    Causes the data from the back buffer to appear on the front buffer.
  */
  void present();

  // graphics data
  // required by simplifier

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* points);
  void triangleOut( const OdInt32* vertices, const OdGeVector3d* pNormal );

  void shellProc(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);

  void generateShellFaces(OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0);
  void generateMeshFaces(OdInt32 rows, OdInt32 columns,
    const OdGiFaceData* pFaceData);

  void meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal);
  void shellFaceOut(OdInt32 faceListSize, const OdInt32* faceList, const OdGeVector3d* pNormal);

  void initTexture(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    bool transparency,
    double brightness,
    double contrast,
    double fade);
  
  void uninitTexture();

  void textProc(const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                const OdChar* msg, OdInt32 nLength, bool raw, const OdGiTextStyle* pStyle, const OdGeVector3d* pExtrusion);

  void polypointProc(OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);

  /** \details
    Returns the DirectX color corresponding to the specified color.
    \param color [in]  Color.
  */
  DWORD getD3DColor(const OdCmEntityColor& color);
  
  /** \details
    Returns the DirectX color corresponding to the specified color.
    \param color [in]  Color.
  */
  static DWORD getD3DColor(ODCOLORREF color);
  static ODCOLORREF getGDIColor(DWORD color);

  /** \details
    Sets the logical palette to be used by this Geometry object.
    
    \param logicalPalette [in]  Logical palette.
    \param numColors [in]  Number of colors in palette.
  */
  void setLogicalPalette(const ODCOLORREF* logicalPalette, OdUInt16 numColors);

  // traits

  /** \details
    Sets the draw color for this Geometry object.
    \param color [in]  Color.
  */
  void draw_color(DWORD color);

  /** \details
    Sets the draw lineweight for this Geometry object.
    \param lwLineWeight [in]  Lineweight as enum.
    \param iLineWeight [in]  Lineweight in pixels.
  */
  void draw_lineweight(OdDb::LineWeight lwLineWeight, int iLineWeight);
  /** \details
    Sets the draw lineweight for this Geometry object.
    \param dLineWeight [in]  Plot styles lineweight.
    \param iLineWeight [in]  Lineweight in pixels.
  */
  void draw_lineweight(double dLineWeight, int iLineWeight);
  /** \details
    Sets the draw lineweight for this Geometry object.
    \param dLineWeight [in]  Lineweight in geometry coordinates system.
    \param iLineWeight [in]  Lineweight in pixels.
  */
  void draw_lineweightOverride(double dLineWeight, int iLineWeight);

  /** \details
    Sets the draw fillstyle for this Geometry object.
    \param fillStyle [in]  Fillstyle.
  */
  void draw_fillStyle(OdPs::FillStyle fillStyle);

  /** \details
    Sets the draw linetype for this Geometry object.
    \param lineType [in]  Linetype.
    \param isAdaptive [in]  Controls if linetype is adaptive.
    \param linePatSize [in]  Line pattern size.
  */
  void draw_linetype(OdPs::LineType lineType, bool isAdaptive, double linePatSize);

  /** \details
    Sets the draw transparency for this Geometry object.
    \param alpha [in]  Transparency level.
  */
  void draw_transparency(OdUInt8 alpha);

  /** \details
    Sets the draw line style for this Geometry object.
    \param bUseDef [in]  Use default lineweight style.
    \param lesStyle [in]  Line end style.
    \param ljsStyle [in]  Line join style.
  */
  void draw_lineStyle(bool bUseDef, OdPs::LineEndStyle lesStyle = OdPs::kLesRound, OdPs::LineJoinStyle ljsStyle = OdPs::kLjsRound);

  /** \details
    Sets "no color modification" mode for this Geometry object.
    \param bSet [in]  Enable "no color modification" mode.
  */
  void draw_noColorMod(bool bSet);

  /** \details
    Controls the TwoSidesMode flag for this Geometry object.
    \param twoSidesMode [in]  Controls flag.
  */
  void setTwoSidesMode(bool twoSidesMode);
  /** \details
    Controls the DoubleBufferEnabled flag for this Geometry object.
    \param doubleBufferEnabled [in]  Controls flag.
  */
  void setDoubleBufferEnabled(bool doubleBufferEnabled);

  /** \details
    Sets the D3DDevice for this Geometry object.
    \param pD3Ddevice [in]  Pointer to the D3Ddevice.
  */
  void setD3DDevice(IDirect3DDevice* pD3Ddevice);
  /** \details
    Sets the D3D device state manager for this Geometry object.
    \param pD3DSM [in]  Pointer to the D3D device state manager.
  */
  void setD3DStateManager(OdD3DStateManager* pD3DSM);

  // for internal use only

  /** \details
    Sets the vectorizer for this Geometry object.
    \param pView [in]  Pointer to the vectorizer.
    \note
    This function is for internal use only.
  */
  void setVectorizer(OdGsBaseVectorizer* pView);

  /** \details
    Sets vectorizer interface for material/mapper functionality.
    \param pMViewRedirect [in]  Pointer to D3dMaterialViewRedirect interface.
    \param pView [in]  Optional pointer to the vectorizer
  */
  void setMaterialViewRedirect(D3dMaterialViewRedirect *pMViewRedirect, OdGsBaseVectorizer* pView = NULL);

  /** \details
    Sets array parameters optimized for rendering state.
    \param b4Cache [in]  Enables optimized parameters for cache.
  */
  void setupIntermediateArrays(bool b4Cache);

  /** \details
    Sets the ForceFill flag for this Geometry object.
    \param forceFill [in]  Controls flag.
  */
  void setForceFill(bool forceFill) { m_mfPlayCtx.setForceFill(forceFill); }

  /** \details
    Sets the draw visual style for this Geometry object.
    \param visualStyle [in]  Visual style.
    \remarks
    This function is not implemented currently.
  */
  void draw_visualStyle(const OdGiVisualStyle &visualStyle);

  // Materials support

  bool skipMaterialProcess(OdDbStub *materialId);
  OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData);
  void renderMaterial(OdGiMaterialItemPtr pCache, OdDbStub *materialId);

  void playDelayCacheEntry(const void *pEntry);
  void mapperChangedForDelayCache();

  OdGiMaterialItemPtr currentMaterial() const;
  void resetCurrentMaterial(OdGiMaterialItemPtr pMaterial);
  bool isMaterialEnabled() const;
  bool isMaterialAvailable() const;
  void resetCurrentMaterial();
  OdGiMapperItemPtr currentMapper(bool bForCoords = true) const;
  void resetCurrentMapper(OdGiMapperItemPtr pMapper);
  bool isMapperEnabled() const;
  bool isMapperAvailable() const;
  bool isMappingDelayed() const;
  void appendDelayCacheEntry(void *pEntry);
  OdGiMaterialTextureManager *materialTextureManager();

  void d3dSelectionFlags(OdUInt8 selFlags);
};

typedef OdSmartPtr<ExGiDirectXGeometry> ExGiDirectXGeometryPtr;

#include "TD_PackPop.h"

#endif // __OD_EX_GI_DIRECT_X_GEOMETRY__
