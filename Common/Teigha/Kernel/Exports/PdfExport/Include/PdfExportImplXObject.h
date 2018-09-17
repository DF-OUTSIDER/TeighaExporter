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
//  PdfExportImplPublish.h : definition of the CPdfExportImplXObject class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PDFEXPORTIMPL_XOBJECT_DEFINED
#define PDFEXPORTIMPL_XOBJECT_DEFINED

#include "PdfExportImplBase.h"

#include "PdfPageDictionary.h"
#include "PdfXObjectForm.h"

using namespace TD_PDF;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {


/** \details
  This class implements the PDF export.
*/
class CPdfExportImplXObject : public CPdfExportImplBase
{
  PdfExportParamsHolder     m_ParamsHolder;
  PDFPageDictionaryPtr      m_pCurrentPage;
  PDFXObjectFormPtr         m_pXobjectForm;
public:
  // Constructor/destructor

  CPdfExportImplXObject(PDFPageDictionaryPtr& page, PDFXObjectFormPtr& xobject_form, PDFFontOptimizer& font_optimizer);
  virtual ~CPdfExportImplXObject();

  // Implementation

  /** \details
  Initiates PDF export with specified parameters.

  \remarks
  Initiates PDF export with specified parameters (passed in the pParams parameter). Returns an instance of the PDFResultEx class as the result of the initiantion.

  \param pParams [in]  Reference to the PDFExportParams class instance containing export parameter values.
  */
  PDFResultEx init(const PDFExportParams &pParams);

  /** \details
  Runs export to PDF format for the current document.

  \remarks
  Initiates PDF export with specified parameters (passed in the pParams parameter). Returns an instance of the PDFResultEx class as the result of the initiantion.
  */
  PDFResultEx run();

};
}
#endif //PDFEXPORTIMPL_XOBJECT_DEFINED
