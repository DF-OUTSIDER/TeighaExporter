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

#ifndef __ODGIVISUALSTYLE_H__
#define __ODGIVISUALSTYLE_H__

#include "TD_PackPush.h"

#include "Gi/Gi.h"
#include "Gi/GiVariant.h"

class OdCmColorBase;

/** \details
    This class represents a set of visual style properties that can be applied to the faces of graphical entities.  
    
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiFaceStyle : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiFaceStyle);

  /** \details
    Defines the lighting model of the face style to apply to the faces of graphical entities.  
  */
  enum LightingModel
  {
    kInvisible = 0, // Faces are invisible.
    kConstant  = 1, // Faces are drawn in a constant color. 
    kPhong     = 2, // Faces are drawn with Phong model to approximate realistic direct lighting.
    kGooch     = 3  // Faces are drawn with non-photorealistic lighting model for automatic technical illustration.
  };

  /** \details
    Defines the calculation method used for application of lighting effects to faces.
  */
  enum LightingQuality
  {
    kNoLighting        = 0, // No lighting calculations are applied to the faces.
    kPerFaceLighting   = 1, // Color is calculated for each face based on its orientation to the light sources of the scene.
    kPerVertexLighting = 2, // Color is calculated for each vertex of a face based on its orientation to the light sources of the scene.
    kPerPixelLighting  = 3  // Color is calculated for each pixel of a face based on its orientation to the light sources of the scene.
  };

  /** \details
    Defines modifications that affect the color of a face before any lighting and shading calculations have been performed.
  */
  enum FaceColorMode
  {
    kNoColorMode     = 0, // Apply no modifications to the faces. 
    kObjectColor     = 1, // Apply the color of the drawable object to the faces of the object.
    kBackgroundColor = 2, // Apply the display background color to the faces.
    kMono            = 3, // All faces are treated as having a specific color, resulting in a "monochromatic" effect. 
    kTint            = 4, // All faces have the hue and saturation values of their color mapped to the hue and saturation values of a specific color, resulting in a "tinted" effect. The luminance value remains unchanged. 
    kDesaturate      = 5  // Apply a desaturation percentage to the diffuse color of a material.
  };

  /** \details
    Defines the modifiers applied to the faces using this face style.
  */
  enum FaceModifier
  {
    kNoFaceModifiers = 0,  // Apply no face modifiers to the faces. 
    kOpacity         = 1,  // Apply an opacity value to the faces. If unset, faces are displayed with full opacity. If set, faces are displayed with a specified degree of opacity from completely opaque to completely transparent. 
    kSpecular        = 2   // Apply a specular highlight (indicating reflection and shininess values) to a material that is applied to the faces.
  };

  /** \details
    Copies values from the given face style to this face style.

    \param style [in]  Reference to the OdGiFaceStyle to copy into this face style.
  */
  void set(const OdGiFaceStyle& style);

  OdGiFaceStyle& operator=(const OdGiFaceStyle& style);
  bool operator==(const OdGiFaceStyle& style) const;

  /** \details
    Sets the lighting model to the given value. All valid values are described in the LightingModel enumeration.

    \param lightingModel [in]  Lighting model value to set.
  */
  virtual void setLightingModel(LightingModel lightingModel) = 0;
  
  /** \details
    Returns the currently used lighting model of the face style.
  */
  virtual LightingModel lightingModel() const = 0;

  /** \details
    Sets the lighting quality of faces to the given value. All valid values are described in the LightingQuality enum.

    \param lightingQuality [in]  Lighting quality value to set.
  */
  virtual void setLightingQuality(LightingQuality lightingQuality) = 0;

  /** \details
    Returns the currently used lighting quality of the face style.
  */
  virtual LightingQuality lightingQuality() const = 0;

  /** \details
    Sets the face color mode of the face style. All valid values are described in the FaceColorMode enum.

    \param mode [in]  Face color mode value to set.
  */
  virtual void setFaceColorMode(FaceColorMode mode) = 0;

  /** \details
    Returns the currently used face color mode value.
  */
  virtual FaceColorMode faceColorMode() const = 0;

  /** \details
    Sets a collection of bitflags representing the face modifiers in effect.

    \param nModifiers [in]  Unsigned long value that represents a collection of bitwise flags defined in the FaceModifier enum.
  */
  virtual void setFaceModifiers(unsigned long nModifiers) = 0;
  
  /** \details
    Enables or disables the specified face modifier flag.

    \param flag [in]  Flag to enable or disable.
    \param bEnable [in]  Value that indicates whether to enable or disable the specified flag. 
  */
  virtual void setFaceModifierFlag(FaceModifier flag, bool bEnable) = 0;

  /** \details
    Returns the collection of currently used face modifier flags.
  */
  virtual unsigned long faceModifiers() const = 0;

  /** \details
    Returns whether the specified face modifier flag is enabled.
    
    \param flag [in]  Flag to be checked whether it's enabled or disabled.
  */
  virtual bool isFaceModifierFlagSet(FaceModifier flag) const = 0;

  /** \details
    Sets the opacity level of the faces.

    \param nLevel [in]  Opacity level to set. Must be in the range from 0.0 (faces are fully transparent) to 1.0 (no transparency).   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable the opacity. 
  */
  virtual void setOpacityLevel(double nLevel, bool bEnableModifier) = 0;

  /** \details
    Returns whether the opacity level is applied to faces.
  */
  virtual double opacityLevel() const = 0;

  /** \details
    Sets the amount of specular highlight (indicating reflection and shininess values) of a faces material.

    \param nAmount [in]  Amount of specular highlight of a faces material to set.
    \param bEnableModifier [in]  Value that indicates whether to enable or disable the specular highlight. 
  */
  virtual void setSpecularAmount(double nAmount, bool bEnableModifier) = 0;

  /** \details
    Returns the amount of specular highlight applied to a faces material.
  */
  virtual double specularAmount() const = 0;

  /** \details
    Sets the color that will be applied to produce a monochromatic effect on the faces.

    \param color [in]  Reference to an OdCmColorBase value to set.
    \param bEnableMode [in]  Value that indicates whether to enable or disable the mono color mode. 
  */
  virtual void setMonoColor(const OdCmColorBase& color, bool bEnableMode) = 0;
  
  /** \details
    Returns a color that is applied to produce a monochromatic effect on the faces.
  */
  virtual const OdCmColorBase& monoColor() const = 0;

  /** \details
    Returns a color that is applied to produce a monochromatic effect on the faces.
  */
  virtual OdCmColorBase& monoColor() = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiFaceStyle object pointers.
