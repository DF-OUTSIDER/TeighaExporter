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




#ifndef _PDFTEXTSTRING_INCLUDED_
#define _PDFTEXTSTRING_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "OdString.h"
#include "OdAnsiString.h"
#include "PdfUnicodeTextHelper.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFTextString : public PDFObject, public PDFUnicodeTextHelper
{
  OdString m_UnicodeStr;
  OdAnsiString m_AnsiStr;

  PDF_DECLARE_OBJECT(PDFTextString, PDFObject, kTextString)

public:
  static PDFSmartPtr<PDFTextString> createObject(PDFDocument &pDoc, OdString pStr, bool isIndirect = false);
  static PDFSmartPtr<PDFTextString> createObject(PDFDocument &pDoc, OdAnsiString pStr, bool isIndirect = false);

  bool isEqualTo(OdString str) const { return m_UnicodeStr.isEmpty() ? (OdString(m_AnsiStr) == str) : (m_UnicodeStr == str);}
//   OdString getString() const { return  }
  OdString getAsUnicode() const
  {
    if (m_UnicodeStr.isEmpty()) 
      return OdString(m_AnsiStr);
    else
      return m_UnicodeStr;
  }
//   OdString getAnsi() const { return m_AnsiStr; }

  void clear();

  bool isInUnicode() const { return !m_UnicodeStr.isEmpty(); }

  const PDFTextString& set(OdString stringSrc);
  const PDFTextString& set(OdAnsiString stringSrc);

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
  bool isAscii(const OdChar* s);

};

typedef PDFSmartPtr<PDFTextString> PDFTextStringPtr;

};

#endif //_PDFTEXTSTRING_INCLUDED_

