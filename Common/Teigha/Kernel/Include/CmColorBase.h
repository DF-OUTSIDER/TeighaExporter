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
#ifndef ODA_CMBASE_COLOR
#define ODA_CMBASE_COLOR

#include "OdString.h"
#include "Gi/Gi.h"

#include "TD_PackPush.h"

class OdDbDwgFiler;

/** \details
    <group OdCm_Classes>
    
    This class implements the entity color object that represents the unnamed colors specified by 
    the byLayer, byBlock, byColor, byACI, byPen, Foreground, byDgnIndex, or None color methods.

    \remarks
    The entity color object can specify any of the following color methods:

    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    An entity has no color from the time it is first instantiated until it is assigned one or added 
    to a database. The ColorMethod enumeration defines the constants for color methods.
 
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_overview.html, Overview of Classes that Implement Color and Transparency>

    <link cm_color_method.html, Methods of the Color Definition>

    The OdCmColorBase class,  the OdCmColor class
*/
class FIRSTDLL_EXPORT OdCmEntityColor
{
public:
  ODRX_HEAP_OPERATORS();
  enum Color
  {
    kRed,
    kGreen,
    kBlue
  };
  
  /** \details
    Represents color method.
  */  
  enum ColorMethod
  {
    /**Color of the layer on which it resides.*/
    kByLayer    = 0xC0,
    /**Color of the block reference in which it is contained.*/
    kByBlock    = 0xC1,
    /**Color specified by RGB value.*/
    kByColor    = 0xC2,
    /**Color specified by an index (ACI) into a color palette.*/
    kByACI      = 0xC3,
    /**Color specified by an index into a pen color table.*/
    kByPen      = 0xC4,
    /**Editor foreground Color.*/
    kForeground = 0xC5,
    /**Color specified by an index into a dgn color table.*/
    kByDgnIndex = 0xC7,
    /**No Color.*/
    kNone       = 0xC8
  };
  
  enum ACIcolorMethod
  {
    kACIbyBlock     = 0,    // byBlock
    kACIforeground  = 7,    // Foreground
    kACIbyLayer     = 256,  // byLayer

    kACIclear       = 0,    // Clear

    kACIRed         = 1,    // Red
    kACIYellow      = 2,    // Yellow
    kACIGreen       = 3,    // Green
    kACICyan        = 4,    // Cyan
    kACIBlue        = 5,    // Blue
    kACIMagenta     = 6,    // Magenta
    kACIWhite       = 7,    // White

    kACIstandard    = 7,    
    kACImaximum     = 255,
    kACInone        = 257,  // None
    kACIminimum     = -255
  };
  
  typedef OdUInt32 RGBM;
  
  /** \param colorMethod [in]  color method.
    \param red [in]  Red component.
    \param green [in]  Green component.
    \param blue [in]  Blue component.
    
    \remarks
    Default ColorMethod is kByColor. 
  */
  OdCmEntityColor() : m_RGBM(0) { setColorMethod(kByColor); }
  OdCmEntityColor(
    const OdCmEntityColor & color) : m_RGBM(color.m_RGBM) { }
  OdCmEntityColor(
    ColorMethod colorMethod) : m_RGBM(0) { setColorMethod(colorMethod); }
  OdCmEntityColor(
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue)
  {
    setColorMethod(kByColor);
    setRGB(red, green, blue);
  }

  /** \details
    Copies an entity color object specified by right into the entity color object specified by left.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>
  */
    OdCmEntityColor& operator =(
    const OdCmEntityColor& color)
  {
    m_RGBM = color.m_RGBM;
    return *this;
  }

