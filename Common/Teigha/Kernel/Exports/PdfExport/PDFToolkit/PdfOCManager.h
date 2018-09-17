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




#ifndef _PDFOCMANAGER_INCLUDED_
#define _PDFOCMANAGER_INCLUDED_ /*!DOM*// 

#include "PdfOptionalContentConfigurationDictionary.h"
#include "PdfName.h"
#include "PdfPageNodeDictionary.h"
#include "PdfResourceDictionary.h"

namespace TD_PDF
{
  namespace PDFOCManager
  {
    void PDFEXPORT_TOOLKIT clearLayersData();
    PDFNamePtr PDFEXPORT_TOOLKIT createOCG(const OdString &name, PDFOCConfigurationDictionaryPtr pOCC, bool bVisible, bool bLocked, PDFResourceDictionaryPtr pResource);
    PDFNamePtr PDFEXPORT_TOOLKIT getOCGbyName(const OdString &name, PDFDocument &PDFDoc, PDFResourceDictionaryPtr pResource);
    PDFPageNodeDictionaryPtr PDFEXPORT_TOOLKIT getOCGNodeByName(const OdString &name, PDFDocument &PDFDoc);
  }
}

#endif //_PDFOCMANAGER_INCLUDED_

