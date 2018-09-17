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

#ifndef OdConstrainedImplicitPointImpl_INCLUDED
#define OdConstrainedImplicitPointImpl_INCLUDED

#include "OdConstrainedGeometry.h"
#include "OdConstrainedPointImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained implicit point node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedImplicitPointImpl: public OdConstrainedPointImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedImplicitPointImpl();

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdConstrainedImplicitPointImpl();

  /** \details
    Initializes this constrained implicit point object.
  */
  OdResult init(OdConstraintGroupNodeId constrCurvId, OdConstrainedImplicitPoint::ImplicitPointType ptype, int index = -1);

  /** \details
    Returns the implicit point type.
  */
  OdConstrainedImplicitPoint::ImplicitPointType  pointType() const;

  /** \details
    Returns the index of define point for this implicit point when type is kDefineImplicit,
    otherwise, returns (-1).
  */
  OdInt32  pointIndex() const;

  /** \details
    Returns an ID of the constraint group node for the constrained curve with which this implicit 
    point is associated.
  */
  OdConstraintGroupNodeId constrainedCurveId() const;

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Compares two constrained implicit points on equality.
  */
  virtual bool operator==(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Compares two constrained implicit points on equality.
  */
  virtual bool operator!=(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Sets data from a given geometry dependency using transformation matrix.
  */
  virtual OdResult setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                             const OdGeMatrix3d& matrFromWrd);

  /** \details
    Updates the database object.
  */
  virtual OdResult updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd);

  /** \details
    Returns the three-dimesional position relative to the plane of the owning group object.
  */
  virtual OdGePoint3d point() const;

  /** \details
    Sets the three-dimesional position of the owning group object.
  */
  virtual OdResult setPoint(const OdGePoint3d& val);

  /** \details
    Returns the three-dimesional original position of the owning group object.
  */
  virtual const OdGePoint3d getOriginalPoint() const;

  /** \details
    Gets an array of subentity paths correspond to this constrained geometry through argument. 
  */
  OdResult getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const;
protected:
  /*!DOM*/
  OdInt32  m_index;
  /*!DOM*/
  OdConstraintGroupNodeId  m_curveId;
  /*!DOM*/
  OdConstrainedImplicitPoint::ImplicitPointType  m_type;
};

#include "TD_PackPop.h"
#endif
