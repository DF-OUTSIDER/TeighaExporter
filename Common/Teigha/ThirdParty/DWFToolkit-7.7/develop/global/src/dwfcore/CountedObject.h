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


#ifndef _DWFCORE_COUNTED_OBJECT_H
#define _DWFCORE_COUNTED_OBJECT_H

///
///\file        dwfcore/CountedObject.h
///\brief       This header contains the declarations for reference counting templates and classes.
///

#include "dwfcore/Core.h"


namespace DWFCore
{

///
///\class       DWFCountedObject    dwfcore/CountedObject.h     "dwfcore/CountedObject.h"
///\brief       Base class for reference counted objects.
///\since       1.0.1
///
///             This class provides reference count tracking
///             and atomic modification operations.  It does
///             not manage the object itself; that is, the
///             object will not self-delete, via this base class,
///             when it's internal count reaches zero.
///
    class DWFCountedObject : virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///
    ///\param   nInitialCount   The starting reference count, default is zero.
    ///\throw   None
    ///
    DWFCountedObject( int nInitialCount = 0 )
        throw()
        : _nCount( nInitialCount )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFCountedObject()
        throw()
    {;}

    ///
    ///         Copy Constructor
    ///    
    ///\throw   None
    ///
    DWFCountedObject( const DWFCountedObject& rSrc )
        throw()
        : _nCount( rSrc._nCount )
    {;}

    ///
    ///         Assignment Operator
    ///
    ///\throw   None
    ///
    
    DWFCountedObject& operator=( const DWFCountedObject& rSrc )
        throw()
    {
        _nCount = rSrc._nCount;
        return *this;
    }

    ///
    ///         Returns the number of references on this object.
    ///         This call is not thread-safe nor otherwise
    ///         synchronized with \a increment() and \b decrement().
    ///
    ///\return  The current reference count.
    ///\throw   None
    ///
    
    int count() const
        throw()
    {
        return _nCount;
    }

    ///
    ///         Atomically increase the reference count by one.
    ///
    ///\throw   None
    ///
    
    virtual void increment() const
        throw()
    {
        DWFCore::AtomicIncrement( &_nCount );
    }

    ///
    ///         Atomically decrease the reference count by one.
    ///         No special behavior is implied when the count reaches
    ///         zero.  This object will not auto-delete.
    ///
    ///\throw   None
    ///
    
    virtual void decrement() const
        throw()
    {
        DWFCore::AtomicDecrement( &_nCount );
    }

    ///
    ///         Invoked to have the object delete itself.
    ///         This method may be overridden to alter the deletion
    ///         itself or perform pre/post deletion work.
    ///         This method uses the \b DWFCORE_FREE_OBJECT macro to delete
    ///         itself so objects of this type should be allocated with the
    ///         \b DWFCORE_ALLOC_OBJECT macro.  If another allocator is used,
    ///         this method should be implemented in the derived class using
    ///         a matching deallocator.
    ///
    ///\throw   None
    ///
    
    virtual void destroy()
    {
        // Note : (todo) the DWFCORE_FREE_OBJECT macro expands to "delete pointer; pointer = NULL;
        //        However, it is very important to note that "this" cannot be set to a value 
        //        i.e. the usage of the statement "this = NULL;" will result in a error.
        delete this;
        //DWFCORE_FREE_OBJECT( this );
    }

private:

    mutable int _nCount;
};


