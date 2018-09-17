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




#ifndef _ODDBTEXTSTYLETABLERECORD_INCLUDED
#define _ODDBTEXTSTYLETABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTableRecord.h"
#include "DwgDeclareMembers.h"

/** \details
  <group OdDb_Classes>

  This class implements the text style record object, which represents a text style in 
  the database. This class inherits the base functionality of named records. 

  \sa
  TD_Db
    
  \sa
  <link db_textstyle_sample_table.html, Example of Working with the Text Style Table Object>

  <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

  \sa
  <link db_textstyle.html, Working with Text Styles>

  OdDbTextStyleTable class
*/
class TOOLKIT_EXPORT OdDbTextStyleTableRecord : public OdDbSymbolTableRecord
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbTextStyleTableRecord);

  /** \details
    Builds an instance of the text style record object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_textstyle_add_name.html, Adding and Naming Text Styles>
  */
  OdDbTextStyleTableRecord();


  /** \details
    Determines whether the font file associated with this text style should be interpreted as  
    a set of shapes or letters (DXF 70, bit 0x01) and returns True if the font file is a set 
    of shapes or False if the font file is a set of letters. 

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::setIsShapeFile() method
  */
  bool isShapeFile() const;


  /** \details
    Sets the interpretation of the associated font file as a Boolean value (DXF 70, bit 0x01). 
    The initial value is False (Letters) by default.

    \param bShapeFile [in]  True - to determine the font file as a set of shapes, or
                   False - to determine the font file as a set of letters.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::isShapeFile() method
  */
  void setIsShapeFile( bool bShapeFile );


  /** \details
    Determines whether text is drawn vertically or horizontally from the start position 
    (DXF 70, bit 0x04) and returns True if text is vertical or False if text is horizontal.
    
    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setIsVertical() method
  */
  bool isVertical() const;


  /** \details
    Sets the orientation typeface as a Boolean value (DXF 70, bit 0x04). The initial value is False 
    (Horizontal) by default.

    \param bVertical [in]  True - to make the vertical text, or False - to make the horizontal text.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::isVertical() method
  */
  void setIsVertical( bool bVertical );


  /** \details
    Determines whether text is drawn backward or forward from the start position 
    (DXF 71, bit 0x02) and returns True if text is backward or False if text is forward.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setIsBackwards() method
  */
  bool isBackwards() const;


  /** \details
    Sets the direction typeface as a Boolean value (DXF 71, bit 0x02). The initial value is False 
    (Forward) by default.
    
    \param bBackwards [in]  True - to draw the text backward, or False - to draw the text forward.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::isBackwards() method
  */
  void setIsBackwards( bool bBackwards );


  /** \details
    Determines whether letters are placed up or down relative to the baseline when text is drawn 
    (DXF 71, bit 0x04) and returns True if letters are placed up or False if letters are placed 
    upside down.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setIsUpsideDown() method
  */
  bool isUpsideDown() const;


  /** \details
    Sets the letter side typeface as a Boolean value (DXF 71, bit 0x04). The initial value is False 
    (Upside) by default.
    
    \param bUpsideDown [in]  True - to draw the text downside, or False - to draw the text upside.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::isUpsideDown() method
  */
  void setIsUpsideDown( bool bUpsideDown );


  /** \details
    Returns the height scale factor of the text boundary (DXF 40). This factor influences on the width 
    and height together and is used for proportional scaling of letters, spaces, and the text boundary.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setTextSize() method
  */
  double textSize() const;


  /** \details
    Sets the text size which is the height scale factor of the text boundary (DXF 40). This factor 
    influences on the text width and text height together and is used for proportional scaling of 
    letters, spaces, and the text boundary. A value less than 1.0 decreases the text. A value 
    greater than 1.0 increases the text. The initial value is 0.0 by default.

    \param vSize [in]  Text size as a positive Double value.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::textSize() method
  */
  void setTextSize( double vSize );


  /** \details
    Returns the width scale factor of the text boundary (DXF 41). This factor influences only on the 
    width and is used for scaling of letters and spaces when the height is constant.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setXScale() method
  */
  double xScale() const;


  /** \details
    Sets the width scale factor of the text boundary (DXF 41). This factor influences only on the  
    text width and is used for scaling of letters and spaces when the height is constant. A value 
    less than 1.0 condenses the text. A value greater than 1.0 expandes the text. The initial value 
    is 1.0 by default.

    \param xFactor [in]  X-scale factor as a positive non-zero Double value.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::xScale() method
  */
  void setXScale( double xFactor );


  /** \details
    Returns the obliquing angle in range ±1.48335 radians (±85°) (DXF 50). This angle influences on
    the lean of letters. Negative angles will have Oda2PI added to them.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setObliquingAngle() method
  */
  double obliquingAngle() const;


  /** \details
    Sets the obliquing angle of letters in range ±1.48335 radians (±85°) (DXF 50). A positive angle 
    leans the letters clockwise (to the right). A negative value leans the letters counterclockwise 
    (to the left). A negative value is converted to its positive equivalent by adding the value 2*PI. 
    The initial value is 0.0 by default.
    
    \param vAngle [in]  Obliquing angle in radians as a Double value in range ±1.48335.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::obliquingAngle() method
  */
  void setObliquingAngle( double vAngle );


  /** \details
    Returns the prior size of the text most recently created with this text style (DXF 42). This value  
    is automatically updated after creating any text object that refers to this text style. This value 
    is also used as a default value when the height scale factor is set to zero.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::setPriorSize() method
  */
  double priorSize() const;


  /** \details
    Sets the prior size of the text most recently created with this text style (DXF 42). This value  
    is automatically updated after creating any text object that refers to this text style.
    
    \param vSize [in]  Prior size as a positive non-zero Double value.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_properties.html, Specific Properties of Text Styles>

    OdDbTextStyleTableRecord::priorSize() method
  */
  void setPriorSize( double vSize );


  /** \details
    Returns the name of the /uni-font/ file associated with this text style (DXF 3).

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::setFileName(), OdDbTextStyleTableRecord::bigFontFileName() methods
  */
  OdString fileName() const;


  /** \details
    Sets the name of the /uni-font/ file and associates this uni-font with the text style (DXF 3). 
    This /uni-font/ defines the shape of typical letters for text.

    \param sUniFont [in]  Uni-font filename without path to be associated with the text style as a String value.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::fileName(), OdDbTextStyleTableRecord::setBigFontFileName() methods
  */
  void setFileName( const OdString& sUniFont );


  /** \details
    Returns the name of the /big-font/ file associated with this text style (DXF 4).

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::setBigFontFileName(), OdDbTextStyleTableRecord::fileName() methods
  */
  OdString bigFontFileName() const;


  /** \details
    Sets the name of the /big-font/ file and associates this big-font with the text style (DXF 4). 
    This /big-font/ defines the shape of Asian-language letters and special characters for text.

    \param sBigFont [in]  Big-font filename without path to be associated with the text style as a String value.

    \sa
    <link db_textstyle_sample_record.html, Example of Working with the Text Style Record Object>

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::bigFontFileName(), OdDbTextStyleTableRecord::setFileName() methods
  */
  void setBigFontFileName( const OdString& sBigFont );


  /** \details
    Gets the characteristics of the Windows-font associated with the text style.

    \param typeface [out]  Typeface name of the /Windows-font/.
    \param bold [out]  True if and only if Bold font.
    \param italic [out]  True if and only if Italic font.
    \param charset [out]  Windows character set identitier.
    \param pitchAndFamily [out]  Windows pitch and character family identifier.

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::setFont() method
  */
  void font( OdString& typeface,
	     bool& bold,
	     bool& italic,
	     int& charset,
	     int& pitchAndFamily) const;


  /** \details
    Associates the /Windows-font/ with the text style and sets its characteristics.

    \param typeface [in]  Typeface name of the /Windows-font/.
    \param bold [in]  True - to set the Bold font.
    \param italic [in]  True - to set the Italic font.
    \param charset [in]  Windows character set identitier.
    \param pitchAndFamily [in]  Windows pitch and character family identifier.
    
    \remarks
    If typeface is an empty string, this method disassociates the /Windows-font/ with this text style
    and removes information about it.

    \sa
    <link db_textstyle_font.html, Fonts of Text Styles>

    OdDbTextStyleTableRecord::font() method
  */
  void setFont( const OdString& typeface,
		bool bold,
		bool italic,
		int charset,
		int pitchAndFamily);


  virtual OdResult subGetClassID( void* pClsid ) const;

  virtual OdResult dwgInFields( OdDbDwgFiler* pFiler );

  virtual void dwgOutFields( OdDbDwgFiler* pFiler ) const;

  virtual OdResult dxfInFields( OdDbDxfFiler* pFiler );

  virtual void dxfOutFields( OdDbDxfFiler* pFiler ) const;

  virtual OdResult dxfInFields_R12( OdDbDxfFiler* pFiler );

  virtual void dxfOutFields_R12( OdDbDxfFiler* pFiler ) const;
  
  OdResult dxfIn( OdDbDxfFiler* pFiler );

  virtual void subClose();

  virtual OdResult subErase( bool erasing );

  virtual void subHandOverTo( OdDbObject* pNewObject );

  virtual OdUInt32 subSetAttributes( OdGiDrawableTraits* pTraits ) const;
};

/** \details
  The typified smart pointer for the text style record object. This template class is 
  specialization of the OdSmartPtr class for the OdDbTextStyleTableRecord object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbTextStyleTableRecord> OdDbTextStyleTableRecordPtr;

class OdGiTextStyle;
TOOLKIT_EXPORT void giFromDbTextStyle(const OdDbTextStyleTableRecord* pTStyle, OdGiTextStyle& giStyle);
TOOLKIT_EXPORT void giFromDbTextStyle(OdDbObjectId styleId, OdGiTextStyle& giStyle);

#include "TD_PackPop.h"

#endif // _ODDBTEXTSTYLETABLERECORD_INCLUDED

