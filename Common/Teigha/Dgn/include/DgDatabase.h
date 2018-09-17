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
#ifndef __DG_DATABASE_H__
#define __DG_DATABASE_H__

#include "Gi/GiDrawable.h"
#include "DgElementId.h"
#include "DgTable.h"
#include "DgTableElement.h"
#include "DgView.h"
#include "DgModel.h"
#include "DgSharedCellDefinition.h"
#include "DgTagSetDefinition.h"
#include "DgColorTable.h"
#include "DgNonModelElementCollection.h"
#include "RxSystemServices.h"
#include "CmColorBase.h"
#include "DgLineStyleInfo.h"
#include "DgCommonStructs.h"
#include "DgElementTemplate.h"
#include "DgColorBook.h"
#include "DgLinkSet.h"
#include "DbBaseDatabase.h"
#include "DgEnvironmentMap.h"
#include "DgDimension.h"
#include "DgDisplayStyle.h"
#include "DgPrintStyleTableRecord.h"
#include "DgDictionaryTableRecord.h"

class OdDgDatabaseImpl;
class OdDgDatabaseReactor;
class OdDgHostAppServices;
class OdDgSummaryInformation;
class OdDgDocumentSummaryInformation;
class OdDgUnitsFormatter;
class OdDgTransactionReactor;

/** \details

<group DD_Namespaces>
*/
namespace OdDg
{
  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum OdDgDesignCenterUnits
  {
    kUnitsUndefined    = 0,
    kUnitsInches       = 1,
    kUnitsFeet         = 2,
    kUnitsMiles        = 3,
    kUnitsMillimeters  = 4,
    kUnitsCentimeters  = 5,
    kUnitsMeters       = 6,
    kUnitsKilometers   = 7,
    kUnitsMicroinches  = 8,
    kUnitsMils         = 9,
    kUnitsYards        = 10,
    kUnitsAngstroms    = 11,
    kUnitsNanometers   = 12,
    kUnitsMicrons      = 13,
    kUnitsDecimeters   = 14,
    kUnitsDekameters   = 15,
    kUnitsHectometers  = 16,
    kUnitsGigameters   = 17,
    kUnitsAstronomical = 18,
    kUnitsLightYears   = 19,
    kUnitsParsecs      = 20,
    kUnitsMax          = kUnitsParsecs
  };

  enum OdDgPlotPaperUnits
  {
    kInches         = 0,   // Inches
    kMillimeters    = 1,   // Millimeters
    kPixels         = 2    // Pixels
  };

  enum OdDgPlotType
  {
    kView           = 0,
    kExtents        = 1,
    kSheetModel     = 2
  };

  enum OdDgAngleReadoutDirectionMode
  {
    kAzimut  = 1,
    kBearing = 2
  };

  enum OdDgDecimalAccuracy
  {
      kDecimalAccuracy0     = 0x01      //DECIMAL_0                = 0x0001,
    , kDecimalAccuracy1     = 0x02      //DECIMAL_01               = 0x0002,
    , kDecimalAccuracy2     = 0x03      //DECIMAL_012              = 0x0003,
    , kDecimalAccuracy3     = 0x04      //DECIMAL_0123             = 0x0004,
    , kDecimalAccuracy4     = 0x00      //DECIMAL_01234            = 0x0000,
    , kDecimalAccuracy5     = 0x06      //DECIMAL_012345           = 0x0006,
    , kDecimalAccuracy6     = 0x07      //DECIMAL_0123456          = 0x0007,
    , kDecimalAccuracy7     = 0x08      //DECIMAL_01234567         = 0x0008,
    , kDecimalAccuracy8     = 0x09      //DECIMAL_012345678        = 0x0009,
  };

