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

#ifndef OdConstrainedGeometry_INCLUDED
#define OdConstrainedGeometry_INCLUDED

#include "OdGeomConstraint.h"
#include "OdConstraintGroupNode.h"
#include "DbAssocGeomDependency.h"
#include "DbSubentId.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeRay3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GePoint3dArray.h"

#include "TD_PackPush.h"


// Forward class declarations.
//
class OdConstrainedRigidSet;

/** \details
  This class implements a constrained geometry node in the owning constraint group. This class 
  is the base class for every type of geometry provided by constraint.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedGeometry :  public OdConstraintGroupNode
{
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedGeometry);

  /** \details
    Initializes this constrained geometry object.
  */
  OdResult init(const OdDbObjectId geomDependencyId);

  /** \details
    Indicates the read-only status for this constrained geometry.
  */
  virtual bool isReadOnly() const;

  /** \details
    Returns the object ID of the associated geometry dependency object that this constrain
    references. 

    \remarks
    The returned ID may be kNull for some types of constrained geometry.
  */
  OdDbObjectId geomDependencyId() const;

  /** \details
    Returns a pointer to the owning constrained rigid set object to which the constrained geometry 
    is belonged. 

    \remarks
    One rigid set can contain only one constrained geometry. The returned value is NULL when the 
    constrained geometry is not placed in a rigid set.
  */
  OdConstrainedRigidSet* getOwningRigidSet() const;

  /** \details
    Returns an array of pointers to geometry constraint objects which are referenced between 
    this constrained geometry and another constrained geometry.
  */
  OdResult getCommonConstraints(OdConstrainedGeometry* pOtherConsGeom,
                                OdArray<OdGeomConstraint*>& apConstraints) const;

  /** \details
    Returns an array of pointers to geometry constraint objects which reference this geometry.

    \remarks
    For some entities (line, arc, ellipse) this array also contains the constraints between its 
    implicit points and constrained geometry.
  */
  OdResult getConnectedConstraints(OdArray<OdGeomConstraint*>& apConstraints) const;

  /** \details
    Returns an array of pointers to geometry constraint objects to which this geometry is 
    indirectly connected using common constraints.
  */
  OdResult getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const;

  /** \details
    Returns an array of subentity paths correspond to this constrained geometry through argument. 
  */
  OdResult getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const;

  /** \details
    Checks whether this constrained geometry object equals another constrained geometry object. 
  */
  virtual bool operator==(const OdConstrainedGeometry& geom) const;

  /** \details
    Checks whether this constrained geometry object unequals another constrained geometry object. 
  */
  virtual bool operator!=(const OdConstrainedGeometry& geom) const;

  /** \details
    Sets data from a given geometry dependency using transformation matrix.
  */
  OdResult setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                     const OdGeMatrix3d& matrFromWrd);

  /** \details
    Checks whether this constrained geometry object is modified. 
  */
  bool isModified() const;

  /*!DOM*/
  void DoPostEvaluateJob();

protected:
  /** \details
    Protected constructor creates an instance of this class.
  */
  OdConstrainedGeometry();
};


/** \details
  This class implements a constrained point node in the owning constraint group. Commonly it catches 
  a reference to an associated geometry dependency object that may be the insertion point of a block 
  reference or custom entity.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedPoint: public OdConstrainedGeometry 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedPoint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedPoint(bool bCreateImp = true);

  /** \details
    Returns the three-dimesional position relative to the plane of the owning group object.
  */
  OdGePoint3d point() const;

  /** \details
    Sets the three-dimesional position of the owning group object.
  */
  OdResult setPoint(const OdGePoint3d& val);

  /** \details
    Returns the three-dimesional original position of the owning group object.
  */
  const OdGePoint3d getOriginalPoint() const;
}; 


