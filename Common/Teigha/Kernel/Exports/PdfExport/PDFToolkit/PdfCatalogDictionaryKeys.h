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

_PDF_DICT_KEYS_A_(Type               , PDFName       ) //(Required)
//_PDF_DICT_KEYS_A_(Version          , PDFName       ) //(Optional; PDF 1.4)
_PDF_DICT_KEYS_A_(Pages              , PDFDictionary ) //(Required; must be an indirect reference)
//_PDF_DICT_KEYS_A_(PageLabels       , PDFNumberTree) //(Optional; PDF 1.3)
_PDF_DICT_KEYS_A_(Names             , PDFNamesDictionary ) //(Optional; PDF 1.2)
//_PDF_DICT_KEYS_A_(Dests            , PDFDictionary ) //(Optional; PDF 1.1; must be an indirect reference)
//_PDF_DICT_KEYS_A_(ViewerPreferences, PDFDictionary ) //(Optional; PDF 1.2)
//_PDF_DICT_KEYS_A_(PageLayout       , PDFName       ) //(Optional)
//_PDF_DICT_KEYS_A_(PageMode         , PDFName       ) //(Optional)
//_PDF_DICT_KEYS_A_(Outlines         , PDFDictionary ) //(Optional; must be an indirect reference)
//_PDF_DICT_KEYS_A_(Threads          , PDFArray      ) //(Optional; PDF 1.1; must be an indirect reference)
//_PDF_DICT_KEYS_A_(OpenAction       , ar||dict   ) //(Optional; PDF 1.1)
//_PDF_DICT_KEYS_A_(AA               , PDFDictionary ) //(Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(URI              , PDFDictionary ) //(Optional; PDF 1.1)
_PDF_DICT_KEYS_BF_(AcroForm          , PDFDictionary ) //(Optional; PDF 1.2)
_PDF_DICT_KEYS_A_(Metadata           , PDFStream) //(Optional; PDF 1.4; must be an indirect reference) A metadata stream containing metadata for the document
//_PDF_DICT_KEYS_A_(StructTreeRoot   , PDFDictionary ) //(Optional; PDF 1.3)
//_PDF_DICT_KEYS_A_(MarkInfo         , PDFDictionary ) //(Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(Lang             , PDFBaseString) //(Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(SpiderInfo       , PDFDictionary ) //(Optional; PDF 1.3)
_PDF_DICT_KEYS_A_(OutputIntents      , PDFArray) //(Optional; PDF 1.4) An array of output intent dictionaries describing the color characteristics of output devices on which the document might be rendered
//_PDF_DICT_KEYS_A_(PieceInfo        , PDFDictionary ) //(Optional; PDF 1.4)
_PDF_DICT_KEYS_A_(OCProperties       , PDFDictionary ) //(Optional; PDF 1.5; required if a document contains optional content)
//_PDF_DICT_KEYS_A_(Perms            , PDFDictionary ) //(Optional; PDF 1.5)
//_PDF_DICT_KEYS_A_(Legal            , PDFDictionary ) //(Optional; PDF 1.5)

#undef _PDF_DICT_KEYS_A_
