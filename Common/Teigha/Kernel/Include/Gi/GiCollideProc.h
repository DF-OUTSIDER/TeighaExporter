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

#ifndef __ODGICOLLIDEPROC__
#define __ODGICOLLIDEPROC__

#include "Gi/GiConveyorNode.h"
#include "Gs/Gs.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"

class OdGiDeviation;
class OdGiConveyorContext;

#include "TD_PackPush.h"

/** \details

    <group OdGi_Classes> 
    This class implements conveyor nodes performing collision detection. 
    
    \remarks
    This conveyor node is used for implementation OdGsView::collide()
    and does not produce any output.
*/
class ODGI_EXPORT OdGiCollideProc : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiCollideProc);

  virtual void set( OdGsCollisionDetectionReactor* pReactor, const OdGsCollisionDetectionContext* pCtx = NULL ) = 0;

  /** Sets max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** Sets deviation object to obtain max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** Sets the draw context object (to access to traits, etc).
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** Enables checcking for line primitives. If disables only triangles will be checked
  */
  virtual void setLinePrimitivesChecking( bool bCheck ) = 0;
  virtual bool linePrimitivesChecking() const = 0;

  /** Additional analytic checking for
  */
  enum AnalyticPrimitives
  {
    kAnalyticNone = 0, //Disabled
    kAnaliticCircles = (1 << 0), //For circles
    kAnalyticComplexCurves = ( 1 << 1 ) // For complex curves
  };

  /** Enables analytic mode for specified primitives
  */
  virtual void enableAnalyticMode( OdUInt8 options ) = 0;
  virtual OdUInt8 analitycMode() const = 0;

  enum ProcessingPhase
  {
    kPhaseGatherInputData = 0,
    kPhaseDetectIntersections
  };

  virtual void setNoFilter( bool bNoFilter ) = 0;
  virtual bool noFilter() const = 0;

  virtual void setProcessingPhase( ProcessingPhase ) = 0;
  virtual ProcessingPhase processingPhase() const = 0;

  virtual const OdGeExtents3d& extents() const = 0;

  virtual void setInputDrawables( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize ) = 0;
  virtual void setCheckWithDrawables( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize ) = 0;

  virtual void processTriangles() = 0;
};

typedef OdSmartPtr<OdGiCollideProc> OdGiCollideProcPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGICOLLIDEPROC__
