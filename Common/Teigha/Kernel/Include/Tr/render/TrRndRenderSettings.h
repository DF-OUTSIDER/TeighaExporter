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
// GLES2 device local rendition render settings interface

#ifndef ODTRRNDRENDERSETTINGS
#define ODTRRNDRENDERSETTINGS

#include "TD_PackPush.h"

#include "TrRndDifferentialTransition.h"
#include "../TrVisDefs.h"
#include "../TrVisMetafileStream.h"

// Forward declarations
class OdGsFiler;
struct OdTrVisViewportDef;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndRenderSettings;

/** \details
    <group ExRender_Windows_Classes> 
*/
typedef OdSharedPtr<OdTrRndRenderSettings> OdTrRndRenderSettingsPtr;

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdStaticDefSharedPtr<OdTrRndRenderSettings> OdTrRndRenderSettingsStaticPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndRenderSettingsReactor
{
  public:
    virtual void renderSettingsChanged(OdTrRndRenderSettings *pRSets) = 0;
};

enum OdTrRndRSUpdateReactor
{
  kRSReactorDefault = -1,
  kRSIgnoreReactor  =  0,
  kRSCallReactor    =  1
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndRenderSettings
{
  protected:
    template <OdUInt32 nElems>
    class BitSet {
      protected:
        OdUInt8 m_flags[nElems];
      public:
        BitSet()
        { clear(); }
        void clear() { for (OdUInt32 n = 0; n < nElems; n++) m_flags[n] = 0; }
        OdUInt8 *getPtr() { return m_flags; } const OdUInt8 *getPtr() const { return m_flags; }
        void set(OdUInt32 nFlag) { SETBIT_1(m_flags[nFlag >> 3], 1 << (nFlag & 7)); }
        void reset(OdUInt32 nFlag) { SETBIT_0(m_flags[nFlag >> 3], 1 << (nFlag & 7)); }
        void set(OdUInt32 nFlag, bool bSet) { SETBIT(m_flags[nFlag >> 3], 1 << (nFlag & 7), bSet); }
        bool get(OdUInt32 nFlag) const { return GETBIT(m_flags[nFlag >> 3], 1 << (nFlag & 7)); }
    };
  public:
    enum DrawFlags
    {
      kNullFlag = 0,
      // Elements rendering
      kDraw3dFacets,                    // Draw shaded facets
      kDraw2dFacets,                    // Draw non-shaded facets
      kDrawRasterImages,                // Draw raster images
      kDraw3dFacetEdges,                // Draw shaded facets wireframe
      kDraw2dFacetEdges,                // Draw non-shaded facets wireframe
      kDrawIsolines,                    // Draw isolines
      kDrawIntersections,               // Draw intersection edges
      kDrawSilhouettes,                 // Draw silhouettes
      kDrawDefaultDisable,              // Disable rendering of default elements
      // Display quality settings
      kDrawOrder,                       // Draw order enabled
      kDrawBackground,                  // Draw backgrounds enabled
      kDrawLineweights,                 // Draw lineweights
      kDrawLighting,                    // Draw lighting
      // Face quality settings
      kDrawMaterials,                   // Draw materials
      kDrawTextures,                    // Draw textures
      kFillFacesColor,                  // Use color to fill faces
      kFillFacesOpacity,                // Override faces opacity
      kFaceNormals,                     // Use face normals instead vertex normals
      kPerVertexLighting,               // Use per-vertex lighting instead of per-pixel lighting
      kGoochLighting,                   // Enable Gooch faces lighting instead of Phong
      kSpecularHighlighting,            // Enable specular highlighting
      kShadingMaterial,                 // Use shading material intead of realistic material
      // Edge quality settings
      kOverrideEdgesColor,              // Use edges color override
      kOverrideEdgesWidth,              // Use edges width override
      kOverrideEdgesLinetype,           // Use edges linetype override
      kOverrideEdgesLinestyle,          // Use edges linestyle override
      kEdgesOpacity,                    // Use edges opacity
      kEdgesCreaseFilter,               // Use edges crease angle filtration
      kEdgesCreaseFilter3d,             // Use separate crease angle filtration for 3d object (active only if kEdgesCreaseFilter enabled)
      kEdgesOverhang,                   // Use edges extension
      kEdgesMultipass,                  // Draw edges multiple times
      kEdgesJitter,                     // Use edges jitter effect
      // FX-related
      kDrawFX,                          // True in case if render settings represent effect rendering
      kDisableColorOutput,              // Disable color output for FX rendering
      kStencilTest,                     // Modifies stencil buffer processing behavior
      kStencil3dTest,                   // Use separate stencil buffer setting for 3d objects (active only if kStencilTest enabled)
      kDepthTest,                       // Modifies depth buffer processing behavior
      kDepth3dTest,                     // Use separate depth buffer setting for 3d objects (active only if kDepthTest enabled)
      // Last flag (do not modify manually)
      kNumFlags,
      kNumFlagRegs = (kNumFlags >> 3) + 1
    };
    // Settings for render modes
#define OdTrRndRS_SetFor2dWireframe                { kDraw3dFacets, kDraw2dFacets, kDrawRasterImages, kDraw2dFacetEdges, kDrawIsolines, \
                                                     kDrawOrder, kDrawBackground, kDrawLineweights, \
                                                     kNullFlag }
#define OdTrRndRS_SetFor3dWireframe                { kDraw2dFacets, kDraw2dFacetEdges, kDrawRasterImages, kDrawIsolines, \
                                                     kDrawBackground, kDrawLineweights, \
                                                     kNullFlag }
