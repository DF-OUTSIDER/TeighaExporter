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




#ifndef _PDFTYPE1FONT_INCLUDED_
#define _PDFTYPE1FONT_INCLUDED_ /*!DOM*// 

#include "PdfFont.h"
#include "PdfStream.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFType1Font : public PDFFont
{
  PDF_DECLARE_OBJECT(PDFType1Font, PDFFont, kType1Font)

protected:
  virtual void InitObject();
public:
  enum StandardType1FontsEnum
  {
    kTimesRoman,
    kHelvetica,
    kCourier,
    kSymbol,
    kTimesBold,
    kHelveticaBold,
    kCourierBold,
    kZapfDingbats,
    kTimesItalic,
    kHelveticaOblique,
    kCourierOblique,
    kTimesBoldItalic,
    kHelveticaBoldOblique,
    kCourierBoldOblique
  };

  void setStandardType1Fonts(StandardType1FontsEnum font_type);

  static OdAnsiString getStandardType1FontsName(const StandardType1FontsEnum font_type);
  static double getTextCapHeight(const StandardType1FontsEnum font_type);
  static double getTextAscender(const StandardType1FontsEnum font_type);
  static double getTextDescender(const StandardType1FontsEnum font_type);
  static ODRECT getTextBBox(const StandardType1FontsEnum font_type);
  static double getTextBaseWidth(const StandardType1FontsEnum font_type, const OdString& text);
  static double getTextBaseWidth(const StandardType1FontsEnum font_type, const OdAnsiString& text);

#include "PdfDictKeysDefs.h"
#include "PdfType1FontKeys.h"
#include "PdfDictKeysUnDefs.h"

};

typedef PDFSmartPtr<PDFType1Font> PDFType1FontPtr;

};

#endif //_PDFTYPE1FONT_INCLUDED_

