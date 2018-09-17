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
// PdfPublishAnimation.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_ANIMATION_INCLUDED_
#define _PDF_PUBLISH_ANIMATION_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublishBaseCameraMotion.h"
#include "PdfPublishBaseNodeMotion.h"
#include "UInt64Array.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements an animation in .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdAnimation : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdAnimation();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdAnimation);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys an object.
  */
  virtual ~OdAnimation();

  void setName(const OdString& name);
  void setPlayOnOpenDocument(bool state);
  void setAutoReplay(bool state);
  void setFramesPerSecond(const OdUInt32 frame_per_second);
  void setFramesCount(const OdUInt64 frame_count);


  void setCameraMotion(const OdUInt64& start_frame, const OdUInt64& end_frame, OdBaseCameraMotionPtr motion);
  void addNodeMotion(const OdUInt64& start_frame, const OdUInt64& end_frame, OdBaseNodeMotionPtr motion);

  void getName(OdString& name) const;
  void getPlayOnOpenDocument(bool& state) const;
  void getAutoReplay(bool& state) const;
  void getFramesPerSecond(OdUInt32& frame_per_second) const;
  void getFramesCount(OdUInt64& frame_count) const;

  void getCameraMotion(OdUInt64& start_frame, OdUInt64& end_frame, OdBaseCameraMotionPtr& motion) const;
  void getNodeMotions(OdUInt64Array& starts_frame, OdUInt64Array& ends_frame, OdBaseNodeMotionPtrArray& motions) const;


  
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdAnimation, OdAnimation> object.
*/
SMARTPTR(OdAnimation);

}
#endif // _PDF_PUBLISH_ANIMATION_INCLUDED_

