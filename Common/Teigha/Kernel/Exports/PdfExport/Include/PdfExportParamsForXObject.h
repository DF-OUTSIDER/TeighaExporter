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
//  PdfExportParamsForXObject.h : definition of the PdfExportParamsForXObject class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PDFEXPORT_PARAMS_FOR_XOBJECT_DEFINED
#define PDFEXPORT_PARAMS_FOR_XOBJECT_DEFINED

#include "PdfFontOptimizer.h"
#include "PdfPageDictionary.h"
#include "PdfXObjectForm.h"
#include "PdfFontOptimizer.h"
#include "PdfExportDef.h"

using namespace TD_PDF;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

// for internal use ONLY
struct PDFEXPORT_DLL PdfExportParamsForXObject
{
  PdfExportParamsForXObject(PDFFontOptimizer &optimizer)
    :m_FontOptimizer(optimizer)
  {

  }
  PDFPageDictionaryPtr      m_pCurrentPage;
  PDFXObjectFormPtr         m_pXobjectForm;
  PDFFontOptimizer          &m_FontOptimizer; // Font optimization options for a PDF document.

};

}
#endif // PDFEXPORT_PARAMS_FOR_XOBJECT_DEFINED
