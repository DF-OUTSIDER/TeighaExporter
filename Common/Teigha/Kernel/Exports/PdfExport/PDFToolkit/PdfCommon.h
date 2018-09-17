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




#ifndef _PDF_COMMON_INCLUDED_
#define _PDF_COMMON_INCLUDED_

#ifdef _MSC_VER
#pragma warning ( disable : 4275 ) // non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#endif

#include "OdaCommon.h"

// DW (Optional) The default width for glyphs in the CIDFont (see “Glyph Metrics
// in CIDFonts” on page 409). Default value: 1000.
#define ODPDF_DEFAULT_FONT_WIDTH 1000

#include "PdfToolkitExportDef.h"

#include "PdfEnums.h"
#include "PdfException.h"

#include "PdfDeclareObject.h"
#include "PdfDeclareObjectImpl.h"
#include "PdfLinkTemplates.h"

#include "OdCharMapper.h"

namespace TD_PDF
{
  namespace TD_PDF_HELPER_FUNCS
  {
    static void getUnicodeTextString(const OdAnsiString &pStr, PDFTextStringPtr pUnicodeStr)
    {
      OdCodePageId systemCodePage = odrxSystemServices()->systemCodePage();

      int nLen = pStr.getLength();

      OdString unicodeChars;
      for (int f = 0; f < nLen; ++f)
      {
        OdChar sourceChar = pStr.getAt(f);
        OdChar Char = 0;
        ODA_VERIFY(OdCharMapper::codepageToUnicode(sourceChar, systemCodePage, Char) == ::eOk);
        unicodeChars += Char;
      }

      pUnicodeStr->set(unicodeChars);
    };

    static void getUnicodeTextString(const OdString &pStr, PDFTextStringPtr pUnicodeStr)
    {
      int nLen = pStr.getLength();

      OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> > unicodeChars;
      unicodeChars.resize(nLen + 1, 0);
      for (int f = 0; f < nLen; ++f)
      {
        OdUInt16 sourceChar = (OdUInt16)pStr.getAt(f);
        unicodeChars[f] = sourceChar;
      }

      pUnicodeStr->set(pStr);
    };
  }
}

#endif // _PDF_COMMON_INCLUDED_


