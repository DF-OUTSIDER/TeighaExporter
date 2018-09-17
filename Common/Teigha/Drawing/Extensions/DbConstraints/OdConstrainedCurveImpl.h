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

#ifndef OdConstrainedCurveImpl_INCLUDED
#define OdConstrainedCurveImpl_INCLUDED

#include "OdConstrainedGeometryImpl.h"
#include "OdConstrainedImplicitPointImpl.h"
#include "DbPolyline.h"
#include "OdAuditInfo.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained curve node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedCurveImpl :  public OdConstrainedGeometryImpl
{
public:
  /** \details
    Declares a pure method which must check whether the constrained curve is bounded and must be overloaded 
    in derived classes. This method returns True when the it is bounded, or False otherwise.
  */
  virtual bool isBounded() const = 0;

  /** \details
    Returns an array of pointers to the implicit points which belong to this constrained curve.
  */
  virtual void getConstrainedImplicitPoints(OdArray<OdConstrainedImplicitPoint*>& apImplicitPoints) const;

  /** \details
    Declares a pure method which must return the three-dimesional position for the specified implicit point 
    of the owning group object and must be overloaded in derived classes.
  */
  virtual OdResult point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const = 0;

  /** \details
    Declares a pure method which must set the three-dimesional position for the specified implicit point 
    of the owning group object and must be overloaded in derived classes.
  */
  virtual OdResult setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val) = 0;
  
  /** \details
    Declares a pure method which must return the three-dimesional original position for the specified 
    implicit point of the owning group object and must be overloaded in derived classes.
  */
  virtual OdResult getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const = 0;

  /** \details
    Gets an array of subentity paths correspond to this constrained curve through argument. 
  */
  virtual OdResult getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const;
  virtual void audit(OdConstrainedGeometry* pConsGeometry, OdAuditInfo* pAuditInfo) {};
protected:
  /** \details
    Protected constructor creates an instance of this class.
  */
  OdConstrainedCurveImpl();

  /** \details
    Gets the segment properties for the constrained geometry object specified by ID. 
  */
  OdResult getPolylineSegmentProp(const OdDbObjectId geomDependencyId, const OdDbPolylinePtr& pLinePtr,
                                  OdGePoint3d& wrdStart, OdGePoint3d& wrdEnd, double& bulge) const;

  /** \details
    Gets the segment properties for the constrained geometry object specified by ID. 
  */
  OdResult getPolylineSegmentPtIdx(const OdDbObjectId geomDependencyId, const OdDbPolylinePtr& pLinePtr,
                                  int& startIdx, int& endIdx) const;
};

#include "TD_PackPop.h"
#endif
