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
#include "RasterExportCmd.h"

#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseInclude.h"
#include "GiDefaultContext.h"
#include "Gi/GiRasterImage.h"
#include "Ge/GeExtents3d.h"
#include "AbstractViewPE.h"
#include "DbBaseHostAppServices.h"
#include "RxRasterServices.h"
#include "DynamicLinker.h"
#include "GetRasterFilePath.h"

namespace TD_RASTER_EXPORT {
ODCOLORREF _bmpout_cmd::getBackgroundColor(OdEdUserIO* )
{
  return ODRGB(0,0,0);
}

void _bmpout_cmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPE.isNull()) 
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  // Get device for Bitmap creation
  OdDbBaseHostAppServices* pAppSvs = pDbPE->appServices(pRxDb);
  OdGsDevicePtr pDevice = pAppSvs->gsBitmapDevice();
  if (pDevice.isNull())
    // No device available
    throw OdError(eNotApplicable);

  // Load RxRasterServices. Throw exception if failed
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME, false);

  int nWhatToExport = pIO->getKeyword(L"What to export? [ActiveView/Extents]", L"ActiveView Extents", 0);
  int nColorBits = pIO->getInt(L"BitPerPixel <24>:", OdEd::kInpDefault, 24);
  int xsize = pIO->getInt(L"Bitmap width <2000>:", OdEd::kInpDefault, 2000);
  int ysize = pIO->getInt(L"Bitmap height <2000>:", OdEd::kInpDefault, 2000);
  bool plotGeneration = (pIO->getKeyword(L"PlotGeneration [OFF/ON] ", L"OFF ON", 1) != 0);
  ODCOLORREF bg = getBackgroundColor(pIO); // palette background

  OdString sOutput = pDbPE->getFilename(pRxDb);
  int n = sOutput.reverseFind(L'.');
  if (n > 0)
    sOutput = sOutput.left(n);
  
  sOutput = getRasterFilePathToSave(pRasSvcs, pIO, sOutput, L"png");

  if (sOutput.isEmpty())
    return; // canceled


  OdGsDCRect gsRect(0, xsize, ysize, 0 );
  OdGiDefaultContextPtr pCtx = pDbPE->createGiContext(pRxDb);
  pDevice = pDbPE->setupActiveLayoutViews(pDevice, pCtx);

  switch (nWhatToExport)
  {
  case 0: // Active layout
    break;
  case 1: // Extents of active layout
    OdGeBoundBlock3d ext;
    pDbPE->zoomToExtents(gsRect, pDevice, pRxDb, ext);
    break;
  }
  
  pDevice->properties()->putAt(L"BitPerPixel", OdRxVariantValue(OdUInt32(nColorBits)));
  pCtx->setPlotGeneration(plotGeneration);
  pDbPE->setupPalette(pDevice, pCtx, 0, bg);

  pDevice->onSize(gsRect);
  pDevice->update();

  pRasSvcs->saveRasterImage(OdGiRasterImagePtr(pDevice->properties()->getAt(L"RasterImage")), 
                            sOutput);
}

ODCOLORREF _bmpoutbg_cmd::getBackgroundColor(OdEdUserIO* pIO)
{
//  urrently it seems there is no way to specify "database-independent" default
//  OdCmColor bgColor;
//  bgColor.setRGB(255, 255, 255);
//  bgColor = *(pIO->getCmColor(L"Specify backround color <255, 255, 255>:", OdEd::kInpDefault, &bgColor));
//  return ODRGB(bgColor.red(), bgColor.green(), bgColor.blue());
  try
  {
    OdSharedPtr<OdCmColorBase> pColor = pIO->getCmColor(L"Specify backround color <255, 255, 255>:", OdEd::kInpThrowEmpty);
    return ODRGBA(pColor->red(), pColor->green(), pColor->blue(), 255);
  }
  catch(const OdEdEmptyInput&)
  {}

  return ODRGB(255, 255, 255);
}
}
