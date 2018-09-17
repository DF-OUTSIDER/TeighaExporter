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
// PdfPublishArtwork.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_ARTWORK_INCLUDED_
#define _PDF_PUBLISH_ARTWORK_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishView.h"
#include "PdfPublishAnimation.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements an artwork entity for .pdf documents created with Teigha Publish.
  Artwork is a user space object that stores a set of object of different types (acts like a container for other objects and controls).
*/
class PDFPUBLISH_TOOLKIT OdArtwork : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdArtwork();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdArtwork);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys an artwork object.
  */
  virtual ~OdArtwork();

  /** \details
    Sets an animation style of the artwork.
    
    \param style [in] A new animation style value.
    
    \sa 
    <link OdPdfPublish::Artwork::AnimationStyle, Artwork::AnimationStyle enumeration>.
  */
  void setAnimationStyle(Artwork::AnimationStyle style);
  
  /** \details
    Sets a JavaScript scenario for the artwork.
    
    \param source [in] A string object that contains JavaScript scenario source code.
  */
  void setJavaScript(const OdString& source, const Source::Type source_type = Source::kCode);
  
  /** \details
    Sets the cross highlighting flag for the Picture Mastering Index (PMI) color of the artwork entity.
    If the flag is set to true, the geometry related to a piece of PMI is hightlighted when the PMI is selected, otherwise is not.

    \param state [in] A new PMI cross highlighting flag value.    
  */
  void setPMICrossHighlighting(bool state);
  
  /** \details
    Sets the semantic data flag for the Picture Mastering Index (PMI) of the artwork entity.
    If the flag is set to true, the artwork entity stores the semantic data, otherwise it does not.

    \param state [in] A new PMI semantic data flag value.    
  */
  void setPMISemanticInformation(bool state);
  
  /** \details
    Sets preferred view options source for the artwork.

    \param preference [in] A new source of preferred view options.
    
    \remarks 
    View options can be taken from .prc or .pdf data (if such data exists).
    
    \sa 
    <link OdPdfPublish::Artwork::ViewPreference, Artwork::ViewPreference enumeration>.
  */
  void setDefaultViewPreference(Artwork::ViewPreference preference);
  
  /** \details
    Sets the source of preferred display options for the artwork entity.

    \param preference [in] A new source of preferred display otpions.
    
    \remarks 
    Display options can be taken from a view options or from an application display settings.
    
    \sa
    <link OdPdfPublish::Artwork::DisplayPreference, Artwork::DisplayPreference>.
  */
  void setDisplayPreference(Artwork::DisplayPreference preference);
  
  /** \details
    Add a new <link OdPdfPublish::OdView, view> to the artwork.

    \param view [in] A smart pointer to a view that should be added.
    
    \sa 
    <link OdPdfPublish::OdView, View class>.
  */
  void addView(const OdViewPtr& view);
  
  /** \details
    Sets a Picture Mastering Index color used by the artwork.

    \param pmi_color [in] A a new PMI color value.
  */
  void setPMIColor(ODCOLORREF pmi_color);

  void addAnimation(const OdAnimationPtr& animation);

  /** \details
    Returns current animation style of the artwork.
    The method fills a passed animation style and returns it to a calling subroutine.
    
    \param style [out] A placeholder for the animation style value.
    
    \sa
    <link OdPdfPublish::Artwork::AnimationStyle, Artwork::AnimationStyle enumeration>.
  */
  void getAnimationStyle(Artwork::AnimationStyle& style) const;
  
  /** \details
    Returns current JavaScript scenario for the artwork.
    The method fills a passed string object with JavaScript scenario source code and returns it to a calling subroutine.
    
    \param source [out] A placeholder for the JavaScript scenario source code.
  */
  void getJavaScript(OdString& source, Source::Type& source_type) const;
  
  /** \details
    Returns current value of the cross highlighting flag for the Picture Mastering Index (PMI) color.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the PMI cross highlighting flag value.    
    
    \remarks 
    If the flag is set to true, the geometry related to a piece of PMI is hightlighted when the PMI is selected, otherwise is not.
  */
  void getPMICrossHighlighting(bool& state) const;
  
  /** \details
    Returns current value of the PMI semantic data flag.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the value of PMI semantic data flag.
    
    \remarks
    If the flag is set to true, the artwork entity stores the semantic Picture Mastering Index data, otherwise it does not.
  */
  void getPMISemanticInformation(bool& state) const;
  
  /** \details
    Returns current source of preferred view options for the artwork.
    The method fills a passed options source value and returns it to a calling subroutine.

    \param preference [out] A placeholder for the source of preferred view options.
    
    \remarks 
    View options can be taken from .prc or .pdf data (if such data exists).
    
    \sa 
    <link OdPdfPublish::Artwork::ViewPreference, Artwork::ViewPreference enumeration>.
  */
  void getDefaultViewPreference(Artwork::ViewPreference& preference) const;
  
  /** \details
    Returns current source of preferred display options for the artwork entity.
    The method fills a passed options source value and returns it to a calling subroutine.

    \param preference [out] A placeholder for the new source of preferred display options.
    
    \remarks 
    Display options can be taken from a view options or from an application display settings.
    
    \sa 
    <link OdPdfPublish::Artwork::DisplayPreference, Artwork::DisplayPreference enumeration>.
  */
  void getDisplayPreference(Artwork::DisplayPreference& preference) const;
  
/** \details
    Returns an array of artwork <link OdPdfPublish::OdView, views>.
    The method fills a passed view array object and returns it to a calling subroutine.

    \param views [out] A smart pointer to a view array that should be filled by the method.
    
    \sa 
    <link OdPdfPublish::OdView, OdView class>.
  */
  void getViews(OdViewPtrArray& views) const;
  
  /** \details
    Returns current Picture Mastering Index color used by the artwork.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.

    \param pmi_color [out] A a placeholder for the PMI color value.
  */
  void getPMIColor(ODCOLORREF& pmi_color) const;

  void getAnimations(OdAnimationPtrArray& animations) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdArtwork, OdArtwork> object.
*/
SMARTPTR(OdArtwork);

}
#endif // _PDF_PUBLISH_ARTWORK_INCLUDED_

