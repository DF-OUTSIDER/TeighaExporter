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

#ifndef OD_GEPENT3D_H
#define OD_GEPENT3D_H /*!DOM*/

#include "Ge/GeEntity3d.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for all OdGe 3D point classes.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGePointEnt3d : public OdGeEntity3d
{
public:


  /** \details
    Returns this object as an OdGePoint3d.
  */
   OdGePoint3d point3d() const;

  // Conversion operator to convert to OdGePoint3d.
   operator OdGePoint3d() const;

  // Assignment operator.
  OdGePointEnt3d& operator =(const OdGePointEnt3d& ent);

  ///////////////////////////////////////////////////////////////////////////

protected:
  OdGePointEnt3d();
  OdGePointEnt3d(
    const OdGePointEnt3d& ent);
};

#include "TD_PackPop.h"

#endif // OD_GEPENT3D_H

