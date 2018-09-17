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
// PdfPublishTable.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_TABLE_INCLUDED_
#define _PDF_PUBLISH_TABLE_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishText.h"
#include "PdfPublishTextField.h"
#include "PdfPublishLink.h"
#include "PdfPublishButton.h"
#include "DoubleArray.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a table entity for .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdTable : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdTable();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTable);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
 
  /** \details
    Destroys a table object.
  */
  virtual ~OdTable();

  /** \details
    Adds a <link OdPdfPublish::OdLink, link> to a specified table cell. 
    The table cell is determined with row and column indexes.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param link   [in] A smart pointer to an OdLink object that should be added.
    
    \sa
    <link OdPdfPublish::OdLink, OdLink class>
  */  
  void setLink(const OdUInt32 row, const OdUInt32 column, const OdLinkPtr& link);

  /** \details
    Adds a <link OdPdfPublish::OdText, text labelÞ to a specified table cell. 
    The table cell is addressed through row and column index.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param text   [in] A smart pointer to an OdText object that should be added.
    
    \sa 
    <link OdPdfPublish::OdText, OdText class>
  */  
  void setText(const OdUInt32 row, const OdUInt32 column, const OdTextPtr& text);

  /** \details
    Adds a <link OdPdfPublish::OdButton, button> to a specified table cell. 
    The table cell is addressed through row and column index.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param button [in] A smart pointer to an OdButton object that should be added.
    
    \sa
    <link OdPdfPublish::OdButton, OdButton class>
  */
  void setButton(const OdUInt32 row, const OdUInt32 column, const OdButtonPtr& button);

  /** \details
    Adds a <link OdPdfPublish::OdTextField, text field> to a specified table cell. 
    The table cell is addressed through row and column index.
        
    \param row        [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column     [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param text_field [in] A smart pointer to an OdTextField object that should be added.
    
    \sa
    <link OdPdfPublish::OdTextField, OdTextField class>
  */
  void setTextField(const OdUInt32 row, const OdUInt32 column, const OdTextFieldPtr& text_field);
  
  /** \details
    Returns a link contained in a specified table cell.
    The table cell is addressed through row and column index.
    The method fills a passed smart pointer with a pointer to the link object and returns it to a calling subroutine.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param link   [out] A placeholder for a smart pointer to the link object.
    
    \returns Returns a smart pointer to the link contained in the table cell. If the cell does not contain a link, null pointer is returned.
  */
  void getLink(const OdUInt32 row, const OdUInt32 column, OdLinkPtr& link) const;

  /** \details
    Returns a <link OdPdfPublish::OdText, text label> contained in a specified table cell.
    The table cell is addressed through row and column index.
    The method fills a passed smart pointer with a pointer to the text label object and returns it to a calling subroutine.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param text   [out] A placeholder for a smart pointer to the text label object.
    
    \returns Returns a smart pointer to the text label contained in the table cell. If the cell does not contain a text label, null pointer is returned.
    
    \sa
    <link OdPdfPublish::OdText, OdText class>
  */
  void getText(const OdUInt32 row, const OdUInt32 column, OdTextPtr& text) const;

  /** \details
    Returns a <link OdPdfPublish::OdButton, button> contained in a specified table cell.
    The table cell is addressed through row and column index.
    The method fills a passed smart pointer with a pointer to the button object and returns it to a calling subroutine.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param button [out] A placeholder for a smart pointer to the button object.
    
    \returns Returns a smart pointer to the button contained in the table cell. If the cell does not contain a button, null pointer is returned.
    
    \sa
    <link OdPdfPublish::OdButton, OdButton class>
  */
  void getButton(const OdUInt32 row, const OdUInt32 column, OdButtonPtr& button) const;

  /** \details
    Returns a <link OdPdfPublish::OdTextField, text field> contained in a specified table cell.
    The table cell is addressed through row and column index.
    The method fills a passed smart pointer with a pointer to the text field object and returns it to a calling subroutine.
        
    \param row    [in] A table row index. Can accept values from 0 to table row quantity - 1.
    \param column [in] A table column index. Can accept values from 0 to table column quantity - 1.
    \param text   [out] A placeholder for a smart pointer to the text field object.
    
    \returns Returns a smart pointer to the text field contained in the table cell. If the cell does not contain a text field, null pointer is returned.
    
    \sa
    <link OdPdfPublish::OdTextField, OdTextField class>
  */
  void getTextField(const OdUInt32 row, const OdUInt32 column, OdTextFieldPtr& text_field) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdTable, OdTable> object.
*/
SMARTPTR(OdTable);

}
#endif // _PDF_PUBLISH_TABLE_INCLUDED_

