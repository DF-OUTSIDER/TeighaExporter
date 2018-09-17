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




#ifndef OD_GEGBLGE_H
#define OD_GEGBLGE_H /*!DOM*/

#include "TD_PackPush.h"

#include "Ge/GeExport.h"
#include "Ge/GeLibVersion.h"

/** \details
  The OdGe classes are used to carry out general 2D and 3D geometric operations. 

  Corresponding C++ library: TD_Ge
  
  <group OdGe_Classes>
*/
struct OdGe
{
  static const OdGeLibVersion gLibVersion;

  enum 
  { 
    eGood, 
    eBad 
  };
  
  enum EntityId 
  {
    kEntity2d,
    kEntity3d,
    kPointEnt2d,
    kPointEnt3d,
    kPosition2d,
    kPosition3d,
    kPointOnCurve2d,
    kPointOnCurve3d,
    kPointOnSurface,
    kBoundedPlane,
    kCircArc2d,
    kCircArc3d,
    kConic2d,
    kConic3d,
    kCurve2d,
    kCurve3d,
    kEllipArc2d,
    kEllipArc3d,
    kLine2d,
    kLine3d,
    kLinearEnt2d,
    kLinearEnt3d,
    kLineSeg2d,
    kLineSeg3d,
    kPlanarEnt,
    kPlane,
    kRay2d,
    kRay3d,
    kSurface,
    kSphere,
    kCylinder,
    kTorus,
    kCone,
    kSplineEnt2d,
    kPolyline2d,
    kAugPolyline2d,
    kNurbCurve2d,
    kDSpline2d,
    kCubicSplineCurve2d,
    kSplineEnt3d,
    kPolyline3d,
    kAugPolyline3d,
    kNurbCurve3d,
    kDSpline3d,
    kCubicSplineCurve3d,
    kTrimmedCrv2d,
    kCompositeCrv2d,
    kCompositeCrv3d,
    kExternalSurface,
    kNurbSurface,
    kTrimmedSurface,
    kOffsetSurface,
    kEnvelope2d,
    kCurveBoundedSurface,
    kExternalCurve3d,
    kExternalCurve2d,
    kSurfaceCurve2dTo3d,
    kSurfaceCurve3dTo2d,
    kExternalBoundedSurface,
    kCurveCurveInt2d,
    kCurveCurveInt3d,
    kBoundBlock2d,
    kBoundBlock3d,
    kOffsetCurve2d,
    kOffsetCurve3d,
    kPolynomCurve3d,
    kBezierCurve3d,
    kObject,
    kFitData3d,
    kHatch,
    kTrimmedCurve2d,
    kTrimmedCurve3d,
    kCurveSampleData,
    kEllipCone,
    kEllipCylinder,
    kIntervalBoundBlock,
    kClipBoundary2d,
    kExternalObject,
    kCurveSurfaceInt,
    kSurfaceSurfaceInt,
    kHelix,

    kSpunSurface,
    kSegmentChain2d,

    kRevolvedSurface,
    kDummy1,
    kDummy2,
    kDummy3,
    kDummy4,
    kDummy5,
    kDummy6,
    kDummy7,
    kDummy8,
    kDummy9
  };

  enum ExternalEntityKind 
  {
    kAcisEntity              = 0, // External Entity is an ACIS entity
    kGe3dCurveEntity         = 1, // External Entity is an OdGeCurve3d* entity
    kGe3dSurfaceEntity       = 2, // External Entity is an OdGeSurface3d* entity
    kExternalEntityUndefined,      // External Entity is undefined. 
    kBimEntity
  }; 
  enum PointContainment 
  {
    kInside,              // Point is inside the boundary.
    kOutside,             // Point is outside the boundary.
    kOnBoundary           // Point on the boundary.
  };
  enum NurbSurfaceProperties 
  {
    kUninit     = 0x00,   // Uninit
    kOpen       = 0x01,   // Open
    kClosed     = 0x02,   // Closed
    kPeriodic   = 0x04,   // Periodic
    kRational   = 0x08,   // Rational
    kNoPoles    = 0x10,   // No Poles
    kPoleAtMin  = 0x20,   // Pole at Min
    kPoleAtMax  = 0x40,   // Pole at Max
    kPoleAtBoth = 0x80    // Pole at Both
  };
  
  enum OffsetCrvExtType {
    kFillet, 
    kChamfer, 
    kExtend
  };
  enum OdGeXConfig 
  {
    kNotDefined      = 1 << 0,
    kUnknown         = 1 << 1,
    kLeftRight       = 1 << 2,
    kRightLeft       = 1 << 3,
    kLeftLeft        = 1 << 4,
    kRightRight      = 1 << 5,
    kPointLeft       = 1 << 6,
    kPointRight      = 1 << 7,
    kLeftOverlap     = 1 << 8,
    kOverlapLeft     = 1 << 9,
    kRightOverlap    = 1 << 10,
    kOverlapRight    = 1 << 11,
    kOverlapStart    = 1 << 12,
    kOverlapEnd      = 1 << 13,
    kOverlapOverlap  = 1 << 14
  };

