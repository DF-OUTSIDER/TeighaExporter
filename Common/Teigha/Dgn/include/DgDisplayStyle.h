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
#ifndef __DG_DISPLAYSTYLE_H__
#define __DG_DISPLAYSTYLE_H__


#include "TD_PackPush.h"
#include "DgElement.h"
#include "DgXAttribute.h"
#include "DgMaterialTableRecord.h"

/** \details
  Corresponding C++ library: TG_Db
  This class represents a display style (a collection of properties) in an OdDgDatabase instance.

  <group OdDb_Classes>
*/

class TG_EXPORT OdDgDisplayStyleTableRecord : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDisplayStyleTableRecord);
public:
  enum OdDgEnvironmentTypeDisplayed
  {
    kBackgroundColor    = 0,
    kEnvironment        = 1
  };

  enum OdDgDisplayMode
  {
    kWireframe          = 0,
    kVisibleEdges       = 3,
    kFilledVisibleEdges = 4,
    kShaded             = 6
  };

  enum OdDgDisplayStyleDisplayHeader
  {
    kNoHeader           = 0x00000000,
    kHeight             = 0x58ea0002,
    kSlope              = 0x58ea0004,
    kAspectAngle        = 0x58ea0006,
    kElementRange       = 0x58ea000d,
    kHillShade          = 0x58ea0012
  };

  enum OdDgDisplayStyleLineType
  {
    kDefault       = 0,
    kDotted        = 1,
    kDashed        = 2,
    kLongDash      = 3,
    kDashDot       = 4,
    kShortDash     = 5,
    kDashDotDot    = 6,
    kShortLongDash = 7
  };
