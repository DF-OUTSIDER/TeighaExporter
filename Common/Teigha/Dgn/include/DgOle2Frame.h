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
#ifndef __DG_OLE2FRAME_H__
#define __DG_OLE2FRAME_H__

#include "TD_PackPush.h"

#include "DgCellHeader.h"
#include "DgLight.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"

class OdOleItemHandler;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgOle2Frame
{

public:

  enum Type
  {
    k2dOleFrame  = OdDgWhiteMaterial::kLastType + 1,
    k3dOleFrame  = OdDgWhiteMaterial::kLastType + 2,
    kLastType    = k3dOleFrame
  };

  enum OdDgOleObjectType
  {
    kUnknown  = 0,
    kLinked   = 1,
    kEmbedded = 2,
    kPicture  = 3
  };

  enum OdDgDrawAspect
  {
    kContent   = 1,
    kThumbnail = 2,
    kIcon      = 4,
    kDocPrint  = 8
  };

  enum OdDgOleViewRotationMode
  {
    kViewDependent     = 0,
    kViewIndependent   = 1,
    kViewProjection    = 2
  };

public:

  virtual OdDgOleObjectType getOleType() const = 0;
  virtual void setOleType( OdDgOleObjectType uType ) = 0;

  virtual OdDgDrawAspect getDrawAspect() const = 0;
  virtual void setDrawAspect( OdDgDrawAspect uAspect ) = 0;

  virtual OdDgOleViewRotationMode getViewRotationMode() const = 0;
  virtual void setViewRotationMode( OdDgOleViewRotationMode uMode) = 0;

  virtual  double  getObjectSizeX() const = 0;
  virtual void    setObjectSizeX( double dSizeX ) = 0;

  virtual double  getObjectSizeY() const = 0;
  virtual void    setObjectSizeY( double dSizeX ) = 0;

  virtual bool    getAspectRatioFlag() const = 0;
  virtual void    setAspectRatioFlag( bool bSet ) = 0;

  virtual bool    getTransparentBackgroundFlag() const = 0;
  virtual void    setTransparentBackgroundFlag( bool bSet ) = 0;

  virtual bool    getCanBeLinkedAndEmbeddedFlag() const = 0;
  virtual void    setCanBeLinkedAndEmbeddedFlag( bool bSet ) = 0;

  virtual bool    getCanBePictureFlag() const = 0;
  virtual void    setCanBePictureFlag( bool bSet ) = 0;

  virtual OdUInt32 getCompoundDocumentDataSize() const = 0;

  virtual void getCompoundDocument( OdStreamBuf& streamBuf) const = 0;
  virtual void setCompoundDocument( OdUInt32 nSize, OdStreamBuf& streamBuf) = 0;

  virtual const OdOleItemHandler* itemHandler() const = 0;
  virtual OdOleItemHandler* getItemHandler() = 0;

