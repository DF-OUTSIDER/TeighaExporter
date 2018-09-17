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


#ifndef _PDF_3D_RENDER_MODE_DICTIONARY_
#define _PDF_3D_RENDER_MODE_DICTIONARY_

#include "PdfStreamDictionary.h"
#include "PdfRectangle.h"
#include "PdfStream.h"
#include "PdfNumber.h"
#include "PdfBoolean.h"

namespace TD_PDF
{

  class PDFIStream;
  class PDFVersion;

  class PDFEXPORT_TOOLKIT PDF3dRenderModeDictionary : public PDFDictionary
  {
    PDF_DECLARE_OBJECT(PDF3dRenderModeDictionary, PDFDictionary, k3DRenderModeDictionary)

    RenderingMode m_Mode;

  protected:
    virtual void InitObject();

  public:
    virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
    void setSubtype(RenderingMode mode);
    RenderingMode getSubtype() const;

#include "PdfDictKeysDefs.h"
#include "Pdf3dRenderModeDictionaryKeys.h"
#include "PdfDictKeysUnDefs.h"

  };

  typedef PDFSmartPtr<PDF3dRenderModeDictionary> PDF3dRenderModeDictionaryPtr;
};

#endif //_PDF_3D_RENDER_MODE_DICTIONARY_
