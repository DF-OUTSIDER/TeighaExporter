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

#ifndef OD_GE_SEGMENTCHAIN_2D_H
#define OD_GE_SEGMENTCHAIN_2D_H /*!DOM*/

#include "OdPlatform.h"
#include "Ge/GeSplineEnt2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents piecewise linear splines in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSegmentChain2d : public OdGeSplineEnt2d
{
public: 
  OdGeSegmentChain2d();
  /** \param source [in]  Object to be cloned.
  */
  OdGeSegmentChain2d(
    const OdGeSegmentChain2d& source);
  /** \param points [in]  Array of fit points.
    \param knots [in]  Knot vector.
    \param crv [in]  A 2D curve to be approximated as a polyline.
    \param approxEps [in]  Approximate geometric tolerance. 
  */
  OdGeSegmentChain2d(
    const OdGePoint2dArray& fitpoints);
  OdGeSegmentChain2d(
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& points);
  OdGeSegmentChain2d(
    const OdGeCurve2d& crv, 
    double approxEps);

  OdGeSegmentChain2d& operator =(const OdGeSegmentChain2d& pline);

  //////////////////////////////////////////////////////////////////////////
  // overrides

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Closes or opens this polyline.

    \param flag [in]  Boolean indicating if this polyline is to be closed.
  */
  OdGeSegmentChain2d& setClosed(
    bool flag);

  /** \details
    Returns a reference to the bulge vector for this polyline.

    \remarks
    The bulge is the tangent of 1/4 the included angle of the arc,
    measured counterclockwise.
  */
  OdGeDoubleArray& bulges();

  /** \details
    Returns the bulge vector for this polyline.

    \remarks
    The bulge is the tangent of 1/4 the included angle of the arc,
    measured counterclockwise.
  */
  const OdGeDoubleArray& getBulges() const;

  /** \details
    Returns a reference to the vertices for this polyline.
  */
  OdGePoint2dArray& vertices();

  /** \details
    Returns the vertices for this polyline.
  */
  const OdGePoint2dArray& getVertices() const;

  /** \details
    Returns true in and only if this polyline has at least one non-zero bulge.
  */
  bool hasBulges() const;
};

#include "TD_PackPop.h"

#endif // OD_GE_SEGMENTCHAIN_2D_H