///
///\class       DWFCountedObjectPointer    dwfcore/CountedObject.h     "dwfcore/CountedObject.h"
///\brief       Smart pointer class for reference counted objects.
///\since       1.0.1
///
///             This class wraps a pointer to a referenced counted object.
///             It will perform some reference counting and if appropriate
///             delete the pointer.
///
class DWFCountedObjectPointer : virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///         This constructor will add a reference to the incoming object.
    ///     
    ///\param   pObject A pointer to the reference counted object to manage.
    ///                 The reference count of \a pObject will be incremented.
    ///                 The allocation of this object should match the deallocator
    ///                 used in \a DWFCountedObject::destroy(), the default is
    ///                 the \b DWFCORE_ALLOC_OBJECT macro.
    ///\throw           None
    ///
    
    DWFCountedObjectPointer( DWFCountedObject* pObject )
        throw()
        : _pObject( pObject )
    {
        if (_pObject)
        {
            _pObject->increment();
        }
    }

    ///
    ///         Destructor
    ///         Decrements the reference count of the managed object
    ///         and will delete it when no references remain.
    ///
    ///\throw   None
    ///
    
    virtual ~DWFCountedObjectPointer()
        throw()
    {
        _unbind();
    }

    ///
    ///         Copy Constructor
    ///         This constructor will add a reference to the managed object.
    ///
    ///\param   rSrc    The smart pointer object to copy from.  The managed pointer
    ///                 will be assigned to this object and the reference count will
    ///                 be incremented.
    ///\throw           None
    ///
    
    DWFCountedObjectPointer( const DWFCountedObjectPointer& rSrc )
        throw()
    {
        _pObject = rSrc._pObject;

        if (_pObject)
        {
            _pObject->increment();
        }
    }

    ///
    ///         Assignment Operator
    ///         This operator will add a reference to the managed object.
    ///
    ///\param   rSrc    The smart pointer object to copy from.  The managed pointer
    ///                 will be assigned to this object and the reference count will
    ///                 be incremented.
    ///\throw           None
    ///
    
    DWFCountedObjectPointer& operator=( const DWFCountedObjectPointer& rSrc )
        throw()
    {
        _unbind();

        _pObject = rSrc._pObject;

        if (_pObject)
        {
            _pObject->increment();
        }

        return *this;
    }

    ///
    ///         Cast Operator
    ///         Used to cast a smart pointer object to the managed DWFCountedObject pointer.
    ///     
    ///\return  The managed object pointer.
    ///\throw   None
    ///
    
    operator DWFCountedObject*() const
        throw()
    {
        return _pObject;
    }

    ///
    ///         Constant Cast Operator
    ///         Used to cast a smart pointer object to the managed DWFCountedObject pointer.
    ///     
    ///\return  The managed object pointer.
    ///\throw   None
    ///
    
    operator const DWFCountedObject*() const
        throw()
    {
        return (const DWFCountedObject*)_pObject;
    }

    ///
    ///         Void Cast Operator
    ///         Used to cast a smart pointer object to a void pointer to
    ///         the managed DWFCountedObject pointer.
    ///     
    ///\return  A void pointer to the managed object pointer.
    ///\throw   None
    ///
    
    operator void*() const
        throw()
    {
        return (void*)_pObject;
    }

    ///
    ///         Constant Void Cast Operator
    ///         Used to cast a smart pointer object to a void pointer to
    ///         the managed DWFCountedObject pointer.
    ///     
    ///\return  A void pointer to the managed object pointer.
    ///\throw   None
    ///
    
    operator const void*() const
        throw()
    {
        return (const void*)_pObject;
    }

    ///
    ///         Reference Cast Operator
    ///         Used to cast a smart pointer object to a reference to the managed DWFCountedObject pointer.
    ///     
    ///\return  The managed object reference.
    ///\throw   None
    ///
    
    operator DWFCountedObject&() const
        throw()
    {
        return *_pObject;
    }

    ///
    ///         Constant Reference Cast Operator
    ///         Used to cast a smart pointer object to a const reference to the managed DWFCountedObject pointer.
    ///     
    ///\return  The managed object reference.
    ///\throw   None
    ///
    
    operator const DWFCountedObject&() const
        throw()
    {
        return (const DWFCountedObject&)*_pObject;
    }

    ///
    ///         Used to determine if the managed object is NULL.
    ///         This method was called null() in version 1.0.1.
    ///
    ///\return  \e true if the managed object pointer NULL,
    ///         \e false otherwise.
    ///\throw   None
    ///
    
    bool isNull() const
        throw()
    {
        return (_pObject == NULL);
    }

    ///
    ///         Equality Operator
    ///         Used to determine if two managed object pointers are identical.
    ///         This method does not dereference or evaluate the two pointers,
    ///         only the memory addresses are compared.
    ///
    ///\param   rRHS    The smart pointer object to compare against.
    ///\return  \e true if the managed object pointers are the same,
    ///         \e false otherwise.
    ///\throw   None
    ///
    
    bool operator==( const DWFCountedObjectPointer& rRHS ) const
        throw()
    {
        return (_pObject == rRHS._pObject);
    }

    ///
    ///         Inequality Operator
    ///         Used to determine if two managed object pointers are not identical.
    ///         This method does not dereference or evaluate the two pointers,
    ///         only the memory addresses are compared.
    ///
    ///\param   rLHS    The first smart pointer object to compare against.
    ///\param   rRHS    The second smart pointer object to compare against.
    ///\return  \e true if the managed object pointers are different,
    ///         \e false otherwise.
    ///\throw   None
    ///
    
    friend bool operator!=( const DWFCountedObjectPointer& rLHS,
                            const DWFCountedObjectPointer& rRHS)
        throw()
    {
        return (rLHS._pObject != rRHS._pObject);
    }

private:

    void _unbind()
        throw()
    {
        if (_pObject)
        {
            _pObject->decrement();
            if (_pObject->count() == 0)
            {
                _pObject->destroy();
                _pObject = NULL;
            }
        }
    }

private:

    DWFCountedObject* _pObject;
};

