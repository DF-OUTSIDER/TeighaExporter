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




#ifndef _PDFDATE_INCLUDED_
#define _PDFDATE_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "OdTimeStamp.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFDate : public PDFObject
{
  OdTimeStamp m_UnivDate;
  OdInt8      m_Offset2LocalDateH;
  OdInt8      m_Offset2LocalDateM;

  PDF_DECLARE_OBJECT(PDFDate, PDFObject, kDate)

protected:

  static bool isOffsetValid(OdInt8 nOffH, OdInt8 nOffM);

public:

  virtual bool set(const OdTimeStamp &UnivDate, OdInt8 Offset2LocalDateH = 100, OdInt8 Offset2LocalDateM = 100); // = 100 no offset
  virtual void get(OdTimeStamp &UnivDate, OdInt8 &Offset2LocalDateH, OdInt8 &Offset2LocalDateM) const;

  virtual bool isOffset2LocalDateValid() const;
  virtual void setOffset2Unknown( );

  virtual OdAnsiString toString(bool bWithOffset = true) const;

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
};

typedef PDFSmartPtr<PDFDate> PDFDatePtr;

};

#endif //_PDFDATE_INCLUDED_