*/
typedef OdSmartPtr<OdGiFaceStyle> OdGiFaceStylePtr;

/** \details
    This class defines a set of visual style properties that can be applied to the edges of graphical entities.    

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiEdgeStyle : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiEdgeStyle);

  /** \details
    Defines the model that is used to display the edges.
  */
  enum EdgeModel
  {
    kNoEdges    = 0,  // No edges are displayed.
    kIsolines   = 1,  // Display isolines.
    kFacetEdges = 2   // Display facet edges (the edges between neighbouring faces).
  };

  /** \details
    Defines a collection of bitflags that represent the combination of edge styles that are in effect for displaying edges.
  */
  enum EdgeStyle
  {
    kNoEdgeStyle  = 0,  // No edge styles are applied to the edges.
    kVisible      = 1,  // Visible edges are displayed.
    kSilhouette   = 2,  // Silhouette edges are displayed.
    kObscured     = 4,  // Obscured edges are displayed.
    kIntersection = 8   // Intersection edges are displayed.
  };

  /** \details
    Defines a collection of modifiers that can be applied to edges to give them various types of effects.
  */
  enum EdgeModifier
  {
    kNoEdgeModifiers = 0x000, // No modifiers are applied.
    kOverhang        = 0x001, // Allow an edge to overhang its start and end points by a specified amount. 
    kJitter          = 0x002, // Apply a jitter effect to the edges. The jitter effect is produced by drawing extra lines adjacent to the original line, with a specific amount of perturbation away from the original line.
    kWidth           = 0x004, // Apply a width to the edges. 
    kColor           = 0x008, // Apply a color that differs from the drawable object color to the edges. 
    kHaloGap         = 0x010, // Apply a halo gap percentage to the edges of an object that is hidden by another object, such that the specified gap is displayed between the edges of the hidden object and the edges of the object which hides it.
    kLinetype        = 0x020, // Apply a linetype to the edges.
    kAlwaysOnTop     = 0x040, // Force edges to be on top. Edges will not participate in the Z-ordering of the displayed drawables.
    kOpacity         = 0x080, // Apply an opacity value to the edges. If unset, edges are displayed with full opacity. If set, edges are displayed with a specified degree of opacity from completely opaque to completely transparent.
    kWiggle          = 0x100, // Apply a wiggle value to the edges.
    kTexture         = 0x200  // Apply a material texture to the edges.
  };

  /** \details
    Defines a collection of jitter values that can be applied to edges.
  */
  enum JitterAmount
  {
    kJitterLow    = 1,  // A small amount of perturbation. 
    kJitterMedium = 2,  // A medium amount of perturbation. 
    kJitterHigh   = 3   // A large amount of perturbation. 
  };

  /** \details
    Defines a collection of wiggle values that can be applied to edges.
  */
  enum WiggleAmount
  {
    kWiggleLow    = 1,  // A small amount of perturbation.
    kWiggleMedium = 2,  // A medium amount of perturbation.
    kWiggleHigh   = 3   // A large amount of perturbation.
  };

  /** \details
    Defines a collection of linetypes that can be applied to edges.
  */
  enum LineType
  {
    kSolid            = 1,  // A solid line pattern is used to display edges.
    kDashed           = 2,  // A dashed line pattern is used to display edges.
    kDotted           = 3,  // A dotted line pattern is used to display edges.
    kShortDash        = 4,  // A short dashed line pattern is used to display edges.
    kMediumDash       = 5,  // A medium dashed line pattern is used to display edges.
    kLongDash         = 6,  // A long dashed line pattern is used to display edges.
    kDoubleShortDash  = 7,  // A double short dashed line pattern is used to display edges.
    kDoubleMediumDash = 8,  // A double medium dashed line pattern is used to display edges.
    kDoubleLongDash   = 9,  // A double long dashed line pattern is used to display edges.
    kMediumLongDash   = 10, // A medium long dashed line pattern is used to display edges.
    kSparseDot        = 11  // A sparse dotted line pattern is used to display edges.
  };

  /** \details
    Defines the type of geometry to which the edge styles should be applied.
  */
  enum EdgeStyleApply
  {
    kDefault, // Apply the edge style to the geometry as designed.
    kAll      // Apply the edge style to all geometry.
  };

  /** \details
    Sets a specified edge style value for this OdGiEdgeStyle object.

    \param style [in]  Reference to an OdGiEdgeStyle value to be copied into this edge style.
  */
  void set(const OdGiEdgeStyle& style);
  OdGiEdgeStyle& operator=(const OdGiEdgeStyle& style);
  bool operator==(const OdGiEdgeStyle& style) const;

  /** \details
    Sets a specified edge model value for this OdGiEdgeStyle object.

    \param model [in]  Edge model value to set. All available values are defined in the EdgeModel enum.
  */
  virtual void setEdgeModel(EdgeModel model) = 0;

  /** \details
    Returns the currently used edge model value for this OdGiEdgeStyle object. All model values are defined in the EdgeModel enumeration.
  */
  virtual EdgeModel edgeModel() const = 0;

  /** \details
    Sets a combination of values from the EdgeStyle enumeration as an edge styles value for this OdGiEdgeStyle object.

    \param nStyles [in]  A combination of bit flags from the EdgeStyle enumeration indicating desired edge styles.
  */
  virtual void setEdgeStyles(unsigned long nStyles) = 0;

  /** \details
    Enables or disables the given edge styles flag.

    \param flag [in]  A flag to enable or disable.
    \param bEnable [in]  A boolean value indicating if the given flag is to be enabled or disabled.
  */
  virtual void setEdgeStyleFlag(EdgeStyle flag, bool bEnable) = 0;

  /** \details
    Returns an unsigned long value that represents a collection of edge style flags in effect.
  */
  virtual unsigned long edgeStyles() const = 0;

  /** \details
    Returns whether the specified face modifier flag is enabled.

    \param flag [in]  Flag to be checked whether it's enabled or disabled.
  */
  virtual bool isEdgeStyleFlagSet(EdgeStyle flag) const = 0;

  /** \details
    Sets the color of intersection edges.

    \param color [in]  Intersection edges color to set.
  */
  virtual void setIntersectionColor(const OdCmColorBase& color) = 0;

  /** \details
    Returns the color of intersection edges.
  */
  virtual const OdCmColorBase& intersectionColor() const = 0;

  /** \details
    Returns the color of intersection edges.
  */
  virtual OdCmColorBase& intersectionColor() = 0;

  /** \details
    Sets the color of obscured edges.

    \param color [in]  Obscured edges color to set.
  */
  virtual void setObscuredColor(const OdCmColorBase& color) = 0;

  /** \details
    Returns the color of obscured edges.
  */
  virtual const OdCmColorBase& obscuredColor() const = 0;
  
  /** \details
    Returns the color of obscured edges.
  */
  virtual OdCmColorBase& obscuredColor() = 0;

  /** \details
    Sets the linetype of obscured edges.

    \param ltype [in]  Obscured edges linetype to set.
  */
  virtual void setObscuredLinetype(LineType ltype) = 0;

  /** \details
    Returns the linetype of obscured edges.
  */
  virtual LineType obscuredLinetype() const = 0;

  /** \details
    Sets the linetype of intersection edges.

    \param ltype [in]  Intersection edges linetype to set.
  */
  virtual void setIntersectionLinetype(LineType ltype) = 0;

  /** \details
    Returns the linetype of intersection edges.
  */
  virtual LineType intersectionLinetype() const = 0;

  /** \details
    Sets the angle threshold, above which a facet edge representing adjoining facets will be displayed.

    \param nAngle [in]  Threshold angle to set. 
  */
  virtual void setCreaseAngle(double nAngle) = 0;

  /** \details
    Returns the double value that is the angle threshold (in degrees) above which a facet edge representing adjoining facets will be displayed.
  */
  virtual double creaseAngle() const = 0;

  /** \details
    Sets a collection of bitflags representing the edge modifiers in effect.

    \param nModifiers [in]  Unsigned long value that represents a collection of bitwise flags defined in the EdgeModifier enumeration.
  */
  virtual void setEdgeModifiers(unsigned long nModifiers) = 0;

  /** \details
    Enables or disables the specified edge modifier flag.

    \param flag [in]  Flag to enable or disable.
    \param bEnable [in]  Value that indicates whether to enable or disable the specified flag. 
  */
  virtual void setEdgeModifierFlag(EdgeModifier flag, bool bEnable) = 0;

  /** \details
    Returns the collection of currently used edge modifier flags.
  */
  virtual unsigned long edgeModifiers() const = 0;

  /** \details
    Returns whether the specified edge modifier flag is enabled.
    
    \param flag [in]  Flag to be checked whether it's enabled or disabled.
  */
  virtual bool isEdgeModifierFlagSet(EdgeModifier flag) const = 0;

  /** \details
    Sets the color of edges.
    
    \param color [in]  Color value to set.
    \param bEnableModifier [in]  Value that indicates whether to enable or disable the color.
  */
  virtual void setEdgeColor(const OdCmColorBase& color, bool bEnableModifier) = 0;

  /** \details
    Returns the color of edges.
  */
  virtual const OdCmColorBase& edgeColor() const = 0;

  /** \details
    Returns the color of edges.
  */
  virtual OdCmColorBase& edgeColor() = 0;

  /** \details
    Sets the opacity level of edges.

    \param nLevel [in]  Opacity level to set. Must be in the range from 0.0 (edges are fully transparent) to 1.0 (no transparency).   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable the opacity. 
  */
  virtual void setOpacityLevel(double nLevel, bool bEnableModifier) = 0;

  /** \details
    Returns the opacity level applied to edges.
  */
  virtual double opacityLevel() const = 0;

  /** \details
    Sets the edge width in pixels.

    \param nWidth [in]  The width to set.   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable the edge width. 
  */
  virtual void setEdgeWidth(int nWidth, bool bEnableModifier) = 0;
  
  /** \details
    Returns the edge width in pixels.
  */
  virtual int edgeWidth() const = 0;

  /** \details
    Sets the amount (in pixels) that edges should overhang their start and end points.

    \param nAmount [in]  The overhang amount.   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable edge overhang. 
  */
  virtual void setOverhangAmount(int nAmount, bool bEnableModifier) = 0;

  /** \details
    Returns the edge overhang in pixels.
  */
  virtual int overhangAmount() const = 0;

  /** \details
    Sets the amount of jitter effect to be applied to jittered edges. 

    \param amount [in]  The jitter amount to set. All applicable values are defined in the JitterAmount enumeration.   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable jitter effect. 
  */
  virtual void setJitterAmount(JitterAmount amount, bool bEnableModifier) = 0;

  /** \details
    Returns the amount of jitter effect as a JitterAmount value.
  */
  virtual JitterAmount jitterAmount() const = 0;

  /** \details
    Sets the amount of wiggle effect to be applied to edges. 

    \param amount [in]  The wiggle amount to set. All applicable values are defined in the WiggleAmount enumeration.   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable wiggle effect. 
  */
  virtual void setWiggleAmount(WiggleAmount amount, bool bEnableModifier) = 0;

  /** \details
    Returns the amount of wiggle effect as a WiggleAmount value.
  */
  virtual WiggleAmount wiggleAmount() const = 0;

  /** \details
    Sets the color of silhouette edges. 

    \param color [in]  Reference to an OdCmColorBase value to set.   
  */
  virtual void setSilhouetteColor(const OdCmColorBase& color) = 0;

  /** \details
    Returns the silhouette color.
  */
  virtual const OdCmColorBase& silhouetteColor() const = 0;

  /** \details
    Returns the silhouette color.
  */
  virtual OdCmColorBase& silhouetteColor() = 0;

  /** \details
    Sets the width of silhouette edges. 

    \param nWidth [in]  Width of silhouette edges (in pixels) to set.   
  */
  virtual void setSilhouetteWidth(short nWidth) = 0;

  /** \details
    Returns the width of silhouette edges in pixels.
  */
  virtual short silhouetteWidth() const = 0;

  /** \details
    Sets the amount of halo gap. 

    \param nHaloGap [in]  The halo gap amount to set. Value must be from 0 to 100.   
    \param bEnableModifier [in]  Value that indicates whether to enable or disable halo gap. 
  */
  virtual void setHaloGap(int nHaloGap, bool bEnableModifier) = 0;

  /** \details
    Returns the halo gap amount.
  */
  virtual int haloGap() const = 0;

   /** \details
    Sets the number of isolines. 

    \param nIsolines [in]  The number of isolines to set. Value must be between 0 and 2047.   
   */
  virtual void setIsolines(unsigned short nIsolines) = 0;

  /** \details
    Returns the number of isolines.
  */
  virtual unsigned short isolines() const = 0;

  /** \details
    Sets whether hide precision should be enabled.

    \param bHidePrecision [in]  Value that indicates whether to enable or disable hide precision.   
  */
  virtual void setHidePrecision(bool bHidePrecision) = 0;

  /** \details
    Returns if hide precision is enabled or disabled.
  */
  virtual bool hidePrecision() const = 0;

  /** \details
    Sets a type to what geometry the edge style should be applied.

    \param apply [in]  Value that represents the type of geometry to which the edge styles should be applied.   
  */
  virtual void setEdgeStyleApply(EdgeStyleApply apply) = 0;

  /** \details
    Returns the type of geometry to which the edge style will be applied as an EdgeStyleApply value.
  */
  virtual EdgeStyleApply edgeStyleApply() const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiEdgeStyle object pointers.
