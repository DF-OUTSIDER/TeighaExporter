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

_PDF_DICT_KEYS_A_(Type               , PDFName)//(Optional; if present, shall be 3DCrossSection)
_PDF_DICT_KEYS_A_(C                  , PDFArray)//(Optional; A three element array specifying the center of rotation on the cutting plane in world space coordinates)
_PDF_DICT_KEYS_A_(O                  , PDFArray)//(Required; A three-element array specifying the orientation of the cutting plane in world space)
_PDF_DICT_KEYS_A_(PO                 , PDFNumber)//(Optional; opacity of the cutting plane [0, 1]
_PDF_DICT_KEYS_A_(PC                 , PDFArray)//(Optional; Colour of the cutting plane; Default value: [/DeviceRGB 1 1 1] representing the colour white)
_PDF_DICT_KEYS_A_(IV                 , PDFBoolean)//(Optional; A flag indicating the visibility of the intersection of the cutting plane with any 3D geometry)
_PDF_DICT_KEYS_A_(IC                 , PDFArray)//(Optional; Colour for the cutting plane’s intersection with the 3D artwork; Default value: [/DeviceRGB 0 1 0] representing the colour green)

#undef _PDF_DICT_KEYS_A_
