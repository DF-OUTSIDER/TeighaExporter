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




#ifndef _PDFBOOLEAN_INCLUDED_
#define _PDFBOOLEAN_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;
class PDFDocument;

class PDFEXPORT_TOOLKIT PDFBoolean : public PDFObject
{
  bool m_Bool;

  PDF_DECLARE_OBJECT(PDFBoolean, PDFObject, kBoolean)
public:
  static PDFSmartPtr<PDFBoolean> createObject(PDFDocument &pDoc, bool is, bool isIndirect);

  bool &Bool() { return m_Bool; }
  const bool &Bool() const { return m_Bool; }

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
};

typedef PDFSmartPtr<PDFBoolean> PDFBooleanPtr;

};

#endif //_PDFBOOLEAN_INCLUDED_

