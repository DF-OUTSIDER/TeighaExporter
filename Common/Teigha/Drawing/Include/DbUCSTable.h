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




#ifndef _ODDBUCSTABLE_INCLUDED
#define _ODDBUCSTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTable.h"
#include "DwgDeclareMembers.h"

class OdDbUCSTableRecord;

/** \details
  <group OdDb_Classes>

  This class implements the UCS table object, which represents a container for storing and 
  accessing of UCS record objects in the database. The UCS is user coordinate system. This 
  class inherits the base functionality of predefined tables.

  \sa
  TD_Db

  \sa
  <link db_ucs_sample_table.html, Example of Working with the UCS Table Object>

  <link db_ucs_sample_record.html, Example of Working with the UCS Record Object>

  \sa
  <link db_ucs.html, Working with UCSs>

  OdDbUCSTableRecord class
*/
class TOOLKIT_EXPORT OdDbUCSTable: public OdDbSymbolTable
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbUCSTable);

  /** \details
    Builds an instance of the UCS table object.
    
    Applications typically will not use this constructor, insofar as the database object 
    creates own instance of the UCS table. The UCS table object exists in the database 
    a prior.
  */
  OdDbUCSTable();

  virtual OdResult subGetClassID(
    void* pClsid) const;
};

/** \details
  The typified smart pointer for the UCS table object. This template class is specialization 
  of the OdSmartPtr class for the OdDbUCSTable object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbUCSTable>  OdDbUCSTablePtr;

#include "TD_PackPop.h"

#endif // _ODDBUCSTABLE_INCLUDED

