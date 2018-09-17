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




#ifndef _PDFINTEGER_INCLUDED_
#define _PDFINTEGER_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFInteger : public PDFObject
{
  OdInt32 m_Int;
  PDF_DECLARE_OBJECT(PDFInteger, PDFObject, kInteger)
public:
  static PDFSmartPtr<PDFInteger> createObject(PDFDocument &pDoc, OdInt32 i, bool isIndirect = false);

  OdInt32 &Int32() { return m_Int; }
  const OdInt32 &Int32() const { return m_Int; }

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
};

typedef PDFSmartPtr<PDFInteger> PDFIntegerPtr;

};

#endif //_PDFINTEGER_INCLUDED_

