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
// PdfPublishCheckBox.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_CHECKBOX_INCLUDED_
#define _PDF_PUBLISH_CHECKBOX_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details 
Namespace contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a checkbox control encapsulated in a .pdf document created with Teigha Publish.
  Checkbox control is used in a .pdf document to set any boolean parameters, which can accept two values: 
  true (the checkbox is switched on) and false (the checkbox is switched off).
*/
class PDFPUBLISH_TOOLKIT OdCheckBox : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdCheckBox();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdCheckBox);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys a checkbox control.
  */
  virtual ~OdCheckBox();

  /** \details
    Sets a name of the checkbox.
    
    \param name [in] A new value of the checkbox name.

    \remarks The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);

  void setFont(Text::StandardFontsType fontType);
  
  /** \details
    Sets font name and style, which should be used for displaying the checkbox caption.
    
    \param name   [in] A new value of the font name.
    \param style  [in] A new value of the font style.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void setFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);
  
  /** \details
    Sets a font size used for displaying the checkbox caption.
    
    \param size [in] A new value of the font size that should be used for the checkbox.
  */
  void setFontSize(OdUInt16 size);
  
  /** \details
    Sets a text color used for displaying checkbox caption.
    
    \param color [in] A new value of the text color that should be used for the checkbox caption.
  */
  void setTextColor(ODCOLORREF color);
  
  /** \details
    Sets a tooltip text for the checkbox.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the checkbox.
    
    \param tooptip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);
  
  /** \details
    Sets a new value of the visibility flag for the checkbox.
    Visibility flag determines whether the checkbox is visible in document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);
  
  /** \details
    Sets a new value of the flag that determines whether the checkbox should be printed.
    If the checkbox should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);
  
  /** \details
    Sets a new rotation value for the checkbox text.
    
    \param rotation [in] A new rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);
  
  /** \details
    Sets a new value of the flag that determines whether the checkbox is locked.
    If the flag value is equal to true, the checkbox is locked and becomes disabled otherwise it enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);
  
  /** \details
    Sets a new value of the flag that determines whether the checkbox has a border.
    If the flag value is equal to true, the checkbox has a border, otherwise it has not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);
  
  /** \details
    Sets a checkbox border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);
  
  /** \details
    Sets a checkbox border thickness.
    
    \param thickness [in] A new thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);
  
  /** \details
    Sets a checkbox border style.
    
    \param style [in] A new border style value.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);
  
  /** \details
    Sets a checkbox filling color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);
  
  /** \details
    Sets an export value for the checkbox.
    
    \param export_value [in] A new export value.
  */
  void setExportValue(const OdString& export_value);
  
  /** \details
    Sets a checkbox default state.
    If default state is equal to true, the checkbox is switched on when it displayed in a document, otherwise the checbox is switched off.
    
    \param state [in] A new default state.
  */
  void setDefaultState(bool state);
  
  /** \details
    Returns current name of the checkbox.
    The method fills a passed string object with checkbox name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the the checkbox name.
  */
  void getName(OdString& name) const;
  
  /** \details
    Returns current font name and style used for displaying checkbox caption.
    The method fills a passed string object and Text::FontStyle value with current font name and style, then returns them to a calling subroutine.
    
    \param font_name  [out] A placeholder for the font name that should be used for the checkbox.
    \param style      [out] A placeholder for the font style that should be used for the checkbox.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;
  
  /** \details
    Returns current font size used for displaying the checkbox caption.
    The method fills a passed integer value with the checkbox font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for the checkbox.
  */
  void getFontSize(OdUInt16& size) const;
  
  /** \details
    Returns current text color used for displaying checkbox caption.
    The method fills a passed ODCOLORREF value with the checkbox caption color and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the text color that should be used for the checkbox caption.
  */
  void getTextColor(ODCOLORREF& color) const;
  
  /** \details
    Returns current tooltip text for the checkbox.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the checkbox.
    
    The method fills a passed string object with the checkbox tooltip text and returns it to a calling subroutine.
    
    \param tooptip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;
  
  /** \details
    Returns current value of the visibility flag for the checkbox.
    Visibility flag determines whether the checkbox is visible in document (if true) or is hidden (if false).
    The method fills a passed boolean value with the checkbox visibility flag value and returns it to a calling subroutine.    
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;
  
  /** \details
    Returns current value of the flag that determines whether the checkbox should be printed.
    If the checkbox should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the checkbox printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;
  
  /** \details
    Returns current rotation for the checkbox text.
    The method fills a passed value with current rotation value and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void getTextRotation(Text::Rotation& rotation) const;
  
  /** \details
    Returns current value of the flag that determines whether the checkbox is locked.
    If the flag value is equal to true, the checkbox is locked and becomes disabled otherwise it enabled and unlocked.
    The method fills a passed boolean value with the checkbox lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;
  
  /** \details
    Returns current value of the flag that determines whether the checkbox has a border.
    If the flag value is equal to true, the checkbox has a border, otherwise it has not.
    The method fills a passed boolean value with the checkbox border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;
  
  /** \details
    Returns current checkbox border color.
    The method fills a passed ODCOLORREF value with the checkbox border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;
  
  /** \details
    Returns current checkbox border thickness.
    The method fills a passed value with current border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness& thickness) const;
  
  /** \details
    Returns current checkbox border style.
    The method fills a passed value with current border style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the style value.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style& style) const;
  
  /** \details
    Returns current checkbox filling color.
    The method fills a passed ODCOLORREF value with the checkbox filling color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;
  
  /** \details
    Returns current export value for the checkbox.
    The method fills a passed string object with with the checkbox export value and returns it to a calling subroutine.

    \param export_value [out] A placeholder for the export value.
  */
  void getExportValue(OdString& export_value) const;
  
  /** \details
    Returns current checkbox default state.
    If default state is equal to true, the checkbox is switched on when it displayed in a document, otherwise the checbox is switched off.
    The method fills a boolean object with with the checkbox default state value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the default state value.
  */
  void getDefaultState(bool& default_state) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdCheckBox, OdCheckBox> object.
*/
SMARTPTR(OdCheckBox);

}
#endif // _PDF_PUBLISH_CHECKBOX_INCLUDED_

