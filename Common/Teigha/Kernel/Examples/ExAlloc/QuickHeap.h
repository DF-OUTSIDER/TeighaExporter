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

#ifndef _QuickHeap_h_Included
#define _QuickHeap_h_Included

#ifdef _MSC_VER
#define TD_CDECL __cdecl
#else
#define TD_CDECL
#endif

#define QUICK_HEAP_SYNCHRONIZATION

#include <memory.h>
#include <string.h>  // for memcpy on some platforms

#ifdef QUICK_HEAP_SYNCHRONIZATION
#include "QhMutex.h"
#endif

namespace OdQuickHeapNS
{
  const int ciQHInitPoolSizeDefault = 0x1000;	// Initial pool size (in bytes)
  const int ciQHInitPoolArraySizeDefault = 1016; // Largest block size, allocated on this heap
  const int GRANULARITY = 16; // heap granularity - requested block size is rounded up to granularity multiple

#ifdef QUICK_HEAP_SYNCHRONIZATION
  struct QHAutoLock
  {
    QhMutex &m_cs;
    QHAutoLock(QhMutex &cs) : m_cs(cs){ cs.lock(); }
    ~QHAutoLock(){ m_cs.unlock(); }
  private: // to remove warning
    QHAutoLock& operator=(const QHAutoLock&){ return *this; }
  };
#endif

  inline void * QuickHeapInternalAlloc(size_t size)
  {
    return ::malloc(size);
  }
  inline void * QuickHeapInternalReAlloc(void * p, size_t size)
  {
    return ::realloc(p,size);
  }
  inline void QuickHeapInternalFree(void * pBlock)
  {
    ::free(pBlock);
  }
  struct QHBlock
  {
    union
    {
      QHBlock * m_pNext;			// Next free block
      void * m_pQuickHeapPool;	// Pool address (if not free)
      char _padding[8]; // ensures proper alignment on RISC processors
    };
  protected:
    inline void * TD_CDECL operator new(size_t) throw() { return 0; }
    inline void   TD_CDECL operator delete(void*){}
  };

  struct QHExtent
  {
    QHExtent * m_pNextExtent;
    void* _padding; // ensures proper alignment on RISC processors
    inline QHBlock* blockStart()
    {
      return (QHBlock*)(this+1);
    }
  protected:
    inline void * TD_CDECL operator new(size_t) throw() { return 0; }
    inline void   TD_CDECL operator delete(void*){  }
  };

  class QuickHeapPool
  {
  public:
    inline void * TD_CDECL operator new(size_t n)
    { return QuickHeapInternalAlloc(n); }
    inline void   TD_CDECL operator delete(void* p)
    { QuickHeapInternalFree(p); }
    void * Alloc()
    {
      QHBlock * p = AllocBlock();
      return ++p;
    }
    void Free(void * p)
    {
      FreeBlock((QHBlock*)((char*)p - sizeof(QHBlock*)));
    }
    QuickHeapPool(size_t BlockSize, int iInitBlockCnt = 0) 
      : m_CurrExtent(0),m_pCurrentBlock(0),m_pFreeBlocksList(0)
    {
      m_iIntrBlockSize = sizeof(QHBlock) + BlockSize;
      if(iInitBlockCnt > 0)
        AddExtent(iInitBlockCnt);
    }
    ~QuickHeapPool()
    {
      QHExtent * pExtent = m_CurrExtent;
      while(pExtent)
      {
        QHExtent * pNext = pExtent->m_pNextExtent;
        QuickHeapInternalFree(pExtent);
        pExtent = pNext;
      }
    }
    inline QHBlock * AllocBlock()
    {
      if(m_pFreeBlocksList)
      {
        register QHBlock * pBlock = m_pFreeBlocksList;
        m_pFreeBlocksList = m_pFreeBlocksList->m_pNext;
        pBlock->m_pQuickHeapPool = this;
        return pBlock;
      }
      else
        return GetBlock();
    }
    inline void FreeBlock(QHBlock * pBlock)
    {
      pBlock->m_pNext = m_pFreeBlocksList;
      m_pFreeBlocksList = pBlock;
    }
    inline size_t GetBlockSize()
    {	// Real block size is m_iIntrBlockSize - sizeof(QHBlock)
      return m_iIntrBlockSize;
    }
  protected:
    void AddExtent(size_t iBlockCnt)
    {
      QHExtent * pExtent = (QHExtent*)QuickHeapInternalAlloc(
        sizeof(QHExtent) + (iBlockCnt/* + 1*/) * m_iIntrBlockSize);
      pExtent->m_pNextExtent = m_CurrExtent;

      m_pCurrentBlock = (QHBlock*)( (char*)pExtent->blockStart()
        + (iBlockCnt - 1) * m_iIntrBlockSize); 

      m_CurrExtent = pExtent;
      m_iCurrExtentSize = iBlockCnt;
    }
    inline QHBlock * GetBlock()
    {
      if ( m_pCurrentBlock == m_CurrExtent->blockStart() )
        AddExtent(m_iCurrExtentSize);
      QHBlock * pRet = m_pCurrentBlock;
      pRet->m_pQuickHeapPool = this;
      (char*&)m_pCurrentBlock -= m_iIntrBlockSize;
      return pRet;
    }

