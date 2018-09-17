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



#ifndef __OD_GI_H_L_REMOVER__
#define __OD_GI_H_L_REMOVER__

#include "Ge/GeDoubleArray.h"
#include "Gi/GiConveyorNode.h"

class OdGiDeviation;

#include "TD_PackPush.h"

class OdGiHlrResults;
typedef OdSmartPtr<OdGiHlrResults> OdGiHlrResultsPtr;


/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiHLRemover : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiHLRemover);

  /** Sets max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** Sets deviation object to obtain max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** Sets the draw context object (to access to traits, etc).
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** Computes and pulls scene with hidden lines removed 
  */
  virtual void process() = 0;

  virtual void enable(bool enabled) = 0;

  virtual bool enabled() const = 0;

  virtual void invertZ(bool bDoIt) = 0;

  virtual bool zInverted() const = 0;

  virtual OdGiHlrResultsPtr hlrResults(unsigned int* pNumItems = 0) const = 0;

  virtual void freeResults() = 0;

  virtual void keepHidden(bool bDoIt) = 0;

  virtual bool hiddenKept() const = 0;
};

typedef OdSmartPtr<OdGiHLRemover> OdGiHLRemoverPtr;

#include "TD_PackPop.h"

#endif // __OD_GI_H_L_REMOVER2__
