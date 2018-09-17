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

#ifndef DBLAYERSTATEMANAGER_H
#define DBLAYERSTATEMANAGER_H

#include "TD_PackPush.h"
#include "StringArray.h"

/** \details
  This class implements an interface for manipulating LayerStates in multiple OdDbDatabase instances.
  
  Corresponding C++ library: TD_Db
  <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbLayerState
{
public:
  enum Mask
  {
    kDefMask          = (long)0xFFFFFFFF,
    kNone             = 0x0000,
    kOn               = 0x0001,
    kFrozen           = 0x0002,
    kLocked           = 0x0004,
    kPlot             = 0x0008,
    kNewViewport      = 0x0010,
    kColor            = 0x0020,
    kLineType         = 0x0040,
    kLineWeight       = 0x0080,
    kPlotStyle        = 0x0100,
    kCurrentViewport  = 0x0200,
    kTransparency     = 0x0400,

    kAll              = kOn | kFrozen | kLocked | kPlot | kNewViewport |
                        kColor | kLineType | kLineWeight | kPlotStyle |
                        kCurrentViewport | kTransparency,

    kHidden           = 0x8000        
  };
  
  enum
  {
    kUndefDoNothing     = 0x00,
    kUndefTurnOff       = 0x01,
    kUndefFreeze        = 0x02,
    kRestoreAsOverrides = 0x04
  };

  /** \details
    Returns the Object ID of the LayerStates dictionary for the specified database.
    \param pDb [in]  Pointer to the database.
    \param createIfNotFound [in]  Controls the creation of the dictionary if it does not exist.
  */
  static OdDbObjectId dictionaryId(OdDbDatabase *pDb, bool createIfNotFound = false);
  
  /** \details
    Returns true if and only if the specified database
    has a LayerState with the specified name.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
  */
  static bool has(OdDbDatabase *pDb, const OdString &layerStateName);
  
  /** \details
    Saves the current LayerState of the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerStateMask [in]  LayerState mask.
    \param viewportId [in]  Viewport whose LayerState is to be saved.
    
    \remarks
    layerStateMask determines which layer attributes are to be stored in the LayerState.  
    layerStateMask must be a combination of one or more of the following:
    
    <table>
    Name                   Value        
    kNone                  0x0000       
    kOn                    0x0001       
    kFrozen                0x0002       
    kLocked                0x0004       
    kPlot                  0x0008       
    kNewViewport           0x0010       
    kColor                 0x0020       
    kLineType              0x0040       
    kLineWeight            0x0080       
    kPlotStyle             0x0100        
    kCurrentViewport       0x0200     
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency       
    kHidden                0x8000        
    </table>
    
  */
  static void save(OdDbDatabase *pDb, const OdString &layerStateName, int layerStateMask, const OdDbObjectId &viewportId = OdDbObjectId::kNull);

  /** \details
    Restores the specified LayerState of the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerStateMask [in]  LayerState mask.
    \param viewportId [in]  Viewport whose LayerState is to be saved.
    \param undefAction [in]  Action for undefined layers.
    
    \remarks
    undefAction specified the action for layers not defined in the LayerState.
    undefAction must be one of the following:
    
    <table>
    Name               Value  
    kUndefDoNothing    0      
    kUndefTurnOff      1        
    kUndefFreeze       2
    </table>
    
    layerStateMask determines which layer attributes are to be restored from the LayerState.  
    layerStateMask must be a combination of one or more of the following:
    
    <table>
    Name                   Value        
    kDefMask               0xFFFFFFFF   
    kNone                  0x0000       
    kOn                    0x0001       
    kFrozen                0x0002       
    kLocked                0x0004       
    kPlot                  0x0008       
    kNewViewport           0x0010       
    kColor                 0x0020       
    kLineType              0x0040       
    kLineWeight            0x0080       
    kPlotStyle             0x0100        
    kCurrentViewport       0x0200        
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency       
    kHidden                0x8000        
    </table>
    
  */
  static void restore(OdDbDatabase *pDb, const OdString &layerStateName,
    int flags = kUndefDoNothing, const int layerStateMask = kDefMask, const OdDbObjectId& viewportId = OdDbObjectId::kNull);
  /** \details
    Sets the LayerState mask for the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerStateMask [in]  LayerState mask.
    
    \remarks
    layerStateMask determines which layer attributes are to be restored from the LayerState.  
    layerStateMask must be a combination of one or more of the following:
    
    <table>
    Name                   Value        
    kNone                  0x0000       
    kOn                    0x0001       
    kFrozen                0x0002       
    kLocked                0x0004       
    kPlot                  0x0008       
    kNewViewport           0x0010       
    kColor                 0x0020       
    kLineType              0x0040       
    kLineWeight            0x0080       
    kPlotStyle             0x0100        
    kCurrentViewport       0x0200        
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency       
    kHidden                0x8000        
    </table>
    
  */
  static void setMask(OdDbDatabase *pDb, const OdString &layerStateName, int layerStateMask);
  
  /** \details
    Returns the LayerState mask for the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    
    \remarks
    The layer state mask determines which layer attributes are to be restored from the LayerState.  

    mask() returns a combination of one or more of the following:
    
    <table>
    Name                   Value        
    kNone                  0x0000       
    kOn                    0x0001       
    kFrozen                0x0002       
    kLocked                0x0004       
    kPlot                  0x0008       
    kNewViewport           0x0010       
    kColor                 0x0020       
    kLineType              0x0040       
    kLineWeight            0x0080       
    kPlotStyle             0x0100        
    kCurrentViewport       0x0200        
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency       
    kHidden                0x8000        
    </table>
    
  */
  static int  mask(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Removes the specified LayerState from the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
  */
  static void remove(OdDbDatabase *pDb, const OdString &layerStateName);
 
  /** \details
    Renames the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param oldName [in]  Old name.
    \param newName [in]  New name.
  */
  static void rename(OdDbDatabase *pDb, const OdString &oldName, const OdString &newName);

  enum ImportResult
  {
    kImported,
    kAlreadyExists
  };

  /** \details
    Imports the LayerState from the specified file to the specified database.

    \param pDb [in]  Pointer to the database.
    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.    
    \param pName [out]  Optional pointer to the string where imported layer state name will be stored.

    Returns one of the following:

    <table>
    Name               Value  
    kImported          0      
    kAlreadyExists     1        
    </table>
  */
  static ImportResult importData(OdDbDatabase *pDb, OdStreamBuf* pStreamBuf, OdString* pName = 0);

  /** \details
    Exports the LayerState from the specified database to the specified file.

    \param pDb [in]  Pointer to the database.
    \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.
    \param layerStateName [in]  LayerState name.
  */
  static void exportData(OdDbDatabase *pDb, const OdString &layerStateName, OdStreamBuf* pStreamBuf);
  
  /** \details
    Sets the description of the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param description [in]  Description.
  */
      static void setDescription(OdDbDatabase *pDb, const OdString &layerStateName, const OdString &description);
  /** \details
    Returns the description of the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
  */
      static OdString description(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Returns true if and only if there is viewport data associated with specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
  */
      static bool hasViewportData(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Adds more layers to a layer state.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerIds [in]  Array of object Ids of layers to be added to the layer state 
  */
      static OdResult addLayerStateLayers(const OdString &sName, OdDbObjectIdArray layerIds);

  /** \details
    Removes layers from a layer state.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerNames [in]  Array of names of layers to be removed from the layer state.
  */
      static OdResult removeLayerStateLayers(OdDbDatabase* pDb, const OdString sName, const OdStringArray& layerNames);

  /** \details
    Creates a layer state where specified layers are assigned one state (e.g. ON), 
    and all the others are assigned other state (e.g. OFF).

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerIds [in]  Array of object Ids of layers to be added to the layer state 
    \param mask [in]  created 
  */
  static OdResult saveGroup(OdDbDatabase *pDb, const OdString &sName,
    const OdDbObjectIdArray& pLayers, int mask = kOn,
    int includedLayersState = kOn, int otherLayersState = kNone);
};

class OdDbLayerStateManagerReactor;
class OdDbLayerStateManagerImpl;

/** \details
  Corresponding C++ library: TD_Db
  <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbLayerStateManager: public OdRxObject
{
protected:
  OdDbLayerStateManager(OdDbLayerStateManagerImpl*);
public:
  ODRX_DECLARE_MEMBERS(OdDbLayerStateManager);
  OdDbLayerStateManager();
  ~OdDbLayerStateManager();

  enum LayerStateMask {
    kNone           = 0x0000,
    kOn             = 0x0001,
    kFrozen         = 0x0002,
    kLocked         = 0x0004,
    kPlot           = 0x0008,
    kNewViewport    = 0x0010,
    kColor          = 0x0020,
    kLineType       = 0x0040,
    kLineWeight     = 0x0080,
    kPlotStyle      = 0x0100,
    kCurrentViewport = 0x0200,
    kTransparency   = 0x0400,
    kAll            = kOn | kFrozen | kLocked | kPlot | kNewViewport |
    kColor | kLineType | kLineWeight | kPlotStyle |
    kCurrentViewport | kTransparency,
    kStateIsHidden  = 0x8000,        
    kLastRestored   = 0x10000,        
    kDecomposition  = kAll | 0x20000
  };

  enum {
    kUndefDoNothing = 0,
    kUndefTurnOff = 1,
    kUndefFreeze = 2,
    kRestoreAsOverrides = 4
  };

  bool                addReactor(OdDbLayerStateManagerReactor * pReactor);
  bool                removeReactor(OdDbLayerStateManagerReactor * pReactor);

  OdDbObjectId        layerStatesDictionaryId(bool bCreateIfNotPresent=false);
  bool                hasLayerState(const OdString& sName);
  OdResult   saveLayerState(const OdString& sName, LayerStateMask mask);
  OdResult   restoreLayerState(const OdString& sName);
  OdResult   setLayerStateMask(const OdString& sName, LayerStateMask mask);
  OdResult   getLayerStateMask(const OdString& sName, LayerStateMask &returnMask);
  OdResult   deleteLayerState(const OdString& sName);
  OdResult   renameLayerState(const OdString& sName, const OdString& sNewName);
  OdResult   importLayerState(OdStreamBuf* pStreamBuf);
  OdResult   importLayerState(OdStreamBuf* pStreamBuf, OdString& sName);
  OdResult   exportLayerState(const OdString& sNameToExport, OdStreamBuf* pStreamBuf);
  OdResult   saveLayerState(const OdString& sName, LayerStateMask mask, const OdDbObjectId& idVp);
  OdResult   restoreLayerState(const OdString& sName, const OdDbObjectId& idVp, int nRestoreFlags = 0, const LayerStateMask* pClientMask = NULL);
  OdResult   setLayerStateDescription(const OdString& sName, const OdString& sDesc);
  OdResult   getLayerStateDescription(const OdString& sName, OdString& sDesc);
  bool       layerStateHasViewportData(const OdString&  sName);
  OdResult   importLayerStateFromDb(const OdString& pStateName, OdDbDatabase* pDb);
  OdResult   getLayerStateNames(OdStringArray& lsArray, bool bIncludeHidden = true, bool bIncludeXref = true);
  OdResult   getLastRestoredLayerState(OdString& sName, OdDbObjectId &restoredLSId);
  OdResult   getLayerStateLayers(OdStringArray& layerArray, const OdString& sName, bool bInvert = false);
  bool       compareLayerStateToDb(const OdString& sName, const OdDbObjectId& idVp);
  OdResult   addLayerStateLayers(const OdString& sName, const OdDbObjectIdArray& layerIds);
  OdResult   removeLayerStateLayers(const OdString& sName, const OdStringArray& layerNames);
  bool       isDependentLayerState(const OdString& sName);
  OdDbDatabase* getDatabase() const;
protected:
  OdDbLayerStateManagerImpl* m_pImpl;
  friend class OdDbLayerStateManagerImpl;
  friend class OdDbDatabase;
};


/** \details
  Corresponding C++ library: TD_Db
  <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbLayerStateManagerReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbLayerStateManagerReactor);

  virtual void layerStateCreated(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);
  virtual void layerStateCompareFailed(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  virtual void layerStateToBeRestored(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);
  virtual void layerStateRestored(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);
  virtual void abortLayerStateRestore(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);


  virtual void layerStateToBeDeleted(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);
  virtual void layerStateDeleted(const OdString& layerStateName);
  virtual void abortLayerStateDelete(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  virtual void layerStateToBeRenamed(const OdString& oldLayerStateName,
    const OdString& newLayerStateName);
  virtual void layerStateRenamed(const OdString& oldLayerStateName,
    const OdString& newLayerStateName);
  virtual void abortLayerStateRename(const OdString& oldLayerStateName,
    const OdString& newLayerStateName);
};

typedef OdSmartPtr<OdDbLayerStateManagerReactor> OdDbLayerStateManagerReactorPtr;

#include "TD_PackPop.h"

#endif //#ifndef DBLAYERSTATEMANAGER_H