#define OdTrRndRS_SetForHiddenLine                 { kDraw3dFacets, kDrawRasterImages, kDraw3dFacetEdges, kDraw2dFacetEdges, \
                                                     kDrawBackground, kDrawLineweights, \
                                                     kFillFacesColor, \
                                                     kNullFlag }
#define OdTrRndRS_SetForFlatShaded                 { kDraw3dFacets, kDraw2dFacets, kDrawRasterImages, \
                                                     kDrawBackground, kDrawLineweights, kDrawLighting, \
                                                     kDrawMaterials, kDrawTextures, kFaceNormals, kPerVertexLighting, kSpecularHighlighting, \
                                                     kNullFlag }
#define OdTrRndRS_SetForGouraudShaded              { kDraw3dFacets, kDraw2dFacets, kDrawRasterImages, \
                                                     kDrawBackground, kDrawLineweights, kDrawLighting, \
                                                     kDrawMaterials, kDrawTextures, kPerVertexLighting, kSpecularHighlighting, \
                                                     kNullFlag }
#define OdTrRndRS_SetForFlatShadedWithWireframe    { kDraw3dFacets, kDraw2dFacets, kDrawRasterImages, kDraw3dFacetEdges, kDraw2dFacetEdges, \
                                                     kDrawBackground, kDrawLineweights, kDrawLighting, \
                                                     kDrawMaterials, kDrawTextures, kFaceNormals, kPerVertexLighting, kSpecularHighlighting, \
                                                     kNullFlag }
#define OdTrRndRS_SetForGouraudShadedWithWireframe { kDraw3dFacets, kDraw2dFacets, kDrawRasterImages, kDraw3dFacetEdges, kDraw2dFacetEdges, \
                                                     kDrawBackground, kDrawLineweights, kDrawLighting, \
                                                     kDrawMaterials, kDrawTextures, kPerVertexLighting, kSpecularHighlighting, \
                                                     kNullFlag }
