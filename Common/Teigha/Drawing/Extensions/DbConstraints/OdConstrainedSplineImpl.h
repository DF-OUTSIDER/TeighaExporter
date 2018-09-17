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

#ifndef OdConstrainedSplineImpl_INCLUDED
#define OdConstrainedSplineImpl_INCLUDED

#include "OdConstrainedCurveImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained spline node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedSplineImpl: public OdConstrainedCurveImpl
{ 
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedSplineImpl();

  /** \details
    Initializes this constrained spline object.
  */
  virtual OdResult init(const OdDbObjectId depId, const OdGeNurbCurve3d& spline);

  /** \details
    Checks whether the constrained spline is bounded and returns True when the it is bounded, 
    or False otherwise.
  */
  virtual bool isBounded() const;

  /** \details
    Returns the NURBS spline for this constrained spline.
  */
  const OdGeNurbCurve3d nurbSpline() const;

  /** \details
    Returns the original NURBS spline for this constrained spline.
  */
  const OdGeNurbCurve3d originalNurbSpline() const;

  /** \details
    Returns the number of define points. If the spline is closed, the this number equals to the number 
    of control points minus 1 for the constrained spline.
  */
  int numOfDefinePoints() const;

  /** \details
    Returns the define point belonged to this constrained spline at given index.
  */
  OdGePoint3d definePointAt(int index) const;

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

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
    Compares two constrained splines on equality.
  */
  virtual bool operator==(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Compares two constrained splines on unequality.
  */
  virtual bool operator!=(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Gets the three-dimesional point for the specified implicit point through argument.
  */
  virtual OdResult point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const;

  /** \details
    Sets the specified implicit point using a three-dimesional point passed through argument.
  */
  virtual OdResult setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val);

  /** \details
    Gets the three-dimesional original point for the specified implicit point through argument.
  */
  virtual OdResult getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const;
protected:
  /*!DOM*/
  bool m_UnkBool1;
  /*!DOM*/
  bool m_periodic;
  /*!DOM*/
  int m_degree;
  /*!DOM*/
  double m_precision;

  /*!DOM*/
  OdGeKnotVector m_knots;
  /*!DOM*/
  OdGeDoubleArray m_weights;
  /*!DOM*/
  OdGePoint3dArray m_controlPoints;

  /*!DOM*/
  OdArray<OdConstraintGroupNodeId,
    OdMemoryAllocator<OdConstraintGroupNodeId> > m_implicitPoints;

  /*!DOM*/
  OdGeNurbCurve3d m_nurb;
  /*!DOM*/
  OdGeNurbCurve3d m_originalNurb;
};

#include "TD_PackPop.h"

#endif
