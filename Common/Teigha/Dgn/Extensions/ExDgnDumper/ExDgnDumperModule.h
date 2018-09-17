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
#ifndef __EXDGNDUMPER_MODULE__H__
#define __EXDGNDUMPER_MODULE__H__

#include "RxModule.h"
#include "DgExport.h"

#include "RxSystemServices.h"
#include "ExDgnDumper.h"

/**
  <group Dg_Extension_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdExDgnDumperModule : public OdRxModule
{
public:
  void initApp() = 0;
  void uninitApp() = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdExDgnDumperModule object pointers.
*/
typedef OdSmartPtr<OdExDgnDumperModule> OdExDgnDumperModulePtr;

#endif // __EXDGNDUMPER_MODULE__H__
