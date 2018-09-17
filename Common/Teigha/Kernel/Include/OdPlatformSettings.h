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




#ifndef _OD_PLATFORM_SETTINGS_H_
#define _OD_PLATFORM_SETTINGS_H_


#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

#define TD_UNICODE

#if defined(__GNUC__) && !defined(GCC_VERSION)
#define GCC_VERSION __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__
#endif

#if defined(_MSC_VER)
// warning C4290: C++ Exception Specification ignored
#pragma warning ( disable : 4290 )
// warning C4514: ... : unreferenced inline function has been removed
#pragma warning ( disable : 4514 )
// identifier was truncated to '255' characters in the debug information
#pragma warning ( disable : 4786 )
// class 'NAME' needs to have dll-interface to be used by clients of class NAME
#pragma warning ( disable : 4251 )
// copy constructor could not be generated
#pragma warning ( disable : 4511 ) 
// assignment operator could not be generated
#pragma warning ( disable : 4512 ) 
// alignment changed after including header
#pragma warning ( disable : 4103 ) 

#if _MSC_VER == 1900
// declaration of local variable hides previous local declaration
#pragma warning ( disable : 4456)
// declaration of local variable hides function parameter
#pragma warning ( disable : 4457)
//
#pragma warning ( disable : 4458)

#endif

#if _MSC_VER < 1600
#define NO_SHARED_PTR
#endif

#ifndef _DEBUG
#pragma warning ( disable : 4100 ) // unreferenced formal parameter
#pragma warning ( disable : 4702 ) // unreachable code
#pragma warning ( disable : 4710 ) // not inlined
#endif

#define ODRX_ABSTRACT __declspec(novtable)

#if (_MSC_VER > 1310)
/*!DOM*/
#define ODRX_OVERRIDE override
/*!DOM*/
#define ODRX_SEALED sealed
/*!DOM*/
#define ODRX_SEALED_VIRTUAL virtual
#pragma warning ( disable : 4481 ) // nonstandard extension used: override specifier 'sealed'
#else
/*!DOM*/
#define ODRX_OVERRIDE
/*!DOM*/
#define ODRX_SEALED
/*!DOM*/
#define ODRX_SEALED_VIRTUAL
#endif 

#else

#ifndef __has_feature
  #define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif

#ifndef __has_extension
  #define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif

#if (defined(__cplusplus) && (__cplusplus >= 201103L)) ||\
    (__has_feature(cxx_override_control) || __has_extension(cxx_override_control))

#ifdef ODRX_OVERRIDE
#undef ODRX_OVERRIDE
#endif
#define ODRX_OVERRIDE override
#else // override is not supported
#define ODRX_OVERRIDE
#endif 

#define ODRX_ABSTRACT
#define ODRX_SEALED
#define ODRX_SEALED_VIRTUAL

#endif  // _MSC_VER

#if defined(_MSC_VER) && (_MSC_VER > 1600)
#define ODRX_OVERRIDE_DESTRUCTOR override
#else // defined(_MSC_VER) && (_MSC_VER > 1600)
#define ODRX_OVERRIDE_DESTRUCTOR
#endif // defined(_MSC_VER) && (_MSC_VER > 1600)

#define TD_FUNC __FUNCTION__

