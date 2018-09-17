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

#ifndef _DWFCORE_CORE_WIN32_H
#define _DWFCORE_CORE_WIN32_H


///
///\file        dwfcore/win32/Core.h
///\brief       This is the Microsoft Windows configuration header for the DWF Core library.
///
///             This header file provides default definitions for build configuration macros.
///


#ifdef  _DWFCORE_WIN32_SYSTEM

#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>

//Use common zlib
    //
    // Win32 build always uses custom ZLIB
    //
//#ifndef DWFTK_BUILD_ZLIB
//#define DWFTK_BUILD_ZLIB
//#endif


//
// Desired functionality
//
#ifdef  _WIN32_WINNT
#if     (_WIN32_WINNT < 0x0400)
#define _WIN32_WINNT    0x0400
#endif
#else
#define _WIN32_WINNT    0x0400
#endif

//
// NOTE:
// Using the c-runtime functions will generally produce smaller object files
// than if you include <windows.h> and use the Win32 APIs.
//
#include <windows.h>

#if defined(ANDROID_WCHAR)
#define USED_WCHAR_NAMESPACE Citrus
#else
#define USED_WCHAR_NAMESPACE
#endif
    //
    // byte block memory allocator
    //
#ifndef DWFCORE_ALLOC_MEMORY
#define DWFCORE_ALLOC_MEMORY( primitive_type, bytes )  \
        (primitive_type*) new primitive_type[bytes]
#endif

    //
    // byte block memory deallocator
    //
#ifndef DWFCORE_FREE_MEMORY
#define DWFCORE_FREE_MEMORY( pointer ) \
        delete [] pointer; pointer = NULL;
#endif

    //
    // object memory allocator
    //
#ifndef DWFCORE_ALLOC_OBJECT
#define DWFCORE_ALLOC_OBJECT( object_type ) \
        new object_type
#endif

    //
    // object memory deallocator
    //
#ifndef DWFCORE_FREE_OBJECT
#define DWFCORE_FREE_OBJECT( pointer )  \
        delete pointer; pointer = NULL;
#endif

    //
    // zero fill memory
    //
#ifndef DWFCORE_ZERO_MEMORY
#define DWFCORE_ZERO_MEMORY( pointer, bytes )   \
        ::memset( (void*)pointer, 0, bytes )
#endif

    //
    // copy memory
    //
#ifndef DWFCORE_COPY_MEMORY
#define DWFCORE_COPY_MEMORY( dest, src, bytes ) \
        ::memcpy( (void*)dest, (const void*)src, bytes )
#endif

    //
    // compare memory regions
    //
#ifndef DWFCORE_COMPARE_MEMORY
#define DWFCORE_COMPARE_MEMORY( a, b, bytes )   \
        ::memcmp( (const void*)a, (const void*)b, bytes )
#endif

    //
    // compare ascii strings
    //
#ifndef DWFCORE_COMPARE_ASCII_STRINGS
#define DWFCORE_COMPARE_ASCII_STRINGS( a, b )   \
        ::strcmp( a, b )
#endif

    //
    // compare ascii strings
    //
#if _MSC_VER < 1400 
    #ifndef DWFCORE_COMPARE_ASCII_STRINGS_NO_CASE
    #define DWFCORE_COMPARE_ASCII_STRINGS_NO_CASE( a, b )   \
            ::stricmp( a, b )
    #endif
#else
    #ifndef DWFCORE_COMPARE_ASCII_STRINGS_NO_CASE
    #define DWFCORE_COMPARE_ASCII_STRINGS_NO_CASE( a, b )   \
            ::_stricmp( a, b )
    #endif
#endif

    //
    // compare atmost n characters in ascii strings 
    //
#ifndef DWFCORE_COMPARE_ASCII_STRINGS_NCHARS
#define DWFCORE_COMPARE_ASCII_STRINGS_NCHARS( a, b, n )   \
        ::strncmp( a, b, n )
#endif

    //
    // compare wide strings
    //
#ifndef DWFCORE_COMPARE_WIDE_STRINGS
#define DWFCORE_COMPARE_WIDE_STRINGS( a, b )   \
       USED_WCHAR_NAMESPACE::wcscmp( a, b )
#endif

    //
    // compare wide strings
    //
#ifndef DWFCORE_COMPARE_WIDE_STRINGS_NO_CASE
#define DWFCORE_COMPARE_WIDE_STRINGS_NO_CASE( a, b )   \
        ::wcsicmp( a, b )
#endif


    //
    // compare atmost n characters in wide strings 
    //
#ifndef DWFCORE_COMPARE_WIDE_STRINGS_NCHARS
#define DWFCORE_COMPARE_WIDE_STRINGS_NCHARS( a, b, n )   \
        USED_WCHAR_NAMESPACE::wcsncmp( a, b, n )
#endif

    //
    // calculate length of ascii string
    //
#ifndef DWFCORE_ASCII_STRING_LENGTH
#define DWFCORE_ASCII_STRING_LENGTH( s )    \
        ::strlen( s )
#endif

    //
    // calculate length of wide string in wchar_t
    //
