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




#ifndef OD_DBMLINESTYLE_H
#define OD_DBMLINESTYLE_H

#include "TD_PackPush.h"

// Bricsys uses some of these constant :
#define MSTYLE_DXF_FILL_ON (0x1)
#define MSTYLE_DXF_SHOW_MITERS (0x2)
#define MSTYLE_DXF_START_SQUARE_CAP (0x10)
#define MSTYLE_DXF_START_INNER_ARCS (0x20)
#define MSTYLE_DXF_START_ROUND_CAP (0x40)
#define MSTYLE_DXF_END_SQUARE_CAP (0x100)
#define MSTYLE_DXF_END_INNER_ARCS (0x200)
#define MSTYLE_DXF_END_ROUND_CAP (0x400)
#define MSTYLE_DXF_JUST_TOP (0x1000)
#define MSTYLE_DXF_JUST_ZERO (0x2000)
#define MSTYLE_DXF_JUST_BOT (0x4000)

#include "DbObject.h"
#include "DwgDeclareMembers.h"

class OdDbMlineStyleImpl;

/** \details
    This class represents Mline Style objects in an OdDbDatabase instance.
    
    \sa
    TD_Db

    OdDbMlineStyle objects are stored in the ACAD_MLINESTYLE dictionary 
    in the Named Object Dictionary of an OdDbDatabase.
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbMlineStyle: public OdDbObject
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbMlineStyle);

  OdDbMlineStyle();

  /** \details
    Initializes or re-initializes this Mline Style object.

    \remarks
    Initialization is done as follows:
  
    <table>
    Property      Value
    Name          Empty string
    Description   Empty string
    FillColor     0
    StartAngle    90°
    EndAngle      90°
    ElementList   Empty
    </table>
  */
  void initMlineStyle();

  /** \details
    Copies the specified OdDdMlineStyle object to this Mline Style object. 

    \param source [in]  Object to be cloned.
    \param checkIfReferenced [in]  Currently ignored.
    
  */
  void set(
    const OdDbMlineStyle & source, 
    bool checkIfReferenced = true);

  /** \details
    Sets the description for this Mline Style object (DXF 3).

    \param description [in]  Description.
  */
  void setDescription(
    const OdString& description);

  /** \details
    Returns the description for this Mline Style object (DXF 3).
  */
  const OdString description() const;

  /** \details
    Sets the name for this Mline Style object (DXF 2).

    \param name [in]  Name.
  */
  virtual void setName(
    const OdString& name);

  /** \details
    Returns the name for this Mline Style object (DXF 2).
  */
  virtual const OdString name() const;

  /** \details
    Controls the display of Miters for this Mline Style object (DXF 70, bit 0x02).

    \param showThem [in]  True if and only if miters are to be displayed.  
  */
  void setShowMiters(
    bool showThem);

  /** \details
    Returns the display of Miters for this Mline Style object (DXF 70, bit 0x02).
    
    \returns
    Returns true if and only if miters are displayed.
  */
  bool showMiters() const;

  /** \details
    Controls the display of Start Square Caps for this Mline Style object (DXF 70, bit 0x01).

    \param showThem [in]  True if and only if Start Square Caps are to be displayed.  
  */
  void setStartSquareCap(
    bool showThem);

  /** \details
    Returns the display of Start Square Caps for this Mline Style object (DXF 70, bit 0x01).

    \returns
    Returns true if and only if Start Square Caps are to be displayed.
  */
  bool startSquareCap() const;

  /** \details
    Controls the display of Start Round Caps for this Mline Style object (DXF 70, bit 0x40).

    \param showThem [in]  True if and only if Start Round Caps are to be displayed.  
  */
  void setStartRoundCap(
    bool showThem);

  /** \details
    Returns the display of Start Round Caps for this Mline Style object (DXF 70, bit 0x40).
    \returns
    Returns true if and only if Start Round Caps are to be displayed.
  */
  bool startRoundCap() const;

  /** \details
    Controls the display of Start Inner Arcs for this Mline Style object (DXF 70, bit 0x20).

    \param showThem [in]  True if and only if Start Inners Arcs are to be displayed.  
  */
  void setStartInnerArcs(
    bool showThem);

  /** \details
    Returns the display of Start Inner Arcs for this Mline Style object (DXF 70, bit 0x20).

    \returns
    Returns true if and only if Start Inners Arcs are to be displayed.
  */
  bool startInnerArcs() const;

  /** \details
    Controls the display of End Square Caps for this Mline Style object (DXF 70, bit 0x80).

    \param showThem [in]  True if and only if End Square Caps are to be displayed.
      
  */
  void setEndSquareCap(
    bool showThem);

  /** \details
    Returns the display of End Square Caps for this Mline Style object (DXF 70, bit 0x80).
    \remarks
    True if and only if End Square Caps are to be displayed.
  */
  bool endSquareCap() const;

  /** \details
    Controls the display of End Round Caps for this Mline Style object (DXF 70, bit 0x200).

    \param showThem [in]  True if and only if End Round Caps are to be displayed.  
  */
  void setEndRoundCap(
    bool showThem);

  /** \details
    Returns the display of End Round Caps for this Mline Style object (DXF 70, bit 0x200).
    
    \returns
    Returns true if and only if End Round Caps are to be displayed.
  */
  bool endRoundCap() const;


  /** \details
    Controls the display of End Inner Arcs for this Mline Style object (DXF 70, bit 0x100).

    \param showThem [in]  True if and only if End Inner Arcs are to be displayed.  
  */
  void setEndInnerArcs(bool showThem);

  /** \details
    Returns the display of End Inner Arcs for this Mline Style object (DXF 70, bit 0x100).
    \returns
    Returns true if and only if End Inner Arcs are to be displayed.
  */
  bool endInnerArcs() const;

  /** \details
    Sets the fill color for this Mline Style object (DXF 62).
    
    \param fillColor [in]  Fill Color.
  */
  void setFillColor(
    const OdCmColor& fillColor);

  /** \details
    Returns the fill color for this Mline Style object (DXF 62).
  */
  OdCmColor fillColor() const;

  /** \details
    Controls the filled flag for this Mline Style object (DXF 70, bit 0x01).
    
    \param filled [in]  True for filled, false for not filled.
  */
  void setFilled(
    bool filled);

  /** \details
    Returns the filled flag for this Mline Style object (DXF 70, bit 0x01).
  */
  bool filled() const;

  /** \details
    Sets the start angle for this Mline Style object (DXF 51).
    
    \param startAngle [in]  Start angle.
  */
  void setStartAngle(
    double startAngle);
  
  /** \details
    Returns the start angle for this Mline Style object (DXF 51).
  */
  double startAngle() const;

  /** \details
    Sets the end angle for this Mline Style object (DXF 52).
    
    \param endAngle [in]  End angle.
  */
  void setEndAngle(double endAngle);

  /** \details
      Returns the end angle for this Mline Style object (DXF 52).
  */
  double endAngle() const;

  /** \details
    Adds an element to this MLine Style object.

    \param offset [in]  Offset of this element.
    \param color [in]  Color of this element.
    \param linetypeId [in]  Object ID of the linetype of this element.
    \param checkIfReferenced [in]  Currently ignored.

    \returns
    Returns the index of the newly added element.
  */
  virtual int addElement(
    double offset, 
    const OdCmColor& color,
    OdDbObjectId linetypeId, 
    bool checkIfReferenced = true);

  /** \details
    Removes the specified element from this MLine Style object.

    \param elementIndex [in]  Index of element to be removed.  
  */
  void removeElementAt(int elem);

  /** \details
      Returns the number of elements in this MLine Style (DXF 71).
  */
  int numElements() const;

  /** \details
    Sets the specified element of this MLine Style object.

    \param elementIndex [in]  Index of element to modify.  
    \param offset [in]  Offset of this element.
    \param color [in]  Color of this element.
    \param linetypeId [in]  Object ID of the linetype of this element.

    \returns
    Returns the index of the newly added element.
  */
  void setElement(
    int elementIndex, 
    double offset, 
    const OdCmColor& color,
    OdDbObjectId linetypeId);

  /** \details
    Returns the values for the specified element of this MLine Style.

    \param elementIndex [in]  Index of element to return.  
    \param offset [out]  Receives the offset of this element.
    \param color [out]  Receives the color of this element.
    \param linetypeId [out]  Receives the Object ID for the linetype of this element.

  */
  void getElementAt(
    int elementIndex, 
    double& offset, 
    OdCmColor& color,
    OdDbObjectId& linetypeId) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  OdResult subGetClassID(
    void* pClsid) const;

  virtual void appendToOwner(
    OdDbIdPair& idPair,
    OdDbObject* pOwnerObject,
    OdDbIdMapping& ownerIdMap);
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbMlineStyle object pointers.
*/
typedef OdSmartPtr<OdDbMlineStyle> OdDbMlineStylePtr;

#include "TD_PackPop.h"

#endif

