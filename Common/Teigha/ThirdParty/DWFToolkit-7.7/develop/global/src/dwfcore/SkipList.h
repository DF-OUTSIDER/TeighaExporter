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

#ifndef _DWFCORE_SKIPLIST_H
#define _DWFCORE_SKIPLIST_H


///
///\file        dwfcore/SkipList.h
///\brief       This header contains the declaration for the skip list collection templates
///             and implementations.
///


#include "dwfcore/STL.h"
#include "dwfcore/Hash.h"
#include "dwfcore/Timer.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Comparator.h"


    

#ifndef DWFCORE_SKIPLIST_PROBABILITY_DISTRIB
///
///\ingroup     ExternalMacros
///\brief       Defines the probability value that each successively 
///             higher level of a node will be chosen.
///
///             The probability value that each successively higher level
///             of a skip list node will be chosen.   Values around 0.5 
///             yield best search performance and smaller values yield
///             lower memory usage (less forward references)
///\todo        This value can be a construction time parameter.
///
#define DWFCORE_SKIPLIST_PROBABILITY_DISTRIB    0.5f
#endif

#ifndef DWFCORE_SKIPLIST_MAX_NODE_LEVEL
///
///\ingroup     ExternalMacros
///\brief       Defines the maximum number of elements in the list.
///
///             The maximum number of elements is 2**(DWFCORE_SKIPLIST_MAX_NODE_LEVEL + 1)
///\todo        This value can be a construction time parameter.
///
#define DWFCORE_SKIPLIST_MAX_NODE_LEVEL         31
#endif

#ifndef DWFCORE_SKIPLIST_INITIAL_HEIGHT
///
///\ingroup     ExternalMacros
///\brief       Defines the initial node level.
///
///             This value can be increased for those lists that are
///             highly likely to contain numerous elements.
///\todo        This value can be a construction time parameter.
///
#define DWFCORE_SKIPLIST_INITIAL_HEIGHT        5
#endif


namespace DWFCore
{


///
///\class       DWFSkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       Skip list collection template.
///\since       1.0.1
///
///             Implementation of a generic Skip List.  This is a collection that maintains sort order using a highly efficient 
///             probabalistic distribution algorithm over a linked list using "skip links".   The performance of this collection
///             is excellent across search, insert, delete and linear-lookup.   
///             The algorithm was developed by William Pugh (University of Maryland, College Park).  
///             An explanation of the algorithm is available at: ftp://ftp.cs.umd.edu/pub/skipLists/skiplists.pdf
///
///             This class may be used in place of the STL \e map collection though some profiling 
///             is suggested to determine which performs better for given data types and application needs.
///
///\param       K   The key type.
///\param       V   The value type.
///\param       E   Defines equality between two keys.
///\param       L   Defines ordering between two keys.
///\param       Z   Defines the empty or \e zero-value for the key type.
///
template < class K, class V, class E = tDWFCompareEqual<K>, class L = tDWFCompareLess<K>, class Z = tDWFDefinedEmpty<K> >
class DWFSkipList : virtual public DWFCoreMemory
{

public:

    ///
    ///\brief       Typedef to get the key type of the skip list.
    ///
    typedef K       KeyType;
    ///
    ///\brief       Typedef to get the value type of the skip list.
    ///
    typedef V       ValueType;

public:

