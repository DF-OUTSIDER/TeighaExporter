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

#ifndef RASTER_EXPORT_MODULE_H_
#define RASTER_EXPORT_MODULE_H_

#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"
#include "RasterExportCmd.h"

using namespace TD_RASTER_EXPORT;
#define ODDB_COMMANDS_GROUP_NAME OD_T("ODDB_EDIT")

/*!DOM*/
#define CMD_DEF_CLASS(CmdName, GroupName)
#define CMD_DEF(CmdName, GroupName) \
class _##CmdName##_cmd : public OdEdCommand \
{ \
public: \
  const OdString groupName() const;\
  const OdString globalName() const;\
  void execute(OdEdCommandContext* pCmdCtx);\
};
#include "RasterExportCmdDef.h"
#undef CMD_DEF_CLASS
#undef CMD_DEF


/** \details
  <group OdExport_Classes> 
*/
namespace TD_RASTER_EXPORT {
/*!DOM*/
namespace OdDbCmd
{
#define CMD_MSG_DEF(code, string)  code,
  enum MessageId
  {
#include "RasterExportCmdDef.h"
    kLastMassage
  };
#undef CMD_MSG_DEF
};

/** \details
  This class implements the raster export module. 
*/
class RasterExportModule : public OdRxModule
{

#define CMD_DEF(CmdName, GroupName) \
  OdStaticRxObject<_##CmdName##_cmd> m_##CmdName##_cmd;
#include "RasterExportCmdDef.h"
#undef CMD_DEF

public:

  void initApp();
  void uninitApp();
};
}
#endif // RASTER_EXPORT_MODULE_H_
