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




#ifndef _ODDBXOBJECT_INCLUDED_
#define _ODDBXOBJECT_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"

#include "Gi/GiDrawable.h"
#include "DbObjectId.h"
#include "OdString.h"
#include "DbObjectReactor.h"
#include "IdArrays.h"
#include "ResBuf.h"
#include "DebugStuff.h"

class OdDbFiler;
class OdDbDwgFiler;
class OdDbDxfFiler;
class OdGiDrawableTraits;
class OdGiWorldDraw;
class OdGiViewportDraw;
class OdGsCache;
class OdDbObjectImpl;
class OdDbEntity;
class OdDbDatabase;
class OdDbIdMapping;
class OdDbAuditInfo;
class OdGeMatrix3d;
class OdDbObjStorage;
class OdString;
class OdGePoint3d;
class OdBinaryData;
class OdDbIdPair;
class OdDbField;
class OdDbDictionary;
class OdDbXrecord;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbXrecord object pointers.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbXrecord> OdDbXrecordPtr;

/** \details
    Declares the member functions for classes derived from OdDbObject. 
        
    \param ClassName [in]  Name of the derived class.
    
    \remarks
    Classes derived 
    from OdDbObject should invoke this macro in their class definitions, 
    passing the name of the derived class.
*/
#define ODDB_DECLARE_MEMBERS(ClassName)\
protected:                                \
  ClassName(OdDbObjectImpl* pImpl);       \
public:                                   \
ODRX_DECLARE_MEMBERS (ClassName)

/** \details
  Creates a new instance of a derived class,
  and returns a SmartPointer to it, without
  incrementing the reference count of the new
  object.
  
  \param ClassName [in]  Name of the derived class.
*/
#define DBOBJECT_CONSTR(ClassName) OdSmartPtr<ClassName> (new ClassName, kOdRxObjAttach)


/** \details
    This class implements OdError objects with Result codes of eXdataSizeExceeded,
    and the Object IDs of the objects related to the errors.
    
    Corresponding C++ library: TD_Db
    <group Error_Classes> 
*/
class TOOLKIT_EXPORT OdError_XdataSizeExceeded : public OdError
{
public:
  /** \param objectId [in]  Object ID of the object with the error.
  */
  OdError_XdataSizeExceeded(
    const OdDbObjectId& objectId);
};
                            

/** \details
  <group TD_Namespaces>
  {noAutoLink}
*/
namespace OdDb
{ 
  /** \details
    The file OdaDefs.h contains const definitions of DuplicateRecordCloning values
    that do not require the "OdDb::" scope resolution qualifier. They should be
    used only if there will be no naming conflicts.
  */
  enum DuplicateRecordCloning
  {
    kDrcNotApplicable  = 0,   // Not applicable to the object.
    kDrcIgnore         = 1,   // If a duplicate record exists, use the existing record
                              // in the *database*, and ignore the clone.
    kDrcReplace        = 2,   // If a duplicate record exists, replace it with the cloned record.
    kDrcXrefMangleName = 3,   // Incoming record names are mangled with <Xref>$0$<name>
    kDrcMangleName     = 4,   // Incoming record names are mangled with $0$<name>
    kDrcUnmangleName   = 5,   // Unmangle the names mangled by kDrcMangleName, then default to kDrcIgnore.
                              // Typically used by RefEdit when checking records into the original *database*.  
    kDrcMax            = kDrcUnmangleName //  The maximum value of this enum.
  };

  /** \details
    The file OdaDefs.h contains const definitions of Visibility values
    that do not require the "OdDb::" scope resolution qualifier. They should be
    used only if there will be no naming conflicts.
  */
  enum Visibility
  {
    kInvisible = 1,
    kVisible = 0
  };

  enum SaveType
  {
    kDwg   = 0,   // Save as .dwg file
    kDxf   = 1,   // Save as .dxf file
    kDxb   = 2,   // Save as binary .dxf file
    kUnknown = -1 // Database was created from scratch
  };
}


