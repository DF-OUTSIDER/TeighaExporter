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

#ifndef __DG_TEXT_NODE_H__
#define __DG_TEXT_NODE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgElementIterator.h"
#include "OdString.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"
#include "DgTextExtendedProperties.h"


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTextNode2d : public OdDgGraphicsElement
                               , public OdDgTextExtendedProperties
                               , public OdDgTextStyleOverrideFlags
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTextNode2d );
public:

  double getLineSpacing () const;
  void setLineSpacing ( double spacing );

  OdUInt32 getFontEntryId () const;
  void setFontEntryId ( OdUInt32 fontId );

  OdUInt16 getMaxLength () const;
  void setMaxLength ( OdUInt16 length );

  OdDg::TextJustification getJustification() const;
  void setJustification( OdDg::TextJustification justification );

  OdUInt32 getTextNodeNumber() const;
  void setTextNodeNumber( OdUInt32 number );

  //double getCharWidth () const;
  //void setCharWidth ( double width );

  //double getCharHeight () const;
  //void setCharHeight ( double height );

  //attributes
  //note: the attributes can do not exist
  //void getTextAttributes( TextAttributes& attributes ); 
  //void setTextAttributes( const TextAttributes& attributes );

  //support for the complexity of TextNode
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;

  virtual OdRxObjectPtr clone() const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  //
  //2d-specific methods
  //

  double getRotation () const;
  void setRotation ( double angle );

  OdGePoint2d getOrigin () const;
  void setOrigin ( const OdGePoint2d& point );

  //
  // OdDgTextExtendedProperties overridden
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

  virtual double getSlant() const;
  virtual void setSlant( double var ); 

  virtual double getUnderlineSpacing() const;
  virtual void setUnderlineSpacing( double var ); 

  virtual OdGePoint2d getLineOffset() const;
  virtual void setLineOffset( const OdGePoint2d& var ); 

  virtual OdUInt32 getCodePage() const;
  virtual void setCodePage( OdUInt32 var ); 

  virtual OdUInt32 getShxBigFontEntryId() const;
  virtual void setShxBigFontEntryId( OdUInt32 var ); 

  virtual OdUInt32 getBackgroundLineStyleEntryId() const;
  virtual OdUInt32 getBackgroundLineWeight() const;
  virtual OdUInt32 getBackgroundColorIndex() const;
  virtual OdUInt32 getBackgroundFillColorIndex() const;
  virtual OdGePoint2d getBackgroundBorder() const;

  virtual double getOverlineSpacing() const;
  virtual void setOverlineSpacing( double var ); 

  virtual OdUInt32 getTextStyleEntryId() const;
  virtual void setTextStyleEntryId( OdUInt32 var ); 

  virtual OdUInt32 getUnderlineColorIndex() const;
  virtual OdUInt32 getUnderlineLineStyleEntryId() const;
  virtual OdUInt32 getUnderlineLineWeight() const;

  virtual OdUInt32 getOverlineColorIndex() const;
  virtual OdUInt32 getOverlineLineStyleEntryId() const;
  virtual OdUInt32 getOverlineLineWeight() const;

  virtual double getTextnodeWordWrapLength() const;
  virtual void setTextnodeWordWrapLength( double var ); 

  virtual OdUInt8 getCrCount() const;
  virtual void setCrCount( OdUInt8 var ); 

  virtual OdUInt32 getColorIndex() const;
  virtual void setColorIndex( OdUInt32 var ); 

  //
  // OdDgTextStyleOverrideFlags overridden
  //
  // OverridesFromStyle is a series of bits, which describe the differences 
  // between this text element and the settings in the textstyle associated 
  // with this text element. 

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
  // Font, HeightMultiplier & LenghtMultiplier are used to display the node number
  virtual double getLengthMultiplier () const;
  virtual void setLengthMultiplier ( double length );

  virtual double getHeightMultiplier () const;
  virtual void setHeightMultiplier ( double height );

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
};

