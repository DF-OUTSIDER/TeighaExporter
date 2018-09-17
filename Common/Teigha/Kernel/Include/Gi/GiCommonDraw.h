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

#ifndef __ODGICOMMONDRAW_H__
#define __ODGICOMMONDRAW_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "Gi/Gi.h"

class OdGiGeometry;
class OdGiContext;
class OdGiSubEntityTraits;
class OdDbStub;
typedef OdRxObject OdDbBaseDatabase;
class OdGePoint3d;
class OdGiDrawable;
class OdGiPathNode;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiDrawable object pointers.
*/
typedef OdSmartPtr<OdGiDrawable> OdGiDrawablePtr;

/** \details
    Viewport regeneration modes. 
*/
typedef enum
{
  eOdGiRegenTypeInvalid         = 0,  // Invalid regen type. 
  kOdGiStandardDisplay          = 2,  // Regenerating for 2D wireframe display. 
  kOdGiHideOrShadeCommand       = 3,  // Regenerating for legacy hide or shade commands. 
  kOdGiRenderCommand            = 4,  // Regenerating for rendered display.
  kOdGiForExplode               = 5,  // Regenerating for explode.  
  kOdGiSaveWorldDrawForProxy    = 6,  // Regenerating for proxy. 
  kOdGiForExtents               = 7   // Regenerating for extents. 
} OdGiRegenType;

/** \details
    Deviation types used for tessellation.
*/
typedef enum
{
  kOdGiMaxDevForCircle      = 0,  // Set maximum deviation for circles.
  kOdGiMaxDevForCurve       = 1,  // Set maximum deviation for curves.
  kOdGiMaxDevForBoundary    = 2,  // Set maximum deviation for boundaries.
  kOdGiMaxDevForIsoline     = 3,  // Set maximum deviation for isolines.
  kOdGiMaxDevForFacet       = 4   // Set maximum deviation for facets.
} OdGiDeviationType;

class OdGiCommonDraw;
class OdGiTextStyle;
class OdPsPlotStyleData;
class OdGiConveyorGeometry;

/** \details
    Text vectorization flags.
*/
enum
{
  kOdGiIncludeScores    = 2,  // Include scores during text vectorization.
  kOdGiRawText          = 4,  // Vectorize text in one raw.
  kOdGiIncludePenups    = 8,  // Include penups during text vectorization.
  kOdGiDrawShape        = 16, // Draw shapes during text vectorization.
  kOdGiIgnoreMIF        = 32,  // Ignore MIF during text vectorization.
  kOdGiLastPosOnly      = 64   // Calculate last position only (for internal use only)
};

class OdGiSectionGeometryManager;
typedef OdSmartPtr<OdGiSectionGeometryManager> OdGiSectionGeometryManagerPtr;

