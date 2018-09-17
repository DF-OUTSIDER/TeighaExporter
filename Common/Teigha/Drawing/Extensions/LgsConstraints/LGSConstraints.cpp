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
/************************************************************************/
/* LGSConstraints.cpp: Defines the entry points for the DLL           */
/************************************************************************/
#include "OdaCommon.h"
#include "RxSysRegistry.h"

#include "RxDynamicModule.h"
#include "StaticRxObject.h"

//#include "DbAssocNetwork.h"
#include "DbAssoc2dConstraintGroup.h"

#include "LGSCommands.h"
#include "LGSAssocActionPE.h"
#include "LGSEmbedPE.h"

class LGSConstraints_Module : public OdRxModule
{
  public:
	void initApp()
  {
    // Lock TD_DbConstraints module
    m_pDbConstraintsModule = ::odrxDynamicLinker()->loadModule(DbConstraintsModuleName, false);

    LGSAssocActionPE::rxInit();
    LGSEmbedPE::rxInit();
    OdDbAssoc2dConstraintGroup::desc()->addX(AssocActionPE::desc(),   &_lgsAssocActionPE);
    //OdDbAssocNetwork::desc()->addX(AssocActionPE::desc(),   &_lgsAssocActionPE);
    odedRegCmds()->addCommand(&_lgsUpdate);
  };

  void uninitApp()
  {
    //OdDbAssocNetwork::desc()->delX(AssocActionPE::desc());
    OdDbAssoc2dConstraintGroup::desc()->delX(AssocActionPE::desc());
    LGSAssocActionPE::rxUninit();
    LGSEmbedPE::rxUninit();
    odedRegCmds()->removeCmd(&_lgsUpdate);

    // Unlock TD_DbConstraints module
    m_pDbConstraintsModule = 0;
  };

private:
    OdStaticRxObject<LGSUpdate> _lgsUpdate;
    OdStaticRxObject<LGSAssocActionPE> _lgsAssocActionPE;
    OdRxModulePtr m_pDbConstraintsModule;  // Smart pointer to prevent TD_DbConstraints module from unloading
};

ODRX_DEFINE_DYNAMIC_MODULE(LGSConstraints_Module);