typedef OdSmartPtr< OdDgTextNode2d > OdDgTextNode2dPtr;

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTextNode3d : public OdDgGraphicsElement
                               , public OdDgTextExtendedProperties
                               , public OdDgTextStyleOverrideFlags
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTextNode3d );
public:

  double getLineSpacing () const;
  void setLineSpacing ( double spacing );

  OdUInt32 getFontEntryId () const;
  void setFontEntryId ( OdUInt32 fontId );

  OdUInt16 getMaxLength () const;
  void setMaxLength ( OdUInt16 length );

  OdDg::TextJustification getJustification() const;
  void setJustification( OdDg::TextJustification justification );

  OdUInt32 getTextNodeNumber() const;
  void setTextNodeNumber( OdUInt32 number );

  //double getCharWidth () const;
  //void setCharWidth ( double width );

  //double getCharHeight () const;
  //void setCharHeight ( double height );

  //attributes
  //note: the attributes can do not exist
  //void getTextAttributes( TextAttributes& attributes ); //the attributes can do not exist
  //void setTextAttributes( const TextAttributes& attributes );

  //support for the complexity of TextNode
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;

  virtual OdRxObjectPtr clone() const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  //
  //3d-specific methods
  //

  OdGeQuaternion getRotation() const;
  void setRotation( const OdGeQuaternion& rotation );

  OdGePoint3d getOrigin() const;
  void setOrigin( const OdGePoint3d& point );

  //
  // OdDgTextExtendedProperties overridden
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

  virtual double getSlant() const;
  virtual void setSlant( double var ); 

  virtual double getUnderlineSpacing() const;
  virtual void setUnderlineSpacing( double var ); 

  virtual OdGePoint2d getLineOffset() const;
  virtual void setLineOffset( const OdGePoint2d& var ); 

  virtual OdUInt32 getCodePage() const;
  virtual void setCodePage( OdUInt32 var ); 

  virtual OdUInt32 getShxBigFontEntryId() const;
  virtual void setShxBigFontEntryId( OdUInt32 var ); 

  virtual OdUInt32 getBackgroundLineStyleEntryId() const;
  virtual OdUInt32 getBackgroundLineWeight() const;
  virtual OdUInt32 getBackgroundColorIndex() const;
  virtual OdUInt32 getBackgroundFillColorIndex() const;
  virtual OdGePoint2d getBackgroundBorder() const;

  virtual double getOverlineSpacing() const;
  virtual void setOverlineSpacing( double var ); 

  virtual OdUInt32 getTextStyleEntryId() const;
  virtual void setTextStyleEntryId( OdUInt32 var ); 

  virtual OdUInt32 getUnderlineColorIndex() const;
  virtual OdUInt32 getUnderlineLineStyleEntryId() const;
  virtual OdUInt32 getUnderlineLineWeight() const;

  virtual OdUInt32 getOverlineColorIndex() const;
  virtual OdUInt32 getOverlineLineStyleEntryId() const;
  virtual OdUInt32 getOverlineLineWeight() const;

  virtual double getTextnodeWordWrapLength() const;
  virtual void setTextnodeWordWrapLength( double var ); 

  // the number of CR which should be added after the text element
  virtual OdUInt8 getCrCount() const;
  virtual void setCrCount( OdUInt8 var ); 

  virtual OdUInt32 getColorIndex() const;
  virtual void setColorIndex( OdUInt32 var ); 

  //
  // OdDgTextStyleOverrideFlags overridden
  //
  // OverridesFromStyle is a series of bits, which describe the differences 
  // between this text element and the settings in the textstyle associated 
  // with this text element. 

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
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Color          )
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
  // Font, HeightMultiplier & LenghtMultiplier are used to display the node number
  virtual double getLengthMultiplier () const;
  virtual void setLengthMultiplier ( double length );

  virtual double getHeightMultiplier () const;
  virtual void setHeightMultiplier ( double height );

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
};

typedef OdSmartPtr< OdDgTextNode3d > OdDgTextNode3dPtr;


#include "TD_PackPop.h"

#endif // __DG_TEXT_NODE_H__
