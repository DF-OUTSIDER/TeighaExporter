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



// DgPlotStyle.h -- interfaces for setting up dgn plotstyles support for rendering

#ifndef __OD_DG_PLOT_STYLE__
#define __OD_DG_PLOT_STYLE__

#include "OdaCommon.h"
#include "DgElement.h"
#include "DgGraphicsElement.h"
#include "UInt32Array.h"
#include "RxObject.h"

#define STL_USING_SET
#define STL_USING_MAP
#include "OdaSTL.h"

#include "TD_PackPush.h"

/** \details

    <group TD_Namespaces>
*/
namespace OdDgPs
{
  enum DgFillStyle
  {
    kFsSolid            = 64,
    kFsCheckerboard     = 65,
    kFsCrosshatch       = 66,
    kFsDiamonds         = 67,
    kFsHorizontalBars   = 68,
    kFsSlantLeft        = 69,
    kFsSlantRight       = 70,
    kFsSquareDots       = 71,
    kFsVerticalBars     = 72,
    kFsUseElement       = 73,

    kFsLast             = kFsUseElement
  };

  enum DgLineEndStyle
  {
    kLesFlat          = 0,
    kLesSquare        = 1,
    kLesRound         = 2,
    kLesTriangle      = 3,
    kLesUseElement    = 4
  };
  
  enum DgLineJoinStyle
  {
    kLjsMiter         = 0,
    kLjsBevel         = 1,
    kLjsRound         = 2,
    kLjsButt          = 3,
    kLjsUseElement    = 5
  };

  enum DgPsUnitType
  {
    kUtMillimeters  = 0,
    kUtInches       = 1
  };

  enum OdDgPsModelFormat
  {
    kAnyModel = 0,
    kDgnModel = 1,
    kDwgModel = 2
  };

  enum OdDgPlotStyleOutColorStyle
  {
    kColorIndex = 0,
    kRGBColor   = 1,
    kGrayScale  = 2
  };

  enum OdDgPlotStyleOutLineWeightStyle
  {
    kByIndex   = 0,
    kByMM      = 1,
    kByInches  = 2
  };
}

