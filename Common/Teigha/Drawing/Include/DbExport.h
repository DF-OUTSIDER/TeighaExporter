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




/*  DbExport.h

*/
#ifndef _DB_EXPORT_DEFINED
#define _DB_EXPORT_DEFINED

#include "OdPlatformSettings.h"

#ifdef ODA_DB_EXPORTS
  #define TOOLKIT_EXPORT            OD_TOOLKIT_EXPORT
  #define TOOLKIT_EXPORT_GLOBAL     OD_DLL_EXPORT
  #define TOOLKIT_EXPORT_STATIC     OD_STATIC_EXPORT
#else
  #define TOOLKIT_EXPORT            OD_TOOLKIT_IMPORT
  #define TOOLKIT_EXPORT_GLOBAL     OD_DLL_IMPORT
  #define TOOLKIT_EXPORT_STATIC     OD_STATIC_IMPORT
#endif

#ifdef ODA_DBLIB_EXPORTS
#define DBLIB_EXPORT            OD_TOOLKIT_EXPORT
#else
#define DBLIB_EXPORT            OD_TOOLKIT_IMPORT
#endif

#ifdef SCENEOE_EXPORTS
# define SCENEOE_EXPORT            OD_TOOLKIT_EXPORT
# define SCENEOE_EXPORT_GLOBAL     OD_DLL_EXPORT
# define SCENEOE_EXPORT_STATIC     OD_STATIC_EXPORT
#else
# define SCENEOE_EXPORT            OD_TOOLKIT_IMPORT
# define SCENEOE_EXPORT_GLOBAL     OD_DLL_IMPORT
# define SCENEOE_EXPORT_STATIC     OD_STATIC_IMPORT
#endif


#ifdef DBENT_EXPORTS
#define DBDIM_EXPORTS
  #define DBENT_EXPORT           OD_TOOLKIT_EXPORT
  #define DBENT_EXPORT_STATIC    OD_STATIC_EXPORT
#else
  #define DBENT_EXPORT           OD_TOOLKIT_IMPORT
  #define DBENT_EXPORT_STATIC    OD_STATIC_IMPORT
#endif


#define DBDIM_EXPORT DBENT_EXPORT


#ifdef DBIO_EXPORTS
  #define DBIO_EXPORT            OD_TOOLKIT_EXPORT
  #define DBIO_EXPORT_GLOBAL     OD_DLL_EXPORT
  #define DBIO_EXPORT_STATIC     OD_STATIC_EXPORT
#else
  #define DBIO_EXPORT            OD_TOOLKIT_IMPORT
  #define DBIO_EXPORT_GLOBAL     OD_DLL_IMPORT
  #define DBIO_EXPORT_STATIC     OD_STATIC_IMPORT
#endif


#define UNDERLAY_EXPORT DBENT_EXPORT


//////////////////////////////////////////////////////////////////////////

#endif  /* _DB_EXPORT_DEFINED */



