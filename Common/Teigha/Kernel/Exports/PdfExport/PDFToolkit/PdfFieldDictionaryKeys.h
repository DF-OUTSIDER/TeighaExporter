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

_PDF_DICT_KEYS_A_(FT, PDFName)                    //(Required for terminal fields; inheritable) The type of field that this dictionary describes
_PDF_DICT_KEYS_A_(Parent, PDFDictionary)          //(Required if this field is the child of another in the field hierarchy; absent otherwise) 
_PDF_DICT_KEYS_A_(Kids, PDFArray)                 //(Sometimes required, as described below) An array of indirect references to the immediate children of this field
_PDF_DICT_KEYS_A_(T, PDFTextString)               //(Optional) The partial field name 
_PDF_DICT_KEYS_A_(TU, PDFTextString)              //(Optional; PDF 1.3) An alternate field name that shall be used in place of the actual field 
                                                  //name wherever the field shall be identified in the user interface
_PDF_DICT_KEYS_A_(TM, PDFTextString)              //(Optional; PDF 1.3) The mapping name that shall be used when exporting interactive form field data from the document.
_PDF_DICT_KEYS_BF_(Ff, PDFFieldFlags)             //(Optional; inheritable) A set of flags specifying various characteristics of the field (see Table 221 ). Default value: 0.
_PDF_DICT_KEYS_A_(V, PDFObject)                   //(Optional; inheritable)
_PDF_DICT_KEYS_A_(DV, PDFObject)                  //(Optional; inheritable)
//_PDF_DICT_KEYS_A_(AA, PDFTriggerEventsDictionary) //(Optional; PDF 1.2) An additional-actions dictionary defining the field’s behaviour in response to various trigger events
_PDF_DICT_KEYS_A_(Opt, PDFArray)                  //(Optional) An array of options that shall be presented to the user.
_PDF_DICT_KEYS_A_(DA, PDFBaseString)              //(Optional) Default appearance


#undef _PDF_DICT_KEYS_A_
