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
#include "OdaCommon.h"
#include "DbCommandContext.h"
#include "DbUserIO.h"
#include "DbSSet.h"
#include "Entities.h"
#include "DbBlockTableRecord.h"

#include "DbAssocNetwork.h"

#include "LGSEmbedPE.h"

#include "LGSCommands.h"

OdEdCommandContext * gpCmdCtx = 0;

void LGSUpdate::execute(OdEdCommandContext* pCmdCtx)
{
  try {
    gpCmdCtx = pCmdCtx;
    OdDbCommandContextPtr dbCmdCtx = OdDbCommandContext::cast(pCmdCtx);
    OdDbObjectId id = OdDbAssocNetwork::getInstanceFromDatabase(dbCmdCtx->database(), false);
    OdDbAssocNetworkPtr pNet = OdDbAssocNetwork::cast(id.openObject(OdDb::kForWrite));

    if (!pNet.isNull()) {
/*
      LGSAssocActionBodyPtr pActionBody;
      if (pNet->actionBody().isNull()) {
        pActionBody = LGSAssocActionBody::createObject();
        OdDbObjectId abId = dbCmdCtx->database()->addOdDbObject(pActionBody);
        if (abId.isValid()) {
          pNet->setActionBody(abId);
        }
      }
*/
      //pNet->setStatus(OdDbAssocStatus::kChangedDirectlyAssocStatus, false, true);
      if (pNet->status() == kIsUpToDateAssocStatus) {
        gpCmdCtx = 0;
        return;
      }
      pNet->evaluate(NULL/*pActionBody->currentEvaluationCallback()*/);
      pNet->setStatus(kIsUpToDateAssocStatus, false, true);
      gpCmdCtx = 0;
    }
  } catch (const OdError& err) {
    gpCmdCtx->userIO()->putString(err.description());
    gpCmdCtx = 0;
  }
}
