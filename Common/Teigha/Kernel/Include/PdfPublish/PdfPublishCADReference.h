/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// PdfPublishCADReference.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_CADREFERENCE_INCLUDED_
#define _PDF_PUBLISH_CADREFERENCE_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublishCADDefinition.h"


/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements an animation in .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdCADReference : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdCADReference();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdCADReference);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys an object.
  */
  virtual ~OdCADReference();

  void setCADDefinition(OdCADDefinitionPtr cad_definition);
  void setBorder(bool state);
  void setBorderColor(ODCOLORREF color);
  void setBorderWidth(const OdUInt16 width);
  void setRotation(double rotation); /*in radians*/
  void setClipBoundary(const Page::PaperUnits units, const OdGePoint2dArray& clipBoundary);
  void setTransform(const OdGeMatrix2d& xfm);
  void setScale(double scale);
  void setTranslation(const OdGeVector2d& vec);

  void getCADDefinition(OdCADDefinitionPtr& cad_definition) const;
  void getBorder(bool& state) const;
  void getBorderColor(ODCOLORREF& color) const;
  void getBorderWidth(OdUInt16& width) const;
  void getRotation(double& rotation) const;
  void getClipBoundary(Page::PaperUnits& units, OdGePoint2dArray& clipBoundary) const;
  void getTransform(OdGeMatrix2d& xfm) const;
  void getScale(double& scale) const;
  void getTranslation(OdGeVector2d& vec) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdCADReference, OdCADReference> object.
*/
SMARTPTR(OdCADReference);

}
#endif // _PDF_PUBLISH_CADREFERENCE_INCLUDED_

