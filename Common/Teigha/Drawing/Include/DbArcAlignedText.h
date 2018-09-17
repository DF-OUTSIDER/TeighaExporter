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




#ifndef _OD_DBARCALIGNEDTEXT_INC_
#define _OD_DBARCALIGNEDTEXT_INC_

#include "TD_PackPush.h"

#include "DbEntity.h"

#ifdef ATEXT_EXPORTS
#define ATEXT_EXPORT           OD_TOOLKIT_EXPORT
#define ATEXT_EXPORT_STATIC    OD_STATIC_EXPORT
#else
#define ATEXT_EXPORT           OD_TOOLKIT_IMPORT
#define ATEXT_EXPORT_STATIC    OD_STATIC_IMPORT
#endif

enum OdArcTextAlignment
{
  kFit    = 1,
  kLeft   = 2,
  kRight  = 3,
  kCenter = 4
};

enum OdArcTextPosition
{
  kOnConvexSide = 1,
  kOnConcaveSide = 2
};

enum OdArcTextDirection
{
  kOutwardFromCenter = 1,
  kInwardToTheCenter = 2
};

/** \details
    This class represents Arc-Aligned Text entities in an OdDbDatabase instance.

    Corresponding C++ library: ATEXT

    <group OdDb_Classes>
*/
class ATEXT_EXPORT OdDbArcAlignedText : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbArcAlignedText);

  OdDbArcAlignedText();
  
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  
  /** \details
    Returns the text string for this Text entity (DXF 1).
  */
  OdString textString() const;
  
  /** \details
    Sets the text string for this Text entity (DXF 1).
    \param textString [in]  Text string.
    
    \remarks
    textString cannot exceed 256 characters excluding the null terminator.
  */
  void setTextString(const OdString& textString);
  
  /** \details
    Returns the Object ID of the Arc entity associated with this Text entity (DXF 330).
  */
  OdDbObjectId arcId() const;

  /** \details
    Sets the Object ID of the Arc entity associated with this Text entity (DXF 330).
    \param arcId [in]  Arc Object ID.
  */
  void setArcId(OdDbObjectId arcId);

  /** \details
    Returns the WCS center of the Arc entity associated with this Text entity (WCS equivalent of DXF 10).
  */
  OdGePoint3d center() const;

  /** \details
    Sets the WCS center of the Arc entity associated with this Text entity (WCS equivalent of DXF 10).
    \param center [in]  Center.
  */
  void setCenter(const OdGePoint3d& center);
  
  /** \details
    Returns the radius of the Arc entity associated with this Text entity (DXF 40).
  */
  double radius() const;

  /** \details
    Sets the radius of the Arc entity associated with this Text entity (DXF 40).
    \param radius [in]  Radius.
  */
  void setRadius(double radius);

  /** \details
    Sets the start angle of the Arc entity associated with this Text entity (DXF 50).
    
    \param startAngle [in]  Start angle.
    
    \remarks
    The angle is measured counterclockwise from the OCS X-axis.

    \remarks
    All angles are expressed in radians.
  */
  void setStartAngle(double startAngle);

  /** \details
    Returns the start angle of the Arc entity associated with this Text entity (DXF 50).

    \remarks
    The angle is measured counterclockwise from the OCS X-axis.

    \remarks
    All angles are expressed in radians.
  */
  double startAngle() const;

  /** \details
    Sets the end angle of the Arc entity associated with this Text entity (DXF 51).
    \param endAngle [in]  End angle.

    \remarks
    The angle is measured counterclockwise from the OCS X-axis.

    \remarks
    All angles are expressed in radians.
  */
  void setEndAngle(double endAngle);

  /** \details
    Returns the end angle of the Arc entity associated with this Text entity (DXF 51).

    \remarks
    The angle is measured counterclockwise from the OCS X-axis.

    \remarks
    All angles are expressed in radians.
  */
  double endAngle() const;

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;
  
  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).
    \param normal [in]  Normal.
  */
  void setNormal(const OdGeVector3d& normal);

  /** \details
    Returns the offset from the Arc entity associated with this Text entity (DXF 44).  
  */
  double offsetFromArc() const;
  
  /** \details
    Sets the offset from the Arc entity associated with this Text entity (DXF 44).
    \param offsetFromArc [in]  Offset from arc.  
  */
  void setOffsetFromArc(double offsetFromArc);
  
  /** \details
    Returns the right offset for this Text entity (DXF 45).  
  */
  double rightOffset() const;
  
  /** \details
    Sets the right offset for this Text entity (DXF 45).  
    \param rightOffset [in]  Right offset.  
  */
  void setRightOffset(double rightOffset);
  
  /** \details
    Returns the left offset for this Text entity (DXF 46).  
  */
  double leftOffset() const;
  
  /** \details
    Sets the left offset for this Text entity (DXF 46).  
    \param leftOffset [in]  Left offset.  
  */
  void setLeftOffset(double leftOffset);
  
  /** \details
    Returns the text size for this Text entity (DXF 42).    
  */
  double textSize() const;
  
  /** \details
    Sets the text size for this Text entity (DXF 42).
    \param textSize [in]  Text size.    
  */
  void setTextSize(double textSize);
  
  /** \details
    Returns the X-scale (width) factor for this Text entity (DXF 41).
  */
  double xScale() const;
  
  /** \details
    Sets the X-scale (width) factor for this Text entity (DXF 41).
    
    \param xScale [in]  X-scale factor.
  */
  void setXScale(double xScale);
  
  /** \details
    Returns the character spacing for this Text entity (DXF 43).  
  */
  double charSpacing() const;
  
  /** \details
    Sets the character spacing for this Text entity (DXF 43).
    \param charSpacing [in]  Character spacing.  
  */
  void setCharSpacing(double charSpacing);

  /** \details
    Returns true if and only if the character order is reversed for this Text entity (DXF 70).
  */
  bool isReversedCharOrder() const;
  
  /** \details
    Controls the reversal of character order for this Text entity (DXF 70).
    \param reverse [in]  Controls the reversal.
  */
  void reverseCharOrder(bool reverse);

  /** \details
    Returns the wizard flag for this Text entity (DXF 280).  
  */
  bool wizardFlag() const;
  
  /** \details
    Controls the wizard flag for this Text entity (DXF 280).  
    \param wizardFlag [in]  Controls the wizard flag.
  */
  void setWizardFlag(bool wizardFlag);

  /** \details
    Returns the alignment for this Text entity (DXF 72).
    
    \remarks
    alignment() returns one of the following:
    
    <table>
    Name      Value
    kFit      1
    kLeft     2
    kRight    3
    kCenter   4
    </table>
  */
  OdArcTextAlignment alignment() const;
  
  /** \details
    Sets the alignment for this Text entity (DXF 72).
    
    \remarks
    alignment must be one of the following:
    
    <table>
    Name      Value
    kFit      1
    kLeft     2
    kRight    3
    kCenter   4
    </table>
  */
  void setAlignment(OdArcTextAlignment alignment);

  /** \details
    Returns the text position of this Text entity.
    \remarks
    textPosition() returns one of the following:
    
    <table>
    Name            Value
    kOnConvexSide   1
    kOnConcaveSide  2
    </table>

  */
  OdArcTextPosition textPosition() const;
  
  /** \details
    Sets the text position of this Text entity.

    \param textPosition [in]  Text position.
    \remarks
    textPosition must be one of the following:
    
    <table>
    Name            Value
    kOnConvexSide   1
    kOnConcaveSide  2
    </table>
  */
  void setTextPosition(OdArcTextPosition textPosition);

  /** \details
    Returns the text direction for this Text entity (DXF 71).
    
    \remarks
    textDirection() returns one of the following:
    
    <table>
    Name                 Value    
    kOutwardFromCenter   1
    kInwardToTheCenter   2
    </table>
  */
  OdArcTextDirection textDirection() const;
  
  /** \details
    Sets the text direction for this Text entity (DXF 71).
    
    \param textDirection [in]  Text direction.
    
    \remarks
    textDirection must be one of the following:
    
    <table>
    Name                 Value    
    kOutwardFromCenter   1
    kInwardToTheCenter   2
    </table>
  */
  void setTextDirection(OdArcTextDirection textDir);

  /** \details
    Returns true if and only if this Text entity is underlined (DXF 76).  
  */
  bool isUnderlined() const;
  
  /** \details
    Controls the underlining of this Text entity (DXF 76).
    \param underlined [in]  Controls underlining.  
  */
  void setUnderlined(bool underlined);

  /** \details
    Returns the Object ID of the text style of this Text entity (DXF 7).
  */
  OdDbObjectId textStyle() const;
  
  /** \details
    Sets the Object ID of the text style of this Text entity (DXF 7).
    \param textStyleId [in]  Text style Object ID.
  */
  void setTextStyle(const OdDbObjectId &textStyleId);
  
  /** \details
    Sets the name of the text style of this Text entity (DXF 7).
    \param styleName [in]  Style name.
  */
  void setTextStyle(const OdString& styleName);

  /** \details
    Returns the name of the font file associated with this Text entity (DXF 3).
  */
  OdString fileName() const;
  
  /** \details
    Sets the name of the font file associated with this Text entity (DXF 3).

    \param filename [in]  Font filename.
  */
  void setFileName(const OdString& filename);
  
  /** \details
    Returns the name of the BigFont file associated with this Text entity (DXF 4).
  */
  OdString bigFontFileName() const;
  
  /** \details
    Sets the name of the BigFont file associated with this Text entity (DXF 4).
    
    \param bigFontFileName [in]  BigFont filename.
  */
  void setBigFontFileName(const OdString& bigFontFileName);

  /** \details
    Sets this Text entity to use the specified Windows font characteristics.

    \param typeface [in]  Typeface name of the font.
    \param bold [in]  Bold if and only if true.
    \param italic [in]  Italic if and only if true.
    \param charset [in]  Windows character set identifier.
    \param pitchAndFamily [in]  Windows pitch and character family identifier.
    
    \remarks
    If typeface is null, the Windows font information is removed from this text style.
  */
  void setFont(
    const OdString& typeface,
		bool bold,
		bool italic,
		int charset,
		int pitchAndFamily);


  /** \details
    Returns the Windows font characteristics for this Text entity.

    \param typeface [out]  Typeface name of the font.
    \param bold [out]  True if and only if bold.
    \param italic [out]  True if and only if italic.
    \param charset [out]  Windows character set identifier.
    \param pitchAndFamily [out]  Windows pitch and character family identifier.
  */
  void font(
    OdString& typeface,
		bool& bold,
		bool& italic,
		int& charset,
		int& pitchAndFamily) const;

  /** \details
    Returns true if and only if this Text entity uses an SHX font (DXF 79).
  */
  bool isShxFont() const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  /* Replace OdRxObjectPtrArray */

  virtual OdResult subExplode(OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE; 
  

  virtual void subClose() ODRX_OVERRIDE;

  /** \details
      Support for persistent reactor to arc.
  */
  
  void modified(const OdDbObject* pObject);
  void erased(const OdDbObject* pObject, bool erasing);
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbArcAlignedText object pointers.
*/
typedef OdSmartPtr<OdDbArcAlignedText> OdDbArcAlignedTextPtr;

#include "TD_PackPop.h"

#endif 


