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

#ifndef _OD_DB_EMBEDDEDASMENT_PE_
#define _OD_DB_EMBEDDEDASMENT_PE_

#include "RxObject.h"
/** <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbEmbeddedASMEntPropsPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbEmbeddedASMEntPropsPE);
  virtual void setIsEmbeddedEnt(OdDbObject* pObj, bool bIsEmbedded = true);
};

#endif