public:

  /** \details
      Returns entry id of display style into display style table.
  */
  OdUInt32        getEntryId() const;

  /** \details
      Returns name of display style.
  */
  OdString        getName() const;

  /** \details
      Sets the name of display style.

      \param strName [in]  Reference to a string that contains the name for this display style to set.
  */
  void            setName( const OdString& strName );

  /** \details
      Returns the environment type of display style. If environment type is kBackgroundColor,
      then display style can change background color of view. Otherwise display style can 
      generate specific background, that described into environment record from array of 
      environments.
  */
  OdDgEnvironmentTypeDisplayed getEnvironmentTypeDisplayed() const;

  /** \details
      Sets the environment type of display style.

      \param uValue [in] Environment type value to set.
  */
  void            setEnvironmentTypeDisplayed( const OdDgEnvironmentTypeDisplayed uValue );

  /** \details
      Returns name of environment from environment table. It used if environment type is
      kEnvironment.
  */
  OdString        getEnvironmentName() const;

  /** \details
      Sets the environment name for display style.

      \param strEnvironmentName [in] Reference to a string that contains the name of environment
       from environment table to set.
  */
  void            setEnvironmentName( const OdString& strEnvironmentName );

  OdDoubleColor   getGroundPlaneColor() const;
  void            setGroundPlaneColor( const OdDoubleColor& clrGroundPlaneColor );

  double          getGroundPlaneHeight() const;
  void            setGroundPlaneHeight( const double dHeight );

  double          getGroundPlaneTransparency() const;
  void            setGroundPlaneTransparency( const double dTransparency );

  bool            getShowGroundFromBelowFlag() const;
  void            setShowGroundFromBelowFlag( const bool bSet );

  bool            getUseFileOrderForDisplayFlag() const;
  void            setUseFileOrderForDisplayFlag( const bool bSet );

  bool            getIgnoreGeometryMapsFlag() const;
  void            setIgnoreGeometryMapsFlag( const bool bSet );

  bool            getIgnorePatternMapsFlag() const;
  void            setIgnorePatternMapsFlag( const bool bSet );

  bool            getDisplayShadowsFlag() const;
  void            setDisplayShadowsFlag( const bool bSet );

  bool            getSmoothIgnoreLightsFlag() const;
  void            setSmoothIgnoreLightsFlag( const bool bSet );

  bool            getUseTransparencyThresholdFlag() const;
  void            setUseTransparencyThresholdFlag( const bool bSet );

  bool            getUseColorFromMaterialFlag() const;
  void            setUseColorFromMaterialFlag( const bool bSet );

  bool            getDisplayGroundPlaneFlag() const;
  void            setDisplayGroundPlaneFlag( const bool bSet );

  bool            getUseElementColorOverrideFlag() const;
  void            setUseElementColorOverrideFlag( const bool bSet );

  bool            getUseElementTransparencyOverrideFlag() const;
  void            setUseElementTransparencyOverrideFlag( const bool bSet );

  bool            getUseElementLineStyleOverrideFlag() const;
  void            setUseElementLineStyleOverrideFlag( const bool bSet );

  bool            getUseElementLineWeightOverrideFlag() const;
  void            setUseElementLineWeightOverrideFlag( const bool bSet );

  bool            getUseMaterialOverrideFlag() const;
  void            setUseMaterialOverrideFlag( const bool bSet );

  bool            getUseBackgroundColorOverrideFlag() const;
  void            setUseBackgroundColorOverrideFlag( const bool bSet );

  bool            getInvisibleToCameraFlag() const;
  void            setInvisibleToCameraFlag( const bool bSet );

  bool            getUseDisplayHandlerFlag() const;
  void            setUseDisplayHandlerFlag( const bool bSet );

  bool            getDisplayVisibleEdgesFlag() const;
  void            setDisplayVisibleEdgesFlag( const bool bSet );

  bool            getDisplayHiddenEdgesFlag() const;
  void            setDisplayHiddenEdgesFlag( const bool bSet );

  bool            getUseVisibleEdgeColorFlag() const;
  void            setUseVisibleEdgeColorFlag( const bool bSet );

  bool            getUseVisibleEdgeSolidLineFlag() const;
  void            setUseVisibleEdgeSolidLineFlag( const bool bSet );

  bool            getUseVisibleEdgeWeightFlag() const;
  void            setUseVisibleEdgeWeightFlag( const bool bSet );

  bool            getUseHiddenEdgeLineStyleFlag() const;
  void            setUseHiddenEdgeLineStyleFlag( const bool bSet );

  bool            getUseHiddenEdgeZeroWeightFlag() const;
  void            setUseHiddenEdgeZeroWeightFlag( const bool bSet );

  bool            getApplyEdgeStyleToLineFlag() const;
  void            setApplyEdgeStyleToLineFlag( const bool bSet );

  bool            getHideInPickerFlag() const;
  void            setHideInPickerFlag( const bool bSet );

  bool            getUsableForViewsFlag() const;
  void            setUsableForViewsFlag( const bool bSet );

  bool            getUsableForClipVolumesFlag() const;
  void            setUsableForClipVolumesFlag( const bool bSet );

  OdDgDisplayStyleTableRecord::OdDgDisplayMode getDisplayMode() const;
  void            setDisplayMode( const OdDgDisplayStyleTableRecord::OdDgDisplayMode uDisplayMode );

  OdUInt32        getVisibleEdgeColor() const;
  void            setVisibleEdgeColor( const OdUInt32 uColorIndex );

  OdUInt32        getVisibleEdgeWeight() const;
  void            setVisibleEdgeWeight( const OdUInt32 uWeight );

  OdUInt32        getBackgroundColor() const;
  void            setBackgroundColor( const OdUInt32 uColorIndex );

  OdUInt32        getElementColor() const;
  void            setElementColor( const OdUInt32 uColorIndex );

  OdDgDisplayStyleLineType getElementLineStyle() const;
  void            setElementLineStyle( const OdDgDisplayStyleLineType& uStyle );

  OdUInt32        getElementLineWeight() const;
  void            setElementLineWeight( const OdUInt32 uWeight );

  double          getElementTransparency() const;
  void            setElementTransparency( const double dTransparency );

  double          getTransparencyThreshold() const;
  void            setTransparencyThreshold( const double dTransparencyThreshold );

  OdUInt64        getMaterialHandle() const;
  void            setMaterialHandle( const OdUInt64 uMaterialHandle );

  OdDgDisplayStyleTableRecord::OdDgDisplayStyleDisplayHeader        getDisplayHeader() const;
  void            setDisplayHeader( const OdDgDisplayStyleTableRecord::OdDgDisplayStyleDisplayHeader uDisplayHeader );

  OdDgDisplayStyleLineType getHiddenEdgeLineStyle() const;
  void            setHiddenEdgeLineStyle( const OdDgDisplayStyleLineType& uStyle );

  OdUInt32        getExtendedPropertyCount();
  OdDgXMLProperty getExtendedProperty( OdUInt32 uPropIndex );
  void            setExtendedProperty( OdUInt32 uPropIndex, const OdDgXMLProperty& xmlProperty );
  void            addExtendedProperty( const OdDgXMLProperty& xmlProperty );
  void            deleteExtendedProperty( OdUInt32 uPropIndex );

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;
};

typedef OdSmartPtr<OdDgDisplayStyleTableRecord> OdDgDisplayStyleTableRecordPtr;

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDisplayStyleTable);
public:

   /** \details
    Searches the Table object for the specified TableRecord.

    \param recordName [in]  Record name.
    \param recordEntryId [in]  Record entry id.
    \param getErasedRecord [in]  If and only if true, erased records will be opened or retrieved.

    \returns
    Returns a OdDgElementId of the object if successful, otherwise a null element id.
  */
  virtual OdDgElementId getAt(
    const OdString& recordName, 
    bool getErasedRecord = false) const;
  virtual OdDgElementId getAt(
    OdUInt32 recordEntryId, 
    bool getErasedRecord = false) const;

  /** \details
    Returns true if and only if this Table object has a record with the specified ElementId.

    \param elementId [in]  Element ID.
  */
  virtual bool has(
    const OdDgElementId& elementId) const;

  /** \details
    Adds the specified record to this Table object.

    \param pRecord [in]  Pointer to the record to add.
    \param bCheckData [in]  If it false then check only record name, not editional properties

    \returns
    Returns the Element ID of the newly added record.
  */
  virtual void add(
    OdDgDisplayStyleTableRecordPtr pRecord, bool bCheckData = true );

  /** \details
    Returns an Iterator object that can be used to traverse this Table object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted records are skipped.
  */
  virtual OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

};

typedef OdSmartPtr<OdDgDisplayStyleTable> OdDgDisplayStyleTablePtr;


#include "TD_PackPop.h"

#endif // __DG_DISPLAYSTYLE_H__
