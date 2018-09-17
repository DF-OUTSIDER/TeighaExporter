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


// Entries in the catalog, PDFDictionary)

#ifndef _PDF_DICT_KEYS_A_
#define _PDF_DICT_KEYS_A_(KEYNAME, KEYTYPE)
#endif

_PDF_DICT_KEYS_A_(Type        , PDFName       ) //(Required)
_PDF_DICT_KEYS_A_(FontName    , PDFName       ) //(Required)
_PDF_DICT_KEYS_A_(FontFamily  , PDFBaseString ) //(Optional; PDF 1.5; strongly recommended for Type 3 fonts in Tagged PDF documents)
_PDF_DICT_KEYS_A_(FontStretch , PDFName       ) //(Optional; PDF 1.5; strongly recommended for Type 3 fonts in Tagged PDF documents)
_PDF_DICT_KEYS_A_(FontWeight  , PDFInteger    ) //(Optional; recommended for Type 3 fonts in Tagged PDF documents) - PDFNumber, but the possible values shall be 100, 200, 300, 400, 500, 600, 700, 800, or 900
_PDF_DICT_KEYS_A_(Flags       , PDFInteger    ) //(Required)
_PDF_DICT_KEYS_A_(FontBBox    , PDFRectangle  ) //(Required, except for Type 3 fonts)
_PDF_DICT_KEYS_A_(ItalicAngle , PDFNumber     ) //(Required)
_PDF_DICT_KEYS_A_(Ascent      , PDFNumber     ) //(Required, except for Type 3 fonts)
_PDF_DICT_KEYS_A_(Descent     , PDFNumber     ) //(Required, except for Type 3 fonts)
_PDF_DICT_KEYS_A_(Leading     , PDFNumber     ) //(Optional)
_PDF_DICT_KEYS_A_(CapHeight   , PDFNumber     ) //(Required for fonts that have Latin characters, except for Type 3 fonts)
_PDF_DICT_KEYS_A_(XHeight     , PDFNumber     ) //(Optional)
_PDF_DICT_KEYS_A_(StemV       , PDFNumber     ) //(Required, except for Type 3 fonts)
_PDF_DICT_KEYS_A_(StemH       , PDFNumber     ) //(Optional)
_PDF_DICT_KEYS_A_(AvgWidth    , PDFNumber     ) //(Optional)
_PDF_DICT_KEYS_A_(MaxWidth    , PDFNumber     ) //(Optional)
_PDF_DICT_KEYS_A_(MissingWidth, PDFNumber     ) //(Optional)
_PDF_DICT_KEYS_A_(FontFile    , PDFStream     ) //(Optional)
_PDF_DICT_KEYS_A_(FontFile2   , PDFStream     ) //(Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(FontFile3   , PDFStream     ) //(Optional; PDF 1.2)
_PDF_DICT_KEYS_A_(CharSet     , PDFBaseString ) //(Optional; meaningful only in Type 1 fonts; PDF 1.1)

#undef _PDF_DICT_KEYS_A_
