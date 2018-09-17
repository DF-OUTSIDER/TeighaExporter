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




#ifndef OD_DBTEXT_H
#define OD_DBTEXT_H

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "TextDefs.h"
#include "DwgDeclareMembers.h"

/** \details
    <group OdDb_Classes>

    This class represents single-line text entities in an OdDbDatabase instance.
  
    Corresponding C++ library: TD_Db

    \sa
    <link db_text_single.html, Working with Single-Line Text>

    OdDbMText, OdDbShape classes
*/
class DBENT_EXPORT OdDbText: public OdDbEntity
{
public:
  
  DWGMAP_DECLARE_MEMBERS(OdDbText);
  
  OdDbText();
  
  /** \details
    Returns the position of this entity (WCS equivalent of DXF 10).
  */
  OdGePoint3d position() const;
  
  /** \details
    Sets the position of this entity (WCS equivalent of DXF 10).

    \param position [in]  Position.
  */
  void setPosition(
    const OdGePoint3d& position);
  
  /** \details
    Returns the alignment point of this Text entity (WCS equivalent of DXF 11).
  */
  OdGePoint3d alignmentPoint() const;
  
  /** \details
    Sets the alignment point of this Text entity (WCS equivalent of DXF 11).
    \param alignment [in]  Alignment point.
  */
  void setAlignmentPoint(
    const OdGePoint3d& alignment);
  
  /** \details
    Return true if and only if this Text entity is in the default alignment.
    \remarks
    The default alignment is when horizontal mode is OdDb::kTextLeft and vertical mode is OdDb::kTextBase, or when horizontal mode is OdDb::kTextMiddle
  */
  bool isDefaultAlignment() const;
  
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
 
  /** \remarks
      Always returns true.
  */
  virtual bool isPlanar() const { return true; }
  
  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;
  
  /** \details
    Returns the thickness of this entity (DXF 39).
    
    \remarks
    Thickness is the extrusion length along the normal.
  */
  double thickness() const;
  
  /** \details
    Sets the thickness of this entity (DXF 39).
    \param thickness [in]  Thickness.
    \remarks
    Thickness is the extrusion length along the normal.
  */
  void setThickness(
    double thickness);
  
  /** \details
    Returns the oblique angle of this Text entity (DXF 51).

    \remarks
    The range of oblique is ±1.48335 radians (±85°).
    
    Oblique angles are measured clockwise from the vertical.

    \remarks
    All angles are expressed in radians.  
  */
  double oblique() const;
  
  /** \details
    Sets the oblique angle of this Text entity (DXF 51).
    \param oblique [in]  Oblique angle.
    
    \remarks
    The range of oblique is ±1.48335 radians (±85°).
    
    Oblique angles are measured clockwise from the vertical.
     
    \remarks
    All angles are expressed in radians.  
  */
  void setOblique(
    double oblique);
  
  /** \details
    Returns the rotation angle of this Text entity (DXF 50).
    \remarks
    All angles are expressed in radians.
  */
  double rotation() const;
  
  /** \details
    Sets the rotation angle of this Text entity (DXF 50).
    \param rotation [in]  Rotation angle.
    \remarks
    All angles are expressed in radians.
  */
  void setRotation(
    double rotation);
  
  /** \details
    Returns the height of this Text entity (DXF 40).
  */
  double height() const;

  /** \details
    Sets the height of this Text entity (DXF 40).
    \param height [in]  Text height.
  */
  void setHeight(
    double height);
  
  /** \details
    Returns the width factor of this Text entity (DXF 41).
  */
  double widthFactor() const;
  
  /** \details
    Sets the width factor of this Text entity (DXF 40).
    \param widthFactor [in]  Width factor.  
  */
  void setWidthFactor(
    double widthFactor);
  
  /** Returns true if and only if this Text entity is mirrored in the X (horizontal) direction (DXF 71, bit 0x02).
  */
  bool isMirroredInX() const;
  
  /** Controls the mirroring of this Text entity in the X (horizontal) direction (DXF 71, bit 0x02).
    \param mirror [in]  Controls mirroring.
  */
  void mirrorInX(
    bool mirror);
  
  /** Returns true if and only if this Text entity is mirrored in the Y (vertical) direction (DXF 71, bit 0x04).
  */
  bool isMirroredInY() const;
  
