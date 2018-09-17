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




#ifndef _PDFISTREAM_INCLUDED_
#define _PDFISTREAM_INCLUDED_ /*!DOM*// 

#include "OdStreamBuf.h"
#include "PdfObject.h"

#define CROP_ZEROES_FOR_ALL 6
#define CROP_ZEROES_FOR_RGB 6

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFIStream : public PDFBaseObject
{
 PDF_DECLARE_BASE_OBJECT(PDFIStream, PDFBaseObject, kIStream)
private:
  OdStreamBufPtr m_pBuf;
  
protected:
  void closeStream();
  virtual void internalPutString(const char *pStr);
  virtual OdStreamBufPtr getStreamBuf() { return m_pBuf; }

public:
  virtual OdUInt8   getByte();
  virtual void      getBytes(void* buffer, OdUInt32 nLen);

  virtual void setStreamBuf(OdStreamBufPtr pBuf);

  virtual ~PDFIStream() {}

  virtual OdString  fileName();

  virtual bool      isEof();
  virtual OdUInt32  tell();
  virtual OdUInt32  length();

  virtual void      truncate();
  virtual void      rewind();
  virtual OdUInt32  seek(OdInt32 offset, OdDb::FilerSeekType whence);

  virtual void      copyDataTo(PDFIStream* pDest, OdUInt32 nSrcStart, OdUInt32 nSrcEnd);
  virtual OdUInt32  getShareMode();
  
  virtual void      putEOL();
  virtual void      putBool(bool val);
  virtual void      putDouble(double val);
  virtual void      putInt32(OdInt32 val);
  virtual void      putString(const char* val);
  virtual void      putByte(OdUInt8 val);

  virtual void      putBytes(const void* buffer, OdUInt32 nLen);

  virtual bool      Export(PDFIStream *pStream, const PDFVersion &ver);
};

typedef PDFSmartPtr<PDFIStream> PDFIStreamPtr;

};

#endif //_PDFISTREAM_INCLUDED_