  virtual double getXScale() const = 0;
  virtual double getYScale() const = 0;
  virtual void   setScale( double dXScale, double dYScale ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgOle2Frame object pointers.
*/
typedef OdSmartPtr<OdDgOle2Frame> OdDgOle2FramePtr;

/** \details
  This class declares the methods for managing 2d ole element.

  Corresponding C++ library: TG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgOle2Frame2d : public OdDgCellHeader2d, public OdDgOle2Frame
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgOle2Frame2d);

  virtual OdDgOle2Frame::OdDgOleObjectType getOleType() const;
  virtual void setOleType( OdDgOle2Frame::OdDgOleObjectType uType );

  virtual OdDgOle2Frame::OdDgDrawAspect getDrawAspect() const;
  virtual void setDrawAspect( OdDgOle2Frame::OdDgDrawAspect uAspect );

  virtual OdDgOle2Frame::OdDgOleViewRotationMode getViewRotationMode() const;
  virtual void setViewRotationMode( OdDgOle2Frame::OdDgOleViewRotationMode uMode);

  virtual double  getObjectSizeX() const;
  virtual void    setObjectSizeX( double dSizeX );

  virtual double  getObjectSizeY() const;
  virtual void    setObjectSizeY( double dSizeX );

  virtual bool    getAspectRatioFlag() const;
  virtual void    setAspectRatioFlag( bool bSet );

  virtual bool    getTransparentBackgroundFlag() const;
  virtual void    setTransparentBackgroundFlag( bool bSet );

  virtual bool    getCanBeLinkedAndEmbeddedFlag() const;
  virtual void    setCanBeLinkedAndEmbeddedFlag( bool bSet );

  virtual bool    getCanBePictureFlag() const;
  virtual void    setCanBePictureFlag( bool bSet );

  virtual OdUInt32 getCompoundDocumentDataSize() const;

  virtual void getCompoundDocument( OdStreamBuf& streamBuf) const;
  virtual void setCompoundDocument( OdUInt32 nSize, OdStreamBuf& streamBuf);

  virtual const OdOleItemHandler* itemHandler() const;
  virtual OdOleItemHandler* getItemHandler();

  virtual void setItemHandlerModified( bool bSet );

  virtual OdGePoint2d getPosition() const;
  virtual void setPosition( const OdGePoint2d& ptPosition );

  virtual double getXScale() const;
  virtual double getYScale() const;
  virtual void   setScale( double dXScale, double dYScale );

  virtual double getRotationAngle() const;
  virtual void   setRotationAngle( double dAngle );

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw( OdGiViewportDraw* pVd ) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDgOle2Frame2d object pointers.
*/
typedef OdSmartPtr<OdDgOle2Frame2d> OdDgOle2Frame2dPtr;

/** \details
  This class declares the methods for managing 3d ole element.

  Corresponding C++ library: TG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgOle2Frame3d : public OdDgCellHeader3d, public OdDgOle2Frame
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgOle2Frame3d);

  virtual OdDgOle2Frame::OdDgOleObjectType getOleType() const;
  virtual void setOleType( OdDgOle2Frame::OdDgOleObjectType uType );

  virtual OdDgOle2Frame::OdDgDrawAspect getDrawAspect() const;
  virtual void setDrawAspect( OdDgOle2Frame::OdDgDrawAspect uAspect );

  virtual OdDgOle2Frame::OdDgOleViewRotationMode getViewRotationMode() const;
  virtual void setViewRotationMode( OdDgOle2Frame::OdDgOleViewRotationMode uMode);

  virtual double  getObjectSizeX() const;
  virtual void    setObjectSizeX( double dSizeX );

  virtual double  getObjectSizeY() const;
  virtual void    setObjectSizeY( double dSizeX );

  virtual bool    getAspectRatioFlag() const;
  virtual void    setAspectRatioFlag( bool bSet );

  virtual bool    getTransparentBackgroundFlag() const;
  virtual void    setTransparentBackgroundFlag( bool bSet );

  virtual bool    getCanBeLinkedAndEmbeddedFlag() const;
  virtual void    setCanBeLinkedAndEmbeddedFlag( bool bSet );

  virtual bool    getCanBePictureFlag() const;
  virtual void    setCanBePictureFlag( bool bSet );

  virtual OdUInt32 getCompoundDocumentDataSize() const;

  virtual void getCompoundDocument( OdStreamBuf& streamBuf) const;
  virtual void setCompoundDocument( OdUInt32 nSize, OdStreamBuf& streamBuf);

  virtual const OdOleItemHandler* itemHandler() const;
  virtual OdOleItemHandler* getItemHandler();

  virtual void setItemHandlerModified( bool bSet );

  virtual OdGePoint3d getPosition() const;
  virtual void setPosition( const OdGePoint3d& ptPosition );

  virtual double getXScale() const;
  virtual double getYScale() const;
  virtual void   setScale( double dXScale, double dYScale );

  virtual OdGeQuaternion getRotation() const;
  virtual void   setRotation( const OdGeQuaternion& rotAngle );

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw( OdGiViewportDraw* pVd ) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDgOle2Frame3d object pointers.
*/
typedef OdSmartPtr<OdDgOle2Frame3d> OdDgOle2Frame3dPtr;

#include "TD_PackPop.h"

#endif // __DG_OLE2FRAME_H__
