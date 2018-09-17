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
// PdfPublishLink.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_LINK_INCLUDED_
#define _PDF_PUBLISH_LINK_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a link control contained in a .pdf document created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdLink : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdLink();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdLink);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys the link control.
  */
  virtual ~OdLink();

  /** \details
    Sets a JavaScript scenario for the link control.
    
    \param source [in] A string object that contains JavaScript scenario source code.
  */
  void setJavaScript(const OdString& source, const Source::Type source_type = Source::kCode);

  /** \details
    Sets the border width of the link control.
    Border width is measured in points.
    
    \param width [in] A border width value (in points).
  */
  void setBorderWidth(const OdUInt16 width);

  /** \details
    Sets the link highlighting mode.
    
    \param mode [in] A new highlighting mode for the link.
    
    \sa
    <link OdPdfPublish::Highlighting::Mode, Highlighting::Mode enumeration>.
  */
  void setHighlighting(const Highlighting::Mode mode);


  /** \details
    Sets the link control border color.
    
    \param color [in] A new value of the border color.
  */
  void setBorderColor(const ODCOLORREF color);
  
  /** \details
    Returns current JavaScript scenario for the link control.
    The method fills a passed string object with JavaScript scenario source code and returns it to a calling subroutine.
    
    \param source [out] A placeholder for the JavaScript scenario source code.
  */
  void getJavaScript(OdString& source, Source::Type& source_type) const;

  /** \details
    Returns current border width of the link control.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param width [out] A placeholder for the border width value (in points).
    
    \remarks 
    Border width is measured in points.
  */
  void getBorderWidth(OdUInt16& width) const;

  /** \details
    Returns current link highlighting mode.
    The method fills a passed value with current highlighting mode and returns it to a calling subroutine.
    
    \param mode [out] A placeholder for the highlighting mode value.
    
    \sa
    <link OdPdfPublish::Highlighting::Mode, Highlighting::Mode enumeration>.
  */

  void getHighlighting(Highlighting::Mode& mode) const;

  /** \details
    Returns the link control border color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the border color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdLink, OdLink> object.
*/
SMARTPTR(OdLink);

}
#endif // _PDF_PUBLISH_LINK_INCLUDED_

