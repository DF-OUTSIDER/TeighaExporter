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
#ifndef __DG_POINT_STRING_H__
#define __DG_POINT_STRING_H__

#include "TD_PackPush.h"

#include "DgMultiVertex.h"
#include "Ge/GeQuaternion.h"


//2D subtype of Line String

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgPointString2d : public OdDgMultiVertex2d
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgPointString2d);
public:

  //
  //methods below are destined to support the points with rotation
  //

  virtual void          addVertex     ( const OdGePoint2d& point, const OdGeMatrix2d& rotation );

  virtual OdGeMatrix2d  getRotationAt ( OdUInt32 index ) const;
  virtual void          setRotationAt ( OdUInt32 index, const OdGeMatrix2d& rotation );

  // OdGiDrawable overridden
  bool subWorldDraw( OdGiWorldDraw* pWd ) const;

  // OdDgPointString2d

  // Point String can be defined as either continuous or disjoint. 
  //        - true is Continuous
  //        - false is Disjoint
  virtual bool getContinuousFlag() const;
  virtual void setContinuousFlag(bool flag);
};

typedef OdSmartPtr<OdDgPointString2d> OdDgPointString2dPtr;


//3D subtype of Point String

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgPointString3d : public OdDgMultiVertex3d
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgPointString3d);
public:

  //
  //methods below are destined to support the points with rotation
  //

  virtual void        addVertex     ( const OdGePoint3d& point, const OdGeQuaternion& rotation );

  virtual OdGeQuaternion  getRotationAt ( OdUInt32 index) const;
  virtual void            setRotationAt ( OdUInt32 index, const OdGeQuaternion& rotation );

  // OdGiDrawable overridden
  bool subWorldDraw( OdGiWorldDraw* pWd ) const;

  // OdDgPointString3d


  // Point String can be defined as either continuous or disjoint. 
  //        - 0 is Continuous
  //        - 1 is Disjoint
  virtual bool getContinuousFlag() const;
  virtual void setContinuousFlag(bool flag);
};

typedef OdSmartPtr<OdDgPointString3d> OdDgPointString3dPtr;


#include "TD_PackPop.h"

#endif // __DG_POINT_STRING_H__