#ifndef DWFCORE_WIDE_STRING_LENGTH_IN_WCHARS
#define DWFCORE_WIDE_STRING_LENGTH_IN_WCHARS( s )    \
        USED_WCHAR_NAMESPACE::wcslen( s )
#endif

    //
    // calculate length of wide string in bytes
    //
#ifndef DWFCORE_WIDE_STRING_LENGTH_IN_BYTES
#define DWFCORE_WIDE_STRING_LENGTH_IN_BYTES( s )    \
        (USED_WCHAR_NAMESPACE::wcslen( s ) * sizeof(wchar_t))
#endif

    //
    // copy ascii strings
    //
#ifndef DWFCORE_ASCII_STRING_COPY
#define DWFCORE_ASCII_STRING_COPY( a, b )    \
        USED_WCHAR_NAMESPACE::strcpy(a, b)
#endif

    //
    // copy wide strings
    //
#ifndef DWFCORE_WIDE_STRING_COPY
#define DWFCORE_WIDE_STRING_COPY( a, b )    \
        USED_WCHAR_NAMESPACE::wcscpy(a, b)
#endif

    //
    // copy ascii strings with a given length
    //
#ifndef DWFCORE_ASCII_STRING_COPY_LENGTH
#define DWFCORE_ASCII_STRING_COPY_LENGTH( a, b, n )    \
        ::strncpy(a, b, n)
#endif

    //
    // copy wide strings with a given length
    //
#ifndef DWFCORE_WIDE_STRING_COPY_LENGTH
#define DWFCORE_WIDE_STRING_COPY_LENGTH( a, b, n )    \
        USED_WCHAR_NAMESPACE::wcsncpy(a, b, n)
#endif

    //
    // concatenate ascii strings
    //
#ifndef DWFCORE_ASCII_STRING_CONCATENATE
#define DWFCORE_ASCII_STRING_CONCATENATE( a, b )    \
        ::strcat(a, b)
#endif

    //
    // concatenate wide strings
    //
#ifndef DWFCORE_WIDE_STRING_CONCATENATE
#define DWFCORE_WIDE_STRING_CONCATENATE( a, b )    \
        USED_WCHAR_NAMESPACE::wcscat(a, b)
#endif

    //
    // tokenize ascii strings
    //
#ifndef DWFCORE_ASCII_STRING_TOKENIZE
#if _MSC_VER < 1400 
#define DWFCORE_ASCII_STRING_TOKENIZE( str, delim, state )    \
        (state, ::strtok(str, delim))
#else
#define DWFCORE_ASCII_STRING_TOKENIZE( str, delim, state )    \
        ::strtok_s(str, delim, state)
#endif    
#endif

    //
    // tokenize wide strings
    //
#ifndef DWFCORE_WIDE_STRING_TOKENIZE
#if _MSC_VER < 1400 
#define DWFCORE_WIDE_STRING_TOKENIZE( str, delim, state )    \
        (state, USED_WCHAR_NAMESPACE::wcstok(str, delim))
#else
#define DWFCORE_WIDE_STRING_TOKENIZE( str, delim, state )    \
        ::wcstok_s(str, delim, state)
#endif
#endif


    //
    // Win32 swprintf doesn't work with count parameter
    //
#define _DWFCORE_SWPRINTF   _snwprintf
#define _DWFCORE_SPRINTF    _snprintf


//
//
// Configuration
//
//

    //
    // Builds Win9x support checks into the library
    //
#ifndef DWFCORE_WIN32_INCLUDE_WIN9X_CODE
#define DWFCORE_WIN32_INCLUDE_WIN9X_CODE
#endif

    //
    // use the ANSI string implementation
    //
#ifndef DWFCORE_USE_ANSI_STRING
#define DWFCORE_USE_ANSI_STRING
#endif

    //
    // use Win32 specific file implementations
    //
#define DWFCORE_USE_WIN32_FILE
#undef  DWFCORE_USE_ANSI_FILE


//
// The MSVC 7.0 compiler is unaware of the 'LL' suffix
// for 64-bit constant values so this unfortunate check must occur.
//
#if defined( _DWFCORE_WIN32_SYSTEM ) && ( _MSC_VER <= 1300 )
#define _DWFCORE_LARGE_CONSTANT(x) x
#else
#define _DWFCORE_LARGE_CONSTANT(x) x##LL
#endif




//
//
// Platform specific utility prototypes
//
//

namespace DWFCore
{


#ifdef  DWFCORE_WIN32_INCLUDE_WIN9X_CODE


extern _DWFCORE_API bool IsWindows9x();
#endif


}


#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif

#endif



    //
    // Never explicitly define this macro - it is ONLY for use by Doxygen.
    //
#ifdef  _DWFCORE_DEFINE_FOR_DOXYGEN_ONLY

namespace DWFCore
{
    ///
    ///\brief   A legacy system type check for Microsoft Windows 95/98.
    ///
    ///\note    This function is only available in the core library
    ///         built for Microsoft Windows platforms.
    ///
    ///\return  \e true if the operating system is legacy, \e false otherwise.
    ///\throw   None
    ///
    _DWFCORE_API bool IsWindows9x();
}

#endif
