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




#ifndef _ODDBBLOCKTABLE_INCLUDED
#define _ODDBBLOCKTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTable.h"
#include "DwgDeclareMembers.h"

class OdDbBlockTableRecord;

/** \details
    This class implements bidirectional Iterator objects that traverse entries in OdDbBlockTable objects in an OdDbDatabase instance.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlockTableIterator : public OdDbSymbolTableIterator
{
public:
  
  ODRX_DECLARE_MEMBERS(OdDbBlockTableIterator);
  
protected:

  OdDbBlockTableIterator();
    
  OdDbBlockTableIterator(
    OdDbSymbolTableIteratorImpl* pImpl);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbBlockTableIterator object pointers.
*/
typedef OdSmartPtr<OdDbBlockTableIterator> OdDbBlockTableIteratorPtr;


/** \details
    This class implements the BlockTable, which represents block definitions in an OdDbDatabase instance.

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlockTable : public OdDbSymbolTable
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbBlockTable);

  /** \details
    Teigha applications typically will not use this constructor, insofar as 
    the OdDbDatabase class creates its own instance.
  */
  OdDbBlockTable();

  /** \details
    Searches the named record object in the predefined table object using the 
    specified name and opens it in the specified mode.
    
    \param recordName [in] Record name. 
    \param openMode [in] Mode in which to open the record. 
    \param getErasedRecord [in] If and only if true, erased records will be opened or retrieved. 
    
    \returns 
    A smart pointer to the object if successful, otherwise a null smart pointer
  */
  virtual OdDbSymbolTableRecordPtr getAt(
    const OdString& recordName,
    OdDb::OpenMode openMode, 
    bool getErasedRecord = false) const;

  /** \details
    Searches the named record object in the predefined table object using the 
    specified name and opens it in the specified mode.
    
    \param recordName [in] Record name. 
    \param openMode [in] Mode in which to open the record. 
    \param getErasedRecord [in] If and only if true, erased records will be opened or retrieved. 
    
    \returns 
    A smart pointer to the object if successful, otherwise a null smart pointer
  */
  virtual OdDbObjectId getAt(
    const OdString& recordName, 
    bool getErasedRecord = false) const;

  /** \details
    Determines whether the named record object exists in the predefined table object 
    using the specified name.
    
    \param recordName [in] Record name. 
    
    \returns 
    Returns true if and only if the predefined table object contains the specified named record object.
  */
  virtual bool has(const OdString& recordName) const;

  /** \details
    Determines whether the named record object exists in the predefined table object 
    using the specified object ID.
      
    \param objectId [in] Object ID. 
    
    \returns 
    Returns true if and only if the predefined table object contains the specified record object.
  */  
  virtual bool has(const OdDbObjectId& objectId) const;

  /** \details
    Returns an Iterator object that can be used to traverse through record objects in the 
    predefined table object.
      
    \param atBeginning [in] true - to start at the beginning, or false - to start at the end.
    \param skipDeleted [in] true - to iterate only unerased records, or false - to iterate through 
    unerased and erased records.   
  */
  OdDbSymbolTableIteratorPtr newIterator( 
    bool atBeginning = true, 
    bool skipDeleted = true) const;

    /** \details
    Adds the specified named record object to the predefined table object.
      
    \param pRecord [in] Pointer to the existing named record object to be added.  

    \returns 
    Returns the Object ID of the added record object.
    
    \remarks
    The record object must be created and must be named before adding. Use the static 
    pseudo-constructor createObject() for creating an instance of the specific record. 
    The predefined table object excludes duplicates of records.
  */
  virtual OdDbObjectId add(OdDbSymbolTableRecord* pRecord);

  /** \details
    Returns the Object ID of the Model Space record within this BlockTable object.
  */
  const OdDbObjectId& getModelSpaceId() const;

  /** \details
    Returns the Object ID of the Paper Space record within this BlockTable object.
  */
  const OdDbObjectId& getPaperSpaceId() const;

  /** \details
    Reads the .dwg file data of this object.
    
    \param pFiler [in]  Filer object from which data are read.
    
    \remarks 
    Returns the filer status. 
    This function is called by dwgIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned. 
    4) Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in
    the order they were written. 
    5) Return pFiler->filerStatus().
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object.
    
    \param pFiler [in]  Pointer to the filer to which data are written.
    
    \remarks 
    This function is called by dwgIn() to allow the object to write its data. 
    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dwgOutFields(pFiler). 
    3) Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items 
    in the order they were written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the DXF format data of this object from the specified filer.    
    
    \param pFiler [in] Pointer to the filer from which the data are to be read.      
    
    \remarks
    Returns the filer status. 
    This function calls this object's dxfInFields(pFiler), then loads any Xdata 
    associated with this object.
  */ 
  virtual OdResult dxfIn(OdDbDxfFiler* pFiler);

  /** \details
    Overridables
  */
  virtual OdResult subGetClassID(void* pClsid) const;

  /** \details
    Notification function called each time an Undo operation is performed this object
    is using partial Undo.

    \param pFiler [in] A pointer to the *undo* filer with the partial *undo* information. 
    \param pClass [in] A pointer to the OdRxClass object for the class that will *handle* the Undo.      
    
    \remarks
    An object indicates it's using the partial *undo* mechanism, if and only if it has set 
    autoUndo false in all calls to assertWriteEnabled(). 
    This member function must know which types of fields to scan, and must stop after reading 
    what it needs. 
    If the class type specified by pClassObj does not matches the class of this object, 
    this member function must call the parent class's applyPartialUndo() and return whatever it returns. 
    If it does match the class of this object, this member function must use pUndoFiler to 
    read the *undo* data, then typically use this object's set() method.
  */ 
  virtual void applyPartialUndo(OdDbDwgFiler* pFiler, OdRxClass* pClass);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbBlockTable object pointers.
*/
typedef OdSmartPtr<OdDbBlockTable> OdDbBlockTablePtr;

#include "TD_PackPop.h"

#endif 

