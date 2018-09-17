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


_PDF_DICT_KEYS_A_(Subtype          , PDFName       ); //(Required)
_PDF_DICT_KEYS_INT32_(Width); // (Required)
_PDF_DICT_KEYS_INT32_(Height); // (Required)
_PDF_DICT_KEYS_A_(ColorSpace       , PDFObject     ); // PDFName or PDFArray (Required for images, except those that use the JPXDecode filter; not allowed for image masks)
_PDF_DICT_KEYS_INT32_(BitsPerComponent); // (Required except for image masks and images that use the JPXDecode filter)
//_PDF_DICT_KEYS_A_(Intent           , PDFName       ); // (Optional; PDF 1.1)
_PDF_DICT_KEYS_BOOL_(ImageMask); // (Optional)
_PDF_DICT_KEYS_A_(Mask             , PDFObject     ); // PDFStream or PDFArray (Optional except for image masks; not allowed for image masks; PDF 1.3)
_PDF_DICT_KEYS_A_(Decode           , PDFArray      ); // (Optional)
//_PDF_DICT_KEYS_A_(Interpolate      , PDFBoolean    ); // (Optional)
//_PDF_DICT_KEYS_A_(Alternates       , PDFArray      ); // (Optional; PDF 1.3)
//_PDF_DICT_KEYS_A_(SMask            , PDFStream     ); // (Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(SMaskInData      , PDFInteger    ); // (Optional for images that use the JPXDecode filter, meaningless otherwise; PDF 1.5)
_PDF_DICT_KEYS_A_(Name             , PDFName       ); // (Required in PDF 1.0; optional otherwise)
//_PDF_DICT_KEYS_A_(StructParent     , PDFInteger    ); // (Required if the image is a structural content item; PDF 1.3)
//_PDF_DICT_KEYS_A_(ID               , PDFString     ); // (Optional; PDF 1.3; indirect reference preferred)
//_PDF_DICT_KEYS_A_(OPI              , PDFDictionary ); // (Optional; PDF 1.2)
//_PDF_DICT_KEYS_A_(Metadata         , PDFStream     ); // (Optional; PDF 1.4)
//_PDF_DICT_KEYS_A_(OC               , PDFDictionary ); // (Optional; PDF 1.5)

#undef _PDF_DICT_KEYS_A_
#undef _PDF_DICT_KEYS_BOOL_
#undef _PDF_DICT_KEYS_INT32_
