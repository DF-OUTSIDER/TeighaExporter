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
#ifndef __DG_DIMENSION_H__
#define __DG_DIMENSION_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgCommonStructs.h"
#include "DgDimOptions.h"
#include "DgCellHeader.h"

#include "OdString.h"
//#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"
#include "SharedPtr.h"

#include "DgFiler.h"

//-------------------------------------------------------------------
// OdDgDimTextInfo
//-------------------------------------------------------------------

/** \details
This dimension settings class is a member of each OdDgDimension.  
It describes the width and height, color, font, weight, etc., of 
the text of the OdDgDimension.  

\remarks
It is important to note that most other dimension settings are 
generated as a ratio of the text height set here, by the tool at
time of dimension creation.  The values set in those other settings
are final and are used as-is (stored value) at rendering time; that
is, the stored value in other settings are the final multiplied value, not the ratio.  But
the dialog boxes for the tools in Microstation will show the ratio to this height.

This class also includes whether to use the Master Units in the labeling text, and
also includes the stacked fraction format, if it is enabled in the OdDgDimension.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimTextInfo
{
public:

  enum TextLocation
  {
    kTextInline,
    kTextAbove,
    kTextOutside,
    kTextTopLeft
  };

  enum TextAlignment
  {
    kManualText,
    kLeftText,
    kCenterLeftText,
    kCenterRightText,
    kRightText
  };

  enum TextRotation
  {
    kHorizontal,
    kVertical,
    kInline
  };

  enum FitOptions
  {
    kTermMovesFirst,
    kTermReversed,
    kTermInside,
    kTermOutside,
    kTextInside,
    kTextMovesFirst,
    kBothMoves,
    kSmallestMoves
  };

  enum StackedFractionType
  {
    kFractionFromText,
    kFractionHorizontal,
    kFractionDiagonal
  };

  enum DimThousandSeparator
  {
    kSeparatorNone,
    kSeparatorSpace,
    kSeparatorComma
  };

  enum StackFractionAlignment
  {
    kFractAlignmentTop,
    kFractAlignmentCenter,
    kFractAlignmentBottom
  };

public:
  OdDgDimTextInfo();
  ~OdDgDimTextInfo();

  void setDefaultValues();

  double getWidth() const;
  double getHeight() const;
  OdUInt32 getFontEntryId() const;
  OdUInt32 getColorIndex() const;
  OdUInt32 getWeight() const;

  void setWidth( double dWidth);
  void setHeight( double dHeight );
  void setFontEntryId( OdUInt32 id );
  void setColorIndex( OdUInt32 color );
  void setWeight( OdUInt32 dWeight );

  // Retrieve all Flags as one OdUInt16
  OdUInt16 getTextFlags() const;
  // Set all Flags as one OdUInt16
  void setTextFlags(OdUInt16 flags);

  // Set/Get method for UseColor, whether to set the text to the dimension's color.
  bool getUseColorFlag() const ;
  void setUseColorFlag(bool val);

  // Set/Get method for UseWeight, whether to use the line weight in rendering the dimension.
  bool getUseWeightFlag() const ;
  void setUseWeightFlag(bool val);

  // Set/Get method for the PrimaryNoMasterUnits flag, whether the primary labeling would include the Master Units.
  bool getPrimaryNoMasterUnitsFlag() const ;
  void setPrimaryNoMasterUnitsFlag(bool val);

  // Set/Get method for the HasAltFormat flag.
  bool getHasAltFormatFlag() const ;
  void setHasAltFormatFlag(bool val);

  // Set/Get method for the SecNoMasterUnits flag, whether the secondary labeling would include the Master Units.
  bool getSecNoMasterUnitsFlag() const ;
  void setSecNoMasterUnitsFlag(bool val);

  // Set/Get method for the HasSecAltFormat flag.
  bool getHasSecAltFormatFlag() const ;
  void setHasSecAltFormatFlag(bool val);

  // Set/Get method for the StackedFractionType enumeration.  Must be enabled in the OdDgDimension.
  StackedFractionType getStackedFractionType() const;
  void setStackedFractionType ( StackedFractionType val );

  // Set/Get method for the type of alignment of stacked fraction. 
  StackFractionAlignment getStackFractAlignment() const;
  void setStackFractAlignment( StackFractionAlignment val );

private:
  double        m_dWidth;
  double        m_dHeight;
  OdUInt32      m_fontEntryId;
  OdUInt32      m_Color;
  OdUInt32      m_nWeight;
  OdUInt16      m_textFlags;    
  OdUInt16      m_Reserved;    
};

//-------------------------------------------------------------------
// OdDgDimPoint
//-------------------------------------------------------------------


/** \details
    Class storage of a point for an OdDgDimension.

  \remarks
  The dimension string (point list) describes the baseline 
  of the dimension.  In other words, each point sets the 
  reference point for each witness line.  The points are
  not necessarily collinear, and their position in the list
  determines their usage, according to the type of tool
  that was used to create them.  
  
  For angularAxis, radial, and
  diameter dimensions, the first point is a center point.

  The second point always seems to set the dimension shaft
  position, using the OffsetToDimLine, from which all other
  witness lines lengths may be calculated.
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimPoint
{
public:
  OdDgDimPoint();
  ~OdDgDimPoint();

  OdGePoint3d getPoint() const;
  void getPoint( OdGePoint3d& pt ) const;
  void setPoint( const OdGePoint3d& pt );

  double getOffsetToDimLine() const;
  void setOffsetToDimLine( double val);

  double getOffsetY() const;
  void setOffsetY( double val);

  OdUInt16 getPointFlags() const;
  void setPointFlags(OdUInt16 flags);

  bool getAssociativeFlag() const;
  void setAssociativeFlag( bool val);

  bool getWitnessControlLocalFlag() const;
  void setWitnessControlLocalFlag( bool val);

  bool getNoWitnessLineFlag() const;
  void setNoWitnessLineFlag( bool val);

  bool getUseAltSymbologyFlag() const;
  void setUseAltSymbologyFlag( bool val);

  bool getPrimaryTextFlag() const;
  void setPrimaryTextFlag( bool val);

  bool getPrimaryTopToleranceTextFlag() const;
  void setPrimaryTopToleranceTextFlag( bool val);

  bool getPrimaryBottomToleranceTextFlag() const;
  void setPrimaryBottomToleranceTextFlag( bool val);

  bool getSecondaryTextFlag() const;
  void setSecondaryTextFlag( bool val);

  bool getSecondaryTopToleranceTextFlag() const;
  void setSecondaryTopToleranceTextFlag( bool val);

  bool getSecondaryBottomToleranceTextFlag() const;
  void setSecondaryBottomToleranceTextFlag( bool val);

  bool getRightCenterJustificationFlag() const;
  void setRightCenterJustificationFlag( bool val);

  OdUInt16 getRelative() const;
  void     setRelative( OdUInt16 uRelative );

  OdDgDimTextInfo::TextAlignment getJustification( ) const;
  void     setJustification( OdDgDimTextInfo::TextAlignment val );

  OdString getPrimaryText() const;
  OdString getPrimaryTopToleranceText() const;
  OdString getPrimaryBottomToleranceText() const;
  OdString getSecondaryText() const;
  OdString getSecondaryTopToleranceText() const;
  OdString getSecondaryBottomToleranceText() const;

  void     setPrimaryText( const OdString& text );
  void     setPrimaryTopToleranceText( const OdString& text );
  void     setPrimaryBottomToleranceText( const OdString& text );
  void     setSecondaryText( const OdString& text );
  void     setSecondaryTopToleranceText( const OdString& text );
  void     setSecondaryBottomToleranceText( const OdString& text );

private:
  OdGePoint3d   m_ptPoint;
  double        m_dOffsetToDimLine;
  double        m_dOffsetY;
  OdInt16       m_nOffsetToText;
  OdUInt16      m_pointFlags;    
  OdUInt32      m_nReserved;

  OdString      m_strPrimaryText;
  OdString      m_strPrimaryPlusTolText;
  OdString      m_strPrimaryMinusTolText;
  OdString      m_strSecondaryText;
  OdString      m_strSecondaryPlusTolText;
  OdString      m_strSecondaryMinusTolText;
};
//-------------------------------------------------------------------
// OdDgDimTextFormat
//-------------------------------------------------------------------

/** \details
    This dimension settings class is a member of each OdDgDimension.  
  It describes the numeric format and accuracy, and other labels, of 
  the text of the OdDgDimension.

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimTextFormat
{
public:

  enum AngleValueDisplayMode
  {
    kAngle_D                = 0,
    kAngle_DM               = 1,
    kAngle_DMS              = 2,
    kAngle_C                = 3,
    kAngle_Radian           = 4
  };

  enum LabelDisplayMode
  {  
    kMu                     = 0x00,
    kMuLabel                = 0x02,
    kMuLabelSuLabel         = 0x03,
    kMu_Su                  = 0x05,
    kMuLabel_SuLabel        = 0x07,
    kSu                     = 0x09,
    kSuLabel                = 0x0B
  };

  enum Accuracy
  {
    kAccuracyNone = 0x00      //ACCURACY_0               = 0x0000,
    , kDecimal1     = 0x81      //DECIMAL_01               = 0x0081,
    , kDecimal2     = 0x82      //DECIMAL_012              = 0x0082,
    , kDecimal3     = 0x84      //DECIMAL_0123             = 0x0084,
    , kDecimal4     = 0x88      //DECIMAL_01234            = 0x0088,
    , kDecimal5     = 0x90      //DECIMAL_012345           = 0x0090,
    , kDecimal6     = 0xA0      //DECIMAL_0123456          = 0x00A0,
    , kDecimal7     = 0xC0      //DECIMAL_01234567         = 0x00C0,
    , kDecimal8     = 0x80      //DECIMAL_012345678        = 0x0080,

    , kFractional2     = 0x01    //FRACTIONAL_1_2          = 0x0001,
    , kFractional4     = 0x02    //FRACTIONAL_1_4          = 0x0002,
    , kFractional8     = 0x04    //FRACTIONAL_1_8          = 0x0004,
    , kFractional16    = 0x08    //FRACTIONAL_1_16         = 0x0008,
    , kFractional32    = 0x10    //FRACTIONAL_1_32         = 0x0010,
    , kFractional64    = 0x20    //FRACTIONAL_1_64         = 0x0020,

    , kExponential1    = 0x40    //EXPONENTIAL_01E1        = 0x0040,
    , kExponential2    = 0x41    //EXPONENTIAL_012E1       = 0x0041,
    , kExponential3    = 0x42    //EXPONENTIAL_0123E1      = 0x0042,
    , kExponential4    = 0x43    //EXPONENTIAL_01234E1     = 0x0043,
    , kExponential5    = 0x44    //EXPONENTIAL_012345E1    = 0x0044,
    , kExponential6    = 0x45    //EXPONENTIAL_0123456E1   = 0x0045,
    , kExponential7    = 0x46    //EXPONENTIAL_01234567E1  = 0x0046,
    , kExponential8    = 0x47    //EXPONENTIAL_012345678E1 = 0x0047,
  };

public:
  OdDgDimTextFormat();
  ~OdDgDimTextFormat();

  Accuracy getPrimaryAccuracy() const;
  void setPrimaryAccuracy( Accuracy val );

  Accuracy getSecondaryAccuracy() const;
  void setSecondaryAccuracy( Accuracy val );

  OdUInt32 getTextFormatFlags() const;
  void setTextFormatFlags(OdUInt32 flags);

  bool getAngleMeasureFlag() const;
  void setAngleMeasureFlag( bool val );

  bool getAngleFormatFlag() const;
  void setAngleFormatFlag( bool val );

  bool getPrimarySubUnitsFlag() const;
  void setPrimarySubUnitsFlag( bool val );

  bool getPrimaryLabelsFlag() const;
  void setPrimaryLabelsFlag( bool val );

  bool getPrimaryDelimiterFlag() const;
  void setPrimaryDelimiterFlag( bool val );

  bool getDecimalCommaFlag() const;
  void setDecimalCommaFlag( bool val );

  bool getSuperScriptLSDFlag() const;
  void setSuperScriptLSDFlag( bool val );

  bool getRoundLSDFlag() const;
  void setRoundLSDFlag( bool val );

  bool getOmitLeadDelimiterFlag() const;
  void setOmitLeadDelimiterFlag( bool val );

  bool getLocalFileUnitsFlag() const;
  void setLocalFileUnitsFlag( bool val );

  bool getUnusedDeprecatedFlag() const;
  void setUnusedDeprecatedFlag( bool val );

  bool getThousandSepFlag() const;
  void setThousandSepFlag( bool val );

  bool getMetricSpaceFlag() const;
  void setMetricSpaceFlag( bool val );

  bool getSecondarySubUnitsFlag() const;
  void setSecondarySubUnitsFlag( bool val );

  bool getSecondaryLabelsFlag() const;
  void setSecondaryLabelsFlag( bool val );

  bool getSecondaryDelimiterFlag() const;
  void setSecondaryDelimiterFlag( bool val );

  bool getRadiansFlag() const;
  void setRadiansFlag( bool val );

  bool getPriAllowZeroMastFlag() const;
  void setPriAllowZeroMastFlag( bool val );

  bool getSecAllowZeroMastFlag() const;
  void setSecAllowZeroMastFlag( bool val );

  bool getPriSubForbidZeroMastFlag() const;
  void setPriSubForbidZeroMastFlag( bool val );

  bool getSecSubForbidZeroMastFlag() const;
  void setSecSubForbidZeroMastFlag( bool val );

  bool getHideAngleSecondsFlag() const;
  void setHideAngleSecondsFlag( bool val );

  bool getSkipNonStackedFractionSpaceFlag() const;
  void setSkipNonStackedFractionSpaceFlag( bool val );

  AngleValueDisplayMode getAngleMode() const;
  void setAngleMode( AngleValueDisplayMode iMode );

  OdUInt32              getDualFormat() const;
  void setDualFormat( OdUInt32 uFormat );

private:
  Accuracy  m_PrimaryAccuracy;
  Accuracy  m_SecondaryAccuracy;
  OdUInt32  m_textFormatFlags;
};

//-------------------------------------------------------------------
// OdDgDimGeometry
//
//  This structure is present in all dimension structures and describes 
//  the common geometry properties
//-------------------------------------------------------------------

/** \details
    This dimension settings class is a member of all OdDgDimension, as 
  the "general settings", applicable for all OdDgDimPoint, 
  and segments of the dimension string. It includes text boundary 
  spacing and terminator sizing and offsets for the stacked dimensions.
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimGeometry
{
public:
  OdDgDimGeometry();
  ~OdDgDimGeometry();
 
  void setDefaultValues();

  double getWitnessLineOffset() const;
  void setWitnessLineOffset( double val);
  
  double getWitnessLineExtend() const;
  void setWitnessLineExtend( double val);
  
  double getTextLift() const;
  void setTextLift( double val);

  double getTextMargin() const;
  void setTextMargin( double val );

  double getMargin() const;
  void setMargin( double val );
  
  double getTerminatorWidth() const;
  void setTerminatorWidth( double val );
  
  double getTerminatorHeight() const;
  void setTerminatorHeight( double val);
  
  double getStackOffset() const;
  void setStackOffset( double val);
  
  double getCenterSize() const;
  void setCenterSize( double val);

  bool getUseMargin() const;
  void setUseMargin( bool bUseMargin );
  
private:
  double          m_dWitnessLineOffset;
  double          m_dWitnessLineExtend;
  double          m_dTextLift;
  double          m_dTextMargin;
  double          m_dMargin;             // dTextMinLeader
  double          m_dTerminatorWidth;
  double          m_dTerminatorHeight;
  double          m_dStackOffset;        // offset of each dimension vertically along the first witness line
  double          m_dCenterSize;
  bool            m_bUseMargin;
};

class TG_EXPORT OdDgDimSizeArrow;
class TG_EXPORT OdDgDimSizeStroke;
class TG_EXPORT OdDgDimSingleLocation;
class TG_EXPORT OdDgDimStackedLocation;
class TG_EXPORT OdDgDimAngleSize;
class TG_EXPORT OdDgDimArcSize;
class TG_EXPORT OdDgDimAngleLocation;
class TG_EXPORT OdDgDimArcLocation;
class TG_EXPORT OdDgDimAngleLines;
//class TG_EXPORT OdDgDimAngleAxis; // RAV 08.19.2009 
class TG_EXPORT OdDgDimRadius;
class TG_EXPORT OdDgDimRadiusExtended;
class TG_EXPORT OdDgDimDiameter;
class TG_EXPORT OdDgDimDiameterExtended;
class TG_EXPORT OdDgDimDiameterParallel;
class TG_EXPORT OdDgDimDiameterPerpendicular;
class TG_EXPORT OdDgDimCustomLinear;
class TG_EXPORT OdDgDimOrdinate;
class TG_EXPORT OdDgDimCenter;


//-------------------------------------------------------------------
// OdDgDimension
//-------------------------------------------------------------------

/** \details
  This class is a base class for the different tool types available.

  \remarks
  The tool types each have a overridable worldDraw implementation, 
  but generally draw the same way.

  The tool type information stores what type of tool was used to 
  generate the dimension style/settings.

  Each tool class makes some assumptions about the settings, but 
  all settings are available.  Each subclass of the OdDgDimension 
  represents a Microstation Dimensioning tool, and the creation of 
  the dimension is controlled by this tool through assumptions 
  about the settings.  The tools' creation of the dimension is 
  customizable, but should mimick the Microstation tools.

  Therefore, the behaviour of each tool's class is customizable, 
  although the settings set by a tool should mimick the settings 
  and behaviours of the Microstation tools, resulting in a final 
  predictable rendering by the worldDraw method.   

  Ultimately, all dimensions are drawn according to the settings 
  stored to the file, whether stored in a DimStyle in the table, 
  through an ID, or in the settings of the dimension class, and
  its member settings classes.
  
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimension : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimension);

public:
    /*
     * The data types declared below are used in dimension style declaration
     *  and (possibly) in dimension entities description.
   */
  
  //  This indicates which tool was used during the creation of the 
  //  dimension, and therefore may lead to assumptions about settings/style.
    enum ToolType
    {
        kToolTypeInvalid          = 0
      , kToolTypeSizeArrow        = 1
      , kToolTypeSizeStroke       = 2
      , kToolTypeLocateSingle     = 3
      , kToolTypeLocateStacked    = 4
      , kToolTypeAngleSize        = 5
      , kToolTypeArcSize          = 6
      , kToolTypeAngleLocation    = 7
      , kToolTypeArcLocation      = 8
      , kToolTypeAngleLines       = 9
      , kToolTypeAngleAxis        = 10
      , kToolTypeRadius           = 11
      , kToolTypeDiameter         = 12
      , kToolTypeDiameterPara     = 13
      , kToolTypeDiameterPerp     = 14
      , kToolTypeCustomLinear     = 15
      , kToolTypeOrdinate         = 16
      , kToolTypeRadiusExtended   = 17
      , kToolTypeDiameterExtended = 18
      , kToolTypeCenter           = 19

      // Do not change this value! it is used as a dimension of an array
    // It should be remain 24 according to Bentley docs, page 46.
      , kToolTypeEnum     

      // These values are used to indicate special dimension types; these are not present
      // in a dimension style properties, but can be used in particular dimension's type
      , kToolTypeAngleAxisX        = 50
      , kToolTypeAngleAxisY        = 51
      , kToolTypeLabelLine         = 52
      , kToolTypeNote              = 53
      , kToolTypeTheLastType = kToolTypeNote
    };

  // Alignment behavior of the Tool. This does not set the rotation. This is a behavior for a tool to mimick.
    enum PlacementAlignment  
    {
        kPaView        = 0
      , kPaDrawing     = 1
      , kPaTrue        = 2
      , kPaArbitrary   = 3
    };
  // Arrow head type applies to arrowheads (kOpened and kClosed and kFilled) and circles (kFilled or not).
    enum TerminatorArrowHeadType
    {
        kOpened   = 0
      , kClosed   = 1
      , kFilled   = 2
    };
  // Terminators may be automatically placed (kAutomatic) by the tool and worldDraw method, or set statically to one of the three types (kOutside, kInside, kReversed).
    enum TerminatorOrientation
    {
        kAutomatic  = 0
      , kReversed   = 1
      , kInside     = 2
      , kOutside    = 3
    };

  //  This retrieves the enumeration value indicating which tool was used during the creation of the 
  //  dimension, and therefore may lead to assumptions about settings/style.
  virtual ToolType dimensionType() const
  {
    return kToolTypeInvalid;
  }
  //  This retrieves the enumeration value indicating which tool type was used during the creation of the 
  //  dimension, and therefore may lead to assumptions about settings/style.
  virtual ToolType getDimensionType() const;
  //  This sets the enumeration value indicating which tool type was used during the creation of the 
  //  dimension, and therefore may lead to assumptions about settings/style.
  virtual void setDimensionType( ToolType tType );
  // Retrieves the count of OdDgDimPoint in the list.
  virtual OdUInt32 getPointsCount() const;
  // Retrieves a single instance of OdDgDimPoint by index from the list.
  virtual OdDgDimPoint getPoint( OdUInt32 iIdx ) const;
  // Sets a single instance of OdDgDimPoint by index in the list.  Must be allocated first.  Use addPoint method, otherwise.
  virtual void setPoint( OdUInt32 iIdx, const OdDgDimPoint& ptDim );
  // Appends a single instance of OdDgDimPoint to the list.
  virtual void addPoint(const OdDgDimPoint& ptDim);

  // Retrieves the settings class OdDgDimTextInfo for this OdDgDimension.
  virtual void getDimTextInfo( OdDgDimTextInfo& info ) const;
  // Sets the settings class OdDgDimTextInfo for this OdDgDimension.
  virtual void setDimTextInfo( const OdDgDimTextInfo& info );

  // Retrieves the settings class OdDgDimTextFormat for this OdDgDimension.
  virtual void getDimTextFormat( OdDgDimTextFormat& format) const;
  // Sets the settings class OdDgDimTextFormat for this OdDgDimension.
  virtual void setDimTextFormat( const OdDgDimTextFormat& format );
  
  // Retrieves the settings class OdDgDimGeometry for this OdDgDimension.
  virtual void getDimGeometry( OdDgDimGeometry& geom ) const;
  // Sets the settings class OdDgDimGeometry for this OdDgDimension.
  virtual void setDimGeometry( const OdDgDimGeometry& geom );

  // Retrieves the OdInt32 value as an Alternate Line Style Entry Id.  Lookup from Line Style Table.
  virtual OdInt32 getAltLineStyleEntryId() const;
  // Sets the OdInt32 value as an Alternate Line Style Entry Id.  Lookup from Line Style Table.
  virtual void setAltLineStyleEntryId(OdInt32 val);

  // Retrieves the OdUInt32 value as an Alternate Line Weight value.
  virtual OdUInt32 getAltLineWeight() const;
  // Sets the OdUInt32 value as an Alternate Line Weight value.
  virtual void setAltLineWeight(OdUInt32 val);

  // Retrieves the OdUInt32 value as an Alternate Color Index value.
  virtual OdUInt32 getAltColorIndex() const;
  // Sets the OdUInt32 value as an Alternate Color Index value.
  virtual void setAltColorIndex(OdUInt32 val);

  // Retrieves the Scale Factor of this dimension.
  virtual double getScaleFactor( ) const;
  // Sets the Scale Factor of this dimension.
  virtual void setScaleFactor( double dScale );
  // Retrieves the Annotation Scale Factor of this dimension.
  virtual double getAnnotationScale( ) const;
  // Sets the Annotation Scale Factor of this dimension.
  virtual void setAnnotationScale( const double dScale );
  // Retrieves the Text Location of this dimension.
  virtual OdDgDimTextInfo::TextLocation getTextLocation() const;
  // Set the Text Location of this dimension.
  virtual void setTextLocation( const OdDgDimTextInfo::TextLocation iLocation );
  // Retrieves the lift for dimension with inline text.
  virtual double getInlineTextLift() const;
  // Set the lift for dimension with inline text.
  virtual void setInlineTextLift( const double dMargin );
  // Retrieves the Fit Options of this dimension for automatic terminator orientation.
  virtual OdDgDimTextInfo::FitOptions getFitOptions() const;
  // Set the Fit Options of this dimension for automatic terminator orientation.
  virtual void setFitOptions( const OdDgDimTextInfo::FitOptions iOptions );
  // Retrieves the Suppress Unfit Terminators of this dimension.
  virtual bool getSuppressUnfitTerminators() const;
  // Set the Suppress Unfit Terminators of this dimension.
  virtual void setSuppressUnfitTerminators( const bool bSet );
  // Retrieves the optimal fit of this dimension if text location is above.
  virtual bool getTextAboveOptimalFit() const;
  // Set the optimal fit of this dimension if text location is above.
  virtual void setTextAboveOptimalFit( const bool bSet );
  // Retrieves the retain fractional accuracy option ( disable fraction simplification: if set - 8/64, otherwise 1/8 )
  // for primary unit string.
  virtual bool getPrimaryRetainFractionalAccuracy() const;
  // Set the retain fractional accuracy option for primary unit string.
  virtual void setPrimaryRetainFractionalAccuracy( const bool bSet );
  // Retrieves the retain fractional accuracy option for secondary unit string.
  virtual bool getSecondaryRetainFractionalAccuracy() const;
  // Set the retain fractional accuracy option for secondary unit string.
  virtual void setSecondaryRetainFractionalAccuracy( const bool bSet );
  // Retrieves the retain fractional accuracy option for alternative format of primary unit string.
  virtual bool getPrimaryAltFormatRetainFractionalAccuracy() const;
  // Set the retain fractional accuracy option for for alternative format of primary unit string.
  virtual void setPrimaryAltFormatRetainFractionalAccuracy( const bool bSet );
  // Retrieves the retain fractional accuracy option for alternative format of secondary unit string.
  virtual bool getSecondaryAltFormatRetainFractionalAccuracy() const;
  // Set the retain fractional accuracy option for alternative format of secondary unit string.
  virtual void setSecondaryAltFormatRetainFractionalAccuracy( const bool bSet );
  // Retrieves the retain fractional accuracy option for tolerance of primary unit string.
  virtual bool getPrimaryToleranceRetainFractionalAccuracy() const;
  // Set the retain fractional accuracy option for for tolerance of primary unit string.
  virtual void setPrimaryToleranceRetainFractionalAccuracy( const bool bSet );
  // Retrieves the retain fractional accuracy option for tolerance of secondary unit string.
  virtual bool getSecondaryToleranceRetainFractionalAccuracy() const;
  // Set the retain fractional accuracy option for tolerance of secondary unit string.
  virtual void setSecondaryToleranceRetainFractionalAccuracy( const bool bSet );
  // Retrieves the Stacked Fraction Scale Factor of this dimension.
  virtual double getStackedFractionScale( ) const;
  // Sets the Stacked Fraction Scale Factor of this dimension.
  virtual void setStackedFractionScale( const double dScale );
  // Retrieves the primary tolerance accuracy if tolerance present.
  virtual bool getPrimaryToleranceAccuracy( OdDgDimTextFormat::Accuracy& iAccuracy ) const;
  // Set the primary tolerance accuracy if tolerance present.
  virtual void setPrimaryToleranceAccuracy( const OdDgDimTextFormat::Accuracy iAccuracy );
  // Retrieves the secondary tolerance accuracy if tolerance present.
  virtual bool getSecondaryToleranceAccuracy( OdDgDimTextFormat::Accuracy& iAccuracy ) const;
  // Set the secondary tolerance accuracy if tolerance present.
  virtual void setSecondaryToleranceAccuracy( const OdDgDimTextFormat::Accuracy iAccuracy );
  // Retrieves the View number (OdUInt32) to which the PlacementAlignment applies, when set to kPaView mode.  Only used in kPaView mode by a tool which has been overridden to make use of this property.
  virtual OdUInt32 getAlignmentView( ) const;
  // Sets the View number (OdUInt32) to which the PlacementAlignment applies, when set to kPaView mode.  Only used in kPaView mode by a tool which has been overridden to make use of this property.
  virtual void setAlignmentView( OdUInt32 uView );

  // Option Blocks
  //

