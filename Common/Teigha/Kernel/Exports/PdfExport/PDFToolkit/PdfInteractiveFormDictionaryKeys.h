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


_PDF_DICT_KEYS_BF_(Fields, PDFArray)            //(Required) An array of references to the document’s root fields (those with no ancestors in the field hierarchy).
_PDF_DICT_KEYS_A_(NeedAppearances, PDFBoolean)  //(Optional) A flag specifying whether to construct appearance streams and appearance dictionaries 
                                                //for all widget annotations in the document . Default value: false.
_PDF_DICT_KEYS_A_(SigFlags, PDFInteger)         //(Optional; PDF 1.3) A set of flags specifying various documentlevel characteristics related to signature 
                                                //fields. Default value: 0.
_PDF_DICT_KEYS_A_(CO, PDFArray)                 //(Required if any fields in the document have additional-actions dictionaries containing a C entry; PDF 1.3) 
                                                //An array of indirect references to field dictionaries with calculation actions, defining
                                                //the calculation order in which their values will be recalculated when the value of any field changes.
_PDF_DICT_KEYS_BF_(DR, PDFDictionary)            //(Optional) A resource dictionary containing default resources (such as fonts, patterns, or colour spaces) 
                                                //that shall be used by form field appearance streams. 
_PDF_DICT_KEYS_A_(DA, PDFBaseString)            //(Optional) A document-wide default value for the DA attribute of variable text fields
_PDF_DICT_KEYS_A_(Q, PDFInteger)                //(Optional) A document-wide default value for the Q attribute of variable text fields
_PDF_DICT_KEYS_A_(XFA, PDFObject)               //(Optional; PDF 1.5) A stream or array containing an XFA resource, whose format 
                                                //shall be described by the Data Package (XDP) Specification. 


#undef _PDF_DICT_KEYS_A_
