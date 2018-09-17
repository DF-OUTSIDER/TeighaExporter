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
// PdfExportImplBase.cpp : implementation of the CPdfExportImplBase class
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
//#ifdef _WIN32
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#endif

#include "PdfExportCommon.h"
#include "PdfExportImplBase.h"

#include "Ge/GeExtents3d.h"
#include "OdTimeStamp.h"
#include "ColorMapping.h"
#include "2dSupport.h"
#include "Pdf2dExportDevice.h"
#include "Pdf2dExportView.h"
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
CPdfExportImplBase::CPdfExportImplBase()
{

}

CPdfExportImplBase::~CPdfExportImplBase()
{

}

void CPdfExportImplBase::applyLineweightToDcScale(PdfExportParamsHolder& params_holder)
{
  OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(params_holder.getParams().database())->currentLayout(params_holder.getParams().database());
  OdDbBaseLayoutPEPtr pLayoutPE(pLayout);

  if (pLayoutPE->printLineweights(pLayout))
  {
    OdGsView* pTo = m_pDevice->viewAt(0);
    pTo->setLineweightToDcScale(params_holder.getParams().getGeomDPI() / kMmPerInch * 0.01);
  }
}

void CPdfExportImplBase::setupPdfRenderDevices(PdfExportParamsHolder& params_holder, OdGiDefaultContextPtr pCtx, PDF2dExportDevice* pDevicePdf, OdDbStub* objectId /*= 0*/, OdGsDCRect* clipBox /*= 0*/)
{
  OdUInt32 extentsFlag = 0;
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kExactExtents, !GETBIT(params_holder.getParams().m_reserved1, 1));
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseViewExtents, params_holder.getParams().useViewExtents());
  bool bZ2E = (GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kZoomToExtentsMode) != 0);
  OdDbBaseDatabasePEPtr pDbPE(params_holder.getParams().database());

  for (int nDevice = 0; nDevice < pDevicePdf->numRenderDevices(); nDevice++)
  {
    OdGsDevice *pRenderDevice = pDevicePdf->renderDeviceAt(nDevice);
    pRenderDevice->setBackgroundColor(params_holder.getParams().background());
    pRenderDevice->setLogicalPalette(params_holder.getParams().palette(), 256);

    OdGsDevicePtr pDevice;
    if (objectId)
    {
      pDevice = pDbPE->setupLayoutView(pRenderDevice, pCtx, objectId);
    }
    else
    {
      pDevice = pDbPE->setupActiveLayoutViews(pRenderDevice, pCtx);
    }

    if (!objectId && !bZ2E)
    {
      pDbPE->applyLayoutSettings(*clipBox, pDevice, params_holder.getParams().database(), extentsFlag, params_holder.getParams().getGeomDPI());
    }
    else if (!objectId && bZ2E)
    {
      OdGeBoundBlock3d ext;//just a stub
      pDbPE->zoomToExtents(*clipBox, pDevice, params_holder.getParams().database(), ext, extentsFlag, 0);//Zero in first bit means the ExactExtents is true
    }

    pDevicePdf->setRenderLayoutHelperAt(nDevice, pDevice);
  }
}

