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




#ifndef _PDFTOUNICODECMAP_INCLUDED_
#define _PDFTOUNICODECMAP_INCLUDED_ /*!DOM*// 

#include "PdfCMap.h"
#include "OdArray.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

struct PDFEXPORT_TOOLKIT PDFCharPair
{
  OdUInt16 nCharCode;
  OdUInt16 nUnicodeCode;

  PDFCharPair()
    : nCharCode(0)
    , nUnicodeCode(0)
  {
  }

  bool operator ==(const TD_PDF::PDFCharPair& otherPair) const
  {
    return nCharCode == otherPair.nCharCode && nUnicodeCode == otherPair.nUnicodeCode;
  }

  bool operator !=(const TD_PDF::PDFCharPair& otherPair) const
  {
    return nCharCode != otherPair.nCharCode || nUnicodeCode != otherPair.nUnicodeCode;
  }
};

class PDFEXPORT_TOOLKIT PDFCharPairArray : public OdArray<PDFCharPair>
{
public:
  void sortByCharCode();
  PDFResult isValid4ToUnicodeCMap() const;
};

class PDFEXPORT_TOOLKIT PDFToUnicodeCMap : public PDFCMap
{
  PDF_DECLARE_OBJECT(PDFToUnicodeCMap, PDFCMap, kToUnicodeCMap)

protected:
public:

  // charPairs must be sorted by nCharCode
  virtual PDFResult fillStream(const PDFCharPairArray &charPairs);
};

typedef PDFSmartPtr<PDFToUnicodeCMap> PDFToUnicodeCMapPtr;

};

#endif //_PDFTOUNICODECMAP_INCLUDED_

