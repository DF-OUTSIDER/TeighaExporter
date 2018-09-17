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
// PdfPublishModel.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_MODEL_INCLUDED_
#define _PDF_PUBLISH_MODEL_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details 
Namespace contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a CAD model that is the source of the .pdf document created with Teigha Publish.
  CAD model contains drawing data, represented in different format supported by Teihga Publish (for example, .prc data).
*/
class PDFPUBLISH_TOOLKIT OdCADModel : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdCADModel();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdCADModel);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the instance of CAD model.
  */
  virtual ~OdCADModel();

  /** \details
    Sets a new source for the CAD model represented as full path to a .prc file.
    
    \param prc_file_name [in] A string that contains a full path to a .prc file.
  */
  void setSource(const OdString& prc_file_name);

  void setSource(const OdStreamBufPtr& pStreamPrc);

  /** \details
    Returns current full path to the source .prc file for the CAD model.
    This method fills a passed string object with file name and returns it to a calling subroutine.
    
    \param prc_file_name [out] A placeholder for the full path to a .prc file.
  */
  void getSource(OdString& prc_file_name) const;

  void getSourceName(OdString& prc_file_name) const;

  void getSource(OdStreamBufPtr& pStreamPrc) const;

};

/** \details
  A data type that represents a smart pointer to an link<OdPdfPublish::OdCADModel, OdCADModel> object.
*/
SMARTPTR(OdCADModel);

}
#endif // _PDF_PUBLISH_MODEL_INCLUDED_

