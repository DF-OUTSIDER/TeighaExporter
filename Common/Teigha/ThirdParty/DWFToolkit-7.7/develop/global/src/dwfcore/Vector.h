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
//  $Header: //DWF/Development/Components/Internal/DWF Core/v1.7/develop/global/src/dwfcore/Vector.h#1 $
//  $DateTime: 2011/02/14 01:23:46 $
//  $Author: caos $
//  $Change: 197965 $
//  $Revision: #1 $
//

#ifndef _DWFCORE_SORTED_VECTOR_H
#define _DWFCORE_SORTED_VECTOR_H


///
///\file        dwfcore/Vector.h
///\brief       This header contains the declaration for the DWFOrderedVector and DWFSortedVector interface.
///

#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/Exception.h"
#include "dwfcore/Comparator.h"
#include "dwfcore/Iterator.h"

#include <algorithm>

namespace DWFCore
{

///
///\class       DWFVector       dwfcore/Vector.h  "dwfcore/Vector.h"
///\brief       A container to store objects in a contiguous block of memory
///             (as in STL vectors)
///\since       1.2
///
///             This abstract class provides the basic interface to DWFVectors.
///
///\param       T   The value type.
///\param       Pr  Predicate defining the order between to values.
///\param       Eq  The equality comparision between values.
///
template < class T, class Pr = tDWFCompareLess<T>, class Eq = tDWFCompareEqual<T> >
class DWFVector : virtual public DWFCoreMemory
{

public:

    ///
    ///\brief   This type defines a const iterator on DWFVector.
    ///
    typedef typename DWFCore::DWFVectorConstIterator<T>      ConstIterator;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFVector()
        throw()
        : _oVector()
    {;}

    ///
    ///         Constructor
    ///
    ///\param   oVector             An STL vector of with the same type of elements
    ///\throw   None
    ///
    DWFVector( const std::vector<T>& oVector )
        throw()
        : _oVector( oVector )
    {;}

    ///
    ///         Copy Constructor
    ///
    ///\param   oVector             The DWFVector to copy.
    ///\throw   None
    ///
    DWFVector( const DWFVector<T>& oVector )
        throw()
        : _oVector( oVector._oVector )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFVector()
        throw()
    {;}

    ///
    ///         Erase a value from the vector. All copies of the value will be removed.
    ///
    ///\param   oValue          The value to remove
    ///\return  If the value is found and removed this returns true.
    ///\throw   None
    ///
    bool erase( const T& oValue )
        throw()
    {
        _tSTLIterator iterNewEnd = std::remove( _oVector.begin(), _oVector.end(), oValue );

        if (iterNewEnd==_oVector.end())
        {
            return false;
        }
        else
        {
            _oVector.erase( iterNewEnd, _oVector.end() );
            return true;
        }
    }

    ///
    ///         Erase an element at the given index.
    ///
    ///\param   index          The index of the element to remove.
    ///\throw   A DWFUnexpectedException is thrown if the index is out of range.
    ///
    void eraseAt( size_t index )
        throw( DWFException )
    {
        if (index<size())
        {
            _oVector.erase( _oVector.begin()+index );
        }
        else
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Vector index is past the end of range" );
        }
    }

