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




#ifndef __ODGIWORLDDRAW_H__
#define __ODGIWORLDDRAW_H__

#include "Gi/GiCommonDraw.h"
#include "Gi/GiWorldGeometry.h"

#include "TD_PackPush.h"

/** \details
    This class defines the functionality for viewport-independent entity-level vectorization.

    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiWorldDraw : public OdGiCommonDraw
{
public:
  ODRX_DECLARE_MEMBERS(OdGiWorldDraw);

  /** \details
    Returns a reference to the OdGiWorldGeometry object associated with this object.  

    \remarks
    This instance contains the functions that can be used by an entity to
    vectorize itself.
  */
  virtual OdGiWorldGeometry& geometry() const = 0;
};


#include "TD_PackPop.h"

#endif //#ifndef __ODGIWORLDDRAW_H__

