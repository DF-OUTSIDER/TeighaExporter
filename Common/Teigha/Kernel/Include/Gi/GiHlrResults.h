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



#ifndef __ODGIHLREMOVERRESULTS__
#define __ODGIHLREMOVERRESULTS__

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiHlrResults : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiHlrResults);

  class Traits {
  public:
    OdDbStub         *plotStyle;
    OdGsMarker        selMarker;
    OdCmEntityColor   color;
    OdDb::LineWeight  lineWeight;
  };

  class Data {
  public:
    Traits*           pTraits;
    OdGePoint3dArray  visible;
    OdGePoint3dArray  hidden;
  };

  /** \details
    Sets this Iterator object to reference the entry following the current entry.

    \returns
    Returns true if and only if not at the end of the list.
  */
  virtual const Data* next() = 0;

  virtual void start() = 0;
};

typedef OdSmartPtr<OdGiHlrResults> OdGiHlrResultsPtr;


#include "TD_PackPop.h"

#endif // __ODGIHLREMOVERRESULTS__
