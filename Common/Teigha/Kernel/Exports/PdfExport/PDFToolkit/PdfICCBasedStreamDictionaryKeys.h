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

_PDF_DICT_KEYS_A_(N,         PDFInteger)//(Required) The number of color components in the color space described by the ICC profile data. As of PDF 1.4, N must be 1, 3, or 4.
_PDF_DICT_KEYS_A_(Alternate, PDFName)//array or name. (Optional) An alternate color space to be used in case the one specified in the stream data is not supported
_PDF_DICT_KEYS_A_(Range,     PDFArray)//(Optional) An array of 2 x N numbers [min0 max0 min1 max1] specifying the minimum and maximum valid values of the corresponding color components
_PDF_DICT_KEYS_A_(Metadata,  PDFStream )//(Optional)A metadata stream containing metadata for the color space

#undef _PDF_DICT_KEYS_A_