*/
typedef OdSmartPtr<OdGiEdgeStyle> OdGiEdgeStylePtr;

/** \details
    This class defines a set of display properties that can be applied to graphical entities.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiDisplayStyle : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiDisplayStyle);
  
  /** \details
      Defines a combination of display settings that can be enabled.
  */
  enum DisplaySettings
  {
    kNone        = 0x00,  // No display settings are used.
    kBackgrounds = 0x01,  // Backgrounds are enabled.
    kLights      = 0x02,  // Lights are enabled.
    kMaterials   = 0x04,  // Materials are enabled.
    kTextures    = 0x08   // Textures are enabled.
  };

  /** \details
    Defines shadows types that can be displayed.
  */
  enum ShadowType
  {
    kShadowsNone          = 0,  // No shadow is displayed.
    kShadowsGroundPlane   = 1,  // Ground plane shadows are displayed.
    kShadowsFull          = 2,  // Full shadows are displayed.       
    kShadowsFullAndGround = 3   // Full shadows are displayed along with shadows on the ground plane. 
  };

  /** \details
    Sets values of the given display style to this display style.

    \param style [in]  Reference to the OdGiDisplayStyle to copy into this display style.
  */
  void set(const OdGiDisplayStyle& style);
  OdGiDisplayStyle& operator=(const OdGiDisplayStyle& style);
  bool operator==(const OdGiDisplayStyle& style) const;

  /** \details
    Sets a collection of bitflags representing the display settings.

    \param nSettings [in]  Unsigned long value that represents a collection of bitwise flags defined in the DisplaySettings enumeration.
  */
  virtual void setDisplaySettings(unsigned long nSettings) = 0;

  /** \details
    Enables or disables the specified display settings flag.

    \param flag [in]  Flag to enable or disable.
    \param bEnable [in]  Value that indicates whether to enable or disable the specified flag. 
  */
  virtual void setDisplaySettingsFlag(DisplaySettings flag, bool bEnable) = 0;

  /** \details
    Returns the collection of currently used display settings flags.
  */
  virtual unsigned long displaySettings() const = 0;

  /** \details
    Returns whether the specified display settings flag is enabled.
    
    \param flag [in]  Flag to be checked whether it's enabled or disabled.
  */
  virtual bool isDisplaySettingsFlagSet(DisplaySettings flag) const = 0;

  /** \details
    Sets the brightness value.

    \param value [in]  Brightness level to set.
  */
  virtual void setBrightness(double value) = 0;

  /** \details
    Returns the brightness level.
  */
  virtual double brightness() const = 0;

  /** \details
    Sets the shadow type.

    \param type [in]  Shadow type to set. All available types are defined in the ShadowType enumeration.
  */
  virtual void setShadowType(ShadowType type) = 0;

  /** \details
    Returns the currently used shadow type.
  */
  virtual ShadowType shadowType() const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiDisplayStyle object pointers.
