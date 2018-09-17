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


#ifndef OD_DBTABLESTYLE_H
#define OD_DBTABLESTYLE_H

#include "TD_PackPush.h"
#include "DbObject.h"
#include "DbColor.h"
#include "OdValue.h"
#include "StringArray.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  // OdDbTable and OdTbTableStyle specific enum
  //
  
  enum CellType          
  { 
    kUnknownCell    = 0,
    kTextCell       = 1,
    kBlockCell      = 2,
    kMultipleContentCell = 3
  };

  enum CellContentType   
  { 
    kCellContentTypeUnknown = 0x0,
    kCellContentTypeValue   = 0x1,
    kCellContentTypeField   = 0x2,
    kCellContentTypeBlock   = 0x4
  };

  enum CellEdgeMask      
  { 
    kTopMask        = 0x1,
    kRightMask      = 0x2,
    kBottomMask     = 0x4,
    kLeftMask       = 0x8 
  };

  enum SelectType
  { 
    kWindow         = 1,
    kCrossing       = 2 
  };

  enum FlowDirection
  { 
    kTtoB           = 0,
    kBtoT           = 1 
  };

  enum RotationAngle     
  { 
    kDegreesUnknown = -1,
    kDegrees000     = 0,
    kDegrees090     = 1,
    kDegrees180     = 2,
    kDegrees270     = 3 
  };

  enum CellAlignment
  { 
    kTopLeft        = 1,
    kTopCenter      = 2,
    kTopRight       = 3,
    kMiddleLeft     = 4,
    kMiddleCenter   = 5,
    kMiddleRight    = 6,
    kBottomLeft     = 7,
    kBottomCenter   = 8,
    kBottomRight    = 9 
  };

  enum GridLineType
  { 
    kInvalidGridLine= 0x00,
    kHorzTop        = 0x01,
    kHorzInside     = 0x02,
    kHorzBottom     = 0x04,
    kVertLeft       = 0x08,
    kVertInside     = 0x10,
    kVertRight      = 0x20,
    kHorzGridLineTypes     = kHorzTop | kHorzBottom | kHorzInside,
    kVertGridLineTypes     = kVertLeft | kVertRight | kVertInside,
    kOuterGridLineTypes    = kHorzTop | kHorzBottom | kVertLeft | kVertRight,
    kInnerGridLineTypes    = kHorzInside | kVertInside,
    kAllGridLineTypes      = kOuterGridLineTypes | kInnerGridLineTypes
  };


  enum RowType
  { 
    kUnknownRow     = 0x0,
    kDataRow        = 0x1,
    kTitleRow       = 0x2,
    kHeaderRow      = 0x4
  };

  enum TableStyleFlags
  { 
    kHorzInsideLineFirst  = 0x1,
    kHorzInsideLineSecond = 0x2,
    kHorzInsideLineThird  = 0x4,
    kTableStyleModified   = 0x8
  };

  enum RowTypes
  { 
    kAllRows = kDataRow | kTitleRow | kHeaderRow 
  };
    
  enum GridLineTypes 
  {   
    kAllGridLines = kHorzTop | kHorzInside | kHorzBottom | kVertLeft | kVertInside | kVertRight 
  };

  enum GridLineStyle  
  { 
    kGridLineStyleSingle = 1,
    kGridLineStyleDouble = 2
  };

  enum CellMargin        
  { 
    kCellMarginTop         = 0x01,
    kCellMarginLeft        = 0x02,
    kCellMarginBottom      = 0x04,
    kCellMarginRight       = 0x08,
    kCellMarginHorzSpacing = 0x10,
    kCellMarginVertSpacing = 0x20 
  };

  enum CellContentLayout
  { 
    kCellContentLayoutFlow              = 0x1,
    kCellContentLayoutStackedHorizontal = 0x2,
    kCellContentLayoutStackedVertical   = 0x4
  };

  enum CellState
  { 
    kCellStateNone                        = 0x00,
    kCellStateContentLocked               = 0x01,
    kCellStateContentReadOnly             = 0x02,
    kCellStateLinked                      = 0x04,
    kCellStateContentModifiedAfterUpdate  = 0x08,
    kCellStateFormatLocked                = 0x10,
    kCellStateFormatReadOnly              = 0x20,
    kCellStateFormatModifiedAfterUpdate   = 0x40,
    kAllCellStates  = (kCellStateContentLocked | kCellStateContentReadOnly |
                       kCellStateLinked | kCellStateContentModifiedAfterUpdate | 
                       kCellStateFormatLocked | kCellStateFormatReadOnly |
                       kCellStateFormatModifiedAfterUpdate)
  };

  enum GridProperty      
  { 
    kGridPropInvalid           = 0x00,
    kGridPropLineStyle         = 0x01,
    kGridPropLineWeight        = 0x02,
    kGridPropLinetype          = 0x04,
    kGridPropColor             = 0x08,
    kGridPropVisibility        = 0x10,
    kGridPropDoubleLineSpacing = 0x20,
    kGridPropAll               = (kGridPropLineStyle | kGridPropLineWeight | kGridPropLinetype |
                                  kGridPropColor | kGridPropVisibility | kGridPropDoubleLineSpacing)
  };

  enum CellOption        
  {
    kCellOptionNone    = 0x0,
    kInheritCellFormat = 0x1
  };

  enum CellProperty      
  { 
    kCellPropInvalid               = 0x00000,
    kCellPropDataType              = 0x00001,
    kCellPropDataFormat            = 0x00002,
    kCellPropRotation              = 0x00004,
    kCellPropScale                 = 0x00008,
    kCellPropAlignment             = 0x00010,
    kCellPropContentColor          = 0x00020,
    kCellPropTextStyle             = 0x00040,
    kCellPropTextHeight            = 0x00080,
    kCellPropAutoScale             = 0x00100,
    kCellPropBackgroundColor       = 0x00200,
    kCellPropMarginLeft            = 0x00400,
    kCellPropMarginTop             = 0x00800,
    kCellPropMarginRight           = 0x01000,
    kCellPropMarginBottom          = 0x02000,
    kCellPropContentLayout         = 0x04000,
    kCellPropMergeAll              = 0x08000,
    kCellPropFlowDirBtoT           = 0x10000,
    kCellPropMarginHorzSpacing     = 0x20000,
    kCellPropMarginVertSpacing     = 0x40000,
    kCellPropDataTypeAndFormat     = (kCellPropDataType | kCellPropDataFormat),
    kCellPropContent               = (kCellPropDataType | kCellPropDataFormat | kCellPropRotation |
                                      kCellPropScale | kCellPropContentColor |  kCellPropTextStyle | 
                                      kCellPropTextHeight | kCellPropAutoScale),
    kCellPropBitProperties         = (kCellPropAutoScale | kCellPropMergeAll | kCellPropFlowDirBtoT),
    kCellPropAll                   = (kCellPropDataType | kCellPropDataFormat | kCellPropRotation |
                                      kCellPropScale | kCellPropAlignment | kCellPropContentColor | kCellPropBackgroundColor | 
                                      kCellPropTextStyle | kCellPropTextHeight | kCellPropMarginLeft | kCellPropMarginTop |
                                      kCellPropMarginRight | kCellPropMarginBottom | kCellPropMarginHorzSpacing | 
                                      kCellPropMarginVertSpacing | kCellPropAutoScale | kCellPropMergeAll | 
                                      kCellPropFlowDirBtoT | kCellPropContentLayout)
  };

  enum TableFillOption
  {
    kTableFillOptionNone = 0,
    kTableFillOptionRow = 0x1,
    kTableFillOptionReverse = 0x2,
    kTableFillOptionGenerateSeries = 0x4,
    kTableFillOptionCopyContent = 0x8,
    kTableFillOptionCopyFormat = 0x10,
    kTableFillOptionOverwriteReadOnlyContent = 0x20,
    kTableFillOptionOverwriteReadOnlyFormat = 0x40
  };

  /** \details
    Specifies the table copy options.
  */
  enum TableCopyOption
  {
    /** None.*/
    kTableCopyOptionNone = 0,
    
    kTableCopyOptionExpandOrContractTable = 0x1,
    /** Content is not copied.*/
    kTableCopyOptionSkipContent = 0x2,
    /** If content is a value, it is not copied.*/
    kTableCopyOptionSkipValue = 0x4,
    /** If content is a field, it is not copied.*/
    kTableCopyOptionSkipField = 0x8,
    /** If content is a formula, it is not copied.*/
    kTableCopyOptionSkipFormula = 0x10,
    /** If content is a block, it is not copied.*/
    kTableCopyOptionSkipBlock = 0x20,
    /** Data links are not copied.*/
    kTableCopyOptionSkipDataLink = 0x40,
    /** If content is a label cell, it is not copied.*/
    kTableCopyOptionSkipLabelCell = 0x80,
    /** If content is a data cell, it is not copied.*/
    kTableCopyOptionSkipDataCell = 0x100,
    /** Format is not copied.*/
    kTableCopyOptionSkipFormat = 0x200,
    /** Cell style is not copied.*/
    kTableCopyOptionSkipCellStyle = 0x400,
    /** Converts formats to overrides after copying.*/
    kTableCopyOptionConvertFormatToOverrides = 0x800,
    /** Cell state is not copied.*/
    kTableCopyOptionSkipCellState = 0x1000,
    /** Content format is not copied. Ignored if kTableCopyOptionSkipFormat is set.*/
    kTableCopyOptionSkipContentFormat = 0x2000,
    /** If content types are not the same, content format is not copied. 
      Ignored if kTableCopyOptionSkipFormat or kTableCopyOptionSkipContentFormat is set.*/
    kTableCopyOptionSkipDissimilarContentFormat = 0x4000,
    /** Geometric information (column width, row height, etc.) is not copied.*/
    kTableCopyOptionSkipGeometry = 0x8000,
    /** Cell merges are not copied.*/
    kTableCopyOptionSkipMerges = 0x10000,
    /** The target range is filled by repeatedly copying the source range.*/
    kTableCopyOptionFillTarget = 0x20000,
    /** Overwrite a target cell if it has read-only content.*/
    kTableCopyOptionOverwriteReadOnlyContent = 0x40000,
    /** Overwrite a target cell if it has read-only format.*/
    kTableCopyOptionOverwriteReadOnlyFormat = 0x80000,
    kTableCopyOptionOverwriteContentModifiedAfterUpdate = 0x100000,
    kTableCopyOptionOverwriteFormatModifiedAfterUpdate = 0x200000,
    kTableCopyOptionOnlyContentModifiedAfterUpdate = 0x400000,
    kTableCopyOptionOnlyFormatModifiedAfterUpdate = 0x800000,
    /** Row height is copied while copying cells. Ignored when whole table is copied or 
      if kTableCopyOptionSkipGeometry is set.*/
    kTableCopyOptionRowHeight = 0x1000000,
    /** Column width is copied while copying cells. Ignored when whole table is copied or 
      if kTableCopyOptionSkipGeometry is set.*/
    kTableCopyOptionColumnWidth = 0x2000000,
    /** The full cell state is copied in a table.*/
    kTableCopyOptionFullCellState = 0x4000000,
    kTableCopyOptionForRountrip = 0x8000000,
    kTableCopyOptionConvertFieldToValue = 0x10000000,
    kTableCopyOptionSkipFieldTranslation = 0x20000000
  };

  /** \details
    Specifies options for merging cell styles.
  */
  enum MergeCellStyleOption {
    /** No options. If the target table style does not have a referenced cell style, 
      it will be copied from the source table style.*/
    kMergeCellStyleNone = 0,
    /** If there are conflicts with cell style names, the conflicting cell styles are copied from 
      the source to target table styles as new cell styles.*/
    kMergeCellStyleCopyDuplicates = 0x1,
    /** If there are conflicts with cell style names, the cell styles in the target 
      table style are copied and overwritten.*/
    kMergeCellStyleOverwriteDuplicates = 0x2,
    /** If there are conflicts with cell style names, the cell style format is set as overriden in the target 
      cells, with rows or columns that reference the cell styles.*/
    kMergeCellStyleConvertDuplicatesToOverrides = 0x4,
    /** If the target table style does not have a referenced cell style,
      it is not copied from the source to target table style.*/
    kMergeCellStyleIgnoreNewStyles = 0x8,
  };
}

  /** \details
    This class represents the parameters of cells for OdDbTable entities..
    
    <group Other_Classes>
  */
