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



#ifndef OD_GESPUNSURF_H
#define OD_GESPUNSURF_H /*!DOM*/

//
/** \details
*/

#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"
#include "OdPlatformSettings.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeCircArc3d;
class OdGeLinearEnt3d;

#include "TD_PackPush.h"

// A spun surface has a parametric representation of the form:
// R( u, v ) = Z( u ) + ( C( u ) - Z( u ))cos( v ) + A X ( C( u ) - Z( u ) ) sin( v)
// where
// • C(u) is the profile curve
// • Z(u) is the projection of C(u) onto the spin axis
// • A is the spin axis direction (unit vector)
// • C must not be an intersection curve or a trimmed curve
// NOTE: Z(u) = P + ( ( C( u ) - P ) . A )A where P is a reference point on the axis.
//
// FOR INTERNAL USE.
//

/** \details
    For Teigha internal use only.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSpunSurf : public OdGeSurface
{
public:


  OdGeSpunSurf();
  OdGeSpunSurf(
  const OdGeCurve3d &pProfile, 
  const OdGePoint3d &pBase, 
  const OdGeVector3d &pAxis);

  OdGeSpunSurf(
    const OdGeSpunSurf&);

  OdGeSpunSurf& operator = (
    const OdGeSpunSurf& extCurve);

  const OdGeCurve3d *getProfile() const;

  OdGePoint3d getBasePoint() const;
  OdGeVector3d getAxis() const;
  OdGeVector3d getRef() const;
  OdGeSpunSurf& setAngles(double startAngle, double endAngle);
  OdGeSpunSurf& setRef(const OdGeVector3d &pRef);

  void set(const OdGeCurve3d &pProfile, const OdGePoint3d &pBase, const OdGeVector3d &pAxis);

};

#include "TD_PackPop.h"

#endif // OD_GESPUNSURF_H

