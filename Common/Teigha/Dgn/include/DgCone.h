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
#ifndef __DG_CONE_H__
#define __DG_CONE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCone : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgCone);
public:

  enum ConeType
  {
    kGeneralCone = 0,
    kRightCylinder,
    kCylinder,
    kRightCone,
    kCone,
    kRightTruncatedCone,
    kTruncatedCone
  };

  virtual OdGePoint3d getCenter1() const;
  virtual OdGePoint3d getCenter2() const;

  virtual void setCenter1( const OdGePoint3d& pt );
  virtual void setCenter2( const OdGePoint3d& pt );

  virtual double getRadius1() const;
  virtual double getRadius2() const;

  virtual void setRadius1( double dVal );
  virtual void setRadius2( double dVal );

  virtual OdGeQuaternion getRotation() const;
  virtual void setRotation( const OdGeQuaternion& rotation );

  virtual bool isSolid() const;
  virtual void setSolid(bool bVal = true);

  virtual ConeType getConeType() const;
  virtual void setConeType( ConeType type );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw *pVd) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  // OdDgCone
  /** \details
    The flag indicates whether the element is a solid or a hole for closed element types:
      * false is Solid
      * true is Hole
  */
  virtual bool getHoleFlag() const;
  virtual void setHoleFlag(bool flag);
};

typedef OdSmartPtr<OdDgCone> OdDgConePtr;

#include "TD_PackPop.h"

#endif // __DG_CONE_H__
