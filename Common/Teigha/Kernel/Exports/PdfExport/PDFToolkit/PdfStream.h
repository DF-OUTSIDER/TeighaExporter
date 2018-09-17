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




#ifndef _PDFSTREAM_INCLUDED_
#define _PDFSTREAM_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "PdfStreamDictionary.h"
#include "PdfIStream.h"
#include "PdfDecodeParametersDictionary.h"

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFStream : public PDFObject
{
  PDF_DECLARE_NCR_OBJECT(PDFStream, PDFObject, kStream)
private:
  PDFStreamDictionaryPtr   m_pDict;
  PDFIStreamPtr            m_pIStream;

protected:
  virtual void InitObject();

  bool CheckFilters() const;
  bool EncodeStream();
  bool DecodeStream();

  void setDictionary(PDFStreamDictionaryPtr pStreamDictionary);
  PDFStreamDictionaryPtr getDictionary();

  PDFIStreamPtr getStreamData();
  void setStreamData(PDFIStreamPtr pStreamData);

  virtual OdUInt32 updateLength();

  virtual bool ExportDictionary(PDFIStream * /*pStream*/, const PDFVersion &/*ver*/);
  virtual bool ExportStreamData(PDFIStream * /*pStream*/, const PDFVersion &/*ver*/);

  void clearDecodeParamsIfEmpty();

public:

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
  virtual OdUInt32 getLength();

  virtual bool AddFilter(OdAnsiString pFilterName, PDFDecodeParametersDictionaryPtr pDecodeParams);
  virtual bool RemoveFilter(OdAnsiString pFilterName);

  virtual OdUInt32 getNumberOfFilters() const;
  virtual bool getFilterAt(OdUInt32 nIndx, OdAnsiString &pFilterName) const;

  // it can return null PDFDecodeParametersDictionaryPtr, if filter hasn't Parameters or it is default.
  virtual bool getDecodeParamsAt(OdUInt32 nIndx, PDFDecodeParametersDictionaryPtr &pDecodeParams) const;
};

typedef PDFSmartPtr<PDFStream> PDFStreamPtr;

};

#endif //_PDFSTREAM_INCLUDED_

