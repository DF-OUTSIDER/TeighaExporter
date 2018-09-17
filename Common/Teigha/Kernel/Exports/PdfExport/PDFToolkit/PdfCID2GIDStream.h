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




#ifndef _PDFCID2GIDSTREAM_INCLUDED_
#define _PDFCID2GIDSTREAM_INCLUDED_ /*!DOM*// 

#include "PdfStream.h"
#include "UInt16Array.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFCID2GIDStream : public PDFStream
{
  PDF_DECLARE_OBJECT(PDFCID2GIDStream, PDFStream, kCID2GIDStream)
protected:
  virtual void InitObject();
public:
  virtual void getData(OdUInt16Array &data) const;
  virtual void fill(const OdUInt16Array &data);
};

typedef PDFSmartPtr<PDFCID2GIDStream> PDFCID2GIDStreamPtr;

};

#endif //_PDFCID2GIDSTREAM_INCLUDED_