  enum BooleanType 
  {
    kUnion,                          // Union
    kSubtract,                       // Subtraction
    kCommon                          // Intersection
  }; 
  enum ClipError 
  {
    eOk,                             // OK
    eInvalidClipBoundary,            // Invalid Clip Boundary
    eNotInitialized                  // Clip Boundary was not Initialized
  };

  enum ClipCondition 
  {
    kInvalid,                        // An error occurred, probably due to invalid initialization of clipping object.
    kAllSegmentsInside,              // All segments are inside the clip boundary.
    kSegmentsIntersect,              // At least one segment crosses the clip boundary.
    kAllSegmentsOutsideZeroWinds,    // The clip boundary is outside, and not encircled by, the clip boundary.
    kAllSegmentsOutsideOddWinds,     // The clip boundary is inside, and encircled by, the clip boundary.    
    kAllSegmentsOutsideEvenWinds     // The clip boundary is outside, but, encircled by, the clip boundary.    
  };

  /**
  */
  enum ErrorCondition 
  {
    kOk,                                // OK
    k0This,                             // This object is 0.
    k0Arg1,                             // Argument 1 is 0.
    k0Arg2,                             // Argument 2 is 0.
    kPerpendicularArg1Arg2,             // Arguments 1 and 2 are perpendicular to each other.
    kEqualArg1Arg2,                     // Arguments 1 and 2 are equal.
    kEqualArg1Arg3,                     // Arguments 1 and 3 are equal.
    kEqualArg2Arg3,                     // Arguments 2 and 3 are equal.
    kLinearlyDependentArg1Arg2Arg3,     // Arguments 1, 2, and 3 are linearly dependent.
    kArg1TooBig,                        // Argument 1 is too big.
    kArg1OnThis,                        // Argument 1 is on this object.
    kArg1InsideThis,                    // Argument 1 is inside this object.
    kNonCoplanarGeometry,               // Geometry is not coplanar.
    kDegenerateGeometry,                // Geometry is degenerate.
    kSingularPoint                      // Geometry is one point.
  };

  enum csiConfig 
  {
    kXUnknown,              // Unknown.
    kXOut,                  // Transverse -- Curve neighborhood is outside this surface.
    kXIn,                   // Transverse -- Curve neighborhood is inside this surface.
    kXTanOut,               // Tangent -- Curve neighborhood is outside this surface.
    kXTanIn,                // Tangent -- Curve neighborhood is inside this surface.
    kXCoincident,           // Non-zero length -- Point is on the intersection boundary.
    kXCoincidentUnbounded   // Non-zero length -- Point is on an arbitrary *point* on an unbounded intersection.
  };

  enum ssiType 
  {
    kSSITransverse,         // Non-tangent intersection.
    kSSITangent,            // Tangent intersection with surface normals codirectional at any point within the component.
    kSSIAntiTangent         // Tangent intersection with surface normals antidirectional at any point within the component.
  };

  enum ssiConfig 
  {
    kSSIUnknown,          // Unknown.
    kSSIOut,              // Neighborhood is outside this surface.
    kSSIIn,               // Neighborhood is inside this surface.
    kSSICoincident        // Non-zero area intersection.
  };

  enum OdGeIntersectError 
  {
    kXXOk,                      // OK
    kXXIndexOutOfRange,         // Index out of range
    kXXWrongDimensionAtIndex,   // Wrong dimension at index.
    kXXUnknown                  // Unknown.
  };


  enum OdGeKnotParameterization {
    kChord                  = 0x0,   //Chord parametrization
    kSqrtChord              = 0x1,   //Chord square root parametrization
    kUniform                = 0x2,   //Uniform parametrization
    kCustomParameterization = 0xF,   //Custom parametrization
    kNotDefinedKnotParam    = 0x10   //Knot parametrization isn't defined
  };
};

/** \details
    Defines a synonym of the fully qualified name.

    \remarks
    May be used only if there are no global name conflicts.
*/
typedef OdGe::ErrorCondition OdGeError;

/** \details
    Defines a synonym of the fully qualified name.

    \remarks
    May be used only if there are no global name conflicts.
*/
typedef OdGe::OdGeIntersectError OdGeIntersectError;
typedef OdGe::OdGeKnotParameterization OdGeKnotParameterization;

#define DECLARE_GE_RTTI() \
  virtual bool isKindOf (OdGe::EntityId entType) const;\
  virtual OdGe::EntityId type() const

#define DEFINE_GE_RTTI(Class,Parent,TYPE) \
  bool Class::isKindOf(OdGe::EntityId entType) const\
  {\
    if ( entType == OdGe::TYPE ) return true;\
    return Parent::isKindOf( entType );\
  }\
  OdGe::EntityId Class::type() const { return OdGe::TYPE; }

GE_TOOLKIT_EXPORT void odgeHeapCleanup();

#include "TD_PackPop.h"

#endif  // AC_GEGBLGE_H