void CPdfExportImplBase::setupPdfLayout(PdfExportParamsHolder& params_holder, OdGsPageParams& pPageParams, OdGiDefaultContextPtr pCtx, PDF2dExportDevice* pDevicePdf, OdDbStub* objectId /*= 0*/)
{
  OdDbBaseDatabasePEPtr pDbPE(params_holder.getParams().database());
  OdUInt32 extentsFlag = 0;
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kExactExtents, !GETBIT(params_holder.getParams().m_reserved1, 1));
  SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseViewExtents, params_holder.getParams().useViewExtents());
  bool bIncludeOffLayers = GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kIncludeOffLayers);

  ////////////////////////////////////////////////////////////////////////////
  //(maybe it is not necessary but it was applied before, so let's keep it to reduce benchmark issues)
  if (GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kZoomToExtentsMode) != 0)
  {
    pPageParams.set(OdRoundToLong(pPageParams.getPaperWidth()), OdRoundToLong(pPageParams.getPaperHeight()), pPageParams.getLeftMargin(), pPageParams.getRightMargin(), pPageParams.getTopMargin(), pPageParams.getBottomMargin());
  }
  ////////////////////////////////////////////////////////////////////////////

  pPageParams.scale((double)params_holder.getParams().getGeomDPI() / 72.);
  OdGsDCRect ResultClipBox(0, OdRoundToLong(pPageParams.getPaperWidth()), 0, OdRoundToLong(pPageParams.getPaperHeight()));
  pDevicePdf->setPaperBox(ResultClipBox);

  if (GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kZoomToExtentsMode) == 0) //!Z2E mode
  {
    pDbPE->applyLayoutSettings(ResultClipBox, m_pDevice, params_holder.getParams().database(), extentsFlag, params_holder.getParams().getGeomDPI());
    pPageParams.set(pPageParams.getPaperWidth(), pPageParams.getPaperHeight(), ResultClipBox.m_min.x,
      pPageParams.getPaperWidth() - ResultClipBox.m_max.x, pPageParams.getPaperHeight() - ResultClipBox.m_max.y, ResultClipBox.m_min.y);

    // Apply clip region to screen
    OdGePoint2dArray clipPoints;
    clipPoints.push_back(OdGePoint2d(ResultClipBox.m_min.x, ResultClipBox.m_max.y));
    clipPoints.push_back(OdGePoint2d(ResultClipBox.m_min.x, ResultClipBox.m_min.y));
    clipPoints.push_back(OdGePoint2d(ResultClipBox.m_max.x, ResultClipBox.m_min.y));
    clipPoints.push_back(OdGePoint2d(ResultClipBox.m_max.x, ResultClipBox.m_max.y));

    pDevicePdf->m_clipPoints = clipPoints;

    if GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kMeasuring)
    {
      setMeasuringViewport(params_holder, ResultClipBox);
    }
  }
  else //Z2E mode
  {
    // temporary enable Frozen layers to calculate valid extents (#16918)
    if (bIncludeOffLayers)
      params_holder.frozenLayers(true);

    ResultClipBox = OdGsDCRect(OdRoundToLong(pPageParams.getLeftMargin()), OdRoundToLong(pPageParams.getPaperWidth() - pPageParams.getRightMargin()),
      OdRoundToLong(pPageParams.getBottomMargin()), OdRoundToLong(pPageParams.getPaperHeight() - pPageParams.getTopMargin()));
    OdGeBoundBlock3d plotExtents; //plot or view extents depending on extents calculation mode
    pDbPE->zoomToExtents(ResultClipBox, m_pDevice, params_holder.getParams().database(), plotExtents, extentsFlag, objectId);//Zero in first bit means the ExactExtents is true

                                                                                                                              //////////////////////////////////////////////////////////////////////////
    OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(params_holder.getParams().database())->currentLayout(params_holder.getParams().database());
    OdDbBaseLayoutPEPtr pLayoutPE(pLayout);

    if (pLayoutPE->printLineweights(pLayout))
      pDevicePdf->m_LwToDcScale = (double)params_holder.getParams().getGeomDPI() / kMmPerInch * 0.01;
    //////////////////////////////////////////////////////////////////////////

    if GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kMeasuring)
    {
      setMeasuringViewport(params_holder, ResultClipBox, &plotExtents, &pPageParams);
    }

    if (bIncludeOffLayers)
      params_holder.frozenLayers(false);
  }
  setupPdfRenderDevices(params_holder, pCtx, pDevicePdf, objectId, &ResultClipBox);
}

