//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#if !defined WHIPCORE_HEADER
#define WHIPCORE_HEADER

///
///\file        dwf/whiptk/whipcore.h
///

//////////////////////////////////////////////////////////////////////////
//
// Recognize the newer DWF Toolkit pre-processor macros
// 
//
#ifdef  DWFTK_STATIC
#ifndef WHIP_STATIC_LIB
#define WHIP_STATIC_LIB
#endif
#endif

#ifdef  DWFTK_READ_ONLY
#ifdef  CODE_NOT_WANTED
#undef  CODE_NOT_WANTED
#endif
#define CODE_NOT_WANTED             WHIP_OUTPUT
#endif

#ifdef  WhipTk_EXPORTS
#undef  WHIPTOOL_CLIENT
#undef  WHIP_STATIC_LIB
#endif
//////////////////////////////////////////////////////////////////////////



//
// There are many products now using the Toolkit, however many
// of these products use only a fraction of the code.  The
// following 'defines' allow a product makefile to exclude
// certain types of routines from the build by defining
// the CODE_NOT_WANTED variable.  I know it makes the code a bit
// ugly having a bunch of #if's everywhere, but the Web demands
// small downloads... If you have a better idea, let me know... BPM
//
#ifndef CODE_NOT_WANTED
#   define CODE_NOT_WANTED 0
#endif
#define WHIP_OUTPUT 0x10000000
#define WHIP_INPUT 0x20000000
// DESIRED_CODE is defined in HEIDI's core.h if needed, and is the same as the one here.
#ifndef DESIRED_CODE
#define DESIRED_CODE(bits)  (!((bits) & (CODE_NOT_WANTED)))
#endif

#if(!DESIRED_CODE(WHIP_INPUT))
#pragma message ("Whip input code disabled")
#endif
#if(!DESIRED_CODE(WHIP_OUTPUT))
#pragma message ("Whip output code disabled")
#endif

#include <stdlib.h>
#include <OdPlatformSettings.h>

#ifdef ODA_BIGENDIAN
#define             WD_BIGENDIAN       1
#else
#define             WD_BIGENDIAN       0
#endif

#if   defined   __sgi
    #define             WD_SGI_SYSTEM
    #define WHIP_USE_WCHAR_STRINGS

#elif   defined   __hpux
#define             WD_HP_SYSTEM
#define WHIP_USE_WCHAR_STRINGS
#define _stat stat

#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

#elif   defined   _AIX
#define             WD_AIX_SYSTEM
#define WHIP_USE_WCHAR_STRINGS
#define _stat stat
#elif   defined   __sun
#define             WD_SUN_SYSTEM
#define WHIP_USE_WCHAR_STRINGS
#ifndef __GNUC__
#define USING(a) using a;
#endif
#define _stat stat
#define _S_IFDIR S_IFDIR

#elif defined   __linux || defined ANDROID || defined EMCC
    #include <endian.h>
    #define             WD_LINUX_SYSTEM

    #define _stat stat
    #define _S_IFDIR S_IFDIR
  
    #define WHIP_USE_WCHAR_STRINGS

    //
    // GNU macros messing with class methods... strange
    // for example, matrix.h will complain about minor()
    //
    #ifdef major
        #undef major
    #endif

    #ifdef minor
        #undef minor
    #endif
    #define             WD_PREFERRED_RGB32     b, g, r, a      // GDI's preference

#elif defined (macintosh) || (defined (__MACH__) && defined (__APPLE__))
    #define     WD_MAC_SYSTEM

    #define WHIP_USE_WCHAR_STRINGS

    //
    // GNU macros messing with class methods... strange
    // for example, matrix.h will complain about minor()
    //
    #ifdef major
        #undef major
    #endif

    #ifdef minor
        #undef minor
    #endif

    #define             WD_PREFERRED_RGB32     b, g, r, a      // GDI's preference

