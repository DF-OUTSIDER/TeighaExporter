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

#ifndef _DWFCORE_CORE_ANSI_H
#define _DWFCORE_CORE_ANSI_H



///
///\file        dwfcore/ansi/Core.h
///\brief       This is the general ANSI configuration header for the DWF Core library.
///
///             This header file provides default definitions for build configuration macros.
///



#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

//
//
// Macros
//
//

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
#ifndef DWFCORE_COMPARE_ASCII_STRINGS_NO_CASE
#define DWFCORE_COMPARE_ASCII_STRINGS_NO_CASE( a, b )   \
        ::strcasecmp( a, b )
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
        ::wcscmp( a, b )
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
        ::wcsncmp( a, b, n )
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
        ::wcslen( s )
#endif

    //
    // calculate length of wide string in bytes
    //
#ifndef DWFCORE_WIDE_STRING_LENGTH_IN_BYTES
#define DWFCORE_WIDE_STRING_LENGTH_IN_BYTES( s )    \
        (::wcslen( s ) * sizeof(wchar_t))
#endif

    //
    // copy ascii strings
    //
#ifndef DWFCORE_ASCII_STRING_COPY
#define DWFCORE_ASCII_STRING_COPY( a, b )    \
        ::strcpy(a, b)
#endif

    //
    // copy wide strings
    //
#ifndef DWFCORE_WIDE_STRING_COPY
#define DWFCORE_WIDE_STRING_COPY( a, b )    \
        ::wcscpy(a, b)
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
        ::wcsncpy(a, b, n)
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
        ::wcscat(a, b)
#endif

    //
    // tokenize ascii strings
    //
#ifndef DWFCORE_ASCII_STRING_TOKENIZE
#define DWFCORE_ASCII_STRING_TOKENIZE( str, delim, state )    \
    ::strtok_r(str, delim, state)
#endif

    //
    // tokenize wide strings
    //
#ifndef DWFCORE_WIDE_STRING_TOKENIZE
#define DWFCORE_WIDE_STRING_TOKENIZE( str, delim, state )    \
    ::wcstok(str, delim, state)
#endif

    //
    //
    //
#define _DWFCORE_SWPRINTF   swprintf
#define _DWFCORE_SPRINTF    snprintf


#if !defined(EMCC) && __GNUC__>3 && !(defined(ANDROID) && defined(__clang__ ))
#include <bits/stl_algobase.h>
using namespace std;
#else
    //
    // max
    //
#ifndef max
#define max( a, b )    ((a) < (b) ? (b) : (a))
#endif

    //
    // min
    //
#ifndef min
#define min( a, b )    ((a) < (b) ? (a) : (b))
#endif
#endif //GNUC

//
//
// Configuration
//
//

    //
    // use the ANSI string implementation
    //
#define DWFCORE_USE_ANSI_STRING

    //
    // use ANSI specific file implementations
    //
#define DWFCORE_USE_ANSI_FILE



#endif


