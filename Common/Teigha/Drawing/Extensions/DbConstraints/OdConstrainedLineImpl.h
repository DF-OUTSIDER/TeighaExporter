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

#ifndef OdConstrainedLineImpl_INCLUDED
#define OdConstrainedLineImpl_INCLUDED

#include "OdConstrainedCurveImpl.h"
#include "Ge/GeLine3d.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained line node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdConstrainedLineImpl: public OdConstrainedCurveImpl
{ 
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedLineImpl();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdConstrainedLineImpl();

  /** \details
    Initializes this constrained line object.
  */
  virtual OdResult init(const OdDbObjectId depId);

  /** \details
    Returns an arbitrary point placed on the line relative to the plane of the owning constraint group.
  */
  virtual OdGePoint3d pointOnLine() const;

  /** \details
    Returns an unit vector that specifies the direction of the line relative to the plane 
    of the owning constraint group.
  */
  virtual OdGeVector3d direction() const;

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
    Sets data from a given geometry dependency using transformation matrix.
  */
  virtual OdResult setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                             const OdGeMatrix3d& matrFromWrd);

  /** \details
    Updates the database object.
  */
  virtual OdResult updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd);

  /** \details
    Sets the specified three-dimensional point on the constrained line.
  */
  virtual OdResult setPointOnLine(const OdGePoint3d& val);

  /** \details
    Sets the specified three-dimensional vector as new direction of the constrained line.
  */
  virtual OdResult setDirection(const OdGeVector3d& val);

  /** \details
    Sets the specified three-dimensional vector as new direction and three-dimensional point for the constrained line.
  */
  virtual OdResult set(const OdGePoint3d pointOnLine, const OdGeVector3d dir);

  /** \details
    Compares two constrained lines on equality.
  */
  bool operator==(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Compares two constrained lines on unequality.
  */
  bool operator!=(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Returns the three-dimesional position relative to the plane of the owning group object.
  */
  virtual OdResult point(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const;

  /** \details
    Sets the three-dimesional position of the owning group object.
  */
  virtual OdResult setPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, const OdGePoint3d& val);

  /** \details
    Returns the three-dimesional original position of the owning group object.
  */
  virtual OdResult getOriginalPoint(const OdConstrainedImplicitPointImpl* pImplicitPoint, OdGePoint3d& res) const;

  /** \details
    Gets the three-dimensional linear segment as Ge-object.
  */
  virtual const OdGeLine3d getOdGeLine3d() const;

  /** \details
    Gets the original three-dimensional linear segment as Ge-object.
  */
  virtual const OdGeLine3d getOriginalOdGeLine3d() const;
protected:
  /*!DOM*/
  OdGeLine3d m_originLine;
  /*!DOM*/
  OdGeLine3d m_line;
}; 

#include "TD_PackPop.h"
#endif
