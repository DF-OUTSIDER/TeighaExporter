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

#ifndef _DWFTK_TOOLKIT_H
#define _DWFTK_TOOLKIT_H


#if !defined(DWFTK_USE_SYSTEM_STL_ALLOCATOR) && !defined(_MSC_VER) 
#define DWFTK_USE_SYSTEM_STL_ALLOCATOR
#endif

#if defined (DWFTK_PORT_TO_VC6) || (defined (__BCPLUSPLUS__) && (__BCPLUSPLUS__ < 1408))
typedef unsigned long uintptr_t;
#endif

///
///\file        dwf/Toolkit.h
///\brief       This is the main configuration header for the DWF Toolkit library.
///

#include "dwfcore/Core.h"
#include "dwfcore/STL.h"

#ifdef  HAVE_CONFIG_H
#include "dwf/config.h"
#endif


#ifndef DWFTK_STATIC

#ifdef  _DWFCORE_WIN32_SYSTEM
#ifdef  DWFTK_EXPORT_API
#define _DWFTK_API          __declspec( dllexport )
#else
#define _DWFTK_API          __declspec( dllimport )
#endif
#else
#define _DWFTK_API
#endif

#else
#define _DWFTK_API

#ifndef BSTREAM_STATIC_LIB
#define BSTREAM_STATIC_LIB
#endif

#endif

///
///\brief       DWF Toolkit class library.
/// 
///             This namespace is used for every function, interface, class 
///             and enumeration defined and exposed by this library.
///
#define _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
#define _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER

#ifndef DWFTK_STATIC

#undef  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
#undef  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER

#define _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE           : virtual public _DWFToolkitMemory
#define _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER    , virtual public _DWFToolkitMemory


namespace DWFToolkit
{
    ///
    ///\class   DWFToolkitMemory      dwf/Toolkit.h  "dwf/Toolkit.h"
    ///\brief   Base class for all toolkit objects.
    ///\since   7.0.1
    ///
    ///\warning Do not derive abstract interfaces from this base.
    ///
    ///         If configured with the \b DWFTK_ENABLE_MEMORY_BASE
    ///         macro, \a new() and \a delete() operators are defined
    ///         for every concrete class in library.  Their implementation
    ///         may be customized or the global operators can be automatically
    ///         provided by also defining the \b DWFTK_USE_DEFAULT_MEMORY_BASE_IMPL macro.
    ///
    class _DWFTK_API _DWFToolkitMemory
    {
        public:

#if !defined(DWFTK_STATIC) && (!defined(_MSC_VER) || _MSC_VER >= 1300)
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

#if !defined(__BCPLUSPLUS__)		
            ///
            ///     Placement Deallocation Operator
            ///
            void  operator delete( void*, void* );
#endif			
#endif
    };
}

#endif


///
/// By default, the toolkit will use it's own STL allocator,
/// primarily so that it can ensure that new's and delete's
/// in DLL configurations all happen under the same runtime.
///
/// If your application has it's own STL allocator, and it is
/// somehow non-standard (presumably because you also have a
/// non-standard implementation of STL containers) you may
/// need to disable the toolkit allocator. You can do this
/// by building the toolkit with the macro
/// \b DWFTK_USE_SYSTEM_STL_ALLOCATOR defined.
///

#ifdef DWFTK_USE_SYSTEM_STL_ALLOCATOR
# define _DWFTK_STL_ALLOCATOR std::allocator
#else
# define _DWFTK_STL_ALLOCATOR DWFTK_STL_Allocator
#endif