    ///
    ///         Erase elements at positions given by the range. iStart is included
    ///         and iStop is excluded. If iStop is greater than the size of the
    ///         vector, all elements upto the end will be removed.
    ///
    ///\param   iStart          Position of first element to remove.
    ///\param   iStop           1 past the position of the last element to remove.
    ///\throw   A DWFUnexpectedException is thrown if the index is out of range.
    ///
    void eraseAt( size_t iStart, size_t iStop )
        throw( DWFException )
    {
        if (iStart<size())
        {
            if (iStop<=size())
            {
                _oVector.erase( _oVector.begin()+iStart, _oVector.begin()+iStop );
            }
            else
            {
                _oVector.erase( _oVector.begin()+iStart, _oVector.end() );
            }
        }
        else
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Starting vector index is past the end of range" );
        }
    }

    ///
    ///         Clear out the vector. This will not change whether or not duplicates
    ///         are allowed.
    ///
    ///\throw   None
    ///
    void clear()
        throw()
    {
        _oVector.clear();
    }

    ///
    ///         Returns whether or not the vector is empty.
    ///
    ///\return  True if the vector is empty.
    ///\throw   None
    ///
    bool empty() const
    {
        return _oVector.empty();
    }

    ///
    ///         Returns the size of the vector.
    ///
    ///\return  The number of elements in the vector
    ///\throw   None
    ///
    size_t size() const
        throw()
    {
        return _oVector.size();
    }

    ///
    ///         Returns the number of copies of a given value in the vector.
    ///
    ///\param   oValue          The value whose count will be returned.
    ///\return  The number of copies of the value.
    ///\throw   None
    ///
    virtual size_t count( const T& oValue ) const
        throw() = 0;

    ///
    ///         Return a const reference to the first element in the vector.
    ///
    ///\return  A const reference to the first element in the vector
    ///\throw   DWFUnexpectedException is thrown if there are no elements in the vector.
    ///
    virtual const T& front() const
        throw( DWFException ) = 0;

    ///
    ///         Return a const reference to the last element in the vector.
    ///
    ///\return  A const reference to the last element in the vector
    ///\throw   DWFUnexpectedException is thrown if there are no elements in the vector.
    ///
    virtual const T& back() const
        throw( DWFException ) = 0;

    ///
    ///         Returns a const reference to the element at the given index in the vector
    ///
    ///\param   index               The index at which to get the element.
    ///\return  A const reference to the element at the index.
    ///\throw   DWFUnexpectedException is thrown is the index is past the end of the range.
    ///
    virtual const T& operator[]( size_t index ) const
        throw( DWFException ) = 0;

    ///
    ///         Search for the first occurence of a value. If it is found the index
    ///         is returned in iFirst. If the value is not found these
    ///         are undefined.
    ///
    ///\param   oValue          The value being searched.
    ///\param   iFirst          The first occurence is returned in the argument
    ///\return  True if the value is found
    ///\throw   None
    ///
    virtual bool findFirst( const T& oValue, size_t& iFirst ) const
        throw() = 0;

    ///
    ///         Test the equality of the vectors.
    ///
    ///\param   oVector             The sorted vector to compare against.
    ///\return  True if each element in the RHS vector is equal to the element at the 
    ///         corresponding location in the LHS vector.
    ///
    virtual bool operator==( const DWFVector<T, Pr, Eq>& oVector ) const
        throw()
    {
        return (_oVector==oVector._oVector);
    }

    ///
    ///         Returns a const iterator from the first element in 
    ///         the vector. The const iterator prevents modification
    ///         of the element values to preserve the sort order.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual ConstIterator* constIterator() const
        throw() = 0;

protected:

    typedef typename std::vector<T>::iterator            _tSTLIterator;
    typedef typename std::vector<T>::const_iterator      _tSTLConstIterator;

protected:

    // The STL vector in which all the elements are stored
    std::vector<T>      _oVector;

    Pr                  _tCompares;
    Eq                  _tEquals;

private:

    DWFVector& operator=( const DWFVector<T, Pr, Eq>& oVector );

};