class OdCellRange
{
public:
  OdInt32  m_topRow;
  OdInt32  m_leftColumn;
  OdInt32  m_bottomRow;
  OdInt32  m_rightColumn;

  OdCellRange() : m_topRow(-1)
    , m_leftColumn(-1)
    , m_bottomRow(-1)
    , m_rightColumn(-1) {}

  OdCellRange(OdInt32 topRow, OdInt32 leftColumn, OdInt32 bottomRow, OdInt32 rightColumn)
    : m_topRow(topRow)
    , m_leftColumn(leftColumn)
    , m_bottomRow(bottomRow)
    , m_rightColumn(rightColumn)
  {}

  bool operator ==(
    const OdCellRange& range) const
  {
    return ( m_topRow == range.m_topRow &&
              m_leftColumn == range.m_leftColumn &&
              m_bottomRow == range.m_bottomRow &&
              m_rightColumn == range.m_rightColumn );
  }
  bool operator !=(
    const OdCellRange& range) const
  {
    return !( *this == range);
  }
};

typedef OdArray<OdCellRange> OdCellRangeArray;

 /** \details
    This class represents a grid for OdDbTable entities.
    
    <group Other_Classes>
 */
class OdGridProperty
{
public:

  OdDb::GridProperty  m_propMask;
  OdDb::GridLineStyle m_lineStyle;
  OdDb::LineWeight    m_lineWeight;
  OdDbHardPointerId   m_linetype;
  OdCmColor           m_color;
  OdDb::Visibility    m_visibility;
  double              m_doubleLineSpacing;

