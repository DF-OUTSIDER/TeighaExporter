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

_PDF_DICT_KEYS_A_(Type          , PDFName           )//(Optional)
_PDF_DICT_KEYS_A_(Subtype       , PDFName           )//(Required)
_PDF_DICT_KEYS_A_(Rect          , PDFRectangle      )//(Required)
_PDF_DICT_KEYS_A_(Contents      , PDFTextString     )//(Optional)
_PDF_DICT_KEYS_A_(P             , PDFDictionary     )//(Optional; PDF 1.3; not used in FDF files)
_PDF_DICT_KEYS_A_(NM            , PDFTextString     )//(Optional; PDF 1.4)
// _PDF_DICT_KEYS_A_(M             , date or string )//(Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(F             , PDFInteger        )//(Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(BS            , PDFDictionary     )//(Optional; PDF 1.2)
_PDF_DICT_KEYS_BF_(AP            , PDFDictionary    )//(Optional; PDF 1.2)
_PDF_DICT_KEYS_A_(AS            , PDFName           )//(Required if the appearance dictionary AP contains one or more subdictionaries;PDF 1.2)
_PDF_DICT_KEYS_A_(Border        , PDFArray          )//(Optional)
_PDF_DICT_KEYS_A_(C             , PDFArray          )//(Optional; PDF 1.1)
_PDF_DICT_KEYS_A_(A             , PDFDictionary     )//(Optional; PDF 1.1)
// _PDF_DICT_KEYS_A_(AA            , dictionary     )//(Optional; PDF 1.2)
// _PDF_DICT_KEYS_A_(StructParent  , integer        )//(Required if the annotation is a structural content item; PDF 1.3)
_PDF_DICT_KEYS_A_(OC            , PDFDictionary     )//(Optional; PDF 1.5)




#undef _PDF_DICT_KEYS_A_
