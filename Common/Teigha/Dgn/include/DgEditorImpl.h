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

#ifndef   _ODDGEDITORIMPL_H_INCLUDED_
#define   _ODDGEDITORIMPL_H_INCLUDED_

#include "TD_PackPush.h"
#include "DgEditor.h"

/** \details

    <group OdRx_Classes> 
*/

#define RXEVENT_FIRE(method, inparams, params) \
inline void fire_##method inparams \
{ \
  TD_AUTOLOCK( m_mutex ); \
  OdArray<OdDgRxEventReactorPtr> reactors = m_reactors; \
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
  OdArray<OdDgRxEventReactorPtr> reactors = m_reactors; \
  for (unsigned i = 0; i < reactors.size(); ++i) \
  { \
    if (m_reactors.contains(reactors[i]))\
    {\
      OdDgEditorReactorPtr pReactor = OdDgEditorReactor::cast(reactors[i]); \
      if (pReactor.get()) pReactor->method params; \
    }\
  } \
}

/** \details
    This class defines the events that trigger OdDgRxEventReactor instances. 
    Corresponding C++ library: TD_Db
    <group OdRx_Classes> 
*/
class TG_EXPORT OdDgRxEventImpl : public OdDgEditor
{
protected:
  OdArray<OdDgRxEventReactorPtr> m_reactors;
  OdMutex m_mutex;
public:
  OdDgRxEventImpl() {}
  ODRX_DECLARE_MEMBERS(OdDgRxEventImpl);
  
  void addReactor(OdDgRxEventReactor* pReactor);
  void removeReactor(OdDgRxEventReactor* pReactor);

  // notifiers
  RXEVENT_FIRE(dgnFileOpened, (OdDgDatabase* db, const OdString& filename),(db, filename))
  RXEVENT_FIRE(initialDgnFileOpenComplete, (OdDgDatabase* db), (db))
  RXEVENT_FIRE(databaseConstructed, (OdDgDatabase* db), (db))
  RXEVENT_FIRE(databaseToBeDestroyed, (OdDgDatabase* db), (db))
  RXEVENT_FIRE(beginSave, (OdDgDatabase* db, const OdString& pIntendedName), (db, pIntendedName))
  RXEVENT_FIRE(saveComplete, (OdDgDatabase* db, const OdString& pActualName), (db, pActualName))
  RXEVENT_FIRE(abortSave, (OdDgDatabase* db), (db))
  // Insert Events.
  RXEVENT_FIRE(beginInsert, (OdDgDatabase* pTo, const OdString& pBlockName, OdDgDatabase* pFrom), (pTo, pBlockName, pFrom))
  RXEVENT_FIRE(beginInsert, (OdDgDatabase* pTo, const OdGeMatrix3d& xform, OdDgDatabase* pFrom), (pTo, xform, pFrom))
  RXEVENT_FIRE(otherInsert, (OdDgDatabase* pTo, OdDgIdMapping& idMap, OdDgDatabase* pFrom), (pTo, idMap, pFrom))
  RXEVENT_FIRE(abortInsert, (OdDgDatabase* pTo), (pTo))
  RXEVENT_FIRE(endInsert, (OdDgDatabase* pTo), (pTo))

  // Wblock Events.
  RXEVENT_FIRE(wblockNotice, (OdDgDatabase* pDb), (pDb))
  RXEVENT_FIRE(beginWblock, (OdDgDatabase* pTo, OdDgDatabase* pFrom, const OdGePoint3d& insertionPoint), (pTo, pFrom, insertionPoint))
  RXEVENT_FIRE(beginWblock, (OdDgDatabase* pTo, OdDgDatabase* pFrom, OdDgElementId blockId), (pTo, pFrom, blockId))
  RXEVENT_FIRE(beginWblock, (OdDgDatabase* pTo, OdDgDatabase* pFrom), (pTo, pFrom))
  RXEVENT_FIRE(otherWblock, (OdDgDatabase* pTo, OdDgIdMapping& m, OdDgDatabase* pFrom), (pTo, m, pFrom))
  RXEVENT_FIRE(abortWblock, (OdDgDatabase* pTo), (pTo))
  RXEVENT_FIRE(endWblock, (OdDgDatabase* pTo), (pTo))
  RXEVENT_FIRE(beginWblockObjects, (OdDgDatabase* pDb, OdDgIdMapping& m), (pDb, m))

