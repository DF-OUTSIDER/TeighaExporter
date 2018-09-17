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
#ifndef __DG_LEVELTABLERECORD_H__
#define __DG_LEVELTABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"
//#include "CmColor.h"
#include "DgCmColor.h"
#include "OdBinaryData.h" //MKU-temporary

struct OdDgAssignedMaterial
{
  OdUInt32 m_uColorIndex;
  OdString m_strMaterialName;
};

/** \details
    This class represents Level records in the OdDgLevelTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLevelTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLevelTableRecord);
public:

  OdUInt32 getEntryId() const;

  OdUInt32 getNumber() const;
  void setNumber(OdUInt32 number);

  /* The element symbology fields (the color, the weight, the style) are the symbology that is used 
   *  to display an element on that level when 'bylevel' value is set on the symbology fields into element.
   */
  virtual void getElementColor( OdCmColorBase& ) const;
  virtual void setElementColor( const OdCmColorBase& );
  virtual OdUInt32 getElementColorIndex() const;
  virtual void setElementColorIndex( OdUInt32 );

  virtual OdUInt32 getElementLineStyleEntryId() const;
  virtual void setElementLineStyleEntryId( OdUInt32 );

  virtual OdUInt32 getElementLineWeight() const;
  virtual void setElementLineWeight( OdUInt32 );

  //virtual void getElementFillColor( OdCmColorBase& ) const;
  //virtual void setElementFillColor( const OdCmColorBase& );
  //virtual OdUInt32 getElementFillColorIndex() const;
  //virtual void setElementFillColorIndex( OdUInt32 colorIndex );

  /* The level symbology fields are the symbology that is used to display an element on that level when the
   *  Level Symbology attribute of the view is turned on, so long as the override flags for color, style and
   *  weightOverride of the level are set to true.
   */

  virtual void getOverrideColor( OdCmColorBase& ) const;
  virtual void setOverrideColor( const OdCmColorBase& );
  virtual OdUInt32 getOverrideColorIndex() const;
  virtual void setOverrideColorIndex( OdUInt32 colorIndex );

  virtual OdUInt32 getOverrideLineStyleEntryId() const;
  virtual void setOverrideLineStyleEntryId( OdUInt32 );

  virtual OdUInt32 getOverrideLineWeight() const;
  virtual void setOverrideLineWeight( OdUInt32 );

  //virtual void getOverrideFillColor( OdCmColorBase& ) const;
  //virtual void setOverrideFillColor( const OdCmColorBase& );
  //virtual OdUInt32 getOverrideFillColorIndex() const;
  //virtual void setOverrideFillColorIndex( OdUInt32 colorIndex );

  virtual bool getUseOverrideColorFlag() const;
  virtual void setUseOverrideColorFlag( bool );

  virtual bool getUseOverrideLineStyleFlag() const;
  virtual void setUseOverrideLineStyleFlag( bool );

  virtual bool getUseOverrideLineWeightFlag() const;
  virtual void setUseOverrideLineWeightFlag( bool );

  //virtual bool getUseOverrideFillColorFlag() const;
  //virtual void setUseOverrideFillColorFlag( bool );

  virtual bool getIsDisplayedFlag() const;
  virtual void setIsDisplayedFlag( bool );

  bool getIsPlotFlag() const;
  void setIsPlotFlag(bool val);

  bool getIsExternalFlag() const;
  void setIsExternalFlag(bool val);

  //bool getIsByCellFlag() const;
  //void setIsByCellFlag(bool val);

  bool getIsSnapFlag() const;
  void setIsSnapFlag(bool val);

  bool getIsReadOnlyFlag() const;
  void setIsReadOnlyFlag(bool val);

  bool getIsHiddenFlag() const;
  void setIsHiddenFlag(bool val);

  enum ElementAccess
  {
      kAccessAll = 0
    , kAccessLocked
    , kAccessReadOnly
    , kAccessViewOnly
    , kUnknown
  };

  ElementAccess getElementAccess() const;
  void setElementAccess(ElementAccess val);

  bool getIsFrozenFlag() const;
  void setIsFrozenFlag(bool val);

  bool getIsCustomStyleFromMasterFlag();
  void setIsCustomStyleFromMasterFlag(bool val);

  virtual OdString getDescription();
  virtual void setDescription(const OdString& name);

  virtual OdCmTransparency getTransparency() const;
  virtual void setTransparency( const OdCmTransparency& val );

  virtual bool getUseTransparencyFlag() const;

  /*
   *  The lowest priority value is (-500). 
   *  The highest priority value is (500).
   *  If value is out of this range it will be cast according to this limitation.
   */
  virtual OdInt32 getPriority() const;
  virtual void setPriority( OdInt32 val );

  //bool getIsInUse() const;
  //OdInt32 getLibraryID() const;
  //bool getIsLibrary() const;

  // OdGiDrawable overridden
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;

  virtual OdRxObjectPtr clone() const;

//  OdBinaryData getUnknownData() const;
//  OdBinaryData getTransparency() const;

  virtual bool                 getUseOverrideMaterialFlag() const;
  virtual void                 setUseOverrideMaterialFlag( bool bSet );

  virtual OdDgElementId        getOverrideMaterial() const;
  virtual void                 setOverrideMaterial( const OdDgElementId& idMaterial );
  virtual OdString             getOverrideMaterialName() const;
  virtual void                 setOverrideMaterialName( const OdString& strName );
  virtual OdString             getOverrideMaterialPalette() const;
  virtual void                 setOverrideMaterialPalette( const OdString& strName );

  virtual OdDgElementId        getByLevelMaterial() const;
  virtual void                 setByLevelMaterial( const OdDgElementId& idMaterial );
  virtual OdString             getByLevelMaterialName() const;
  virtual void                 setByLevelMaterialName( const OdString& strName );
  virtual OdString             getByLevelMaterialPalette() const;
  virtual void                 setByLevelMaterialPalette( const OdString& strName );

  /** \remarks
    The methods are deprecated now. Use OdDgMaterialAssignmentPE methods to work with material
  assignments.
  */
  virtual OdUInt32             getAssignedMaterialsCount() const;
  virtual OdDgAssignedMaterial getAssignedMaterial( OdUInt32 uIndex ) const;
  virtual void                 setAssignedMaterial( OdUInt32 uIndex, const OdDgAssignedMaterial& matData );
  virtual void                 addAssignedMaterial( const OdDgAssignedMaterial& matData );
  virtual void                 deleteAssignedMaterial( OdUInt32 uIndex );

  virtual bool                 isTrueColorOverride() const;
  virtual OdDgCmEntityColor    getTrueColorOverride() const;

  /** \details
    Determines whether the level record object is in-use when the generateUsageData() method  
    is called and returns true if the level is in-use, or false if the level is not use.
    
    \remarks
    Returns true when the generateUsageData() of the level table object has not been called
    or this level record object is not database resident.

    OdDbLevelTable::generateUsageData() method
  */
  bool isInUse() const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLevelTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLevelTableRecord> OdDgLevelTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_LEVELTABLERECORD_H__