#elif defined   _MSC_VER || defined __BCPLUSPLUS__ || (defined(__GNUC__) && defined(_WIN32))
    #define             WD_WIN32_SYSTEM
    //#define           WD_PREFERRED_RGB32         r, g, b, a // OpenGL's preference
    #define             WD_PREFERRED_RGB32         b, g, r, a // GDI's preference
    #define             WD_NEWLINE          "\r\n"

#if defined(__GNUC__)
    #define WHIP_USE_WCHAR_STRINGS
#endif

    #ifndef WHIP_STATIC_LIB
        #if defined WHIPTOOL_CLIENT
            #ifdef WHIPTOOL_CLIENT_NODECLSPEC
                  #define         WHIPTK_API
            #pragma message ("Whip client (no declspec) ")
            #else
                  #define         WHIPTK_API  __declspec(dllimport)
            #pragma message ("Whip client (dllimport) ")
            #endif
        #else
            #define         WHIPTK_API  __declspec(dllexport)
            #pragma message ("Whip server (dllexport) ")
        #endif
        #define     WHIPTOOL_LOADABLE  extern "C" __declspec(dllexport)
    #else
        #define WHIPTK_API
        #define WHIPTOOL_LOADABLE  extern "C"
        #pragma message ("Whip static library ")
    #endif
#if defined __BCPLUSPLUS__
#define WHIP_USE_WCHAR_STRINGS
#endif 

    #define     WHIPTOOL_CALL      __cdecl

    #pragma warning (disable : 4127)  // conditional expression is constant (used in "do {...} while (0)" for macro safety)
    #pragma warning (disable : 4251)  // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'

    #if defined (_DEBUG) && defined (_AFXDLL)
        #include <afx.h>
        #if !defined WHIPTOOL_CLIENT
            #define new DEBUG_NEW
        #endif
    #else
        #include <windows.h>
    #endif

#else
    #error      "Fatal WHIP! toolkit build error -- Unknown system type"
#endif


// default definitions if not set per-system

#if !defined WD_PREFERRED_RGB32
    #define     WD_PREFERRED_RGB32          a, b, g, r
#endif

#if !defined WD_NEWLINE
    #define     WD_NEWLINE                  "\n"
#endif

#if !defined WD_SET_MEMORY
    #include <string.h>
    #define     WD_SET_MEMORY(dest, size, value)   memset ((dest), (value), (size))
#endif

#if !defined WD_COPY_MEMORY
    #include <string.h>
    #define     WD_COPY_MEMORY(src, count, dest)   memcpy ((dest), (src), (count))
#endif

#if !defined WHIPTK_API
    #define     WHIPTK_API
#endif

#if !defined WHIPTOOL_LOADABLE
    #define     WHIPTOOL_LOADABLE          extern "C"
#endif

#if !defined WHIPTOOL_CALL
    #define     WHIPTOOL_CALL
#endif

//
// The presence of _DEBUG contraindicates "production" code.
// Control all other debugging flags with this global
// sense of debuggability.
//
#if defined _DEBUG
    #define DEBUG_ASSERTIONS    // enables assertions
#endif

extern WHIPTK_API  void WHIPTOOL_CALL WD_Fail_Assertion (char const *assertion, char const * file, int line);
#if defined DEBUG_ASSERTIONS
    #define WD_Complain(reason) WD_Fail_Assertion (reason, __FILE__, __LINE__)
    #define WD_Assert(condition) \
       for(;;) {if (!(condition)) WD_Complain (#condition); break; }
#else
    #define WD_Complain(reason)
    #define WD_Assert(condition)
#endif

#if defined (_DEBUG) && defined (_MSC_VER)
    #define CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

#include "whiptk/typedefs_defines.h"
#include "whiptk/whiperrs.h"
#include "whiptk/rgb.h"
#include "whiptk/logical_point.h"
#include "whiptk/point.h"
#include "whiptk/logical_box.h"
#include "whiptk/transform.h"
#include "whiptk/wtstring.h"
#include "whiptk/opcode_defs.h"

#endif // WHIPCORE_HEADER