  /** \details
    Compares two entity color objects as integer values and returns true when their values are equal, 
    or false when their values are not equal.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
    bool operator ==(
    const OdCmEntityColor& color) const
  {
    return m_RGBM == color.m_RGBM;
  }

  /** \details
    Compares two entity color objects as integer values and returns true when their values are not equal, 
    or false when their values are equal.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
    bool operator !=(
    const OdCmEntityColor& color) const
  {
    return m_RGBM != color.m_RGBM;
  }

  /** \details
    Sets the color method for the entity color object.

    \param colorMethod [in]  color method as the ColorMethod enumeration.

    \remarks
    The color method can be a one of the following: 
    
    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  void setColorMethod(
    ColorMethod colorMethod) { OdCmEntityColor::setColorMethod(&m_RGBM, colorMethod); }

  /** \details
    Returns the color method of the entity color object as a value of the ColorMethod enumeration.

    \remarks
    The color method can be a one of the following:
     
    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  ColorMethod colorMethod() const
  {
    return colorMethod(&m_RGBM);
  } 
  
  /** \details
    Sets the color method and color components for the entity color object as an integer value.

    \param color [in]  A packed 32-bits integer value that specifies the color method and color components.
 
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
  void setColor(
    OdUInt32 color);

  /** \details
    Returns the packed 32-bits integer value that stores the color method and color components
    of the entity color object.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
  OdUInt32 color() const { return m_RGBM; } 
  
  /** \details
    Sets the color index (ACI) of a some color palette and sets the color method to byACI 
    for the entity color object.

    \param colorIndex [in]  An integer value that is the index of the color in a some palette.
    
    \remarks
    The color index can be a one of the following:

    <table>
    Name              Value   Description
    _kACIbyBlock_       0       Sets the color method to byBlock.
    _kACIRed_           1       Red. 
    _kACIYellow_        2       Yellow. 
    _kACIGreen_         3       Green. 
    _kACICyan_          4       Cyan. 
    _kACIBlue_          5       Blue. 
    _kACIMagenta_       6       Magenta. 
    _kACIforeground_    7       Sets the color method to Foreground.
    ..                8-255   Defined by display device.
    _kACIbyLayer_       256     Sets the color method to byLayer. 
    _kACInone_          257     Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  void setColorIndex(
    OdInt16 colorIndex) { setColorIndex(&m_RGBM, colorIndex); }

  /** \details
    Returns the color index (ACI) of the entity color object.

    \remarks
    The color index can be a one of the following:

    <table>
    Name              Value   Description
    _kACIbyBlock_       0       Sets the color method to byBlock.
    _kACIRed_           1       Red. 
    _kACIYellow_        2       Yellow. 
    _kACIGreen_         3       Green. 
    _kACICyan_          4       Cyan. 
    _kACIBlue_          5       Blue. 
    _kACIMagenta_       6       Magenta. 
    _kACIforeground_    7       Sets the color method to Foreground.
    ..                8-255   Defined by display device.
    _kACIbyLayer_       256     Sets the color method to byLayer. 
    _kACInone_          257     Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  OdInt16 colorIndex() const { return OdCmEntityColor::colorIndex(&m_RGBM); }
  
  
  /** \details
    Sets the red, green, blue color components and the color method to byColor for the entity color object.
    
    \param red [in]  Red component as an integer value in range 0 to 255.
    \param green [in]  Green component as an integer value in range 0 to 255.
    \param blue [in]  Blue component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  void setRGB(
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue)
  {
    setRGB(&m_RGBM, red, green, blue);
  }    

  /** \details
    Sets the red color component for the entity color object.
    
    \param red [in]  Red component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  void setRed(
    OdUInt8 red) { setRed(&m_RGBM, red); }

  /** \details
    Sets the green color component for the entity color object.
    
    \param green [in]  Green component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  void setGreen(
    OdUInt8 green) { setGreen(&m_RGBM, green); }

  /** \details
    Sets the blue color component for the entity color object.
    
    \param blue [in]  Blue component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  void setBlue(
    OdUInt8 blue) { setBlue(&m_RGBM, blue); }

  /** \details
    Returns the red color component of the entity color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  OdUInt8 red() const { return red(&m_RGBM); }
  
  /** \details
    Returns the green color component of the entity color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  OdUInt8 green() const { return green(&m_RGBM); }

  /** \details
    Returns the blue color component of the entity color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  OdUInt8 blue() const { return blue(&m_RGBM); }
  
  // Method check
  
  /** \details
    Checks whether the color method is byColor for the entity color object and 
    returns true if and only if the color method is set to _kByColor_, otherwise it returns false.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isByColor() const { return isByColor(&m_RGBM); }

  /** \details
    Checks whether the color method is byLayer for the entity color object and 
    returns true if and only if the color method is set to _kByLayer_ or was set to _kACIbyLayer_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isByLayer() const { return isByLayer(&m_RGBM); }

  
  /** \details
    Checks whether the color method is byBlock for the entity color object and 
    returns true if and only if the color method is set to _kByBlock_ or was set to _kACIbyBlock_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isByBlock() const { return isByBlock(&m_RGBM); }
   

  /** \details
    Checks whether the color method is byACI for the entity color object and 
    returns true if and only if the color method is set to _kByACI_, otherwise it returns false.
    
    \remarks
    This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isByACI() const { return isByACI(&m_RGBM); }

  /** \details
    Checks whether the color method is Foreground for the entity color object and 
    returns true if and only if the color method is set to _kForeground_ or was set to _kACIforeground_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isForeground() const { return isForeground(&m_RGBM); }

  /** \details
    Checks whether the color method is byDgnIndex for the entity color object and 
    returns true if and only if the color method is set to _kByDgnIndex_, otherwise it returns false.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isByDgnIndex() const { return isByDgnIndex(&m_RGBM); }

  /** \details
    Checks whether the color method is None (invisible) for the entity color object and 
    returns true if and only if the color method is set to _kNone_ or was set to _kACInone_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  bool isNone() const  { return isNone(&m_RGBM); }
  
  /** \details
    Returns the TrueColor value of the entity color object.

    \sa
    <link cm.html, Working with Colors and Transparencies>
  */
  OdUInt32 trueColor() const;
  
