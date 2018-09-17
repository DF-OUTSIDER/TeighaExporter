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


#ifndef __ODGIPLANEPROJECTOR_H__
#define __ODGIPLANEPROJECTOR_H__

#include "Gi/GiConveyorNode.h"

#include "TD_PackPush.h"

/* This conveyor nodes works same as Xform with projection() transform (and in some cases use
   projection() matrices for transform), but this transforms works faster and calculate geometry
   normals correctly in contrast to Xform, so if you want projection to plane transformation, use
   this classes instead of Xform with projection() transform matrix.
*/

class OdGePlane;

/** \details
    Optimized version of projection on XY plane (simply removes Z-coord).

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiXYProjector : public OdGiConveyorNode
{
protected:
  OdGiXYProjector();
public:
  ODRX_DECLARE_MEMBERS(OdGiXYProjector);

  /** \details
      Enables this conveyor node.
  */
  virtual void enable() = 0;
  /** \details
      Disables this conveyor node.
  */
  virtual void disable() = 0;
  /** \details
      Returns true if this conveyor node enabled and false elsewhere.
  */
  virtual bool isEnabled() const = 0;
};

typedef OdSmartPtr<OdGiXYProjector> OdGiXYProjectorPtr;

/** \details
    Project geometry onto specified plane.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPlaneProjector : public OdGiConveyorNode
{
protected:
  OdGiPlaneProjector();
public:
  ODRX_DECLARE_MEMBERS(OdGiPlaneProjector);

  /** \details
      Enables this conveyor node.
  */
  virtual void enable() = 0;
  /** \details
      Disables this conveyor node.
  */
  virtual void disable() = 0;
  /** \details
      Returns true if this conveyor node enabled and false elsewhere.
  */
  virtual bool isEnabled() const = 0;

  /** \details
      Sets new projection plane (default projection plane is same with XY plane).
      \param plane [in]  New projection plane.
  */
  virtual void setPlane(const OdGePlane &plane) = 0;
  /** \details
      Returns current projection plane (default projection plane is same with XY plane).
  */
  virtual const OdGePlane &plane() const = 0;
  /** \details
      Returns current projection direction (default projection direction is same with vector Z-axis).
  */
  virtual const OdGeVector3d &projectionDirection() const = 0;
};

typedef OdSmartPtr<OdGiPlaneProjector> OdGiPlaneProjectorPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGIPLANEPROJECTOR_H__
