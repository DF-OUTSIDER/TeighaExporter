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




#ifndef _PDCOLORKEYMASKARRAY_INCLUDED_
#define _PDCOLORKEYMASKARRAY_INCLUDED_ /*!DOM*// 

#include "OdArray.h"
#include "PdfObject.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFColorKeyMaskArray : public PDFArray
{
  PDF_DECLARE_OBJECT(PDFColorKeyMaskArray, PDFArray, kColorKeyMaskArray)
public:
  virtual void AddSingleColorMask(OdUInt8 r, OdUInt8 g, OdUInt8 b);
  virtual void AddColorRangeMask(OdUInt8 r1, OdUInt8 g1, OdUInt8 b1, OdUInt8 r2, OdUInt8 g2, OdUInt8 b2);
  virtual void AddSingleColorIndexMask(OdUInt8 i);
  virtual void AddColorIndexRangeMask(OdUInt8 i1, OdUInt8 i2);
};

typedef PDFSmartPtr<PDFColorKeyMaskArray> PDFColorKeyMaskArrayPtr;

};

#endif //_PDCOLORKEYMASKARRAY_INCLUDED_

