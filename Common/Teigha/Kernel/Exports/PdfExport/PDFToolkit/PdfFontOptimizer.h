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

#ifndef _PDF_FONT_OPTIMIZER_
#define _PDF_FONT_OPTIMIZER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PdfFont.h"

namespace TD_PDF {

/** \details
  This class implements the font optimizer for PDF export.
*/
class PDFEXPORT_TOOLKIT PDFFontOptimizer
{
  bool m_bEmbeddedMode;
  bool m_bOptimizedEmbeddedMode;

protected:
  struct PDFFontOptElem
  {
    PDFFontOptElem()
      : nFirstChar(0xFFFF)
      , nLastChar(0)
      //, bOptimizedEmbeddedMode(false)
    {
    }
    PDFFontPtr pFont;
    OdUInt16Array fontUsedUnicodeChars;
    OdUInt16 nFirstChar;
    OdUInt16 nLastChar;
   // bool bOptimizedEmbeddedMode;

    void AddChar(char ch)
    {
      nFirstChar = odmin(nFirstChar, (OdUInt16)(ch));
      nLastChar  = odmax(nLastChar , (OdUInt16)(ch));
      for(OdUInt32 f=0; f<fontUsedUnicodeChars.size(); ++f)
      {
        if (fontUsedUnicodeChars[f] == ch)
          return;
      }
      fontUsedUnicodeChars.push_back(ch);
    }
    void AddUChar(OdUInt16 nch)
    {
      nFirstChar = odmin(nFirstChar, nch);
      nLastChar  = odmax(nLastChar , nch);
      for(OdUInt32 f=0; f<fontUsedUnicodeChars.size(); ++f)
      {
        if (fontUsedUnicodeChars[f] == nch)
          return;
      }
      fontUsedUnicodeChars.push_back(nch);
    }
  };

  typedef OdArray<PDFFontOptElem> PDFFontOptElemArray;

  virtual PDFFontOptElem *AddNewElem(PDFFontPtr pFont);
  virtual PDFFontOptElem *Find(PDFFontPtr pFont);

private:
  PDFFontOptElemArray m_pFonts;

public:
  PDFFontOptimizer()
    : m_bOptimizedEmbeddedMode(false)
    , m_bEmbeddedMode(false)
  {
  }
  virtual ~PDFFontOptimizer();

  void clear(bool bOptimizedEmbeddedMode, bool bEmbeddedMode);
  void addText(PDFFontPtr pFont, const char *pStr);
  void addUnicodeText(PDFFontPtr pFont, const OdArray<OdUInt16> &pUnicode);

  virtual void Optimize();
};

}
#endif //_PDF_FONT_OPTIMIZER_
