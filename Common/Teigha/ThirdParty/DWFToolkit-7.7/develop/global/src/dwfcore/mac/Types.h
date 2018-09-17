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


#ifndef DWFCORE_TYPES_MAC_H
#define DWFCORE_TYPES_MAC_H

///
///\file        dwfcore/mac/Types.h
///\brief       This is a Apple Macintosh OS X configuration header for the DWF Core library.
///


#include <stdint.h>
#include <sys/types.h>


#define _DWFCORE_WCHAR_MAX      __WCHAR_MAX__
#define _DWFCORE_SHRT_MAX       __SHRT_MAX__
#define _DWFCORE_INT_MAX        __INT_MAX__

#ifndef OBJC_BOOL_DEFINED
typedef int BOOL;
#endif
#if (_DWFCORE_WCHAR_MAX == _DWFCORE_SHRT_MAX)

typedef char        ASCII_char_t;
typedef wchar_t     UCS2_char_t;
typedef u_int32_t   UCS4_char_t;

#else

typedef char        ASCII_char_t;
typedef u_int16_t   UCS2_char_t;
typedef wchar_t     UCS4_char_t;

#endif




#endif

