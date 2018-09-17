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

#ifndef OdGeomConstraintImpl_INCLUDED_
#define OdGeomConstraintImpl_INCLUDED_ /*!DOM*/

#include "OdConstraintGroupNodeImpl.h"
#include "OdGeomConstraint.h"

#include "TD_PackPush.h"

/** \details
  This class represents a custom implementation of a geometry node in the owning constraint group
  which is the base for all constraints.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdGeomConstraintImpl : public OdConstraintGroupNodeImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdGeomConstraintImpl();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdGeomConstraintImpl();

  /** \details
    Returns the array of pointer to the constrain geometrical objects which are connected to.
  */
  virtual OdResult getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const;

  /** \details
    Returns the array of pointers referred to the objects of help parameters to which this constraint is connected. 
    The object number is same as the number of geometries involved in this constraint.
  */
  virtual OdResult getConnectedHelpParameters(OdArray<OdHelpParameter*>& apHelpParameters) const;

  /** \details
    Returns a pointer to the connected object of help parameters. A help parameter object is always connected 
    with a geometry constraint object.
  */
  virtual OdHelpParameter*  getConnectedHelpParameterFor(const OdConstrainedGeometry* pConsGeom) const;

  /** \details
    Returns a pointer to the composite constraint object when it is owned by a composite constraint. 
  */
  OdCompositeConstraint*    getOwningCompositeConstraint() const;

  /** \details
    Sets an ID to the owing composite constraint object. 
  */
  OdResult setOwningCompositeConstraintId(const OdConstraintGroupNodeId id);

  /** \details
    Leaves the geomentry constraint node in the constraint group and removes it from the DCM dimension system. 
  */
  OdResult deactivate();

  /** \details
    Takes an inactive the geometry constraint and adds it to the DCM dimension system. 
  */
  OdResult reactivate();

  /** \details
    Checks whether the geometry constraint is in DCM dimension system. 
  */
  bool isActive() const;

  /** \details
    Checks whether the geometry constraint is implied which means the constraints are applied by the 
    framework for implied geometric conditions.
  */
  bool isImplied() const;

  /** \details
    Checks whether the geometry constraint is implied or is part of a composite constraint. Internal 
    constraints should not be directly dealt with at client-code level. 
  */
  bool isInternal() const;

  /** \details
    Checks whether the geometry constraint is enabled. 
  */
  bool isEnabled() const;

  /** \details
    Sets the implied status. 
  */
  void setImplied(bool implied);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

protected:
  /*!DOM*/
  int m_owningCompositeConstraint;
  /*!DOM*/
  bool m_implied; //need verify
  /*!DOM*/
  bool m_active; // need verify. But only after ExplicitConstraint implementation

  // these fields are not read from file
  /*!DOM*/
  bool m_enabled;
  /*!DOM*/
  bool m_partOfComposite;
};

#include "TD_PackPop.h"

#endif