    ///
    ///         Constructor
    ///
    ///\throw   DWFMemoryException
    ///
    DWFSkipList()
        throw( DWFMemoryException )
        : _pHeader( NULL )
        , _nMaxHeight( DWFCORE_SKIPLIST_INITIAL_HEIGHT )
        , _nMaxLevel( 0 )
        , _nCount( 0 )
    {
        _pHeader = DWFCORE_ALLOC_OBJECT( _Node(DWFCORE_SKIPLIST_MAX_NODE_LEVEL) );
        if (_pHeader == NULL)
        {
            _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate header node" );
        }
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFSkipList()
        throw()
    {
        _Node* pNext = NULL;
        typename _Node::_Iterator iNode( _pHeader->forward(0) );

        while (iNode.valid())
        {
            pNext = iNode.get();
            iNode.next();

            DWFCORE_FREE_OBJECT( pNext );
        }

        DWFCORE_FREE_OBJECT( _pHeader );
    }

    //
    // Inner class definitions
    //
private:

    class _Node : virtual public DWFCoreMemory
    {

    public:

        class _Iterator : public DWFIterator<_Node*>
                        , virtual public DWFCoreMemory
        {
        
        public:

            _Iterator( _Node* pFirst )
                throw()
                : _pFirst( pFirst )
                , _pNext( pFirst )
            {;}

            virtual ~_Iterator()
                throw()
            {;}

            void reset()
                throw()
            {
                _pNext = _pFirst;
            }

            bool valid()
                throw()
            {
                return (_pNext != NULL);
            }

            bool next()
                throw()
            {
                _pNext = _pNext->forward( 0 );
                
                return valid();
            }
            
            _Node*& get()
                throw()
            {
                return _pNext;
            }

        private:

            _Node* _pFirst;
            _Node* _pNext;
        };

        class _ConstIterator : public DWFIterator<_Node const*>
                             , virtual public DWFCoreMemory
        {
        
        public:

            _ConstIterator( _Node const* pFirst )
                throw()
                : _pFirst( pFirst )
                , _pNext( pFirst )
            {;}

            virtual ~_ConstIterator()
                throw()
            {;}

            void reset()
                throw()
            {
                _pNext = _pFirst;
            }

            bool valid()
                throw()
            {
                return (_pNext != NULL);
            }

            bool next()
                throw()
            {
                _pNext = _pNext->forward( 0 );
                
                return valid();
            }
            
            _Node const*& get()
                throw( DWFException )
            {
                return _pNext;
            }

        private:

            _Node const* _pFirst;
            _Node const* _pNext;
        };

    public:

        _Node( int nHeight )
            throw( DWFMemoryException )
            : _ppForward( NULL )
        {
            _ppForward = DWFCORE_ALLOC_MEMORY( _Node*, (nHeight + 1) );

            if (_ppForward == NULL)
            {
                _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate array" );
            }

            DWFCORE_ZERO_MEMORY( _ppForward, sizeof(_Node*) * (nHeight + 1) );

            Z fNull;
            _Key = fNull();
        }

        _Node( int      nHeight,
               const K& rKey,
               const V& rValue )
            throw( DWFMemoryException )
            : _ppForward( NULL )
            , _Key( rKey )
            , _Value( rValue )
        {
            _ppForward = DWFCORE_ALLOC_MEMORY( _Node*, (nHeight + 1) );

            if (_ppForward == NULL)
            {
                _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate array" );
            }

            DWFCORE_ZERO_MEMORY( _ppForward, sizeof(_Node*) * (nHeight + 1) );
        }

        ~_Node()
            throw()
        {
            if (_ppForward)
            {
                DWFCORE_FREE_MEMORY( _ppForward );
            }
        }

        _Node* forward( uint16_t iLevel )
            throw()
        {
            return (_ppForward ? _ppForward[iLevel] : NULL);
        }

        _Node const* forward( uint16_t iLevel ) const
            throw()
        {
            return (_ppForward ? _ppForward[iLevel] : NULL);
        }

        void forward( uint16_t iLevel, _Node* pNode )
            throw()
        {
            _ppForward[iLevel] = pNode;
        }

        K& key()
            throw()
        {
            return _Key;
        }

        const K& key() const
            throw()
        {
            return _Key;
        }

        void key( const K& rKey )
            throw()
        {
            _Key = rKey;
        }

        V& value()
            throw()
        {
            return _Value;
        }

        const V& value() const
            throw()
        {
            return _Value;
        }

        void value( const V& rValue )
            throw()
        {
            _Value = rValue;
        }

    private:

        _Node** _ppForward;
        K       _Key;
        V       _Value;
    };

public:

    ///
    ///\class       Iterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of a key-value iterator for skip lists.
    ///\since       1.0.1
    ///
    class Iterator : virtual public DWFCoreMemory
                   , public DWFKVIterator<K,V>
    {
    
    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        Iterator()
            throw()
            : _pNodeIterator( NULL )
            , _pCurrent( NULL )
        {;}      

        ///
        ///         Copy Constructor
        ///
        ///\throw   None
        ///
        Iterator( typename _Node::_Iterator* pNodeIterator )
            throw()
            : _pNodeIterator( pNodeIterator )
            , _pCurrent( NULL )
        {;}      

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Iterator()
            throw()
        {
            if (_pNodeIterator)
            {
                DWFCORE_FREE_OBJECT( _pNodeIterator );
            }
        }

        virtual void reset()
            throw()
        {
            _pCurrent = NULL;

            if (_pNodeIterator)
            {
                _pNodeIterator->reset();
            }
        }

        virtual bool valid()
            throw()
        {
            return (_pNodeIterator ? _pNodeIterator->valid() : false);
        }

        virtual bool next()
            throw()
        {
            _pCurrent = NULL;

            if (_pNodeIterator)
            {
                return _pNodeIterator->next();
            }
            else
            {
                return false;
            }
        }
        
        virtual K& key()
            throw( DWFException )
        {
            if (_pCurrent == NULL)
            {
                if (_pNodeIterator)
                {
                    _pCurrent = _pNodeIterator->get();
                }
            }
            
            if (_pCurrent)
            {
                return _pCurrent->key();
            }
            else
            {
                _DWFCORE_THROW( DWFIllegalStateException, /*NOXLATE*/L"Cannot get key from iterator" );
            }
        }

        virtual V& value()
            throw( DWFException )
        {
            if (_pCurrent == NULL)
            {
                if (_pNodeIterator)
                {
                    _pCurrent = _pNodeIterator->get();
                }
            }
            
            if (_pCurrent)
            {
                return _pCurrent->value();
            }
            else
            {
                _DWFCORE_THROW( DWFIllegalStateException, /*NOXLATE*/L"Cannot get value from iterator" );
            }
        }

    private:

        typename _Node::_Iterator*  _pNodeIterator;
        _Node*                      _pCurrent;
    };

    ///
    ///\class       ConstIterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of a key-value const iterator for skip lists.
    ///\since       1.2
    ///
    class ConstIterator : virtual public DWFCoreMemory
                        , public DWFKVConstIterator<K,V>
    {
    
    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        ConstIterator()
            throw()
            : _pNodeIterator( NULL )
            , _pCurrent( NULL )
        {;}      

        ///
        ///         Copy Constructor
        ///
        ///\throw   None
        ///
        ConstIterator( typename _Node::_ConstIterator* pNodeIterator )
            throw()
            : _pNodeIterator( pNodeIterator )
            , _pCurrent( NULL )
        {;}      

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ConstIterator()
            throw()
        {
            if (_pNodeIterator)
            {
                DWFCORE_FREE_OBJECT( _pNodeIterator );
            }
        }

        virtual void reset()
            throw()
        {
            _pCurrent = NULL;

            if (_pNodeIterator)
            {
                _pNodeIterator->reset();
            }
        }

        virtual bool valid()
            throw()
        {
            return (_pNodeIterator ? _pNodeIterator->valid() : false);
        }

        virtual bool next()
            throw()
        {
            _pCurrent = NULL;

            if (_pNodeIterator)
            {
                return _pNodeIterator->next();
            }
            else
            {
                return false;
            }
        }
        
        virtual const K& key()
            throw( DWFException )
        {
            if (_pCurrent == NULL)
            {
                if (_pNodeIterator)
                {
                    _pCurrent = _pNodeIterator->get();
                }
            }
            
            if (_pCurrent)
            {
                return _pCurrent->key();
            }
            else
            {
                _DWFCORE_THROW( DWFIllegalStateException, /*NOXLATE*/L"Cannot get key from iterator" );
            }
        }

        virtual const V& value()
            throw( DWFException )
        {
            if (_pCurrent == NULL)
            {
                if (_pNodeIterator)
                {
                    _pCurrent = _pNodeIterator->get();
                }
            }
            
            if (_pCurrent)
            {
                return _pCurrent->value();
            }
            else
            {
                _DWFCORE_THROW( DWFIllegalStateException, /*NOXLATE*/L"Cannot get value from iterator" );
            }
        }

    private:

        typename _Node::_ConstIterator*  _pNodeIterator;
        _Node const*                     _pCurrent;
    };

public:

    ///
    ///         Empties the list and restores the initial state.
    ///
    ///\throw   DWFMemoryException
    ///
    virtual void clear()
        throw( DWFMemoryException )
    {
        _Node* pNext = NULL;
        typename _Node::_Iterator iNode( _pHeader->forward(0) );

        while (iNode.valid())
        {
            pNext = iNode.get();
            iNode.next();

            DWFCORE_FREE_OBJECT( pNext );
        }

        DWFCORE_FREE_OBJECT( _pHeader );

        //
        // Reset the maximum level and the initial maximum height.
        //
        _nMaxLevel  = 0;
        _nMaxHeight = DWFCORE_SKIPLIST_INITIAL_HEIGHT;
        
        //
        // Reset the element count to 0.
        //
        _nCount = 0;

        //
        // rebuild the header node
        //
        _pHeader = DWFCORE_ALLOC_OBJECT( _Node(DWFCORE_SKIPLIST_MAX_NODE_LEVEL) );
        if (_pHeader == NULL)
        {
            _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate header node" );
        }
    }

    ///
    ///         Returns the number of elements stored in the list.
    ///
    ///\return  The number of list elements.
    ///\throw   None
    ///
    virtual size_t size() const
        throw()
    {
        return _nCount;
    }

    ///
    ///         Returns an iterator from the first element in the list.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual Iterator* iterator()
        throw()
    {
        return DWFCORE_ALLOC_OBJECT(Iterator(DWFCORE_ALLOC_OBJECT(typename _Node::_Iterator(_pHeader->forward(0)))));
    }

    ///
    ///         Returns a const iterator from the first element in the list.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///\since   1.2
    ///
    virtual ConstIterator* constIterator() const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT(ConstIterator(DWFCORE_ALLOC_OBJECT(typename _Node::_ConstIterator(_pHeader->forward(0)))));
    }

    ///
    ///         Returns an iterator from the keyed element in the list.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rKey    The key of the first list element from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual Iterator* iterator( const K& rKey )
        throw()
    {
        return DWFCORE_ALLOC_OBJECT(Iterator(DWFCORE_ALLOC_OBJECT(typename _Node::_Iterator(_search(rKey)))));
    }

    ///
    ///         Returns a const iterator from the keyed element in the list.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rKey    The key of the first list element from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///\since   1.2
    ///
    virtual ConstIterator* constIterator( const K& rKey ) const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT(ConstIterator(DWFCORE_ALLOC_OBJECT(typename _Node::_ConstIterator(_search(rKey)))));
    }

    ///
    ///         Search the list with a given key and return the associated value if it exists.
    ///
    ///\param   rKey    The key for which to search the list.
    ///\return  A pointer to the value associated with the key or NULL if the key was not found.
    ///\throw   None
    ///
    virtual V* find( const K& rKey ) const
        throw()
    {    
        _Node* pSearch = _search( rKey );
          
            //
            // If the node was found then return the value. 
            //
        if (pSearch)
        {
            return &(pSearch->value());
        }
        else
        {
            return NULL;
        }
    }

    ///
    ///         Return the element key at a given position if it exists.
    ///
    ///\param   iPos    The position at which the element resides.
    ///\return  A pointer to the key or NULL if no elements was found at \a iPos.
    ///\throw   DWFException
    ///
    virtual K* key( uint64_t iPos )
        throw( DWFException )
    {
        if (iPos >= _nCount)
        {
            _DWFCORE_THROW( DWFOverflowException, /*NOXLATE*/L"Index exceeds list size" );
        }

        _Node* pNode = _pHeader->forward( 0 );

        for (; iPos > 0; iPos--)
        {
            pNode = pNode->forward( 0 );
        }

        return &(pNode->key());
    }

