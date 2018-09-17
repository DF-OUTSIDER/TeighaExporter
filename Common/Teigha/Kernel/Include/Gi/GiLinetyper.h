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




#ifndef __ODGILINETYPER_H__
#define __ODGILINETYPER_H__


#include "Gi/GiConveyorNode.h"
#include "Gi/GiNonEntityTraits.h"
#include "Ge/GeDoubleArray.h"

class OdGiDeviation;
class OdGiConveyorGeometry;

#include "TD_PackPush.h"


/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiLinetyper : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiLinetyper);

  virtual void setDrawContext( OdGiConveyorContext* pDrawCtx ) = 0;

  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  virtual OdUInt32 setLinetype(OdDbStub* id, double scale, double generationCriteria = 0.) = 0;

  virtual OdGiLinetypeTraits& linetypeTraits() const = 0;

  virtual void enable() = 0;

  virtual bool enabled() const = 0;

  virtual void disable() = 0;
  
  // makes linetyper to linetype circle curves (circles, circular arcs) 
  // analytically or using breaking to polyline.
  // bAnalytic = true turns on analytic linetyper
  // bAnalytic = false turns off analytic linetyper
  virtual void setAnalyticLinetypingCircles(bool bAnalytic) = 0;  
  virtual bool isAnalyticLinetypingCircles() const = 0;
  
  // makes linetyper to linetype complex curves (ellipses, elliptic arcs, nurbs curves) 
  virtual void setAnalyticLinetypingComplexCurves(bool bAnalytic) = 0;  
  virtual bool isAnalyticLinetypingComplexCurves() const = 0;

  virtual OdGiConveyorGeometry* redirectionGeometry() = 0;

  virtual void enableCache() = 0;
  virtual void disableCache() = 0;
  virtual bool cacheEnabled() const = 0;
  virtual void clearCache() = 0;
};

typedef OdSmartPtr<OdGiLinetyper> OdGiLinetyperPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGILINETYPER_H__
