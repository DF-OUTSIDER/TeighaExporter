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




#ifndef _PDFTRAILERDICTIONARY_INCLUDED_
#define _PDFTRAILERDICTIONARY_INCLUDED_ /*!DOM*// 

#include "PdfDictionary.h"
#include "PdfInteger.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFTrailerDictionary : public PDFDictionary
{
  DEFINE_PDF_RTTI(PDFTrailerDictionary, PDFDictionary, kTrailerDictionary)

  OdUInt32 m_nLastXREFOffset;
  bool     m_bPdfA;
  bool     m_Linearized;

  PDFTrailerDictionary(const PDFTrailerDictionary&);
  PDFTrailerDictionary& operator = (const PDFTrailerDictionary&);
public:
  PDFTrailerDictionary();

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

  void setLastXREFOffset(OdUInt32 nOffset);

  void setPdfA(bool bEnable);
  bool isPdfA() const;

  void setLinearized(bool bEnable);
  bool isLinearized() const;

#include "PdfDictKeysDefs.h"
#include "PdfTrailerDictionaryKeys.h"
#include "PdfDictKeysUnDefs.h"

};

};

#endif //_PDFTRAILERDICTIONARY_INCLUDED_

