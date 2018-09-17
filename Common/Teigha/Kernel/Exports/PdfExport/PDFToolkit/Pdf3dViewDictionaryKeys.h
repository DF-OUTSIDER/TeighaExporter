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

// #ifdef IN
// #undef IN
// #endif

_PDF_DICT_KEYS_A_(Type       , PDFName       )//(Optional)
_PDF_DICT_KEYS_A_(XN         , PDFTextString )//(Required)
//_PDF_DICT_KEYS_A_(IN         , PDFTextString )//(Optional)
_PDF_DICT_KEYS_A_(MS         , PDFName       )//(Optional)
_PDF_DICT_KEYS_A_(C2W        , PDFArray      )//(Required if the value of MS is M, ignored otherwise)
// _PDF_DICT_KEYS_A_(U3DPath    , PDFObject      )//string or array )//(Required if the value of MS is U3D, ignored otherwise)
_PDF_DICT_KEYS_A_(CO         , PDFNumber     )//(Optional; used only if MS is present)
_PDF_DICT_KEYS_A_(P          , PDFDictionary )//(Optional)
_PDF_DICT_KEYS_A_(O          , PDFStream     )//(Optional; meaningful only if MS and P are present)
_PDF_DICT_KEYS_A_(BG         , PDFDictionary )//(Optional)
_PDF_DICT_KEYS_A_(SA         , PDFArray      )//(Optional; cross sections data for 3d view)
_PDF_DICT_KEYS_A_(NR         , PDFBoolean    )//(Optional; reset nodes visibility when views are changed)
_PDF_DICT_KEYS_A_(NA         , PDFArray      )//(Optional; node visibility data for 3d view)
_PDF_DICT_KEYS_A_(RM         , PDFDictionary )//(Optional) A render mode dictionary. If omitted, the render mode specified in the 3D artwork shall be used.
_PDF_DICT_KEYS_A_(LS         , PDFDictionary )//(Optional) A lighting scheme dictionary. If omitted, the lighting scheme specified in the 3D artwork shall be used.

#undef _PDF_DICT_KEYS_A_
