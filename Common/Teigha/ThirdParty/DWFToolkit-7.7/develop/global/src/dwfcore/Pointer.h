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


#ifndef _DWFCORE_POINTER_H
#define _DWFCORE_POINTER_H

///
///\file        dwfcore/Pointer.h
///\brief       This header contains the implementations for the hashing functions
///             used by the library.
///

#include "dwfcore/Core.h"


namespace DWFCore
{

///
///\class       DWFPointer    dwfcore/Pointer.h    "dwfcore/Pointer.h"
///\brief       Auto-pointer template.
///\since       1.0.1
///
///             This pointer template automatically deletes the object or
///             memory block associated with it.
///
///\param       T   The object or memory allocation type.
///
template<class T>
class DWFPointer : virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///
    ///         Attention should be paid to the \a bArray parameter that controls the
    ///         deallocator.  Use the following as an example:
    ///         \code
    ///         DWFPointer<DWFString> apString( DWFCORE_ALLOC_OBJECT(DWFString), false );
    ///
    ///         DWFPointer<int> apValues( DWFCORE_ALLOC_MEMORY(int, 16), true );
    ///         \endcode
    ///
    ///\param   pT      A pointer to the object or memory to scope.
    ///\param   bArray  \e true if  \a pT points to a memory block the \b DWFCORE_FREE_MEMORY macro
    ///                 will be used to release it, the \b DWFCORE_FREE_OBJECT macro
    ///                 will be used otherwise.
    ///\throw   None
    ///
    DWFPointer( T* pT, bool bArray )
        : _pT( pT )
        , _bArray( bArray )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFPointer()
    {
        if (_pT)
        {
            if (_bArray)
            {
                DWFCORE_FREE_MEMORY( _pT );
            }
            else
            {
                DWFCORE_FREE_OBJECT( _pT );
            }
        }
    }

    ///
    ///         Assignment Operator
    ///
    ///\param   pT  A pointer to scope.
    ///\throw   None
    ///
    DWFPointer&
    operator=( T* pT )
        throw()
    {
        _pT = pT;
        return *this;
    }

    ///
    ///         Cast Operator
    ///
    ///\return  The scoped pointer.
    ///\throw   None
    ///
    operator T*() const
        throw()
    {
        return _pT;
    }

    ///
    ///         Constant Cast Operator
    ///
    ///\return  The scoped pointer.
    ///\throw   None
    ///
    operator const T*() const
        throw()
    {
        return (const T*)_pT;
    }

    ///
    ///         Void Cast Operator
    ///     
    ///\return  A void pointer to the scoped pointer.
    ///\throw   None
    ///
    operator void*() const
        throw()
    {
        return (void*)_pT;
    }

    ///
    ///         Constant Void Cast Operator
    ///     
    ///\return  A void pointer to the scoped pointer.
    ///\throw   None
    ///
    operator const void*() const
        throw()
    {
        return (const void*)_pT;
    }

    ///
    ///         Reference Cast Operator
    ///     
    ///\return  The scoped reference.
    ///\throw   None
    ///
    operator T&() const
    {
        return *_pT;
    }

    ///
    ///         Constant Reference Cast Operator
    ///     
    ///\return  The scoped reference.
    ///\throw   None
    ///
    operator const T&() const
    {
        return (const T&)*_pT;
    }

    ///
    ///         Used to determine if the managed object is NULL.
    ///         This method was called null() in version 1.0.1.
    ///
    ///\return  \e true if the scoped pointer NULL,
    ///         \e false otherwise.
    ///\throw   None
    ///
    bool isNull() const
        throw()
    {
        return (_pT == NULL);
    }

    ///
    ///         Member Operator
    ///     
    ///\return  The scoped pointer.
    ///\throw   None
    ///
    T* operator->() const
    {
        return _pT;
    }

private:

    T*      _pT;
    bool    _bArray;

private:

    //
    // not implemented
    //

    DWFPointer( const DWFPointer& );
    const DWFPointer& operator=( const DWFPointer& );
};

}

#endif