  OdGridProperty() : m_propMask(OdDb::kGridPropInvalid)
    , m_lineStyle(OdDb::kGridLineStyleSingle)
    , m_lineWeight(OdDb::kLnWtByBlock)
    , m_visibility(OdDb::kVisible)
    , m_doubleLineSpacing(0.0) {}

};

/** \details
    This class represents TableStyles for OdDbTable entities in an OdDbDatabase instance.

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbTableStyle : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbTableStyle);

  OdDbTableStyle();
  // virtual ~OdDbTableStyle();

  // General Properties
  //
  
  /** \details
    Returns the name of this TableStyle object.
  */
  virtual OdString getName() const;

  /** \details
    Sets the name of this TableStyle object.

    \param name [in]  Name.
  */
  virtual void setName(const OdString& name);

  /** \details
    Returns the description of this TableStyle object.
  */
  virtual OdString description() const;

  /** \details
    Sets the description of this TableStyle object.
    
    \param description [in]  Description.
  */
  virtual void setDescription(
    const OdString& description);

  /** \details
    Returns the bit flags for this TableStyle object (DXF 71). 
      
    \remarks
    bitFlags() returns a combination of zero or more of the following:
    
    <table>
    Name                      Value
    kHorzInsideLineFirst      1
    kHorzInsideLineSecond     2
    kHorzInsideLineThird      4
    kTableStyleModified       8 
    </table>
  */
  virtual OdUInt32 bitFlags() const;

  /** \details
    Sets the bit flags for this TableStyle object (DXF 71). 
    
    \param bitFlags [in]  Bit flags.
    
    \remarks
    bitFlags must be a combination of one or more of the following:
    
    <table>
    Name                      Value
    kHorzInsideLineFirst      1
    kHorzInsideLineSecond     2
    kHorzInsideLineThird      4
    kTableStyleModified       8 
    </table>
  */
  virtual void setBitFlags(
    OdUInt32 bitFlags);

  /** \details
    Returns the direction this TableStyle object flows from its first row to its last (DXF 70).

    \remarks
    flowDirection() returns one of the following:
    
    <table>
    Name          Value   Description
    OdDb::kTtoB   0       Top to Bottom
    OdDb::kBtoT   1       Bottom to Top
    </table>
  */
  virtual OdDb::FlowDirection flowDirection() const; 

  /** \details
    Sets the direction this TableStyle objectflows from its first row to its last. (DXF 70).

    \param flowDirection [in]  Flow direction.
    
    \remarks
    flowDirection must be one of the following: 
         
    <table>
    Name          Value   Description
    OdDb::kTtoB   0       Top to Bottom
    OdDb::kBtoT   1       Bottom to Top
    </table>
  */
  virtual void setFlowDirection(
    OdDb::FlowDirection flowDirection);

  /** \details
    Returns the horizontal cell margin for this TableStyle object (DXF 40). 
    \remarks
    The horizontal cell margin is the horizontal space between the cell text and the cell border.
  */
  virtual double horzCellMargin() const;

  /** \details
    Sets the horizontal cell margin for this TableStyle object (DXF 40).
    
    \param cellMargin [in]  Cell margin.
    
    \remarks
    The horizontal cell margin is the horizontal space between the cell text and the cell border.
  */
  virtual void   setHorzCellMargin(
    double cellMargin);


  /** \details
    Returns the vertical cell margin for this TableStyle object (DXF 41). 

    \remarks
    The vertical cell margin is the vertical space between the cell text and the cell border.
  */
  virtual double vertCellMargin() const;

  /** \details
    Sets the vertical cell margin for this Table entity (DXF 41).
    
    \param cellMargin [in]  Cell margin.
    
    \remarks
    The vertical cell margin is the vertical space between the cell text and the cell border.
  */
  virtual void setVertCellMargin(
    double cellMargin);

  /** \details
      Returns true if and only if the title row is suppressed for this TableStyle object (DXF 280).
  */
  virtual bool isTitleSuppressed() const;

  /** \details
    Controls the suppression of the title row (DXF 280).
    \param suppress [in]  Controls suppression.
  */
  virtual void suppressTitleRow(
    bool suppress);

  /** \details
    Returns true if and only if the header row is suppressed for this TableStyle object (DXF 281). 
  */
  virtual bool isHeaderSuppressed() const;

  /** \details
      Controls the suppression of the header row for this TableStyle object (DXF 280).
      \param enable [in]  Controls suppression.
  */
  virtual void suppressHeaderRow(
    bool suppress);


  /** \details
    Returns the Object ID of the text style for the specified row type in this TableStyle object (DXF 7).

    \param rowType [in]  Row type.
    
    \remarks
    rowType must be one of the following:
    
    <table>
    Name              Value
    kTitleRow         1
    kHeaderRow        2
    kDataRow          4
    </table>
  */
  virtual OdDbObjectId textStyle(
    OdDb::RowType rowType = OdDb::kDataRow) const;

  /** \details
    Sets the Object ID of the text style for the specified row types for this TableStyle object (DXF 7).
    
    \param rowTypes [in]  Row types.
    \param textStyleId [in]  Text style Object ID.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name              Value
    kTitleRow         1
    kHeaderRow        2
    kDataRow          4
    </table>
  */
  virtual void setTextStyle(
    const OdDbObjectId textStyleId, 
    int rowTypes = OdDb::kAllRows);

  /** \details
    Returns the text height for the specified row type in this TableStyle object (DXF 140).

    \param rowType [in]  Row type
    
    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual double textHeight(
    OdDb::RowType rowType = OdDb::kDataRow) const;

  /** \details
    Sets the text height for the specified row types in this TableStyle object (DXF 140).

    \param rowTypes [in]  Row types.
    \param height [in]  Text height.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name              Value
    kTitleRow         1
    kHeaderRow        2
    kDataRow          4
    </table>
  */
  virtual void setTextHeight(
    double height, 
    int rowTypes = OdDb::kAllRows);

  /** \details
    Returns the cell alignment for the specified row type in this TableStyle object (DXF 170).

    \param rowType [in]  Row type
    
    \remarks
    rowType() returns one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
    
    alignment() returns one of the following:
    
    <table>
    Name                    Value
    OdDb::kTopLeft          1
    OdDb::kTopCenter        2 
    OdDb::kTopRight         3
    OdDb::kMiddleLeft       4
    OdDb::kMiddleCenter     5
    OdDb::kMiddleRight      6
    OdDb::kBottomLeft       7 
    OdDb::kBottomCenter     8
    OdDb::kBottomRight      9
    </table>
  */
  virtual OdDb::CellAlignment alignment(
    OdDb::RowType rowType = OdDb::kDataRow) const;


  /** \details
    Sets the cell alignment for the specified row types in this TableStyle object (DXF 170).
    
    \param rowTypes [in]  Row types.
    \param alignment [in]  Alignment.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
    
    alignment must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTopLeft          1
    OdDb::kTopCenter        2 
    OdDb::kTopRight         3
    OdDb::kMiddleLeft       4
    OdDb::kMiddleCenter     5
    OdDb::kMiddleRight      6
    OdDb::kBottomLeft       7 
    OdDb::kBottomCenter     8
    OdDb::kBottomRight      9
    </table>
  */
  virtual void setAlignment(
    OdDb::CellAlignment alignment, 
    int rowTypes = OdDb::kAllRows);

  /** \details
    Returns the text color for the specified row type in this TableStyle object (DXF 62).

    \param rowTypes [in]  Row types.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual OdCmColor color(
    OdDb::RowType rowType = OdDb::kDataRow) const;
    

  /** \details
    Sets the text color for the specified row types in this TableStyle object (DXF 62).

    \param rowTypes [in]  Row types.
    \param color [in]  Color.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual void setColor(
    const OdCmColor& color,
    int rowTypes = OdDb::kAllRows);

  /** \details
    Returns the background color for the specified row type in this TableStyle object (DXF 63).

    \param rowType [in]  Row type.

    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual OdCmColor backgroundColor(
    OdDb::RowType rowType = OdDb::kDataRow) const;

  /** \details
    Sets the background color for the specified row type in this TableStyle object (DXF 63). 

    \param rowTypes [in]  Row types.
    \param color [in]  Background color.
    
    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual void setBackgroundColor(
    const OdCmColor& color,
    int rowTypes = OdDb::kAllRows);

  /** \details
    Returns true if and only if the background color for the specified row 
    type is disabled for this TableStyle object (DXF 283).

    \param rowType [in]  Row type
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.

    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual bool isBackgroundColorNone(
    OdDb::RowType rowType = OdDb::kDataRow) const;

  /** \details
    Controls the background color setting for the specified row types or cell in this TableStyle object (DXF 283). 

    \param disable [in]  Disables the background color if true, enables if false.
    \param rowTypes [in]  Row types.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual void setBackgroundColorNone(
    bool disable,
    int rowTypes = OdDb::kAllRows);

  //Gridline properties
  //
  
  /** \details
    Returns the grid lineweight for the specified gridline type and row type in this TableStyle object (DXF 274-279).
      
    \param gridlineType [in]  Gridline type.
    \param rowType [in]  Row type.

    \remarks
    gridlineType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kHorzTop          1
    OdDb::kHorzInside       2
    OdDb::kHorzBottom       4
    OdDb::kVertLeft         8
    OdDb::kVertInside       0x10
    OdDb::kVertRight        0x20
    </table>
    
    rowType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
    
  */
  virtual OdDb::LineWeight gridLineWeight(
    OdDb::GridLineType gridlineType,
    OdDb::RowType rowType = OdDb::kDataRow) const; 
  
  /** \details
    Sets the grid lineweight for the specified gridline types and row types,
    or the specified cell and edges in this Table entity (DXF 274-279).
      
    \param lineWeight [in]  Lineweight.      
    \param gridlineTypes [in]  Gridline types.
    \param rowTypes [in]  Row types.

    \remarks
    gridlineTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kHorzTop          1
    OdDb::kHorzInside       2
    OdDb::kHorzBottom       4
    OdDb::kVertLeft         8
    OdDb::kVertInside       0x10
    OdDb::kVertRight        0x20
    </table>
    
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual void setGridLineWeight(
    OdDb::LineWeight lineWeight, 
    int gridlineTypes = OdDb::kAllGridLines, 
    int rowTypes = OdDb::kAllRows);


  /** \details
    Returns the grid color for the specified gridline type and row type
    in this TableStyle object (DXF 63,64,65,66,68,69).
      
    \param gridlineType [in]  Gridline type.
    \param rowType [in]  Row type.

    \remarks
    gridlineType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kHorzTop          1
    OdDb::kHorzInside       2
    OdDb::kHorzBottom       4
    </table>
    
    rowType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual OdCmColor gridColor(
    OdDb::GridLineType gridlineType,
    OdDb::RowType rowType = OdDb::kDataRow) const; 

  /** \details
    Returns the grid color for the specified gridline types and row type
    in this TableStyle object (DXF 63,64,65,66,68,69).
      
    \param gridlineTypes [in]  Gridline types.
    \param rowTypes [in]  Row types.

    \remarks
    gridlineTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kHorzTop          1
    OdDb::kHorzInside       2
    OdDb::kHorzBottom       4
    </table>
    
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
  */
  virtual void setGridColor(
    const OdCmColor color, 
    int gridlineTypes = OdDb::kAllGridLines, 
    int rowTypes = OdDb::kAllRows);


  /** \details
    Returns the grid visibility for the specified gridline type and row type,
    in this TableStyle object (DXF 284-289).
      
    \param gridlineType [in]  Gridline type.
    \param rowType [in]  Row type.

    \remarks
    gridVisibility() returns one of the following:
    
    <table>
    Name                    Value
    OdDb::kInvisible        1
    OdDb::kVisible          0 
    </table>
        
    gridlineType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kHorzTop          1
    OdDb::kHorzInside       2
    OdDb::kHorzBottom       4
    OdDb::kVertLeft         8
    OdDb::kVertInside       0x10
    OdDb::kVertRight        0x20
    </table>
    
    rowType must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
    
  */
  virtual OdDb::Visibility gridVisibility(
    OdDb::GridLineType gridlineType,
    OdDb::RowType rowType = OdDb::kDataRow) const; 


