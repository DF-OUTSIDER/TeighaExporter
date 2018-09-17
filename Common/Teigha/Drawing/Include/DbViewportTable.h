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




#ifndef _ODDBVIEWPORTTABLE_INCLUDED
#define _ODDBVIEWPORTTABLE_INCLUDED

#include "TD_PackPush.h"

#include "DbAbstractViewTable.h"
#include "DbObject.h"
#include "DwgDeclareMembers.h"

class OdDbViewportTableRecord;

/** \details
    This class implements the ViewportTable, which represents tiled viewports in an OdDbDatabase instance.

    \sa
    TD_Db

    \remarks
    Do not derive from this class.
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbViewportTable: public OdDbAbstractViewTable
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbViewportTable);

  /** \details
    Teigha applications typically will not use this constructor, insofar as 
    this class is a base class.
  */
  OdDbViewportTable();

  /** \details
    Makes the specified viewport the active viewport of this ViewportTable object.

    \param viewportId [in]  Object ID of the active viewport.      
  */
  void SetActiveViewport(
    OdDbObjectId viewportId);

  /** \details
    Returns the active viewport of this ViewportTable object.
  */
  OdDbObjectId getActiveViewportId() const;

  /** \details
    Deletes the specified configuration from this ViewportTable object.
    
    \param configName [in]  Configuration name to delete.
  */
  void DeleteConfiguration(
    const OdString& configName);

  OdDbObjectId add(
    OdDbSymbolTableRecord* pRecord);

  OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  OdResult subGetClassID(
    void* pClsid) const;

  void subClose();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbTextStyleTable object pointers.
*/
typedef OdSmartPtr<OdDbViewportTable> OdDbViewportTablePtr;

#include "TD_PackPop.h"

#endif // _ODDBVIEWPORTTABLE_INCLUDED

