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
// GLES2 device base rendition interface

#ifndef ODTRVISRENDITION
#define ODTRVISRENDITION

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "TrVisInfoString.h"
#include "TrVisSharingProviderClient.h"
#include "TrVisSurfaceDef.h"
#include "TrVisViewportDef.h"
#include "TrVisMetafileDef.h"
#include "TrVisTextureDef.h"
#include "TrVisMaterialDef.h"
#include "TrVisLightDef.h"
#include "TrVisExtentsDef.h"
#include "TrVisHlBranchDef.h"
#include "TrVisVisualStyle.h"
#include "TrVisLayerDef.h"

#include "Ge/GeExtents3d.h"
#include "Gs/GsDCRectArray.h"
#include "StaticRxObject.h"
#include "ThreadsCounter.h"

class OdTrVisRenderClient;

class OdTrVisUniqueID;
class OdGiDrawable;

class OdGsFiler;
class OdTrVisIdMap;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRendition : public OdRxObject
{
  public:
    // Viewport data fields
    enum KindOfViewportModification
    {
      // Viewport visibility flag
      kViewportModVisibility  = OdTrVisViewportDef::kViewportModVisibility,
      // Transform matrices, camera positioning
      kViewportModOrientation = OdTrVisViewportDef::kViewportModOrientation,
      // Persistent viewport parameters, flags and etc.
      kViewportModPersistent  = OdTrVisViewportDef::kViewportModPersistent,
      // Contextual viewport parameters (lineweight style, fading, etc.)
      kViewportModContextual  = OdTrVisViewportDef::kViewportModContextual,
      // Viewport rectangle in screen coordinates
      kViewportModRect        = OdTrVisViewportDef::kViewportModRect,
      // Viewport Nrc clipping boundary in screen coordinates
      kViewportModNrcClip     = OdTrVisViewportDef::kViewportModNrcClip,
      // Array of lineweights
      kViewportModLineweights = OdTrVisViewportDef::kViewportModLineweights,
      // Base shader program
      kViewportModShader      = OdTrVisViewportDef::kViewportModShader,
      // Viewport border in screen coordinates
      kViewportModBorder      = OdTrVisViewportDef::kViewportModBorder,

      kViewportModAll         = OdTrVisViewportDef::kViewportModAll
    };
    // Core rendition data types
    enum DataType
    {
      kSurfaceData = 0,
      kViewportData, kViewportPropsData, kOrderData, kLightsData, kBackgroundData, kExtentsData,
      kMetafileData, kMetafilePropsData,
      kHlBranchData,
      kTextureData,
      kMaterialData,
      kProgramData,
      kVisualStyleData,
      kLayerData,
      kOverlayData, kOverlayPropsData,

      kNDataTypes
    };
    // Mt handling types
    enum MtHandlerType
    {
      kMtSkip = 0, // Skip data
      kMtRedirect, // Redirect data without handling
      kMtGlobal,   // Redirect under global mutex
      kMtCompete,  // Redirect under specified mutex
      kMtRecord    // Record till completion (placed under own recorder mutex)
    };
    // Special DisplayId codes, provide fast access to renderer functionality directly from display list
    enum DisplayIdCodes
    {
      kDisplayCodeZero        = 0, // Empty display code
      kDisplayCodeHighlight   = 1, // Execute highlighting procedure
      kDisplayCodeUnhighlight = 2, // Execute unhighlighting procedure
      kDisplayCodePushMatrix  = 3, // Execute push transformation matrix procedure
      kDisplayCodePopMatrix   = 4, // Execute pop transformation matrix procedure
      kDisplayCodeSelMarkers  = 5, // Setup selection markers
      kDisplayCodeDrawOrder   = 6, // Provide drawing order for upcoming geometry
      kDisplayCodeMultiOrder  = 7, // Upcoming geometry contain implicated drawing order

