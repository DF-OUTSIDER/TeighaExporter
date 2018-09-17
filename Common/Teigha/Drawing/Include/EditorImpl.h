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




#ifndef   _ODEDITORIMPL_H_INCLUDED_
#define   _ODEDITORIMPL_H_INCLUDED_

#include "TD_PackPush.h"

#include "Editor.h"

/** \details

    <group OdRx_Classes> 
*/

#define RXEVENT_FIRE(method, inparams, params) \
inline void fire_##method inparams \
{ \
  TD_AUTOLOCK( m_mutex ); \
  OdArray<OdRxEventReactorPtr> reactors = m_reactors; \
  for (unsigned i = 0; i < reactors.size(); ++i) \
  { \
    if (m_reactors.contains(reactors[i]))\
      reactors[i]->method params; \
  } \
}

#define EDEVENT_FIRE(method, inparams, params) \
inline void fire_##method inparams \
{ \
  TD_AUTOLOCK( m_mutex ); \
  OdArray<OdRxEventReactorPtr> reactors = m_reactors; \
  for (unsigned i = 0; i < reactors.size(); ++i) \
  { \
    if (m_reactors.contains(reactors[i]))\
    {\
      OdEditorReactorPtr pReactor = OdEditorReactor::cast(reactors[i]); \
      if (pReactor.get()) pReactor->method params; \
    }\
  } \
}

/** \details
    This class defines the events that trigger OdRxEventReactor instances. 
    Corresponding C++ library: TD_Db
    <group OdRx_Classes> 
*/
class TOOLKIT_EXPORT OdRxEventImpl : public OdEditor
{
protected:
  OdArray<OdRxEventReactorPtr> m_reactors;
  OdMutex m_mutex;
public:
  OdRxEventImpl() {}
  ODRX_DECLARE_MEMBERS(OdRxEventImpl);
  
  void addReactor(OdRxEventReactor* pReactor);
  void removeReactor(OdRxEventReactor* pReactor);