#define OdTrRndRS_FX                               { kDrawFX, kNullFlag }
    enum ColorType
    {
      kColorNormal = 0,
      kColorBackground,
      kColorForeground,
      kColorTint,
      kColorDesaturate
    };
    struct Color
    {
      ColorType m_colorType;
      ODCOLORREF m_colorVal;
      Color(ColorType colorType = kColorBackground, ODCOLORREF colorVal = ODRGBA(0, 0, 0, 255))
        : m_colorType(colorType), m_colorVal(colorVal) { }
    };
    // Draw properties
    enum ColorProps
    {
      // Viewport (global)
      kPropBackgroundColor = 0,         // Background viewport color
      kPropForegroundColor,             // Foreground viewport color
      // Faces
      kPropFacesFillColor,              // Faces fill color override
      // Edges
      kPropEdgesColor,                  // Edges color override
      // FX-related
      // . . .
      // Number of color properties
      kNumColorProps
    };
    enum FloatProps
    {
      // Faces
      kPropFacesSpecularPower = 0,      // Material specilar power coefficient
      kPropFacesOpacity,                // Faces opacity override
      // Edges
      kPropEdgesCreaseAngle,            // Edge crease filtration angle
      kPropEdgesOpacity,
      // FX-related
      // . . .
      // Number of floating point properties
      kNumFloatProps
    };
    enum IntegerProps
    {
      // Viewport (global)
      kPropDrawFilter = 0,              // Inclusive metafiles rendering filter
      // Faces
      // . . .
      // Edges
      kPropEdgesWidth,                  // Edges lineweight override (in pixels)
      kPropEdgesLinetype,               // Edges linetype override (using OdPs::LineType enum)
      kPropEdgesLinestyle,              // Edges linestyle override (using make_edgesStyle() form)
      kPropEdgesCreaseFilter,           // Edges crease filter (using CreaseFlags enum)
      kPropEdgesCreaseFilter3d,         // Separate edges crease filter for 3d objects (using CreaseFlags enum)
      kPropEdgesOverhang,               // Edges extension length (in pixels)
      kPropEdgesRepetition,             // Number of repetitions per edges rendering (applicable for jitter effect only)
      kPropEdgesJitterAmount,           // Jitter effect amount (in pixels)
      // FX-related
      kPropStencilFlags,                // Stencil buffer flags (using StencilFlags enum)
      kPropStencil3dFlags,              // Separate stencil buffer flags for 3d objects (using StencilFlags enum)
      kPropDepthFlags,                  // Depth buffer flags (using DepthFlags enum)
      kPropDepth3dFlags,                // Separate depth buffer flags for 3d objects (using DepthFlags enum)
      // Number of integer properties
      kNumIntegerProps
    };
    // Type of multi-pass effect
    enum MultiPassType
    {
      kMultiPassByObject = 0,     // Multi-pass rendering of separate object
      kMultiPassByScene,          // Multi-pass rendering of entire scene
      kMultiPassInputFX,          // Input effect for current rendering stage
      kMultiPassOutputFX,         // Output effect for current rendering stage

      // !!! To add multi-pass support for additional geometry types modify geomTypeMultiPass() method implementation also.
      //kMultiPassFor3dFacets,      // Multi-pass rendering of shaded facets               // kDraw3dFacets
      //kMultiPassFor2dFacets,      // Multi-pass rendering of non-shaded facets           // kDraw2dFacets
      //kMultiPassForRasterImages,  // Multi-pass rendering of raster images               // kDrawRasterImages
      //kMultiPassFor3dFacetEdges,  // Multi-pass rendering of shaded facets wireframe     // kDraw2dFacetEdges
      //kMultiPassFor2dFacetEdges,  // Multi-pass rendering of non-shaded facets wireframe // kDraw2dFacetEdges
      //kMultiPassForIsolines,      // Multi-pass rendering of isolines                    // kDrawIsolines
      kMultiPassForIntersections, // Multi-pass rendering of intersection edges          // kDrawIntersections
      //kMultiPassForSilhouettes,   // Multi-pass rendering of silhouettes                 // kDrawSilhouettes

      kNumMultiPassTypes
    };
    // kPropStencilFlags property settings
    enum StencilFlags
    {
      kStencilWriteBit1 = (1 << 0), kStencilWriteBit2 = (1 << 1), kStencilWriteBit3 = (1 << 2), kStencilWriteBit4 = (1 << 3),
      kStencilWriteMask = kStencilWriteBit1 | kStencilWriteBit2 | kStencilWriteBit3 | kStencilWriteBit4, kStencilWriteOff  = 0,
      kStencilTestBit1  = (1 << 4), kStencilTestBit2  = (1 << 5), kStencilTestBit3  = (1 << 6), kStencilTestBit4  = (1 << 7),
      kStencilTestMask  = kStencilTestBit1  | kStencilTestBit2  | kStencilTestBit3  | kStencilTestBit4,  kStencilTestOff   = 4,
      kStencilReadBit1  = (1 << 8), kStencilReadBit2  = (1 << 9), kStencilReadBit3 = (1 << 10), kStencilReadBit4 = (1 << 11),
      kStencilReadMask  = kStencilReadBit1  | kStencilReadBit2  | kStencilReadBit3  | kStencilReadBit4,  kStencilReadOff   = 8
    };
    // kPropDepthFlags property settings
    enum DepthFlags
    {
      kDepthNever = 1, kDepthLess = 2, kDepthEqual = 3, kDepthLEqual = 4, kDepthGreater = 5,
      kDepthNEqual = 6, kDepthGEqual = 7, kDepthAlways = 8, kDepthFuncMask = 0x0F,
      kDepthReadOnly = (1 << 4)
    };
    // kPropEdgesCreaseFilter property settings
    enum CreaseFlags
    {
      // Most reasonable states:
      kCreaseFrontEdges   = -2, // Completely front edge
      kCreaseFrontCorners = -1, // Corner edge at front
      kCreaseBreakEdges   =  0, // Break edge (partially at front, partially at back)
      kCreaseBackCorners  =  1, // Corner edge at back
      kCreaseBackEdges    =  2  // Completely back edge
    };
    enum CreaseFilterDefault
    {
      kCreaseDrawAllEdges              = 0x32, // -2, 2 - Draw everything (default)
      kCreaseDrawFrontEdgesOnly        = 0x35, // -2,-1 - Draw front edges only
      kCreaseDrawFrontEdgesWithCorners = 0x30, // -2, 0 - Draw front edges and front corner edges
      kCreaseDrawBreakEdgesWithCorners = 0x29, // -1, 1 - Draw break edges and corner edges
      kCreaseDrawBreakEdgesOnly        = 0x00, //  0, 0 - Draw break edges only
      kCreaseDrawBackEdgesWithCorners  = 0x02, //  0, 2 - Draw back edges and back corner edges
      kCreaseDrawBackEdgesOnly         = 0x0A  //  1, 2 - Draw back edges only
    };
    inline static OdUInt32 creaseFlagEncode(CreaseFlags flags)
    { return (flags < kCreaseBreakEdges) ? OdUInt32(-flags | 4) : OdUInt32(flags); }
    inline static CreaseFlags creaseFlagDecode(OdUInt32 flags)
    { return (CreaseFlags)((GETBIT(flags, 4)) ? -OdInt32(flags & 3) : OdInt32(flags & 3)); }
    inline static OdUInt32 make_creaseFilter(CreaseFlags frontLimit, CreaseFlags backLimit)
    { return (creaseFlagEncode(frontLimit) << 3) | creaseFlagEncode(backLimit); }
    inline static OdUInt32 make_creaseFilter(CreaseFilterDefault defFilter) { return (OdUInt32)defFilter; }
    inline static CreaseFlags get_creaseFilterFront(OdUInt32 creaseFilter) { return creaseFlagDecode((creaseFilter >> 3) & 7); }
    inline static CreaseFlags get_creaseFilterBack(OdUInt32 creaseFilter) { return creaseFlagDecode(creaseFilter & 7); }
    // kPropEdgesLinestyle property setting
    inline static OdUInt32 make_edgesStyle(OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle)
    { return OdUInt32(lesStyle) | (OdUInt32(ljsStyle) << 8); }
    inline static OdPs::LineEndStyle get_lineEndStyle(OdUInt32 lineStyle) { return (OdPs::LineEndStyle)(lineStyle & 0xFF); }
    inline static OdPs::LineJoinStyle get_lineJoinStyle(OdUInt32 lineStyle) { return (OdPs::LineJoinStyle)((lineStyle >> 8) & 0xFF); }
    // UI properties grouping
    enum PropType
    {
      kPropTypeGroup = -1,
      kPropTypeDrawFlag = 0,
      kPropTypeColor,
      kPropTypeFloat,
      kPropTypeInt,
      kPropTypeUInt
    };
    union UIDefVal
    {
      bool m_drawFlag;
      struct ColorProp
      {
        ColorType m_colorType;
        ODCOLORREF m_colorVal;
        void reset() { m_colorType = kColorNormal; m_colorVal = 0; }
      } m_colorProp;
      float m_floatProp;
      OdInt32 m_intProp;
      OdUInt32 m_uintProp;
      UIDefVal() { m_colorProp.reset(); }
      UIDefVal(bool bDrawFlag) { m_colorProp.reset(); m_drawFlag = bDrawFlag; }
      UIDefVal(ColorType colorType, ODCOLORREF colorVal = ODRGBA(0, 0, 0, 255)) { m_colorProp.m_colorType = colorType; m_colorProp.m_colorVal = colorVal; }
      UIDefVal(float floatProp) { m_colorProp.reset(); m_floatProp = floatProp; }
      UIDefVal(OdInt32 intProp) { m_colorProp.reset(); m_intProp = intProp; }
      UIDefVal(OdUInt32 uintProp) { m_colorProp.reset(); m_uintProp = uintProp; }
    };
    union UIPropRange
    {
      float m_limFlt;
      OdInt32 m_limInt;
      OdUInt32 m_limUInt;
      UIPropRange() { m_limFlt = 0.0f; }
    };
    struct UIPropRanges
    {
      UIPropRange m_min;
      UIPropRange m_max;
      UIPropRanges() {}
      UIPropRanges(float fltMin, float fltMax)     { m_min.m_limFlt = fltMin; m_max.m_limFlt = fltMax; }
      UIPropRanges(OdInt32 intMin, OdInt32 intMax) { m_min.m_limInt = intMin; m_max.m_limInt = intMax; }
      UIPropRanges(OdUInt32 uintMin, OdUInt32 uintMax) { m_min.m_limUInt = uintMin; m_max.m_limUInt = uintMax; }
    };
    struct UIPropEntry
    {
      PropType m_propType;
      OdUInt32 m_nPropCode;
      OdUInt32 m_nLinkUIId1;
      OdUInt32 m_nLinkUIId2;
      const OdChar *m_pName;
      UIDefVal m_defaultVal;
      UIPropRanges m_ranges;
    };
  protected:
    enum RSFlags
    {
      kRenderModeOverride = (1 << 0), // If set to true render mode settings will be got directly from viewport.
      kSuppressReactor    = (1 << 1), // If set then reactor willn't be called by default for this render settings object.
      kRSFlagsCopyMask    = kRenderModeOverride // Flags which can be copied between render settings.
    };
  protected:
    OdUInt16 m_rsFlags;                   // Render settings specific flags
    OdGsView::RenderMode m_renderMode;    // Used to set basic parameters.
    BitSet<kNumFlagRegs> m_nDrawFlags;    // Set of bit (draw) flags
    Color m_colorProps[kNumColorProps];   // Set of color data properties
    float m_floatProps[kNumFloatProps];   // Set of floating point data properties
    OdInt32 m_intProps[kNumIntegerProps]; // Set of integer properties
    // Pointer to next pass for multipass render settings
    OdTrRndRenderSettingsPtr m_pNextPass[kNumMultiPassTypes];
    // Differential transition from previous pass
    OdTrRndDifferentialTransitionPtr m_pDifTrans[kNumMultiPassTypes];
    // Attached render setting reactors
    OdArray<OdTrRndRenderSettingsReactor*, OdMemoryAllocator<OdTrRndRenderSettingsReactor*> > m_reactors;
  public:
    OdTrRndRenderSettings(OdGsView::RenderMode initialRenderMode = OdGsView::k2DOptimized, OdTrRndRSUpdateReactor rsDefault = kRSCallReactor)
      : m_rsFlags(kRenderModeOverride)
      , m_renderMode(initialRenderMode)
      , m_nDrawFlags()
    {
      setReactorDefault(rsDefault);
      setForRenderMode(initialRenderMode, kRSIgnoreReactor);
      setColor(kPropBackgroundColor, Color(kColorNormal, ODRGBA(0, 0, 0, 255)), kRSIgnoreReactor);
      setColor(kPropForegroundColor, Color(kColorNormal, ODRGBA(255, 255, 255, 255)), kRSIgnoreReactor);
    }
    OdTrRndRenderSettings(const OdTrRndRenderSettings &rs)
    {
      *this = rs;
    }
    ~OdTrRndRenderSettings()
    {
    }

    OdTrRndRenderSettings &operator =(const OdTrRndRenderSettings &rs);

    // Control rendering settings overriding
    bool isRenderModeOverride() const
    {
      return GETBIT(m_rsFlags, kRenderModeOverride);
    }
    void setRenderModeOverride(bool bSet, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      SETBIT(m_rsFlags, kRenderModeOverride, bSet);
      react(rsReact);
    }

    // Basic rendering mode
    OdGsView::RenderMode renderMode() const
    {
      return m_renderMode;
    }
    void setRenderMode(OdGsView::RenderMode rMode, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_renderMode = rMode;
      react(rsReact);
    }

    // Draw flags accessors
    bool drawFlag(OdUInt32 nFlag) const
    {
      return m_nDrawFlags.get(nFlag);
    }
    void setDrawFlag(OdUInt32 nFlag, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_nDrawFlags.set(nFlag);
      react(rsReact);
    }
    void resetDrawFlag(OdUInt32 nFlag, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_nDrawFlags.reset(nFlag);
      react(rsReact);
    }
    void setDrawFlag(OdUInt32 nFlag, bool bSet, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_nDrawFlags.set(nFlag, bSet);
      react(rsReact);
    }

    // Draw flags array accessors
    void setDrawFlags(const OdUInt32 *pFlags, OdUInt32 nFlags = kNumFlags, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      while (nFlags && *pFlags)
        setDrawFlag(*pFlags++, kRSIgnoreReactor), nFlags--;
      react(rsReact);
    }
    void resetDrawFlags(const OdUInt32 *pFlags, OdUInt32 nFlags = kNumFlags, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      while (nFlags && *pFlags)
        resetDrawFlag(*pFlags++, kRSIgnoreReactor), nFlags--;
      react(rsReact);
    }

    // Clear flags
    void clearDrawFlags(OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_nDrawFlags.clear();
      react(rsReact);
    }

    // Draw flags serialization support
    OdUInt8 *serializationFlagsPtr()
    {
      return m_nDrawFlags.getPtr();
    }
    const OdUInt8 *serializationFlagsPtr() const
    {
      return m_nDrawFlags.getPtr();
    }

    // Access color properties
    const Color &color(ColorProps nProp, bool bResolved = true) const
    {
      return (!bResolved) ? m_colorProps[nProp] : resolveColor(m_colorProps[nProp]);
    }
    void setColor(ColorProps nProp, const Color &clr, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_colorProps[nProp] = clr;
      react(rsReact);
    }

    // Access floating point properties
    float value(FloatProps nProp) const
    {
      return m_floatProps[nProp];
    }
    void setValue(FloatProps nProp, float fVal, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_floatProps[nProp] = fVal;
      react(rsReact);
    }

    // Access integer properties
    OdInt32 value(IntegerProps nProp) const
    {
      return m_intProps[nProp];
    }
    OdUInt32 valueU(IntegerProps nProp) const
    {
      return (OdUInt32)m_intProps[nProp];
    }
    void setValue(IntegerProps nProp, OdInt32 nVal, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_intProps[nProp] = nVal;
      react(rsReact);
    }
    void setValueU(IntegerProps nProp, OdUInt32 nVal, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      m_intProps[nProp] = (OdInt32)nVal;
      react(rsReact);
    }

    // Multipass render settings
    void setNextPass(MultiPassType passType, OdTrRndRenderSettings *pNextPass, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      if (m_pNextPass[passType] != pNextPass)
      {
        m_pNextPass[passType] = pNextPass;
        m_pDifTrans[passType] = NULL;
        react(rsReact);
      }
    }
    void setNextPassCopy(MultiPassType passType, const OdTrRndRenderSettings *pNextPass, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault)
    {
      if (!pNextPass)
        m_pNextPass[passType] = NULL;
      else
        m_pNextPass[passType] = new OdTrRndRenderSettings(*pNextPass);
      m_pDifTrans[passType] = NULL;
      react(rsReact);
    }
    void setLastPass(MultiPassType passType, OdTrRndRenderSettings *pNextPass, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault);
    void setLastPassCopy(MultiPassType passType, const OdTrRndRenderSettings *pNextPass, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault);
    bool hasNextPass(MultiPassType passType) const
    {
      return !m_pNextPass[passType].isNull();
    }
    OdTrRndRenderSettings *nextPass(MultiPassType passType)
    {
      return m_pNextPass[passType].get();
    }
    const OdTrRndRenderSettings *nextPass(MultiPassType passType) const
    {
      return m_pNextPass[passType].get();
    }
    OdTrRndRenderSettingsPtr &nextPassPtr(MultiPassType passType)
    {
      return m_pNextPass[passType];
    }
    const OdTrRndRenderSettingsPtr &nextPassPtr(MultiPassType passType) const
    {
      return m_pNextPass[passType];
    }

    void setDifferentialTransition(MultiPassType passType, const OdTrRndDifferentialTransitionPtr &pDifTrans)
    {
      m_pDifTrans[passType] = pDifTrans;
    }
    const OdTrRndDifferentialTransitionPtr &differentialTransition(MultiPassType passType) const
    {
      return m_pDifTrans[passType];
    }
    void resetDifferentialTransition(MultiPassType passType)
    {
      m_pDifTrans[passType] = NULL;
    }

    // Multipass support for geometry types
    MultiPassType geomTypeMultiPass(OdTrVisGeomType geomType) const;

    bool hasNextPass(OdTrVisGeomType geomType) const
    { const MultiPassType passType = geomTypeMultiPass(geomType);
      if (passType >= kNumMultiPassTypes) return false;
      return hasNextPass(passType);
    }
    OdTrRndRenderSettings *nextPass(OdTrVisGeomType geomType)
    { const MultiPassType passType = geomTypeMultiPass(geomType);
      if (passType >= kNumMultiPassTypes) return NULL;
      return nextPass(passType);
    }
    const OdTrRndRenderSettings *nextPass(OdTrVisGeomType geomType) const
    { const MultiPassType passType = geomTypeMultiPass(geomType);
      if (passType >= kNumMultiPassTypes) return NULL;
      return nextPass(passType);
    }
    OdTrRndRenderSettingsPtr nextPassPtr(OdTrVisGeomType geomType)
    { const MultiPassType passType = geomTypeMultiPass(geomType);
      if (passType >= kNumMultiPassTypes) return OdTrRndRenderSettingsPtr();
      return nextPassPtr(passType);
    }
    const OdTrRndRenderSettingsPtr nextPassPtr(OdTrVisGeomType geomType) const
    { const MultiPassType passType = geomTypeMultiPass(geomType);
      if (passType >= kNumMultiPassTypes) return OdTrRndRenderSettingsPtr();
      return nextPassPtr(passType);
    }

    // Render settings reactors
    void addReactor(OdTrRndRenderSettingsReactor* pReactor);
    void removeReactor(OdTrRndRenderSettingsReactor* pReactor);
    bool hasReactor(OdTrRndRenderSettingsReactor* pReactor) const;

    // Reactor default behavior
    void setReactorDefault(OdTrRndRSUpdateReactor rsDefault);
    OdTrRndRSUpdateReactor reactorDefault() const;
    void forceReactorUpdate(OdTrRndRSUpdateReactor rsReact = kRSCallReactor)
    {
      react(rsReact);
    }

    // Extended functions
    void setForRenderMode(OdGsView::RenderMode rm, OdTrRndRSUpdateReactor rsReact = kRSReactorDefault);
    void setPropsDefault(OdTrRndRSUpdateReactor rsReact = kRSReactorDefault);
    void resolveByViewportDef(const OdTrVisViewportDef &vpDef);
    bool isGeomTypeVisible(OdTrVisGeomType geomType) const;
    OdUInt32 makeGeomMarkerVisibilities() const;

    // Some basic properties access
    // !RenderMode::depthBufferEnabled
    bool is2dModeEnabled() const
    {
      return drawFlag(kDrawOrder);
    }
    // RenderMode::stencilBufferEnabled
    bool isFaceColorOverrideEnabled() const
    {
      return drawFlag(kFillFacesColor);
    }
    // RenderMode::polygonOffsetEnabled
    bool is3dFacesWithLinesEnabled() const
    {
      return !drawFlag(kDrawOrder) && ((drawFlag(kDraw3dFacets) && (drawFlag(kDraw3dFacetEdges) || drawFlag(kDrawIsolines))) || drawFlag(kDrawIntersections));
    }
    // RenderMode::lightingEnabled
    bool isLightingEnabled_() const
    {
      return drawFlag(kDraw3dFacets) && !drawFlag(kDrawOrder) && !drawFlag(kFillFacesColor);
    }
    bool isLightingEnabled() const
    {
      return drawFlag(kDrawLighting);
    }
    // RenderMode::faceNormalsEnabled
    bool isFaceNormalsEnabled() const
    {
      return drawFlag(kFaceNormals);
    }
    // RenderConfig::isForceDefaultLighting
    bool isDefaultLighting() const
    {
      return drawFlag(kGoochLighting);
    }
    // RenderConfig::isHardwareColorModifiersEnabled
    bool isColorModifiersEnabled() const
    {
      return (drawFlag(kDraw3dFacets)     && (drawFlag(kFillFacesColor)     ||
                                              drawFlag(kFillFacesOpacity))) ||
            ((drawFlag(kDraw3dFacetEdges) ||
              drawFlag(kDrawIsolines))    && (drawFlag(kOverrideEdgesColor) ||
                                              drawFlag(kEdgesOpacity)));
    }

    // Loading/saving ability
    bool save(OdGsFiler *pFiler) const;
    bool load(OdGsFiler *pFiler);

    // Extended properties information
    static OdUInt32 extendedPropsInfo(const UIPropEntry **ppEntries = NULL);
  protected:
    bool loadNextPass(MultiPassType passType, OdGsFiler *pFiler);
    const Color &resolveColor(const Color &clr) const;
    void reactImpl();
    void react(OdTrRndRSUpdateReactor rsReact)
    {
      if ((rsReact == kRSCallReactor) || ((rsReact == kRSReactorDefault) && !GETBIT(m_rsFlags, kSuppressReactor)))
        reactImpl();
    }
};