#ifdef _MSC_VER                                    ///////////////////////// Microsoft Visual C++

  #define ODA_WINDOWS
  #define OD_USE_WMAIN
  //#define TD_USING(a)
  #if _MSC_VER <= 1200  
    #undef TD_FUNC
    #define TD_FUNC OD_T("")
  #endif

  #if _MSC_VER > 1200  //&& !defined(_WIN32_WCE) // || _MSC_VER = 1   // 1200 == VC6
    #define OD_TYPENAME typename
    #define OD_STD_ALLOCATOR_AS_IS
  #endif

  #if _MSC_VER >= 1400
    #pragma warning ( disable : 4996 )
    #ifndef _CRT_SECURE_NO_DEPRECATE
      #define _CRT_SECURE_NO_DEPRECATE
    #endif
  #endif

  #if defined(_IA64_)
    #define TD_STRICT_ALIGNMENT
  #endif

  #if defined(_WIN32_WCE)  ///////////////////////// Windows CE

    #define OD_TYPENAME typename
    #define OD_STRING_FNS
    #define OD_NO_GETENV

    #ifdef ARM
      #define TD_STRICT_ALIGNMENT
    #endif // ARM

  #define OD_USE_STD_LIST
  #define OD_HAVE_PATHFILEEXISTS_FUNC
  #define OD_HAVE_GETFILEATTRIBUTES_FUNC
  #define OD_HAVE_SHLWAPI_FILE
  #define OD_HAVE_MEM_H_FILE

  #define OD_NONEED_ALLOC_REDEFINITION
  #define _IOFBF          0x0000
  #endif // _WIN32_WCE

  #if defined(_WINRT)
    #define OD_NO_GETENV
    #ifdef ARM
      #define TD_STRICT_ALIGNMENT
    #endif //ARM
    #define OD_HAVE_WINDOWS_STORAGE_FILE
    #define OD_HAVE_WRL_FILE
    #define OD_BUILD_EXE_AS_LIBRARY
    #define OD_NEED_S_ISDIR_FUNC

    #define GetDateFormat(lpLocaleName, dwFlags, lpDate, lpFormat, lpDateStr, cchDate) GetDateFormatEx(lpLocaleName, dwFlags, lpDate, lpFormat, lpDateStr, cchDate, NULL)
    #define GetTimeFormat(Locale, dwFlags, lpTime, lpFormat, lpTimeStr, cchTime) GetTimeFormatEx(Locale, dwFlags, lpTime, lpFormat, lpTimeStr, cchTime)

    #define access _access
    #define R_OK 04
    #define W_OK 02
  #endif // _WINRT


  #ifndef _WIN32_WCE
    // Available functoinality
    #define OD_HAVE_WSTAT_FUNC
    #define OD_HAVE_SETLOCALE_FUNC
    #define OD_HAVE_PRINT_FUNC
    #define OD_HAVE_REMOVE_FUNC
    // Available header files
    #define OD_HAVE_WINSPOOL_FILE
    #define OD_HAVE_FSTREAM_FILE
    #define OD_HAVE_SSTREAM_FILE
    #define OD_HAVE_IO_FILE
    #define OD_HAVE_SYS_STAT_FILE
    #define OD_HAVE_SYS_TYPES_FILE
    #define OD_HAVE_IOMANIP_FILE
    #define OD_HAVE_ERROR_FILE
    #define OD_HAVE_TIME_FILE

    #define ODA_USE_FTIME
    // Use extended functionality
    #define OD_USE_EXTENDED_FUNCTIONS
  #endif //_WIN32_WCE

  #ifndef _WINRT
    #define OD_HAVE_FINDFIRSTFILE_FUNC
    #define OD_HAVE_MATH_FILE
    #define OD_HAVE_OLE_ITEM_HANDLER_MODULE
    #define OD_HAVE_GET_LOCALE_INFO

    #ifndef RC_INVOKED // prevent RC4011 warning
      #define OD_HAS_OPENGL
      #define OD_HAS_GDI
      #define OD_HAS_GLES2
    #endif // !RC_INVOKED
  #endif // _WINRT

  #if !defined(_WIN32_WCE) && !defined(_WINRT)
    #define OD_HAVE_GETSTDHANDLE_FUNC
    #define OD_WINDOWS_DESKTOP
    #define OD_HAS_DIRECTX
    #define OD_HAS_SYSTEM_FUNC
  #endif

  #define OD_HAVE_WINDOWS_FILE
  #define OD_HAVE_DIRECT_FILE
  #define OD_LOGB _logb

