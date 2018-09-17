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
// PdfPublish2dGeometryReference.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_2DGEOM_REF_INCLUDED_
#define _PDF_PUBLISH_2DGEOM_REF_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublish2dGeometryBlock.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements an 2d geometry in .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT Od2dGeometryReference : public OdObject
{
protected:
  //DOM-IGNORE-BEGIN
  Od2dGeometryReference();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(Od2dGeometryReference);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys an object.
  */
  virtual ~Od2dGeometryReference();

  void setBorder(bool state);
  void setBorderColor(ODCOLORREF color);
  void getBorder(bool& state) const;
  void getBorderColor(ODCOLORREF& color) const;

  void setTransform(const OdGeMatrix2d& xfm); //transformation matrix which overrides scale, rotation and location of the reference
  void getTransform(OdGeMatrix2d& xfm) const;

  void setGeometryBlock(const Od2dGeometryBlockPtr& block);
  void getGeometryBlock(Od2dGeometryBlockPtr& block) const;

  void setScale(double scale);
  void getScale(double& scale) const;

  void setRotation(double rotation = 0); /*in radians*/
  void getRotation(double& rotation) const;

  void setTranslation(const OdGeVector2d& vec);
  void getTranslation(OdGeVector2d& vec) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::Od2dGeometryReference, Od2dGeometryReference> object.
*/
SMARTPTR(Od2dGeometryReference);

}
#endif // _PDF_PUBLISH_2DGEOM_REF_INCLUDED_

