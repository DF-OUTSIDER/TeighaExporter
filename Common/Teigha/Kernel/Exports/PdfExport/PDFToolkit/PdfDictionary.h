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




#ifndef _PDFDICTIONARY_INCLUDED_
#define _PDFDICTIONARY_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "PdfName.h"
#include "OdArray.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFDictionary : public PDFObject
{
  PDF_DECLARE_OBJECT(PDFDictionary, PDFObject, kDictionary)

protected:
  struct DictItem
  {
    PDFNamePtr   key;
    PDFObjectPtr val;
  };

  typedef OdArray<DictItem> DictVector;
  DictVector m_DictData;

  const DictItem* FindInternal(const char *key) const;
        DictItem* FindInternal(const char *key);
  const DictItem* FindInternal(const PDFObjectPtr val) const;
        DictItem* FindInternal(const PDFObjectPtr val);
protected:
  
  virtual bool AddInt32(const char *key, OdInt32 nVal);
  virtual bool AddBool(const char *key, bool bVal);
  virtual bool AddItem(const char *key, PDFObjectPtr val);
  virtual bool AddItemNoSearch(const char *key, PDFObjectPtr val);

public:
  
  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

  virtual bool RemoveItem(const char *key);
  
  virtual const PDFObjectPtr Find(const char *key) const;
  virtual       PDFObjectPtr Find(const char *key);

  virtual bool HasItem(const char *key) const ;

  virtual PDFNamePtr getUniqueName(const char *base) const;

  virtual void clearDictionaries();

  virtual bool isEmpty();
};

typedef PDFSmartPtr<PDFDictionary> PDFDictionaryPtr;

};

#endif //_PDFDICTIONARY_INCLUDED_