#elif defined(__sun)                               ///////////////////////// Solaris

  #define TD_STRICT_ALIGNMENT
  #define OD_STRING_FNS
  #define OD_STD_ALLOCATOR
  #define ODA_UNIXOS

  #ifndef __GNUC__
    #undef TD_FUNC
    #define TD_FUNC OD_T("")
  #else
    #ifdef CS
      #undef CS
    #endif
    #define OD_USE_STD_LIST
    #define OD_STD_ALLOCATOR_AS_IS
  #endif
  
  #define OD_TYPENAME typename

  #ifndef __GNUC__
    #define OD_TYPENAME2 typename
    #define OD_LINKEDARRAY_SCOPE OdLinkedArray<T, A>::
    #define OD_HAVE_SPECIFIC_DISTANCE_FUNC
  #endif

  #if defined(sparc)
    #define ODA_BIGENDIAN
  #endif

  #if defined(__GNUC__)
    #define TD_USING(a) using a
    #define ODA_NEED_TEMP_USING
    #define OD_SUN_GCC
  #endif

  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_CLOCALE_FILE
  #define OD_HAVE_FUNCTIONAL_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_RESOURCE_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC

  #define ODA_USE_FTIME
#elif defined(ANDROID)                               ///////////////////////// Android

  #if defined(__arm__)
    #define TD_STRICT_ALIGNMENT
  #endif // ARM
  #define OD_STRING_FNS
  #define OD_TYPENAME typename
  #define ODA_UNIXOS
  #define ODA_NEED_TEMP_USING
  #define OD_STD_ALLOCATOR_AS_IS
  #define OD_NO_TMPDIR
  #define OD_CONVERT_UNICODETOUTF8

  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_RESOURCE_FILE
  #define OD_HAVE_MATH_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC

  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2

  #define OD_BUILD_EXE_AS_LIBRARY
  #define ODA_USE_FTIME
#elif defined(__APPLE__) && defined(__GNUC__)    ///////////////////////// Mac OS/X

  #ifdef __IPHONE_OS_VERSION_MIN_REQUIRED        // iOS devices require aligned doubles
    #define TD_STRICT_ALIGNMENT
    #define OD_BUILD_EXE_AS_LIBRARY
  #endif

  #define OD_STRING_FNS

  #if defined(__BIG_ENDIAN__) || !defined(__LITTLE_ENDIAN__)
    #define ODA_BIGENDIAN
  #endif
  #define OD_TYPENAME typename
  #define ODA_UNIXOS
  #define ODA_NEED_TEMP_USING
  #define OD_STD_ALLOCATOR_AS_IS
  #define OD_CONVERT_UNICODETOUTF8

  #define ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER
  #define ODA_LINKED_WITH_FRAMEWORK_FONDATION
  // Available functionality
  #define OD_HAVE_UUID_FUNC
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_NEED_ADDITIONAL_OUTPUT
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAVE_GLOB_FUNC
  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_UUID_UUID_FILE
  #define OD_HAVE_DLFCN_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_RESOURCE_FILE
  #define OD_HAVE_MATH_FILE
  #ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
  #define OD_HAS_SYSTEM_FUNC
  #endif

  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2

  #define ODA_USE_FTIME
#elif defined(sgi)                                 ///////////////////////// SGI

  #define OD_STRING_FNS
  #define ODA_BIGENDIAN
  #define OD_STD_ALLOCATOR
  #ifdef ODA_SGI_721 
    #define OD_TYPENAME 
  #else 
    #define OD_TYPENAME typename 
  #endif 
  #define ODA_UNIXOS
  #undef TD_FUNC
  #define TD_FUNC OD_T("")

  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_RESOURCE_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC

  #define OD_REDEFINE_HEAP_OPERATORS
  #define ODA_USE_FTIME
