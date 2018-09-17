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


#ifndef OD_GEGBL_H
#define OD_GEGBL_H /*!DOM*/

#include "Ge/GeTol.h"

#include "Ge/Ge.h"
#include "OdArrayPreDef.h"
#include "Ge/GePoint3dArray.h"

class OdGeSurface;
class OdGeVector3d;
class OdGePlane;
class OdGeCurve3d;
class OdGeNurbCurve2d;
class OdGeNurbCurve3d;
class OdGePoint3d;
class OdGeCurve2d;

#include "TD_PackPush.h"

/** \details
    This structure provides a namespace for tolerance values and functions ubiquitous to the OdGe library.

    \sa
    TD_Ge

    <group !!RECORDS_TD_APIRef>
*/
struct GE_TOOLKIT_EXPORT OdGeContext
{
  /** \details
    Provides the global default OdGeTol tolerance object.
    \remarks
    gTol.equalPoint == 1e-10  gTolequalVector == 1.e-10.
  */
  GE_STATIC_EXPORT static OdGeTol gTol;

  /** \details
    Provides the global 0.0 default OdGeTol tolerance object.
    \remarks
    gZeroTol.equalPoint == 0.0 and gZeroTol.equalVector == 1.e-10.
  */
  GE_STATIC_EXPORT static OdGeTol gZeroTol;

  /** \details
    A function pointer to a user-defined error handler.
    \remarks
    By default, points to a function that does nothing but return.
  */
  GE_STATIC_EXPORT static void (*gErrorFunc)(OdResult);

  /** \details
    A function pointer to a user-defined function that returns orthoVector orthogonal to vect.

    \remarks
    By default, points to a function that computes orthoVector with the arbitrary axis algorithm:

                if ( (vect.x < 0.015625) && (vect.x < 0.015625)) {
                  orthoVector.x = vect.z;
                  orthoVector.y = 0.0;
                  orthoVector.z = -vect.x;
                }
                else {
                  orthoVector.x = -vect.y;
                  orthoVector.y = vect.x;
                  orthoVector.z = 0.0;
                }
    \param vect [in]  Vector.
    \param orthoVector [out]  Receives the orthogonal vector.
  */
  GE_STATIC_EXPORT static void (*gOrthoVector) (const OdGeVector3d& vect, OdGeVector3d& orthoVector);

  /** \details
    A function pointer to a user-defined memory allocation function
    for all new operations in the OdGe library.
    \remarks
    Allows the OdGe library to use the same memory manager as the user application.
    \param size [in]  Size in bytes to allocate.
  */
  GE_STATIC_EXPORT static void* (*gAllocMem) (unsigned long size);


  /** A function pointer to a user-defined memory allocation function
    for all delete operations in the OdGe library.

    \remarks
    Allows the OdGe library to use the same memory manager as the user application.

    \param memBlock [in]  Allocated memory block to be freed.
  */
  GE_STATIC_EXPORT static void (*gFreeMem) (void* memBlock);
};

  /** \details
    Checks topology of a triangulated solid.

    \param points [in]  Array of 3D points (in order of the triangles they form).
    \param isValid [out]  Receives the validation result.
    \param tol [in]  Geometric tolerance.

    \remarks
    Array of points should be divided by 3 without remainder.

    Corresponding C++ library: TD_Ge
  */
GE_TOOLKIT_EXPORT OdGeError geValidSolid (
    const OdGePoint3dArray &points,
    bool &isValid,
    const OdGeTol &tol = OdGeContext::gTol);

  /** \details
    Calculates signed volume of a triangulated solid.

    \param points [in]  Array of 3D points (in order of the triangles they form).
    \param volume [out]  Receives the volume value.
    \param basePoint [in]  Point to replace (0,0,0) for calculation purposes.
    
    \remarks
    Array of points should be divided by 3 without remainder.
    Negative sign of volume means that triangles in the solid are ordered clockwise.

    Corresponding C++ library: TD_Ge
  */
GE_TOOLKIT_EXPORT OdGeError geSolidSignedVolume (
    const OdGePoint3dArray &points,
    double &volume,
    const OdGePoint3d *basePoint);

/** \details
    Returns the normal to the plane defined by the specified points.

    \param points [in]  Array of 3D points.
    \param numPoints [in]  Number of points.
    \param pNormal [out]  Receives the normal to the calculated plane.
    \param tol [in]  Geometric tolerance.

    \remarks
    Possible return values are as follows:

    @untitled table
    kOk
    kNonCoplanarGeometry
    kDegenerateGeometry
    kSingularPoint

    Corresponding C++ library: TD_Ge
*/
GE_TOOLKIT_EXPORT OdGeError geCalculateNormal (
    const OdGePoint3dArray& points,
    OdGeVector3d * pNormal,
    const OdGeTol& tol = OdGeContext::gTol);

