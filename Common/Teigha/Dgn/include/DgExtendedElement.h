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
#ifndef __DG_EXTENDEDELEMENT_H__
#define __DG_EXTENDEDELEMENT_H__

#include "TD_PackPush.h"
#include "DgXAttribute.h"
#include "DgGraphicsElement.h"

// Extended dgn element with graphics cache

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgExtendedGraphicsElement : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgExtendedGraphicsElement );
public:

  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  virtual OdUInt16 getTypeValue() const;
  virtual void     setTypeValue( OdUInt16 uTypeValue );

  virtual OdUInt32 getCacheItemCount() const;
  virtual OdDgGeometryCacheActionPtr getCacheItem( OdUInt32 uIndex ) const;
  virtual void setCacheItem( OdUInt32 uIndex, const OdDgGeometryCacheActionPtr& pCacheItem );
  virtual void addCacheItem( const OdDgGeometryCacheActionPtr& pCacheItem );
  virtual void insertCacheItem( OdUInt32 uInsertBeforeIndex, const OdDgGeometryCacheActionPtr& pCacheItem );
  virtual void deleteCacheItem( OdUInt32 uIndex );

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  virtual OdRxObjectPtr clone() const;
};

typedef OdSmartPtr< OdDgExtendedGraphicsElement > OdDgExtendedGraphicsElementPtr;

//----------------------------------------------------------
//
// OdDgCreateGraphicsCacheTool
//
//----------------------------------------------------------

class TG_EXPORT OdDgCreateGraphicsCacheTool
{
public:
  OdDgCreateGraphicsCacheTool();
  virtual ~OdDgCreateGraphicsCacheTool();

  OdDgXAttributePtr createGraphicsCache( const OdDgGraphicsElementPtr& pGraph, 
                                         OdGiRegenType regenType = kOdGiStandardDisplay, 
                                         OdUInt16 uXAttrValue = 0x0100 );
};

#include "TD_PackPop.h"

#endif // __DG_EXTENDEDELEMENT_H__
