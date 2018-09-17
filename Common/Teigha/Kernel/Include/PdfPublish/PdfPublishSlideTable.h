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
// PdfPublishSlideTable.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_SLIDETABLE_INCLUDED_
#define _PDF_PUBLISH_SLIDETABLE_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishTextField.h"
#include "DoubleArray.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a slide table entity for .pdf documents created with Teigha Publish.
  Slide Table is a kind of table, which implements showing a set of slides. Each cell of the slide table contains an image (slide). 
  There are also two specific cells: 
  * A cell for Next button. Next button moves the slides forward when user presses it.
  * A cell for Previous button. Previous button moves the slides backward when user presses it.
  Slide table is suitable for showing preview images for different views in .pdf document with 3D objects.
*/
class PDFPUBLISH_TOOLKIT OdSlideTable : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdSlideTable();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdSlideTable);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
 
  /** \details
    Destroys a slide table object.
  */
  virtual ~OdSlideTable();


  void setDimensions(const OdDoubleArray& column_proportions, const OdUInt32 row_height);  

  /** \details
    Sets an HTML markup source code for the slide table.
    The HTML markup source code is represented with a string in UTF8 format.
        
    \param source [in] A new HTML markup source code.
  */
  void setHTML(const OdString& source);

  /** \details
    Sets style options for the HTML markup of the slide table.
    The HTML markup style options are represented with a string in UTF8 format.
        
    \param source [in] A new HTML markup style options.
  */
  void setHTMLStyle(const OdString&);

  /** \details
    Sets special buttons for sliding forward and backward for the slide table.
        
    \param previous_button_name [in] A name for previous button that slides backward.
    \param next_button_name     [in] A name for next button that slides forward.
  */
  void setButtons(const OdString& previous_button_name, const OdString& next_button_name);

  /** \details
    Adds text fields for slide table cells.
        
    \param rows     [in] A quantity of rows in slide table, which should be filled with text fields.
    \param columns  [in] A quantity of columns in slide table, which should be filled with text fields.
    \param text     [in] An array of smart pointers to a text fields, which should be added to the slide table cells.
  */
  void setText(const OdUInt32 rows, const OdUInt32 columns, const OdTextFieldPtrArray& text);

  /** \details
    Sets a header flag value. Header flag determines whether the slide table have header (if set to true).
        
    \param state [in] A new value of the header flag.
  */
  void setHeader(bool state);
  
  void getDimensions(OdDoubleArray& column_proportions, OdUInt32& row_height) const;

  /** \details
    Returns current HTML markup source code for the slide table.
    The HTML markup source code is represented with a string in UTF8 format.
    The method fills a passed string object with HTML source code and returns it to a calling subroutine.
        
    \param source [out] A placeholder for the HTML markup source code.
  */

  void getHTML(OdString& source) const;

  /** \details
    Returns current style options for the HTML markup of the slide table.
    The HTML markup style options are represented with a string in UTF8 format.
    The method fills a passed string object and returns it to a calling subroutine.
        
    \param source [in] A placeholder for the HTML markup style options.
  */
  void getHTMLStyle(OdString& source) const;

  /** \details
    Returns current names of previous and next buttons contained in the slide table.
    The method fills passed string objects with previous and next button names and returns them to a calling subroutine.
        
    \param previous_button_name [out] A placeholder for the previous button name.
    \param next_button_name     [out] A placeholder for the next button name.
  */
  void getButtons(OdString& previous_button_name, OdString& next_button_name) const;

  /** \details
    Returns text fields of the slide table.
    The method fills passed values with quantity of rows and column and adds text field smart pointers into a passed array, 
    then returns filled values and array to a calling subroutine.
        
    \param rows     [out] A placeholder for the quantity of slide table rows.
    \param columns  [out] A placeholder for the quantity of slide table columns.
    \param text     [out] An placeholder for smart pointers to text fields.
  */
  void getText(OdUInt32 & rows, OdUInt32 & columns, OdTextFieldPtrArray& text) const;

  /** \details
    Returns current header flag value. Header flag determines whether the slide table have header (if set to true).
    The method fills a passed boolean value with header flag value and returns it to a calling subroutine.
        
    \param state [in] A placeholder for the header flag value.
  */
  void getHeader(bool& state) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdSlideTable, OdSlideTable> object.
*/
SMARTPTR(OdSlideTable);

}
#endif // _PDF_PUBLISH_SLIDETABLE_INCLUDED_

