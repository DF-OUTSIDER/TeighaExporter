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

#ifndef OD_GECSINT_H
#define OD_GECSINT_H /*!DOM*/

#include "Ge/GeEntity3d.h"

#include "TD_PackPush.h"

class OdGeCurve3d;
class OdGeSurface;
class OdGePointOnCurve3d;
class OdGePointOnSurface;

/** \details
    This class represents intersections of 3D curves and surfaces.
    Currently not implemented.

    \remarks
    The intersection object links to curve and surface objects, but does not
	contain them. Calculation of the intersection is performed by a calling 
	function, which owns the intersection output.
	
	If one of the objects is deleted, the intersection must be recalculated.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeCurveSurfInt : public OdGeEntity3d
{
public:


  /** \param curve [in]  Any 3D curve.
    \param surface [in]  Any surface.
    \param tol [in]  Geometric tolerance.
    \param source [in]  Object to be cloned.
  */
  OdGeCurveSurfInt();
  OdGeCurveSurfInt(
    const OdGeCurve3d& curve, 
    const OdGeSurface& surface,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCurveSurfInt(
    const OdGeCurveSurfInt& source);

  /** \details
    Returns the 3D curve.
  */
   const OdGeCurve3d* curve() const;

  /** \details
    Returns the surface.
  */
   const OdGeSurface* surface() const;

  /** \details
    Returns the tolerance for determining intersections.
  */
   OdGeTol tolerance() const;

  /** \details
    Returns the number of intersections between the curve and the surface,
    and returns the status of the intersection.

    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   int numIntPoints(
    OdGeIntersectError& status) const;

  /** \details
    Returns a 3D point representing the specified intersection,
    and returns the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \returns
    Returns NULL if the dimension of this intersection is not 0 (not a 3D point).
\remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   OdGePoint3d intPoint(
    int intNum, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the parameters of the specified intersection point with respect to the curve and surface,
    and returns the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param param1 [out]  Receives the parameter of the intersection point with respect to the curve.
    \param param2 [out]  Receives the parameter pair of the intersection point with respect to the surface.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   void getIntParams(
    int intNum,
    double& param1,
    OdGePoint2d& param2,
    OdGeIntersectError& status) const;

  /** \details
    Returns the intersection point on the curve, and returns the 
	status of the intersection.

    \param intNum [in]  The zero-based index of the intersection pooint to return.
    \param intPnt [out]  Receives the specified intersection point on the curve.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   void getPointOnCurve(
    int intNum, 
    OdGePointOnCurve3d& intPnt, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the intersection point on the surface, and returns the 
	status of the intersection.

    \param intNum [in]  The zero-based index of the intersection point to return.
    \param intPnt [out]  Receives the specified intersection point on the surface.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   void getPointOnSurface(
    int intNum, 
    OdGePointOnSurface& intPnt, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the configurations on either side of the surface at the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param lower [out]  Unknown.
    \param higher [out]  Unknown.
    \param smallAngle [out]  Unknown.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for lower and higher are as follows:

    @untitled table
    kXUnknown               
    kXOut                   
    kXIn                    
    kXTanOut                
    kXTanIn                 
    kXCoincident            
    kXCoincidentUnbounded   

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   void getIntConfigs(
    int intNum, 
    OdGe::csiConfig& lower, 
    OdGe::csiConfig& higher, 
    bool& smallAngle, 
    OdGeIntersectError& status) const;

  /** \details
    Specifies the 3D curve, surface, and tolerance for determining
	intersections.

    \param curve [in]  Any 3D curve.
    \param surface [in]  Any surface.
    \param tol [in]  Geometric tolerance.
  */
   OdGeCurveSurfInt& set(
    const OdGeCurve3d& cvr,
    const OdGeSurface& surface,
    const OdGeTol& tol = OdGeContext::gTol);

  OdGeCurveSurfInt& operator =(
    const OdGeCurveSurfInt& crvSurfInt);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif

