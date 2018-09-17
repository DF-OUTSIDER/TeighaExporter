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

#ifndef _ODGSBACKGROUND_H_INCLUDED_
#define _ODGSBACKGROUND_H_INCLUDED_

#include "RxObject.h"

class OdGsBaseVectorizer;
class OdGsPropertiesDirectRenderOutput;
class OdGiBackgroundTraitsData;
class OdGiDrawable;

#include "TD_PackPush.h"

/** \details
    Corresponding C++ library: TD_Gs
    <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsNestedBackground
{
  public:
    virtual void nestedBackgroundDisplay(OdGsBaseVectorizer& view,
        const OdGiDrawable *pDrawable,
        OdGiBackgroundTraitsData* pBackgroundTraits,
        OdGsPropertiesDirectRenderOutput *pdro = NULL) = 0;
};

/** \details
    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBackground : public OdRxObject
                                       , public OdGsNestedBackground
{
public:
  ODRX_DECLARE_MEMBERS(OdGsBackground);

  enum BackgroundType
  {
    kSolidBackground       = 0,
    kGradientBackground    = 1,
    kImageBackground       = 2,
    kGroundPlaneBackground = 3,
    kSkyBackground         = 4,
    kIBLBackground         = 5
  };

  virtual BackgroundType type() const = 0;

  virtual void display(OdGsBaseVectorizer& view,
      const OdGiDrawable *pDrawable,
      OdGiBackgroundTraitsData* pBackgroundTraits,
      OdGsPropertiesDirectRenderOutput *pdro = NULL) = 0;

  virtual void nestedBackgroundDisplay(OdGsBaseVectorizer& view,
    const OdGiDrawable *pDrawable,
    OdGiBackgroundTraitsData* pBackgroundTraits,
    OdGsPropertiesDirectRenderOutput *pdro = NULL)
  {
    display(view, pDrawable, pBackgroundTraits, pdro);
  }

  virtual void setNestedBackground(OdGsNestedBackground * /*pNestedBackground*/) { }
  virtual OdGsNestedBackground *getNestedBackground() const { return NULL; }

  static OdSmartPtr<OdGsBackground> createObject(BackgroundType type);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsBackground object pointers.
*/
typedef OdSmartPtr<OdGsBackground> OdGsBackgroundPtr;

#include "TD_PackPop.h"

#endif // _ODGSBACKGROUND_H_INCLUDED_
