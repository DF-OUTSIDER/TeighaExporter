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
#ifndef __DG_SECTIONCLIP_H__
#define __DG_SECTIONCLIP_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSectionClip : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSectionClip);
public:

  virtual bool getPerspectiveUpFlag() const;
  virtual void setPerspectiveUpFlag( bool bSet );

  virtual bool getLeftCropFlag() const;
  virtual void setLeftCropFlag( bool bSet );

  virtual bool getRightCropFlag() const;
  virtual void setRightCropFlag( bool bSet );

  virtual bool getTopCropFlag() const;
  virtual void setTopCropFlag( bool bSet );

  virtual bool getBottomCropFlag() const;
  virtual void setBottomCropFlag( bool bSet );

  virtual bool getFrontCropFlag() const;
  virtual void setFrontCropFlag( bool bSet );

  virtual bool getBackCropFlag() const;
  virtual void setBackCropFlag( bool bSet );

  virtual double getFrontDepth() const;
  virtual void setFrontDepth( double dDepth );

  virtual double getBackDepth() const;
  virtual void setBackDepth( double dDepth );

  virtual double getTopHeight() const;
  virtual void setTopHeight( double dHeight );

  virtual double getBottomHeight() const;
  virtual void setBottomHeight( double dHeight );

  virtual OdGeMatrix3d getRotation() const;
  virtual void setRotation( const OdGeMatrix3d& matRotation );

  virtual OdGePoint3d getOrigin() const;
  virtual void setOrigin( const OdGePoint3d& ptOrigin );

  virtual double getClipWidth() const;
  virtual void setClipWidth( double dWidth );

  virtual OdString getName() const;
  virtual void setName( const OdString& strName );
  
  bool  subWorldDraw( OdGiWorldDraw* pWd ) const;

  virtual void getClipBox( OdGePoint3dArray& arrBoxPts ) const;
};

typedef OdSmartPtr<OdDgSectionClip> OdDgSectionClipPtr;

#include "TD_PackPop.h"

#endif // __DG_SECTIONCLIP_H__
