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
#ifndef __DG_TEXT_EXTENDED_PROPERTIES_H__
#define __DG_TEXT_EXTENDED_PROPERTIES_H__

#include "TD_PackPush.h"

#include "Ge/GePoint2d.h"

namespace OdDg
{
  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum TextDirection
  {
    kHorizontal   = 0x0,
    kVertical     = 0x4,
    kJapanese     = 0x2, // vertical multiline right to left
    kRightToLeft  = 0x8,
    kUpsideDown   = 0x10,
    kBackwards    = 0x20
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum TextJustification
  {
    kLeftTop            = 0,
    kLeftCenter         = 1,
    kLeftBottom         = 2,
    kLeftMarginTop      = 3,
    kLeftMarginCenter   = 4,
    kLeftMarginBottom   = 5,
    kCenterTop          = 6,
    kCenterCenter       = 7,
    kCenterBottom       = 8,
    kRightMarginTop     = 9,
    kRightMarginCenter  = 10,
    kRightMarginBottom  = 11,
    kRightTop           = 12,
    kRightCenter        = 13,
    kRightBottom        = 14,
    kLeftDescender      = 16,
    kCenterDescender    = 20,
    kRightDescender     = 24
  };
}

// Text Extended Properties
//
//  More information about the text & text node element’s display properties.
//

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class OdDgTextExtendedProperties
{
public:
  OdDgTextExtendedProperties()
  {}
  virtual ~OdDgTextExtendedProperties()
  {}

  //
  // Text Extended Properties
  //
  //  More information about the text & text node element’s display properties.
  //  If the proper flag is on then the text element applies the value.
  //
  // Value                           // Flag(s) On:
  //
  // IntercharSpacing         InterCharSpacing || FixedWidthSpacing || AcadInterCharSpacing
  // Slant                    Italics
  // UnderlineSpacing          Underline
  // LineOffset               Offset
  // CodePage                 Codepage
  // ShxBigFont               ShxBigFont
  // BackgroundColor          BgColor
  // BackgroundStyle          BgColor
  // BackgroundWeight         BgColor
  // BackgroundBorder         BgColor
  // BackgroundFillColor      BgColor
  // OverlineSpacing          Overline    
  // TextStyleId              Textstyle
  // UnderlineColor           UnderlineStyle
  // UnderlineStyle           UnderlineStyle
  // UnderlineWeight          UnderlineStyle
  // OverlineColor            OverlineStyle
  // OverlineStyle            OverlineStyle
  // OverlineWeight           OverlineStyle
  // TextnodeWordWrapLength   WordWrapTextNode
  // OverridesFromStyle       StyleOverrides
  // Color                    Color
 
#undef TEXT_FLAGS_DEF
#define TEXT_FLAGS_DEF(name)                  \
  virtual bool get##name##Flag() const  = 0;  \
  virtual void set##name##Flag(bool val)  = 0;

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

  enum Spacing
  {
      sNone                   = 0
    , sInterCharSpacing       = 1
    , sFixedWidthSpacing      = 2
    , sAcadInterCharSpacing   = 3
  };

  enum LineSpacingType
  {
    kExact      = 0 
    , kAutomatic  = 1
    , kFromLineTop= 2
    , kAtLeast    = 3
  };

  virtual LineSpacingType getLineSpacingType() const = 0;
  virtual void setLineSpacingType( LineSpacingType uType ) = 0;

  virtual void getSpacing( double& var, Spacing& flag ) const = 0;
  virtual void setSpacing( double var, Spacing flag ) = 0;

  virtual double getSlant() const = 0;
  virtual void setSlant( double var ) = 0; 

  virtual double getUnderlineSpacing() const = 0;
  virtual void setUnderlineSpacing( double var ) = 0; 

  virtual OdGePoint2d getLineOffset() const = 0;
  virtual void setLineOffset( const OdGePoint2d& var ) = 0; 

  virtual OdUInt32 getCodePage() const = 0;
  virtual void setCodePage( OdUInt32 var ) = 0; 

  virtual OdUInt32 getShxBigFontEntryId() const = 0;
  virtual void setShxBigFontEntryId( OdUInt32 var ) = 0; 

  virtual OdUInt32 getBackgroundLineStyleEntryId() const = 0;
  virtual OdUInt32 getBackgroundLineWeight() const = 0;
  virtual OdUInt32 getBackgroundColorIndex() const = 0;
  virtual OdUInt32 getBackgroundFillColorIndex() const = 0;
  virtual OdGePoint2d getBackgroundBorder() const = 0;

  virtual double getOverlineSpacing() const = 0;
  virtual void setOverlineSpacing( double var ) = 0; 

  virtual OdUInt32 getTextStyleEntryId() const = 0;
  virtual void setTextStyleEntryId( OdUInt32 var ) = 0; 

  virtual OdUInt32 getUnderlineColorIndex() const = 0;
  virtual OdUInt32 getUnderlineLineStyleEntryId() const = 0;
  virtual OdUInt32 getUnderlineLineWeight() const = 0;

  virtual OdUInt32 getOverlineColorIndex() const = 0;
  virtual OdUInt32 getOverlineLineStyleEntryId() const = 0;
  virtual OdUInt32 getOverlineLineWeight() const = 0;

  virtual double getTextnodeWordWrapLength() const = 0;
  virtual void setTextnodeWordWrapLength( double var ) = 0; 

  virtual void setOverlineStyle(OdUInt32 dOverlineColorIndex, OdUInt32 dOverlineLineStyleEntryId, OdUInt32 dOverlineLineWeight) = 0;
  virtual void setUnderlineStyle(OdUInt32 dUnderlineColorIndex, OdUInt32 dUnderlineLineStyleEntryId, OdUInt32 dUnderlineLineWeight) = 0;
  virtual void setBackground( OdUInt32 backgroundStyle,
                              OdUInt32 backgroundWeight,
                              OdUInt32 backgroundColor,
                              OdUInt32 backgroundFillColor,
                              OdGePoint2d const& backgroundBorder) = 0;


  // the number of CR which should be added after the text element
  virtual OdUInt8 getCrCount() const = 0;
  virtual void setCrCount( OdUInt8 var ) = 0;
  
  virtual OdUInt32 getColorIndex() const = 0;
  virtual void setColorIndex( OdUInt32 color ) = 0;

  //// Add here for DG 2.7
  //virtual void setOverlineStyle(OdUInt32 dOverlineColorIndex, OdUInt32 dOverlineLineStyleEntryId, OdUInt32 dOverlineLineWeight) = 0;
  //virtual void setUnderlineStyle(OdUInt32 dUnderlineColorIndex, OdUInt32 dUnderlineLineStyleEntryId, OdUInt32 dUnderlineLineWeight) = 0;
  //virtual void setBackground( OdUInt32 backgroundStyle,
  //                            OdUInt32 backgroundWeight,
  //                            OdUInt32 backgroundColor,
  //                            OdUInt32 backgroundFillColor,
  //                            OdGePoint2d const& backgroundBorder) = 0;
  //
  //enum Spacing
  //{
  //    sNone                   = 0
  //  , sInterCharSpacing       = 1
  //  , sFixedWidthSpacing      = 2
  //  , sAcadInterCharSpacing   = 3
  //};
  //
  //virtual void getSpacing( double& var, Spacing& flag ) const = 0;
  //virtual void setSpacing( double var, Spacing flag ) = 0;
  //
  //// Applies TextStyle settings to Text element. 
  ////  Attention: it should be called only after adding element to model
  //virtual bool applyTextStyle( OdDgElementId idTextStyle );
};

//
// OverrideFlags
//
//  If the textstyles are arranged hierarchically this set of flags are used to 
//  determine which settings are gotten from this style.
//

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class OdDgTextStyleOverrideFlags
{
public:
  OdDgTextStyleOverrideFlags()
  {}
  virtual ~OdDgTextStyleOverrideFlags()
  {}

#undef TEXTSTYLE_OVERRIDE_FLAGS_DEF
#define TEXTSTYLE_OVERRIDE_FLAGS_DEF(name)          \
  virtual bool get##name##OverrideFlag() const = 0; \
  virtual void set##name##OverrideFlag(bool val) = 0;

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

};

#include "TD_PackPop.h"

#endif // __DG_TEXT_EXTENDED_PROPERTIES_H__
