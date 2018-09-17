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
// PdfPublishView.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_VIEW_INCLUDED_
#define _PDF_PUBLISH_VIEW_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishCamera.h"

/** \details 
  <group TD_Namespaces> 
  Namespace contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a view entity for .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdView : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdView();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdView);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys a view object.
  */
  virtual ~OdView();

  /** \details 
    Sets an internal name of the view.
   
    \param name [in] A new name value.
  */
  void setInternalName(const OdString& name);

  /** \details 
    Sets an external name of the view.
   
    \param name [in] A new name value.
  */
  void setExternalName(const OdString& name);
  
  /** \details 
    Sets the view camera.
    
    \param camera [in] A smart pointer to the camera object.
  */
  void setCamera(const OdCameraPtr& camera);
  
  /** \details 
    Sets the view background color.
    
    \param color [in] A background color value.
  */  
  void setBackgroundColor(ODCOLORREF color);
  
  /** \details
    Sets a flag that determines whether the view is the default.
    If the flag value is equal to true, the view is the default, otherwise it is not.
    
    \param state [in] A new default flag value.
  */
  void setDefault(bool state);
  
  /** \details 
    Sets the view lighting mode.
    
    \param mode [in] A new lighting mode value.
    
    \sa
    <link OdPdfPublish::Lighting::Mode, Lighting::Mode enumeration>.
  */
  void setLighting(Lighting::Mode mode);
  
  /** \details 
    Sets the view rendering mode.
    
    \param mode [in] A new rendering mode value.
    
    \sa
    <link OdPdfPublish::Rendering::Mode, Rendering::Mode enumeration>.
  */
  void setRendering(Rendering::Mode mode);

  /** \details 
    Returns current internal name of the view.
    The method fills a passed string object and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the name value.
  */
  void getInternalName(OdString& name) const;
  
  /** \details 
    Returns current external name of the view.
    The method fills a passed string object and returns it to a calling subroutine.
    
    \param name [out] A placeholder for the name value.
  */
  void getExternalName(OdString& name) const;
  
  /** \details 
    Returns current view camera.
    The method fills a passed smart pointer to a camera object and returns it to a calling subroutine.
    
    \param camera [out] A placeholder for the smart pointer to the camera object.
  */
  void getCamera(OdCameraPtr& camera) const;
  
  /** \details
    Returns the current default flag value.
    The method fills a passed boolean value and returns it to a calling subroutine.
    
    \param state [out] A placeholder for the default flag value.
    
    \remarks
    If the flag value is equal to true, the view is the default, otherwise it is not.
  */
  void getDefault(bool& state) const;
  
  /** \details 
    Returns current view background color.
    The method fills a passed ODCOLORREF value and returns it to a calling subroutine.
    
    \param color [out] A placeholder for the background color value.
  */
  void getBackgroundColor(ODCOLORREF& color) const;
  
  /** \details 
    Returns current view lighting mode.
    The method fills a passed lighting mode value and returns it to a calling subroutine.
    
    \param mode [out] A placeholder for the lighting mode value.
    
    \sa
    <link OdPdfPublish::Lighting::Mode, Lighting::Mode enumeration>.
  */
  void getLighting(Lighting::Mode& mode) const;
  
  /** \details 
    Returns current view rendering mode.
    The method fills a passed rendering mode value and returns it to a calling subroutine.
    
    \param mode [out] A placeholder for the rendering mode value.
    
    \sa
    <link OdPdfPublish::Rendering::Mode, Rendering::Mode enumeration>.
  */
  void getRendering(Rendering::Mode& mode) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdView, OdView> object.
*/
SMARTPTR(OdView);

}
#endif // _PDF_PUBLISH_VIEW_INCLUDED_

