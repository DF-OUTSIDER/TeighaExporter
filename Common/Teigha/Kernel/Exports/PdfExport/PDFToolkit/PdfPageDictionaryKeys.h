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

_PDF_DICT_KEYS_A_(Type             , PDFName       )
// moved to header _PDF_DICT_KEYS_B_(Parent           , PDFDictionary ) //(Required; must be an indirect reference)
//_PDF_DICT_KEYS_A_(LastModified     , PDFDate       ) //(Required if PieceInfo is present; optional otherwise; PDF 1.3)
_PDF_DICT_KEYS_A_(Resources        , PDFDictionary ) //(Required; inheritable)
_PDF_DICT_KEYS_A_(MediaBox         , PDFRectangle  ) //(Required; inheritable)
_PDF_DICT_KEYS_A_(CropBox          , PDFRectangle  ) //(Optional; inheritable)
_PDF_DICT_KEYS_A_(BleedBox         , PDFRectangle  ) //(Optional; PDF 1.3)
_PDF_DICT_KEYS_A_(TrimBox          , PDFRectangle  ) //(Optional; PDF 1.3)
_PDF_DICT_KEYS_A_(ArtBox           , PDFRectangle  ) //(Optional; PDF 1.3)
//_PDF_DICT_KEYS_A_(BoxColorInfo     , PDFDictionary ) //(Optional; PDF 1.4)
_PDF_DICT_KEYS_A_(Contents         , PDFArray      ) // or ar  (Optional)
//_PDF_DICT_KEYS_A_(Rotate           , PDFInteger    ) //(Optional; inheritable)
//_PDF_DICT_KEYS_A_(Group            , PDFDictionary ) //(Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(Thumb            , stream        ) //(Optional)
//_PDF_DICT_KEYS_A_(B                , PDFArray      ) //(Optional; PDF 1.1; recommended if the page contains article beads)
//_PDF_DICT_KEYS_A_(Dur              , number        ) //(Optional; PDF 1.1)
//_PDF_DICT_KEYS_A_(Trans            , PDFDictionary ) //(Optional; PDF 1.1)
_PDF_DICT_KEYS_BF_(Annots           , PDFArray     ) //(Optional)
//_PDF_DICT_KEYS_A_(AA               , PDFDictionary ) //(Optional; PDF 1.2)
//_PDF_DICT_KEYS_A_(Metadata         , stream        ) //(Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(PieceInfo        , PDFDictionary ) //(Optional; PDF 1.3)
//_PDF_DICT_KEYS_A_(ID               , PDFBaseString ) //(Optional; PDF 1.3; indirect reference preferred)
//_PDF_DICT_KEYS_A_(PZ               , number        ) //(Optional; PDF 1.3)
//_PDF_DICT_KEYS_A_(Tabs             , PDFName       ) //(Optional; PDF 1.5)
//_PDF_DICT_KEYS_A_(PresSteps        , PDFDictionary ) //(Optional; PDF 1.5)
//_PDF_DICT_KEYS_A_(UserUnit         , number        ) //(Optional; PDF 1.6)
_PDF_DICT_KEYS_A_(VP               , PDFArray      ) //(Optional; PDF 1.6)

#undef _PDF_DICT_KEYS_A_