*/
typedef OdSmartPtr<OdGiDisplayStyle> OdGiDisplayStylePtr;


/** \details
    Namespace contains visual style property related enumerations.
    <group TD_Namespaces>
*/
namespace OdGiVisualStyleProperties
{
  /** \details
    Defines the get/set properties for a visual style.
  */
  enum Property
  {
    kInvalidProperty              = -1, // Invalid property. 

    // Face properties

    kFaceLightingModel            =  0, // The lighting model applied to the faces of drawables. See the FaceLightingModel enumeration.
    kFaceLightingQuality          =  1, // The calculation method of lighting effects applied to faces. See the FaceLightingQuality enumeration.
    kFaceColorMode                =  2, // The color mode applied to faces. See the FaceColorMode enumeration.
    kFaceModifiers                =  3, // Modifications applied to faces. See the FaceModifiers enumeration.
    kFaceOpacity                  =  4, // The face opacity property. Double value representing the face opacity level in the range from 0.0 (faces are fully transparent) to 1.0 (no transparency).
    kFaceSpecular                 =  5, // The face specular property. Double value representing the specular highlight amount (indicating reflection and shininess values) of the material that is applied to faces.
    kFaceMonoColor                =  6, // The face monocolor effect property. OdCmColor representing the color applied to produce a "monochromatic" effect on faces.

