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




#ifndef OD_DBMLEADERSTYLE_H
#define OD_DBMLEADERSTYLE_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "DbMText.h"

class OdGeScale3d;
class OdString;

/** \details
    This class represents MLeader Style objects in an OdDbDatabase instance.
    
    \sa
    TD_Db

    OdDbMLeaderStyle objects are stored in the ACAD_MLEADERSTYLE dictionary 
    in the Named Object Dictionary of an OdDbDatabase.
    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbMLeaderStyle : public OdDbObject
{
public:
  /** \details
    Enumeration for the content type of an MLeader.
  */
  enum ContentType {
    /** \details
      The MLeader has no content.
    */
    kNoneContent                = 0,

    /** \details
      The MLeader has block content.
    */
    kBlockContent               = 1,

    /** \details
      The MLeader has mtext content.
    */
    kMTextContent               = 2,

    /** \details
      The MLeader has tolerance content.
    */
    kToleranceContent           = 3
  };

  /** \details
    Enumeration for the draw order type of an MLeader.
  */
  enum DrawMLeaderOrderType {
    /** \details
      MLeader content should be drawn first.
    */
    kDrawContentFirst           = 0,

    /** \details
      MLeader leaderline should be drawn first.
    */
    kDrawLeaderFirst            = 1
  };

  /** \details
    Enumeration for MLeader Line draw order type.
  */
  enum DrawLeaderOrderType {
    /** \details
      MLeader leaderline head should be drawn first.
    */
    kDrawLeaderHeadFirst        = 0,

    /** \details
      MLeader leaderline tail should be drawn first.
    */
    kDrawLeaderTailFirst        = 1
  };

  /** \details
    Enumeration for the leader type of MLeader.
  */
  enum LeaderType {
    /** \details
      Invisible leader.
    */
    kInVisibleLeader            = 0,

    /** \details
      Straight line leader.
    */
    kStraightLeader             = 1,
    
    /** \details
      Spline leader.
    */
    kSplineLeader               = 2
  };

  /** \details
    Enumeration for the text attachment direction of MLeader text.
  */
  enum TextAttachmentDirection {
    /** \details
      Horizontal.
    */
    kAttachmentHorizontal       = 0,
    /** \details
      Vertical.
    */
    kAttachmentVertical         = 1
  };

  /** \details
    Enumeration for the text attachment type of MLeader text.
  */
  enum TextAttachmentType {
    /** \details
      Use the top of the top text line as the attachment point.
    */
    kAttachmentTopOfTop         = 0,

    /** \details
      Use the middle of the top text line as the attachment point.
    */
    kAttachmentMiddleOfTop      = 1,

    /** \details
      Use the middle of the text as the attachment point.
    */
    kAttachmentMiddle           = 2,

    /** \details
      Use the middle of the bottom text line as the attachment point.
    */
    kAttachmentMiddleOfBottom   = 3,

    /** \details
      Use the bottom of the bottom text line as the attachment point.
    */
    kAttachmentBottomOfBottom   = 4,

    /** \details
      Use the bottom of bottom text line as the attachment point. Underline the bottom line.
    */
    kAttachmentBottomLine       = 5,

    /** \details
      Use the bottom of the top text line as the attachment point. Underline the top line.
    */
    kAttachmentBottomOfTopLine  = 6,

    /** \details
      Use the bottom of the top text line as the attachment point.
    */
    kAttachmentBottomOfTop      = 7,

    /** \details
      Use the bottom of the top text line as the attachment point, and underline all content.
    */
    kAttachmentAllLine          = 8,

    /** \details
      Use the center of the mtext as the attachment point for vertical attachment.
    */
    kAttachmentCenter           = 9,

    /** \details
      Use the center of the mtext as the attachment point for vertical attachment, and overline/underline all content.
    */
    kAttachmentLinedCenter      = 10
  };

  /** \details
    Enumeration for the text angle type of MLeader text.
  */
  enum TextAngleType {
    /** \details
      Text angle is specified according to the angle of the last leader line segment.
    */
    kInsertAngle                = 0,

    /** \details
      Text is unconditionally horizontal.
    */
    kHorizontalAngle            = 1,
        
    /** \details
      Text angle is specified according to the angle of the last leader line segment, 
      and text is always oriented right side up to be readable.
    */
    kAlwaysRightReadingAngle    = 2
  };

  /** \details
    Enumeration for MLeader text alignment type.
  */
  enum TextAlignmentType {
    /** \details
      MLeader text is left justified.
    */
    kLeftAlignment              = 0,

    /** \details
      MLeader text is center justified.
    */
    kCenterAlignment            = 1,

    /** \details
      MLeader text is right justified.
    */
    kRightAlignment             = 2
  };

  /** \details
    Enumeration for the block connection type of MLeader block content.
  */
  enum BlockConnectionType {
    /** \details
      MLeader should connect to the extents of the block.
    */
    kConnectExtents             = 0,

    /** \details
      MLeader should connect to the base point of the block.
    */
    kConnectBase                = 1
  };

  /** \details
    Enumeration for the direction type of an MLeader.
  */
  enum LeaderDirectionType {
    /** \details
      Unknown direction type.
    */
    kUnknownLeader              = 0,

    /** \details
      Direction type of left.
    */
    kLeftLeader                 = 1,

    /** \details
      Direction type of right.
    */
    kRightLeader                = 2,

    /** \details
      Direction type of top.
    */
    kTopLeader                  = 3,

    /** \details
      Direction type of bottom.
    */
    kBottomLeader               = 4
  };


  /** \details
    Enumeration for the segment angle type of an MLeader.
  */
  enum SegmentAngleType {
    /** \details
      Any.
    */
    kAny                        = 0, 

    /** \details
      Angle of 15 degrees.
    */
        k15                         = 1,

    /** \details
      Angle of 30 degrees.
    */
    k30                         = 2,

    /** \details
      Angle of 45 degrees.
    */
    k45                         = 3,

    /** \details
      Angle of 60 degrees.
    */
    k60                         = 4,

    /** \details
      Angle of 90 degrees.
    */
    k90                         = 6,

    /** \details
      Angle of 180 degrees.
    */
    kHorz                       = 12
  };

