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
// PdfPublishSignatureField.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_SIGNATUREFIELD_INCLUDED_
#define _PDF_PUBLISH_SIGNATUREFIELD_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a signature field contained in a .pdf document created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdSignatureField : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdSignatureField();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdSignatureField);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the signature field instance.
  */
  virtual ~OdSignatureField();

  /** \details
    Sets the signature field name.
    
    \param name [in] A new signature field name.

    \remarks The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);

  void setFont(Text::StandardFontsType fontType);

  /** \details
    Sets a font name and style of the signature field.
    
    \param name   [in] A new font name of the signature field.
    \param style  [in] A new font style of the signature field.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
  */
  void setFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);

  /** \details
    Sets a font size of the signature field.
    
    \param size [in] A new font size value of the signature field.
  */
  void setFontSize(OdUInt16 size);

  /** \details
    Sets a text color of the signature field.
    
    \param color [in] A new color value.
  */
  void setTextColor(ODCOLORREF color);

  /** \details
    Sets a tooltip text for the signature field.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the signature field.
    
    \param tooptip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);

  /** \details
    Sets a new value of the visibility flag for the signature field.
    Visibility flag determines whether the signature field is visible in document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);

  /** \details
    Sets a new value of the flag that determines whether the signature field should be printed.
    If the signature field should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);

  /** \details
    Sets a new value of the signature field text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>
  */
  void setTextRotation(Text::Rotation rotation);

  /** \details
    Sets a new value of the flag that determines whether the signature field is locked.
    If the flag value is equal to true, the signature field is locked and becomes disabled otherwise it enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);

  /** \details
    Sets a new value of the flag that determines whether the signature field has a border.
    If the flag value is equal to true, the signature field has a border, otherwise it has not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);

  /** \details
    Sets the signature field border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets the signature field border thickness.
    
    \param thickness [in] A new border thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets the signature field border style.
    
    \param style [in] A new border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>
  */
  void setBorderStyle(Border::Style style);

  /** \details
    Sets the signature field button filling color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);
  
  /** \details
    Returns current name of the signature field.
    The method fills a passed string object with signature field name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the the signature field name.
  */
  void getName(OdString& name) const;

  /** \details
    Returns current font name used for displaying signature field caption.
    The method fills a passed string object and Text::FontStyle value with current signature field font name and style, then returns them to a calling subroutine.
    
    \param font_name [out] A placeholder for the font name that should be used for the signature field.
    \param style     [out] A placeholder for the font style that should be used for the signature field.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns current font size used for displaying signature field caption.
    The method fills a passed integer value with the signature field font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for the signature field.
  */
  void getFontSize(OdUInt16& size) const;

  /** \details
    Returns current text color used for displaying signature field caption.
    The method fills a passed ODCOLORREF value with the signature field caption color and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color that should be used for the signature field caption.
  */
  void getTextColor(ODCOLORREF& color) const;

  /** \details
    Returns current tooltip text for the signature field.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the signature field.
    
    The method fills a passed string object with the signature field tooltip text and returns it to a calling subroutine.
    
    \param tooptip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;

  /** \details
    Returns current value of the visibility flag for the signature field.
    Visibility flag determines whether the signature field is visible in document (if true) or is hidden (if false).
    The method fills a passed boolean value with the signature field visibility flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;

  /** \details
    Returns current value of the flag that determines whether the signature field should be printed.
    If the signature field should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the signature field printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;

  /** \details
    Returns current text rotation of the signature field.
    The method fills a passed value with current signature field text rotation and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>
  */
  void getTextRotation(Text::Rotation& rotation) const;

  /** \details
    Returns current value of the flag that determines whether the signature field is locked.
    If the flag value is equal to true, the signature field is locked and becomes disabled otherwise it enabled and unlocked.
    The method fills a passed boolean value with the signature field lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;

  /** \details
    Returns current value of the flag that determines whether the signature field has a border.
    If the flag value is equal to true, the signature field has a border, otherwise it has not.
    The method fills a passed boolean value with the signature field border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns current signature field border color.
    The method fills a passed ODCOLORREF value with the signature field border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns current signature field border thickness.
    The method fills a passed value with curretn signature field border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>
  */
  void getBorderThickness(Border::Thickness& thickness) const;

  /** \details
    Returns current signature field border style.
    The method fills a passed value with current signature field border style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>
  */
  void getBorderStyle(Border::Style& style) const;

  /** \details
    Returns current signature field filling color.
    The method fills a passed ODCOLORREF value with the signature field filling color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdSignatureField, OdSignatureField> object.
*/
SMARTPTR(OdSignatureField);

}
#endif // _PDF_PUBLISH_SIGNATUREFIELD_INCLUDED_