  /** Controls the mirroring of this Text entity in the Y (vertical) direction (DXF 71, bit 0x04).
    \param mirror [in]  Controls mirroring.
  */
  void mirrorInY(
    bool mirror);
  
  /** \details
    Returns the text string of this Text entity (DXF 1).
  */
  OdString textString() const;
  
  /** \details
    Sets the text string of this Text entity (DXF 1).
    \param textString [in]  Text string.
    
    \remarks
    textString cannot exceed 256 characters excluding the null terminator.
  */
  virtual void setTextString(
    const OdString& textString);
  
  /** \details
    Returns the Object ID of the text style of this Text entity (DXF 7).
  */
  OdDbObjectId textStyle() const;
  
  /** \details
    Sets the Object ID of the text style of this Text entity (DXF 7).
    \param textStyleId [in]  Text style Object ID.
  */
  void setTextStyle(
    OdDbObjectId textStyleId);
  
  /** \details
    Returns the horizontal mode of this Text entity (DXF 72).

    \remarks
    horizontalMode returns one of the following:
    
    <table>
    Name                 Value
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
    Returns the horizontal mode of this Text entity (DXF 72).

    \param horizontalMode [in]  Horizontal mode.

    \remarks
    horizontalMode returns one of the following:
    
    <table>
    Name                 Value
    OdDb::kTextLeft      0
    OdDb::kTextCenter    1 
    OdDb::kTextRight     2
    OdDb::kTextAlign     3
    OdDb::kTextMid       4    
    OdDb::kTextFit       5
    </table>
  */
  void setHorizontalMode(OdDb::TextHorzMode horizontalMode);
  
  /** \details
    Returns the vertical mode of this Text entity (DXF 73).

    \remarks
    verticalMode() returns one of the following:
    
    <table>
    Name                 Value
    OdDb::kTextBase      0
    OdDb::kTextBottom    1 
    OdDb::kTextVertMid   2
    OdDb::kTextTop       3
    </table>
  */
  OdDb::TextVertMode verticalMode() const;

  /** \details
    Sets the vertical mode of this Text entity (DXF 73).

    \param verticalMode [in]  Vertical mode.
    \remarks
    verticalMode must be one of the following:
    
    <table>
    Name                 Value
    OdDb::kTextBase      0
    OdDb::kTextBottom    1 
    OdDb::kTextVertMid   2
    OdDb::kTextTop       3
    </table>
  */
  void setVerticalMode(
    OdDb::TextVertMode verticalMode);
  
  /** \details
    Evokes the spell checker on this Text entity.
    
    \returns
    Returns 0 is successful, or 1 if not.
  */
  int correctSpelling();
  
  virtual OdResult subGetClassID(
    void* pClsid) const;
  
  /** \details
    Adjusts the position of this Text entity if its alignent is not left baseline.
    
    \param pDb [in]  Pointer to database used to resolve the text style of this
            Text entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.

    \remarks
    This function is called by Teigha when a Text entity is closed.

    If this Text entity is database resident, pDb is ignored.
    
    If this Text entity is not database resident, pDb cannot be NULL.
  */
  virtual void adjustAlignment(
    OdDbDatabase* pDb = 0);
  
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);
  
  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;
  
  void subClose();
  
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xfm, 
    OdDbEntityPtr& pCopy) const ODRX_OVERRIDE;

  /** \details
    Returns the WCS bounding points of this Text entity.
    
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

  /* Constructor. */
  /*OdDbText(const OdGePoint3d& position,
      const OdChar* text,
      OdDbObjectId style = OdDbObjectId::kNull,
      double height = 0,
      double rotation = 0);
  */

  OdDbObjectId setField(
    const OdString& fieldName, 
    OdDbField* pField);
  OdResult removeField(
    OdDbObjectId fieldId);
  OdDbObjectId removeField(
    const OdString& fieldName);

  /** \details
    Converts the fields in this Text entity to text, and removes the fields.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    The fields are not evaluated before conversion.
  */
  void convertFieldToText();

  /** \details
    This function is an override for OdDbEntity::subSetDatabaseDefaults() to set 
    the text style of this entity to the current style and text size for the specified database.
  */
  virtual void subSetDatabaseDefaults(OdDbDatabase *pDb, bool doSubents) ODRX_OVERRIDE;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbText object pointers.
*/
typedef OdSmartPtr<OdDbText> OdDbTextPtr;

#include "TD_PackPop.h"

#endif // ODDBTEXT_H

