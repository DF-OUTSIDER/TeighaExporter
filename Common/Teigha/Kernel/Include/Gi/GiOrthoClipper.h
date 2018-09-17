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

#ifndef __ODGIORTOCLIPPER_H__
#define __ODGIORTOCLIPPER_H__

#include "Gi/GiConveyorNode.h"
#include "Ge/GeDoubleArray.h"
#include "Gi/GiClipBoundary.h"

class OdGiDeviation;
class OdGiConveyorContext;

#include "TD_PackPush.h"

/** \details
    
    This class is the base class for geometry clipper objects.
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiOrthoClipper : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiOrthoClipper);

  /** \details
    Creates a clipping stage.
    
    \param nPoints [in]  Count of points.
    \param points [in]  Points defining sides of clipping prism perpendicular to XY. 
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane.
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
    
    \remarks
    If nPoints is 0 - XY clip is disabled. If nPoints is 2 - defines a rectangle clipping prism.
  */
  virtual void set(OdUInt32 nPoints,          // points counter. 0 - disables XY clip; 2 - defines rectangle.
                   const OdGePoint2d* points, // points defining sides of clipping prism perpendicular to XY
                   bool   bClipLowerZ = false,
                   double dLowerZ = 0.0,
                   bool   bClipUpperZ = false,
                   double dUpperZ = 0.0) = 0;

  /** \details
    Creates a clipping stage.
    
    \param points [in]  Array of 2D points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane. 
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
  */
  virtual void set(const OdGePoint2dArray& points, // points defining sides of clipping prism perpendicular to XY
                   bool   bClipLowerZ = false,
                   double dLowerZ = 0.0,
                   bool   bClipUpperZ = false,
                   double dUpperZ = 0.0) = 0;

  /** \details
    Returns a clipping stage.
    
    \param points [out]  Array of points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [out]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [out]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [out]  Flag which indicates that there is an upper clipping plane.
    \param dUpperZ [out]  Z-coordinate of the upper clipping plane.
  */
  virtual void get(OdGePoint2dArray& points,
                   bool&   bClipLowerZ,
                   double& dLowerZ,
                   bool&   bClipUpperZ,
                   double& dUpperZ) const = 0;

  /** \details
    Sets max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
    Sets deviation object to obtain max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** \details 
    Sets the draw context object (to access to traits, etc).
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;
};

/** \details
  This template class is specialization of the OdSmartPtr class for the OdGiOrthoClipper class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdGiOrthoClipper> OdGiOrthoClipperPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGIORTOCLIPPER_H__
