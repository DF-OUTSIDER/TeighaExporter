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
// PdfPublishAnnotation.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_ANNOTATION_INCLUDED_
#define _PDF_PUBLISH_ANNOTATION_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishArtwork.h"
#include "PdfPublishImage.h"
#include "PdfPublishModel.h"
#include "PdfPublishListBox.h"
#include "PdfPublishTextField.h"
#include "UInt32Array.h"
#include "StringArray.h"
#include "DoubleArray.h"
 

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements an annotation entity for .pdf documents created with Teigha Publish.
  Annotation is a user space object that stores a set of object of different types (acts like a container for other objects and controls).
  Annotation can contain compressed and uncompressed .prc tessellations, .prc boundary representation data , .prc attributes, carousel buttons, 
  which allow to interactively switch between different views. Annotation entity is based on a <link OdPdfPublish::OdCADModel, CAD model>. 
  <link OdPdfPublish::OdCADModel, CAD model> represents any drawing in format supported by Teigha (for example, a .dwg drawing).
*/
class PDFPUBLISH_TOOLKIT OdAnnotation : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdAnnotation();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdAnnotation);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys an annotation object.
  */
  virtual ~OdAnnotation();

  /** \details
    Sets the annotation source.
    Source contains drawing data that should be exported to the annotation.
    
    \param cad_model [in] A smart pointer to an <link OdPdfPublish::OdCADModel, OdCADModel> object to be set as the annotation source.
    
    \sa
    <link OdPdfPublish::OdCADModel, OdCADModel>
  */
  void setSource(const OdCADModelPtr& cad_model);
  
  /** \details
    Sets the compression flag value for .prc tessellation of the annotation.
    Compression algorithm for .prc tessellation is applied if the flag value is true, otherwise .prc tessellation is processed without compressing.
    
    \param state [in] A new tessellation compress flag value.
  */
  void setPRCTessellationCompression(bool state);
  
  /** \details
    Sets the compression level for .prc boundary representation data that the annotation exports.
    
    \param level [in] A new compression level to be applied.
    
    \sa
    <link OdPdfPublish::PRC::BRepCompression, BRepCompression enumeration>
  */
  void setPRCBRepCompression(PRC::BRepCompression level);
  
  /** \details
    Sets the removing flag for .prc boundary representation data that the annotation exports.
    If the flag is equal to true, b-rep data is removed from the annotation, otherwise it stays in the annotation.
    
    \param state [in] A new b-rep data removing flag value.
  */  
  void setPRCBRepRemoval(bool state);
  
  /** \details
    Sets the removing flag for .prc attribute information that the annotation exports.
    If the flag is equal to true, attribute information is removed from the annotation, otherwise it stays in the annotation.
    
    \param state [in] A new attributes data removing flag value.
  */    
  void setPRCAttributeRemoval(bool state);
  
  /** \details
    Sets the annotation name.
    
    \param name [in] A name value.
  */  
  void setName(const OdString& name);
  
  /** \details
    Sets an <link OdPdfPublish::OdArtwork, artwork> object of the annotation.
    
    \param artwork [in] An <link OdPdfPublish::OdArtwork, artwork> object.
  */  
  void setArtwork(const OdArtworkPtr& artwork);

  /** \details
    Sets the visibility flag of the annotation toolbar.
    If the flag is true, the toolbar is diplayed when the annotation becomes active, otherwise toolbar stays hidden.
    
    \param state [in] A visibility flag value.
  */  
  void setToolbarVisibility(bool state);
  
  /** \details
    Sets the visibility flag of the annotation model tree.
    If the flag is true, the model tree is diplayed when the annotation becomes active, otherwise model tree stays hidden.
    
    \param state [in] A visibility flag value.
  */
  void setModelTreeVisibility(bool state);
  
  /** \details
    Sets the interactivity flag of the annotation.
    If the flag is true, the annotation interactivity is enabled, otherwise interactivity is disabled.
    
    \param state [in] An interactivity flag value.
  */
  void setInteractivity(bool state);
  
  /** \details
    Sets the border width of the annotation.
    Border width is measured in points.
    
    \param width [in] A border width value (in points).
  */
  void setBorderWidth(OdUInt16 width);
  
  /** \details
    Sets the transparency flag of the annotation.
    If the flag is true, the annotation background is transparent, otherwise the annotation background is opaque.
    
    \param state [in] A transparency flag value.
  */
  void setTransparentBackground(bool state);
  
  /** \details
    Sets the background color of the annotation.
    
    \param color [in] A background color value.
  */
  void setBackgroundColor(ODCOLORREF color);
  
  /** \details
    Sets the annotation postger <link OdPdfPublish::OdImage, image>.
    
    \param image [in] A smart pointer to an <link OdPdfPublish::OdImage, image> object that should be used as poster.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void setPosterImage(const OdImagePtr& image);

  /** \details
    Sets the annotation activation type.
    Activation type determines when the annotation becomes active.
    
    \param when [in] An activation type value.
    
    \sa 
    <link OdPdfPublish::Activation::When, Activation::When enumeration>.
  */
  void setActivation(Activation::When when);
  
  /** \details
    Sets the annotation deactivation type.
    Deactivation type determines when the annotation becomes inactive.
    
    \param when [in] A deactivation type value.
    
    \sa
    <link OdPdfPublish::Deactivation::When, Deactivation::When enumeration>.
  */
  void setDeactivation(Deactivation::When when);
  
  /** \details
    Sets the annotation lighting mode.
    
    \param mode [in] A lighting mode value.
    
    \sa
    <link OdPdfPublish::Lighting::Mode, Lighting::Mode enumeration>.
  */  
  void setLighting(Lighting::Mode mode);
  
  /** \details
    Sets the annotation rendering mode.
    
    \param mode [in] A rendering mode value.
    
    \sa 
    <link OdPdfPublish::Rendering::Mode, Rendering::Mode enumeration>.
  */
  void setRendering(Rendering::Mode mode);

  /** \details
    Sets the annotation carousel buttons.

    \param button_names         [in] An array of button names.
    \param previous_button_name [in] A name of previous button.
    \param next_button_name     [in] A name of previous button.
    \param scroll_size          [in] A scroll size value.
    
    \remarks
    
    Carousel buttons array includes a collection of buttons to switch between different views (each button contains a small preview) 
    and two specific buttons for switching to a previous or next view relatively the view that is currently displayed. 
    The scroll size property determines how many buttons are scrolled when user presses next or previous button.
    
    It is necessary to set carousel buttons to generate carousel view.
  */
  void setCarouselButtons(const OdStringArray& button_names, const OdString& previous_button_name, const OdString& next_button_name, OdUInt16 scroll_size);
  
  /** \details
    Sets carousel views of the annotation.

    \param indices [in] An array of view indices.
    \param images  [in] An array of smart pointers to image objects, which are used as button preview images.
    
    \remarks
    Carousel view array includes a collection of views to represent different points of view to a published drawing.
    To switch from one view to another a set of carousel buttons is used. Each button (except previous and next buttons) is associated with a particular view. 
  */
  void setCarouselViews(const OdUInt32Array& indices, const OdImagePtrArray& images);
  
  /** \details
    Sets a new list of view names concatenated in a single string.

    \param field [out] A string containing view names.

    \remarks 
    The name is mandatory and must be unique among the document object names.
  */
  void setViewListByField(const OdString& field);
  
  /** \details
    Sets a new list box object that contains the list of view names.

    \param location [out] A list box location.
    \param list		  [out] A list box object.
  */
  void setViewList(const OdRect& location, const OdListBoxPtr& list);
  
  void setPartsList(const OdStringArray node_names, const OdRect& location, const OdStringArray& headers, const OdDoubleArray& columns, 
    const OdTextFieldPtr text_style = OdTextFieldPtr(), const OdTextFieldPtr header_style = OdTextFieldPtr());

  void setPropertyList(const OdRect& location);

  void setPropertyToTextField(const OdString& property_name, const OdTextFieldPtr text_fields);

  /** \details
    Returns the annotation source.
    The method fills a passed smart pointer to the model object and returns it to a calling subroutine.
    
    \param cad_model [out] A placeholder for the smart pointer to the model.    
    
    \remarks
    Source contains drawing data that has been exported to the annotation.
    
    \sa
    <link OdPdfPublish::OdCADModel, OdCADModel class>.
  */
  void getSource(OdCADModelPtr& cad_model) const;
  
  /** \details
    Returns current value of the compression flag for .prc tessellation of the annotation.
    The method fills a passed boolean value and returns it to a calling subroutine.

    \param state [out] A placeholder for the tessellation compress flag value.
    
    \remarks
    Compression algorithm for .prc tessellation is applied if the flag value is true, otherwise .prc tessellation is processed without compressing.
  */
  void getPRCTessellationCompression(bool& state) const;
  
  /** \details
    Returns current value of the compression level for .prc boundary representation data that the annotation exports.
    The method fills a passed level value and returns it to a calling subroutine.
    
    \param level [out] A placeholder for the compression level value.
    
    \sa
    <link OdPdfPublish::PRC::BRepCompression, PRC::BRepCompression enumeration>.
  */
  void getPRCBRepCompression(PRC::BRepCompression& level) const;
  
  /** \details
    Returns current value of the removing flag for .prc boundary representation data that the annotation exports.
    The method fills a passed boolean value and returns it to a calling subroutine.

    \param state [out] A placeholder for the value of the b-rep data removing flag.
    
    \remarks 
    If the flag is equal to true, b-rep data is removed from the annotation, otherwise it stays in the annotation.
  */
  void getPRCBRepRemoval(bool& state) const;
  
  /** \details
    Returns current value of the the removing flag for .prc attribute information that the annotation exports.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the value of the attribute data removing flag.
    
    \remarks
    If the flag is equal to true, attribute information is removed from the annotation, otherwise it stays in the annotation.
  */
  void getPRCAttributeRemoval(bool& state) const;
  
  /** \details
    Returns the annotation name.
    The method fills a passed string object and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the annotation name.
  */
  void getName(OdString& name) const;
  
  /** \details
    Returns a smart pointer to the annotation <link OdPdfPublish::OdArtwork, artwork> object.
    The method fills a passed smart pointer value and returns it to a calling subroutine.
    
    \param artwork [out] A placeholder for the smart pointer to the annotation artwork.
    
    \sa
    <link OdPdfPublish::OdArtwork, OdArtwork class>.
  */
  void getArtwork(OdArtworkPtr& artwork) const;

  /** \details
    Returns current value of the visibility flag of the annotation toolbar.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the visibility flag value.
    
    \remarks
    If the flag is true, the toolbar is diplayed when the annotation becomes active, otherwise toolbar stays hidden.
  */  
  void getToolbarVisibility(bool& state) const;
  
  /** \details
    Returns current value of the visibility flag of the annotation model tree.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the visibility flag value.
    
    \remarks
    If the flag is true, the model tree is diplayed when the annotation becomes active, otherwise model tree stays hidden.
  */
  void getModelTreeVisibility(bool& state) const;
  
  /** \details
    Returns current value of the interactivity flag of the annotation.
    The method fills a passed boolean value and returns it to a calling subroutine.

    \param state [out] A placeholder for the interactivity flag value.
    
    \remarks
    If the flag is true, the annotation interactiviy is enabled, otherwise interactivity is disabled.
  */
  void getInteractivity(bool& state) const;
  
  /** \details
    Returns current border width of the annotation.
    The method fills a passed value and returns it to a calling subroutine.
    
    \param width [out] A placeholder for the border width value (in points).
    
    \remarks 
    Border width is measured in points.
  */
  void getBorderWidth(OdUInt16& width) const;
  
  /** \details
    Returns current value of the transparency flag of the annotation.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the transparency flag value.    
    
    \remarks 
    If the flag is true, the annotation background is transparent, otherwise the annotation background is opaque.
  */
  void getTransparentBackground(bool& state) const;
  
  
  /** \details
    Returns the annotation background color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the background color value.
  */
  void getBackgroundColor(ODCOLORREF& color) const;
  
  /** \details
    Returns the annotation postger image.
    The method fills a passed smart pointer to an <link OdPdfPublish::OdImage, image> object and returns it to a calling subroutine.
    
    \param image [out] A placeholder for the smart pointer to an image object.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void getPosterImage(OdImagePtr& image) const;

  /** \details
    Returns the annotation activation type.
    The method fills a passed activation type value and returns it to a calling subroutine.
    
    \param when [out] A placeholder for the activation type value.
    
    \remarks
    Activation type determines when the annotation becomes active.
    
    \sa
    <link OdPdfPublish::Activation::When, Activation::When enumeration>.
  */
  void getActivation(Activation::When& when) const;
  
  /** \details
    Returns the annotation deactivation type.
    The method fills a passed deactivation type value and returns it to a calling subroutine.

    \param when [out] A placeholder for the deactivation type value.
    
    \remarks
    Dectivation type determines when the annotation becomes inactive.
    
    \sa 
    <link OdPdfPublish::Deactivation::When, Deactivation::When enumeration>.
  */
  void getDeactivation(Deactivation::When& when) const;
  
  /** \details
    Returns the annotation lighting mode.
    The method fills a passed lighting mode value and returns it to a calling subroutine.
    
    \param mode [out] A placeholder for the lighting mode value.
    
    \sa 
    <link OdPdfPublish::Lighting::Mode, Lighting::Mode enumeration>.
  */  
  void getLighting(Lighting::Mode& mode) const;
  
  /** \details
    Returns the annotation rendering mode.
    The method fills a passed rendering mode value and returns it to a calling subroutine.
    
    \param mode [out] A placeholder for the rendering mode value.
    
    \sa 
    <link OdPdfPublish::Rendering::Mode, Rendering::Mode enumeration>.
  */
  void getRendering(Rendering::Mode& mode) const;

  /** \details
    Returns the annotation carousel buttons information.
    Information contains button names, including previous and next button names and scrolling size value.
    The method fills passed objects with button information and returns them to a calling subroutine.
    
    \param button_names         [out] A placeholder for the array of button names.
    \param previous_button_name [out] A placeholder for the name of previous button.
    \param next_button_name     [out] A placeholder for the name of previous button.
    \param scroll_size          [out] A placeholder for the scroll size value.
    
    \remarks
    Carousel buttons array includes a collection of buttons to switch between different views (each button contains a small preview) 
    and two specific buttons for switching to a previous or next view relatively the view that is currently displayed. 
    The scroll size property determines how many buttons are scrolled when user presses next or previous button.
    
    It is necessary to set carousel buttons to generate carousel view.
  */
  void getCarouselButtons(OdStringArray& button_names, OdString& previous_button_name, OdString& next_button_name, OdUInt16& scroll_size) const;
  
  /** \details
    Returns carousel views of the annotation.
    Views information contains an array of view indicies and array of button icons.
    The method fills passed objects with data about views and returns them to a calling subroutine.

    \param indices [out] A placeholder for the array of view indices.
    \param images   [out] A placeholder for the array of smart pointers to image objects, which are used as button preview images.
    
    \remarks
    Carousel view array includes a collection of views to represent different points of view to a published drawing.
    To switch from one view to another a set of carousel buttons is used. Each button (except previous and next buttons) is associated with a particular view. 
  */
  void getCarouselViews(OdUInt32Array& indices, OdImagePtrArray& images) const;
  
  /** \details
    Returns a list of view names concatenated in a single string.
    The method fills passed string objects with view names and returns it to a calling subroutine.

    \param field [out] A placeholder for the string containing view names.
  */
  void getViewListField(OdString& field) const;

  /** \details
    Returns the list box object that contains the list of view names.
    The method fills passed location and list box objects with, then returns them to a calling subroutine.

    \param location [out] A placeholder for the list box location.
    \param list		[out] A placeholder for the list box object.
  */
  void getViewList(OdRect& location, OdListBoxPtr& list) const;

  void getPartsList(OdStringArray& node_names, OdRect& location, OdStringArray& headers, OdDoubleArray& columns, 
    OdTextFieldPtr text_style, OdTextFieldPtr header_style) const;

  void getPropertyList(OdRect& location) const;

  void getPropertyToTextField(const OdString& property_name, OdTextFieldPtrArray& text_field) const;
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdAnnotation, OdAnnotation> object.
*/
SMARTPTR(OdAnnotation);

}
#endif // _PDF_PUBLISH_ANNOTATION_INCLUDED_

