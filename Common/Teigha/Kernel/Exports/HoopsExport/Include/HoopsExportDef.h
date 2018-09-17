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
// HoopsExport.h - Open Design Pdf Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _HOOPS_EXPORT_DEF_INCLUDED_
#define _HOOPS_EXPORT_DEF_INCLUDED_

#include "OdPlatformSettings.h"

#ifdef _TOOLKIT_IN_DLL_
#define HOOPSEXPORT_DLL           OD_TOOLKIT_EXPORT
#define HOOPSEXPORT_DLL_STATIC    OD_STATIC_EXPORT
#else
#define HOOPSEXPORT_DLL           OD_TOOLKIT_IMPORT
#define HOOPSEXPORT_DLL_STATIC    OD_STATIC_IMPORT
#endif

//////////////////////////////////////////////////////////////////////////

#endif // _HOOPS_EXPORT_DEF_INCLUDED_

