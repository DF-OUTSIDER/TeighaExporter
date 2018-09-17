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

_PDF_DICT_KEYS_A_(Type, PDFName) //(Optional) The type of PDF object that this dictionary describes; if present, must be OutputIntent for an output intent dictionary.
_PDF_DICT_KEYS_A_(S, PDFName) //(Required) The output intent subtype; must be GTS_PDFX for a PDF / X output intent.
_PDF_DICT_KEYS_A_(OutputCondition, PDFBaseString) //(Optional) A text string concisely identifying the intended output device or production condition in human - readable form.
_PDF_DICT_KEYS_A_(OutputConditionIdentifier, PDFBaseString) //(Required) A string identifying the intended output device or production condition in human - or machine - readable form.
_PDF_DICT_KEYS_A_(RegistryName, PDFBaseString) //(Optional) A string(conventionally a uniform resource identifier, or URI) identifying the registry in which the condition designated by OutputConditionIdentifier is defined.
_PDF_DICT_KEYS_A_(Info, PDFBaseString) //(Required if OutputConditionIdentifier does not specify a standard production condition; optional otherwise)
_PDF_DICT_KEYS_A_(DestOutputProfile, PDFStream) //(Required if OutputConditionIdentifier does not specify a standard production condition; optional otherwise)

#undef _PDF_DICT_KEYS_A_
