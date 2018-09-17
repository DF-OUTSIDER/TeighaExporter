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




#ifndef   _XREFMAN_H_
#define   _XREFMAN_H_

#include "RxObject.h"
#include "SmartPtr.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTableRecord.h"
#include "DbLinetypeTableRecord.h"
#include "DbTextStyleTableRecord.h"
#include "DbSecurity.h"

#include "TD_PackPush.h"

/** \details
    This class manages External References (Xrefs) in an OdDbDatabase object.
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbXRefMan
{
public:
  /** \details
    Loads of specified Xref(s) if not currently loaded.
    \param pHostDb [in]  Pointer to the host database.
    \param xrefBlockname [in]  Xref block name to load.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult load(OdDbDatabase* pHostDb, const OdString& xrefBlockname);

  /** \details
    Loads of specified Xref(s) if not currently loaded.

    \param xrefBTRids [in]  Array of the object IDs of the Block Table Records of the Xrefs to load.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult load(OdDbObjectIdArray& xrefBTRids);

  /** \details
    Loads of specified Xref(s) if not currently loaded.

    \param pBTR [in]  Pointer to the Block Table Record of the Xref to load.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult load(OdDbBlockTableRecord* pBTR);

  /** \details
    Loads of all Xrefs not currently loaded.
    \param pHostDb [in]  Pointer to the host database.
    \param verify [in]  Verify.
    \remarks
    If verify is true, this function attempts to load those Xrefs that
    are not unloaded, not unresolved, and not resolved.

    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult loadAll(OdDbDatabase* pHostDb, bool verify = false);

  /** \details
    Unloads the specified Xref(s).
    \param pBTR [in]  Pointer to the Block Table Record of the Xref to unload.
  */
  static void unload(OdDbBlockTableRecord* pBTR);

  /** \details
    Unloads the specified Xref(s).
    \param xrefBTRids [in]  Array of the object IDs of the Block Table Records of the Xrefs to nload.
  */
  static void unload(OdDbObjectIdArray& xrefBTRids);

  /** \details
    Unloads all loaded Xrefs.
    \param pHostDb [in]  Pointer to the host database.
  */
  static void unloadAll(OdDbDatabase* pHostDb);

  /** \details
    Binds the specified Xref.
    \param pBTR [in]  Pointer to the Block Table Record of the Xref to bind.
    \param insertBind [in]  True for Xref Insert, false for Xref Bind.
  */
  static OdResult bind(OdDbBlockTableRecord* pBTR, bool insertBind = false);

  /** \details
    Detaches the specfied Xref.
    \param pBTR [in]  Pointer to the Block Table Record of the Xref to detach.
  */
  static OdResult detach(OdDbBlockTableRecord* pBTR);

  /** \details
    Controls the reference type fo the specified Xref.
    \param pBTR [in]  Pointer to the Block Table Record of the Xref to detach.
    \param overlaid [in]  True for Xref Overlay, false for Xref Attach.
  */
  static void setOverlaid(OdDbBlockTableRecord* pBTR, bool  overlaid = true);

};

