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
#ifndef _IDV_EXPORT_DEFINED
#define _IDV_EXPORT_DEFINED

#include "OdPlatformSettings.h"

#ifdef ID_DLL_EXPORTS
#define IDVIEW_EXPORT           OD_TOOLKIT_EXPORT
#define IDVIEW_EXPORT_STATIC    OD_STATIC_EXPORT
#else                           
#define IDVIEW_EXPORT           OD_TOOLKIT_IMPORT
#define IDVIEW_EXPORT_STATIC    OD_STATIC_IMPORT
#endif

#endif //_IDV_EXPORT_DEFINED
