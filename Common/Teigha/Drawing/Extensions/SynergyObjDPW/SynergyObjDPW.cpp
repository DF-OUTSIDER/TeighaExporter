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
/* SynergyObjDPW.cpp : Defines the entry points for the DLL             */
/************************************************************************/

#include "OdaCommon.h"
#include "RxSysRegistry.h"

#include "RxDynamicModule.h"
#include "StaticRxObject.h"

#include "DbAssocViewRepActionParam.h"
#include "DbAssocViewStyleActionParam.h"
#include "DbAssocViewLabelActionParam.h"
#include "DbAssocViewBorderActionParam.h"
#include "DbAssocViewSymbolActionParam.h"
#include "DbAssocViewRepHatchActionParam.h"
#include "DbAssocViewRepHatchManager.h"
#include "DbAssocViewRepActionBody.h"
#include "DbIdwVersion.h"
#include "DbIdwStream.h"
// awaited for sample files:
//#include "DbAssocInventorPersSubentIdManager.h"
//#include "DbAssocViewRepProvisionalPersSubentId.h"
//#include "DbAssocViewRepProvisionalPersSubentIdVisitor.h"
#include "DbSynergyModule.h"
#include "DbAssocDependency.h"

class SynergyObjDPW_Module : public OdDbSynergyModule
{
protected:
  virtual OdDbObjectId getViewRepId(OdDbObjectPtr pViewBorderObj)
  {
    OdDbObjectId idViewRep;
    OdDbAssocDependencyPtr pDep;
    OdDbObjectId idDep = OdDbAssocDependency::getFirstDependencyOnObject(pViewBorderObj);
    if (!idDep.isNull() && (pDep = OdDbAssocDependency::cast(idDep.openObject())).get())
    {
      OdDbAssocActionPtr pAction;
      OdDbObjectId idAction = pDep->owningAction();
      if (!idAction.isNull() && (pAction = OdDbAssocAction::cast(idAction.openObject())).get())
      {
        OdDbAssocViewRepActionBodyPtr pBody;
        OdDbObjectId idBody = pAction->actionBody();
        if (!idBody.isNull() && (pBody = OdDbAssocViewRepActionBody::cast(idBody.openObject())).get())
          idViewRep = pBody->viewRepId();
      }
    }
    return idViewRep;
  }

public:
	void initApp()
  {
    /////
    // Load and lock DbConstraints
    m_pDbConstraintsModule = ::odrxDynamicLinker()->loadModule(DbConstraintsModuleName, false);
    OdDbAssocViewRepActionParam::rxInit();
    OdDbAssocViewStyleActionParam::rxInit();
    OdDbAssocViewLabelActionParam::rxInit();
    OdDbAssocViewBorderActionParam::rxInit();
    OdDbAssocViewSymbolActionParam::rxInit();
    OdDbAssocViewRepHatchActionParam::rxInit();
    OdDbAssocViewRepHatchManager::rxInit();
    OdDbAssocViewRepActionBody::rxInit();
    OdDbIdwVersion::rxInit();
    OdDbIdwStream::rxInit();
    // awaited for sample files:
    //OdDbAssocInventorPersSubentIdManager::rxInit();
    //OdDbAssocViewRepProvisionalPersSubentId::rxInit();
    //OdDbAssocViewRepProvisionalPersSubentIdVisitor::rxInit();

    /////
  }

  void uninitApp()
  {
    /////

    //// awaited for sample files:
    //OdDbAssocViewRepProvisionalPersSubentIdVisitor::rxUninit();
    //OdDbAssocViewRepProvisionalPersSubentId::rxUninit();
    //OdDbAssocInventorPersSubentIdManager::rxUninit();
    //
    OdDbIdwStream::rxUninit();
    OdDbIdwVersion::rxUninit();
    OdDbAssocViewRepActionBody::rxUninit();
    OdDbAssocViewRepHatchManager::rxUninit();
    OdDbAssocViewRepHatchActionParam::rxUninit();
    OdDbAssocViewSymbolActionParam::rxUninit();
    OdDbAssocViewBorderActionParam::rxUninit();
    OdDbAssocViewLabelActionParam::rxUninit();
    OdDbAssocViewStyleActionParam::rxUninit();
    OdDbAssocViewRepActionParam::rxUninit();

    /////
    // Unlock DbConstraints
    m_pDbConstraintsModule = 0;
  }
  private:
    OdRxModulePtr m_pDbConstraintsModule;
};

ODRX_DEFINE_DYNAMIC_MODULE(SynergyObjDPW_Module);
