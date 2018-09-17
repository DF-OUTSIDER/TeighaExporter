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

#ifndef OdConstrainedConstructionLineImpl_INCLUDED
#define OdConstrainedConstructionLineImpl_INCLUDED

#include "OdConstrainedLineImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained construction line node 
  in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedConstructionLineImpl: public OdConstrainedLineImpl 
{ 
public:
  virtual OdResult setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                             const OdGeMatrix3d& matrFromWrd);
  /** \details
    Updates the database object.
  */
  virtual OdResult updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd);
protected:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedConstructionLineImpl();
};


/** \details
  This class represents custom implementation of the constrained two-points construction line node 
  in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrained2PointsConstructionLineImpl: public OdConstrainedConstructionLineImpl
{ 
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrained2PointsConstructionLineImpl();
  OdConstrained2PointsConstructionLineImpl(const OdGePoint3d& point1, const OdGePoint3d& point2);

  /** \details
    Sets the construction line using specified two three-dimensional points.
  */
  void set(const OdGePoint3d& point1, const OdGePoint3d& point2);

  /*!DOM*/
  virtual void DoPostEvaluateJob();
};


/** \details
  This class represents custom implementation of the constrained datum infinity line node 
  in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedDatumLineImpl: public OdConstrainedConstructionLineImpl 
{ 
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedDatumLineImpl();
  OdConstrainedDatumLineImpl(const OdGePoint3d& pointOnLine, const OdGeVector3d& direction);

  /** \details
    Initializes this constrained datum infinity line object.
  */
  void init(const OdGePoint3d& pointOnLine, const OdGeVector3d& direction);
};

#include "TD_PackPop.h"

#endif
