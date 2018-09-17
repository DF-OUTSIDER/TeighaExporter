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




#ifndef _ODDBREGAPPTABLERECORD_INCLUDED
#define _ODDBREGAPPTABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTableRecord.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents records in the OdDbRegAppTable in an OdDbDatabase instance.
    
    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbRegAppTableRecord: public OdDbSymbolTableRecord
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbRegAppTableRecord);

  OdDbRegAppTableRecord();

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subErase( bool erasing );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRegAppTableRecord object pointers.
*/
typedef OdSmartPtr<OdDbRegAppTableRecord> OdDbRegAppTableRecordPtr;

#include "TD_PackPop.h"

#endif // _ODDBREGAPPTABLERECORD_INCLUDED