GE_TOOLKIT_EXPORT OdGeError geCalculateNormal (
    const OdGePoint3d *points,
    OdUInt32 numPoints,
    OdGeVector3d * pNormal,
    const OdGeTol& tol = OdGeContext::gTol);

// Same with previous functions, but returns result as plane

/** \details
    Returns the plane defined by the specified points.

    \param points [in]  Array of 3D points.
    \param curves [in]  Array of 3D curves.
    \param numPoints [in]  Number of points.
    \param numCurves [in]  Number of curves.
    \param plane [out]  Receives the calculated plane.
    \param tol [in]  Geometric tolerance.

    \remarks
    Supported curves are OdGeCircArc3d, OdGeEllipArc3d, OdGeNurbCurve3d, and OdGeLineSeg3d.

    Possible return values are as follows:

    @untitled table
    kOk
    kNonCoplanarGeometry
    kDegenerateGeometry
    kSingularPoint

    Corresponding C++ library: TD_Ge
*/
GE_TOOLKIT_EXPORT OdGeError geCalculatePlane (
    const OdGePoint3dArray& points,
    OdGePlane& plane,
    const OdGeTol& tol = OdGeContext::gTol);

GE_TOOLKIT_EXPORT OdGeError geCalculatePlane (
    const OdGePoint3d *points,
    OdUInt32 numPoints,
    OdGePlane& plane,
    const OdGeTol& tol = OdGeContext::gTol);

GE_TOOLKIT_EXPORT OdGeError geCalculatePlane (
    const OdGeCurve3d * const* curves,
    OdUInt32 numCurves,
    OdGePlane& plane,
    const OdGeTol& tol = OdGeContext::gTol);

/** \details
    Converts a 3D NURBS curve to a 2D NURBS curve by projecting it onto the specified plane.

    \param nurb3d [in]  Any 3D NURBS curve.
    \param plane [in]  Projection plane.
    \param nurb2d [out]  Receives the 2D NURBS curve.
    \param tol [in]  Geometric tolerance.

    Corresponding C++ library: TD_Ge
*/
GE_TOOLKIT_EXPORT bool geNurb3dTo2d (
    const OdGeNurbCurve3d &nurb3d,
    OdGePlane& plane,
    OdGeNurbCurve2d &nurb2d,
    const OdGeTol& tol = OdGeContext::gTol);

/** \details
    Calculates a properties for a specified surface.

    \param pS [in]  Any surface.
    \param propU, propV [out] Receives the properties.
    \param tol [in] Geometric tolerance.

    Corresponding C++ library: TD_Ge

    \remarks
      Rational state valid only for NURB surfaces.
*/
GE_TOOLKIT_EXPORT bool geSurfaceProp(
    const OdGeSurface &pS, 
    OdGe::NurbSurfaceProperties &propU,
    OdGe::NurbSurfaceProperties &propV,
    const OdGeTol& tol);

// was moved from Examples\Common\toString
/// Convert the specified value to an OdGe::EntityId string
GE_TOOLKIT_EXPORT OdString geToString(const OdGe::EntityId val);

/** \details
    Determine is direction of 3d curve is the same as direction of curve on surface that can be gotten via 2d parametric curve.

    \param pSurf        [in]  Surface.
    \param pCurve       [in]  3d curve that lie on pSurf.
    \param pParamCurve  [in]  2d parametric curve which corresponds to pCurve on pSurf.
    \param isCoincide   [out] Coincide flag.
    \param tol          [in]  Geometric tolerance.

    \remarks
    3d curve should lie on surface with tolerance tol.

    Corresponding C++ library: TD_Ge
*/
GE_TOOLKIT_EXPORT OdResult geIsDir2dOnSurfCoincide3d(
  const OdGeSurface* pSurf,
  const OdGeCurve3d* pCurve,
  const OdGeCurve2d* pParamCurve,
  bool& isCoincide,
  const OdGeTol& tol = OdGeContext::gTol);

#include "TD_PackPop.h"

inline void GE_ERROR(OdResult res)
{
  (*OdGeContext::gErrorFunc)(res);
}

/** \details
  <group Error_Classes> 

  Corresponding C++ library: TD_Ge
*/
class GE_TOOLKIT_EXPORT OdError_GeNotImplemted : public OdError
{
public:
  OdError_GeNotImplemted(const OdString& sMessage);
};

#define GE_NOT_IMPL(pGeEnt) \
{ \
  try \
  { \
    (*OdGeContext::gErrorFunc)(eNotImplemented); \
  } \
  catch (OdError &err) \
  { \
    if (err.code() == eNotImplemented) \
    { \
      OdString sFunc(TD_FUNC); \
      OdString sMessage; \
      sMessage.format(OD_T("%s method %s for Ge type %s"), \
                      OD_T("%s"), \
                      sFunc.c_str(), \
                      geToString(pGeEnt->type()).c_str()); \
      throw OdError_GeNotImplemted(sMessage); \
    } \
    throw; \
  } \
}

#endif // OD_GEGBL_H

