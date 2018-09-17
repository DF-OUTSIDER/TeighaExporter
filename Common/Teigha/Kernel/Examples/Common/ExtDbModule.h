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
//
// ExtDbModule.h
//

#ifndef __EXTDB_MODULE__H__
#define __EXTDB_MODULE__H__

#include "RxModule.h"
#include "RxSystemServices.h"
#include "DbBaseHostAppServices.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizeDevice.h"
#include "Gs/GsDCRectArray.h"
#include "StringArray.h"
#include "SSet.h"
#include "DbGrip.h"
#include "Ed/EdCommandContext.h"
#include "OdUnitsFormatter.h"
#include "Ed/EdUserIO.h"
#include "TDVersion.h"
#include "DynamicLinker.h"

class ODRX_ABSTRACT OdExtDbModule : public OdRxModule
{
public:
  //ODRX_DECLARE_MEMBERS(OdExtDbModule);

  // init issues
  virtual void setBaseHostAppServices(OdDbBaseHostAppServices* pServices) = 0;
  virtual void initOdExtPE() = 0;
  virtual void uninitOdExtPE() = 0;
  virtual bool isValid() = 0; // to unload if using components are missing (false)
  
  virtual OdStringArray resetRegVariables() = 0; // set to default values

  virtual bool getPreviewBitmap(const OdString& fileName, 
                                class OdBinaryData& dataBmp) = 0; // out