  enum OdDgCompressDatabaseFlags
  {
    kRemoveEmptyCellHeaders        = 0x0001,
    kRemoveEmptyTexts              = 0x0002,
    kRemoveTextsSpaceOnly          = 0x0004,
    kRemoveUnusedCellDefinitions   = 0x0008,
    kRemoveUnusedLineStyles        = 0x0010,
    kRemoveUnusedTextStyles        = 0x0020,
    kRemoveUnusedLevels            = 0x0040,
    kRemoveUnusedDimStyles         = 0x0080,
    kRemoveUnusedNestedXRefLevels  = 0x0100,
    kRemoveUnusedMultilineStyles   = 0x0200,
    kRemoveUnusedElementTemplates  = 0x0400,
    kRemoveUnusedItemTypeLibraries = 0x0800,
    kRemoveXRefsIfFileNotFound     = 0x1000
  };
}


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDatabase : public OdGiDrawable
{
  ODRX_DECLARE_MEMBERS(OdDgDatabase);
protected:
  OdDgDatabase();
  virtual ~OdDgDatabase();

public:

  /** \details
    Returns the OdDgHostAppServices object associated with this database object.
  */
  OdDgHostAppServices* appServices() const;

  /** \details
    Adds the default set of objects and settings to this database object.
  */
  void initialize(
    bool defaultModelIs3d = true);

  /** \details
    Adds an object to this database object, and returns its Element ID.

    \param pObject [in]  Pointer to the object.
    \param ownerId [in]  Object ID of the owner of pObject.
    \param handle [in]  Handle for pObject

    \remarks
    A unique handle will be generated if handle is not supplied or is 0.

  */
  OdDgElementId addOdDgElement(
    OdDgElement* pObject,
    OdDgElementId ownerId = OdDgElementId::kNull,
    OdDbHandle handle = 0);


  /** \details
    Reads the contents of the specified file into this database object.

    \param fileName [in]  Name of the file from which the data are to be read.
    \param shareMode [in]  Share mode to use when opening the specified file.
    \param partialLoad [in]  Controls the partial loading of DGN files.
    \param pAuditInfo [in]  Pointer to an OdDgAuditInfo object.
    \param password [in]  Password for file.

    \remarks
    The specified OdDgAuditInfo object controls the audit, and receives the audit status.
    If pAuditInfo is non-zero, a recover will be performed instead of a load.

    Throws:
    OdError if the write is unsuccessful.
  */
  void readFile(
    const OdString& fileName,
    Oda::FileShareMode shareMode = Oda::kShareDenyWrite);
    //bool partialLoad = false,
    //OdDgAuditInfo* pAuditInfo = 0,
    //const OdString& password = OdString()

  /** \details
    Writes the contents of this database object to the specified file.

    \param filename [in]  Name of the file to which the data are to be written.

    Throws:
    OdError if the write is unsuccessful.
  */
  void writeFile(
    OdString fileName);

  /** \details
  Reads the contents of the memory stream into this database object.

  \param pStream [in] Pointer to the memory stream from which the data are to be read.
  \param shareMode [in] Share mode to use when opening the specified file.

  \remarks
  The specified OdDgAuditInfo object controls the audit, and receives the audit status.
  If pAuditInfo is non-zero, a recover will be performed instead of a load.

  Throws:
  OdError if the write is unsuccessful.
  */
  void readFromStream(
    OdStreamBufPtr& pStream,
    Oda::FileShareMode shareMode = Oda::kShareDenyWrite);

  /** \details
  Writes the contents of this database object to the memory stream.

  \param pStream [out] Pointer to the memory stream to which the data are to be written.

  Throws:
  OdError if the write is unsuccessful.
  */
  void writeToStream(
    OdStreamBufPtr& pStream );
  /** \details
  Read the thumbnail of file.

  \param filename [in]  Name of file.
  \param dibData [out] Binary data of thumbnail

  */
  static bool getThumbnail(
    const OdString& filename, 
    OdBinaryData& dibData );

  OdDgLevelTablePtr getLevelTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgLevelFilterTablePtr getLevelFilterTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgFontTablePtr getFontTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgTextStyleTablePtr getTextStyleTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgDimStyleTablePtr getDimStyleTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgMultilineStyleTablePtr getMultilineStyleTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgLineStyleTablePtr getLineStyleTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgLineStyleDefTablePtr getLineStyleDefTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgRegAppTablePtr getRegAppTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgViewGroupTablePtr getViewGroupTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgNamedViewTablePtr getNamedViewTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgPrintStyleTablePtr getPrintStyleTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgModelTablePtr getModelTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgSharedCellDefinitionTablePtr getSharedCellDefinitionTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgTagDefinitionSetTablePtr getTagDefinitionSetTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgColorTablePtr getColorTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgElementTemplateTablePtr getElementTemplateTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgColorBookTablePtr getColorBookTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgLinkSetTablePtr getLinkSetTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgNonModelElementCollectionPtr getNonModelElementCollection(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgEnvironmentMapTablePtr getEnvironmentMapTable( 
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgDisplayStyleTablePtr getDisplayStyleTable( 
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgPrototypeElementTablePtr getPrototypeElementTable( 
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgDictionaryPtr getDictionaryTable( 
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  OdDgElementId getDictionaryTableId() const;

  /** \details
    Returns the Element ID corresponding to the specified handle object in this database.

    \param handle [in]  Database handle.

    \sa
    OdDgElementId
  */
  OdDgElementId getElementId(
    const OdDbHandle& handle);

   /** \details
    Adds the specified reactor to this object's reactor list.

    \param pReactor [in]  Pointer to the reactor object.
   */
  void addReactor(
    OdDgDatabaseReactor* pReactor) const;

  /** \details
    Removes the specified reactor from this object's reactor list.

    \param pReactor [in]  Pointer to the reactor object.
  */
  void removeReactor(
    OdDgDatabaseReactor* pReactor) const;

  /** \details
      Returns the name of the file associated with this database object.
  */
  OdString getFilename() const;

  /** \details
    Adds the specified name to the APPID table of this database object.

    \param regAppName [in]  Application name.

    \remarks
    True if and only if the specified name had not been, and is now, in the APPID table.
  */
  bool newRegApp(
    const OdString& regAppName);

  // Design File Header data
  //

  OdUInt32 getMajorVersion() const;
  OdUInt32 getMinorVersion() const;

  OdUInt32 getControlFlags() const;
  OdUInt16 getControl1Flags() const;
  OdUInt16 getLockFlags() const;
  OdUInt32 getExtLockFlags() const;
  OdUInt32 getExtLock2Flags() const;

  bool getSaveMaterialAssignmentTableFileFlag() const;
  void setSaveMaterialAssignmentTableFileFlag( bool bSet );

  double getActAngle() const;                  // active angle
  void setActAngle( double val );

  double getAngleRnd() const;                  // angle round-off
  void setAngleRnd( double val );

  double getXActScale() const;                 // active scale in x-axis
  void setXActScale( double val );

  double getYActScale() const;                 // active scale in y-axis
  void setYActScale( double val );

  double getZActScale() const;                 // active scale in z-axis
  void setZActScale( double val );

  double getRoundScale() const;                // scale lock roundoff value
  void setRoundScale( double val );

  double getAzimuth() const;                   // azimuth true north angle
  void setAzimuth( double val );

  OdGePoint3d getExtMin() const;                     //range - lo
  void setExtMin( const OdGePoint3d& point);

  OdGePoint3d getExtMax() const;                    //range - high
  void setExtMax( const OdGePoint3d& point);

  OdUInt32 getActiveLevelEntryId() const;             // active level ID
  void setActiveLevelEntryId( OdUInt32 id );

  OdInt32 getActiveLineStyleEntryId() const;             // element symbology - style
  void setActiveLineStyleEntryId(OdInt32 val);

  OdUInt32 getActiveLineWeight() const;           // element symbology - weight
  void setActiveLineWeight(OdUInt32 val);

  OdUInt32 getActiveColorIndex() const;           // element symbology - color
  void setActiveColorIndex(OdUInt32 val);

  OdUInt32 getFillColorIndex() const;        // Color for filled elements
  void setFillColorIndex( OdUInt32 color);

  OdUInt16 getActiveProps() const;               // element properties word
  void setActiveProps( OdUInt16 val);

  OdDgLineStyleInfo getLineStyle() const;         // Active line style info
  void setLineStyle( const OdDgLineStyleInfo& info);

  double getLineStyleScale() const;            // Line style global display scale
  void setLineStyleScale( double scale );

  OdUInt16 getMultiLineFlags() const;            // Multiline flags
  void setMultiLineFlags( OdUInt16 flags );

  //OdDgTextStyle getTextStyle() const;             // current text data
  //void setTextStyle( const OdDgTextStyle& style );

  OdUInt32 getActiveTextStyleEntryId() const;               // last selected text style id
  void setActiveTextStyleEntryId( OdUInt32 id );

  OdUInt16 getTextScaleLock() const;             // lock for text scale
  void setTextScaleLock( OdUInt16 val );

  //active view group id
  //note: this id should refers to the active model
  //otherwise, it will be replaced (while saving) to the first available view group of the active model
  OdDgElementId getActiveViewGroupId(); 
  void setActiveViewGroupId( const OdDgElementId& id );

  //active model ID
  //note: will set to the first available model (while saving) if is not initialized/not correct
  OdDgElementId getActiveModelId() const;
  void setActiveModelId( const OdDgElementId& modelId );

  //this method recommends the first view group that belongs to the 'active model'
  OdDgElementId recommendActiveViewGroupId() const;

  // Angle readout settings

  OdDgDimTextFormat::AngleValueDisplayMode getAngleFormat() const;               // angle readout format
  void setAngleFormat( OdDgDimTextFormat::AngleValueDisplayMode val );

  OdDg::OdDgDecimalAccuracy getAngleReadoutPrec() const;          // control angle-to-string conversion.
  void setAngleReadoutPrec( OdDg::OdDgDecimalAccuracy val );

  OdDg::OdDgAngleReadoutDirectionMode getAngleReadoutDirectionMode() const;
  void setAngleReadoutDirectionMode( OdDg::OdDgAngleReadoutDirectionMode uMode );

  double getAngleReadoutBaseAngle() const;
  void   setAngleReadoutBaseAngle( double dAngle );

  bool   getAngleReadoutClockwiseFlag() const;
  void   setAngleReadoutClockwiseFlag( bool bSet );
  //

  OdUInt16 getTentativeMode() const;             // tentative point mode flag
  void setTentativeMode( OdUInt16 val ); 

  OdUInt16 getTentativeSubMode() const;          // tentative point sub mode
  void setTentativeSubMode( OdUInt16 val );

  OdUInt16 getKeyPointDividend() const;          // divisor for keypoint snapping
  void setKeyPointDividend(OdUInt16 val );

  OdUInt16 getDefaultSnapMode() const;           // default mode for snapping
  void setDefaultSnapMode( OdUInt16 val );

  OdUInt16 getSystemClass() const;               // system class bit map
  void setSystemClass( OdUInt16 val );

  OdUInt8 getDMRSFlag() const;                  // DMRS flag
  void setDMRSFlag( OdUInt8 val );

  OdUInt16 getDMRSLinkageGenerationMode() const; // linkage generation mode (DMRS)
  void setDMRSLinkageGenerationMode( OdUInt16 mode );

  OdUInt16 getAutoDimFlags() const;              // automatic dimensioning flags
  void setAutoDimFlags( OdUInt16 flags ); 

  OdUInt16 getAssocLockMask() const;             // elements to make associations for
  void setAssocLockMask( OdUInt16 mask );

  OdString getActiveCell() const;            // active cell name
  void setActiveCell( const OdString& name);

  OdString getActiveTermCell() const;             // active line terminator cell name, if cell
  void setActiveTermCell( const OdString& name);

  double getActiveTermScale() const;           // active line terminator scale
  void setActiveTermScale( double scale );                 

  OdString getActivePatternCell() const;         // active patterning cell
  void setActivePatternCell( const OdString& cell );

  double getActivePatternScale() const;        // active patterning scale
  void setActivePatternScale( double scale );      

  double getActivePatternAngle() const;        // active patterning angle
  void setActivePatternAngle( double angle );        

  double getActivePatternAngle2() const;       // second pattern angle (for x-hatch)
  void setActivePatternAngle2( double angle );       

  double getActivePatternRowSpacing() const;   // active patterning row spacing
  void setActivePatternRowSpacing( double val );

  double getActivePatternColumnSpacing() const;// active patterning column spacing
  void setActivePatternColumnSpacing( double val );

  double getPatternTolerance() const;          // Stroke tol for patterns (uors)
  void setPatternTolerance( double val );

  OdUInt32 getActivePointType() const;           // active point type. 0 = none, 1 = symbol, 2 = cell
  void setActivePointType( OdUInt32 val );

  OdUInt16 getActivePointSymbol() const;         // active point character, if symbol
  void setActivePointSymbol( OdUInt16 val );

  OdString getActivePointCell() const;           //active point cell name
  void setActivePointCell( const OdString& name ); 

  double getAreaPatternAngle() const;            // active area patterning angle
  void setAreaPatternAngle( double val );         

  double getAreaPatternRowSpacing() const;        // active area patterning row spacing
  void setAreaPatternRowSpacing( double val );    

  double getAreaPatternColumnSpacing() const;    // active area patterning column spacing
  void setAreaPatternColumnSpacing( double val ); 

  double getReservedCell() const;                // reserved for future cell settings
  void setReservedCell( double val );

  double getZRange2dLow() const;             // get Z range of 2d elements for this value
  void setZRange2dLow( double val );

  double getZRange2dHigh() const;            // get Z range of 2d elements for this value
  void setZRange2dHigh( double val );

  double getStreamDelta() const;             // stream delta
  void setStreamDelta( double val );

  double getStreamTolerance() const;         // stream Tolerance
  void setStreamTolerance( double val );

  double getAngleTolerance() const;          // angle threshold (radians)
  void setAngleTolerance( double val );

  double getAreaTolerance() const;           // area threshold (radians)
  void setAreaTolerance( double val );

  OdUInt32 getHighlightColorIndex() const;    // Highlight color
  void setHighlightColorIndex( OdUInt32 color );

  OdUInt32 getXorColorIndex() const;               // xor color
  void setXorColorIndex( OdUInt32 color );

  double getAxisLockAngle() const;
  void setAxisLockAngle( double val );

  double getAxisLockOrigin() const;
  void setAxisLockOrigin( double val );

  double getChamferDist1() const;
  void setChamferDist1( double val );

  double getChamferDist2() const;
  void setChamferDist2( double val );

  double getAutochainTolerance() const;
  void setAutochainTolerance( double val );

  double getConslineDistance() const;
  void setConslineDistance( double val );

  double getArcRadius() const;                // for circles and arcs
  void setArcRadius( double val );

  double getArcLength() const;
  void setArcLength( double val );

  double getConeRadius1() const;              // used for cones only
  void setConeRadius1( double val );

  double getConeRadius2() const;
  void setConeRadius2( double val );

  double getPolygonRadius() const;
  void setPolygonRadius( double val );

  double getSurrevAngle() const;
  void setSurrevAngle( double val );

  double getExtendDistance() const;
  void setExtendDistance( double val );

  double getFilletRadius() const;
  void setFilletRadius( double val );

  double getCopparDistance() const;
  void setCopparDistance( double val );

  double getArrayRowDistance() const;
  void setArrayRowDistance( double val );

  double getArrayColumnDistance() const;
  void setArrayColumnDistance( double val );

  double getArrayFillAngle() const;
  void setArrayFillAngle( double val );

  double getPointDistance() const;
  void setPointDistance( double val );

  OdUInt16 getPolygonEdges() const;
  void setPolygonEdges( OdUInt16 val );

  OdUInt16 getPointsBetween() const;
  void setPointsBetween( OdUInt16 val );

  OdUInt16 getArrayNumItems() const;
  void setArrayNumItems( OdUInt16 val );

  OdUInt16 getArrayNumRows() const;
  void setArrayNumRows( OdUInt16 val );

  OdUInt16 getArrayNumCols() const;
  void setArrayNumCols( OdUInt16 val );

  OdUInt8 getArrayRotate() const;
  void setArrayRotate( OdUInt8 val );

  OdUInt8 getBSplineOrder() const;
  void setBSplineOrder( OdUInt8 val );

  OdUInt8 getDispAttrType() const;            // displayable attribute type #
  void setDispAttrType( OdUInt8  val );

  //OdUInt16 getRenderFlags() const;
  //void setRenderFlags( OdUInt16 val );

  bool getAmbientFlag() const;
  void setAmbientFlag( bool bSet );

  bool getFlashbulbFlag() const;
  void setFlashbulbFlag( bool bSet );

  bool getSolarFlag() const;
  void setSolarFlag( bool bSet );

  bool getSolarShadowsFlag() const;
  void setSolarShadowsFlag( bool bSet );

  OdUInt16 getSolarShadowResolution() const;
  void     setSolarShadowResolution( OdUInt16 uResalution );

  bool getKeepSolarDirectionFlag() const;
  void setKeepSolarDirectionFlag( bool bSet );

  OdAngleCoordinate getLatitude() const;
  void setLatitude( const OdAngleCoordinate& val );

  OdAngleCoordinate getLongitude() const;
  void setLongitude( const OdAngleCoordinate& val );

  OdUInt32 getSolarTime() const;
  void setSolarTime( OdUInt32 val );

  OdUInt32 getSolarYear() const;                // Year for solar light calculations
  void setSolarYear( OdUInt32 val );

  double getGMTOffset() const;                // Offset to greenwich mean time
  void setGMTOffset( double val );

  OdGePoint3d getSolarDirection() const;
  void setSolarDirection( const OdGePoint3d& val );

  OdGePoint3d getSolarVectorOverride() const;   // solar direction vector override
  void setSolarVectorOverride( const OdGePoint3d& val );

  OdDgLightColor getSolarIntensity() const;
  void setSolarIntensity( const OdDgLightColor& val );

  OdDgLightColor getAmbientIntensity() const;
  void setAmbientIntensity( const OdDgLightColor& val );

  OdDgLightColor getFlashIntensity() const;
  void setFlashIntensity( const OdDgLightColor& val );

  double getNearDepthDensity() const;      // Degree of distance cueing at near clip plane
  void setNearDepthDensity( double val );

  double getFarDepthDensity() const;       // Degree of distance cueing at far clip plane
  void setFarDepthDensity( double val );

  double getNearDepthDistance() const;     // Distance to begin distance cueing
  void setNearDepthDistance( double val );

  OdDgLightColor getHazeColor() const;           // Color of fog
  void setHazeColor( const OdDgLightColor& val );

  double getShadowTolerance() const;
  void setShadowTolerance( double val );

  double getStrokeTolerance() const;
  void setStrokeTolerance( double val );

  OdInt16 getMaxPolygonSize() const;
  void setMaxPolygonSize( OdInt16 val );

  OdInt16 getArcMinimum() const;
  void setArcMinimum( OdInt16 val );

  OdUInt32 getExactHLineAccuracy() const;
  void setExactHLineAccuracy( OdUInt32 val );

  double getExactHLineTolerance() const;      // Tolerance for exact hidden line removal
  void setExactHLineTolerance( double val );

  OdUInt32 getSelectionHighlightOverride() const; // FALSE means use highlight color...
  void setSelectionHighlightOverride( OdUInt32 val );

  OdUInt32 getSelectionHighlightColor() const;    // ...otherwise use this.
  void setSelectionHighlightColor( OdUInt32 val );

  OdString getCellFileName() const;            //cell library file name
  void setCellFileName( const OdString& name );

  OdString getBackgroundFile() const;
  void setBackgroundFile( const OdString& name );

  OdDg::OdDgDesignCenterUnits getDesignCenterUnits() const;
  void setDesignCenterUnits( OdDg::OdDgDesignCenterUnits iUnits );

  OdUInt32 getNextGraphicGroup() const;
  void     setNextGraphicGroup( OdUInt32 uNumber );

  //
  // The end of Design File Header data

  OdDbHandle getHandseed() const;

  double getLastSaveTime() const;

  OdDgElementId getDefaultModelId() const;
  bool setDefaultModelId( const OdDgElementId& modelId );

  /** \details
    Returns the version of the file from which this database object was read.

    \param pMinorVersion [in]  Pointer to the MinorVersion variable to receive the minor version.

    \remarks
    The minor version is returned if and only if pMinorVersion != 0.
  */
  OdUInt32 getOriginalFileVersion(
    OdUInt32* pMinorVersion = 0) const;

  // OdGiDrawable overridden
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  virtual bool isPersistent() const;
  virtual OdDbStub* id() const;
  virtual void setGsNode(OdGsCache* pGsNode);
  virtual OdGsCache* gsNode() const;

  // OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  OdDgMaterialTablePtr getMaterialTable(
    OdDg::OpenMode openMode = OdDg::kForRead) const;

  virtual OdDgUnitsFormatter& formatter();

  /** \details
    Deep clones a set of objects, and appends the clones to the specified in owner object.

    \param objectIds [in]  Array of Object IDs of the objects to be cloned.
    \param ownerId [in]  Object ID of owner object.
    \param idMap [in/out] ID map.
    \param deferXlation [in] Defer translation.

    \remarks
    idMap associates the Object IDs of the original objects with the Object IDs of the newly created clones.

    When cloning objects with different owner IDs, a separate call to this function is required for each
    ownerId. In this case, all but the final call to this function should be made with deferXlation == true.
    This defers translation until all objects have been cloned.

    A deep clone is a clone of the specified objects and everything they own.

    \sa
    wblockCloneObjects()

  */
  void deepCloneObjects(
    const OdDgElementIdArray& objectIds,
    OdDgElementId ownerId,
    OdDgIdMapping& idMap,
    bool deferXlation = false);

  /** \details
    Shallow clones a set of objects, and appends the clones to the specified in owner object.

    \param objectIds [in]  Array of Element IDs of the elements to be cloned.
    \param ownerId [in]  Element ID of the owner element.
    \param idMap [in/out] ID map.
    \param deferXlation [in] Defer translation.

    \remarks
    idMap associates the Element IDs of the original elements with the Element IDs of the newly created clones.

    When cloning elements with different owner IDs, a separate call to this function is required for each
    ownerId. In this case, all but the final call to this function should be made with deferXlation == true.
    This defers translation until all objects have been cloned.

    A shallow clone is a clone of the specified elements, but not what they own.

	duplicateRecordCloning must be one of the following:

    <table>
    Name                        Value   Description
    OdDg::kDrcNotApplicable     0       Not applicable to the object.
    OdDg::kDrcIgnore            1       If a duplicate record exists, use the existing record in the database, and ignore the clone.
    OdDg::kDrcReplace           2       If a duplicate record exists, replace it with the cloned record.
    OdDg::kDrcXrefMangleName    3       Incoming record names are mangled with <Xref>$0$<name>
    OdDg::kDrcMangleName        4       Incoming record names are mangled with $0$<name>
    OdDg::kDrcUnmangleName      5       Unmangle the names mangled by OdDb::kDrcMangleName, then default to OdDb::kDrcIgnore. Typically used by RefEdit when checking records into the original database.
    </table>

    \sa
    deepCloneObjects()

  */
  void wblockCloneObjects( const OdDgElementIdArray& elementIds,
                           OdDgElementId ownerId,
                           OdDgIdMapping& idMap,
                           OdDg::DuplicateRecordCloning duplicateRecordCloning,
                           bool deferXlation = false);

  /** \details
    Shallow clones a set of objects, and appends the clones to the specified in owner object.

    \param objectIds [in]  Array of Element IDs of the elements to be cloned.
    \param pOwner [in]  pointer to the owner element.
    \param idMap [in] ID map.
    \param deferXlation [in] Defer translation.

    \remarks
    idMap associates the Element IDs of the original elements with the Element IDs of the newly created clones.

    When cloning elements with different owner IDs, a separate call to this function is required for each
    ownerId. In this case, all but the final call to this function should be made with deferXlation == true.
    This defers translation until all objects have been cloned.

    A shallow clone is a clone of the specified elements, but not what they own.

	duplicateRecordCloning must be one of the following:

    <table>
    Name                        Value   Description
    OdDg::kDrcNotApplicable     0       Not applicable to the object.
    OdDg::kDrcIgnore            1       If a duplicate record exists, use the existing record in the database, and ignore the clone.
    OdDg::kDrcReplace           2       If a duplicate record exists, replace it with the cloned record.
    OdDg::kDrcXrefMangleName    3       Incoming record names are mangled with <Xref>$0$<name>
    OdDg::kDrcMangleName        4       Incoming record names are mangled with $0$<name>
    OdDg::kDrcUnmangleName      5       Unmangle the names mangled by OdDb::kDrcMangleName, then default to OdDb::kDrcIgnore. Typically used by RefEdit when checking records into the original database.
    </table>

  */
  void wblockCloneObjects( const OdDgElementIdArray& elementIds,
                           OdDgElementPtr pOwner,
                           OdDgIdMapping& idMap,
                           OdDg::DuplicateRecordCloning duplicateRecordCloning,
                           bool deferXlation = false);

  /** \details
    Starts a new transaction of this database object.
  */
  virtual void startTransaction();

  /** \details
    Ends the current transaction associated with this database object.

    \remarks
    This function commits all changes to objects since the start of the current transaction.
  */
  virtual void endTransaction();

  /** \details
    Aborts the current transaction associated with this database object.

    \remarks
    This function initiates an immediate rollback of all changes to objects
    since the start of the current transaction. This rollback occurs for
    all transaction-resident objects.
  */
  virtual void abortTransaction();

  /** \details
    Returns the number of active transactions associated with this database object.
  */
  virtual int numActiveTransactions();

  /** \details
    Adds a transaction reactor to this database object.

    \param reactor [in]  Transaction reactor.
  */
  virtual void addTransactionReactor(
    OdDgTransactionReactor* reactor);

  /** \details
    Removes the specified transaction reactor from this database object.

    \param reactor [in]  Transaction reactor.
  */
  virtual void removeTransactionReactor(
    OdDgTransactionReactor* reactor);

  /** \details
      Starts undo recording of this database object.
  */
  void startUndoRecord();

  /** \details
      Returns true if undo information exists of this database object.
  */
  bool hasUndo() const;

  /** \details
    Performs an undo operation on this database object.

    \remarks
    All operations performed since the
    last call to startUndoRecording will be undone.

    At least 1 undo step is always supported by Teigha for internal needs.
  */
  void undo();

  /** \details
    Controls the undo recording of this object in OdDbDatabase.

    \param disable [in]  Boolean to control undo recording.
    
    \remarks
    Disabling undo recording does not erase the undo recording; it merely suspends it.
    Undo recording is initially off for newly created OdDbDatabase objects.
  */
  void disableUndoRecording(
    bool disable);

  void blockUndoRecording(bool bBegin);

  bool isUndoBlockStarted();

  void setUndoMark();

  bool hasUndoMark() const;

  void undoBack();

  int getUNDOMARKS() const;

  void clearUndo();

  /** \details
    Returns true if redo information exists of this database object.
  */
  bool hasRedo() const;

  /** \details
    Performs a redo operation on this database object.

    \remarks
    Restores operations undone by the last undo.
  */
  void redo();
  OdDgFiler* undoFiler();

  virtual void applyPartialUndo( OdDgFiler* pUndoFiler, OdRxClass* pClassObj);

//  bool isUndoRecordingDisabled() const;
//  void disableUndoRecording(bool bDoIt);

  void assertWriteEnabled(bool autoUndo = true, bool recordModified = true);

  /** \details
    Returns true if and only if this object is open OdDb::kForWrite.
  */
  bool isWriteEnabled() const;
  bool isDBRO() const;

  // Current dgn layout settings

  virtual OdString getPaperName() const;
  virtual void     setPaperName( const OdString& strName );

  virtual bool     getPrintLineWeightsFlag() const;
  virtual void     setPrintLineWeightsFlag( bool bSet );

  virtual bool     getScalePlotStyleLineTypesFlag() const;
  virtual void     setScalePlotStyleLineTypesFlag( bool bSet );

  virtual bool     getShowPlotStyleFlag() const;
  virtual void     setShowPlotStyleFlag( bool bSet );

  virtual void     getPaperSize( double& dWidth, double& dHeight ) const;
  virtual void     setPaperSize( double dWidth, double dHeight );

  virtual OdDbBaseLayoutPE::PlotRotation getPlotRotation() const;
  virtual void     setPlotRotation( OdDbBaseLayoutPE::PlotRotation uRotation );

  virtual void     getMargins( double& dTopMargin, double& dBottomMargin, double& dLeftMargin, double& dRightMargin );
  virtual void     setMargins( double dTopMargin, double dBottomMargin, double dLeftMargin, double dRightMargin );

  virtual bool     getUseStandartScaleFlag() const;
  virtual void     setUseStandartScaleFlag( bool bSet );  
  
  virtual bool     getCenteredFlag() const;
  virtual void     setCenteredFlag( bool bSet );

  virtual double   getStandartScale() const;
  virtual void     setStandartScale( double dScale );

  virtual void     getCustomScale( double& dNumerator, double& dDenomerator ) const;
  virtual void     setCustomScale( double dNumerator, double dDenomerator );

  virtual void     getPlotWindow( double& dMinX, double& dMinY, double& dMaxX, double& dMaxY ) const;
  virtual void     setPlotWindow( double dMinX, double dMinY, double dMaxX, double dMaxY );

  virtual void     getPlotOrigin( double& dOriginX, double& dOriginY ) const;
  virtual void     setPlotOrigin( double dOriginX, double dOriginY );

  virtual OdDg::OdDgPlotPaperUnits     getPlotPaperUnits() const;
  virtual void     setPlotPaperUnits( OdDg::OdDgPlotPaperUnits uUnits );

  virtual OdDg::OdDgPlotType     getPlotType() const;
  virtual void     setPlotType( OdDg::OdDgPlotType uPlotType );

  /** \details
    Sets the pen table (*.tbl) file.
    
    \param currentStyleSheet [in]  Name of current pen table file.
  */
  virtual void setCurrentPenTableFile( const OdString& currentPenTableFile );
  virtual OdString getCurrentPenTableFile() const;

  /** \details
    Remove unused dgn elements from database.
    
    \param compressFlags [in] Compress tool options.
  */
  virtual void compressDatabase( const OdDg::OdDgCompressDatabaseFlags& compressFlags );

private:
  OdDgDatabaseImpl* m_pImpl;
  friend class OdDgDatabaseImpl;
};

typedef OdSmartPtr<OdDgDatabase> OdDgDatabasePtr;

/** \details
    This class defines the interface for the Database Recovery Protocol Extension classes.
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDatabaseRecoveryPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDatabaseRecoveryPE);

  /** \details
    Update database data and recovery incorrect data. 
    Function is called automatically after loading of file.
    
    \param pDb [in /out] Dgn database to recovery.

    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult recoveryDatabase( OdDgDatabase* pDb ) = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDgDatabaseRecoveryPE object pointers.
*/
typedef OdSmartPtr<OdDgDatabaseRecoveryPE> OdDgDatabaseRecoveryPEPtr;

/** \details
    This class defines the interface for the Database Lock Flags Protocol Extension classes.
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDatabaseLockFlagsPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgDatabaseLockFlagsPE);
  long        m_references;     //used by each object separately
public:

  OdDgDatabaseLockFlagsPE();
  ~OdDgDatabaseLockFlagsPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

public:

  virtual bool getSolidHoleFlag( OdDgDatabase* pDb );
  virtual void setSolidHoleFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getScaleLockFlag( OdDgDatabase* pDb );
  virtual void setScaleLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getTextNodeLockFlag( OdDgDatabase* pDb );
  virtual void setTextNodeLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAngleLockFlag( OdDgDatabase* pDb );
  virtual void setAngleLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getSnapLockFlag( OdDgDatabase* pDb );
  virtual void setSnapLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getMirrorCharactersFlag( OdDgDatabase* pDb );
  virtual void setMirrorCharactersFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getFenceClipFlag( OdDgDatabase* pDb );
  virtual void setFenceClipFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getGraphicsGroupLockFlag( OdDgDatabase* pDb );
  virtual void setGraphicsGroupLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getLevelLockFlag( OdDgDatabase* pDb );
  virtual void setLevelLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getFenceOverlapFlag( OdDgDatabase* pDb );
  virtual void setFenceOverlapFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAxisLockFlag( OdDgDatabase* pDb );
  virtual void setAxisLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAuxiliaryInputFlag( OdDgDatabase* pDb );
  virtual void setAuxiliaryInputFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAutoPanFlag( OdDgDatabase* pDb );
  virtual void setAutoPanFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getCellStretchFlag( OdDgDatabase* pDb );
  virtual void setCellStretchFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getSelectionSetFlag( OdDgDatabase* pDb );
  virtual void setSelectionSetFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAutoHandlesFlag( OdDgDatabase* pDb );
  virtual void setAutoHandlesFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getSingleShotModeFlag( OdDgDatabase* pDb );
  virtual void setSingleShotModeFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getDontRestartFlag( OdDgDatabase* pDb );
  virtual void setDontRestartFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getViewSingleShotFlag( OdDgDatabase* pDb );
  virtual void setViewSingleShotFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getSnapConstructionPlaneFlag( OdDgDatabase* pDb );
  virtual void setSnapConstructionPlaneFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getConstructPlanePerpFlag( OdDgDatabase* pDb );
  virtual void setConstructPlanePerpFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getIntersectSnapFlag( OdDgDatabase* pDb );
  virtual void setIntersectSnapFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAssociationFlag( OdDgDatabase* pDb );
  virtual void setAssociationFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getSharedCellsFlag( OdDgDatabase* pDb );
  virtual void setSharedCellsFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getFenceVoidFlag( OdDgDatabase* pDb );
  virtual void setFenceVoidFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getSnappablePatternsFlag( OdDgDatabase* pDb );
  virtual void setSnappablePatternsFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getDepthLockFlag( OdDgDatabase* pDb );
  virtual void setDepthLockFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getAssociativePatternFlag( OdDgDatabase* pDb );
  virtual void setAssociativePatternFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getTrueScaleCellsFlag( OdDgDatabase* pDb );
  virtual void setTrueScaleCellsFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getIgnoreRefOverridesFlag( OdDgDatabase* pDb );
  virtual void setIgnoreRefOverridesFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getScaleViewportLStyleFlag( OdDgDatabase* pDb );
  virtual void setScaleViewportLStyleFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getDisplayRefBoundariesFlag( OdDgDatabase* pDb );
  virtual void setDisplayRefBoundariesFlag( OdDgDatabase* pDb, bool bSet );

  virtual bool getContinuousLineStylesFlag( OdDgDatabase* pDb );
  virtual void setContinuousLineStylesFlag( OdDgDatabase* pDb, bool bSet );

  virtual OdUInt8 getFillMode( OdDgDatabase* pDb );
  virtual void setFillMode( OdDgDatabase* pDb, OdUInt8 uFillMode );
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDgDatabaseLockFlagsPE object pointers.
*/
typedef OdSmartPtr<OdDgDatabaseLockFlagsPE> OdDgDatabaseLockFlagsPEPtr;

#endif //__DG_DATABASE_H__
