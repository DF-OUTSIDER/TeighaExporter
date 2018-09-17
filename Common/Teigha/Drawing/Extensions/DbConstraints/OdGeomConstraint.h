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

#ifndef OdGeomConstraint_INCLUDED
#define OdGeomConstraint_INCLUDED

#include "OdConstraintGroupNode.h"


#include "TD_PackPush.h"

// Forward class declarations.
//
class OdConstrainedGeometry;
class OdHelpParameter;
class OdEqualHelpParameterConstraint;
class OdCompositeConstraint;

/** \details
  This class implements a constrained geometry node in the owning constraint group. 
  It is the base class for all constraints.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdGeomConstraint : public OdConstraintGroupNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGeomConstraint);

  /** \details
    The type of a constraint used to indicate the new constraint type when it is added between geometries.
  */
  enum GeomConstraintType
  {
    kHorizontal    = 0,    // Horizontal constraint applied to a line or two points.
    kVertical         ,    // Vertical constraint applied to a line or two points. 
    kParallel         ,    // Parallel constraint applied between two lines.
    kPerpendicular    ,    // Perpendicular constraint applied between two lines. 
    kNormal           ,    // Normal constraint applied between a line and circle (or arc). 
    kColinear         ,    // Colinear constraint applied between two lines.
    kCoincident       ,    // Coincident constraint applied between point and point or point and curve. 
    kConcentric       ,    // Concentric constraint applied between any two of circles, ellipses, or arcs.
    kTangent          ,    // Tangent constraint applied between two curves (except two lines).
    kEqualRadius      ,    // Equal radius constraint applied between any two of circles or arcs.
    kEqualLength      ,    // Equal length constraint applied between two bounded lines (not rays).
    kSymmetric	      ,    // Symmetric constraint applied between two of same geometry types. 
    kG2Smooth         ,    // G2Smooth constraint applied between a bounded spline and a bounded curve.
    kFix	           // Fix constraint applied on any supported geometry.
  };


  /** \details
    Returns the array of pointer to the constrain geometrical objects which are connected to.
  */
  virtual OdResult getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const;

  /** \details
    Returns the array of pointers refered to the objects of help parameters to which this constraint is connected. 
    The object number is same as the number of geometries involved in this constraint.
  */
  virtual OdResult getConnectedHelpParameters(OdArray<OdHelpParameter*>& apHelpParameters) const;

  /** \details
    Returns a pointer to the connected object of help parameters. A help parameter object is always connected 
    with a geometry constraint object.
  */
  virtual OdHelpParameter*  getConnectedHelpParameterFor(const OdConstrainedGeometry* pConsGeom) const;

  /** \details
    Returns a pointer to the owning constraint object when it is owned by a composite constraint. 
  */
  OdCompositeConstraint*    getOwningCompositeConstraint() const;

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

protected:
  /** \details
    Protected constructor creates an instance of this class.
  */
  OdGeomConstraint();
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdGeomConstraint class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdGeomConstraint> OdGeomConstraintPtr;


/** \details
  This class implements a help parameter node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdHelpParameter : public OdConstraintGroupNode
{
public:
  ODRX_DECLARE_MEMBERS(OdHelpParameter);

  /** \details
    Constructor creates an instance of this class.
  */
  OdHelpParameter();

  /** \details
    Returns a pointer to the geomtry constraint node to which this help parameter is connected. 
    This method does not take in account of equal parameter applied to this help parameter.
  */
  OdResult getConnectedGeometry(OdConstrainedGeometry*& pGeometry) const;

  /** \details
    Returns a pointer to geometry constrain object to which the help parameter is connected.
  */
  OdResult getConnectedConstraint(OdGeomConstraint*& pConstraint) const;

  /** \details
    Returns the array of pointers to equal help parameter constraint object to which the constraint is connected.
  */
  OdResult getConnectedEqualparamConstraints(OdArray<OdEqualHelpParameterConstraint*>& apEqualParamConstrs) const;

  /** \details
    Returns the value assigned to the help parameter (larger than or equal to 0.0).
  */
  double getValue() const;

  /** \details
    Sets the value for the help parameter (larger than or equal to 0.0).
  */
  void setValue(const double &val);
};


/** \details
  This class implements a composite constraint node in the owning constraint group. 
  This class is base for kind of composite constraint.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdCompositeConstraint : public OdGeomConstraint
{
public:
  ODRX_DECLARE_MEMBERS(OdCompositeConstraint);

  /** \details
    Returns an array of pointers to the geometry constraint objects that are owned for 
    the composite constraint.
  */
  OdResult getOwnedConstraints(OdArray<OdGeomConstraint*>& apSubConstraints) const;

protected:
  /** \details
    Constructor creates an instance of this class.
  */
  OdCompositeConstraint();
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdHelpParameter class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdHelpParameter> OdHelpParameterPtr;


#include "TD_PackPop.h"

#endif
