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

_PDF_DICT_KEYS_A_(Title        , PDFTextString ) // (Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(Author       , PDFTextString ) // (Optional)
_PDF_DICT_KEYS_A_(Subject      , PDFTextString ) // (Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(Keywords     , PDFTextString ) // (Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(Creator      , PDFTextString ) // (Optional)
_PDF_DICT_KEYS_A_(Producer     , PDFTextString ) // (Optional)
_PDF_DICT_KEYS_A_(CreationDate , PDFDate       ) // (Optional)
_PDF_DICT_KEYS_A_(ModDate      , PDFDate       ) // date (Required if PieceInfo is present in the document catalog; otherwise optional; PDF 1.1)
_PDF_DICT_KEYS_A_(Trapped      , PDFName       ) // (Optional; PDF 1.3)

#undef _PDF_DICT_KEYS_A_