#elif defined(__hpux)                              ///////////////////////// HP RISC/Itanium

  #define TD_STRICT_ALIGNMENT
  #define OD_STRING_FNS
  #define ODA_BIGENDIAN
  #define OD_STD_ALLOCATOR
  #define ODA_NEED_TEMP_USING
  #define ODA_FORMAT
  #define ODA_UNIXOS

  // Requires typename keyword in some contexts, which are illegal with other compilers.
  #define OD_TYPENAME typename

  #define TD_USING(a) using a;

  #define OD_REDEFINE_STD 1
  #define OD_USING_BUGWARE 1

  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_FUNCTIONAL_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_RESOURCE_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC

  #define OD_REDEFINE_HEAP_OPERATORS
  #define ODA_USE_FTIME
#elif defined(_AIX)                                //////////////////////// IBM AIX

  #define OD_STRING_FNS
  #define ODA_BIGENDIAN
  #define OD_STD_ALLOCATOR_AS_IS
  #define ODA_NEED_TEMP_USING
  // Requires typename keyword in some contexts, which are illegal with other compilers.
  #define OD_TYPENAME typename
  #define OD_TYPENAME3 typename
  #define ODA_UNIXOS

  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_RESOURCE_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC

  #define OD_REDEFINE_HEAP_OPERATORS
  #define ODA_USE_FTIME
#elif defined(__BCPLUSPLUS__)|| defined(__BORLANDC__) //////////////////////// Borland C++
  #define ODA_WINDOWS
  #define OD_USE_WMAIN
  #undef TD_FUNC
  #define TD_FUNC __FUNC__

  #define OD_STD_ALLOCATOR_AS_IS
  #define ODA_GCC_2_95
  #define OD_BSEARCH std::bsearch
  #define OD_LOGB _logb

  // Available finctionality
  #define OD_HAVE_WSTAT_FUNC
  #define OD_HAVE_FINDFIRSTFILE_FUNC
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_GET_LOCALE_INFO
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC

  // Available header files
  #define OD_HAVE_IO_FILE
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_WINDOWS_FILE

  #define OD_HAVE_GETSTDHANDLE_FUNC
  #define OD_WINDOWS_DESKTOP
  #define OD_HAS_SYSTEM_FUNC

  #define OD_HAVE_OLE_ITEM_HANDLER_MODULE

  #define OD_HAS_OPENGL
  #define OD_HAS_GDI
  #define OD_HAS_GLES2

  #define ODA_USE_FTIME
#elif defined(__GNUC__) && defined(_WIN32)
  #define ODA_WINDOWS
  #define ODA_WINDOWS_GCC
  #define OD_WINDOWS_DESKTOP
  #define OD_POSIX_THREADS

  #define OD_TYPENAME typename
  #define OD_STD_ALLOCATOR_AS_IS

  #define OD_LOGB ilogb
  #define ODA_NEED_TEMP_USING

  #define OD_HAVE_RPC_H_FILE
  #define OD_HAVE_WINBASE_H_FILE

  // Available header files/functionality
  #define OD_HAVE_IO_FILE
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_STDIO_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC
  #define OD_HAVE_GLOB_FUNC

  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2

  #define ODA_USE_FTIME

  // Available functoinality
  #define OD_HAVE_WSTAT_FUNC
  // Available header files
  #define OD_HAVE_WINSPOOL_FILE
  #define OD_HAVE_FSTREAM_FILE
  #define OD_HAVE_SSTREAM_FILE

#elif defined(__linux__)|| defined(__FreeBSD__)  || defined(__MVS__)                               ///////////////////////// Linux/FreeBSD/MVS

  #define OD_STRING_FNS
  #define OD_TYPENAME typename
  #define ODA_UNIXOS
  #define ODA_NEED_TEMP_USING
  #define OD_STD_ALLOCATOR_AS_IS
  #define OD_CONVERT_UNICODETOUTF8
  #define ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER
  // Linux ppc requires big endian byte ordering
  #if defined(__BIG_ENDIAN__) || defined(__s390__)
    #define ODA_BIGENDIAN
  #endif
  #if defined(ARM) ||  defined(EMCC)
    #define TD_STRICT_ALIGNMENT
  #endif

  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_RESOURCE_FILE
  // Available functionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC
  #define OD_HAVE_GLOB_FUNC
  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2

  #define ODA_USE_FTIME
