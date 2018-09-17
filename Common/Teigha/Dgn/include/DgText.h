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
#ifndef __DG_TEXT_H__
#define __DG_TEXT_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "OdString.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"
#include "DgTextExtendedProperties.h"


/** \details

   Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
struct OdDgTextEditField
{
  OdDgTextEditField()
    : m_uStartPosition( 0 )
    , m_uLength( 0 )
    , m_uJustification( 0 )
  {}

  OdDgTextEditField( OdUInt8 startPosition, OdUInt8 length, OdUInt8 justification )
    : m_uStartPosition( startPosition )
    , m_uLength( length )
    , m_uJustification( justification )
  {}

  OdUInt8 m_uStartPosition;
  OdUInt8 m_uLength;
  OdUInt8 m_uJustification;
};


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class OdDgTextExtended : public OdDgTextExtendedProperties
                       , public OdDgTextStyleOverrideFlags
{
public:
  OdDgTextExtended()
  {}
  virtual ~OdDgTextExtended()
  {}

  virtual OdString getText() const = 0;
  virtual void setText( const OdString& text ) = 0;

  virtual OdUInt32 getFontEntryId () const = 0;
  virtual void setFontEntryId ( OdUInt32 fontId ) = 0;

  virtual OdDg::TextJustification getJustification() const = 0;
  virtual void setJustification( OdDg::TextJustification justification ) = 0;

  virtual double getLength () const = 0;
  virtual void setLength ( double length ) = 0;

  virtual double getHeight () const = 0;
  virtual void setHeight ( double height ) = 0;

  virtual double getLengthMultiplier () const = 0;
  virtual void setLengthMultiplier ( double lengthMult ) = 0;

  virtual double getHeightMultiplier () const = 0;
  virtual void setHeightMultiplier ( double heightMult ) = 0;

  virtual OdInt16 getTextEditFieldCount() const = 0;
  virtual OdDgTextEditField getTextEditFieldAt( OdInt16 index ) const = 0;
  virtual void setTextEditFieldAt( OdInt16 index, const OdDgTextEditField& field ) = 0;
  virtual void addTextEditField(  const OdDgTextEditField& field ) = 0;
  virtual void removeTextEditFieldAt( OdInt16 index ) = 0;
};


//2D subtype of text

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgText2d : public OdDgGraphicsElement
                           , public OdDgTextExtended
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgText2d );
public:

  //the text
  virtual OdString getText() const;
  virtual void setText( const OdString& text );

  // MKU 06/19/07
  virtual OdUInt32  getFontEntryId () const;
  virtual void      setFontEntryId   ( OdUInt32 fontId );

  virtual OdDg::TextJustification   getJustification() const;
  virtual void                      setJustification( OdDg::TextJustification justification );

  virtual double  getLength () const;
  virtual void    setLength ( double length );

  virtual double  getHeight () const;
  virtual void    setHeight ( double height );

  virtual double  getLengthMultiplier () const;
  virtual void    setLengthMultiplier ( double lengthMult );

  virtual double  getHeightMultiplier () const;
  virtual void    setHeightMultiplier ( double heightMult );

  virtual OdInt16 getTextEditFieldCount() const;
  virtual OdDgTextEditField getTextEditFieldAt( OdInt16 index ) const;
  virtual void setTextEditFieldAt( OdInt16 index, const OdDgTextEditField& field );
  virtual void addTextEditField(  const OdDgTextEditField& field );
  virtual void removeTextEditFieldAt( OdInt16 index );

  //2d-specific methods
  virtual double getRotation() const;
  virtual void setRotation  ( double angle );

  // get/set origin value which text element is to be placed with left-bottom (default) justification 
  //  without taking into account other text properties as height, width, direction, font and etc...
  virtual OdGePoint2d getOrigin    () const;
  virtual void        setOrigin    ( const OdGePoint2d& point );

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  //overrides from OdDgElement
  OdResult subOpen( OdDg::OpenMode mode );
  void copyFrom(const OdRxObject* pSource);

  virtual OdRxObjectPtr clone() const;
  
  //
  // The processing of the Text Extended Properties
  //
 
#undef TEXT_FLAGS_DEF
#define TEXT_FLAGS_DEF(name)              \
  virtual bool get##name##Flag() const;   \
  virtual void set##name##Flag(bool val);

  TEXT_FLAGS_DEF( InterCharSpacing )
  TEXT_FLAGS_DEF( FixedWidthSpacing )
  TEXT_FLAGS_DEF( Underline )
  TEXT_FLAGS_DEF( Slant )
  TEXT_FLAGS_DEF( Vertical )
  TEXT_FLAGS_DEF( RightToLeft )
  TEXT_FLAGS_DEF( ReverseMline )
  TEXT_FLAGS_DEF( Kerning )
  TEXT_FLAGS_DEF( Offset )
  TEXT_FLAGS_DEF( Codepage )
  TEXT_FLAGS_DEF( ShxBigFont )
  TEXT_FLAGS_DEF( BgColor )
  TEXT_FLAGS_DEF( Subscript )
  TEXT_FLAGS_DEF( Superscript )
  TEXT_FLAGS_DEF( ExText )
  TEXT_FLAGS_DEF( TextStyle )
  TEXT_FLAGS_DEF( Overline )
  TEXT_FLAGS_DEF( Bold )
  TEXT_FLAGS_DEF( UnderlineStyle )
  TEXT_FLAGS_DEF( OverlineStyle )
  TEXT_FLAGS_DEF( StyleOverrides )
  TEXT_FLAGS_DEF( WordWrapTextNode )
  TEXT_FLAGS_DEF( FullJustification )
  TEXT_FLAGS_DEF( Color )
  TEXT_FLAGS_DEF( AcadInterCharSpacing )
  TEXT_FLAGS_DEF( Backwards )
  TEXT_FLAGS_DEF( Upsidedown )
  TEXT_FLAGS_DEF( AcadFittedText )

  virtual OdDgTextExtendedProperties::LineSpacingType getLineSpacingType() const;
  virtual void setLineSpacingType( OdDgTextExtendedProperties::LineSpacingType uType );

  virtual OdUInt8     getStackedFractionAlign() const;
  virtual void        setStackedFractionAlign( OdUInt8 var ); 
  virtual OdUInt8     getStackedFractionSection() const;
  virtual void        setStackedFractionSection( OdUInt8 var ); 
  virtual OdUInt8     getStackedFractionType() const;
  virtual void        setStackedFractionType( OdUInt8 var ); 

  virtual double      getSlant() const;
  virtual void        setSlant( double var ); 

  virtual double      getUnderlineSpacing() const;
  virtual void        setUnderlineSpacing( double var ); 

  virtual OdGePoint2d getLineOffset() const;
  virtual void        setLineOffset( const OdGePoint2d& var ); 

  virtual OdUInt32    getCodePage() const;
  virtual void        setCodePage( OdUInt32 var ); 

  virtual OdUInt32    getShxBigFontEntryId() const;
  virtual void        setShxBigFontEntryId( OdUInt32 var ); 

  virtual OdUInt32    getBackgroundLineStyleEntryId() const;
  virtual OdUInt32    getBackgroundLineWeight() const;
  virtual OdUInt32    getBackgroundColorIndex() const;
  virtual OdUInt32    getBackgroundFillColorIndex() const;
  virtual OdGePoint2d getBackgroundBorder() const;

  virtual double      getOverlineSpacing() const;
  virtual void        setOverlineSpacing( double var ); 

  virtual OdUInt32    getTextStyleEntryId() const;
  virtual void        setTextStyleEntryId( OdUInt32 var ); 

  virtual OdUInt32    getUnderlineColorIndex() const;
  virtual OdUInt32    getUnderlineLineStyleEntryId() const;
  virtual OdUInt32    getUnderlineLineWeight() const;

  virtual OdUInt32    getOverlineColorIndex() const;
  virtual OdUInt32    getOverlineLineStyleEntryId() const;
  virtual OdUInt32    getOverlineLineWeight() const;

  virtual double      getTextnodeWordWrapLength() const;
  virtual void        setTextnodeWordWrapLength( double var ); 

  // the number of CR which should be added after the text element
  virtual OdUInt8     getCrCount() const;
  virtual void        setCrCount( OdUInt8 var ); 

  virtual OdUInt32    getColorIndex() const;
  virtual void        setColorIndex( OdUInt32 color );

//
// OverrideFlags
//
//  OverridesFromStyle is a series of bits, which describe the differences 
//  between this text element and the settings in the textstyle associated 
//  with this text element. 

//
#undef TEXTSTYLE_OVERRIDE_FLAGS_DEF
#define TEXTSTYLE_OVERRIDE_FLAGS_DEF(name)        \
  virtual bool get##name##OverrideFlag() const;   \
  virtual void set##name##OverrideFlag(bool val);

  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FontNumber         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(ShxBigFont         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Width              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Height             )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Slant              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineSpacing        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineCharSpacing    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineOffset    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineOffset     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Justification      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(NodeJustification  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineLength         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Direction          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Underline          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Overline           )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Italics            )
                      
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Bold               )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Superscript        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Subscript          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FixedSpacing       )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Background         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundStyle    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundWeight   )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundColor    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundFillColor)
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundBorder   )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineStyle     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineWeight    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineColor     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineStyle      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineWeight     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineColor      )

  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineOffset         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Fractions          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineStyleFlag  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineStyleFlag )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Color              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(WidthFactor        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(ColorFlag          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FullJustification  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineSpacingType    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Backwards          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Upsidedown         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(AcadInterCharSpacing)

  // for DG 2.6.3
/*
  enum Spacing
  {
      sNone                   = 0
    , sInterCharSpacing       = 1
    , sFixedWidthSpacing      = 2
    , sAcadInterCharSpacing   = 3
  };
*/
  virtual void setOverlineStyle(OdUInt32 dOverlineColorIndex, OdUInt32 dOverlineLineStyleEntryId, OdUInt32 dOverlineLineWeight);
  virtual void setUnderlineStyle(OdUInt32 dUnderlineColorIndex, OdUInt32 dUnderlineLineStyleEntryId, OdUInt32 dUnderlineLineWeight);
  virtual void setBackground( OdUInt32 backgroundStyle,
                              OdUInt32 backgroundWeight,
                              OdUInt32 backgroundColor,
                              OdUInt32 backgroundFillColor,
                              OdGePoint2d const& backgroundBorder);

  virtual void getSpacing( double& var, Spacing& flag ) const;
  virtual void setSpacing( double var, Spacing flag );

  // Applies TextStyle settings to Text element. 
  //  Attention: it should be called only after adding element to model
  virtual bool applyTextStyle( OdDgElementId idTextStyle );
  virtual void subSetDatabaseDefaults( OdDgDatabase* pDb, bool doSubents);


  //virtual OdResult subClose();

  // get/set text position as a result of calculation text properties as 
  //  origin, actual justification, height, width, direction, font and etc...
  //  Call it when an element has been already added to database. Only in this case it is possible 
  //  to get complete data (for instance, text font data) to calculate right geometry extents. 
  virtual OdGePoint2d getPosition() const;
  virtual void        setPosition( const OdGePoint2d& point );

  // Text can be 'italics' even if it has 0 slant (for instance, TrueType font). 
  //  So this method sets 'italics' flag in case of true;
  //  and resets 'italics' flag and 'slant' in case of false.
  virtual void        setItalics( bool bItalics );
};

