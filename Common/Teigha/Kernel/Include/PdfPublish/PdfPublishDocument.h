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
// PdfPublishDocument.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_DOCUMENT_INCLUDED_
#define _PDF_PUBLISH_DOCUMENT_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishPage.h"

#include "DbBaseHostAppServices.h"

/** \details 
Namespace contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements working with a .pdf document created with Teigha Publish.
  Document is a container for other elements and controls 
  (such as <link OdPdfPublish::OdPage, pages>, <link OdPdfPublish::OdImage, images>, <link OdPdfPublish::OdLink, links>, 
  <link OdPdfPublish::OdAnnotation, annotations> and so on).
*/
class PDFPUBLISH_TOOLKIT OdDocument : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdDocument();
  //DOM-IGNORE-BEGIN
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDocument);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details 
    Destroys the Teigha Publish document.
  */
  virtual ~OdDocument();

  /** \details 
    Sets common information about the Teigha Publish document.
    \param title    [in] A string that contains the document title.
    \param author   [in] A string that contains the document author.
    \param subject  [in] A string that contains the document subject.
    \param creator  [in] A string that contains the information about the document creator.
  */
  void setInformation(const OdString& title, const OdString& author, const OdString& subject, const OdString& creator);
  
  /** \details 
    Adds a new JavaScript scenario to the Teigha Publish document.
    \param name   [in] A name of a JavaScript scenario.
    \param source [in] A source code text of a JavaScript scenario.
  */
  void addJavaScript(const OdString& name, const OdString& source, const Source::Type source_type = Source::kCode);
  
  /** \details 
    Adds a new icon <link OdPdfPublish::OdImage, image> to the Teigha Publish document.
    \param name   [in] A name of an icon image.
    \param image [in] An image object.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void addIconImage(const OdString& name, const OdImagePtr& image);
  
  /** \details 
    Adds a new <link OdPdfPublish::OdPage, page> to the Teigha Publish document.
    \param page [in] A smart pointer to an OdPage object that should be added.
    
    \sa 
    <link OdPdfPublish::OdPage, OdPage class>.
  */
  void addPage(const OdPagePtr& page);

  /** \details 
    Returns current common information about the Teigha Publish document.
    This method fills passed strings and returns them to a calling subroutine.
    
    \param title    [out] A placeholder for the document title.
    \param author   [out] A placeholder for the document author.
    \param subject  [out] A placeholder for the document subject.
    \param creator  [out] A A placeholder for the information about the document creator.
  */
  void getInformation(OdString& title, OdString& author, OdString& subject, OdString& creator) const;
  
  /** \details 
    Returns the collection of JavaScript scenarios encapsulated in the Teigha Publish document.
    This method fills passed string arrays with names and source code texts, then returns them to a calling subroutine.
    
    \param names    [out] A placeholder for JavaScript scenario names.
    \param sources  [out] A placeholder for JavaScript scenario source code texts.
  */
  void getJavaScripts(OdStringArray& names, OdStringArray& sources, OdSourceTypeArray& source_types) const;
  
  /** \details
    Returns a collection of icon <link OdPdfPublish::OdImage, images> from the Teigha Publish document.
    This method fills passed string array and smart pointer array with image names and pointers to image objects, then returns them to a calling subroutine.
    
    \param names  [out] A placeholder for icon images names.
    \param images [out] A placeholder for an array of smart pointers, which point to the document icon images.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */

  void getIconImages(OdStringArray& names, OdImagePtrArray& images) const;

  /** \details 
    Returns the array of <link OdPdfPublish::OdPage, pages>, which the Teigha Publish document contains.
    This method fills a passed array with OdPage object, which represent document pages and returns it to a calling subroutine.
    
    \param pages [out] A placeholder for the document pages array.
    
    \sa 
    <link OdPdfPublish::OdPage, OdPage class>.
  */
  void getPages(OdPagePtrArray& pages) const;

  void setHostServices(OdDbBaseHostAppServices* pHostApp);
  OdDbBaseHostAppServices* appServices() const;
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdDocument, OdDocument> object.
*/
SMARTPTR(OdDocument);

}
#endif // _PDF_PUBLISH_DOCUMENT_INCLUDED_

