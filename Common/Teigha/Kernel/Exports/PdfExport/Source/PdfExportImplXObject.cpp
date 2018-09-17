/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// PdfExportImplPublish.cpp : implementation of the CPdfExportImplXObject class
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
//#ifdef _WIN32
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#endif

#include "PdfExportCommon.h"

#include "Ge/GeExtents3d.h"
#include "OdTimeStamp.h"
#include "ColorMapping.h"
#include "2dSupport.h"
#include "Pdf2dExportDevice.h"
#include "Pdf2dExportView.h"
#include "PdfExportImplXObject.h"
#include "PdfPageNodeDictionary.h"
#include "PdfOCManager.h"
#include "Gs/GsPageParams.h"
#include "PdfAux.h"
#include "PdfLinearizer.h"

#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"

#include "PdfExportService.h"
#include "RxObject.h"
#include "AbstractViewPE.h"
#include "MemoryStream.h"

namespace TD_PDF_2D_EXPORT {

//***************************************************************************
// 
//***************************************************************************
 CPdfExportImplXObject::CPdfExportImplXObject(PDFPageDictionaryPtr& page, PDFXObjectFormPtr& xobject_form, PDFFontOptimizer& font_optimizer)
   :m_ParamsHolder(*page->document(), font_optimizer, m_Type3Optimizer)
   ,m_pCurrentPage(page)
   ,m_pXobjectForm(xobject_form)
{
   m_ParamsHolder.setCurrentPage(m_pCurrentPage);
   m_ParamsHolder.setContentCommands(m_pXobjectForm);
}

CPdfExportImplXObject::~CPdfExportImplXObject()
{

}

PDFResultEx CPdfExportImplXObject::init(const PDFExportParams &pParams)
{
  PDFResultEx res = m_ParamsHolder.init(pParams);
  if (m_ParamsHolder.getParams().pageParams().size() > 1)
    return (PDFResultEx)(exOdError | (eInvalidInput & 0xFFFF));
  return res;
}

PDFResultEx CPdfExportImplXObject::run()
{
  m_Type3Optimizer.clear();

  PDFResultEx result = base_run(m_ParamsHolder, true);
  if (::exOk != result)
    return result;

  m_Type3Optimizer.Optimize();

  PDFRectanglePtr rect = PDFRectangle::createObject(m_ParamsHolder.document());
  rect->set(0, 0, m_ParamsHolder.getParams().pageParams()[0].getPaperWidth(), m_ParamsHolder.getParams().pageParams()[0].getPaperHeight());
  m_pXobjectForm->dictionary()->setBBox(rect);

  return ::exOk;
}

}