/** \details
    This class is the Xref Manager Extenstion class.
    
    \remarks
    This class creates and augments Xref blocks in OdDbDatabase instances. 
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbXRefManExt
{
public:
  /** \details
    Creates an Xref block in the specified database.

    \param pDb [in]  Pointer to the database.
    \param pathName [in]  Path and filename for Xref block.
    \param blockName [in]  Name of Xref block.
    \param overlaid [in]  True for Xref Overlay, false for Xref Attach.
    \param password [in]  Password for file.
    \param handle [in]  Handle for Xref block.
    
    \returns
    Returns a SmartPointer to the newly added Block Table Record.
  */
  static OdDbBlockTableRecordPtr     
    addNewXRefDefBlock(OdDbDatabase* pDb, const OdString& pathName, 
                        const OdString& blockName, bool overlaid, 
                        const OdPassword& password = OdPassword(),
                        OdDbHandle handle = 0);

  /** \details
    Adds an Xref dependent layer to the specified Xref block.
    
    \param pXRefBlock [in]  Pointer to the Xref Block Table Record.
    \param layerName [in]  Name of the layer.

    \returns
    Returns a SmartPointer to the newly added Layer Table Record.
  */
  static OdDbLayerTableRecordPtr     
    addNewXRefDependentLayer(const OdDbBlockTableRecord* pXRefBlock, const OdString& layerName);

  /** \details
    Adds an Xref dependent layer to the specified Xref block.
    
    \param xRefBlockId [in]  Object ID of the Xref Block Table Record.
    \param layerName [in]  Name of the layer.

    \returns
    Returns a SmartPointer to the newly added Layer Table Record.
  */
  static OdDbLayerTableRecordPtr addNewXRefDependentLayer(OdDbObjectId xRefBlockId, const OdString& layerName)
  { return addNewXRefDependentLayer(OdDbBlockTableRecordPtr(xRefBlockId.safeOpenObject()), layerName); }

  /** \details
    Adds an Xref dependent linetype to the specified Xref block.
    
    \param pXRefBlock [in]  Pointer to the Xref Block Table Record.
    \param linetypeName [in]  Name of the linetype.

    \returns
    Returns a SmartPointer to the newly added Linetype Table Record.
  */
  static OdDbLinetypeTableRecordPtr     
    addNewXRefDependentLinetype(const OdDbBlockTableRecord* pXRefBlock, const OdString& linetypeName);

  /** \details
    Adds an Xref dependent linetype to the specified Xref block.
    
    \param xRefBlockId [in]  Object ID of the Xref Block Table Record.
    \param linetypeName [in]  Name of the linetype.

    \returns
    Returns a SmartPointer to the newly added Linetype Table Record.
  */
  static OdDbLinetypeTableRecordPtr
    addNewXRefDependentLinetype(OdDbObjectId xRefBlockId, const OdString& linetypeName)
  { return addNewXRefDependentLinetype(OdDbBlockTableRecordPtr(xRefBlockId.safeOpenObject()), linetypeName); }

  /** \details
    Adds an Xref dependent text style to the specified Xref block.
    
    \param pXRefBlock [in]  Pointer to the Xref Block Table Record.
    \param textStyleName [in]  Name of the text style.

    \returns
    Returns a SmartPointer to the newly added Text Style Table Record.
  */
  static OdDbTextStyleTableRecordPtr     
    addNewXRefDependentTextStyle(const OdDbBlockTableRecord* pXRefBlock, const OdString& textStyleName);

  /** \details
    Adds an Xref dependent text style to the specified Xref block. 
    
    \param xRefBlockId [in]  Object ID of the Xref Block Table Record.
    \param textStyleName [in]  Name of the text style.

    \returns
    Returns a SmartPointer to the newly added Text Style Table Record.
  */
  static OdDbTextStyleTableRecordPtr
    addNewXRefDependentTextStyle(OdDbObjectId xRefBlockId, const OdString& textStyleName)
  { return addNewXRefDependentTextStyle(OdDbBlockTableRecordPtr(xRefBlockId.safeOpenObject()), textStyleName); }

  /** \details
    Adds nested XRef Id to parent block.
  */
  static void addNestedXRefId(OdDbBlockTableRecord* pXRefBlock, OdDbObjectId nestedBlockId);

  /** \details
    Gets nested XRef Ids.
  */
  static void getNestedXRefIds(OdDbBlockTableRecord* pXRefBlock, OdDbObjectIdArray& ids);

  /** \details
    Gets block Id for a table record.
  */
  static OdDbObjectId getSymbolTableRecordXrefBlockId(const OdDbSymbolTableRecord* pRec);
};

#include "TD_PackPop.h"

#endif //_XREFMAN_H_


