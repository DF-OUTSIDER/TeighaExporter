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

#ifndef _OD_GI_H_
#define _OD_GI_H_

#include "RxObject.h"
#include "CmColorBase.h"

class OdDbStub;
class OdCmEntityColor;
class OdGeVector3d;
class OdGiMapper;
class OdCmTransparency;
struct OdGiDgLinetypeModifiers;
class OdGiFill;
struct OdGiLineweightOverride;
class OdGiPalette;

typedef enum
{
  kOdGiFillAlways = 1,  // Always fill closed graphic primitives.
  kOdGiFillNever  = 2   // Never fill closed graphic primitives.
} OdGiFillType;

typedef enum
{
  kOdGiInvisible      = 0, // Invisible
  kOdGiVisible        = 1, // Visible
  kOdGiSilhouette     = 2  // Silhouette edge
} OdGiVisibility;

#include "TD_PackPush.h"

/** \details
    This class provides an interface to the graphical attributes of graphic
    primitives.
  
    \remarks
    OdGiDrawableTraits is a superset of OdGiSubEntityTraits, and exposes
    additional traits that are consistant for the entire OdGiDrawable.
    
    An OdGiDrawableTraits instance is available during calls to subSetAttributes.
    Graphical attribute settings are used for all graphical primitives until they are changed.
    
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiDrawableTraits : public OdRxObject
{ 
public:
  ODRX_DECLARE_MEMBERS(OdGiDrawableTraits);
};

/** \details
    This class provides an interface to the graphical attributes of graphic
    primitives.
  
    \remarks
    An OdGiSubEntityTraits instance is available during calls to subWorldDraw and subViewportDraw,
    so that drawable classes can control attributes during the vectorization process. 
    Graphical attribute settings are used for all graphical primitives until they are changed.
    
    The functions in this class are implemented by the OdGiAbstractVectorizer
    class.

    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiSubEntityTraits : public OdGiDrawableTraits
{
public:
  enum DrawFlags
  {
    kNoDrawFlags        = 0x0000,   // Default value. No DrawFlags specified.
    kDrawBackfaces      = 0x0001,   // Forces backfaces vectorization.
    kDrawHatchGroup     = 0x0002,   // Next shells and meshes should draw hatches.
    kDrawFrontfacesOnly = 0x0004,   // Forces discard backfaces mode.
    kDrawGradientFill   = 0x0008,   // Draws with gradient fill.
    kDrawSolidFill      = 0x0010,   // Draws with solid fill.
    kDrawNoLineWeight   = 0x0020,   // Disables lineweight.
    kDrawNoPlotstyle    = 0x0080,   // Disables plotstyles.
    // the range before 0x8000 is reserved.
    kDrawContourFill            = 0x08000, // Teigha(R) for .dgn files specific.
    kDisableLayoutCache         = 0x10000, // Disable metafile caching for layout switching.
    kDrawBoundaryForClipping    = 0x20000, // Draw (rough) boundary of object to be checked
                                           // when clipping is performed, set by vectorizer.
    kDrawBoundaryForClippingDrw = 0x40000, // Draw (rough) boundary of object to be checked
                                           // when clipping is performed, set by a drawable.
    kClipSetIsEmpty             = 0x80000,
    kInternalDrawFlags          = kDisableLayoutCache | kClipSetIsEmpty,
    kDrawPolygonFill            = 0x100000, // Fill in all modes if FillAlways mode enabled.
    kExcludeFromViewExt         = 0x200000, // Don't use extents of the entity when calculating total extents.
    kDrawDgLsPolyWithoutBreaks  = 0x400000, // Draw polylines/polygons with Dgn Line Style without segmentation.
    kPolygonProcessing          = 0x800000, // Polygon filling logic must be applied for shell primitive.
    kDisableAutoGenerateNormals = 0x1000000, // Avoid automatic generation of normals for mesh/shell primitives.
    kDisableFillModeCheck       = 0x2000000, // Avoid checking of OdGiContext::fillMode during filling of geometry primitives.
    kDrawUnderlayReference      = 0x4000000, // Draw underlay reference object.
    // Flags which can be inherited by nested drawables.
    kInheritableDrawFlags       = kDrawNoLineWeight | kDrawNoPlotstyle | kDrawBoundaryForClipping | kDrawBoundaryForClippingDrw |
                                  kInternalDrawFlags | kExcludeFromViewExt | kDrawBackfaces | kDrawFrontfacesOnly
  };

  enum ShadowFlags
  {
    kShadowsCastAndReceive = 0,   // Geometry could cast and receive shadows.
    kShadowsDoesNotCast    = 1,   // Geometry doesn't cast shadows.
    kShadowsDoesNotReceive = 2,   // Geometry doesn't receive shadows.
    kShadowsIgnore         = kShadowsDoesNotCast | kShadowsDoesNotReceive
  };

  enum SelectionFlags
  {
    kNoSelectionFlags     = 0, // No selection flags set.
    kSelectionIgnore      = 1, // Avoid geometry selection.
    kHighlightingGeometry = 2  // Geometry drawn only in case if entire entity highlighted.
  };

  enum SubEntityHints
  {
    kHint2DAttributes  = 1,  // Setup attributes only available in 2DWireframe mode (plotStyles and etc.)
    kHint3DAttributes  = 2,  // Setup attributes only available in shaded modes (materials and etc.)
    kHintAllAttributes = kHint2DAttributes | kHint3DAttributes,
    kHintSnapModeOn  = 4,  // Setup that we currently inside selection for snap method
    kHintDefaultState  = kHintAllAttributes
  };
public:
  ODRX_DECLARE_MEMBERS(OdGiSubEntityTraits);

  /** \details
    Sets the current color index (ACI).

    \param color [in]  Color index.
    
    \remarks
    color must be one of the following:
    
    <table>
    Name              Value   Description
    kACIbyBlock       0       ByBlock.
    kACIforeground    7       Foreground color.
    kACIbyLayer       256     ByLayer. 
    kACIRed           1       Red. 
    kACIYellow        2       Yellow. 
    kACIGreen         3       Green. 
    kACICyan          4       Cyan. 
    kACIBlue          5       Blue. 
    kACIMagenta       6       Magenta. 
    ..                8-255   Defined by AcadPalette.h
    </table>
  */
  virtual void setColor(
    OdUInt16 color) = 0;

  /** \details
    Sets the current color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  virtual void setTrueColor(
    const OdCmEntityColor& color) = 0;

  /** \details
    Sets the current layer.
    \param layerId [in]  Object ID of the LayerTableRecord.  
  */
  virtual void setLayer(
    OdDbStub* layerId) = 0;

  /** \details
    Sets the current linetype.
    \param lineTypeId [in]  Object ID of the LinetypeTableRecord.  
  */
  virtual void setLineType(
    OdDbStub* lineTypeId) = 0;

  /** \details
    Sets the current selection marker.
    \param marker [in]  Selection marker.
  */
  virtual void setSelectionMarker(
    OdGsMarker selectionMarker) = 0;

  /** \details
    Sets the current fill type.
    \param fillType [in]  Fill type.
    
    \remarks
    fillType must be one of the following:
    
    <table>
    Name              Value   Description
    kOdGiFillAlways   1       Fill closed graphic primitives.
    kOdGiFillNever    2       Don't fill closed graphic primitives.
    </table>
       
    Closed graphic primitives consist of the following:
    * arcs with (OdGiArcType == kOdGiArcSector) || (OdGiArcType == kOdGiArcChord) 
    * circles 
    * meshes 
    * polygons 
    * shells 

  */
  virtual void setFillType(
    OdGiFillType fillType) = 0;

  /** \details
    Sets the current fill plane.
    
    \param pNormal [in]  Pointer to the normal to the plane.
    
    \remarks
    As implemented, this function does nothing but return.
    It will be fully implemented in a future release.
  */
  virtual void setFillPlane(
    const OdGeVector3d* pNormal = 0);

  /** \details
    Sets the current lineweight.

    \param lineWeight [in]  Lineweight.
  */
  virtual void setLineWeight(
    OdDb::LineWeight lineWeight) = 0;

  /** \details
    Sets the current linetype scale.
    \param lineTypeScale [in]  Linetype scale factor.
  */
  virtual void setLineTypeScale(
    double lineTypeScale = 1.0) = 0;

  /** \details
    Sets the current thickness.
    \param thickness [in]  Thickness.
  */
  virtual void setThickness(
    double thickness) = 0;

  /** \details
    Sets the current PlotStyleName.

    \param plotStyleNameType [in]  Plot Style Name Type.
    \param plotStyleNameId [in]  Object ID of the Plot Style Name.
    
    \remarks
    plotStyleNameId is used only when plotStyleNameType == kPlotStyleNameById.
    
    plotStyleNameType must be one of the following:
    
    <table>
    Name                           Value                         
    kPlotStyleNameByLayer          0
    kPlotStyleNameByBlock          1
    kPlotStyleNameIsDictDefault    2
    kPlotStyleNameById             3
    </table>
  */
  virtual void setPlotStyleName(
    OdDb::PlotStyleNameType plotStyleNameType, 
    OdDbStub* plotStyleNameId = 0) = 0;

  /** \details
    Sets the current material.
    \param materialId [in]  Object ID of the material.
  */
  virtual void setMaterial(
    OdDbStub* materialId) = 0;

  /** \details
    Sets the current material mapper.
    \param pMapper [in]  Pointer to the GI mapper object.
  */
  virtual void setMapper(
    const OdGiMapper *pMapper) = 0;

  /** \details
    Sets the current visual style.
    \param visualStyleId [in]  Object ID of the visual style.
  */
  virtual void setVisualStyle(
    const OdDbStub* visualStyleId);

  /** \details
    Sets the current transparency.
    
    \param transparency [in]  OdCmTransparency object.
  */
  virtual void setTransparency(
    const OdCmTransparency &transparency);

  /** \details
    Sets the drawing flags.
    
    \param drawFlags [in]  Input bitmask with DrawFlags.
  */
  virtual void setDrawFlags(
    OdUInt32 drawFlags);

  /** \details
    Sets selection geometry flag.
    
    \param bSelectionFlag [in]  New selection geometry flag.
  */
  virtual void setSelectionGeom(
    bool bSelectionFlag);

  /** \details
    Sets the shadow flags.
    
    \param shadowFlags [in]  Input ShadowFlags.
  */
  virtual void setShadowFlags(
    ShadowFlags shadowFlags);

  /** \details
    Sets sectionable flag.
    
    \param bSectionableFlag [in]  New sectionable flag.
  */
  virtual void setSectionable(
    bool bSectionableFlag);

  /** \details
    Sets the selection flags.
    
    \param selectionFlags [in]  Input SelectionFlags.
  */
  virtual void setSelectionFlags(
    SelectionFlags selectionFlags);

  /** \details
    Returns the current color index (ACI).

    \remarks
    color() returns one of the following:
    
    <table>
    Name              Value   Description
    kACIbyBlock       0       ByBlock.
    kACIforeground    7       Foreground color.
    kACIbyLayer       256     ByLayer. 
    kACIRed           1       Red. 
    kACIYellow        2       Yellow. 
    kACIGreen         3       Green. 
    kACICyan          4       Cyan. 
    kACIBlue          5       Blue. 
    kACIMagenta       6       Magenta. 
    ..                8-255   Defined by AcadPalette.h
    </table>
  */
  virtual OdUInt16 color() const = 0;

  /** \details
    Returns the current color as an OdCmEntityColor instance.
  */
  virtual OdCmEntityColor trueColor() const = 0;

  /** \details
      Returns the current layer.
  */
  virtual OdDbStub* layer() const = 0;

  /** \details
      Returns the current linetype.
  */
  virtual OdDbStub* lineType() const = 0;

  /** \details
    Returns the current fill type.
      
    \returns
    fillType() returns one of the following:
    
    <table>
    Name              Value   Description
    kOdGiFillAlways   1       Fill closed graphic primitives.
    kOdGiFillNever    2       Don't fill closed graphic primitives.
    </table>
       
    Closed graphic primitives consist of the following:
    * arcs with (OdGiArcType == kOdGiArcSector) || (OdGiArcType == kOdGiArcChord) 
    * circles 
    * meshes 
    * polygons 
    * shells 
  */
  virtual OdGiFillType fillType() const = 0;

  /** \details
    Returns the normal to the current fill plane.
    
    \param normal [out]  Receives the normal.
    
    \remarks
    As implemented, this function does nothing but return false.
    It will be fully implemented in a future release.
  */
  virtual bool fillPlane(
    OdGeVector3d& normal);

  /** \details
    Returns the current lineweight.
  */
  virtual OdDb::LineWeight lineWeight() const = 0;

  /** \details
    Returns the current linetype scale.
  */
  virtual double lineTypeScale() const = 0;

  /** \details
    Returns the current thickness.
  */
  virtual double thickness() const = 0;

  /** \details
    Returns the current PlotStyleName type.
    
    \remarks
    plotStyleNameType() returns one of the following:
    
    <table>
    Name                           Value                         
    kPlotStyleNameByLayer          0
    kPlotStyleNameByBlock          1
    kPlotStyleNameIsDictDefault    2
    kPlotStyleNameById             3
    </table>
  */
  virtual OdDb::PlotStyleNameType plotStyleNameType() const = 0;

  /** \details
    Returns object ID of the the plot style name.
  */
  virtual OdDbStub* plotStyleNameId() const = 0;

  /** \details
    Returns the Object ID of the material.
  */
  virtual OdDbStub* material() const = 0;
  
  /** \details
    Returns pointer to the current material mapper.
  */
  virtual const OdGiMapper* mapper() const = 0;

  /** \details
    Returns the Object ID of the visual style.
  */
  virtual OdDbStub* visualStyle() const;

  /** \details
    Returns the current transparency.
  */
  virtual OdCmTransparency transparency() const;

  /** \details
    Return the current DrawFlags bitmask.
  */
  virtual OdUInt32 drawFlags() const;

  /** \details
    Return the current selection geometry flag.
  */
  virtual bool selectionGeom() const;

  /** \details
    Return the current ShadowFlags.
  */
  virtual ShadowFlags shadowFlags() const;

  /** \details
    Return the current sectionable flag.
  */
  virtual bool sectionable() const;

  /** \details
    Return the current SelectionFlags.
  */
  virtual SelectionFlags selectionFlags() const;

  // Teigha(R) for .dgn files specific

  /** \details
    Sets the current secondary color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  virtual void setSecondaryTrueColor(
    const OdCmEntityColor& color);

  /** \details
    Returns the current secondary color as an OdCmEntityColor instance.
  */
  virtual OdCmEntityColor secondaryTrueColor() const;

  /** \details
    Sets Dgn Line Style modifiers.

    \param pLSMod [in]  Pointer to structure with Dgn Line Style modifiers.
  */
  virtual void setLineStyleModifiers(
    const OdGiDgLinetypeModifiers* pLSMod);

  /** \details
    Return pointer to the current Dgn Line Style modifiers structure.
  */
  virtual const OdGiDgLinetypeModifiers* lineStyleModifiers() const;

  // eof Teigha(R) for .dgn files specific section

  /** \details
    Sets boundary filling.

    \param pFill [in]  Boundary filling parameters.
  */
  virtual void setFill(
    const OdGiFill* pFill);

  /** \details
    Return boundary filling parameters.
  */
  virtual const OdGiFill* fill() const;

  /** \details
    Push lineweight scale and style override into lineweight overrides stack.
    \param pOverride [in]  New lineweight override.
    \returns Returns true in case if lineweight overrides supported by underlying vectorization pipeline.
  */
  virtual bool pushLineweightOverride(const OdGiLineweightOverride* pOverride);

  /** \details
    Pop lineweight scale and style override from lineweight overrides stack.
  */
  virtual void popLineweightOverride();

  /** \details
    Push palette override into palette overrides stack.
    \param pOverride [in]  New palette override.
    \returns Returns true in case if palette overrides supported by underlying vectorization pipeline.
  */
  virtual bool pushPaletteOverride(const OdGiPalette* pOverride);

  /** \details
    Pop palette override from palette overrides stack.
  */
  virtual void popPaletteOverride();

  /** \details
    Returns the set of SubEntityHints flags which can be used to provide more flexible tuning of
    subSetAttributes traits processing.
  */
  virtual OdUInt32 setupForEntity() const;

  /** \details
    Adds a light object to the model.

    \param lightId [in]  Object ID of the light.

    \remarks
    Through this interface, drawables that emit light can add light objects to the model. 
  */
  virtual void addLight(
    const OdDbStub* lightId);
};

typedef OdSmartPtr<OdGiSubEntityTraits> OdGiSubEntityTraitsPtr;

#include "TD_PackPop.h"

#endif //_OD_GI_H_
