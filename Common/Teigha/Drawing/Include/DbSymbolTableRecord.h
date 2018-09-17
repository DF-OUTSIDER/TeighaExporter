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




#ifndef _ODDBSYMBOLTABLERECORD_INCLUDED
#define _ODDBSYMBOLTABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbObject.h"

class OdString;

/** \details
  <group OdDb_Classes>

  This class implements the named record object, which is the base class for all record 
  objects in the database. 
  
  \sa
  TD_Db

  \sa
  <link db_datasheet_sample_select.html, Example of Using the Record-Table Interface for Selecting Objects>

  <link db_database_object_sample_about.html, Example of Using the Record-Table and Dictionary Interfaces for Getting Information about Objects>  

  \sa
  <link db_datasheet.html, Working with Predefined Tables of Named Records>

  OdDbSymbolTable class
*/
class TOOLKIT_EXPORT OdDbSymbolTableRecord : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSymbolTableRecord);

  /** \details
    Builds an instance of the named record object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_datasheet_add_delete.html, Adding, Naming, Deleting, Recovering Records>
  */
  OdDbSymbolTableRecord();


  /** \details
    Returns the name of the record object (DXF 2).

    \sa
    <link db_datasheet_add_delete.html, Adding, Naming, Deleting, Recovering Records>

    OdDbSymbolTableRecord::setName() method
  */
  virtual OdString getName() const;


  /** \details
    Sets the name for the record object (DXF 2).

    \param sName [in]  Record name as a String value up to 255 characters long.

    \remarks
    The name is an arbitrary non-empty string that can contain letters, digits, blank spaces, 
    underscores, and some special characters, but cannot contain inadmissible letters 
    (see {Naming Objects|objects_naming.html}).

    \sa
    <link db_datasheet_add_delete.html, Adding, Naming, Deleting, Recovering Records>

    OdDbSymbolTableRecord::getName() method
  */
  virtual void setName( const OdString& sName );


  /** \details
    Returns true if and only if this Record object is Xref dependent (DXF 70, bit 0x10).
  */
  bool isDependent() const;

  /** \details
    Returns true if and only if this Record object is Xref dependent and resolved (DXF 70, bit 0x20).
  */
  bool isResolved() const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  void appendToOwner(
    OdDbIdPair& idPair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap);

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  /** \details
      Copies the contents of other into the messaged object, whenever feasible
  */
  virtual void copyFrom(
    const OdRxObject* pSource);

  virtual OdResult subErase(
    bool erasing);

  virtual void subSwapIdWith(
    const OdDbObjectId& otherId, 
    bool swapXdata = false, 
    bool swapExtDict = false);
};

/** \details
  The typified smart pointer for the named record object. This template class is 
  specialization of the OdSmartPtr class for the OdDbSymbolTableRecord object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSymbolTableRecord> OdDbSymbolTableRecordPtr;

#include "TD_PackPop.h"

#endif // _ODDBSYMBOLTABLERECORD_INCLUDED