    // Edge properties

    kEdgeModel                    =  7, // The edge model property. See the EdgeModel enumeration.
    kEdgeStyles                   =  8, // The edge style property. See the EdgeStyles enumeration.
    kEdgeIntersectionColor        =  9, // The intersected edges color. OdCmColor representing the color applied to intersection edges.
    kEdgeObscuredColor            = 10, // The obscured edges color. OdCmColor representing the color applied to obscured edges.
    kEdgeObscuredLinePattern      = 11, // The line pattern applied to obscured edges. See the EdgeLinePattern enumeration.
    kEdgeIntersectionLinePattern  = 12, // The line pattern applied to intersection edges. See the EdgeLinePattern enumeration.
    kEdgeCreaseAngle              = 13, // The edge crease angle. Double value indicating angle threshold, above which a facet edge representing adjoining facets will be displayed.
    kEdgeModifiers                = 14, // Modifications applied to edges. See the EdgeModifiers enumeration.
    kEdgeColor                    = 15, // The edge color property. OdCmColor representing the color applied to edges. 
    kEdgeOpacity                  = 16, // The edge opacity property. Double value representing the edge opacity level in the range from 0.0 (faces are fully transparent) to 1.0 (no transparency).
    kEdgeWidth                    = 17, // The edge width property. Integer value representing edge width in pixels.
    kEdgeOverhang                 = 18, // The edge overhang property. Integer value representing the amount, in pixels, that edges should overhang their start and end points.
    kEdgeJitterAmount             = 19, // The edge jitter amount. See the EdgeJitterAmount enum.
    kEdgeSilhouetteColor          = 20, // The edge silhouette color property. OdCmColor enumeration the color applied to silhouette edges. 
    kEdgeSilhouetteWidth          = 21, // The edge silhouette width property. Short value representing silhouette edge width in pixels. 
    kEdgeHaloGap                  = 22, // The edge halo gap amount. Integer value from 0 to 100 representing the halo gap. 
    kEdgeIsolines                 = 23, // The isolines number property. Unsigned short value, between 0 and 2047, indicating the number of isolines to display for curved surfaces.
    kEdgeHidePrecision            = 24, // The hide precision property. Boolean value indicating hide precision enabled. True if enabled; otherwise, false.

    // Display properties

    kDisplayStyles                = 25, // Display settings. See the DisplayStyles enumeration.
    kDisplayBrightness            = 26, // The display brightness property. Double value indicating brightness. 
    kDisplayShadowType            = 27, // The display shadow type property. See the DisplayShadowType enumeration.

    // New in AC2011, 2013
    kUseDrawOrder                 = 28, // Boolean value indicating whether draw order is enabled. True if enabled; otherwise, false.
    kViewportTransparency         = 29, // Boolean value indicating whether viewport transparency is enabled. True if enabled; otherwise, false.
    kLightingEnabled              = 30, // Boolean value indicating whether lightning is enabled. True if enabled; otherwise, false.
    kPosterizeEffect              = 31, // Boolean value indicating whether posterize effect is enabled. True if enabled; otherwise, false.
    kMonoEffect                   = 32, // Boolean value indicating whether mono effect is enabled. True if enabled; otherwise, false.

    // New in 2013
    kBlurEffect                   = 33, // Boolean value indicating whether blur effect is enabled. True if enabled; otherwise, false.
    kPencilEffect                 = 34, // Boolean value indicating whether pencil effect is enabled. True if enabled; otherwise, false.
    kBloomEffect                  = 35, // Boolean value indicating whether bloom effect is enabled. True if enabled; otherwise, false.
    kPastelEffect                 = 36, // Boolean value indicating whether pastel effect is enabled. True if enabled; otherwise, false.
    kBlurAmount                   = 37, // The blur amount property. Integer value representing amount of blur effect.
    kPencilAngle                  = 38, // The pencil angle amount property. Double value representing the angle amount.
    kPencilScale                  = 39, // The pencil scale amount property. Double value representing the scale amount.
    kPencilPattern                = 40, // The pencil pattern amount property. Integer value representing the pencil pattern.
    kPencilColor                  = 41, // The pencil color property. OdCmColor representing the pencil color.
    kBloomThreshold               = 42, // The bloom effect threshold property. Integer value representing the threshold value.
    kBloomRadius                  = 43, // The bloom effect radius. Integer value representing the bloom radius.
    kTintColor                    = 44, // The tint color property. OdCmColor representing the color used for tint effect.
    kFaceAdjustment               = 45, // Boolean value indicating face adjustment enabled. True if enabled; otherwise, false.
    kPostContrast                 = 46, // The post-processing contrast property. Integer value representing the contrast value.
    kPostBrightness               = 47, // The post-processing brightness property. Integer value representing the brightness value.
    kPostPower                    = 48, // Integer value.
    kTintEffect                   = 49, // Boolean value indicating tint effect enabled. True if enabled; otherwise, false.
    kBloomIntensity               = 50, // The bloom intensity property. Integer value representing the intensity of the bloom effect.
    kColor                        = 51, // OdCmColor value.
    kTransparency                 = 52, // Double value.
    kEdgeWiggleAmount             = 53, // The edge wiggle amount property. See the WiggleAmount enumeration.
    kEdgeTexturePath              = 54, // The edge texture path property. String value representing the path to texture. 
    kDepthOfField                 = 55, // Boolean value indicating depth of field effect enabled. True if enabled; otherwise, false.
    kFocusDistance                = 56, // The focus distance property. Double value indicating the focus distance value.
    kFocusWidth                   = 57, // The focus width property. Double value indicating the focus width value.

    kPropertyCount                      // The number of visual style properties. 
  };

