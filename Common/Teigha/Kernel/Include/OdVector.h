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

#ifndef OdVector_H_INCLUDED
#define OdVector_H_INCLUDED

#include <new>

#include "TD_PackPush.h"

#include "OdArrayMemAlloc.h"
#include "OdAlloc.h"

template <class T, class A = OdObjectsAllocator<T>, class Mm = OdrxMemoryManager> class OdVector;

/** \details
    This template class implements dynamic Array objects within Teigha.

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
template <class T, class A, class Mm> class OdVector
{
public:
  typedef typename A::size_type size_type;
  typedef T* iterator;
  typedef const T* const_iterator;
  // compatibility with OdArray
  typedef T value_type;
  typedef const T& const_reference;
  typedef T& reference;

private:
  static T* allocate(size_type physicalLength);
  
  void release();
  
  void reallocate(size_type physicalLength, bool isUseRealloc = false, bool isForcePhysicalLength = false);
  
  bool isValid(size_type index) const;
  void assertValid(size_type index) const;
  
  static void riseError(OdResult res);
  
  const_iterator begin_const() const;
  iterator begin_non_const();
  const_iterator end_const() const;
  iterator end_non_const();

public:
  /** \param physicalLength [in]  Initial physical length.
  \param growLength [in]  Initial grow length.
  */
  explicit OdVector(size_type physicalLength, int growLength = 8);
  OdVector();
  OdVector(const OdVector& vec);
  
  ~OdVector();
  OdVector& operator=(const OdVector& vec);
  
  /** \details
      Returns an iterator that references the first element in this Array object.
  */
  iterator begin();
  
  /** \details
      Returns an iterator that references the first element in this Array object.
  */
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  
  /** \details
  Inserts an element into this Array object.

  \param before [in]  Position after which the first element should be inserted.
  \param first [in]  Position of first element to be inserted.
  \param afterLast [in]  Position of first element after the last element to be inserted.

  \remarks
  The range of elements may be from another Array object.
  */
  void insert(iterator before, const_iterator first, const_iterator afterLast);

  /** \param numElem [in]  Number of elements to insert.
  \param value [in]  Value to insert.
  */
  /** \details
  Inserts a number of elements into this Array object.

  \param before [in]  Position after which the first element should be inserted.
  \param numElem [in]  Number of elements to insert.
  \param value [in]  Value to insert.

  \remarks
  The range of elements may be from another Array object.
  */
  iterator insert(iterator before, size_type numElem, const T& value);
  /** \details
  Inserts a range of elements into this Array object.

  \param before [in]  Position after which the first element should be inserted.
  \param value [in]  Value to insert.

  \remarks
  The range of elements may be from another Array object.
  */
  iterator insert(iterator before, const T& value = T());

  /** \details
  Inserts the specified value into this Array object at the specified index.

  \param index [in]  Array index.
  \param value [in]  Value to insert.

  \remarks
        0 <= index <= length()

  Elements starting at index will have their indices incremented.

  Returns a reference to this Array object.
  */
  OdVector& insertAt(size_type index, const T& value);

  /** \details
  Removes the element at the specified index from this Array object.

  \param index [in]  Array index.

  \remarks
      0 <= index < length()

  Elements starting at index+1 will have their indices decremented.

  Returns a reference to this Array object.
  */
  OdVector& removeAt(size_type index);

  /** \details
  Removes the specified elements from this Array object.

  \param startIndex [in]  Start index.
  \param endIndex [in]   End index.

  \remarks
  Elements from startIndex through endIndex inclusive will be removed.

  Returns a reference to this Array object.
  */
  OdVector& removeSubArray(size_type startIndex, size_type endIndex);

  /** \details
  Removes the element with the specified value from this Array object.

  \param value [in]  Value for which to search.
  \param startIndex [in]  Starting index of search.

  \remarks
  Removes the first occurance of value starting at startIndex.

  Returns true if and only if an element was removed.
  */
  bool remove(const T& value, size_type startIndex = 0);

  /** \details
      Specifies the logical length for this Array object.
      \param logicalLength [in]  Logical length.
      \param value [in]  Value for the elements added to obtain the new logical length.
  */
  void resize(size_type logicalLength, const T& value);
  /** \details
      Specifies the logical length for this Array object.
      \param logicalLength [in]  Logical length.
  */
  void resize(size_type logicalLength);

  /** \details
  Returns the logical length of this Array object.
  */
  size_type size() const;

  /** \details
  Returns true if and only if this Array is empty.
  */
  bool empty() const;

  /** \details
  Returns the physical length of this Array object.
  */
  size_type capacity() const;

  /** \details
  Sets the physical length of this Array object to the specified
  reserve length if the reserve length is greater than its physical length.

  \param reserveLength [in]  Minimum physical length.
  */
  void reserve(size_type reserveLength);

  /** \details
  Assigns the specified range of elements to this Array object.

  \param first [in]  Position of first element to be assigned.
  \param afterLast [in]  Position of first element after the last element to be assigned.

  \remarks
  After this Array object is cleared, this function assigns the specified range of elements from
  another Array object.
  */
  void assign(const_iterator first, const_iterator afterLast);

  /** \details
      Removes the range of elements from this Array object.

      \param first [in]  Position of first element to be assigned.
      \param afterLast [in]  Position of first element after the last element to be assigned.
  */
  iterator erase(iterator first, iterator afterLast);

  /** \details
      Removes the specified element from this Array object.

      where [in]  Element to remove.
  */
  iterator erase(iterator where);

  /** \details
  Removes all elements from this Array object.
  */
  void clear();

  /** \details
  Appends an element to the end of this Array object.
  */
  void push_back(const T& value);

  /** \details
  Returns true if and only if this Array object contains ths specified value.

  \param value [in]  Value for which to search.
  \param startIndex [in]  Starting index of search.
  */
  bool contains(const T& value, size_type startIndex = 0) const;

  /** \details
  Returns the number of elements in this Array object.
  */
  size_type length() const;

  /** \details
  Returns true if and only if this Array is empty.
  */
  bool isEmpty() const;

  /** \details
  Returns the logical length of this Array object.
  */
  size_type logicalLength() const;

  /** \details
  Returns the physical length of this Array object.
  */
  size_type physicalLength() const;

  /** \details
  Returns the grow length of this Array object.
  */
  int growLength() const;

  /** \details
  Returns the data buffer of this Array object.
  */
  const T* asArrayPtr() const;

  /** \details
  Returns the data buffer of this Array object.
  */
  const T* getPtr() const;

  /** \details
      Returns the data buffer of this Array object.
  */
  T* asArrayPtr();

  /** \details
      For convenient access to the data.
  */
  const T& operator[](size_type index) const;
  /** \details
      For convenient access to the data.
  */
  T& operator[](size_type index);

  /** \details
      Returns the element of this Array object at the specified index.
      \param arrayIndex [in]  Array index.
  */
  T& at(size_type index);
  /** \details
      Returns the element of this Array object at the specified index.
      \param arrayIndex [in]  Array index.
  */
  const T& at(size_type index) const;

  /** \details
  Sets the element of this Array object at the specified index.
  \param arrayIndex [in]  Array index.
  \param value [in]  Value.
  */
  OdVector& setAt(size_type index, const T& value);

  /** \details
  Returns the element of this Array object at the specified position.
  \param arrayIndex [in]  Array index.
  */
  const T& getAt(size_type index) const;

  /** \details
      Returns the first element of this Array object.
  */
  T& first();
  /** \details
      Returns the first element of this Array object.
  */
  const T& first() const;

  /** \details
      Returns the last element of this Array object.
  */
  T& last();
  /** \details
      Returns the last element of this Array object.
  */
  const T& last() const;

  /** \details
  Appends the specified value to the end of this Array object.

  \param value [in]  Value to append.

  \remarks
  Returns the index of the new last element.
  */
  size_type append(const T& value);
  
  /** \details
  Returns an interator (pointer) to the first element
  after the last element in the array.
  */
  iterator append();

  /** \details
  Appends the Array object to the end of this Array object.

  \param vec [in]  Array to append.

  \remarks
  Returns a reference to this Array object.
  */
  OdVector& append(const OdVector& vec);

  /** \details
  Removes the first element in this Array object.
  */
  OdVector& removeFirst();

  /** \details
  Removes the last element in this Array object.
  */
  OdVector& removeLast();

  bool operator==(const OdVector& vec) const;

  /** \details
  Sets all the elements in this Array object to the specified value.
  \param value [in]  Value to assign.
  */
  OdVector& setAll(const T& value);

  /** \details
  Returns true if and only if this Array object contains ths specified value.

  \param value [in]  Value for which to search.
  \param index [out]  Receives the index of the found value.
  \param startIndex [in]  Starting index of search.

  \returns
  Returns the index at which the element was found.
  */
  bool find(const T& value, size_type& index, size_type startIndex = 0) const;

  /** \details
  Sets the logical length of this Array object.
  \param logicalLength [in]  Logical length.
  \remarks
  The physical length is increased as required.
  */
  OdVector& setLogicalLength(size_type logicalLength);

  /** \details
  Sets the physical length of this Array object.
  \param physicalLength [in]  Physical length.
  \remarks
  The logical length is decreased as required.
  */
  OdVector& setPhysicalLength(size_type physicalLength);

  /** \details
  Sets the grow length of this Array object.
  \param growLength [in]  Grow length.
  */
  OdVector& setGrowLength(int growLength);

  /** \details
  Reverses the order of the elements in this Array object.
  */
  OdVector& reverse();

  /** \details
  Swaps the specified elements in this Array object.
  \param firstIndex [in]  Index of first element.
  \param secondIndex [in]  Index of second element.
  */
  OdVector& swap(size_type firstIndex, size_type secondIndex);

