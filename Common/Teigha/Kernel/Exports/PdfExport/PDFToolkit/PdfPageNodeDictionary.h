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




#ifndef _PDFPAGENODEDICTIONARY_INCLUDED_
#define _PDFPAGENODEDICTIONARY_INCLUDED_ /*!DOM*// 

#include "PdfDictionary.h"
#include "PdfArray.h"
#include "PdfPageDictionary.h"
#include "PdfInteger.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFPageNodeDictionary;
typedef PDFSmartPtr<PDFPageNodeDictionary> PDFPageNodeDictionaryPtr;

class PDFEXPORT_TOOLKIT  PDFPageNodeDictionary : public PDFDictionary
{
  PDF_DECLARE_OBJECT(PDFPageNodeDictionary, PDFDictionary, kPageNodeDictionary)

  PDFDictionary *m_pParent;  

  void increaseCountBy(OdUInt32 nPages);

protected:
  virtual void InitObject();

public:
  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

  void setParent(PDFDictionaryPtr pVal);
  PDFDictionaryPtr getParent() const;

#include "PdfDictKeysDefs.h"
#include "PdfPageNodeDictionaryKeys.h"
#include "PdfDictKeysUnDefs.h"

  void AddKids(PDFPageDictionaryPtr pPage);
  void AddKids(PDFPageNodeDictionaryPtr pPage);

  virtual ~PDFPageNodeDictionary();
};

};

#endif //_PDFPAGENODEDICTIONARY_INCLUDED_

