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

#ifndef OD_DB_SYNERGY_MODOLE_H
#define OD_DB_SYNERGY_MODOLE_H

#include "DbObject.h"
#include "RxModule.h"

#include "TD_PackPush.h"

/** \details
    This class represents synergy module object.
    
    Corresponding C++ library: TD_DbEntities
*/
class ODRX_ABSTRACT DBENT_EXPORT OdDbSynergyModule : public OdRxModule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSynergyModule);

  /** \details
    Returns the view rep object ID of view border.

    \param pObject [in]  View border object.

  \returns
    View rep object ID.
  */
  virtual OdDbObjectId getViewRepId(OdDbObjectPtr pViewBorderObj) = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSynergyModule object pointers.
*/
typedef OdSmartPtr<OdDbSynergyModule> OdDbSynergyModulePtr;

#include "TD_PackPop.h"

#endif // OD_DB_SYNERGY_MODOLE_H
