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

#include "OdaCommon.h"
#include "DgDatabase.h"
#include "DgCommandContext.h"
#include "Ed/EdUserIO.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseInclude.h"
#include "DgGiContext.h"
#include "Gi/GiRasterImage.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "DynamicLinker.h"

#include "RasterSavingCmd.h"

//#include "ColorMapping.h"
//#include "Ge/GeExtents3d.h"
//#include "Ge/GePlane.h"
//#include "AbstractViewPE.h"
//#include "DbViewportTable.h"
//#include "DbViewportTableRecord.h"
//#include "DbLayout.h"
//#include "DbViewport.h"

#include "DgGsManager.h"
#include "RxRasterServices.h"

#include "TGVersion.h"
namespace TG_RASTER_SAVING {
ODCOLORREF _tg_bmpout_cmd::getBackgroundColor(OdEdUserIO* )
{
  return ODRGB(0,0,0);
}

void _tg_bmpout_cmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDgDatabasePtr pDb = pCmdCtx->baseDatabase();
  OdEdUserIO* pIO = pCmdCtx->userIO();
  
  OdString sPmpt;
#ifdef _TOOLKIT_IN_DLL_
  m_sDevice = pIO->getFilePath(OD_T("Enter Graphic System file name"),
                               OdEd::kGfpForOpen,
                               OD_T("Select Graphic System"),
                               OdString::kEmpty,
                               m_sDevice,
                               L"ODA Graphic System (*." VECTORIZATION_MODULE_EXTENSION_W L")|*." VECTORIZATION_MODULE_EXTENSION_W L"||"
                               );
  OdGsModulePtr pGs = odrxDynamicLinker()->loadModule(m_sDevice, false);
#else
  m_sDevice = pIO->getString(L"Enter Graphic System name <" OdWinBitmapModuleName L">:", OdEd::kGstDefault, OdWinBitmapModuleName);
  if (m_sDevice.isEmpty())
    m_sDevice = OdWinBitmapModuleName;
  OdGsModulePtr pGs = odrxDynamicLinker()->loadModule(m_sDevice, false);
#endif
  if (!pGs.get())
    return;
  
  int nWhatToExport = 0;
  //int nWhatToExport = pIO->getKeyword(L"What to export? [Active Layout/Extents/Block] <Active Layout>:", L"Active Extents Block", 0);

  //OdGsDevicePtr pDevice = pGs->createBitmapDevice();
  OdGsDevicePtr pBitmapDevice = pGs->createBitmapDevice();

  OdGiContextForDgDatabasePtr pCtx = OdGiContextForDgDatabase::createObject();
  pCtx->setDatabase(pDb);

//      if( !bUseMicrostationLineWeightStyle )
//      {
//        pCtx->setLineWeightConfiguration( OdGiContextForDgDatabase::kPointLineWeight, 1 );
//        pCtx->setLineWeightConfiguration( OdGiContextForDgDatabase::kLineCapStyle, (OdUInt32)(OdPs::kLesRound) );
//        pCtx->setLineWeightConfiguration( OdGiContextForDgDatabase::kLineJoinStyle, (OdUInt32)(OdPs::kLjsRound) );
//      }

  OdGsDevicePtr pDevice;
  switch (nWhatToExport)
  {
  case 0: // Active layout
      //pDevice = OdDbGsManager::setupActiveLayoutViews(pBitmapDevice, pCtx);

      OdDgElementId vectorizedViewId;
      OdDgViewGroupPtr pViewGroup = pDb->getActiveViewGroupId().openObject();
      if( pViewGroup.isNull() )
      {
        //  Some files can have invalid id for View Group. Try to get & use a valid (recommended) View Group object.
        pViewGroup = pDb->recommendActiveViewGroupId().openObject();
      }      
      if( !pViewGroup.isNull() )
      {
        OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
        for( ; !pIt->done(); pIt->step() )
        {
          OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject() );
          if( pView.get() && pView->getVisibleFlag() )
          {
            vectorizedViewId = pIt->item();
            break;
          }
        }
      }
      if( vectorizedViewId.isNull() )
      {
        return;
        //STD( _tcout ) << "Can not find an active view group or all its views are disabled\n";
        //throw OdError( eInvalidInput );
      }

      pCtx->setView( vectorizedViewId.openObject( OdDg::kForRead ) );

