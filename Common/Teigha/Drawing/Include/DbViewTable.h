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




#ifndef _ODDBVIEWTABLE_INCLUDED
#define _ODDBVIEWTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbAbstractViewTable.h"
#include "DwgDeclareMembers.h"

class OdDbViewTableRecord;

/** \details
    This class implements the ViewTable, which represents stored views in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbViewTable: public OdDbAbstractViewTable
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbViewTable);

  /** \details
    Teigha applications typically will not use this constructor, insofar as 
    this class is a base class.
  */
  OdDbViewTable();

  virtual OdResult subGetClassID(
    void* pClsid) const;
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbViewTable object pointers.
*/
typedef OdSmartPtr<OdDbViewTable> OdDbViewTablePtr;

#include "TD_PackPop.h"

#endif // _ODDBVIEWTABLE_INCLUDED

