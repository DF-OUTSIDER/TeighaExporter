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




#ifndef _ODDBSYMBOLTABLE_INCLUDED
#define _ODDBSYMBOLTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbObject.h"

class OdDbSymbolTableRecord;
class OdDbSymbolTableIterator;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSymbolTableIterator object pointers.
*/
typedef OdSmartPtr<OdDbSymbolTableIterator> OdDbSymbolTableIteratorPtr;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSymbolTableRecord object pointers.
*/
typedef OdSmartPtr<OdDbSymbolTableRecord> OdDbSymbolTableRecordPtr;


/** \details
    This class implements OdError objects with Result codes of eDuplicateRecordName errors, 
    and the Object IDs of the objects related to the errors.
    
    Corresponding C++ library: TD_Db
    <group Error_Classes> 
*/
class TOOLKIT_EXPORT OdError_DuplicateRecordName : public OdError
{
public:
  /** \param existingRecordId [in]  Object ID of the existing record.
  */
  OdError_DuplicateRecordName(   
    OdDbObjectId existingRecId);
  /** \details
    Returns the Object ID of the existing record.
  */
  OdDbObjectId existingRecordId() const;
};


/** \details
  <group OdDb_Classes>

  This class implements the predefined table object, which represents a container for  
  storing and accessing of named record objects in the database.

  \sa
  TD_Db

  \sa
  <link db_datasheet_sample_root.html, Example of Selecting a Predefined Table Object>

  <link db_database_object_sample_about.html, Example of Using the Record-Table and Dictionary Interfaces for Getting Information about Objects>  

  \sa
  <link db_datasheet.html, Working with Predefined Tables of Named Records>

  OdDbSymbolTableRecord class
*/
class TOOLKIT_EXPORT OdDbSymbolTable : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSymbolTable);

  /** \details
    Builds an instance of the predefined table object.
    
    Applications typically will not use this constructor, insofar as the database object 
    creates own instance of all predefined tables.
  */
  OdDbSymbolTable();

  /** \details
    Searches the named record object in the predefined table object using the
    specified name and opnes it in the specified mode.

    \param sName [in]  Record name as a String value up to 255 characters long.
    \param openMode [in]  Mode in which to open the record.
    \param getErasedRecord [in]  If and only if true, erased records will be opened or retrieved.

    \returns
    Returns a SmartPointer to the object if successful, otherwise a null SmartPointer.

    \sa
    <link db_datasheet_get_check.html, Getting and Checking Records>
  */
  virtual OdDbSymbolTableRecordPtr getAt(
    const OdString& sName,
    OdDb::OpenMode openMode, 
    bool getErasedRecord = false) const;

  /** \details
    Searches the named record object in the predefined table object using the
    specified name.

    \param sName [in]  Record name as a String value up to 255 characters long.
    \param getErasedRecord [in]  If and only if true, erased records will be opened or retrieved.

    \sa
    <link db_datasheet_get_check.html, Getting and Checking Records>
  */
  virtual OdDbObjectId getAt(
    const OdString& sName, 
    bool getErasedRecord = false) const;


  /** \details
    Determines whether the named record object exists in the predefined table object
    using the specified name, and returns True if and only if the predefined table
    object contains the specified named record object.

    \param sName [in]  Record name as a String value up to 255 characters long.

    \sa
    <link db_datasheet_get_check.html, Getting and Checking Records>
  */
  virtual bool has( const OdString& sName ) const;
  
  /** \details
    Determines whether the named record object exists in the predefined table object
    using the specified object ID, and returns True if and only if the predefined table
    object contains the specified named record object.

    \param objectId [in]  Object ID.

    \sa
    <link db_datasheet_get_check.html, Getting and Checking Records>
  */
  virtual bool has( const OdDbObjectId& objectId ) const;


  /** \details
    Returns an Iterator object that can be used to traverse through record objects
    in the predefined table object.

    \param atBeginning [in]  True - to start at the beginning, or False - to start at the end. 
    \param skipDeleted [in]  True - to iterate only unerased records, or False - to iterate 
                    through unerased and erased records.

    \sa
    <link db_datasheet_iterate.html, Iterating through Records>
  */
  virtual OdDbSymbolTableIteratorPtr newIterator( 
    bool atBeginning = true, 
    bool skipDeleted = true) const;


  /** \details
    Adds the specified named record object to the predefined table object and 
    returns the Object ID of the added record object.

    \param pRecord [in]  Pointer to the existing named record object to be added.

    \remarks
    The record object must be created and must be named before adding. Use the  
    static pseudo-constructor createObject() for creating an instance of the 
    specific record. The predefined table object excludes duplicates of records.

    \sa
    <link db_datasheet_add_delete.html, Adding, Naming, Deleting, Recovering Records>

    OdError_DuplicateRecordName class
  */
  virtual OdDbObjectId add( OdDbSymbolTableRecord* pRecord );


  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;
};

/** \details
  The typified smart pointer for the predefined table object. This template class is specialization 
  of the OdSmartPtr class for the OdDbSymbolTable object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSymbolTable> OdDbSymbolTablePtr;



class OdDbSymbolTableIteratorImpl;

/** \details
    This class implements bidirectional Iterators for OdDbSymbolTable objects in an OdDbDatabase instance.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSymbolTableIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSymbolTableIterator);

  /** \details
    Sets this Iterator object to reference the SymbolTableRecord that it would normally return first.
     
    \remarks
    Allows multiple traversals of the iterator list.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipErased [in]  If and only if true, erased records are skipped.
  */
    virtual void start(
    bool atBeginning = true, 
    bool skipErased = true);

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() const;

  /** \details
    Returns the Object ID of the record currently referenced by this Iterator object.
  */
  virtual OdDbObjectId getRecordId() const;

  /** \details
    Opens the record currently referenced by this Iterator object.

    \param openMode [in]  Mode in which to open the record.
    \param openErasedRecord [in]  If and only if true, erased records will be opened.

    \returns
    Returns a SmartPointer to the opened record if successful, otherwise a null SmartPointer.
  */
  virtual OdDbSymbolTableRecordPtr getRecord(
    OdDb::OpenMode openMode = OdDb::kForRead,
    bool openErasedRecord = false) const;

  /** \details
    Steps this Iterator object.

    \param forward [in]  True to step forward, false to step backward.
    \param skipErased [in]  If and only if true, erased records are skipped.
  */
  virtual void step(
    bool forward = true, 
    bool skipErased = true);

  /** \details
    Positions this Iterator object at the specified record.
    \param objectId [in]  Object ID of the record.
  */
    virtual void seek(
    const OdDbObjectId& ObjectId);
    
  /** \details
    Positions this Iterator object at the specified record.
    \param pRecord [in]  Pointer to the record.
  */
  virtual void seek(
    const OdDbSymbolTableRecord* pRecord);

  virtual ~OdDbSymbolTableIterator();
protected:
  friend class OdDbSymbolTable;

  OdDbSymbolTableIterator();
  OdDbSymbolTableIterator(
    OdDbSymbolTableIteratorImpl* pImpl);

  OdDbSymbolTableIteratorImpl* m_pImpl;
};

#include "TD_PackPop.h"

#endif // _ODDBSYMBOLTABLE_INCLUDED

