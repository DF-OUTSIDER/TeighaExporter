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

_PDF_DICT_KEYS_A_(Dests                   , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.2) A name tree mapping name strings to destinations
_PDF_DICT_KEYS_A_(AP                      , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.3) A name tree mapping name strings to annotation appearance streams
_PDF_DICT_KEYS_A_(JavaScript             , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.3) A name tree mapping name strings to document-level JavaScript actions
_PDF_DICT_KEYS_A_(Pages                   , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.3) A name tree mapping name strings to visible pages for use in interactive forms
_PDF_DICT_KEYS_A_(Templates               , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.3) A name tree mapping name strings to invisible (template) pages for use in interactive forms
_PDF_DICT_KEYS_A_(IDS                     , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.3) A name tree mapping digital identifiers to Web Capture content sets
_PDF_DICT_KEYS_A_(URLS                    , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.3) A name tree mapping uniform resource locators (URLs) to Web Capture content sets
_PDF_DICT_KEYS_A_(EmbeddedFiles           , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.4) A name tree mapping name strings to file specifications for embedded file streams
_PDF_DICT_KEYS_A_(AlternatePresentations  , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.4) A name tree mapping name strings to alternate presentations
_PDF_DICT_KEYS_A_(Renditions              , PDFNameTreeNodeDictionary ) // (Optional; PDF 1.5) A name tree mapping name strings (which shall have Unicode encoding) to rendition objects


#undef _PDF_DICT_KEYS_A_
