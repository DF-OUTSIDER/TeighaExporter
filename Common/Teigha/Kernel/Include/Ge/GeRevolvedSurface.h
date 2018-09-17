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



#ifndef OD_GEREVOLVEDSURFACE_H
#define OD_GEREVOLVEDSURFACE_H /*!DOM*/

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

// A revolved surface has a parametric representation of the form:
// retPnt(u, v) = P + C(v).x * (XVec * cos(u) + YVec * sin(u)) + C(v).z * A
// where
// • C(v) is the profile curve. Curve controls only 2 parameters - the height and the distance from the axis
// • P is the origin of the surface
// • A is the axis of the surface
// • XVec, YVec are the unit vectors for the rotated curve.
//
// FOR INTERNAL USE.
//

/** \details
    For Teigha internal use only.

    Library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeRevolvedSurface : public OdGeSurface
{
public:


  OdGeRevolvedSurface();
  OdGeRevolvedSurface(
    const OdGeCurve3d &pProfile, 
    const OdGePoint3d &pBase, 
    const OdGeVector3d &pAxis,
    const OdGeVector3d &pRef);

  OdGeRevolvedSurface(
    const OdGeRevolvedSurface&);

  OdGeRevolvedSurface& operator = (
    const OdGeRevolvedSurface& extCurve);

  const OdGeCurve3d& getProfile() const;

  OdGePoint3d getBasePoint() const;
  OdGeVector3d getAxis() const;
  OdGeVector3d getRef() const;
  OdGeRevolvedSurface& setAngles(double startAngle, double endAngle);
  OdGeRevolvedSurface& setRef(const OdGeVector3d &pRef);

  void set(const OdGeCurve3d &pProfile, const OdGePoint3d &pBase, const OdGeVector3d &pAxis, const OdGeVector3d &pRef);

};

#include "TD_PackPop.h"

#endif // OD_GERevolvedSurface_H

