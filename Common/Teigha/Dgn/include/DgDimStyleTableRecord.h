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
#ifndef __DG_DIMSTYLETABLERECORD_H__
#define __DG_DIMSTYLETABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"
#include "DgDimension.h"

/** \details
    This class represents DimStyle records in the OdDgDimStyleTable in an OdDgDatabase instance.
 
    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimStyleTableRecord : public OdDgTableRecord
{
 ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDimStyleTableRecord);
public:
  //virtual const Error getTool ( const enum DimensionToolType iIndex, IDGDimensionTool** ppTool );
  //virtual const Error getCustomSymbol ( const enum CustomSymbolIdentifyer eID, IDGDimCustomSymbol** ppCS );

  //virtual const Error getPrimaryMasterUnitInterface ( IDGMeasureUnit** ppUnit );
  //virtual const Error getPrimarySubUnitInterface ( IDGMeasureUnit** ppUnit );

  //virtual const Error getSecondaryMasterUnitInterface ( IDGMeasureUnit** ppUnit );
  //virtual const Error getSecondarySubUnitInterface ( IDGMeasureUnit** ppUnit );

  //virtual const Error getDimLineSettings ( IDGDimLineSettings** ppLine );
  //virtual const Error getDimTextSettings ( IDGDimTextSettings** ppText );
  //virtual const Error getDimExtSettings ( IDGDimExtLineSettings** ppExtLine );
  //virtual const Error getDimTermSettings ( IDGDimTerminators** ppTerm );
  //virtual const Error getDimToleranceSettings ( IDGDimToleranceSettings** ppTol );
  //virtual const Error getDimUnitsSettings ( IDGDimUnitsSettings** ppUnits );
  //virtual const Error getDimUnitFormatSettings( IDGDimUnitFormatSettings** ppUF );
  //virtual const Error getDimPlacementSettings ( IDGDimPlacementSettings** ppPS );
  //virtual const Error getDimLeaderSettings ( IDGDimLeaderSettings** ppLS );

  // the set of functions, which determine what components of dimension are visible
  virtual bool getShowDimLineFlag() const;
  virtual bool getShowExtLineFlag() const;
  // Get enable tolerance flag.
  virtual bool getShowToleranceFlag() const;

  virtual void setShowDimLineFlag( bool );
  virtual void setShowExtLineFlag( bool );
  // Set enable tolerance flag.
  virtual void setShowToleranceFlag( bool );

  // set of functions, which handle the dimension special symbols
  virtual OdChar getMainPrefix() const;
  virtual OdChar getMainSuffix() const;
  virtual OdChar getTolerancePrefix() const;
  virtual OdChar getToleranceSuffix() const;
  virtual OdChar getUpperPrefix() const;
  virtual OdChar getUpperSuffix() const;
  virtual OdChar getLowerPrefix() const;
  virtual OdChar getLowerSuffix() const;

  virtual void setMainPrefix( OdChar );
  virtual void setMainSuffix( OdChar );
  virtual void setTolerancePrefix( OdChar );
  virtual void setToleranceSuffix( OdChar );
  virtual void setUpperPrefix( OdChar );
  virtual void setUpperSuffix( OdChar );
  virtual void setLowerPrefix( OdChar );
  virtual void setLowerSuffix( OdChar );

public:
  /** \details
  Notification function called by the Teigha™ for .dgn files framework immediately before an object is opened. 

  \param mode [in]  Mode in which the object is being opened.

  \remarks
  This function is notified just before an object is to be opened; giving this function
  the ability to cancel the object being opened.

  Returns eOk if and only if open() is to continue.

  Overriding this function in a child class allows a child instance to be notified ach time an
  object is opened.

  When overriding this function:

  1)  If the OdDgElement's state is incorrect, return 
  something other than eOk.

  2)  If the parent class's subOpen() returns anything
  other than eOk, immediately return it. 

  3)  If other actions are required before open, do them.

  4)  Return eOk.

  The default implementation of this function does nothing but return eOk.  This function can be
  overridden in custom classes.
  */
  virtual OdResult subOpen( OdDg::OpenMode mode);
  /** \details
  Called as the first operation as this object is being closed, for
  database -resident objects only. 

  When overriding this function:

  1)  If the OdDgElement's state is incorrect, return 
  something other than eOk.

  2)  If the parent class's subClose() returns anything
  other than eOk, immediately return it. 

  3)  If other actions are required before close, do them.

  4)  Return eOk.

  If you must make changes to this object's state, either make them after
  step 2, or roll them back if step 2 returns false. 

  The default implementation of this function does nothing but return eOk.  This function can be
  overridden in custom classes.
  */
  virtual OdResult subClose();

  // Get dimension style description.
  virtual OdString getDescription() const;
  // Set dimension style description
  virtual void setDescription(const OdString description);
  // Get dimension style entry id
  virtual OdUInt32 getEntryId() const;
  // Get parent dimension style entry id
  virtual OdUInt32 getParentDimensionStyleEntryId() const;
  // Set parent dimension style entry id
  virtual void setParentDimensionStyleEntryId(const OdUInt32 uDimStyleEntryId );
  // Get text style entry id
  virtual OdUInt32 getTextStyleEntryId() const;
  // Set text style entry id
  virtual void setTextStyleEntryId(const OdUInt32 uTextStyleEntryId );
  // Get accuracy for primary units.
  virtual OdDgDimTextFormat::Accuracy getPrimaryAccuracy() const;
  // Set accuracy for primary units.
  virtual void setPrimaryAccuracy(const OdDgDimTextFormat::Accuracy iAccur );
  // Get accuracy for secondary units.
  virtual OdDgDimTextFormat::Accuracy getSecondaryAccuracy() const;
  // Set accuracy for secondary units.
  virtual void setSecondaryAccuracy(const OdDgDimTextFormat::Accuracy iAccur );
  // Get the level on which dimensions are placed.
  virtual OdUInt32 getLevel() const;
  // Set the level on which dimensions are placed.
  virtual void setLevel( const OdUInt32 uLevel );
  // Get placement alignment for dimension style.
  virtual OdDgDimension::PlacementAlignment getPlacementAlignment() const;
  // Set placement alignment for dimension style.
  virtual void setPlacementAlignment( const OdDgDimension::PlacementAlignment iAlign );
  // Get the manual placement location flag. If it isn't set the mode is Automatic or Semi-Auto.
  // To detect Semi-Auto mode call getSemiAutoFlag()
  virtual bool getManualLocationFlag() const;
  // Set the manual placement location flag.
  virtual void setManualLocationFlag( const bool bSet );
  // Get extension lines present flag.
  virtual bool getExtensionLinesPresentFlag() const;
  // Set extension lines present flag.
  virtual void setExtensionLinesPresentFlag( const bool bSet );
  // Get text justification.
  virtual OdDgDimTextInfo::TextAlignment getTextJustification() const;
  // Set text justification. Value OdDgDimTextInfo::kManualText isn't used.
  virtual void setTextJustification( const OdDgDimTextInfo::TextAlignment iJust );
  // Get show angle format flag. If it's enabled, then angle format units is "Angle", otherwise it's "Length"
  virtual bool getShowAngleFormatFlag() const;
  // Set show angle format flag.
  virtual void setShowAngleFormatFlag( const bool bSet );
  // Get hide sub units if its value is zero flag for primary units.
  virtual bool getHideZeroPrimarySubUnitsFlag() const;
  // Set hide sub units if its value is zero flag for primary units.
  virtual void setHideZeroPrimarySubUnitsFlag( const bool bSet );
  // Get hide sub units if its value is zero flag for secondary units.
  virtual bool getHideZeroSecondarySubUnitsFlag() const;
  // Set hide sub units if its value is zero flag for secondary units.
  virtual void setHideZeroSecondarySubUnitsFlag( const bool bSet );
  // Get show master units if its value is zero flag for primary units.
  virtual bool getShowZeroPrimaryMasterUnitsFlag() const;
  // Set show master units if its value is zero flag for primary units.
  virtual void setShowZeroPrimaryMasterUnitsFlag( const bool bSet );
  // Get show master units if its value is zero flag for secondary units.
  virtual bool getShowZeroSecondaryMasterUnitsFlag() const;
  // Set show master units if its value is zero flag for secondary units.
  virtual void setShowZeroSecondaryMasterUnitsFlag( const bool bSet );
  // Get use custom units as primary units flag. If its value is false then use working units as primary units.
  virtual bool getUseCustomUnitsFlag() const;
  // Set use custom units as primary units flag.
  virtual void setUseCustomUnitsFlag( const bool bSet );
  // Get dual mode ( show both primary and secondary units ) flag.
  virtual bool getShowSecondaryUnitsFlag() const;
  // Set dual mode flag.
  virtual void setShowSecondaryUnitsFlag( const bool bSet );
  // Get tolerance mode. if true then mode is "Limit", otherwise mode is "Plus/Minus".
  virtual bool getUseToleranceLimitModeFlag() const;
  // Set tolerance mode.
  virtual void setUseToleranceLimitModeFlag( const bool bSet );
  // Get Inline text location flag. To get text location recommended to use getTextLocation() function. 
  virtual bool getEmbedTextFlag() const;
  // Set Inline text location flag. To set text location recommended to use setTextLocation() function.
  virtual void setEmbedTextFlag( const bool bSet );
  // Get Horizontal text orientation flag. If flag isn't set then text orientation is Aligned.
  virtual bool getHorizontalTextFlag() const;
  // Set Horizontal text orientation flag.
  virtual void setHorizontalTextFlag( const bool bSet );
  // Get tolerance stack if equal flag.
  virtual bool getToleranceStackIfEqualFlag() const;
  // Set tolerance stack if equal flag.
  virtual void setToleranceStackIfEqualFlag( const bool bSet );
  // Get label display mode for primary units.
  virtual OdDgDimTextFormat::LabelDisplayMode getPrimaryLabelDisplayMode() const;
  // Set label display mode for primary units.
  virtual void setPrimaryLabelDisplayMode( const OdDgDimTextFormat::LabelDisplayMode iMode );
  // Get label display mode for secondary units.
  virtual OdDgDimTextFormat::LabelDisplayMode getSecondaryLabelDisplayMode() const;
  // Set label display mode for secondary units.
  virtual void setSecondaryLabelDisplayMode( const OdDgDimTextFormat::LabelDisplayMode iMode );
  // Get text location.
  virtual OdDgDimTextInfo::TextLocation getTextLocation() const;
  // Set text location.
  virtual void setTextLocation( const OdDgDimTextInfo::TextLocation iLocation );
  // Get arrow symbol font entry id.
  virtual OdUInt32 getArrowFont() const;
  // Set arrow symbol font entry id.
  virtual void setArrowFont( const OdUInt32 uFontEntryID );
  // Get stroke symbol font entry id.
  virtual OdUInt32 getStrokeFont() const;
  // Set stroke symbol font entry id.
  virtual void setStrokeFont( const OdUInt32 uFontEntryID );
  // Get origin symbol font entry id.
  virtual OdUInt32 getOriginFont() const;
  // Set origin symbol font entry id.
  virtual void setOriginFont( const OdUInt32 uFontEntryID );
  // Get diameter symbol font entry id.
  virtual OdUInt32 getDiameterFont() const;
  // Set diameter symbol font entry id.
  virtual void setDiameterFont( const OdUInt32 uFontEntryID );
  // Get arrow symbol char code.
  virtual OdUInt16 getArrowChar() const;
  // Set arrow symbol char code.
  virtual void setArrowChar( const OdUInt16 uCharCode );
  // Get stroke symbol char code.
  virtual OdUInt16 getStrokeChar() const;
  // Set stroke symbol char code.
  virtual void setStrokeChar( const OdUInt16 uCharCode );
  // Get origin symbol char code.
  virtual OdUInt16 getOriginChar() const;
  // Set origin symbol char code.
  virtual void setOriginChar( const OdUInt16 uCharCode );
  // Get diameter symbol char code.
  virtual OdUInt16 getDiameterChar() const;
  // Set diameter symbol char code.
  virtual void setDiameterChar( const OdUInt16 uCharCode );
  // Get dimension text height.
  virtual double getTextHeight() const;
  // Set dimension text height.
  virtual void setTextHeight( const double dHeight );
  // Get lower tolerance value.
  virtual double getLowerToleranceValue() const;
  // Set lower tolerance value.
  virtual void setLowerToleranceValue( const double dValue );
  // Get upper tolerance value.
  virtual double getUpperToleranceValue() const;
  // Set upper tolerance value.
  virtual void setUpperToleranceValue( const double dValue );
  // Get tolerance text scale factor.
  virtual double getToleranceTextScale() const;
  // Set tolerance text scale factor.
  virtual void setToleranceTextScale( const double dScale );
  // Get geometry witness line offset.
  virtual double getWitnessLineOffset() const;
  // Set geometry witness line offset.
  virtual void setWitnessLineOffset( const double dOffset );
  // Get geometry witness line extension.
  virtual double getWitnessLineExtension() const;
  // Set geometry witness line extension.
  virtual void setWitnessLineExtension( const double dExtension );
  // Get dimension value scale factor.
  virtual double getDimensionScale() const;
  // Set dimension value scale factor.
  virtual void setDimensionScale( const double dScale );
  // Get geometry margin. ( min. leader length )
  virtual double getGeometryMargin() const;
  // Set geometry margin.
  virtual void setGeometryMargin( const double dMargin );
  // Get joiner flag. If it's true then join extension lines when terminator orientation is outside. 
  virtual bool getJoinerFlag() const;
  // Set joiner flag.
  virtual void setJoinerFlag( const bool bSet );
  // Get draw box around text flag.
  virtual bool getBoxTextFlag() const;
  // Set draw box around text flag.
  virtual void setBoxTextFlag( const bool bSet );
  // Get semi-auto placement location mode flag.
  virtual bool getSemiAutoFlag() const;
  // Set semi-auto placement location mode flag.
  virtual void setSemiAutoFlag( const bool bSet );
  // Get leading zero flag for primary units.
  virtual bool getPrimaryLeadingZeroFlag() const;
  // Set leading zero flag for primary units.
  virtual void setPrimaryLeadingZeroFlag( const bool bSet );
  // Get trailing zeros flag for primary units.
  virtual bool getPrimaryTrailingZerosFlag() const;
  // Set trailing zeros flag for primary units.
  virtual void setPrimaryTrailingZerosFlag( const bool bSet );
  // Get leading zero flag for secondary units.
  virtual bool getSecondaryLeadingZeroFlag() const;
  // Set leading zero flag for secondary units.
  virtual void setSecondaryLeadingZeroFlag( const bool bSet );
  // Get trailing zeros flag for secondary units.
  virtual bool getSecondaryTrailingZerosFlag() const;
  // Set trailing zeros flag for secondary units.
  virtual void setSecondaryTrailingZerosFlag( const bool bSet );
  // Get decimal comma flag.
  virtual bool getDecimalCommaFlag() const;
  // Set decimal comma flag.
  virtual void setDecimalCommaFlag( const bool bSet );
  // Get draw capsule around text flag.
  virtual bool getCapsuleTextFlag() const;
  // Set draw capsule around text flag.
  virtual void setCapsuleTextFlag( const bool bSet );
  // Get superscript LSD flag. The least significant digit is displayed as superscript.
  virtual bool getSuperscriptLSDFlag() const;
  // Set superscript LSD flag.
  virtual void setSuperscriptLSDFlag( const bool bSet );
  // Get round LSD flag. If on, round to the closest half unit. 0 is not displayed.
  virtual bool getRoundLSDFlag() const;
  // Set round LSD flag.
  virtual void setRoundLSDFlag( const bool bSet );
  // Get omit leading delimiter flag.
  virtual bool getOmitLeadingDelimiterFlag() const;
  // Set omit leading delimiter flag.
  virtual void setOmitLeadingDelimiterFlag( const bool bSet );
  // Get dimension line color override flag.
  virtual bool getDimLinesColorOverrideFlag() const;
  // Set dimension line color override flag.
  virtual void setDimLinesColorOverrideFlag( const bool bSet );
  // Get dimension line weight override flag.
  virtual bool getDimLinesWeightOverrideFlag() const;
  // Set dimension line weight override flag.
  virtual void setDimLinesWeightOverrideFlag( const bool bSet );
  // Get text color override flag.
  virtual bool getTextColorOverrideFlag() const;
  // Set text color override flag.
  virtual void setTextColorOverrideFlag( const bool bSet );
  // Get text weight override flag.
  virtual bool getTextWeightOverrideFlag() const;
  // Set text weight override flag.
  virtual void setTextWeightOverrideFlag( const bool bSet );
  // Get text font override flag.
  virtual bool getTextFontOverrideFlag() const;
  // Set text font override flag.
  virtual void setTextFontOverrideFlag( const bool bSet );
  // Get text size override flag.
  virtual bool getTextHeightOverrideFlag() const;
  // Set text size override flag.
  virtual void setTextHeightOverrideFlag( const bool bSet );
  // Get level override flag.
  virtual bool getLevelOverrideFlag() const;
  // Set level override flag.
  virtual void setLevelOverrideFlag( const bool bSet );
  // Get explode dimension to simple elements after creation flag.
  virtual bool getDropDimensionAfterCraetionFlag() const;
  // Set explode dimension to simple elements after creation flag.
  virtual void setDropDimensionAfterCraetionFlag( const bool bSet );
  // Get terminator arrow head type.
  virtual OdDgDimension::TerminatorArrowHeadType getTerminatorArrowHead() const;
  // Set terminator arrow head type.
  virtual void setTerminatorArrowHead( const OdDgDimension::TerminatorArrowHeadType iTerm );
  // Get use reference file scale flag.
  virtual bool getUseReferenceFileScaleFlag() const;
  // Set use reference file scale flag.
  virtual void setUseReferenceFileScaleFlag( const bool bSet );
  // Get relative dimension lines flag.
  virtual bool getRelativeDimensionLineFlag() const;
  // Set relative dimension lines flag.
  virtual void setRelativeDimensionLineFlag( const bool bSet );
  // Get underline text mode flag.
  virtual bool getTextUnderlineFlag() const;
  // Set underline text mode flag.
  virtual void setTextUnderlineFlag( const bool bSet );
  // Get dimension line style override flag.
  virtual bool getDimLinesStyleOverrideFlag() const;
  // Set dimension line style override flag.
  virtual void setDimLinesStyleOverrideFlag( const bool bSet );
  // Get dimension line adjust automatically flag.
  virtual bool getNoAutoTextLiftFlag() const;
  // Set dimension line adjust automatically flag.
  virtual void setNoAutoTextLiftFlag( const bool bSet );
  // Get terminator orientation type.
  virtual OdDgDimension::TerminatorOrientation getTerminatorOrientation() const;
  // Set terminator orientation type.
  virtual void setTerminatorOrientation( const OdDgDimension::TerminatorOrientation iOrient );
  // Get master file units flag.
  virtual bool getMasterFileUnitsFlag() const;
  // Set master file units flag.
  virtual void setMasterFileUnitsFlag( const bool bSet );
  // Get override level symbology flag.
  virtual bool getOverrideLevelSymbologyFlag() const;
  // Set override level symbology flag.
  virtual void setOverrideLevelSymbologyFlag( const bool bSet );
  // Get save view rotation option flag.
  virtual bool getViewRotationFlag() const;
  // Set save view rotation option flag.
  virtual void setViewRotationFlag( const bool bSet );
  // Get dimension lines geometry stack offset.
  virtual double getStackOffset() const;
  // Set dimension lines geometry stack offset.
  virtual void setStackOffset( const double dOffset );
  // Get center mark size.
  virtual double getCenterMarkSize() const;
  // Set center mark size.
  virtual void setCenterMarkSize( const double dSize );
  // Get current dimension command type.
  virtual OdDgDimension::ToolType getCurrentDimensionCmd() const;
  // Set current dimension command type.
  virtual void setCurrentDimensionCmd( const OdDgDimension::ToolType iCmd );
  // Get display mode for angle value.
  virtual OdDgDimTextFormat::AngleValueDisplayMode getAngleDisplayMode() const;
  // Set display mode for angle value.
  virtual void setAngleDisplayMode( const OdDgDimTextFormat::AngleValueDisplayMode iMode );
  // Get accuracy for angle value.
  virtual OdDgDimTextFormat::Accuracy getAngleAccuracy() const;
  // To set accuracy value for angular dimension you must convert them to angle format and
  // set primary accuracy value for dimension element. 
  // To convert accuracy to angle format you must change value by rule:
  // -----------------------------------------------------------------------
  //           Accuracy                            Angle Accuracy
  // -----------------------------------------------------------------------
  // OdDgDimTextFormat::kAccuracyNone = OdDgDimTextFormat::kAccuracyNone (0)
  // OdDgDimTextFormat::kDecimal1     =                  1
  // OdDgDimTextFormat::kDecimal2     =                  2
  // OdDgDimTextFormat::kDecimal3     =                  3
  // OdDgDimTextFormat::kDecimal4     =                  4
  // OdDgDimTextFormat::kDecimal5     =                  5
  // OdDgDimTextFormat::kDecimal6     =                  6
  // -----------------------------------------------------------------------
  // To set angle accuracy for dimension style use this function and accuracy value ( don't change format )
  virtual void setAngleAccuracy( const OdDgDimTextFormat::Accuracy iAccur );
  // Get dimension lines color.
  virtual OdUInt32 getDimensionColor() const;
  // Set dimension lines color.
  virtual void setDimensionColor( const OdUInt32 uColor );
  // Get dimension lines weight.
  virtual OdUInt32 getDimensionLineWeight() const;
  // Set dimension lines weight.
  virtual void setDimensionLineWeight( const OdUInt32 uWeight );
  // Get dimension text color.
  virtual OdUInt32 getTextColor() const;
  // Set dimension text color.
  virtual void setTextColor( const OdUInt32 uColor );
  // Get dimension text weight.
  virtual OdUInt32 getTextWeight() const;
  // Set dimension text weight.
  virtual void setTextWeight( const OdUInt32 uWeight );
  // Get dimension text font entry id.
  virtual OdUInt32 getTextFontId() const;
  // Set dimension text font entry id.
  virtual void setTextFontId( const OdUInt32 uFontID );
  // Get dot symbol font entry id.
  virtual OdUInt32 getDotSymbolFontId() const;
  // Set dot symbol font entry id.
  virtual void setDotSymbolFontId( const OdUInt32 uFontID );
  // Get dot symbol char code.
  virtual OdUInt16 getDotSymbolCode() const;
  // Set dot symbol char code.
  virtual void setDotSymbolCode( const OdUInt16 uCode );
  // Get lending zero flag for angle units.
  virtual bool getAngleLeadingZeroFlag() const;
  // Set lending zero flag for angle units.
  virtual void setAngleLeadingZeroFlag( const bool bSet );
  // Get trailing zeros flag for angle units.
  virtual bool getAngleTrailingZerosFlag() const;
  // Set trailing zeros flag for angle units.
  virtual void setAngleTrailingZerosFlag( const bool bSet );
  // Get superscript char flag. If false then used special font for superscript char, 
  // otherwise used automatic mode for superscript char.
  virtual bool getAutoSuperscriptCharFlag() const;
  // Set superscript char.
  virtual void setAutoSuperscriptCharFlag( const bool bSet );
  // Get dimension line style entry id.
  virtual OdUInt32 getDimensionLineStyleId() const;
  // Set dimension line style entry id.
  virtual void setDimensionLineStyleId( const OdUInt32 uLStyleID );
  // Get text lower margin value.
  virtual double getTextLowerMargin() const;
  // Set text lower margin value.
  virtual void setTextLowerMargin( const double dMargin );
  // Get text left margin value.
  virtual double getTextLeftMargin() const;
  // Set text left margin value.
  virtual void setTextLeftMargin( const double dMargin );
  // Get tolerance left margin value.
  virtual double getToleranceLeftMargin() const;
  // Set tolerance left margin value.
  virtual void setToleranceLeftMargin( const double dMargin );
  // Get tolerance separator margin value.
  virtual double getToleranceSepMargin() const;
  // Set tolerance separator margin value.
  virtual void setToleranceSepMargin( const double dMargin );
  // Get show sign for zero tolerance value flag.
  virtual bool getToleranceShowSignForZeroFlag() const;
  // Set show sign for zero tolerance value flag.
  virtual void setToleranceShowSignForZeroFlag( const bool bSet );
  // Get terminator height.
  virtual double getTerminatorHeight() const;
  // Set terminator height.
  virtual void setTerminatorHeight( const double dHeight );
  // Get terminator width.
  virtual double getTerminatorWidth() const;
  // Set terminator width.
  virtual void setTerminatorWidth( const double dWidth );
  // Get text symbol middle width.
  virtual double getTextWidth() const;
  // Set text symbol middle width.
  virtual void setTextWidth( const double dWidth );
  // Get text width override flag.
  virtual bool getTextWidthOverrideFlag() const;
  // Set text width override flag.
  virtual void setTextWidthOverrideFlag( const bool bSetFlag );
  // Get extension line line style override flag.
  virtual bool getExtensionLineStyleOverrideFlag() const;
  // Set extension line line style override flag.
  virtual void setExtensionLineStyleOverrideFlag( const bool bSetFlag );
  // Get extension line line weight override flag.
  virtual bool getExtensionLineWeightOverrideFlag() const;
  // Set extension line line weight override flag.
  virtual void setExtensionLineWeightOverrideFlag( const bool bSetFlag );
  // Get extension line color override flag.
  virtual bool getExtensionLineColorOverrideFlag() const;
  // Set extension line color override flag.
  virtual void setExtensionLineColorOverrideFlag( const bool bSetFlag );
  // Get terminator line style override flag.
  virtual bool getTerminatorLineStyleOverrideFlag() const;
  // Set terminator line style override flag.
  virtual void setTerminatorLineStyleOverrideFlag( const bool bSetFlag );
  // Get terminator line weight override flag.
  virtual bool getTerminatorLineWeightOverrideFlag() const;
  // Set terminator line weight override flag.
  virtual void setTerminatorLineWeightOverrideFlag( const bool bSetFlag );
  // Get terminator color override flag.
  virtual bool getTerminatorColorOverrideFlag() const;
  // Set terminator color override flag.
  virtual void setTerminatorColorOverrideFlag( const bool bSetFlag );
  // Get attachment frame type for note dimension.
  virtual OdDgDimNote::NoteFrameType getNoteFrameType() const;
  // Set attachment frame type for note dimension.
  virtual void setNoteFrameType( const OdDgDimNote::NoteFrameType iFrameType );
  // Get use note inline leader mode flag.
  virtual bool getNoteInlineLeaderFlag() const;
  // Set use note inline leader mode flag.
  virtual void setNoteInlineLeaderFlag( const bool bSetFlag );
  // Get justification for note dimension.
  virtual OdDgDimNote::NoteJustification getNoteJustification() const;
  // Set justification for note dimension.
  virtual void setNoteJustification( const OdDgDimNote::NoteJustification iJust );
  // Get metric space flag. If true then draw separator for values more then thousand.
  virtual bool getMetricSpaceFlag() const;
  // Set metric space flag.
  virtual void setMetricSpaceFlag( const bool bSetFlag );
  // Get thousand separator flag. If true then separator is comma else separator is space.
  virtual bool getThousandSeparatorFlag() const;
  // Set thousand separator flag.
  virtual void setThousandSeparatorFlag( const bool bSetFlag );
  // Get stacked fraction type.
  virtual OdDgDimTextInfo::StackedFractionType getStackedFractionType() const;
  // Set stacked fraction type.
  virtual void setStackedFractionType( const OdDgDimTextInfo::StackedFractionType iType );
  // Get stacked fraction alignment.
  virtual OdDgDimTextInfo::StackFractionAlignment getStackedFractionAlign() const;
  // Set stacked fraction alignment.
  virtual void setStackedFractionAlign( const OdDgDimTextInfo::StackFractionAlignment iAlign );
  // Get enable extended stacked fraction mode.
  virtual bool getUseStackedFractionFlag() const;
  // Set enable extended stacked fraction mode.
  virtual void setUseStackedFractionFlag( const bool bSetFlag );
  // Get enable uniformly scales terminator cells by the text width.
  virtual bool getUniformCellScaleFlag() const;
  // Set enable uniformly scales terminator cells by the text width.
  virtual void setUniformCellScaleFlag( const bool bSetFlag );
  // Get line style entry id for dimension extension lines.
  virtual OdUInt32 getExtensionLineStyleId() const;
  // Set line style entry id for dimension extension lines.
  virtual void setExtensionLineStyleId( const OdUInt32 uStyleId );
  // Get line weight for dimension extension lines.
  virtual OdUInt32 getExtensionLineWeight() const;
  // Set line weight for dimension extension lines.
  virtual void setExtensionLineWeight( const OdUInt32 uWeight );
  // Get color for dimension extension lines.
  virtual OdUInt32 getExtensionLineColor() const;
  // Set color for dimension extension lines.
  virtual void setExtensionLineColor( const OdUInt32 uColor );
  // Get line style entry id for dimension terminators.
  virtual OdUInt32 getTerminatorLineStyleId() const;
  // Set line style entry id for dimension terminators.
  virtual void setTerminatorLineStyleId( const OdUInt32 uStyleId );
  // Get line weight for dimension terminators.
  virtual OdUInt32 getTerminatorLineWeight() const;
  // Set line weight for dimension terminators.
  virtual void setTerminatorLineWeight( const OdUInt32 uWeight );
  // Get color for dimension terminators.
  virtual OdUInt32 getTerminatorColor() const;
  // Set color for dimension terminators.
  virtual void setTerminatorColor( const OdUInt32 uColor );
  // Get symbol type for arrow terminators.
  virtual OdDgDimOptionTerminators::TerminatorStyle getArrowSymbolType() const;
  // Set symbol type for arrow terminators.
  virtual void setArrowSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get symbol type for stroke terminators.
  virtual OdDgDimOptionTerminators::TerminatorStyle getStrokeSymbolType() const;
  // Set symbol type for stroke terminators.
  virtual void setStrokeSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get symbol type for origin terminators.
  virtual OdDgDimOptionTerminators::TerminatorStyle getOriginSymbolType() const;
  // Set symbol type for origin terminators.
  virtual void setOriginSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get symbol type for dot terminators.
  virtual OdDgDimOptionTerminators::TerminatorStyle getDotSymbolType() const;
  // Set symbol type for dot terminators.
  virtual void setDotSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get prefix symbol type.
  virtual OdDgDimOptionTerminators::TerminatorStyle getPrefixSymbolType() const;
  // Set prefix symbol type.
  virtual void setPrefixSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get suffix symbol type.
  virtual OdDgDimOptionTerminators::TerminatorStyle getSuffixSymbolType() const;
  // Set suffix symbol type.
  virtual void setSuffixSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get diameter symbol type.
  virtual OdDgDimOptionTerminators::TerminatorStyle getDiameterSymbolType() const;
  // Set diameter symbol type.
  virtual void setDiameterSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get plus/minus symbol type.
  virtual OdDgDimOptionTerminators::TerminatorStyle getPlusMinusSymbolType() const;
  // Set plus/minus symbol type.
  virtual void setPlusMinusSymbolType( const OdDgDimOptionTerminators::TerminatorStyle iType );
  // Get hide leader line into arrow terminator space.
  virtual bool getNoLineThroughArrowTerminatorFlag() const;
  // Set hide leader line into arrow terminator space.
  virtual void setNoLineThroughArrowTerminatorFlag( const bool bSetFlag );
  // Get hide leader line into stroke terminator space.
  virtual bool getNoLineThroughStrokeTerminatorFlag() const;
  // Set hide leader line into stroke terminator space.
  virtual void setNoLineThroughStrokeTerminatorFlag( const bool bSetFlag );
  // Get hide leader line into origin terminator space.
  virtual bool getNoLineThroughOriginTerminatorFlag() const;
  // Set hide leader line into origin terminator space.
  virtual void setNoLineThroughOriginTerminatorFlag( const bool bSetFlag );
  // Get hide leader line into dot terminator space.
  virtual bool getNoLineThroughDotTerminatorFlag() const;
  // Set hide leader line into dot terminator space.
  virtual void setNoLineThroughDotTerminatorFlag( const bool bSetFlag );
  // Get underline text override flag.
  virtual bool getUnderlineOverrideFlag() const;
  // Set underline text override flag.
  virtual void setUnderlineOverrideFlag( const bool bSetFlag );  
  // Get non-stacked fraction space flag.
  virtual bool getNonStackedFractionSpaceFlag() const;
  // Set non-stacked fraction space flag.
  virtual void setNonStackedFractionSpaceFlag( const bool bSetFlag );
  // Get prefix symbol font entry id.
  virtual OdUInt32 getPrefixFont() const;
  // Set prefix symbol font entry id.
  virtual void setPrefixFont( const OdUInt32 uFontId );
  // Get suffix symbol font entry id.
  virtual OdUInt32 getSuffixFont() const;
  // Set suffix symbol font entry id.
  virtual void setSuffixFont( const OdUInt32 uFontId );
  // Get prefix symbol code.
  virtual OdUInt16 getPrefixChar() const;
  // Set prefix symbol code.
  virtual void setPrefixChar( const OdUInt16 uChar );
  // Get suffix symbol code.
  virtual OdUInt16 getSuffixChar() const;
  // Set suffix symbol code.
  virtual void setSuffixChar( const OdUInt16 uChar );
  // Get plus/minus symbol code.
  virtual OdUInt16 getPlusMinusChar() const;
  // Set plus/minus symbol code.
  virtual void setPlusMinusChar( const OdUInt16 uChar );
  // Get threshold for alternate format for primary units.
  virtual double getPrimaryAltThreshold() const;
  // Set threshold for alternate format for primary units.
  virtual void setPrimaryAltThreshold( const double dValue );
  // Get label display mode for alternate format for primary units.
  virtual OdDgDimTextFormat::LabelDisplayMode getPrimaryAltLabelDisplayMode() const;
  // Set label display mode for alternate format for primary units.
  virtual void setPrimaryAltLabelDisplayMode( const OdDgDimTextFormat::LabelDisplayMode iMode );
  // Get comparison operator alternate format for primary units.
  virtual OdDgDimOptionAltFormat::ComparisonOperator getPrimaryAltOperator() const;
  // Set comparison operator for alternate format for primary units.
  virtual void setPrimaryAltOperator( const OdDgDimOptionAltFormat::ComparisonOperator iOper );
  // Get subunit threshold flag for alternate format for primary units.
  virtual bool getPrimaryAltSubunitThresholdFlag() const;
  // Set subunit threshold flag for alternate format for primary units.
  virtual void setPrimaryAltSubunitThresholdFlag( const bool bSetFlag );
  // Get show zero master units flag for alternate format for primary units.
  virtual bool getPrimaryAltShowZeroMasterUnitsFlag() const;
  // Set show zero master units flag for alternate format for primary units.
  virtual void setPrimaryAltShowZeroMasterUnitsFlag( const bool bSetFlag );
  // Get hide zero sub units flag for alternate format for primary units.
  virtual bool getPrimaryAltHideZeroSubUnitsFlag() const;
  // Set hide zero sub units flag for alternate format for primary units.
  virtual void setPrimaryAltHideZeroSubUnitsFlag( const bool bSetFlag );
  // Get enable alternate format for primary units.
  virtual bool getPrimaryAltPresentFlag() const;
  // Set enable alternate format for primary units.
  virtual void setPrimaryAltPresentFlag( const bool bSetFlag );
  // Get accuracy for alternate format for primary units.
  virtual OdDgDimTextFormat::Accuracy getPrimaryAltAccuracy() const;
  // Set accuracy for alternate format for primary units.
  virtual void setPrimaryAltAccuracy( const OdDgDimTextFormat::Accuracy iAccur );
  // Get leader terminator for dimension with leader.
  virtual OdDgDimTool::TerminatorType getDimLeaderTerminator() const;
  // Set leader terminator for dimension with leader.
  virtual void setDimLeaderTerminator( const OdDgDimTool::TerminatorType iTerm );
  // Get chain type for dimension with leader.
  virtual OdDgDimOptionOffset::ChainType getDimLeaderChainType() const;
  // Set chain type for dimension with leader.
  virtual void setDimLeaderChainType( const OdDgDimOptionOffset::ChainType iType );
  // Get text alignment for dimension with leader.
  virtual OdDgDimOptionOffset::LeaderAlignment getDimLeaderAlignment() const;
  // Set text alignment for dimension with leader.
  virtual void setDimLeaderAlignment( const OdDgDimOptionOffset::LeaderAlignment iAlign );
  // Get inline leader flag for dimension with leader.
  virtual bool getDimLeaderInlineLeaderFlag() const;
  // Set inline leader flag for dimension with leader.
  virtual void setDimLeaderInlineLeaderFlag( const bool bSetFlag );
  // Get enable dimension with leader flag.
  virtual bool getDimLeaderEnableFlag() const;
  // Set enable dimension with leader flag.
  virtual void setDimLeaderEnableFlag( const bool bSetFlag );
  // Get undock text flag for dimension with leader flag.
  virtual bool getDimLeaderUndockTextFlag() const;
  // Set undock text flag for dimension with leader flag.
  virtual void setDimLeaderUndockTextFlag( const bool bSetFlag );
  // Get threshold for alternate format for secondary units.
  virtual double getSecondaryAltThreshold() const;
  // Set threshold for alternate format for secondary units.
  virtual void setSecondaryAltThreshold( const double dValue );
  // Get label display mode for alternate format for secondary units.
  virtual OdDgDimTextFormat::LabelDisplayMode getSecondaryAltLabelDisplayMode() const;
  // Set label display mode for alternate format for secondary units.
  virtual void setSecondaryAltLabelDisplayMode( const OdDgDimTextFormat::LabelDisplayMode iMode );
  // Get comparison operator alternate format for secondary units.
  virtual OdDgDimOptionAltFormat::ComparisonOperator getSecondaryAltOperator() const;
  // Set comparison operator for alternate format for secondary units.
  virtual void setSecondaryAltOperator( const OdDgDimOptionAltFormat::ComparisonOperator iOper );
  // Get subunit threshold flag for alternate format for secondary units.
  virtual bool getSecondaryAltSubunitThresholdFlag() const;
  // Set subunit threshold flag for alternate format for secondary units.
  virtual void setSecondaryAltSubunitThresholdFlag( const bool bSetFlag );
  // Get show zero master units flag for alternate format for secondary units.
  virtual bool getSecondaryAltShowZeroMasterUnitsFlag() const;
  // Set show zero master units flag for alternate format for secondary units.
  virtual void setSecondaryAltShowZeroMasterUnitsFlag( const bool bSetFlag );
  // Get hide zero sub units flag for alternate format for secondary units.
  virtual bool getSecondaryAltHideZeroSubUnitsFlag() const;
  // Set hide zero sub units flag for alternate format for secondary units.
  virtual void setSecondaryAltHideZeroSubUnitsFlag( const bool bSetFlag );
  // Get enable alternate format for secondary units.
  virtual bool getSecondaryAltPresentFlag() const;
  // Set enable alternate format for secondary units.
  virtual void setSecondaryAltPresentFlag( const bool bSetFlag );
  // Get accuracy for alternate format for secondary units.
  virtual OdDgDimTextFormat::Accuracy getSecondaryAltAccuracy() const;
  // Set accuracy for alternate format for secondary units.
  virtual void setSecondaryAltAccuracy( const OdDgDimTextFormat::Accuracy iAccur );
  // Get primary master units.
  virtual OdDgModel::UnitDescription getPrimaryMasterUnits() const;
  // Set primary master units.
  virtual void setPrimaryMasterUnits( const OdDgModel::UnitDescription units );
  // Get primary sub units.
  virtual OdDgModel::UnitDescription getPrimarySubUnits() const;
  // Set primary sub units.
  virtual void setPrimarySubUnits( const OdDgModel::UnitDescription units );
  // Get secondary master units.
  virtual OdDgModel::UnitDescription getSecondaryMasterUnits() const;
  // Set secondary master units.
  virtual void setSecondaryMasterUnits( const OdDgModel::UnitDescription units );
  // Get secondary sub units.
  virtual OdDgModel::UnitDescription getSecondarySubUnits() const;
  // Set secondary sub units.
  virtual void setSecondarySubUnits( const OdDgModel::UnitDescription units );
  // Get annotation scale for dimension. If scale isn't used the function returns 1.
  virtual double getAnnotationScale() const;
  // Set annotation scale for dimension.
  virtual void setAnnotationScale( const double dScale );
  // Get accuracy for tolerance for primary units.
  virtual OdDgDimTextFormat::Accuracy getPrimaryToleranceAccuracy() const;
  // Set accuracy for tolerance for primary units.
  virtual void setPrimaryToleranceAccuracy( const OdDgDimTextFormat::Accuracy iAccur );
  // Get accuracy for tolerance for secondary units.
  virtual OdDgDimTextFormat::Accuracy getSecondaryToleranceAccuracy() const;
  // Set accuracy for tolerance for secondary units.
  virtual void setSecondaryToleranceAccuracy( const OdDgDimTextFormat::Accuracy iAccur );
  // Get stacked fraction scale for dimension. If scale isn't used the function returns 1.
  virtual double getStackedFractionScale() const;
  // Set stacked fraction scale for dimension.
  virtual void setStackedFractionScale( const double dScale );
  // Get datum value for ordinate dimension.
  virtual double getOrdinateDimensionDatumValue() const;
  // Set datum value for ordinate dimension.
  virtual void setOrdinateDimensionDatumValue( const double dValue );
  // Get inline leader length for dimension with leader.
  virtual double getDimLeaderInlineLeaderLength() const;
  // Set inline leader length for dimension with leader.
  virtual void setDimLeaderInlineLeaderLength( const double dLength );
  // Get decrement in reverse direction flag for ordinate dimensions.
  virtual bool getOrdinateDimDecrementInReverseDirectionFlag() const;
  // Set decrement in reverse direction flag for ordinate dimensions.
  virtual void setOrdinateDimDecrementInReverseDirectionFlag( const bool bSetFlag );
  // Get free text location flag for ordinate dimensions.
  virtual bool getOrdinateDimFreeLocationTextFlag() const;
  // Set free text location flag for ordinate dimensions.
  virtual void setOrdinateDimFreeLocationTextFlag( const bool bSetFlag );
  // Get use geometry margin value for terminator leader. If false then used default value.
  virtual bool getEnableTerminatorMinLeaderLengthFlag() const;
  // Set use geometry margin value for terminator leader.
  virtual void setEnableTerminatorMinLeaderLengthFlag( const bool bSetFlag );
  // Get enable suppress unfit terminators flag.
  virtual bool getEnableSuppressUnfitTerminatorFlag() const;
  // Set enable suppress unfit terminators flag.
  virtual void setEnableSuppressUnfitTerminatorFlag( const bool bSetFlag );
  // Get enable inline leader mode for dimensions with leader.
  virtual bool getDimLeaderEnableInlineLeaderLengthFlag() const;
  // Set enable inline leader mode for dimensions with leader.
  virtual void setDimLeaderEnableInlineLeaderLengthFlag( const bool bSetFlag );
  // Get enable text optimal fit flag for above text location.
  virtual bool getEnableTextAboveOptimalFitFlag() const;
  // Set enable text optimal fit flag for above text location.
  virtual void setEnableTextAboveOptimalFitFlag( const bool bSetFlag );
  // Get fit true dimension text option. If true then value is "Narrow", otherwise value is "Wide".
  virtual bool getEnableNarrowFontOptimalFitFlag() const;
  // Set fit true dimension text option.
  virtual void setEnableNarrowFontOptimalFitFlag( const bool bSetFlag );
  // Get retain fractional accuracy flag for primary units.
  virtual bool getPrimaryRetainFractionalAccuracyFlag() const;
  // Set retain fractional accuracy flag for primary units.
  virtual void setPrimaryRetainFractionalAccuracyFlag( const bool bSetFlag );
  // Get retain fractional accuracy flag for secondary units.
  virtual bool getSecondaryRetainFractionalAccuracyFlag() const;
  // Set retain fractional accuracy flag for secondary units.
  virtual void setSecondaryRetainFractionalAccuracyFlag( const bool bSetFlag );
  // Get retain fractional accuracy flag for tolerance of primary units.
  virtual bool getPrimaryToleranceRetainFractionalAccuracyFlag() const;
  // Set retain fractional accuracy flag for tolerance of primary units.
  virtual void setPrimaryToleranceRetainFractionalAccuracyFlag( const bool bSetFlag );
  // Get retain fractional accuracy flag for tolerance of secondary units.
  virtual bool getSecondaryToleranceRetainFractionalAccuracyFlag() const;
  // Set retain fractional accuracy flag for tolerance of secondary units.
  virtual void setSecondaryToleranceRetainFractionalAccuracyFlag( const bool bSetFlag );
  // Get retain fractional accuracy flag for alternate format for primary units.
  virtual bool getPrimaryAltRetainFractionalAccuracyFlag() const;
  // Set retain fractional accuracy flag for alternate format for primary units.
  virtual void setPrimaryAltRetainFractionalAccuracyFlag( const bool bSetFlag );
  // Get retain fractional accuracy flag for alternate format for secondary units.
  virtual bool getSecondaryAltRetainFractionalAccuracyFlag() const;
  // Set retain fractional accuracy alternate format for flag for secondary units.
  virtual void setSecondaryAltRetainFractionalAccuracyFlag( const bool bSetFlag );
  // Get fit dependent text/terminator options.
  virtual OdDgDimTextInfo::FitOptions getFitOptions() const;
  // Set fit dependent text/terminator options.
  virtual void setFitOptions( const OdDgDimTextInfo::FitOptions iOpt );
  // Get the display and location of length and angle value for label-line dimension.
  virtual OdDgDimLabelLine::LabelLineDimensionMode getLabelLineDimensionMode() const;
  // Set the display and location of length and angle value for label-line dimension.
  virtual void setLabelLineDimensionMode( const OdDgDimLabelLine::LabelLineDimensionMode iMode );
  // Get line spline fit for note dimension.
  virtual bool getNoteSplineFitFlag() const;
  // Set line spline fit for note dimension.
  virtual void setNoteSplineFitFlag( const bool bSetFlag );
  // Get enable frame scale flag for note dimension. If false then note frame scale is 1.
  virtual bool getEnableNoteFrameScaleFlag() const;
  // Set enable frame scale flag for note dimension.
  virtual void setEnableNoteFrameScaleFlag( const bool bSetFlag );
  // Get leader length for note dimension. If value is 0 then used default leader length.
  virtual double getNoteLeaderLength() const;
  // Set leader length for note dimension.
  virtual void setNoteLeaderLength( const double dLength );
  // Get left margin for note dimension. If value is 0 then used default left margin.
  virtual double getNoteLeftMargin() const;
  // Set left margin for note dimension.
  virtual void setNoteLeftMargin( const double dMargin );
  // Get lower margin for note dimension.
  virtual double getNoteLowerMargin() const;
  // Set lower margin for note dimension.
  virtual void setNoteLowerMargin( const double dMargin );
  // Get frame scale for note dimension.
  virtual double getNoteFrameScale() const;
  // Set frame scale for note dimension.
  virtual void setNoteFrameScale( const double dScale );
  // Get terminator type for note dimension.
  virtual OdDgDimTool::TerminatorType getNoteTerminator() const;
  // Set terminator type for note dimension.
  virtual void setNoteTerminator( const OdDgDimTool::TerminatorType iTerm );
  // Get text rotation type for note dimension.
  virtual OdDgDimTextInfo::TextRotation getNoteTextRotation() const;
  // Set text rotation type for note dimension.
  virtual void setNoteTextRotation( const OdDgDimTextInfo::TextRotation iRot );
  // Get leader direction of horizontal attachment for note dimension.
  virtual OdDgDimNote::NoteAttachmentDirection getNoteHorizontalAttachmentLeaderDirection() const;
  // Set leader direction of horizontal attachment for note dimension.
  virtual void setNoteHorizontalAttachmentLeaderDirection( const OdDgDimNote::NoteAttachmentDirection iDir );
  // Get edit about type for note dimension.
  virtual OdDgDimNote::NoteEditAbout getNoteEditAbout() const;
  // Set edit about type for note dimension.
  virtual void setNoteEditAbout( const OdDgDimNote::NoteEditAbout iMode );
  // Get alignment for attachment with left justification for note dimension.
  virtual OdDgDimNote::NoteAttachmentAlignment getNoteLeftAttachmentAlignment() const;
  // Set alignment for attachment with left justification for note dimension.
  virtual void setNoteLeftAttachmentAlignment( const OdDgDimNote::NoteAttachmentAlignment iAlign );
  // Get alignment for attachment with right justification for note dimension.
  virtual OdDgDimNote::NoteAttachmentAlignment getNoteRightAttachmentAlignment() const;
  // Set alignment for attachment with right justification for note dimension.
  virtual void setNoteRightAttachmentAlignment( const OdDgDimNote::NoteAttachmentAlignment iAlign );
  // Get text lift for text with inline location.
  virtual double getInlineTextLift() const;
  // Set text lift for text with inline location.
  virtual void setInlineTextLift( const double dMargin );
  // Get extend dimension line under text flag.
  virtual bool getExtendDimLineUnderTextFlag() const;
  // Set extend dimension line under text flag.
  virtual void setExtendDimLineUnderTextFlag( const bool bSetFlag );
  // Get auto mode for dimension with leader.
  virtual bool getDimLeaderAutoLeaderModeFlag() const;
  // Set auto mode for dimension with leader.
  virtual void setDimLeaderAutoLeaderModeFlag( const bool bSetFlag );
  // Get cell name for arrow symbol.
  virtual OdString getArrowCellName() const;
  // Set cell name for arrow symbol.
  virtual void setArrowCellName(const OdString strName );
  // Get cell name for stroke symbol.
  virtual OdString getStrokeCellName() const;
  // Set cell name for stroke symbol.
  virtual void setStrokeCellName(const OdString strName );
  // Get cell name for origin symbol.
  virtual OdString getOriginCellName() const;
  // Set cell name for origin symbol.
  virtual void setOriginCellName(const OdString strName );
  // Get cell name for dot symbol.
  virtual OdString getDotCellName() const;
  // Set cell name for dot symbol.
  virtual void setDotCellName(const OdString strName );
  // Get cell name for suffix symbol.
  virtual OdString getSuffixCellName() const;
  // Set cell name for suffix symbol.
  virtual void setSuffixCellName(const OdString strName );
  // Get cell name for prefix symbol.
  virtual OdString getPrefixCellName() const;
  // Set cell name for prefix symbol.
  virtual void setPrefixCellName(const OdString strName );

  // Tool functions

  // Get first terminator. For radial and diametrical dimensions it returns Leader type.
  virtual OdDgDimTool::TerminatorType getToolTermFirst(OdDgDimension::ToolType iType) const;
  // Set first terminator.
  virtual void setToolTermFirst( OdDgDimension::ToolType iType, const OdDgDimTool::TerminatorType iTerm );
  // Get left terminator.
  virtual OdDgDimTool::TerminatorType getToolTermLeft(OdDgDimension::ToolType iType) const;
  // Set left terminator.
  virtual void setToolTermLeft( OdDgDimension::ToolType iType, const OdDgDimTool::TerminatorType iTerm );
  // Get right terminator.
  virtual OdDgDimTool::TerminatorType getToolTermRight(OdDgDimension::ToolType iType) const;
  // Set right terminator.
  virtual void setToolTermRight( OdDgDimension::ToolType iType, const OdDgDimTool::TerminatorType iTerm );
  // Get joint terminator.
  virtual OdDgDimTool::TerminatorType getToolTermJoint(OdDgDimension::ToolType iType) const;
  // Set joint terminator.
  virtual void setToolTermJoint( OdDgDimension::ToolType iType, const OdDgDimTool::TerminatorType iTerm );
  // Get tool prefix.
  virtual OdDgDimTool::CustomSymbol getToolPrefix(OdDgDimension::ToolType iType) const;
  // Set tool prefix.
  virtual void setToolPrefix( OdDgDimension::ToolType iType, const OdDgDimTool::CustomSymbol iSymb );
  // Get tool suffix.
  virtual OdDgDimTool::CustomSymbol getToolSuffix(OdDgDimension::ToolType iType) const;
  // Set tool suffix.
  virtual void setToolSuffix( OdDgDimension::ToolType iType, const OdDgDimTool::CustomSymbol iSymb );
  // Get tool text type.
  virtual OdDgDimTool::TextType getToolTextType(OdDgDimension::ToolType iType) const;
  // Set tool text type.
  virtual void setToolTextType( OdDgDimension::ToolType iType, const OdDgDimTool::TextType iTerm );
  // Get stack extension lines flag.
  virtual bool getToolStackExtLinesFlag(OdDgDimension::ToolType iType) const;
  // Set stack extension lines flag.
  virtual void setToolStackExtLinesFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show arc symbol flag.
  virtual bool getToolExtLinesArcFlag(OdDgDimension::ToolType iType) const;
  // Set show arc symbol flag.
  virtual void setToolExtLinesArcFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show left extension line flag.
  virtual bool getToolLeftExtLinesPresentFlag(OdDgDimension::ToolType iType) const;
  // Set show left extension line flag.
  virtual void setToolLeftExtLinesPresentFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show right extension line flag.
  virtual bool getToolRightExtLinesPresentFlag(OdDgDimension::ToolType iType) const;
  // Set show right extension line flag.
  virtual void setToolRightExtLinesPresentFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show center mark flag.
  virtual bool getToolCenterMarkFlag(OdDgDimension::ToolType iType) const;
  // Set show center mark flag.
  virtual void setToolCenterMarkFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show center mark left extension line flag.
  virtual bool getToolCenterMarkLeftExtendFlag(OdDgDimension::ToolType iType) const;
  // Set show center mark left extension line flag.
  virtual void setToolCenterMarkLeftExtendFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show center mark right extension line flag.
  virtual bool getToolCenterMarkRightExtendFlag(OdDgDimension::ToolType iType) const;
  // Set show center mark right extension line flag.
  virtual void setToolCenterMarkRightExtendFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show center mark top extension line flag.
  virtual bool getToolCenterMarkTopExtendFlag(OdDgDimension::ToolType iType) const;
  // Set show center mark top extension line flag.
  virtual void setToolCenterMarkTopExtendFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get show center mark bottom extension line flag.
  virtual bool getToolCenterMarkBottomExtendFlag(OdDgDimension::ToolType iType) const;
  // Set show center mark bottom extension line flag.
  virtual void setToolCenterMarkBottomExtendFlag( OdDgDimension::ToolType iType, const bool bSet );
  // Get enable chord align flag.
  virtual bool getToolChordAlignFlag(OdDgDimension::ToolType iType) const;
  // Set enable chord align flag.
  virtual void setToolChordAlignFlag( OdDgDimension::ToolType iType, const bool bSet );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDimStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgDimStyleTableRecord> OdDgDimStyleTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_DIMSTYLETABLERECORD_H__