/** \details
    This class defines common operations and properties that are used in the
    Teigha vectorization process.
    
    \remarks
    An instance of an OdGiContext subclass is normally created as a preliminary step in the vectorization process, which is to be
    used throughout the vectorization.

    Most of the virtual functions in this class (the ones that are not pure virtual) are no-ops, serving only to define an interface.

    Corresponding C++ library: TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiContext : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiContext);

  /** \details
    Returns the database that is currently being vectorized.
  */
  virtual OdDbBaseDatabase* database() const = 0;

  /** \details
    Opens for reading the specified drawable object that belongs to the database associated with this Context object.
    
    \param drawableId [in]  Object ID of the drawable to be opened.
    
    \returns
    Returns a SmartPointer to the drawable object.
  */
  virtual OdGiDrawablePtr openDrawable(
    OdDbStub* drawableId) = 0;

  /** \details
      Returns the default lineweight associated with this Context object.
  */
  virtual OdDb::LineWeight defaultLineWeight() const;

  /** \details
    Returns the common linetype scale for this Context object.
  */
  virtual double commonLinetypeScale() const;

  /** \details
    Returns the default text style associated with this Context object.
    
    \param textStyle [out]  Receives the TextStyle object.
  */
  virtual void getDefaultTextStyle(
    OdGiTextStyle& textStyle);

  /** \details
    Vectorizes the specified shape to the specified interface object.

    \param pDraw [in]  Pointer to the CommonDraw object.
    \param position [in]  Position of the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
  */
  virtual void drawShape(
    OdGiCommonDraw* pDraw, 
    OdGePoint3d& position, 
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle);

  /** \param pDest [in]  Pointer to the Conveyor object.
      \param direction [in]  Baseline direction for the text.
      \param upVector [in]  Up vector for the text.
      \param pExtrusion [in]  Pointer to the Extrusion vector for the text.
  */
  virtual void drawShape(
    OdGiConveyorGeometry* pDest,
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion);

  /** \details
    Vectorizes the specified text string to the supplied CommonDraw object.

    \param pDraw [in]  Pointer to the CommonDraw object.
    \param position [in]  Position of the text.
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg (not including the optional null byte).
    \param pTextStyle [in]  Pointer to the TextStyle for msg.
    \param flags [in]  Vectorization flags.
    
    \remarks
    msg must be null terminated if numBytes is not supplied.
  */
  virtual void drawText(
    OdGiCommonDraw* pDraw, 
    OdGePoint3d& position,
    const OdChar* msg, 
    OdInt32 numBytes,
    const OdGiTextStyle* pTextStyle, 
    OdUInt32 flags = 0);

  /** \param height [in]  Height of the text.
    \param width [in]  Width of the text.
    \param oblique [in]  Oblique angle of the text.
      
    \remarks
    All angles are expressed in radians.
    
    As currently implemented, this function ignores width and oblique.
    They will be fully implemented in a future release.
  */
  virtual void drawText(
    OdGiCommonDraw* pDraw, 
    OdGePoint3d& position,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg);

  /** \param pDest [in]  Pointer to the Conveyor object.
      \param direction [in]  Baseline direction for the text.
      \param upVector [in]  Up vector for the text.
      \param pExtrusion [in]  Pointer to the extrusion vector for the text.
      \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
  */
  virtual void drawText(
    OdGiConveyorGeometry* pDest,
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw,
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion);

  /** \details
    Returns the extents box for the specified text.
       
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg (not including the optional null byte).
    \param textStyle [in]  TextStyle for msg.
    \param flags [in]  Vectorization flags.
    \param min [out]  Receives the lower-left corner of the extents box.
    \param max [out]  Receives the upper-right corner of the extents box.
    \param pEndPos [out]  If non-NULL, receives the end position of the text string.

  */
  virtual void textExtentsBox(
    const OdGiTextStyle& textStyle, 
    const OdChar* msg, 
    int nLength,
    OdUInt32 flags, 
    OdGePoint3d& min, 
    OdGePoint3d& max, 
    OdGePoint3d* pEndPos = 0);

  /** \details
    Returns the extents box for the specified shape.
    
    \param textStyle [in]  TextStyle for the shape.
    \param shapeNumber [in]  Shape number.
    \param min [out]  Receives the lower-left corner of the extents box.
    \param max [out]  Receives the upper-right corner of the extents box.
  */
  virtual void shapeExtentsBox(
    const OdGiTextStyle& textStyle, 
    int shapeNumber, 
    OdGePoint3d& min, 
    OdGePoint3d& max);

  /** \details
    Returns the circle zoom percent for this vectorization process.
    
    \param viewportId [in]  Pointer to the Object ID of the Viewport object to be queried.
    
    \returns
    Returns a value in the range [1,20000]. 100 is the default.
  */
  virtual unsigned int circleZoomPercent(
    OdDbStub* viewportId) const;

  /** \details
    Returns true if and only if this vectorization is intended for hard copy output.
  */
  virtual bool isPlotGeneration() const;

  /** \details
    Returns the palette background color.
  */
  virtual ODCOLORREF paletteBackground() const;

  /** \details
    Returns true if and only if TrueType text should be filled during this vectorization.
  */
  virtual bool fillTtf() const;

  /** \details
    Returns the number of isolines to be drawn on surfaces during this vectorization.
  */
  virtual OdUInt32 numberOfIsolines() const;

  /** \details
    Returns true if and only if shell/mesh geometry primitives should be filled during this vectorization.
  */
  virtual bool fillMode() const;

  /** \details
    Returns true if and only if quick text mode is enabled for this vectorization process.
  */
  virtual bool quickTextMode() const;

  /** \details
    Returns the text quality percent for this vectorization process.

    \returns
    Returns a value in the range [0,100]. 50 is the default.
  */
  virtual OdUInt32 textQuality() const;

  /** \details
    Image quality types.
  */
  enum ImageQuality
  {
    kImageQualityDraft = 0,  // Draft quality.
    kImageQualityHigh  = 1   // High quality.
  };

  /** \details
    Returns the image quality for this vectorization process.
  */
  virtual ImageQuality imageQuality() const;

  /** \details
    Fading types.
  */
  enum FadingType
  {
    kLockedLayerFade = 0,  // Fading objects on locked layers.
    kXrefFade        = 1,  // Fading xref objects.
    kRefEditFade     = 2   // Fading references being edited.
  };

  /** \details
    Fading intensity percentage.
  */
  virtual OdUInt32 fadingIntensityPercentage(FadingType fadingType) const;

  /** \details
    Glyph types.
  */
  enum GlyphType
  {
    kLightGlyph  = 0,  // Light glyph.
    kCameraGlyph = 1   // Camera glyph.
  };

  /** \details
    Returns the glyph size for the specified glyph type.

    \param glyphType [in]  Type of glyph to return glyph size for.

    \returns
    Returns a value in the range [0,100].

    The default return values are the following:
    <table>
    Name         Value
    kLightGlyph  0
    kCameraGlyph 50
    </table>
  */
  virtual OdUInt32 glyphSize(GlyphType glyphType) const;
  
  /** \details
    Lineweight styles.
  */
  enum LineWeightStyle
  {
    kPointLineWeight = 0,  // Point lineweight style.
    kLineCapStyle    = 1,  // Line cap style.
    kLineJoinStyle   = 2   // Line join style.
  };

  /** \details
    Returns the lineweight display style configuration.

    \param styleEntry [in]  Lineweight style entry for which the current setting will be returned.

    \remarks
    For kPointLineWeight style, 0 will be returned if point lineweight is disabled or 1 will be returned if enabled.
    For kLineCapStyle style, entry will be returned as a member of the OdPs::LineEndStyle enumeration.
    For kLineJoinStyle style, entry will be returned as a member of the OdPs::LineJoinStyle enumeration.
  */
  virtual OdUInt32 lineWeightConfiguration(LineWeightStyle styleEntry) const;
  
  /** \details
    Input flags of drawable filter.
  */
  enum DrawableFilterInputFlags 
  { 
    kNestedDrawable = 0x1000000  // Nested drawable flag.
  };
  
  /** \details
    Output flags of drawable filter.
  */
  enum DrawableFilterOutputFlags 
  { 
    kSkipVectorization = 1  // Skip vectorization flag.
  };
  
  /** \details
    Ranges of drawable filter.
  */
  enum DrawableFilterRanges 
  { 
	  kDrawableFilterAppRangeStart = 0x1000000                    // Start of the drawable filter range.
#if defined(OD_SWIGCSHARP)
    , kDrawableFilterAppRangeMask          = (uint)0xFF000000     // Drawable filter range mask for C#.
#elif defined(OD_SWIGJAVA)
    , kDrawableFilterAppRangeMask          = (long)0xFF000000     // Drawable filter range mask for Java.
#else
    , kDrawableFilterAppRangeMask          = (OdInt32)0xFF000000  // Drawable filter range mask for other languages.
#endif
  };

  /** \details
    Returns internal drawable filtration function ID.
    
    \param viewportId [in]  Pointer to the Object ID of the Viewport object to be queried.
  */
  virtual OdIntPtr drawableFilterFunctionId(OdDbStub* viewportId) const;

  /** \details
    Returns the set of flags for the drawable filtration function (will be called for each drawable vectorization).

    \param functionId [in]  Internal drawable filtration function ID.
    \param pDrawable [in]  Currently vectorizing drawable pointer.
    \param nFlags [in]  Set of input flags.
  */
  virtual OdUInt32 drawableFilterFunction(OdIntPtr functionId, const OdGiDrawable *pDrawable, OdUInt32 nFlags);

  /** \details
    Enables smooth contours information output for TrueType fonts.
  */
  virtual bool ttfPolyDraw() const;

  /** \details
    Returns true if and only if this vectorization process should be aborted.
  */
  virtual bool regenAbort() const;

  /** \details
    Plot style types.
  */
  enum PStyleType
  {
    kPsNone           = 0,  // Style is disabled.
    kPsByColor        = 1,  // "By color" style.
    kPsByName         = 2   // "By name" style.
  };
  
  /** \details
    Returns the plot style type of this Context object.
    
    \remarks
    plotStyleType() returns one of the following:
    
    <table>
    Name          Value
    kPsNone       0
    kPsByColor    1
    kPsByName     2
    </table>
  */
  virtual PStyleType plotStyleType() const;
  
  /** \details
    Returns the PaperSpace PlotStyle data for this vectorization.

    \param penNumber [in]  Pen number.
    \param plotStyleData [out]  Receives the PlotStyle data.
    \param objectId [in]  Object ID of plot style.
  */
  virtual void plotStyle(
    int penNumber, 
    OdPsPlotStyleData& plotStyleData) const;

  virtual void plotStyle(
    OdDbStub* objectId, 
    OdPsPlotStyleData& plotStyleData) const;

  /** \details
    Converts an object handle into an Object ID.

    \param objectId [in]  Object handle.
  */
  virtual OdDbStub* getStubByID( OdUInt64 persistentId ) const;

  /** \details
    Converts an Object ID to an object handle (database persistent ID).

    \param objectId [in]  Object ID.
  */
  virtual OdUInt64 getIDByStub( OdDbStub* objectId ) const;

  /** \details
    Returns the database of an Object ID.

    \param objectId [in]  Object ID.
  */
  virtual OdDbBaseDatabase *getDatabaseByStub( OdDbStub* objectId ) const;

  /** \details
    Returns the Owner Object ID of the specified Object ID.

    \param objectId [in]  Object ID.
  */
  virtual OdDbStub* getOwnerIDByStub( OdDbStub* objectId ) const;

  /** \details
    Converts a material name into an Object ID.

    \param strMatName [in]  Material name.
  */
  virtual OdDbStub* getStubByMatName( const OdString& strMatName ) const;

  /** \details
    Silhouette types.
  */
  enum SilhouetteType
  {
    kNoSilhouettes      = 0,        // Disables all types of silhouettes.
    kMeshSilhouettes    = (1 << 0), // Generates silhouettes for shell primitives on Gi side.
    kModelerSilhouettes = (1 << 1), // Generates silhouettes for Brep's on modeler geometry side.
    kAllSilhouettes     = kMeshSilhouettes | kModelerSilhouettes // Enables all types of silhouettes.
  };

  /** \details
    Controls the display of silhouettes.
  */
  virtual OdUInt32 displaySilhouettes() const;
  /** \details
    Returns true if generation of silhouettes is enabled for shell primitives on Gi side.
  */
  inline bool displayMeshSilhouettes() const    { return GETBIT(displaySilhouettes(), kMeshSilhouettes); }
  /** \details
    Returns true if generation of silhouettes is enabled for modeler geometry.
  */
  inline bool displayModelerSilhouettes() const { return GETBIT(displaySilhouettes(), kModelerSilhouettes); }

  /** \details
    Returns the interface for accessing section geometry functionality.
  */
  virtual OdGiSectionGeometryManagerPtr getSectionGeometryManager();

  /** \details
    Controls rendering anti-aliasing.
  */
  virtual OdUInt32 antiAliasingMode() const;

  /** \details
    Controls overriding of Xref properties.
  */
  virtual bool xrefPropertiesOverride() const;
};