/** \details
    This class is the base class for all objects contained in an OdDbDatabase instance (OdDb objects).

    \remarks
    
    Creating and Deleting Database Objects
    
    * Database objects are normally created by calling OdDbXXXXX::createObject().
       When an object is created, it will be in kOpenForWrite mode.  The delete
       operator should never be called on database objects.  Instead, erase()
       should be called, which marks the object as erased.
    * OdDbObject instances should be created by calling the createObject() method.
    * OdDbObject instances are created in kOpenForWrite mode.  
    * Depending on other OdDbObject instances while constructing or deleting instances is prohibited.
    * OdDbObject instances should be deleted from an OdDbDatabase with erase().
    * Never access a pointer to a closed object.
    * The delete operator must never be called on OdDbObject instances. Instead, the erase() method
       should be called, which marks this object as erased.
    
    Accessing Database Objects
    
    * Database objects must be opened before they can be accessed.  
    * Given a valid OdDbObjectId, a database object is opened by calling OdDbObjectId::safeOpenObject().  
    * Database objects should be opened in the most restrictive mode possible, and should be released 
       immediately when access is no longer needed.
       
    Object may be opened in any one of the following modes:
    
    <table>
    Mode              Description
    OdDb::kForRead    Allows operations that do not modify this object.  
                      A database object can be opened in this mode any number of times (simultaneously), 
                      if has not open in OdDb::kForWrite or OdDb::kForNotify mode.  
                      An exception will be thrown if any type of write operation is attempted 
                      on an object open in this mode.
    OdDb::kForWrite   Allows read and write operations to be performed on this object.  
                      An database object can be opened in this mode only if it is not
                      already open in any mode.
    OdDb::kForNotify  This object is opened for notification purposes.  
                      A database object can be opened in this mode so long as it is 
                      not already open in kNotify mode.
    </table>
    
    \sa
    * OdDbObjectId
    * OdDbDatabase

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObject : public OdGiDrawable
{
public:
  ODDB_DECLARE_MEMBERS(OdDbObject);

protected:
  OdDbObject();
public:
  ~OdDbObject();
  
  /** \details
    Increments the reference count of this object.
  */
  void addRef();

  /** \details
    Decrements the reference count of this object.
    \remarks
    This object is deleted when the reference count reaches zero.
  */
  void release();

  /** \details
      Returns the reference count of this object.
  */
  long numRefs() const;
  
  /** \details
    Returns the Object ID of this object.
    
    \remarks
    Returns a null ID if this object has not been added to a database.
  */
  OdDbObjectId objectId() const;
  
  /** \details
    Returns the persistent handle of this database object.
  */
  OdDbHandle getDbHandle() const;
  
  /** \details
    Returns the persistent handle of this database object.
  */
  OdDbHandle handle() const 
  { return getDbHandle (); }
  
  /** \details
    Returns the Object ID of this object's owner.
    
    \remarks
    Returns a null ID if this object has not been added to a database,
    or if OdDbObject::setOwnerId has not been called.
    
    Throws:
    <table>
    Exception        Cause
    eNotInDatabase   objectId().isNull()
    </table>
  */
  OdDbObjectId ownerId() const;
  
  /** \details
    Sets this object's ownerId data member. 
    
    \param ownerId [in]  Owner's objectId.
    
    \remarks
    This function lets this object know its owner; it does not
    notify the owner. 
    
    Throws:
    <table>
    Exception             Cause
    eNotInDatabase        Owner is not in the database.
    </table>
  */
  virtual void setOwnerId(
    OdDbObjectId ownerId);
    
  /** \details
    Returns the OdDbDatabase that contains this object.
  */
  OdDbDatabase* database() const;
  
  /** \details
    Creates an OdDbDictionary extension dictionary of this object.

    \remarks
    If this object's extension dictionary has been erased, it will
    be unerased.  
    
    An object owns its extension dictionary.
  */
  void createExtensionDictionary();
  
  /** \details
    Returns the Object ID of this object's extension dictionary.  
    
    \remarks
    Returns a null ID if this object does not have an extension dictionary, 
    or if its extension dictionary has been erased.
  */
  OdDbObjectId extensionDictionary() const;
  
  /** \details
    Releases and erases this object's extension dictionary if it exists and is empty. 
    
    \remarks
    Returns true if and only if either the dictionary did not exist or was released.
  */
  bool releaseExtensionDictionary();  

  // Opens/creates/unerases Xdictionary, and retrieves/creates Xrecord for the given name
  //
  
  /** \details
    Creates or retrieves the specified Xrecord in this object's extension dictionary.

    \param xrecordName [in]  Xrecord name.
    \param style [in]  Merge style.
    
    \returns
    Returns a SmartPointer to the specified Xrecord.
    
    * If the extension dictionary has been erased, it is unerased.
    * If the extension dictionary does not exist, it is created.
    * If the specified Xrecord does not exist, it is created.
  */
  OdDbXrecordPtr createXrecord(
    const OdString& xrecordName, 
    OdDb::DuplicateRecordCloning style = OdDb::kDrcIgnore);

  /** \details
    Upgrades this object from OdDb::kForRead to OdDb::kForWrite.
  */
  void upgradeOpen();
  
  /** \details
    Downgrades this object from OdDb::kForWrite to OdDb::kForRead
     
    \remarks
    Any pending changes to this object are committed to the database. 
  */
  void downgradeOpen();
    
  /** \details
    Notification function called by the Teigha framework immediately before an object is opened. 

    \param mode [in]  Mode in which the object is being opened.
    
    \remarks
    This function is notified just before an object is to be opened; giving this function
    the ability to cancel the object being opened.
         
    Returns eOk if and only if open() is to continue.

    Overriding this function in a child class allows a child instance to be notified each time an
    object is opened.

    When overriding this function:
    
    1)  If the OdDbObject's state is incorrect, return 
        something other than eOk.
    
    2)  If the parent class's subOpen() returns anything
        other than eOk, immediately return it. 
    
    3)  If other actions are required before open, do them.
    
    4)  Return eOk.
    
    The default implementation of this function does nothing but return eOk.  This function can be
    overridden in custom classes.
  */
  virtual OdResult subOpen(
    OdDb::OpenMode mode);
    

  /** \details
    Called as the first operation as this object is being closed, for
    database -resident objects only. 
    
    \remarks
    This function is notified just before the current open operation is to be closed, giving this function
    the ability to perform necessary operatons.
    
    When overriding this function:
    
    1)  If the OdDbObject's state is incorrect, throw exception.
    
    2)  Call parent class's subClose().
    
    3)  If other actions are required before close, do them.
    
    The default implementation of this function does nothing.
    This function can be overridden in custom classes.
  */
  virtual void subClose();
  
  /** \details
    Sets the erased mode of this object. 
    
    \param eraseIt [in]  Boolean to specify if object is to be erased or unerased.
                
    \remarks
    Erased objects are not deleted from the database or from memory.
    
    Erased objects are not filed when the database is saved or sent to a DXF file.
  */
  OdResult erase(
    bool eraseIt = true);
  
  /** \details
    Called as the first operation as this object is being erased or unerased. 

    \param erasing [in]  A copy of the erasing argument passed to erase().

    \remarks
    This function is notified just before the current object is to be erased, giving this function
    the ability to cancel the erase.
         
    Returns eOk if and only if erase() is to continue.

    When overriding this function:
    
    1)  If the OdDbObject's state is incorrect, return 
        something other than eOk.
    
    2)  If the parent class's subErase() returns anything
        other than eOk, immediately return it. 
    
    3)  If other actions are required before erase, do them.
    
    4)  Return eOk.

    If you must make changes to this object's state, either make them after
    step 2, or roll them back if step 2 returns other than eOk. 
    
    The default implementation of this function does nothing but return eOk.  This function can be
    overridden in custom classes.
    
  */
  virtual OdResult subErase(
    bool erasing);
  
  /** \details
    Replaces this database -resident (DBRO) object with the specified non- database -resident (NDBRO) object,
    while retaining this object's objectId, handle, owner, and reactor list.
   
    \param pNewObject [in]  Pointer to the object with which to replace this object in the database.
    \param keepXData [in]  This object's XData will be retained if and only if keepXData is true.
    \param keepExtDict [in]  This object's extension dictionary will be retained if and only if keepExtDict is true.
                    
    \remarks
    This object must be open OdDb::kForWrite. 
    
    The replacement object will opened OdDb::kForWrite, and must be closed.
    
    It is up to the caller to delete the replaced (this) object.
    
    Throws:
    <table>
    Exception               Cause
    eIllegalReplacement     This object is NDBRO or NewObject is DBRO.
    </table>
  */
  void handOverTo(
    OdDbObject* pNewObject, 
    bool keepXData = true, bool 
    keepExtDict = true);
  
  /** \details
    Called as the first operation of the handOverTo function.  
    
    \param pNewObject [in]  Pointer to the object with which to replace this object in the database.

    \remarks
    This function allows custom classes to populate the new object.

    \remarks
    Overriding this function in a child class allows a child instance to be notified each time an
    object is handed over.
    
    This function is notified just before an object is to be handed over; giving this function
    the ability to cancel the handover.
         
    When overriding this function:
    
    1)  If the OdDbObject's state is incorrect, throw exception.
    
    2)  Call parent class's subHandover().
    
    3)  If other actions are required before handover, do them.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.

  */
  virtual void subHandOverTo(
    OdDbObject* pNewObject);
  
  /** \details
    Swaps the objectIDs, handles, XData,and extension dictionary between
    this object and another object.

    \param otherId [in]  Object ID of object with which to swap.
    \param swapXdata [in]  XData will be swapped if and only if swapXData is true.
    \param swapExtDict [in]  Extension dictionaries will be swapped if and only if swapExtDict is true.
    
    Throws:
    <table>
    Exception               Cause
    eIllegalReplacement     Either object is NDBRO.
    </table>
  */
  void swapIdWith(
    OdDbObjectId otherId, 
    bool swapXdata = false, 
    bool swapExtDict = false);
  
  /** \details
    Called as the first operation of swapIdWith

    \param otherId [in]  Object ID to be swapped with this object's Object ID.
    \param swapXdata [in]  If and only if true, extended data are swapped.
    \param swapExtDict [in]  If and only if true, extension dictionaries are swapped.

    \remarks
    This function is notified just before an object is to be opened; giving this function
    the ability to cancel the object being swapped or perform necessary actions.
         
    When overriding this function:
    
    1)  If the OdDbObject's state is incorrect, throw exception.
    
    2)  Call parent class's subSwapIdWith().
    
    3)  If other actions are required before swapping IDs, do them.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
 
  */
  virtual void subSwapIdWith(
    const OdDbObjectId& otherId, 
    bool swapXdata = false, 
    bool swapExtDict = false);
    
  /** \details
    Perform an audit operation on this object.

    \param pAuditInfo [in]  Pointer to an OdDbAuditInfo object.
    
    \remarks
    The specified OdDbAuditInfo object controls the audit, and receives the audit status.
    
    When overriding this function for a custom class, first call OdDbObject::audit(pAuditInfo) 
    to validate the audit operation.
  */
  virtual void audit(
    OdDbAuditInfo* pAuditInfo);
  
  /** \details
    Reads the .dwg file format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
    \remarks
    This function calls dwgInFields(pFiler),
    then loads any Xdata associated with this object.
  */
  void dwgIn(
    OdDbDwgFiler* pFiler);
  
  /** \details
    Writes the .dwg file format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    
    \remarks
    This function calls this object's dwgOutFields() function,
    writes loads any Xdata associated with this object.
  */
  void dwgOut(
    OdDbDwgFiler* pFiler) const;
  
  /** \details
    Reads the DXF format data of this object from the specified filer. 
    
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
    \returns
    Returns the filer status.

    This function calls this object's dxfInFields(pFiler),
    then loads any Xdata associated with this object.
  */
  virtual OdResult dxfIn(
    OdDbDxfFiler* pFiler);
  
  /** \details
    Writes the DXF format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    
    \remarks
    This function calls this object's dxfOutFields(pFiler) function,
    writes any Xdata associated with this object.
  */
  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;
  
  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \returns
    Returns the filer status.
    
    This function is called by dwgIn() to allow the object to read its data.

    When overriding this function:
     
    
    1)  Call assertWriteEnabled(). 
    2)  Call the parent class's dwgInFields(pFiler). 
    3)  If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned. 
    4)  Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the order they were written.
    5)  Return pFiler->filerStatus().
  */
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  
  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
    
    \remarks
   
    This function is called by dwgIn() to allow the object to write its data.

    When overriding this function:
     
    
    1)  Call assertReadEnabled(). 
    2)  Call the parent class's dwgOutFields(pFiler). 
    3)  Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items in the order they were written.
  */
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  
  /** \details
    Reads the DXF data of this object. 
    
    \param pFiler [in]  Pointer to the filer from which data are read.
    
    \returns
    Returns the filer status.
    
    This function is called by dxfIn() to allow the object to read its data.

    When overriding this function:
     
    
    1)  Call assertWriteEnabled(). 
    2)  Call the parent class's dwgInFields(pFiler). 
    3)  If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) returned. 
    4)  Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written.
    5)  Return pFiler->filerStatus().
  */
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  
  /** \details
    Writes the DXF data of this object.

    \param pFiler [in]  Pointer to the filer to which data are to be written.
    
    \remarks
    
    This function is called by dxfOut() to allow the object to write its data.

    When overriding this function:
     
    
    1)  Call assertReadEnabled(). 
    2)  Call the parent class's dxfOutFields(pFiler). 
    4)  Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items in the order they were written.
  */
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  /** \details
    Reads the DXF R12 format data of this object. 

    \param pFiler [in]  Pointer to the filer from which data are to be read.
    
    \returns
    Returns the filer status.

    This function is called by dxfIn() to allow the object to read its data.

    When overriding this function:
    
    1)  Call assertWriteEnabled(). 
    2)  Call the parent class's dxfInFields_R12(pFiler). 
    3)  If it returns eOK, continue; otherwise return whatever the parent's dxfOutFields_R12(pFiler) returned. 
    4)  Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written.
    5)  Return pFiler->filerStatus().
  */
  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);
  
  /** \details
    Writes the DXF data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are to be written.
    
    \returns
    Returns the filer status.

    This function is called by dxfOut() to allow the object to write its data.

    When overriding this function:
     
    
    1)  Call assertReadEnabled(). 
    2)  Call the parent class's dxfOutFields(pFiler). 
    3)  Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items in the order they were written.
  */
  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;
  
  /** \details
    Returns the merge style of this object.
  */
  virtual OdDb::DuplicateRecordCloning mergeStyle() const;
  
  /** \details
    Returns a linked list of ResBufs containing
    a copy of the XData of this object.

    \param regappName [in]  Registered application for which to return XData. 

    \remarks
    If regappName is empty, all XData of this object will be returned.

    If regappName is not empty, XData for only that application will be
    returned.

    Returns a null SmartPointer if there are no XData.
  */
  virtual OdResBufPtr xData(
    const OdString& regappName = OdString::kEmpty) const;

  
  /** \details
    Sets the XData of this object.
    
    \param pRb [in]  A pointer to the ResBuf list containing the XData.
    
    \remarks
    The sublist for each RegApp, and the ResBuf list itself, must begin with a ResBuf with 
    resbuf.restype == kDxfRegAppName and resbuf.rstring == a valid regAppName string. 
    
    Any existing XData for the specified regAppName will be replaced.
    
    To remove the regAppName and its XData, just provide the regAppName with no data. 
    
    If you override this method, you should supermessage this classes parent class
    to add add the XData to the object.
  */
  virtual void setXData(
    const OdResBuf* pRb);
     
  /** \details
    Returns true if and only if this object's erased status has been toggled since it was opened.
  */
  bool isEraseStatusToggled() const;
  
  /** \details
    Returns true if and only if this object is marked as erased.
  */
  bool isErased() const;
  
  /** \details
    Returns true if and only if this object is open OdDb::kForRead.
  */
  bool isReadEnabled() const;
  
  /** \details
    Returns true if and only if this object is open OdDb::kForWrite.
  */
  bool isWriteEnabled() const;
  
  /** \details
    Returns true if and only if this object is open OdDb::kForNotify.
  */
  bool isNotifyEnabled() const;
  
  /** \details
    Returns true if and only if this object's assertWriteEnabled() has been called since 
    it was opened.
  */
  bool isModified() const;

  /** \details
  */