      kDisplayCodeRange       = 0x10 // We reserve first 16 codes for special procedures
    };
    // Rendition flags
    enum RenditionFlags
    {
      kSupportPartialUpdate       = (1 << 0), // isSupportPartialUpdate
      kSupportSnapshotQueries     = (1 << 1), // isSupportSnapshotQueries
      kSupportCompositeMetafiles  = (1 << 2), // isSupportCompositeMetafiles
      kSupportVisualStyles        = (1 << 3), // isSupportVisualStyles
      kSupportGsStateSaving       = (1 << 4), // isSupportGsStateSaving
      kSupportOverlayBuffers      = (1 << 5), // isSupportOverlayBuffers
      kUpdateClientSectionOnly    = (1 << 6), // updateClientDependentSectionOnly
      kUpdateShareableSectionOnly = (1 << 7)  // updateShareableSectionOnly
    };
  public:
    struct IdRegistrator
    {
      // Register persistent resource
      virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId) = 0;
      // Register transient resource
      virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId) = 0;
      // Register persistent or transient resource
      void reg(DataType type, OdTrVisId id, OdDbStub *pPersId, const void *pTransId)
      { if (pPersId) reg(type, id, pPersId); else reg(type, id, reinterpret_cast<const OdGiDrawable*>(pTransId)); }
      // Copy resource registration
      virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId) = 0;
      // Remove resource registration
      virtual void kill(DataType type, OdTrVisId id) = 0;
      // Access resource registration
      virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const = 0;
    };
  public:
    // Client information
    struct ClientInfo
    {
      OdTrVisInfoString m_glVendor;     // OpenGL vendor
      OdTrVisInfoString m_glRenderer;   // OpenGL renderer name
      OdTrVisInfoString m_glVersion;    // OpenGL version
      OdTrVisInfoString m_glSLVersion;  // GLSL version
      OdTrVisInfoString m_glExtensions; // List of extensions
    };
    // Client settings
    struct ClientSettings
    {
      bool m_bObtained; // True if settings obtained from client.
      // Client information
      ClientInfo m_ciInfo;
      // Flags
      enum Flags
      {
        // Texture settings
        kNonPow2Textures = (1 << 0), // True if support non power of two textures.
        kNonPow2Wrapping = (1 << 1), // True if support all non power of two textures wrapping modes.
        kSupportsBGR     = (1 << 2), // True if support BGR textures format.
        kSupportsBGRA    = (1 << 3), // True if support BGRA textures format.
        kRequireBGR      = (1 << 4), // True if support only BGR/BGRA textures format.
        kOverrideLut     = (1 << 5), // True if client override Lut device settings.
        kLutMonochrome   = (1 << 6), // True if client prefer Lut monochrome textures.
        kLutPalette      = (1 << 7), // True if client prefer Lut 4-8bpp textures.
        // Performance/quality degradation
        kShadeByVertex   = (1 << 8)  // Mobile devices could have serious performance loss in shaded modes, since per-pixel
                                     // calculations too massive. Enable this flag to use per-vertex lighting calculation
                                     // instead. This will degenerate lighting quality, but will increase performance.
      };
      OdUInt32 m_uFlags; // Client flags.
      OdUInt32 m_nMaxTextureSize; // Maximal texture size
      OdUInt32 m_nTextureAlignment; // Required default texture alignment
      // Lineweights
      float m_nMaxLineWidth; // Maximal line width
      float m_nMaxPointSize; // Maximal point size
      // Shaders
      OdUInt32 m_nMaxFragShadUniformVecs; // Maximum uniform vectors per fragment shader
                                          // (for NUM_LIGHTS computation. Pass zero to truncate count for minimal)

      ClientSettings()
        : m_bObtained(false)
        , m_ciInfo()
        , m_uFlags(0)
        , m_nMaxTextureSize(2048)
        , m_nTextureAlignment(4)
        , m_nMaxLineWidth(1.0f)
        , m_nMaxPointSize(1.0f)
        , m_nMaxFragShadUniformVecs(16)
      {
      }

      // Flag accessors

      // Textures

      bool isSupportNonPow2Textures() const { return GETBIT(m_uFlags, kNonPow2Textures); }
      void setSupportNonPow2Textures(bool bSet) { SETBIT(m_uFlags, kNonPow2Textures, bSet); }

      bool isSupportNonPow2TexturesWrapping() const { return GETBIT(m_uFlags, kNonPow2Wrapping); }
      void setSupportNonPow2TexturesWrapping(bool bSet) { SETBIT(m_uFlags, kNonPow2Wrapping, bSet); }

      bool isSupportBGR() const { return GETBIT(m_uFlags, kSupportsBGR); }
      void setSupportBGR(bool bSet) { SETBIT(m_uFlags, kSupportsBGR, bSet); }

      bool isSupportBGRA() const { return GETBIT(m_uFlags, kSupportsBGRA); }
      void setSupportBGRA(bool bSet) { SETBIT(m_uFlags, kSupportsBGRA, bSet); }

      bool isRequireBGR() const { return GETBIT(m_uFlags, kRequireBGR); }
      void setRequireBGR(bool bSet) { SETBIT(m_uFlags, kRequireBGR, bSet); }

      bool isLutOverride() const { return GETBIT(m_uFlags, kOverrideLut); }
      void setLutOverride(bool bSet) { SETBIT(m_uFlags, kOverrideLut, bSet); }

      bool isLutMonochrome() const { return GETBIT(m_uFlags, kLutMonochrome); }
      void setLutMonochrome(bool bSet) { SETBIT(m_uFlags, kLutMonochrome, bSet); }
      bool overridenLutMonochromeState(bool bDeviceSetting) const
        { return (isLutOverride()) ? isLutMonochrome() : bDeviceSetting; }

      bool isLutPalette() const { return GETBIT(m_uFlags, kLutPalette); }
      void setLutPalette(bool bSet) { SETBIT(m_uFlags, kLutPalette, bSet); }
      bool overridenLutPaletteState(bool bDeviceSetting) const
        { return (isLutOverride()) ? isLutPalette() : bDeviceSetting; }

      // Performance/quality degradation

      bool isShadeByVertex() const { return GETBIT(m_uFlags, kShadeByVertex); }
      void setShadeByVertex(bool bSet) { SETBIT(m_uFlags, kShadeByVertex, bSet); }
    };
  public:
    ODRX_USING_HEAP_OPERATORS(OdRxObject);

    OdTrVisRendition() { }
    ~OdTrVisRendition() { }

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice) = 0;
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice) = 0;

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const = 0;
    // Rendition capabilities as separate methods
    // Called to check does rendition supports partial update (have sense for local renditions only).
    bool isSupportPartialUpdate() const { return GETBIT(queryRenditionCaps(kSupportPartialUpdate), kSupportPartialUpdate); }
    // Check does rendition support snapshot queries
    bool isSupportSnapshotQueries() const { return GETBIT(queryRenditionCaps(kSupportSnapshotQueries), kSupportSnapshotQueries); }
    // Check does rendition supports composite metafiles
    bool isSupportCompositeMetafiles() const { return GETBIT(queryRenditionCaps(kSupportCompositeMetafiles), kSupportCompositeMetafiles); }
    // Check does rendition supports visual styles
    bool isSupportVisualStyles() const { return GETBIT(queryRenditionCaps(kSupportVisualStyles), kSupportVisualStyles); }
    // Check does rendition supports GsFiler-compatible state saving/loading
    bool isSupportGsStateSaving() const { return GETBIT(queryRenditionCaps(kSupportGsStateSaving), kSupportGsStateSaving); }
    // Check does rendition supports multiple overlapping rendering buffers
    bool isSupportOverlayBuffers() const { return GETBIT(queryRenditionCaps(kSupportOverlayBuffers), kSupportOverlayBuffers); }
    // Update client-dependent section only
    bool updateClientDependentSectionOnly() const { return GETBIT(queryRenditionCaps(kUpdateClientSectionOnly), kUpdateClientSectionOnly); }
    // Update shareable section only
    bool updateShareableSectionOnly() const { return GETBIT(queryRenditionCaps(kUpdateShareableSectionOnly), kUpdateShareableSectionOnly); }

    // Partial update support for local renditions:
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId overlayId, const OdGsDCRectArray &invalidRects, bool bOverlayInvalid = false) = 0;
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries) = 0;

    // Query snapshot
    virtual void querySnapshot(OdUInt8 *pScanlines, long minX, long minY, long nWidth, long nHeight) = 0;

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider() = 0;

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const = 0;

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const = 0;

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisDisplayId displayId, const OdTrVisFlatMetafileContainer **pStream) const = 0;

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode mode, OdTrVisVisualStyleId vsId = kTrVisNegativeId,
                                            OdTrVisGeomType geomType = OdTrVisGeomType_Default, OdUInt32 shadingFlags = 0) const = 0;

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const = 0;
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap) = 0;

    // Mid-level callbacks

    // Client information

    virtual void obtainClientSettings(ClientSettings &clientSets) = 0;

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef) = 0;

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef) = 0;
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId) = 0;
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility) = 0;
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder) = 0;

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId/*, const OdTrVisViewportDef &pDef*/) = 0;
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId) = 0;
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex) = 0;
    // Called when viewport already exist in device modified.
    virtual void onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod = kViewportModAll) = 0;

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed (for obtain full list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries) = 0;
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen) = 0;
    // Called when order of metafiles can be inherit from other source
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 ) = 0;

    // Viewport light lists

    // Called when lights list in viewport is changed.
    virtual void onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                     const OdTrVisLightDef *pList, OdUInt32 nEntries) = 0;

    // Viewport background

    // Called when background stream in viewport is changed.
    virtual void onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                     OdTrVisProgramId baseProgramId = kTrVisNegativeId) = 0;

    // Viewport extents

    // Called when viewport extents changed
    virtual void onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef) = 0;

    // Overlay viewport parameters override

    // Called when viewport needs override view parameters for specific overlay
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef()) = 0;

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef) = 0;
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId) = 0;
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef) = 0;
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility) = 0;
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight) = 0;
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade) = 0;
    // Called to attach highlighting branch to metafile.
    virtual void onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId) = 0;
    // Called to detach highlighting branch from metafile.
    virtual void onMetafileHlBranchDetached(OdTrVisMetafileId metafileId) = 0;

    // Highlighting

    // Called when new highlighting branch added.
    virtual void onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef) = 0;
    // Called when exist highlighting branch modified.
    virtual void onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                    bool bChildsModified, bool bMarkersModified) = 0;
    // Called when exist highlighting branch deleted.
    virtual void onHlBranchDeleted(OdTrVisHlBranchId hlBranchId) = 0;

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef) = 0;
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId) = 0;

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef) = 0;
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId) = 0;

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef) = 0;
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef) = 0;
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId) = 0;

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef) = 0;
    // Called when exist layer modified.
    virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef) = 0;
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId) = 0;
    //Called when layer viewport-dependent properties changed
    virtual void onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps = OdTrVisLayerProps() ) = 0;
};

