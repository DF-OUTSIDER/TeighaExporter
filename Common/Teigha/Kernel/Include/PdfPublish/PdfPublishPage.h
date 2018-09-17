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
// PdfPublishPage.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_PAGE_INCLUDED_
#define _PDF_PUBLISH_PAGE_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishAnnotation.h"
#include "PdfPublishText.h"
#include "PdfPublishImage.h"
#include "PdfPublishTextField.h"
#include "PdfPublishSignatureField.h"
#include "PdfPublishDropDownList.h"
#include "PdfPublishListBox.h"
#include "PdfPublishRadioButton.h"
#include "PdfPublishCheckBox.h"
#include "PdfPublishLink.h"
#include "PdfPublishSlideTable.h"
#include "PdfPublishButton.h"
#include "PdfPublishTable.h"
#include "PdfPublishTableCreator.h"
#include "PdfPublish2dGeometryReference.h"
#include "PdfPublishCADReference.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a page object of .pdf document created with Teigha Publish.
  Any page is determined with two properties: format (sizes of its sides) and orientation (which side aligns horizontally) 
  and can contain a set of different controls and fields (such as <link OdPdfPublish::OdText, text labels>, 
  <link OdPdfPublish::OdAnnotation, annotations>, <link OdPdfPublish::OdTextField, text fields>, <OdPdfPublish::OdButton, buttons>, 
  <OdPdfPublish::OdLink, links>, etc.).
