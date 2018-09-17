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

#ifndef OdConstrainedCircleImpl_INCLUDED_
#define OdConstrainedCircleImpl_INCLUDED_/*!DOM*/

#include "OdConstrainedImplicitPointImpl.h"
#include "OdConstrainedCurveImpl.h"
#include "Ge/GeCircArc3d.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained circle node 
  in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedCircleImpl: public OdConstrainedCurveImpl
{ 
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedCircleImpl();

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdConstrainedCircleImpl();

  /** \details
    Initializes this constrained circle object.
  */
  virtual OdResult init(const OdDbObjectId depId);

  /** \details
    Returns the radius of this constrained circle.
  */
  double radius() const; 

  /** \details
    Returns the center of this constrained circle as three-dimensional point.
  */
  OdGePoint3d centerPoint() const;

  /** \details
    Checks whether the constrained circle is bounded and returns True when the it is bounded, or False otherwise.
  */
  virtual bool isBounded() const;

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
    Sets the radius for this constrained circle.
  */
  virtual OdResult setRadius(const double val);

  /** \details
    Sets the center for this constrained circle.
  */
  virtual OdResult setCenterPoint(const OdGePoint3d& center);

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
    Compares two constrained circles on equality.
  */
  virtual bool operator==(const OdConstrainedGeometryImpl & geom) const;

  /** \details
    Compares two constrained circles on unequality.
  */
  virtual bool operator!=(const OdConstrainedGeometryImpl & geom) const;

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

  /** \details
    Gets the three-dimensional circular arc as Ge-object.
  */
  virtual const OdGeCircArc3d getOdGeCircArc3d() const;

  /** \details
    Gets the original three-dimensional circular arc as Ge-object.
  */
  virtual const OdGeCircArc3d getOriginalOdGeCircArc3d() const;

  /** \details
    Sets the center and radius for this constrained circle.
  */
  OdResult set(const OdGePoint3d centerPt, const double rad);
protected:
  /*!DOM*/
  OdGeCircArc3d m_geArc;
  /*!DOM*/
  OdGeCircArc3d m_originGeArc;
}; 

#include "TD_PackPop.h"

#endif
