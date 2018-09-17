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
// PdfPublishText.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_TEXT_INCLUDED_
#define _PDF_PUBLISH_TEXT_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a text label for .pdf documents created with Teigha Publish.
  Text label options consist of two parts: text content and formatting parameters (font type, size, color).
*/
class PDFPUBLISH_TOOLKIT OdText : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdText();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdText);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys a text label.
  */
  virtual ~OdText();

  /** \details
    Sets the label text.
    
    \param text [in] A new label text.
  */
  void setText(const OdString& text);

  /** \details
    Sets the text label font type.
    
    \param font_type [in] A new font type value.
    
    \sa
    <link OdPdfPublish::Text::StandardFontsType, Text::StandardFontsType enumeration>.
  */
  void setFont(Text::StandardFontsType fontType);

  void setFont(const OdString& fontName, Text::Language language, bool embed);

  /** \details
    Sets the text label font size.
    
    \param font_type [in] A new font size value.
  */
  void setSize(OdUInt16 size);
  
  /** \details
    Sets the text label foreground color.
    
    \param color [in] A new color value.
  */
  void setColor(ODCOLORREF color);

  /** \details
    Returns the text label content.
    The method fills a passed string object with label text and returns this string to a calling subroutine.
    
    \param text [out] A placeholder for the label text. 
  */
  void getText(OdString& text) const;
  
  void getFont(Text::StorageType& storageType, Text::StandardFontsType& fontType, OdString& fontName, Text::Language& language, bool& embed) const;
  
  /** \details
    Returns the text label font size.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param size [out] A placeholder for the font size value.
  */
  void getSize(OdUInt16& size) const;
  
  /** \details
    Returns the text label color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the text label color value.
  */
  void getColor(ODCOLORREF& color) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdText, OdText> object.
*/
SMARTPTR(OdText);

}
#endif // _PDF_PUBLISH_TEXT_INCLUDED_