/** \details
    This class is the base class for entity-level vectorization within Teigha.

    \sa
    TD_Gi

    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiCommonDraw : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiCommonDraw);

  /** \details
    Returns the regeneration type of the current vectorization process.

    \remarks
    regenType() returns one of the following:
    
    <table>
    Name                            Value 
    eOdGiRegenTypeInvalid           0
    kOdGiStandardDisplay            2
    kOdGiHideOrShadeCommand         3
    kOdGiRenderCommand              4
    kOdGiForExplode                 5
    kOdGiSaveWorldDrawForProxy      6
    kOdGiForExtents                 7
    </table>
  */
  virtual OdGiRegenType regenType() const = 0;

  /** \details
    Returns true if and only if this vectorization process should be aborted.
  */
  virtual bool regenAbort() const = 0;

  /** \details
    Provides access to this object's subentity traits.

    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const = 0;

  /** \details
    Provides access to this object's "drawing interface."
    
    \remarks
    The "drawing interface" is a set of geometry functions used during the vectorization process.
  */
  virtual OdGiGeometry& rawGeometry() const = 0;

  /** \details
    Returns true if this vectorization process is the result of a "drag" operation.
    
    \remarks
  */
  virtual bool isDragging() const = 0;
  
  /** \details
    Returns the recommended maximum deviation of the
    current vectorization for the specified point on a curve.

    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on a curve.
        
    \remarks
    This function returns the recommended maximum difference (with respect to the current active viewport) between the actual curve or surface, 
    and the tessellated curve or surface. 
    
    deviationType must be one of the following:
    
    <table>
    Name                       Value
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>

    \remarks
    This function uses circle zoom percent or FacetRes as appropriate.
  */
  virtual double deviation(
    const OdGiDeviationType deviationType, 
    const OdGePoint3d& pointOnCurve) const = 0;

  /** \details
    Returns the number of isolines to be drawn on surfaces during this vectorization.
  */
  virtual OdUInt32 numberOfIsolines() const = 0;

  /** \details
    Returns the OdGiContext instance associated with this object.
  */
  virtual OdGiContext* context() const = 0;

  /** \details
    Returns the current drawable nesting graph.
  */
  virtual const OdGiPathNode* currentGiPath() const;

  /** \details
    Returns whether silhouette curves should be generated (basically should return OdDbDatabase::getDISPSILH,
    but the behavior could be specialized).
  */
  virtual bool displayModelerSilhouettes() const;
};

/** \details
    Draw flags helper.
    
    \remarks
    Modifies draw flags and restores them back in the destructor.

    Corresponding C++ library: TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiDrawFlagsHelper
{
  protected:
    OdGiSubEntityTraits *m_pTraits;
    OdUInt32 m_prevFlags;
  public:
    OdGiDrawFlagsHelper(OdGiSubEntityTraits &pTraits, OdUInt32 addFlags, OdUInt32 delFlags = 0)
      : m_pTraits(NULL)
    {
      if (addFlags || delFlags)
      {
        m_prevFlags = pTraits.drawFlags();
        OdUInt32 newFlags = (m_prevFlags & ~delFlags) | addFlags;
        if (m_prevFlags != newFlags)
        {
          pTraits.setDrawFlags(newFlags);
          m_pTraits = &pTraits;
        }
      }
    }
    ~OdGiDrawFlagsHelper()
    {
      if (m_pTraits)
        m_pTraits->setDrawFlags(m_prevFlags);
    }
};

#include "TD_PackPop.h"

#endif

