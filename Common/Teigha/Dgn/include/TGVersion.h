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


#ifndef TG_VERSION_DEFINED
#define TG_VERSION_DEFINED

#include <TDVersion.h>

// Full Product version is:
//   TD_MAJOR_VERSION.TD_MINOR_VERSION.TD_MAJOR_BUILD_NUMBER.TD_MINOR_BUILD_NUMBER
// Example: 1.11.00.00 (initial 1.11 release).
// OpenDesign maintenance updates to 1.11 will increase the major build number,
// for example, 1.11.01.00, 1.11.02.00, etc.  Minor build number is reserved for
// client builds.

#define TG_COMPANY_NAME_S       TD_COMPANY_NAME_S
#define TG_COMPANY_NAME_W       TD_COMPANY_NAME_W

#define TG_COPYRIGHT_S          TD_COPYRIGHT_S
#define TG_COPYRIGHT_W          TD_COPYRIGHT_W
#define TG_PRODUCT_NAME_END_S   "DGN SDK"
#define TG_PRODUCT_NAME_END_W  L"DGN SDK"

#define TG_PRODUCT_NAME_S TD_PRODUCT_NAME_START_S " " TG_PRODUCT_NAME_END_S
#define TG_PRODUCT_NAME_W TD_PRODUCT_NAME_START_W L" " TG_PRODUCT_NAME_END_W

#define TG_MAJOR_VERSION        TD_MAJOR_VERSION
#define TG_MINOR_VERSION        TD_MINOR_VERSION
#define TG_TYPELIB_VER(MAJ,MIN) TD_TYPELIB_VER(MAJ,MIN)
#define TG_MAJOR_BUILD_VERSION  TD_MAJOR_BUILD
#define TG_MINOR_BUILD_VERSION  TD_MINOR_BUILD

#define TG_PRODUCT_VER_STR_S    TD_PRODUCT_VER_STR_S	// "3, 6, 0, 0\0"
#define TG_PRODUCT_VER_STR_W    TD_PRODUCT_VER_STR_W	// L"3, 6, 0, 0\0"

#define TG_BUILD_COMMENTS_S     TG_PRODUCT_NAME_S " " TG_PRODUCT_VER_STR_S
#define TG_BUILD_COMMENTS_W     TG_PRODUCT_NAME_W L" " TG_PRODUCT_VER_STR_W
#define TG_SHORT_STRING_VER_S   TD_SHORT_STRING_VER_S
#define TG_SHORT_STRING_VER_W   TD_SHORT_STRING_VER_W


#endif  // TG_VERSION_DEFINED