  // load / save issues
  virtual OdRxObjectPtr readFile(const OdString& fileName, 
                                 Oda::FileShareMode shareMode = Oda::kShareDenyWrite) = 0;
  virtual OdRxObjectPtr createUninitializedDatabase() = 0;
  virtual bool loadToUninitializedDatabase(OdRxObjectPtr pRxDb, 
                                           const OdString& fileName, 
                                           Oda::FileShareMode shareMode = Oda::kShareDenyWrite) = 0;
  virtual bool writeFile(const OdString& fileName, OdEdCommandContext* ctx) = 0;
  virtual OdRxObject* getWrappedDatabase(OdRxObject* pRxDb) = 0; // pRxDb is a wrapper for TD_TvfDb now
  virtual OdString getFilename(OdRxObject* pRxDb) = 0;
  // Cloud command command context issues
  virtual class OdEdBaseIO* baseIO(OdEdCommandContext* ctx) = 0;
  virtual OdEdCommandContextPtr cloneCommandContext(OdEdCommandContext* ctx,
                                                    OdEdBaseIO* pIOStream = NULL, 
                                                    OdRxObject* pRxDatabase = NULL) = 0;
  // render issues
  // for Dgn active model(group) only
  virtual bool getVisibleLayoutViewIds(OdRxObject* pRxDatabase,
                                       OdDbStub*& idActiveLayoutView, // out
                                       OdDbStubPtrArray& idsLayoutViews, OdGsDCRectArray& rects) = 0;
  // for all models (return false to use standard way (via OdDbBaseDatabasePE & OdDbBaseLayoutPE))
  virtual bool getLayoutViewNamesAndIds(OdRxObject* pRxDatabase,
                                        OdStringArray& namesLayoutView,
                                        OdDbStubPtrArray& idsLayoutView,
                                        OdString* pActiveLayoutViewName = NULL,
                                        bool bOneForModel = true,
                                        OdGsDCRectArray* pRects = NULL) = 0;
  virtual OdGsDevicePtr createDevice(OdGsModulePtr pGs, OdRxObjectPtr pRxDatabase) = 0;
  virtual OdGsDevicePtr createBitmapDevice(OdGsModulePtr pGs, OdRxObjectPtr pRxDatabase) = 0;
  virtual OdGiDefaultContextPtr createGiContext(OdRxObject* pRxDb) = 0;
  virtual OdGsDevicePtr setupActiveLayoutViews(OdRxObject* pRxDb, OdGsDevice* pGsDevice, OdGiDefaultContext* pGiCtx,
                                               void* winId = NULL, long widthQWidget = 800, long heightQWidget = 600) = 0;
  virtual OdGsDevicePtr takeUnderlyingDeviceAndViewports(OdGsDevicePtr pGsDevice,
                                                         OdArray<OdGsViewPtr>& viewportsToDeleteAfterUpdate) = 0; // out
  virtual void enableGsModel(OdGiDefaultContext* pCtx, bool bGsCache) = 0;
  virtual bool getModelBackground(OdRxObject* pRxDatabase, ODCOLORREF& clrBackground, 
                                                           ODGSPALETTE* pPalette = NULL) = 0;
  virtual bool setModelBackground(OdRxObject* pRxDatabase, ODCOLORREF clrBackground) = 0;
  virtual bool argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm) = 0;

  // zoom issues
  virtual OdGsView* getActiveGsView(OdGsDevice* pGsDevice) = 0;
  virtual OdGsModel* getGsModel(OdGsDevice* pGsDevice) = 0;
  virtual bool getUcsPlane(OdRxObject* pRxDatabase, OdGsView* pGsView, class OdGePlane& plane, 
                           const OdGeVector3d* pvHintDirToIntersect = NULL) = 0;
  virtual bool setGsViewToDb(OdRxObject* pRxDatabase, OdGsView* pGsView) = 0;

  virtual OdUnitsFormatter* getFormatter(OdRxObject* pRxDatabase) = 0;

  // selection issues
  virtual OdSelectionSetPtr createSelectionSet(OdRxObject* pRxDatabase) = 0;
  virtual OdSelectionSetPtr select(OdGsView* gsView,
                                   int nPoints,
                                   const OdGePoint3d* wcsPts,
                                   OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
                                   OdDbVisualSelection::SubentSelectionMode sm = OdDbVisualSelection::kDisableSubents,
                                   const OdRxObject* pFilter = NULL) = 0;
  virtual void getVisibleAllIds(OdRxObject* pRxDatabase, OdDbStubPtrArray& ids) = 0;
  virtual OdDbStub* getVisibleLastId(OdRxObject* pRxDatabase) = 0;
  virtual void highlight(OdGsView* gsView, bool bValue,
                         OdDbStub* id, const OdDbBaseFullSubentPath* pPath) = 0;
  //
  virtual OdGiDrawablePtr cloneEntity(OdDbStub* id) = 0;
  virtual OdRxObjectPtr openObject(OdDbStub* id, bool isForWriteMode = false) = 0;
  // alternative to OdDbGripPointsPE methods
  // possible TODO redesign OdDbGripPointsPE(OdDgGripPointsPE...) to OdRxObject & use it instead next methods
  // next methods can be invoked for non-resident clone object (see also OdBaseGripManager) 
  virtual OdResult getGripPointsAtSubentPath(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdDbBaseFullSubentPath& path, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, const OdUInt32 bitflags) = 0;
  virtual OdResult getGripPoints(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, int bitFlags) = 0;
  virtual OdResult getGripPoints(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints) = 0;
  virtual OdResult moveGripPointsAtSubentPaths(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdDbBaseFullSubentPathArray& paths, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, OdUInt32 bitflags) = 0;
  virtual OdResult moveGripPointsAt(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags) = 0;
  virtual OdResult moveGripPointsAt(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset) = 0;
  virtual void subentGripStatus(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity) = 0;
  virtual void gripStatus(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDb::GripStat st) = 0;
  virtual void dragStatus(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDb::DragStat st) = 0;
  virtual bool isModel(OdRxObject* pRxDb, OdGiDrawable* pEntity) = 0;
  //
  virtual bool isErased(OdDbStub* id) = 0;
  virtual class OdDbHandle getElementHandle(OdDbStub* id) = 0;
  virtual OdString getElementClassName(OdDbStub* id) = 0;

  // undo/redo issues
  virtual bool startUndoRecord(OdRxObject* pRxDb) = 0;
  virtual bool blockUndoRecording(OdRxObject* pRxDb, bool bBegin) = 0;
  virtual bool isUndoBlockStarted(OdRxObject* pRxDb) = 0;
  virtual bool hasUndo(OdRxObject* pRxDb) = 0;
  virtual void undo(OdRxObject* pRxDb) = 0;
  virtual bool hasRedo(OdRxObject* pRxDb) = 0;
  virtual void redo(OdRxObject* pRxDb) = 0;

  // command issues
  virtual bool cmdErase(class OdEdCommandContext* pCmdCtx) = 0;

  // filters issues
  virtual bool isResponsible(OdRxObject* pRxDatabase) = 0;
  virtual OdStringArray getExtensions(enum OdEd::GetFilePathFlags flg = OdEd::kGfpForOpen, 
                                      OdRxObject* pRxDatabase = NULL,
                                      bool bWithExtSynonyms = false) = 0; // like "jpg" for "jpg","jpeg",...
  virtual OdString getFilterByExtension(const OdString& sExt, 
                                        int index = 0, // TODO for such formats as DXF (it may be binary or ascii)
                                        enum OdEd::GetFilePathFlags flg = OdEd::kGfpForOpen,
                                        OdStringArray* pExtSynonyms = NULL) = 0; 
  virtual OdString baseModuleName(bool bMnemoOnly = false);

  // import / export issues
  virtual OdStringArray getImportExportExtensions(OdRxObject* pRxDatabase,
                                                  bool isForImport = true,
                                                  bool bWithExtSynonyms = false) = 0; // TODO
  virtual OdString getImportExportFilterByExtension(const OdString& sExt,
                                                    int index = 0, // TODO for such formats as DXF (it may be binary or ascii)
                                                    bool isForImport = true,
                                                    OdStringArray* pExtSynonyms = NULL) = 0; // TODO
  virtual bool importFromFile(OdRxObject* pRxDatabase, const OdString& fileName,
                              ODCOLORREF& clrBackground) = 0; // in / out
  virtual bool importFromFile(OdEdCommandContext* pCmdCtx, const OdString& fileName,
                              ODCOLORREF& clrBackground) = 0; // in / out
  virtual bool exportToFile(OdRxObject* pRxDatabase, const OdString& fileName, 
                            int filterIndex, ODCOLORREF clrBackground) = 0;
  virtual bool exportToFile(OdEdCommandContext* pCmdCtx, const OdString& fileName,
                            int filterIndex, ODCOLORREF clrBackground) = 0;
};

