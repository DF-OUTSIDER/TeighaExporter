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

#ifndef _DwfFontManager_Included_
#define _DwfFontManager_Included_

class TK_Text_Font;

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {

class DwfImporter;

/////////////////////////////////////////////////////
// DWF fonts & text management
//

// Flags   Flag bits reserved for the WHIP! data generatorӳ use. 
// Currently the AutoCAD DWF ePlot driver uses this field to set one or more of the following flags: 

#define FLAG_VERTICAL        0x0001      // TVERT
#define FLAG_MIRROR_X        0x0002      // TXMIR
#define FLAG_MIRROR_Y        0x0004      // TYMIR
#define FLAG_UNDERSCORE      0x0008      // TUNDER
#define FLAG_OVERSCORE       0x0010      // TOVER
#define FLAG_MTEXT_START     0x0020      // TMSTART
#define FLAG_MTEXT_END       0x0040      // TMEND
#define FLAG_MTEXT           0x0080      // TMTEXT
#define FLAG_GREEK_TEXT      0x1000      // Whip and GDI font engine only
#define FLAG_PATH_TEXT       0x2000      // Whip and GDI font engine only
#define FLAG_OUTLINE_TEXT    0x4000      // Outline Text Only

/** \details
  This class implements the DWF font manager.
*/
class DwfFontManager
{
  DwfImporter* _importer;
  // text style description
  struct TextStyle
  {
    TextStyle() : _ascentRatio(0.) {}
    TextStyle(WT_Font& font, OdDbTextStyleTableRecordPtr& textStyle, double ascentRatio = 1.)
      :_wtFont(font), _textStyle(textStyle), _ascentRatio(ascentRatio){}
    WT_Font _wtFont;
    OdDbTextStyleTableRecordPtr _textStyle;
    double _ascentRatio; // tmHeight/tmAscent (for DWF->ACAD text height conversion)
  };
  // mapping: WT_Font -> text style id
  typedef OdArray<TextStyle> FontStyleMap;
  FontStyleMap _fontStyles;
  // current text style (index in the _fontStyles array)
  int _currentTextStyle;
public:
  DwfFontManager(DwfImporter* importer)
    :_importer(importer), _currentTextStyle(-1){}
  void clear();
  // update text style table & set TEXTSTYLE
  void setFontStyle(WT_Font& font);
  void setFontStyle(TK_Text_Font& font);
  // calculate acad font height from current style
  double getTextHeight(double);
  // calculate text height scaling, for the style
  void calculateAscent(TextStyle& pStyle);
};
}
#endif
