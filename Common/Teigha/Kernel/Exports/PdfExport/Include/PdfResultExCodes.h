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




#ifndef PDF_ERROR_MESSAGE_DEFS
  #define PDF_ERROR_MESSAGE_DEFS(a, b)
#endif

  PDF_ERROR_MESSAGE_DEFS(exOk                           , OD_T("No error."));
  PDF_ERROR_MESSAGE_DEFS(exInternalError                , OD_T("Internal error."));
  PDF_ERROR_MESSAGE_DEFS(exHugeShxFont                  , OD_T("Cannot convert SHX font to PDF font format."));
  PDF_ERROR_MESSAGE_DEFS(exInvalidPageParams            , OD_T("Paper size or margins are invalid."));
  PDF_ERROR_MESSAGE_DEFS(exNullOutputStream             , OD_T("Output stream is uninit."));
  PDF_ERROR_MESSAGE_DEFS(exLayoutNotFound               , OD_T("Database hasn't specified layout name."));
  PDF_ERROR_MESSAGE_DEFS(exNullDatabase                 , OD_T("Input parameter database is null."));
  PDF_ERROR_MESSAGE_DEFS(exUnsupportedVersion           , OD_T("Pdf version for output file is unsupported."));
  PDF_ERROR_MESSAGE_DEFS(exWrongNumberOfPages           , OD_T("Number of Layouts are not equal to number of pages."));
  PDF_ERROR_MESSAGE_DEFS(exCannotRestorePaperFromLayout , OD_T("Cannot restore page parameters from layout."));
  PDF_ERROR_MESSAGE_DEFS(exCannotFillFontDescriptor     , OD_T("Cannot restore font descriptor from font."));
  PDF_ERROR_MESSAGE_DEFS(exCannotOpenOverallVport       , OD_T("Layout overall viewport is invalid."));
  PDF_ERROR_MESSAGE_DEFS(exBadViewExtents               , OD_T("Cannot calculate view extents."));
  PDF_ERROR_MESSAGE_DEFS(exLayersRequired_v15           , OD_T("Layer support required v1.5."));
  PDF_ERROR_MESSAGE_DEFS(exInvalidImageDPI              , OD_T("Invalid resolution value (DPI)."));
  PDF_ERROR_MESSAGE_DEFS(exCannotStartPrcEngine         , OD_T("Missing PdfExportService service."));
  PDF_ERROR_MESSAGE_DEFS(exUseHLRConflictedWithEnablePRC, OD_T("EnablePRC cannot be used with UseHLR flag."));
  PDF_ERROR_MESSAGE_DEFS(exPrcRequired_v16              , OD_T("Version 1.6 or higher required for 3d entities processing."));
  PDF_ERROR_MESSAGE_DEFS(exPdfExportServiceMissed       , OD_T("PdfExportService missed."));

#undef PDF_ERROR_MESSAGE_DEFS
