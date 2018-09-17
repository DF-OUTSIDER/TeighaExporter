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

#ifndef OdConstrainedBoundedLineImpl_INCLUDED
#define OdConstrainedBoundedLineImpl_INCLUDED

#include "OdConstrainedLineImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained bounded line node 
  in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedBoundedLineImpl: public OdConstrainedLineImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedBoundedLineImpl();
  OdConstrainedBoundedLineImpl(bool bRay);

  /** \details
    Initializes this constrained bounded line object.
  */
  virtual OdResult init(const OdDbObjectId depId, bool bRay);

  /** \details
    Checks whether this constrained line is a ray.
  */
  bool isRay() const;

  /** \details
    Returns the start point of this line relative to the plane of the owning constraint group.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this line relative to the plane of the owning constraint group.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Returns the middle point of this line relative to the plane of the owning constraint group.
  */
  OdGePoint3d midPoint() const;

  /** \details
    Checks whether the constrained line is bounded and returns True when the it is bounded, or False otherwise.
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
    Sets the start point for this constrained line.
  */
  OdResult setStartPoint(const OdGePoint3d& val);

  /** \details
    Sets the end point for this constrained line.
  */
  OdResult setEndPoint(const OdGePoint3d& val);

  /** \details
    Sets the specified three-dimensional point on the constrained line.
  */
  virtual OdResult setPointOnLine(const OdGePoint3d& val);

  /** \details
    Sets the specified three-dimensional vector as new direction of the constrained line.
  */
  virtual OdResult setDirection(const OdGeVector3d& val);

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
    Compares two constrained points on equality.
  */
  virtual bool operator==(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Compares two constrained points on unequality.
  */
  virtual bool operator!=(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Gets the three-dimensional linear segment as Ge-object.
  */
  const OdGeLineSeg3d getOdGeLineSeg3d() const;

  /** \details
    Gets the original three-dimensional linear segment as Ge-object.
  */
  const OdGeLineSeg3d getOriginalOdGeLineSeg3d() const;

  /** \details
    Gets the three-dimensional ray as Ge-object.
  */
  const OdGeRay3d getOdGeRay3d() const;

  /** \details
    Gets the original three-dimensional ray as Ge-object.
  */
  const OdGeRay3d getOriginalOdGeRay3d() const;

  /** \details
    Returns an array of full subent paths.
  */
  OdResult getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const;

  /** \details
    Sets the start point and direction vector for this constrained line.
  */
  virtual OdResult set(const OdGePoint3d pointOnLine, const OdGeVector3d dir);

  /** \details
    Sets the start point and end point for this constrained line.
  */
  OdResult set(const OdGePoint3d startPt, const OdGePoint3d endPt);
private:
  /*!DOM*/
  bool m_bRay;
  /*!DOM*/
  OdGeLineSeg3d m_originLine;
  /*!DOM*/
  OdGeLineSeg3d m_line;
  /*!DOM*/
  OdGeRay3d m_originRay;
  /*!DOM*/
  OdGeRay3d m_ray;
};

#include "TD_PackPop.h"

#endif
