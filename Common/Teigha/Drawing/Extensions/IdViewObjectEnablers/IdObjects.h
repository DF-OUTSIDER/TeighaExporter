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

#ifndef _IDOBJECTS_H_INCLUDED_
#define _IDOBJECTS_H_INCLUDED_

#include "TD_PackPush.h"

#include "IdViewExport.h"

class OdIdObjectToNotifyImpl;

/** \details

    Library: Id
    <group OdId_Classes> 
*/
class IDVIEW_EXPORT OdIdObjectToNotify : public OdDbObject
{
  OdIdObjectToNotifyImpl* m_pImpl;
public:
  ODDB_DECLARE_MEMBERS(OdIdObjectToNotify);
  OdIdObjectToNotify();
  ~OdIdObjectToNotify();
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdDbObjectPtr decomposeForSave(
    OdDb::SaveType format, 
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);
};

/** \details

    Library: Id
    <group OdId_Classes> 
*/
class IDVIEW_EXPORT OdIdVersion : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdIdVersion);
  OdIdVersion(){}
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdDbObjectPtr decomposeForSave(
    OdDb::SaveType format, 
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);
};

#include "TD_PackPop.h"

#endif //_IDOBJECTS_H_INCLUDED_