typedef OdSmartPtr< OdDgText2d > OdDgText2dPtr;


//3D subtype of text

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgText3d : public OdDgGraphicsElement
                           , public OdDgTextExtended
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgText3d );
public:
  
  //the text
  virtual OdString getText() const;
  virtual void setText( const OdString& text );

  // MKU 06/19/07
  virtual OdUInt32  getFontEntryId () const;
  virtual void      setFontEntryId   ( OdUInt32 fontId );

  //
  //attributes of the text
  //

  virtual OdDg::TextJustification   getJustification() const;
  virtual void                      setJustification( OdDg::TextJustification justification );

  // The “length” and “height” members of Text elements are not used by MicroStation v8.
  // They could be set to display text elements a distance from the origin for the x and y  of the text-aligned box
  virtual double  getLength () const;
  virtual void    setLength ( double length );

  virtual double  getHeight () const;
  virtual void    setHeight ( double height );

  // The Length and Height multipliers are used to display Text element.
  virtual double  getLengthMultiplier () const;
  virtual void    setLengthMultiplier ( double lengthMult );

  virtual double  getHeightMultiplier () const;
  virtual void    setHeightMultiplier ( double heightMult );

  virtual OdInt16 getTextEditFieldCount() const;
  virtual OdDgTextEditField getTextEditFieldAt( OdInt16 index ) const;
  virtual void setTextEditFieldAt( OdInt16 index, const OdDgTextEditField& field );
  virtual void addTextEditField(  const OdDgTextEditField& field );
  virtual void removeTextEditFieldAt( OdInt16 index );

  //3d-specific methods
  virtual OdGeQuaternion getRotation() const;
  virtual void setRotation  ( const OdGeQuaternion& rotation );

  // get/set origin value which text element is to be placed with left-bottom (default) justification 
  //  without taking into account other text properties as height, width, direction, font and etc...
  virtual OdGePoint3d getOrigin    () const;
  virtual void        setOrigin    ( const OdGePoint3d& point );

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  virtual OdRxObjectPtr clone() const;

  //overrides from OdDgElement
  OdResult subOpen( OdDg::OpenMode mode );
  void copyFrom(const OdRxObject* pSource);

  //
  // The processing of the Text Extended Properties
  //
 
