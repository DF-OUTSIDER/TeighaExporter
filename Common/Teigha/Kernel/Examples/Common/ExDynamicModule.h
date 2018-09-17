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
//
// ExDynamicModule.h
//

#ifndef _EX_RXDYNAMICMODULE_H_
#define _EX_RXDYNAMICMODULE_H_

#include "OdaCommon.h"
#include "RxDynamicModule.h"

//////////////////////////////////////////////////////////////////////////
// 
// SpecialImpl may be used to define special flags for example :
//
// ODRX_DECLARE_COMMAND(... , OdEdCommand, NO_UNDO_IMPL)
//

#define ODRX_DECLARE_COMMAND(GlobalName, ClassName, BaseClass, SpecialImpl)       \
  class ClassName : public BaseClass                                              \
  {                                                                               \
  public:                                                                         \
    const OdRxModule* m_pModule;                                                  \
    ClassName()                                                                   \
      : m_pModule(NULL)                                                           \
    {                                                                             \
    }                                                                             \
    virtual const OdString groupName() const                                      \
    {                                                                             \
      return ODRX_MODULE_NAME_W;                                                  \
    }                                                                             \
    virtual const OdString globalName() const                                     \
    {                                                                             \
      return OD_T(#GlobalName);                                                        \
    }                                                                             \
    virtual const OdString localName() const                                      \
    {                                                                             \
      return  globalName();                                                       \
    }                                                                             \
    virtual const OdRxModule* module() const                                      \
    {                                                                             \
      return m_pModule;                                                           \
    }                                                                             \
    virtual void execute(OdEdCommandContext* pCmdCtx);                            \
    SpecialImpl                                                                   \
  };

#define NO_UNDO_IMPL virtual OdInt32 flags() const                                \
  { return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker; }

#define ODRX_SELECTION_SET L"Teigha Working Selection Set"

#endif  // _EX_RXDYNAMICMODULE_H_
