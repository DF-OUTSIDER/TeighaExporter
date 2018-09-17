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



//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

#ifndef _PDF_TYPE3_OPTIMIZER_
#define _PDF_TYPE3_OPTIMIZER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PdfType3Font.h"
#include "PdfContentStream4Type3.h"
#include "PdfResourceDictionary.h"
#include "UInt16Array.h"
#include "Int32Array.h"
#include "PdfShxGeomStore.h"
#include "BoolArray.h"

using namespace TD_PDF;
class OdGiTextStyle;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

/** \details
  This class implements the type optimizer for PDF export.
*/
class PDFType3Optimizer
{
protected:

  struct PDFType3OptElem
  {
    PDFType3OptElem()
    {
    }

    PDFType3FontPtr pFont;
    OdDoubleArray    nWidths;
    OdUInt16Array   UnicodeChars;
    OdArray<PDFContentStream4Type3Ptr> ContStreams;
    PDFBBoxStore m_BBox;

    const static OdUInt32 nMaxCharactersInType3Font;

    bool AddUChar(OdChar nch, double nWidth, PDFContentStream4Type3Ptr pGeomData, OdUInt8 &singleByteCode)
    {
      if (UnicodeChars.size() < nMaxCharactersInType3Font)
      {
        nWidths.push_back(nWidth);
        UnicodeChars.push_back(nch);
        ContStreams.push_back(pGeomData);
        singleByteCode = OdUInt8(UnicodeChars.size() - 1);
        return true;
      }
      return false;
    }

    bool hasCharacter(OdChar nch, OdUInt8 &singleByteCode)
    {
      OdUInt16Array::size_type index;
      
      bool b = UnicodeChars.find(nch, index);
      singleByteCode = OdUInt8(b ? index : 0);
      
      return b;
    }

    bool hasFreeSpace(OdUInt16 nNumChars)
    {
      return (nMaxCharactersInType3Font - UnicodeChars.size() >= nNumChars);
    }
  };

  typedef OdArray<PDFType3OptElem> PDFType3OptElemArray;

  virtual PDFType3OptElem *AddNewElem(PDFType3FontPtr pFont);
  virtual PDFType3OptElem *Find(PDFType3FontPtr pFont);

private:
  PDFType3OptElemArray m_pFonts;

public:
  PDFType3Optimizer();
  virtual ~PDFType3Optimizer();

  void clear();

  void addUnicodeText(const OdGiTextStyle& pTextStyle, PDFType3FontPtr pFont, const OdUInt16Array &pUnicode, OdAnsiString &text, const OdBoolArray &isInBigFont);
  void addUnicodeChar(const OdGiTextStyle& pTextStyle, PDFType3FontPtr pFont, OdChar pUnicode, OdAnsiString &text, OdBool isInBigFont);
  
  PDFResultEx getFontForCharacter(PDFDocument &PDFDoc, const OdGiTextStyle& pTextStyle, OdChar pUnicode, PDFFontPtr &charFont, OdAnsiString &fontName, double lineWeight, OdGeVector2d u2d, OdGeVector2d v2d,
    PDFResourceDictionaryPtr pResDict);

  virtual void Optimize();
};

}
#endif // #ifndef _PDF_FONT_OPTIMIZER_
  