#undef TEXT_FLAGS_DEF
#define TEXT_FLAGS_DEF(name)              \
  virtual bool get##name##Flag() const;   \
  virtual void set##name##Flag(bool val);

  TEXT_FLAGS_DEF( InterCharSpacing )
  TEXT_FLAGS_DEF( FixedWidthSpacing )
  TEXT_FLAGS_DEF( Underline )
  TEXT_FLAGS_DEF( Slant )
  TEXT_FLAGS_DEF( Vertical )
  TEXT_FLAGS_DEF( RightToLeft )
  TEXT_FLAGS_DEF( ReverseMline )
  TEXT_FLAGS_DEF( Kerning )
  TEXT_FLAGS_DEF( Offset )
  TEXT_FLAGS_DEF( Codepage )
  TEXT_FLAGS_DEF( ShxBigFont )
  TEXT_FLAGS_DEF( BgColor )
  TEXT_FLAGS_DEF( Subscript )
  TEXT_FLAGS_DEF( Superscript )
  TEXT_FLAGS_DEF( ExText )
  TEXT_FLAGS_DEF( TextStyle )
  TEXT_FLAGS_DEF( Overline )
  TEXT_FLAGS_DEF( Bold )
  TEXT_FLAGS_DEF( UnderlineStyle )
  TEXT_FLAGS_DEF( OverlineStyle )
  TEXT_FLAGS_DEF( StyleOverrides )
  TEXT_FLAGS_DEF( WordWrapTextNode )
  TEXT_FLAGS_DEF( FullJustification )
  TEXT_FLAGS_DEF( Color )
  TEXT_FLAGS_DEF( AcadInterCharSpacing )
  TEXT_FLAGS_DEF( Backwards )
  TEXT_FLAGS_DEF( Upsidedown )
  TEXT_FLAGS_DEF( AcadFittedText )

  virtual OdDgTextExtendedProperties::LineSpacingType getLineSpacingType() const;
  virtual void setLineSpacingType( OdDgTextExtendedProperties::LineSpacingType uType );

  virtual OdUInt8     getStackedFractionAlign() const;
  virtual void        setStackedFractionAlign( OdUInt8 var ); 
  virtual OdUInt8     getStackedFractionSection() const;
  virtual void        setStackedFractionSection( OdUInt8 var ); 
  virtual OdUInt8     getStackedFractionType() const;
  virtual void        setStackedFractionType( OdUInt8 var ); 

  virtual double      getSlant() const;
  virtual void        setSlant( double var ); 

  virtual double      getUnderlineSpacing() const;
  virtual void        setUnderlineSpacing( double var ); 

  virtual OdGePoint2d getLineOffset() const;
  virtual void        setLineOffset( const OdGePoint2d& var ); 

  virtual OdUInt32    getCodePage() const;
  virtual void        setCodePage( OdUInt32 var ); 

  virtual OdUInt32    getShxBigFontEntryId() const;
  virtual void        setShxBigFontEntryId( OdUInt32 var ); 

  virtual OdUInt32    getBackgroundLineStyleEntryId() const;
  virtual OdUInt32    getBackgroundLineWeight() const;
  virtual OdUInt32    getBackgroundColorIndex() const;
  virtual OdUInt32    getBackgroundFillColorIndex() const;
  virtual OdGePoint2d getBackgroundBorder() const;

  virtual double      getOverlineSpacing() const;
  virtual void        setOverlineSpacing( double var ); 

  virtual OdUInt32    getTextStyleEntryId() const;
  virtual void        setTextStyleEntryId( OdUInt32 var ); 

  virtual OdUInt32    getUnderlineColorIndex() const;
  virtual OdUInt32    getUnderlineLineStyleEntryId() const;
  virtual OdUInt32    getUnderlineLineWeight() const;

  virtual OdUInt32    getOverlineColorIndex() const;
  virtual OdUInt32    getOverlineLineStyleEntryId() const;
  virtual OdUInt32    getOverlineLineWeight() const;

  virtual double      getTextnodeWordWrapLength() const;
  virtual void        setTextnodeWordWrapLength( double var ); 

  // the number of CR which should be added after the text element
  virtual OdUInt8     getCrCount() const;
  virtual void        setCrCount( OdUInt8 var ); 

  virtual OdUInt32    getColorIndex() const;
  virtual void        setColorIndex( OdUInt32 color );