///
///\class       DWFParameterizedCountedObjectPointer    dwfcore/CountedObject.h     "dwfcore/CountedObject.h"
///\brief       Smart pointer template for objects with a reference counted base class.
///\since       1.0.1
///
///             This class wraps a pointer to a referenced counted object.
///             It will perform some reference counting and if appropriate
///             delete the pointer.
///\param       T   Must specialize DWFCountedObject.
///
template<class T>
class DWFParameterizedCountedObjectPointer : public DWFCountedObjectPointer
                                           , virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///         This constructor will add a reference to the incoming object.
    ///     
    ///\param   pObject A pointer to the reference counted object to manage.
    ///                 The reference count of \a pObject will be incremented.
    ///                 The allocation of this object should match the deallocator
    ///                 used in \a DWFCountedObject::destroy(), the default is
    ///                 the \b DWFCORE_ALLOC_OBJECT macro.
    ///\throw           None
    ///
    DWFParameterizedCountedObjectPointer( T* pObject )
        throw()
        : DWFCountedObjectPointer( pObject )
    {;}

    ///
    ///         Destructor
    ///         Decrements the reference count of the managed object
    ///         and will delete it when no references remain.
    ///
    ///\throw   None
    ///
    virtual ~DWFParameterizedCountedObjectPointer()
        throw()
    {;}

    ///
    ///         Copy Constructor
    ///         This constructor will add a reference to the managed object.
    ///
    ///\param   rSrc    The smart pointer object to copy from.  The managed pointer
    ///                 will be assigned to this object and the reference count will
    ///                 be incremented.
    ///\throw           None
    ///
    DWFParameterizedCountedObjectPointer( const DWFParameterizedCountedObjectPointer& rSrc )
        throw()
        : DWFCountedObjectPointer( rSrc )
    {
        ;
    }

    ///
    ///         Assignment Operator
    ///         This operator will add a reference to the managed object.
    ///
    ///\param   rSrc    The smart pointer object to copy from.  The managed pointer
    ///                 will be assigned to this object and the reference count will
    ///                 be incremented.
    ///\throw           None
    ///
    DWFParameterizedCountedObjectPointer& operator=( const DWFParameterizedCountedObjectPointer& rSrc )
        throw()
    {
        (DWFCountedObjectPointer&)*this = (const DWFCountedObjectPointer&)rSrc;
        return *this;
    }

    ///
    ///         Cast Operator
    ///         Used to cast a smart pointer object to the type \e T managed pointer.
    ///     
    ///\return  The managed object pointer.
    ///\throw   None
    ///
    operator T*() const
        throw()
    {
        return (T*)(DWFCountedObjectPointer::operator DWFCountedObject*());
    }

    ///
    ///         Constant Cast Operator
    ///         Used to cast a smart pointer object to the type \e T managed pointer.
    ///     
    ///\return  The managed object pointer.
    ///\throw   None
    ///
    operator const T*() const
        throw()
    {
        return (const T*)(DWFCountedObjectPointer::operator const DWFCountedObject*());
    }

    ///
    ///         Reference Cast Operator
    ///         Used to cast a smart pointer object to the reference type \e T managed pointer.
    ///     
    ///\return  The managed object reference.
    ///\throw   None
    ///
    operator T&() const
        throw()
    {
        return (T&)*(DWFCountedObjectPointer::operator DWFCountedObject*());
    }

    ///
    ///         Constant Reference Cast Operator
    ///         Used to cast a smart pointer object to the reference type \e T managed pointer.
    ///     
    ///\return  The managed object reference.
    ///\throw   None
    ///
    operator const T&() const
        throw()
    {
        return (const T&)*(DWFCountedObjectPointer::operator const DWFCountedObject*());
    }

    ///
    ///         Member Operator
    ///         Used to cast a smart pointer object to the type \e T managed pointer.
    ///     
    ///\return  The managed object pointer.
    ///\throw   None
    ///
    T* operator->() const
        throw()
    {
        return (T*)(DWFCountedObjectPointer::operator DWFCountedObject*());
    }


    ///
    ///         Equality Operator
    ///         Used to determine if two managed object pointers are identical.
    ///         This method does not dereference or evaluate the two pointers,
    ///         only the memory addresses are compared.
    ///
    ///\param   rRHS    The smart pointer object to compare against.
    ///\return  \e true if the managed object pointers are the same,
    ///         \e false otherwise.
    ///\throw   None
    ///
    bool operator==( const DWFParameterizedCountedObjectPointer& rRHS ) const
        throw()
    {
        return ((DWFCountedObjectPointer&)*this == (DWFCountedObjectPointer&)rRHS);
    }

    ///
    ///         Inequality Operator
    ///         Used to determine if two managed object pointers are not identical.
    ///         This method does not dereference or evaluate the two pointers,
    ///         only the memory addresses are compared.
    ///
    ///\param   rLHS    The first smart pointer object to compare against.
    ///\param   rRHS    The second smart pointer object to compare against.
    ///\return  \e true if the managed object pointers are different,
    ///         \e false otherwise.
    ///\throw   None
    ///
    friend bool operator!=( const DWFParameterizedCountedObjectPointer& rLHS,
                            const DWFParameterizedCountedObjectPointer& rRHS)
        throw()
    {
        return ((DWFCountedObjectPointer&)rLHS != (DWFCountedObjectPointer&)rRHS);
    }
};

}

#endif
