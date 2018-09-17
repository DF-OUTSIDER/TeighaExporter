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




#ifndef _PDFRESULTEX_INCLUDED_
#define _PDFRESULTEX_INCLUDED_ /*!DOM*/

/** \details
  PDF export result enumeration.

  <group OdExport_Classes> 
*/

enum PDFResultEx
{
  exOk                           = 0x00000000, // Export finished successfully.
  exInternalError                = 0x00010000,  // Export failed: internal exception occured.
  exHugeShxFont                  , // Export failed: huge value for Shx Font size.
  exInvalidPageParams            , // Export failed: invalid PDF document page parameters provided.
  exNullOutputStream             , // Export failed: output stream for writing PDF document is NULL.
  exLayoutNotFound               , // Export failed: layout with the specified name can't be found.
  exNullDatabase                 , // Export failed: invalid database pointer.
  exUnsupportedVersion           , // Export failed: specified version of PDF document is not supported by Teigha.
  exWrongNumberOfPages           , // Export failed: invalid PDF document page counter value.
  exCannotRestorePaperFromLayout , // Export failed: error occurred during getting paper parameters from the specified layout.
  exCannotFillFontDescriptor     , // Export failed: font descriptor for the PDF document can't be filled.
  exCannotOpenOverallVport       , // Export failed: view port can't be opened.
  exBadViewExtents               , // Export failed: invalid view extents.
  exLayersRequired_v15           , // Export failed: specified layout requires version 1.5 or higher for correct processing.
  exInvalidImageDPI              , // Export failed: invalid resolution value (DPI).
  exCannotStartPrcEngine         , // Export failed: missing PdfExportService service.
  exUseHLRConflictedWithEnablePRC, // Export failed: EnablePRC cannot be used with UseHLR flag.
  exPrcRequired_v16              , // Export failed: version 1.6 or higher required for 3d entities processing.
  exPdfExportServiceMissed       , // Export failed: Missed PrcExport service.
  exOdError                      = 0x00020000, // Export failed: ODA base class error occured.

  exLastErrorNum                 = 0xFFFFFFFF // Number of the last occured error.
};

#endif //_PDFRESULTEX_INCLUDED_

