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
#ifndef __DG_COMPLEXCURVE_H__
#define __DG_COMPLEXCURVE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgElementIterator.h"
#include "DgCurveElement3d.h"

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgComplexCurve: public OdDgCurveElement3d
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgComplexCurve );
public:
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  /**
    \details
    The functions allow to work with complex curve sub items.
    \remarks 
    getItem() function can return non-dbro element with NULL element id for additional items.
    ( if end point of item has difference with start point of next item we create temporary 
      linear segment with index. ) If you want modify the item, you must add it to complex
      curve. ( call setItem(...) function with item index and linear element. )
  */
  OdUInt32       getItemsCount(bool bSkipDeleted = true) const;
  OdDgElementPtr getItem( OdUInt32 uItemIndex, bool bSkipDeleted = true, OdDg::OpenMode mode = OdDg::kForRead );
  void           setItem( OdUInt32 uItemIndex, OdDgElementPtr pElement, bool bSkipDeleted = true );
  void           insertItem( OdUInt32 uIndexBefore, OdDgElementPtr pElement, bool bSkipDeleted = true );
  void           removeItem( OdUInt32 uItemIndex, bool bSkipDeleted = true );

  //overrides for OdDgCurveElement3d

  virtual bool     isClosed() const;
  virtual bool     isPeriodic() const;
  virtual OdResult getStartParam( double& startParam ) const;
  virtual OdResult getEndParam( double& endParam ) const;
  virtual OdResult getStartPoint( OdGePoint3d& startPoint ) const;
  virtual OdResult getEndPoint( OdGePoint3d& endPoint ) const;
  virtual OdResult getPointAtParam( double param, OdGePoint3d& pointOnCurve ) const;
  virtual OdResult getParamAtPoint( const OdGePoint3d& pointOnCurve, double& param ) const;
  virtual OdResult getDistAtParam( double param, double& dist ) const;
  virtual OdResult getParamAtDist( double dist, double& param ) const;
  virtual OdResult getFirstDeriv ( double param, OdGeVector3d& firstDeriv) const;
  virtual OdResult getSecondDeriv( double param, OdGeVector3d& secondDeriv) const;
  virtual OdResult getArea( double& area) const;
  virtual OdResult reverseCurve();
  virtual OdResult getOdGeCurve ( OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol ) const;
  virtual OdResult setFromOdGeCurve ( const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol );

  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdRxObjectPtr clone() const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
  virtual void copyFrom(const OdRxObject* pSource);

  virtual void subHandOverTo( OdDgElement* pNewElement );
  virtual void subSwapIdWith( const OdDgElementId& otherId );
};

typedef OdSmartPtr<OdDgComplexCurve> OdDgComplexCurvePtr;

#include "TD_PackPop.h"

#endif // __DG_COMPLEXCURVE_H__
