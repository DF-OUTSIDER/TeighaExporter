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




#ifndef _PDFPROPERTIESDICTIONARY_INCLUDED_
#define _PDFPROPERTIESDICTIONARY_INCLUDED_ /*!DOM*// 

#include "PdfSubDictionary.h"
#include "PdfXObjectForm.h"

namespace TD_PDF
{

  class PDFEXPORT_TOOLKIT PDFPropertiesSubDictionary : public PDFSubDictionary
  {
    PDF_DECLARE_OBJECT(PDFPropertiesSubDictionary, PDFSubDictionary, kPropertiesSubDictionary)

  public:
    virtual const char * getDefBase() const  { return 0; }
  };

  class PDFEXPORT_TOOLKIT PDFPatternSubDictionary : public PDFSubDictionary
  {
    PDF_DECLARE_OBJECT(PDFPatternSubDictionary, PDFSubDictionary, kPatternSubDictionary)

  public:
    virtual const char * getDefBase() const  { return "Pn"; }
  };

  class PDFEXPORT_TOOLKIT PDFShadingSubDictionary : public PDFSubDictionary
  {
    PDF_DECLARE_OBJECT(PDFShadingSubDictionary, PDFSubDictionary, kShadingSubDictionary)

  public:
    virtual const char * getDefBase() const  { return "Sh"; }
  };

  class PDFEXPORT_TOOLKIT PDFColorSpaceSubDictionary : public PDFSubDictionary
  {
    PDF_DECLARE_OBJECT(PDFColorSpaceSubDictionary, PDFSubDictionary, kColorSpaceSubDictionary)

  public:
    virtual const char * getDefBase() const  { return 0; }
  };

  class PDFEXPORT_TOOLKIT PDFExtGStateSubDictionary : public PDFSubDictionary
  {
    PDF_DECLARE_OBJECT(PDFExtGStateSubDictionary, PDFSubDictionary, kExtGStateSubDictionary)

  public:
    virtual const char * getDefBase() const  { return 0; }
  };


  class PDFEXPORT_TOOLKIT PDFXObjectSubDictionary : public PDFSubDictionary
  {
    PDF_DECLARE_OBJECT(PDFXObjectSubDictionary, PDFSubDictionary, kXObjectSubDictionary)

  public:
    virtual const char * getDefBase() const  { return 0; }
    virtual PDFXObjectFormPtr hasForm(const OdAnsiString& name) const;
    virtual void getForms(OdArray<PDFXObjectFormPtr>& forms);
  };

  typedef PDFSmartPtr<PDFPropertiesSubDictionary> PDFPropertiesSubDictionaryPtr;
  typedef PDFSmartPtr<PDFPatternSubDictionary> PDFPatternSubDictionaryPtr;
  typedef PDFSmartPtr<PDFShadingSubDictionary> PDFShadingSubDictionaryPtr;
  typedef PDFSmartPtr<PDFColorSpaceSubDictionary> PDFColorSpaceSubDictionaryPtr;
  typedef PDFSmartPtr<PDFExtGStateSubDictionary> PDFExtGStateSubDictionaryPtr;
  typedef PDFSmartPtr<PDFXObjectSubDictionary> PDFXObjectSubDictionaryPtr;

};

#endif //_PDFPROPERTIESDICTIONARY_INCLUDED_

