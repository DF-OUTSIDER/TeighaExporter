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

#ifndef __DGPRINTSTYLETABLERECORD_H__
#define __DGPRINTSTYLETABLERECORD_H__

#include "DgApplicationData.h"

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/

struct OdDgPrintStyleFenceCoordinate
{
  double m_dMU;
  double m_dSU;
  double m_dPU;
  OdDgPrintStyleFenceCoordinate() { m_dMU = 0; m_dSU = 0; m_dPU = 0;}
};

//==============================================================================================

struct OdDgPrintStyleFencePoint
{
  OdDgPrintStyleFenceCoordinate m_X;
  OdDgPrintStyleFenceCoordinate m_Y;
  OdDgPrintStyleFenceCoordinate m_Z;
};

//==============================================================================================

class TG_EXPORT OdDgPrintStyleFenceCreationData
{
  public:

    enum OdDgPrintStyleFenceCreationMode
    {
      kNone                     = 0,
      kFromShape                = 1,
      kFromAllShapes            = 2,
      kFromCell                 = 3,
      kFromAllCells             = 4,
      kFitToMasterModel         = 5,
      kFitToMasterModelAndXRefs = 6,
      kFitToElementRange        = 7,
      kFromPoints               = 8
    };

  public:
    OdDgPrintStyleFenceCreationData();
    ~OdDgPrintStyleFenceCreationData();

    OdDgPrintStyleFenceCreationMode getMode() const;
    void setMode( const OdDgPrintStyleFenceCreationMode& uMode );

    bool getSearchMasterModel() const;
    void setSearchMasterModel( bool bSet );

    bool getSearchReferences() const;
    void setSearchReferences( bool bSet );

    OdArray<OdString> getReferenceNames() const;
    void              setReferenceNames( const OdArray<OdString>& arrNames );

    OdArray<OdString> getLevelNames() const;
    void              setLevelNames( const OdArray<OdString>& arrNames );

    OdArray<OdString> getCellNames() const;
    void              setCellNames( const OdArray<OdString>& arrNames );

    OdArray<OdUInt32> getColorIndexes() const;
    void              setColorIndexes( const OdArray<OdUInt32>& arrIndexes );

    OdArray<OdUInt32> getLineStyles() const;
    void              setLineStyles( const OdArray<OdUInt32>& arrLineStyles );

    OdArray<OdUInt32> getLineWeights() const;
    void              setLineWeights( const OdArray<OdUInt32>& arrWeights );

    OdArray<OdDgElement::ElementTypes> getElementTypes() const;
    void              setElementTypes( const OdArray<OdDgElement::ElementTypes>& arrTypes );

    OdArray<OdDgPrintStyleFencePoint> getFencePoints() const;
    void              setFencePoints( const OdArray<OdDgPrintStyleFencePoint>& arrPoints );

  private:
    OdDgPrintStyleFenceCreationMode    m_uMode;
    bool                               m_bSearchMasterModel;
    bool                               m_bSearchReferences;
    OdArray<OdString>                  m_arrReferenceNames;
    OdArray<OdString>                  m_arrLevelNames;
    OdArray<OdString>                  m_arrCellNames;
    OdArray<OdUInt32>                  m_arrColorIndexes;
    OdArray<OdUInt32>                  m_arrLineStyles;
    OdArray<OdUInt32>                  m_arrLineWeights;
    OdArray<OdDgElement::ElementTypes> m_arrElementTypes;
    OdArray<OdDgPrintStyleFencePoint>  m_arrFencePoints;
};

//==============================================================================================

struct OdDgPrintStyleLevelOverride
{
  OdString m_strLevelNameRegex;
  OdString m_strFileNameRegex;
  bool     m_bOn;
};

//==============================================================================================

struct OdDgPrintStyleReferenceOverride
{
  OdString m_strFileNameRegex;
  bool     m_bOn;
};


//==============================================================================================

