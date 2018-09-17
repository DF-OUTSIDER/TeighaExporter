//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFCORE_TYPES_WIN32_H
#define _DWFCORE_TYPES_WIN32_H

///
///\file        dwfcore/win32/Types.h
///\brief       This is a Microsoft Windows configuration header for the DWF Core library.
///

#ifdef  _DWFCORE_WIN32_SYSTEM


#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>

#define _DWFCORE_WCHAR_MAX      SHRT_MAX
#define _DWFCORE_SHRT_MAX       SHRT_MAX
#define _DWFCORE_INT_MAX        INT_MAX

typedef char                    ASCII_char_t;
typedef wchar_t                 UCS2_char_t;
typedef unsigned __int32        UCS4_char_t;

#ifndef int16_t
typedef __int16                 int16_t;
#endif

#ifndef uint16_t
typedef unsigned __int16        uint16_t;
#endif

#ifndef int32_t
typedef __int32                 int32_t;
#endif

#ifndef uint32_t
typedef unsigned __int32        uint32_t;
#endif

#ifndef int64_t
typedef __int64                 int64_t;
#endif

#ifndef uint64_t
typedef unsigned __int64        uint64_t;
#endif




#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif


#endif