//#pragma MARKMESSAGE("DEBUG: Option count returning one OdDgDimOption, with an invalid value, always.")
  // Retrieves the count of the settings class OdDgDimOption.  Currently retrieves count of one, that has an invalid value.
//  virtual OdUInt32 getOptionCount( ) const;

  virtual OdDgDimOptionPtr getOption( OdDgDimOption::Type type );
  virtual void setOption( const OdDgDimOptionPtr& pOption );

public:
  // Accessor method to get all included flags as single unsigned integer (32bit).
  OdUInt32 getFlags() const;
  // Accessor method to set all included flags as single unsigned integer (32bit).
  void setFlags(OdUInt32 flags);

  // Method to get/set the Joiner flag, indicating whether the inner line is created if the text is moved outside (to reversed mode).
  bool getJoinerFlag() const;
  void setJoinerFlag( bool val );

  // Method to get/set the Box text frame flag.
  bool getBoxTextFlag() const;
  void setBoxTextFlag( bool val );

  // Method to get/set the Capsule text frame flag.
  bool getCapsuleTextFlag() const;
  void setCapsuleTextFlag( bool val );

  // Method to get/set the LeadingZeros flag, indicating whether to allow the leading zero to show on a decimal value less than 1.
  bool getLeadingZerosFlag() const;
  void setLeadingZerosFlag( bool val );
  
  // Method to get/set the TrailingZeros flag, indicating whether or not to drop the remaining zero digits to the length specified in the Accuracy.
  bool getTrailingZerosFlag() const;
  void setTrailingZerosFlag( bool val );
  
  // Method to get/set the Dual flag.
  bool getDualFlag() const;
  void setDualFlag( bool val );

  // Method to get/set the Tolerance flag.
  bool getToleranceFlag() const;
  void setToleranceFlag( bool val );
  
  // Method to get/set the Tolmode flag.
  bool getTolmodeFlag() const;
  void setTolmodeFlag( bool val );
  
  // Method to get/set the Embed flag.
  bool getEmbedFlag() const;
  void setEmbedFlag( bool val );

  // Method to get/set the Horizontal flag.
  bool getHorizontalFlag() const;
  void setHorizontalFlag( bool val );
  
  // Method to get/set the NoAutoTextLift flag.
  bool getNoAutoTextLiftFlag() const;
  void setNoAutoTextLiftFlag( bool val );

  // Method to get/set the CrossCenter flag.
  bool getCrossCenterFlag() const;
  void setCrossCenterFlag( bool val );
  
  // Method to get/set the UnderlineText flag, indicating NTS "not to scale".
  bool getUnderlineTextFlag() const;
  void setUnderlineTextFlag( bool val );
  
  // Method to get/set the RelDimLine flag.
  bool getRelDimLineFlag() const;
  void setRelDimLineFlag( bool val );

  // Method to get/set the StackFract flag, indicating whether to use the getStackedFractionType of the OdDgTextInfo class; whether numerator is vertically aligned over the denominator, rather than at an angle (font-dependent).
  bool getStackFractFlag() const;
  void setStackFractFlag( bool val );
  
  // Method to get/set the LeadingZero2 flag, indicating whether to leave the leading zero on a decimal value less than one, for the second label.
  bool getLeadingZero2Flag() const;
  void setLeadingZero2Flag( bool val );
  
  // Method to get/set the TrailingZeros2 flag, indicating whether or not to drop the remaining zero digits to the length specified in the Accuracy parameter for the second label.
  bool getTrailingZeros2Flag() const;
  void setTrailingZeros2Flag( bool val );
  
  // Method to get/set the ThousandComma flag, indicating whether to use the separator for grouping thousands, etc. See DecimalComma flag.
  bool getThousandCommaFlag() const;
  void setThousandCommaFlag( bool val );
  
  // Method to get/set the Centesimal flag.
  bool getCentesimalFlag() const;
  void setCentesimalFlag( bool val );
  
  // Method to get/set the NoLevelSymb flag.
  bool getNoLevelSymbFlag() const;
  void setNoLevelSymbFlag( bool val );
  
  // Method to get/set the RelStat flag.
  bool getRelStatFlag() const;
  void setRelStatFlag( bool val );

  // Method to get/set the TextHeapPad flag.
  bool getTextHeapPadFlag() const;
  void setTextHeapPadFlag( bool val );

  PlacementAlignment getAlignment() const;
  void setAlignment( PlacementAlignment val );

  //TODO: this is allowing both bits to get set, when only one is allowed.  World_draw must (does currently) handle this by masking with kClosed.

  TerminatorArrowHeadType getArrowHead() const;
  void setArrowHead( TerminatorArrowHeadType val );

  TerminatorOrientation getTermMode() const;
  void setTermMode( TerminatorOrientation val );

  // Accessor method to get all included extended flags as single unsigned integer (16bit).
  OdUInt16 getExtendedFlags() const;

  // Accessor method to set all included flags as single unsigned integer (16bit).
  void setExtendedFlags(OdUInt16 data);

  // Method to get/set the DimExtFlgNoLineThruArrowTerm extended flag, 
  // indicating whether line is drawn through arrow terminator.
  bool getDimExtNoLineThruArrowTermFlag() const;
  void setDimExtNoLineThruArrowTermFlag( bool val );
  
  // Method to get/set the DimExtFlgNoLineThruArrowTerm extended flag, 
  // indicating whether line is drawn through stroke terminator.
  bool getDimExtNoLineThruStrokeTermFlag() const;
  void setDimExtNoLineThruStrokeTermFlag( bool val );
  
  // Method to get/set the DimExtFlgNoLineThruArrowTerm extended flag, 
  // indicating whether line is drawn through origin terminator.
  bool getDimExtNoLineThruOriginTermFlag() const;
  void setDimExtNoLineThruOriginTermFlag( bool val );
  
  // Method to get/set the DimExtFlgNoLineThruArrowTerm extended flag, 
  // indicating whether line is drawn through dot terminator.
  bool getDimExtNoLineThruDotTermFlag() const;
  void setDimExtNoLineThruDotTermFlag( bool val );

  // Method to get/set the DimExtFlgNoLineThruArrowTerm extended flag, 
  // indicating whether angle of angular or arc dimension clockwise or anticlockwise. 
  bool getDimExtClockwiseAngleFlag() const;
  void setDimExtClockwiseAngleFlag( bool val );

  // 2d rotation uses OdGeMatrix2d and 3d rotation uses OdGeQuaternion, based on the model type (3d or 2d).  See Is3D().
  virtual void getRotation( OdGeMatrix2d& rot ) const;
  // 2d rotation uses OdGeMatrix2d and 3d rotation uses OdGeQuaternion, based on the model type (3d or 2d).  See Is3D().
  virtual void setRotation( const OdGeMatrix2d& quat );
                                                                
  // 2d rotation uses OdGeMatrix2d and 3d rotation uses OdGeQuaternion, based on the model type (3d or 2d).  See Is3D().
  virtual void getRotation( OdGeQuaternion& quat ) const;
  // 2d rotation uses OdGeMatrix2d and 3d rotation uses OdGeQuaternion, based on the model type (3d or 2d).  See Is3D().
  virtual void setRotation( const OdGeQuaternion& rotation );

  // The dimensionality of the parent model is returned. True if 3D model.
  virtual bool Is3D() const;
  // Get text style table record entry id for dimension
  virtual OdUInt32 getTextStyleEntryId( ) const;
  // Set text style table record entry id for dimension
  virtual void     setTextStyleEntryId( OdUInt32 uTextStyleId );
  // Get dimension style table record id for dimension
  virtual OdUInt32 getDimensionStyleEntryId() const;
  // Set dimension style table record id for dimension
  virtual void     setDimensionStyleEntryId( OdUInt32 uDimStyle ); 
  // Initialize dimension properties by dimension style.
  virtual bool     applyDimensionStyle( OdUInt32 uDimStyle );
  virtual void     subSetDatabaseDefaults( OdDgDatabase* pDb, bool doSubents);

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  virtual OdRxObjectPtr clone() const;

  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  //-------------------------------------------------------------------------
};

