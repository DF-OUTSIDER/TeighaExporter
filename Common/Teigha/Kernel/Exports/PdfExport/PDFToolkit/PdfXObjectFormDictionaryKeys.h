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

#ifndef _PDF_DICT_KEYS_BOOL_
#define _PDF_DICT_KEYS_BOOL_(KEYNAME)
#endif

#ifndef _PDF_DICT_KEYS_INT32_
#define _PDF_DICT_KEYS_INT32_(KEYNAME)
#endif


_PDF_DICT_KEYS_A_(Subtype, PDFName);         //(Required)
_PDF_DICT_KEYS_INT32_(FormType);             //(Optional)A code identifying the type of form XObject that this dictionary describes.The only valid value defined at the time of publication is 1. Default value : 1.
_PDF_DICT_KEYS_A_(BBox, PDFRectangle);       //(Required) An array of four numbers in the form coordinate system, giving the coordinates of the left, bottom, right, and top edges, respectively, of the form XObject’s bounding box.These boundaries are used to clip the form XObject and to determine its size for caching.
_PDF_DICT_KEYS_A_(Matrix, PDFArray);         //(Optional) An array of six numbers specifying the form matrix, which maps form space into user space(see Section 4.2.3, “Transformation Matrices”). Default value : the identity matrix[1 0 0 1 0 0].
_PDF_DICT_KEYS_BF_(Resources, PDFDictionary); //(Optional but strongly recommended; PDF 1.2) A dictionary specifying any resources(such as fonts and images) required by the form XObject(see Section 3.7, “Content Streams and Resources”).
_PDF_DICT_KEYS_A_(Name, PDFName);            // (Required in PDF 1.0; optional otherwise)
#undef _PDF_DICT_KEYS_A_
#undef _PDF_DICT_KEYS_BOOL_
#undef _PDF_DICT_KEYS_INT32_
#undef _PDF_DICT_KEYS_BF_
