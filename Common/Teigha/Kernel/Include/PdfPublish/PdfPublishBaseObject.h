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
// PdfPublishBaseObject.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_BASE_OBJECT_INCLUDED_
#define _PDF_PUBLISH_BASE_OBJECT_INCLUDED_

#include "PdfPublishDef.h"
#include "RxObject.h"
#include "Ge/GePoint2d.h"
#include "Gs/GsDefs.h"
#include "PdfPublishCommon.h"

/** \details 
Namespace contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

  /** \details 
  Defines the data type for representing a two-dimensional rectangle object.
  
  \sa
  <exref target="https://docsdev.opendesign.com/td/OdGsDCRect.html"> OdGsDCRect Class</exref>
  */
  typedef OdGsDCRect OdRect;
  
  /** \details 
  Defines the data type for representing an array of two-dimensional rectangles.
  
  \sa
  <exref target="https://docsdev.opendesign.com/td/OdArray.html">OdArray Template Class</exref>
  */
  typedef OdArray<OdRect> OdRectArray;

  typedef OdArray<Action::Type> OdActionTypeArray;

  typedef OdArray<Source::Type> OdSourceTypeArray;

  //DOM-IGNORE-BEGIN
 class OdPublishObjectImpl;
  //DOM-IGNORE-END
 
   /** \details
   This class implements the base class for Teigha Publish objects.
   */
 class PDFPUBLISH_TOOLKIT OdObject : public OdRxObject
 {
  //DOM-IGNORE-BEGIN
   friend class OdPublishObjectImpl;
   OdObject();
   OdPublishObjectImpl* m_pImpl;
 protected:
      OdObject(OdPublishObjectImpl* pImpl);
  //DOM-IGNORE-END
 public:
    //DOM-IGNORE-BEGIN
   ODRX_DECLARE_MEMBERS(OdObject);
   ODRX_HEAP_OPERATORS();
    //DOM-IGNORE-END
 
    /** \details 
    Destroys an instance of Teigha Publish object.
    */
   virtual ~OdObject();
 
    /** \details 
    Returns true if the Teigha Publish object is empty, otherwise returns false.
    */
   virtual bool isEmpty() const;
   
    /** \details 
    Returns true if the Teigha Publish object is valid, otherwise returns false.
    */
   virtual bool isValid() const;
   
    /** \details 
    Clears the Teigha Publish object.
    */
   virtual void clear();
 
 };
 
 #define SMARTPTR(classname) class classname; typedef OdSmartPtr<classname> classname##Ptr; typedef OdArray<classname##Ptr> classname##PtrArray;

  /** \details
   A data type that represents smart pointer to an OdPublishObject object.
  */
 SMARTPTR(OdObject);

}
#endif // _PDF_PUBLISH_BASE_OBJECT_INCLUDED_

