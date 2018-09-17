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

#ifndef _ODGITRAITSCACHE_INCLUDED_
#define _ODGITRAITSCACHE_INCLUDED_

#include "GiCommonDraw.h"

#define STL_USING_MAP
#include "OdaSTL.h"

class OdDbStub;

#include "TD_PackPush.h"

/** \details
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
template <class traitsContainer, class traitsTaker, OdUInt32 byMask = 0>
class OdGiTraitsCache : public traitsTaker
{
  public:
    enum CacheType
    {
      kCacheNone   = 0,
      kCacheByMask = 1,
      kCacheAll    = 2
    };
  protected:
    struct CacheEntry
    {
      traitsContainer m_data;
      OdUInt32        m_flags;
      void set(const traitsContainer& data, OdUInt32 flags) { m_data = data; m_flags = flags; }
    };
   
  protected:
    CacheType           m_cacheType;
    typedef std::map<OdDbStub*, CacheEntry> _mmap;
    _mmap m_cacheData;
    OdUInt32            m_traitsAttrib;
  public:
    OdGiTraitsCache()
      : traitsTaker()
      , m_cacheType(kCacheNone)
      , m_traitsAttrib(0)
    {
    }
    explicit OdGiTraitsCache(CacheType cacheType)
      : traitsTaker()
      , m_cacheType(cacheType)
      , m_traitsAttrib(0)
    {
    }
    ~OdGiTraitsCache()
    {
    }

    void setCacheType(CacheType cacheType)
    {
      m_cacheType = cacheType;
    }
    CacheType cacheType() const
    {
      return m_cacheType;
    }

    void setAttributes(OdDbStub* objId, OdGiContext &giCtx, bool forceCaching = false)
    {
      if (m_cacheType != kCacheNone && m_cacheData.size() > 0)
      {
        // Note: iterator won't compile in this context on all platforms.
        typename _mmap::iterator it = m_cacheData.find(objId);
        if(it != m_cacheData.end())
        {
          m_traitsAttrib = it->second.m_flags;
          if(!(m_traitsAttrib & byMask))
            traitsTaker::setData(it->second.m_data);
          return;
        }
      }
      OdGiDrawablePtr drawable = giCtx.openDrawable(objId);
      if (!drawable.isNull())
      {
        m_traitsAttrib = drawable->setAttributes(this);
        if (m_cacheType != kCacheNone)
        {
          if (m_cacheType == kCacheAll || forceCaching || ((m_traitsAttrib & byMask) != 0))
          {
            m_cacheData[objId].set(this->getData(), m_traitsAttrib);
          }
        }
      }
    }

    void setFlags(OdUInt32 uAttrib)
    {
      m_traitsAttrib = uAttrib;
    }
    OdUInt32 getFlags() const
    {
      return m_traitsAttrib;
    }

    void clear()
    {
      m_cacheData.clear();
    }
};

// traitsContainer - must have operator =.
// traitsTaker - must have setData(traitsContainer), getData() methods.

#include "TD_PackPop.h"

#endif // #ifndef _ODGITRAITSCACHE_INCLUDED_