/** \details
  This class implements an implicit point node in the owning constraint group. The constrained implicit 
  point is permanent associated with a constrained curve.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedImplicitPoint: public OdConstrainedPoint 
{
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedImplicitPoint);

  /** The implicit point type 
  */
   enum ImplicitPointType 
  {
    kStartImplicit = 0,   // Start point of line, arc, ellipse
    kEndImplicit,         // End point of line, arc, ellipse
    kMidImplicit,         // Middle point of line, arc
    kCenterImplicit,      // Start point of circle, arc, ellipse
    kDefineImplicit       // Define point of curve
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedImplicitPoint();

  /** \details
    Initializes this implicit point node.
  */
  OdResult init(OdConstraintGroupNodeId constrCurvId, ImplicitPointType ptype, int index = -1);

  /** \details
    Returns the implicit point type.
  */
  ImplicitPointType pointType() const;

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
};


/** \details
  This class implements a constrained curve node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedCurve :  public OdConstrainedGeometry
{
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedCurve);

  /** \details
    Returns True when this constrained curve is bounded, or False, otherwise.
  */
  virtual bool isBounded() const = 0;

  /** \details
    Returns an array of pointers to the implicit points which belong to this constrained curve.
  */
  virtual void getConstrainedImplicitPoints(OdArray<OdConstrainedImplicitPoint*>& apImplicitPoints) const;

  /** \details
    Gets the three-dimesional point for the specified implicit point through argument.
  */
  OdResult point(const OdConstrainedImplicitPoint* pImplicitPoint, OdGePoint3d& res) const;

  /** \details
    Sets the specified implicit point using a three-dimesional point passed through argument.
  */
  OdResult setPoint(const OdConstrainedImplicitPoint* pImplicitPoint, const OdGePoint3d& val);

  /** \details
    Gets the three-dimesional original point for the specified implicit point through argument.
  */
  OdResult getOriginalPoint(const OdConstrainedImplicitPoint* pImplicitPoint, OdGePoint3d& res) const;
protected:
  /** \details
    Protected constructor creates an instance of this class.
  */
  OdConstrainedCurve();
};


/** \details
  This class implements a constrained line node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedLine: public OdConstrainedCurve 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedLine);
  
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedLine(bool bCreateImp = true);

  /** \details
    Returns an arbitrary point placed on the line relative to the plane of the owning constraint group.
  */
  OdGePoint3d pointOnLine() const;

  /** \details
    Returns an unit vector that specifies the direction of the line relative to the plane 
    of the owning constraint group.
  */
  OdGeVector3d direction() const;

  /** \details
    Checks whether the constrained line is bounded and returns True when the it is bounded, or False otherwise.
  */
  virtual bool isBounded() const;

  /** \details
    Sets the specified three-dimensional point on the constrained line.
  */
  OdResult setPointOnLine(const OdGePoint3d& val);

  /** \details
    Sets the specified three-dimensional vector as new direction of the constrained line.
  */
  OdResult setDirection(const OdGeVector3d& val);

  /** \details
    Sets the specified three-dimensional vector as new direction and three-dimensional point for the constrained line.
  */
  OdResult set(const OdGePoint3d pointOnLine, const OdGeVector3d dir);

  /** \details
    Gets the three-dimensional linear segment as Ge-object.
  */
  const OdGeLine3d getOdGeLine3d() const;

  /** \details
    Gets the original three-dimensional linear segment as Ge-object.
  */
  const OdGeLine3d getOriginalOdGeLine3d() const;
}; 


/** \details
  This class implements a constrained infinity line node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedConstructionLine: public OdConstrainedLine 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedConstructionLine);
protected:
  /** \details
    Protected constructor creates an instance of this class.
  */
  OdConstrainedConstructionLine();
};