  /** \details
    Sets the color of the calling entity with the entity color object.

    \sa
    <link cm.html, Working with Colors and Transparencies>
  */
  void setTrueColor();
  
  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and returns the ColorMethod value.
    
    \remarks
    Assumes the current color method is ACIcolorMethod.

    \sa
    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  OdUInt8 trueColorMethod() const;

  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and 
    assumes the current color method to ACIcolorMethod.

    \sa
    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  void setTrueColorMethod();
  
  /** \details
    Sets the color method for the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param colorMethod [in]  color method as the ColorMethod enumeration.

    \remarks
    The color method can be a one of the following: 
    
    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  static void setColorMethod(
    RGBM* pRGBM, 
    ColorMethod colorMethod);

  
  /** \details
    Returns the color method of the entity color object as a value of the ColorMethod enumeration.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \remarks
    The color method can be a one of the following:
     
    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  static ColorMethod colorMethod(
    const RGBM* pRGBM)
  {
    return OdCmEntityColor::ColorMethod((*pRGBM >> 24) & 0xFF);
  }
  
  /** \details
    Sets the color method and color components for the entity color object as an integer value.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param color [in]  A packed 32-bits integer value that specifies the color method and color components.
 
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */ 
  static void setColor(
    RGBM* pRGBM, 
    OdUInt32 color);
    
  /** \details
    Returns the packed 32-bits integer value that stores the color method and color components
    of the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
   
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
  static OdUInt32 color(
    const RGBM* pRGBM); 
  
  /** \details
    Sets the color index (ACI) of a some color palette and sets the color method to byACI 
    for the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param colorIndex [in]  An integer value that is the index of the color in a some palette.
    
    \remarks
    The color index can be a one of the following:

    <table>
    Name              Value   Description
    _kACIbyBlock_       0       Sets the color method to byBlock.
    _kACIRed_           1       Red. 
    _kACIYellow_        2       Yellow. 
    _kACIGreen_         3       Green. 
    _kACICyan_          4       Cyan. 
    _kACIBlue_          5       Blue. 
    _kACIMagenta_       6       Magenta. 
    _kACIforeground_    7       Sets the color method to Foreground.
    ..                8-255   Defined by display device.
    _kACIbyLayer_       256     Sets the color method to byLayer. 
    _kACInone_          257     Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  static void setColorIndex(
    RGBM* pRGBM, 
    OdInt16 colorIndex);

    /** \details
    Sets the dgn color index (DgnIndex) of a some color palette and sets the color method to byDgnIndex 
    for the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param colorIndex [in]  An integer value that is the index of the color in a some palette.
  */
  static void setDgnColorIndex(
    RGBM* pRGBM, 
    OdInt16 colorIndex);
    
