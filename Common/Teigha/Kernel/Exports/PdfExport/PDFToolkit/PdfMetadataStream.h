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


#ifndef _PDF_METADATA_STREAM_INCLUDED_
#define _PDF_METADATA_STREAM_INCLUDED_ /*!DOM*/ 

#include "PdfStream.h"
#include "OdCharMapper.h"
#include "PdfMetadataStreamDictionary.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFMetadataStream : public PDFStream
{
  PDF_DECLARE_OBJECT(PDFMetadataStream, PDFStream, kMetadataStream)

  OdAnsiCharArray m_sTitle;
  OdAnsiCharArray m_sAuthor;
  OdAnsiCharArray m_sSubject;
  OdAnsiCharArray m_sKeywords;
  OdAnsiCharArray m_sCreator;
  OdAnsiCharArray m_sProducer;
  OdTimeStamp m_creationDate;

  OdUInt16    m_iPdfALevel;
  OdUInt16    m_iPdfAConformance;

protected:
  virtual void InitObject();
public:
  PDFMetadataStreamDictionaryPtr getDictionary();

  virtual void putData(const char *pBuf, OdUInt32 nSize);
  virtual void getData(char *pBuf, OdUInt32 nSize);

  void setTitle(OdString sTitle)
  {
    OdCharMapper::unicodeToUtf8(sTitle.c_str(), sTitle.getLength(), m_sTitle);
  };
  void setAuthor(OdString sAuthor)
  {
    OdCharMapper::unicodeToUtf8(sAuthor.c_str(), sAuthor.getLength(), m_sAuthor);
  };
  void setSubject(OdString sSubject)
  {
    OdCharMapper::unicodeToUtf8(sSubject.c_str(), sSubject.getLength(), m_sSubject);
  };
  void setKeywords(OdString sKeywords)
  {
    OdCharMapper::unicodeToUtf8(sKeywords.c_str(), sKeywords.getLength(), m_sKeywords);
  };
  void setCreator(OdString sCreator)
  {
    OdCharMapper::unicodeToUtf8(sCreator.c_str(), sCreator.getLength(), m_sCreator);
  };
  void setProducer(OdString sProducer)
  {
    OdCharMapper::unicodeToUtf8(sProducer.c_str(), sProducer.getLength(), m_sProducer);
  };
  void setCreationDate(OdTimeStamp creationDate) { m_creationDate = creationDate; };
  void setPdfALevelConf(OdUInt16 lev, OdUInt16 conf) //lev (1,2,3) conf (1 - A, 2 - B)
  { 
    m_iPdfALevel = lev;
    m_iPdfAConformance = conf;
  }

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

};

typedef PDFSmartPtr<PDFMetadataStream> PDFMetadataStreamPtr;

};

#endif //_PDF_METADATA_STREAM_INCLUDED_

