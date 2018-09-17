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

#ifndef GFXSRV_OPS_H
#define GFXSRV_OPS_H


#include "OdaCommon.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DynamicLinker.h"
#include "DbBaseDatabase.h"
#include "TxDefs.h"
#include "DbGsManager.h"
#include "Gs/GsModel.h"
#include "DbLayout.h"
#include "DbBlockTableRecord.h"
#include "RxObjectImpl.h"
#include "Tf/TFSQL.h"
#include "MemoryStream.h"

extern OdString sInitializedGs;

using namespace Oda;
using namespace OdEd;

inline bool isAutoUpdateReactorAttached(OdEdCommandContext* ctx) {
  return !ctx->arbitraryData("AutoUpdateReactor").isNull();
}

inline void UpdateGs(OdEdCommandContext* ctx) {
  OdGsDevicePtr gsDev = Oda::arbVarSafe(ctx, sInitializedGs);
  gsDev->update();
}

inline OdGsDevicePtr setupDevice(OdRxObject* rxDb, OdGsDevice* gsDev, bool bEnableGsModel, ODCOLORREF palBg, OdGiDefaultContextPtr giCtx) {
  OdDbBaseDatabasePEPtr dbPx = rxDb;
  if (giCtx.isNull()) {
    giCtx = dbPx->createGiContext(rxDb);
    giCtx->enableGsModel(bEnableGsModel);
  }

  OdGsDevicePtr gsHelper = dbPx->setupActiveLayoutViews(gsDev, giCtx);
  dbPx->setupPalette(gsDev, giCtx, 0, palBg);

  return gsHelper;
}

inline OdString getCurrentLayoutName(OdEdCommandContext* ctx){
  typedef OdDbBaseDatabasePEPtr BaseDbPxPtr;
  typedef OdDbBaseLayoutPEPtr   BaseLytPxPtr;
  OdRxObjectPtr db = ctx->baseDatabase();
  BaseDbPxPtr dbpx = BaseDbPxPtr(db);
  OdRxObjectPtr curLayout = dbpx->currentLayout(db);
  if (curLayout)
    return BaseLytPxPtr(curLayout)->name(curLayout);
  return OdString(L"Model");
}

class GfxSrvcParams {
public:
  GfxSrvcParams() {
    sewtol = simpltol = 0.0;
    sortByWeight = false;
    dataLimit = 0xFFFFFFFF;
    chunkSz = 0xFFFFFFFF;
  }

  const OdString& outFileName() const { return outFName; }
  void setOutFileName(const OdString& val) { outFName = val; }
  OdUInt32 chunkSize() const { return chunkSz; }
  void setChunkSize(OdUInt32 val) { chunkSz = val; }
  OdUInt32 outputDataLimit() const { return dataLimit; }
  void setOutputDataLimit(OdUInt32 val) {
    dataLimit = val;
    if(val<0xFFFFFFFF)
      enableSorting(true);
  }

  double simplTol() const { return simpltol; }
  double sewTol() const { return sewtol; }
  void setSimplificationTolerances(double sewTol, double simplTol) {
    simpltol = simplTol; sewtol = sewTol;
  }

  OdString layoutName() const { return layout; }
  void setLayoutName(const OdString& name) { layout = name; }

  bool isSortingEnabled() const { return sortByWeight; }
  void enableSorting(bool val) { sortByWeight = val; }
  OdString cachePath() const { return pathToCache; }
  void setCachePath(const OdString& val) { pathToCache = val; }
private:
  OdString outFName;
  OdString pathToCache;
  OdUInt32 chunkSz;
  OdUInt32 dataLimit;
  double sewtol;
  double simpltol;
  OdString layout;
  bool sortByWeight;
};

OdGsDevicePtr newGfxSrv(const OdString& fmtCtx, OdEdCommandContext* ctx, OdGsDCRect& rect, bool autoupdate, GfxSrvcParams& params);



using namespace Oda;
typedef OdDbBaseDatabasePEPtr BaseDbPxPtr;
typedef OdDbBaseLayoutPEPtr   BaseLytPxPtr;


inline OdUInt32 layoutCacheRowId(OD::TFSQL* strg) {
  return strg->prepare(
    "SELECT layouts.rowid,settings.var,settings.val"
    " FROM layouts,settings WHERE settings.var='layout' AND layouts.name=settings.val")
    ->exe()->row().int32(0);
}
inline void cacheDeviceState(CMDCTX* ctx) {
  OdGsLayoutHelperPtr helper = OdGsLayoutHelper::cast(ctx->arbitraryData(sInitializedGs));
  OD::TFSQLP strg = OD::TFSQL::cast(ctx->arbitraryData("stateStorage"));
  if (helper.get() && strg.get()) {
    OdUInt32 layoutRow = layoutCacheRowId(strg);
    OD::TFSQL::BlobP buf = strg->openR("layouts", "state", layoutRow);
    helper->storeLayoutGsState(buf);
  }
}
#endif //GFXSRV_OPS_H
