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
// PdfAux.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_AUX_INCLUDED_
#define _PDF_AUX_INCLUDED_

#include "PdfExportDef.h"
#include "PdfResourceDictionary.h"
#include "PdfIndexedRGBColorSpace.h"

#include "2dExportDevice.h"
#include "PdfPageDictionary.h"
#include "Gs/GsPageParams.h"
#include "PdfDocument.h"
#include "PdfImageHelper.h"
#include "PdfFontHelper.h"

using namespace TD_PDF;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

//class PDFTTFontData;
class PDFExportParams;

/*!DOM*/
namespace PDFAUX
{
  void createBasePDF4DWG(PDFDocument &PDFDoc, const OdArray<OdGsPageParams> &pageParams, const PDFExportParams &pParams);
  void createMeasuringViewPort(const OdGsDCRect& bbox, PDFDocument &PDFDoc, PDFPageDictionary* pPage, double iMeasureScale);
  void updateContentStreamWithCM(PDFDocument &PDFDoc, PDFPageDictionary* pPage, const PDFExportParams &pParams);
  void createDocumentInformation(PDFDocument &PDFDoc, const PDFExportParams &pParams);
  PDFResourceDictionaryPtr createResDictionary4DWG(PDFDocument &PDFDoc);
  void createIndexedDWGPalette(PDFResourceDictionaryPtr pDict, const ODCOLORREF *pRGB, OdUInt32 num, PDFDocument &PDFDoc);
  
  void CreateOutputIntent(PDFDocument& PDFDoc);
  PDFIndexedRGBColorSpacePtr CreateIndexedRGBColorSpace(const OdUInt8 *pRGB, OdUInt32 num, PDFDocument &PDFDoc);
  PDFIndexedRGBColorSpacePtr CreateIndexedRGBColorSpace(const ODCOLORREF *pRGB, OdUInt32 num, PDFDocument &PDFDoc);
};
}
#endif // _PDF_AUX_INCLUDED_