/** \details
  This class implements a constrained infinity line node in the owning constraint group which 
  internally coincide with two constrained points.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrained2PointsConstructionLine: public OdConstrainedConstructionLine 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrained2PointsConstructionLine);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrained2PointsConstructionLine();
};


/** \details
  This class implements a constrained datum infinity line node in the owning constraint group which
  is read-only and for reference as datum line.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedDatumLine: public OdConstrainedConstructionLine 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedDatumLine);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedDatumLine();
};


/** \details
  This class implements a constrained bounded line node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedBoundedLine: public OdConstrainedLine
{
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedBoundedLine);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedBoundedLine();

  /** \details
    Initializes this constrained line object.
  */
  OdResult init(const OdDbObjectId depId, bool bRay);

  /** \details
    Checks whether this constrained line is a ray. If the line is a ray that the start point defines 
    the single point, and this object is considered to be pointing away from this point in the 
    specified direction.
  */
  bool isRay() const;

  /** \details
    Returns the start point of this line relative to the plane of the owning constraint group.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this line relative to the plane of the owning constraint group.

    \remarks
    This property is not applied for a ray.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Returns the middle point of this line relative to the plane of the owning constraint group. 

    \remarks
    This property is not applied for a ray.
  */
  OdGePoint3d midPoint() const;

  /** \details
    Checks whether the constrained line is bounded and returns True when the it is bounded, or False otherwise.
  */
  virtual bool isBounded() const;

  /** \details
    Sets the start point for this constrained line.
  */
  OdResult setStartPoint(const OdGePoint3d& val);

  /** \details
    Sets the end point for this constrained line.
  */
  OdResult setEndPoint(const OdGePoint3d& val);

  /** \details
    Sets the start point and end point for this constrained line.
  */
  OdResult set(const OdGePoint3d startPt, const OdGePoint3d endPt);

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
private:
  /*!DOM*/
  OdGeLineSeg3d m_originLine;
  /*!DOM*/
  OdGeLineSeg3d m_line;
  /*!DOM*/
  OdGeRay3d m_originRay;
  /*!DOM*/
  OdGeRay3d m_ray;
  /*!DOM*/
  bool m_bRay;
};


/** \details
  This class implements a constrained circle node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedCircle: public OdConstrainedCurve 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedCircle);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedCircle(bool bCreateImp = true);

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
    Sets the radius for this constrained circle.
  */
  OdResult setRadius(const double val);

  /** \details
    Sets the center for this constrained circle.
  */
  OdResult setCenterPoint(const OdGePoint3d& center);

  /** \details
    Sets the center and radius for this constrained circle.
  */
  OdResult set(const OdGePoint3d centerPt, const double rad);

  /** \details
    Gets the three-dimensional circular arc as Ge-object.
  */
  const OdGeCircArc3d getOdGeCircArc3d() const;

  /** \details
    Gets the original three-dimensional circular arc as Ge-object.
  */
  const OdGeCircArc3d getOriginalOdGeCircArc3d() const;
}; 


/** \details
  This class implements a constrained circular arc node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedArc: public OdConstrainedCircle 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedArc);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedArc();

  /** \details
    Returns the start point of this constrained circular arc.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this constrained circular arc.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Returns the middle point of this constrained circular arc.
  */
  OdGePoint3d midPoint() const;

  /** \details
    Checks whether the constrained circular arc is bounded and returns True when the it is bounded, 
    or False otherwise.
  */
  virtual bool isBounded() const;

  /** \details
    Sets the start point, end point, center and radius for this constrained circular arc.
  */
  OdResult set(const OdGePoint3d centerPt, const double rad, const OdGePoint3d startPt, const OdGePoint3d endPt);

  /** \details
    Sets the start point for this constrained circular arc as three-dimensional point.
  */
  OdResult setStartPoint(const OdGePoint3d& val);

  /** \details
    Sets the end point for this constrained circular arc as three-dimensional point.
  */
  OdResult setEndPoint(const OdGePoint3d& val);
};