  /** \details
    Returns the color index (ACI) of the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
   
    \remarks
    The color index can be a one of the following:

    <table>
    Name              Value   Description
    _kACIbyBlock_       0       Sets the color method to byBlock.
    _kACIRed_           1       Red. 
    _kACIYellow_        2       Yellow. 
    _kACIGreen_         3       Green. 
    _kACICyan_          4       Cyan. 
    _kACIBlue_          5       Blue. 
    _kACIMagenta_       6       Magenta. 
    _kACIforeground_    7       Sets the color method to Foreground.
    ..                8-255   Defined by display device.
    _kACIbyLayer_       256     Sets the color method to byLayer. 
    _kACInone_          257     Sets the color method to None.
    </table>

    An entity does not have a color from the time it is first instantiated until it is assigned one
    or added to a database.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  static OdInt16 colorIndex(
    const RGBM* pRGBM);
  
    
  /** \details
    Sets the red, green, blue color components and the color method to byColor for the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param red [in]  Red component as an integer value in range 0 to 255.
    \param green [in]  Green component as an integer value in range 0 to 255.
    \param blue [in]  Blue component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  static void setRGB(
    RGBM* pRGBM, 
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue)
  {
    setColorMethod(pRGBM, kByColor);
    setRed(pRGBM, red);
    setGreen(pRGBM, green);
    setBlue(pRGBM, blue);
  }
    
  /** \details
    Sets the red color component for the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param red [in]  Red component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */ 
  static void setRed(
    RGBM* pRGBM, 
    OdUInt8 red)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor); 
    *pRGBM &= 0xFF00FFFF;
    *pRGBM |= RGBM(((OdUInt32)red << 16) & 0x00FF0000);
  }
      
  /** \details
    Sets the green color component for the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param green [in]  Green component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  static void setGreen(
    RGBM* pRGBM, 
    OdUInt8 green)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor); 
    *pRGBM &= 0xFFFF00FF;
    *pRGBM |= RGBM(((OdUInt32)green << 8) & 0x0000FF00);
  }
    
    
  /** \details
    Sets the blue color component for the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \param blue [in]  Blue component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  static void setBlue(
    RGBM* pRGBM, 
    OdUInt8 blue)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor); 
    *pRGBM &= 0xFFFFFF00;
    *pRGBM |= RGBM((blue) & 0x000000FF);
  }    
    
  /** \details
    Returns the red color component of the entity color object as a value in range 0 to 255.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  static OdUInt8 red(
    const RGBM* pRGBM)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor); 
    return OdUInt8((*pRGBM >> 16) & 0xFF);
  }
    
  /** \details
    Returns the green color component of the entity color object as a value in range 0 to 255.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  static OdUInt8 green(
    const RGBM* pRGBM)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor); 
    return OdUInt8((*pRGBM >> 8) & 0xFF);
  }
   
  /** \details
    Returns the blue color component of the entity color object as a value in range 0 to 255.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */   
  static OdUInt8 blue(
    const RGBM* pRGBM)
  {
    ODA_ASSERT(colorMethod(pRGBM) == kByColor); 
    return OdUInt8(*pRGBM & 0xFF);
  }
    
    
  /** \details
    Checks whether the color method is byColor for the entity color object and 
    returns true if and only if the color method is set to _kByColor_, otherwise it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */ 
  static bool isByColor(
    const RGBM* pRGBM) { return colorMethod(pRGBM) == kByColor; }  
    
  /** \details
    Checks whether the color method is byLayer for the entity color object and 
    returns true if and only if the color method is set to _kByLayer_ or was set to _kACIbyLayer_, 
    otherwise it returns false.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */ 
  static bool isByLayer(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kByLayer || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIbyLayer)); 
  }    
    
  /** \details
    Checks whether the transparency method is byBlock for the transparency object and 
    returns true if and only if the transparency method is set to _kByBlock_, otherwise it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */ 
  static bool isByBlock(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kByBlock || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIbyBlock)); 
  }    
   
  /** \details
    Checks whether the color method is byACI for the entity color object and 
    returns true if and only if the color method is set to _kByACI_, otherwise it returns false.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    
    \remarks
    This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  static bool isByACI(
    const RGBM* pRGBM)
  {
    return colorMethod(pRGBM) == kByACI;
  }
    
  /** \details
    Checks whether the color method is Foreground for the entity color object and 
    returns true if and only if the color method is set to _kForeground_ or was set to _kACIforeground_, 
    otherwise it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  static bool isForeground(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kForeground || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIforeground)); 
  }
    
  /** \details
    Checks whether the color method is byDgnIndex for the entity color object and 
    returns true if and only if the color method is set to _kByDgnIndex_, otherwise it returns false.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  static bool isByDgnIndex(
    const RGBM* pRGBM)
  {
    return colorMethod(pRGBM) == kByDgnIndex;
  }
    
  /** \details
    Checks whether the color method is None (invisible) for the entity color object and 
    returns true if and only if the color method is set to _kNone_ or was set to _kACInone_, 
    otherwise it returns false.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  static bool isNone(
    const RGBM* pRGBM)
  {
    return (colorMethod(pRGBM) == kNone || (colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACInone)); 
  }
     
  /** \details
    Returns the TrueColor value of the entity color object.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm.html, Working with Colors and Transparencies>
  */ 
  static OdUInt32 trueColor(
    const RGBM* pRGBM);
    
  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and returns the ColorMethod value.
    
    \param aciColorMethod [in]  ACIcolorMethod value.
    
    \remarks
    Assumes the current color method is ACIcolorMethod.

    \sa
    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  static OdUInt8 trueColorMethod(
    OdInt16 aciColorMethod);
    
  /** \details
    Sets the color of the calling entity with the entity color object.
    
    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
  
    \sa
    <link cm.html, Working with Colors and Transparencies>
  */
  static void setTrueColor(
    RGBM* pRGBM);
    
  /** \details
    Converts the color method from ACIcolorMethod to ColorMethod and 
    assumes the current color method to ACIcolorMethod.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
    
    \sa
    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  static void setTrueColorMethod(
    RGBM* pRGBM);
  
  /** \details
    Converts the specified color index (ACI-value) to the RGB-value. 
    
    \remarks
    For converting, this method uses the default palette specified by the mLUT static array
    and returns the RGB-value without color method as a packed integer value in which:

    <table>
    Byte      Description
    0         Blue component.
    1         Green component.
    2         Red component.
    </table>

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  static OdUInt32 lookUpRGB(
    OdUInt8 colorIndex);
  
  /** \details
    Converts the  RGB-value specified by the red, green, blue components to the ACI-value. 

    \param red [in]  Red component an an integer value in range 0 to 255.
    \param green [in]  Green component an an integer value in range 0 to 255.
    \param blue [in]  Blue component an an integer value in range 0 to 255.

    \remarks
    For converting, this method uses the default palette specified by the mLUT static array
    and returns the color index without color method as an integer value in range 0 to 257.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  static OdUInt8 lookUpACI(
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue); 
  
protected:
  /*!DOM*/
  static OdInt16 indirect(
    const RGBM* pRGBM) 
  {
    ODA_ASSERT(colorMethod(pRGBM) != kByColor); 
    return OdInt16((*pRGBM) & 0x0000FFFF);
  }

  /*!DOM*/
  static void setIndirect(
    RGBM* pRGBM, 
    OdInt16 indirect)
  {
    ODA_ASSERT(colorMethod(pRGBM) != kByColor); 
    *pRGBM = ((*pRGBM & 0xFF000000) | (OdUInt32(indirect) & 0x0000FFFF));
  }
      
  /*!DOM*/
  OdInt16 indirect() const { return indirect(&m_RGBM); }

  /*!DOM*/
  void setIndirect(OdInt16 indirect) { setIndirect(&m_RGBM, indirect); }

  RGBM m_RGBM;
  