    ///
    ///         Return the element value at a given position if it exists.
    ///
    ///\param   iPos    The position at which the element resides.
    ///\return  A pointer to the value or NULL if no elements was found at \a iPos.
    ///\throw   DWFException
    ///
    virtual V* value( uint64_t iPos )
        throw( DWFException )
    {
        if (iPos >= _nCount)
        {
            _DWFCORE_THROW( DWFOverflowException, /*NOXLATE*/L"Index exceeds list size" );
        }

        _Node* pNode = _pHeader->forward( 0 );

        for (; iPos > 0; iPos--)
        {
            pNode = pNode->forward( 0 );
        }

        return &(pNode->value());
    }

    ///
    ///         Removes an element from the list.
    ///
    ///\param   rKey    The key of the element to erase.
    ///\return  \e true if keyed element was erased, \e false if the element was not found.
    ///\throw   None
    ///
    virtual bool erase( const K& rKey )
        throw()
    {
        //
        // refresh _ppUpdate
        //
        DWFCORE_ZERO_MEMORY( _ppUpdate, sizeof(_Node*) * (DWFCORE_SKIPLIST_MAX_NODE_LEVEL + 1) );

        _Node*  pCurrentNode = _pHeader;
        _Node*  pAlreadyChecked = NULL;
            
        //
        // Locate key of value immediately to the left of the key in rank.  During
        // the loop skip first using the largest steps possible and then proceed
        // with successively smaller steps (levels).   At each level maintain the
        // last visited node of that level (used for updating later).
        //
        int16_t iLevel = _nMaxLevel;
        for (; iLevel >= 0; iLevel--)
        {
            while ((pCurrentNode->forward(iLevel) != NULL) && 
                   (pCurrentNode->forward(iLevel) != pAlreadyChecked) &&
                   (_tLessThan(pCurrentNode->forward(iLevel)->key(), rKey)))
            {
                pCurrentNode = pCurrentNode->forward( iLevel );
            }
                
            pAlreadyChecked = pCurrentNode->forward( iLevel );
            _ppUpdate[iLevel] = pCurrentNode;
        }
        
        //
        // Get the next node.
        //
        pCurrentNode = pCurrentNode->forward( 0 );
          

            //
            // If the current key is equal to that supplied then just update the value
            // and we are done.
            //
        if ((pCurrentNode != NULL) && _tEquals(pCurrentNode->key(), rKey))
        {
                //
                // Iterate through all previously last-visited nodes of each level and update
                // their forward references to be the same of the deleted nodes level-equivalent
                // forward references.
                //
            for (iLevel = 0; iLevel <= _nMaxLevel; iLevel++)
            {
                if (_ppUpdate[iLevel]->forward(iLevel) != pCurrentNode)
                {
                    break;
                }
                    
                _ppUpdate[iLevel]->forward( iLevel, pCurrentNode->forward(iLevel) );
            }

                //
                // Readjust the maximum level in the skip-list if necessary by iterating through the
                // header's forward references and checking for null references at the highest levels back.
                //
            while((_nMaxLevel>0) && (_pHeader->forward(_nMaxLevel) == NULL))
            {
                _nMaxLevel--;
            }
        
            _nCount--;
        
            DWFCORE_FREE_OBJECT( pCurrentNode );

            return true;
        } 
        else
        {
            return false;
        }
    }

    ///
    ///         Adds an element to the list.
    ///
    ///\param   rKey        The key of the element to add.
    ///\param   rValue      The value of the element to add.
    ///\param   bReplace    If the new element uses a key that already exists, 
    ///                     this flag determines whether or not the new element
    ///                     will be inserted and replace the previous element or ignored.
    ///\return  \e false the the element key is a duplicate, \e true otherwise.
    ///\throw   DWFException
    ///
    virtual bool insert( const K& rKey, const V& rValue, bool bReplace = true )
        throw( DWFException )
    {
        //
        // refresh _ppUpdate
        //
        DWFCORE_ZERO_MEMORY( _ppUpdate, sizeof(_Node*) * (DWFCORE_SKIPLIST_MAX_NODE_LEVEL + 1) );

        _Node*  pCurrentNode = _pHeader;
        _Node*  pAlreadyChecked = NULL;
            
        //
        // Locate key of value immediately to the left of the key in rank.  During
        // the loop skip first using the largest steps possible and then proceed
        // with successively smaller steps (levels).   At each level maintain the
        // last visited node of that level (used for updating later).
        //
        int16_t iLevel = _nMaxLevel;
        for (; iLevel >= 0; iLevel--)
        {
            while ((pCurrentNode->forward(iLevel) != NULL) && 
                   (pCurrentNode->forward(iLevel) != pAlreadyChecked) &&
                   (_tLessThan(pCurrentNode->forward(iLevel)->key(), rKey)))
            {
                pCurrentNode = pCurrentNode->forward( iLevel );
            }
                
            pAlreadyChecked = pCurrentNode->forward( iLevel );
            _ppUpdate[iLevel] = pCurrentNode;
        }
            
        //
        // Get the next node.
        //
        pCurrentNode = pCurrentNode->forward( 0 );
          
            //
            // If the current key is equal to that supplied then just update the value
            // and we are done.
            //
        if ((pCurrentNode != NULL) && (_tEquals(pCurrentNode->key(), rKey)))
        {                  
            if (bReplace)
            {
                pCurrentNode->key( rKey );
                pCurrentNode->value( rValue );
            }

            return false;
        }
        else
        {
            //
            // Generate a random level (height) for the new node.
            //
            uint16_t nNewLevel = _random();
              
                //
                // Update the maximum level height if this level is now larger.  (Could only be one greater!)
                //
            if (nNewLevel >= _nMaxHeight)
            {
                _nMaxHeight = nNewLevel + 1;
            }
                
                //
                // If this level is the largest level so far selected in the skip-list then extend the
                // list of maintain "update" pointers (of previously visited nodes of each level) for the
                // intermediate levels between the previous maximum and the new maximum.
                //
                // Also checks to see that the absolute maximum has been reached and restricts the height increase.
                //
            if (nNewLevel > _nMaxLevel)
            {
                for(iLevel = _nMaxLevel + 1; iLevel <= nNewLevel; iLevel++)
                {
                    _ppUpdate[iLevel] = _pHeader;
                }
                 
                //
                // Update the skip-list maximum level from here on.
                //
                _nMaxLevel = nNewLevel;
            }
                
            //
            // Create a new node.
            //
            pCurrentNode = DWFCORE_ALLOC_OBJECT( _Node(nNewLevel, rKey, rValue) );

            if (pCurrentNode == NULL)
            {
                _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate node" );
            }
              
                //
                // Update all forward references of previous skip-list nodes to reference the new value and 
                // set the forward references of the new node to those of the previous update nodes at each level.
                //
            for (iLevel = 0; iLevel <= nNewLevel; iLevel++)
            {
                pCurrentNode->forward( iLevel, _ppUpdate[iLevel]->forward(iLevel) );
                _ppUpdate[iLevel]->forward( iLevel, pCurrentNode );
            }                            
                
            _nCount++;

            return true;
        }
    }

private:

    //
    //
    //
    _Node* _search( const K& rKey ) const
        throw()
    {    
        _Node*  pCurrentNode = _pHeader;
        _Node*  pAlreadyChecked = NULL;

        //
        // Locate key of value immediately to the left of the key in rank.  During
        // the loop skip first using the largest steps possible and then proceed
        // with successively smaller steps (levels).   At each level maintain the
        // last visited node of that level (used for updating later).
        //
        int16_t iLevel = _nMaxLevel;
        for (; iLevel >= 0; iLevel--)
        {
            while ((pCurrentNode->forward(iLevel) != NULL) && 
                   (pCurrentNode->forward(iLevel) != pAlreadyChecked) &&
                   (_tLessThan(pCurrentNode->forward(iLevel)->key(), rKey)))
            {
                pCurrentNode = pCurrentNode->forward( iLevel );
            }
                
            pAlreadyChecked = pCurrentNode->forward( iLevel );
        }

        //
        // Get the next node.
        //
        pCurrentNode = pCurrentNode->forward( 0 );
          
            //
            // If the node was found then return the value. 
            //
        if ((pCurrentNode != NULL) && (_tEquals(pCurrentNode->key(), rKey)))
        {
            return pCurrentNode;
        }
        else
        {
            return NULL;
        }
    }

