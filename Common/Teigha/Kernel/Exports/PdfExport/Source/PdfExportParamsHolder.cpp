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
// PdfExportParamsHolder.cpp : implementation of the PdfExportParamsHolder class
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
#include "PdfExportParamsHolder.h"
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
PdfExportParamsHolder::PdfExportParamsHolder(PDFDocument& document, PDFFontOptimizer& font_optimizer, PDFType3Optimizer& type_3_optimizer)
  :m_Document(document)
  ,m_FontOptimizer(font_optimizer)
  , m_Type3Optimizer(type_3_optimizer)
  ,m_pContentCommands(0)
  ,m_pCurPage(0)
{
}

PdfExportParamsHolder::~PdfExportParamsHolder()
{

}

PDFResultEx PdfExportParamsHolder::getPageParamsFromLayout(OdRxObject* pLayoutPtr, OdGsPageParams &pageParams)
{
  OdDbBaseLayoutPEPtr pLayout(pLayoutPtr);
  if (pLayout->paperName(pLayoutPtr).isEmpty())
    return exCannotRestorePaperFromLayout;

  // Get printer paper info
  double dPrinterWidth, dPrinterHeight;
  pLayout->getPaperSize(pLayoutPtr, dPrinterWidth, dPrinterHeight); //in mm
  
  double dLeftMargin   = pLayout->getLeftMargin(pLayoutPtr);  // in mm
  double dRightMargin  = pLayout->getRightMargin(pLayoutPtr); // in mm
  double dTopMargin    = pLayout->getTopMargin(pLayoutPtr);   // in mm
  double dBottomMargin = pLayout->getBottomMargin(pLayoutPtr);// in mm

  if (pLayout->plotRotation(pLayoutPtr) == OdDbBaseLayoutPE::k90degrees || pLayout->plotRotation(pLayoutPtr) == OdDbBaseLayoutPE::k270degrees)
  {
    pageParams.set( dPrinterHeight, dPrinterWidth, dBottomMargin, dTopMargin, dLeftMargin, dRightMargin );
  }
  else
  {
    pageParams.set( dPrinterWidth, dPrinterHeight, dLeftMargin, dRightMargin, dBottomMargin, dTopMargin );
  }

  return exOk;
}

void PdfExportParamsHolder::frozenLayers(bool showUp)
{
  FrozenLayerMap::iterator pFrozenLayer = m_FrozenLayers.begin();
  FrozenLayerMap::iterator pEnd = m_FrozenLayers.end();

  while(pFrozenLayer != pEnd)
  {
    OdRxObjectPtr pLayer = pFrozenLayer->second;
    OdDbBaseLayerPEPtr(pLayer)->setIsFrozen(pLayer, showUp);
    ++pFrozenLayer;
  }
}

void PdfExportParamsHolder::showAllLayers()
{
  m_OffLayers.clear();
  m_FrozenLayers.clear();

  for (OdRxIteratorPtr it = OdDbBaseDatabasePEPtr(m_Params.database())->layers(m_Params.database()); !it->done(); it->next())
  {
    OdRxObjectPtr pLayer = it->object();
    OdDbBaseLayerPEPtr lpe(pLayer);

    if (lpe->isOff(pLayer))
    {
      lpe->setIsOff(pLayer, false);
      m_OffLayers.append( lpe->name(pLayer) );
    }
    if (lpe->isFrozen(pLayer))
    {
      lpe->setIsFrozen(pLayer, false);
      m_FrozenLayers[lpe->name(pLayer)] = pLayer;
      m_OffLayers.append( lpe->name(pLayer) );
    }
  }
}

