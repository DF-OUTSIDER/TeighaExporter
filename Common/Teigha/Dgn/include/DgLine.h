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
#ifndef __DG_LINE_H__
#define __DG_LINE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgCurveElement2d.h"
#include "DgCurveElement3d.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"


//2D subtype of Line

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLine2d : public OdDgCurveElement2d
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgLine2d );
public:

  //
  //method below are destined for manipulation with 2 points of the line
  //

  virtual OdGePoint2d getStartPoint() const;
  virtual void setStartPoint( const OdGePoint2d& point );

  virtual OdGePoint2d getEndPoint() const;
  virtual void setEndPoint( const OdGePoint2d& point );

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

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

typedef OdSmartPtr< OdDgLine2d > OdDgLine2dPtr;

//3D subtype of Line

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLine3d : public OdDgCurveElement3d
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgLine3d );
public:

  //
  //method below are destined for manipulation with 2 points of the line
  //

  virtual OdGePoint3d getStartPoint() const;
  virtual void setStartPoint( const OdGePoint3d& point );

  virtual OdGePoint3d getEndPoint() const;
  virtual void setEndPoint( const OdGePoint3d& point );

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

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

typedef OdSmartPtr< OdDgLine3d > OdDgLine3dPtr;

#include "TD_PackPop.h"

#endif // __DG_LINE_H__
