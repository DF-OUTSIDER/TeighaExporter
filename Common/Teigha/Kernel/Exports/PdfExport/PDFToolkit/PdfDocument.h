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




#ifndef _PDFDOCUMENT_INCLUDED_
#define _PDFDOCUMENT_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "PdfHeader.h"
#include "PdfBody.h"
#include "PdfCrossTable.h"
#include "PdfTrailerDictionary.h"
#include "PdfCatalogDictionary.h"
#include "PdfStreamFilter.h"
#include "PdfDocumentInformation.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFDocument
{
  OdString             m_PathForTmpFiles;
  PDFHeader            m_Header;
  PDFBody              m_Body;
  PDFCrossTable        m_CrossTable;
  PDFTrailerDictionary m_Trailer;
  OdString             m_sUniqueKey;
  
  PDFObjectID          m_CurObjectID;

  bool                 m_useFlateEncoding;
  bool                 m_useASCIIHEXEncoding;
  bool                 m_useDCTEncoding;
  bool                 m_PdfA;
  bool                 m_Linearized;

  typedef OdArray<PDFStreamFilterPtr> PDFFilterArray;
  PDFFilterArray m_Filters;

  PDFDocument(const PDFDocument&);
  PDFDocument& operator = (const PDFDocument&);
public:
  PDFDocument();

  void clearDictionaries();

  OdString getUniqueTempPath() const;
  OdString getUniqueKey() const;
  void setTmpStream(OdStreamBuf *pTmpStream);

  bool encodingEnabled() const { return m_useFlateEncoding; }
  void enableEncoding(bool bEnable) { m_useFlateEncoding = bEnable; }

  bool encodingASCIIHEXEnabled() const { return m_useASCIIHEXEncoding; }
  void enableEncodingASCIIHEX(bool bEnable) { m_useASCIIHEXEncoding = bEnable; }

  bool encodingDCTEnabled() const { return m_useDCTEncoding; }
  void enableEncodingDCT(bool bEnable) { m_useDCTEncoding = bEnable; }

  bool isPdfA() const { return m_PdfA; }
  void setPdfA(bool bEnable);

  bool isLinearized() const { return m_Linearized; }
  void setLinearized(bool bEnable);

public:
  PDFVersion getVersion() const;

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

  bool AddObject(PDFObjectPtr pObj);
  void RemoveObject(PDFObjectPtr pObj);

  bool setRoot  (PDFCatalogDictionaryPtr pObj); // must be indirect reference
  PDFCatalogDictionaryPtr Root();

  bool setDocumentInformation(PDFDocumentInformationPtr pDI);  // must be indirect reference
  PDFDocumentInformationPtr getDocumentInformation() const;

  PDFObjectID getNextObjectID();
  PDFObjectID getLastObjectID() const;

  // Stream filters functionality
  bool RegistryFilter(PDFStreamFilterPtr pFilter);
  bool HasFilter(OdAnsiString FilterName) const;
  PDFStreamFilterPtr GetFilter(OdAnsiString FilterName) const;

  virtual ~PDFDocument();
};

};

#endif //_PDFDOCUMENT_INCLUDED_