/** \details
    This class represents DgPlotStyle General data section.
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleGeneralData
{
public:

  enum OdDgPsSymbologyType
  {
    kAsStoredInElementHeader = 0,
    kAsDisplayedInView       = 1
  };

public:

  OdDgPsPlotStyleGeneralData();

  OdUInt32 getVersion() const;
  void     setVersion( OdUInt32 uVersion );

  OdUInt32 getPlottingViewsNumber() const;
  OdUInt32 getPlottingViewAt( OdUInt32 uIndex ) const;
  void     addPlottingView( OdUInt32 uViewNum );
  bool     isPlottingForViewEnable( OdUInt32 uViewNum );
  void     deletePlottingView( OdUInt32 uViewNum );
  void     deleteAllPlottingViews();
  OdUInt32Array getPlotingViewArray() const;

  OdDgPsSymbologyType getSymbologyType() const;
  void     setSymbologyType( OdDgPsSymbologyType uType );

  bool     getMatchMultipleSectionsFlag() const;
  void     setMatchMultipleSectionsFlag( bool bSet );

  bool     getDwgPlotStyleCompatibleModeFlag() const;
  void     setDwgPlotStyleCompatibleModeFlag( bool bSet );

  bool     getSortExportedGroupsFlag() const;
  void     setSortExportedGroupsFlag( bool bSet );

  bool     getExplodeSharedCellsFlag() const;
  void     setExplodeSharedCellsFlag( bool bSet );

  bool     getExplodeDimensionsFlag() const;
  void     setExplodeDimensionsFlag( bool bSet );

  bool     getExplodeMultilinesFlag() const;
  void     setExplodeMultilinesFlag( bool bSet );

  bool     getExplodeTagsFlag() const;
  void     setExplodeTagsFlag( bool bSet );

private:

  OdUInt32      m_uVersion;
  std::set<OdUInt32>  m_uPlottingViewsSet;
  OdDgPsSymbologyType m_uSymbology;
  bool          m_bMatchMultipleSections;
  bool          m_bDwgPlotStyleCompatibleMode;
  bool          m_bSortExportedGroups;
  bool          m_bExplodeSharedCells;
  bool          m_bExplodeDimensions;
  bool          m_bExplodeMultilines;
  bool          m_bExplodeTags;

};

/** \details
    This class represents DgPlotStyle data for one line weight index.
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleWeightData
{
public:

  OdDgPsPlotStyleWeightData();

  double   getLineWeightValue() const;
  void     setLineWeightValue( double dValue );

  OdDgPs::DgPsUnitType getLineWeightUnits() const;
  void     setLineWeightUnits( OdDgPs::DgPsUnitType uUnits );

private:

  double   m_dValue;
  OdDgPs::DgPsUnitType m_uUnitType;

};

/** \details
    This class represents DgPlotStyle data for one color index.
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleColorData
{
public:

  OdDgPsPlotStyleColorData();

  bool getEnablePenMapFlag() const;
  void setEnablePenMapFlag( bool bSet );

  bool getUsePrintColorFlag() const;
  void setUsePrintColorFlag( bool bSet );

  bool getUseGrayScaleFlag() const;
  void setUseGrayScaleFlag( bool bSet );

  ODCOLORREF getPringColor() const;
  void       setPrintColor( ODCOLORREF clrPrintColor );

  OdUInt8    getTransparency() const;
  void       setTransparency( OdUInt8 uTransp );

  OdDgPs::DgPsUnitType getPrintWidthUnits() const;
  void       setPrintWidthUnits( OdDgPs::DgPsUnitType uUnitsType );

  double     getPrintWidth() const;
  void       setPrintWidth( double dWidth );

  double     getPrintScreeningFactor() const;
  void       setPrintScreeningFactor( double dFactor );

  bool       isDefaultData() const;

private:

  bool        m_bEnablePenMap;
  bool        m_bUsePrintColor;
  bool        m_bGrayScale;
  ODCOLORREF  m_clrPrintColor;
  OdUInt8     m_uTransparency;
  OdDgPs::DgPsUnitType m_uPrintWidthUnits;
  double      m_dPrintWidthValue;
  double      m_dPrintScreeningFactor;

  bool          m_bUseOnlyDefaultValues;

};

/** \details
    This class represents DgPlotStyle section internal data.
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleSectionFileData
{
public:
  OdDgPsPlotStyleSectionFileData();

  OdUInt32  getSlotNumberCount() const;
  OdUInt32  getSlotNumberAt( OdUInt32 uIndex ) const;
  void addSlotNumber( OdUInt32 uNumber );
  void deleteSlotNumber( OdUInt32 uNumber );
  bool isSlotNumberPresent(  OdUInt32 uNumber ) const;
  OdUInt32Array getSlotNumberArray() const;

  OdUInt32  getReferenceLogicalNameCount() const;
  OdString  getReferenceLogicalNameAt( OdUInt32 uIndex ) const;
  void addReferenceLogicalName( const OdString& strName );
  void deleteReferenceLogicalName( const OdString& strName );
  bool isReferenceLogicalNamePresent( const OdString& strName ) const;

  void clearData();

private:

  std::set<OdUInt32> m_setSlotNumber;
  std::set<OdString> m_setStrReferenceLogicalNames;
};

//------------------------------------------------------

/** <group !!RECORDS_TD_APIRef>
*/
struct OdDgPsPlotStyleSectionEntityCriteria
{
  OdUInt32 m_uMinIndex;
  OdUInt32 m_uMaxIndex;
};

//------------------------------------------------------

