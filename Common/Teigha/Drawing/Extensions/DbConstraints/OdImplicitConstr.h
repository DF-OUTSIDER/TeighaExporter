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

#ifndef OdImplicitConstr_INCLUDED
#define OdImplicitConstr_INCLUDED

#include "OdConstrainedGeometry.h"
#include "OdGeomConstraint.h"


#include "TD_PackPush.h"

/** \details
  This class implements a perpendicular constraint node in the owning constraint group.
  This constraint can be applied between two lines.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdPerpendicularConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdPerpendicularConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdPerpendicularConstraint();
};


/** \details
  This class implements a normal constraint node in the owning constraint group.
  This constraint can be applied between a line and circle(or arc).

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdNormalConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdNormalConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdNormalConstraint();
};


/** \details
  This class implements a coincident constraint node in the owning constraint group.
  This constraint can be applied between a point and curve.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdPointCurveConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdPointCurveConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdPointCurveConstraint();
};


/** \details
  This class implements a colinear constraint node in the owning constraint group.
  This constraint can be applied between two lines.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdColinearConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdColinearConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdColinearConstraint();
};


/** \details
  This class implements a coincident constraint node in the owning constraint group.
  This constraint can be applied between two points.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdPointCoincidenceConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdPointCoincidenceConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdPointCoincidenceConstraint();
};


/** \details
  This class implements a concentric constraint node in the owning constraint group.
  This constraint can be applied between two circles, arcs or ellipses.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConcentricConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdConcentricConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdConcentricConstraint();
};


/** \details
  This class implements a concentric constraint node in the owning constraint group.
  This constraint can be applied between two circles, arcs or ellipses.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdCenterPointConstraint: public OdConcentricConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdCenterPointConstraint);
};


/** \details
  This class implements a tangent constraint node in the owning constraint group.
  This constraint can be applied between two curves.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdTangentConstraint : public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdTangentConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdTangentConstraint();
};


/** \details
  This class implements an equal radius constraint node in the owning constraint group.
  This constraint can be applied between two circles (arcs).

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdEqualRadiusConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEqualRadiusConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdEqualRadiusConstraint();
};


/** \details
  This class implements an equal distance constraint node in the owning constraint group.
  This constraint can be applied between two pairs of points.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdEqualDistanceConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEqualDistanceConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdEqualDistanceConstraint();
};


/** \details
  This class implements an equal length constraint node in the owning constraint group.
  This constraint can be applied between two bounded lines (not rays).

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdEqualLengthConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEqualLengthConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdEqualLengthConstraint();
};


/** \details
  This class implements a parallel constraint node in the owning constraint group.
  This constraint can be applied between two lines.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdParallelConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdParallelConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdParallelConstraint();
  OdParallelConstraint(bool createImpl);
};


/** \details
  This class implements a horizontal constraint node in the owning constraint group.
  This constraint can be applied one line.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdHorizontalConstraint: public OdParallelConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdHorizontalConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdHorizontalConstraint();
};


/** \details
  This class implements a vertical constraint node in the owning constraint group.
  This constraint can be applied one line.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdVerticalConstraint: public OdParallelConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdVerticalConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdVerticalConstraint();
};


/** \details
  This class implements an equal curvature constraint node in the owning constraint group.
  This constraint can be applied between a bounded spline and a bounded curve.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdEqualCurvatureConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEqualCurvatureConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdEqualCurvatureConstraint();
};


/** \details
  This class implements a symmetric constraint node in the owning constraint group.
  This constraint can be applied between two same type of constrained geometries (except spline).

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdSymmetricConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdSymmetricConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdSymmetricConstraint();
};


/** \details
  This class implements a middle point constraint node in the owning constraint group.
  This constraint can be applied between a point and a bounded line (not ray) or arc.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdMidPointConstraint: public OdGeomConstraint 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdMidPointConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdMidPointConstraint();
};


/** \details
  This class implements a fixed constraint node in the owning constraint group.
  This constraint can be applied between on any constrained geometry.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdFixedConstraint : public OdGeomConstraint
{
public:
  ODRX_DECLARE_MEMBERS(OdFixedConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdFixedConstraint();
};


/** \details
  This class implements a smooth composite constraint node in the owning constraint group.
  This constraint can be applied between a bounded spline and other bounded curve.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdG2SmoothConstraint : public OdCompositeConstraint
{
public:
  ODRX_DECLARE_MEMBERS(OdG2SmoothConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdG2SmoothConstraint();
};

#if 0 // TODO:

/** \details
  This class implements an equal help parameter constraint node in the owning constraint group.
  This constraint can be applied between two help parameter objects which belong to the same 
  spline or ellipse.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdEqualHelpParameterConstraint : public OdGeomConstraint
{
public:
  ODRX_DECLARE_MEMBERS(OdEqualHelpParameterConstraint);

  /** \details
    Constructor creates an instance of this class.
  */
  OdEqualHelpParameterConstraint();

  /** \details
    Returns two pointers to the help parameter objects to which this constraint is connected.
  */
  OdResult getEqualHelpParameters(OdHelpParameter*& pHelpParameter1, OdHelpParameter*& pHelpParameter2) const;
};
#endif


#include "TD_PackPop.h"

#endif
