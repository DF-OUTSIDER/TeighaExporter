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




#ifndef _PDFVERSION_INCLUDED_
#define _PDFVERSION_INCLUDED_ /*!DOM*// 

#include "PdfToolkitExportDef.h"

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFVersion
{
public:
  enum Versions
  {
    kPDFUnknown = 0,
    kPDFv1_0 = 10,
    kPDFv1_1 = 11,
    kPDFv1_2 = 12,
    kPDFv1_3 = 13,
    kPDFv1_4 = 14,
    kPDFv1_5 = 15,
    kPDFv1_6 = 16,
    kPDFv1_7 = 17,
    kLast    = kPDFv1_7
  };

private:
  Versions m_PdfVer;

public:
  PDFVersion();

  PDFVersion(Versions PdfVer);

  Versions Version() const;

  OdString asString() const;

  Versions FromString(const OdChar *str);

  PDFVersion& operator = (const PDFVersion::Versions& ver)
  {
    m_PdfVer = ver;
    return *this;
  }

  bool operator ==(
    const PDFVersion& ver) const
  {
    return m_PdfVer == ver.m_PdfVer;
  }

  bool operator ==(
    const PDFVersion::Versions& ver) const
  {
    return m_PdfVer == ver;
  }
};

};

#endif //_PDFVERSION_INCLUDED_

