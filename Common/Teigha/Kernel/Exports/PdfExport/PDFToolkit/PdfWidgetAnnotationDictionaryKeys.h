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

_PDF_DICT_KEYS_A_(H, PDFName)                     //(Optional) The annotation’s highlighting mode, the visual effect that shall be used when the mouse button 
                                                  //is pressed or held down inside its active area. Default value: I.
_PDF_DICT_KEYS_A_(MK, PDFDictionary)              //(Optional) An appearance characteristics dictionary 
//_PDF_DICT_KEYS_A_(A, PDFDictionary)             //(Optional; PDF 1.1) An action that shall be performed when the annotation is activated
_PDF_DICT_KEYS_A_(AA, PDFTriggerEventsDictionary) //(Optional; PDF 1.2) An additional-actions dictionary defining the annotation’s behaviour in response to 
                                                  //various trigger events
_PDF_DICT_KEYS_A_(BS, PDFDictionary)              //(Optional; PDF 1.2) A border style dictionary
_PDF_DICT_KEYS_A_(Parent, PDFDictionary)          //(Required if this widget annotation is one of multiple children in a field; absent otherwise)

#undef _PDF_DICT_KEYS_A_
