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


#ifndef _DWFCORE_CORE_H
#define _DWFCORE_CORE_H

#ifdef HAVE_CONFIG_H
#include "dwfcore/config.h"
#endif

//
// we don't expose expat
//
#ifndef EXPAT_STATIC
#define EXPAT_STATIC
#endif

///
///\file        dwfcore/Core.h
///\brief       This is the main configuration header for the DWF Core library.
///
///             This header file should be included to determine the platform
///             setttings and internal configuration of the library.  Generally,
///             however, this file is automatically included by the interface headers.
///


    //
    // Don't want Doxygen to look at this this stuff
    //
#ifndef _DWFCORE_DEFINE_FOR_DOXYGEN_ONLY

//
// Hardware identification
//
#if     defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686) || defined(_M_IX86)
#define _DWFCORE_X86_SYSTEM
#elif   defined(_M_X64) || defined(__x86_64__) || defined (__hpux)
#define _DWFCORE_X64_SYSTEM
#elif   defined(_M_ALPHA)
#define _DWFCORE_ALPHA_SYSTEM
#elif   defined(_M_PPC)
#define _DWFCORE_PPC_SYSTEM
#elif   defined(_M_MRX000) || defined(_MIPS_)
#define _DWFCORE_MIPS_SYSTEM
#elif   defined(_M_M68K) || defined(_68K_)
#define _DWFCORE_68K_SYSTEM
#endif

#include "OdPlatformSettings.h"
#ifdef ODA_BIGENDIAN
#define _DWFCORE_BIG_ENDIAN_SYSTEM
#undef  _DWFCORE_LITTLE_ENDIAN_SYSTEM
#else
#define _DWFCORE_LITTLE_ENDIAN_SYSTEM
#undef  _DWFCORE_BIG_ENDIAN_SYSTEM
#endif


//
// System identification
// 

#if   defined (__APPLE__) && defined (__GNUC__)
#define _DWFCORE_MAC_SYSTEM

    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/mac/Types.h"
#include "dwfcore/mac/Core.h"



#elif   defined (_MSC_VER) || (defined(__GNUC__) && defined(_WIN32))
#define _DWFCORE_WIN32_SYSTEM

#if defined(__GNUC__) && defined(_WIN32)
#define _DWFCORE_WIN32_GCC_SYSTEM
#endif

    //
    // Currently a lot of the code between the Win32 and Win64 are the same
    // hence we are defining _DWFCORE_WIN64_SYSTEM within the scope of Win32
#ifdef  _WIN64
#define _DWFCORE_WIN64_SYSTEM
#endif

    //
    // Module API identification
    //
#ifndef DWFCORE_STATIC
#ifdef  DWFCORE_EXPORT_API
#define _DWFCORE_API    __declspec( dllexport )
#else
#define _DWFCORE_API    __declspec( dllimport )
#endif
#else
#define _DWFCORE_API
#endif


#undef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY

#include "dwfcore/win32/Types.h"
#include "dwfcore/win32/Core.h"

    //
    // MSVC compiler help us out?
    //
#ifndef _DWFCORE_LITTLE_ENDIAN_SYSTEM
#define _DWFCORE_LITTLE_ENDIAN_SYSTEM
#undef  _DWFCORE_BIG_ENDIAN_SYSTEM
#endif



#elif   defined (__VMS)
#define _DWFCORE_VMS_SYSTEM

    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/vms/Types.h"
#include "dwfcore/ansi/Core.h"

#elif   defined (__sgi)
#define _DWFCORE_SGI_SYSTEM

    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/ansi/Types.h"
#include "dwfcore/ansi/Core.h"

#elif   defined (__sun)
#define _DWFCORE_SUN_SYSTEM

    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/ansi/Types.h"
#include "dwfcore/ansi/Core.h"

#ifndef __GNUC__
#define USING(a) using a;
#endif