      pDevice = OdGsDeviceForDgModel::setupModelView(pDb->getActiveModelId(), vectorizedViewId, pBitmapDevice, pCtx);

    break;
  //case 1: // Extents of active layout
  //  {
  //    pDevice = OdDbGsManager::setupActiveLayoutViews(pBitmapDevice, pCtx);
  //    OdAbstractViewPEPtr pDeviceView;
  //    OdGsViewPtr pDV;
  //    OdRxObjectPtr pLayoutView;
  //    if (!pDb->getTILEMODE())
  //    { // Paper Space
  //      pDV = OdGsPaperLayoutHelperPtr(pDevice)->overallView();
  //      pDeviceView = pDV;
  //
  //      OdDbLayoutPtr pLayout = pDb->currentLayoutId().safeOpenObject();
  //      OdDbObjectId overallVpId = pLayout->overallVportId();
  //      OdDbViewportPtr pActiveVP = overallVpId.safeOpenObject();
  //      pLayoutView = pActiveVP;
  //    }
  //    else
  //    { // Model Space
  //      pDV = OdGsModelLayoutHelperPtr(pDevice)->activeView();
  //      pDeviceView = pDV;
  //
  //      OdDbViewportTablePtr pVPT = pDb->getViewportTableId().safeOpenObject();
  //      OdDbViewportTableRecordPtr pActiveVP = pVPT->getActiveViewportId().safeOpenObject();
  //      pLayoutView = pActiveVP;
  //    }
  //
  //    // Get extents from current layout
  //    OdGeBoundBlock3d bbox;
  //    OdAbstractViewPEPtr pLayoutViewPE(pLayoutView);
  //    pLayoutViewPE->plotExtents(pLayoutView, bbox);
  //    pDeviceView->zoomExtents(pDV, &bbox);
  //  }
  //  break;
  //
  //case 2: // Block
  //  {
  //    OdString sBlock = pIO->getString(OD_T("Enter block name to save:"), OdEd::kGstAllowSpaces);
  //    OdDbBlockTableRecordPtr pBlock = OdDbSymUtil::getBlockId(sBlock, pDb).safeOpenObject();
  //
  //    pBitmapDevice->setUserGiContext(pCtx);
  //
  //    OdGsViewPtr pView = pBitmapDevice->createView();
  //
  //    pDevice->addView(pView);
  //
  //    OdGeExtents3d ext;
  //    if(pBlock->getGeomExtents(ext)==eOk)
  //    {
  //      pView->add(pBlock, 0);
  //
  //      // World extents to view CS:
  //
  //      OdGePoint3d targ = pView->target();
  //      OdGeVector3d dirFromTarg = OdGeVector3d::kZAxis;
  //
  //      // set target to center of the scene, keep view direction:
  //      targ = ext.minPoint() + (ext.maxPoint() - ext.minPoint()) / 2.0;
  //      targ.transformBy(pView->viewingMatrix().invert());
  //
  //      double fw = ext.maxPoint().x - ext.minPoint().x;
  //      double fh = ext.maxPoint().y - ext.minPoint().y;
  //      pView->setView(targ + dirFromTarg, targ, pView->upVector(), fw * 1.02, fh * 1.02);
  //    }
  //    //else we can't initialize view properly so draw empty bmp
  //
  //    pView->setLineweightToDcScale(0.);
  //
  //    pView->setViewportBorderVisibility(false);
  //  }
  }
  
  int nColorBits = pIO->getInt(OD_T("BitPerPixel <24>"), OdEd::kInpDefault, 24);
  int xsize = pIO->getInt(OD_T("Bitmap width <2000>"), OdEd::kInpDefault, 2000);
  int ysize = pIO->getInt(OD_T("Bitmap height <2000>"), OdEd::kInpDefault, 2000);
  bool plotGeneration = (pIO->getKeyword(OD_T("PlotGeneration [OFF/ON] <ON>"), OD_T("OFF ON"), 1) != 0);
  ODCOLORREF bg = getBackgroundColor(pIO); // palette background

  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  ODA_ASSERT_ONCE(pRasSvcs.get())
  if (pRasSvcs.isNull())
    throw OdError(eNotApplicable);

  static OdString s_sPrompt = OD_T("Enter file name to save BMP:"),
                  s_sDialogCaption = OD_T("Save BMP"), 
                  s_sFilters,
                  s_sDefExt = OD_T("bmp"); 
  if (s_sFilters.isEmpty())
  {
    OdUInt32Array types = pRasSvcs->getRasterImageTypes();
    if (types.size())
    {
      for (unsigned index = 0; index < types.size(); index++)
      {
        OdUInt32 tp = types.getAt(index);
        OdString sFilterName,
        sExt = pRasSvcs->mapTypeToExtension(tp, &sFilterName);
        if (sExt.isEmpty())
          continue;
        sExt.makeLower();
        if (sFilterName.isEmpty())
        {
          sFilterName = sExt;
          sFilterName.makeUpper().remove(__OD_T('.'));
          sFilterName += OD_T(" (*") + sExt + OD_T(")");
        }

        s_sFilters += sFilterName + OD_T("|*") + sExt + OD_T("|");
      }

      s_sPrompt.replace(OD_T("BMP"), OD_T("image"));
      s_sDialogCaption.replace(OD_T("BMP"), OD_T("image"));
    }
    if (s_sFilters.isEmpty())
      s_sFilters = OD_T("Windows Bitmap Files (*.bmp)|*.bmp|");
    s_sFilters += OD_T("|");
  }

  m_sOutput = pIO->getFilePath(s_sPrompt, OdEd::kGfpForSave, s_sDialogCaption,
                               s_sDefExt, m_sOutput, s_sFilters);
  if (m_sOutput.isEmpty())
    return; // canceled

  // Can be processed on OdRxRasterServices side if format requires downsampling
  //int pos = m_sOutput.reverseFind(__OD_T('.'));
  //if (pos > 0)
  //{
  //  s_sDefExt = m_sOutput.mid(++pos);
  //  if (s_sDefExt.makeLower() == L"gif")
  //    nColorBits = 8; // only 256 colors are supported for GIF
  //}