public:
  /** \details
      Maps the colors between ACI-values and RGB-values.
  */
  FIRSTDLL_EXPORT_STATIC static const OdUInt8 mLUT[256][3]; 
};


/** \details
   <group OdCm_Classes>

    This class is a virtual interface for classes that represent the unnamed and named colors
    specified by the byLayer, byBlock, byColor, byACI, byPen, Foreground, byDgnIndex, or None 
    color methods.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_overview.html, Overview of Classes that Implement Color and Transparency>

    The OdCmEntityColor class,  the OdCmColor class
*/
class FIRSTDLL_EXPORT OdCmColorBase
{
public:
  ODRX_HEAP_OPERATORS();
  virtual ~OdCmColorBase() {}

  /** \details
    Returns the color method of the database color object as a value of the ColorMethod enumeration.

    \remarks
    The color method can be a one of the following:
     
    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    \remarks
    Using of this method by third-party applications is neither supported nor recommended.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual OdCmEntityColor::ColorMethod colorMethod() const = 0;
  

  /** \details
    Sets the color method the database color object using a value of the ColorMethod enumeration.

    \param colorMethod [in]  color method as the ColorMethod enumeration.

    \remarks
    The color method can be a one of the following:
    
    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    \remarks
    Using of this method by third-party applications is neither supported nor recommended.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual void  setColorMethod(
    OdCmEntityColor::ColorMethod colorMethod) = 0;


  /** \details
    Checks whether the color method is byColor for the database color object and 
    returns true if and only if the color method is set to _kByColor_, otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual bool isByColor() const = 0;
    
  /** \details
    Checks whether the color method is byLayer for the database color object and 
    returns true if and only if the color method is set to _kByLayer_ or was set to _kACIbyLayer_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual bool isByLayer() const = 0;
  
  /** \details
    Checks whether the color method is byBlock for the database color object and 
    returns true if and only if the color method is set to _kByBlock_ or was set to _kACIbyBlock_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual bool isByBlock() const = 0;

  /** \details
    Checks whether the color method is byACI for the database color object and 
    returns true if and only if the color method is set to _kByACI_, otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \remarks
    This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual bool isByACI() const = 0;

  /** \details
    Checks whether the color method is Foreground for the database color object and 
    returns true if and only if the color method is set to _kForeground_ or was set to _kACIforeground_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual bool isForeground()   const = 0;

  /** \details
    Checks whether the color method is byDgnIndex for the database color object and 
    returns true if and only if the color method is set to _kByDgnIndex_, otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
  */
  virtual bool isByDgnIndex()   const = 0;

