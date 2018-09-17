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


#ifndef _OD_TF_OBJECTCHANGESCALCULATIONINTERFACE_Included_
#define _OD_TF_OBJECTCHANGESCALCULATIONINTERFACE_Included_

#include "OdaCommon.h"
#include "RxObject.h"

#include "TD_PackPush.h"
#include "DbHostAppServices.h"
#include "Tf/TfIO.h"
#include "OdStreamBuf.h"

class OdCalcDiffFiler;// opaque pointer
namespace OdTfRevisionControl
{
  enum DiffData
  {
    kDbDatabase = 0,
    kClassMap = 1,
    kObject = 2
  };
};

/** \details
<group OdTf_Classes>
*/
class TfObjectChangesCalculationInterface : public OdRxObject
{
public:
  virtual OdStreamBufPtr changesCalculation(OdCalcDiffFiler& currentObjectState, OdCalcDiffFiler& previousObjectState, OdTfDigest newSha, const OdAnsiString& className, OdDbHostAppServices* pAppServices = 0, const OdTfRevisionControl::DiffData typeData = OdTfRevisionControl::kObject) = 0;
  virtual OdStreamBufPtr restoreObjectFromTfDiff(OdStreamBuf* previousObjectState, OdStreamBufPtr& diffState, OdDbHostAppServices* pAppServices = 0, const OdTfRevisionControl::DiffData typeData = OdTfRevisionControl::kObject) = 0;
  TfObjectChangesCalculationInterface() {};
  virtual ~TfObjectChangesCalculationInterface() {};
};

#include "TD_PackPop.h"

#endif // _OD_TF_OBJECTCHANGESCALCULATIONINTERFACE_Included_