    uint16_t _random()
        throw()
    {
        uint16_t nLevel = 1;
        static bool bSeed = true;

        if (bSeed)
        {
            ::srand( DWFTimer::Tick32() );
            bSeed = false;
        }


        while ((::rand() < (RAND_MAX * DWFCORE_SKIPLIST_PROBABILITY_DISTRIB)) &&
               (nLevel <= _nMaxHeight) &&
               (nLevel < DWFCORE_SKIPLIST_MAX_NODE_LEVEL))
        {
            nLevel++;
        }

        return nLevel;
    }


private:

        //
        // First node in the list ... does not represent a key.
        //
    _Node*      _pHeader;   

        //
        // Update node array used in _insert and _delete
        // Keep it here and use the maximum node level size
        // to eliminate allocations
        //
    _Node*      _ppUpdate[DWFCORE_SKIPLIST_MAX_NODE_LEVEL + 1];

        //
        // Current maximum height of a node allowed.
        //
    uint16_t    _nMaxHeight;

        //
        // Current maximum level used in any node.
        //
    uint16_t    _nMaxLevel;

        //
        // Current count of items in the list.
        //
    uint32_t    _nCount;

    //
    // Comparators
    //
    E           _tEquals;
    L           _tLessThan;

private:

    DWFSkipList( const DWFSkipList& );
    DWFSkipList& operator=( const DWFSkipList& );
};

///
///\class       DWFCharKeySkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       Skip list collection template that uses \e const \e char* string keys.
///\since       1.0.1
///
///\note        Key strings are not copied into the list and therefore must not
///             be destroyed while the key is in use by the list.
///
///\param       V   The value type.
///
template<class V>
class DWFCharKeySkipList : public DWFSkipList<const char*, V, tDWFCharCompareEqual, tDWFCharCompareLess>
                         , virtual public DWFCoreMemory
{
public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFCharKeySkipList() throw() {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFCharKeySkipList() throw() {;}

private:

    //
    // Not Implemented
    //
    DWFCharKeySkipList( const DWFCharKeySkipList& );
    DWFCharKeySkipList& operator=( const DWFCharKeySkipList& );
};

///
///\class       DWFWCharKeySkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       Skip list collection template that uses \e const \e wchar_t* string keys.
///\since       1.0.1
///
///\note        Key strings are not copied into the list and therefore must not
///             be destroyed while the key is in use by the list.  However, if 
///             the value is an object that itself scopes the lifetime of the key
///             it may be used:
///             \code
///                 //
///                 // this is ok
///                 //
///             {
///                 DWFWCharKeySkipList<DWFString> oStringList;
///                 DWFString zString( L"some string" );
///
///                 oStringList.insert( (const wchar_t*)zString, zString );
///             }
///
///                 //
///                 // this isn't
///                 //
///             {
///                 DWFWCharKeySkipList<DWFString> oStringList;
///                 DWFString zString( L"some string" );
///                 wchar_t zBadKey[16] = L"bad key";
///
///                 oStringList.insert( zBadKey, zString );
///             }
///             \endcode
///
///\param       V   The value type.
///
template<class V>
class DWFWCharKeySkipList : public DWFSkipList<const wchar_t*, V, tDWFWCharCompareEqual, tDWFWCharCompareLess>
                          , virtual public DWFCoreMemory
{
public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFWCharKeySkipList() throw() {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFWCharKeySkipList() throw() {;}

private:

    //
    // Not Implemented
    //
    DWFWCharKeySkipList( const DWFWCharKeySkipList& );
    DWFWCharKeySkipList& operator=( const DWFWCharKeySkipList& );
};

///
///\class       DWFStringKeySkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       Skip list collection template that uses DWFString keys.
///\since       1.0.1
///
///\param       V   The value type.
///
template<class V>
class DWFStringKeySkipList : public DWFSkipList<DWFString, V, tDWFCompareEqual<DWFString>, tDWFCompareLess<DWFString>, tDWFStringDefinedEmpty>
                           , virtual public DWFCoreMemory
{
public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFStringKeySkipList() throw() {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFStringKeySkipList() throw() {;}

private:

    //
    // Not Implemented
    //
    DWFStringKeySkipList( const DWFStringKeySkipList& );
    DWFStringKeySkipList& operator=( const DWFStringKeySkipList& );
};


///
///\class       DWFSortedList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A skip list based collection with ordered keys.
///\since       1.0.1
///
///\param       T   The list type.
///\param       E   Defines equality between two elements of type \a T.
///\param       L   Defines ordering between two elements of type \a T.
///\param       Z   Defines the empty or \e zero-value for elements of type \a T.
///
template<class T, class E = tDWFCompareEqual<T>, class L = tDWFCompareLess<T>, class Z = tDWFDefinedEmpty<T> >
class DWFSortedList : virtual public DWFCoreMemory
{

private:

    typedef typename DWFSkipList<T, T, E, L, Z>::Iterator tListIterator;
    typedef typename DWFSkipList<T, T, E, L, Z>::ConstIterator tListConstIterator;

public:

    ///
    ///\class       Iterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of the iterator for sorted lists.
    ///\since       1.0.1
    ///
    class Iterator : virtual public DWFCoreMemory
                   , public DWFIterator<T>
    {
        
    public:

        ///
        ///         Constructor
        ///
        ///\param   pIterator   The inner skip list iterator that provides the data for this class.
        ///\throw   None
        ///
        Iterator( tListIterator* pIterator )
            throw()
            : _pIterator( pIterator )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Iterator()
            throw()
        {
            DWFCORE_FREE_OBJECT( _pIterator );
        }

        virtual void reset()
            throw()
        {
            _pIterator->reset();
        }

        virtual bool valid()
            throw()
        {
            return _pIterator->valid();
        }

        virtual bool next()
            throw()
        {
            return _pIterator->next();
        }
            
        virtual T& get()
            throw()
        {
            return _pIterator->key();
        }

    private:

        tListIterator* _pIterator;
    };

    ///
    ///\class       ConstIterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of the const iterator for sorted lists.
    ///\since       1.2
    ///
    class ConstIterator : virtual public DWFCoreMemory
                        , public DWFConstIterator<T>
    {
        
    public:

        ///
        ///         Constructor
        ///
        ///\param   pConstIterator   The inner skip list iterator that provides the data for this class.
        ///\throw   None
        ///
        ConstIterator( tListConstIterator* pConstIterator )
            throw()
            : _pConstIterator( pConstIterator )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ConstIterator()
            throw()
        {
            DWFCORE_FREE_OBJECT( _pConstIterator );
        }

        virtual void reset()
            throw()
        {
            _pConstIterator->reset();
        }

        virtual bool valid() const
            throw()
        {
            return _pConstIterator->valid();
        }

        virtual bool next()
            throw()
        {
            return _pConstIterator->next();
        }
            
        virtual const T& get() const
            throw( DWFException )
        {
            return _pConstIterator->key();
        }

    private:

        tListConstIterator* _pConstIterator;
    };

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFSortedList()
        throw()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFSortedList()
        throw()
    {;}

    ///
    ///\copydoc DWFSkipList::clear()
    ///
    virtual void clear()
        throw()
    {
        _oList.clear();
    }

    ///
    ///\copydoc DWFSkipList::size()
    ///
    virtual size_t size() const
        throw()
    {
        return _oList.size();
    }
    
    ///
    ///\copydoc DWFSkipList::iterator()
    ///
    virtual Iterator* iterator()
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator(_oList.iterator()) );
    }

    ///
    ///\copydoc DWFSkipList::constIterator()
    ///
    virtual ConstIterator* constIterator() const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oList.constIterator()) );
    }

    ///
    ///         Returns an iterator from the keyed element in the list.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rT  The element from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual Iterator* iterator( const T& rT )
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator(_oList.iterator(rT)) );
    }

    ///
    ///         Returns a const iterator from the keyed element in the list.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rT  The element from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///\since   1.2
    ///
    virtual ConstIterator* constIterator( const T& rT ) const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oList.constIterator(rT)) );
    }

    ///
    ///         Determine the existence of an element in the list.
    ///
    ///\param   rT  The element to look up.
    ///\return  \e true if the element exists in the list, \e false otherwise.
    ///\throw   None
    ///
    virtual bool exists( const T& rT ) const
        throw()
    {    
        return (_oList.find(rT) != NULL);
    }

    ///
    ///         Removes an element from the list.
    ///
    ///\param   rT  The element to erase.
    ///\return  \e true if keyed element was erased, \e false if the element was not found.
    ///\throw   None
    ///
    virtual bool erase( const T& rT )
        throw()
    {
        return _oList.erase( rT );
    }
 
    ///
    ///         Adds an element to the list.
    ///
    ///\param   rT          The element to add.
    ///\param   bReplace    If the new element already exists in the list, 
    ///                     this flag determines whether or not the it
    ///                     will be inserted and replace the previous element or ignored.
    ///\return  \e false the the element is a duplicate, \e true otherwise.
    ///\throw   DWFException
    ///
    virtual bool insert( const T& rT, bool bReplace = true )
        throw( DWFException )
    {
        return _oList.insert( rT, rT, bReplace );
    }