  /** \details
    Returns the packed 32-bits integer value that stores the color method and color components
    of the database color object.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
  virtual OdUInt32  color() const = 0;
  
  /** \details
    Sets the color method and color components for the database color object as an integer value.

    \param color [in]  A packed 32-bits integer value that specifies the color method and color components.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
  virtual void setColor(
    OdUInt32 color) = 0;

  /** \details
    Sets the red, green, blue color components and the color method to byColor for the database color object.

    \param red [in]  Red component as an integer value in range 0 to 255.
    \param green [in]  Green component as an integer value in range 0 to 255.
    \param blue [in]  Blue component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  virtual void setRGB(
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue) = 0;
                           
  /** \details
    Sets the red color component for the database color object.
    
    \param red [in]  Red component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  virtual void setRed(
    OdUInt8 red) = 0;
    
  /** \details
    Sets the green color component for the database color object.
    
    \param green [in]  Green component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  virtual void setGreen(
    OdUInt8 green) = 0;
    
  /** \details
    Sets the blue color component for the database color object.
    
    \param blue [in]  Blue component as an integer value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  virtual void setBlue(
    OdUInt8 blue) = 0;
    
  /** \details
    Returns the red color component of the database color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  virtual OdUInt8 red() const = 0;

   /** \details
    Returns the green color component of the database color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
   */
  virtual OdUInt8 green() const = 0;

  /** \details
    Returns the blue color component of the database color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
  */
  virtual OdUInt8 blue() const = 0;

  /** \details
    Returns the color index (ACI) of the database color object.

    \remarks
    The color index can be a one of the following:

    <table>
    Name              Value   Description
    _kACIbyBlock_       0       Sets the color method to byBlock.
    _kACIRed_           1       Red. 
    _kACIYellow_        2       Yellow. 
    _kACIGreen_         3       Green. 
    _kACICyan_          4       Cyan. 
    _kACIBlue_          5       Blue. 
    _kACIMagenta_       6       Magenta. 
    _kACIforeground_    7       Sets the color method to Foreground.
    ..                8-255   Defined by display device.
    _kACIbyLayer_       256     Sets the color method to byLayer. 
    _kACInone_          257     Sets the color method to None.
    </table>

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  virtual OdUInt16  colorIndex() const = 0;

  /** \details
    Sets the color index (ACI) of a some color palette and sets the color method to byACI 
    for the database color object.

    \param colorIndex [in]  An integer value that is the index of the color in a some palette.
    
    \remarks
    The color index can be a one of the following:

    <table>
    Name              Value   Description
    _kACIbyBlock_       0       Sets the color method to byBlock.
    _kACIRed_           1       Red. 
    _kACIYellow_        2       Yellow. 
    _kACIGreen_         3       Green. 
    _kACICyan_          4       Cyan. 
    _kACIBlue_          5       Blue. 
    _kACIMagenta_       6       Magenta. 
    _kACIforeground_    7       Sets the color method to Foreground.
    ..                8-255   Defined by display device.
    _kACIbyLayer_       256     Sets the color method to byLayer. 
    _kACInone_          257     Sets the color method to None.
    </table>

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>

    <link cm_color_index.html, Color Functionality as an ACI-value>
  */
  virtual void setColorIndex(
    OdUInt16 colorIndex) = 0;