  // notifiers
  RXEVENT_FIRE(dwgFileOpened, (OdDbDatabase* db, const OdString& filename),(db, filename))
  RXEVENT_FIRE(initialDwgFileOpenComplete, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(databaseConstructed, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(databaseToBeDestroyed, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(beginSave, (OdDbDatabase* db, const OdString& pIntendedName), (db, pIntendedName))
  RXEVENT_FIRE(saveComplete, (OdDbDatabase* db, const OdString& pActualName), (db, pActualName))
  RXEVENT_FIRE(abortSave, (OdDbDatabase* db), (db))
  // DXF In/Out Events.
  RXEVENT_FIRE(beginDxfIn, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(abortDxfIn, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(dxfInComplete, (OdDbDatabase* db), (db))
  //
  RXEVENT_FIRE(beginDxfOut, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(abortDxfOut, (OdDbDatabase* db), (db))
  RXEVENT_FIRE(dxfOutComplete, (OdDbDatabase* db), (db))
  // Insert Events.
  RXEVENT_FIRE(beginInsert, (OdDbDatabase* pTo, const OdString& pBlockName, OdDbDatabase* pFrom), (pTo, pBlockName, pFrom))
  RXEVENT_FIRE(beginInsert, (OdDbDatabase* pTo, const OdGeMatrix3d& xform, OdDbDatabase* pFrom), (pTo, xform, pFrom))
  RXEVENT_FIRE(otherInsert, (OdDbDatabase* pTo, OdDbIdMapping& idMap, OdDbDatabase* pFrom), (pTo, idMap, pFrom))
  RXEVENT_FIRE(abortInsert, (OdDbDatabase* pTo), (pTo))
  RXEVENT_FIRE(endInsert, (OdDbDatabase* pTo), (pTo))

  // Wblock Events.
  RXEVENT_FIRE(wblockNotice, (OdDbDatabase* pDb), (pDb))
  RXEVENT_FIRE(beginWblock, (OdDbDatabase* pTo, OdDbDatabase* pFrom, const OdGePoint3d& insertionPoint), (pTo, pFrom, insertionPoint))
  RXEVENT_FIRE(beginWblock, (OdDbDatabase* pTo, OdDbDatabase* pFrom, OdDbObjectId blockId), (pTo, pFrom, blockId))
  RXEVENT_FIRE(beginWblock, (OdDbDatabase* pTo, OdDbDatabase* pFrom), (pTo, pFrom))
  RXEVENT_FIRE(otherWblock, (OdDbDatabase* pTo, OdDbIdMapping& m, OdDbDatabase* pFrom), (pTo, m, pFrom))
  RXEVENT_FIRE(abortWblock, (OdDbDatabase* pTo), (pTo))
  RXEVENT_FIRE(endWblock, (OdDbDatabase* pTo), (pTo))
  RXEVENT_FIRE(beginWblockObjects, (OdDbDatabase* pDb, OdDbIdMapping& m), (pDb, m))

  // Deep Clone Events.
  RXEVENT_FIRE(beginDeepClone, (OdDbDatabase* pTo, OdDbIdMapping& m), (pTo, m))
  RXEVENT_FIRE(beginDeepCloneXlation, (OdDbIdMapping& m), (m))
  RXEVENT_FIRE(abortDeepClone, (OdDbIdMapping& m), (m))
  RXEVENT_FIRE(endDeepClone, (OdDbIdMapping& m), (m))

  // Partial Open Events.
  RXEVENT_FIRE(partialOpenNotice, (OdDbDatabase* pDb), (pDb))

  //
  // OdEditorReactor events.
  //
  // file events
  EDEVENT_FIRE(beginDwgOpen, (const OdString& filename), (filename))
  EDEVENT_FIRE(endDwgOpen,   (const OdString& filename), (filename))
  EDEVENT_FIRE(beginClose,   (OdDbDatabase* pDb), (pDb))

  // xref events
  EDEVENT_FIRE(beginAttach,  (OdDbDatabase* pToDb, const OdString& filename, OdDbDatabase* pFromDb), (pToDb, filename, pFromDb))
  EDEVENT_FIRE(otherAttach,  (OdDbDatabase* pToDb, OdDbDatabase* pFromDb), (pToDb, pFromDb))
  EDEVENT_FIRE(abortAttach,  (OdDbDatabase* pFromDb), (pFromDb))
  EDEVENT_FIRE(endAttach,    (OdDbDatabase* pToDb), (pToDb))
  EDEVENT_FIRE(redirected,   (OdDbObjectId newId, OdDbObjectId oldId), (newId, oldId))
  EDEVENT_FIRE(comandeered,  (OdDbDatabase* pToDb, OdDbObjectId id, OdDbDatabase* pFromDb), (pToDb, id, pFromDb))
  EDEVENT_FIRE(beginRestore, (OdDbDatabase* pToDb, const OdString& filename, OdDbDatabase* pFromDb), (pToDb, filename, pFromDb))
  EDEVENT_FIRE(abortRestore, (OdDbDatabase* pToDb), (pToDb))
  EDEVENT_FIRE(endRestore,   (OdDbDatabase* pToDb), (pToDb))

  // xref subcommand events
  EDEVENT_FIRE(xrefSubcommandBindItem,    (int activity, OdDbObjectId blockId), (activity, blockId))
  EDEVENT_FIRE(xrefSubcommandAttachItem,  (int activity, const OdString& xrefPath), (activity, xrefPath))
  EDEVENT_FIRE(xrefSubcommandOverlayItem, (int activity, const OdString& xrefPath), (activity, xrefPath))
  EDEVENT_FIRE(xrefSubcommandDetachItem,  (int activity, OdDbObjectId blockId), (activity, blockId))
  EDEVENT_FIRE(xrefSubcommandPathItem,    (int activity, OdDbObjectId blockId, const OdString& newPath), (activity, blockId, newPath))
  EDEVENT_FIRE(xrefSubcommandReloadItem,  (int activity, OdDbObjectId blockId), (activity, blockId))
  EDEVENT_FIRE(xrefSubcommandUnloadItem,  (int activity, OdDbObjectId blockId), (activity, blockId))

  // command undo events
  EDEVENT_FIRE(undoSubcommandAuto,   (int activity, bool state), (activity, state))
  EDEVENT_FIRE(undoSubcommandControl,(int activity, int option), (activity, option))
  EDEVENT_FIRE(undoSubcommandBegin,  (int activity), (activity))
  EDEVENT_FIRE(undoSubcommandEnd,    (int activity), (activity))
  EDEVENT_FIRE(undoSubcommandMark,   (int activity), (activity))
  EDEVENT_FIRE(undoSubcommandBack,   (int activity), (activity))
  EDEVENT_FIRE(undoSubcommandNumber, (int activity, int num), (activity, num))

  // pickfirst selection event
  EDEVENT_FIRE(pickfirstModified, (), ())

  // layout event
  EDEVENT_FIRE(layoutSwitched, (const OdString& newLayoutName), (newLayoutName))

  // window events
  EDEVENT_FIRE(docFrameMovedOrResized, (OdHWnd hwndDocFrame, bool moved), (hwndDocFrame, moved))
  EDEVENT_FIRE(mainFrameMovedOrResized, (OdHWnd hwndMainFrame, bool moved), (hwndMainFrame, moved))

  // mouse events
  EDEVENT_FIRE(beginDoubleClick, (const OdGePoint3d& clickPoint), (clickPoint))
  EDEVENT_FIRE(beginRightClick, (const OdGePoint3d& clickPoint), (clickPoint))

  // toolbar events
  EDEVENT_FIRE(toolbarBitmapSizeWillChange, (bool largeBitmaps), (largeBitmaps))
  EDEVENT_FIRE(toolbarBitmapSizeChanged, (bool largeBitmaps), (largeBitmaps))

  // partial open events
  EDEVENT_FIRE(objectsLazyLoaded, (const OdDbObjectIdArray& objectIds), (objectIds))

  // quit events
  EDEVENT_FIRE(beginQuit,       (), ())
  EDEVENT_FIRE(quitAborted,     (), ())
  EDEVENT_FIRE(quitWillStart,   (), ())

  // modeless operation events
  EDEVENT_FIRE(modelessOperationWillStart, (const OdString& contextString), (contextString))
  EDEVENT_FIRE(modelessOperationEnded, (const OdString& contextString), (contextString))

  // SysVar events
  EDEVENT_FIRE( sysVarChanged, (OdDbDatabase* pDb, const OdString& varName),(pDb, varName))
  EDEVENT_FIRE( sysVarWillChange, (OdDbDatabase* pDb, const OdString& varName), (pDb, varName))
};

typedef OdSmartPtr<OdRxEventImpl> OdRxEventImplPtr;

/** \details
    This class defines the events that trigger OdRxEventReactor instances. 
    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdEditorImpl : public OdRxEventImpl
{
public:
  OdEditorImpl() {}
  ODRX_DECLARE_MEMBERS(OdEditorImpl);
};

typedef OdSmartPtr<OdEditorImpl> OdEditorImplPtr;

#include "TD_PackPop.h"

#endif // _ODEDITORIMPL_H_INCLUDED_