class TG_EXPORT OdDgPrintStyleTableRecord : public OdDgApplicationData
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgPrintStyleTableRecord);
public:

  enum
  {
    kSignature = 0x571A
  };

  enum OdDgPrintStylePrintArea
  {
    kUndefinedArea = 0,
    kVeiw          = 1,
    kFence         = 2,
    kSheet         = 3
  };

  enum OdDgPrintStyle3StateBoolean
  {
    kFalse          = 0,
    kTrue           = 1,
    kUndefinedValue = 2
  };

  enum OdDgPrintStylePaperOrientation
  {
    kUndefinedOrientation = 0,
    kLandscape            = 1,
    kPortrait             = 2
  };

  enum OdDgPrintStyleLayoutUnits
  {
    kUndefinedUnits = 0,
    kFeet           = 1,
    kInches         = 2,
    kMeters         = 3,
    kDecimeters     = 4,
    kCentimeters    = 5,
    kMillimeters    = 6
  };

  enum OdDgPrintStyleLayoutSizeScaleMode
  {
    kUndefinedMode  = 0,
    kMaximize       = 1,
    kSizeX          = 2,
    kSizeY          = 3,
    kScale          = 4,
    kRescaleFactor  = 5,
    kRescaleSize    = 6
  };

  enum OdDgPrintStyleLayoutOriginMode
  {
    kUndefinedOrigin   = 0,
    kAutoCenter        = 1,
    kUserDefinedOrigin = 2
  };

  enum OdDgPrintStyleLayoutMirrorMode
  {
    kUndefinedMirror   = 0,
    kMirrorNone        = 1,
    kMirrorAboutX      = 2,
    kMirrorAboutY      = 3,
    kMirrorAboutBoth   = 4
  };

  enum OdDgPrintStyleLayoutFenceAlign
  {
    kUndefinedAlign    = 0,
    kNoneAlign         = 1,
    kXAxisAlign        = 2,
    kYAxisAlign        = 3
  };

  enum OdDgPrintStyleColorMode
  {
    kUndefinedColorMode  = 0,
    kMonochrome          = 1,
    kGrayscale           = 2,
    kTrueColor           = 3
  };

  enum OdDgPrintStylePropertyPubMethod
  {
    kUndefinedPubMethod  = 0,
    kNone                = 1,
    kAll                 = 2,
    kFilterByFile        = 3
  };

  enum OdDgPrintStylePrintDefCreationMode
  {
    kUndefined             = 0,
    kFromPaperSheetModels  = 1,
    kFromAllModels         = 2,
    kFromAllSheetModels    = 3,
    kFromAllDesignModels   = 4,
    kFromListedModels      = 5
  };

  enum OdDgPrintStylePdfPaperSizeMode
  {
    kPaperNotSet      = 0,
    kAutoPaperSize    = 1,
    kDefinedPaperSize = 2
  };

  enum OdDgPrintStyleRasterRotation
  {
    kRotUndefined     = 0,
    kRot0             = 1,
    kRot90            = 2,
    kRot180           = 3,
    kRot270           = 4
  };

public:
  virtual OdUInt16 getSignature() const;
  virtual void     setSignature(OdUInt16 type);

  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  virtual bool     getDefaultStyleFlag() const;
  virtual void     setDefaultStyleFlag( bool bSet );

// Global section

  virtual OdString getPrinterConfigurationFilePath() const;
  virtual void     setPrinterConfigurationFilePath( const OdString& strPath );

  virtual OdString getWindowsPrinterName() const;
  virtual void     setWindowsPrinterName( const OdString& strWinPrinterName );

