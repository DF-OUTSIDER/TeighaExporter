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

#ifndef OdConstrainedGeometryImpl_INCLUDED
#define OdConstrainedGeometryImpl_INCLUDED

#include "OdConstrainedGeometry.h"
#include "OdConstraintGroupNodeImpl.h"
#include "DbAssoc2dConstraintGroup.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained geometry node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedGeometryImpl :  public OdConstraintGroupNodeImpl
{
public:
  /** The constraint geometry type 
  */
   enum ConstrainedGeometryImplType
  {
    ConstrainedGeometryUnknown,   // Unknown
    PointImpl,                    // Point
    ImplicitPointImpl,            // Implicit point
    CircleImpl,                   // Circle
    CircleArcImpl,                // Circlar arc
    LineImpl,                     // Line
    BoundedLineImpl,              // Bounded line
    EllipseImpl,                  // Ellipse
    BoundedEllipseImpl,           // Elliptical arc
    SplineImpl                    // Spline
  };

  /** \details
    Constructor creates an instance of this class.
  */
  virtual ~OdConstrainedGeometryImpl();

  /** \details
    Initializes this constrained geometry object.
  */
  virtual OdResult init(const OdDbObjectId depId);

  /** \details
    Checks whether this constrained geometry object is read-only and returns True when it is only for reading,
    or False, otherwise.
  */
  virtual bool isReadOnly() const;

  /** \details
    Returns the object ID of the associated geometry dependency object that this constrain references. 
  */
  OdDbObjectId geomDependencyId() const;

  /** \details
    Returns a pointer to the owning constrained rigid set object to which the constrained geometry 
    is belonged. 
  */
  OdConstrainedRigidSet* getOwningRigidSet() const;

  /** \details
    Returns an array of pointers to geometry constraint objects which reference this geometry.
  */
  OdResult getConnectedConstraints(OdArray<OdGeomConstraint*>& apConstraints) const;

  /** \details
    Returns an array of pointers to geometry constraint objects which are referenced between 
    this constrained geometry and another constrained geometry.
  */
  OdResult getCommonConstraints(OdConstrainedGeometry* pOtherConsGeom,
                                OdArray<OdGeomConstraint*>& apConstraints) const;

  /** \details
    Returns an array of pointers to geometry constraint objects to which this geometry is 
    indirectly connected using common constraints.
  */
  OdResult getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const;

  /** \details
    Returns an array of subentity paths correspond to this constrained geometry through argument. 
  */
  virtual OdResult getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const;

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
    Compares two constrained geometry objects on equality.
  */
  virtual bool operator==(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Compares two constrained geometry objects on unequality.
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
    Checks whether this constrained geometry object is modified and returns True when it was modified,
    or False, otherwise.
  */
  bool isModified() const;

  /** \details
    Returns the contraint geometry type.
  */
  ConstrainedGeometryImplType getType(const OdConstrainedGeometryImpl & geom) const {return geom.m_implObjType;}

  /*!DOM*/
  virtual void DoPostEvaluateJob(){}
protected:
  /** \details
    Protected constructor creates an instance of this class.
  */
  OdConstrainedGeometryImpl();

  /** \details
    Returns the associated geometry dependency.
  */
  OdDbAssocGeomDependencyPtr geomDependency(const OdDbObjectId geomDepId,
                                              OdDb::OpenMode mode = OdDb::kForRead);

  OdDbAssocGeomDependencyPtr geomDependency(const OdDbObjectId geomDepId) const;

  /** \details
    Returns the associated geometry object.
  */
  OdDbObjectPtr geomObject(const OdDbObjectId geomDepId, OdDb::OpenMode mode = OdDb::kForRead);

  /** \details
    Converts the transformation matrix to WCS.
  */
  void matrToWrdCoord(const OdDbObjectId geomDepId, OdGeMatrix3d &matr);

  /** \details
    Returns an array of full subent paths.
  */
  OdResult getFullSubentPath(OdDbFullSubentPath& path) const;

  /*!DOM*/
  OdDbObjectId                m_idDependency;
  /*!DOM*/
  OdConstraintGroupNodeId     m_rigidSetIdx;
  /*!DOM*/
  bool                        m_origGeomSet;
  /*!DOM*/
  ConstrainedGeometryImplType m_implObjType;
};

#include "TD_PackPop.h"
#endif
