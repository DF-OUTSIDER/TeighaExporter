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




#ifndef _PDFCROSSTABLE_INCLUDED_
#define _PDFCROSSTABLE_INCLUDED_ /*!DOM*// 


#include "PdfObject.h"

#define STL_USING_FUNCTIONAL
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFCrossTable
{
public:
  struct XREFDATA {
    PDFObjectPtr pIndirectObj;
    OdUInt32 offset;

    XREFDATA()
      : offset(0)
    {
    }

    bool Export(PDFIStream *pStream, const PDFVersion &/*ver*/);
  };

  class xRefDataComparer : public std::binary_function<XREFDATA, XREFDATA, bool>
  {
  public:
    bool operator()(const XREFDATA& a1, const XREFDATA& a2)
    {
      PDFObjectID id1, id2;
      a1.pIndirectObj->IndirectObjectID(id1);
      a2.pIndirectObj->IndirectObjectID(id2);

      return id1.ObjectNumber() < id2.ObjectNumber();
    }
  };

  typedef OdArray<XREFDATA> XREFDATAARRAY;

private:

  mutable XREFDATAARRAY m_xRefData;

  PDFCrossTable(const PDFCrossTable&);
  PDFCrossTable& operator = (const PDFCrossTable&);
public:
  
  
  PDFCrossTable();

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

  void StartCrossTable();
  void AddIndirectObjectOffset(PDFObjectPtr pIndirectObj, OdUInt32 offset);
  void EndCrossTable();
};

};

#endif //_PDFCROSSTABLE_INCLUDED_

