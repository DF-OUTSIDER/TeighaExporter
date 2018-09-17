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

_PDF_DICT_KEYS_A_(3DD , PDFObject     ) // stream or dictionary (Required)
_PDF_DICT_KEYS_A_(3DV , PDFObject     ) // (various) )//(Optional)
_PDF_DICT_KEYS_A_(3DA , PDFDictionary ) //(Optional)
_PDF_DICT_KEYS_A_(3DI , PDFBoolean    ) // (Optional)
_PDF_DICT_KEYS_A_(3DB , PDFRectangle  ) // (Optional)
//The dictionaries below are not described in PDF reference (PDF 32000-1:2008), but they are work
_PDF_DICT_KEYS_A_(RM  , PDFDictionary ) //(Optional) A render mode dictionary. If omitted, the render mode specified in the 3D artwork shall be used.
_PDF_DICT_KEYS_A_(LS  , PDFDictionary ) //(Optional) A lighting scheme dictionary. If omitted, the lighting scheme specified in the 3D artwork shall be used.

#undef _PDF_DICT_KEYS_A_
