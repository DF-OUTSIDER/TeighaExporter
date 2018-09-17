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




#ifndef _PDFNAME_TREE_NODE_NAMES_ELEMENT_INCLUDED_
#define _PDFNAME_TREE_NODE_NAMES_ELEMENT_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "PdfTextString.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFNameTreeNodeNamesElement : public PDFObject
{
  PDF_DECLARE_OBJECT(PDFNameTreeNodeNamesElement, PDFObject, kNameTreeNodeNamesElement)

protected:
  
  PDFTextStringPtr m_ObjectName;
  PDFObjectPtr m_Object;

public:

  const PDFTextStringPtr &name() const { return m_ObjectName; }
  PDFTextStringPtr &name() { return m_ObjectName; }

  const PDFObjectPtr &object() const { return m_Object; }
  PDFObjectPtr &object() { return m_Object; }

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);

  static PDFSmartPtr<PDFNameTreeNodeNamesElement> createObject(PDFDocument &pDoc, const OdString& pStr, const PDFObjectPtr pObject, bool isIndirect = false);
};

typedef PDFSmartPtr<PDFNameTreeNodeNamesElement> PDFNameTreeNodeNamesElementPtr;

};

#endif //_PDFNAME_TREE_NODE_NAMES_ELEMENT_INCLUDED_

