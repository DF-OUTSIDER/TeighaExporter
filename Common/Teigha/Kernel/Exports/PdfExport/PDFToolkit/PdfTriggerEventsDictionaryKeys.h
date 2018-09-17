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

_PDF_DICT_KEYS_A_(E, PDFDictionary)      //(Optional; PDF 1.2) An action that shall be performed when the cursor enters the annotation’s active area.
_PDF_DICT_KEYS_A_(X, PDFDictionary)      //(Optional; PDF 1.2) An action that shall be performed when the cursor exits the annotation’s active area
_PDF_DICT_KEYS_A_(D, PDFDictionary)      //(Optional; PDF 1.2) An action that shall be performed when the mouse button is pressed inside the annotation’s active area
_PDF_DICT_KEYS_A_(U, PDFDictionary)      //(Optional; PDF 1.2) An action that shall be performed when the mouse button is released inside the annotation’s active area. 
_PDF_DICT_KEYS_A_(Fo, PDFDictionary)     //(Optional; PDF 1.2; widget annotations only) An action that shall be performed when the annotation receives the input focus.
_PDF_DICT_KEYS_A_(Bl, PDFDictionary)     //(Optional; PDF 1.2; widget annotations only) An action that shall be performed when the annotation loses the input focus.
_PDF_DICT_KEYS_A_(PO, PDFDictionary)     //(Optional; PDF 1.5) An action that shall be performed when the page containing the annotation is opened.
_PDF_DICT_KEYS_A_(PC, PDFDictionary)     //(Optional; PDF 1.5) An action that shall be performed when the page containing the annotation is closed.
_PDF_DICT_KEYS_A_(PV, PDFDictionary)     //(Optional; PDF 1.5) An action that shall be performed when the page containing the annotation becomes visible.
_PDF_DICT_KEYS_A_(PI, PDFDictionary)     //(Optional; PDF 1.5) An action that shall be performed when the page containing the annotation is no longer visible in the conforming reader’s user interface.

#undef _PDF_DICT_KEYS_A_
