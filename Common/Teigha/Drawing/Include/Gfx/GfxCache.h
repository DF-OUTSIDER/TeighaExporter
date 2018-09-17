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

#ifndef _CLOUDGFXCACHE_INCLUDED_
#define _CLOUDGFXCACHE_INCLUDED_ /* { Secret } */

#include "RxModule.h"
#include "Gs/Gs.h"
#include "MemoryStream.h"
#include "StringArray.h"
#include "Tf/TfIO.h"

#include "TD_PackPush.h"

class CloudGfxCache;
typedef OdSmartPtr<CloudGfxCache> CloudGfxCachePtr;


class CloudGfxCache : public OdTfStorage {
public:
  class LayoutData;
  typedef OdSmartPtr<LayoutData> LayoutDataPtr;
  class VectorizerCtx;
  typedef OdSmartPtr<VectorizerCtx> VectorizerCtxPtr;

  virtual OdStringArray layoutNames() = 0;
  virtual LayoutDataPtr layoutData(const OdString& name) = 0;

  class LayoutData : public OdRxObject {
  public:
    virtual OdStreamBufPtr readState() = 0;
    virtual OdStreamBufPtr newStateBuf() { return OdMemoryStream::createNew(); };
    virtual void putState(OdStreamBuf* buf) = 0;

    virtual VectorizerCtxPtr vectorizerCtx() = 0;

    virtual void delGfx(const OdUInt64 handle) = 0;
  };

  class VectorizerCtx : public OdRxObject {
  public:
    virtual void setViewCtx(const OdGsView& view, double simplfTol, double sewTol) = 0;
    virtual OdStreamBufPtr newGfxBuf(const OdUInt64 handle) { return OdStreamBufPtr(); };
    virtual void putGfx(const OdUInt64 handle, OdStreamBuf& gfxData, 
                        const OdUInt32 awareFlags, const OdGeExtents3d& extents, 
                        const OdStringArray& fonts) = 0;
    virtual OdStreamBufPtr getGfx(const OdUInt64 handle) = 0;
    virtual void getFonts(const OdUInt64 handle, OdStringArray& fontsIn) = 0;
    virtual bool getGfx(const OdUInt64 handle, OdUInt32& awareFlags, OdGeExtents3d& extents) = 0;
    virtual void delGfx(const OdUInt64 handle) = 0;
  };
};

#include "TD_PackPop.h"


#endif //_CLOUDGFXCACHE_INCLUDED_