inline OdString OdExtDbModule::baseModuleName(bool bMnemoOnly) // = false
{
  OdString sName = moduleName();
  sName.replace(L'\\', L'/');
  int pos = sName.reverseFind(L'/');
  if (pos >= 0)
    sName = sName.mid(++pos);
  OdString sVersionSuffix = L".tx"; // way for linux & others
#ifdef TD_DLL_VERSION_SUFFIX_STR
  sVersionSuffix = TD_DLL_VERSION_SUFFIX_STR;
#endif
  if (!sVersionSuffix.isEmpty() && (pos = sName.find(sVersionSuffix)) > 0)
    sName = sName.left(pos);
  if (!bMnemoOnly)
    sName += L".tx";
  else
    sName = sName.mid(3, sName.getLength() - 5).makeLower();
  return sName;
}

// way to use module without linking - impossible if TG is missing
// (via loading to OdRxModulePtr and next convertion)
inline OdExtDbModule* OdExtDbModule_cast(OdRxModule* pExtDb)
{
  return static_cast<OdExtDbModule*>(pExtDb);
}
inline OdExtDbModule* OdExtDbModule_cast(OdRxModulePtr pExtDb)
{
  return OdExtDbModule_cast(pExtDb.get());
}

inline bool isAcadExtension(const OdString& sExt)
{
  return sExt == L"dwg" || sExt == L"dxf" || sExt == L"dxb" || sExt == L"dws" || sExt == L"dwt";
}

inline OdString getExtDbModuleName(OdRxObject* pRxDatabase)
{
  //ODA_ASSERT_ONCE(pRxDatabase);
  if (!pRxDatabase)
    return OdString::kEmpty;

  // TODO
  // store database class name with modile name in system dictionary 
  // in implementation of readFile & createUninitializedDatabase.
  // and this data in this function

  OdRxClass* pClass = pRxDatabase->isA();
  OdString sClassName = pClass->name();
  ODA_ASSERT_ONCE(!sClassName.isEmpty());
  if (sClassName.isEmpty())
    return OdString::kEmpty;
  if (sClassName == L"OdDbDatabase")
    return OdString::kEmpty;
  if (sClassName == L"OdDgDatabase")
    return L"TD_DgnDb.tx";
  if (sClassName == L"OdBmDatabase")
    return L"TD_BimDb.tx";
  if (sClassName == L"OdDwfDatabase")
    return L"TD_DwfDb.tx";
  ODA_FAIL_ONCE(); // TODO
  return OdString::kEmpty;
}

// it is better use getIApp()->getExtDbModule(pRxDb) now 
// (see todo in getExtDbModuleName) if Qt is accessible
inline OdExtDbModule* getExtDbModule(OdRxObject* pRxDatabase)
{
  OdString sModuleName = getExtDbModuleName(pRxDatabase);
  if (sModuleName.isEmpty())
    return NULL;
  return OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
}

//#ifdef OD_QT_INTERFACES_H_
//// it may be usable in Qt modules only. And better call it directly
//inline OdStringArray getExtDbModuleNames(OdRxObject* pRxDb = NULL,
//                                         bool bNamesOnly = true)
//{
//  return getIApp()->getExtDbModuleNames(pRxDb, bNamesOnly);
//}
//inline OdExtDbModule* getExtDbModuleByExtension(const OdString& csExt)
//{
//  return getIApp()->getExtDbModuleByExtension(csExt);
//}
//#endif // OD_QT_INTERFACES_H_

#endif // __EXTDB_MODULE__H__
