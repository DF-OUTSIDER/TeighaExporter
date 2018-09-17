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

#ifndef _OdGsLight_h_Included_
#define _OdGsLight_h_Included_

#include "RxObject.h"

class OdGePoint3d;
class OdGeVector3d;

#include "TD_PackPush.h"

/** \details
    This class is the base class for Light objects in an OdGs framework.
    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class FIRSTDLL_EXPORT OdGsLight : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsLight);

  enum LightType
  {
    kPointLight      = 0,
    kDistantLight    = 1,
    kSpotlight       = 2
  };

  enum LightAttenuation
  {
    kAttenNone            = 0,
    kAttenInverseLinear   = 1,
    kAttenInverseSquare   = 2
  };

  /** \details
    Returns the type of this Light object.
    
    \remarks
    type() returns one of the following:
    
    <table>
    Name             Value
    kPointLight      0
    kDistantLight    1
    kSpotlight       2
    </table>
  */
  virtual LightType type() const = 0;
  
  /** \details
    Returns the WCS position of this Light object. 
  */
  virtual OdGePoint3d position() const = 0;

  /** \details
    Returns the WCS target of this Light object. 
  */
  virtual OdGePoint3d target() const = 0;
  /** \details
    Returns the WCS direction of this Light object. 
  */
  virtual OdGeVector3d direction() const = 0;
  /** \details
    Returns the color of this Light object. 
  */
  virtual ODCOLORREF color() const = 0;
  /** \details
    Returns the intensity of this Light object. 
  */
  virtual double intensity() const = 0;
  /** \details
    Returns the attenuation of this Light object. 
    
    \remarks
    attenuation() returns one of the following:
    
    <table>
    Name                  Value      
    kAttenNone            0          
    kAttenInverseLinear   1
    kAttenInverseSquare   2
    </table>
  */
  virtual LightAttenuation attenuation() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsLight object pointers.
*/
typedef OdSmartPtr<OdGsLight> OdGsLightPtr;

#include "TD_PackPop.h"

#endif // _OdGsLight_h_Included_
