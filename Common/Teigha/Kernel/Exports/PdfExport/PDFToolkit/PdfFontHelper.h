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
// PdfPublishImageFunction.h - This temporary solution. Must move to PDFToolkit in next refactoring
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_FONT_HELPER_INCLUDED_
#define _PDF_FONT_HELPER_INCLUDED_

#include "PdfCommon.h"

#include "PdfResourceDictionary.h"
#include "PdfType1Font.h"
#include "PdfType3Font.h"
#include "PdfTrueTypeFont.h"
#include "PdfFontDescriptor.h"
#include "PdfFontFileStream.h"
#include "PdfTTFontData.h"

#include "Gi/GiTextStyle.h"

namespace TD_PDF
{

class PDFTTFontData;

  namespace TD_PDF_HELPER_FUNCS
  {
    void fixPDFName(OdAnsiString& name);
    OdAnsiString generatePDFSHXName(const OdGiTextStyle& pTextStyle, OdUInt32 fontIndx, double lineWeight);
    OdAnsiString generatePDFTTFName(const PDFTTFontData &fd, const OdTtfDescriptor& ttfDesc, bool bSubset);
    OdAnsiString PDFEXPORT_TOOLKIT addTrueTypeFont(PDFDocument &PDFDoc, const OdGiTextStyle& pTextStyle, bool bEmbededFont, bool bOptimizedEmbeddedFont, PDFFontPtr *pOutFont, PDFResourceDictionaryPtr pResDict, bool& bAllowedEdit);
    OdAnsiString PDFEXPORT_TOOLKIT addType0Font(PDFDocument &PDFDoc, const OdGiTextStyle& pTextStyle, bool bEmbededFont, bool bOptimizedEmbeddedFont, PDFFontPtr *pOutFon, PDFResourceDictionaryPtr pResDict, bool& bAllowedEdit);
    OdAnsiString PDFEXPORT_TOOLKIT addType1Font(PDFDocument &PDFDoc, const PDFType1Font::StandardType1FontsEnum font_type, PDFFontPtr* pOutFont, PDFResourceDictionaryPtr pResDict);
    OdAnsiString PDFEXPORT_TOOLKIT addType3Font(PDFDocument &PDFDoc, const OdGiTextStyle& pTextStyle, PDFFontPtr *pOutFont, OdUInt32 fontIndx, double lineWeight, OdGeVector2d u2d, OdGeVector2d v2d, PDFResourceDictionaryPtr pResDict);

    const PDFType3FontPtr PDFEXPORT_TOOLKIT getType3FontByName(PDFDocument &PDFDoc, const OdAnsiString &fontName);

    void fillFontFileStream(PDFFontFileStreamPtr &pTTFStream, const OdFont *pOdTTF, OdString path);
    OdResult FillFontDescriptor(PDFDocument &PDFDoc, PDFTTFontData &fd, PDFTrueTypeFontPtr pNewTTF, PDFFontDescriptorPtr pFontDesc);

    // Calculate ttf font size, corresponding to this textstyle.
    // This formula contradicts documentation, but is verified by practics.
    double PDFEXPORT_TOOLKIT textHeight(const OdGiTextStyle& textStyle);

    double PDFEXPORT_TOOLKIT textWidth(const OdString& text, const OdGiTextStyle* pTextStyle);
    double PDFEXPORT_TOOLKIT textAsc(const OdGiTextStyle* pTextStyle);
    double PDFEXPORT_TOOLKIT textDesc(const OdGiTextStyle* pTextStyle);
    ODRECT PDFEXPORT_TOOLKIT textBBox(const OdGiTextStyle* pTextStyle);
    OdAnsiString generatePDFTTFStyleName(const OdGiTextStyle* pTextStyle);
  }
}
#endif // _PDF_FONT_HELPER_INCLUDED_

