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

#ifndef __ODGISPATIALFILTER__
#define __ODGISPATIALFILTER__

#include "Gi/GiConveyorNode.h"

#include "Ge/GeDoubleArray.h"
#include "Ge/GeExtents2d.h"

#include "TD_PackPush.h"

class OdGiDeviation;
class OdGiConveyorContext;

/** \details

    Conveyor member performing spatial filtering by 3D rectangular prism.

    * exts        - base of the prism.
    * bClipLowerZ - if lower boundary exists.
    * dLowerZ     - lower boundary.
    * bClipUpperZ - if upper boundary exists.
    * dUpperZ     - upper boundary.

    <group OdGi_Classes> 
*/

/** \details
    This class defines an interface for spatial filtering by 3D rectangular prisms.
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiSpatialFilter : public OdRxObject
{
protected:
  OdGiSpatialFilter();

public:
  ODRX_DECLARE_MEMBERS(OdGiSpatialFilter);

  virtual OdGiConveyorInput& input() = 0;
  virtual OdGiConveyorOutput& insideOutput() = 0;
  virtual OdGiConveyorOutput& intersectsOutput() = 0;
  virtual OdGiConveyorOutput& disjointOutput() = 0;

  // set output conveyor geometry to this member, if it is needn't

  ODGI_EXPORT_STATIC static OdGiConveyorGeometry& kNullGeometry;

  virtual void set(const OdGeExtents2d& exts,
                   bool bClipLowerZ = false,
                   double dLowerZ = 0.0,
                   bool   bClipUpperZ = false,
                   double dUpperZ = 0.0) = 0;

  virtual void get(OdGeExtents2d& exts,
                   bool&   bClipLowerZ,
                   double& dLowerZ,
                   bool&   bClipUpperZ,
                   double& dUpperZ) const = 0;

  virtual bool isSimplifyOpt(OdUInt32 opt) const = 0;

  /** Sets the draw context object (to access to traits, etc).
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;
};

typedef OdSmartPtr<OdGiSpatialFilter> OdGiSpatialFilterPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGISPATIALFILTER__