///
///\class       DWFOrderedVector    dwfcore/Vector.h  "dwfcore/Vector.h"
///\brief       A container to store objects in a contiguous block of memory.
///             (as in STL vectors)
///\since       1.2
///
///             This class allows the storage of elements in a contiguous block in memory
///             while preserving the order in which they are added to the vector.
///
///\param       T   The value type.
///\param       Pr  Predicate defining the order between to values.
///\param       Eq  The equality comparision between values.
///
template < class T, class Pr = tDWFCompareLess<T>, class Eq = tDWFCompareEqual<T> >
class DWFOrderedVector : public DWFVector<T, Pr, Eq>
                       , virtual public DWFCoreMemory
{

public:

    ///
    ///\brief   This type defines an iterator on the vector. 
    ///
    typedef DWFVectorIterator<T>                Iterator;

    ///
    ///\brief   This type defines the enumeration of standard locations to 
    ///         insert new elements.
    ///
    typedef enum
    {
        ///
        ///     The default value depends on the implementation
        ///
        eDefault        = 0,

        ///
        ///     Insert elements at the front
        ///
        eFront          = 1,

        ///
        ///     Insert elements at the back
        ///
        eBack           = 2

    } teVectorInsertLocation;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFOrderedVector<T, Pr, Eq>()
        throw()
        : DWFVector<T, Pr, Eq>()
    {;}

    ///
    ///         Constructor
    ///
    ///         This constructor initializes the vector with the provided
    ///         size, filling it with copies of the provided value. 
    ///
    ///\param   nSize            The size of the vector.
    ///\param   tValue           The value that must be used to initialize the vector.
    ///\throw   None
    ///
    DWFOrderedVector<T, Pr, Eq>( size_t nSize, const T& tValue )
        throw()
        : DWFVector<T, Pr, Eq>( std::vector<T>( nSize, tValue ) )
    {;}

    ///
    ///         Constructor
    ///
    ///\param   oVector             An STL vector of with the same type of elements
    ///\throw   None
    ///
    DWFOrderedVector<T, Pr, Eq>( const std::vector<T>& oVector )
        throw()
        : DWFVector<T, Pr, Eq>( oVector )
    {;}

    ///
    ///         Copy Constructor
    ///
    ///\param   oVector             The DWFVector to copy.
    ///\throw   None
    ///
    DWFOrderedVector<T, Pr, Eq>( const DWFOrderedVector<T, Pr, Eq>& oVector )
        throw()
        : DWFVector<T, Pr, Eq>( oVector )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    ~DWFOrderedVector()
        throw()
    {;}

    ///
    ///         Assignment operator for copying
    ///
    ///\param   oVector             The DWFVector to copy.
    ///\throw   None
    ///
    virtual DWFOrderedVector<T, Pr, Eq>& operator=( const DWFOrderedVector<T, Pr, Eq>& oVector )
        throw()
    {
        if (this != &oVector)
        {
            this->_oVector = oVector._oVector;
        }

        return *this;
    }

    ///
    ///         Insert a value into the vector.
    ///
    ///\param   oValue              The value to insert.
    ///\param   eLoc                The location to insert at.
    ///\throw   None
    ///
    void insert( const T& oValue, 
                 teVectorInsertLocation eLoc = eDefault )
        throw()
    {
        switch (eLoc)
        {
            case eFront:
            {
                this->_oVector.insert( this->_oVector.begin(), oValue );
                break;
            }
            case eBack:
            case eDefault:
            default:
            {
                this->_oVector.push_back( oValue );
                break;
            }
        }
    }

    ///
    ///         Insert values from an STL vector into this vector.
    ///
    ///\param   oVector             The STL vector to copy.
    ///\param   eLoc                The location to insert at.
    ///\throw   None
    ///
    void insert( const std::vector<T>& oVector, 
                 teVectorInsertLocation eLoc = eDefault )
        throw()
    {
        switch (eLoc)
        {
            case eFront:
            {
                this->_oVector.insert( this->_oVector.begin(), oVector.begin(), oVector.end() );
                break;
            }
            case eBack:
            case eDefault:
            default:
            {
                this->_oVector.insert( this->_oVector.end(), oVector.begin(), oVector.end() );
                break;
            }
        }
    }

    ///
    ///         Insert copies of values from another ordered vector.
    ///
    ///\param   oOrderedVector              The value to insert.
    ///\param   eLoc                        The location to insert at.
    ///\throw   None
    ///
    void insert( const DWFOrderedVector<T>& oOrderedVector, 
                 teVectorInsertLocation eLoc = eDefault )
        throw()
    {
        const std::vector<T>& oVector = oOrderedVector._oVector;
        insert( oVector, eLoc );
    }

    ///
    ///         Insert a value at the given index. If the given index is greater
    ///         than the size of the vector, an exception is thrown. 
    ///
    ///\param   oValue              The value to insert.
    ///\param   index               The index of the location to insert it.
    ///\throw   A DWFUnexpectedException is thrown if the index is greater than the size.
    void insertAt( const T& oValue, size_t index )
        throw( DWFException )
    {
        if (index>this->size())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The insertion index is larger than the vector size" );
        }

        if (index==this->size())
        {
            push_back( oValue );
        }
        else
        {
            this->_oVector.insert( this->_oVector.begin()+index, oValue );
        }
    }

    ///
    ///         Append a value at the end of the vector. 
    ///
    ///\param   oValue              The value to append.
    ///\throw   None
    ///
    void push_back( const T& oValue )
        throw()
    {
        this->_oVector.push_back( oValue );
    }

    ///
    ///         Returns the number of copies of a given value in the vector.
    ///
    ///\param   oValue          The value whose count will be returned.
    ///\return  The number of copies of the value.
    ///\throw   None
    ///
    virtual size_t count( const T& oValue ) const
        throw()
    {
        int nCount=0;

        _tSTLConstIterator iter = this->_oVector.begin();
        for (; iter!=this->_oVector.end(); ++iter)
        {
            if (this->_tEquals(*iter, oValue))
            {
                ++nCount;
            }
        }

        return nCount;
    }


    ///
    ///         Search for the first occurence of a value. If it is found the index
    ///         is returned in iFirst. If the value is not found these
    ///         are undefined.
    ///
    ///\param   oValue          The value being searched.
    ///\param   iFirst          The first occurence is returned in the argument
    ///\return  True if the value is found
    ///\throw   None
    ///
    virtual bool findFirst( const T& oValue, size_t& iFirst ) const
        throw()
    {
        iFirst = 0;

        _tSTLConstIterator iter = this->_oVector.begin();
        for (; iter!=this->_oVector.end(); ++iter)
        {
            if (this->_tEquals( oValue, *iter ))
            {
                return true;
            }
            else
                ++iFirst;
        }
 
        // The value was not found
        return false;
    }

    ///
    ///         Find the instances of oValue in the vector. The method returns the number
    ///         such instances. The position of each instance is returned in the 
    ///         oIndices vector passed in as argument.
    ///
    ///\param   oValue          The value being searched.
    ///\param   oIndices        A vector to return the indices of each found instance.
    ///\return  The number of elements that are equal to oValue.
    ///\throw   None
    ///
    virtual size_t findAll( const T& oValue, DWFOrderedVector<unsigned int>& oIndices ) const
        throw()
    {
        oIndices.clear();

        _tSTLConstIterator it = this->_oVector.begin();
        unsigned int i = 0;
        for (; it != this->_oVector.end(); ++it, ++i)
        {
            if (this->_tEquals( oValue, *it ))
            {
                oIndices.push_back( i );
            }
        }

        return oIndices.size();
    }

    ///
    ///         Return a reference to the first element in the vector.
    ///
    ///\return  A reference to the first element in the vector
    ///\throw   DWFUnexpectedException is thrown if there are no elements in the vector.
    ///
    T& front()
        throw( DWFException )
    {
        if (this->empty())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The vector has no elements" );
        }

        return this->_oVector.front();
    }

    ///
    ///\copydoc DWFVector::front().
    ///
    virtual const T& front() const
        throw( DWFException )
    {
        if (this->empty())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The vector has no elements" );
        }

        return this->_oVector.front();
    }

    ///
    ///         Return a reference to the last element in the vector.
    ///
    ///\return  A reference to the last element in the vector
    ///\throw   DWFUnexpectedException is thrown if there are no elements in the vector.
    ///
    T& back()
        throw( DWFException )
    {
        if (this->empty())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The vector has no elements" );
        }

        return this->_oVector.back();
    }

    ///
    ///\copydoc DWFVector::back().
    ///
    virtual const T& back() const
        throw( DWFException )
    {
        if (this->empty())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The vector has no elements" );
        }

        return this->_oVector.back();
    }

    ///
    ///         Returns a reference to the element at the given index in the vector
    ///
    ///\param   index               The index at which to get the element.
    ///\return  A reference to the element at the index.
    ///\throw   DWFUnexpectedException is thrown if the index is past the end of the range.
    ///
    T& operator[]( size_t index )
        throw( DWFException )
    {
        if (index<this->size())
        {
            return this->_oVector[index];
        }
        else
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Vector index is past the end of range" );
        }
    }

    ///
    ///         Returns a const reference to the element at the given index in the vector
    ///
    ///\param   index               The index at which to get the element.
    ///\return  A const reference to the element at the index.
    ///\throw   DWFUnexpectedException is thrown is the index is past the end of the range.
    ///
    const T& operator[]( size_t index ) const
        throw( DWFException )
    {
        if (index<this->size())
        {
            return this->_oVector[index];
        }
        else
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Vector index is past the end of range" );
        }
    }

    ///
    ///         Return an iterator from the first element in the vector. 
    ///         The caller owns the iterator and is responsible for releasing it with
    ///         the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual typename DWFOrderedVector<T>::Iterator* iterator()
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( DWFVectorIterator<T>( this->_oVector ) );
    }

    ///
    ///\copydoc DWFVector::constIterator()
    ///
    virtual typename DWFOrderedVector<T>::ConstIterator* constIterator() const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( DWFVectorConstIterator<T>( this->_oVector ) );
    }

