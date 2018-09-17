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
#ifndef __DG_TAG_ELEMENT_H__
#define __DG_TAG_ELEMENT_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgTagSetDefinition.h"
#include "OdString.h"
#include "OdBinaryData.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"
#include "DgTextExtendedProperties.h"

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTagAssociationReactor : public OdDgElementReactor
{
public:

  static OdDgTagAssociationReactor* attach( const OdDgElementId& idDgElement, const OdDgElementId& idTagElement );
  OdDgElementId getTagElementId() const;
  virtual void closed( const OdDgElementId& elementId);
  virtual void transformTag( const OdGeMatrix3d& matTransform );
private:
  OdDgElementId  m_idTagElement;
};

typedef OdSmartPtr< OdDgTagAssociationReactor > OdDgTagAssociationReactorPtr;


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTagElement : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTagElement );

public:

  //
  //position
  //

  OdGePoint3d getOrigin() const;
  void setOrigin( const OdGePoint3d& point );

  OdGeVector3d getOffset() const;
  void setOffset( const OdGeVector3d& offset );

  //2D
  double getRotation2d() const;
  void setRotation2d( double rotation );

  //3D
  OdGeQuaternion getRotation3d() const;
  void setRotation3d( const OdGeQuaternion& rotation );

  //
  //flags of the tag
  //

  bool getAssociatedFlag() const;
  void setAssociatedFlag( bool value );

  bool getOffsetUsedFlag() const;
  void setOffsetUsedFlag( bool value );

  bool getJustificationUsedFlag() const;
  void setJustificationUsedFlag( bool value );

  bool getHideForSharedCellFlag() const;
  void setHideForSharedCellFlag( bool value );

  //
  //settings
  //

  void getTagDefinitionId( OdDgElementId& setId, OdUInt16& definitionId ) const;
  void setTagDefinitionId( const OdDgElementId& setId, OdUInt16 definitionId );

  OdDgElementId getAssociationId() const;
  void setAssociationId( OdDgElementId elementId, bool bRecalculateOrigin = false );

  //this field is obsolete, should be zero
  OdUInt32 getFreezeGroup() const;
  void setFreezeGroup( OdUInt32 group );

  //
  //operations with data
  //

  OdDgTagDefinition::Type getDataType() const;
  void setDataType( OdDgTagDefinition::Type type ); //if the operation is successful, then the new type and zero value are set

  OdString getString() const;
  void setString( const OdString& string );

  OdInt16 getInt16() const;
  void setInt16( OdInt16 value );

  OdInt32 getInt32() const;
  void setInt32( OdInt32 value );

  double getDouble() const;
  void setDouble( double value );

  OdUInt16 getBinarySize() const; //returns zero if the current type is not binary
  void getBinaryData( OdBinaryData& data ) const;
  void setBinaryData( const OdBinaryData& data );

  //
  //draw flags for the text
  //

#undef TEXT_DRAW_FLAG_DEF
#define TEXT_DRAW_FLAG_DEF(name) \
  bool get##name##Flag() const; \
  void set##name##Flag(bool val);

  TEXT_DRAW_FLAG_DEF( UseInterCharSpacing )
  TEXT_DRAW_FLAG_DEF( FixedWidthSpacing )
  TEXT_DRAW_FLAG_DEF( Underline )
  TEXT_DRAW_FLAG_DEF( UseSlant )
  TEXT_DRAW_FLAG_DEF( Vertical )
  TEXT_DRAW_FLAG_DEF( RightToLeft )
  TEXT_DRAW_FLAG_DEF( ReverseMline )
  TEXT_DRAW_FLAG_DEF( Kerning )
  TEXT_DRAW_FLAG_DEF( UseCodepage )
  TEXT_DRAW_FLAG_DEF( UseShxBigFont )
  TEXT_DRAW_FLAG_DEF( Subscript )
  TEXT_DRAW_FLAG_DEF( Superscript )
  TEXT_DRAW_FLAG_DEF( UseTextStyle )

  //
  //extended flags for the text
  //

#undef TEXT_EXT_FLAG_DEF
#define TEXT_EXT_FLAG_DEF(name) \
  bool get##name##Flag() const; \
  void set##name##Flag(bool val);  
  
  TEXT_EXT_FLAG_DEF( Overline )
  TEXT_EXT_FLAG_DEF( Bold )
  TEXT_EXT_FLAG_DEF( FullJustification )
  TEXT_EXT_FLAG_DEF( AcadInterCharSpacing )
  TEXT_EXT_FLAG_DEF( Backwards )
  TEXT_EXT_FLAG_DEF( UpsideDown )
  TEXT_EXT_FLAG_DEF( AcadFittedText )

  //
  //settings of the text
  //

  double getSlant() const;
  void setSlant( double angle );

  double getCharacterSpacing() const;
  void setCharacterSpacing( double value );

  double getUnderlineSpacing() const;
  void setUnderlineSpacing( double value );

  double getLengthMultiplier() const;
  void setLengthMultiplier( double lengthMult );

  double getHeightMultiplier() const;
  void setHeightMultiplier( double heightMult );

  OdUInt32 getTextStyleEntryId() const;
  void setTextStyleEntryId( OdUInt32 id );

  OdUInt32 getShxBigFont() const;
  void setShxBigFont( OdUInt32 value );

  OdUInt32 getFontEntryId() const;
  void setFontEntryId( OdUInt32 ID );

  OdDg::TextJustification getJustification() const;
  // Note: the next method sets a proper "use justification" flag on. By default it is set off.
  // So setJustificationUsedFlag() will return true after this method call
  void setJustification( OdDg::TextJustification value );

  OdUInt32 getCodepage() const;
  void setCodepage( OdUInt32 codepage );

  virtual bool applyTextStyle( OdDgElementId idTextStyle );

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  void setLinkages( OdUInt16 appId, const OdRxObjectPtrArray& linkages);
  void removeLinkages( OdUInt16 appId );
  void addLinkage( OdUInt16 linkageId, OdRxObjectPtr pLinkage );
  void addLinkage( OdUInt16 linkageId, OdUInt8* pData, OdUInt32 dataSize);
};

typedef OdSmartPtr< OdDgTagElement > OdDgTagElementPtr;


#include "TD_PackPop.h"

#endif // __DG_TAG_ELEMENT_H__
