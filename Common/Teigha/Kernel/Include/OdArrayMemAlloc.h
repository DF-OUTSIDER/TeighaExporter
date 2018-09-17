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
#ifndef ODARRAYMEMALLOC_H_INCLUDED
#define ODARRAYMEMALLOC_H_INCLUDED

#include <new>

#include "TD_PackPush.h"

#include "OdArray.h"
#include "OdAlloc.h"

/** <group Other_Classes>
*/
class OdrxMemoryManager
{
public:
  static void* Alloc(size_t nBytes) { return ::odrxAlloc(nBytes); }
  static void Free(void* pMemBlock) { ::odrxFree(pMemBlock); }
  static void* Realloc(void* pMemBlock, size_t newSize, size_t oldSize)
  {
    return ::odrxRealloc(pMemBlock, newSize, oldSize);
  }
};
template <class T, class A = OdObjectsAllocator<T>, class Mm = OdrxMemoryManager> class OdArrayMemAlloc;

/** \details
    This template class implements dynamic Array objects within DWGdirect.

    \remarks
    Methods are provided to access Array elements via both array indices and array pointers (iterators).

    Some definitions are in order:

    1. Logical Length or Size -- The number of entries in the array. Initially zero.
    2. Physical Length -- The maximum Logical Length of the array before it automatically grows.
    3. Grow Length -- The number of entries by which the Physical Length will grow as required.

    \sa
    Db

    <group Other_Classes>
*/
template <class T, class A, class Mm> class OdArrayMemAlloc
{
public:
  typedef typename A::size_type size_type;
  typedef T* iterator;
  typedef const T* const_iterator;
private:
  struct Buffer : OdArrayBuffer
  {
    T* data() const { return (T*)(this+1); }

    static Buffer* allocate(size_type nLength2Allocate, int nGrowBy)
    {
      size_type nBytes2Allocate = sizeof(Buffer) + nLength2Allocate * sizeof(T);
      ODA_ASSERT(nBytes2Allocate > nLength2Allocate); // size_type overflow
      if(nBytes2Allocate > nLength2Allocate)
      {
        Buffer* pBuffer = (Buffer*)Mm::Alloc(nBytes2Allocate);
        if (pBuffer)
        {
          pBuffer->m_nRefCounter = 1;
          pBuffer->m_nGrowBy     = nGrowBy;
          pBuffer->m_nAllocated  = nLength2Allocate;
          pBuffer->m_nLength     = 0;
          return pBuffer;
        }
      }
      throw OdError(eOutOfMemory);
    }
    static Buffer* _default()
    {
      return (Buffer*)&g_empty_array_buffer;
    }
    void release()
    {
      ODA_ASSERT(m_nRefCounter);
      if((--m_nRefCounter)==0 && this != _default())
      {
        A::destroy(data(), m_nLength);
        Mm::Free(this);
      }
    }
    void addref() const { ++m_nRefCounter; }
  };
  class reallocator
  {
    bool _may_use_realloc;
    Buffer* m_pBuffer;    
  public:
    inline reallocator( bool may_use_realloc = false ) : _may_use_realloc(may_use_realloc) , m_pBuffer(NULL)
    {
      if ( !_may_use_realloc )
      {
        m_pBuffer = Buffer::_default();
        m_pBuffer->addref();
      }
    }
    inline void reallocate(OdArrayMemAlloc* pArray, size_type nNewLen )
    {
      if(!pArray->referenced())
      {
        if(nNewLen > pArray->physicalLength())
        {
          if ( !_may_use_realloc )
          {
            m_pBuffer->release();
            m_pBuffer = pArray->buffer();
            m_pBuffer->addref(); // save buffer to ensure copy from itself would work (e.g insertAt)
          }
          pArray->copy_buffer(nNewLen, _may_use_realloc);
        }
      }
      else
      {
        pArray->copy_buffer(nNewLen);
      }
    }
    inline ~reallocator()
    {
      if ( !_may_use_realloc ) m_pBuffer->release();
    }
  };
  friend class reallocator;
  const_iterator begin_const() const { return begin(); }
  iterator begin_non_const() { return begin(); }
  const_iterator end_const() { return end(); }
  iterator end_non_const() { return end(); }
  void copy_before_write(size_type len, bool may_use_realloc = false )
  {
    if( referenced() )
      copy_buffer(len);
    else if ( len > physicalLength() )
      copy_buffer( len, may_use_realloc );
  }
  void copy_if_referenced() { if(referenced()) { copy_buffer(physicalLength()); } }
  void copy_buffer( size_type len, bool may_use_realloc = false, bool force_size = false )
  {
    Buffer* pOldBuffer = buffer();
    int nGrowBy = pOldBuffer->m_nGrowBy;
    size_type len2 = len;
    if ( !force_size )
    {
      if(nGrowBy > 0)
      {
        len2 += nGrowBy;
        len2 = ((len2 - 1) / nGrowBy) * nGrowBy;
      }
      else
      {
        len2 = pOldBuffer->m_nLength;
        len2 = len2 + -nGrowBy * len2 / 100;
        if(len2 < len)
        {
          len2 = len;
        }
      }
    }
    if ( may_use_realloc && A::useRealloc() && !empty() )
    {
      Buffer* pNewBuffer = reinterpret_cast<Buffer*>( Mm::Realloc(
        pOldBuffer, len2 * sizeof(T) + sizeof(Buffer), pOldBuffer->m_nAllocated * sizeof(T) + sizeof(Buffer) ) );
      pNewBuffer->m_nAllocated = len2;
      pNewBuffer->m_nLength = odmin(pNewBuffer->m_nLength, len);
      m_pData = pNewBuffer->data();
    }
    else
    {
      Buffer* pNewBuffer = Buffer::allocate(len2, nGrowBy);
      len = odmin(pOldBuffer->m_nLength, len);
      A::constructn(pNewBuffer->data(), pOldBuffer->data(), len);
      pNewBuffer->m_nLength = len;
      m_pData = pNewBuffer->data();
      pOldBuffer->release();
    }
  }
  inline void assertValid(size_type index) const { if(!isValid(index)) { ODA_FAIL(); throw OdError_InvalidIndex(); } }
  static inline void rise_error(OdResult e) { ODA_FAIL(); throw OdError(e); }
public:
  // STL-like interface

  /** \details
    Returns an iterator that references the first element in this Array object.
  */
  iterator begin() { if(!empty()) { copy_if_referenced(); return data(); } return 0; }
  /** \details
    Returns an iterator that references the first element in this Array object.
  */
  const_iterator begin() const { if(!empty()) { return data(); } return 0; }

  /** \details
    Returns an iterator that references the location after the last element in this Array object.
  */
  iterator end() { if(!empty()) { copy_if_referenced(); return data() + length(); } return 0; }
  /** \details
    Returns an iterator that references the location after the last element in this Array object.
  */
  const_iterator end() const { if(!empty()) { return data()+length(); } return 0; }

  /** \details
    Inserts an elementinto this Array object.

    \param before [in]  Position where first element is to be inserted.
    \param first [in]  Position of first element to be inserted.
    \param afterLast [in]  Position of first element after the last element to be inserted.

    \remarks
    The range of elements may be from another Array object.
  */
  void insert(
    iterator before,
    const_iterator first,
    const_iterator afterLast)
  {
    size_type len = length();
    size_type index = (size_type)(before - begin_const());
    if(index <= len && afterLast>=first)
    {
      if(afterLast > first)
      {
        size_type num2copy = (size_type)(afterLast - first);
        reallocator r( first < begin() || first >= end() );
        r.reallocate(this, len + num2copy);
        A::constructn(m_pData + len, first, num2copy);
        buffer()->m_nLength = len + num2copy;
        T* pDestination = m_pData + index;
        if(index != len)
        {
          A::move(pDestination + num2copy, pDestination, len - index);
        }
        A::copy(pDestination, first, (size_type)(afterLast - first));
      }
    }
    else
    {
      rise_error(eInvalidInput);
    }
  }
  /** \details
    Specifies the logical length for this Array object.
    \param logicalLength [in]  Logical length.
    \param value [in]  Value for the elements added to obtain the new logical length.
  */
  void resize(
    size_type logicalLength,
    const T& value )
  {
    size_type len = length();
    int d = logicalLength - len;
    if ( d > 0 )
    {
      reallocator r( m_pData > &value || &value > (m_pData + len) );
      r.reallocate(this, logicalLength);
      A::constructn(m_pData + len, d, value);
    }
    else if ( d < 0 )
    {
      d=-d;
      if(!referenced())
      {
        A::destroy(m_pData + logicalLength, d);
      }
      else
      {
        copy_buffer(logicalLength);
      }
    }
    buffer()->m_nLength = logicalLength;
  }

  /** \details
    Specifies the logical length for this Array object.
    \param logicalLength [in]  Logical length.
  */
  void resize(
    size_type logicalLength )
  {
    size_type len = length();
    int d = logicalLength - len;
    if ( d > 0 )
    {
      copy_before_write( len + d, true );
      A::constructn(m_pData + len, d);
    }
    else if ( d < 0 )
    {
      d = -d;
      if ( !referenced() )
      {
        A::destroy( m_pData + logicalLength, d );
      }
      else
      {
        copy_buffer(logicalLength);
      }
    }
    buffer()->m_nLength = logicalLength;
  }

  /** \details
    Returns the logical length of this Array object.
  */
  size_type size() const { return buffer()->m_nLength; }

  /** \details
    Returns true if and only if this Array is empty.
  */
  bool empty() const { return size() == 0; }

  /** \details
    Returns the physical length of this Array object.
  */
  size_type capacity() const { return buffer()->m_nAllocated; }

  /** \details
    Sets the physical length of this Array object to the specified
    reserve length if the reserve length is greater than its physical length.

    \param reserveLength [in]  Minimum physical length.
  */
  void reserve(
    size_type reserveLength) { if(physicalLength() < reserveLength) { setPhysicalLength(reserveLength); } }

  /** \details
    Assigns the specified range of elements to this Array object.

    \param first [in]  Position of first element to be assigned.
    \param afterLast [in]  Position of first element after the last element to be assigned.

    \remarks
    After this Array object is cleared, this function assigns the specified range of elements from
    another Array object.
  */
  void assign(
    const_iterator first,
    const_iterator afterLast)
  { erase(begin_non_const(), end_non_const()); insert(begin_non_const(), first, afterLast); }

  /** \details
    Removes the range of elements from this Array object.

    \param first [in]  Position of first element to be assigned.
    \param afterLast [in]  Position of first element after the last element to be assigned.
  */
  iterator erase(
    iterator first,
    iterator afterLast)
  {
    size_type i = (size_type)(first - begin_const());
    if(first != afterLast)
    {
      removeSubArray(i, (size_type)(afterLast-begin_const()-1));
    }
    return begin_non_const()+i;
  }

  /** \details
    Removes the specified element from this Array object.

    \param where [in]  Element to remove.
  */
  iterator erase(
    iterator where)
  {
    size_type i = where - begin_const();
    removeAt(i);
    return begin_non_const()+i;
  }
  /** \details
    Removes all elements from this Array object.
  */
  void clear() { erase(begin_non_const(), end_non_const()); }

  /** \details
    Appends an element to the end of this Array object.
  */
  void push_back(
    const T& value) { insertAt(length(), value); }

  /** \details
    Inserts number of elements into this Array object.

    \param before [in]  Position where first element is to be inserted.
    \param numElements [in]  Number of elements to insert.
    \param value [in]  Value to insert.

    \remarks
    The range of elements may be from another Array object.
  */
  iterator insert(
    iterator before,
    size_type numElements,
    const T& value)
  {
    size_type len = length();
    size_type index = before - begin_const();
    reallocator r( m_pData > &value || &value > (m_pData + len) );
    r.reallocate(this, len + numElements);
    A::constructn(m_pData + len, numElements, value);
    buffer()->m_nLength = len + numElements;
    T* pData = data();
    pData += index;
    if(index != len)
    {
      A::move(pData + numElements, pData, len - index);
    }
    while(numElements--)
    {
      pData[numElements] = value;
    }
    return begin_non_const()+index;
  }

  /** \details
    Inserts range of elements into this Array object.

    \param before [in]  Position where first element is to be inserted.
    \param value [in]  Value to insert.

    \remarks
    The range of elements may be from another Array object.
  */
  iterator insert(
    iterator before,
    const T& value = T())
  {
    size_type index = before - begin_const();
    insertAt(index, value);
    return (begin_non_const() + index);
  }

  // ARX-like interface

  /** \details
    Returns true if and only if this Array object contains ths specified value.

    \param value [in]  Value for which to search.
    \param start [in]  Starting index of search.
  */
  bool contains(
    const T& value,
    size_type start = 0) const
  { size_type dummy; return find(value, dummy, start); }


  /** \details
    Returns the number of elements in this Array object.
  */
  size_type length() const { return buffer()->m_nLength; }

  /** \details
    Returns true if and only if this Array is empty.
  */
  bool isEmpty() const { return length() == 0; }

  /** \details
    Returns the logical length of this Array object.
  */
  size_type logicalLength() const { return length(); }

  /** \details
    Returns the physical length of this Array object.
  */
  size_type physicalLength() const { return buffer()->m_nAllocated; }

  /** \details
    Returns the grow length of this Array object.
  */
  int growLength() const { return buffer()->m_nGrowBy; }

  /** \details
    Returns the data buffer of this Array object.
  */
  const T* asArrayPtr() const { return data(); }

  /** \details
    Returns the data buffer of this Array object.
  */
  const T* getPtr() const { return data(); }

  /** \details
    Returns the data buffer of this Array object.
  */
  T* asArrayPtr() { copy_if_referenced(); return data(); }

  /** \remarks
    For convenient access to the data.
  */
  const T& operator [](
    size_type index) const { assertValid(index); return m_pData[index]; }
  T& operator [](
    size_type index) { assertValid(index); copy_if_referenced(); return m_pData[index]; }


  /** \details
    Returns the element of this Array object at the specified index.
    \param arrayIndex [in]  Array index.
  */
  T& at(
    size_type arrayIndex) { assertValid(arrayIndex); copy_if_referenced(); return *(data() + arrayIndex); }
  /** \details
    Returns the element of this Array object at the specified index.
    \param arrayIndex [in]  Array index.
  */
  const T& at(size_type arrayIndex) const { assertValid(arrayIndex); return *(data() + arrayIndex); }

  /** \details
    Sets the element of this Array object at the specified index.
    \param arrayIndex [in]  Array index.
    \param value [in]  Value.
  */
  OdArrayMemAlloc& setAt(
    size_type arrayIndex,
    const T& value)
  { assertValid(arrayIndex); copy_if_referenced(); m_pData[arrayIndex] = value; return *this; }

  /** \details
    Returns the element of this Array object at the specified position.
    \param arrayIndex [in]  Array index.
  */
  const T& getAt(
    size_type arrayIndex) const { assertValid(arrayIndex); return *(data() + arrayIndex); }

  /** \details
    Returns the first element of this Array object.
  */
  T& first() { return *begin(); }
  /** \details
    Returns the first element of this Array object.
  */
  const T& first() const { return *begin(); }

  /** \details
    Returns the last element of this Array object.
  */
  T& last() { return at(length()-1); }
  /** \details
    Returns the last element of this Array object.
  */
  const T& last() const { return at(length()-1); }

  /** \details
    Appends the specified value to the end of this Array object.

    \param value [in]  Value to append.

    \returns
    Returns the index of the new last element.
  */
  size_type append(
    const T& value) { insertAt(length(), value); return length()-1; }

  /** \details
    Returns an interator (pointer) to the first element
    after the last element in the array.
  */
  iterator append() { size_type i = append(T()); return begin_non_const() + i; }

  /** \details
    Removes the first element in this Array object.
  */
  OdArrayMemAlloc& removeFirst() { return removeAt(0); }

  /** \details
    Removes the last element in this Array object.
  */
  OdArrayMemAlloc& removeLast() { return removeAt(length()-1); }

  /** \details
    Sets the grow length of this Array object.
    \param growLength [in]  Grow length.
  */
  OdArrayMemAlloc& setGrowLength(
    int growLength)
  {
    if(growLength != 0)
    {
      copy_if_referenced();
      buffer()->m_nGrowBy = growLength;
    }
    else
    {
      ODA_FAIL();
    }
    return *this;
  }

  /** \param physicalLength [in]  Initial physical length.
    \param growLength [in]  Initial grow length.
  */
  explicit OdArrayMemAlloc(
    size_type physicalLength,
    int growLength = 8) : m_pData(0)
  {
    if(growLength != 0)
    {
      m_pData = Buffer::allocate(physicalLength, growLength)->data();
    }
    else
    {
      ODA_FAIL();
      *this = OdArrayMemAlloc<T,A>();
    }
  }

  OdArrayMemAlloc() : m_pData(Buffer::_default()->data()) { buffer()->addref(); }

  OdArrayMemAlloc(const OdArrayMemAlloc& source) : m_pData((T*)source.data()) { buffer()->addref(); }

  ~OdArrayMemAlloc() { buffer()->release(); }

  OdArrayMemAlloc& operator =(
    const OdArrayMemAlloc& source)
  {
    source.buffer()->addref();
    if (m_pData != 0)
        buffer()->release();
    m_pData = source.m_pData;
    return *this;
  }

  bool operator ==(
    const OdArrayMemAlloc& array) const
  {
    if(length() == array.length())
    {
      for(size_type i = 0; i < length(); i++)
      {
        if(at(i) != array[i])
        {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  /** \details
    Sets all the elements in this Array object to the specified value.
    \param value [in]  Value to assign.
  */
  OdArrayMemAlloc& setAll(
    const T& value)
  {
    copy_if_referenced();
    T* pData = data();
    size_type n = length();
    while(n)
    {
      pData[--n] = value;
    }
    return *this;
  }
  /** \details
    Appends the specified Array object to the end of this Array object.

    \param otherArray [in]  Array to append.

    \returns
    Returns a reference to this Array object.
  */
  OdArrayMemAlloc& append(
    const OdArrayMemAlloc& otherArray)
  {
    insert(end_non_const(), otherArray.begin(), otherArray.end());
    return *this;
  }

  /** \details
    Inserts the specified value into this Array object at the specified index.

    \param arrayIndex [in]  Array index.
    \param value [in]  Value to insert.

    \remarks
                0 <= arrayIndex <= length()

    Elements starting at arrayIndex will have their indices incremented.

    Returns a reference to this Array object.
  */
  OdArrayMemAlloc& insertAt(
    size_type arrayIndex,
    const T& value)
  {
    size_type len = length();
    if( arrayIndex == len )
    {
      resize( len + 1, value );
    }
    else if ( arrayIndex < len )
    {
      reallocator r( m_pData > &value || &value > (m_pData + len) );
      r.reallocate( this, len+1 );
      A::construct( m_pData + len );
      ++(buffer()->m_nLength);
      A::move(m_pData + arrayIndex + 1, m_pData + arrayIndex, len - arrayIndex);
      m_pData[arrayIndex] = value;
    }
    else
    {
      rise_error(eInvalidIndex);
    }
    return *this;
  }

  /** \details
    Removes the element at the specified index from this Array object.

    \param arrayIndex [in]  Array index.

    \remarks
                0 <= arrayIndex < length()

    Elements starting at arrayIndex+1 will have their indices decremented.

    Returns a reference to this Array object.
  */
  OdArrayMemAlloc& removeAt(
    size_type arrayIndex)
  {
    assertValid(arrayIndex);
    size_type len = length();
    if(arrayIndex < --len)
    {
      copy_if_referenced();
      T* pData = data();
      A::move(pData + arrayIndex, pData + arrayIndex + 1, len - arrayIndex);
    }
    resize(len);
    return *this;
  }

  /** \details
    Removes the specified elements from this Array object.

    \param startIndex [in]  Start index.
    \param endIndex [in]   End index.

    \remarks
    Elements from startIndex through endIndex inclusive will be removed.

    Returns a reference to this Array object.
  */
  OdArrayMemAlloc& removeSubArray(
    size_type startIndex,
    size_type endIndex)
  {
    if(!isValid(startIndex) || startIndex > endIndex)
    {
      rise_error(eInvalidIndex);
    }
    size_type len = length();
    copy_if_referenced();
    T* pData = data();
    ++endIndex;
    size_type n2remove = endIndex - startIndex;
    A::move(pData + startIndex, pData + endIndex, len - endIndex);
    A::destroy(pData + len - n2remove, n2remove);
    buffer()->m_nLength -= n2remove;
    return *this;
  }

  /** \details
    Returns true if and only if this Array object contains ths specified value.

    \param value [in]  Value for which to search.
    \param findIndex [out]  Receives the index of the found value.
    \param start [in]  Starting index of search.

    \returns
    Returns the index at which the element was found.
  */
  bool find(
    const T& value,
    size_type& findIndex,
    size_type start=0) const
  {
    if(!empty())
    {
      assertValid(start);
      size_type len = length();
      const T* pData = data();
      for(size_type i = start; i<len; ++i)
      {
        if(pData[i] == value)
        {
          findIndex = i;
          return true;
        }
      }
    }
    return false;
  }

  /** \details
    Sets the logical length of this Array object.
    \param logLength [in]  Logical length.
    \remarks
    The physical length is increased as required.
  */
  OdArrayMemAlloc& setLogicalLength(
    size_type logLength)
  {
    resize(logLength);
    return *this;
  }

  /** \details
    Sets the physical length of this Array object.
    \param physLength [in]  Physical length.
    \remarks
    The logical length is decreased as required.
  */
  OdArrayMemAlloc& setPhysicalLength(
    size_type physLength)
  {
    if(physLength==0)
    {
      *this = OdArrayMemAlloc<T, A>();
    }
    else if(physLength != physicalLength())
    {
      copy_buffer(physLength,true,true);
    }
    return *this;
  }

  /** \details
    Reverses the order of the elements in this Array object.
  */
  OdArrayMemAlloc& reverse()
  {
    if(!empty())
    {
      copy_if_referenced();
      T tmp;
      iterator iter1 = begin_non_const();
      iterator iter2 = end_non_const();
      --iter2;
      while(iter1 < iter2)
      {
        tmp = *iter1;
        *iter1 = *iter2;
        *iter2 = tmp;
        ++iter1;
        --iter2;
      }
    }
    return *this;
  }


  /** \details
    Swaps the specified elements in this Array object.
    \param firstIndex [in]  Index of first element.
    \param secondIndex [in]  Index of second element.
  */
  OdArrayMemAlloc& swap(
    size_type firstIndex,
    size_type secondIndex)
  {
    if(!isValid(firstIndex) || !isValid(secondIndex))
    {
      rise_error(eInvalidIndex);
    }
    if(firstIndex != secondIndex)
    {
      const T tmp = at(firstIndex);
      at(firstIndex) = at(secondIndex);
      at(secondIndex) = tmp;
    }
    return *this;
  }
  /** \details
    Removes the element with the specified value from this Array object.

    \param value [in]  Value for which to search.
    \param start [in]  Starting index of search.

    \remarks
    Removes the first occurance of value starting at start.

    Returns true if and only if an element was removed.
  */
  bool remove(
    const T& value,
    size_type start = 0)
  {
    size_type i = 0;
    if(find(value, i, start))
    {
      removeAt(i);
      return true;
    }
    return false;
  }
private:

  T*    m_pData;

  bool isValid(size_type i) const { return (i < length()); }

  T* data() { return (length() ? m_pData : 0); }

  const T* data() const { return m_pData; }

  Buffer* buffer() const
  {
    return (reinterpret_cast<Buffer*>(const_cast<OdArrayMemAlloc*>(this)->m_pData) - 1);
  }
  bool referenced() const
  {
    return (buffer()->m_nRefCounter>1);
  }
};

#include "TD_PackPop.h"

#endif // ODARRAYMEMALLOC_H_INCLUDED
