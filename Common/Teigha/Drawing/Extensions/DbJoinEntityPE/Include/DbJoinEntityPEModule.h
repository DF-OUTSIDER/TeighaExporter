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


#ifndef _DB_JOIN_ENTITY_PE_MODULEIMPL_INCLUDED_
#define _DB_JOIN_ENTITY_PE_MODULEIMPL_INCLUDED_

#include "DbJoinEntityPEDef.h"
#include "RxDynamicModule.h"
#include "DbBaseDatabase.h"

/** \details
  This class implements the module for join functionality.

  Library: Source code provided.
  <group Join_Classes> 
*/
class JOINEXPORT_DLL DbJoinEntityPEModule : public OdRxModule
{
protected:
  /** \details
    Initializes an application.
  */
  void initApp();

  /** \details
    Uninitializes an application.
  */
  void uninitApp();
};

typedef OdSmartPtr<DbJoinEntityPEModule> DbJoinEntityPEModulePtr;

#endif //_DB_JOIN_ENTITY_PE_MODULEIMPL_INCLUDED_
