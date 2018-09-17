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
#ifndef __DG_TERRAIN_H__
#define __DG_TERRAIN_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"

//-------------------------------------------------------------------
// OdDgTerrainOptions
//-------------------------------------------------------------------

class OdDgTerrainOptions;
typedef OdSmartPtr<OdDgTerrainOptions> OdDgTerrainOptionsPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainOptions : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTerrainOptions);

  //
  // Type declarations
  //
  enum OdDgTerrainOptionsType
  {
      kUnknown                    = 0x00
    , kTriangles                  = 0x01
    , kTriangleVertices           = 0x02
    , kLowPoints                  = 0x03
    , kHighPoints                 = 0x04
    , kFlowArrows                 = 0x05
    , kMinorContour               = 0x06
    , kMajorContour               = 0x07
    , kSourceVertex               = 0x08
    , kSourceBreakLine            = 0x09
    , kSourceHole                 = 0x0A
    , kSourceIsland               = 0x0B
    , kSourceVoid                 = 0x0C
    , kSourceBoundary             = 0x0D
    , kSourceContour              = 0x0E
  };

  enum OdDgTerrainPointDisplayType
  {
    kElement = 0,
    kSymbol  = 1,
    kCell    = 2
  };

  enum OdDgTerrainContourSmoothingType
  {
    kNoneSmoothing                          = 0,
    kVertexSmoothing                        = 1,
    kSplineSmoothing                        = 2,
    kSplineWithoutOverlapDetectionSmoothing = 3
  };

public:

  //
  // Manipulations with sub-types
  //
  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual OdUInt32  getLevelEntryId() const = 0;
  virtual void      setLevelEntryId( OdUInt32 uLevelId ) = 0;

  virtual OdUInt32  getColorIndex() const = 0;
  virtual void      setColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32  getLineStyleEntryId() const = 0;
  virtual void      setLineStyleEntryId( OdUInt32 uLineStyleId ) = 0;

  virtual OdUInt32  getLineWeight() const = 0;
  virtual void      setLineWeight( OdUInt32 uLineWeight ) = 0;

  virtual double    getTransparency() const = 0;
  virtual void      setTransparency( double dTransparency ) = 0;

  virtual bool      getOnFlag() const = 0;
  virtual void      setOnFlag( bool bOn ) = 0;

  virtual void      set( const OdDgTerrainOptionsPtr& pOptions ) = 0;

  static OdDgTerrainOptionsPtr createObject( OdDgTerrainOptionsType type );
};

//-------------------------------------------------------------------
// OdDgTerrainOptionsTriangles
// Type = 1
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsTriangles : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsTriangles );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual OdUInt64 getMaterialId() const = 0;
  virtual void     setMaterialId( OdUInt64 uMaterialId ) = 0;

  virtual OdUInt32 getDisplayStyleEntryId() const = 0;
  virtual void     setDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsTriangles> OdDgTerrainOptionsTrianglesPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsTriangleVertices
// Type = 2
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsTriangleVertices : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsTriangleVertices );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual bool getDisplayTextFlag() const = 0;
  virtual void setDisplayTextFlag( bool bOn ) = 0;

  virtual OdGeScale3d getCellScale() const = 0;
  virtual void      setCellScale( const OdGeScale3d& sclData ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainPointDisplayType getPointDisplayType() const = 0;
  virtual void      setPointDisplayType( OdDgTerrainOptions::OdDgTerrainPointDisplayType uDisplayType ) = 0;

  virtual OdChar    getCharSymbol() const = 0;
  virtual void      setCharSymbol( const OdChar& chSymbol ) = 0;

  virtual OdUInt32  getTextStyleEntryId() const = 0;
  virtual void      setTextStyleEntryId( OdUInt32 uTextStyleId ) = 0;

  virtual OdString  getCellName() const = 0;
  virtual void      setCellName( const OdString& strCellName ) = 0;

  virtual OdString  getPrefix() const = 0;
  virtual void      setPrefix( const OdString& strPrefix ) = 0;

  virtual OdString  getSuffix() const = 0;
  virtual void      setSuffix( const OdString& strSuffix ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsTriangleVertices> OdDgTerrainOptionsTriangleVerticesPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsLowPoints
// Type = 3
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsLowPoints : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsLowPoints );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual bool getDisplayTextFlag() const = 0;
  virtual void setDisplayTextFlag( bool bOn ) = 0;

  virtual OdGeScale3d getCellScale() const = 0;
  virtual void      setCellScale( const OdGeScale3d& sclData ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainPointDisplayType getPointDisplayType() const = 0;
  virtual void      setPointDisplayType( OdDgTerrainOptions::OdDgTerrainPointDisplayType uDisplayType ) = 0;

  virtual OdChar    getCharSymbol() const = 0;
  virtual void      setCharSymbol( const OdChar& chSymbol ) = 0;

  virtual OdUInt32  getTextStyleEntryId() const = 0;
  virtual void      setTextStyleEntryId( OdUInt32 uTextStyleId ) = 0;

  virtual OdString  getCellName() const = 0;
  virtual void      setCellName( const OdString& strCellName ) = 0;

  virtual OdString  getPrefix() const = 0;
  virtual void      setPrefix( const OdString& strPrefix ) = 0;

  virtual OdString  getSuffix() const = 0;
  virtual void      setSuffix( const OdString& strSuffix ) = 0;

  virtual double    getMinValue() const = 0;
  virtual void      setMinValue( double dMinValue ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsLowPoints> OdDgTerrainOptionsLowPointsPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsHighPoints
// Type = 4
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsHighPoints : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsHighPoints );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual bool getDisplayTextFlag() const = 0;
  virtual void setDisplayTextFlag( bool bOn ) = 0;

  virtual OdGeScale3d getCellScale() const = 0;
  virtual void      setCellScale( const OdGeScale3d& sclData ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainPointDisplayType getPointDisplayType() const = 0;
  virtual void      setPointDisplayType( OdDgTerrainOptions::OdDgTerrainPointDisplayType uDisplayType ) = 0;

  virtual OdChar    getCharSymbol() const = 0;
  virtual void      setCharSymbol( const OdChar& chSymbol ) = 0;

  virtual OdUInt32  getTextStyleEntryId() const = 0;
  virtual void      setTextStyleEntryId( OdUInt32 uTextStyleId ) = 0;

  virtual OdString  getCellName() const = 0;
  virtual void      setCellName( const OdString& strCellName ) = 0;

  virtual OdString  getPrefix() const = 0;
  virtual void      setPrefix( const OdString& strPrefix ) = 0;

  virtual OdString  getSuffix() const = 0;
  virtual void      setSuffix( const OdString& strSuffix ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsHighPoints> OdDgTerrainOptionsHighPointsPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsFlowArrows
// Type = 5
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsFlowArrows : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsFlowArrows );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual OdGeScale3d getCellScale() const = 0;
  virtual void      setCellScale( const OdGeScale3d& sclData ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainPointDisplayType getPointDisplayType() const = 0;
  virtual void      setPointDisplayType( OdDgTerrainOptions::OdDgTerrainPointDisplayType uDisplayType ) = 0;

  virtual OdChar    getCharSymbol() const = 0;
  virtual void      setCharSymbol( const OdChar& chSymbol ) = 0;

  virtual OdString  getCellName() const = 0;
  virtual void      setCellName( const OdString& strCellName ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsFlowArrows> OdDgTerrainOptionsFlowArrowsPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsMinorContour
// Type = 6
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsMinorContour : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsMinorContour );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual double getInterval() const = 0;
  virtual void setInterval( double dInterval ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainContourSmoothingType getSmoothingType() const = 0;
  virtual void setSmoothingType( const OdDgTerrainOptions::OdDgTerrainContourSmoothingType& uSmoothingType ) = 0;

  virtual OdUInt32 getSmoothingFactor() const = 0; // minimum value is 1, maximum value is 10
  virtual void setSmoothingFactor( OdUInt32 uSmoothingFactor ) = 0;

  virtual bool getDisplayTextFlag() const = 0;
  virtual void setDisplayTextFlag( bool bSet ) = 0;

  virtual double getTextInterval() const = 0;
  virtual void setTextInterval( double dInterval ) = 0;

  virtual double getUORPerWorkingUnitsScale() const = 0;
  virtual void setUORPerWorkingUnitsScale( double dScale ) = 0;

  virtual OdUInt32 getTextStyleEntryId() const = 0;
  virtual void setTextStyleEntryId( OdUInt32 uTextStyleId ) = 0;

  virtual bool getIgnoreMaxSlopesFlag() const = 0;
  virtual void setIgnoreMaxSlopesFlag( bool bSet ) = 0;

  virtual double getMaxSlopeValue() const = 0;
  virtual void setMaxSlopeValue( double dValue ) = 0;

  virtual OdUInt32  getContourLabelPrecision() const = 0;
  virtual void      setContourLabelPrecision( OdUInt32 uPecision ) = 0;

  virtual OdUInt32  getDepressionColorIndex() const = 0;
  virtual void      setDepressionColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32  getDepressionLineStyleEntryId() const = 0;
  virtual void      setDepressionLineStyleEntryId( OdUInt32 uLineStyleId ) = 0;

  virtual OdUInt32  getDepressionLineWeight() const = 0;
  virtual void      setDepressionLineWeight( OdUInt32 uLineWeight ) = 0;

};

typedef OdSmartPtr<OdDgTerrainOptionsMinorContour> OdDgTerrainOptionsMinorContourPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsMajorContour
// Type = 7
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsMajorContour : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsMajorContour );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual double getInterval() const = 0;
  virtual void setInterval( double dInterval ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainContourSmoothingType getSmoothingType() const = 0;
  virtual void setSmoothingType( const OdDgTerrainOptions::OdDgTerrainContourSmoothingType& uSmoothingType ) = 0;

  virtual OdUInt32 getSmoothingFactor() const = 0; // minimum value is 1, maximum value is 10
  virtual void setSmoothingFactor( OdUInt32 uSmoothingFactor ) = 0;

  virtual bool getDisplayTextFlag() const = 0;
  virtual void setDisplayTextFlag( bool bSet ) = 0;

  virtual double getTextInterval() const = 0;
  virtual void setTextInterval( double dInterval ) = 0;

  virtual double getUORPerWorkingUnitsScale() const = 0;
  virtual void setUORPerWorkingUnitsScale( double dScale ) = 0;

  virtual OdUInt32 getTextStyleEntryId() const = 0;
  virtual void setTextStyleEntryId( OdUInt32 uTextStyleId ) = 0;

  virtual bool getIgnoreMaxSlopesFlag() const = 0;
  virtual void setIgnoreMaxSlopesFlag( bool bSet ) = 0;

  virtual double getMaxSlopeValue() const = 0;
  virtual void setMaxSlopeValue( double dValue ) = 0;

  virtual OdUInt32  getContourLabelPrecision() const = 0;
  virtual void      setContourLabelPrecision( OdUInt32 uPecision ) = 0;

  virtual OdUInt32  getDepressionColorIndex() const = 0;
  virtual void      setDepressionColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32  getDepressionLineStyleEntryId() const = 0;
  virtual void      setDepressionLineStyleEntryId( OdUInt32 uLineStyleId ) = 0;

  virtual OdUInt32  getDepressionLineWeight() const = 0;
  virtual void      setDepressionLineWeight( OdUInt32 uLineWeight ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsMajorContour> OdDgTerrainOptionsMajorContourPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceVertices
// Type = 8
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceVertices : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceVertices );

  virtual OdDgTerrainOptionsType getType() const = 0;

  virtual bool getDisplayTextFlag() const = 0;
  virtual void setDisplayTextFlag( bool bOn ) = 0;

  virtual OdGeScale3d getCellScale() const = 0;
  virtual void      setCellScale( const OdGeScale3d& sclData ) = 0;

  virtual OdDgTerrainOptions::OdDgTerrainPointDisplayType getPointDisplayType() const = 0;
  virtual void      setPointDisplayType( OdDgTerrainOptions::OdDgTerrainPointDisplayType uDisplayType ) = 0;

  virtual OdChar    getCharSymbol() const = 0;
  virtual void      setCharSymbol( const OdChar& chSymbol ) = 0;

  virtual OdUInt32  getTextStyleEntryId() const = 0;
  virtual void      setTextStyleEntryId( OdUInt32 uTextStyleId ) = 0;

  virtual OdString  getCellName() const = 0;
  virtual void      setCellName( const OdString& strCellName ) = 0;

  virtual OdString  getPrefix() const = 0;
  virtual void      setPrefix( const OdString& strPrefix ) = 0;

  virtual OdString  getSuffix() const = 0;
  virtual void      setSuffix( const OdString& strSuffix ) = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceVertices> OdDgTerrainOptionsSourceVerticesPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceBreakLine
// Type = 9
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceBreakLine : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceBreakLine );

  virtual OdDgTerrainOptionsType getType() const = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceBreakLine> OdDgTerrainOptionsSourceBreakLinePtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceHole
// Type = 10
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceHole : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceHole );

  virtual OdDgTerrainOptionsType getType() const = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceHole> OdDgTerrainOptionsSourceHolePtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceIsland
// Type = 11
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceIsland : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceIsland );

  virtual OdDgTerrainOptionsType getType() const = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceIsland> OdDgTerrainOptionsSourceIslandPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceVoid
// Type = 12
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceVoid : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceVoid );

  virtual OdDgTerrainOptionsType getType() const = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceVoid> OdDgTerrainOptionsSourceVoidPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceBoundary
// Type = 13
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceBoundary : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceBoundary );

  virtual OdDgTerrainOptionsType getType() const = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceBoundary> OdDgTerrainOptionsSourceBoundaryPtr;

//-------------------------------------------------------------------
// OdDgTerrainOptionsSourceContour
// Type = 14
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainOptionsSourceContour : public OdDgTerrainOptions
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTerrainOptionsSourceContour );

  virtual OdDgTerrainOptionsType getType() const = 0;
};

typedef OdSmartPtr<OdDgTerrainOptionsSourceContour> OdDgTerrainOptionsSourceContourPtr;

//-------------------------------------------------------------------
// OdDgTerrainElement
//-------------------------------------------------------------------

// type of Extended graphics element, subtype Terrain Graphics Element

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrain : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTerrain );
public:

  // Terrain rendering options
  virtual OdDgTerrainOptionsPtr getOptions( OdDgTerrainOptions::OdDgTerrainOptionsType uType ) const;
  virtual void setOption( const OdDgTerrainOptionsPtr& pOptions );

  // Reference to terrain control element that contains geometry of terrain surface and feature description
  OdDgElementId getTerrainControlElementId() const;
  void          setTerrainControlElementId( const OdDgElementId& idTerrainCtrlElement );

  // Transformation of terrain element.(translation + rotation + scale )
  OdGeMatrix3d  getTransformation() const;
  void          setTransformation( const OdGeMatrix3d& matTransform );

  //overrides for OdGiDrawable
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

typedef OdSmartPtr< OdDgTerrain > OdDgTerrainPtr;

#include "TD_PackPop.h"

#endif // __DG_TERRAIN_H__