#elif   (defined (__linux__) || defined(EMCC))
#define _DWFCORE_LINUX_SYSTEM
#ifndef BYTE
typedef unsigned char BYTE;
#endif
    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/ansi/Types.h"
#include "dwfcore/ansi/Core.h"


#elif   defined (BSD)
#define _DWFCORE_BSD_SYSTEM

    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/ansi/Types.h"
#include "dwfcore/ansi/Core.h"

#elif   defined (__BCPLUSPLUS__)

#define _DWFCORE_WIN32_SYSTEM

    //
    // Module API identification
    //
#ifndef DWFCORE_STATIC
#ifdef  DWFCORE_EXPORT_API
#define _DWFCORE_API    __declspec( dllexport )
#else
#define _DWFCORE_API    __declspec( dllimport )
#endif
#else
#define _DWFCORE_API
#endif

#undef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY

#include "dwfcore/win32/Types.h"
#include "dwfcore/win32/Core.h"
#include "dwfcore/dwf_porting.h"

#else
#define _DWCORE_UNKNOWN_ANSI_SYSTEM

    //
    // Module API identification
    //
#define _DWFCORE_API

#ifndef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#define  _DWFCORE_FOR_ANSI_SYSTEM_ONLY
#endif

#include "dwfcore/ansi/Types.h"
#include "dwfcore/ansi/Core.h"

#endif


//
// Some compilers (msvc 7.0) have a problem with the 'LL' suffix.
//
#if !defined( _DWFCORE_LARGE_CONSTANT )
#define _DWFCORE_LARGE_CONSTANT(x) x##LL
#endif


#endif   // _DWFCORE_DEFINE_FOR_DOXYGEN_ONLY


///
///\brief       Cross-platform core library.
/// 
///             This namespace is used for every function, interface, class 
///             and enumeration defined and exposed by this library.
///
namespace DWFCore
{
    ///
    ///\class   DWFCoreMemory      dwfcore/Core.h  "dwfcore/Core.h"
    ///\brief   Base class for all core library objects.
    ///\since   1.0.1
    ///
    ///\warning Do not derive abstract interfaces from this base.
    ///
    ///         If configured with the \b DWFCORE_ENABLE_MEMORY_BASE
    ///         macro, \a new() and \a delete() operators are defined
    ///         for every concrete class in library.  Their implementation
    ///         may be customized or the global operators can be automatically
    ///         provided by also defining the \b DWFCORE_USE_DEFAULT_MEMORY_BASE_IMPL macro.
    ///
    class _DWFCORE_API DWFCoreMemory
    {
    #ifdef  DWFCORE_ENABLE_MEMORY_BASE
        public:

            ///
            ///     Allocation Operator
            ///
            void* operator new( size_t );

            ///
            ///     Placement Allocation Operator
            ///
            void* operator new( size_t, void* );

            ///
            ///     Deallocation Operator
            ///
            void  operator delete( void* );

            ///
            ///     Placement Deallocation Operator
            ///
            void  operator delete( void*, void* );

    #endif
    };
        

    ///
    ///\brief   Adds \b 1 to \a *pValue in a single instruction or by some protected mechanism.
    ///
    ///         The actual implementation of this method is provided by a hardware (preferrably)
    ///         or software platform specific component within the library.
    ///
    ///\param   pValue A pointer to the value to be incremented.
    ///\return  The incremented value.
    ///\throw   None
    ///
    _DWFCORE_API int AtomicIncrement( volatile int* pValue ) throw();

    ///
    ///\brief   Subtracts \b 1 from \a *pValue in a single instruction or by some protected mechanism.
    ///
    ///         The actual implementation of this method is provided by a hardware (preferrably)
    ///         or software platform specific component within the library.
    ///
    ///\param   pValue A pointer to the value to be incremented.
    ///\return  The decremented value.
    ///\throw   None
    ///
    _DWFCORE_API int AtomicDecrement( volatile int* pValue ) throw();
}



#endif


