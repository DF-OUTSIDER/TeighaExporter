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
//  PdfExportImpl.h : definition of the CPdfExportImpl class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PDFEXPORTIMPL_DEFINED
#define PDFEXPORTIMPL_DEFINED

#include "Gs/Gs.h"

#include "PdfExportImplBase.h"
#include "PdfExportParamsHolder.h"

#include "PdfDocument.h"
#include "PdfIContentCommands.h"

#include "PdfFontOptimizer.h"
#include "PdfType3Optimizer.h"
#include "PdfPageDictionary.h"

using namespace TD_PDF;


/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

/** \details
  This class implements the PDF export.
*/
class CPdfExportImpl : public CPdfExportImplBase
{
  // Members
  //
  PDFDocument               m_Document; // PDF document for export process implementation.
  PdfExportParamsHolder     m_ParamsHolder;

  PDFFontOptimizer          m_FontOptimizer; // Font optimization options for a PDF document.
  PDFType3Optimizer         m_Type3Optimizer; // Type optimization options for a PDF document.

  static const double dLogPixelX, dLogPixelY, koeffX, koeffY;

public:
    // Constructor/destructor

    CPdfExportImpl();
    virtual ~CPdfExportImpl();

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
	
/** \details
    Exports PDF object tree to output stream.

    \remarks
    Runs export to PDF format for the current document (stored in the m_Document property). Returns an instance of the PDFResultEx class as the export process result. 
*/
    PDFResultEx exportFile();

};
}
#endif //PDFEXPORTIMPL_DEFINED
