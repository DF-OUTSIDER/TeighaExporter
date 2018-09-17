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

//_PDF_DICT_KEYS_A_(Type    , PDFName)//(Optional) if present, shall be 3DRenderMode.
//_PDF_DICT_KEYS_A_(Subtype , PDFName)//(Required) name of the mode.
_PDF_DICT_KEYS_A_(AC      , PDFArray)//(Optional) An array that specifies the auxiliary colour that shall be used when rendering the 3D image. Default value : [/ DeviceRGB 0 0 0] representing the black.
_PDF_DICT_KEYS_A_(FC      , PDFArray)//(Optional) A name or array that specifies the face color to be used when rendering the 3D image. Default value : BG
_PDF_DICT_KEYS_A_(O       , PDFNumber)//(Optional) A number specifying the opacity of the added transparency applied by some render modes, using a standard additive blend. Default value : 0.5
_PDF_DICT_KEYS_A_(CV      , PDFNumber)//(Optional) A number specifying the angle, in degrees, that shall be used as the crease value when determining silhouette edges. Default value : 45

#undef _PDF_DICT_KEYS_A_
