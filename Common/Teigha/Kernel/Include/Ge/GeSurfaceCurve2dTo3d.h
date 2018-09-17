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




#ifndef OD_SURFACECURVE2DTO3D_H
#define OD_SURFACECURVE2DTO3D_H /*!DOM*/

#include "Ge/GeCurve3d.h"

#include "TD_PackPush.h"

/** \details
    This class provides a 3D representation for curves defined in the 2D surface parameter space.

    \remarks
    A OdGeSurfaceCurve2dTo3d object is a 3D curve, and a point of this curve at specific parameter value can be calculated by evaluating a point of the UV-curve at the corresponding parameter value:
    pointCurve3d = pSurf->evalPoint(pUVCurve->evalPoint(param));

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSurfaceCurve2dTo3d : public OdGeCurve3d
{
public:

  /** \param source [in]  The Source OdGeSurfaceCurve2dTo3d object to be cloned.
    \param uvc [in]  The 2D curve defined in parameters space of the surf surface.
    \param surf [in]  The surface that contains the uvc curve.
  */
  OdGeSurfaceCurve2dTo3d();
  OdGeSurfaceCurve2dTo3d(
    const OdGeSurfaceCurve2dTo3d& source);
  OdGeSurfaceCurve2dTo3d(
    const OdGeCurve2d& uvc, 
    const OdGeSurface& surf);

  OdGeSurfaceCurve2dTo3d& operator =(
    const OdGeSurfaceCurve2dTo3d& arc);

  /** \details
    Returns the stored UV curve.
  */
  const OdGeCurve2d* curve() const;

  /** \details
    Returns the stored parametric surface.
  */
  const OdGeSurface* surface() const;

  /** \details
    Returns the geometric extents of this curve.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents3d& extents) const;
};

#include "TD_PackPop.h"

#endif