  /** \details
    Converts to the named color and sets the color name and book name for the database color object.
    
    \param colorName [in]  Color name as a string value.
    \param bookName [in]  Book name as a string value.

    \returns
    Returns true if and only if successful.
    If the book name is an empty string that the method sets the UNNAMED name. 
    If the color name is an empty string that the method ignores the specified values.
    If the book name and the color name are an empty string together that the method converts to the unnamed color.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>

    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
  virtual bool  setNames(
    const OdString& colorName,
    const OdString& bookName = OdString::kEmpty) = 0;
    
  /** \details
    Returns the Color name of the database color object.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
  virtual OdString colorName() const = 0;

  /** \details
    Returns the Book name of the database color object.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
  virtual OdString bookName() const = 0;
  
  /** \details
    Returns the display color name of the database color object.
    
    \remarks
    For named colors, this is the same as colorName(). For unnamed colors, it is an 'appropriate' name.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
  virtual OdString colorNameForDisplay() const = 0;
};


/** \details
    <group OdCm_Classes>

    This class implements the Transparency object, which provides the transparency information about OdGiDrawable objects.
   
    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_overview.html, Overview of Classes that Implement Color and Transparency>

    <link cm_transparency_method.html, Methods of the Transparency Definition>
*/
class OdCmTransparency 
{
public:
  ODRX_HEAP_OPERATORS();
  
  enum transparencyMethod 
  {
    kByLayer    = 0, // Use the setting for the layer.
    kByBlock    = 1, // Use the setting for the block.
    kByAlpha    = 2, // Use the Alpha-value in this object.     
    kErrorValue = 3  // Error value.
  };
  
  /** \remarks
    The default transparency method is _kByLayer_.
  */
  OdCmTransparency() { setMethod(kByLayer); }
  OdCmTransparency(transparencyMethod method) { setMethod(method); }
  OdCmTransparency(OdUInt8 alpha) { setAlpha(alpha); }
  OdCmTransparency(double alphaPercent) { setAlphaPercent(alphaPercent); }
  OdCmTransparency(
    const OdCmTransparency& transparency) { m_AM = transparency.m_AM; }
  ~OdCmTransparency() {}
  
  OdCmTransparency& operator=(
    const OdCmTransparency& transparency) { m_AM = transparency.m_AM; return *this; }

  /** \details
    Compares two transparency objects as integer values and returns true when their values are equal, 
    or false when their values are not equal.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
  */
    bool operator==(
    const OdCmTransparency& transparency) const { return (m_AM == transparency.m_AM); }

  /** \details
    Compares two transparency objects as integer values and returns true when their values are not equal, 
    or false when their values are equal.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
  */
    bool operator!=(
    const OdCmTransparency& transparency) const { return (m_AM != transparency.m_AM); }
  
  /** \details
    Sets the alpha value and switches the transparency method to _kByAlpha_ for the transparency object.
    
    \param alpha [in]  Alpha as an integer value in range 0 to 255.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha == 0 means totally clarity. Alpha == 255 means totally opaque.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
  */
  void setAlpha(
    OdUInt8 alpha)
  {
    m_AM = ((OdUInt32)kByAlpha << 24) | alpha;
  }

  /** \details
    Returns the alpha value in range 0 to 255 of the transparency object.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha == 0 means totally clarity. Alpha == 255 means totally opaque.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
  */
  OdUInt8 alpha() const
  {
    if (!isByAlpha())
      return 255;
    return OdUInt8(m_AM & 0xFF);
  }