// Null render settings (applicable as stub for initial render settings switch)
extern OdTrRndRenderSettingsStaticPtr kTrRndNullRenderSettings;

inline
bool OdTrRndRenderSettings::isGeomTypeVisible(OdTrVisGeomType geomType) const
{
  switch (geomType)
  {
    case OdTrVisGeomType_Default:           return !drawFlag(OdTrRndRenderSettings::kDrawDefaultDisable);
    case OdTrVisGeomType_2dFacets:          return drawFlag(OdTrRndRenderSettings::kDraw2dFacets);
    case OdTrVisGeomType_2dFacetsNoFill:    return drawFlag(OdTrRndRenderSettings::kDraw2dFacets) && !drawFlag(OdTrRndRenderSettings::kDrawOrder);
    case OdTrVisGeomType_3dFacets:          return drawFlag(OdTrRndRenderSettings::kDraw3dFacets);
    case OdTrVisGeomType_3dFacetsNoFill:    return drawFlag(OdTrRndRenderSettings::kDraw3dFacets) && !drawFlag(OdTrRndRenderSettings::kDrawOrder);
    case OdTrVisGeomType_RasterImageFacets: return drawFlag(OdTrRndRenderSettings::kDrawRasterImages);
    case OdTrVisGeomType_2dFacetEdges:      return drawFlag(OdTrRndRenderSettings::kDraw2dFacetEdges);
    case OdTrVisGeomType_3dFacetEdges:      return drawFlag(OdTrRndRenderSettings::kDraw3dFacetEdges);
    case OdTrVisGeomType_Isolines:          return drawFlag(OdTrRndRenderSettings::kDrawIsolines);
    case OdTrVisGeomType_EdgesWithIsolines: return drawFlag(OdTrRndRenderSettings::kDraw3dFacetEdges) || drawFlag(OdTrRndRenderSettings::kDrawIsolines);
    case OdTrVisGeomType_HatchIsolineEdges: return !drawFlag(OdTrRndRenderSettings::kDrawDefaultDisable) && (!drawFlag(OdTrRndRenderSettings::kDraw3dFacets) ||
                                            isLightingEnabled() || !drawFlag(OdTrRndRenderSettings::kFillFacesColor));
    case OdTrVisGeomType_FillPatternEdges:  return drawFlag(OdTrRndRenderSettings::kDraw3dFacets) && drawFlag(OdTrRndRenderSettings::kShadingMaterial) &&
                                                   drawFlag(OdTrRndRenderSettings::kDraw3dFacetEdges);
    case OdTrVisGeomType_IntersectionEdges: return drawFlag(OdTrRndRenderSettings::kDrawIntersections);
    case OdTrVisGeomType_Silhouettes:       return drawFlag(OdTrRndRenderSettings::kDrawSilhouettes);
    default:                                return false;
  }
}

