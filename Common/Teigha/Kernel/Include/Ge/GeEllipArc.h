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


#ifndef __OD_GE_ELLIP_ARC__
#define __OD_GE_ELLIP_ARC__

#include "OdRound.h"

/** \details
    Elliptical arc helper functions.

    <group TD_Namespaces>
*/
namespace OdGeEllipArc 
{  
  /** \details
    calibrates value, so that it is "near" input (not farther than OdaPi)
    (value and input should not be farther than OdaPi/2 modulo Oda2Pi, 
    for the function to work correcly)
  */
  inline double calibrateAngle(double val, double input)
  {
    if (input > val)
    {
      if ( (input - val) < OdaPI )
        return val;
      else 
        return val + OdRound((input - val)/Oda2PI)*Oda2PI;
    }
    else
    {
      if ( (val - input) < OdaPI )
        return val;
      else 
        return val - OdRound((val - input)/Oda2PI)*Oda2PI;
    }
  }

  /** \details
      Converts an elliptical parameter to an angle.
  
      \param param [in]  Elliptical parameter.
      \param radiusRatio [in]  The minorRadius:majorRadius ratio of the ellipse.
      
      \remarks
      The angle of a point on an ellipse corresponding to a parameter
      of the ellipse is determined by projecting a vector perpendicular to the
      major axis from the parameter point on the parameter circle. The angle from the major axis to the intersection
      of the vector with the ellipse is the desired angle.
  
      Corresponding C++ library: TD_Ge
  */
  inline double angleFromParam (
      double param, 
      double radiusRatio)
  {
    if(OdZero(param)) 
      return 0;
    if(OdZero(param - Oda2PI)) 
      return Oda2PI;

    return calibrateAngle(OD_ATAN2(radiusRatio*sin(param), cos(param)), param);
  }
  
  /** \details
      Converts an elliptical angle to a parameter.
      
      \param angle [in]  Elliptical angle.
      \param radiusRatio [in]  The minorRadius:majorRadius ratio of the ellipse.
      
      \remarks
      The parameter of a point on an ellipse corresponding to the
      angle of the point is determined by projecting a vector perpendicular to the
      major axis from the point on the ellipse to the parameter circle. 
      The angle from the major axis to the intersection
      of the vector with the circle is the desired parameter.

      Corresponding C++ library: TD_Ge
  */
  inline double paramFromAngle (
      double angle, 
      double radiusRatio)
  {
    if(OdZero(angle)) 
      return 0;
    if(OdZero(angle - Oda2PI)) 
      return Oda2PI;
  
    return calibrateAngle(OD_ATAN2(sin(angle), radiusRatio*cos(angle)), angle);
  }  
}

#endif // __OD_GE_ELLIP_ARC__