/** \details
    Sets the grid visibility for the specified gridline types and row types,
    in this TableStyle object (DXF 284-289).
      
    \param gridVisibility [in]  Grid visibility.      
    \param gridlineTypes [in]  Gridline types.
    \param rowTypes [in]  Row types.

    \remarks
    gridVisibility must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kInvisible        1
    OdDb::kVisible          0 
    </table>
        
    gridlineTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kHorzTop          1
    OdDb::kHorzInside       2
    OdDb::kHorzBottom       4
    OdDb::kVertLeft         8
    OdDb::kVertInside       0x10
    OdDb::kVertRight        0x20
    </table>
    
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value
    OdDb::kTitleRow         1
    OdDb::kHeaderRow        2
    OdDb::kDataRow          4
    </table>
    
*/
  virtual void setGridVisibility(
    OdDb::Visibility gridVisiblity, 
    int gridlineTypes = OdDb::kAllGridLines, 
    int rowTypes = OdDb::kAllRows);


  // NEW 2007

  virtual void getDataType(
    OdValue::DataType& nDataType,
    OdValue::UnitType& nUnitType,
    OdDb::RowType rowType = OdDb::kDataRow ) const;

  virtual void setDataType(
    OdValue::DataType nDataType, 
    OdValue::UnitType nUnitType,
    int rowTypes = OdDb::kAllRows);

  virtual OdString format (
    OdDb::RowType rowType = OdDb::kDataRow) const;

  virtual void setFormat(
    const OdString& pszFormat,
    int rowTypes = OdDb::kAllRows);


  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  // virtual OdResult audit(OdDbAuditInfo* pAuditInfo);

  // Utility functions
  //

  /** \details
    Applies the default properties of the specified database to this TableStyle object.
    
    \param pDb [in]  Pointer to the database whose default values are to be used.
     
    \remarks
    If pDb is null, the database containing this object is used
  */
  void setDatabaseDefaults(OdDbDatabase* pDb = 0);

  /** \details
    Adds this TableStyle object to the specified database.
  
    \param pDb [in]  Pointer to the database in which to post.
    \param styleName [in]  Name for the table style. 
    \param tableStyleId [out]  Receives 

    \returns
    Returns the Object ID of the posted table style.
  */
  virtual OdDbObjectId postTableStyleToDb(OdDbDatabase* pDb, const OdString& styleName);


  const OdString  createCellStyle(void);

  void createCellStyle(const OdString& cellStyle);

  void createCellStyle(const OdString& cellStyle, const OdString& fromCellStyle);

  void renameCellStyle(const OdString& oldName, const OdString& newName);

  void deleteCellStyle(const OdString& cellStyle);

  void copyCellStyle(const OdString& srcCellStyle, const OdString& targetCellStyle);

  void copyCellStyle(const OdDbTableStyle* pSrc, const OdString& srcCellStyle, const OdString& targetCellStyle);
  
  void getUniqueCellStyleName(const OdString& baseName, OdString& sUniqueName) const;

  bool isCellStyleInUse(const OdString& cellStyle) const;

  OdInt32 numCellStyles(void) const;

  OdInt32 getCellStyles(OdStringArray& cellstyles) const;

  OdInt32 cellStyleId(const OdString& cellStyle) const;
  OdString cellStyleName(OdInt32 cellStyle) const;

  OdDbObjectId textStyle(const OdString& cellStyle) const;
  void setTextStyle (const OdDbObjectId& id, const OdString& cellStyle);

  double textHeight(const OdString& cellStyle) const;
  void setTextHeight(double dHeight, const OdString& cellStyle);

  OdDb::CellAlignment alignment(const OdString& cellStyle) const;
  void setAlignment(OdDb::CellAlignment alignment, const OdString& cellStyle);

  OdCmColor color(const OdString& cellStyle) const;
  void setColor(const OdCmColor& color, const OdString& cellStyle);

  OdCmColor backgroundColor(const OdString& cellStyle) const;
  void setBackgroundColor(const OdCmColor& color, const OdString& cellStyle);

  void getDataType(OdValue::DataType& nDataType, OdValue::UnitType& nUnitType, const OdString& cellStyle) const;
  void setDataType(OdValue::DataType nDataType, OdValue::UnitType nUnitType, const OdString& cellStyle);

  const OdString format(const OdString& cellStyle) const;
  void setFormat(const OdString& format, const OdString& cellStyle);

  OdInt32 cellClass(const OdString& cellStyle) const;
  void setCellClass(OdInt32 nClass, const OdString& cellStyle);

  double rotation(const OdString& cellStyle) const;
  void setRotation(double rotation, const OdString& cellStyle);

  bool isMergeAllEnabled (const OdString& cellStyle) const;
  void enableMergeAll(bool bEnable, const OdString& cellStyle);

  double margin(OdDb::CellMargin nMargin, const OdString& cellStyle) const;
  void setMargin(OdDb::CellMargin nMargins, double fMargin, const OdString& cellStyle);

  //Gridline properties
  //
  OdDb::LineWeight gridLineWeight(OdDb::GridLineType gridLineType, const OdString& cellStyle) const;
  void setGridLineWeight(OdDb::LineWeight lineWeight, OdDb::GridLineType gridLineTypes, const OdString& cellStyle);

  OdCmColor gridColor(OdDb::GridLineType gridLineType, const OdString& cellStyle) const;
  void setGridColor(const OdCmColor color, OdDb::GridLineType gridLineTypes, const OdString& cellStyle);

  OdDb::Visibility gridVisibility(OdDb::GridLineType gridLineType, const OdString& cellStyle) const;
  void setGridVisibility(OdDb::Visibility visible, OdDb::GridLineType gridLineTypes, const OdString& cellStyle);

  double gridDoubleLineSpacing(OdDb::GridLineType gridLineType, const OdString& cellStyle) const;
  void setGridDoubleLineSpacing(double fSpacing, OdDb::GridLineType gridLineTypes, const OdString& cellStyle);

  OdDb::GridLineStyle gridLineStyle(OdDb::GridLineType gridLineType, const OdString& cellStyle) const;
  void setGridLineStyle(OdDb::GridLineStyle nLineStyle, OdDb::GridLineType gridLineTypes, const OdString& cellStyle);

  OdDbObjectId gridLinetype(OdDb::GridLineType gridLineType, const OdString& cellStyle) const;
  void setGridLinetype(const OdDbObjectId& id, OdDb::GridLineType gridLineTypes, const OdString& cellStyle);

  void getGridProperty(OdGridProperty& gridProp, OdDb::GridLineType nGridLineTypes, const OdString& cellStyle) const;
  void setGridProperty (const OdGridProperty& gridProp, OdDb::GridLineType nGridLineTypes, const OdString& cellStyle);

  virtual OdResult subGetClassID(void* pClsid) const;

  // Template functions
  OdDbObjectId  getTemplate(void) const;

  void setTemplate(const OdDbObjectId templateId, OdDb::MergeCellStyleOption nOption);

  OdDbObjectId removeTemplate(void);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbTableStyle object pointers.
*/
typedef OdSmartPtr<OdDbTableStyle> OdDbTableStylePtr;

#include "TD_PackPop.h"

#endif // OD_DBTABLESTYLE_H