inline
OdTrRndRenderSettings::MultiPassType OdTrRndRenderSettings::geomTypeMultiPass(OdTrVisGeomType geomType) const
{
  switch (geomType)
  {
    //case OdTrVisGeomType_2dFacets:          case OdTrVisGeomType_2dFacetsNoFill:    return kMultiPassFor2dFacets;
    //case OdTrVisGeomType_3dFacets:          case OdTrVisGeomType_3dFacetsNoFill:    return kMultiPassFor3dFacets;
    //case OdTrVisGeomType_RasterImageFacets:                                         return kMultiPassForRasterImages;
    //case OdTrVisGeomType_2dFacetEdges:                                              return kMultiPassFor2dFacetEdges;
    //case OdTrVisGeomType_3dFacetEdges:      case OdTrVisGeomType_EdgesWithIsolines: return kMultiPassFor3dFacetEdges;
    //case OdTrVisGeomType_Isolines:                                                  return kMultiPassForIsolines;
    case OdTrVisGeomType_IntersectionEdges:                                         return kMultiPassForIntersections;
    //case OdTrVisGeomType_Silhouettes:                                               return kMultiPassForSilhouettes;
    default:                                                                        return kNumMultiPassTypes;
  }
}

inline
const OdTrRndRenderSettings::Color &OdTrRndRenderSettings::resolveColor(const Color &clr) const
{
  switch (clr.m_colorType)
  {
    case kColorBackground: return color(kPropBackgroundColor, false);
    case kColorForeground: return color(kPropForegroundColor, false);
    default: break;
  }
  return clr;
}

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndRenderSettingsManager
{
  public:
    enum RSViewportFlags
    {
      kRSViewportVisible = (1 << 0),
      kRSViewportPS      = (1 << 1),
      kRSViewportOverall = (1 << 2),
      kRSViewportHelper  = (1 << 3)
    };
  public:
    OdTrRndRenderSettingsManager() { }
    virtual ~OdTrRndRenderSettingsManager() { }

    virtual OdUInt32 rsNumViewports() const = 0;

    virtual OdTrVisViewportId rsViewportId(OdUInt32 numViewport) const = 0;
    virtual OdUInt32 rsViewportFlags(OdUInt32 numViewport) const = 0;

    virtual void rsViewportSettings(OdUInt32 numViewport, OdTrRndRenderSettingsPtr &rs) const = 0;
    virtual OdTrRndRenderSettings &rsViewportSettings(OdUInt32 numViewport) = 0;

    virtual bool rsIsViewportRenderModeOverride(OdUInt32 numViewport) const = 0;
    virtual void rsSetViewportRenderModeOverride(OdUInt32 numViewport, bool bOverride) = 0;
    virtual void rsApplyViewportRenderModeOverride(OdUInt32 numViewport) = 0;
};

#include "TD_PackPop.h"

#endif // ODTRRNDRENDERSETTINGS