PDFResultEx CPdfExportImplBase::base_run(PdfExportParamsHolder& paramsHolder, bool isExport2XObj)
{
    bool bIncludeOffLayers = GETBIT(paramsHolder.getParams().exportFlags(), PDFExportParams::kIncludeOffLayers);
    OdUInt32 extentsFlag = 0;
    SETBIT(extentsFlag, OdDbBaseDatabasePE::kExactExtents, !GETBIT(paramsHolder.getParams().m_reserved1, 1));
    SETBIT(extentsFlag, OdDbBaseDatabasePE::kUseViewExtents, paramsHolder.getParams().useViewExtents());

    OdUInt32 nLayoutCount = paramsHolder.getParams().layouts().size();

    OdDbBaseDatabasePEPtr pDbPE(paramsHolder.getParams().database());
    OdGiDefaultContextPtr pCtx = pDbPE->createGiContext(paramsHolder.getParams().database());

    PDFPageNodeDictionaryPtr pPageTree(paramsHolder.document().Root()->getPages());
    PDFDictionaryPtr pResDict = pPageTree->getResources();

    OdDbStub* objectId = 0;

    bool bDwgPaletteNeeded = false;
    OdArray<ODCOLORREF> palette_array;

    for (OdUInt32 nPageIndx = 0; nPageIndx < nLayoutCount; ++nPageIndx)
    {
      OdDbBaseDatabasePEPtr(paramsHolder.getParams().database())->setCurrentLayout(paramsHolder.getParams().database(), paramsHolder.getParams().layouts()[nPageIndx]);

      if (!isExport2XObj)
      {
        PDFArrayPtr pPages(pPageTree->Find(("Kids")));
        PDFPageDictionaryPtr pCurPage = pPages->getAt(nPageIndx);
        paramsHolder.setCurrentPage(pCurPage);
        paramsHolder.setContentCommands(PDFContentStreamPtr(pCurPage->getContents()->last()));
      }
      OdGsPageParams& pPageParams = paramsHolder.getParams().pageParams()[nPageIndx];

      OdGsDevicePtr pDevice = PDF2dExportDevice::createObject();
      PDF2dExportDevice* pDevicePdf = (PDF2dExportDevice*)pDevice.get();
      pDevicePdf->setUseHLR(GETBIT(paramsHolder.getParams().exportFlags(), PDFExportParams::kUseHLR));
      pDevicePdf->eraseAllViews();
      pDevicePdf->setContext(&paramsHolder); //set special context for pdf device

      pCtx->setPlotGeneration(true);
      pDbPE->loadPlotstyleTableForActiveLayout(pCtx, paramsHolder.getParams().database());

      pDevicePdf->setHatchDPI(paramsHolder.getParams().hatchDPI());
      pDevicePdf->setBwBitmapDPI(paramsHolder.getParams().bwImagesDPI());
      pDevicePdf->setColorBitmapDPI(paramsHolder.getParams().colorImagesDPI());
      pDevicePdf->setFrozenLayers(paramsHolder.getFrozenLayers());

      if (nPageIndx > 0)
      {
        if (!paramsHolder.getParams().layouts()[nPageIndx].compare(paramsHolder.getParams().layouts()[nPageIndx - 1])) //the same layout as previous
          objectId = pDbPE->getNextViewForActiveLayout(pCtx, objectId);
        else
          objectId = 0;
      }
      if ((NULL == paramsHolder.getParams().palette()) && (0 == nPageIndx))
      {
        // set palette if it is not init.
        ODCOLORREF Bgr = ODRGBA(ODGETRED(paramsHolder.getParams().background()), ODGETGREEN(paramsHolder.getParams().background()), ODGETBLUE(paramsHolder.getParams().background()), 255);
        pDbPE->setupPalette(pDevicePdf, pCtx, objectId, Bgr);
        int num_color;
        const ODCOLORREF* refColors = pDevicePdf->getLogicalPalette(num_color);
        if (256 != num_color)
          throw OdError(::eBadColorIndex);
        palette_array.insert(palette_array.begin(), refColors, refColors + num_color);
        paramsHolder.getParams().setPalette(palette_array.asArrayPtr());
     }
      else
      {
        pCtx->setPaletteBackground((ODCOLORREF)(paramsHolder.getParams().background()));
        pDevicePdf->setLogicalPalette(paramsHolder.getParams().palette(), 256);
      }
      pDevicePdf->setBackgroundColor(paramsHolder.getParams().background());


      if (!objectId)
      {
        m_pDevice = pDbPE->setupActiveLayoutViews(pDevicePdf, pCtx);
        objectId = pDbPE->getNextViewForActiveLayout(pCtx, objectId);
      }
      else
      {
        m_pDevice = pDbPE->setupLayoutView(pDevicePdf, pCtx, objectId);
      }
      // !paramsHolder.getParams().enablePRC will disable rendering of shaded viewports as bitmap
      pDevicePdf->setRenderDevice(0, paramsHolder.getParams().getPRCMode() == PDFExportParams::kDisabled);//!m_Params.enablePRC);

                                                                                                          // some initialization
      applyLineweightToDcScale(paramsHolder);

      try
      {
        setupPdfLayout(paramsHolder, pPageParams, pCtx, pDevicePdf, objectId);
      }
      catch (OdError&)
      {
        return exCannotOpenOverallVport;
      }

      pDevicePdf->enableRecording();
      m_pDevice->update(0);
      pDevicePdf->enableRecording(false);
      if (pDevicePdf->isDWGPaletteNeeded())
      {
        bDwgPaletteNeeded = true;
      }
      pDevicePdf->m_LwToDcScale = 0.;
    }//for(OdUInt32 nPageIndx = 0; nPageIndx < nLayoutCount; ++nPageIndx)

     //put indexed Dwg palette
    if (bDwgPaletteNeeded)
      PDFAUX::createIndexedDWGPalette(pResDict, paramsHolder.getParams().palette(), 256, paramsHolder.document());

    if (0 != palette_array.size())
      paramsHolder.getParams().setPalette(NULL);
    return ::exOk;
}