#elif defined(__MWERKS__)                          //////////////////////// Metrowerks Codewarrior

  #include <TargetConditionals.h>

  #if TARGET_OS_MAC
    #define ODA_BIGENDIAN
    #define OD_STRING_FNS
    #define OD_NO_GETENV
  #endif
  #define OD_HAVE_TIME_FILE

  // Available header files/functionality
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_MATH_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_RESOURCE_FILE

  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC
  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2
#elif defined(__CYGWIN32__) || defined(__MINGW32__) //////////////////////// MINGW (Cygwin)

  #define ODA_UNIXOS
  #define OD_TYPENAME typename
  #define OD_LOGB ilogb
  #define OD_STRING_FNS
  #define ODA_NEED_TEMP_USING
  // Available header files/functionality
  #define OD_HAVE_IO_FILE
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_STDIO_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC

  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2

  #define ODA_USE_FTIME
#elif defined(EMCC)                                 //////////////////////// EMCC

  #define ODA_UNIXOS
  #define OD_TYPENAME typename
  #define OD_LOGB ilogb
  #define OD_STRING_FNS
  #define ODA_NEED_TEMP_USING
  #define OD_STD_ALLOCATOR_AS_IS
  #define TD_STRICT_ALIGNMENT
  #define OD_CONVERT_UNICODETOUTF8
  #define ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER
  // Available header files
  #define OD_HAVE_SYS_STAT_FILE
  #define OD_HAVE_SYS_TYPES_FILE
  #define OD_HAVE_IOMANIP_FILE
  #define OD_HAVE_ERROR_FILE
  #define OD_HAVE_TIME_FILE
  #define OD_HAVE_UNISTD_FILE
  #define OD_HAVE_SYS_TIME_FILE
  #define OD_HAVE_DIRENT_FILE
  #define OD_HAVE_STDIO_FILE
  #define OD_HAVE_RESOURCE_FILE
  // Available finctionality
  #define OD_HAVE_SETLOCALE_FUNC
  #define OD_HAVE_PRINT_FUNC
  #define OD_HAVE_REMOVE_FUNC
  #define OD_HAS_SYSTEM_FUNC

  #define OD_HAS_OPENGL
  #define OD_HAS_GLES2

  #define ODA_USE_FTIME
#else                                              //////////////////////// Unsupported

#error "Unsupported Platform/Compiler"
#endif

#ifndef OD_LOGB
  #define OD_LOGB logb
#endif

#if !defined(ODA_WINDOWS)
// @@@TODO: check for this platforms
#if (!defined(__sun) && !defined(__hpux) && !defined(sgi))
  #define OD_POSIX_THREADS
#endif
#define OD_HAVE_COMPLEX_FILE
#endif

#if defined(TARGET_OS_MAC) && !defined(__MACH__)
#define OD_HAVE_CONSOLE_H_FILE
#define OD_HAVE_CCOMMAND_FUNC
#endif

#if defined(__APPLE__) || __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ > 10)
#define OD_HAVE_MKSTEMPS_FUNC
#endif
#define OD_T(x) ( (const OdChar*)__OD_T(x))
#ifdef ODA_WINDOWS_GCC
#define __OD_T(x) L##x
#else
#define __OD_T(x) L ## x
#endif

#if defined(ODA_WINDOWS) && !defined(ODA_WINDOWS_GCC)
#if _MSC_VER >= 1900
#define OD_CONCAT(a, b) __OD_T(a) __OD_T(b)
#else
#define OD_CONCAT(a, b) __OD_T(a) ## __OD_T(b)
#endif
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 1408)
  #define DDLL(a) a##ULL
#else
  #define DDLL(a) a
#endif
#else
  #if defined(__sun) || defined(sgi) || defined(__hpux) || defined(_AIX)
    #define OD_CONCAT(a, b) L##a L##b
  #else
    #define OD_CONCAT(a, b) L##a b
  #endif
  #define DDLL(a) a##ULL