    size_t m_iCurrExtentSize;
    size_t m_iIntrBlockSize;
    QHExtent * m_CurrExtent;
    QHBlock * m_pCurrentBlock;
    QHBlock * m_pFreeBlocksList;
    void* _padding; // ensures proper alignment on RISC processors
  };

  class QuickHeap
  {
  public:
    inline void * TD_CDECL operator new(size_t n){ return QuickHeapInternalAlloc(n); }
    inline void TD_CDECL operator delete(void* p){ QuickHeapInternalFree(p); }

    QuickHeap()
    {
      PoolSizeLoad();
      const int iCountByte = (m_iQHInitPoolArraySize / GRANULARITY + 1) * sizeof(QuickHeapPool*) ;
      m_arypQHPool = (QuickHeapPool **) QuickHeapInternalAlloc(iCountByte);
      memset(m_arypQHPool, 0, iCountByte);
    }

    ~QuickHeap()
    {
      for ( int i = 0; i < (m_iQHInitPoolArraySize/GRANULARITY + 1); i++ )
        delete m_arypQHPool[i];
      QuickHeapInternalFree(m_arypQHPool);
    }
    inline void * Alloc(size_t cb)
    {
      const int iMaxHeapSize = m_iQHInitPoolArraySize;
      if(cb >= (size_t)iMaxHeapSize)
      {
        QHBlock * pBlock = (QHBlock*)QuickHeapInternalAlloc(
          sizeof(QHBlock) + cb);
        pBlock->m_pQuickHeapPool = (QuickHeapPool*)-1;
        ++pBlock;
        return pBlock;

      }
      else if(0 == cb)
        return 0;
      else
      {
        size_t index = ((cb + (GRANULARITY-1))/GRANULARITY);
        QuickHeapPool * pCurrPool = m_arypQHPool[index-1];
#ifdef QUICK_HEAP_SYNCHRONIZATION
        QHAutoLock qhal(m_cs);
#endif
        if(!pCurrPool)
          pCurrPool = m_arypQHPool[index-1] = 
          QuickHeapPoolNew(index*GRANULARITY, m_iQHInitPoolSize / (int)(index*GRANULARITY));
        return pCurrPool->Alloc();
      }
    }
    inline void Free(void * p)
    {
      if (p == 0)
        return;

      QHBlock * pBlock = (QHBlock *)p;
      pBlock--;
      QuickHeapPool* pQuickHeapPool = (QuickHeapPool*)pBlock->m_pQuickHeapPool;
      if((QuickHeapPool*)-1 == pQuickHeapPool)
        QuickHeapInternalFree(pBlock);
      else
      {
#ifdef QUICK_HEAP_SYNCHRONIZATION
        QHAutoLock qhal(m_cs);
#endif
        pQuickHeapPool->FreeBlock(pBlock);
      }
    }
    inline void * ReAlloc(void * p, size_t size )
    {
      if (p)
      {
        QHBlock * pBlock = (QHBlock *)p;
        pBlock--;
        if ( pBlock->m_pQuickHeapPool == reinterpret_cast<QuickHeapPool*>(-1) )
        {
          QHBlock* b = (QHBlock*)QuickHeapInternalReAlloc(pBlock, size + sizeof(QHBlock) );
          ++b;
          return b;
        }

        void * pNew = Alloc(size);
        size_t dwSizeOld = ((QuickHeapPool*)(pBlock->m_pQuickHeapPool))->GetBlockSize();
        dwSizeOld -= sizeof(QHBlock);
        if ( size < dwSizeOld ) dwSizeOld = size;
        if ( dwSizeOld )
          memcpy(pNew, p, dwSizeOld);
        Free(p);
        return pNew;
      }
      return Alloc(size);
    }
  protected:
    inline QuickHeapPool * QuickHeapPoolNew(size_t BlockSize, int iInitBlockCnt)
    {
      return new QuickHeapPool(BlockSize, iInitBlockCnt);
    }

    inline void PoolSizeLoad()
    {
      m_iQHInitPoolSize = ciQHInitPoolSizeDefault;
      m_iQHInitPoolArraySize = ciQHInitPoolArraySizeDefault;

      if(m_iQHInitPoolArraySize >= m_iQHInitPoolSize)
      {
        if(1 == m_iQHInitPoolSize)
          ++m_iQHInitPoolSize;
        m_iQHInitPoolArraySize = m_iQHInitPoolSize - 1;
      }
    }
  public:
    QuickHeapPool ** m_arypQHPool;
#ifdef QUICK_HEAP_SYNCHRONIZATION
    QhMutex m_cs;
#endif
  protected:
    int m_iQHInitPoolSize;
    int m_iQHInitPoolArraySize;
  };
}
using namespace OdQuickHeapNS;

#endif
