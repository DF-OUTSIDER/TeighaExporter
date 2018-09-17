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




#ifndef   _ODED_H_INCLUDED_
#define   _ODED_H_INCLUDED_

#include "TD_PackPush.h"

#include "RxNames.h"
#include "RxEvent.h"
#include "OdArrayPreDef.h"
#include "DbObjectId.h"
#include "IdArrays.h"

#define ODED_EDITOR_OBJ OD_T("OdEditor")

class OdDbDatabase;


/** \details
    This class is the base class for custom classes that receive notification
    of OdEditor events.

    \remarks
    The default implementations of all methods in this class do nothing but return.
    
    \sa
    TD_Db
    
    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdEditorReactor : public OdRxEventReactor 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEditorReactor);

  /** \details
    Notification function called whenever a .dwg file is about to be opened.
    
    \param filename [in]  Name of the .dwg file.
    
    \remarks
    This function is called before the operation.
    
    \sa
    * dwgFileOpened
    * endDwgOpen
  */
  virtual void beginDwgOpen(
    const OdString& filename);
    
  /** \details
    Notification function called whenever a .dwg file has been opened.
    
    \param filename [in]  Name of the .dwg file.
    
    \remarks
    This function is called after the operation.
    
    \sa
    * beginDwgOpen
    * dwgFileOpened
  */
  virtual void endDwgOpen(
    const OdString& filename);
    
  /** \details
    Notification function called whenever an OdDbDatabase instance is about to be closed.
    
    \param pDb [in]  Pointer to the database being closed.
    
    \remarks
    This function is called before the operation.
  */
  virtual void beginClose(
    OdDbDatabase* pDb);
  /** \sa
    * beginDwgOpen
    * endDwgOpen
  */
  virtual void dwgFileOpened(
    OdDbDatabase* pDb, 
    const OdString& filename);
  
  /** \details
    Notification function called whenever an Xref database is about 
    to be attached to a host database.
    
    \param pToDb [in]  Host database.
    \param pFromDb [in]  Xref database.
    \param filename [in]  Xref filename.
    
    \remarks
    This function is called before the operation.
    
    \remarks
    filename may not have a path or extension.
    
    \sa
    * abortAttach
    * endAttach  
    * otherAttach
  */
  virtual void beginAttach(
    OdDbDatabase* pToDb, 
    const OdString& filename, 
    OdDbDatabase* pFromDb);
    
  /** \details
    Notification function called whenever an Xref database has been attached to a host database.
    
    \param pToDb [in]  Host database.
    \param pFromDb [in]  Xref database.
    
    \remarks
    This function is called after the operation, and is sent just after
    beginDeepCloneXlation.
    
    \sa
    * abortAttach
    * beginAttach
    * endAttach  
  */
  virtual void otherAttach(
    OdDbDatabase* pToDb, 
    OdDbDatabase* pFromDb);
    
  /** \details
    Notification function called whenever the attachment of an Xref database has failed.
    
    \param pToDb [in]  Host database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * beginAttach
    * endAttach  
    * otherAttach
  */
  virtual void abortAttach(
    OdDbDatabase* pFromDb);
    
  /** \details
    Notification function called whenever the attachment of an Xref database has succeeded.
    
    \param pToDb [in]  Host database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * abortAttach
    * beginAttach
    * otherAttach
  */
  virtual void endAttach(
    OdDbDatabase* pToDb);
    
  /** \details
    Notification function called whenever an Object ID in an Xref database has been
    modified to point at an associated object in a host database. 
    
    \param oldId [in]  Object Id in the Xref database.
    \param neId [in]  Object Id in the host database.
    
    \remarks
    This function is called after the operation.
    
    Redirection is used to implement VISRETAIN.
  */
  virtual void redirected(
    OdDbObjectId newId, 
    OdDbObjectId oldId);
    
  /** \details
    Notification function called whenever an object in an Xref database has been comandeered.
    
    \param pToDb [in]  Host database.
    \param pFromDb [in]  Xref database.
    \param objectId [in]  Object ID of the comandeered object.
    
    \remarks
    Rather than copy an Xref dependent object to the host database, the Object ID of the object
    is merely appended to the host database symbol table with the name of the Xref prepended to it.
    
    Thus, the block BAR in the xrefed database FOO, becomes FOO|BAR in the host database.
  */
  virtual void comandeered(
    OdDbDatabase* pToDb, 
    OdDbObjectId id, 
    OdDbDatabase* pFromDb);
    
  
  /** \details
    Notification function called whenever an Xref database is about 
    to be reloaded to a host database, when the Xref drawing file is unchanged.
    
    \param pToDb [in]  Host database.
    \param pFromDb [in]  Xref database.
    \param filename [in]  Xref filename.
    
    \remarks
    This function is called before the operation.
    
    \remarks
    filename may not have a path or extension.
    
    \sa
    * abortRestore
    * endRestore  
  */
  virtual void beginRestore(
    OdDbDatabase* pToDb, 
    const OdString& filename, 
    OdDbDatabase* pFromDb);

  /** \details
    Notification function called whenever the restore of an Xref database has failed.
    
    \param pToDb [in]  Host database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * beginRestore
    * endRestore  
  */
  virtual void abortRestore(
    OdDbDatabase* pToDb);
  /** \details
    Notification function called whenever the restore of an Xref database has succeeded.
    
    \param pToDb [in]  Host database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * abortRestore
    * beginAttach
  */
  virtual void endRestore(
    OdDbDatabase* pToDb);
  
  enum XrefSubcommandActivities
  {    
    kStart = 0,
    kStartItem = 2,
    kEndItem = 3,
    kEnd = 4,
    kWillAbort = 5,
    kAborted = 6,
    kStartXBindBlock = 7,
    kStartXBindSymbol = 8
  };

  /** \details
    Notification function called during an Xref Bind operation.
    
    \param activity [in]  Bind activity.
    \param blockId [in]  Object ID of the Xref.
    
    \remarks
    This function will be called multiple times as an Xrefs are bound.
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Bind has started. blockId is undefined.
    kStartItem          2         The Bind of the Xref with the specified blockId has been started. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEndItem            3         The Bind of the Xref with the specified blockId has been successfully completed. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEnd                4         The Bind has completed successfully. blockId is undefined.
    kAborted            6         The Bind has failed to complete for the Xref with the specified blockId. 
    kStartXBindBlock    7         Notification of an Xref-dependent block being bound.
    kStartXBindSymbol   8         Notification of all other Xref-dependent symbols.
    </table>

    \sa
    * xrefSubcommandAttachItem
    * xrefSubcommandDetachItem 
    * xrefSubcommandOverlayItem
    * xrefSubcommandPathItem
    * xrefSubcommandReloadItem
    * xrefSubcommandUnloadItem
    
  */
  virtual void xrefSubcommandBindItem(
    int activity, 
    OdDbObjectId blockId);
    
  /** \details
    Notification function called during an Xref Attach operation.
    
    \param activity [in]  Attach activity.
    \param xrefPath [in]  Xref path.
    
    \remarks
    This function will called multiple times as an Xrefs are attached. 
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Attach has started. xrefPath is undefined.
    kStartItem          2         The Attach of the Xref with the specified path has been started. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEndItem            3         The Attach of the Xref with the specified path has been successfully completed. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEnd                4         The Attach has completed successfully. xrefPath is undefined.
    kAborted            6         The Attach has failed to complete for the Xref with the specified filename. 
    </table>
    
    \remarks
    This function can be triggered by any action that results in the resolution of Xrefs. 
    This includes plot, IGES and DXF input, block redefinition, and Xref Reload operations.

    \sa
    * xrefSubcommandBindItem
    * xrefSubcommandDetachItem 
    * xrefSubcommandOverlayItem
    * xrefSubcommandPathItem
    * xrefSubcommandReloadItem
    * xrefSubcommandUnloadItem
  */
  virtual void xrefSubcommandAttachItem(
    int activity, 
    const OdString& xrefPath);

  /** \details
    Notification function called during an Xref Overlay operation.
    
    \param activity [in]  Overlay activity.
    \param xrefPath [in]  Xref path.
    
    \remarks
    This function will called multiple times as an Xrefs are overlayed. 
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Overlay has started. xrefPath is undefined.
    kStartItem          2         The Overlay of the Xref with the specified path has been started. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEndItem            3         The Overlay of the Xref with the specified path has been successfully completed. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEnd                4         The Overlay has completed successfully. xrefPath is undefined.
    kAborted            6         The Overlay has failed to complete for the Xref with the specified filename. 
    </table>
    
    \remarks
    This function can be triggered by any action that results in the resolution of Xrefs. 
    This includes plot, IGES and DXF input, block redefinition, and Xref Reload operations.

    \sa
    * xrefSubcommandAttachItem
    * xrefSubcommandBindItem
    * xrefSubcommandDetachItem 
    * xrefSubcommandPathItem
    * xrefSubcommandReloadItem
    * xrefSubcommandUnloadItem
  */
  virtual void xrefSubcommandOverlayItem(
    int activity, 
    const OdString& xrefPath);
    
  /** \details
    Notification function called during an Xref Detach operation.
    
    \param activity [in]  Detach activity.
    \param blockId [in]  Object ID of the Xref.
    
    \remarks
    This function will be called multiple times as an Xrefs are detached.
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Detach has started. blockId is undefined.
    kStartItem          2         The Detach of the Xref with the specified blockId has been started. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEndItem            3         The Detach of the Xref with the specified blockId has been successfully completed. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEnd                4         The Detach has completed successfully. blockId is undefined.
    kAborted            6         The Detach has failed to complete for the Xref with the specified blockId. 
    </table>

    \sa
    * xrefSubcommandAttachItem
    * xrefSubcommandBindItem
    * xrefSubcommandOverlayItem
    * xrefSubcommandPathItem
    * xrefSubcommandReloadItem
    * xrefSubcommandUnloadItem
  */
  virtual void xrefSubcommandDetachItem(
    int activity, 
    OdDbObjectId blockId);
    
  /** \details
    Notification function called during an Xref Path operation.
    
    \param activity [in]  Path activity.
    \param newPath [in]  New Xref path.
    \param blockId [in]  Object ID of the Xref.
    
    \remarks
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Path operation has started. blockId and newPath are undefined.
    kStartItem          2         The Path operation the Xref with the specified blockId has been started. 
    kEndItem            3         The Path of the Xref with the specified blockId has been successfully changed to newPath.  
    kEnd                4         The Path operation has completed successfully. blockId and newPath are undefined.
    kAborted            6         The Path has failed to complete for the Xref with the specified blockId. 
    </table>

    \sa
    * xrefSubcommandAttachItem
    * xrefSubcommandBindItem
    * xrefSubcommandDetachItem 
    * xrefSubcommandOverlayItem
    * xrefSubcommandReloadItem
    * xrefSubcommandUnloadItem
  */
  virtual void xrefSubcommandPathItem(
    int activity, OdDbObjectId blockId, 
    const OdString& newPath);
    
  /** \details
    Notification function called during an Xref Reload operation.
    
    \param activity [in]  Reload activity.
    \param blockId [in]  Object ID of the Xref.
    
    \remarks
    This function will be called multiple times as an Xrefs are reloaded.
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Reload has started. blockId is undefined.
    kStartItem          2         The Reload of the Xref with the specified blockId has been started. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEndItem            3         The Reload of the Xref with the specified blockId has been successfully completed. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEnd                4         The Reload has completed successfully. blockId is undefined.
    kAborted            6         The Reload has failed to complete for the Xref with the specified blockId. 
    </table>

    \sa
    * xrefSubcommandAttachItem
    * xrefSubcommandBindItem
    * xrefSubcommandDetachItem 
    * xrefSubcommandOverlayItem
    * xrefSubcommandPathItem
    * xrefSubcommandUnloadItem
  */
  virtual void xrefSubcommandReloadItem(
    int activity, 
    OdDbObjectId blockId);
    
  /** \details
    Notification function called during an Xref Unload operation.
    
    \param activity [in]  Reload activity.
    \param blockId [in]  Object ID of the Xref.
    
    \remarks
    This function will be called multiple times as an Xrefs are unloaded.
    
    activity must be one of the following:
    
    <table>
    Name                Value     Description
    kStart              0         The Unload has started. blockId is undefined.
    kStartItem          2         The Unload of the Xref with the specified blockId has been started. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEndItem            3         The Unload of the Xref with the specified blockId has been successfully completed. 
                                  Notification will be made for the 
                                  original Xref and any dependent Xrefs.
    kEnd                4         The Unload has completed successfully. blockId is undefined.
    kAborted            6         The Unload has failed to complete for the Xref with the specified blockId. 
    </table>
  */
  virtual void xrefSubcommandUnloadItem(
    int activity, 
    OdDbObjectId blockId);
  
  /** \details
    Notification function called during an Undo Auto operation.
    
    \param activity [in]  Auto activity.
    \param undoAuto [in]  True if and only if Undo Auto mode is on.
        
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kEnd                4         The Auto operation has completed successfully.
    </table>

    \sa
    * undoSubcommandBack
    * undoSubcommandBegin
    * undoSubcommandControl
    * undoSubcommandEnd
    * undoSubcommandMark
    * undoSubcommandNumber
  */
  virtual void undoSubcommandAuto(
    int activity, 
    bool undoAuto);
    
  /** \details
    Notification function called during an Undo Control operation.
    
    \param activity [in]  Control activity.
    \param option [in]  Undo option.
        
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kEnd                4         The Control operation has completed successfully.
    </table>

    option must be one of the following:
    
    <table>
    Name    Value   Description
    kNone   0       Undo None 
    kOne    1       Undo One  
    kAll    2       Undo All 
    </table>

    \sa
    * undoSubcommandAuto
    * undoSubcommandBack
    * undoSubcommandBegin
    * undoSubcommandEnd
    * undoSubcommandMark
    * undoSubcommandNumber
  */
  virtual void undoSubcommandControl(
    int activity, 
    int option);
    
  /** \details
    Notification function called during an Undo Begin or Undo Group operation.
    
    \param activity [in]  Begin activity.
        
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kStart              0         The Undo Begin/Group has started.
    </table>

    \sa
    * undoSubcommandAuto
    * undoSubcommandBack
    * undoSubcommandControl
    * undoSubcommandEnd
    * undoSubcommandMark
    * undoSubcommandNumber
  */
  virtual void undoSubcommandBegin(
    int activity);
    
  /** \details
    Notification function called during an Undo End or Undo Group operation.
    
    \param activity [in]  End activity.
        
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kStart              0         The Undo End has started.
    </table>

    \sa
    * undoSubcommandAuto
    * undoSubcommandBack
    * undoSubcommandBegin
    * undoSubcommandControl
    * undoSubcommandMark
    * undoSubcommandNumber
  */
  virtual void undoSubcommandEnd(
    int activity);
    
  /** \details
    Notification function called during an Undo Mark operation.
    
    \param activity [in]  Mark activity.
        
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kStart              0         The Undo Mark has started.
    </table>

    \sa
    * undoSubcommandAuto
    * undoSubcommandBack
    * undoSubcommandBegin
    * undoSubcommandControl
    * undoSubcommandEnd
    * undoSubcommandNumber
  */
  virtual void undoSubcommandMark(
    int activity);
    
  /** \details
    Notification function called during an Undo Back operation.
    
    \param activity [in]  Back activity.
        
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kStart              0         The Undo Back has started.
    </table>

    \sa
    * undoSubcommandAuto
    * undoSubcommandBegin
    * undoSubcommandControl
    * undoSubcommandEnd
    * undoSubcommandMark
    * undoSubcommandNumber
  */
  virtual void undoSubcommandBack(
    int activity);
    
  /** \details
    Notification function called during an Undo <number>operation.
    
    \param activity [in]  Number activity.
    \param numSteps [in]  Number of steps to be undone.    
    \remarks
    activity will always be:
    
    <table>
    Name                Value     Description
    kStart              0         The Undo Back has started.
    </table>

    \sa
    * undoSubcommandAuto
    * undoSubcommandBack
    * undoSubcommandBegin
    * undoSubcommandControl
    * undoSubcommandEnd
    * undoSubcommandMark
  */
  virtual void undoSubcommandNumber(
    int activity, 
    int numSteps);
  
  /** \details
    Notification function called to indicate the 
    number of entities in the pickfirst selection set has been changed.
  */
  virtual void pickfirstModified();
  
  /** \details
    Notification function called to indicate the current layout has changed.
    
    \param newLayoutName [in]  New layout name.  
  */
  virtual void layoutSwitched(
    const OdString& newLayoutName);
  
  /** \details
    Notification function called to indicate an MDI document frame window has been moved or resized.
    
    \param hwndDocFrame [in]  HWND of document frame.
    \param moved [in]  True if moved, false if resized.
    
    \sa
    mainFrameMovedOrResized
  */
  virtual void docFrameMovedOrResized(
    OdHWnd hwndDocFrame, 
    bool moved);

  /** \details
    Notification function called to indicate the Editor main frame window has been moved or resized.
    
    \param hwndMainFrame [in]  HWND of the main frame.
    \param moved [in]  True if moved, false if resized.
    \sa
    docFrameMovedOrResized
  */
  virtual void mainFrameMovedOrResized(
    OdHWnd hwndMainFrame, 
    bool moved);
  
  /** \details
    Notification function called to indicate the mouse button has been double-clicked
    in the graphics screen area.
    
    \param clickPoint [in]  WCS double-click point.
    
    \sa
    beginRightClick
  */
  virtual void beginDoubleClick(
    const OdGePoint3d& clickPoint);

  /** \details
    Notification function called to indicate the mouse button has been right-clicked
    in the graphics area.
    
    \param clickPoint [in]  WCS right-click point.
    
    \sa
    beginDoubleClick
  */
  virtual void beginRightClick(
  const OdGePoint3d& clickPoint);
  
  /** \details
    Notification function called to indicate the toolbar bitmap size is about to change.
    
    \param largeBitmaps [in]  True if and only if large bitmaps. 
    
	  \remarks
	  This function is called before the operation.

    \sa
    toolbarBitmapSizeChanged
  */
  virtual void toolbarBitmapSizeWillChange(
    bool largeBitmaps);
    
  /** \details
    Notification function called to indicate the toolbar bitmap size has changed.
    
    \param largeBitmaps [in]  True if and only if large bitmaps. 
    
	  \remarks
	  This function is called after the operation.

    \sa
    toolbarBitmapSizeWillChange
  */
  virtual void toolbarBitmapSizeChanged(
    bool largeBitmaps);
  
  /** \details
    Notification function called after objects are lazy during a partial open of a drawing.
    
    \param objectIDs [in]  Object IDs lazy loaded.
  */
      virtual void objectsLazyLoaded(const OdDbObjectIdArray& objectIds);
  
  /** \details
    Notification function called whenever the Editor is about to shut down.
    
    \remarks
    Calling veto() will cancel the shutdown.    
  */
  virtual void beginQuit();
  
  /** \details
    Notification function called whenever the Editor has failed to shut down.

    \sa
    * beginQuit
    * quitWillStart
  */
  virtual void quitAborted();

  /** \details
    Notification function called whenever the Editor is about to shut down.

    \remarks
    This function is called after beginQuit was not vetoed.
    
    \sa
    * beginQuit
    * quitAborted
  */
  virtual void quitWillStart();

  /** \details
    Notification function called whenever a modeless operation is about to start.

    \param contextString [in]  Context string identifying the operation.
    \sa
    modelessOperationEnded
  */
    virtual void modelessOperationWillStart(
    const OdString& contextString);

  /** \details
    Notification function called whenever a modeless operation has ended.

    \param contextString [in]  Context string identifying the operation.
    \sa
    modelessOperationWillStart
  */
    virtual void modelessOperationEnded(const OdString& contextString);

  virtual void sysVarChanged(OdDbDatabase* pDb, const OdString& varName);
  
  virtual void sysVarWillChange(OdDbDatabase* pDb, const OdString& varName);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdEditorReactor object pointers.
*/
typedef OdSmartPtr<OdEditorReactor> OdEditorReactorPtr;

/** \details
    This class manages application level OdEditorReactor instances.
    
    Corresponding C++ library: TD_Db
    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdEditor : public OdRxEvent 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEditor);
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdEditor object pointers.
*/
typedef OdSmartPtr<OdEditor> OdEditorPtr;

TOOLKIT_EXPORT OdEditorPtr odedEditor();

#include "TD_PackPop.h"

#endif // _ODED_H_INCLUDED_