typedef OdSmartPtr<OdDgDimension> OdDgDimensionPtr;

//-------------------------------------------------------------------
// OdDgDimSizeArrow
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimSizeArrow : public OdDgDimension, public OdDgDimToolSizeArrow
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimSizeArrow);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeSizeArrow;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getJointTerminator() const;
  virtual void setJointTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TextType getTextType() const;
  virtual void setTextType( OdDgDimTool::TextType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimSizeArrow> OdDgDimSizeArrowPtr;


//-------------------------------------------------------------------
// OdDgDimSizeStroke
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimSizeStroke : public OdDgDimension, public OdDgDimToolSizeStroke
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimSizeStroke);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeSizeStroke;
  }

  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getJointTerminator() const;
  virtual void setJointTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TextType getTextType() const;
  virtual void setTextType( OdDgDimTool::TextType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimSizeStroke> OdDgDimSizeStrokePtr;

//-------------------------------------------------------------------
// OdDgDimSingleLocation
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimSingleLocation : public OdDgDimension, public OdDgDimToolSingleLocation
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimSingleLocation);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeLocateSingle;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getJointTerminator() const;
  virtual void setJointTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TextType getTextType() const;
  virtual void setTextType( OdDgDimTool::TextType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimSingleLocation> OdDgDimSingleLocationPtr;

//-------------------------------------------------------------------
// OdDgDimStackedLocation
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimStackedLocation :  public OdDgDimension, public OdDgDimToolStackedLocation
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimStackedLocation);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeLocateStacked;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getJointTerminator() const;
  virtual void setJointTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TextType getTextType() const;
  virtual void setTextType( OdDgDimTool::TextType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimStackedLocation> OdDgDimStackedLocationPtr;

//-------------------------------------------------------------------
// OdDgDimAngleSize
//-------------------------------------------------------------------

/** \details
   
    LLibrary: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimAngleSize :  public OdDgDimension, public OdDgDimToolAngleSize
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimAngleSize);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeAngleSize;
  }

  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimAngleSize> OdDgDimAngleSizePtr;

//-------------------------------------------------------------------
// OdDgDimArcSize
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimArcSize :  public OdDgDimension, public OdDgDimToolArcSize
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimArcSize);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeArcSize;
  }

  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual bool getChordAlignFlag() const;
  virtual void setChordAlignFlag( bool val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimArcSize> OdDgDimArcSizePtr;

//-------------------------------------------------------------------
// OdDgDimAngleLocation
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimAngleLocation :  public OdDgDimension, public OdDgDimToolAngleLocation
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimAngleLocation);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeAngleLocation;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );

};
typedef OdSmartPtr<OdDgDimAngleLocation> OdDgDimAngleLocationPtr;

//-------------------------------------------------------------------
// OdDgDimArcLocation
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimArcLocation :  public OdDgDimension, public OdDgDimToolArcLocation
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimArcLocation);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeArcLocation;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimArcLocation> OdDgDimArcLocationPtr;

//-------------------------------------------------------------------
// OdDgDimAngleLines
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimAngleLines :  public OdDgDimension, public OdDgDimToolAngleLines
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimAngleLines);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeAngleLines;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimAngleLines> OdDgDimAngleLinesPtr;

//-------------------------------------------------------------------
// OdDgDimAngleAxis
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/

// RAV 08.19.2009 

//
//class TG_EXPORT OdDgDimAngleAxis : public OdDgDimension
//{
//  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimAngleAxis);
//
//public:
//  virtual ToolType dimensionType() const
//  {
//    return kToolTypeAngleAxis;
//  }
//  bool subWorldDraw(OdGiWorldDraw* pWd) const;
//};
//typedef OdSmartPtr<OdDgDimAngleAxis> OdDgDimAngleAxisPtr;

//-------------------------------------------------------------------
// OdDgDimAngleAxisX
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimAngleAxisX :  public OdDgDimension, public OdDgDimToolAngleAxis
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimAngleAxisX);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeAngleAxisX;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimAngleAxisX> OdDgDimAngleAxisXPtr;

//-------------------------------------------------------------------
// OdDgDimAngleAxisY
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimAngleAxisY :  public OdDgDimension, public OdDgDimToolAngleAxis
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimAngleAxisY);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeAngleAxisY;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimAngleAxisY> OdDgDimAngleAxisYPtr;

//-------------------------------------------------------------------
// OdDgDimNote
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimNote :  public OdDgDimension, public OdDgDimToolNote
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimNote);

public:

  enum NoteFrameType
  {
    kFrameNone = 0,
    kFrameLine = 1,
    kFrameBox  = 2,
    kFrameRotatedBox = 3,
    kFrameCircle = 4,
    kFrameCapsule = 5,
    kFrameHexagon = 6,
    kFrameRotatedHexagon = 7,
    kFrameTriangle = 8,
    kFramePentagon = 9,
    kFrameOctagon = 10
  };

  enum NoteAttachmentAlignment
  {
    kAlignTop = 0,
    kAlignFirstLine = 1,
    kAlignMiddle = 2,
    kAlignLastLine = 3,
    kAlignBottom = 4,
    kAlignDynamicLine = 5,
    kAlignDynamicCorner = 6,
    kAlignUnderline = 7
  };

  enum NoteEditAbout
  {
    kEditTop = 0,
    kEditCenter = 1,
    kEditBottom = 2,
    kEditDynamic = 3
  };

  enum NoteJustification
  {
    kJustificationLeft = 0,
    kJustificationRight = 1,
    kJustificationDynamic = 2,
    kJustificationCenter = 3
  };

  enum NoteAttachmentDirection
  {
    kDirectionAuto  = 0,
    kDirectionLeft  = 1,
    kDirectionRight = 2
  };

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeNote;
  }

  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  double getFrameScale() const;
  void   setFrameScale( double val );

  OdDgDimTool::TerminatorType getTerminator() const;
  void   setTerminator( OdDgDimTool::TerminatorType val );

  OdDgDimTextInfo::TextRotation getTextRotation() const;
  void   setTextRotation( OdDgDimTextInfo::TextRotation val );

  bool getSplineFit() const;
  void   setSplineFit( bool val );

  bool getLeaderLength( double& retVal ) const;
  void setLeaderLength( double val );

  bool getLeftMargin( double& retVal ) const;
  void setLeftMargin( double val );

  bool getLowerMargin( double& retVal ) const;
  void setLowerMargin( double val );

  // Initialize dimension properties by dimension style.
  virtual bool applyDimensionStyle( OdUInt32 uDimStyle );

  NoteFrameType getNoteFrameType() const;
  void          setNoteFrameType( NoteFrameType uType );

  bool   getDimNoteInlineLeaderFlag() const;
  void   setDimNoteInlineLeaderFlag( bool bSet );

  OdDgDimNote::NoteAttachmentAlignment  getNoteLeftAttachmentAlignment() const;
  void    setNoteLeftAttachmentAlignment( const OdDgDimNote::NoteAttachmentAlignment iAlignment );
  OdDgDimNote::NoteAttachmentAlignment  getNoteRightAttachmentAlignment() const;
  void    setNoteRightAttachmentAlignment( const OdDgDimNote::NoteAttachmentAlignment iAlignment );

  OdDgDimNote::NoteAttachmentDirection  getNoteHorizontalAttachmentLeaderDirection() const;
  void    setNoteHorizontalAttachmentLeaderDirection( const OdDgDimNote::NoteAttachmentDirection iDirection );
};
typedef OdSmartPtr<OdDgDimNote> OdDgDimNotePtr;

//-------------------------------------------------------------------
// OdDgDimLabelLine
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimLabelLine :  public OdDgDimension, public OdDgDimToolLabelLine
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimLabelLine);

public:

  enum LabelLineDimensionMode
  {
    kLengthAngle,
    kAngleLength,
    kLengthAbove,
    kAngleAbove,
    kLengthBelow,
    kAngleBelow,
    kLengthAngleAbove,
    kLengthAngleBelow
  };

  virtual ToolType dimensionType() const
  {
    return kToolTypeLabelLine;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  LabelLineDimensionMode getLabelLineMode() const;
  void setLabelLineMode( LabelLineDimensionMode iMode );
  // Initialize dimension properties by dimension style.
  virtual bool applyDimensionStyle( OdUInt32 uDimStyle );
};
typedef OdSmartPtr<OdDgDimLabelLine> OdDgDimLabelLinePtr;

//-------------------------------------------------------------------
// OdDgDimRadius
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimRadius :  public OdDgDimension, public OdDgDimToolRadius
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimRadius);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeRadius;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getCenterMarkFlag() const;
  virtual void setCenterMarkFlag( bool val );

  virtual bool getCenterMarkLeftExtendFlag() const;
  virtual void setCenterMarkLeftExtendFlag( bool val );

  virtual bool getCenterMarkRightExtendFlag() const;
  virtual void setCenterMarkRightExtendFlag( bool val );

  virtual bool getCenterMarkTopExtendFlag() const;
  virtual void setCenterMarkTopExtendFlag( bool val );

  virtual bool getCenterMarkBottomExtendFlag() const;
  virtual void setCenterMarkBottomExtendFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::Leader getLeader() const;
  virtual void setLeader( OdDgDimTool::Leader val );
};
typedef OdSmartPtr<OdDgDimRadius> OdDgDimRadiusPtr;

//-------------------------------------------------------------------
// OdDgDimRadiusExtended
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimRadiusExtended :  public OdDgDimension, public OdDgDimToolRadiusExtended
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimRadiusExtended);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeRadiusExtended;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getCenterMarkFlag() const;
  virtual void setCenterMarkFlag( bool val );

  virtual bool getCenterMarkLeftExtendFlag() const;
  virtual void setCenterMarkLeftExtendFlag( bool val );

  virtual bool getCenterMarkRightExtendFlag() const;
  virtual void setCenterMarkRightExtendFlag( bool val );

  virtual bool getCenterMarkTopExtendFlag() const;
  virtual void setCenterMarkTopExtendFlag( bool val );

  virtual bool getCenterMarkBottomExtendFlag() const;
  virtual void setCenterMarkBottomExtendFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::Leader getLeader() const;
  virtual void setLeader( OdDgDimTool::Leader val );
};
typedef OdSmartPtr<OdDgDimRadiusExtended> OdDgDimRadiusExtendedPtr;

//-------------------------------------------------------------------
// OdDgDimDiameter
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimDiameter :  public OdDgDimension, public OdDgDimToolDiameter
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimDiameter);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeDiameter;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getCenterMarkFlag() const;
  virtual void setCenterMarkFlag( bool val );

  virtual bool getCenterMarkLeftExtendFlag() const;
  virtual void setCenterMarkLeftExtendFlag( bool val );

  virtual bool getCenterMarkRightExtendFlag() const;
  virtual void setCenterMarkRightExtendFlag( bool val );

  virtual bool getCenterMarkTopExtendFlag() const;
  virtual void setCenterMarkTopExtendFlag( bool val );

  virtual bool getCenterMarkBottomExtendFlag() const;
  virtual void setCenterMarkBottomExtendFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::Leader getLeader() const;
  virtual void setLeader( OdDgDimTool::Leader val );
};
typedef OdSmartPtr<OdDgDimDiameter> OdDgDimDiameterPtr;

//-------------------------------------------------------------------
// OdDgDimDiameterExtended
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimDiameterExtended :  public OdDgDimension, public OdDgDimToolDiameterExtended
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimDiameterExtended);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeDiameterExtended;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getCenterMarkFlag() const;
  virtual void setCenterMarkFlag( bool val );

  virtual bool getCenterMarkLeftExtendFlag() const;
  virtual void setCenterMarkLeftExtendFlag( bool val );

  virtual bool getCenterMarkRightExtendFlag() const;
  virtual void setCenterMarkRightExtendFlag( bool val );

  virtual bool getCenterMarkTopExtendFlag() const;
  virtual void setCenterMarkTopExtendFlag( bool val );

  virtual bool getCenterMarkBottomExtendFlag() const;
  virtual void setCenterMarkBottomExtendFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::Leader getLeader() const;
  virtual void setLeader( OdDgDimTool::Leader val );
};
typedef OdSmartPtr<OdDgDimDiameterExtended> OdDgDimDiameterExtendedPtr;

//-------------------------------------------------------------------
// OdDgDimDiameterParallel
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimDiameterParallel :  public OdDgDimension, public OdDgDimToolDiameterParallel
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimDiameterParallel);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeDiameterPara;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );

};
typedef OdSmartPtr<OdDgDimDiameterParallel> OdDgDimDiameterParallelPtr;

//-------------------------------------------------------------------
// OdDgDimDiameterPerpendicular
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimDiameterPerpendicular :  public OdDgDimension, public OdDgDimToolPerpendicular
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimDiameterPerpendicular);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeDiameterPerp;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimDiameterPerpendicular> OdDgDimDiameterPerpendicularPtr;

//-------------------------------------------------------------------
// OdDgDimCustomLinear
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimCustomLinear :  public OdDgDimension, public OdDgDimToolCustomLinear
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimCustomLinear);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeCustomLinear;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getLeftExtLinesPresentFlag() const;
  virtual void setLeftExtLinesPresentFlag( bool val );

  virtual bool getRightExtLinesPresentFlag() const;
  virtual void setRightExtLinesPresentFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator() const;
  virtual void setFirstTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator() const;
  virtual void setLeftTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator() const;
  virtual void setRightTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getJointTerminator() const;
  virtual void setJointTerminator( OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TextType getTextType() const;
  virtual void setTextType( OdDgDimTool::TextType val );

  virtual OdDgDimTool::CustomSymbol getPrefix() const;
  virtual void setPrefix( OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix() const;
  virtual void setSuffix( OdDgDimTool::CustomSymbol val );
};
typedef OdSmartPtr<OdDgDimCustomLinear> OdDgDimCustomLinearPtr;

//-------------------------------------------------------------------
// OdDgDimOrdinate
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOrdinate :  public OdDgDimension, public OdDgDimToolOrdinate
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimOrdinate);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeOrdinate;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getStackExtLinesFlag() const;
  virtual void setStackExtLinesFlag( bool val );

  virtual bool getArcSymbolFlag() const;
  virtual void setArcSymbolFlag( bool val );

  virtual bool getDecrementInReverseDirectionFlag() const;
  virtual void setDecrementInReverseDirectionFlag( bool val );

  virtual bool getFreeLocationOfTxtFlag() const;
  virtual void setFreeLocationOfTxtFlag( bool val );

  virtual double getDatumValue() const;
  virtual void setDatumValue( double val );

  // Initialize dimension properties by dimension style.
  virtual bool applyDimensionStyle( OdUInt32 uDimStyle );
};
typedef OdSmartPtr<OdDgDimOrdinate> OdDgDimOrdinatePtr;


//-------------------------------------------------------------------
// OdDgDimCenter
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimCenter :  public OdDgDimension, public OdDgDimToolCenter
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimCenter);

public:
  virtual ToolType dimensionType() const
  {
    return kToolTypeCenter;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual bool getCenterMarkLeftExtendFlag() const;
  virtual void setCenterMarkLeftExtendFlag( bool val );

  virtual bool getCenterMarkRightExtendFlag() const;
  virtual void setCenterMarkRightExtendFlag( bool val );

  virtual bool getCenterMarkTopExtendFlag() const;
  virtual void setCenterMarkTopExtendFlag( bool val );

  virtual bool getCenterMarkBottomExtendFlag() const;
  virtual void setCenterMarkBottomExtendFlag( bool val );
};
typedef OdSmartPtr<OdDgDimCenter> OdDgDimCenterPtr;

//----------------------------------------------------------------------------

////////////////////////////////////////////////////////////
//            Dimension Rotation class                    //
//========================================================//

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimRotation
{

private:
  OdGeQuaternion m_quatDim;
  OdGeMatrix3d   m_matDim;
  bool           m_b3DDim;

public:

  OdDgDimRotation();
  OdDgDimRotation( OdDgDimension* pDim );
  OdDgDimRotation( OdGeQuaternion quatData, bool b3D );

  void create( OdGeQuaternion quatData, bool b3D );
  OdGeVector3d rotate( OdGeVector3d& vector ) const;
  OdGePoint3d rotate( OdGePoint3d& point ) const;
  OdGeMatrix3d getMatrix() const;

};

//========================================================//

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionExportPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgDimensionExportPE);
  long        m_references;     //used by each object separately
public:

  OdDgDimensionExportPE();
  ~OdDgDimensionExportPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  // Access to dimension tools

  virtual bool getStackExtLinesFlag( const OdDgDimension* pDim ) const;
  virtual void setStackExtLinesFlag( OdDgDimension* pDim, bool val );

  virtual bool getLeftExtLinesPresentFlag( const OdDgDimension* pDim ) const;
  virtual void setLeftExtLinesPresentFlag( OdDgDimension* pDim, bool val );

  virtual bool getRightExtLinesPresentFlag( const OdDgDimension* pDim ) const;
  virtual void setRightExtLinesPresentFlag( OdDgDimension* pDim, bool val );

  virtual bool getCenterMarkFlag( const OdDgDimension* pDim ) const;
  virtual void setCenterMarkFlag( OdDgDimension* pDim, bool val );

  virtual bool getCenterMarkLeftExtendFlag( const OdDgDimension* pDim ) const;
  virtual void setCenterMarkLeftExtendFlag( OdDgDimension* pDim, bool val );

  virtual bool getCenterMarkRightExtendFlag( const OdDgDimension* pDim ) const;
  virtual void setCenterMarkRightExtendFlag( OdDgDimension* pDim, bool val );

  virtual bool getCenterMarkTopExtendFlag( const OdDgDimension* pDim ) const;
  virtual void setCenterMarkTopExtendFlag( OdDgDimension* pDim, bool val );

  virtual bool getCenterMarkBottomExtendFlag( const OdDgDimension* pDim ) const;
  virtual void setCenterMarkBottomExtendFlag( OdDgDimension* pDim, bool val );

  virtual bool getArcSymbolFlag( const OdDgDimension* pDim ) const;
  virtual void setArcSymbolFlag( OdDgDimension* pDim, bool val );

  virtual bool getChordAlignFlag( const OdDgDimension* pDim ) const;
  virtual void setChordAlignFlag( OdDgDimension* pDim, bool val );

  virtual OdDgDimTool::TerminatorType getFirstTerminator( const OdDgDimension* pDim ) const;
  virtual void setFirstTerminator( OdDgDimension* pDim, OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getLeftTerminator( const OdDgDimension* pDim ) const;
  virtual void setLeftTerminator( OdDgDimension* pDim, OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getRightTerminator( const OdDgDimension* pDim ) const;
  virtual void setRightTerminator( OdDgDimension* pDim, OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TerminatorType getJointTerminator( const OdDgDimension* pDim ) const;
  virtual void setJointTerminator( OdDgDimension* pDim, OdDgDimTool::TerminatorType val );

  virtual OdDgDimTool::TextType getTextType( const OdDgDimension* pDim ) const;
  virtual void setTextType( OdDgDimension* pDim, OdDgDimTool::TextType val );

  virtual OdDgDimTool::CustomSymbol getPrefix( const OdDgDimension* pDim ) const;
  virtual void setPrefix( OdDgDimension* pDim, OdDgDimTool::CustomSymbol val );

  virtual OdDgDimTool::CustomSymbol getSuffix( const OdDgDimension* pDim ) const;
  virtual void setSuffix( OdDgDimension* pDim, OdDgDimTool::CustomSymbol val );

  virtual bool getDecrementInReverseDirectionFlag( const OdDgDimension* pDim ) const;
  virtual void setDecrementInReverseDirectionFlag( OdDgDimension* pDim, bool val );

  virtual bool getFreeLocationOfTxtFlag( const OdDgDimension* pDim ) const;
  virtual void setFreeLocationOfTxtFlag( OdDgDimension* pDim, bool val );

  virtual double getDatumValue( const OdDgDimension* pDim ) const;
  virtual void setDatumValue( OdDgDimension* pDim, double val );

  virtual OdDgDimTool::Leader getLeader( const OdDgDimension* pDim ) const;
  virtual void setLeader( OdDgDimension* pDim, OdDgDimTool::Leader val );

  // Access to dimension text

  double getDimensionTextWidth( const OdDgDimension* obj, OdUInt32 uTextNumber, bool bExcludeTolerance = false );
  double getDimensionTextHeight( const OdDgDimension* obj, OdUInt32 uTextNumber, bool bExcludeTolerance = false );
  OdGePoint3d getDimensionTextPosition( const OdDgDimension* obj, OdUInt32 uTextNumber );
  OdGeVector3d getDimensionTextDirection( const OdDgDimension* obj, OdUInt32 uTextNumber );

  bool isTextInsideOfDimension( const OdDgDimension* obj, OdUInt32 uTextNumber );

  // To draw part of complex dimension |<-Seg1->|<-Seg2->|...|<-SegN->|

  void   drawDimensionSegment( const OdDgDimension* obj, OdGiWorldDraw* pWd, OdUInt32 uSegmentNum ) const;

  // Other dimension data

  double getWitnessLineSlantAngle( const OdDgDimension* obj ) const;
  void   setWitnessLineSlantAngle( const OdDgDimension* obj, double dAngle );

  bool   getUseDimLeaderAutomaticMode( const OdDgDimension* obj ) const;
  void   setUseDimLeaderAutomaticMode( const OdDgDimension* obj, bool bSet );
  };

typedef OdSmartPtr<OdDgDimensionExportPE> OdDgDimensionExportPEPtr;
#include "TD_PackPop.h"

#endif // __DG_DIMENSION_H__
