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

#ifndef _PDF_DICT_KEYS_BOOL_
#define _PDF_DICT_KEYS_BOOL_(KEYNAME)
#endif

#ifndef _PDF_DICT_KEYS_INT32_
#define _PDF_DICT_KEYS_INT32_(KEYNAME)
#endif

_PDF_DICT_KEYS_INT32_(K);                //(Optional) CCITTFaxDecode filter param - coding type
_PDF_DICT_KEYS_INT32_(Columns);          //(Optional) CCITTFaxDecode filter param - picture width
_PDF_DICT_KEYS_INT32_(Rows);             //(Optional) CCITTFaxDecode filter param - picture height
_PDF_DICT_KEYS_BOOL_ ( BlackIs1 );       //(Optional) CCITTFaxDecode filter param - if true, black is 1, else black is 0

_PDF_DICT_KEYS_INT32_(TmpHeight);        //Image Height - necessary for DCTDecode filter and should be erased after using
_PDF_DICT_KEYS_INT32_(TmpWidth);         //Image Width - necessary for DCTDecode filter and should be erased after using
_PDF_DICT_KEYS_INT32_(TmpQuality);       //(Optional) Image quality for DCTDecode filter, should be erased after using
_PDF_DICT_KEYS_BOOL_ (TmpAlreadyEncoded);//(Optional) Used to prevent unnecessary encoding, should be erased after using

#undef _PDF_DICT_KEYS_BOOL_
#undef _PDF_DICT_KEYS_INT32_
