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



#ifndef OD_DBLINKEDDATA_H
#define OD_DBLINKEDDATA_H

#include "TD_PackPush.h"

#include "DbObject.h"

	/** \details
    Work-in-progress. This class represents LinkedData objects in an OdDbDatabase instance.

    \sa
    TD_Db

	<group OdDb_Classes> 
	*/
class DBENT_EXPORT OdDbLinkedData : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbLinkedData);

  OdDbLinkedData();

  // virtual void clear(void);
  // virtual bool isEmpty(void) const;

  virtual const OdString name (void) const;
  virtual void setName(const OdString& name);
  virtual const OdString description(void) const;
  virtual void setDescription(const OdString& description);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbLinkedData> OdDbLinkedDataPtr;

#include "TD_PackPop.h"

#endif // OD_DBLINKEDDATA_H
