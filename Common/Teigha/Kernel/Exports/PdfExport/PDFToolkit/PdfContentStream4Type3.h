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




#ifndef _PDFCONTENTSTREAM4TYPE3_INCLUDED_
#define _PDFCONTENTSTREAM4TYPE3_INCLUDED_ /*!DOM*// 

#include "PdfIContentCommands4Type3.h"
#include "PdfContentStream.h"

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFContentStream4Type3 : public PDFContentStream, public PDFIContentCommands4Type3
{
  PDF_DECLARE_OBJECT(PDFContentStream4Type3, PDFContentStream, kContentStream4Type3)

public:

  // Type 3 font operators
  virtual void d0(double wx, double wy); // wy must be 0
  virtual void d1(double wx, double wy, double llx, double lly, double urx, double ury); // wy must be 0

  virtual void m(double x, double y, bool bToInt = true);
  virtual void l(double x, double y);
};

typedef PDFSmartPtr<PDFContentStream4Type3> PDFContentStream4Type3Ptr;

};

#endif //_PDFCONTENTSTREAM4TYPE3_INCLUDED_