/** \details
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleSectionCriteriaData
{

public:
  OdDgPsPlotStyleSectionCriteriaData();

  bool      getDisableSectionFlag() const;
  void      setDisableSectionFlag( bool bSet );

  OdString  getLevelExpression() const;
  void      setLevelExpression( const OdString& strExpression );

  OdDgPs::OdDgPsModelFormat  getModelFormat() const;
  void      setModelFormat( OdDgPs::OdDgPsModelFormat uFormat );

  OdDgPsPlotStyleSectionFileData  getFileData() const;
  void      setFileData( OdDgPsPlotStyleSectionFileData fileData );

  OdUInt32  getElementTypesCount() const;
  OdDgElement::ElementTypes getElementTypeAt( OdUInt32 uIndex ) const;
  void addElementType( OdDgElement::ElementTypes uType );
  void deleteElementType( OdDgElement::ElementTypes uType );
  bool isElementTypePresent(  OdDgElement::ElementTypes uType ) const;
  OdUInt32Array getElementTypeArray() const;

  OdUInt32  getLineTypesCount() const;
  OdString getLineTypeAt( OdUInt32 uIndex ) const;
  void addLineType( const OdString& strLineTypeName );
  void deleteLineType( const OdString& strLineTypeName );
  bool isLineTypePresent( const OdString& strLineTypeName ) const;

  OdUInt32  getElementClassCount() const;
  OdDgGraphicsElement::Class getElementClassAt( OdUInt32 uIndex ) const;
  void addElementClass( OdDgGraphicsElement::Class uClass );
  void deleteElementClass( OdDgGraphicsElement::Class uClass );
  bool isElementClassPresent( OdDgGraphicsElement::Class uClass ) const;
  OdUInt32Array getElementClassArray() const;

  OdUInt32  getLevelCount() const;
  OdString getLevelAt( OdUInt32 uIndex ) const;
  void addLevel( const OdString& strLevelName );
  void deleteLevel( const OdString& strLevelName );
  bool isLevelPresent( const OdString& strLevelName ) const;

  OdUInt32  getLineWeightCount() const;
  OdUInt32  getLineWeightAt( OdUInt32 uIndex ) const;
  void addLineWeight( OdUInt32 uLineWeightIndex );
  void deleteLineWeight( OdUInt32 uLineWeightIndex );
  bool isLineWeightPresent( OdUInt32 uLineWeightIndex ) const;
  OdUInt32Array getLineWeightArray() const;

  OdUInt32  getColorCount() const;
  OdUInt32  getColorAt( OdUInt32 uIndex ) const;
  void addColor( OdUInt32 uColorIndex );
  void deleteColor( OdUInt32 uColorIndex );
  bool isColorPresent( OdUInt32 uColorIndex ) const;
  OdUInt32Array getColorArray() const;

  OdUInt32  getFillColorCount() const;
  OdUInt32  getFillColorAt( OdUInt32 uIndex ) const;
  void addFillColor( OdUInt32 uColorIndex );
  void deleteFillColor( OdUInt32 uColorIndex );
  bool isFillColorPresent( OdUInt32 uColorIndex ) const;
  OdUInt32Array getFillColorArray() const;

  OdUInt32  getMSLinksCount() const;
  OdUInt32  getMSLinkAt( OdUInt32 uIndex ) const;
  void addMSLink( OdUInt32 uLinkIndex );
  void deleteMSLink( OdUInt32 uLinkIndex );
  bool isMsLinkPresent( OdUInt32 uLinkIndex ) const;
  OdUInt32Array getMsLinkArray() const;

  OdUInt32  getEntityCriteriaCount() const;
  OdDgPsPlotStyleSectionEntityCriteria  getEntityCriteriaAt( OdUInt32 uIndex ) const;
  void setEntityCriteria( OdUInt32 uIndex, const OdDgPsPlotStyleSectionEntityCriteria& criteria );
  void addEntityCriteria( const OdDgPsPlotStyleSectionEntityCriteria& criteria );
  void deleteEntityCriteria( OdUInt32 uIndex );

  void clearData();

private:

  bool                                 m_bDisableSection;
  std::set<OdDgElement::ElementTypes>  m_setElementTypes;
  std::set<OdString>                   m_setStrLineTypes;
  std::set<OdDgGraphicsElement::Class> m_setClasses;
  std::set<OdString>                   m_setStrLevels;
  OdString                             m_strLevelExpression;
  std::set<OdUInt32>                   m_setLineWeights;
  std::set<OdUInt32>                   m_setColors;
  std::set<OdUInt32>                   m_setFillColors;
  OdDgPs::OdDgPsModelFormat            m_modelFormat;
  std::set<OdUInt32>                   m_setMSLinks;
  OdArray<OdDgPsPlotStyleSectionEntityCriteria> m_arrEntities;
  OdDgPsPlotStyleSectionFileData       m_fileData;
};

//------------------------------------------------------

/** <group !!RECORDS_TD_APIRef>
*/
struct OdDgPlotStyleSectionOutputColor
{
  OdDgPs::OdDgPlotStyleOutColorStyle  m_uColorStyle;
  OdUInt32                            m_uColorIndex;
  ODCOLORREF                          m_clrRGBColor;
};

//------------------------------------------------------

/** <group !!RECORDS_TD_APIRef>
*/
struct OdDgPlotStyleSectionOutputLineWeight
{
  OdDgPs::OdDgPlotStyleOutLineWeightStyle   m_uLineWidthStyle;
  OdUInt32                                  m_uLineWidthIndex;
  double                                    m_dLineWidthValue;
};

//------------------------------------------------------

/** <group !!RECORDS_TD_APIRef>
*/
struct OdDgPlotStyleSectionOutputLineStyle
{
  OdString                            m_strLineStyleName;
  double                              m_dLineStyleScale;
  bool                                m_bAdaptiveLineScale;
};