//  void setModified (bool bModified);
  
  /** \details
    Returns true if and only this object's assertWriteEnabled() and setXData() have been called since it was opened.
  */
  bool isModifiedXData() const;
  
  /** \details
    Returns true if and only if an object derived from OdDbEntity has been modified.
    
    \remarks
    
    This function returns true if and only if an object derived from OdDbEntity
    either
    
        1) Calls assertWriteEnabled()
        
        2) Calls recordGraphicsModified(true).
  */
  bool isModifiedGraphics() const;
   
  /** \details
    Returns true if and only if this object has not been closed since it was created.
  */
  bool isNewObject() const;
  
  /** \details
    Returns true if and only if this object is sending notification.
  */
  bool isNotifying() const;
  
  /** \details
    Returns true if and only if this object is taking part in an Undo operation.
  */
  bool isUndoing() const;
  
//  bool isCancelling() const;

  /** \details
    Returns true if and only if a call to close would completely close this object at this time.
    
    \remarks
    Returns true if and only if a this object is open OdDb::kForRead with only one reader, and is not
    in a transaction.
  */
    bool isReallyClosing() const;
  
  /** \details
    Returns true if and only this object is a database -resident object.
  */
  bool isDBRO() const;
    
  /** \details
    Throws an exception if this object is not open OdDb::kForRead.
    
    \remarks
    This function should be used only inside member functions that do not modify this object. 
    It should be the first function called by these functions.
    
    Throws:
    <table>
    Exception             Cause
    eNotOpenForRead       !isReadEnabled()
    </table>
  */
  void assertReadEnabled() const;
  
  /** \details
    Throws an exception if this object is not open OdDb::kForWrite,
    and controls automatic undo and notification of modifications.
    
    \param autoUndo [in]  Specifies if automatic undo should be done.
    \param recordModified [in]  Specifies if graphics are to be updated, and
                        "openedForModify", "modified" and "modifiedGraphics"
                        notifications are to be sent. 
    
    \remarks
    This function should be used only inside member functions that modify this object. 
    It should be the first function called by these functions.
    
    Throws:
    <table>
    Exception             Cause
    eNotOpenForWrite     !isWriteEnabled()
    </table>
  */
  void assertWriteEnabled(
    bool autoUndo = true, 
    bool recordModified = true);
  
  /** \details
    Throws an exception if this object is not open OdDb::kForNotify.
    
    \remarks
    This function should be used only inside member functions that are used only when this object
    is open OdDb::kForNotify. It should be the first function called by these functions.
    
    Throws:
    <table>
    Exception            Cause
    eInvalidOpenState    !isNotifyEnabled()
    </table>
  */
  void assertNotifyEnabled() const;
  
  /** \details
    Controls the undo recording of this object in OdDbDatabase.

    \param disable [in]  Boolean to control undo recording.
    
    \remarks
    Disabling undo recording does not erase the undo recording; it merely suspends it.
    Undo recording is initially off for newly created OdDbDatabase objects.
  */
  void disableUndoRecording(
    bool disable);
  
  /** \details
    Returns the undo filer associated with this object.
    
    \remarks
    This function is typically used by custom classes using partial Undo
    to add Undo information to the filer that would be used by this
    object applyPartialUndo().
    
    Throws:
    <table>
    Exception           Cause
    eNotOpenForWrite    !isWriteEnabled()
    </table>
  */
  OdDbDwgFiler* undoFiler();
  
  /** \details
    Notification function called each time an Undo operation is performed 
    this object is using partial Undo.
    
    \param pUndoFiler [in]  A pointer to the undo filer with the partial undo information.
    \param pClassObj [in] A pointer to the OdRxClass object for the class that will handle the Undo.
    
    \remarks
    An object indicates it's using the partial undo mechanism, if and only if
    it has set autoUndo false in all calls to assertWriteEnabled().
    
    This member function must know which types of fields to scan, 
    and must stop after reading what it it needs.

    If the class type specified by pClassObj does not matches the class of this object,
    this member function must call the parent class's applyPartialUndo() 
    and return whatever it returns.

    If it does match the class of this object, this member function must use pUndoFiler to read the undo data, 
    then typically use this object's set() method.
    
    Throws:
    <table>
    Exception                   Cause
    eNotThatKindOfClass         pClassObj != OdDbObject::desc()
    </table>
  */
  virtual void applyPartialUndo(
    OdDbDwgFiler* pUndoFiler, 
    OdRxClass* pClassObj);
  
  /** \details
    Adds the specified transient reactor to this object's reactor list.

    \param pReactor [in]  Pointer to the transient reactor object.

    \remarks
    An object must be open either OdDb::kForRead or OdDb::kForWrite in order to add a transient reactor.
  */
  void addReactor(
    OdDbObjectReactor* pReactor) const;
  
  /** \details
    Removes the specified transient reactor from this object's reactor list.

    \param pReactor [in]  Pointer to the transient reactor object.

    \remarks
    An object must be open either OdDb::kForRead or OdDb::kForWrite in order to remove a transient reactor.
  */
  void removeReactor(
    OdDbObjectReactor* pReactor) const;
  
  /** \details
    Adds the specified persistent reactor to this object's reactor list.

    \param objId [in]  Object ID of the persistent reactor.

    \remarks
    An object must be open OdDb::kForWrite in order to add a persistent reactor.

    If the persistent reactor does not have an owner, 
    it isn't saved with the drawing. Non-graphical objects used as persistent 
    reactors are typically stored in a dictionary in the Named Objects Dictionary 
    or in an extension dictionary associated with some object.
  */
  virtual void addPersistentReactor(
    const OdDbObjectId& objId);
  
  /** \details
    Removes the specified persistent reactor from this object's reactor list.

    \param objId [in]  Object ID of the persistent reactor.

    \remarks
    An object must be open OdDb::kForWrite in order to remove one of its persistent reactors.
  */
  virtual void removePersistentReactor(
    const OdDbObjectId& objId);
  
  /** \details
    This method returns true if objId is the Object ID of a reactor attached to this object. Otherwise, it returns false.

    \param objId [in]  Object ID of the persistent reactor.
  */
  bool hasPersistentReactor(
    const OdDbObjectId& objId) const;

  /** \details
    Returns Object IDs of this object's persistent reactors.
  */
  OdDbObjectIdArray getPersistentReactors() const;

  /** \details
    Returns this object's transient reactors.
  */
  OdDbObjectReactorArray getTransientReactors() const;
  
  /** \details
    Allows a subobject of a complex object to notify its root object that it has been changed. 
    
    \param pSubObj [in]  Pointer to the modifiedsubobject.

    \remarks
    Here's how it's supposed to work:
    
    1)  The subobject class's close() calls its triggers a "modified" notification which
        calls its xmitPropagateModify().
    2)  The subobject class calls the its parent's recvPropagateModify() its object's pointer.
    3)  The owner's class's recvPropagateModify() sends a "modified" notification to the top of its class.
    
    The default implementation of this function does nothing. This function can be overridden in custom classes.
  */
  virtual void recvPropagateModify(
    const OdDbObject* pSubObj);
  
  /** \details
    Allows a subobject of a complex object to notify its root object that it has been changed. 

    \remarks
    This function must be called within an subobject's close() method.
    
    The owner can then propagate the notification that it has been been modified.
    
    The default implementation of this function inform the database 
    in which the owner resides has been modified; triggering any
    OdDbDatabaseReactors attached to it. This function can be
    overridden in custom classes.

    When overriding this function, it should: 
   
    \remarks
    Here's how it's supposed to work:
    
    1)  The subobject class's close() calls its triggers a "modified" notification which
        calls its xmitPropagateModify().
    2)  The subobject class calls the its owner's recvPropagateModify() its object's pointer.
    3)  This owner's class's recvPropagateModify() sends a "modified" notification to the top of its class.
  */
  virtual void xmitPropagateModify() const;
  
  /** \details
    Performs a deep clone of this object.
    
    \param ownerIdMap [in]  Owner's ID map.

    \remarks
    Returns a SmartPointer to the newly created clone,
    and adds a record to the specified ID map. 

    If the cloning operation fails, a null SmartPointer is returned.
    
    A deep clone is a clone of this object and everything it owns.
    
    This function should not be called by client code; use OdDbDatabase::deepCloneObjects() instead.

    This function can be overridden in custom classes.

    The default implementation of this function appends the cloned object to the specified owner object.   
  */
  ODRX_SEALED_VIRTUAL OdDbObjectPtr deepClone(OdDbIdMapping& ownerIdMap, OdDbObject* pOwner, bool bPrimary = true) const ODRX_SEALED;
  
  /** \details
    Performs a shallow clone of this object. 
    
    \param ownerIdMap [in/out] Owner's ID map.

    \remarks
    Returns a SmartPointer to the newly created clone,
    and adds a record to the specified ID map. 
    If the cloning operation fails, a null SmartPointer is returned.
   
    A shallow clone is a clone of only this object.
    
    This function should not be called by client code; use OdDbDatabase::wblockCloneObjects() instead.

    The default implementation of this function calls subWblockClone() for all objects hard-referenced by this object.
    This function can be overridden in custom classes.
    
    \sa
    subDeepClone()

  */
        ODRX_SEALED_VIRTUAL OdDbObjectPtr wblockClone(OdDbIdMapping& ownerIdMap, OdDbObject* pOwner, bool bPrimary = true) const ODRX_SEALED;
  
  
  /** \details
    This function appends this object to the specified owner object.
    
    \param idPair [in]  ID pair to append.
    \param pOwnerObject [in]  Pointer to the owner object.
    \param ownerIdMap [in/out] Owner's ID map.
    
    \remarks
    Adds a record to the specified ID map.     
    
    This function is used internally to subDeepClone() and subWblockClone().
    
    Throws:
    <table>
    Exception              Cause
    eInvalidOwnerObject    !pOwnerObject->get()
    </table>
    
  */
  virtual void appendToOwner(
    OdDbIdPair& idPair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap);

  /** \details
    Called on a new created cloned object to indicate that its Object ID is not valid. 
    
    \remarks
    The flags are cleared when the OdDbObject::subDeepClone or OdDbObject::subWblockClone() operation has been completed.
  */
  void setOdDbObjectIdsInFlux();
  
  /** \details
      Returns true if and only if this object's Object ID is not valid because
      the OdDbObject::subDeepClone or OdDbObject::subWblockClone() have yet to be completed.
  */
  bool isOdDbObjectIdsInFlux() const;
  
  /** \details
    Notification function called whenever the notifying object has had its clone() member function called. 
    
    \param pObject [in]  Pointer to the notifying object.
    \param pNewObject [in]  Pointer to the object resulting from the copy.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void copied(
    const OdDbObject* pObject, 
    const OdDbObject* pNewObject);
  

  /** \details
    Notification function called whenever an object has been erased or unerased.

    \param pObject [in]  Pointer to the object that was erased/unerased.
    \param erasing [in]  True if and only if this object is being erased.
    
    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void erased(
    const OdDbObject* pObject, 
    bool erasing = true);
  
  /** \details
    Notification function called just before an object is deleted from memory. 

    \param pObject [in]  Pointer to the object that is being deleted.

    \remarks
    An object is deleted when its reference count reaches 0.

    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void goodbye(
    const OdDbObject* pObject);
    
  /** \details
    Notification function called whenever an object is opened for modify OdDb::kForWrite.

    \param pObject [in]  Pointer to the object that is being opened.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void openedForModify(
    const OdDbObject* pObject);
  
  /** \details
      Notification function called whenever an object is opened OdDb::kForWrite, a function
      has been called that could modify the contents of this object, and this object is now being
      closed.

      \param pObject [in]  Pointer to the object that is being closed after being modified.

      \remarks
      The default implementation of this function does nothing.  This function can be
      overridden in custom classes.
  */
  virtual void modified(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever a method of the
    specified subobject has called assertWriteEnabled()
    
    \param pObject [in]  Pointer to the notifying object.
    \param pSubObj [in]  A pointer to the subobject owned by pObject.
    
    \remarks
    Notification is made upon pObject->close() or pObject->cancel(). 
 
    \remarks
    This function is called only for the modification of the following:
       
    * Vertices of OdDb2dPolylines, OdDb3dPolylines, OdDbPolygonMeshs, and OdDbPolyFaceMeshes
    * OdDbFaceRecords of OdDbPolyFaceMeshs 
    * OdDbAttributes owned by OdDbBlockReferences, classes derived from OdDbBlockReference, and OdDbMInsertBlocks
  */
  virtual void subObjModified(
    const OdDbObject* pObject, 
    const OdDbObject* pSubObj);
  
  /** \details
    Notification function called whenever the notifying
    object is in the midst an Undo operation that
    is undoing modifications.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    The notifying object is open OdDb::kForRead.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void modifyUndone(
    const OdDbObject* pObject);
  
  /** \details
    Notification function called whenever the XData has been written
    to the notifying object.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    Notification is made upon pObject->close() or pObject->cancel(). 

    The notifying object is open OdDb::kForRead.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void modifiedXData(
    const OdDbObject* pObject);
  
  /** \details
    Notification function called whenever the Undo process
    processes the appending of the notifying object to the database.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    The notifying object is marked as erased. It is not removed the database, and can be unerased,
    even 'before' it was created. 
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void unappended(
    const OdDbObject* pObject);
  
  
  /** \details
    Notification function called whenever a Redo process
    processes the reappending of the notifying object to the database.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void reappended(
    const OdDbObject* pObject);
  
  /** \details
    Returns true if and only if this object is a proxy object or entity.
  */
  bool isAProxy() const;  

  /** \details
    Notification function called immediately before an object is closed.

    \param objectId [in]  Object ID of the object that is being closed.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void objectClosed(
    const OdDbObjectId& objectId);
  
  
  /** \details
    Notification function called whenever an object derived from OdDbEntity is modified.
    
    \param pObject [in]  Pointer to the modified object.
    
    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
    
    This function is called whenever the following has occurred.
    
    1)  The calling object is opened OdDb::kForWrite.
    
    2)  One of its member functions either
    
        a Calls its assertWriteEnabled with recordModified == true.
        
        b Calls its recordGraphicsModified(true).
    
    3) The calling object is being closed.
    
    This function is called whenever the object as been modified. It therefore
    indicates only that the graphics for it may have changed.
            
  */
  virtual void modifiedGraphics(
    const OdDbObject* pObject);
    
  /** \details
    Copies the contents of the specified object into this object when possible.

    \param pSource [in]  Pointer to the source object.

    \remarks
    The source object and this object need not be of the same type.  
    This function is meaningful only when implemented by custom classes.
    
    Throws:
    <table>
    Exception           Cause
    eNullObjectPointer  pSource->isNull()
    </table>
  */
  virtual void copyFrom(
    const OdRxObject* pSource);

  /** \details
    Returns true if and only if this object has its
    bit set to override the filer version.
  */
       bool hasSaveVersionOverride() const;
  
  /** \details
    Controls the flag specifying that this object
    overrides the save filer version.
    
    \param hasSaveVersionOverride [in]  Has save version override.
               
    \remarks
    By default, objects are saved in the highest of the your object's birth version and the filer version.
  */
       void setHasSaveVersionOverride(
    bool hasSaveVersionOverride);
  
  /** \details
    Returns the drawing and maintenance release version into which this object must be stored.

    \param pFiler [in]  Pointer to the DWG/DXF filer to be used.
    \param pMaintVer [out]  Receives the maintenance version. 

    \remarks
    The default implementation of this function returns filer->dwgVersion().  This function can be
    overridden in custom classes.
    
    Do not use filer->dwgVersion() with dwg/dxf(in/out)Fields(); use self()->getObjectSaveVersion() instead. 
  */
  virtual OdDb::DwgVersion getObjectSaveVersion(
    const OdDbFiler* pFiler,
    OdDb::MaintReleaseVer* pMaintVer = 0) const;
      
  /** \details
    Determines the behavior for custom objects when saving to .dwg or .dxf file. 
    
    \param ver [in]  Drawing version to save as.
    \param replaceId [out]  Object ID of the object replacing this object.
    \param exchangeXData [out]  Set to true if and only if this function did not add XData to the replacement object.

    \remarks
    This function either
    
    * Returns an OdDbObjectPtr for a non- database -resident (NDBRO) replacement object, setting replaceId to OdDbObjectId::kNull.
    * Returns NULL, setting replaceId for a database -resident (DBRO) replacement object.
    
    Custom objects can decompose themselves into other objects, adding additional XData as required. Teigha 
    transfers XData from this object to the replacement object if and only if exchangeXData is true.
    
    The default implementation returns NULL and sets replaceId to OdDbObjectId::kNull.  This function can be
    overridden in custom classes.

    The method is not intended to be called by client code. For processing owned objects use *convertForSave()*
    method.
  */
  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);

  /** \details
    Determines the behavior for custom objects when saving to .dwg or .dxf file. 
    
    \param format [in]  File type.
    \param ver [in]  Drawing version to save as.
    \param replaceId [out]  Object ID of the object replacing this object.
    \param exchangeXData [out]  Set to true if and only if this function did not add XData to the replacement object.

    \remarks
    This function either
    
    * Returns an OdDbObjectPtr for a non- database -resident (NDBRO) replacement object, setting replaceId to OdDbObjectId::kNull.
    * Returns NULL, setting replaceId for a database -resident (DBRO) replacement object.
    
    Custom objects can decompose themselves into other objects, adding additional XData as required. Teigha 
    transfers XData from this object to the replacement object if and only if exchangeXData is true.
    
    The default implementation returns NULL and sets replaceId to OdDbObjectId::kNull.  This function can be
    overridden in custom classes.

    The method is not intended to be called by client code. For processing owned objects use *convertForSave()*
    method.
  */
  virtual OdDbObjectPtr decomposeForSave(
    OdDb::SaveType format, 
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);

  /** \details
    The method should be called from *decomposeForSave()* of custom container objects for owned child objects.
    It calls child's decomposeForSave() method plus performs other required actions
    (handOverTo, exchanging XData, etc.)
    
    \param ver [in]  Drawing version to save as.

    \remarks
  */
  void convertForSave(OdDb::DwgVersion ver);

  /** \details
    After loading from file, performs necessary actions which require accessing other Database objects.
    For example, processing round-trip data.
    
    \param format [in]  File type.
    \param version [in]  Drawing version of file loaded.
    \param pAuditInfo [in]  Pointer to an OdDbAuditInfo object. Can be Null. Not Null if drawing is being loaded in Recover mode.
    If so, audit() will be called later.

    \remarks
    If an object in its dwgInFields() method accesses other database objects, it's non-thread-safe.
    Such objects can't be loaded in MT mode. To enable MT loading of custom objects, its dwgInFields()
    should not access other database objects. Such actions should be performed in composeForLoad().
    composeForLoad() is called in single-thread mode after all thread-safe objects are loaded (their dwgInFields()
    were called in MT mode).

    Being overridden in custom classes, the function first should call the parent class method.
    Parent class implementation takes care of parent class round-trip data and other actions which require
    access to other objects.
  */
  virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

  /** \details
    Returns a pointer to the OdGiDrawable for the object. If the object doesn't have an associated OdGiDrawable object, this function returns NULL. 
  */
  virtual OdGiDrawable* drawable();

  /** \details
    Sets the values of this object's subentity traits, and returns with the calling object's subentity traits.

    \param pTraits [in]  Pointer to the OdGiDrawableTraits object to be set.
    
    \remarks
    When overriding subSetAttributes(), you must OR (|) the return value 
    of <base class>::subSetAttributes(pTraits) with any flags you add. 
    
    A derived class may not remove flags for any reason.
    
    The default implementation does nothing but returns kDrawableNone. This function can be
    overridden in custom classes.
  */
  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;
  
  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;
  
  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;
  
  /** \details
    Return true if and only if this object is database -resident (DBRO).
    
    \remarks
    Persistent objects belong to an object that must be opened and closed.

    Non-persistent objects can be accessed through their pointers.
  */
    virtual bool isPersistent() const;
  
  /** Returns the database ID corresponding to this object.
  */
  virtual OdDbStub* id() const;
    
  /** \details
    Returns the CLSID value associated with this object.
    
    \param pClsid [out]  Pointer to the CLSID value.
    
    \remarks
    Valid only on Windows with Teigha built as a DLL.
  */
  ODRX_SEALED_VIRTUAL OdResult getClassID(void* pClsid) const ODRX_SEALED;

  // Override of OdGiDrawable
  
  /** \details
    Assigns the specified OdGsCache to the calling object.

    \param pNode [in]  Pointer to the OdGsCache object.
  */
  void setGsNode(OdGsCache* pNode);

  // Override of OdGiDrawable
  
  /** \details
    Returns the OdGsCache of an OdGiDrawable object.
  */
  OdGsCache* gsNode() const;

  /*
    void upgradeFromNotify (bool& wasWritable);
    void downgradeToNotify (bool wasWritable);
    OdResult closeAndPage (bool onlyWhenClean = true);
    virtual void swapReferences (const OdDbIdMapping& idMap);
    virtual OdGiDrawable* drawable ();
    OdDbObjPtrArray* reactors (); 
    virtual OdRxObjectPtr clone (OdDbIdMapping& ownerIdMap) const;
  */
  
  /** \details
    Applies the 3D transformation matrix to the XData of this object.
    
    \param xfm [in]  3D transformation matrix.
    
    \remarks
    Applies the transformation matrix to only the following XData data types:
    
    <table>
    Name                Value
    kDxfXdWorldXCoord   1011   
    kDxfXdWorldYCoord   1021   
    kDxfXdWorldZCoord   1031   
    kDxfXdWorldXDisp    1012   
    kDxfXdWorldYDisp    1022   
    kDxfXdWorldZDisp    1032   
    kDxfXdWorldXDir     1013   
    kDxfXdWorldYDir     1023   
    kDxfXdWorldZDir     1033   
    kDxfXdDist          1041   
    kDxfXdScale         1042   
    </table>
  */
  void xDataTransformBy(
    const OdGeMatrix3d& xfm);

  /** \details
    Returns true if and only if this object has fields.
  */
  bool hasFields() const;
  
  /** \details
    Returns, and optionally opens, the specified field object from the field dictionary of this object.
    
    \param fieldName [in]  Name (key) for the new entry.
  */
  OdDbObjectId getField(
    const OdString& fieldName) const;
    
  /** \details
    Returns, and optionally opens, the specified field object from the field dictionary of this object.
    
    \param fieldName [in]  Name (key) for the new entry.
    \param mode [in]  Open mode.
  */
  OdDbObjectPtr getField(
    const OdString& fieldName, 
    OdDb::OpenMode mode) const;

  /** \details
    Adds the specified field to the field dictionary of this object.
    
    \param fieldName [in]  Name (key) for the new entry.
    \param pField [in]  Pointer to the field object.
    
    \returns
    Returns the Object ID of the new entry.
  */
  virtual OdDbObjectId setField(
    const OdString& fieldName, 
    OdDbField* pField);

  /** \details
    Removes the specified field from the field dictionary of this object.
    
    \param fieldId [in]  Object ID for the field.
    
    \remarks
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult removeField(
    OdDbObjectId fieldId);
    
  /** \details
    Removes the specified field from the field dictionary of this object.
    
    \param fieldName [in]  Name (key) for the entry.
    
    \remarks
    Returns the Object ID of the removed field if successful, or a null Object ID if not.
  */
  virtual OdDbObjectId removeField(
    const OdString& fieldName);

  /** \details
    Returns the field dictionary of this Object.
  */
  OdDbObjectId getFieldDictionary() const;
  
  /** \details
    Returns, and optionally opens, the field dictionary of this Object.
    \param mode [in]  Open mode.
  */
  OdDbObjectPtr getFieldDictionary(
    OdDb::OpenMode mode) const;


  /** \details
      For Teigha internal use only
  */
  
  /*!DOM*/
  virtual OdRxClass* saveAsClass(
    OdRxClass* pClass) const;