//
// OverrideFlags
//
//  OverridesFromStyle is a series of bits, which describe the differences 
//  between this text element and the settings in the textstyle associated 
//  with this text element. 

//
#undef TEXTSTYLE_OVERRIDE_FLAGS_DEF
#define TEXTSTYLE_OVERRIDE_FLAGS_DEF(name)        \
  virtual bool get##name##OverrideFlag() const;   \
  virtual void set##name##OverrideFlag(bool val);

  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FontNumber         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(ShxBigFont         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Width              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Height             )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Slant              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineSpacing        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineCharSpacing    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineOffset    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineOffset     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Justification      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(NodeJustification  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineLength         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Direction          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Underline          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Overline           )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Italics            )
                      
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Bold               )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Superscript        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Subscript          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FixedSpacing       )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Background         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundStyle    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundWeight   )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundColor    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundFillColor)
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundBorder   )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineStyle     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineWeight    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineColor     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineStyle      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineWeight     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineColor      )

  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineOffset         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Fractions          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineStyleFlag  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineStyleFlag )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Color              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(WidthFactor        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(ColorFlag          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FullJustification  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineSpacingType    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Backwards          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Upsidedown         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(AcadInterCharSpacing)

  // for DG 2.6.3
/*
  enum Spacing
  {
      sNone                   = 0
    , sInterCharSpacing       = 1
    , sFixedWidthSpacing      = 2
    , sAcadInterCharSpacing   = 3
  };
*/
  virtual void setOverlineStyle(OdUInt32 dOverlineColorIndex, OdUInt32 dOverlineLineStyleEntryId, OdUInt32 dOverlineLineWeight);
  virtual void setUnderlineStyle(OdUInt32 dUnderlineColorIndex, OdUInt32 dUnderlineLineStyleEntryId, OdUInt32 dUnderlineLineWeight);
  virtual void setBackground( OdUInt32 backgroundStyle,
                              OdUInt32 backgroundWeight,
                              OdUInt32 backgroundColor,
                              OdUInt32 backgroundFillColor,
                              OdGePoint2d const& backgroundBorder);

  virtual void getSpacing( double& var, Spacing& flag ) const;
  virtual void setSpacing( double var, Spacing flag );

  // Applies TextStyle settings to Text element. 
  //  Attention: it should be called only after adding element to model
  virtual bool applyTextStyle( OdDgElementId idTextStyle );
  virtual void subSetDatabaseDefaults( OdDgDatabase* pDb, bool doSubents);

  //virtual OdResult subClose();

  // get/set text position as a result of calculation text properties as 
  //  origin, actual justification, height, width, direction, font and etc...
  //  Call it when an element has been already added to database. Only in this case it is possible 
  //  to get complete data (for instance, text font data) to calculate right geometry extents. 
  virtual OdGePoint3d getPosition() const;
  virtual void        setPosition( const OdGePoint3d& point );

  // Text can be 'italics' even if it has 0 slant (for instance, TrueType font). 
  //  So this method sets 'italics' flag in case of true;
  //  and resets 'italics' flag and 'slant' in case of false.
  virtual void        setItalics( bool bItalics );
};

typedef OdSmartPtr< OdDgText3d > OdDgText3dPtr;


//// temporary: 
////
//// OdRscFont is a class for fonts from MicroStation Recource files.
////
//#include "OdFont.h"
//#include "UInt16Array.h"
//
//#include "OdPlatformStreamer.h"
//
//// Resource file platform ID's
//enum OdRscFontFilePlatforms
//{
//  rscFont_PC          = 1,
//  rscFont_CLIPPER     = 2,
//  rscFont_MACOS       = 3,
//  rscFont_SPARC       = 4,
//  rscFont_VMS         = 5,
//  rscFont_HP700       = 6,
//  rscFont_SGI         = 7,
//  rscFont_MIPS        = 8,
//  rscFont_RS6000      = 9,
//  rscFont_PC_WINNT    = 10,
//  rscFont_ALPHA_WINNT = 11,
//  rscFont_CLIPPER_EV  = 20,
//  rscFont_CLIPPER_X   = 21
//};
//
//struct OdRscFontHeader
//{
//   OdChar     m_unknown_1[32];
//   OdUInt16   m_nChars;
//   OdUInt16   m_nFirstChar;
//   OdUInt16   m_nLastChar;
//   OdUInt16   m_nDefaultChar;
//   OdChar     m_unknown_2[8];
//   OdUInt16   m_nMaxWidth;
//   OdUInt16   m_nMaxHeight;
//   OdUInt16   m_nWidth;
//   OdUInt16   m_nHeight;
//   OdUInt16   m_nWord2;
//   OdUInt16   m_nWord3;
//   OdUInt16   m_nDefaultWidth;
//   OdChar     m_unknown_3[10];
//   OdUInt16   m_nWord4;
//   OdChar     m_unknown_4[16];
//   OdUInt16   m_nLenFontName1;
//   OdUInt16   m_nLenFontName2;
//   OdUInt16   m_nLenFontName3;
//};
//
//struct OdRscFontCharacter
//{
//   OdUInt16   m_nChar;
//   OdUInt16   m_nWidth;
//   OdUInt16   m_nUnknown;
//   OdUInt16   m_nHeight;
//   OdChar     m_Fill[12];
//};
//
//struct OdRscFontVectorOffset
//{
//	OdUInt32  m_Offset;   // offset to vector commands
//	OdUInt32	m_Size;     // size of vector commands in bytes
//  OdUInt16Array m_arrayVectorData;
//};
//
//struct OdRscFontDescriptor // each font's information
//{
//	OdUInt32			      m_idFont;			    // font id number
//	OdUInt32			      m_idResource;	    // font resource id
//	OdString            m_sName;	        // font name
//	OdRscFontHeader     m_Header;         // font header
//	OdArray<OdRscFontCharacter>     m_CharacterInfo;  // character info
//	OdArray<OdRscFontVectorOffset>  m_VectorOffset;	  // vectors offset array
//	//char*		pVectors;	// vectors base pointer
//};
//
//
//class OdRscFont;
//typedef OdSmartPtr<OdRscFont> OdRscFontPtr;
//
//class /* DBROOT_EXPORT */ TG_EXPORT OdRscFont : public OdFont
//{
//protected:
//	double  m_Above;
//	double  m_Below;
//
//  OdRscFontDescriptor m_desc;
//
//public:
//  ODRX_DECLARE_MEMBERS(OdRscFont);
//
//	OdRscFont();
//	//virtual ~OdRscFont();
//
//	static OdRscFontPtr createFont(OdRscFontDescriptor& desc);
//
//  // OdFont overwritings
//  //
//
//  OdResult initialize(OdStreamBuf* pStreamBuf);
//  OdResult drawCharacter(OdChar character, OdGePoint2d& advance, OdGiCommonDraw* pWd, OdTextProperties& textProperties);
//  OdResult drawCharacter(OdChar character, OdGePoint2d& advance, OdGiConveyorGeometry* pGeometry, OdTextProperties& textProperties);
//
//  double  getAbove() const;
//  double  getBelow() const;
//  OdUInt32 getAvailableChars(OdArray<OdChar>& characters);
//  bool    hasCharacter(OdChar character);
//
//  // OdRscFont
//  //
//  virtual OdResult  initialize(OdRscFontDescriptor& desc);
//
//};

#include "TD_PackPop.h"

#endif // __DG_TEXT_H__
