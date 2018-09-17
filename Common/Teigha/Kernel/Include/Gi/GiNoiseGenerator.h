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

// Perlin noise generation for procedural textures.

#ifndef __ODGINOISEGENERATOR_H__
#define __ODGINOISEGENERATOR_H__

#include "RootExport.h"
#include "GiExport.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    Noise generator for procedural textures generation (based on Perlin noise).

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiNoiseGenerator : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiNoiseGenerator);

    /** \details
      Initialize noise generation tables to specified seed.
      \param seed [in]  Random number generator seed.
    */
    virtual void initSeed(OdUInt32 seed) = 0;

    /** \details
      Generate noise value for a 1D location.
      Returns values between -1.0 and 1.0.
      \param point [in]  1D point location.
    */
    virtual double noise1d(double point) const = 0;

    /** \details
      Generate noise value for a 2D location.
      Returns values between -1.0 and 1.0.
      \param point [in]  2D point location.
    */
	  virtual double noise2d(const OdGePoint2d &point) const = 0;

    /** \details
      Generate noise value for a 3D location.
      Returns values between -1.0 and 1.0.
      \param point [in]  3D point location.
    */
    virtual double noise3d(const OdGePoint3d &point) const = 0;

    /** \details
      General noise function calls the previous 3 depending on len.
      Input range from 0 to 3.
      Returns values between -1.0 and 1.0.
      \param point [in]  1D, 2D or 3D point location.
      \param len [in]  Point dimensions.
    */
    virtual double noise(double point[], OdInt32 len) const = 0;

    /** \details
      Generate turbulent noise across a number of octaves.
      Returns values between -1.0 and 1.0.
      \param point [in]  3D input point location.
      \param freq [in]  Turbulence frequency.
    */
    virtual double turbulence(const OdGePoint3d &point, double freq) const = 0;

    /** \details
      Noise across a number of octaves.
      Returns values between -1.0 and 1.0.
      \param point [in]  3D input point location.
      \param freq [in]  Turbulence frequency.
    */
    virtual double oNoise(const OdGePoint3d &point, double freq) const = 0;

    /** \details
      Fractal brownian motion.
      Returns values between -1.0 and 1.0.
      \param point [in]  3D input point location.
      \param H [in]  Range fractal dimension 1.0 - 0.0 where 0.0 is total chaos.
      \param lacunarity [in]  Gap between successive frequencies, use 2.0.
      \param octaves [in]  Number of frequencies in the fBm. 3 - 7.
    */
    virtual double fBm(const OdGePoint3d &point, double H, double lacunarity, double octaves) const = 0;

    /** \details
      Create OdGiNoiseGenerator, initialized by specified seed.
      \param seed [in]  Random number generator seed.
    */
    static OdSmartPtr<OdGiNoiseGenerator> createObject(OdUInt32 seed);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiNoiseGenerator object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiNoiseGenerator> OdGiNoiseGeneratorPtr;

#include "TD_PackPop.h"

#endif // __ODGINOISEGENERATOR_H__