  /** \details
    Sets the alpha value in percent and switches the transparency method to _kByAlpha_ for the transparency object.
    
    \param alphaPercent [in]  Alpha as a double value in range 0.0 (=0) to 1.0 (=255).

    \remarks
    An alpha value defines the degree of transparency.
    Alpha == 0.0 means totally clarity. Alpha == 1.0 means totally opaque.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
  */
  void setAlphaPercent(
    double alphaPercent)
  {
    if (alphaPercent < 0.0)
      setAlpha(0);
    else if (alphaPercent > 1.0)
      setAlpha(0xFF);
    else
      setAlpha((OdUInt8)(alphaPercent * 255));
  }

  /** \details
    Returns the alpha value in range 0.0 to 1.0 of the transparency object.

    \remarks
    An alpha value defines the degree of transparency.
    Alpha == 0.0 means totally clarity. Alpha == 1.0 means totally opaque.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
  */
  double alphaPercent() const
  {
    return (double)alpha() / 255;
  }


  /** \details
    Returns the transparency method of the transparency object as a value of the transparencyMethod enumeration.
    
    \remarks
    The transparency method can be a one of the following:
    
    <table>
    Name        Value   Description
    _kByLayer_    0       Transparency is specified by the layer object to which the object is resided.
    _kByBlock_    1       Transparency is specified by the block object in which the object is contained.
    _kByAlpha_    2       Transparency is specified by an Alpha-value.
    </table>

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */
  transparencyMethod method() const
  {
    return transparencyMethod(OdUInt8(m_AM >> 24));
  }

  /** \details
    Sets the transparency method for the transparency object.
    
    \param method [in]  Transparency method as the transparencyMethod enumeration.
    
    \remarks
    The transparency method can be a one of the following:
    
    <table>
    Name        Value   Description
    _kByLayer_    0       Transparency is specified by the layer object to which the object is resided.
    _kByBlock_    1       Transparency is specified by the block object in which the object is contained.
    _kByAlpha_    2       Transparency is specified by an Alpha-value.
    </table>

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */
  void setMethod(
    transparencyMethod method)
  {
    m_AM = (OdUInt32)method << 24;
  }

  /** \details
    Checks whether the transparency method is byAlpha for the transparency object and 
    returns true if and only if the transparency method is set to _kByAlpha_, otherwise it returns false.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */
  bool isByAlpha() const { return (method() == kByAlpha); }

  /** \details
    Checks whether the transparency method is byBlock for the transparency object and 
    returns true if and only if the transparency method is set to _kByBlock_, otherwise it returns false.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */
  bool isByBlock() const { return (method() == kByBlock); }
  
  /** \details
    Checks whether the transparency method is byLayer for the transparency object and 
    returns true if and only if the transparency method is set to _kByLayer_, otherwise it returns false.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */
  bool isByLayer() const { return (method() == kByLayer); }

  /** \details
    Checks whether the transparency method is error value for the transparency object and 
    returns true if and only if the transparency method is set to _kErrorValuer_, otherwise it returns false.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>
  */
  bool isInvalid() const { return (method() == kErrorValue); }

  /** \details
    Checks whether the transparency method is byAlpha and object is totally clarity for the transparency object.
    It returns true if and only if the transparency method is set to _kByAlpha_ and Alpha == 0, 
    otherwise it returns false.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>

    <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
  */
  bool isClear() const   { return (method() == kByAlpha) && (alpha() == 0);}
  
  /** \details
    Checks whether the transparency method is byAlpha and object is totally opaque for the transparency object.
    It returns true if and only if the transparency method is set to _kByAlpha_ and Alpha == 255, 
    otherwise it returns false.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>

    <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
  */
  bool isSolid() const   { return (method() == kByAlpha) && (alpha() == 255);}
  

  /** \details
    Returns the packed 32-bits integer value that stores the transparency method and transparency components.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>

    <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
  */
  OdUInt32 serializeOut() const { return m_AM; }

  /** \details
    Sets the transparency method and transparency components as an integer value for the transparency object.

    \param transparency [in]  A packed 32-bits integer value that specifies the transparency method and transparency components.

    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_transparency_method.html, Methods of the Transparency Definition>

    <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
  */
  void serializeIn(
    OdUInt32 transparency) { m_AM = transparency; }

  /** \details
    Reads the .dwg file format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
  */
  void dwgIn(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
  */
  void dwgOut(
    OdDbDwgFiler* pFiler) const;

private: 
  OdUInt32 m_AM;
};

#include "TD_PackPop.h"

#endif //ODA_CMBASE_COLOR
