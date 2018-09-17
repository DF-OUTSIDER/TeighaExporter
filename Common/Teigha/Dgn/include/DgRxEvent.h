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

#ifndef   _ODDGRX_EVENT_H__
#define   _ODDGRX_EVENT_H__

#include "TD_PackPush.h"

#include "DgElementId.h"

#define ODDGRX_EVENT_OBJ OD_T("OdDgRxEvent")

class OdDgDatabase;
class OdGeMatrix3d;
class OdDgIdMapping;
class OdGePoint3d;
class OdDgElementId;

/** \details
    This class is the base class for custom classes that receive notification
    of OdDgRxEvent (application level) events.
    
    \remarks
    The default implementations of all methods in this class do nothing but return.

    \sa
    TG_Db
  
    <group OdRx_Classes> 
*/
class TG_EXPORT OdDgRxEventReactor : public OdRxObject 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdDgRxEventReactor);
  
  /** \details
    Notification function called whenever a .dgn file is being read.
    
    \param pDb [in]  Pointer to the database being created.
    \param filename [in]  Name of the .dgn file.
    
    \remarks
    This function is called during the operation.
    
    \sa
    * databaseConstructed
    * initialDgnFileOpenComplete
  */
  virtual void dgnFileOpened(
    OdDgDatabase* pDb, 
    const OdString& filename);
    
  /** \details
    Notification function called whenever a .dgn file has been read.
    
    \param pDb [in]  Pointer to the database being created.
    
    \remarks
    This function is called after the read operation, but before the database is constructed.
    
    \sa
    * databaseConstructed
    * dgnFileOpened
  */
  virtual void initialDgnFileOpenComplete(
    OdDgDatabase* pDb);
    
  /** \details
    Notification function called whenever a database has been constructed from a file.
    
    \param pDb [in]  Pointer to the database constructed.
    
    \remarks
    This function is called after the operation.
    
    \sa
    * dwgFileOpened
    * initialDgnFileOpenComplete
  */
  virtual void databaseConstructed(
    OdDgDatabase* pDb);

  /** \details
    Notification function called whenever a database is about to be deleted from memory.  

    \param pDb [in]  Pointer to the database to be destroyed.
    
    \remarks
    This function is called before the operation.
  */
  virtual void databaseToBeDestroyed(
    OdDgDatabase* pDb);
  
  /** \details
    Notification function called whenever a database is about to be saved to a .dgn file.
    
    \param pDb [in]  Pointer to the database to be saved.
    \param intendedName [in]  Intended name of the .dgn file.
    
    \remarks
    This function is called before the operation, and before the user 
    has had a chance to modify the filename. The file may not have the intendedName.
    
    \sa
    * abortSave
    * saveComplete
  */
  virtual void beginSave(
    OdDgDatabase* pDb, 
    const OdString& intendedName);

  /** \details
    Notification function called whenever a database has been saved 
    to a .dgn file.
    
    \param pDb [in]  Pointer to the database saved.
    \param actualName [in]  Actual name of the .dwg file.
    
    \remarks
    This function is called after the operation, but before the database is constructed.
    
    \sa
    * abortSave
    * beginSave
  */
  virtual void saveComplete(
    OdDgDatabase* pDb, 
    const OdString& actualName);
    
  /** \details
    Notification function called whenever the save of a database has failed.
    
    \param pDb [in]  Pointer to the database being saved.
    
    \remarks
    This function is called after the operation.

    \sa
    * beginSave
    * saveComplete
  */
  virtual void abortSave(
    OdDgDatabase* pDb);
  
  // Insert Events.
  
  /** \details
    Notification function called whenever one database is about to be inserted into another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    \param blockName [in]  Name of the block.
    
    \remarks
    This function is called before the operation.
    
    \remarks
    If blockName is specified, pFromDb was inserted into pToDb as a OdDbBlockTableRecord.
    
    \sa
    * abortInsert
    * endInsert
    * otherInsert
  */
  virtual void beginInsert(
    OdDgDatabase* pToDb, 
    const OdString& blockName, 
    OdDgDatabase* pFromDb);
    
  /** \details
    Notification function called whenever one database is about to be inserted into another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    \param xfm [in]  Transformation matrix.
    
    \remarks
    This function is called before the operation.
    
    \remarks
    If xfm is specified, pFromDb was inserted into pToDb as entities.
    
    \sa
    * abortInsert
    * endInsert
    * otherInsert
  */
  virtual void beginInsert(
    OdDgDatabase* pToDb, 
    const OdGeMatrix3d& xfm, 
    OdDgDatabase* pFromDb);
    
  /** \details
    Notification function called whenever one database has been inserted into another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    This function is called after the operation, and is sent just before 
    beginDeepCloneXlation. It is called before any Object IDs are translated.
    
    \remarks
    Element IDs of cloned objects do not point at the cloned objects, and
    must therefore not not be used at this point for any operations on
    those objects.
    
    \sa
    * abortInsert
    * beginInsert
    * endInsert
  */
  virtual void otherInsert(
    OdDgDatabase* pToDb, 
    OdDgIdMapping& idMap, 
    OdDgDatabase* pFromDb);
    
  /** \details
    Notification function called whenever the insertion of a database has failed.
    
    \param pToDb [in]  Destination database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * beginInsert
    * endInsert
    * otherInsert
  */
  virtual void abortInsert(
    OdDgDatabase* pToDb);
    
  /** \details
    Notification function called whenever the insertion of a database has succeeded.
    
    \param pToDb [in]  Destination database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * abortInsert
    * beginInsert
    * otherInsert
  */
  virtual void endInsert(
    OdDgDatabase* pToDb);
  
  // Wblock Events.

  /** \details
    Notification function called while one database is about to be wblocked to another.
    
    \param pFromDb [in]  Source database.
  */
  virtual void wblockNotice(
    OdDgDatabase* pFromDb);
  
  /** \details
    Notification function called while one database is being wblocked to another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    \param insertionPoint [in]  INSBASE of pToDb.
    
    \remarks
    This function is called during the operation.
    
    If insertionPoint is specified, the wblock operation is being performed on a set
    of entities in pFromDb.
    
    \sa
    * abortWblock
    * beginWblockObjects
    * endWblock
    * otherWblock
    * wblockNotice
  */
  virtual void beginWblock(
    OdDgDatabase* pToDb, 
    OdDgDatabase* pFromDb, 
    const OdGePoint3d& insertionPoint);
    
  /** \details
    Notification function called while one database is being wblocked to another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    \param blockId [in]  Element ID of element being wblocked.
    
    \remarks
    This function is called during the operation.
    
    If blockId is specified, the wblock operation is being performed on a 
    BlockTableRecord in pFromDb.
    
    \sa
    * abortWblock
    * beginWblockObjects
    * endWblock
    * otherWblock
    * wblockNotice
  */
  virtual void beginWblock(
    OdDgDatabase* pToDb, 
    OdDgDatabase* pFromDb, 
    OdDgElementId blockId);
    
  /** \details
    Notification function called while one database is being wblocked to another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    
    \remarks
    This function is called during the operation.
    
    The entire pFromDb database is wblocked to pToDb.  
    
    \sa
    * abortWblock
    * beginWblockObjects
    * endWblock
    * otherWblock
    * wblockNotice
  */
  virtual void beginWblock(
    OdDgDatabase* pToDb, 
    OdDgDatabase* pFromDb);

     
  /** \details
    Notification function called whenever one database is being wblocked to another.
    
    \param pToDb [in]  Destination database.
    \param pFromDb [in]  Source database.
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    idMap contains mapping of the original objects in pFromDb to the objects created in pToDb.

    This function is called after the operation, and is sent just before 
    beginDeepCloneXlation.
    
    \sa
    * abortWblock
    * beginWblock
    * beginWblockObjects
    * endWblock
    * wblockNotice
  */
  virtual void otherWblock(
    OdDgDatabase* pToDb, 
    OdDgIdMapping& idMap, 
    OdDgDatabase* pFromDb);
    
  /** \details
    Notification function called whenever the wblock of a database has failed.
    
    \param pToDb [in]  Destination database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    beginWblock
    beginWblockObjects
    endWblock
    otherWblock
    wblockNotice
  */
  virtual void abortWblock(
    OdDgDatabase* pToDb);
    
  /** \details
    Notification function called whenever the wblock of a database has succeded.
    
    \param pToDb [in]  Destination database.
    
    \remarks
    This function is called after the operation.
        
    \sa
    * abortWblock
    * beginWblock
    * beginWblockObjects
    * otherWblock
    * wblockNotice
  */
  virtual void endWblock(
    OdDgDatabase* pToDb);
    
  /** \details
    Notification function called whenever one database is being wblocked to another.
    
    \param pFromDb [in]  Source database.
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    idMap contains mapping of the original objects in pFromDb to the objects created in pToDb.

    This notification function gives wblockCloneObjects a method
    of being notified before each pFrom database before the actual cloning begins.
    
    \sa
    * abortWblock
    * beginWblock
    * endWblock
    * otherWblock
    * wblockNotice
  */
  virtual void beginWblockObjects(
    OdDgDatabase* pFromDb, 
    OdDgIdMapping& idMap);
  
  // Deep Clone Events.
  
  /** \details
    Notification function called whenever a subDeepClone operation is about to be started on a database.
    
    \param pToDb [in]  Destination database.
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    idMap will always be empty.

    \sa
    * abortDeepClone
    * beginDeepCloneXlation
    * endDeepClone
  */
  virtual void beginDeepClone(
    OdDgDatabase* pToDb, 
    OdDgIdMapping& idMap);

  /** \details
    Notification function called whenever a the translation stage of a subDeepClone 
    operation is about to be started on a database.
    
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    This function is called after all objects, and their owned objects, have been cloned.
    It is called before any Object IDs are translated 
    
    \remarks
    Object IDs of cloned objects do not point at the cloned objects, and
    must therefore not not be used at this point for any operations on
    those objects.
    
    \sa
    * abortDeepClone
    * beginDeepClone
    * endDeepClone
  */
  virtual void beginDeepCloneXlation(
    OdDgIdMapping& idMap);

  /** \details
    Notification function called whenever a subDeepClone operation has failed.
    
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    This function is called after the operation.
   
    \remarks
    The objects involved in the subDeepClone operation are in an indeterminate state 
    and must be cleaned up.
   
    \sa
    * beginDeepClone
    * beginDeepCloneXlation
    * endDeepClone
  */
  virtual void abortDeepClone(
    OdDgIdMapping& idMap);

  /** \details
    Notification function called whenever a subDeepClone operation has succeeded.
    
    \param idMap [in]  ID Map source -> destination.
    
    \remarks
    This function is called after the operation.
   
    \sa
    * abortDeepClone
    * beginDeepClone
    * beginDeepCloneXlation
  */
  virtual void endDeepClone(
    OdDgIdMapping& idMap);
  
  // Partial Open Events.
  
  /** \details
    Notification function called before an attempt is made to partially open an OdDbDatabase.

    \param pDb [in]  Pointer to the database.
    
    \remarks
    To 
    veto the partial open, an application should override partialOpenNotice, and from it call
    
                pDb->disablePartialOpen()
    
  */
  virtual void partialOpenNotice(
    OdDgDatabase* pDb);
  
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgRxEventReactor object pointers.
*/
typedef OdSmartPtr<OdDgRxEventReactor> OdDgRxEventReactorPtr;

/** \details
    This class manages OdDgRxEventReactor instances.
    
    \sa
    TD_Db
    <group OdRx_Classes> 
*/
class TG_EXPORT OdDgRxEvent : public OdRxObject 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdDgRxEvent);
  
  /** \details
    Adds the specified event reactor to the host application's event reactor list.
    \param pReactor [in]  Pointer to the event reactor.
  */
  virtual void addReactor(
    OdDgRxEventReactor* pReactor) = 0;

  /** \details
    Removes the specified event reactor from the host application's event reactor list.
    \param pReactor [in]  Pointer to the event reactor.
  */
  virtual void removeReactor(
    OdDgRxEventReactor* pReactor) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgRxEvent object pointers.
*/
typedef OdSmartPtr<OdDgRxEvent> OdDgRxEventPtr;


TG_EXPORT OdDgRxEventPtr odDgRxEvent();

#include "TD_PackPop.h"

#endif //_ODDGRX_EVENT_H__

