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
#ifndef __DG_RASTER_H__
#define __DG_RASTER_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgElementIterator.h"

#include "Ge/GePoint3d.h"

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class OdDgRaster
{
public:
  OdDgRaster()
  {}
  virtual ~OdDgRaster()
  {}
 
  enum RasterFormat
  {
      kBitmap    = 1 
    , kByteData  = 2
    , kBinaryRLE = 9
    , kByteRLE   = 24
  };

  virtual bool getRightJustifiedFlag() const = 0;
  virtual void setRightJustifiedFlag( bool value ) = 0;

  virtual bool getLowerJustifiedFlag() const = 0;
  virtual void setLowerJustifiedFlag( bool value ) = 0;

  virtual bool getHorizontalDataFlag() const = 0;
  virtual void setHorizontalDataFlag( bool value ) = 0;

  virtual bool getColorFlag() const = 0;
  virtual void setColorFlag( bool value ) = 0;

  virtual bool getTransparentFlag() const = 0;
  virtual void setTransparentFlag( bool value ) = 0;

  virtual bool getPositiveFlag() const = 0;
  virtual void setPositiveFlag( bool value ) = 0;

  //format of raster
  virtual RasterFormat getFormat() const = 0;
  virtual void setFormat( RasterFormat format ) = 0;

  //
  //colors for monochrome rasters
  //

  virtual OdUInt8 getForeground() const = 0;
  virtual void setForeground( OdUInt8 color ) = 0;

  virtual OdUInt8 getBackground() const = 0;
  virtual void setBackground( OdUInt8 color ) = 0;

};

//raster header (2D sub-type)

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgRasterHeader2d : public OdDgGraphicsElement
                                   , public OdDgRaster
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgRasterHeader2d );
public:
  
  //
  //flags
  //

  bool getRightJustifiedFlag() const;
  void setRightJustifiedFlag( bool value );

  bool getLowerJustifiedFlag() const;
  void setLowerJustifiedFlag( bool value );

  bool getHorizontalDataFlag() const;
  void setHorizontalDataFlag( bool value );

  bool getColorFlag() const;
  void setColorFlag( bool value );

  bool getTransparentFlag() const;
  void setTransparentFlag( bool value );

  bool getPositiveFlag() const;
  void setPositiveFlag( bool value );

  //format of raster
  RasterFormat getFormat() const;
  void setFormat( RasterFormat format );

  //
  //colors for monochrome rasters
  //

  OdUInt8 getForeground() const;
  void setForeground( OdUInt8 color );

  OdUInt8 getBackground() const;
  void setBackground( OdUInt8 color );

  //
  //the size
  //

  OdUInt16 getXExtent() const;
  void setXExtent( OdUInt16 value );

  OdUInt16 getYExtent() const;
  void setYExtent( OdUInt16 value );

  //scale (UORs per pixel)
  double getScale() const;
  void setScale( double scale );

  //origin
  OdGePoint3d getOrigin() const;
  void setOrigin( const OdGePoint3d& origin );

  //support for the complexity
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  //
  // the boundary of Point2D is not implemented because it is not used yet
  //

  //overrides for OdGiDrawable
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  virtual OdRxObjectPtr clone() const;

};

typedef OdSmartPtr< OdDgRasterHeader2d > OdDgRasterHeader2dPtr;


//raster header (3D sub-type)

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgRasterHeader3d : public OdDgGraphicsElement
                                   , public OdDgRaster
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgRasterHeader3d );
public:
  
  //
  //flags
  //

  bool getRightJustifiedFlag() const;
  void setRightJustifiedFlag( bool value );

  bool getLowerJustifiedFlag() const;
  void setLowerJustifiedFlag( bool value );

  bool getHorizontalDataFlag() const;
  void setHorizontalDataFlag( bool value );

  bool getColorFlag() const;
  void setColorFlag( bool value );

  bool getTransparentFlag() const;
  void setTransparentFlag( bool value );

  bool getPositiveFlag() const;
  void setPositiveFlag( bool value );

  //format of raster
  RasterFormat getFormat() const;
  void setFormat( RasterFormat format );

  //
  //colors for monochrome rasters
  //

  OdUInt8 getForeground() const;
  void setForeground( OdUInt8 color );

  OdUInt8 getBackground() const;
  void setBackground( OdUInt8 color );

  //
  //the size
  //

  OdUInt16 getXExtent() const;
  void setXExtent( OdUInt16 value );

  OdUInt16 getYExtent() const;
  void setYExtent( OdUInt16 value );

  //scale (UORs per pixel)
  double getScale() const;
  void setScale( double scale );

  //origin
  OdGePoint3d getOrigin() const;
  void setOrigin( const OdGePoint3d& origin );

  //support for the complexity
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  //
  // the boundary of Point3D is not implemented because it is not used yet
  //

  //overrides for OdGiDrawable
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  virtual OdRxObjectPtr clone() const;
};

typedef OdSmartPtr< OdDgRasterHeader3d > OdDgRasterHeader3dPtr;


//raster component

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgRasterComponent : public OdDgGraphicsElement
                                    , public OdDgRaster
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgRasterComponent );
public:
  
  //
  //flags
  //

  bool getRightJustifiedFlag() const;
  void setRightJustifiedFlag( bool value );

  bool getLowerJustifiedFlag() const;
  void setLowerJustifiedFlag( bool value );

  bool getHorizontalDataFlag() const;
  void setHorizontalDataFlag( bool value );

  bool getColorFlag() const;
  void setColorFlag( bool value );

  bool getTransparentFlag() const;
  void setTransparentFlag( bool value );

  bool getPositiveFlag() const;
  void setPositiveFlag( bool value );

  //format of raster
  RasterFormat getFormat() const;
  void setFormat( RasterFormat format );

  //
  //colors for monochrome rasters
  //

  OdUInt8 getForeground() const;
  void setForeground( OdUInt8 color );

  OdUInt8 getBackground() const;
  void setBackground( OdUInt8 color );

  //
  //offset
  //

  OdUInt16 getOffsetX() const;
  void setOffsetX( OdUInt16 offset );

  OdUInt16 getOffsetY() const;
  void setOffsetY( OdUInt16 offset );

  //number of pixels
  OdUInt16 getNumberOfPixels() const;
  void setNumberOfPixels( OdUInt16 value );

  //
  //data
  //

  OdUInt32 getDataSize() const;
  OdUInt32 getData( OdUInt8* buffer, OdUInt32 size ) const;
  void setData( const OdUInt8* buffer, OdUInt32 size );
};

typedef OdSmartPtr< OdDgRasterComponent > OdDgRasterComponentPtr;


#include "TD_PackPop.h"

#endif // __DG_RASTER_H__
