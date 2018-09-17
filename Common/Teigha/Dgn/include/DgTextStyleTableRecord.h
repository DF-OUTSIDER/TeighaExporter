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
#ifndef __DG_TEXTSTYLETABLERECORD_H__
#define __DG_TEXTSTYLETABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"
#include "Ge/GePoint2d.h"
#include "DgTextExtendedProperties.h"

class OdGiTextStyle;


/** \details
    This class represents TextStyle records in the OdDgTextStyleTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTextStyleTableRecord : public OdDgTableRecord,
                                           public OdDgTextStyleOverrideFlags
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgTextStyleTableRecord);
public:

  OdUInt32 getEntryId() const;

  virtual OdDg::TextJustification getJustification() const;
  virtual void setJustification( OdDg::TextJustification );

  virtual OdUInt32 getLineLength() const;
  virtual void setLineLength( OdUInt32 );

  virtual OdUInt32 getFontEntryId() const;
  virtual void setFontEntryId( OdUInt32 );

  virtual OdUInt32 getParentTextStyleEntryId() const;
  virtual void setParentTextStyleEntryId( OdUInt32 );

  virtual double getSlant() const;
  virtual void setSlant( double );

  virtual OdDg::TextDirection getTextDirection() const;
  virtual void setTextDirection( OdDg::TextDirection );

  virtual double getTextHeight() const;
  virtual void setTextHeight( double );

  virtual OdDg::TextJustification getTextNodeJustification() const;
  virtual void setTextNodeJustification( OdDg::TextJustification );

  virtual double getTextWidth() const;
  virtual void setTextWidth( double );

  virtual OdUInt32 getColorIndex() const;
  virtual void setColorIndex( OdUInt32 var ); 

  // Spacing properties
  virtual double getLineSpacing() const;
  virtual void setLineSpacing( double );

  virtual OdDgTextExtendedProperties::LineSpacingType getLineSpacingType() const;
  virtual void setLineSpacingType( OdDgTextExtendedProperties::LineSpacingType type );

  virtual OdGePoint2d getLineOffset() const;
  virtual void setLineOffset( const OdGePoint2d& );

  virtual double getInterCharacter() const;
  virtual void setInterCharacter( double );

  // text style flags
  virtual bool getUnderlineFlag() const;
  virtual bool getOverlineFlag() const;
  virtual bool getItalicsFlag() const;
  virtual bool getBoldFlag() const;
  virtual bool getSuperscriptFlag() const;
  virtual bool getSubscriptFlag() const;
  virtual bool getBackgroundFlag() const;
  virtual bool getOverlineStyleFlag() const;
  virtual bool getUnderlineStyleFlag() const;
  virtual bool getFixedSpacingFlag() const;
  virtual bool getFractionsFlag() const;
  virtual bool getColorFlag() const;
  virtual bool getAcadIntercharSpacingFlag() const;
  virtual bool getFullJustificationFlag() const;
  virtual bool getAcadShapeFileFlag() const;

  virtual void setUnderlineFlag( bool flag );
  virtual void setOverlineFlag( bool flag );
  virtual void setItalicsFlag( bool flag );
  virtual void setBoldFlag( bool flag );
  virtual void setSuperscriptFlag( bool flag );
  virtual void setSubscriptFlag( bool flag );
  virtual void setBackgroundFlag( bool flag );
  virtual void setOverlineStyleFlag( bool flag );
  virtual void setUnderlineStyleFlag( bool flag );
  virtual void setFixedSpacingFlag( bool flag );
  virtual void setFractionsFlag( bool flag );
  virtual void setColorFlag( bool flag );
  virtual void setAcadIntercharSpacingFlag( bool flag );
  virtual void setFullJustificationFlag( bool flag );
  virtual void setAcadShapeFileFlag( bool flag );

  // background properties
  virtual OdUInt32 getBackgroundLineStyleEntryId() const;
  virtual OdUInt32 getBackgroundLineWeight() const;
  virtual OdUInt32 getBackgroundColorIndex() const;
  virtual OdUInt32 getBackgroundFillColorIndex() const;
  virtual OdGePoint2d getBackgroundBorder() const;

  virtual void setBackgroundLineStyleEntryId( OdUInt32 );
  virtual void setBackgroundLineWeight( OdUInt32 );
  virtual void setBackgroundColorIndex( OdUInt32 );
  virtual void setBackgroundFillColorIndex( OdUInt32 );
  virtual void setBackgroundBorder( const OdGePoint2d& );

  // overline properties
  virtual OdUInt32 getOverlineLineStyleEntryId() const;
  virtual OdUInt32 getOverlineLineWeight() const;
  virtual OdUInt32 getOverlineColorIndex() const;
  virtual double getOverlineOffset() const;

  virtual void setOverlineLineStyleEntryId( OdUInt32 );
  virtual void setOverlineLineWeight( OdUInt32 );
  virtual void setOverlineColorIndex( OdUInt32 );
  virtual void setOverlineOffset( double );

  // underline properties
  virtual OdUInt32 getUnderlineLineStyleEntryId() const;
  virtual OdUInt32 getUnderlineLineWeight() const;
  virtual OdUInt32 getUnderlineColorIndex() const;
  virtual double getUnderlineOffset() const;

  virtual void setUnderlineLineStyleEntryId( OdUInt32 );
  virtual void setUnderlineLineWeight( OdUInt32 );
  virtual void setUnderlineColorIndex( OdUInt32 );
  virtual void setUnderlineOffset( double );

  //
  //  Compiles TextStyle properties to OdGiTextStyle form taking into account
  //  overrides & parent text style hierarchy.
  //
  //    The parentId value if non-0 is the id of a parent text style which this style is 
  //     hierarchically linked to.
  //
  //    The settings of the individual bits in the overrideFlags set of flags are used to 
  //     determine which settings are used from this style when the textstyles are arranged 
  //     hierarchically. If the bit is on it means that the setting in this text style should 
  //     be used in preference to the equivalent value in the parent text style.

  virtual void getGiTextStyle(OdGiTextStyle& giStyle) const;

  //
  // OverrideFlags
  //
  //  If the textstyles are arranged hierarchically this set of flags are used to 
  //  determine which settings are gotten from this style.
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

  // DG 2.6.3
  virtual OdUInt32 getShxBigFontId() const;
  virtual void setShxBigFontId( OdUInt32 );

  // OdGiDrawable overridden
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgTextStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgTextStyleTableRecord> OdDgTextStyleTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_TEXTSTYLETABLERECORD_H__
