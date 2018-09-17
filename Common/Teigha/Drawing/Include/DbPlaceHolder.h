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




#ifndef OD_DBPLACEHOLDER_H
#define OD_DBPLACEHOLDER_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "DwgDeclareMembers.h"

/** \details
  This class implements Placeholder objects in an OdDbDatabase instance.
  
  \sa
  TD_Db
  
  \remarks
  Placeholder objects are designed to be added to dictionaries,
  providing Object IDs that can be linked to dictionary keys. They are used in the
  Plot Style Name dictionary.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbPlaceHolder : public OdDbObject
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbPlaceHolder);

  OdDbPlaceHolder();
  
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /*
  virtual void subWblockClone(OdDbIdMapping& idMap) const;
  */
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbPlaceHolder object pointers.
*/
typedef OdSmartPtr<OdDbPlaceHolder> OdDbPlaceHolderPtr;

#include "TD_PackPop.h"

#endif

