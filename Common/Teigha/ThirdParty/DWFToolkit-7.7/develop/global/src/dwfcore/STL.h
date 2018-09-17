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

#ifndef _DWFCORE_STL_H
#define _DWFCORE_STL_H

#include "dwfcore/Core.h"

// VC9.0 can set _SECURE_SCL to be 0 which is 1 by default to release configuration.
// The performance will be improved much by setting it to be 0.
// So some users would like to set it to be 0. But, if TK don't set it to be 0,
// Clients' application with 0 can't work with it. So we have to provide them 
// the DLLs with this configuration. 
#ifdef	 _SECURE_SCL_DISABLE
#include "dwfcore/CompileConfig.h"
#endif

///
///\file        dwfcore/STL.h
///\brief       This file includes all STL headers required by the library.
///

///
///\page        notes       Notes
///\section     stl         STL Header Files
///             \e xtree based collections generate unreachable code warnings when compiled
///             with Microsoft Visual Studio Compilers in non-Debug build configurations with
///             warning level 4 enabled.  STL.h includes all header files required by the
///             core library and temporarily disables the MSVC \b C4702: \b unreachable \b code warning.
///             It should be included instead of individal STL headers if your build configuration
///             requires a high level of warning notification.
///             

#ifdef  _DWFCORE_WIN32_SYSTEM
#pragma warning(push)
#pragma warning(disable: 4702)
#endif

#include <set>
#include <map>
#include <list>
#if defined(__sun) || defined (__linux__) || defined (__clang__) || defined(EMCC)
#ifdef max
#undef max
#define max max
#endif
#ifdef min
#undef min
#define min min
#endif
#endif
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

#ifdef  _DWFCORE_WIN32_SYSTEM
#pragma warning(pop)
#endif

#if defined(__hpux)
#undef major
#undef minor
#endif


#endif

