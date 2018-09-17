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




#ifndef OD_DBMTEXT_H
#define OD_DBMTEXT_H

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "TextDefs.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeDoubleArray.h"
#include "DwgDeclareMembers.h"

class OdGeVector3d;
class OdGePoint2d;

/** \details

    <group TD_Namespaces>
*/
/** \details
    This struct is used to describe a fragment of text created by explodeFragments().

    \remarks
    Each fragment contains a text string and its attributes.
    Corresponding C++ library: TD_Db
    <group !!RECORDS_tdrawings_apiref>
*/
struct OdDbMTextFragment
{
  OdGePoint3d location;           // Insertion point.

  /* OdGeVector3d normal;
     OdGeVector3d direction;
  */

  OdString text;                  // Text string.

  OdString font;                  // SHX Font.
  OdString bigfont;               // SHX Bigfont.

  OdGePoint2d extents;            // Extents in OCS.
  double capsHeight;              // Height.
  double widthFactor;             // Width factor.
  double obliqueAngle;            // Obliquing angle.
  double trackingFactor;          // Tracking factor.

  // OdUInt16 colorIndex;

  OdCmEntityColor color;          // Color.
  bool vertical;                  // Text is vertical.

  bool stackTop;                  // Text is top of stacked text.
  bool stackBottom;               // Text is bottom of stacked text.

  bool underlined;                // Text is underlined.
  bool overlined;                 // Text is overlined.
  bool strikethrough;

  OdGePoint3d underPoints[2];     // Underline endpoints.
  OdGePoint3d overPoints[2];      // Overline endpoints.
  OdGePoint3d strikePoints[2];

  //  true type font data

  OdString  fontname;             // TrueType font name, or empty string.
  int       charset;              // TrueType character set.
  bool      bold;                 // Text is bold.
  bool      italic;               // Text is italic.

  // 0 - no change 1 - change to original 2 - change to other

  int      changeStyle;           // 0 == No change; 1 == Change to original; 2 == Change to other
  bool     lineBreak;             // Text is followed by a line break.
  bool     newParagraph;          // Text fragment starts new paragraph.
};

/** \details
    This struct is used by OdDbMText::getParagraphsIndent() to return indentation and tab data.
    Corresponding C++ library: TD_Db
    <group !!RECORDS_tdrawings_apiref>
*/
struct OdDbMTextIndent
{
  double  paragraphInd;  // Subsequent line indent.

  double  firstLineInd;  // First line indent.

  OdGeDoubleArray tabs; // Tab settings.
};

typedef int(*OdDbMTextEnum)(
    OdDbMTextFragment *,
    void *);

typedef OdArray<OdDbMTextIndent> OdDbMTextIndents;

class OdDbText;