//------------------------------------------------------

/** <group !!RECORDS_TD_APIRef>
*/
struct OdDgPlotStyleSectionOutputMacro
{
  OdString                            m_strBasicMacroFile;
  OdString                            m_strBasicMacroFunction;
};

//------------------------------------------------------

/** \details
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleSectionOutputData
{
public:

  enum OdDgPlotStyleFillStatus
  {
    kFillOff = 0,
    kFillOn  = 1
  };

public:
  OdDgPsPlotStyleSectionOutputData();

  bool isEmpty() const;

  bool getDontPlotFlag() const;
  void setDontPlotFlag( bool bSet );

  bool getUsePriorityFlag() const;
  void setUsePriorityFlag( bool bSet );

  bool getUseFillStatusFlag() const;
  void setUseFillStatusFlag( bool bSet );

  bool getUseColorFlag() const;
  void setUseColorFlag( bool bSet );

  bool getUseFillColorFlag() const;
  void setUseFillColorFlag( bool bSet );

  bool getUseScreeningFlag() const;
  void setUseScreeningFlag( bool bSet );

  bool getUseTransparencyFlag() const;
  void setUseTransparencyFlag( bool bSet );

  bool getUseFillPatternFlag() const;
  void setUseFillPatternFlag( bool bSet );

  bool getUseLineWidthFlag() const;
  void setUseLineWidthFlag( bool bSet );

  bool getUseLineStyleFlag() const;
  void setUseLineStyleFlag( bool bSet );

  bool getUseLineCapFlag() const;
  void setUseLineCapFlag( bool bSet );

  bool getUseLineJoinFlag() const;
  void setUseLineJoinFlag( bool bSet );

  bool getUseBasicMacroFlag() const;
  void setUseBasicMacroFlag( bool bSet );

  OdInt32 getPriority() const;
  void    setPriority( OdInt32 iNewPriority );

  OdDgPlotStyleFillStatus getFillStatus() const;
  void setFillStatus( OdDgPlotStyleFillStatus uNewStatus );

  OdDgPlotStyleSectionOutputColor getColor() const;
  void setColor( OdDgPlotStyleSectionOutputColor newColor );

  OdDgPlotStyleSectionOutputColor getFillColor() const;
  void setFillColor( OdDgPlotStyleSectionOutputColor newColor );

  OdUInt8 getScreening() const;
  void    setScreening( OdUInt8 uNewScreening );

  OdUInt8 getTransparency() const;
  void    setTransparency( OdUInt8 uNewTransparency );

  OdDgPs::DgFillStyle getFillPattern() const;
  void setFillPattern( OdDgPs::DgFillStyle uStyle );

  OdDgPlotStyleSectionOutputLineWeight getLineWeight() const;
  void setLineWeight( OdDgPlotStyleSectionOutputLineWeight lineWeight );

  OdDgPlotStyleSectionOutputLineStyle getLineStyle() const;
  void setLineStyle( OdDgPlotStyleSectionOutputLineStyle lineStyle );

  OdDgPs::DgLineEndStyle getLineCaps() const;
  void setLineCaps( OdDgPs::DgLineEndStyle uCaps );

  OdDgPs::DgLineJoinStyle getLineJoin() const;
  void setLineJoin( OdDgPs::DgLineJoinStyle uJoin );

  OdDgPlotStyleSectionOutputMacro getBasicMacro() const;
  void setBasicMacro( OdDgPlotStyleSectionOutputMacro macroData );

  void clearData();

private:

  bool                                m_bDontPlot;
  bool                                m_bUsePriority;
  bool                                m_bUseFillStatus;
  bool                                m_bUseColor;
  bool                                m_bUseFillColor;
  bool                                m_bUseScreening;
  bool                                m_bUseTransparency;
  bool                                m_bUseFillPattern;
  bool                                m_bUseLineWidth;
  bool                                m_bUseLineStyle;
  bool                                m_bUseLineCap;
  bool                                m_bUseLineJoin;
  bool                                m_bUseBasicMacro;

  OdInt32                             m_iPriority;
  OdDgPlotStyleFillStatus             m_uFillStatus;
  OdDgPlotStyleSectionOutputColor     m_color;
  OdDgPlotStyleSectionOutputColor     m_fillColor;
  OdUInt8                             m_uScreening;
  OdUInt8                             m_uTransparency;
  OdDgPs::DgFillStyle                 m_uFillPattern;
  OdDgPlotStyleSectionOutputLineWeight m_lineWeight;
  OdDgPlotStyleSectionOutputLineStyle m_lineStyle;
  OdDgPs::DgLineEndStyle              m_uLineCaps;
  OdDgPs::DgLineJoinStyle             m_uLineJoin;
  OdDgPlotStyleSectionOutputMacro     m_basicMacro;
};

/** \details
    This class represents DgPlotStyle section data.
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes> 
*/
class TG_EXPORT OdDgPsPlotStyleSectionData
{
public:

