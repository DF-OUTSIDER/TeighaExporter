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
#ifndef __DG_MULTIVERTEX_H__
#define __DG_MULTIVERTEX_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgCurveElement2d.h"
#include "DgCurveElement3d.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultiVertex2d : public OdDgCurveElement2d
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgMultiVertex2d);
public:
  virtual OdGePoint2d getVertexAt( OdUInt32 ind ) const;
  virtual void        setVertexAt( OdUInt32 ind, const OdGePoint2d& pt );
  virtual void        addVertex( const OdGePoint2d& pt );
  virtual void        insertVertex( OdUInt32 uInsertBefore, const OdGePoint2d& pt );
  virtual OdUInt32    getVerticesCount() const;

  virtual void        removeAllVertices();
  virtual void        removeVertexAt( OdUInt32 index );
  virtual void        removeLastVertex();

  //overrides for OdDgCurveElement2d

  virtual bool     isClosed() const;
  virtual bool     isPeriodic() const;
  virtual OdResult getStartParam( double& startParam ) const;
  virtual OdResult getEndParam( double& endParam ) const;
  virtual OdResult getStartPoint( OdGePoint2d& startPoint ) const;
  virtual OdResult getEndPoint( OdGePoint2d& endPoint ) const;
  virtual OdResult getPointAtParam( double param, OdGePoint2d& pointOnCurve ) const;
  virtual OdResult getParamAtPoint( const OdGePoint2d& pointOnCurve, double& param ) const;
  virtual OdResult getDistAtParam( double param, double& dist ) const;
  virtual OdResult getParamAtDist( double dist, double& param ) const;
  virtual OdResult getFirstDeriv ( double param, OdGeVector2d& firstDeriv) const;
  virtual OdResult getSecondDeriv( double param, OdGeVector2d& secondDeriv) const;
  virtual OdResult getArea( double& area) const;

  virtual OdResult reverseCurve();
  virtual OdResult getOdGeCurve ( OdGeCurve2d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol ) const;
  virtual OdResult setFromOdGeCurve ( const OdGeCurve2d& geCurve, const OdGeTol& tol = OdGeContext::gTol );

  //override for OdGiDrawable
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

typedef OdSmartPtr<OdDgMultiVertex2d> OdDgMultiVertex2dPtr;


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultiVertex3d : public OdDgCurveElement3d
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgMultiVertex3d);
public:
  virtual OdGePoint3d getVertexAt( OdUInt32 ind ) const;
  virtual void        setVertexAt( OdUInt32 ind, const OdGePoint3d& pt );
  virtual void        addVertex( const OdGePoint3d& pt );
  virtual void        insertVertex( OdUInt32 uInsertBefore, const OdGePoint3d& pt );
  virtual OdUInt32    getVerticesCount() const;

  virtual void        removeAllVertices();
  virtual void        removeVertexAt( OdUInt32 index );
  virtual void        removeLastVertex();

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

  //override for OdGiDrawable
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

typedef OdSmartPtr<OdDgMultiVertex3d> OdDgMultiVertex3dPtr;


#include "TD_PackPop.h"

#endif // __DG_MULTIVERTEX_H__
