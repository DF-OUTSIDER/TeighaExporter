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

#ifndef OD_GEPLIN3D_H
#define OD_GEPLIN3D_H /*!DOM*/

#include "OdPlatform.h"
#include "Ge/GeSplineEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents piecewise linear splines in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGePolyline3d : public OdGeSplineEnt3d
{
public:


 /** \param points [in]  An OdGePoint3dArray of fit points.
    \param pPoints [in]  An array of fit points.
    \param numPoints [in]  The number of points in pPoints.
    \param crv [in]  A 2D curve to be approximated as a polyline.
    \param approxEps [in]  Approximate geometric tolerance. 
    \param source [in]  Object to be cloned.
 */
  OdGePolyline3d();
  OdGePolyline3d(const OdGePolyline3d& source);
  OdGePolyline3d(const OdGePoint3dArray& points);
  OdGePolyline3d(const OdGeKnotVector& knots, 
                 const OdGePoint3dArray& controlPoints);
  OdGePolyline3d(const OdGeCurve3d& crv, 
                 double approxEps);
  // TD Special
  OdGePolyline3d(OdInt32 numPoints, 
                 const OdGePoint3d* pPoints);

  /** \details
    Returns the number of fit points.
  */
   int numFitPoints() const;

  /** \details
    Returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
  */
   OdGePoint3d fitPointAt(int fitPointIndex) const;

   OdGeSplineEnt3d& setFitPointAt(int index, const OdGePoint3d& point);

  // Assignment operator.
  OdGePolyline3d& operator =(const OdGePolyline3d& polyline);

  ///////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
  Returns the point on the specified segment corresponding to the specified parameter value.

  \param param [in]  Parameter on specified segment.
  \param numSeg [in]  The segment to be queried.
  */
   OdGePoint3d evalPointSeg(
    double param, 
    int& numSeg) const;

};

#include "TD_PackPop.h"

#endif // OD_GEPLIN3D_H