  //
  // Face properties
  //
  /** \details
    Defines the lighting model to apply to the faces of drawables.
  */
  enum FaceLightingModel
  {
    kInvisible            = 0,  // Faces are invisible.
    kConstant             = 1,  // Faces are drawn in a constant color.
    kPhong                = 2,  // Faces are drawn with Phong model to approximate realistic direct lighting.
    kGooch                = 3,  // Faces are drawn with non-photorealistic lighting model for automatic technical illustration.
    // New in 2011
    kZebra                = 4
  };
  
  /** \details
    Defines the calculation method of lighting effects to apply to faces.
  */
  enum FaceLightingQuality
  {
    kNoLighting           = 0,  // No lighting calculations are applied to the faces.
    kPerFaceLighting      = 1,  // Color is calculated for each face based on its orientation to the light sources of the scene.
    kPerVertexLighting    = 2,  // Color is calculated for each vertex of a face based on its orientation to the light sources of the scene.
    kPerPixelLighting     = 3   // Color is calculated for each pixel of a face based on its orientation to the light sources of the scene.
  };
  
  /** \details
    Defines modifications that affect the color of a face before 
    any lighting and shading calculations have been performed.
  */
  enum FaceColorMode    
  {
    kNoColorMode          = 0,  // Apply no modifications to the faces. 
    kObjectColor          = 1,  // Apply the color of the drawable object to the faces of the object.
    kBackgroundColor      = 2,  // Apply the display background color to the faces.
    kMono                 = 3,  // All faces are treated as having a specific color, resulting in a "monochromatic" effect. 
    kTint                 = 4,  // All faces have the hue and saturation values of their colors mapped to the hue and saturation values of a specific color, resulting in a "tinted" effect. The luminance value remains unchanged. 
    kDesaturate           = 5   // Apply a desaturation percentage to the diffuse color of a material.
  };

  /** \details
    Bitfield that describes modifications to faces made before 
    any lighting and shading calculations have been performed.
  */
  enum FaceModifiers    
  {
    kNoFaceModifiers      = 0,  // Apply no face modifiers to the faces. 
    kFaceOpacityFlag      = 1,  // Apply an opacity value to the faces. If unset, faces are displayed with full opacity. If set, faces are displayed with a specified degree of opacity from completely opaque to completely transparent. 
    kSpecularFlag         = 2   // Apply a specular highlight (indicating reflection and shininess values) to a material that is applied to the faces.
  };

  //
  // Edge properties
  //

  /** \details
    Defines the model that is used to display the edges.
  */
  enum  EdgeModel       
  {
    kNoEdges              = 0,  // No edges are displayed.
    kIsolines             = 1,  // Display isolines.
    kFacetEdges           = 2   // Display facet edges (the edges between neighbouring faces).
  };

  /** \details
    Defines a collection of bitflags that represent the combination of edge styles to display.
  */
  enum EdgeStyles       
  {
    kNoEdgeStyle          = 0,  // No edge styles are applied to the edges.
    kVisibleFlag          = 1,  // Visible edges are displayed.
    kSilhouetteFlag       = 2,  // Silhouette edges are displayed. A silhouette edge is an edge connecting a back facing polygon with a front facing polygon.
    kObscuredFlag         = 4,  // Obscured edges are displayed.
    kIntersectionFlag     = 8   // Intersection edges are displayed.
  };

  /** \details
    Defines a collection of modifiers that can be applied to edges to give them various types of effects.
  */
  enum EdgeModifiers    
  {
    kNoEdgeModifiers      =   0,  // No modifiers are applied.
    kEdgeOverhangFlag     =   1,  // Allow an edge to overhang its start and end points by a specified amount. 
    kEdgeJitterFlag       =   2,  // Apply a jitter effect to the edges. The jitter effect is produced by drawing extra lines adjacent to the original line, with a specific amount of perturbation away from the original line.
    kEdgeWidthFlag        =   4,  // Apply a width to the edges.
    kEdgeColorFlag        =   8,  // Apply a color that differs from the drawable object color to the edges. 
    kEdgeHaloGapFlag      =  16,  // Apply a halo gap percentage to the edges of an object that is hidden by another object, such that the specified gap is displayed between the edges of the hidden object and the edges of the object which hides it.
    kAlwaysOnTopFlag      =  64,  // Force edges to be on top. Edges will not participate in the Z-ordering of the displayed drawables.
    kEdgeOpacityFlag      = 128,  // Apply an opacity value to the edges. If unset, edges are displayed with full opacity. If set, edges are displayed with a specified degree of opacity from completely opaque to completely transparent.
    // New in 2013
    kEdgeWiggleFlag       = 256,  // Apply a wiggle value to the edges.
    kEdgeTextureFlag      = 512   // Apply a material textures to the edges.
  };

  /** \details
    Defines a collection of jitter values that can be applied to edges.
  */
  enum EdgeJitterAmount
  {
    kJitterLow            = 1,  // A small amount of perturbation.
    kJitterMedium         = 2,  // A medium amount of perturbation.
    kJitterHigh           = 3   // A large amount of perturbation.
  };

  /** \details
    Defines a collection of wiggle values that can be applied to edges.
  */
  enum EdgeWiggleAmount
  {
    kWiggleLow            = 1,  // A small amount of perturbation.
    kWiggleMedium         = 2,  // A medium amount of perturbation.
    kWiggleHigh           = 3   // A large amount of perturbation.
  };

  /** \details
    Defines a collection of line patterns that can be applied to edges.
  */
  enum EdgeLinePattern
  {
    kSolid                =  1, // A solid line pattern is used to display edges.
    kDashedLine           =  2, // A dashed line pattern is used to display edges.
    kDotted               =  3, // A dotted line pattern is used to display edges.
    kShortDash            =  4, // A short dashed line pattern is used to display edges.
    kMediumDash           =  5, // A medium dashed line pattern is used to display edges.
    kLongDash             =  6, // A long dashed line pattern is used to display edges.
    kDoubleShortDash      =  7, // A double short dashed line pattern is used to display edges.
    kDoubleMediumDash     =  8, // A double medium dashed line pattern is used to display edges.
    kDoubleLongDash       =  9, // A double long dashed line pattern is used to display edges.
    kMediumLongDash       = 10, // A medium long dashed line pattern is used to display edges.
    kSparseDot            = 11  // A sparse dotted line pattern is used to display edges.
  };