private:

    DWFSkipList<T, T>   _oList;

private:

    //
    // Not Implemented
    //
    DWFSortedList( const DWFSortedList& );
    DWFSortedList& operator=( const DWFSortedList& );
};

///
///
///


///
///\class       DWFCharKeyHashList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A skip list based collection with hashed \e const \e char* string keys.
///\since       1.0.1
///
///             This collection uses a skip list with 32-bit keys
///             to store it's elements.  These keys are computed from the
///             string keys using the hash function provided in the definition.
///\warning     No element chaining is performed so it is mathematically possible that 
///             duplicate hash values could be computed from different string keys.
///             
///\param       T   The list type.
///\param       H   The hashing function (must provide 32-bit keys)
///                 tDWFFNV1A32HashKernel is used by default.
///
template< class T, class H = tDWFFNV1A32HashKernel<const char> >
class DWFCharKeyHashList    : virtual public DWFCoreMemory
{

private:

    typedef typename DWFSkipList<uint32_t, T>::Iterator tListIterator;
    typedef typename DWFSkipList<uint32_t, T>::ConstIterator tListConstIterator;

public:

    ///
    ///\class       Iterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of the iterator for this hash list.
    ///\since       1.0.1
    ///
    class Iterator : virtual public DWFCoreMemory
                   , public DWFIterator<T>
    {
        
    public:

        ///
        ///         Constructor
        ///
        ///\param   pIterator   The inner skip list iterator that provides the data for this class.
        ///\throw   None
        ///
        Iterator( tListIterator* pIterator )
            throw()
            : _pIterator( pIterator )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Iterator()
            throw()
        {
            DWFCORE_FREE_OBJECT( _pIterator );
        }

        virtual void reset()
            throw()
        {
            _pIterator->reset();
        }

        virtual bool valid()
            throw()
        {
            return _pIterator->valid();
        }

        virtual bool next()
            throw()
        {
            return _pIterator->next();
        }
            
        virtual T& get()
            throw()
        {
            return _pIterator->value();
        }

    private:

        tListIterator* _pIterator;
    };    

    ///
    ///\class       ConstIterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of the const iterator for this hash list.
    ///\since       1.2
    ///
    class ConstIterator : virtual public DWFCoreMemory
                        , public DWFConstIterator<T>
    {
        
    public:

        ///
        ///         Constructor
        ///
        ///\param   pConstIterator   The inner skip list iterator that provides the data for this class.
        ///\throw   None
        ///
        ConstIterator( tListConstIterator* pConstIterator )
            throw()
            : _pConstIterator( pConstIterator )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ConstIterator()
            throw()
        {
            DWFCORE_FREE_OBJECT( _pConstIterator );
        }

        virtual void reset()
            throw()
        {
            _pConstIterator->reset();
        }

        virtual bool valid() const
            throw()
        {
            return _pConstIterator->valid();
        }

        virtual bool next()
            throw()
        {
            return _pConstIterator->next();
        }
            
        virtual const T& get() const
            throw( DWFException )
        {
            return _pConstIterator->value();
        }

    private:

        tListConstIterator* _pConstIterator;
    };    

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFCharKeyHashList()
        throw()
        : _tHash()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFCharKeyHashList()
        throw()
    {;}

    ///
    ///\copydoc DWFSkipList::clear()
    ///
    virtual void clear()
        throw()
    {
        //
        // clear base
        //
        _oList.clear();
    }

    ///
    ///\copydoc DWFSkipList::size()
    ///
    virtual size_t size() const
        throw()
    {
        return _oList.size();
    }

    ///
    ///\copydoc DWFSkipList::iterator()
    ///
    virtual Iterator* iterator()
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator(_oList.iterator()) );
    }

    ///
    ///\copydoc DWFSkipList::constIterator()
    ///
    virtual ConstIterator* constIterator() const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oList.constIterator()) );
    }

    ///
    ///\copydoc DWFSkipList::iterator(const K&)
    ///
    virtual Iterator* iterator( const char*& rKey )
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator(_oList.iterator(_tHash(rKey))) );
    }

    ///
    ///\copydoc DWFSkipList::constIterator(const K&)
    ///
    virtual ConstIterator* constIterator( const char*& rKey ) const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oList.constIterator(_tHash(rKey))) );
    }

    ///
    ///\copydoc DWFSkipList::find()
    ///
    virtual T* find( const char*& rKey ) const
        throw()
    {
        return _oList.find( _tHash(rKey) );
    }

    ///
    ///\copydoc DWFSkipList::erase()
    ///
    virtual bool erase( const char*& rKey )
        throw()
    {
        return _oList.erase( _tHash(rKey) );
    }

    ///
    ///\copydoc DWFSkipList::insert()
    ///
    virtual bool insert( const char*& rKey, const T& rValue, bool bReplace = true )
        throw( DWFException )
    {
        return _oList.insert( _tHash(rKey), rValue, bReplace );
    }

private:

    H                        _tHash;
    DWFSkipList<uint32_t, T> _oList;

private:

    //
    // Not Implemented
    //
    DWFCharKeyHashList( const DWFCharKeyHashList& );
    DWFCharKeyHashList& operator=( const DWFCharKeyHashList& );
};

///
///
///

///
///\class       DWFWCharKeyHashList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A skip list based collection with hashed \e const \e wchar_t* string keys.
///\since       1.0.1
///
///             This collection uses a skip list with 32-bit keys
///             to store it's elements.  These keys are computed from the
///             string keys using the hash function provided in the definition.
///\warning     No element chaining is performed so it is mathematically possible that 
///             duplicate hash values could be computed from different string keys.
///             
///\param       T   The list type.
///\param       H   The hashing function (must provide 32-bit keys)
///                 tDWFFNV1A32HashKernel is used by default.
///
template< class T, class H = tDWFFNV1A32HashKernel<const wchar_t> >
class DWFWCharKeyHashList   : virtual public DWFCoreMemory
{

private:

    typedef typename DWFSkipList<uint32_t, T>::Iterator tListIterator;
    typedef typename DWFSkipList<uint32_t, T>::ConstIterator tListConstIterator;

public:

    ///
    ///\class       Iterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of the iterator for this hash list.
    ///\since       1.0.1
    ///
    class Iterator : virtual public DWFCoreMemory
                   , public DWFIterator<T>
    {
        
    public:

        ///
        ///         Constructor
        ///
        ///\param   pIterator   The inner skip list iterator that provides the data for this class.
        ///\throw   None
        ///
        Iterator( tListIterator* pIterator )
            throw()
            : _pIterator( pIterator )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Iterator()
            throw()
        {
            DWFCORE_FREE_OBJECT( _pIterator );
        }

        virtual void reset()
            throw()
        {
            _pIterator->reset();
        }

        virtual bool valid()
            throw()
        {
            return _pIterator->valid();
        }

        virtual bool next()
            throw()
        {
            return _pIterator->next();
        }
            
        virtual T& get()
            throw()
        {
            return _pIterator->value();
        }

    private:

        tListIterator* _pIterator;
    };    

