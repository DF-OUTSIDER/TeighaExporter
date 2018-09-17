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

#ifndef __CHUNK_ALLOCATOR__
#define __CHUNK_ALLOCATOR__

#include "RootExport.h"
#include "RxObjectImpl.h"

#include "TD_PackPush.h"

/** \details
    <group Other_Classes> 
*/
class FIRSTDLL_EXPORT IAllocator
{
public:
    virtual void* alloc(int) = 0;
    virtual void release(void* p) = 0;
    virtual ~IAllocator() {}

    IAllocator();
#ifdef _DEBUG
    virtual int numChunks() = 0;
#endif
};

/** \details
    <group Other_Classes> 
*/
class FIRSTDLL_EXPORT AllocatorArray
{
public:
    //'mask' is a bit flags value, each bit represents 
    // enhancedForFixedSize flag per item.
    // If size > sizeof(mask) then the rest items have the flag '0'
    AllocatorArray(unsigned size, int mask);
    ~AllocatorArray();

    unsigned size() const { return m_size; }
    IAllocator* getAt(unsigned i)
    {
        ODA_ASSERT(i < m_size);
        return m_data[i];
    }

protected:
    IAllocator** m_data;
    unsigned m_size;
};

FIRSTDLL_EXPORT void addLocalHeaps(unsigned nThreadId, const unsigned* aThreadId);
FIRSTDLL_EXPORT void releaseLocalHeaps(unsigned nThreadId, const unsigned* aThreadId);

// Prevent MFC memleaks displaying

/** \details
    <group Other_Classes> 
*/
class AllocatorArrayML : public AllocatorArray
{
  protected:
    bool m_bAllocated;
    unsigned m_origSize;
    int m_origMask;

    void clear()
    {
      m_bAllocated = false;
      m_data = NULL;
      m_size = 0;
    }
  public:
    AllocatorArrayML(unsigned size, int mask)
      : AllocatorArray(size, mask)
      , m_bAllocated(true), m_origSize(size), m_origMask(mask)
    { }
    ~AllocatorArrayML() { }
    void reset()
    {
      if (!m_bAllocated)
      {
        AllocatorArrayML tmp(m_origSize, m_origMask);
        *this = tmp;
        tmp.clear();
      }
    }
    void uninit()
    {
      if (m_bAllocated) {
        this->~AllocatorArrayML();
        clear();
      }
    }
};

// Some simplifiers for allocator specification

#define ODCA_HEAP_OPERATORS() \
  void* operator new(size_t); \
  void  operator delete(void*)

#define ODCA_HEAP_ALLOCATOR() \
  static AllocatorArrayML s_aAlloc

#define ODCA_HEAP_OPERATORS_DEFINE(baseClass, num) \
  void* baseClass::operator new(size_t size) \
  { \
    return s_aAlloc.getAt(num)->alloc((int)size); \
  } \
  void baseClass::operator delete(void* p) \
  { \
    s_aAlloc.getAt(num)->release(p); \
  }

#define ODCA_HEAP_OPERATORS_DEFINE_0(baseClass) \
  ODCA_HEAP_OPERATORS_DEFINE(baseClass, 0)

#define ODCA_HEAP_ALLOCATOR_DEFINE(baseClass, numAllocs, allocMask) \
  AllocatorArrayML baseClass::s_aAlloc(numAllocs, allocMask)

#define ODCA_HEAP_ALLOCATOR_DEFINE_1(baseClass) \
  ODCA_HEAP_ALLOCATOR_DEFINE(baseClass, 1, 0)
#define ODCA_HEAP_ALLOCATOR_DEFINE_1_ENH(baseClass) \
  ODCA_HEAP_ALLOCATOR_DEFINE(baseClass, 1, 1)

#define ODCA_HEAP_ALLOCATOR_RESET(baseClass) \
  baseClass::s_aAlloc.reset()
#define ODCA_HEAP_ALLOCATOR_UNINIT(baseClass) \
  baseClass::s_aAlloc.uninit()

/** \details
    <group Other_Classes> 
*/
template<class T, class TInterface = T>
class OdGiRxObjectReImpl : public OdRxObjectImpl<T, TInterface>
{
  /** \details
      Assignment operator prohibited.
  */
  OdGiRxObjectReImpl& operator = (const OdGiRxObjectReImpl&) { return *this; }

  protected:
    void* operator new (size_t size) { return T::operator new (size); }
    void  operator delete (void* p) { T::operator delete (p); }

  public:
    /** \details
        Creates an instance of OdGiRxObjectReImpl<T, TInterface>
        and returns SmartPointer to it.
    */
    static OdSmartPtr<TInterface> createObject()
    {
      return OdSmartPtr<TInterface>(static_cast<TInterface*>(new OdGiRxObjectReImpl<T, TInterface>), kOdRxObjAttach);
    }
};

#define GIRXREIMPL_CONSTR(ClassName) OdGiRxObjectReImpl<ClassName>::createObject()

#include "TD_PackPop.h"

#endif // __CHUNK_ALLOCATOR__