protected: // overridables
  virtual OdResult subGetClassID(void* pClsid) const;
  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& ownerIdMap, OdDbObject* pOwner, bool bPrimary) const;
  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& ownerIdMap, OdDbObject* pOwner, bool bPrimary) const;

  friend class OdDbSystemInternals;
  friend class OdDbObjectOverrule;
  friend class OdDbPropertiesOverrule;
  friend class OdDbObjectImpl;
  OdDbObjectImpl* m_pImpl;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for all objects contained 
  in the database.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbObject> OdDbObjectPtr;


/** \details
    Returns the data for the specified object.
        
    \param id [in]  Object ID of the object.
    \param pObj [in]  Pointer to the object.
    \param regapps [in]  Registered application name to retrieve extended data for. 
                May be a wildcard or comma separated list. If empty - no XData is returned.
    
    \remarks
    Each OdResBuf in the list contains a single DXF group code and data value pertaining 
    to the object.
    
    The object is opened in kForRead mode, and the returned list of data
    is independent of the original object (it may be modified or deleted without 
    affecting the original object).  To affect changes in the original object
    via the returned OdResBuf data, oddbEntMod must be called.

    throws:
    An OdError exception will be thrown if the passed in object cannot be opened.
*/
DBIO_EXPORT OdResBufPtr oddbEntGet( const OdDbObject* pObj, const OdString& regapps = OdString::kEmpty );