#endif

#if defined(_NATIVE_WCHAR_T_DEFINED)
typedef unsigned short OdChar;
typedef unsigned short OdUChar;
#else
typedef wchar_t OdChar;
typedef wchar_t OdUChar;
#endif

#if defined(ODA_WINDOWS)
#define ODCHAR_IS_INT16LE 1
#else
#define ODCHAR_IS_INT16LE 0
#endif

typedef signed char OdInt8;
typedef short OdInt16;
typedef int OdInt;

typedef unsigned char OdUInt8;
typedef unsigned short OdUInt16;

#if   UINT_MAX == 0xFFFFFFFFUL
#define OD_SIZEOF_INT  4
#elif UINT_MAX > 0xFFFFFFFFU && UINT_MAX == 0xFFFFFFFFFFFFFFFFU
#error "8 byte size of `int' type unsupported!"
#else
#error "Unsupported number of *bytes* in `int' type!"
#endif

#if   ULONG_MAX == 0xFFFFFFFFUL
#define OD_SIZEOF_LONG  4
#elif (ULONG_MAX > 0xFFFFFFFFU && ULONG_MAX == 0xFFFFFFFFFFFFFFFFU) || (defined(sparc) && defined(_LP64))
#define OD_SIZEOF_LONG  8
#else
#error "Unsupported number of *bytes* in `long' type!"
#endif

#if OD_SIZEOF_LONG == 4
typedef long OdInt32;
typedef unsigned long OdUInt32;
#else // assumes 4-byte int type
typedef int OdInt32;
typedef unsigned int OdUInt32;
#endif

#if defined(_IA64_) || defined(ODA_WINDOWS)
#define BUILTIN_INT64 __int64
#define OD_ATOI64 _atoi64
#define PERCENT18LONG "%18I64d\r\n"
#define HANDLEFORMAT "%I64X\r\n"

#elif OD_SIZEOF_LONG == 8
#define BUILTIN_INT64 long
#define OD_ATOI64 atol
#define PERCENT18LONG  "%18ld\r\n"
#define HANDLEFORMAT  "%lX\r\n"

#elif defined(ODA_UNIXOS)
#define BUILTIN_INT64 long long
#define OD_ATOI64 atoll
#if (defined(__hpux) && !defined(atoll) && defined(__cplusplus)) || defined(_AIX_SOURCE)
inline long long atoll(const char *instr)
{
  long long retval;
  int i;

  retval = 0;
  for (; *instr; instr++) {
    retval = 10*retval + (*instr - '0');
  }
  return retval;
}
#endif
#define PERCENT18LONG "%18lld\r\n"
#define HANDLEFORMAT "%llX\r\n"
#endif

#ifdef BUILTIN_INT64
typedef BUILTIN_INT64 OdInt64;
typedef unsigned BUILTIN_INT64 OdUInt64;
#else
#include "Int64.h"
#define OD_ATOI64 atoi // TODO
#error ("sprinf can't process this without built-in in64 type")
#define PERCENT18LONG "%18d\r\n" // TODO
#define HANDLEFORMAT "%X\r\n"
#endif

// C99 definitions for int64 portable printf format
#ifdef _MSC_VER
#ifndef PRId64W
#define PRId64W L"%I64d"
#endif
#ifndef PRIu64W
#define PRIu64W L"%I64u"
#endif
#ifndef PRIx64W
#define PRIx64W L"%I64x"
#endif
#ifndef PRIX64W
#define PRIX64W L"%I64X"
#endif
#ifndef PRId64
#define PRId64 "%I64d"
#endif
#ifndef PRIu64
#define PRIu64 "%I64u"
#endif
#ifndef PRIx64
#define PRIx64 "%I64x"
#endif
#ifndef PRIX64
#define PRIX64 "%I64X"
#endif
#else // ifdef _MSC_VER
#ifndef PRId64W
#define PRId64W L"%lld"
#endif
#ifndef PRIu64W
#define PRIu64W L"%llu"
#endif
#ifndef PRIx64W
#define PRIx64W L"%llx"
#endif
#ifndef PRIX64W
#define PRIX64W L"%llX"
#endif
#ifndef PRId64
#define PRId64 "%lld"
#endif
#ifndef PRIu64
#define PRIu64 "%llu"
#endif
#ifndef PRIx64
#define PRIx64 "%llx"
#endif
#ifndef PRIX64
#define PRIX64 "%llX"
#endif
#endif //ifdef _MSC_VER


