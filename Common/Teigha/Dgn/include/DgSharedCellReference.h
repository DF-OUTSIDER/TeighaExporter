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
#ifndef __DG_SHAREDCELLREFERENCE_H__
#define __DG_SHAREDCELLREFERENCE_H__


#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgSharedCellDefinition.h"

#include "Ge/GeMatrix3d.h"
#include "Ge/GePoint3d.h"


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSharedCellReference : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSharedCellReference);
public:

  //Name of the definition for reference
  OdString getDefinitionName() const;
  void setDefinitionName( OdString const& name );

  //try to find its definition
  OdDgSharedCellDefinitionPtr findDefinition() const;

  //
  //positioning
  //

  //it defines the both: rotation and scale factor
  OdGeMatrix3d getTransformation() const;
  void setTransformation( OdGeMatrix3d const& transformation );

  OdGePoint3d getOrigin() const;
  void setOrigin( OdGePoint3d const& origin );

  //
  //summary information
  //

  OdUInt16 getClassMap() const;
  void setClassMap( OdUInt16 map );

  //
  //flags
  //

  bool  getLevelOverrideFlag() const;
  void  setLevelOverrideFlag( bool value );

  bool  getRelativeOverrideFlag() const;
  void  setRelativeOverrideFlag( bool value );

  bool  getClassOverrideFlag() const;
  void  setClassOverrideFlag( bool value );

  bool  getColorOverrideFlag() const;
  void  setColorOverrideFlag( bool value );

  bool  getWeightOverrideFlag() const;
  void  setWeightOverrideFlag( bool value );

  bool  getStyleOverrideFlag() const;
  void  setStyleOverrideFlag( bool value );

  bool  getAssociativePointOverrideFlag() const;
  void  setAssociativePointOverrideFlag( bool value );

  bool  getAnnotationCellFlag() const;
  void  setAnnotationCellFlag( bool value );

  bool  getUseAnnotationScaleFlag() const;
  void  setUseAnnotationScaleFlag( bool value );

  double getAnnotationScale() const;
  void   setAnnotationScale( double value );

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr<OdDgSharedCellReference> OdDgSharedCellReferencePtr;


#include "TD_PackPop.h"

#endif // __DG_SHAREDCELLREFERENCE_H__
