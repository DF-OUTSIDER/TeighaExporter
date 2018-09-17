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
// PdfExport.h - Open Design Pdf Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_EXPORT_INCLUDED_
#define _PDF_EXPORT_INCLUDED_

#include "RxModule.h"

#include "PdfExportDef.h"
#include "PdfExportParams.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT{

  struct PdfExportParamsForXObject;
/** \details
  This class implements the PDF export.
*/
class OdPdfExport : public OdRxObject
{
public:

/** \details
  Pure virtual method for PDF export implementation.
  
  \remarks
  Using an instance of PDFExportParams class as a set of export parameters.
  
  \param pParams [in]  Parameters used by the PDF export procedure.
*/
  virtual OdUInt32 exportPdf(PDFExportParams &pParams) = 0;
  
  /** \details
  Pure virtual method returning PDF export error description by error code.
  
  \remarks
  Returns string with error description according the passed error code. 
  
  \param errorCode [in]  Error code, that can be received from the /exportPdf()/ method.
  */
  virtual OdString exportPdfErrorCode(OdUInt32 errorCode) = 0;

  /** \details
  Pure virtual method for Publish implementation.

  \remarks
  Using an instance of PDFExportParams class as a set of publish parameters.

  \param pParams [in]  Parameters used by the publish procedure.
  \param outStreamsPRC [out]  Receives the PRCStreamsMap parameter. PRCStreamsMap includes the index of the viewport and the prc stream from this viewport.
  */
  virtual OdUInt32 publish3d(PDFExportParams &pParams, PRCStreamsMap &outStreamsPRC) = 0;

  // for internal use ONLY
  virtual OdUInt32 export2XObject(PDFExportParams &pParams, PdfExportParamsForXObject* pExportParams) = 0;
};

/** \details
  Implements the smart pointer to the PDF export.
*/
typedef OdSmartPtr<OdPdfExport> OdPdfExportPtr;

/** \details
  This class implements the PDF export module.
*/
class PdfExportModule : public OdRxModule
{
public:

/** \details
  Pure virtual method for implementing creation of the OdPdfExport class instance. 
  
  \remarks
  Returns a smart pointer to the created module.
*/
  virtual OdPdfExportPtr create() = 0;
};

/** \details
 Implements the smart pointer to the PDF export module.
*/
typedef OdSmartPtr<PdfExportModule> OdPdfExportModulePtr;
}
#endif // _PDF_EXPORT_INCLUDED_