inline OdResBufPtr oddbEntGet( const OdDbObjectId& id, const OdString& regapps = OdString::kEmpty )
{
  return oddbEntGet(id.safeOpenObject(), regapps);
}


/** \details
    Sets the data for the specified object.
        
    \param id [in]  Object ID of the object.
    \param pObj [in]  Pointer to the object.
    
    \remarks
    Each OdResBuf in the list contains a single DXF group code and data value pertaining 
    to the object.
    
    throws:
    An OdError exception will be thrown if the passed in object cannot be opened in kForWrite
    mode.
*/
DBIO_EXPORT OdResult oddbEntMod( OdDbObject* pObj, OdResBuf* pRb );

inline OdResult oddbEntMod( const OdDbObjectId& id, OdResBuf* pRb )
{
  return oddbEntMod(id.safeOpenObject(OdDb::kForWrite), pRb);
}

TOOLKIT_EXPORT OdDbObjectId oddbEntNext(OdDbObjectId id, OdDbDatabase* db);
TOOLKIT_EXPORT OdDbObjectId oddbEntLast(OdDbDatabase* db);

DBIO_EXPORT OdResult oddbEntMake(OdDbDatabase* pDb, OdResBuf* pRb, OdDbObjectPtr& pObj);
DBIO_EXPORT OdResult oddbEntMakeX(OdDbDatabase* pDb, OdResBuf* pRb, OdDbObjectPtr& pObj);



#include "TD_PackPop.h"

#endif //_ODDBXOBJECT_INCLUDED_


