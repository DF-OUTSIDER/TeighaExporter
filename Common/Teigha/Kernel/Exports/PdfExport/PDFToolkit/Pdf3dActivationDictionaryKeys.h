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

_PDF_DICT_KEYS_A_(A          , PDFName )  // (Optional)
_PDF_DICT_KEYS_A_(AIS        , PDFName )  // (Optional)
_PDF_DICT_KEYS_A_(D          , PDFName )  // (Optional)
_PDF_DICT_KEYS_A_(DIS        , PDFName )  // (Optional)
_PDF_DICT_KEYS_A_(TB         , PDFBoolean)// (Optional; PDF 1.7) A flag indicating the default behavior of an interactive toolbar associated with this annotation. Default value : true.
_PDF_DICT_KEYS_A_(NP         , PDFBoolean)// (Optional; PDF 1.7) A flag indicating the default behavior of the user interface for viewing or managing information about the 3D artwork. Default value: false.

_PDF_DICT_KEYS_A_(Transparent, PDFBoolean)

#undef _PDF_DICT_KEYS_A_