  OdDgPsPlotStyleSectionData();

  OdString getSectionName() const;
  void     setSectionName( const OdString& strName );

  void getSectionCriteriaData( OdDgPsPlotStyleSectionCriteriaData& data ) const;
  void setSectionCriteriaData( const OdDgPsPlotStyleSectionCriteriaData& data );

  void getSectionOutputData( OdDgPsPlotStyleSectionOutputData& data ) const;
  void setSectionOutputData( const OdDgPsPlotStyleSectionOutputData& data );

private:

  OdString                            m_strName;
  OdDgPsPlotStyleSectionCriteriaData  m_inData;
  OdDgPsPlotStyleSectionOutputData    m_outData;
};

class OdDgPsPlotStyle;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgPsPlotStyle object pointers.
*/
typedef OdSmartPtr<OdDgPsPlotStyle> OdDgPsPlotStylePtr;

/** \details
    This class represents PlotStyle objects.
 
    Corresponding C++ library: DgPlotStyleSvcs
    <group Other_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdDgPsPlotStyle : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPsPlotStyle);

  virtual OdString  fileName() const = 0;
  virtual void      setFileName( const OdString& fileName ) = 0;

  virtual void      getGeneralData(OdDgPsPlotStyleGeneralData& data) const = 0;
  virtual void      setGeneralData(const OdDgPsPlotStyleGeneralData& data) = 0;

  virtual OdUInt32 getWeightDataCount() const = 0;
  virtual bool getWeightData( OdUInt32 uIndex, OdDgPsPlotStyleWeightData& linwWeightData, OdUInt32& lineWeightIndex ) const = 0;
  virtual void addWeightData( const OdDgPsPlotStyleWeightData& data, OdUInt32 lineWeightIndex ) = 0;
  virtual void deleteWeightData( OdUInt32 lineWeightIndex ) = 0;
  virtual bool isLineWeightDataPresent( OdUInt32 lineWeightIndex ) const = 0;
  virtual bool getWeightDataByIndex( OdUInt32 lineWeightIndex, OdDgPsPlotStyleWeightData& data, bool bUseDefaultData = true ) const = 0;

  virtual OdUInt32 getColorDataCount() const = 0;
  virtual bool getColorData( OdUInt32 uIndex, OdDgPsPlotStyleColorData& data, OdUInt32& uColorIndex ) const = 0;
  virtual void addColorData( OdDgPsPlotStyleColorData& data, OdUInt32 uColorIndex ) = 0;
  virtual void deleteColorData( OdUInt32 uColorIndex ) = 0;
  virtual bool isColorDataPresent( OdUInt32 uColorIndex ) const = 0;
  virtual bool getColorDataByIndex( OdUInt32 uColorIndex, OdDgPsPlotStyleColorData& data ) const = 0;

  virtual bool getUseColorDataFlag() const = 0;

  virtual void addStringPair( const OdString& strActual, const OdString& strReplacement ) = 0;
  virtual void deleteStringPair( const OdString& strActual ) = 0;
  virtual OdString getReplacementString( const OdString& strActual ) const = 0;
  virtual bool isReplacementStringPresent( const OdString& strActual ) const = 0;
  virtual OdUInt32 getStringPairCoint() const = 0;
  virtual bool getStringPair( OdUInt32 uIndex, OdString& strActual, OdString& strReplacement ) const = 0;

  virtual OdUInt32 getSectionDataCount() const = 0;
  virtual OdDgPsPlotStyleSectionData getSectionData( OdUInt32 uIndex ) const = 0;
  virtual void setSectionData( OdUInt32 uIndex, OdDgPsPlotStyleSectionData& data ) = 0;
  virtual void addSectionData( OdDgPsPlotStyleSectionData& data ) = 0;
  virtual void deleteSectionData( OdUInt32 uIndex ) = 0;
};

#include "TD_PackPop.h"

#endif // __OD_DG_PLOT_STYLE__
