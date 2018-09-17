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

_PDF_DICT_KEYS_A_(T, PDFTextString)//Optional - The text label to be displayed in the title bar of the annotation’s pop-up window when open and active
//_PDF_DICT_KEYS_A_(Popup, PDFDictionary)//Optional - An indirect reference to a pop-up annotation for entering or editing the text associated with this annotation.
//_PDF_DICT_KEYS_A_(CA            , PDFNumber     )//Optional - The constant opacity value to be used in painting the annotation
//_PDF_DICT_KEYS_A_(RC, text string or text stream)//Optional PDF 1.5 - A rich text string to be displayed in the pop-up window when the annotation is opened.
//_PDF_DICT_KEYS_A_(CreationDate, PDFDate)//Optional PDF 1.5 - The date and time when the annotation was created.
//_PDF_DICT_KEYS_A_(IRT, PDFDictionary)//(Required if an RT entry is present, otherwise optional; PDF 1.5)
//_PDF_DICT_KEYS_A_(Subj, PDFTextString)//Optional  PDF 1.5 - Text representing a short description of the subject being addressed by the annotation.
//_PDF_DICT_KEYS_A_(RT, PDFName)//(Optional; meaningful only if IRT is present; PDF 1.6)
//_PDF_DICT_KEYS_A_(IT, PDFName)//(Optional; PDF 1.6)


#undef _PDF_DICT_KEYS_A_