// Dgn/Dwg section

  virtual OdDgPrintStyleTableRecord::OdDgPrintStylePrintArea getPrintArea() const;
  virtual void     setPrintArea( const OdDgPrintStyleTableRecord::OdDgPrintStylePrintArea& uArea );

  virtual OdString getPrintModelName() const;
  virtual void     setPrintModelName( const OdString& strModelName );

  virtual OdString getViewGroupName() const;
  virtual void     setViewGroupName( const OdString& strViewGroupName );

  virtual OdString getViewName() const;
  virtual void     setViewName( const OdString& strViewName );

  virtual OdInt32  getViewNumber() const;
  virtual void     setViewNumber( OdInt32 iViewNumber );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getFullSheetFlag() const;
  virtual void setFullSheetFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getRasterizedFlag() const;
  virtual void setRasterizedFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintTo3DFlag() const;
  virtual void setPrintTo3DFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdString getPaperSizeName() const;
  virtual void     setPaperSizeName( const OdString& strPaperSizeName );

  virtual OdString getPaperSource() const;
  virtual void     setPaperSource( const OdString& strPaperSource );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStylePaperOrientation getPaperOrientation() const;
  virtual void setPaperOrientation( const OdDgPrintStyleTableRecord::OdDgPrintStylePaperOrientation& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutUnits getLayoutUnits() const;
  virtual void setLayoutUnits( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutUnits& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutSizeScaleMode getLayoutSizeScaleMode() const;
  virtual void setLayoutSizeScaleMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutSizeScaleMode& value );

  virtual double getLayoutSizeX() const;
  virtual void   setLayoutSizeX( double dSize );

  virtual double getLayoutSizeY() const;
  virtual void   setLayoutSizeY( double dSize );

  virtual double getLayoutScale() const;
  virtual void   setLayoutScale( double dScale );

  virtual void   getLayoutRescaleFactor( double& dNumerator, double& dDenominator ) const;
  virtual void   setLayoutRescaleFactor( double dNumerator, double dDenominator );

  virtual OdGePoint2d getLayoutRescaleSizeValues() const;
  virtual void   setLayoutRescaleSizeValues( const OdGePoint2d& ptSizeValues );

  virtual double getLayoutRotation() const;
  virtual void   setLayoutRotation( double dAngle );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutOriginMode getLayoutOriginMode() const;
  virtual void setLayoutOriginMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutOriginMode& value );

  virtual OdGePoint2d getLayoutUserDefinedOrigin() const;
  virtual void   setLayoutUserDefinedOrigin( const OdGePoint2d& ptOrigin );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutMirrorMode getLayoutMirrorMode() const;
  virtual void setLayoutMirrorMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutMirrorMode& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutFenceAlign getLayoutFenceAlign() const;
  virtual void setLayoutFenceAlign( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutFenceAlign& value );

  virtual OdString getPenTableFilename() const;
  virtual void     setPenTableFilename( const OdString& strFileName );

  virtual OdString getDesignScriptFilename() const;
  virtual void     setDesignScriptFilename( const OdString& strFileName );

  virtual OdString getProjectWorkspace() const;
  virtual void     setProjectWorkspace( const OdString& strWorkspaceName );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleColorMode getColorMode() const;
  virtual void setColorMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleColorMode& value );

  virtual double getCadRasterQualityFactor() const;
  virtual void   setCadRasterQualityFactor( double dFactor );

  virtual double getCadRasterBrightness() const;
  virtual void   setCadRasterBrightness( double dValue );

  virtual double getCadRasterContrast() const;
  virtual void   setCadRasterContrast( double dValue );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintRasterFlag() const;
  virtual void setPrintRasterFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintBinaryRasterAsIsFlag() const;
  virtual void setPrintBinaryRasterAsIsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintRasterAsGrayscaleFlag() const;
  virtual void setPrintRasterAsGrayscaleFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStylePropertyPubMethod getPropertyPublishingMethod() const;
  virtual void setPropertyPublishingMethod( const OdDgPrintStyleTableRecord::OdDgPrintStylePropertyPubMethod& value );

  virtual OdString getPropertyFilterFileName() const;
  virtual void     setPropertyFilterFileName( const OdString& strFileName );

  virtual bool  getUpdatePrintDefNameFlag() const;
  virtual void  setUpdatePrintDefNameFlag( bool bSet );

  virtual bool  getUpdateFromSourceFlag() const;
  virtual void  setUpdateFromSourceFlag( bool bSet );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStylePrintDefCreationMode getPrintDefCreationMode() const;
  virtual void setPrintDefCreationMode( const OdDgPrintStyleTableRecord::OdDgPrintStylePrintDefCreationMode& value );

  virtual OdArray<OdString> getPrintDefCreationModelList() const;
  virtual void              setPrintDefCreationModelList( const OdArray<OdString>& arrModelNames );

  virtual OdDgPrintStyleFenceCreationData getFenceCreationData() const;
  virtual void                            setFenceCreationData( const OdDgPrintStyleFenceCreationData& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintClipFrontFlag() const;
  virtual void setPrintClipFrontFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintClipBackFlag() const;
  virtual void setPrintClipBackFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintClipVolumeFlag() const;
  virtual void setPrintClipVolumeFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintConstructionsFlag() const;
  virtual void setPrintConstructionsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintDimensionsFlag() const;
  virtual void setPrintDimensionsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintDataFieldsFlag() const;
  virtual void setPrintDataFieldsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintFastCellsFlag() const;
  virtual void setPrintFastCellsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintFillFlag() const;
  virtual void setPrintFillFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintLevelOverridesFlag() const;
  virtual void setPrintLevelOverridesFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintCustomLineStylesFlag() const;
  virtual void setPrintCustomLineStylesFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintLineWeightsFlag() const;
  virtual void setPrintLineWeightsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintPatternsFlag() const;
  virtual void setPrintPatternsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintPointsFlag() const;
  virtual void setPrintPointsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintTagsFlag() const;
  virtual void setPrintTagsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintTextsFlag() const;
  virtual void setPrintTextsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintTextNodesFlag() const;
  virtual void setPrintTextNodesFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintTransparencyFlag() const;
  virtual void setPrintTransparencyFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintBrokenAssociationSymbologyFlag() const;
  virtual void setPrintBrokenAssociationSymbologyFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getApplyColorModeToRasterFlag() const;
  virtual void setApplyColorModeToRasterFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getUseViewBackgroundColorForRenderFlag() const;
  virtual void setUseViewBackgroundColorForRenderFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintBorderFlag() const;
  virtual void setPrintBorderFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPrintFenceBoundaryFlag() const;
  virtual void setPrintFenceBoundaryFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdString getBorderComment() const;
  virtual void     setBorderComment( const OdString& strComment );

  virtual OdArray<OdDgPrintStyleLevelOverride> getLevelOverrides() const;
  virtual void setLevelOverrides( const OdArray<OdDgPrintStyleLevelOverride>& arrOverrides );

  virtual OdArray<OdDgPrintStyleReferenceOverride> getReferenceOverrides() const;
  virtual void setReferenceOverrides( const OdArray<OdDgPrintStyleReferenceOverride>& arrOverrides );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStylePdfPaperSizeMode getPdfPaperSizeMode() const;
  virtual void setPdfPaperSizeMode( const OdDgPrintStyleTableRecord::OdDgPrintStylePdfPaperSizeMode& value );

  virtual OdString getPdfPaperName() const;
  virtual void     setPdfPaperName( const OdString& strPaperName );

  virtual OdString getPdfPaperSource() const;
  virtual void     setPdfPaperSource( const OdString& strPaperSource );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPdfFullSheetFlag() const;
  virtual void setPdfFullSheetFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPdfReduceToPaperSizeFlag() const;
  virtual void setPdfReduceToPaperSizeFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPdfAutoCenterFlag() const;
  virtual void setPdfAutoCenterFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getPdfPrintCommentsFlag() const;
  virtual void setPdfPrintCommentsFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdString getRasterPaperName() const;
  virtual void     setRasterPaperName( const OdString& strPaperName );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStylePaperOrientation getRasterPaperOrientation() const;
  virtual void setRasterPaperOrientation( const OdDgPrintStyleTableRecord::OdDgPrintStylePaperOrientation& value );

  virtual OdString getRasterPaperSource() const;
  virtual void     setRasterPaperSource( const OdString& strPaperSource );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getRasterFullSheetFlag() const;
  virtual void setRasterFullSheetFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutUnits getRasterLayoutUnits() const;
  virtual void setRasterLayoutUnits( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutUnits& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutSizeScaleMode getRasterLayoutSizeScaleMode() const;
  virtual void setRasterLayoutSizeScaleMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutSizeScaleMode& value );

  virtual double getRasterLayoutSizeX() const;
  virtual void   setRasterLayoutSizeX( double dSize );

  virtual double getRasterLayoutSizeY() const;
  virtual void   setRasterLayoutSizeY( double dSize );

  virtual double getRasterLayoutScale() const;
  virtual void   setRasterLayoutScale( double dScale );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleRasterRotation getRasterRotation() const;
  virtual void setRasterRotation( const OdDgPrintStyleTableRecord::OdDgPrintStyleRasterRotation& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutOriginMode getRasterLayoutOriginMode() const;
  virtual void setRasterLayoutOriginMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutOriginMode& value );

  virtual OdGePoint2d getRasterLayoutUserDefinedOrigin() const;
  virtual void   setRasterLayoutUserDefinedOrigin( const OdGePoint2d& ptOrigin );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutMirrorMode getRasterLayoutMirrorMode() const;
  virtual void setRasterLayoutMirrorMode( const OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutMirrorMode& value );

  virtual OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean getRasterInvertBinaryFlag() const;
  virtual void setRasterInvertBinaryFlag( const OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean& value );
};

typedef OdSmartPtr<OdDgPrintStyleTableRecord> OdDgPrintStyleTableRecordPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgPrintStyleTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgPrintStyleTable);
public:

  /** \details
    Adds the specified saved (named) view to this collection.

    \param pView [in]  Pointer to the view to add.
  */
  void add( OdDgPrintStyleTableRecordPtr pPrintStyle );

  /** \details
    Returns an Iterator object that can be used to traverse this collection object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  virtual OdDgElementId getAt( const OdString& name, bool getErasedRecord = false) const;

  virtual bool has( const OdDgElementId& elementId) const;
};

typedef OdSmartPtr<OdDgPrintStyleTable> OdDgPrintStyleTablePtr;


#endif // __DGPRINTSTYLETABLERECORD_H__
