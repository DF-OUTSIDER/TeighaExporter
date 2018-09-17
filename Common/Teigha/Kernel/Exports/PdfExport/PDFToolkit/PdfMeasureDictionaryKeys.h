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

_PDF_DICT_KEYS_A_(Type     , PDFName)       //(Optional) The type of PDF object that this dictionary describes; must be Measure for a measure dictionary.
_PDF_DICT_KEYS_A_(Subtype  , PDFName)       //(Optional) A name specifying the type of coordinate system to use for measuring. Default value : RL, which specifies a rectilinear coordinate system
_PDF_DICT_KEYS_A_(R        , PDFBaseString) //(Required) A text string expressing the scale ratio of the drawing in the region corresponding to this dictionary.
_PDF_DICT_KEYS_BF_(X       , PDFArray)      //(Required) A number format array for measurement of change along the x axis and, if Y is not present, along the y axis as well.The first element in the array contains the
//scale factor for converting from default user space units to the largest units in the measuring coordinate system along that axis.
_PDF_DICT_KEYS_BF_(Y       , PDFArray)      //(Required when the x and y scales have different units or conversion factors) A number format array for measurement of change along the y axis.
_PDF_DICT_KEYS_BF_(D       , PDFArray)      //(Required)A number format array for measurement of distance in any direction. The first element in the array specifies the conversion to the largest distance unit
//from units represented by the first element in X.
_PDF_DICT_KEYS_BF_(A       , PDFArray)      //(Required) A number format array for measurement of area. The first element in the array specifies the conversion to the largest area unit from units represented by the
//first element in X, squared.
_PDF_DICT_KEYS_BF_(T       , PDFArray)      //(Optional) A number format array for measurement of angles. The first element in the array specifies the conversion to the largest angle unit from degrees.
_PDF_DICT_KEYS_BF_(S       , PDFArray)      //(Optional) A number format array for measurement of the slope of a line. The first element in the array specifies the conversion to the largest slope unit from units
//represented by the first element in Y divided by the first element in X.
_PDF_DICT_KEYS_BF_(O       , PDFArray)      //(Optional) An array of two numbers specifying the origin of the measurement coordinate system in default user space coordinates.
_PDF_DICT_KEYS_A_(CYX      , PDFNumber)     //(Optional; meaningful only when Y is present)

#undef _PDF_DICT_KEYS_A_
