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


///////////////////////////////////////////////////////////////////////////////
//
// PdfPublishRadioButton.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_RADIOBUTTON_INCLUDED_
#define _PDF_PUBLISH_RADIOBUTTON_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a radio button control contained in a .pdf document created with Teigha Publish.
  A radio button represent a dependent option to be selected.
  Option is called dependent because if several options are joined into a group, only one option from the group can be selected. 
  Other options become unselected.
*/
class PDFPUBLISH_TOOLKIT OdRadioButton : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdRadioButton();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRadioButton);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the radio button.
  */
  virtual ~OdRadioButton();

/** \details
    Sets the radio button name.
    
    \param name [in] A new radio button name.

    \remarks The name is mandatory and must be unique among the document object names, except for OdRadioButton in same group.
  */
  void setName(const OdString& name);

  void setFont(Text::StandardFontsType fontType);

  /** \details
    Sets font name and type of the radio button.
    
    \param name  [in] A new font name of the radio button.
    \param style [in] A new font type of the radio button.
    
    \sa
    <link OdPdfPublish::TextFontStyle, Text::FontStyle enumeration>.
  */
  void setFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);

  /** \details
    Sets a font size of the radio button.
    
    \param size [in] A new font size value of the radio button.
  */
  void setFontSize(OdUInt16 size);

  /** \details
    Sets a text color of the radio button.
    
    \param color [in] A new color value.
  */
  void setTextColor(ODCOLORREF color);

  /** \details
    Sets a tooltip text for the radio button.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the radio button.
    
    \param tooptip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);

  /** \details
    Sets a new value of the visibility flag for the radio button.
    Visibility flag determines whether the radio button is visible in document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);

  /** \details
    Sets a new value of the flag that determines whether the radio button should be printed.
    If the radio button should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);

  /** \details
    Sets a new value of the radio button text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa 
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);

  /** \details
    Sets a new value of the flag that determines whether the radio button is locked.
    If the flag value is equal to true, the radio button is locked and becomes disabled otherwise it enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);

  /** \details
    Sets a new value of the flag that determines whether the radio button has a border.
    If the flag value is equal to true, the radio button has a border, otherwise it has not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);

  /** \details
    Sets the radio button border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets the radio button border thickness.
    
    \param thickness [in] A new border thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets the radio button border style.
    
    \param style [in] A new border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);

  /** \details
    Sets the radio button filling color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);

  /** \details
    Sets an export value for the radio button.
    
    \param export_value [in] A new export value.
  */  
  void setExportValue(const OdString& export_value);

  /** \details
    Sets the radio button default state.
    If default state is equal to true, the radio button is switched on when it displayed in a document, otherwise the radio button is switched off.
    
    \param state [in] A new default state.
  */
  void setDefaultState(bool state);

  /** \details
    Sets a new value of the flag that determines whether the radio button is included in a group of radio buttons.
    If the flag value is equal to true, the radio button is in group, otherwise it is a stand alone radio button.
    
    \param state [in] A new flag value.
  */
  void setGrouping(bool state);

  /** \details
    Returns current name of the radio button.
    The method fills a passed string object with radio button name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the the radio button name.
  */
  void getName(OdString& name) const;

  /** \details
    Returns current font name and style used for displaying radio button caption.
    The method fills a passed string object and Text::FontStyle value with current font name and style, then returns them to a calling subroutine.
    
    \param font_name [out] A placeholder for the font name of the radio button.
    \param style     [out] A placeholder for the font style of the radio button.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns current font size used for displaying radio button caption.
    The method fills a passed integer value with the radio button font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for the radio button.
  */
  void getFontSize(OdUInt16& size) const;

  /** \details
    Returns current text color used for displaying radio button caption.
    The method fills a passed ODCOLORREF value with the radio button caption color and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color that should be used for the radio button caption.
  */
  void getTextColor(ODCOLORREF& color) const;

  /** \details
    Returns current tooltip text for the radio button.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the radio button.
    
    The method fills a passed string object with the radio button tooltip text and returns it to a calling subroutine.
    
    \param tooptip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;

  /** \details
    Returns current value of the visibility flag for the radio button.
    Visibility flag determines whether the radio button is visible in document (if true) or is hidden (if false).
    The method fills a passed boolean value with the radio button visibility flag value and returns it to a calling subroutine.    
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;

  /** \details
    Returns current value of the flag that determines whether the radio button should be printed.
    If the radio button should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the radio button printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;

  /** \details
    Returns current value of the radio text rotation.
    The method fills a passed value with current text rotation value and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the text rotation value.
    
    \sa 
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void getTextRotation(Text::Rotation& rotation) const;

  /** \details
    Returns current value of the flag that determines whether the radio button is locked.
    If the flag value is equal to true, the radio button is locked and becomes disabled otherwise it enabled and unlocked.
    The method fills a passed boolean value with the radio button lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;

  /** \details
    Returns current value of the flag that determines whether the radio button has a border.
    If the flag value is equal to true, the radio button has a border, otherwise it has not.
    The method fills a passed boolean value with the radio button border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns current radio button border color.
    The method fills a passed ODCOLORREF value with the radio button border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns current radio button border thickness.
    The method fills a passed value with current radio button border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness& thickness) const;

  /** \details
    Returns current radio button border style.
    The method fills a passed value with current radio button border style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style& style) const;

  /** \details
    Returns current radio button filling color.
    The method fills a passed ODCOLORREF value with the radio button filling color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;

  /** \details
    Returns current export value for the radio button.
    The method fills a passed string object with with the radio button export value and returns it to a calling subroutine.

    \param export_value [out] A placeholder for the export value.
  */
  void getExportValue(OdString& export_value) const;

  /** \details
    Returns current radio button default state.
    If default state is equal to true, the radio button is switched on when it displayed in a document, otherwise the radio button is switched off.
    The method fills a boolean object with with the radio button default state value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the default state value.
  */
  void getDefaultState(bool& default_state) const;

  /** \details
    Returns current value of the grouping flag that determines whether the radio button is included in a group of radio buttons.
    If the flag value is equal to true, the radio button is in group, otherwise it is a stand alone radio button.
    The method fills a passed boolean value with current flag value and returns it to a calling subroutine.
    
    \param grouping [out] A new flag value.
  */
  void getGrouping(bool& grouping) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdRadioButton, OdRadioButton> object.
*/
SMARTPTR(OdRadioButton);

}
#endif // _PDF_PUBLISH_RADIOBUTTON_INCLUDED_

