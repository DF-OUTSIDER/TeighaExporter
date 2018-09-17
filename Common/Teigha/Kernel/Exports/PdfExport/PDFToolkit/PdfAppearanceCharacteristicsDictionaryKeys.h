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

_PDF_DICT_KEYS_A_(R , PDFInteger)   //(Optional) The number of degrees by which the widget annotation shall be rotated counterclockwise relative to the page. Default value : 0.
_PDF_DICT_KEYS_A_(BC, PDFArray)     //(Optional) An array of numbers that shall be in the range 0.0 to 1.0 specifying the colour of the border. 0 - transparent, 1 - DeviceGray, 3 - DeviceRGB
_PDF_DICT_KEYS_A_(BG, PDFArray)     //(Optional) An array of numbers that shall be in the range 0.0 to 1.0 specifying the colour of the background.
_PDF_DICT_KEYS_A_(CA, PDFTextString)//(Optional; button fields only) The widget annotation’s normal caption, which shall be displayed when it is not interacting with the user.
_PDF_DICT_KEYS_A_(RC, PDFTextString)//(Optional; pushbutton only) The rollover caption, which shall be displayed when the user rolls the cursor into its active area without pressing the mouse button.
_PDF_DICT_KEYS_A_(AC, PDFTextString)//(Optional; pushbutton  only) The alternate(down) caption, which shall be displayed when the mouse button is pressed within its active area.
_PDF_DICT_KEYS_A_(I , PDFStream)    //(Optional; pushbutton only; indirect reference) A form XObject defining the normal icon, which shall be displayed when it is not interacting with the user.
_PDF_DICT_KEYS_A_(RI, PDFStream)    //(Optional; pushbutton only; indirect reference) A form XObject defining the rollover icon.
_PDF_DICT_KEYS_A_(IX, PDFStream)    //(Optional; pushbutton only; indirect reference) A form XObject defining the alternate(down) icon.
_PDF_DICT_KEYS_A_(IF, PDFDictionary)//(Optional; pushbutton only) An icon fit dictionary(see Table 247) specifying how the icon shall be displayed within its annotation rectangle.
_PDF_DICT_KEYS_A_(TP, PDFInteger)   //(Optional; pushbutton only) A code indicating where to position the text of the caption relative to its icon : Default value : 0 - No icon; caption only

#undef _PDF_DICT_KEYS_A_
