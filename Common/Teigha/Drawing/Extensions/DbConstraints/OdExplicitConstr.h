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

#ifndef OdExplicitConstraint_INCLUDED_
#define OdExplicitConstraint_INCLUDED_ /*!DOM*/

#include "OdGeomConstraint.h"
#include "Ge/GeVector3d.h"

#include "TD_PackPush.h"

/** \details
  This class implements the dimensional constraint node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdExplicitConstraint : public OdGeomConstraint
{
public:
  ODRX_DECLARE_MEMBERS(OdExplicitConstraint);

  /** \details
    Returns an object ID of the dimension dependency object referenced from the dimensional constraint node.
  */
  OdDbObjectId dimDependencyId() const;

  /** \details
    Set the object ID of the dimension dependency object to be referenced from the dimensional constraint node.
  */
  OdResult setDimDependencyId(const OdDbObjectId& dimDependencyId);

  /** \details
    Returns an object ID of the dependency object referenced from this constraint node.
  */
  OdDbObjectId valueDependencyId() const;

  /** \details
    Sets the object ID of the dependency object to be referenced from this constraint node.
  */
  OdResult setValueDependencyId(const OdDbObjectId& valueDependencyId);

  /** \details
    Returns the value of the dimensional constraint object measured from units of the constained geometries. 
  */
  OdResult getMeasuredValue(double& val) const;

  /** \details
    Updates the dimensional dependencies.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd) = 0;

protected:
  /** \details
    Constructor creates an instance of this class.
  */
  OdExplicitConstraint(){};
};


/** \details
  This class implements the distance constraint node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDistanceConstraint: public OdExplicitConstraint
{ 
public:
  ODRX_DECLARE_MEMBERS(OdDistanceConstraint);

  /** \details
    Declares the direction type for distance constraints which is used for indicating the distance 
    is measured between two geometries.
  */
  enum DirectionType
  {
    kNotDirected         = 0,    // Not directed distance. Specifies minimum distance between two geometries which is measured.
    kFixedDirection         ,    // Directed distance between two geometries with fixed direction which is measured along the fixed direction. 
    kPerpendicularToLine    ,    // Directed distance between two geometries with relative direction which is measured in the direction perpendicular to an existing constraint line.
    kParallelToLine              // Directed distance between two geometries with relative direction which is measured in the direction parallel to an existing constraint line.
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDistanceConstraint();

  //OdDistanceConstraint(const OdGeVector3d& direction);
  //OdDistanceConstraint(const OdConstraintGroupNodeId consLineId, DirectionType type = kPerpendicularToLine);

  /** \details
    Sets the fixed direction and direction type to kFixedDirection.
  */
  OdResult setDirection(const OdGeVector3d& direction);

  /** \details
    Sets the direction relative to the constrained line and direction type to kPerpendicularToLine.
  */
  OdResult setDirection(const OdConstraintGroupNodeId consLineId, DirectionType type = kPerpendicularToLine);

  /** \details
    Returns the direction type of the distance constraint.
  */
  DirectionType directionType() const;

   /** \details
    Returns the fixed direction when the direction type is set to kFixedDirection.
   */
  OdGeVector3d direction() const;

  /** \details
    Returns an ID of the constrained line when the direction type is set to kPerpendicularToLine or kParallelToLine.
  */
  OdConstraintGroupNodeId constrainedLineId() const;

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);
};


/** \details
  This class implements the angle constraint node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdAngleConstraint: public OdExplicitConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdAngleConstraint);

  /** \details
    Declares the angle sector type for angle constraints which is used for indicating 
    the angle is measured between two lines.
  */
  enum SectorType
  {
    kParallelAntiClockwise = 0,    // The angle measured from the forward direction of first line to the forward direction of second line counterclockwise.
    kAntiParallelClockwise    ,    // The angle measured from the forward direction of first line to the non forward direction of second line clockwise.
    kParallelClockwise        ,    // The angle measured from the forward direction of first line to the forward direction of second line clockwise.
    kAntiParallelAntiClockwise     // The angle measured from the forward direction of first line to the non forward direction of second line counterclockwise.
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdAngleConstraint();

  //OdAngleConstraint(OdAngleConstraint::SectorType type);

  /** \details
    Returns the angle sector type of the angle constraint.
  */
  SectorType sectorType() const;

  /** \details
    Sets the angle sector type for the angle constraint.
  */
  OdResult setSectorType(OdAngleConstraint::SectorType type);

  /** \details
    Returns the current angle factor of the angle constraint.
  */
  static double angleMultiplier();

  /** \details
    Sets the current angle factor of the angle constraint. The value is OdaPI/180 by default.
  */
  static void setAngleMultiplier(double multiplier);

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);
};


/** \details
  This class implements the angle constraint node measured between three constrained points 
  in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT Od3PointAngleConstraint: public OdAngleConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(Od3PointAngleConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  Od3PointAngleConstraint();

  //Od3PointAngleConstraint(OdAngleConstraint::SectorType type);

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);
};


/** \details
  This class implements a radius or diameter constraint node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdRadiusDiameterConstraint : public OdExplicitConstraint
{
public:
  ODRX_DECLARE_MEMBERS(OdRadiusDiameterConstraint);

  /** \details
    Declares the radius type for radius constraints which is used for indicating 
    whether radius or diameter is measured.
  */
  enum RadiusDiameterConstrType
  { 
    kCircleRadius   = 0,    // The radius of a circle or arc is measured. 
    kCircleDiameter    ,    // The diameter of a circle or arc is measured.
    kMinorRadius       ,    // The minor radius of an ellipse is measured.
    kMajorRadius            // The major radius of an ellipse is measured.
  }; 

  /** \details
    Constructor creates an instance of this class.
  */
  OdRadiusDiameterConstraint();

  /** \details
    Returns the radius type of the radius constraint.
  */
  RadiusDiameterConstrType constrType() const;

  /** \details
    Sets the radius type for the radius constraint.
  */
  OdResult setConstrType(RadiusDiameterConstrType type);

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);
};

#include "TD_PackPop.h"
#endif