    ///
    ///\class       ConstIterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of the const iterator for this hash list.
    ///\since       1.2
    ///
    class ConstIterator : virtual public DWFCoreMemory
                        , public DWFConstIterator<T>
    {
        
    public:

        ///
        ///         Constructor
        ///
        ///\param   pConstIterator   The inner skip list iterator that provides the data for this class.
        ///\throw   None
        ///
        ConstIterator( tListConstIterator* pConstIterator )
            throw()
            : _pConstIterator( pConstIterator )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ConstIterator()
            throw()
        {
            DWFCORE_FREE_OBJECT( _pConstIterator );
        }

        virtual void reset()
            throw()
        {
            _pConstIterator->reset();
        }

        virtual bool valid() const
            throw()
        {
            return _pConstIterator->valid();
        }

        virtual bool next()
            throw()
        {
            return _pConstIterator->next();
        }
            
        virtual const T& get() const
            throw( DWFException )
        {
            return _pConstIterator->value();
        }

    private:

        tListConstIterator* _pConstIterator;
    };    

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFWCharKeyHashList()
        throw()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFWCharKeyHashList()
        throw()
    {;}

    ///
    ///\copydoc DWFSkipList::clear()
    ///
    virtual void clear()
        throw()
    {
        //
        // clear base
        //
        _oList.clear();
    }

    ///
    ///\copydoc DWFSkipList::size()
    ///
    virtual size_t size() const
        throw()
    {
        return _oList.size();
    }

    ///
    ///\copydoc DWFSkipList::iterator()
    ///
    virtual Iterator* iterator()
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator(_oList.iterator()) );
    }

    ///
    ///\copydoc DWFSkipList::constIterator()
    ///
    virtual ConstIterator* constIterator() const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oList.constIterator()) );
    }

    ///
    ///\copydoc DWFSkipList::iterator(const K&)
    ///
    virtual Iterator* iterator( const wchar_t*& rKey )
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator(_oList.iterator(_tHash(rKey))) );
    }

    ///
    ///\copydoc DWFSkipList::constIterator(const K&)
    ///
    virtual ConstIterator* constIterator( const wchar_t*& rKey ) const
        throw()
    {
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oList.constIterator(_tHash(rKey))) );
    }

    ///
    ///\copydoc DWFSkipList::find()
    ///
    virtual T* find( const wchar_t*& rKey ) const
        throw()
    {
        return _oList.find( _tHash(rKey) );
    }

    ///
    ///\copydoc DWFSkipList::erase()
    ///
    virtual bool erase( const wchar_t*& rKey )
        throw()
    {
        return _oList.erase( _tHash(rKey) );
    }

    ///
    ///\copydoc DWFSkipList::insert()
    ///
    virtual bool insert( const wchar_t*& rKey, const T& rValue, bool bReplace = true )
        throw( DWFException )
    {
        return _oList.insert( _tHash(rKey), rValue, bReplace );
    }

private:

    H                        _tHash;
    DWFSkipList<uint32_t, T> _oList;

private:

    //
    // Not Implemented
    //
    DWFWCharKeyHashList( const DWFWCharKeyHashList& );
    DWFWCharKeyHashList& operator=( const DWFWCharKeyHashList& );
};


///
///\class       DWFChainedSkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A chained skip list allows for value types keyed in two dimensions 
///             where the first can be an aggregate and the second is unique.
///\since       1.0.1
///
///\param       PK  The primary (list) key type.
///\param       SK  The secondary (chains) key type.
///\param       V   The value type.
///\param       PE  Defines equality between two primary keys.
///\param       SE  Defines equality between two secondary keys.
///\param       PL  Defines ordering between two primary keys.
///\param       SL  Defines ordering between two secondary keys.
///\param       PZ  Defines the empty or \e zero-value for the primary key type.
///\param       SZ  Defines the empty or \e zero-value for the secondary key type.
///
template<class PK, class SK, class V, 
         class PE = tDWFCompareEqual<PK>, class SE = tDWFCompareEqual<SK>, 
         class PL = tDWFCompareLess<PK>, class SL = tDWFCompareLess<SK>, 
         class PZ = tDWFDefinedEmpty<PK>, class SZ = tDWFDefinedEmpty<SK> >
class DWFChainedSkipList    : virtual public DWFCoreMemory
{

    typedef typename DWFSkipList<SK, V, SE, SL, SZ>::Iterator _tChainIterator;
    typedef typename DWFSkipList<SK, V, SE, SL, SZ>::ConstIterator _tChainConstIterator;
    typedef typename DWFSkipList<PK, DWFSkipList<SK, V, SE, SL, SZ>*, PE, PL, PZ>::Iterator _tListIterator;
    typedef typename DWFSkipList<PK, DWFSkipList<SK, V, SE, SL, SZ>*, PE, PL, PZ>::ConstIterator _tListConstIterator;

public:

    ///
    ///\class       Iterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of a skip list iterator for chained lists.
    ///\since       1.0.1
    ///
    class Iterator : public DWFSkipList<SK, V, SE, SL, SZ>::Iterator
                   , virtual public DWFCoreMemory
    {
        public:

            Iterator( _tChainIterator* piChain )
                throw()
                : _piList( NULL )
                , _piChain( piChain )
            {;}

    //
    // The MSVC 7.0 pre-processor cannot resolve the difference
    // between  typedef typename _tChainIterator 
    // and      typedef typename _tListIterator
    // so this hack becomes necessary to prevent the constructor
    // appearing to be declared twice.
    //
#if defined( _DWFCORE_WIN32_SYSTEM ) && ( _MSC_VER <= 1300 )
            Iterator( _tListIterator* piList, int )
#else
            ///
            ///         Constructor
            ///
            ///\param   piList  An iterator for obtaining the chained skip lists.
            ///\throw   None
            ///
            Iterator( _tListIterator* piList )
#endif
                throw()
                : _piList( piList )
                , _piChain( NULL )
            {
                if (piList && (piList->valid()))
                {
                    _piChain = piList->value()->iterator();
                }
            }

            ///
            ///         Destructor
            ///
            ///\throw   None
            ///
            virtual ~Iterator()
                throw()
            {
                if (_piChain)
                {
                    DWFCORE_FREE_OBJECT( _piChain );
                }
                if (_piList)
                {
                    DWFCORE_FREE_OBJECT( _piList );
                }
            }

            void reset()
                throw()
            {
                    //
                    // always work on composite iterator first
                    //
                if (_piList)
                {
                    //
                    // reset
                    //
                    _piList->reset();

                        //
                        // free any intermediate chain iterator
                        //
                    if (_piChain)
                    {
                        DWFCORE_FREE_OBJECT( _piChain );
                        _piChain = NULL;
                    }
                }
                    //
                    // we are only a chain iterator
                    //
                else if (_piChain)
                {
                    _piChain->reset();
                }
            }

            bool valid()
                throw()
            {
                    //
                    // always work on composite iterator first
                    //
                if (_piList && _piList->valid())
                {
                        //
                        // the intermediate chain iterator is not valid
                        // move on the the next one
                        //
                    if (_piChain && (_piChain->valid() == false))
                    {
                        DWFCORE_FREE_OBJECT( _piChain );
                        _piChain = NULL;

                            //
                            // get the next intermediate iterator
                            //
                        if (_piList->next())
                        {
                            _piChain = _piList->value()->iterator();
                        }
                    }

                    //
                    // if _piChain is NULL at this point, 
                    // there are no more chained iterators
                    //
                }

                return (_piChain ? _piChain->valid() : false);
            }

            bool next()
                throw()
            {
                    //
                    // this pointer should always be accurate
                    //
                if (_piChain)
                {
                        //
                        // in this case, if next() is false,
                        // we need to try and get the next intermediate iterator
                        //
                    if (_piList && (_piChain->next() == false))
                    {
                        DWFCORE_FREE_OBJECT( _piChain );
                        _piChain = NULL;

                            //
                            // get the next intermediate iterator
                            //
                        if (_piList->next())
                        {
                            _piChain = _piList->value()->iterator();
                        }
                    }

                    if (_piChain)
                    {
                        return _piChain->valid();
                    }
                }

                return false;
            }
            
            SK& key()
                throw( DWFException )
            {
                if (_piChain)
                {
                    return _piChain->key();
                }
                else
                {
                    _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
                }
            }

            V& value()
                throw( DWFException )
            {
                if (_piChain)
                {
                    return _piChain->value();
                }
                else
                {
                    _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
                }
            }

        private:

            _tListIterator*     _piList;
            _tChainIterator*    _piChain;
        };