  // Deep Clone Events.
  RXEVENT_FIRE(beginDeepClone, (OdDgDatabase* pTo, OdDgIdMapping& m), (pTo, m))
  RXEVENT_FIRE(beginDeepCloneXlation, (OdDgIdMapping& m), (m))
  RXEVENT_FIRE(abortDeepClone, (OdDgIdMapping& m), (m))
  RXEVENT_FIRE(endDeepClone, (OdDgIdMapping& m), (m))

  // Partial Open Events.
  RXEVENT_FIRE(partialOpenNotice, (OdDgDatabase* pDb), (pDb))

  //
  // OdEditorReactor events.
  //
  // file events
  EDEVENT_FIRE(beginDgnOpen, (const OdString& filename), (filename))
  EDEVENT_FIRE(endDgnOpen,   (const OdString& filename), (filename))
  EDEVENT_FIRE(beginClose,   (OdDgDatabase* pDb), (pDb))

  // xref events
  EDEVENT_FIRE(beginAttach,  (OdDgDatabase* pToDb, const OdString& filename, OdDgDatabase* pFromDb), (pToDb, filename, pFromDb))
  EDEVENT_FIRE(otherAttach,  (OdDgDatabase* pToDb, OdDgDatabase* pFromDb), (pToDb, pFromDb))
  EDEVENT_FIRE(abortAttach,  (OdDgDatabase* pFromDb), (pFromDb))
  EDEVENT_FIRE(endAttach,    (OdDgDatabase* pToDb), (pToDb))
  EDEVENT_FIRE(redirected,   (OdDgElementId newId, OdDgElementId oldId), (newId, oldId))
  EDEVENT_FIRE(comandeered,  (OdDgDatabase* pToDb, OdDgElementId id, OdDgDatabase* pFromDb), (pToDb, id, pFromDb))
  EDEVENT_FIRE(beginRestore, (OdDgDatabase* pToDb, const OdString& filename, OdDgDatabase* pFromDb), (pToDb, filename, pFromDb))
  EDEVENT_FIRE(abortRestore, (OdDgDatabase* pToDb), (pToDb))
  EDEVENT_FIRE(endRestore,   (OdDgDatabase* pToDb), (pToDb))

  // xref subcommand events
  EDEVENT_FIRE(xrefSubcommandBindItem,    (int activity, OdDgElementId blockId), (activity, blockId))
  EDEVENT_FIRE(xrefSubcommandAttachItem,  (int activity, const OdString& xrefPath), (activity, xrefPath))
  EDEVENT_FIRE(xrefSubcommandOverlayItem, (int activity, const OdString& xrefPath), (activity, xrefPath))
  EDEVENT_FIRE(xrefSubcommandDetachItem,  (int activity, OdDgElementId blockId), (activity, blockId))
  EDEVENT_FIRE(xrefSubcommandPathItem,    (int activity, OdDgElementId blockId, const OdString& newPath), (activity, blockId, newPath))
  EDEVENT_FIRE(xrefSubcommandReloadItem,  (int activity, OdDgElementId blockId), (activity, blockId))
  EDEVENT_FIRE(xrefSubcommandUnloadItem,  (int activity, OdDgElementId blockId), (activity, blockId))

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
  EDEVENT_FIRE(objectsLazyLoaded, (const OdDgElementIdArray& objectIds), (objectIds))

  // quit events
  EDEVENT_FIRE(beginQuit,       (), ())
  EDEVENT_FIRE(quitAborted,     (), ())
  EDEVENT_FIRE(quitWillStart,   (), ())

  // modeless operation events
  EDEVENT_FIRE(modelessOperationWillStart, (const OdString& contextString), (contextString))
  EDEVENT_FIRE(modelessOperationEnded, (const OdString& contextString), (contextString))

  // SysVar events
  EDEVENT_FIRE( sysVarChanged, (OdDgDatabase* pDb, const OdString& varName),(pDb, varName))
  EDEVENT_FIRE( sysVarWillChange, (OdDgDatabase* pDb, const OdString& varName), (pDb, varName))
};

typedef OdSmartPtr<OdDgRxEventImpl> OdDgRxEventImplPtr;

/** \details
    This class defines the events that trigger OdDgRxEventReactor instances. 
    <group Other_Classes>
*/
class TG_EXPORT OdDgEditorImpl : public OdDgRxEventImpl
{
public:
  OdDgEditorImpl() {}
  ODRX_DECLARE_MEMBERS(OdDgEditorImpl);
};

typedef OdSmartPtr<OdDgEditorImpl> OdDgEditorImplPtr;

#include "TD_PackPop.h"

#endif // _ODDGEDITORIMPL_H_INCLUDED_