  //
  // Display properties
  //
  /** \details
      Defines a combination of display settings that can be enabled.
  */
  enum DisplayStyles
  {
    kNoDisplayStyle       = 0,  // No display settings are used.
    kBackgroundsFlag      = 1,  // Backgrounds are enabled.
    kLightingFlag         = 2,  // Lights are enabled.
    kMaterialsFlag        = 4,  // Materials are enabled.
    kTexturesFlag         = 8   // Textures are enabled.
  };

  /** \details
    Defines shadow types that can be displayed.
  */
  enum DisplayShadowType
  {
    kShadowsNone          = 0,  // No shadow is displayed.
    kShadowsGroundPlane   = 1,  // Ground plane shadows are displayed.
    kShadowsFull          = 2,  // Full shadows are displayed. 
    kShadowsFullAndGround = 3   // Full shadows are displayed along with shadows on the ground plane.
  };
};

/** \details
    Describes what kind of operation to perform when setting a visual style property.

    <group TD_Namespaces>
*/
namespace OdGiVisualStyleOperations
{
  enum Operation
  {
    kInvalidOperation = -1, // Invalid operation.
    kInherit          =  0, // Use the last property value specified.
    kSet              =  1, // Replace the last property value with the one specified. 
    kDisable          =  2, // Disable the specified bits in the property bitmask. 
    kEnable           =  3  // Enable the specified bits in the property bitmask.
  };
};

