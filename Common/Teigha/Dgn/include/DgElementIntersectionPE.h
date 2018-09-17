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


#ifndef _OD_DG_INTERSECTIONPE_
#define _OD_DG_INTERSECTIONPE_


#include "DgGraphicsElement.h"

/** \details
    This class defines the interface for the Graphics Element Intersection Protocol Extension classes.
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgElementIntersectionPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgElementIntersectionPE);

  /** \details
    Intersects this entity with other entity if possible.
    
    \param pEnt   [in] Pointer to other entity
    \param inType [in] Determines type of intersection to be calculated
  */
  virtual OdResult intersectWith(const OdDgGraphicsElement* pThisElm,
    const OdDgGraphicsElement* pElm, OdDg::OdDgIntersectType intType,
    OdGePoint3dArray& points) = 0;

  virtual OdResult intersectWith(const OdDgGraphicsElement* pThisElm,
    const OdDgGraphicsElement* pElm, OdDg::OdDgIntersectType intType,
    const OdGePlane& projPlane, OdGePoint3dArray& points) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class
  for OdDgElementIntersectionPE object pointers.
*/
typedef OdSmartPtr<OdDgElementIntersectionPE> OdDgElementIntersectionPEPtr;

#endif //_OD_DG_INTERSECTIONPE_