*/
class PDFPUBLISH_TOOLKIT OdPage : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdPage();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdPage);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
 
  /** \details
    Destroys a page object.
  */
  virtual ~OdPage();

  /** \details
    Sets the page format.
    
    \param format [in] A new page format value.
    
    \sa
    <link OdPdfPublish::Page::Format, Page::Format enumeration>.
  */
  void setFormat(Page::Format format);
  
  /** \details
    Sets the page orientation.
    
    \param orientation [in] A new page orientation value.
    
    \sa
    <link OdPdfPublish::Page::Orientation, Page::Orientation enumeration>.
  */
  void setOrientation(Page::Orientation orientation);

  void setPaperSize(const Page::PaperUnits units, const double width, const double height);
  
  /** \details
    Adds a new <link OdPdfPublish::OdAnnotation, annotation> to the page.
    
    \param annotation [in] A smart pointer to an annotation object that should be added.
    \param location   [in] A rectangle that specifies the location of added annotation within the page. It determines where the annotation will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdAnnotation, OdAnnotation class>.
  */
  void addAnnotation(const OdAnnotationPtr& annotation, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdText, text> label to the page.
    
    \param text       [in] A smart pointer to a text label that should be added.
    \param location   [in] A rectangle that specifies the location of added text label within the page. It determines where the text will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdText, OdText class>.
  */
  void addText(const OdTextPtr& text, const OdRect& location);

  /** \details
    Adds an <link OdPdfPublish::OdImage, image> to the page.
    
    \param image      [in] A smart pointer to an image object that should be added.
    \param location   [in] A rectangle that specifies the location of added image within the page. It determines where the image will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void addImage(const OdImagePtr& image, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdTable, table> to the page.
    
    \param table      [in] A smart pointer to a table object that should be added.
    \param location   [in] A rectangle that specifies the location of added table within the page. It determines where the table will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdTable, OdTable class>.
  */
  void addTable(const OdTablePtr& table, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdLink, link> to the page.
    
    \param link       [in] A smart pointer to a link object that should be added.
    \param location   [in] A rectangle that specifies the location of added link within the page. It determines where the link will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdLink, OdLink class>.
  */
  void addLink(const OdLinkPtr& link, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdButton, button control> to the page.
    
    \param button     [in] A smart pointer to a button control that should be added.
    \param location   [in] A rectangle that specifies the location of added button control within the page. It determines where the button control will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdButton, OdButton class>.
  */
  void addButton(const OdButtonPtr& button, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdTextField, text field> to the page.
    
    \param text_field [in] A smart pointer to a text field object that should be added.
    \param location   [in] A rectangle that specifies the location of added text field within the page. It determines where the text field will be placed at the page.
    
    \sa
    <link OdPdfPublish::OdTextField, OdTextField class>.
  */
  void addTextField(const OdTextFieldPtr& text_field, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdSlideTable, table with slides> to the page.
    
    \param slide_table [in] A smart pointer to a slide table object that should be added.
    \param location    [in] A rectangle that specifies the location of added slide table within the page. It determines where the slide table is placed at the page.
    
    \sa
    <link OdPdfPublish::OdSlideTable, OdSlideTable class>.
  */
  void addSlideTable(const OdSlideTablePtr& slide_table, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdCheckBox, checkbox> to the page.
    
    \param check_box [in] A smart pointer to a checkbox object that should be added.
    \param location  [in] A rectangle that specifies the location of added checkbox within the page. It determines where the checkbox is placed at the page.
    
    \sa
    <link OdPdfPublish::OdCheckBox, OdCheckBox class>.
  */
  void addCheckBox(const OdCheckBoxPtr& check_box, const OdRect& location);

  /** \details
    Adds a <OdPdfPublish::OdRadioButton, radio button> to the page.
    
    \param radio_button [in] A smart pointer to a radio button object that should be added.
    \param location     [in] A rectangle that specifies the location of added radio button within the page. It determines where the radio button is placed at the page.
    
    \sa
    <link OdPdfPublish::OdRadioButton, OdRadioButton class>.
  */
  void addRadioButton(const OdRadioButtonPtr& radio_button, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdListBox, list box> to the page.
    
    \param list_box     [in] A smart pointer to a list box object that should be added.
    \param location     [in] A rectangle that specifies the location of added list box within the page. It determines where the list box is placed at the page.
    
    \sa
    <link OdPdfPublish::OdListBox, OdListBox class>.
  */
  void addListBox(const OdListBoxPtr& list_box, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdDropDownList, drop-down list> (combo box) to the page.
    
    \param drop_down_list [in] A smart pointer to a drop-down list object that should be added.
    \param location       [in] A rectangle that specifies the location of added drop-down list within the page. It determines where the drop-down list is placed at the page.
    
    \sa
    <link OdPdfPublish::OdDropDownList, OdDropDownList class>.
  */
  void addDropDownList(const OdDropDownListPtr& drop_down_list, const OdRect& location);

  /** \details
    Adds a <link OdPdfPublish::OdSignatureField, signature field> to the page.
    
    \param signature_field  [in] A smart pointer to a signature field object that should be added.
    \param location         [in] A rectangle that specifies the location of added signature field within the page. It determines where the signature field is placed at the page.
    
    \sa
    <link OdPdfPublish::OdSignatureField, OdSignatureField class>.
  */
  void addSignatureField(const OdSignatureFieldPtr& signature_field, const OdRect& location);

  void add2dGeometry(const Od2dGeometryReferencePtr& geom, const OdRect& location);
  void add2dGeometry(const Od2dGeometryReferencePtr& geom);

  void addCADReference(const OdCADReferencePtr& CADreferense, const OdRect& location);

  void addCADReference(const OdCADReferencePtr& CADreferense);
 
  void addJavaScriptActionByField(const OdString& field_name, const OdString& source, const Action::Type action_type, const Source::Type source_type = Source::kCode);

  /** \details
    Returns current page format value.
    
    \param format [out] A placeholder for the format value. Format value is stored here to be returned to the calling subroutine.
    
    \sa
    <link OdPdfPublish::Page::Format, Page::Format enumeration>.
  */
  void getFormat(Page::Format& format) const;
  
  /** \details
    Returns current page orientation value.
    
    \param orientation [out] A placeholder for the orientation value. Format value is stored here to be returned to the calling subroutine.
    
    \sa
    <link OdPdfPublish::Page::Orientation, Page::Orientation enumeration>.
  */
  void getOrientation(Page::Orientation& orientation) const;

  void getPaperSize(const Page::PaperUnits units, double& width, double& height) const;

  /** \details
    Returns all <link OdPdfPublish::OdAnnotation, annotations>, which the page contains, and their locations.
    The method fills two passed arrays with annotation objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param annotations [out] A placeholder for the page annotations array. 
    \param locations   [out] A placeholder for the annotation locations array. 
    
    \sa
    <link OdPdfPublish::OdAnnotation, OdAnnotation class>.
  */
  void getAnnotations(OdAnnotationPtrArray& annotations, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdText, text labels>, which the page contains, and their locations.
    The method fills two passed arrays with text label objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param texts     [out] A placeholder for the page text labels array. 
    \param locations [out] A placeholder for the text label locations array. 
    
    \sa
    <link OdPdfPublish::OdText, OdText class>.
  */
  void getTexts(OdTextPtrArray& texts, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdImage, images>, which the page contains, and their locations.
    The method fills two passed arrays with image objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param images      [out] A placeholder for the page images array. 
    \param locations   [out] A placeholder for the image locations array. 
    
    \sa
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void getImages(OdImagePtrArray& images, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdTable, tables>, which the page contains, and their locations.
    The method fills two passed arrays with table objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param tables    [out] A placeholder for the page tables array. 
    \param locations [out] A placeholder for the table locations array. 
    
    <link OdPdfPublish::OdTable, OdTable class>.
  */
  void getTables(OdTablePtrArray& tables, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdLink, links>, which the page contains, and their locations.
    The method fills two passed arrays with link objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param links     [out] A placeholder for the page link array. 
    \param locations [out] A placeholder for the link locations array. 
    
    \sa
    <link OdPdfPublish::OdLink, OdLink class>.
  */
  void getLinks(OdLinkPtrArray& links, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdButton, buttons>, which the page contains, and their locations.
    The method fills two passed arrays with button objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param buttons   [out] A placeholder for the page button array. 
    \param locations [out] A placeholder for the link locations array. 
    
    \sa
    <link OdPdfPublish::OdButton, OdButton class>.
  */
  void getButtons(OdButtonPtrArray& buttons, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdCheckBox, checkboxes>, which the page contains, and their locations.
    The method fills two passed arrays with checkboxes objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param check_boxes [out] A placeholder for the page checkbox array. 
    \param locations   [out] A placeholder for the checkbox locations array. 
    
    \sa
    <link OdPdfPublish::OdCheckBox, OdCheckBox class>.
  */
  void getCheckBoxes(OdCheckBoxPtrArray& check_boxes, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdRadionButton, radio buttons>, which the page contains, and their locations.
    The method fills two passed arrays with radion button objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param radio_buttons [out] A placeholder for the page radio buttons array. 
    \param locations     [out] A placeholder for the radio button locations array. 
    
    \sa
    <link OdPdfPublish::OdRadionButton, OdRadionButton class>.
  */
  void getRadioButtons(OdRadioButtonPtrArray& radio_buttons, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdListBox, list boxes>, which the page contains, and their locations.
    The method fills two passed arrays with list box objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param list_boxes [out] A placeholder for the page list boxes array. 
    \param locations  [out] A placeholder for the list box locations array. 
    
    \sa
    <link OdPdfPublish::OdListBox, OdListBox class>.
  */
  void getListBoxes(OdListBoxPtrArray& list_boxes, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdDropDownList, drop-down lists>, which the page contains, and their locations.
    The method fills two passed arrays with drop-down list objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param drop_down_list [out] A placeholder for the page drop-down lists array. 
    \param locations      [out] A placeholder for the drop-down list locations array. 
    
    <link OdPdfPublish::OdDropDownList, OdDropDownList class>.
  */
  void getDropDownLists(OdDropDownListPtrArray& drop_down_lists, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdSignatureField, signature fields>, which the page contains, and their locations.
    The method fills two passed arrays with signature field objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param signature_fields [out] A placeholder for the page signature fields array. 
    \param locations        [out] A placeholder for the signature field locations array. 
    
    \sa
    <link OdPdfPublish::OdSignatureField, OdSignatureField class>.
  */  
  void getSignatureFields(OdSignatureFieldPtrArray& signature_fields, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdTextField, text fields>, which the page contains, and their locations.
    The method fills two passed arrays with text field objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param text_fields [out] A placeholder for the page text fields array. 
    \param locations   [out] A placeholder for the text field locations array. 
    
    \sa
    <link OdPdfPublish::OdTextField, OdTextField class>.
  */
  void getTextFields(OdTextFieldPtrArray& text_fields, OdRectArray& locations) const;

  /** \details
    Returns all <link OdPdfPublish::OdSlideTable, slide tables>, which the page contains, and their locations.
    The method fills two passed arrays with slide table objects and their locations, then returns these arrays to a calling subroutine. 
    
    \param slide_tables [out] A placeholder for the page slide tables array. 
    \param locations    [out] A placeholder for the slide table locations array. 
    
    \sa
    <link OdPdfPublish::OdSlideTable, OdSlideTable class>.
  */
  void getSlideTables(OdSlideTablePtrArray& slide_tables, OdRectArray& locations) const;

  void getJavaScriptActionsByField(const OdString& field_name, OdStringArray& sources, OdActionTypeArray& action_types, OdSourceTypeArray& source_types) const;

  void get2dGeoms(Od2dGeometryReferencePtrArray& geoms, OdRectArray& locations) const;

  void getCADReferences(OdCADReferencePtrArray& CADreferenses, OdRectArray& locations) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdPage, OdPage> object.
*/
SMARTPTR(OdPage);

}
#endif // _PDF_PUBLISH_PAGE_INCLUDED_