    ///
    ///\class       ConstIterator   dwfcore/SkipList.h  "dwfcore/SkipList.h"
    ///\brief       An implementation of a skip list const iterator for chained lists.
    ///\since       1.2
    ///
    class ConstIterator : public DWFSkipList<SK, V, SE, SL, SZ>::ConstIterator
                        , virtual public DWFCoreMemory
    {
        public:

            ConstIterator( _tChainConstIterator* piChain )
                throw()
                : _piList( NULL )
                , _piChain( piChain )
            {;}

    //
    // The MSVC 7.0 pre-processor cannot resolve the difference
    // between  typedef typename _tChainConstIterator 
    // and      typedef typename _tListConstIterator
    // so this hack becomes necessary to prevent the constructor
    // appearing to be declared twice.
    //
#if defined( _DWFCORE_WIN32_SYSTEM ) && ( _MSC_VER <= 1300 )
            ConstIterator( _tListConstIterator* piList, int )
#else
            ///
            ///         Constructor
            ///
            ///\param   piList  A const iterator for obtaining the chained skip lists.
            ///\throw   None
            ///
            ConstIterator( _tListConstIterator* piList )
#endif
                throw()
                : _piList( piList )
                , _piChain( NULL )
            {
                if (piList && (piList->valid()))
                {
                    _piChain = piList->value()->constIterator();
                }
            }

            ///
            ///         Destructor
            ///
            ///\throw   None
            ///
            virtual ~ConstIterator()
                throw()
            {
                if (_piChain)
                {
                    DWFCORE_FREE_OBJECT( _piChain );
                }
                if (_piList)
                {
                    DWFCORE_FREE_OBJECT( _piList );
                }
            }

            void reset()
                throw()
            {
                    //
                    // always work on composite iterator first
                    //
                if (_piList)
                {
                    //
                    // reset
                    //
                    _piList->reset();

                        //
                        // free any intermediate chain iterator
                        //
                    if (_piChain)
                    {
                        DWFCORE_FREE_OBJECT( _piChain );
                        _piChain = NULL;
                    }
                }
                    //
                    // we are only a chain iterator
                    //
                else if (_piChain)
                {
                    _piChain->reset();
                }
            }

            bool valid() const
                throw()
            {
                    //
                    // always work on composite iterator first
                    //
                if (_piList && _piList->valid())
                {
                        //
                        // the intermediate chain iterator is not valid
                        // move on the the next one
                        //
                    if (_piChain && (_piChain->valid() == false))
                    {
                        DWFCORE_FREE_OBJECT( _piChain );
                        _piChain = NULL;

                            //
                            // get the next intermediate iterator
                            //
                        if (_piList->next())
                        {
                            _piChain = _piList->value()->constIterator();
                        }
                    }

                    //
                    // if _piChain is NULL at this point, 
                    // there are no more chained iterators
                    //
                }

                return (_piChain ? _piChain->valid() : false);
            }

            bool next()
                throw()
            {
                    //
                    // this pointer should always be accurate
                    //
                if (_piChain)
                {
                        //
                        // in this case, if next() is false,
                        // we need to try and get the next intermediate iterator
                        //
                    if (_piList && (_piChain->next() == false))
                    {
                        DWFCORE_FREE_OBJECT( _piChain );
                        _piChain = NULL;

                            //
                            // get the next intermediate iterator
                            //
                        if (_piList->next())
                        {
                            _piChain = _piList->value()->constIterator();
                        }
                    }

                    if (_piChain)
                    {
                        return _piChain->valid();
                    }
                }

                return false;
            }
            
            const SK& key()
                throw( DWFException )
            {
                if (_piChain)
                {
                    return _piChain->key();
                }
                else
                {
                    _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
                }
            }

            const V& value()
                throw( DWFException )
            {
                if (_piChain)
                {
                    return _piChain->value();
                }
                else
                {
                    _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
                }
            }

        private:

            _tListConstIterator*     _piList;
            _tChainConstIterator*    _piChain;
        };
public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFChainedSkipList()
        throw()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFChainedSkipList()
        throw()
    {
        _tListIterator* piChain = _oChains.iterator();

        if (piChain)
        {
            for (;piChain->valid(); piChain->next())
            {
                DWFCORE_FREE_OBJECT( piChain->value() );
            }
            
            DWFCORE_FREE_OBJECT( piChain );
        }
    }

    ///
    ///\copydoc DWFSkipList::clear()
    ///
    virtual void clear()
        throw()
    {
        _tListIterator* piChain = _oChains.iterator();
        if (piChain)
        {
            for (;piChain->valid(); piChain->next())
            {
                piChain->value()->clear();
            }
            DWFCORE_FREE_OBJECT( piChain );
        }
        
        _oChains.clear();
    }

    ///
    ///         Returns the number of elements stored in the list.
    ///         This count is the sum of all chained lists counts.
    ///
    ///\return  The number of list elements.
    ///\throw   None
    ///
    virtual size_t size() const
        throw()
    {
        size_t nCount = 0;
        _tListConstIterator* piChain = _oChains.constIterator();
    
        if (piChain)
        {
            for (;piChain->valid(); piChain->next())
            {
                nCount += piChain->value()->size();
            }
            DWFCORE_FREE_OBJECT( piChain );
        }

        return nCount;
    }

    ///
    ///         Returns the number of elements stored in one chained list.
    ///
    ///\return  The number of list elements.
    ///\throw   None
    ///
    virtual size_t size( const PK& rPKey ) const
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? (*ppChain)->size() : 0);
    }

    ///
    ///\copydoc DWFSkipList::iterator()
    ///
    virtual Iterator* iterator()
        throw()
    {
        //
        // see above
        //
#if defined( _DWFCORE_WIN32_SYSTEM ) && ( _MSC_VER <= 1300 )
        return DWFCORE_ALLOC_OBJECT( Iterator(_oChains.iterator(), 0) );
#else
        return DWFCORE_ALLOC_OBJECT( Iterator(_oChains.iterator()) );
#endif
    }

    ///
    ///\copydoc DWFSkipList::constIterator()
    ///
    virtual ConstIterator* constIterator() const
        throw()
    {
        //
        // see above
        //
#if defined( _DWFCORE_WIN32_SYSTEM ) && ( _MSC_VER <= 1300 )
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oChains.constIterator(), 0) );
#else
        return DWFCORE_ALLOC_OBJECT( ConstIterator(_oChains.constIterator()) );
