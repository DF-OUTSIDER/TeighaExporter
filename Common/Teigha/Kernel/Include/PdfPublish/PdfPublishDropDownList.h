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
// PdfPublishDropDownList.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_DROPDOWN_INCLUDED_
#define _PDF_PUBLISH_DROPDOWN_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "StringArray.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a drop-down list control contained in a .pdf document created with Teigha Publish.
  Drop-down list contains a list of predefined items, which can be selected. Only one of predefined items can be selected. 
  User can also enter a custom value to a drop-down list if allowed (see <link OdPdfPublish::OdDropDownList::getCustomText@bool&@const, setCustomText()> 
  and <link OdPdfPublish::OdDropDownList::setCustomText@bool, getCustomText()> methods).
*/
class PDFPUBLISH_TOOLKIT OdDropDownList : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdDropDownList();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDropDownList);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the drop-down list control.
  */
  virtual ~OdDropDownList();

  /** \details
    Sets the drop-down list name.
    
    \param name [in] A new drop-down list name.

    \remarks The name is mandatory and must be unique among the document object names.
  */
  void setName(const OdString& name);

  void setFont(Text::StandardFontsType fontType);
  
  /** \details
    Sets a font name and style of the drop-down list.
    
    \param name  [in] A new font name of the drop-down list.
    \param style [in] A new font style of the drop-down list.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void setFont(const OdString& fontName, Text::FontStyle fontStyle = Text::kRegular);

  /** \details
    Sets a font size of the drop-down list.
    
    \param size [in] A new font size value of the drop-down list.
  */
  void setFontSize(OdUInt16 size);

  /** \details
    Sets a text color of the drop-down list.
    
    \param color [in] A new color value.
  */
  void setTextColor(ODCOLORREF color);

  /** \details
    Sets a tooltip text for the drop-down list.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the drop-down list.
    
    \param tooptip [in] A new value of the tooltip text.
  */
  void setTooltip(const OdString& tooltip);

  /** \details
    Sets a new value of the visibility flag for the drop-down list.
    Visibility flag determines whether the drop-down list is visible in document (if true) or is hidden (if false).
    
    \param state [in] A new flag value.
  */
  void setVisibility(bool state);

  /** \details
    Sets a new value of the flag that determines whether the drop-down list should be printed.
    If the drop-down list should be printed, the flag value is equal to true, otherwise it is equal to false.
    
    \param state [in] A new flag value.
  */
  void setPrintability(bool state);

  /** \details
    Sets a new value of the drop-down list text rotation.
    
    \param rotation [in] A new text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void setTextRotation(Text::Rotation rotation);

  /** \details
    Sets a new value of the flag that determines whether the drop-down list is locked.
    If the flag value is equal to true, the drop-down list is locked and becomes disabled otherwise it enabled and unlocked.
    
    \param state [in] A new flag value.
  */
  void setLock(bool state);

  /** \details
    Sets a new value of the flag that determines whether the drop-down list has a border.
    If the flag value is equal to true, the drop-down list has a border, otherwise it has not.
    
    \param state [in] A new flag value.
  */
  void setBorder(bool state);

  /** \details
    Sets a drop-down list border color.
    
    \param color [in] A new color value.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Sets a new value of the drop-down list border thickness.
    
    \param thickness [in] A new thickness value.
    
    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void setBorderThickness(Border::Thickness thickness);

  /** \details
    Sets a new border style for the drop-down list.
    
    \param style [in] A new border style.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void setBorderStyle(Border::Style style);

  /** \details
    Sets a drop-down list filling color.
    
    \param color [in] A new color value.
  */
  void setFillColor(ODCOLORREF color);

  /** \details
    Sets a new value of the custom text flag of the drop-down list.
    Custom text flag determines whether user can add custom value in the drop-down list. 
    If the flag is equal to true, user can enter his own value, otherwise user can only select one of the predefined values of the drop-down list.
    
    \param state [in] A new flag value.
  */
  void setCustomText(bool state);

  /** \details
    Sets a new value of the spell checking flag of the drop-down list.
    Spell checking flag determines whether user text entered to the drop-down list should be checked. 
    If the flag is equal to true, custom value of the drop-down list entered by a user is checked, otherwise drop-down list does not check a user text.
    
    \param state [in] A new flag value.
  */
  void setSpellChecking(bool state);

  /** \details
    Sets a new value of the immediate commit flag of the drop-down list.
    Immediate commit flag determines whether user text entered to the drop-down list, is committed immediately (if set to true). 
    
    \param state [in] A new flag value.
  */
  void setImmediateCommit(bool state);

  /** \details
    Sets the drop-down list content.
    The method fills the drop-down list with an array of items. Each item is a string value.
    
    \param displayed_values [in] An array of string items to be displayed as the content of the drop-down list control.
    \param export_values    [in] An array of string items to be exported from the drop-down list control.
  */
  void setContents(const OdStringArray& displayed_values, const OdStringArray& export_values);
  
  /** \details
    Returns current name of the drop-down list.
    The method fills a passed string object with drop-down list name and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the the drop-down list name.
  */
  void getName(OdString& name) const;

  /** \details
    Returns current font name used for displaying drop-down list items.
    The method fills a passed string object and Text::FontStyle value with the drop-down list font name and font style, then returns them to a calling subroutine.
    
    \param font_name  [out] A placeholder for the font name that should be used for displaying drop-down list items.
    \param style      [out] A placeholder for the font style that should be used for displaying drop-down list items.
    
    \sa
    <link OdPdfPublish::Text::FontStyle, Text::FontStyle enumeration>.
  */
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::FontStyle& fontStyle) const;

  /** \details
    Returns current font size used for displaying drop-down list items.
    The method fills a passed integer value with the drop-down list font size and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size that should be used for displaying drop-down list items.
  */
  void getFontSize(OdUInt16& size) const;

  /** \details
    Returns current text color used for displaying drop-down list items.
    The method fills a passed ODCOLORREF value with the drop-down list caption color and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text color that should be used for displaying drop-down list items.
  */
  void getTextColor(ODCOLORREF& color) const;

  /** \details
    Returns current tooltip text for the drop-down list.
    Tooltip text is a text that appears when a user hovers the mouse pointer over the drop-down list.
    
    The method fills a passed string object with the drop-down list tooltip text and returns it to a calling subroutine.
    
    \param tooptip [out] A placeholder for the tooltip text.
  */
  void getTooltip(OdString& tooltip) const;

  /** \details
    Returns current value of the visibility flag for the drop-down list.
    Visibility flag determines whether the drop-down list is visible in document (if true) or is hidden (if false).
    The method fills a passed boolean value with the drop-down list visibility flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getVisibility(bool& state) const;

  /** \details
    Returns current value of the flag that determines whether the drop-down list should be printed.
    If the drop-down list should be printed, the flag value is equal to true, otherwise it is equal to false.
    The method fills a passed boolean value with the drop-down list printability flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getPrintability(bool& state) const;

  /** \details
    Returns current rotation of the drop-down list text.
    The method fills a passed value with current text rotation value and returns it to a calling subroutine.
    
    \param rotation [out] A placeholder for the text rotation value.
    
    \sa
    <link OdPdfPublish::Text::Rotation, Text::Rotation enumeration>.
  */
  void getTextRotation(Text::Rotation& rotation) const;

  /** \details
    Returns current value of the flag that determines whether the drop-down list is locked.
    If the flag value is equal to true, the drop-down list is locked and becomes disabled otherwise it enabled and unlocked.
    The method fills a passed boolean value with the drop-down list lock flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getLock(bool& state) const;

  /** \details
    Returns current value of the flag that determines whether the drop-down list has a border.
    If the flag value is equal to true, the drop-down list has a border, otherwise it has not.
    The method fills a passed boolean value with the drop-down list border flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns current drop-down list border color.
    The method fills a passed ODCOLORREF value with the drop-down list border color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns current drop-down list border thickness.
    The method fills a passed value with current border thickness and returns it to a calling subroutine.
    
    \param thickness [out] A placeholder for the border thickness.

    \sa
    <link OdPdfPublish::Border::Thickness, Border::Thickness enumeration>.
  */
  void getBorderThickness(Border::Thickness& thickness) const;

  /** \details
    Returns current drop-down list border style.
    The method fills a passed value with current border style value and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the border style value.
    
    \sa
    <link OdPdfPublish::Border::Style, Border::Style enumeration>.
  */
  void getBorderStyle(Border::Style& style) const;

  /** \details
    Returns current drop-down list filling color.
    The method fills a passed ODCOLORREF value with the drop-down list filling color value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the color value.
  */
  void getFillColor(ODCOLORREF& color) const;

  /** \details
    Returns current value of the custom text flag of the drop-down list.
    Custom text flag determines whether user can add custom value in the drop-down list. 
    If the flag is equal to true, user can enter his own value, otherwise user can only select one of the predefined values of the drop-down list.
    The method fills a passed boolean value with the custom text flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getCustomText(bool& state) const;

  /** \details
    Returns current value of the spell checking flag of the drop-down list.
    Spell checking flag determines whether user text entered to the drop-down list should be checked. 
    If the flag is equal to true, custom value of the drop-down list entered by a user is checked, otherwise drop-down list does not check a user text.
    The method fills a passed boolean value with the spell checking flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the flag value.
  */
  void getSpellChecking(bool& state) const;

  /** \details
    Returns current value of the immediate commit flag of the drop-down list.
    Immediate commit flag determines whether user text entered to the drop-down list, is committed immediately (if set to true). 
    The method fills a passed boolean value with immediate commit flag value and returns it to a calling subroutine.
    
    \param state [out] A placeholder of the flag value.
  */
  void getImmediateCommit(bool& state) const;

  /** \details
    Returns current drop-down list content.
    The method fills passed string arrays with display and export items respectively and returns them to a calling subroutine.
    
    \param displayed_values [out] A placeholder for the array of string items to be displayed as the content of the drop-down list.
    \param export_values    [out] A placeholder for the array of string items to be exported as the content of the drop-down list.
  */
  void getContents(OdStringArray& displayed_values, OdStringArray& export_values) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdDropDownList, OdDropDownList> object.
*/
SMARTPTR(OdDropDownList);

}
#endif // _PDF_PUBLISH_DROPDOWN_INCLUDED_

