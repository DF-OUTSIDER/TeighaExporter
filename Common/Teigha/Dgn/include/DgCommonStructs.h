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
#ifndef __DG_COMMONSTRUCTS_H__
#define __DG_COMMONSTRUCTS_H__


#include "OdaCommon.h"
#include "DgExport.h"

#include "TD_PackPush.h"


/** \details
   
    Corresponding C++ library: TG_Db

    <group Other_Classes>
*/
class TG_EXPORT OdAngleCoordinate
{
public:
  OdAngleCoordinate()
    : m_degrees(0)
    , m_minutes(0)
    , m_seconds(0)
  {
  }
  OdAngleCoordinate(OdUInt16 degrees, OdUInt8 minutes, OdUInt8 seconds)
    : m_degrees(degrees)
    , m_minutes(minutes)
    , m_seconds(seconds)
  {
  }
  ~OdAngleCoordinate()
  {
  }
  OdInt16 getDegrees() const
  {
    return m_degrees;
  }
  void setDegrees( OdUInt16 val)
  {
    m_degrees = val;
  }
  OdUInt8 getMinutes() const
  {
    return m_minutes;
  }
  void setMinutes( OdUInt8 val)
  {
    m_minutes = val;
  }
  OdUInt8 getSeconds() const
  {
    return m_seconds;
  }
  void setSeconds( OdUInt8 val)
  {
    m_seconds = val;
  }
private:
  OdInt16  m_degrees;
  OdUInt8  m_minutes;
  OdUInt8  m_seconds;
};

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLightColor
{
public:
  OdDgLightColor()
    : m_dRed(0)
    , m_dGreen(0)
    , m_dBlue(0)
    , m_dIntensityScale(0)
  {
  }
  OdDgLightColor(double red, double green, double blue, double intensityScale)
    : m_dRed(red)
    , m_dGreen(green)
    , m_dBlue(blue)
    , m_dIntensityScale(intensityScale)
  {
  }
  ~OdDgLightColor()
  {
  }
  double getRed() const
  {
    return m_dRed;
  }
  void setRed( double red)
  {
    m_dRed = red;
  }

  double getGreen() const
  {
    return m_dGreen;
  }
  void setGreen( double green)
  {
    m_dGreen = green;
  }

  double getBlue() const
  {
    return m_dBlue;
  }
  void setBlue( double blue)
  {
    m_dBlue = blue;
  }

  double getIntensityScale() const
  {
    return m_dIntensityScale;
  }
  void setIntensityScale( double intensityScale)
  {
    m_dIntensityScale = intensityScale;
  }
private:
  // r, g, b intensity scales
  double    m_dRed;
  double    m_dGreen;
  double    m_dBlue;
  // overall intensity
  double    m_dIntensityScale;
};


/** \details
   
   Basically it is used to store 3d value in UORs (without fractional part).

    Corresponding C++ library: TG_Db

    <group Other_Classes>
*/
class TG_EXPORT OdRange3d
{
public:
  OdRange3d()
    : m_x(0)
    , m_y(0)
    , m_z(0)
  {}
  OdRange3d(OdInt64 x, OdInt64 y, OdInt64 z)
    : m_x(x)
    , m_y(y)
    , m_z(z)
  {}
  OdRange3d(const OdRange3d& source) 
    : m_x(source.m_x)
    , m_y(source.m_y)
    , m_z(source.m_z) 
  {}

  OdRange3d& set(OdInt64 x, OdInt64 y, OdInt64 z)
  {
    m_x = x;
    m_y = y;
    m_z = z;
    return *this;
  }

  OdRange3d& operator  = (
    const OdRange3d& range)
  {
    m_x = range.m_x;
    m_y = range.m_y;
    m_z = range.m_z;
    return *this;
  }  

  /** \param i [in]  Index of coordinate.
    
    \returns
    Returns or references the ith coordinate of this range.

    * 0 returns or references the X-coordinate.
    * 1 returns or references the Y-coordinate.
    * 2 returns or references the Z-coordinate.
  */
  OdInt64 operator [] (unsigned int i) const
  {
    return * (&m_x+i);
  }
      
  OdInt64& operator [] (unsigned int i)
  {
    return * (&m_x+i);
  }

  OdInt64 m_x;
  OdInt64 m_y;
  OdInt64 m_z;
};

#include "TD_PackPop.h"

#endif // __DG_COMMONSTRUCTS_H__
