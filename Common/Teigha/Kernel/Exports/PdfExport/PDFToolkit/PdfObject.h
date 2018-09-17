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




#ifndef _PDFOBJECT_INCLUDED_
#define _PDFOBJECT_INCLUDED_ /*!DOM*// 

#include "PdfRefCountImpl.h"

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFAbstractObject
{
  DECLARE_PDF_ABSTRACT_RTTI()
private:
  PDFAbstractObject(const PDFAbstractObject&);
  PDFAbstractObject& operator = (const PDFAbstractObject&);

protected:
  PDFAbstractObject() { }
  virtual void InitObject() {}

public:
  virtual ~PDFAbstractObject() {}
};

class PDFEXPORT_TOOLKIT PDFBaseObject : public PDFRefCountImpl<PDFAbstractObject>
{
public:
  virtual bool isKindOf(PDFTypeId objType) const
  {
    return objType == kBaseObject;
  }
  virtual PDFTypeId type() const { return kBaseObject; }

  PDFBaseObject(const PDFBaseObject&);
  PDFBaseObject& operator = (const PDFBaseObject&);

protected:
  PDFBaseObject() {}
};

};

#include "PdfSmartPtr.h"

namespace TD_PDF
{
class PDFIStream;
class PDFVersion;
class PDFDocument;
class PDFObjectID;

class PDFEXPORT_TOOLKIT PDFObject : public PDFBaseObject
{
  DEFINE_PDF_RTTI(PDFObject, PDFBaseObject, kObject)

  PDFObject(const PDFObject&);
  PDFObject& operator = (const PDFObject&);

  PDFDocument *m_pDocument;

protected:
  PDFObject()
    : m_pDocument(0)
  {}

public:
  virtual void IndirectObjectID(PDFObjectID & /*id*/) const {}
  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver) = 0;
  virtual bool ExportLikeRef(PDFIStream * /*pStream*/, const PDFVersion &/*ver*/) { return false; }
  virtual bool ExportLikeRefObj(PDFIStream * /*pStream*/, const PDFVersion &/*ver*/) { return false; }
  virtual bool isIndirect() const { return false; }
  virtual PDFDocument *document() const { return m_pDocument; }
  virtual void setDocument(PDFDocument *pDoc) { if (pDoc) m_pDocument = pDoc; };
  virtual void clearDictionaries() {}
};

typedef PDFSmartPtr<PDFObject> PDFObjectPtr;

};

#endif //_PDFOBJECT_INCLUDED_

