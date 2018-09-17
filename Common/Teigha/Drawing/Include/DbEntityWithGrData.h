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




#ifndef _OD_DB_ENTITYWITHGRDATA_
#define _OD_DB_ENTITYWITHGRDATA_

#include "TD_PackPush.h"

#include "DbEntity.h"


/** \details
    This class is the Protocol Extension for OdDbEntity objects with stored graphics data.
    
    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbEntityWithGrDataPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbEntityWithGrDataPE);
  virtual bool worldDraw( const OdDbEntity* entity, OdGiWorldDraw* pWd ) const = 0;

  static void addX_to(OdRxClass* cls);
  static void delX_from(OdRxClass* cls);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbEntityWithGrData object pointers.
*/
typedef OdSmartPtr<OdDbEntityWithGrDataPE> OdDbEntityWithGrDataPEPtr;


inline void OdDbEntityWithGrDataPE::addX_to(OdRxClass* cls) {
  if (desc()) {
    OdDbEntityWithGrDataPEPtr grDataPeImpl = desc()->create();
    if (grDataPeImpl.get()) // dna: impl constructor initialized in OdDbIOModule::initApp()
      cls->addX(desc(), grDataPeImpl);
  }
}

inline void OdDbEntityWithGrDataPE::delX_from(OdRxClass* cls) {
  if (desc())
    cls->delX(desc());
}

#include "TD_PackPop.h"

#endif
