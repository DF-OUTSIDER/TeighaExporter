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
// PdfPublishButton.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_BUTTON_INCLUDED_
#define _PDF_PUBLISH_BUTTON_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishImage.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a button control encapsulated in a .pdf document created with Teigha Publish.
  Button control is used in a .pdf document to implement reaction on a user actions (such as different kinds of mouse clicks).
  A button control appearance (in other words, how a button looks in a document) is defined with a set of special properties (for example, font, color, text and so on).
*/
class PDFPUBLISH_TOOLKIT OdButton : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdButton();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdButton);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys a button object.
  */
  virtual ~OdButton();

  /** \details
    Sets the button name.
    
    \param name [in] A new value of the button name.

    \remarks The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);
  
  /** \details
    Sets the button label text.
    
    \param label [in] A new value of the label text.
  */
  void setLabel(const OdString& label);
  
  /** \details
    Sets the button font name and style. 
    
    \param name       [in] A new font name.
    \param font_style [in] A new font style.
    
    \sa 
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void setFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);
  
  /** \details
    Sets the button font type. 
    
    \param font_type [in] A new font type value.
    
    \sa 
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>.
  */
  void setFont(const Text::StandardFontsType fontType);
  
  /** \details
    Sets the button font size. 
    
    \param size [in] A new font size value.
  */
  void setFontSize(OdUInt16 size);
  
  /** \details
    Sets the button text color. Text color is applied for button label.
    
    \param color [in] A new text color value.
  */
  void setTextColor(ODCOLORREF color);
  
  /** \details
    Sets the button tooltip text. Tooltip text is a text that appears when user hovers over the button.
    
    \param tooltip [in] A new text of the tooltip.
  */
  void setTooltip(const OdString& tooltip);
  
  /** \details
    Sets the button visibility.
    
    \param state [in] A flag value that determines whether the button is visible (if is true) or not (if is false).
  */
  void setVisibility(bool state);
  
  /** \details
    Sets the button behavior when a document is printed.
    
    \param state [in] A flag value that determines whether the button should be printed (if is true) or not (if is false).
  */
  void setPrintability(bool state);
  
  /** \details
    Sets the button text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa 
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);
  
  /** \details
    Locks or unlocks the button.
    
    \param state [in] A flag value that determines whether the button is locked (if is true) or is unlocked (if is false).
  */
  void setLock(bool state);
  
  /** \details
    Shows or hides a border around the button.
    
    \param state [in] A flag value that determines whether the button has a border (if is true) or has not (if is false).
  */
  void setBorder(bool state);
  
  /** \details
    Sets the button border color.
    
    \param color [in] A new value of the border color.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets the button border thickness.
    
    \param thickness [in] A new value of the border thickness.
    
    \sa 
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets the button border style.
    
    \param style [in] A new border style.
    \sa 
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);
  
  /** \details
    Sets a color that is used for filling the button.
    
    \param color [in] A new value of the filling color.
  */
  void setFillColor(ODCOLORREF color);

  /** \details
    Sets a position of the button label.
    
    \param position [in] A new value of the label position.
    
    \sa 
    <link OdPdfPublish::Label::Position, Label::Position enumeration>.
  */
  void setLabelPosition(Label::Position position);

  /** \details
    Sets the button highlighting mode.
    
    \param mode [in] A new value of the highlighting mode.
    
    \sa 
    <link OdPdfPublish::Highlighting::Mode, Highlighting::Mode enumeration>.
  */
  void setHighlighting(Highlighting::Mode mode);
  
  /** \details
    Sets the button icon <link OdPdfPublish::OdImage,  image>.
    
    \param image [in] A smart pointer to an image object to be used as button icon.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void setIconImage(const OdImagePtr& image);

  /** \details
    Returns the button name.
    The method fills a passed string object with name value and returns this string to a calling subroutine.
    
    \param name [out] A placeholder for the button name. 
  */
  void getName(OdString& name) const;
  
  /** \details
    Returns the button label text.
    The method fills a passed string object with label text and returns this string to a calling subroutine.
    
    \param label [out] A placeholder for the button label text. 
  */
  void getLabel(OdString& label) const;
  
  /** \details
    Returns current button font name and style.
    The method fills passed string object and Text::FontStyle value with font text name and font style, then returns them to a calling subroutine.
    
    \param font_name  [out] A placeholder for the font name.
    \param style      [out] A placeholder for the font style. 
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns the button font size.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size value.
  */
  void getFontSize(OdUInt16 & size) const;
  
  /** \details
    Returns the button text color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color value.
  */  
  void getTextColor(ODCOLORREF& color) const;
  
  /** \details
    Returns the button tooltip text.
    The method fills a passed string object and returns it to a calling subroutine.
    
    \param tooltip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;
  
  /** \details
    Returns current button visibility state.
    The method fills a passed boolean value and returns it to a calling subroutine.
    If the button is visible, the method returns true, otherwise it returns false.
    
    \param state [out] A placeholder for the visibility state.
  */
  void getVisibility(bool& state) const;
  
  /** \details
    Returns current flag value that defines whether the button should be printed.
    The method fills a passed boolean value and returns it to a calling subroutine.
    If the button should be printed, the method returns true, otherwise it returns false.
    
    \param state [out] A placeholder for the print flag value.
  */  
  void getPrintability(bool& state) const;
  
  /** \details
    Returns current button text rotation.
    The method fills a passed value with current rotation and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the button text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */  
  void getTextRotation(Text::Rotation & rotation) const;
  
  /** \details
    Returns whether the button is locked.
    The method fills a passed boolean value and returns it to a calling subroutine.
    If the button is locked, the method returns true, otherwise it returns false.
    
    \param state [out] A placeholder for the lock state value.
  */
  void getLock(bool& state) const;
  
  /** \details
    Returns whether the button has border.
    The method fills a passed boolean value and returns it to a calling subroutine.
    If the button has border, the method returns true, otherwise it returns false.
    
    \param state [out] A placeholder for the border flag value.
  */
  void getBorder(bool& state) const;
  
  /** \details
    Returns the button border color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the border color value.
  */
  void getBorderColor(ODCOLORREF& color) const;
  
  /** \details
    Returns the button border thickness.
    The method fills a passed value with current thickness value and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness & thickness) const;
  
  /** \details
    Returns the button border style.
    The method fills a passed value with current style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style & style) const;
  
  /** \details
    Returns the button filling color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the filling color value.
  */
  void getFillColor(ODCOLORREF& color) const;
  
  /** \details
    Returns current button label position.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param position [out] A placeholder for the button label position value.
    
    \sa
    <link OdPdfPublish::Label::Position, Label::Position enumeration>.
  */
  void getLabelPosition(Label::Position & position) const;
  
  /** \details
    Returns current button highlighting mode.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param mode [out] A placeholder for the button hightlighting mode.
    
    \sa
    <link OdPdfPublish::Highlighting::Mode, Highlighting::Mode enumeration>.
  */
  void getHighlighting(Highlighting::Mode & mode) const;
  
  /** \details
    Returns the button icon <link OdPdfPublish::OdImage, image>.
    The method fills a passed OdImage object and returns it to a calling subroutine.
    
    \param image [out] A placeholder for the icon image.
    
    \sa
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void getIconImage(OdImagePtr& image) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdButton, OdButton> object.
*/
SMARTPTR(OdButton);

}
#endif // _PDF_PUBLISH_BUTTON_INCLUDED_