void CPdfExportImplBase::setMeasuringViewport(PdfExportParamsHolder& params_holder, const OdGsDCRect& rect, const OdGeBoundBlock3d* ext, const OdGsPageParams* pPageParams)
{
  OdRxObjectPtr pLayoutPtr = OdDbBaseDatabasePEPtr(params_holder.getParams().database())->currentLayout(params_holder.getParams().database());
  OdDbBaseLayoutPEPtr pLayout(pLayoutPtr);
  if (!pLayout.isNull() && !pLayoutPtr.isNull())
  {
    double measure_scale = 0.;
    if (GETBIT(params_holder.getParams().exportFlags(), PDFExportParams::kZoomToExtentsMode) && ext && pPageParams)
    {
      OdGsPageParams tmpParams(*pPageParams);
      tmpParams.scale(kMmPerInch / (double)params_holder.getParams().getGeomDPI());
      OdGePoint3d pMin = ext->minPoint();
      OdGePoint3d pMax = ext->maxPoint();
      double m_dRealWorldUnits = 1.;
      double m_dDrawingUnits = odmax(fabs(pMax.x - pMin.x) / OdRoundToLong(tmpParams.getPaperWidth()), fabs(pMax.y - pMin.y) / OdRoundToLong(tmpParams.getPaperHeight()));
      measure_scale = m_dRealWorldUnits / m_dDrawingUnits;
    }
    else
    {
      if (pLayout->useStandardScale(pLayoutPtr))
        pLayout->getStdScale(pLayoutPtr, measure_scale);
      else
      {
        double numerator = 0., denominator = 0.;
        pLayout->getCustomPrintScale(pLayoutPtr, numerator, denominator);
        if (denominator)
          measure_scale = numerator / denominator;
      }
    }
    if (measure_scale)
    {
      double plotUnitPerInch = 25.4;
      if (pLayout->plotPaperUnits(pLayoutPtr) == 0 /*kInches*/)
        plotUnitPerInch = 1.;
      measure_scale = (plotUnitPerInch / 72./*plot unit per dot*/) / measure_scale;

      OdGsDCRect clipBox = rect;
      clipBox.m_min.x *= (72. / (double)params_holder.getParams().getGeomDPI());
      clipBox.m_min.y *= (72. / (double)params_holder.getParams().getGeomDPI());
      clipBox.m_max.x *= (72. / (double)params_holder.getParams().getGeomDPI());
      clipBox.m_max.y *= (72. / (double)params_holder.getParams().getGeomDPI());
      PDFAUX::createMeasuringViewPort(clipBox, params_holder.document(), params_holder.CurrentPage(), measure_scale);
    }
  }
}

}