//------------
//  pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(nColorBits)));
//  pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg, "device background"
//  pCtx->setPlotGeneration(plotGeneration);
//  pCtx->setPaletteBackground(bg);
//  const ODCOLORREF* palette = odcmAcadPalette(bg);
//  ODGSPALETTE pPalCpy;
//  pPalCpy.insert(pPalCpy.begin(), palette, palette+256);
//  pPalCpy[0] = bg;
//  pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
//  OdGsDCRect gsRect(0, xsize, ysize, 0 );
//  pDevice->onSize(gsRect);
//  if(pCtx->isPlotGeneration())
//    pDevice->setBackgroundColor(bg);
//  else
//    pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg, "device background"
//  pDevice->update();
//-------------
  pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(nColorBits)));
  pCtx->setPlotGeneration(plotGeneration);
  const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );
  ODGSPALETTE pPalCpy;
  pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
  OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();
  ODCOLORREF background = pModel->getBackground();
	  
	// Color with #255 always defines backround. The background of the active model must be considered in the device palette.
	pPalCpy[255] = background;
  // Note: This method should be called to resolve "white background issue" before setting device palette
  bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

  pCtx->setPaletteBackground( background );
  pDevice->setBackgroundColor( background );
  pDevice->setLogicalPalette( pPalCpy.asArrayPtr(), 256);

  //OdGsDCRect screenRect(OdGsDCPoint(0, 768), OdGsDCPoint(1024, 0));
  //pDevice->onSize(screenRect);
  OdGsDCRect gsRect(0, xsize, ysize, 0 );
  pDevice->onSize(gsRect);

  pDevice->update();

  //OdGiRasterImagePtr ptrImage = pDevice->properties()->getAt( OD_T("RasterImage") );
  pRasSvcs->saveRasterImage(OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage"))), 
                            m_sOutput);
}

ODCOLORREF _tg_bmpoutbg_cmd::getBackgroundColor(OdEdUserIO* pIO)
{
  int nR = pIO->getInt(OD_T("get background R component <255>"), OdEd::kInpDefault, 255 );
  int nG = pIO->getInt(OD_T("get background G component <255>"), OdEd::kInpDefault, 255 );
  int nB = pIO->getInt(OD_T("get background B component <255>"), OdEd::kInpDefault, 255 );

  return ODRGB(nR,nG,nB);
}
}