namespace DWFToolkit
{
    ///
    ///\class   DWFTK_STL_Allocator  dwf/Toolkit.h  "dwf/Toolkit.h"
    ///\brief   STL allocator replacement.
    ///\since   7.2.0
    ///
    ///         This implementation of the STL allocator class ensures
    ///         that all allocations and deletions are routed through
    ///         the DWFToolkitMemory class, if it is enabled. This is
    ///         required to ensure that any STL containers have their
    ///         calls to new & delete all happen in the same runtime.
    ///
    template<class T >
    class DWFTK_STL_Allocator
            : public std::allocator<T>
            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {
    public:
        typedef std::allocator<T>                   _Mybase;
        typedef typename _Mybase::value_type        value_type;
        typedef typename _Mybase::pointer           pointer;
        typedef typename _Mybase::reference         reference;
        typedef typename _Mybase::const_pointer     const_pointer;
        typedef typename _Mybase::const_reference   const_reference;

        typedef typename _Mybase::size_type         size_type;
        typedef typename _Mybase::difference_type   difference_type;

        template<class _Other>
            struct rebind
            {    // convert a DWFTK_STL_Allocator<_Ty> to a DWFTK_STL_Allocator <_Other>
                typedef DWFTK_STL_Allocator<_Other> other;
            };

        inline DWFTK_STL_Allocator() {}
        inline ~DWFTK_STL_Allocator() {}
        inline DWFTK_STL_Allocator( const DWFTK_STL_Allocator& ) {}
        template<typename U>
        inline DWFTK_STL_Allocator( const DWFTK_STL_Allocator<U>& ) {}

        /*
        template<class _Other>
        DWFTK_STL_Allocator<T>& operator=(const DWFTK_STL_Allocator<_Other>&)
        {   // assign from a related allocator (do nothing)
            return (*this);
        }
        */

        //
        // Intentionally not using DWFCORE_ALLOC, for a few reasons:
        //   - DWFCORE_ALLOC_OBJECT doesn't work for this size_t variant of new.
        //   - DWFCORE_ALLOC_MEMORY causes random crashes in delete.
        //   - We *really* want to be sure to alloc & delete in the same dll,
        //     so we're using operator new & delete.
        //
        pointer allocate( size_type n )
        {
            size_t nBytes = n * sizeof (T);
            return ((pointer) operator new (nBytes) );
        }

        pointer allocate( size_type n, const void* /*q*/ ) // ignore hint
        {
            return allocate( n );
        }

        void deallocate( pointer p, size_type /*n*/ )
        {
            operator delete( p );
        }
    };

#ifdef DWFTK_USE_SYSTEM_STL_ALLOCATOR
# define _DWFTK_STD_VECTOR(type) std::vector<type>
# define _DWFTK_STD_MAP_COMP(type1, type2, comp) std::map<type1, type2, comp > 
# define _DWFTK_STD_MAP(type1, type2) _DWFTK_STD_MAP_COMP(type1, type2, less<type1>)
# define _DWFTK_STD_MULTIMAP_COMP(type1, type2, comp) std::multimap<type1, type2, comp > 
# define _DWFTK_STD_MULTIMAP(type1, type2) _DWFTK_STD_MULTIMAP_COMP(type1, type2, less<type1>)
# define _DWFTK_STD_LIST(type) std::list<type>
#else
# define _DWFTK_STD_VECTOR(type) std::vector<type, _DWFTK_STL_ALLOCATOR<type> >
# define _DWFTK_STD_MAP_COMP(type1, type2, comp) std::map<type1, type2, comp, _DWFTK_STL_ALLOCATOR< std::pair<type1 const, type2> > >
# define _DWFTK_STD_MAP(type1, type2) _DWFTK_STD_MAP_COMP(type1, type2, less<type1>)
# define _DWFTK_STD_MULTIMAP_COMP(type1, type2, comp) std::multimap<type1, type2, comp, _DWFTK_STL_ALLOCATOR< std::pair<type1 const, type2> > > 
# define _DWFTK_STD_MULTIMAP(type1, type2) _DWFTK_STD_MULTIMAP_COMP(type1, type2, less<type1>)
# define _DWFTK_STD_LIST(type) std::list<type, _DWFTK_STL_ALLOCATOR<type> >
#endif

}


#endif