private:

    typedef typename std::vector<T>::iterator            _tSTLIterator;
    typedef typename std::vector<T>::const_iterator      _tSTLConstIterator;

};


///
///\class       DWFSortedVector     dwfcore/Vector.h  "dwfcore/Vector.h"
///\brief       A sorted container to store objects in a contiguous block of memory
///             (as in STL vectors)
///\since       1.2
///
///             This class implements the sorted vector container. Elements get sorted
///             as they are inserted, but all elements are stored in a contiguous block
///             of memory, just as in the STL vectors (internally we use an STL vector).
///             The sorted vector allows multiple copies of an element to be stored.
///
///\param       T   The value type.
///\param       Pr  Predicate defining the order between to values
///\param       Eq  The equality comparision between values
///
template < class T, class Pr = tDWFCompareLess<T>, class Eq = tDWFCompareEqual<T> >
class DWFSortedVector : public DWFVector<T, Pr, Eq>
                      , virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///
    ///\param   bAllowDuplicates    By default, the container can have multiple copies
    ///                             of the same object.
    ///\throw   None
    ///
    DWFSortedVector<T, Pr, Eq>( bool bAllowDuplicates = true )
        throw()
        : DWFVector<T, Pr, Eq>()
        , _bAllowDuplicates( bAllowDuplicates )
    {;}

    ///
    ///         Constructor
    ///
    ///\param   oVector             An STL vector of with the same type of elements
    ///\param   bAllowDuplicates    Specifies whether duplicates are to be allowed or not.
    ///\throw   None
    ///
    DWFSortedVector<T, Pr, Eq>( const std::vector<T>& oVector, bool bAllowDuplicates = true )
        throw()
        : DWFVector<T, Pr, Eq>( oVector )
        , _bAllowDuplicates( bAllowDuplicates )
    {
        std::sort( this->_oVector.begin(), this->_oVector.end(), this->_tCompares );
        
        // Remove duplicates if they are not allowed
        if (_bAllowDuplicates==false)
        {
            // Since the vector already sorted, we can use std::unique
            typename DWFVector<T>::_tSTLIterator iterNewEnd  = std::unique( 
                this->_oVector.begin(), 
                this->_oVector.end(), 
                this->_tEquals );
            this->_oVector.erase( iterNewEnd, this->_oVector.end() );
        }
    }

    ///
    ///         Copy Constructor
    ///
    ///\param   oVector       The sorted vector to copy.
    ///\throw   None
    ///
    DWFSortedVector<T, Pr, Eq>( const DWFSortedVector<T, Pr, Eq>& oVector )
        throw()
        : DWFVector<T, Pr, Eq>( oVector )
        , _bAllowDuplicates( oVector._bAllowDuplicates )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    ~DWFSortedVector()
        throw()
    {;}


    ///
    ///         Assignment operator for copying
    ///
    ///\param   oVector       The sorted vector to copy.
    ///\throw   None
    ///
    DWFSortedVector<T, Pr, Eq>& operator=( const DWFSortedVector<T, Pr, Eq>& oVector )
        throw()
    {
        if (this!=&oVector)
        {
            this->_oVector = oVector._oVector;
            _bAllowDuplicates = oVector._bAllowDuplicates;
        }

        return *this;
    }

    ///
    ///         Insert a value into the vector.
    ///
    ///\param   oValue              The value to insert.
    ///\throw   None
    ///
    void insert( const T& oValue )
        throw()
    {
        // Lower bound returns the position with a value that is greater (lesser depending on predicate)
        // than or equivalent to the given value.
        _tSTLIterator iter = std::lower_bound( this->_oVector.begin(), this->_oVector.end(), oValue, this->_tCompares );

        if (_bAllowDuplicates == true)
        {
            this->_oVector.insert( iter, oValue );
        }
        else
        {
            // Insert only if the value was not found 
            if ( iter==this->_oVector.end() || this->_tCompares( oValue, *iter) )
            {
                this->_oVector.insert( iter, oValue );
            }
            else
            {
                return;
            }
        }
    }

    ///
    ///         Insert values from an STL vector into this vector.
    ///
    ///\param   oVector             The STL vector to copy
    ///\throw   None
    ///
    void insert( const std::vector<T>& oVector )
        throw()
    {
        DWFSortedVector<T, Pr, Eq> v( oVector );
        insert( v );
    }

    ///
    ///         Insert values from another sorted vector into this vector.
    ///
    ///\param   oSortedVector       The sorted vector to copy
    ///\throw   None
    ///
    void insert( const DWFSortedVector<T, Pr, Eq>& oSortedVector )
        throw()
    {
        // Get a reference directly to the STL vector of the input DWFSortedVector
        const std::vector<T>& oVectorIn = oSortedVector._oVector;

        // Make a copy, since we will overwrite _oVector
        std::vector<T>  oOriginal = this->_oVector;

        // Resize _oVector to hold it original elements and the new ones
        this->_oVector.resize( this->_oVector.size() + oVectorIn.size() );

        std::merge( oOriginal.begin(), oOriginal.end(), 
                    oVectorIn.begin(), oVectorIn.end(),
                    this->_oVector.begin(), this->_tCompares );

        if (_bAllowDuplicates==false)
        {
            // Since the vector already sorted, we can use std::unique
            _tSTLIterator iterNewEnd  = std::unique( this->_oVector.begin(), this->_oVector.end(), this->_tEquals );
            this->_oVector.erase( iterNewEnd, this->_oVector.end() );
        }
    }

    ///
    ///         Returns the number of copies of a given value in the vector.
    ///
    ///\param   oValue          The value whose count will be returned.
    ///\return  The number of copies of the value.
    ///\throw   None
    ///
    virtual size_t count( const T& oValue ) const
        throw()
    {
        std::pair< _tSTLConstIterator, _tSTLConstIterator > iterResult;
        iterResult = std::equal_range( this->_oVector.begin(), this->_oVector.end(), oValue, this->_tCompares );

        return (iterResult.second - iterResult.first);
    }

    ///
    ///\copydoc DWFVector::front().
    ///
    virtual const T& front() const
        throw( DWFException )
    {
        if (this->empty())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The vector has no elements" );
        }

        return this->_oVector.front();
    }

    ///
    ///\copydoc DWFVector::back().
    ///
    virtual const T& back() const
        throw( DWFException )
    {
        if (this->empty())
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The vector has no elements" );
        }

        return this->_oVector.back();
    }

    ///
    ///         Returns a const reference to the element at the given index in the vector
    ///
    ///\param   index               The index at which to get the element.
    ///\return  A const reference to the element at the index.
    ///\throw   DWFUnexpectedException is thrown is the index is past the end of the range.
    ///
    virtual const T& operator[]( size_t index ) const
        throw( DWFException )
    {
        if (index<this->size())
        {
            return this->_oVector[index];
        }
        else
        {
            _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Vector index is past the end of range" );
        }
    }

    ///
    ///         Search for the first occurence of a value. If it is found the index
    ///         is returned in iFirst. If the value is not found these
    ///         are undefined.
    ///
    ///\param   oValue          The value being searched.
    ///\param   iFirst          The first occurence is returned in the argument
    ///\return  True if the value is found
    ///\throw   None
    ///
    virtual bool findFirst( const T& oValue, size_t& iFirst ) const
        throw()
    {
        // Using lower_bound is twice as fast as using equal_range, hence the rationale
        // for this method.
        _tSTLConstIterator iter = std::lower_bound( this->_oVector.begin(), this->_oVector.end(), oValue, this->_tCompares );

        if ( iter==this->_oVector.end() || this->_tCompares( oValue, *iter) )
        {
            // The value was not found
            return false;
        }
        else
        {
            iFirst = iter - this->_oVector.begin();
            return true;
        }
    }

    ///
    ///         Search for the first and last occurence of a given value. If the 
    ///         value is found the first and last indices for the value are returned
    ///         in iFirst and iLast respectively. If the value is not found these
    ///         are undefined.
    ///
    ///\param   oValue          The value being searched.
    ///\param   iFirst          The first occurence is returned in the argument
    ///\param   iLast           The last occurence is returned in the argument
    ///\return  True if the value is found
    ///\throw   None
    ///
    bool find( const T& oValue, size_t& iFirst, size_t& iLast ) const
        throw()
    {
        std::pair<_tSTLConstIterator, _tSTLConstIterator> iterResult;
        iterResult = std::equal_range( this->_oVector.begin(), this->_oVector.end(), oValue, this->_tCompares );
        
        size_t diff = iterResult.second - iterResult.first;
        if (diff > 0)
        {
            iFirst = iterResult.first - this->_oVector.begin();
            iLast  = iFirst + diff - 1;
            return true;
        }
        else
        {
            return false;
        }
    }

    ///
    ///         Change whether or not duplicates are allowed. If duplicates are being
    ///         disallowed with this method, existing duplicates will be removed.
    ///
    ///\param   bAllow          Set to true to allow duplicates (default).
    ///\throw   None
    ///
    void allowDuplicates( bool bAllow = true )
        throw()
    {
        if (bAllow==_bAllowDuplicates)
        {
            return;
        }

        if (bAllow==false)
        {
            _tSTLIterator iterNewEnd = std::unique( this->_oVector.begin(), this->_oVector.end(), this->_tEquals );
            this->_oVector.erase( iterNewEnd, this->_oVector.end() );
        }

        _bAllowDuplicates = bAllow;
    }

    ///
    ///         Returns whether or not duplicates are allowed.
    ///
    ///\return  True if duplicates are allowed.
    ///\throw   None
    ///
    bool duplicatesAllowed() const
        throw()
    {
        return _bAllowDuplicates;
    }

    ///
    ///\copydoc DWFVector::constIterator()
    ///
    virtual typename DWFSortedVector<T>::ConstIterator* constIterator() const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( DWFVectorConstIterator<T>( this->_oVector ) );
    }

private:

    bool                _bAllowDuplicates;


private:

    typedef typename std::vector<T>::iterator            _tSTLIterator;
    typedef typename std::vector<T>::const_iterator      _tSTLConstIterator;

};

}

#endif

