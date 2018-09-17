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

#ifndef OD_DETAILVIEWSTYLE_H
#define OD_DETAILVIEWSTYLE_H

#include "DbModelDocViewStyle.h"

#include "TD_PackPush.h"

/** \details
    This class represents detail view style object.

    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbDetailViewStyle : public OdDbModelDocViewStyle
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDetailViewStyle);

  /** \details
    Defines the identifier placement inside the detail view symbol. 
  */
	enum IdentifierPlacement {
	  kOutsideBoundary = 0, // Identifier is placed outside of the detail view symbol boundary.
    kOutsideBoundaryWithLeader = 1, // Identifier is placed outside of the detail view symbol boundary, and the leader between the identifier and the boundary is drawn.
    kOnBoundary = 2, // Identifier is placed on the detail view symbol boundary.
    kOnBoundaryWithLeader, // Identifier is placed on the detail view symbol boundary. If then the identifier is moved out of the boundary, the leader is added.
  };

  /** \details
    Defines the edge type of the detail view symbol boundary. 
  */
  enum ModelEdge {
    kSmooth = 0, // The edge of detail view symbol is to be smooth.
    kSmoothWithBorder,
    kSmoothWithConnectionLine,
    kJagged,
  };

  /** \details
    Default constructor. Creates an OdDbDetailViewStyle instance.
  */
  OdDbDetailViewStyle();
  ~OdDbDetailViewStyle();

  /** \details
    Returns the object ID of the identifier's text style.

    \returns
    Text style object ID.
  */
  OdDbObjectId identifierStyleId() const;

  /** \details
    Sets the identifier's text style.

    \param objId [in]  Object ID of the text style.

	\returns
    OdDb::eOk if successful.
  */
  OdResult setIdentifierStyleId(const OdDbObjectId& objId);

  /** \details
    Returns the identifier's color.
  */
  OdCmColor identifierColor() const;

  /** \details
    Sets the identifier's color.

    \param color [in]  Color for the identifier.

    \returns
    Return OdDb::eOk if successful.
  */
  OdResult setIdentifierColor(const OdCmColor& color);

  /** \details
    Returns the identifier's text height.
  */
  double identifierHeight() const;

  /** \details
    Sets the identifier's text height.

    \param height [in]   Text height.

    \returns
    Return OdDb::eOk if successful.
  */
  OdResult setIdentifierHeight(double height);

  /** \details
    Returns the identifier's offset between its arrow line and extension line.
  */
  double identifierOffset() const;

  /** \details
    Sets the identifier's offset between its arrow line and extension line.
  */
  OdResult setIdentifierOffset(double offset);

  /** \details
    Returns the placement for the label and identifier.
  */
  OdDbDetailViewStyle::IdentifierPlacement identifierPlacement() const;

  /** \details
    Sets the placement for the label and identifier.

    \param placement [in]   Bit coded value that defines the placements of label and identifier.

    \returns
    OdDb::eOk if successful.
  */
  OdResult setIdentifierPlacement(OdDbDetailViewStyle::IdentifierPlacement placement);

  /** \details
    Returns the arrow symbol's object ID.
  */
  OdDbObjectId arrowSymbolId() const;

  /** \details
    Sets the arrow symbol's block.
  */
  OdResult setArrowSymbolId(const OdDbObjectId& arrowSymbolId);

  /** \details
    Returns the arrow symbol's color.
  */
  OdCmColor arrowSymbolColor() const;

  /** \details
    Sets the arrow symbol's color.
  */
  OdResult setArrowSymbolColor(const OdCmColor& color);

  /** \details
    Returns the arrow symbol's size.
  */
  double arrowSymbolSize() const;

  /** \details
    Sets the arrow symbol's size.
  */
  OdResult setArrowSymbolSize(double size);

  /** \details
    Returns whether arrowheads display.
  */
  bool showArrows() const;
  OdResult setShowArrows(bool bValue);

  /** \details
    Returns the boundary line's lineweight.
  */
  OdDb::LineWeight  boundaryLineWeight() const;

  /** \details
    Sets the boundary line's lineweight.
  */
  OdResult setBoundaryLineWeight(OdDb::LineWeight lineweight);

  /** \details
    Returns the boundary line's color.
  */
  OdCmColor boundaryLineColor() const;

  /** \details
    Sets the boundary line's color.
  */
  OdResult setBoundaryLineColor(const OdCmColor& color);

  /** \details
    Returns the object ID of the boundary line's linetype.
  */
  OdDbObjectId boundaryLineTypeId() const;
  
  /** \details
    Sets the boundary line's linetype.
  */
  OdResult setBoundaryLineTypeId(const OdDbObjectId &objId);

  /** \details
    Returns the connection line's lineweight.
  */
  OdDb::LineWeight connectionLineWeight() const;

  /** \details
    Sets the connection line's lineweight.
  */
  OdResult setConnectionLineWeight(OdDb::LineWeight lineweight);

  /** \details
    Returns the connection line's color.
  */
  OdCmColor connectionLineColor() const;

  /** \details
    Sets the connection line's color.
  */
  OdResult setConnectionLineColor(const OdCmColor& color);

  /** \details
    Returns the object ID of the connection line's linetype.
  */
  OdDbObjectId connectionLineTypeId() const;

  /** \details
    Sets the connection line's linetype.
  */
  OdResult setConnectionLineTypeId(const OdDbObjectId& objId);

  /** \details
    Returns the object ID of the view label's text style.
  */
  OdDbObjectId viewLabelTextStyleId() const;

  /** \details
    Sets the view label's text style.
  */
  OdResult setViewLabelTextStyleId(const OdDbObjectId &objId);

  /** \details
    Returns the view label's color.
  */
  OdCmColor viewLabelTextColor() const;

  /** \details
    Sets the view label's color.
  */
  OdResult setViewLabelTextColor(const OdCmColor& color);

  /** \details
    Returns the view label's text height.
  */
  double viewLabelTextHeight() const;
  OdResult setViewLabelTextHeight(double height);

  /** \details
    Returns the view label's offset.
  */
  double viewLabelOffset() const;

  /** \details
    Sets the view label's offset.
  */
  OdResult setViewLabelOffset(double offset);

  /** \details
    Returns the view label's attachment.
  */
  OdDbModelDocViewStyle::AttachmentPoint viewLabelAttachment() const;

  /** \details
    Sets the view label's attachment.
  */
  OdResult setViewLabelAttachment(OdDbModelDocViewStyle::AttachmentPoint attachment);

  /** \details
    Returns the view label's text alignment.
  */
  OdDbModelDocViewStyle::TextAlignment viewLabelAlignment() const;

  /** \details
    Sets the view label's text alignment.
  */
  OdResult setViewLabelAlignment(OdDbModelDocViewStyle::TextAlignment alignment);

  /** \details
    Returns the view label's pattern.
  */
  OdString viewLabelPattern() const;

  /** \details
    Returns the view label's pattern.


    pField  If the pattern uses fields,
            they are copied 'pField'.

    \returns
    Pattern of the view label.
  */
  OdString getViewLabelPattern(OdDbField* pField = NULL) const;

  /** \details
    Sets the view label's pattern.
    

    pattern     Pattern to assign to the view label.
    pField      (Optional) Specify a field to assign the pattern from the field
                (instead of the pattern itself).

    \returns
    OdDb::eOk if successful.
  */
  OdResult setViewLabelPattern(const OdString& pattern,
                                        const OdDbField* pField = NULL);

  /** \details
    Returns whether view label.
  */
  bool showViewLabel() const;
  OdResult setShowViewLabel(bool bValue);

  /** \details
    Returns label and model edge.
  */
  OdDbDetailViewStyle::ModelEdge modelEdge() const;

  /** \details
    Sets the placement of labels and identifiers.
  */
  OdResult setModelEdge(OdDbDetailViewStyle::ModelEdge placement);

  /** \details
    Returns the border line's lineweight.
  */
  OdDb::LineWeight borderLineWeight() const;

  /** \details
    Sets the border line's lineweight.
  */
  OdResult setBorderLineWeight(OdDb::LineWeight lineweight);

  /** \details
    Returns the border line's color.
  */
  OdCmColor borderLineColor() const;

  /** \details
    Sets the border line's color.
  */
  OdResult setBorderLineColor(const OdCmColor& color);

  /** \details
    Returns the object ID of the border line's linetype.
  */
  OdDbObjectId borderLineTypeId() const;

  /** \details
    Sets the border line's linetype.
  */
  OdResult setBorderLineTypeId(const OdDbObjectId &objId);

  // OdDbModelDocViewStyle overridden functions
  virtual OdDbObjectId postViewStyleToDb(OdDbDatabase* pDb, const OdString& styleName);

protected:
  // OdDbObject overridden functions
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

//protected:
//  // For internal use only
//  virtual OdDbObjectPtr subDeepClone(OdDbObject* pOwner,
//                                     OdDbIdMapping& idMap, bool isPrimary = true) const;
//  virtual OdDbObjectPtr subWblockClone(OdDbObject* pOwner,
//                                       OdDbIdMapping& idMap, bool isPrimary = true) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDetailViewStyle object pointers.
*/
typedef OdSmartPtr<OdDbDetailViewStyle> OdDbDetailViewStylePtr;

#include "TD_PackPop.h"

#endif // OD_DETAILVIEWSTYLE_H
