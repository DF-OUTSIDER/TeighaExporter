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




#ifndef _PDFOCCONFIGURATIONDICTIONARY_INCLUDED_
#define _PDFOCCONFIGURATIONDICTIONARY_INCLUDED_ /*!DOM*// 

#include "PdfDictionary.h"
#include "PdfContentStream4Type3.h"
#include "PdfOptionalContentGroupDictionary.h"
#include "PdfArray.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFOCConfigurationDictionary : public PDFDictionary
{
  PDF_DECLARE_OBJECT(PDFOCConfigurationDictionary, PDFDictionary, kOCConfigurationDictionary)

protected:
  virtual void InitObject();

public:

#include "PdfDictKeysDefs.h"
#include "PdfOptionalContentConfigurationKeys.h"
#include "PdfDictKeysUnDefs.h"

  void addOC(PDFOCGroupDictionaryPtr pOCG, bool bVisible, bool bLocked = false);

};

typedef PDFSmartPtr<PDFOCConfigurationDictionary> PDFOCConfigurationDictionaryPtr;

};

#endif //_PDFOCCONFIGURATIONDICTIONARY_INCLUDED_