/** \details
    <group OdDb_Classes>

    This class represents paragraph (multi-line) text (MText) entities in an OdDbDatabase instance.

    Corresponding C++ library: TD_Db

    \sa
    <link db_text_multi.html, Working with Multi-Line Text>

    OdDbText, OdDbMline classes
*/
class DBENT_EXPORT OdDbMText : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbMText);

  OdDbMText();

  /** \details
    Returns the insertion point of this MText entity (WCS equivalent of DXF 10).
  */
  OdGePoint3d location() const;

  /** \details
    Sets the insertion point of this MText entity (WCS equivalent of DXF 10).

    \param location [in]  Insertion point.
  */
  void setLocation(
    const OdGePoint3d& location);

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;

  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).

    \param normal [in]  Normal.
  */
  void setNormal(
    const OdGeVector3d& normal);

  /** \details
    Returns the WCS X-axis direction vector of this MText entity (DXF 11).
  */
  OdGeVector3d direction() const;

  /** \details
    Sets the WCS X-axis direction vector of this MText entity (DXF 11).

    \param direction [in]  Direction vector.

    \remarks
    Direction and rotation are equivalent, and only one need be set when creating (or modifying) an MText entity.
  */
  void setDirection(
    const OdGeVector3d& direction);

  /** \details
    Returns the rotation angle of this MText entity in radians (DXF 50).
  */
  double rotation() const;

  /** \details
    Sets the rotation angle of this MText entity in radians (DXF 50).

    \param rotation [in]  Rotation angle in radians.
  */
  void setRotation(
    double rotation);

  /** \details
    Returns the word-wrap width of this MText entity (DXF 41).

    \remarks
    Words exceeding this value will extend beyond this width.
  */
  double width() const;

  /** \details
    Sets the word-wrap width of this MText entity (DXF 41).

    \param width [in]  Word-wrap width

    \remarks
    Words exceeding this value will extend beyond the Specified width.
  */
  void setWidth(
    double width);

  /** \details
    Returns the Object ID of the text style of this MText entity (DXF 7).
  */
  OdDbObjectId textStyle() const;

  /** \details
    Sets the Object ID of the text style of this MText entity (DXF 7).

    \param textStyleId [in]  Text style Object ID.
  */
  void setTextStyle(
    OdDbObjectId textStyleId);

  /** \details
    Returns the text height of this MText entity (DXF 40).
  */
  double textHeight() const;

  /** \details
    Sets the text height of this MText entity (DXF 40).

    \param height [in]  New text height value.

    Throws:
    Method generates the eInvalidInput exception when the text height is less than or equal to zero.
  */
  void setTextHeight(
    double height);

  enum AttachmentPoint
  {
    kTopLeft        = 1,    // Top Left
    kTopCenter      = 2,    // Top Center
    kTopRight       = 3,    // Top Right
    kMiddleLeft     = 4,    // Middle Left
    kMiddleCenter   = 5,    // Middle Center
    kMiddleRight    = 6,    // Middle Right
    kBottomLeft     = 7,    // Bottom Left
    kBottomCenter   = 8,    // Bottom Center
    kBottomRight    = 9,    // Bottom Right
    kBaseLeft       = 10,   // Baseline Left /reserved for future use/
    kBaseCenter     = 11,   // Baseline Center /reserved for future use/
    kBaseRight      = 12,   // Baseline Right /reserved for future use/
    kBaseAlign      = 13,   // Baseline Aligned /reserved for future use/
    kBottomAlign    = 14,   // Bottom Aligned /reserved for future use/
    kMiddleAlign    = 15,   // Middle Aligned /reserved for future use/
    kTopAlign       = 16,   // Top Aligned /reserved for future use/
    kBaseFit        = 17,   // Baseline Fit /reserved for future use/
    kBottomFit      = 18,   // Bottom Fit /reserved for future use/
    kMiddleFit      = 19,   // Middle Fit /reserved for future use/
    kTopFit         = 20,   // Top Fit /reserved for future use/
    kBaseMid        = 21,   // Baseline Middled /reserved for future use/
    kBottomMid      = 22,   // Bottom Middled /reserved for future use/
    kMiddleMid      = 23,   // Middle Middled /reserved for future use/
    kTopMid         = 24    // Top Middled /reserved for future use/
  };

  /** \details
    Returns the type of the attachment point of this MText entity (DXF 71).

    \remarks
    attachment() returns one of the following:

    <table>
    Name            Value
    kTopLeft        1
    kTopCenter      2
    kTopRight       3
    kMiddleLeft     4
    kMiddleCenter   5
    kMiddleRight    6
    kBottomLeft     7
    kBottomCenter   8
    kBottomRight    9
    </table>
  */
  AttachmentPoint attachment() const;

  /** \details
    Sets the type of the attachment point of this MText entity (DXF 71).
    \param type [in]  Type of the attachment point.

    \remarks
    attachment must be one of the following:

    <table>
    Name            Value
    kTopLeft        1
    kTopCenter      2
    kTopRight       3
    kMiddleLeft     4
    kMiddleCenter   5
    kMiddleRight    6
    kBottomLeft     7
    kBottomCenter   8
    kBottomRight    9
    </table>

    \remarks
    setAttachment() keeps the location of this MText entity constant
    while changing the attachment type, thereby changing the extents of (moving) this MText entity.

    setAttachmentMovingLocation() moves the location of the MText entity
    while changing the attachment type, thereby maintaining the extents (not moving) this MText entity.

    Throws:
    Method generates the eInvalidInput exception when the text attachment number is out of the range 1 to 9.

    \sa
    OdDbMText::setAttachmentMovingLocation() method
  */
  void setAttachment(
    AttachmentPoint type);

  enum FlowDirection
  {
    kLtoR     = 1,  // Left to Right
    kRtoL     = 2,  // Right to Left
    kTtoB     = 3,  // Top to Bottom
    kBtoT     = 4,  // Bottom to Top
    kByStyle  = 5   // By Style
  };

  /** \details
    Returns the flow direction of this MText entity (DXF 72).

    \remarks
    flowDirection() returns one of the following:

    <table>
    Name        Value
    kLtoR       1
    kRtoL       2
    kTtoB       3
    kBtoT       4
    kByStyle    5
    </table>
  */
  FlowDirection flowDirection() const;

  /** \details
    Sets the flow direction of this MText entity (DXF 72).

    \param flowDirection [in]  New flow direction enumerator.

    \remarks
    flowDirection() returns one of the following:

    <table>
    Name        Value
    kLtoR       1
    kRtoL       2
    kTtoB       3
    kBtoT       4
    kByStyle    5
    </table>

    Throws:
    Method generates the eInvalidInput exception when the flow direction is out of the range 1 to 5.
  */
  void setFlowDirection(
    FlowDirection flowDirection);

  /** \details
    Returns a copy of the character content of this MText entity (DXF 1 & 3).
  */
  OdString contents() const;

  /** \details
    Sets the character content of this MText entity (DXF 1 & 3).

    \param text [in]  New character content.
  */
  int setContents(const OdString& text);

  /** \details
  Sets the character content of this MText entity (DXF 1 & 3) from the RTF-encoded text string.

    \param text [in]  New character content.
  */
  int setContentsRTF(const OdString& text);

  /** \details
    Returns the width of the bounding box of this MText entity.

    \remarks
    This value will probably be different than that returned by OdDbMText::width().
  */
  double actualWidth() const;

  /** \details
    Returns the non-break space string "\~".

    \remarks
    May be used in place of "\~" to make code more understandable.
  */
  static const OdString nonBreakSpace();

  /** \details
    Returns the overline on string "\O".

    \remarks
    May be used in place of "\O" to make code more understandable.
  */
  static const OdString overlineOn();

  /** \details
    Returns the overline off string "\o".

    \remarks
    May be used in place of "\o" to make code more understandable.
  */
  static const OdString overlineOff();

  /** \details
    Returns the underline on string "\L".

    \remarks
    May be used in place of "\L" to make code more understandable.
  */
  static const OdString underlineOn();

  /** \details
    Returns the underline off string "\l".

    \remarks
    May be used in place of "\l" to make code more understandable.
  */
  static const OdString underlineOff();

  /** \details
    Returns the color change string "\C".

    \remarks
    May be used in place of "\C" to make code more understandable.
  */
  static const OdString colorChange();

  /** \details
    Returns the font change string "\F".

    \remarks
    May be used in place of "\F" to make code more understandable.
  */
  static const OdString fontChange();

  /** \details
    Returns the height change string "\H".

    \remarks
    May be used in place of "\H" to make code more understandable.
  */
  static const OdString heightChange();

  /** \details
    Returns the width change string "\W".

    \remarks
    May be used in place of "\W" to make code more understandable.
  */
  static const OdString widthChange();

  /** \details
    Returns the oblique angle change string "\Q".

    \remarks
    May be used in place of "\Q" to make code more understandable.
  */
  static const OdString obliqueChange();

  /** \details
    Returns the track change string "\T".

    \remarks
    May be used in place of "\T" to make code more understandable.
  */
  static const OdString trackChange();

  /** \details
    Returns the line break string "\p".

    \remarks
    May be used in place of "\p" to make code more understandable.
  */
  static const OdString lineBreak();

  /** \details
    Returns the paragraph break string "\P".
    \remarks
    May be used in place of "\P" to make code more understandable.
  */
  static const OdString paragraphBreak();

  /** \details
    Returns the stacked text start string "\S".
    \remarks
    May be used in place of "\S" to make code more understandable.
  */
  static const OdString stackStart();

  /** \details
    Returns the alignment change string "\A".

    \remarks
    May be used in place of "\A" to make code more understandable.
  */
  static const OdString alignChange();

  /** \details
    Returns the begin block string "{".

    \remarks
    May be used in place of "{" to make code more understandable.
  */
  static const OdString blockBegin();

  /** \details
    Returns the end block string "}".

    \remarks
    May be used in place of "}" to make code more understandable.
  */
  static const OdString blockEnd();

  /** \details
    Returns the strike through on string "\K".

    \remarks
    May be used in place of "\K" to make code more understandable.
  */
  static const OdString strikethroughOn(); 

  /** \details
    Returns the strike through off string "\k".

    \remarks
    May be used in place of "\k" to make code more understandable.
  */
  static const OdString strikethroughOff();

  /** \details
    Sets the linespacing style of this MText entity (DXF 73).

    \param lineSpacingStyle [in]  Linespacing style.

    <table>
    Name        Value
    kAtLeast       1
    kExactly       2
    </table>

    Throws:
    Method generates the eInvalidInput exception when the linespacing style is not 1 or 2.
  */
  void setLineSpacingStyle(
    OdDb::LineSpacingStyle lineSpacingStyle);

  /** \details
    Returns the linespacing style of this MText entity (DXF 73).
  */
  OdDb::LineSpacingStyle lineSpacingStyle() const;

  /** \details
    Sets the linespacing factor of this MText entity (DXF 44).

    \param lineSpacingFactor [in]  Linespacing Factor.

    Throws:
    Method generates the eInvalidInput exception when the line spacing factor is out of the range 0.25 to 4.00.
  */
  void setLineSpacingFactor(
    double lineSpacingFactor);

  /** \details
      Returns the linespacing factor of this MText entity (DXF 44).
  */
  double lineSpacingFactor() const;

  /** \details
    Returns the horizontal mode of this MText entity (used for DXF 71).

    \remarks
    horizontalMode returns one of the following:

    <table>
    Name                Value
    OdDb::kTextLeft      0
    OdDb::kTextCenter    1
    OdDb::kTextRight     2
    OdDb::kTextAlign     3
    OdDb::kTextMid       4
    OdDb::kTextFit       5
    </table>

  */
  OdDb::TextHorzMode horizontalMode() const;

  /** \details
    Sets the horizontal mode of this MText entity (used for DXF 71).

    \param horizontalMode [in]  Horizontal mode.

    \remarks
    horizontalMode must be one of the following:

    <table>
    Name                Value
    OdDb::kTextLeft      0
    OdDb::kTextCenter    1
    OdDb::kTextRight     2
    OdDb::kTextAlign     3
    OdDb::kTextMid       4
    OdDb::kTextFit       5
    </table>

    Throws:
    Method generates the eNotApplicable exception when the horizontal mode is out of the range 0 to 5.
  */
  void setHorizontalMode(
    OdDb::TextHorzMode horizontalMode);

  /** \details
    Returns the vertical mode of this MText entity (used for DXF 71).

    \remarks
    verticalMode() returns one of the following:

    <table>
    Name                  Value
    OdDb::kTextBase       0
    OdDb::kkTextBottom    1
    OdDb::kkTextVertMid   2
    OdDb::kkTextTop       3
    </table>
  */
  OdDb::TextVertMode verticalMode() const;

  /** \details
    Sets the vertical mode of this MText entity (used for DXF 71).

    \param verticalMode [in]  Vertical mode.

    \remarks
    verticalMode must be one of the following:

    <table>
    Name                  Value
    OdDb::kTextBottom     1
    OdDb::kTextVertMid    2
    OdDb::kTextTop        3
    </table>

    Throws:
    Method generates the eNotApplicable exception when the vertical mode is out of the range 1 to 3.
  */
  void setVerticalMode(
    OdDb::TextVertMode verticalMode);

  /** \details
  Returns the entity coordinate system matrix.
  */
  OdGeMatrix3d getEcs( ) const;
  
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  virtual OdResult subTransformBy(const OdGeMatrix3d& xfm);

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult explodeGeometry(
    OdRxObjectPtrArray& entitySet) const;

  OdResult subGetClassID(
    void* pClsid) const;

  void subClose();

  /** \details
    Explodes this MText entity into a sequence of simple text fragments, passing each
    fragment to the specified function

    \param fragmentFn [in]  Function pointer to the fragment elaboration callback function.
    \param params [in]  Void pointer to the fragmentFn callback function's second argument.
    \param ctxt [in]  Drawing context.

    \remarks
    The prototype of the fragment elaboration is as follows:

            int (*OdDbMtextEnum)(OdDbMTextFragment *fragment, void *param)

    If ctxt is null, the current OdGiWorldDraw object will be used.

    The elaboration function should return 1 to continue, or 0 to terminate the explosion.

    Each new line in this MText entity, and each change of text attributes, will start a new fragment.
  */
  void explodeFragments(
    OdDbMTextEnum fragmentFn,
    void *params,
    OdGiWorldDraw *ctxt = 0) const;

  /** \details
    Returns the actual height of this MText entity (depends on contents).

    \param ctxt [in]  Drawing context.

    \remarks
    If ctxt is null, the current OdGiWorldDraw object will be used.
  */
  double actualHeight(
    OdGiWorldDraw *ctxt = 0) const;

  /** \details
    Returns the WCS actual bounding points of this MText entity.

    \param boundingPoints [out]  Receives the bounding points.
    \param dGapX [in]  specifies gap aroung actual text in X direction.
    \param dGapY [in]  specifies gap aroung actual text in Y direction.

    \remarks
    The points are returned as follows:

    <table>
    Point                Corner
    boundingPoints[0]    Top left
    boundingPoints[1]    Top right
    boundingPoints[2]    Bottom left
    boundingPoints[3]    Bottom right
    </table>
  */
  void getActualBoundingPoints(
    OdGePoint3dArray& boundingPoints, double dGapX = 0., double dGapY = 0.) const;

  /** \details
    Returns the WCS bounding points of this MText entity box.

    \param boundingPoints [out]  Receives the bounding points.

    \remarks
    The points are returned as follows:

    <table>
    Point                Corner
    boundingPoints[0]    Top left
    boundingPoints[1]    Top right
    boundingPoints[2]    Bottom left
    boundingPoints[3]    Bottom right
    </table>
  */
  void getBoundingPoints(
    OdGePoint3dArray& boundingPoints) const;

  /** \details
    Sets the type of the attachment point of this MText entity (DXF 71).

    \param attachment [in]  Type of the attachment point.

    \remarks
    attachment must be one of the following:

    <table>
    Name            Value
    kTopLeft        1
    kTopCenter      2
    kTopRight       3
    kMiddleLeft     4
    kMiddleCenter   5
    kMiddleRight    6
    kBottomLeft     7
    kBottomCenter   8
    kBottomRight    9
    </table>

    \remarks
    setAttachment() keeps the location of this MText entity constant
    while changing the attachment type, thereby changing the extents of (moving) this MText entity.

    setAttachmentMovingLocation() adjusts the location of the MText entity
    while changing the attachment type, so as to maintain the extents (not move) this MText entity.

    \sa
    OdDbMText::setAttachment() method
  */
  OdResult setAttachmentMovingLocation(
    AttachmentPoint attachment);

  virtual OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Returns true if and only if background fill is on of this MText entity (DXF 90, bit 0x01).
  */
  bool backgroundFillOn() const;

  /** \details
    Controls the background fill of this MText entity (DXF 90, bit 0x01).
    \param bEnable [in]  True to enable background fill, false to disable.
  */
  void setBackgroundFill(
    bool bEnable);

  /** \details
    Returns the background fill color of this MText entity (DXF 63 and optionally 421 & 431).
  */
  OdCmColor getBackgroundFillColor() const;

  /** \details
    Sets the background fill color of this MText entity (DXF 63 and optionally 421 & 431).
    \param color [in]  New color value.
  */
  void setBackgroundFillColor(
    const OdCmColor& color);

  /** \details
    Returns the background scale factor of this MText entity (DXF 45).

    \remarks
    The filled background area is extended by (ScaleFactor - 1) * TextHeight in all directions.
  */
  double getBackgroundScaleFactor() const;

  /** \details
    Sets the background scale factor of this MText entity (DXF 45).

    \param scaleFactor [in]  Background scale factor. [1..5]

    \remarks
    The filled background area is extended by (ScaleFactor - 1) * TextHeight in all directions.

    Throws:
    Method generates the eInvalidInput exception when the scale factor is out of the range 1.0 to 5.0.
  */
  void setBackgroundScaleFactor(
    const double scaleFactor);

  /** \details
    Returns the background transparency of this MText entity (DXF 441).
  */
  OdCmTransparency getBackgroundTransparency() const;

  /** \details
    Sets the background transparency of this MText entity (DXF 441).
    \param transparency [in]  New transparency value.
  */
  void setBackgroundTransparency(
    const OdCmTransparency& transparency);

  /** \details
    Returns true if and only if the screen background color is
    used as the/ background color/ of this MText entity (DXF 90, bit 0x02).
  */
  bool useBackgroundColorOn() const;

  /** \details
    Controls the use of the screen background color as the background color
    of this MText entity (DXF 90, bit 0x02).

    \param enable [in]  True to use the screen background color, false to use
               the setBackgroundFillColor() method.
  */
  void setUseBackgroundColor(
    bool enable);

  /** \details
    Returns the paragraph indentation and tab data of this MText entity.
    \param indents [out]  Receives the indentation and tab data.
  */
  void getParagraphsIndent(
    OdDbMTextIndents& indents) const;

  OdDbObjectId setField(
    const OdString& fieldName,
    OdDbField* pField);
  OdResult removeField(
    OdDbObjectId fieldId);
  OdDbObjectId removeField(
    const OdString& fieldName);

  /** \details
    Converts the fields in this MText entity to text, and removes the fields.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
    
    \remarks
    The fields are not evaluated before conversion.
  */
  void convertFieldToText();

  /** \details
    This function is an override for OdDbEntity::subSetDatabaseDefaults() to set 
    the dimension style of this entity to the current style for the specified database.
  */
  void subSetDatabaseDefaults(
    OdDbDatabase *pDb,
    bool doSubents);

  enum ColumnType {
    kNoColumns,
    kStaticColumns,
    kDynamicColumns
  };

  void setDynamicColumns(double width, double gutter, bool bAutoHeigh);
  void setStaticColumns(double width, double gutter, OdInt16 count);

  /** \details
    Returns the column type of this MText entity.

    \remarks
    The type is returned as follows:

    <table>
    Name            Value
    kNoColumns        1
    kStaticColumns    2
    kDynamicColumns   3
    </table>
  */
  ColumnType getColumnType() const;

  /** \details
    Sets the column type of this MText entity.

    \param colType [in]  Column type.

    <table>
    Name            Value
    kNoColumns        1
    kStaticColumns    2
    kDynamicColumns   3
    </table>
  */
  void setColumnType(ColumnType colType);

  /** \details
    Returns the column auto height status of this MText entity.
  */
  bool getColumnAutoHeight() const;

  /** \details
    Switches the column auto height status of this MText entity.

    \param bAutoHeigh [in]  New column auto height status.

    Throws:
    Method generates the eNotApplicable exception when the column type is not set to kDynamicColumns.
  */
  void setColumnAutoHeight(bool bAutoHeigh);

  /** \details
    Returns the column count of this MText entity.
  */
  OdInt32 getColumnCount() const;

  /** \details
    Sets the column count of this MText entity.

    \param numCol [in]  Number of columns.

    Throws:
    Method generates the eInvalidInput exception when the column count is out of the range 1 to 100.
  */
  void setColumnCount(OdInt32 numCol);

  /** \details
    Returns the column width of this MText entity.
  */
  double getColumnWidth() const;

  /** \details
    Sets the column width of this MText entity.

    \param colWidth [in]  Width of columns.

    Throws:
    Method generates the eInvalidInput exception when the column width is a non-positive double value.
  */
  void setColumnWidth(double colWidth);

  /** \details
    Returns the column gutter of this MText entity.
  */
  double getColumnGutterWidth() const;

  /** \details
    Sets the column gutter of this MText entity.

    \param colGutter [in]  Gutter of columns.

    Throws:
    Method generates the eInvalidInput exception when the column gutter is a negative value.
  */
  void setColumnGutterWidth(double colGutter);

  /** \details
    Returns the column flow reverse status of this MText entity.
  */
  bool getColumnFlowReversed() const;

  /** \details
    Switches the column flow reverse status of this MText entity.

    \param bFlowRev [in]  New status.
  */
  void setColumnFlowReversed(bool bFlowRev);

  /** \details
    Returns the column height of this MText entity.

    \param col [in]  Column number.
  */
  double getColumnHeight(OdInt16 col) const;

  /** \details
    Sets the column height of this MText entity.

    \param col [in]  Column number.
    \param colHeight [in]  Height of the specified column.

    Throws:
    Method generates the eInvalidInput exception when the column height is a non-positive double value.
  */
  void setColumnHeight(OdInt16 col, double colHeight);

  /** \details
    Returns the height of the box of this MText entity.
  */
  double height() const;

  /** \details
    Sets the height of the box of this MText entity.

    \param value [in]  New height value.
  */
  void setHeight(double value);

  virtual OdResult getPlane(OdGePlane& plane, OdDb::Planarity& planarity) const;

  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  /** \details
    Returns the ascent of this MText entity.
  */
  double      ascent() const;

  /** \details
    Returns the descent of this MText entity.
  */
  double      descent() const;

  /** \details
    Sets the flag controlling mtext border's visibility.

    \param bEnable [in]  - True means borders will be visible and false means borders will be invisible.
  */
  void setShowBorders(bool bEnable);

  /** Gets the flag controlling mtext border's visibility.  
  */
  bool showBorders();

  /** Gets contents without formate codes.
  */
  OdString text();
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbMText object pointers.
*/
typedef OdSmartPtr<OdDbMText> OdDbMTextPtr;

#include "TD_PackPop.h"

#endif

