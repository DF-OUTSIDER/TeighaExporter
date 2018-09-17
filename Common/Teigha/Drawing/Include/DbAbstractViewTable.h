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




#ifndef _ODDBABSTRACTVIEWTABLE_INCLUDED
#define _ODDBABSTRACTVIEWTABLE_INCLUDED

#include "DbSymbolTable.h"

class OdDbAbstractViewTableRecord;

/** \details
    This class is the base class for OdDbViewTable and OdDbViewportTable.

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbAbstractViewTable : public OdDbSymbolTable
{
public:

  ODDB_DECLARE_MEMBERS(OdDbAbstractViewTable);

  /** \details
    Teigha applications typically will not use this constructor, insofar as 
    this class is a base class.
  */
  OdDbAbstractViewTable();

//  void getAt(const OdChar* entryName, OdDbAbstractViewTableRecord** pRec,
//    OdDb::OpenMode openMode, bool openErasedRec = false) const;

//  OdDbObjectId getAt(const OdChar* entryName, bool getErasedRecord = false) const;

//  bool has(const OdChar* name) const;
//  bool has(const OdDbObjectId& id) const;

//  void newIterator(OdDbAbstractViewTableIterator** pIterator,
//    bool atBeginning = true, bool skipDeleted = true) const;

//  virtual const OdDbObjectId& add(OdDbSymbolTableRecord* pRecord);

};

#endif // _ODDBABSTRACTVIEWTABLE_INCLUDED