/** \details
  This class implements a constrained ellipse node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedEllipse: public OdConstrainedCurve 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedEllipse);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedEllipse(bool bCreateImp = true);

  /** \details
    Checks whether the constrained ellipse is bounded and returns True when the it is bounded, 
    or False otherwise.
  */
  virtual bool isBounded() const;

  /** \details
    Returns the direction vector of this constrained ellipse.
  */
  OdGeVector3d direction() const;

  /** \details
    Returns the major radius of this constrained ellipse.
  */
  double majorRadius() const;

  /** \details
    Returns the minor radius of this constrained ellipse.
  */
  double minorRadius() const;

  /** \details
    Returns the center point of this constrained ellipse.
  */
  OdGePoint3d centerPoint() const;

  /** \details
    Sets the direction vector for this constrained ellipse.
  */
  OdResult setDirection(const OdGeVector3d& val); 

  /** \details
    Sets the major radius for this constrained ellipse.
  */
  OdResult setMajorRadius(const double val);

  /** \details
    Sets the minor radius for this constrained ellipse.
  */
  OdResult setMinorRadius(const double val);

  /** \details
    Sets the center point for this constrained ellipse.
  */
  OdResult setCenterPoint(const OdGePoint3d& val);

  /** \details
    Sets the center point, direction vector, major radius and minor radius for this constrained ellipse.
  */
  OdResult set(const OdGePoint3d centerPt, const OdGeVector3d dir, const double majorRad, const double minorRad);

  /** \details
    Gets the three-dimensional elliptical arc as Ge-object.
  */
  const OdGeEllipArc3d getOdGeEllipArc3d() const;

  /** \details
    Gets the original three-dimensional elliptical arc as Ge-object.
  */
  const OdGeEllipArc3d getOriginalOdGeEllipArc3d() const;
};


/** \details
  This class implements a constrained elliptical arc node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedBoundedEllipse : public OdConstrainedEllipse
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedBoundedEllipse);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedBoundedEllipse();

  /** \details
    Returns the start point of this constrained elliptical arc.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this constrained elliptical arc.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Sets the start point for this constrained elliptical arc.
  */
  OdResult setStartPoint(const OdGePoint3d& val);

  /** \details
    Sets the end point for this constrained elliptical arc.
  */
  OdResult setEndPoint(const OdGePoint3d& val);

  /** \details
    Sets the center, direction vector, major radius, minor radius, start point, and end point 
    for this constrained elliptical arc.
  */
  OdResult set(const OdGePoint3d centerPt, const OdGeVector3d dir, const double majorRad, const double minorRad, const OdGePoint3d startPt, const OdGePoint3d endPt);
protected:
  /*!DOM*/
  OdGePoint3d m_start;
  /*!DOM*/
  OdGePoint3d m_end;
};


/** \details
  This class implements a constrained spline node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedSpline: public OdConstrainedCurve 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedSpline);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedSpline();

  /** \details
    Initializes this constrained spline object.
  */
  virtual OdResult init(const OdDbObjectId depId, const OdGeNurbCurve3d& spline);

  /** \details
    Checks whether the constrained NURBS spline is bounded and returns True when the it is bounded, 
    or False otherwise.
  */
  virtual bool isBounded() const;

  /** \details
    Returns the NURBS spline for the constrained spline relative to the plane of the owning constraint group.
  */
  const OdGeNurbCurve3d nurbSpline() const;

  /** \details
    Returns the original NURBS spline for the constrained spline relative to the plane of the owning constraint group.
  */
  const OdGeNurbCurve3d originalNurbSpline() const;

  /** \details
    Returns the number of define points. If the spline is closed, the this number equals to the number 
    of control points minus 1 for the constrained spline.
  */
  int numOfDefinePoints() const;

  /** \details
    Returns the define point relative to the plane of the owning constraint group.
  */
  OdGePoint3d definePointAt(int index) const;
};


/** \details
  This class implements a constrained rigid set node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedRigidSet : public OdConstrainedGeometry
{
public:
  ODRX_DECLARE_MEMBERS(OdConstrainedRigidSet);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedRigidSet();

  /** \details
    Returns the number of constrained geometries in the group.
  */
  int numOfConstrainedGeoms() const;

  /** \details
    Returns a pointer which refers to the constrained geometry at the given index.
  */
  OdConstrainedGeometry* getConstrainedGeomAt(int index) const;

  /** \details
    Returns the transformation matrix for this rigid set relative to the plane of the owning constraint group.
  */
  OdGeMatrix3d transform() const;

  /** \details
    Applies the transformation matrix for this rigid set.
  */
  void setTransform(const OdGeMatrix3d &val);
};

#include "TD_PackPop.h"

#endif