typedef OdSmartPtr<OdTrVisRendition> OdTrVisRenditionPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionDef : public OdTrVisRendition
{
  protected:
    struct IdRegistratorStub : public IdRegistrator
    {
      virtual void reg(DataType /*type*/, OdTrVisId /*id*/, OdDbStub * /*pPersId*/) { }
      virtual void reg(DataType /*type*/, OdTrVisId /*id*/, const OdGiDrawable * /*pTransId*/) { }
      virtual void reg(DataType /*type*/, OdTrVisId /*id*/, const OdTrVisUniqueID & /*uId*/) { }
      virtual void kill(DataType /*type*/, OdTrVisId /*id*/) { }
      virtual OdTrVisUniqueID *get(DataType /*type*/, OdTrVisId /*id*/, bool /*bKill*/ = false) const { return NULL; }
    } m_IdRegistratorStub;
  public:
    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient * /*pDevice*/) { }
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient * /*pDevice*/) { }

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 /*requestFor*/ = 0xFFFFFFFF) const { return 0; }

    // Partial update support for local renditions:
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId /*overlayId*/, const OdGsDCRectArray & /*invalidRects*/, bool /*bOverlayInvalid*/ = false) { }
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, const OdTrVisDisplayId * /*pList*/, OdUInt32 /*nEntries*/) { }

    // Query snapshot
    virtual void querySnapshot(OdUInt8 * /*pScanlines*/, long /*minX*/, long /*minY*/, long /*nWidth*/, long /*nHeight*/) { }

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider() { return OdTrVisSharingProviderClientPtr(); }

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType & /*type*/) const { return kMtSkip; }

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const { return NULL; }

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisDisplayId /*displayId*/, const OdTrVisFlatMetafileContainer ** /*pStream*/) const { return false; }

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode /*mode*/, OdTrVisVisualStyleId /*vsId*/ = kTrVisNegativeId,
                                            OdTrVisGeomType /*geomType*/ = OdTrVisGeomType_Default, OdUInt32 /*shadingFlags*/ = 0) const
    { return kTrVisNegativeId; }

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler * /*pFiler*/) const { return false; }
    virtual bool loadRenditionState(OdGsFiler * /*pFiler*/, OdTrVisIdMap * /*pIdMap*/) { return false; }

    // Mid-level callbacks

    // Client information

    virtual void obtainClientSettings(ClientSettings & /*clientSets*/) { }

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef & /*pDef*/) { }

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId /*overlayId*/, const OdTrVisOverlayDef & /*pDef*/) { }
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId /*overlayId*/) { }
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId /*overlayId*/, bool /*bVisibility*/) { }
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId /*overlayId*/, OdInt16 /*nRenderOrder*/) { }

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId /*viewportId*/ /*, const OdTrVisViewportDef &pDef*/) { }
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId /*viewportId*/) { }
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId /*viewportId*/, int /*nIndex*/) { }
    // Called when viewport already exist in device modified.
    virtual void onViewportModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportDef & /*pDef*/, OdUInt32 /*kindOfMod*/ = kViewportModAll) { }
    // Called when order of metafiles can be inherit from other source
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 ) {}

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, const OdTrVisDisplayId * /*pList*/, OdUInt32 /*nEntries*/) { }
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, OdUInt32 /*nInvalidFrom*/, OdUInt32 /*nInvalidLen*/,
                                        const OdTrVisDisplayId * /*pValidFrom*/, OdUInt32 /*nValidLen*/) { }

    // Viewport light lists

    // Called when lights list in viewport is changed.
    virtual void onLightsListChanged(OdTrVisViewportId /*viewportId*/, float /*ambientLight*/ [4], bool /*bDefaultLightsOn*/,
                                     const OdTrVisLightDef * /*pList*/, OdUInt32 /*nEntries*/) { }

    // Viewport background

    // Called when background stream in viewport is changed.
    virtual void onBackgroundChanged(OdTrVisViewportId /*viewportId*/, OdTrVisFlatMetafileContainerPtr /*pStream*/,
                                     OdTrVisProgramId /*baseProgramId*/ = kTrVisNegativeId) { }

    // Viewport extents

    // Called when viewport extents changed
    virtual void onExtentsChanged(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, const OdTrVisExtentsDef & /*pDef*/) { }

    // Overlay viewport parameters override

    // Called when viewport needs override view parameters for specific overlay
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, bool /*bOverride*/,
                                             const OdTrVisViewParamsDef & /*pDef*/ = OdTrVisViewParamsDef()) { }

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef & /*pDef*/) { return (OdTrVisDisplayId)metafileId; }
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId /*metafileId*/) { }
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId /*metafileId*/, const OdTrVisMetafileDef & /*pDef*/) { }
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId /*metafileId*/, bool /*bVisibility*/) { }
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId /*metafileId*/, bool /*bHighlight*/) { }
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId /*metafileId*/, bool /*bFade*/) { }
    // Called to attach highlighting branch to metafile.
    virtual void onMetafileHlBranchAttached(OdTrVisMetafileId /*metafileId*/, OdTrVisHlBranchId /*hlBranchId*/) { }
    // Called to detach highlighting branch from metafile.
    virtual void onMetafileHlBranchDetached(OdTrVisMetafileId /*metafileId*/) { }

    // Highlighting

    // Called when new highlighting branch added.
    virtual void onHlBranchAdded(OdTrVisHlBranchId /*hlBranchId*/, const OdTrVisHlBranchDef & /*pDef*/) { }
    // Called when exist highlighting branch modified.
    virtual void onHlBranchModified(OdTrVisHlBranchId /*hlBranchId*/, const OdTrVisHlBranchDef & /*pDef*/,
                                    bool /*bChildsModified*/, bool /*bMarkersModified*/) { }
    // Called when exist highlighting branch deleted.
    virtual void onHlBranchDeleted(OdTrVisHlBranchId /*hlBranchId*/) { }

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId /*textureId*/, const OdTrVisTextureDef & /*pDef*/) { }
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId /*textureId*/) { }

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId /*materialId*/, const OdTrVisMaterialDef & /*pDef*/) { }
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId /*materialId*/) { }

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId /*visualStyleId*/, const OdTrVisVisualStyle & /*pDef*/) { }
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId /*visualStyleId*/, const OdTrVisVisualStyle & /*pDef*/) { }
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId /*visualStyleId*/) { }

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId /*layerId*/, const OdTrVisLayerDef & /*pDef*/) { }
    // Called when exist layer modified.
    virtual void onLayerModified(OdTrVisLayerId /*layerId*/, const OdTrVisLayerDef & /*pDef*/) { }
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId /*layerId*/) { }
    //Called when layer viewport-dependent properties changed
    virtual void onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps = OdTrVisLayerProps() )
    {
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionRedir : public OdTrVisRendition
{
  protected:
    OdTrVisRendition *m_pRedir;
    OdTrVisRendition *m_pInformationalRedir;
  public:
    struct IdRegistratorRedir : public IdRegistrator
    {
      IdRegistrator *m_pIdRegRedir;
      IdRegistratorRedir() : m_pIdRegRedir(NULL) { }

      void setIdRegRedirection(IdRegistrator *pRedir) { m_pIdRegRedir = pRedir; }
      IdRegistrator *getIdRegRedirection() { return m_pIdRegRedir; }
      const IdRegistrator *getIdRegRedirection() const { return m_pIdRegRedir; }
      bool hasIdRegRedirection() const { return (m_pIdRegRedir != NULL); }

      virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->reg(type, id, pPersId);
      }
      virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->reg(type, id, pTransId);
      }
      virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->reg(type, id, uId);
      }
      virtual void kill(DataType type, OdTrVisId id)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->kill(type, id);
      }
      virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const
      {
        if (hasIdRegRedirection())
          return getIdRegRedirection()->get(type, id, bKill);
        return NULL;
      }
    };
  public:
    OdTrVisRenditionRedir() : m_pRedir(NULL), m_pInformationalRedir(NULL) { }

    void setRedirection(OdTrVisRendition *pRedir) { m_pRedir = pRedir; }
    OdTrVisRendition *getRedirection() { return m_pRedir; }
    const OdTrVisRendition *getRedirection() const { return m_pRedir; }
    bool hasRedirection() const { return (m_pRedir != NULL); }

    void setInformationalRedirection(OdTrVisRendition *pRedir) { m_pInformationalRedir = pRedir; }
    OdTrVisRendition *getInformationalRedirection()
    {
      if (!m_pInformationalRedir)
        throw OdError(eNullPtr);
      return m_pInformationalRedir;
    }
    const OdTrVisRendition *getInformationalRedirection() const
    {
      if (!m_pInformationalRedir)
        throw OdError(eNullPtr);
      return m_pInformationalRedir;
    }
    bool hasInformationalRedirection() const { return (m_pInformationalRedir != NULL); }

    void setRedirections(OdTrVisRendition *pRedir)
    {
      setRedirection(pRedir);
      setInformationalRedirection(pRedir);
    }
    bool hasRedirections() const { return hasRedirection() && hasInformationalRedirection(); }

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onDeviceUpdateBegin(pDevice);
    }
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onDeviceUpdateEnd(pDevice);
    }

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->queryRenditionCaps(requestFor);
      return 0;
    }

    // Partial update support for local renditions:
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId overlayId, const OdGsDCRectArray &invalidRects, bool bOverlayInvalid = false)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onPartialUpdateInvalidRects(overlayId, invalidRects, bOverlayInvalid);
    }
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onPartialUpdateList(viewportId, overlayId, pList, nEntries);
    }

    // Query snapshot
    virtual void querySnapshot(OdUInt8 *pScanlines, long minX, long minY, long nWidth, long nHeight)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->querySnapshot(pScanlines, minX, minY, nWidth, nHeight);
    }

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider()
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->createSharingProvider();
      return OdTrVisSharingProviderClientPtr();
    }

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->mtDataTypeProcessing(type);
      return kMtSkip;
    }

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->idRegistrator();
      return NULL;
    }

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisDisplayId displayId, const OdTrVisFlatMetafileContainer **pStream) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->queryMetafileStream(displayId, pStream);
      return false;
    }

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode mode, OdTrVisVisualStyleId vsId = kTrVisNegativeId,
                                            OdTrVisGeomType geomType = OdTrVisGeomType_Default, OdUInt32 shadingFlags = 0) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->queryProgramId(mode, vsId, geomType, shadingFlags);
      return kTrVisNegativeId;
    }

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->saveRenditionState(pFiler);
      return false;
    }
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->loadRenditionState(pFiler, pIdMap);
      return false;
    }

    // Mid-level callbacks

    // Client information

    virtual void obtainClientSettings(ClientSettings &clientSets)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->obtainClientSettings(clientSets);
    }

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onSurfaceChanged(pDef);
    }

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onOverlayAdded(overlayId, pDef);
    }
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId)
    {
      if (hasRedirection())
        getRedirection()->onOverlayDeleted(overlayId);
    }
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onOverlayVisibilityChanged(overlayId, bVisibility);
    }
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder)
    {
      if (hasRedirection())
        getRedirection()->onOverlayRenderOrderChanged(overlayId, nRenderOrder);
    }

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId /*, const OdTrVisViewportDef &pDef*/)
    {
      if (hasRedirection())
        getRedirection()->onViewportAdded(viewportId);
    }
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId)
    {
      if (hasRedirection())
        getRedirection()->onViewportDeleted(viewportId);
    }
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex)
    {
      if (hasRedirection())
        getRedirection()->onViewportInserted(viewportId, nIndex);
    }
    // Called when viewport already exist in device modified.
    virtual void onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod = kViewportModAll)
    {
      if (hasRedirection())
        getRedirection()->onViewportModified(viewportId, pDef, kindOfMod);
    }

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries)
    {
      if (hasRedirection())
        getRedirection()->onMetafileOrderChanged(viewportId, overlayId, pList, nEntries);
    }
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen)
    {
      if (hasRedirection())
        getRedirection()->onMetafileOrderChanged(viewportId, overlayId, nInvalidFrom, nInvalidLen, pValidFrom, nValidLen);
    }
    // Called when order of metafiles can be inherit from other source
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
    {
      if( hasRedirection() )
        getRedirection()->onMetafileOrderInheritance( viewportId1, overlayId1, viewportId2 );
    }

    // Viewport light lists

    // Called when lights list in viewport is changed.
    virtual void onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                     const OdTrVisLightDef *pList, OdUInt32 nEntries)
    {
      if (hasRedirection())
        getRedirection()->onLightsListChanged(viewportId, ambientLight, bDefaultLightsOn, pList, nEntries);
    }

    // Viewport background

    // Called when background stream in viewport is changed.
    virtual void onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                     OdTrVisProgramId baseProgramId = kTrVisNegativeId)
    {
      if (hasRedirection())
        getRedirection()->onBackgroundChanged(viewportId, pStream, baseProgramId);
    }

    // Viewport extents

    // Called when viewport extents changed
    virtual void onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onExtentsChanged(viewportId, overlayId, pDef);
    }

    // Overlay viewport parameters override

    // Called when viewport needs override view parameters for specific overlay
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef())
    {
      if (hasRedirection())
        getRedirection()->onOverlayViewParamsOverride(viewportId, overlayId, bOverride, pDef);
    }

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
    {
      if (hasRedirection())
        return getRedirection()->onMetafileAdded(metafileId, pDef);
      return metafileId;
    }
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId)
    {
      if (hasRedirection())
        getRedirection()->onMetafileDeleted(metafileId);
    }
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
    //{
    //  if (hasRedirection())
    //    getRedirection()->onMetafileModified(metafileId, pDef);
    //}
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onMetafileVisibilityChanged(metafileId, bVisibility);
    }
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight)
    {
      if (hasRedirection())
        getRedirection()->onMetafileHighlightingChanged(metafileId, bHighlight);
    }
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade)
    {
      if (hasRedirection())
        getRedirection()->onMetafileFadingChanged(metafileId, bFade);
    }
    // Called to attach highlighting branch to metafile.
    virtual void onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId)
    {
      if (hasRedirection())
        getRedirection()->onMetafileHlBranchAttached(metafileId, hlBranchId);
    }
    // Called to detach highlighting branch from metafile.
    virtual void onMetafileHlBranchDetached(OdTrVisMetafileId metafileId)
    {
      if (hasRedirection())
        getRedirection()->onMetafileHlBranchDetached(metafileId);
    }

    // Highlighting

    // Called when new highlighting branch added.
    virtual void onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onHlBranchAdded(hlBranchId, pDef);
    }
    // Called when exist highlighting branch modified.
    virtual void onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                    bool bChildsModified, bool bMarkersModified)
    {
      if (hasRedirection())
        getRedirection()->onHlBranchModified(hlBranchId, pDef, bChildsModified, bMarkersModified);
    }
    // Called when exist highlighting branch deleted.
    virtual void onHlBranchDeleted(OdTrVisHlBranchId hlBranchId)
    {
      if (hasRedirection())
        getRedirection()->onHlBranchDeleted(hlBranchId);
    }

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onTextureAdded(textureId, pDef);
    }
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId)
    {
      if (hasRedirection())
        getRedirection()->onTextureDeleted(textureId);
    }

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onMaterialAdded(materialId, pDef);
    }
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId)
    {
      if (hasRedirection())
        getRedirection()->onMaterialDeleted(materialId);
    }

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
    {
      if (hasRedirection())
        getRedirection()->onVisualStyleAdded(visualStyleId, pDef);
    }
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
    {
      if (hasRedirection())
        getRedirection()->onVisualStyleModified(visualStyleId, pDef);
    }
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId)
    {
      if (hasRedirection())
        getRedirection()->onVisualStyleDeleted(visualStyleId);
    }

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onLayerAdded(layerId, pDef);
    }
    // Called when exist layer modified.
    virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onLayerModified(layerId, pDef);
    }
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId)
    {
      if (hasRedirection())
        getRedirection()->onLayerDeleted(layerId);
    }
    //Called when layer viewport-dependent properties changed
    virtual void onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps = OdTrVisLayerProps() )
    {
      if( hasRedirection())
        getRedirection()->onLayerViewportPropsOverride( viewportId, layerId, bOverride, pProps );
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionRecorder : public OdTrVisRenditionDef, protected OdTrVisRendition::IdRegistrator
{
  public:
    struct CallRecord
    {
      CallRecord *m_pNextRecord;
      CallRecord() : m_pNextRecord(NULL) { }
      virtual ~CallRecord() { }
      virtual void reDrop(OdTrVisRendition *pRendition) = 0;
    };
  protected:
    CallRecord *m_pFirstRecord;
    CallRecord *m_pLastRecord;
  public:
    OdTrVisRenditionRecorder();
    ~OdTrVisRenditionRecorder();

    void appendRecord(CallRecord *pRecord);
    void playRecords(OdTrVisRendition *pRendition, bool bClear = false);
    void clearRecords();
    bool hasRecords() const { return (m_pFirstRecord != NULL); }

    // High-level callbacks

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType & /*type*/) const { return kMtGlobal; }

    // Returns Id registrator, or NULL if no registration supported
    virtual OdTrVisRendition::IdRegistrator *idRegistrator() const;

    // Mid-level callbacks

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef);

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef);
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId);
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility);
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder);

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId /*, const OdTrVisViewportDef &pDef*/);
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId);
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex);
    // Called when viewport already exist in device modified.
    virtual void onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod = kViewportModAll);

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries);
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen);
    // Called when order of metafiles can be inherit from other source
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 );

    // Viewport light lists

    // Called when lights list in viewport is changed.
    virtual void onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                     const OdTrVisLightDef *pList, OdUInt32 nEntries);

    // Viewport background

    // Called when background stream in viewport is changed.
    virtual void onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                     OdTrVisProgramId baseProgramId = kTrVisNegativeId);

    // Viewport extents

    // Called when viewport extents changed
    virtual void onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef);

    // Overlay viewport parameters override

    // Called when viewport needs override view parameters for specific overlay
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef());

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId);
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility);
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight);
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade);
    // Called to attach highlighting branch to metafile.
    virtual void onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId);
    // Called to detach highlighting branch from metafile.
    virtual void onMetafileHlBranchDetached(OdTrVisMetafileId metafileId);

    // Highlighting

    // Called when new highlighting branch added.
    virtual void onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef);
    // Called when exist highlighting branch modified.
    virtual void onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                    bool bChildsModified, bool bMarkersModified);
    // Called when exist highlighting branch deleted.
    virtual void onHlBranchDeleted(OdTrVisHlBranchId hlBranchId);

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef);
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId);

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef);
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId);

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId);

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer modified.
    virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId);
    //Called when layer viewport-dependent properties changed
    virtual void onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps = OdTrVisLayerProps() );
  protected:
    // IdRegistrator overrides
    virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId);
    virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId);
    virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId);
    virtual void kill(DataType type, OdTrVisId id);
    virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionMt : public OdTrVisRenditionRedir
{
  protected:
    struct DataTypeEntry
    {
      MtHandlerType m_handler;
      OdTrVisRendition *m_pRendition;
      OdMutexPtr m_ownMutex;
      OdMutexPtr *m_pMutex;
      DataTypeEntry() : m_handler(kMtSkip), m_pRendition(NULL), m_pMutex(NULL) { }
    } m_dataTypes[kNDataTypes];
    OdMutexPtr m_pGlobalMutex;
    OdMutexPtr m_pRecordMutex;
    OdStaticRxObject<OdTrVisRenditionDef> m_skipRenditionHandler;
    OdStaticRxObject<OdTrVisRenditionRecorder> m_recordRenditionHandler;
    ClientSettings m_cliSets;
  protected:
    mutable struct IdRegistratorMt : public IdRegistrator
    {
      mutable OdMutexPtr m_pMutex[kNDataTypes];
      IdRegistrator *m_pBaseRegistrator;
      IdRegistratorMt() : m_pBaseRegistrator(NULL) { }
      virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId);
      virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId);
      virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId);
      virtual void kill(DataType type, OdTrVisId id);
      virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const;
      void getset(IdRegistrator *pBaseRegistrator) { m_pBaseRegistrator = pBaseRegistrator; }
      IdRegistrator *setget() { return (m_pBaseRegistrator) ? this : NULL; }
    } m_mtRegistrator;
  public:
    OdTrVisRenditionMt();
    ~OdTrVisRenditionMt();

    // Initialize Mt rendition
    void init(OdTrVisRendition *pRendition);
    // Check does rendition is ready for Mt
    bool isMt() const;

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice);
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice);

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const;

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisDisplayId displayId, const OdTrVisFlatMetafileContainer **pStream) const;

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode mode, OdTrVisVisualStyleId vsId = kTrVisNegativeId,
                                            OdTrVisGeomType geomType = OdTrVisGeomType_Default, OdUInt32 shadingFlags = 0) const;

    // Mid-level callbacks

    // Client information

    virtual void obtainClientSettings(ClientSettings &clientSets);

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef);

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef);
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId);
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility);
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder);

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId /*, const OdTrVisViewportDef &pDef*/);
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId);
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex);
    // Called when viewport already exist in device modified.
    virtual void onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod = kViewportModAll);

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries);
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen);
    // Called when order of metafiles can be inherit from other source
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 );

    // Viewport light lists

    // Called when lights list in viewport is changed.
    virtual void onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                     const OdTrVisLightDef *pList, OdUInt32 nEntries);

    // Viewport background

    // Called when background stream in viewport is changed.
    virtual void onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                     OdTrVisProgramId baseProgramId = kTrVisNegativeId);

    // Viewport extents

    // Called when viewport extents changed
    virtual void onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef);

    // Overlay viewport parameters override

    // Called when viewport needs override view parameters for specific overlay
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef());

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId);
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility);
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight);
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade);
    // Called to attach highlighting branch to metafile.
    virtual void onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId);
    // Called to detach highlighting branch from metafile.
    virtual void onMetafileHlBranchDetached(OdTrVisMetafileId metafileId);

    // Highlighting

    // Called when new highlighting branch added.
    virtual void onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef);
    // Called when exist highlighting branch modified.
    virtual void onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                    bool bChildsModified, bool bMarkersModified);
    // Called when exist highlighting branch deleted.
    virtual void onHlBranchDeleted(OdTrVisHlBranchId hlBranchId);

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef);
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId);

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef);
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId);

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId);

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer modified.
    virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId);
    //Called when layer viewport-dependent properties changed
    virtual void onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps = OdTrVisLayerProps() );
};

#include "TD_PackPop.h"

#endif // ODTRVISRENDITION
