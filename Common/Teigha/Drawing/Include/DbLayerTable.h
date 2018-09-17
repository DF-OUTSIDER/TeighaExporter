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




#ifndef _ODDBLAYERTABLE_INCLUDED
#define _ODDBLAYERTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTable.h"
#include "DwgDeclareMembers.h"

class OdDbLayerTableRecord;

/** \details
  <group OdDb_Classes>

  This class implements the layer table object, which represents a container for storing 
  and accessing of layer record objects in the database. This class inherits the base 
  functionality of predefined tables.

  \sa
  TD_Db

  \sa
  <link db_layer_sample_table.html, Example of Working with the Layer Table Object>

  <link db_layer_sample_record.html, Example of Working with the Layer Record Object>

  \sa
  <link db_layer.html, Working with Layers>

  OdDbLayerTableRecord class
*/
class TOOLKIT_EXPORT OdDbLayerTable: public OdDbSymbolTable
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbLayerTable);

  /** \details
    Builds an instance of the layer table object.
    
    Applications typically will not use this constructor, insofar as the database object 
    creates own instance of the layer table. The layer table object exists in the database 
    a prior.
  */
  OdDbLayerTable();

  virtual OdResult subGetClassID(
    void* pClsid) const;


  /** \details
    Generates usage data for each record in the layer table object.
    
    \remarks
    Usage data can be accessed via the OdDbLayerTableRecord::isInUse() method.
  */
  void generateUsageData();
};

/** \details
  The typified smart pointer for the layer table object. This template class is specialization 
  of the OdSmartPtr class for the OdDbLayerTable object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbLayerTable> OdDbLayerTablePtr;

#include "TD_PackPop.h"

#endif // _ODDBLAYERTABLE_INCLUDED

