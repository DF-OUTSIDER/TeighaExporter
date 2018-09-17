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


///////////////////////////////////////////////////////////////////////////////
//
// HoopsExport.h - Open Design Hoops Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _HOOPS_EXPORT_INCLUDED_
#define _HOOPS_EXPORT_INCLUDED_

#include "OdaCommon.h"
#include "DebugStuff.h"
#include "HoopsExportDef.h"
#include "DbBaseDatabase.h"
#include "OdStreamBuf.h"
#include "HoopsResultEx.h"
#include "HoopsExportParams.h"
using namespace TD_HOOPS_COMMON_EXPORT;

/** \details
  Proceeds the export from a hoops file to a drawing database (.dwg or .dxf file). 
  
  \param params [in] A set of parameters used to export from a hoops file.
  
  \returns Returns 0 if the export has been successfully done, otherwise returns an error code. 
*/
OdUInt32 HOOPSEXPORT_DLL exportHoops( HoopsExportParams &params );

/** \details
  Returns export to hoops error code description. 
  
  \param errorCode [in] An error code to be interpreted.
*/
OdString HOOPSEXPORT_DLL exportHoopsErrorCode(OdUInt32 errorCode);

// The HInputDWG is used for reading .dwg files.

#endif // _HOOPS_EXPORT_INCLUDED_