typedef void * VoidPtr;
typedef ptrdiff_t OdIntPtr; // Integer able to store a pointer (32 or 64 depending on platform)
#if (OD_SIZEOF_LONG == 4) && !defined(_WIN64) && !defined(__hpux) && !defined(_AIX)
#define OD_INTPTR_UNIQUE
#endif

typedef ptrdiff_t OdGsMarker; // Integer able to store a pointer (32 or 64 depending on platform)

typedef ptrdiff_t OdHWnd;


#if defined (_WIN64)
#define OD_SIZEOF_PTR 8
#else
// This may need to be adjusted
#define OD_SIZEOF_PTR OD_SIZEOF_LONG
#endif

#define OdIntToPtr(arg)   ((void*)((ptrdiff_t)(arg)))

#define OdPtrToInt32(arg)   ((OdInt32)((ptrdiff_t)(arg)))

#define OdIntToGsMarker(arg) ((OdGsMarker)((ptrdiff_t)(arg)))

//////////////////////////////////////////////////////////////////////////
//  Non-MSVC dynamic library loading.

#ifdef  _TOOLKIT_IN_DLL_

  #if defined(ODA_WINDOWS)

    #pragma warning( disable: 4275 4251 )
    #define OD_DLL_EXPORT __declspec(dllexport)
    #define OD_DLL_IMPORT __declspec(dllimport)

  #elif defined(ODA_UNIXOS) // GNU compiler MacOS X (xCode), unix, linex and others

    #define OD_DLL_EXPORT __attribute__((visibility("default")))
    #define OD_DLL_IMPORT __attribute__((visibility("default")))

  #endif

  #define OD_EXTERN  extern

  #if defined(_MSC_VER)
    #define OD_TOOLKIT_EXPORT OD_DLL_EXPORT
    #define OD_STATIC_EXPORT
    #define OD_TOOLKIT_IMPORT 
    #define OD_STATIC_IMPORT OD_DLL_IMPORT
  #elif defined(__BORLANDC__)
    #define OD_TOOLKIT_EXPORT  OD_DLL_EXPORT
    #define OD_STATIC_EXPORT
    #define OD_TOOLKIT_IMPORT  OD_DLL_IMPORT
    #define OD_STATIC_IMPORT
  #else
    #define OD_TOOLKIT_EXPORT  OD_DLL_EXPORT
    #define OD_STATIC_EXPORT
    #define OD_TOOLKIT_IMPORT
    #define OD_STATIC_IMPORT   OD_DLL_IMPORT
  #endif

#else   // of _TOOLKIT_IN_DLL_

  #define OD_TOOLKIT_EXPORT
  #define OD_STATIC_EXPORT
  #define OD_TOOLKIT_IMPORT
  #define OD_STATIC_IMPORT

  #define OD_DLL_EXPORT
  #define OD_DLL_IMPORT
  #define OD_EXTERN

#endif  // of _TOOLKIT_IN_DLL_
//////////////////////////////////////////////////////////////////////////

#if (defined(__GNUC__ ) && !defined(__FreeBSD__) )  ||  (defined(_SOLX64))
  #define TD_USE_VA_COPY
#endif

#if (defined EMCC && !defined(OD_TYPENAME))
#define OD_TYPENAME typename
#endif

#endif //#ifndef _OD_PLATFORM_SETTINGS_H_

