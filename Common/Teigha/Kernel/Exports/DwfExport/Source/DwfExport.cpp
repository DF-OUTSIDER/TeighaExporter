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
#include "DwfExportParams.h"

#include "DwfExportImpl.h"
#include "DwfExport.h"

#include "RxDispatchImpl.h"
#include "RxDynamicModule.h"

namespace TD_DWF_EXPORT{

class DwfExporter : public OdDwfExport
{
public:
  void exportDwf(DwExportParams& pParams);
  void export3dDwf(Dwf3dExportParams& pParams);
};
typedef OdSmartPtr<DwfExporter> DwfExporterPtr;


void DwfExporter::export3dDwf(Dwf3dExportParams& pParams) 
{
  //OdDbBaseDatabasePEPtr(pParams.database())->startTransaction(pParams.database());

  try
  {
    C3dDwfExportImpl dwfExport(pParams);
 
    dwfExport.run();
  }  
  // hold all unknown exceptions
  catch(const OdError &)
  {
  }
  catch(...)
  {
  }

  //OdDbBaseDatabasePEPtr(pParams.database())->abortTransaction(pParams.database());
}

void DwfExporter::exportDwf(DwExportParams &pParams) 
{
  //OdDbBaseDatabasePEPtr(pParams.database())->startTransaction(pParams.database());

  try
  {
    CDwfExportImpl dwfExport(pParams);
 
    dwfExport.run();
  }  
  // hold all unknown exceptions
  catch(const OdError &)
  {
  }
  catch(...)
  {
  }

  //OdDbBaseDatabasePEPtr(pParams.database())->abortTransaction(pParams.database());
}

class OdDwfExportModule : public DwfExportModule
{
public:
  virtual void initApp(){}
  virtual void uninitApp(){}

  virtual OdDwfExportPtr create ()
  {
    return OdRxObjectImpl<DwfExporter>::createObject();
  }

};
}

using namespace TD_DWF_EXPORT;
ODRX_DEFINE_DYNAMIC_MODULE(OdDwfExportModule);