/** \details
    This class defines a collection of visual properties that can be applied to graphical entities.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiVisualStyle : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiVisualStyle);

/** \details
    Describes the type of visual style.
*/
  enum Type
  {
    kFlat,              // Flat shaded visual style.
    kFlatWithEdges,     // Flat shaded visual style with edges displayed.
    kGouraud,           // Gouraud shaded visual style. 
    kGouraudWithEdges,  // Gouraud shaded visual style with edges displayed.
    k2DWireframe,       // 2D wireframe visual style (using 2D graphics system). 
    k3DWireframe,       // 3D wireframe visual style (using 3D graphics system).
    kHidden,            // Hidden visual style.
    kBasic,             // Basic visual style (default).
    kRealistic,         // Phong shaded visual style.
    kConceptual,        // Custom, user defined visual visual style. 
    kCustom,            // Custom, user defined visual visual style. 
    kDim,               // Visual style used for a dimming effect.
    kBrighten,          // Visual style used for a brightening effect.
    kThicken,           // Visual style used for a thickening effect.
    kLinePattern,       // Visual style used to apply a line pattern.
    kFacePattern,       // Visual style used to apply a face pattern.
    kColorChange,       // Visual style used to apply a change of color.

    kFaceOnly,          // Visual style with only face properties. All non-face properties are set to inherit.
    kEdgeOnly,          // Visual stle with edge properties only. All non-edge properties are set to inherit.
    kDisplayOnly,       // Visual style with display properties only. All non-display properties are set to inherit.

    kJitterOff,         // Edge style override visual style with jitter edges off. All other properties are set to inherit.
    kOverhangOff,       // Edge style override visual style with overhang edges off. All other properties are set to inherit.
    kEdgeColorOff,      // Edge style override visual style with edge color off. All other properties are set to inherit.
    kShadesOfGray,      // Shades of gray visual style.
    kSketchy,           // Sketchy visual style. 
    kXRay,              // Xray visual style. 
    kShadedWithEdges,   // Shade visual style with edges displayed.
    kShaded,            // Shaded visual style.
    kByViewport,        // Visual style by viewport.
    kByLayer,           // Visual style by layer.
    kByBlock,           // Visual style by block.
    kEmptyStyle         // Visual style with all properties set to inherit. This effectively creates an empty style upon which a custom visual style can be built.
  };

  OdGiVisualStyle& operator=(const OdGiVisualStyle& visualStyle);
  bool operator==(const OdGiVisualStyle& visualStyle) const;

  /** \details
    Returns a reference to OdGiFaceStyle of this visual style.
  */
  virtual OdGiFaceStyle& faceStyle (void) = 0;
  /** \details
    Returns a reference to OdGiEdgeStyle of this visual style.
  */
  virtual OdGiEdgeStyle& edgeStyle (void) = 0;
  /** \details
    Returns a reference to OdGiDisplayStyle of this visual style.
  */
  virtual OdGiDisplayStyle& displayStyle (void) = 0;

  /** \details
    Returns a reference to OdGiFaceStyle of this visual style.
  */
  virtual const OdGiFaceStyle& faceStyle (void) const = 0;
  /** \details
    Returns a reference to OdGiEdgeStyle of this visual style.
  */
  virtual const OdGiEdgeStyle& edgeStyle (void) const = 0;
  /** \details
    Returns a reference to OdGiDisplayStyle of this visual style.
  */
  virtual const OdGiDisplayStyle& displayStyle (void) const = 0;

  /** \details
    Copies all properties of OdGiFaceStyle into this visual style.
    \param style [in]  Collection of face properties.
  */
  virtual void setFaceStyle (const OdGiFaceStyle& style) = 0;
  /** \details
    Copies all properties of OdGiEdgeStyle into this visual style.
    \param style [in]  Collection of edge properties.
  */
  virtual void setEdgeStyle (const OdGiEdgeStyle& style) = 0;
  /** \details
    Copies all properties of OdGiDisplayStyle into this visual style.
    \param style [in]  Collection of display properties.
  */
  virtual void setDisplayStyle (const OdGiDisplayStyle& style) = 0;

  /** \details
    Sets up all properties of this visual style to values predefined for the specified visual style type.
    \param type [in]  Visual style type.
    \remarks
    Throws an exception if invalid type of configuration is specified.
  */
  void configureForType (Type type);

  // New interface

  /** \details
    Sets the given visual style type. Multiple properties are modified to change the visual style into the specified type.
    
    \param type [in]  Type of the visual style to set. All available types are defined in the Type enumeration.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setType(Type type) = 0;

  /** \details
    Returns the current visual style type. 
  */
  virtual Type type() const = 0;

  typedef OdGiVisualStyleProperties::Property Property;
  typedef OdGiVisualStyleOperations::Operation Operation;

  /** \details
    Sets the operation associated with the given property.
    
    \param prop [in]  The property for which the operation is to set.
    \param op [in]  The operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, Operation op) = 0;

  /** \details
    Sets a property of the visual style.
    
    \param prop [in]  Property to set.
    \param pVal [in]  Pointer to the OdGiVariant property value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, const OdGiVariant *pVal,
                        Operation op = OdGiVisualStyleOperations::kSet) = 0;

  /** \details
    Sets an integer property of the visual style.
    
    \param prop [in]  Property to set.
    \param nVal [in]  Property value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, OdInt32 nVal,
                        Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
    Sets a boolean property of the visual style.
    
    \param prop [in]  Property to set.
    \param bVal [in]  Property value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, bool bVal,
                        Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
    Sets a double property of the visual style.
    
    \param prop [in]  Property to set.
    \param bVal [in]  Property value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, double dVal,
                        Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
    Sets a color property of the visual style.
    
    \param prop [in]  Color property to set.
    \param red [in]  Red color component value. Valid values are from 0.0 to 1.0.
    \param green [in]  Green component value. Valid values are from 0.0 to 1.0.
    \param blue [in]  Blue component value. Valid values are from 0.0 to 1.0.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, double red, double green, double blue,
                        Operation op = OdGiVisualStyleOperations::kSet);
  /** \details
    Sets a color property of the visual style.
    
    \param prop [in]  Color property to set.
    \param pColor [in]  Reference to the color value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, const OdCmColorBase &pColor,
                        Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
    Sets a color property of the visual style.
    
    \param prop [in]  Color property to set.
    \param pColor [in]  Reference to the color value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, const OdCmEntityColor &pColor,
                        Operation op = OdGiVisualStyleOperations::kSet);
  
  /** \details
    Sets a string property of the visual style.
    
    \param prop [in]  String property to set.
    \param pStr [in]  Reference to string value to set.
    \param op [in]  Operation to use for setting the property.

    \returns
    True if successful, otherwise false.
  */
  virtual bool setTrait(Property prop, const OdString &pStr,
                        Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
    Gets a property of the visual style.
    
    \param prop [in]  Property to get.
    \param pOp [out]  Pointer to the operation used for the property. If NULL, nothing is returned.

    \returns
    OdGiVariantPtr for the property value if successful, otherwise an empty OdGiVariantPtr object.
  */
  virtual OdGiVariantPtr trait(Property prop, Operation *pOp = NULL) const = 0;

  /** \details
    Gets the operation associated with the given property of the visual style.
    
    \param prop [in]  Property for which the associated operation value is to be returned.
    
    \returns
    Operation associated with the specified property if successful, otherwise the kInvalidOperation value.
  */
  virtual Operation operation(Property prop) const = 0;


  /** \details
    Sets a property flag into the visual style (for properties which are bitfield enumerations).
    
    \param flagProp [in]  Bitfield enumeration property to set.
    \param flagVal [in]  Bit flag enumeration property to set into the visual style. 
    \param bEnable [in]  Bool value set to true to enable the flag, false to disable.
    
    \returns
    True if successful, otherwise false.
  */
  virtual bool setTraitFlag(Property flagProp, OdUInt32 flagVal, bool bEnable = true);

  /** \details
    Returns whether the given property flag from the visual style is enabled (for properties which are bitfield enumerations).
  
    \param flagProp [in]  Bitfield enumeration property.
    \param flagVal [in]  Bit flag enumeration property to check if it's enable.
    
    \returns
    True if the flag is enabled, otherwise false.
  */
  virtual bool traitFlag(Property flagProp, OdUInt32 flagVal) const;

  /** \details
    Returns the data type of the given visual style property.
  
    \param prop [in]  Visual style property.
  */
  static OdGiVariant::VariantType propertyType(Property prop);
private:
  void configureForFlat();
  void configureForFlatWithEdges();
  void configureForGouraud();
  void configureForGouraudWithEdges();
  void configureForBasic();
  void configureFor2DWireframe();
  void configureFor3DWireframe();
  void configureForHidden();
  void configureForRealistic();
  void configureForConceptual();
  void configureForDim();
  void configureForBrighten();
  void configureForThicken();
  void configureForLinePattern();
  void configureForFacePattern();
  void configureForColorChange();

  // Extended configurations
  void configureForFaceOnly();
  void configureForEdgeOnly();
  void configureForDisplayOnly();

  void configureForJitterOff();
  void configureForOverhangOff();
  void configureForEdgeColorOff();
  void configureForShadesOfGray();
  void configureForSketchy();
  void configureForXRay();
  void configureForShadedWithEdges();
  void configureForShaded();
  void configureForByViewport();
  void configureForByLayer();
  void configureForByBlock();
  void configureForEmptyStyle();

  // Extended flags configuration
  void configureForAc27(bool b2d = false);

  // Clear all changes which is made before
  void configureForInit();
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiVisualStyle object pointers.
*/
typedef OdSmartPtr<OdGiVisualStyle> OdGiVisualStylePtr;

/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiVisualStyleTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiVisualStyleTraits);

  virtual void setOdGiVisualStyle(const OdGiVisualStyle& visualStyle) = 0;
  virtual const OdGiVisualStyle& odgiVisualStyle() const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiVisualStyleTraits object pointers.
*/
typedef OdSmartPtr<OdGiVisualStyleTraits> OdGiVisualStyleTraitsPtr;

#include "TD_PackPop.h"

#endif //__ODGIVISUALSTYLE_H__