public:
  ODDB_DECLARE_MEMBERS(OdDbMLeaderStyle);

  OdDbMLeaderStyle();

  void setDatabaseDefaults( OdDbDatabase* pDb );

//  virtual ~OdDbMLeaderStyle();

  /** \details
    Returns the name of this OdDbMLeaderStyle.
  */
  virtual OdString getName() const;
 
  /** \details
    Sets the name of this OdDbMLeaderStyle.

    \param name [in]  Holds the name of OdDbMLeaderStyle.
  */
  virtual void   setName(
    const OdString& name);

  /** \details
    Returns the description of this OdDbMLeaderStyle.
  */
  virtual OdString  description() const;

  /** \details
    Sets the description of this OdDbMLeaderStyle.

    \param description [in]  OdDbMLeaderStyle description string.
  */
  virtual void   setDescription(
    const OdString& description);

  /** \details
    Returns the set of bit flags this OdDbMLeaderStyle.
  */
  virtual OdUInt32       bitFlags() const;

  /** \details
    Sets the bit flags for this OdDbMLeaderStyle.

    \param flags [in]  The bit flags for this OdDbMLeaderStyle.
  */
  virtual void setBitFlags(
    OdUInt32 flags);

  /** \details
    Sets the content type for this OdDbMLeaderStyle.

    \param contentType [in]  The content type.
  */
  void setContentType(
    ContentType contentType);

  /** \details
    Returns the content type for this OdDbMLeaderStyle.
  */
  ContentType contentType() const;

  /** \details
    Sets the draw order for the content of OdDbMLeader objects using this OdDbMLeaderStyle.

    \param drawMLeaderOrderType [in] Draw order type for MLeader content.
  */
  void setDrawMLeaderOrderType(
    DrawMLeaderOrderType drawMLeaderOrderType);

  /** \details
    Returns the draw order for the content of OdDbMLeader objects using this OdDbMLeaderStyle.
  */
  DrawMLeaderOrderType  drawMLeaderOrderType() const;

  /** \details
    Sets the leader line draw order for OdDbMLeader objects using this OdDbMLeaderStyle.
  
    \param drawLeaderOrderType [in]  The Leader line draw order.
  */
  void setDrawLeaderOrderType(
    DrawLeaderOrderType drawLeaderOrderType);

  /** \details
    Returns the leader line draw order for OdDbMLeader objects using this OdDbMLeaderStyle.
  */
  DrawLeaderOrderType drawLeaderOrderType() const;

  /** \details
    Sets the maximun number of segment points used for leader lines.

    \param maxLeaderSegmentsPoints [in]  The maximum number of segment points.

    \remarks
    A value of 0 for maxLeaderSegmentsPoints indicates no limit.
  */
  void setMaxLeaderSegmentsPoints(
    int maxLeaderSegmentsPoints);

  /** \details
    Returns the maximun number of segment points used for leader lines.
  */
  int maxLeaderSegmentsPoints() const;

  /** \details
    Sets the constraint angle for the first segment (used for creating OdDbMLeader objects).

    \param angle [in]  Constraint angle.

    \remarks
    A value of kAny for "angle" indicates that no constraint is used.
  */
  void setFirstSegmentAngleConstraint(
    SegmentAngleType angle);

  /** \details
    Returns the constraint angle for the first segment (used for creating OdDbMLeader objects).

    \remarks
    A return value of kAny indicates that no constraint is used.
  */
  SegmentAngleType  firstSegmentAngleConstraint() const;

  /** \details
    Sets the constraint angle for the second segment (used for creating OdDbMLeader objects).

    \param angle [in]  Constraint angle.

    \remarks
    A value of kAny for "angle" indicates that no constraint is used.
  */
  void setSecondSegmentAngleConstraint(
    SegmentAngleType angle);

  /** \details
    Sets the constraint angle for the second segment (used for creating OdDbMLeader objects).

    \remarks
    A return value of kAny indicates that no constraint is used.
  */
  SegmentAngleType  secondSegmentAngleConstraint() const;

  /** \details
    Sets the leader line type for this OdDbMLeaderStyle.

    \param leaderLineType [in]  The leader line type.
  */
  void setLeaderLineType(
    LeaderType leaderLineType);

  /** \details
    Returns the leader line type for this OdDbMLeaderStyle.
  */
  LeaderType  leaderLineType() const;

  /** \details
    Set the leader line color for this OdDbMLeaderStyle.

    \param leaderLineColor [in]  The leader line color.
  */
  void setLeaderLineColor(
    const OdCmColor& leaderLineColor);

  /** \details
    Returns the leader line color for this OdDbMLeaderStyle.
  */
  OdCmColor leaderLineColor() const;

  /** \details
    Sets the leader line linetype for this OdDbMLeaderStyle.

    \param leaderLineTypeId [in]  The linetype Id.
  */
  void setLeaderLineTypeId(
    OdDbObjectId leaderLineTypeId);

  /** \details
    Returns the leader line linetype for this OdDbMLeaderStyle.
  */
  OdDbObjectId  leaderLineTypeId() const;

  /** \details
    Sets the leader line lineweight for this OdDbMLeaderStyle.

    \param leaderLineWeight [in]  The leader line lineweight.
  */
  void setLeaderLineWeight(
    OdDb::LineWeight leaderLineWeight);

  /** \details
    Returns the leader line lineweight for this OdDbMLeaderStyle.
  */
  OdDb::LineWeight  leaderLineWeight() const;

  /** \details
    Sets the leader line landing for this OdDbMLeaderStyle.

    \param enableLanding [in]  The leader line landing.

    \remarks
    If landing is disabled, landing gap is ignored.
  */
  void setEnableLanding(
    bool enableLanding);

  /** \details
    Returns leader line landing for this OdDbMLeaderStyle.
  */
  bool  enableLanding() const;

  /** \details
    Sets the gap between the leader line tail and the MText associated with the OdDbMLeader.

    \param landingGap [in]  The gap between the leader line tail and the MText associated with the OdDbMLeader.
  */
  void setLandingGap(
    double landingGap);

  /** \details
    Returns the gap between the leader line tail and the MText associated with the OdDbMLeader.
  */
  double  landingGap() const;

  /** \details
    Sets the "dog leg" flag for this OdDbMLeaderStyle, which controls dog leg leader lines.

    \param enableDogleg [in]  "Dog leg" flag.
  */
  void setEnableDogleg(
    bool enableDogleg);

  /** \details
    Returns the "dog leg" flag for this OdDbMLeaderStyle, which controls dog leg leader lines.
  */
  bool  enableDogleg() const;

  /** \details
    Sets the "dog leg" leader line length for this OdDbMLeaderStyle.

    \param doglegLength [in]  The "dog leg" leader line length.
  */
  void setDoglegLength(
    double doglegLength);

  /** \details
    Returns the "dog leg" leader line length for this OdDbMLeaderStyle.
  */
  double  doglegLength() const;

  /** \details
    Sets the arrow symbol for this OdDbMLeaderStyle by name.

    \param name [in]  Name of the arrow symbol.
  */
  void setArrowSymbolId(
    const OdString& name);

  /** \details
    Sets the arrow symbol for this OdDbMLeaderStyle by OdDbObjectId.

    \param arrowSymbolId [in]  OdDbObjectId of the arrow symbol.
  */
  void setArrowSymbolId(
    OdDbObjectId arrowSymbolId);

  /** \details
    Returns the arrow symbol for this OdDbMLeaderStyle as an OdDbObjectId.
  */
  OdDbObjectId  arrowSymbolId() const;

  /** \details
    Sets the arrow size for this OdDbMLeaderStyle.

    \param arrowSize [in]  The arrow size.
  */
  void setArrowSize(
    double arrowSize);

  /** \details
    Returns the arrow size for this OdDbMLeaderStyle.
  */
  double  arrowSize() const;

  /** \details
    Sets the default text used in an associated MText balloon.

    \param defaultMText [in]  The default text used in an associated MText balloon.
  */
  void setDefaultMText(
    const OdDbMTextPtr defaultMText);

  /** \details
    Sets the default text used in an associated MText balloon.

    \remarks
    The returned MText object is cloned, and can be NULL if there is no default MText.
  */
  OdDbMTextPtr  defaultMText() const;

  /** \details
    Sets the text style for this OdDbMLeaderStyle by OdDbObjectId.

    \param textStyleId [in]  The text style.
  */
  void setTextStyleId(
    OdDbObjectId textStyleId);

  /** \details
    Returns the text style OdDbObjectId for this OdDbMLeaderStyle.
  */
  OdDbObjectId  textStyleId() const;

  /** \details
    Sets the text attachment type for this OdDbMLeaderStyle.

    \param textAttachmentType [in]  The text attachment type.
    \param leaderDirection [in]  The leader direction type.
  */
  void setTextAttachmentType(
    TextAttachmentType textAttachmentType,
    LeaderDirectionType leaderDirection);

  /** \details
    Returns the text attachment type for this OdDbMLeaderStyle.

    \param leaderDirection [in]  The leader direction type.

    \returns
    Returns the text attachment type.
  */
  TextAttachmentType  textAttachmentType(
    LeaderDirectionType leaderDirection) const;

  /** \details
    Sets the text angle type for this OdDbMLeaderStyle.

    \param textAngleType [in]  The text angle type.
  */
  void setTextAngleType(
    TextAngleType textAngleType);

  /** \details
    Returns the text angle type for this OdDbMLeaderStyle.
  */
  TextAngleType textAngleType() const;

  /** \details
    Sets the text alignment type for this OdDbMLeaderStyle.

    \param textAlignmentType [in]  The text alignment type.
  */
  void setTextAlignmentType(
    TextAlignmentType textAlignmentType);

  /** \details
    Returns the text alignment type for this OdDbMLeaderStyle.
  */
  TextAlignmentType textAlignmentType() const;

  /** \details
    Sets the "always create left aligned text" property for this OdDbMLeaderStyle.

    \param bAlwaysLeft [in]  If true, text will be left aligned for newly created OdDbMLeader objects.
  */
  void setTextAlignAlwaysLeft(
    bool bAlwaysLeft);

  /** \details
    Returns the "always create left aligned text" property for this OdDbMLeaderStyle.
  */
  bool  textAlignAlwaysLeft() const;

  /** \details
    Sets the text color for this OdDbMLeaderStyle.

    \param textColor [in]  The text color used for associated OdDbMText object.
  */
  void setTextColor(
    const OdCmColor& textColor);

  /** \details
    Returns the text color for this OdDbMLeaderStyle.
  */
  OdCmColor textColor() const;

  /** \details
    Sets the text height for the associated OdDbMText object.
  
    \param textHeight [in]  The text height.
  */
  void setTextHeight(
    double textHeight);

  /** \details
    Returns the text height for the associated OdDbMText object.
  */
  double  textHeight() const;

  /** \details
    Sets the display/hide property for the frame around associated text.
  
    \param enableFrameText [in]  True if a frame is to be displayed around associated text, false otherwise.
  */
  void setEnableFrameText(
    bool enableFrameText);

  /** \details
    Returns true if a frame is to be displayed around associated text, false otherwise.
  */
  bool  enableFrameText() const;

  /** \details
    Sets the alignment space value for this OdDbMLeaderStyle.

    \param alignSpace [in]  The alignment space value.
  */
  void setAlignSpace(
    double alignSpace);

  /** \details
    Returns the alignment space value for this OdDbMLeaderStyle.
  */
  double  alignSpace() const;

  /** \details
    Sets the block to be associated with this OdDbMLeaderStyle by name.

    \param name [in]  The name of the standard block.
  */
  void setBlockId(
    const OdString& name);

  /** \details
    Sets the block to be associated with this OdDbMLeaderStyle by OdDbObjectId.

    \param blockId [in]  OdDbObjectId of the OdDbBlockTableRecord associated with this OdDbMLeaderStyle.
  */
  void setBlockId(
    OdDbObjectId blockId);

  /** \details
    Returns the OdDbObjectId of the block associated with this OdDbMLeaderStyle.
  */
  OdDbObjectId  blockId() const;

  /** \details
    Sets the color for block content associated with this OdDbMLeaderStyle.

    \param blockColor [in]  The color for block content.
  */
  void setBlockColor(
    const OdCmColor& blockColor);

  /** \details
    Returns the color for block content associated with this OdDbMLeaderStyle.
  */
  OdCmColor blockColor() const;

  /** \details
    Sets the scale for the block associated with this by OdDbMLeaderStyle.

    \param scale [in]  The block scale.
  */
  void setBlockScale(
    const OdGeScale3d& scale);

  /** \details
    Returns the scale for the block associated with this by OdDbMLeaderStyle.
  */
  OdGeScale3d blockScale() const;

  /** \details
    Enables/disables usage of the scale value set by setBlockScale().

    \param enableBlockScale [in]  true if block scale is to be used, false otherwise.
  */
  void setEnableBlockScale(
    bool enableBlockScale);

  /** \details
    Returns true if block scale (set by setBlockScale) is to be used, false otherwise.
  */
  bool  enableBlockScale() const;

  /** \details
    Sets the rotation value for the block referenced by this OdDbMLeaderStyle.

    \param rotation [in]  The rotation value.
  */
  void setBlockRotation(
    double rotation);

  /** \details
    Returns the rotation value for the block referenced by this OdDbMLeaderStyle.
  */
  double  blockRotation() const;

  /** \details
    Enables/disables usage of the rotation value set by setBlockRotation().

    \param enableBlockRotation [in]  true if block rotation is to be used, false otherwise.

  */
  void setEnableBlockRotation(
    bool enableBlockRotation);

  /** \details
    Returns true if block rotation (set by setBlockRotation) is to be used, false otherwise.
  */
  bool  enableBlockRotation() const;

  /** \details
    Sets the connection type for the associated block.

    \param blockConnectionType [in]  The connection type.
  */
  void setBlockConnectionType(
    BlockConnectionType blockConnectionType);

  /** \details
    Returns the connection type for the associated block.
  */
  BlockConnectionType blockConnectionType() const;

  /** \details
    Sets the scale factor for this OdDbMLeaderStyle.

    \param scale [in]  Scale factor.
  */
  void setScale(
    double scale);

  /** \details
    Returns the scale factor for this OdDbMLeaderStyle.
  */
  virtual double  scale() const;

  /** \details
    Returns true if properties were modified, false otherwise.
  */
  bool  overwritePropChanged() const;

  /** \details
    Add this OdDbMLeaderStyle to the specified OdDbDatabase object.
    
    \param pDb [in]  Pointer of the database which will receive the OdDbMLeaderStyle object.
    \param styleName [in]  Name of the style.
    
    \returns
    Returns the OdDbObjectID of the newly added OdDbMLeaderStyle object.
  */
  OdDbObjectId postMLeaderStyleToDb(
    OdDbDatabase* pDb, 
    const OdString& styleName);

  /** \details
    Sets the annotative flag for this OdDbMLeaderStyle.

    \param isAnnotative [in]  The annotative status.
  */
  void setAnnotative(
    bool isAnnotative);

  /** \details
    Returns the annotative flag for this OdDbMLeaderStyle.
  */
  virtual bool  annotative() const;

  /** \details
    Sets the gap used in the process of breaking leader lines.

    \param size [in]  The gap used in the process of breaking leader lines.
  */
  void setBreakSize (
    double size);

  /** \details
    Returns the gap used in the process of breaking leader lines.
  */
  double  breakSize() const;

  /** \details
    Sets the text attachment direction of MText.

    \param direction [in]  The text attachment direction of MText.
  */

  void setTextAttachmentDirection(
    TextAttachmentDirection direction);

  /** \details
    Returns the text attachment direction of MText.
  */
  TextAttachmentDirection textAttachmentDirection() const;

  /** \details
    Sets horizontal mleader lines extend to text mode.

    \param bSet [in]  Extend to text mode status.
  */
  void setExtendLeaderToText(bool bSet);

  /** \details
    Returns horizontal mleader lines extend to text mode.
  */
  bool extendLeaderToText() const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual void subClose();

  OdResult subGetClassID(void* pClsid) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbMLeaderStyle object pointers.
*/
typedef OdSmartPtr<OdDbMLeaderStyle> OdDbMLeaderStylePtr;

#include "TD_PackPop.h"

#endif
