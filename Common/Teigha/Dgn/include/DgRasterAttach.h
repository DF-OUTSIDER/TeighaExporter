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
#ifndef __DG_RASTER_ATTACH_H__
#define __DG_RASTER_ATTACH_H__

#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgElementIterator.h"
#include "DgRasterFrame.h"

#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GePoint2dArray.h"

#include "Ge/avgpc.h"

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes> 
*/
class TG_EXPORT OdDgRasterAttachmentHeader : public OdDgElement
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgRasterAttachmentHeader );
public:

  enum OdDgRasterAttachmentPlane
  {
    kUndefined   = 0,
    kBackground  = 1,
    kDesignPlane = 2,
    kForeground  = 3
  };

public:

  struct OdDgIndexTransEntry
  {
    OdUInt8  m_uIndex;
    double   m_dTransparencyPercentage;
  };

  struct OdDgRGBTransEntry
  {
    ODCOLORREF m_minColor;
    ODCOLORREF m_maxColor;
    double     m_dTransparencyPercentage;
  };

public:

  //
  // Properties
  //

  //These U & V vectors determine either scale factors (for each U&V axis of a raster) and its rotation.
  //If the vectors are not perpendicular, then the raster has a bias.
  //The lengths of vectors determine the height & weight of one pixel.
  //Therefore, the resulting image is the tetragon with points:
  // 1. origin
  // 2. origin + uVector * [X of the image resolution]
  // 3. origin + uVector * [X of the image resolution] + vVector * [Y of the image resolution]
  // 4. origin + vVector * [Y of the image resolution]
  void getOrientation( OdGePoint3d& origin, OdGeVector3d& uVector, OdGeVector3d& vVector ) const;
  void setOrientation( OdGePoint3d const& origin, OdGeVector3d const& uVector, OdGeVector3d const& vVector );

  OdGePoint2d getExtent() const;
  void setExtent( const OdGePoint2d& point );

  double getDisplayGamma() const;
  void setDisplayGamma( double gamma );

  double getPlotGamma() const;
  void setPlotGamma( double gamma );

  OdDgRasterAttachmentPlane getRasterPlane() const;
  void                      setRasterPlane( OdDgRasterAttachmentPlane uPlane );

  OdUInt32 getRasterLayer() const;
  void     setRasterLayer( OdUInt32 uLayerNum );

  //
  // Flags
  //

  bool getTransparencyFlag() const;
  void setTransparencyFlag( bool value );

  bool getApplyRotationFlag() const;
  void setApplyRotationFlag( bool value );

  bool getClippingFlag() const;
  void setClippingFlag( bool value );

  bool getPlotFlag() const;
  void setPlotFlag( bool value );

  bool getInvertFlag() const;
  void setInvertFlag( bool value );

  bool getViewFlag( int number ) const;
  void setViewFlag( int number, bool value );

  //
  // Clipping
  //
  OdUInt16 getMaskClipNumber() const;
  OdUInt16 getBoundaryClipNumber() const;
  bool     getMaskClipVertices(OdUInt16 Index, OdGePoint2dArray& vertexArr ) const;
  bool     getBoundaryClipVertices(OdUInt16 Index, OdGePoint2dArray& vertexArr ) const;
  bool     setMaskClipVertices(OdUInt16 Index, const OdGePoint2dArray& vertexArr );
  bool     setBoundaryClipVertices(OdUInt16 Index, const OdGePoint2dArray& vertexArr );
  void     addMaskClipVertices( OdGePoint2dArray& vertexArr );
  void     addBoundaryClipVertices( OdGePoint2dArray& vertexArr );
  void     deleteMaskClipVertices(OdUInt16 Index );
  void     deleteBoundaryClipVertices(OdUInt16 Index );

  void getImageSize(OdUInt32& width, OdUInt32& height) const;
  OdGiRasterImagePtr getImage();

  //
  // Transparency
  //

  OdDgRasterFrame::OdDgRasterColorMode getColorMode() const;

  double getAllColorTransparency() const;
  void   setAllColorTransparency( double dTrans );

  // for kGrayscaleMode, kMonochromeMode and k2ColorsMode modes

  double getBackgroundTransparency() const;
  void   setBackgroundTransparency( double dTrans );

  double getForegroundTransparency() const;
  void   setForegroundTransparency( double dTrans );

  // for k16ColorsMode and k256ColorsMode mode.

  OdUInt16    getColorTableEntryCount() const;
  ODCOLORREF getColorTableEntry( OdUInt8 uEntry );

  OdUInt32            getTransparencyIndexEntryCount() const;
  OdDgIndexTransEntry getTransparencyIndexEntry( OdUInt32 uIndex ) const;
  void                setTransparencyIndexEntry( OdUInt32 uIndex, const OdDgIndexTransEntry& trans );
  void                addTransparencyIndexEntry( const OdDgIndexTransEntry& trans );
  void                deleteTransparencyIndexEntry( OdUInt32 uIndex );

  // for kRGBMode and kRGBAMode modes.

  OdUInt32            getTransparencyRgbEntryCount() const;
  OdDgRGBTransEntry   getTransparencyRgbEntry( OdUInt32 uIndex ) const;
  void                setTransparencyRgbEntry( OdUInt32 uIndex, const OdDgRGBTransEntry& trans );
  void                addTransparencyRgbEntry( const OdDgRGBTransEntry& trans );
  void                deleteTransparencyRgbEntry( OdUInt32 uIndex );

  //
  // Names, etc.
  //

  OdString getFilename() const;
  void setFilename( const OdString& name );

  OdString getFilePath() const;
  void setFilePath( const OdString& path );

  OdString getLogicalName() const;
  void setLogicalName( const OdString& name );

  OdString getDescription() const;
  void setDescription( const OdString& string );

  ODCOLORREF getBackgroundColor() const;
  void       setBackgroundColor( ODCOLORREF clr );

  ODCOLORREF getForegroundColor() const;
  void       setForegroundColor( ODCOLORREF clr );

  ////associated element of type 94
  OdDgElementId getRasterFrameId() const;
  void setRasterFrameId( OdDgElementId const& id );

  // OdGiDrawable overrides
  bool subWorldDraw( OdGiWorldDraw* pWd ) const;
  void subViewportDraw( OdGiViewportDraw* pVd ) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  //Note: it will be the origin if no image could be found (its size is needed to calculate the extent)
  OdResult subGetGeomExtents( OdGeExtents3d& extents ) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;

  // OdDgElement overrides
  virtual OdResult subErase( bool erasing );
};

typedef OdSmartPtr< OdDgRasterAttachmentHeader > OdDgRasterAttachmentHeaderPtr;


#include "TD_PackPop.h"

#endif // __DG_RASTER_ATTACH_H__