#endif
    }

    ///
    ///         Returns an iterator from the keyed element in the list.
    ///         The iterator will begin at the first element of the chain
    ///         keyed by \a rPKey.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rPKey   The key of the first chain list from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual Iterator* iterator( const PK& rPKey )
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? DWFCORE_ALLOC_OBJECT(Iterator((*ppChain)->iterator())) : NULL);
    }

    ///
    ///         Returns a const iterator from the keyed element in the list.
    ///         The iterator will begin at the first element of the chain
    ///         keyed by \a rPKey.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rPKey   The key of the first chain list from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///\since   1.2
    ///
    virtual ConstIterator* constIterator( const PK& rPKey ) const
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? DWFCORE_ALLOC_OBJECT(ConstIterator((*ppChain)->constIterator())) : NULL);
    }

    ///
    ///         Returns an iterator from the keyed element in the list.
    ///         The iterator will begin at the element keyed by \a rSKey 
    ///         of the chain keyed by \a rPKey.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rPKey   The key of the first chain list from which to start the iterator.
    ///\param   rSKey   The key of the first list element in the chain from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///
    virtual Iterator* iterator( const PK& rPKey, const SK& rSKey )
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? DWFCORE_ALLOC_OBJECT(Iterator((*ppChain)->iterator(rSKey))) : NULL);
    }

    ///
    ///         Returns a const iterator from the keyed element in the list.
    ///         The iterator will begin at the element keyed by \a rSKey 
    ///         of the chain keyed by \a rPKey.
    ///         The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   rPKey   The key of the first chain list from which to start the iterator.
    ///\param   rSKey   The key of the first list element in the chain from which to start the iterator.
    ///\return  A pointer to a new list iterator.
    ///\throw   None
    ///\since   1.2
    ///
    virtual ConstIterator* constIterator( const PK& rPKey, const SK& rSKey ) const
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? DWFCORE_ALLOC_OBJECT(ConstIterator((*ppChain)->constIterator(rSKey))) : NULL);
    }

    ///
    ///         Search a keyed chain list with a given [secondary] key and return the associated value if it exists.
    ///
    ///\param   rPKey   The key of the chain list to search.
    ///\param   rSKey   The key for which to search within the chain list.
    ///\return  A pointer to the value associated with the key or NULL if the key was not found.
    ///\throw   None
    ///
    virtual V* find( const PK& rPKey, const SK& rSKey ) const
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? (*ppChain)->find(rSKey) : NULL);
    }

    ///
    ///         Removes chain list from the list.
    ///
    ///\param   rPKey   The key of the element to erase.
    ///\return  \e true if the chained list was erased, \e false if the list was not found.
    ///\throw   None
    ///
    virtual bool erase( const PK& rPKey )
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        //
        // forward the call
        //
        bool bErased = _oChains.erase( rPKey );

        if (*ppChain)
        {
            DWFCORE_FREE_OBJECT( *ppChain );
        }

        return bErased;
    }

    ///
    ///         Removes an element from the list.
    ///
    ///\param   rPKey   The key of the chain list to search.
    ///\param   rSKey   The key of the element in the chain list to remove.
    ///\return  \e true if keyed element was erased, \e false if the element was not found.
    ///\throw   None
    ///
    virtual bool erase( const PK& rPKey, const SK& rSKey )
        throw()
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

        return (ppChain ? (*ppChain)->erase(rSKey) : false);
    }
        

    ///
    ///         Adds an element to the list.
    ///
    ///\param   rPKey       The key of the chain list into which the element will be added.
    ///\param   rSKey       The key of the element in the chain list.
    ///\param   rValue      The value of the element to add.
    ///\param   bReplace    If the new element uses a key that already exists, 
    ///                     this flag determines whether or not the new element
    ///                     will be inserted and replace the previous element or ignored.
    ///\return  \e false the the element key is a duplicate, \e true otherwise.
    ///\throw   DWFException
    ///
    virtual bool insert( const PK& rPKey, const SK& rSKey, const V& rValue, bool bReplace = true )
        throw( DWFException )
    {
        DWFSkipList<SK, V, SE, SL, SZ>** ppChain = _oChains.find( rPKey );

            //
            // add element into chain
            //
        if (ppChain)
        {
            return (*ppChain)->insert( rSKey, rValue, bReplace );
        }
            //
            // new chain
            //
        else
        {
            DWFSkipList<SK, V, SE, SL, SZ>* pNewChain = DWFCORE_ALLOC_OBJECT( (DWFSkipList<SK, V, SE, SL, SZ>) );
            
            if (pNewChain == NULL)
            {
                _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate chain" );
            }

            _oChains.insert( rPKey, pNewChain );

            return pNewChain->insert( rSKey, rValue, bReplace );
        }
    }

private:

    DWFSkipList<PK, DWFSkipList<SK, V, SE, SL, SZ>*, PE, PL, PZ> _oChains;

private:

    //
    // Not Implemented
    //
    DWFChainedSkipList( const DWFChainedSkipList& );
    DWFChainedSkipList& operator=( const DWFChainedSkipList& );
};

///
///\class       DWFCharKeyChainedSkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A chained skip list collection template that uses \e const \e char* string keys.
///\since       1.0.1
///
///\note        Key strings are not copied into the list and therefore must not
///             be destroyed while the key is in use by the list.
///
///\param       V   The value type.
///
template<class V>
class DWFCharKeyChainedSkipList : public DWFChainedSkipList<const char*, const char*, V, tDWFCharCompareEqual, tDWFCharCompareEqual, tDWFCharCompareLess, tDWFCharCompareLess>
                                , virtual public DWFCoreMemory
{
public:

    DWFCharKeyChainedSkipList() throw() {;}
    virtual ~DWFCharKeyChainedSkipList() throw() {;}

private:

    DWFCharKeyChainedSkipList( const DWFCharKeyChainedSkipList& );
    DWFCharKeyChainedSkipList& operator=( const DWFCharKeyChainedSkipList& );
};

///
///\class       DWFWCharKeyChainedSkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A chained skip list collection template that uses \e const \e wchar_t* string keys.
///\since       1.0.1
///
///\note        Key strings are not copied into the list and therefore must not
///             be destroyed while the key is in use by the list.
///
///\param       V   The value type.
///
template<class V>
class DWFWCharKeyChainedSkipList : public DWFChainedSkipList<const wchar_t*, const wchar_t*, V, tDWFWCharCompareEqual, tDWFWCharCompareEqual, tDWFWCharCompareLess, tDWFWCharCompareLess>
                                 , virtual public DWFCoreMemory
{
public:

    DWFWCharKeyChainedSkipList() throw() {;}
    virtual ~DWFWCharKeyChainedSkipList() throw() {;}

private:

    DWFWCharKeyChainedSkipList( const DWFWCharKeyChainedSkipList& );
    DWFWCharKeyChainedSkipList& operator=( const DWFWCharKeyChainedSkipList& );
};

///
///\class       DWFStringKeyChainedSkipList     dwfcore/SkipList.h  "dwfcore/SkipList.h"
///\brief       A chained skip list collection template that uses DWFString keys.
///\since       1.0.1
///
///\note        Key strings are not copied into the list and therefore must not
///             be destroyed while the key is in use by the list.
///
///\param       V   The value type.
///
template<class V>
class DWFStringKeyChainedSkipList : public DWFChainedSkipList<DWFString, DWFString, V, 
                                                             tDWFCompareEqual<DWFString>, tDWFCompareEqual<DWFString>,
                                                             tDWFCompareLess<DWFString>, tDWFCompareLess<DWFString>,
                                                             tDWFStringDefinedEmpty, tDWFStringDefinedEmpty>
                                  , virtual public DWFCoreMemory
{
public:

    DWFStringKeyChainedSkipList() throw() {;}
    virtual ~DWFStringKeyChainedSkipList() throw() {;}

private:

    DWFStringKeyChainedSkipList( const DWFStringKeyChainedSkipList& );
    DWFStringKeyChainedSkipList& operator=( const DWFStringKeyChainedSkipList& );
};

//
// Delete allocated values in list type classes, namely DWFSkipList and DWFVector,
// that provided the Iterator and ValueType typedefs.
//
// TODO: Add TypeTrait code to ensure that the value of the list are pointer types.
template< class L >
void DeleteAllocatedValuesInList( L& a )
{
    typename L::Iterator* pIter = a.iterator();
    typename L::ValueType ptr = NULL;

    if (pIter)
    {
        for (; pIter->valid(); pIter->next())
        {
            ptr = pIter->value();
            DWFCORE_FREE_OBJECT( ptr );
        }
        DWFCORE_FREE_OBJECT( pIter );
    }
}

}

#endif

