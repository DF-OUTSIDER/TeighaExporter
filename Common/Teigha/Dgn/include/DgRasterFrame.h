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
#ifndef __DG_RASTER_FRAME_H__
#define __DG_RASTER_FRAME_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgModel.h"

/** \details

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgRasterFrame : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgRasterFrame);

  public:

  enum OdDgGeoPriority
  {
    kFromPreferences = 0,
    kAttachment      = 1,
    kRasterHeader    = 2,
    kSisterFile      = 3
  };

  enum OdDgRasterColorMode
  {
    kUnknownMode     = 0,
    kRGBMode         = 1,
    k16ColorsMode    = 2,
    k256ColorsMode   = 3,
    kGrayscaleMode   = 4,
    kMonochromeMode  = 5,
    kRGBAMode        = 6,
    k2ColorsMode     = 9
  };

  enum OdDgRasterDrawingPlane
  {
    kUnknownPlane    = 0,
    kBackgroundPlane = 1,
    kDesignPlane     = 2,
    kForegroundPlane = 3
  };

  public:

    bool getReadOnlyFlag() const;
    void setReadOnlyFlag( bool bSet );

    OdDgGeoPriority getGeoPriority() const;
    void setGeoPriority( OdDgGeoPriority value );

    // use Working Units as GeoTiff Default Units
    bool getWUAsGeotiffDefaultUnitsFlag() const;
    void setWUAsGeotiffDefaultUnitsFlag( bool bSet );

    // use Unit Definition GeoKey if presents
    bool getUseUnitDefinitionGeokeyFlag() const;
    void setUseUnitDefinitionGeokeyFlag( bool bSet );

    bool getIgnoreRotationFlag() const;
    void setIgnoreRotationFlag( bool bSet );

    OdDgRasterDrawingPlane getDrawingPlane() const;
    void                   setDrawingPlane( OdDgRasterDrawingPlane iPlane );

    bool                   getTransparentFlag() const;
    void                   setTransparentFlag( bool bSet );

    bool                   getClippingFlag() const;
    void                   setClippingFlag( bool bSet );

    bool                   getPrintableFlag() const;
    void                   setPrintableFlag( bool bSet );

    bool                   getInvertFlag() const;
    void                   setInvertFlag( bool bSet );

    OdGeMatrix3d getTransformation() const;
    void setTransformation( OdGeMatrix3d const& transformation );

    OdUInt32 getPageNumder() const;
    void     setPageNumber( OdUInt32 uPageNumber );

    OdDgRasterColorMode getRasterColorMode() const;
    void setRasterColorMode( OdDgRasterColorMode uMode );

    OdGePoint2d getRasterDimensions() const;
    void        setRasterDimensions( const OdGePoint2d& ptDimensions );

    OdGePoint2d getScanningResolution() const;
    void        setScanningResolution( const OdGePoint2d& ptResolution );

    // Used as display order for background and foreground planes.
    OdUInt32    getLayer() const;
    void        setLayer( OdUInt32 uLayer );

    double      getViewGamma() const;
    void        setViewGamma( double dGamma );

    double      getPlotGamma() const;
    void        setPlotGamma( double dGamma );

    void setDisplayAtViewFlag( OdUInt32 viewNumber, bool value );
    bool getDisplayAtViewFlag( OdUInt32 viewNumber ) const;

    OdUInt32    getTintColorIndex() const;
    void        setTintColorIndex( OdUInt32 uIndex );

    OdUInt32    getForegroundColorIndex() const;
    void        setForegroundColorIndex( OdUInt32 uIndex );

    OdUInt32    getBackgroundColorIndex() const;
    void        setBackgroundColorIndex( OdUInt32 uIndex );

    OdUInt32    getHighlightColorIndex() const;
    void        setHighlightColorIndex( OdUInt32 uIndex );

    // Contrast and brightness values are from -1 to 1. Default value is 0.

    double      getContrast() const;
    void        setContrast( double dContrast );

    double      getBrightness() const;
    void        setBrightness( double dBrightness );

    double      getTransparentPercent() const;
    void        setTransparentPercent( double dPercent );

    // For monochrome and 2 color mode images

    double      getBGTransparentPercent() const;
    void        setBGTransparentPercent( double dPercent );

    // For other modes

    double      getAllColorsTransparentPercent() const ;
    void        setAllColorsTransparentPercent( double dPercent );

    // For RGB and RGBA modes

    ODCOLORREF  getTransparentColor() const;
    void        setTransparentColor( ODCOLORREF clrColor );

    // For other modes

    OdUInt8     getTransparentColorPaletteIndex() const;
    void        setTransparentColorPaletteIndex( OdUInt8 uIndex );

    //

    OdDgModel::UnitDescription getWorldFileDefaultUnit() const;
    void                       setWorldFileDefaultUnit( OdDgModel::UnitDescription units );

    double getWorldFileDefaultUnitFactor() const;
    void   setWorldFileDefaultUnitFactor( double dFactor );

    OdDgModel::UnitDescription getGeoTiffDefaultUnit() const;
    void                       setGeoTiffDefaultUnit( OdDgModel::UnitDescription units );

    double getGeoTiffDefaultUnitFactor() const;
    void   setGeoTiffDefaultUnitFactor( double dFactor );

    OdDgElementId getRasterReferenceId() const;
    void          setRasterReferenceId( const OdDgElementId& idRasterReference );

    OdString      getFileName() const;
    void          setFileName( const OdString& strFileName );

    OdString      getFullFileName() const;
    void          setFullFileName( const OdString& strFileName );

    OdString      getLogicalName() const;
    void          setLogicalName( const OdString& strLogicalName );

    OdString      getDescription() const;
    void          setDescription( const OdString& strDescription );

    bool subWorldDraw( OdGiWorldDraw* pWd ) const;
    void subViewportDraw( OdGiViewportDraw* pVd ) const;
    //Note: it will be the origin if no image could be found (its size is needed to calculate the extent)
    OdResult subGetGeomExtents( OdGeExtents3d& extents ) const;
    OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
};

typedef OdSmartPtr<OdDgRasterFrame> OdDgRasterFramePtr;

//not implemented yet


#include "TD_PackPop.h"

#endif // __DG_RASTER_FRAME_H__