PDFResultEx PdfExportParamsHolder::init(const PDFExportParams &pParams)
{
  // Prepare input parameters for using in CPdfExportImpl
  m_Params = pParams;

  if (m_Params.getPRCMode() != PDFExportParams::kDisabled && GETBIT(m_Params.exportFlags(), PDFExportParams::kUseHLR))
  {
    return exUseHLRConflictedWithEnablePRC;
  }

  if (m_Params.version() < PDFExportParams::kPDFv1_4)
    m_Params.setVersion(PDFExportParams::kPDFv1_4);

  if (m_Params.version() < PDFExportParams::kPDFv1_6 && GETBIT(m_Params.exportFlags(), PDFExportParams::kMeasuring))
    m_Params.setVersion(PDFExportParams::kPDFv1_6);

  if (m_Params.version() < PDFExportParams::kPDFv1_5 && (GETBIT(m_Params.exportFlags(), PDFExportParams::kEnableLayers) || GETBIT(m_Params.exportFlags(), PDFExportParams::kIncludeOffLayers)))
    m_Params.setVersion(PDFExportParams::kPDFv1_5);

  if (m_Params.getPRCMode() != PDFExportParams::kDisabled)
  {
    if (m_Params.archived() != PDFExportParams::kPDFA_None)
    {
      return exInternalError;//Annotation types not defined in ISO 32000-1 shall not be permitted. Additionally, the 3D, Sound, Screen and Movie types shall not be permitted.
    }
    if (m_Params.version() < PDFExportParams::kPDFv1_6)
      m_Params.setVersion(PDFExportParams::kPDFv1_6);

    OdRxClassPtr pService = getPdfExportService();
    if (pService.isNull())
    {
      return exPdfExportServiceMissed;
    }
  }

  if (m_Params.database() == 0)
    return exNullDatabase;

  if (m_Params.output().isNull())
    return exNullOutputStream;

  if (m_Params.hatchDPI() < 1)
    return exInvalidImageDPI;

  // Get active layout, if m_Params.layouts is empty.
  if (m_Params.layouts().isEmpty())
  {
    OdRxObjectPtr activeLayout = OdDbBaseDatabasePEPtr(m_Params.database())->currentLayout(m_Params.database());
    m_Params.layouts().push_back( OdDbBaseLayoutPEPtr(activeLayout)->name(activeLayout) );
    if (m_Params.pageParams().isEmpty())
    {
      OdGsPageParams params;
      getPageParamsFromLayout( activeLayout, params );
      m_Params.pageParams().push_back( params );
    }
  }
  else
  {
    // layout name validation
    for(OdUInt32 nPageIndx = 0; nPageIndx < m_Params.layouts().size(); ++nPageIndx)
    {
      OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(m_Params.database())->getLayout( m_Params.database(), m_Params.layouts()[nPageIndx] );
      if (pLayout.isNull())
        return exLayoutNotFound;
    }
  }

  // calculate paper box
  if (GETBIT(m_Params.exportFlags(), PDFExportParams::kZoomToExtentsMode) == 0)
  {
    m_Params.pageParams().resize( m_Params.layouts().size() );
    // try to get paper sizes from layout settings

    for(OdUInt32 nPageIndx = 0; nPageIndx < m_Params.layouts().size(); ++nPageIndx)
    {
      OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(m_Params.database())->getLayout( m_Params.database(), m_Params.layouts()[nPageIndx] );
      if (getPageParamsFromLayout(pLayout, m_Params.pageParams()[nPageIndx]) != exOk)
        return exCannotRestorePaperFromLayout;
      
      m_Params.pageParams()[nPageIndx].scale(MM2INCH72(1.));
    }

  }
  else // zoom to extents mode, in that case user must define paper sizes.
  {
    OdUInt32 nPages   = m_Params.pageParams().size();
    OdUInt32 nLayouts = m_Params.layouts().size();

    if (nPages != 1 && nLayouts != nPages)
      return exWrongNumberOfPages;

    // validation of page parameters is in PDFPageParams::set()
    // here is just scaling to 1/72 inch (pdf point size)
    for(OdUInt32 f=0; f<nLayouts; ++f)
    {
      m_Params.pageParams()[f].scale(MM2INCH72(1.));
    }

    if (nLayouts != nPages)
    {
      // resize it with coping of first element
      m_Params.pageParams().resize(nLayouts, m_Params.pageParams().first());
    }
  }

  // Layers functionality
  if (!GETBIT(m_Params.exportFlags(), PDFExportParams::kEnableLayers))
    m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kIncludeOffLayers));

  if (GETBIT(m_Params.exportFlags(), PDFExportParams::kIncludeOffLayers))
  {
    showAllLayers();
  }

  if (m_Params.archived() != PDFExportParams::kPDFA_None)
  {
    if (m_Params.archived() == PDFExportParams::kPDFA_2b)
    {
      if (m_Params.version() < PDFExportParams::kPDFv1_7)
        m_Params.setVersion(PDFExportParams::kPDFv1_7);
    }
    else if (m_Params.archived() == PDFExportParams::kPDFA_1b)
    {
      m_Params.setVersion(PDFExportParams::kPDFv1_4);
      //Check layers (Layers are not supported in PDF/A-1)
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kEnableLayers));
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kIncludeOffLayers));

      //Transparency (including merging colors) is not supported in PDF/A-1
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kMergeLines));

      //Measuring functionality is not supported in PDF/A-1
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kMeasuring));

      ////According to PDF 1.4 specs we must prevent strings (include stream parts) from being greater then 64KB
      m_Params.setDCTCompression(true);
      m_Params.setDCTQuality(40);
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() | PDFExportParams::kFlateCompression));
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kASCIIHexEncoding));

      ///Check paper size and resolution
      double max_coord = odmax(m_Params.pageParams()[0].getPaperWidth(), m_Params.pageParams()[0].getPaperHeight()) / (72. / (double)m_Params.getGeomDPI());
      if (max_coord > 32000.) //According to specs we must prevent real values (like coordinates) from being greater then 32767 so we have to reduce DPI with some reserve
      {
        double scale = max_coord / 32000.;
        OdUInt16 newDPI = (double)m_Params.getGeomDPI() / scale;
        if (newDPI > 200)
          newDPI = (newDPI - 100) - ((newDPI - 100) % 100); //another reserve
        if (newDPI < 72)
          newDPI = 72;
        m_Params.setGeomDPI(newDPI);
        if (m_Params.colorImagesDPI() > newDPI)
          m_Params.setColorImagesDPI(newDPI);
        if (m_Params.bwImagesDPI() > newDPI)
          m_Params.setBWImagesDPI(newDPI);
      }
    }

    if (!GETBIT(m_Params.exportFlags(), PDFExportParams::kTTFTextAsGeometry))
    {
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() | PDFExportParams::kEmbededTTF)); //only embedded fonts are supported for PDF/A
      m_Params.setExportFlags((PDFExportParams::PDFExportFlags)(m_Params.exportFlags() & ~PDFExportParams::kEmbededOptimizedTTF)); //there is a problem with optimized fonts yet
    }
  }

  PDFOCManager::clearLayersData();

  return ::exOk;
}

bool PdfExportParamsHolder::isLayerOriginallyVisible(const OdString &layerName) const
{
  OdUInt32 f;
  for(f=0; f<m_OffLayers.size(); ++f)
  {
    if (m_OffLayers[f] == layerName)
      return false;
  }

  OdString tmp = layerName.right(PDF_FROZEN_LAYER_SIZE);
  if (tmp == PDF_FROZEN_LAYER)
  {
    tmp = layerName.left(layerName.getLength() - PDF_FROZEN_LAYER_SIZE);
    if (m_FrozenLayers.find(tmp) != m_FrozenLayers.end())
    {
      return false;
    }
  }

  return true;
}

}
