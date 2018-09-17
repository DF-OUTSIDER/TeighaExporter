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
#define _PDF_DICT_KEYS_A_    (KEYNAME, KEYTYPE)
#endif

_PDF_DICT_KEYS_A_    (Name          , PDFName      ); // (Required in PDF 1.0; optional otherwise)
_PDF_DICT_KEYS_A_    (BaseFont      , PDFName      ); // (Required)
_PDF_DICT_KEYS_INT32_(FirstChar                    ); // (Required except for the standard 14 fonts)
_PDF_DICT_KEYS_INT32_(LastChar                     ); // (Required except for the standard 14 fonts)
_PDF_DICT_KEYS_A_    (Widths        , PDFArray     ); // (Required except for the standard 14 fonts; indirect reference preferred)
_PDF_DICT_KEYS_A_    (FontDescriptor, PDFDictionary); //  (Required except for the standard 14 fonts; must be an indirect reference)
_PDF_DICT_KEYS_A_    (Encoding      , PDFObject    ); // (Optional) name or dictionary 
_PDF_DICT_KEYS_A_    (ToUnicode     , PDFStream    ); // (Optional; PDF 1.2)

#undef _PDF_DICT_KEYS_A_