private:
  T* m_pData;
  size_type m_physicalLength;
  size_type m_logicalLength;
  int m_growLength;
};


#define VEC_SIZE_TYPE typename OdVector<T, A, Mm>::size_type
#define VEC_ITERATOR typename OdVector<T, A, Mm>::iterator
#define VEC_CONST_ITERATOR typename OdVector<T, A, Mm>::const_iterator


template<class T, class A, class Mm>
inline OdVector<T, A, Mm>::OdVector(VEC_SIZE_TYPE physicalLength, int growLength)
: m_pData(NULL), m_physicalLength(physicalLength), m_logicalLength(0)
, m_growLength(growLength)
{
  if(m_growLength == 0)
  {
    ODA_FAIL();
    m_growLength = -200;
  }
  if (m_physicalLength)
  {
    m_pData = allocate(m_physicalLength);
  }
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>::OdVector()
: m_pData(NULL), m_physicalLength(0), m_logicalLength(0), m_growLength(-200)
{
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>::OdVector(const OdVector<T, A, Mm>& vec)
: m_pData(NULL), m_physicalLength(vec.m_physicalLength)
, m_logicalLength(vec.m_logicalLength), m_growLength(vec.m_growLength)
{
  if(m_physicalLength > 0)
  {
    m_pData = allocate(m_physicalLength);

    A::copy(m_pData, vec.m_pData, m_logicalLength);
  }
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>::~OdVector()
{
  release();
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::operator=(const OdVector<T, A, Mm>& vec)
{
  if(this != &vec)
  {
    if(m_physicalLength < vec.m_logicalLength)
    {
      release();
      
      m_pData = allocate(vec.m_logicalLength);
      m_physicalLength = vec.m_logicalLength;
    }
    
    m_logicalLength = vec.m_logicalLength;
    
    A::copy(m_pData, vec.m_pData, m_logicalLength);
  }
  return *this;
}

template <class T, class A, class Mm>
inline T* OdVector<T, A, Mm>::allocate(VEC_SIZE_TYPE physicalLength)
{
  ODA_ASSERT(physicalLength != 0);
  const VEC_SIZE_TYPE  numByte = physicalLength*sizeof(T);

  ODA_ASSERT(numByte >= physicalLength); // size_type overflow

  T* pData = ((numByte >= physicalLength)
    ? reinterpret_cast<T*>(Mm::Alloc(numByte))
    : NULL);

  if(pData == NULL)
    throw OdError(eOutOfMemory);

  return pData;
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::release()
{
  if(m_pData != NULL)
  {
    A::destroy(m_pData, m_logicalLength);

    Mm::Free(m_pData);
    m_pData = NULL;
    m_physicalLength = 0;
  }
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::reallocate(VEC_SIZE_TYPE physicalLength, bool isUseRealloc, bool isForcePhysicalLength)
{
  T* pOldData = m_pData;
  VEC_SIZE_TYPE newPhysicalLength = physicalLength;

  if(!isForcePhysicalLength)
  {
    if(m_growLength > 0)
    {
      newPhysicalLength = ((newPhysicalLength + m_growLength - 1)/m_growLength) * m_growLength;
    }
    else
    {
      newPhysicalLength = m_logicalLength + (-m_growLength)*m_logicalLength/100;

      if(newPhysicalLength < physicalLength)
        newPhysicalLength = physicalLength;
    }
  }
  
  if(isUseRealloc && A::useRealloc() && m_logicalLength > 0 && m_pData != NULL)
  {
    m_pData = reinterpret_cast<T*>(Mm::Realloc(pOldData, newPhysicalLength*sizeof(T), m_physicalLength*sizeof(T)));

    if (!m_pData)
      throw OdError(eOutOfMemory);

    m_physicalLength = newPhysicalLength;

    if(physicalLength < m_logicalLength)
      m_logicalLength = physicalLength;
  }
  else
  {
    T* pNewData = allocate(newPhysicalLength);
    const VEC_SIZE_TYPE newLogicalLength = odmin(m_logicalLength, physicalLength);

    A::constructn(pNewData, pOldData, newLogicalLength);
    
    release();

    m_pData = pNewData;
    m_physicalLength = newPhysicalLength;
    m_logicalLength = newLogicalLength;
  }
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::isValid(VEC_SIZE_TYPE index) const
{
  // index is unsigned here, no need >= 0 check
  return (index < m_logicalLength);
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::assertValid(VEC_SIZE_TYPE index) const
{
  if(!isValid(index))
  {
    ODA_FAIL();
    throw OdError_InvalidIndex();
  }
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::riseError(OdResult res)
{
  ODA_FAIL();
  throw OdError(res);
}

template <class T, class A, class Mm>
inline VEC_CONST_ITERATOR OdVector<T, A, Mm>::begin_const() const
{
  return begin();
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::begin_non_const()
{
  return begin();
}

template <class T, class A, class Mm>
inline VEC_CONST_ITERATOR OdVector<T, A, Mm>::end_const() const
{
  return end();
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::end_non_const()
{
  return end();
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::begin()
{
  return (!isEmpty() ? m_pData : NULL);
}

template <class T, class A, class Mm>
inline VEC_CONST_ITERATOR OdVector<T, A, Mm>::begin() const
{
  return (!isEmpty() ? m_pData : NULL);
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::end()
{
  return (!isEmpty() ? m_pData + m_logicalLength : NULL);
}

template <class T, class A, class Mm>
inline VEC_CONST_ITERATOR OdVector<T, A, Mm>::end() const
{
  return (!isEmpty() ? m_pData + m_logicalLength : NULL);
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::insert(VEC_ITERATOR before, VEC_CONST_ITERATOR first, VEC_CONST_ITERATOR afterLast)
{
  const VEC_SIZE_TYPE oldLogicalLength = m_logicalLength;
  const VEC_SIZE_TYPE index = (VEC_SIZE_TYPE)(before - begin_const());
  
  if(index <= m_logicalLength && afterLast >= first)
  {
    if(afterLast > first)
    {
      const VEC_SIZE_TYPE numElem = (VEC_SIZE_TYPE)(afterLast - first);
      const VEC_SIZE_TYPE newLogicalLength = oldLogicalLength + numElem;
      
      if(newLogicalLength > m_physicalLength)
        reallocate(newLogicalLength, first < begin() || first >= end());
      
      A::constructn(m_pData + oldLogicalLength, first, numElem);
      
      m_logicalLength = newLogicalLength;
      
      T* pData = m_pData + index;
      
      if(index != oldLogicalLength)
        A::move(pData + numElem, pData, oldLogicalLength - index);
      
      A::copy(pData, first, numElem);
    }
  }
  else
    riseError(eInvalidInput);
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::insert(VEC_ITERATOR before, VEC_SIZE_TYPE numElem, const T& value)
{
  const VEC_SIZE_TYPE oldLogicalLength = m_logicalLength;
  const VEC_SIZE_TYPE newLogicalLength = oldLogicalLength + numElem;
  const VEC_SIZE_TYPE index = (VEC_SIZE_TYPE)(before - begin_const());
  
  if(newLogicalLength > m_physicalLength)
    reallocate(newLogicalLength, &value < begin() || &value >= end());
  
  A::constructn(m_pData + oldLogicalLength, numElem, value);
  
  m_logicalLength = newLogicalLength;
  
  T* pData = m_pData + index;
  
  if(index != oldLogicalLength)
    A::move(pData + numElem, pData, oldLogicalLength - index);
  
  while(numElem--)
    pData[numElem] = value;
  
  return (begin_non_const() + index);
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::insert(VEC_ITERATOR before, const T& value)
{
  const VEC_SIZE_TYPE index = (VEC_SIZE_TYPE)(before - begin_const());
  
  insertAt(index, value);
  
  return (begin_non_const() + index);
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::insertAt(VEC_SIZE_TYPE index, const T& value)
{
  const VEC_SIZE_TYPE oldLogicalLength = m_logicalLength;
  const VEC_SIZE_TYPE newLogicalLength = oldLogicalLength + 1;
  
  if(index == oldLogicalLength)
    resize(newLogicalLength, value);
  else if(index < oldLogicalLength)
  {
    if(newLogicalLength > m_physicalLength)
      reallocate(newLogicalLength, &value < begin() || &value >= end());
    
    A::construct(m_pData + oldLogicalLength);
    
    ++m_logicalLength;
    
    T* pData = m_pData + index;
    
    A::move(pData + 1, pData, oldLogicalLength - index);
    
    m_pData[index] = value;
  }
  else
    riseError(eInvalidIndex);
  
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::removeAt(VEC_SIZE_TYPE index)
{
  assertValid(index);
  
  const VEC_SIZE_TYPE newLogicalLength = m_logicalLength - 1;
  
  if(index < newLogicalLength)
  {
    T* pData = m_pData + index;
    A::move(pData, pData + 1, newLogicalLength - index);
  }
  
  resize(newLogicalLength);
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::removeSubArray(VEC_SIZE_TYPE startIndex, VEC_SIZE_TYPE endIndex)
{
  if(!isValid(startIndex) || startIndex > endIndex)
    riseError(eInvalidIndex);
  
  const VEC_SIZE_TYPE oldLogicalLength = m_logicalLength;
  
  T* pData = m_pData;
  
  ++endIndex;
  
  const VEC_SIZE_TYPE numElem = endIndex - startIndex;
  
  A::move(pData + startIndex, pData + endIndex, oldLogicalLength - endIndex);
  A::destroy(pData + oldLogicalLength - numElem, numElem);
  
  m_logicalLength -= numElem;  
  return *this;
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::remove(const T& value, VEC_SIZE_TYPE startIndex)
{
  VEC_SIZE_TYPE index = 0;
  if(find(value, index, startIndex))
  {
    removeAt(index);
    return true;
  }
  return false;
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::resize(VEC_SIZE_TYPE logicalLength, const T& value)
{
  const VEC_SIZE_TYPE oldLogicalLength = m_logicalLength;
  const int lengthDiff = logicalLength - oldLogicalLength;

  if(lengthDiff > 0)
  {
    if(logicalLength > m_physicalLength)
      reallocate(logicalLength, &value < begin() || &value >= end());

    A::constructn(m_pData + oldLogicalLength, lengthDiff, value);
  }
  else if(lengthDiff < 0)
    A::destroy(m_pData + logicalLength, -lengthDiff);

  m_logicalLength = logicalLength;
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::resize(VEC_SIZE_TYPE logicalLength)
{
  const VEC_SIZE_TYPE oldLogicalLength = m_logicalLength;
  const int lengthDiff = logicalLength - oldLogicalLength;

  if(lengthDiff > 0)
  {
    if(logicalLength > m_physicalLength)
      reallocate(logicalLength, true);
    A::constructn(m_pData + oldLogicalLength, lengthDiff);
  }
  else if(lengthDiff < 0)
    A::destroy(m_pData + logicalLength, -lengthDiff);

  m_logicalLength = logicalLength;
}

template <class T, class A, class Mm>
inline VEC_SIZE_TYPE OdVector<T, A, Mm>::size() const
{
  return m_logicalLength;
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::empty() const
{
  return (m_logicalLength == 0);
}

template <class T, class A, class Mm>
inline VEC_SIZE_TYPE OdVector<T, A, Mm>::capacity() const
{
  return m_physicalLength;
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::reserve(VEC_SIZE_TYPE reserveLength)
{
  if(m_physicalLength < reserveLength)
    setPhysicalLength(reserveLength);
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::assign(VEC_CONST_ITERATOR first, VEC_CONST_ITERATOR afterLast)
{
  erase(begin_non_const(), end_non_const());
  
  insert(begin_non_const(), first, afterLast);
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::erase(VEC_ITERATOR first, VEC_ITERATOR afterLast)
{
  const VEC_SIZE_TYPE index = (VEC_SIZE_TYPE)(first - begin_const());
	
  if(first != afterLast)
    removeSubArray(index, (VEC_SIZE_TYPE)(afterLast - begin_const() - 1));
	
  return (begin_non_const() + index);
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::erase(VEC_ITERATOR it)
{
  const VEC_SIZE_TYPE index = (VEC_SIZE_TYPE)(it - begin_const());
	
  removeAt(index);

  return (begin_non_const() + index);
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::clear()
{
  erase(begin_non_const(), end_non_const());
}

template <class T, class A, class Mm>
inline void OdVector<T, A, Mm>::push_back(const T& value)
{
  insertAt(m_logicalLength, value);
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::contains(const T& value, VEC_SIZE_TYPE startIndex) const
{
  VEC_SIZE_TYPE index;

  return find(value, index, startIndex);
}

template <class T, class A, class Mm>
inline VEC_SIZE_TYPE OdVector<T, A, Mm>::length() const
{
  return m_logicalLength;
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::isEmpty() const
{
  return (m_logicalLength == 0);
}

template <class T, class A, class Mm>
inline VEC_SIZE_TYPE OdVector<T, A, Mm>::logicalLength() const
{
  return m_logicalLength;
}

template <class T, class A, class Mm>
inline VEC_SIZE_TYPE OdVector<T, A, Mm>::physicalLength() const
{
  return m_physicalLength;
}

template <class T, class A, class Mm>
inline int OdVector<T, A, Mm>::growLength() const
{
  return m_growLength;
}

template <class T, class A, class Mm>
inline const T* OdVector<T, A, Mm>::asArrayPtr() const
{
  return m_pData;
}

template <class T, class A, class Mm>
inline const T* OdVector<T, A, Mm>::getPtr() const
{
  return m_pData;
}

template <class T, class A, class Mm>
inline T* OdVector<T, A, Mm>::asArrayPtr()
{ // OdArray::asArrayPtr invokes non-const version of data() method which checks length and return null.
  // Constant version of asArrayPtr and getPtr will return m_pData as is for OdArray too.
  return (length()) ? m_pData : NULL;
}

template <class T, class A, class Mm>
inline const T& OdVector<T, A, Mm>::operator[](VEC_SIZE_TYPE index) const
{
  assertValid(index);
  return m_pData[index];
}

template <class T, class A, class Mm>
inline T& OdVector<T, A, Mm>::operator[](VEC_SIZE_TYPE index)
{
  assertValid(index);
  return m_pData[index];
}

template <class T, class A, class Mm>
inline T& OdVector<T, A, Mm>::at(VEC_SIZE_TYPE index)
{
  assertValid(index);
  return m_pData[index];
}

template <class T, class A, class Mm>
inline const T& OdVector<T, A, Mm>::at(VEC_SIZE_TYPE index) const
{
  assertValid(index);
  return m_pData[index];
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::setAt(VEC_SIZE_TYPE index, const T& value)
{
  assertValid(index);
  
  m_pData[index] = value;
  
  return *this;
}

template <class T, class A, class Mm>
inline const T& OdVector<T, A, Mm>::getAt(VEC_SIZE_TYPE index) const
{
  assertValid(index);
  return m_pData[index];
}

template <class T, class A, class Mm>
inline T& OdVector<T, A, Mm>::first()
{
  return m_pData[0];
}

template <class T, class A, class Mm>
inline const T& OdVector<T, A, Mm>::first() const
{
  return m_pData[0];
}

template <class T, class A, class Mm>
inline T& OdVector<T, A, Mm>::last()
{
  return m_pData[m_logicalLength - 1];
}

template <class T, class A, class Mm>
inline const T& OdVector<T, A, Mm>::last() const
{
  return m_pData[m_logicalLength - 1];
}

template <class T, class A, class Mm>
inline VEC_SIZE_TYPE OdVector<T, A, Mm>::append(const T& value)
{
  insertAt(m_logicalLength, value);
  return (m_logicalLength - 1);
}

template <class T, class A, class Mm>
inline VEC_ITERATOR OdVector<T, A, Mm>::append()
{
  const VEC_SIZE_TYPE index = append(T());
  return (begin_non_const() + index);
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::append(const OdVector<T, A, Mm>& vec)
{
  insert(end_non_const(), vec.begin(), vec.end());
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::removeFirst()
{
  return removeAt(0);
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::removeLast()
{
  return removeAt(m_logicalLength - 1);
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::operator==(const OdVector<T, A, Mm>& vec) const
{
  if(m_logicalLength == vec.m_logicalLength)
  {
    for(VEC_SIZE_TYPE i = 0; i < m_logicalLength; ++i)
    {
      if(m_pData[i] != vec.m_pData[i])
        return false;
    }
    return true;
  }
  return false;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::setAll(const T& value)
{
  for(VEC_SIZE_TYPE i = 0; i < m_logicalLength; ++i)
    m_pData[i] = value;
  
  return *this;
}

template <class T, class A, class Mm>
inline bool OdVector<T, A, Mm>::find(const T& value, VEC_SIZE_TYPE& index, VEC_SIZE_TYPE startIndex) const
{
  if(!isEmpty())
  {
    assertValid(startIndex);
    
    for(VEC_SIZE_TYPE i = startIndex; i < m_logicalLength; ++i)
    {
      if(m_pData[i] == value)
      {
        index = i;
        return true;
      }
    }
  }
  return false;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::setLogicalLength(VEC_SIZE_TYPE logicalLength)
{
  resize(logicalLength);
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::setPhysicalLength(VEC_SIZE_TYPE physicalLength)
{
  if(physicalLength == 0)
  {
    release();
    
    m_pData = NULL;
    m_physicalLength = 0;
  }
  else if(physicalLength != m_physicalLength)
    reallocate(physicalLength, true, true);
  
  if(m_physicalLength < m_logicalLength)
    m_logicalLength = m_physicalLength;
  
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::setGrowLength(int growLength)
{
  if(growLength != 0)
    m_growLength = growLength;
  else
    ODA_FAIL();
  
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::reverse()
{
  if(!isEmpty())
  {
    T value;
    VEC_ITERATOR it1 = begin_non_const();
    VEC_ITERATOR it2 = end_non_const();
    
    --it2;
    
    while(it1 < it2)
    {
      value = *it1;
      *it1 = *it2;
      *it2 = value;
      
      ++it1;
      --it2;
    }
  }  
  return *this;
}

template <class T, class A, class Mm>
inline OdVector<T, A, Mm>& OdVector<T, A, Mm>::swap(VEC_SIZE_TYPE firstIndex, VEC_SIZE_TYPE secondIndex)
{
  if(!isValid(firstIndex) || !isValid(secondIndex))
    riseError(eInvalidIndex);
  
  if(firstIndex != secondIndex)
  {
    const T value = m_pData[firstIndex];
    m_pData[firstIndex] = m_pData[secondIndex];
    m_pData[secondIndex] = value;
  }
  
  return *this;
}


#include "TD_PackPop.h"

typedef OdVector<int, OdMemoryAllocator<int> > OdIntVector;
typedef OdVector<OdUInt32, OdMemoryAllocator<OdUInt32> > OdUInt32Vector;
typedef OdVector<OdInt32, OdMemoryAllocator<OdInt32> > OdInt32Vector;
typedef OdVector<OdUInt8, OdMemoryAllocator<OdUInt8> > OdUInt8Vector;
typedef OdVector<OdUInt64, OdMemoryAllocator<OdUInt64> > OdUInt64Vector;
#ifdef OD_GEPNT3D_H
typedef OdVector<OdGePoint3d, OdMemoryAllocator<OdGePoint3d> > OdGePoint3dVector;
#endif
#ifdef OD_GEVEC3D_H
typedef OdVector<OdGeVector3d, OdMemoryAllocator<OdGeVector3d> > OdGeVector3dVector;
#endif

#endif // OdVector_H_INCLUDED
