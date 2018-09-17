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


#ifndef OD_DBTABLE_H
#define OD_DBTABLE_H

#include "TD_PackPush.h"
#include "DbBlockReference.h"
#include "DbTableStyle.h"
#include "DbDataLink.h"
#include "UInt32Array.h"

// typedef for OdSubentPathArray
//

class OdDbTableImpl;
class OdDbLinkedTableData;
class OdDbTableTemplate;

namespace OdDb
{
  /** \details
  Enumerator representing table break options.
  */
  enum TableBreakOption  
  { 
    /** None.*/  
    kTableBreakNone                 = 0,
    /** Enable table breaking.*/ 
    kTableBreakEnableBreaking       = 0x01,
    /** Repeat top labels for all tables.*/
    kTableBreakRepeatTopLabels      = 0x02,
    /** Repeat bottom labels for all tables.*/
    kTableBreakRepeatBottomLabels   = 0x04,
    /** Allow manual positions for individual tables.*/
    kTableBreakAllowManualPositions = 0x08,
    /** Allow setting manual height of all tables individually.*/
    kTableBreakAllowManualHeights   = 0x10
  };
  
  /** \details
  Enumerator representing flow direction options for table breaking.
  */
  enum TableBreakFlowDirection 
  { 
    /** Break tables in the direction from left to right.*/  
    kTableBreakFlowRight      = 0x1,
    /** Break tables in the directions from top to bottom (for top to bottom flow table) or from bottom to top (for bottom to top flow table).*/  
    kTableBreakFlowDownOrUp   = 0x2,
    /** Break tables in the direction from right to left.*/ 
    kTableBreakFlowLeft       = 0x4
  };

  /** \details
  Enumerator representing a location of the item in the table.
  */
  enum TableHitItem        
  { 
    /** There is no item at the specified point.*/  
    kTableHitNone            = 0,
    /** The item is a cell.*/  
    kTableHitCell            = 0x1,
    /** The item is at a row.*/
    kTableHitRowIndicator    = 0x2,
    /** The item is at a column.*/
    kTableHitColumnIndicator = 0x4,
    /** The item is at a grid line.*/
    kTableHitGridLine        = 0x8,
  };
};

/** \details
    This class represents table entities in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbTable: public OdDbBlockReference
{
public:
  ODDB_DECLARE_MEMBERS(OdDbTable);

  /** \details
  Enumerator representing the table style overrides for an OdDbTable object.
  */
  enum TableStyleOverrides 
  {
    /** Title suppressed table style override for OdDbTable object.*/
    kTitleSuppressed             = 1,
    /** Header suppressed table style override for OdDbTable object.*/
    kHeaderSuppressed            = 2,
    /** Flow direction table style override for OdDbTable object.*/
    kFlowDirection               = 3,   
    /** Horizontal cell margin table style override for OdDbTable object.*/    
    kHorzCellMargin              = 4,
    /** Vertical cell margin table style override for OdDbTable object.*/    
    kVertCellMargin              = 5,
    /** Title row color table style override for OdDbTable object.*/   
    kTitleRowColor               = 6,
    /** Header row color table style override for OdDbTable object.*/ 
    kHeaderRowColor              = 7,
    /** Data row color table style override for OdDbTable object.*/ 
    kDataRowColor                = 8,
    /** Title row no fill table style override for OdDbTable object.*/
    kTitleRowFillNone            = 9,
    /** Header row no fill table style override for OdDbTable object.*/
    kHeaderRowFillNone           = 10,
    /** Data row no fill table style override for OdDbTable object.*/
    kDataRowFillNone             = 11,
    /** Title row fill color table style override for OdDbTable object.*/
    kTitleRowFillColor           = 12,
    /** Header row fill color table style override for OdDbTable object.*/
    kHeaderRowFillColor          = 13,
    /** Data row fill color table style override for OdDbTable object.*/
    kDataRowFillColor            = 14,
    /** Title row alignment table style override for OdDbTable object.*/
    kTitleRowAlignment           = 15,
    /** Header row alignment table style override for OdDbTable object.*/
    kHeaderRowAlignment          = 16,
    /** Data row alignment table style override for OdDbTable object.*/
    kDataRowAlignment            = 17,
    /** Title row text style table style override for OdDbTable object.*/
    kTitleRowTextStyle           = 18,
    /** Header row text style table style override for OdDbTable object.*/
    kHeaderRowTextStyle          = 19,
    /** Data row text style table style override for OdDbTable object.*/
    kDataRowTextStyle            = 20,
    /** Title row text height table style override for OdDbTable object.*/
    kTitleRowTextHeight          = 21,
    /** Header row text height table style override for OdDbTable object.*/
    kHeaderRowTextHeight         = 22,
    /** Data row text height table style override for OdDbTable object.*/
    kDataRowTextHeight           = 23,
    /** Title row data type table style override for OdDbTable object.*/
    kTitleRowDataType            = 24,
    /** Header row data type table style override for OdDbTable object.*/
    kHeaderRowDataType           = 25,
    /** Data row data type table style override for OdDbTable object.*/
    kDataRowDataType             = 26,
    /** Title row horizontal top gridline color table style override for OdDbTable object.*/
    kTitleHorzTopColor           = 40,
    /** Title row horizontal inside gridline color table style override for OdDbTable object.*/
    kTitleHorzInsideColor        = 41,
    /** Title row horizontal bottom gridline color table style override for OdDbTable object.*/
    kTitleHorzBottomColor        = 42,
    /** Title row vertical left gridline color table style override for OdDbTable object.*/
    kTitleVertLeftColor          = 43,
    /** Title row vertical inside gridline color table style override for OdDbTable object.*/
    kTitleVertInsideColor        = 44,
    /** Title row vertical right gridline color table style override for OdDbTable object.*/
    kTitleVertRightColor         = 45,
    /** Header row horizontal top gridline color table style override for OdDbTable object.*/
    kHeaderHorzTopColor          = 46,
    /** Header row horizontal inside gridline color table style override for OdDbTable object.*/
    kHeaderHorzInsideColor       = 47,
    /** Header row horizontal bottom gridline color table style override for OdDbTable object.*/
    kHeaderHorzBottomColor       = 48,
    /** Header row vertical left gridline color table style override for OdDbTable object.*/
    kHeaderVertLeftColor         = 49,
    /** Header row vertical inside gridline color table style override for OdDbTable object.*/
    kHeaderVertInsideColor       = 50,
    /** Header row vertical right gridline color table style override for OdDbTable object.*/
    kHeaderVertRightColor        = 51,
    /** Data row horizontal top gridline color table style override for OdDbTable object.*/
    kDataHorzTopColor            = 52,
    /** Data row horizontal inside gridline color table style override for OdDbTable object.*/
    kDataHorzInsideColor         = 53,
    /** Data row horizontal bottom gridline color table style override for OdDbTable object.*/
    kDataHorzBottomColor         = 54,
    /** Data row vertical left gridline color table style override for OdDbTable object.*/
    kDataVertLeftColor           = 55,
    /** Data row vertical inside gridline color table style override for OdDbTable object.*/
    kDataVertInsideColor         = 56,
    /** Data row vertical right gridline color table style override for OdDbTable object.*/
    kDataVertRightColor          = 57,
    /** Title row horizontal top gridline lineweight table style override for OdDbTable object.*/
    kTitleHorzTopLineWeight      = 70,
    /** Title row horizontal inside gridline lineweight table style override for OdDbTable object.*/
    kTitleHorzInsideLineWeight   = 71,
    /** Title row horizontal bottom gridline lineweight table style override for OdDbTable object.*/
    kTitleHorzBottomLineWeight   = 72,
    /** Title row vertical left gridline lineweight table style override for OdDbTable object.*/
    kTitleVertLeftLineWeight     = 73,
    /** Title row vertical inside gridline lineweight table style override for OdDbTable object.*/
    kTitleVertInsideLineWeight   = 74,
    /** Title row vertical right gridline lineweight table style override for OdDbTable object.*/
    kTitleVertRightLineWeight    = 75,
    /** Header row horizontal top gridline lineweight table style override for OdDbTable object.*/
    kHeaderHorzTopLineWeight     = 76,
    /** Header row horizontal inside gridline lineweight table style override for OdDbTable object.*/
    kHeaderHorzInsideLineWeight  = 77,
    /** Header row horizontal bottom gridline lineweight table style override for OdDbTable object.*/
    kHeaderHorzBottomLineWeight  = 78,
    /** Header row vertical left gridline lineweight table style override for OdDbTable object.*/
    kHeaderVertLeftLineWeight    = 79,
    /** Header row vertical inside gridline lineweight table style override for OdDbTable object.*/
    kHeaderVertInsideLineWeight  = 80,
    /** Header row vertical right gridline lineweight table style override for OdDbTable object.*/
    kHeaderVertRightLineWeight   = 81,
    /** Data row horizontal top gridline lineweight table style override for OdDbTable object.*/
    kDataHorzTopLineWeight       = 82,
    /** Data row horizontal inside gridline lineweight table style override for OdDbTable object.*/
    kDataHorzInsideLineWeight    = 83,
    /** Data row horizontal bottom gridline lineweight table style override for OdDbTable object.*/
    kDataHorzBottomLineWeight    = 84,
    /** Data row vertical left gridline lineweight table style override for OdDbTable object.*/
    kDataVertLeftLineWeight      = 85,
    /** Data row vertical inside gridline lineweight table style override for OdDbTable object.*/
    kDataVertInsideLineWeight    = 86,
    /** Data row vertical right gridline lineweight table style override for OdDbTable object.*/
    kDataVertRightLineWeight     = 87,

    /** Title row horizontal top gridline visibility table style override for OdDbTable object.*/
    kTitleHorzTopVisibility      = 100,
    /** Title row horizontal inside gridline visibility table style override for OdDbTable object.*/
    kTitleHorzInsideVisibility   = 101,
    /** Title row horizontal bottom gridline visibility table style override for OdDbTable object.*/
    kTitleHorzBottomVisibility   = 102,
    /** Title row vertical left gridline visibility table style override for OdDbTable object.*/
    kTitleVertLeftVisibility     = 103,
    /** Title row vertical inside gridline visibility table style override for OdDbTable object.*/
    kTitleVertInsideVisibility   = 104,
    /** Title row vertical right gridline visibility table style override for OdDbTable object.*/
    kTitleVertRightVisibility    = 105,
    /** Header row horizontal top gridline visibility table style override for OdDbTable object.*/
    kHeaderHorzTopVisibility     = 106,
    /** Header row horizontal inside gridline visibility table style override for OdDbTable object.*/
    kHeaderHorzInsideVisibility  = 107,
    /** Header row horizontal bottom gridline visibility table style override for OdDbTable object.*/
    kHeaderHorzBottomVisibility  = 108,
    /** Header row vertical left gridline visibility table style override for OdDbTable object.*/
    kHeaderVertLeftVisibility    = 109,
    /** Header row vertical inside gridline visibility table style override for OdDbTable object.*/
    kHeaderVertInsideVisibility  = 110,
    /** Header row vertical right gridline visibility table style override for OdDbTable object.*/
    kHeaderVertRightVisibility   = 111,
    /** Data row horizontal top gridline visibility table style override for OdDbTable object.*/
    kDataHorzTopVisibility       = 112,
    /** Data row horizontal inside gridline visibility table style override for OdDbTable object.*/
    kDataHorzInsideVisibility    = 113,
    /** Data row horizontal bottom gridline visibility table style override for OdDbTable object..*/
    kDataHorzBottomVisibility    = 114,
    /** Data row vertical left gridline visibility table style override for OdDbTable object..*/
    kDataVertLeftVisibility      = 115,
    /** Data row vertical inside gridline visibility table style override for OdDbTable object..*/
    kDataVertInsideVisibility    = 116,
    /** Data row vertical right gridline visibility table style override for OdDbTable object..*/
    kDataVertRightVisibility     = 117,

    /** Alignment table style override for table cell.*/
    kCellAlignment               = 130,
    /** Background no fill table style override for table cell.*/
    kCellBackgroundFillNone      = 131,
    /** Background color table style override for table cell.*/
    kCellBackgroundColor         = 132,
    /** Content color table style override for table cell.*/
    kCellContentColor            = 133,
    /** Text style table style override for table cell.*/
    kCellTextStyle               = 134,
    /** Text height table style override for table cell.*/
    kCellTextHeight              = 135,
    /** Top edge grid color table style override for table cell.*/
    kCellTopGridColor            = 136,
    /** Right edge grid color table style override for table cell.*/
    kCellRightGridColor          = 137,
    /** Bottom edge grid color table style override for table cell.*/
    kCellBottomGridColor         = 138,
    /** Left edge grid color table style override for table cell.*/
    kCellLeftGridColor           = 139,
    /** Top edge grid lineweight table style override for table cell.*/
    kCellTopGridLineWeight       = 140,
    /** Right edge grid lineweight table style override for table cell.*/
    kCellRightGridLineWeight     = 141,
    /** Bottom edge grid lineweight table style override for table cell.*/
    kCellBottomGridLineWeight    = 142,
    /** Left edge grid lineweight table style override for table cell.*/
    kCellLeftGridLineWeight      = 143,
    /** Top edge grid visibility table style override for table cell.*/
    kCellTopVisibility           = 144,
    /** Right edge grid visibility table style override for table cell.*/
    kCellRightVisibility         = 145,
    /** Bottom edge grid visibility table style override for table cell.*/
    kCellBottomVisibility        = 146,
    /** Left edge grid visibility table style override for table cell.*/
    kCellLeftVisibility          = 147    
  };

  OdDbTable();
  // virtual ~OdDbTable();


  /** \details
    Returns the Object ID of the OdDbTableStyle used by this table entity (DXF 342).
  */
  virtual OdDbObjectId tableStyle() const;

  /** \details
    Sets the Object ID of the OdDbTableStyle for use by this table entity (DXF 342).

    \param tableStyleId [in]  Object ID of the table style.

    \remarks
    Method generates the eInvalidInput exception when the object ID is invalid.
  */
  virtual void setTableStyle(
    const OdDbObjectId& tableStyleId);

  /** \details
    Returns the unit X-axis for this table entity in WCS coordinates (DXF 11, 21, 31).
  */
  virtual OdGeVector3d direction() const;
  
  /** \details
    Sets the X-axis for this table entity in WCS coordinates (DXF 11, 21, 31).
    
    \param horizVector [in] Horizontal vector.
  */
  virtual void setDirection(
    const OdGeVector3d& horizVector);

  /** \details
    Returns the number of rows in this table entity (DXF 91).
    
    \remarks
    This includes title and header rows, if any. 
  */
  virtual OdUInt32 numRows() const;

  /** \details
    Sets the number of rows for this table entity (DXF 91).
    
    \param numRows [in]  Number of rows.
    
    \remarks
    This includes title and header rows, if any. The number of rows must be greater than zero.
    
    Method generates the eInvalidInput exception when the number of rows is less than one.
  */
  virtual void setNumRows(
    OdUInt32 numRows);

  /** \details
    Returns the number of columns in this table entity (DXF 92). 
  */
  virtual OdUInt32 numColumns() const;

  /** \details
    Sets the number of columns for this table entity (DXF 92). 
    
    \param numColumns [in]  Number of columns.
    
    \remarks
    The number of columns must be greater than zero.
    
    Method generates the eInvalidInput exception when the number of columns is less than one.
  */
  virtual void setNumColumns(OdUInt32 numColumns);

  /** \details
    Returns the overall width of this table entity.
  */
  virtual double width() const;

  /** \details
    Sets the overall width for this table entity. 
    
    \param width [in]  Overall width.
    
    \remarks
    Column widths may be adjusted proportionally.
    
    Method generates the eInvalidInput exception when the width is negative.

    \sa
    columWidth
  */
  virtual void setWidth(double width);

  /** \details
    Returns the width of the specified column in this table entity (DXF 142).
    
    \param column [in]  Column index. 
  */
  virtual double columnWidth(OdUInt32 column) const;
  
  /** \details
    Sets the width of the specified column in this table entity (DXF 142).

    \param column [in]  Column index. 
    \param width [in]  Column width.

    \remarks
    Method generates the eInvalidInput exception when the column width is negative.
  */
  virtual void setColumnWidth(
    OdUInt32 column, 
    double width);
  
  /** \details
    Sets the width of all columns in this table entity (DXF 142).

    \param width [in]  Column width.

    \remarks
    Method generates the eInvalidInput exception when the column width is negative.
  */
  virtual void setColumnWidth(
    double width);

  /** \details
    Returns the overall height of this table entity.
  */
  virtual double height() const;

  /** \details
    Sets the overall height of this table entity.
    
    \param height [in]  Overall height.
      
    \remarks
    Row heights may be adjusted proportionally.
    
    Method generates the eInvalidInput exception when the height is negative.
  */
  virtual void setHeight(
    double height);

  /** \details
    Returns the height of the specified row in this table entity (DXF 141).
    
    \param row [in]  Row index.
  */
  virtual double rowHeight(
    OdUInt32 row) const;
  
  /** \details
    Sets the height of the specified row in this table entity (DXF 141).

    \param row [in]  Row index.
    \param height [in]  Row height.

    \remarks
    Method generates the eInvalidInput exception when the row height is negative.
  */
  virtual void setRowHeight(
    OdUInt32 row, 
    double height);
    
  /** \details
    Sets the height of all rows in this table entity (DXF 141).

    \param height [in]  Row height.

    \remarks
    Method generates the eInvalidInput exception when the row height is negative.
  */
  virtual void setRowHeight(
    double height);
  
  /** \details
    Returns the minimum column width for the specified column in this table entity.
    
    \param column [in]  Column index. 
  */
  virtual double minimumColumnWidth(
    OdUInt32 column) const;

  /** \details
    Returns the minimum row height for the specified row in this table entity.
    
    \param row [in]  Row index.
  */
  virtual double minimumRowHeight(
    OdUInt32 row) const;    

  /** \details
    Returns the minimum overall width for this table entity.
  */
  virtual double minimumTableWidth() const;
  
  /** \details
    Returns the minimum overall height for this table entity.
  */
  virtual double minimumTableHeight() const;        

  //********************************************************************
  // Get and set methods for table style overrides
  //********************************************************************
  
  /** \details
    Returns the horizontal cell margin for this table entity (DXF 40). 

    \remarks
    The horizontal cell margin is the horizontal space between the cell text and the cell border.
  */
  virtual double horzCellMargin() const;

  /** \details
    Sets the horizontal cell margin for this table entity (DXF 40).
    
    \param cellMargin [in]  Cell margin.
    
    \remarks
    The horizontal cell margin is the horizontal space between the horizontal cell text and the cell border.
    
    Method generates the eInvalidInput exception when the cell margin is negative or zero.
  */
  virtual void setHorzCellMargin(
    double cellMargin);

  /** \details
    Returns the vertical cell margin for this table entity (DXF 41). 

    \remarks
    The vertical cell margin is the vertical space between the cell text and the cell border.
  */
  virtual double vertCellMargin() const;

  /** \details
    Sets the vertical cell margin for this table entity (DXF 41).
    
    \param cellMargin [in]  Cell margin.
    
    \remarks
    The vertical cell margin is the vertical space between the cell text and the cell border.
    
    Method generates the eInvalidInput exception when the vertical cell margin is negative or zero.
  */
  virtual void setVertCellMargin(double cellMargin);

  /** \details
    Returns the direction that this table entity flows from its first row to its last (DXF 70).

    \remarks
    flowDirection() Returns one of the following:
    
    <table>
    Name          Value   Description
    OdDb::kTtoB   0       Top to Bottom
    OdDb::kBtoT   1       Bottom to Top
    </table>
  */
  virtual OdDb::FlowDirection flowDirection() const; 

  /** \details
    Sets the direction that this table entity flows from its first row to its last. (DXF 70).

    \param flowDirection [in]  Flow direction.
    
    \remarks
    flowDirection must be one of the following: 
         
    <table>
    Name          Value   Description
    OdDb::kTtoB   0       Top to Bottom
    OdDb::kBtoT   1       Bottom to Top
    </table>

    Method generates the eInvalidInput exception when the flow direction is not equal to 0 or 1.
  */
  virtual void setFlowDirection(
    OdDb::FlowDirection flowDirection);

  /** \details
      Returns true if and only if the title row is suppressed for this table entity (DXF 280).
  */
  virtual bool isTitleSuppressed() const;

  /** \details
    Controls the suppression of the title row (DXF 280).
    
    \param suppress [in]  Controls suppression.
  */
  virtual void suppressTitleRow(
    bool suppress);

  /** \details
    Returns true if and only if the header row is suppressed for this table entity (DXF 281). 
  */
  virtual bool isHeaderSuppressed() const;

  /** \details
    Controls the suppression of the header row for this table entity (DXF 280).
    
    \param suppress [in]  Controls suppression.
  */
  virtual void suppressHeaderRow(
    bool suppress);

  /** \details
    Returns the cell alignment for the specified row type in this table entity (DXF 170).

    \param rowType [in]  Row type.
    
    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
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
    Returns the cell alignment for the specified cell in this table entity (DXF 170).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    
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
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the cell alignment for the specified row types in this table entity (DXF 170).
    
    \param alignment [in]  Alignment.
    \param rowTypes [in]  Row types.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
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

    Method generates the eInvalidInput exception when the alignment is out of the available range 1 to 9, 
    when the row argument is more than the number of rows, or when the column argument is more than the number of columns.
  */
  virtual void setAlignment(
    OdDb::CellAlignment alignment, 
    OdUInt32 rowTypes = OdDb::kAllRows);

  /** \details
    Sets the cell alignment for the specified cell in this table entity (DXF 170).
    
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param alignment [in]  Alignment.

    Alignment must be one of the following:
    
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

    Method generates the eInvalidInput exception when the alignment is out of the available range 1 to 9, 
    when the row argument is more than the number of rows, or when the column argument is more than the number of columns.
  */ 
  virtual void setAlignment(
    OdUInt32 row, 
    OdUInt32 column, 
    OdDb::CellAlignment alignment);

  /** \details
    Returns true if and only if the background color for the specified row type is disabled for this table entity (DXF 283).

    \param rowType [in]  Row type.

    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual bool isBackgroundColorNone(
    OdDb::RowType rowType = OdDb::kDataRow) const;
    
  /** \details
    Returns true if and only if the background color for the specified cell is disabled for this table entity (DXF 283).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual bool isBackgroundColorNone(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Controls the background color setting for the specified row types in this table entity (DXF 283). 

    \param disable [in]  Disables the background color if true, enables if false.
    \param rowTypes [in]  Row types.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>

    Method generates the eInvalidInput exception when the row type mask is more than 7, 
    the row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setBackgroundColorNone(
    bool disable, 
    OdUInt32 rowTypes = OdDb::kAllRows);
    
  /** \details
    Controls the background color setting for the specified cell in this table entity (DXF 283). 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param disable [in]  Disables the background color if true, enables if false.

    Method generates the eInvalidInput exception when the row type mask is more than 7, 
    the row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setBackgroundColorNone(
    OdUInt32 row, 
    OdUInt32 column, 
    bool disable);
    
  /** \details
    Returns the background color for the specified row type in this table entity (DXF 63).

    \param rowType [in]  Row type.

    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdCmColor backgroundColor(
    OdDb::RowType rowType = OdDb::kDataRow) const;
    
  /** \details
    Returns the background color for the specified cell in this table entity (DXF 63).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */    
  virtual OdCmColor backgroundColor(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the background color for the specified row types in this table entity (DXF 63). 

    \param color [in]  Background color.
    \param rowTypes [in]  Row types.
    
    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>

    Method generates the eInvalidInput exception when the row type mask is more than 7, 
    the row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setBackgroundColor(
    const OdCmColor& color, 
    OdUInt32 rowTypes = OdDb::kAllRows);
    
  /** \details
    Sets the background color for the specified cell in this table entity (DXF 63). 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param color [in]  Background color.
    
    Method generates the eInvalidInput exception when the row type mask is more than 7, 
    the row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setBackgroundColor(
    OdUInt32 row, 
    OdUInt32 column,
    const OdCmColor& color);

  /** \details
    Returns the content color for the specified row type in this table entity (DXF 64).

    \param rowType [in]  Row type.

    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdCmColor contentColor(
    OdDb::RowType rowType = OdDb::kDataRow) const;
  
  /** \details
    Returns the content color for the specified cell in this table entity (DXF 64).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual OdCmColor contentColor(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the content color for the specified row types in this table entity (DXF 64). 

    \param color [in]  Content color.
    \param nRowType [in]  Row types.

    \remarks
    nRowType must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>

    Method generates the eInvalidInput exception when the row type mask is more than 7, the 
    row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setContentColor(
    const OdCmColor& color, 
    OdUInt32 nRowType = OdDb::kAllRows);
  
  /** \details
    Sets the content color for the specified cell in this table entity (DXF 64). 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param color [in]  Content color.
   
    Method generates the eInvalidInput exception when the row type mask is more than 7, the 
    row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setContentColor(
    OdUInt32 row, 
    OdUInt32 column,
    const OdCmColor& color);

  /** \details
    Returns the Object ID of the text style for the specified row type in this table entity (DXF 7).

    \param rowType [in]  Row type.
    
    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdDbObjectId textStyle(
    OdDb::RowType rowType = OdDb::kDataRow) const;
  
  /** \details
    Returns the Object ID of the text style for the specified cell in this table entity (DXF 7).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual OdDbObjectId textStyle(
    OdUInt32 row, 
    OdUInt32 column) const;

    
  /** \details
    Sets the Object ID of the text style for the specified row types in this table entity (DXF 7).
    
    \param textStyleId [in]  Text style Object ID.
    \param rowTypes [in]  Row types.

    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>

    Method generates the eInvalidInput exception when the row type mask is more than 7 
    or the object ID is invalid or kNull.
  */
  virtual void setTextStyle(
    const OdDbObjectId& textStyleId, 
    OdUInt32 rowTypes = OdDb::kAllRows);
    
  /** \details
    Sets the Object ID of the text style for the specified cell in this table entity (DXF 7).
    
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param textStyleId [in]  Text style Object ID.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows, 
    the column argument is more than the number of columns, or the cell type is not OdDb::kTextCell.
  */
  virtual void setTextStyle(
    OdUInt32 row, 
    OdUInt32 column, 
    const OdDbObjectId& textStyleId);
    
  /** \details
    Returns the text height for the specified row type in this table entity (DXF 140).

    \param rowType [in]  Row type.
    
    \remarks
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual double textHeight(
    OdDb::RowType rowType = OdDb::kDataRow) const;
    
  /** \details
    Returns the text height for the specified cell in this table entity (DXF 140).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual double textHeight(
    OdUInt32 row, 
    OdUInt32 column) const;


  /** \details
    Sets the text height for the specified row types or cell in this table entity (DXF 140).

    \param height [in]  Text height.
    \param rowTypes [in]  Row types.


    \remarks
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
    
    Method generates the eInvalidInput exception when the row type mask is more than 7 or the text 
    height is negative.
  */
  virtual void setTextHeight(
    double height, 
    OdUInt32 rowTypes = OdDb::kAllRows);
    
    
  /** \details
    Sets the text height for the specified cell in this table entity (DXF 140).

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param height [in]  Text height.

    Method generates the eInvalidInput exception when the row argument is more than the number of rows, 
    the column argument is more than the number of columns, or the cell type is not OdDb::kTextCell.
  */
  virtual void setTextHeight(
    OdUInt32 row, 
    OdUInt32 column, 
    double height);

  /** \details
    Returns the grid lineweight for the specified gridline type and row type in this table entity (DXF 274-279).
      
    \param gridlineType [in]  Gridline type.
    \param rowType [in]  Row type.

    \remarks
    gridlineType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kHorzTop          1       Top or bottom horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kHorzInside       2       All horizontal grid lines, excluding the bottom and top lines.
    OdDb::kHorzBottom       4       Bottom or top horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kVertLeft         8       Left-most table's grid line.
    OdDb::kVertInside       0x10    All vertical grid lines, excluding the left-most and right-most lines.
    OdDb::kVertRight        0x20    Right-most table's grid line.
    </table>
    
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdDb::LineWeight gridLineWeight(
    OdDb::GridLineType gridlineType,
    OdDb::RowType rowType = OdDb::kDataRow) const; 

  /** \details
    Returns the grid lineweight for the specified cell and edge in this table entity (DXF 274-279).
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param edgeType [in]  Edge type.


    \remarks
    edgeType must be one of the following:

    <table>
    Name                    Value   Description
    OdDb::kTopMask          1       Top-edge index of the table cell.
    OdDb::kRightMask        2       Right-edge index of the table cell.
    OdDb::kBottomMask       4       Bottom-edge index of the table cell.
    OdDb::kLeftMask         8       Left-edge index of the table cell.
    </table>
  */
  virtual OdDb::LineWeight gridLineWeight(
    OdUInt32 row, 
    OdUInt32 column,
    OdDb::CellEdgeMask edgeType) const;
    
  /** \details
    Sets the grid lineweight for the specified gridline types and row types in this table entity (DXF 274-279).
      
    \param lineWeight [in]  Lineweight.      
    \param gridlineTypes [in]  Gridline types.
    \param rowTypes [in]  Row types.

    \remarks
    gridlineTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kHorzTop          1       Top or bottom horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kHorzInside       2       All horizontal grid lines, excluding the bottom and top lines.
    OdDb::kHorzBottom       4       Bottom or top horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kVertLeft         8       Left-most table's grid line.
    OdDb::kVertInside       0x10    All vertical grid lines, excluding the left-most and right-most lines.
    OdDb::kVertRight        0x20    Right-most table's grid line.
    </table>
    
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>

    Method generates the eInvalidInput exception when the row argument is more than the number of rows 
    or the column argument is more than the number of columns.
  */
  virtual void setGridLineWeight(
    OdDb::LineWeight lineWeight, 
    OdUInt32 gridlineTypes, 
    OdUInt32 rowTypes);
    
  /** \details
    Sets the grid lineweight for the specified cell and edges in this table entity (DXF 274-279).
       
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param edgeTypes [in]  Edge types.
    \param lineWeight [in]  Lineweight. 

    \remarks
    edgeTypes must be a combination of one or more of the following:

    <table>
    Name                    Value   Description
    OdDb::kTopMask          1       Top-edge index of the table cell.
    OdDb::kRightMask        2       Right-edge index of the table cell.
    OdDb::kBottomMask       4       Bottom-edge index of the table cell.
    OdDb::kLeftMask         8       Left-edge index of the table cell.
    </table>

    Method generates the eInvalidInput exception when the row argument is more than the number of rows 
    or the column argument is more than the number of columns.
  */    
  virtual void setGridLineWeight(
    OdUInt32 row, 
    OdUInt32 column, 
    OdInt16 edgeTypes,
    OdDb::LineWeight lineWeight);

  /** \details
    Returns the grid color for the specified gridline type and row type in this table entity (DXF 63,64,65,66,68,69).
      
    \param gridlineType [in]  Gridline type.
    \param rowType [in]  Row type.

    \remarks
    gridlineType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kHorzTop          1       Top or bottom horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kHorzInside       2       All horizontal grid lines, excluding the bottom and top lines.
    OdDb::kHorzBottom       4       Bottom or top horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    </table>
    
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdCmColor gridColor(
    OdDb::GridLineType gridlineType,
    OdDb::RowType rowType = OdDb::kDataRow) const; 
    
  /** \details
    Returns the grid color for the specified cell and edge in this table entity (DXF 63,64,65,66,68,69).
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param edgeType [in]  Edge type.

    \remarks
    edgeType must be one of the following:

    <table>
    Name                    Value   Description
    OdDb::kTopMask          1       Top-edge index of the table cell.
    OdDb::kRightMask        2       Right-edge index of the table cell.
    OdDb::kBottomMask       4       Bottom-edge index of the table cell.
    OdDb::kLeftMask         8       Left-edge index of the table cell.
    </table>
  */
  virtual OdCmColor gridColor(
    OdUInt32 row, 
    OdUInt32 column,
    OdDb::CellEdgeMask edgeType) const;

  /** \details
    Returns the grid visibility for the specified gridline type and row type in this table entity (DXF 284-289).
      
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
    Name                    Value   Description
    OdDb::kHorzTop          1       Top or bottom horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kHorzInside       2       All horizontal grid lines, excluding the bottom and top lines.
    OdDb::kHorzBottom       4       Bottom or top horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kVertLeft         8       Left-most table's grid line.
    OdDb::kVertInside       0x10    All vertical grid lines, excluding the left-most and right-most lines.
    OdDb::kVertRight        0x20    Right-most table's grid line.
    </table>
    
    rowType must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdDb::Visibility gridVisibility(
    OdDb::GridLineType gridlineType,
    OdDb::RowType rowType = OdDb::kDataRow) const; 
    
  /** \details
    Returns the grid visibility for the specified cell and edge in this table entity (DXF 284-289).
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param edgeType [in]  Edge type.

    \remarks
    gridVisibility() returns one of the following:
    
    <table>
    Name                    Value
    OdDb::kInvisible        1
    OdDb::kVisible          0 
    </table>
        
    edgeType must be one of the following:

    <table>
    Name                    Value   Description
    OdDb::kTopMask          1       Top-edge index of the table cell.
    OdDb::kRightMask        2       Right-edge index of the table cell.
    OdDb::kBottomMask       4       Bottom-edge index of the table cell.
    OdDb::kLeftMask         8       Left-edge index of the table cell.
    </table>
  */
  virtual OdDb::Visibility gridVisibility(
    OdUInt32 row, 
    OdUInt32 column,
    OdDb::CellEdgeMask edgeType) const;
    
/** \details
    Sets the grid visibility for the specified gridline types and row types in this table entity (DXF 284-289).
      
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
    Name                    Value   Description
    OdDb::kHorzTop          1       Top or bottom horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kHorzInside       2       All horizontal grid lines, excluding the bottom and top lines.
    OdDb::kHorzBottom       4       Bottom or top horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kVertLeft         8       Left-most table's grid line.
    OdDb::kVertInside       0x10    All vertical grid lines, excluding the left-most and right-most lines.
    OdDb::kVertRight        0x20    Right-most table's grid line.
    </table>
    
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>

    Method generates the eInvalidInput exception when the row argument is more than the number of rows 
    or the column argument is more than the number of columns.
  */
  virtual void setGridVisibility(
    OdDb::Visibility gridVisiblity, 
    OdUInt32 gridlineTypes, 
    OdUInt32 rowTypes);

  /** \details
    Sets the grid visibility for the specified cell and edges in this table entity (DXF 284-289).
       
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param edgeTypes [in]  Edge types.
    \param gridVisibility [in]  Grid visibility. 

    \remarks
    gridVisibility must be one of the following:
    
    <table>
    Name                    Value
    OdDb::kInvisible        1
    OdDb::kVisible          0 
    </table>
        
    edgeTypes must be a combination of one or more of the following:

    <table>
    Name                    Value   Description
    OdDb::kTopMask          1       Top-edge index of the table cell.
    OdDb::kRightMask        2       Right-edge index of the table cell.
    OdDb::kBottomMask       4       Bottom-edge index of the table cell.
    OdDb::kLeftMask         8       Left-edge index of the table cell.
    </table>

    Method generates the eInvalidInput exception when the row argument is more than the number of rows 
    or the column argument is more than the number of columns.
*/
  virtual void setGridVisibility(
    OdUInt32 row, 
    OdUInt32 column, 
    OdInt16 edgeTypes,
    OdDb::Visibility gridVisibility);

  /** \details
    Returns the table style overrides for this table entity. 

    \param overrides [out]  Receives an array of table style overrides for this table entity.         
    
    \returns
    Returns true only if successful.
  */
  virtual bool tableStyleOverrides( 
    OdUInt32Array& overrides) const;

  /** \details
    Clears the table style overrides for this table entity and/or its cells. 

    \param option [in]  Option.
        
    \remarks
    option can be one of the following:
    
    <table>
    Value   Description
    0       Clears all overrides.         
    1       Clears all table overrides.   
    2       Clears all cell overrides.    
    </table>
  */
  virtual void clearTableStyleOverrides(
    int option = 0);
   
  /** \details
    Returns the cell type of the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.

    \remarks
    cellType() returns one of the following:
    
    <table>
    Name               Value    Description
    OdDb::kTextCell    1        Text cell type
    OdDb::kBlockCell   2        Block cell type
    </table>
  */
  virtual OdDb::CellType cellType(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the cell type for the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param cellType [in]  Cell type.
    
    \remarks
    cellType must be one of the following:
    
    <table>
    Name               Value    Description
    OdDb::kTextCell    1        Text cell type
    OdDb::kBlockCell   2        Block cell type
    </table>

    Method generates the eInvalidInput exception when the text cell type number is out of the range 1 to 2, 
    the row argument is more than the number of rows, or the column argument is more than the number of columns.
  */
  virtual void setCellType(
    OdUInt32 row, 
    OdUInt32 column, 
    OdDb::CellType cellType);

  /** \details
    Returns the cell extents for the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param isOuterCell [in]  If and only if true, ignores margins.
    \param pts [out]  Receives the cell extents information.
    
    \remarks
    If isOuterCell is true, this function returns the extents of the cell without regard to margins.
    
    If isOuterCell is false, this function returns the extents of cell reduced 
    by the horizontal and vertical cell margins.

    Method generates the eInvalidInput exception when the row argument is more than number of rows 
    or the column argument is more than number of columns.
  */
  virtual void getCellExtents(
    OdUInt32 row, 
    OdUInt32 column,
    bool isOuterCell, 
    OdGePoint3dArray& pts) const;

  /** \details
    Returns the attachment point of the specified cell in this table entity.
    
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than number of rows 
    or the column argument is more than number of columns.
  */
  virtual OdGePoint3d attachmentPoint(
    OdUInt32 row, 
    OdUInt32 column) const; 

 
  /** \details
    Returns the cell style overrides for the specified cell in this table entity.
      
    \param row [in]  Row index of the 
    \param column [in]  Column index of the cell.
    \param overrides [out]  Receives the overrides.   
  */
  virtual bool cellStyleOverrides(
    OdUInt32 row, 
    OdUInt32 column,
    OdUInt32Array& overrides) const;

  /** \details
    Clears the cell overrides for the specified cell in this table entity. 

    \param row [in]  Row index of the 
    \param column [in]  Column index of the cell.
  */
  void clearCellOverrides(
    OdUInt32 row, 
    OdUInt32 column);

  /** \details
    Deletes the content of the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than number of rows 
    or the column argument is more than the number of columns.
  */
  virtual void deleteCellContent(
    OdUInt32 row, 
    OdUInt32 column);

  /** \details
    Returns the type of the specified row in this table entity.
    
    \param row [in]  Row index.
  
    \remarks
    rowType() returns one of the following:

    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual OdDb::RowType rowType(
    OdUInt32 row) const;

  /** \details
    Returns the text string in the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual OdString textString(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the text string for the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param textString [in]  Text string.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows 
    or the column argument is more than the number of columns.
  */
  virtual void setTextString(
    OdUInt32 row, 
    OdUInt32 column, 
    const OdString& textString);
    
  /** \details
    Returns the Object ID of the OdDbField in the specified cell in this table entity. 
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual OdDbObjectId fieldId(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the Object ID for OdDbField in the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param fieldId [in]  Object ID of the AdDbField.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number 
    of rows, the column argument is more than the number of columns, or the cell type is not OdDb::kTextCell.
  */
  virtual void setFieldId(
    OdUInt32 row, 
    OdUInt32 column, 
    const OdDbObjectId& fieldId);


  /** \details
    Returns the text rotation angle for the specified cell in this table entity. 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.

    \remarks
    textRotation() returns one of the following:
    
    <table>
    Name                    Value     Description
    OdDb::kDegrees000       0         0°
    OdDb::kDegrees090       1         90° CCW
    OdDb::kDegrees180       2         180°
    OdDb::kDegrees270       3         90° CW
    </table>
  */
  virtual OdDb::RotationAngle textRotation(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the text rotation angle of the text in the specified cell in this table entity. 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param textRotation [in]  Text rotation angle.
    
    \remarks
    textRotation must be one of the following:
    
    <table>
    Name                    Value     Description
    OdDb::kDegrees000       0         0°
    OdDb::kDegrees090       1         90° CCW
    OdDb::kDegrees180       2         180°
    OdDb::kDegrees270       3         90° CW
    </table>

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows, 
    the column argument s more than the number of columns, or the cell type is not OdDb::kTextCell.
  */
  virtual void setTextRotation(
    OdUInt32 row, 
    OdUInt32 column, 
    OdDb::RotationAngle textRotation);
  

  /** \details
    Returns true if and only if the block in the specified cell in this table entity is
    automatically scaled and positioned to fit into the cell.
     
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual bool isAutoScale(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Determines if the block in the specified cell in this table entity is to be
    automatically scaled and positioned to fit into the cell.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param autoScale [in]  True to autoscale the block to the cell.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows, 
    the column argument is more than the number of columns, or the cell type is not OdDb::kBlockCell.
  */
  virtual void setAutoScale(
    OdUInt32 row, 
    OdUInt32 column, 
    bool autoScale);
 
  /** \details
    Returns the Object ID of the block table record in the specified cell in this table entity. 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual OdDbObjectId blockTableRecordId(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the Object ID of the block table record in the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param blockId [in]  Object ID of the block.
    \param autoScale [in]  If true, autoscales the block to the cell.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows, 
    the column argument is more than the number of columns, the cell type is not OdDb::kBlockCell, or 
	the object ID is invalid or kNull.
  */
  virtual void setBlockTableRecordId(
    OdUInt32 row, 
    OdUInt32 column,
    const OdDbObjectId& blockId, 
    bool autoScale = false);

  /** \details
    Returns the scale factor of the block reference in the specified cell in this table entity. 

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual double blockScale(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the scale factor of the block reference in the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param blockScale [in]  Uniform scale factor.
    
    \remarks
    blockScale cannot be zero.

    Method generates the eInvalidInput exception when the row argument is more than the number of rows,  
    the column argument is more than the number of columns, or the cell type is not OdDb::kBlockCell.
  */
  virtual void setBlockScale(
    OdUInt32 row, 
    OdUInt32 column, 
    double blockScale);

  /** \details
    Returns the rotation angle of the block reference in the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
  */
  virtual double blockRotation(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Sets the rotation angle of the block reference in the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param blockRotation [in]  Rotation angle.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows,  
    the column argument is more than the number of columns, or the cell type is not OdDb::kBlockCell.
  */
  virtual void setBlockRotation(
    OdUInt32 row, 
    OdUInt32 column, 
    double blockRotation);

  /** \details
    Gets the attribute value for the specified Object ID OdDb::key for the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param attdefId [in]  Object ID of the OdDbAttributeDefinition.
    \param attValue [out]  Receives the attribute value.
  */
  virtual void getBlockAttributeValue(
    OdUInt32 row, 
    OdUInt32 column, 
    const OdDbObjectId& attdefId, 
    OdString& attValue) const;

  /** \details
    Sets the attribute value for the specified Object ID OdDb::key for the specified cell in this table entity.

    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param attdefId [in]  Object ID of the OdDbAttributeDefinition.
    \param attValue [out]  Sets the attribute value.

    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number of rows, 
    the column argument is more than the number of columns, or the cell type is not OdDb::kBlockCell.
  */
  virtual void setBlockAttributeValue(
    OdUInt32 row, 
    OdUInt32 column,
    const OdDbObjectId& attdefId, 
    const OdString& attValue);

  /** \details
    Sets the grid color for the specified gridline types and row type in this table entity (DXF 63,64,65,66,68,69).
      
    \param color [in]  Grid color.
    \param gridlineTypes [in]  Gridline types.
    \param rowTypes [in]  Row types.

    \remarks
    gridlineTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kHorzTop          1       Top or bottom horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    OdDb::kHorzInside       2       All horizontal grid lines, excluding the bottom and top lines.
    OdDb::kHorzBottom       4       Bottom or top horizontal table's grid line, depending on the flow direction of the table rows (down or up).
    </table>
    
    rowTypes must be a combination of one or more of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
    
    Method generates the eInvalidInput exception when the row argument is more than the number of rows or the 
    column argument is more than the number of columns.
  */
  virtual void setGridColor(
    const OdCmColor& color, 
    OdUInt32 gridlineTypes, 
    OdUInt32 rowTypes);
    
  /** \details
    Sets the grid color for the specified cell and edges in this table entity (DXF 63,64,65,66,68,69).
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param edgeTypes [in]  Edge types.
    \param color [in]  Grid color.

    \remarks
    edgeTypes must be a combination of one or more of the following:

    <table>
    Name                    Value   Description
    OdDb::kTopMask          1       Top-edge index of the table cell.
    OdDb::kRightMask        2       Right-edge index of the table cell.
    OdDb::kBottomMask       4       Bottom-edge index of the table cell.
    OdDb::kLeftMask         8       Left-edge index of the table cell.
    </table>
    
    Method generates the eInvalidInput exception when the row argument is more than the number of rows or the 
    column argument is more than the number of columns.
  */
  virtual void setGridColor(
    OdUInt32 row, 
    OdUInt32 column, 
    OdInt16 edgeTypes,
    const OdCmColor& color);
  
  /** \details
    Inserts the specified number of columns into this table entity at the specified column index.
    
    \param column [in]  Column index.
    \param width [in]  Width of the inserted columns.
    \param numColumns [in]  Number of columns to insert.

    \remarks
    Method generates the eInvalidInput exception when the column argument is more than the number 
    of columns or the width is zero.
  */
  virtual void insertColumns(
    OdUInt32 column, 
    double width, 
    OdUInt32 numColumns = 1);

  /** \details
    Deletes the specified number of columns from this table entity.

    \param column [in]  Index of first column to delete.
    \param numColumns [in]  Number of columns to delete. 

    \remarks
    Method generates the eInvalidInput exception when the number of deleted columns is more 
    than the number of columns.
  */
  virtual void deleteColumns(
    OdUInt32 column, 
    OdUInt32 numColumns = 1);

  /** \details
    Inserts the specified number of rows into this table entity at the specified row index.

    \param row [in]  Row index.
    \param height [in]  Height of the inserted rows.
    \param numRows [in]  Number of rows to insert.

    \remarks
    Method generates the eInvalidInput exception when the height is zero.
  */
    virtual void insertRows(
    OdUInt32 row, 
    double height, 
    OdUInt32 numRows = 1);
  
  /** \details
    Deletes the specified number of rows from this table entity.

    \param row [in]  Index of first row to delete.
    \param numRows [in]  Number of rows to delete. 

    \remarks
    Method generates the eInvalidInput exception when the number of deleted rows is more 
    than the number of rows.
  */
  virtual void deleteRows(
    OdUInt32 row, 
    OdUInt32 numRows = 1);
 
  /** \details
    Merges a rectangular region of cells in this table entity.
      
    \param minRow [in]  Minimum row index of the merged cells.
    \param maxRow [in]  Maximum row index of the merged cells. 
    \param minColumn [in]  Minimum column index of the merged cells.
    \param maxColumn [in]  Maximum column index of the merged cells. 

    \remarks
    Method generates the eInvalidInput exception when the minimum row index is more than the maximum 
    row index, the minimum column index is more than the maximum column index, the maximum row index 
    is more than the number of rows, or the maximum column index is more than the number of columns.
  */
    virtual void mergeCells(
    OdUInt32 minRow, 
    OdUInt32 maxRow,
    OdUInt32 minColumn, 
    OdUInt32 maxColumn);

  /** \details
    Unmerges a rectangular region of cells in this table entity.
      
    \param minRow [in]  Minimum row index of the merged cells.
    \param maxRow [in]  Maximum row index of the merged cells. 
    \param minColumn [in]  Minimum column index of the merged cells.
    \param maxColumn [in]  Maximum column index of the merged cells. 

    \remarks
    Method generates the eInvalidInput exception when the minimum row index is more than the maximum 
    row index, the minimum column index is more than the maximum column index, the maximum row index 
    is more than the number of rows, or the maximum column index is more than the number of columns.
  */
    virtual void unmergeCells(
    OdUInt32 minRow, 
    OdUInt32 maxRow,
    OdUInt32 minColumn, 
    OdUInt32 maxColumn);
    
  /** \details
    Returns true if and only if the specified cell has been merged, and returns the range of the merged cells 
    in this table entity.
    
    \param row [in]  Row index.
    \param column [in]  Column index.
    \param minRow [out]  Receives the minimum row index of the merged cells.
    \param maxRow [out]  Receives the maximum row index of the merged cells. 
    \param minColumn [out]  Receives the minimum column index of the merged cells.
    \param maxColumn [out]  Receives the maximum column index of the merged cells. 
        
    \remarks
    Method generates the eInvalidInput exception when the row argument is more than the number
    of rows or the column argument is more than the number of columns.
  */
  virtual bool isMergedCell(
    OdUInt32 row, 
    OdUInt32 column, 
    OdUInt32* minRow = 0, 
    OdUInt32* maxRow = 0,
    OdUInt32* minColumn = 0, 
    OdUInt32* maxColumn = 0) const;

  /** \details
    Returns cell property "merged height". The first cell may have a value > 1. Next cells have a value equal 1.

    \param row [in]  Row index.
    \param column [in]  Column index.
  */
  OdUInt32 mergedHeight(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Returns cell property "merged width". The first cell may have a value > 1. Next cells have a value equal 1.

    \param row [in]  Row index.
    \param column [in]  Column index.
  */
  OdUInt32 mergedWidth(
    OdUInt32 row, 
    OdUInt32 column) const;

  /** \details
    Returns cell property "merged flag". The first cell has a value equal false. Next cells have a value equal to true.

    \param row [in]  Row index.
    \param column [in]  Column index.
  */
  bool mergedFlag(
    OdUInt32 row, 
    OdUInt32 column) const;
  
  /** \details
    Updates this table entity according to its current table style.
    
    \returns
    Returns eOk if successful or an appropriate error code if not.
  */
  virtual OdResult generateLayout();

  /** \details
    Updates the block table record referenced by this table entity.
    
    \param forceUpdate [in]  Force an update of the block table record.
    
    \returns
    Returns eOk if successful or an appropriate error code if not.
    
    If forceUpdate is false, the block table record is updated 
    if and only if this table entity has been changed since the 
    block table record was last updated.
    
    If forceUpdate is true, the block table will be unconditionally updated.
  */
  virtual OdResult recomputeTableBlock(
    bool forceUpdate = true);
 
  //********************************************************************
  // Methods for sub-selection  
  //********************************************************************
  //
  
  /** \details
    Performs a hit test for the specified point and viewing direction in this table entity. Returns 
    the row index and the column index of the cell, hit by the ray in the output arguments. 
    Returns true if a table set is hit or returns false in the other case.
    
    \param wpt [in]  Input 3D picking point in WCS.
    \param wviewVec [in]  3D vector in WCF that specifies the view direction for the hit test.
    \param wxaper [in]  Width of aperture box centered at the hit point.
    \param wyaper [in]  Height of aperture box centered at the hit point.
    \param resultRowIndex [out]  Row index.
    \param resultColumnIndex [out]  Column index.
  */
  virtual bool  hitTest(const OdGePoint3d& wpt, 
                                    const OdGeVector3d& wviewVec,
                                    double wxaper,
                                    double wyaper,
                                    OdInt32& resultRowIndex, 
                                    OdInt32& resultColumnIndex) const;
  /** \details
    Performs a hit test for the specified point and viewing direction in this table entity. Returns 
    the row index and the column index of the cell, hit by the ray in the output arguments. 
    Returns true if a table set is hit or returns false in the other case.
    
    \param wpt [in]  Input 3D picking point in WCS.
    \param wviewVec [in]  3D vector in WCS that specifies the view direction for the hit test.
    \param wxaper [in]  Width of aperture box centered at the hit point.
    \param wyaper [in]  Height of aperture box centered at the hit point.
    \param resultRowIndex [out]  Row index.
    \param resultColumnIndex [out]  Column index.
    \param contentIndex [out]  Context index.
    \param nItem [out]  Hit flags, which indicate the table item at the specified point.
  */
  virtual bool  hitTest(const OdGePoint3d& wpt, 
                                    const OdGeVector3d& wviewVec,
                                    double wxaper,
                                    double wyaper,
                                    OdInt32& resultRowIndex, 
                                    OdInt32& resultColumnIndex,
                                    OdInt32& contentIndex,
                                    OdDb::TableHitItem& nItem) const;
  /** \details
    Selects a cell in this table by the specified point, viewing direction, and orientation.
    Returns the row index and the column index of the selected cell that encloses the input point.
       
    \param wpt [in]  Input 3D picking point in WCS.
    \param wvwVec [in]  3D vector in WCS that specifies the view direction for the hit test.
    \param wvwxVec [in]  3D vector in WCS that specifies the view orientation for the hit test.
    \param wxaper [in]  Width of aperture box centered at the hit point.
    \param wyaper [in]  Height of aperture box centered at the hit point.
    \param allowOutside [in]  Indicates whether a pick point outside the table will select a cell.
    \param bInPickFirst [in]  If true, the entity is already in the pickfirst set; if false, the pickfirst logic should attempt to sub-select the entity directly.
    \param resultRowIndex [out]  Row index.
    \param resultColumnIndex [out]  Column index.
    \param pPaths [out]  Pointer to an OdDbFullSubentPathArray.
    
    \returns
    Returns eOk if successful or an appropriate error code if not.
    
    If pPaths is not null, the cell subentities will be returned in pPaths.
  */
  virtual OdResult select(const OdGePoint3d& wpt, 
                                   const OdGeVector3d& wvwVec, 
                                   const OdGeVector3d& wvwxVec, 
                                   double wxaper,
                                   double wyaper,
                                   bool allowOutside,
                                   bool bInPickFirst, 
                                   OdInt32& resultRowIndex, 
                                   OdInt32& resultColumnIndex,
                                   OdDbFullSubentPathArray* pPaths = 0) const;
  /** \details
    Selects a set of cells in this table by the specified window box, viewing direction, and orientation.
    Returns the set of cells in the output arguments rowMin, rowMax, colMin, colMax.
    
    
    \param wpt1 [in]  3D picking point in WCS that specifies the first corner point of the window box selection.
    \param wpt2 [in]  3D picking point in WCS that specifies the second corner point of the window box selection.
    \param wvwVec [in]  3D vector in WCS that specifies the view direction for the selection.
    \param wvwxVec [in]  3D vector in WCS that specifies the view orientation for the hit test.
    \param wxaper [in]  Width of aperture box centered at the hit point.
    \param wyaper [in]  Height of aperture box centered at the hit point.
    \param seltype [in]  Selection type.
    \param bIncludeCurrentSelection [in]  Indicates whether the selected cells will include currently selected cells and newly selected cells or only newly selected cells.
    \param bInPickFirst [in]  If true, the entity is already in the pickfirst set; if false, the pickfirst logic should attempt to sub-select the entity directly.
    \param rowMin [out]  Lower bound of row index.
    \param rowMax [out]  Upper bound of row index.
    \param colMin [out]  Lower bound of column index.
    \param colMax [out]  Upper bound of column index.
    \param pPaths [out]  Pointer to an OdDbFullSubentPathArray.
    
    \returns
    Returns eOk if successful or an appropriate error code if not.
    
    If pPaths is not null, the cell sub-entities will be returned in pPaths.
  */
  virtual OdResult selectSubRegion(const OdGePoint3d& wpt1, 
                                   const OdGePoint3d& wpt2,
                                   const OdGeVector3d& wvwVec, 
                                   const OdGeVector3d& wvwxVec,
                                   double wxaper,
                                   double wyaper,                             
                                   OdDb::SelectType seltype,
                                   bool bIncludeCurrentSelection,
                                   bool bInPickFirst,                             
                                   OdInt32& rowMin,
                                   OdInt32& rowMax,
                                   OdInt32& colMin,
                                   OdInt32& colMax,
                                   OdDbFullSubentPathArray* pPaths = 0) const;
  /** \details
    Returns an array of sub-entities of the current sub-selection cells in the output argument paths.
    
    \param pPaths [out]  Pointer to an OdDbFullSubentPathArray.
    
    \returns
    Returns true if successful or returns false if not.
    
    If pPaths is empty, there are no sub-selection cells.
  */
  virtual bool reselectSubRegion(OdDbFullSubentPathArray& paths) const;
          
  /** \details
    Returns the row and column indexes of the cells in the sub-selection set.
    
    \param rowMin [out]  Lower bound of row index.
    \param rowMax [out]  Upper bound of row index.
    \param colMin [out]  Lower bound of column index.
    \param colMax [out]  Upper bound of column index.
    
    \returns
    Returns eOk if successful, eSubSelectionSetEmpty if there are no sub-selection cells, or an appropriate error code if not.
  */        
  virtual OdResult getSubSelection(OdInt32& rowMin,
                                    OdInt32& rowMax,
                                    OdInt32& colMin,
                                    OdInt32& colMax) const;

  /** \details
    Returns the range of cells in the sub-selection set.
    
    \remarks
    If there are no sub-selection cells, returns invalid range.
  */ 
  OdCellRange  getSubSelection (void) const;

  /** \details
    Sets the cell range to the sub-selection set.
    
    \param range [in]  Cell range.
    
    \returns
    Returns eOk if successful or appropriate error code in the other case.
  */  
  virtual OdResult setSubSelection  (const OdCellRange& range);

  /** \details
    Sets the row and column indexes of the cells in the sub-selection set.
    
    \param rowMin [in]  Lower bound of row index.
    \param rowMax [in]  Upper bound of row index.
    \param colMin [in]  Lower bound of column index.
    \param colMax [in]  Upper bound of column index.
    
    \returns
    Returns eOk if successful or eInvalidInput if the input indexes are out of range.
  */
  virtual OdResult setSubSelection(OdInt32 rowMin,
                                    OdInt32 rowMax,
                                    OdInt32 colMin,
                                    OdInt32 colMax);
  /** \details
    Clears the sub-selection set of cells from this table.
  */
  virtual void clearSubSelection();

  /** \details
    Returns true if the table has a sub-selection set of cells, or returns false in the other case.
  */
  virtual bool hasSubSelection() const;
  
  virtual OdResult select_next_cell(OdInt32 dir,
                                    OdInt32& resultRowIndex, 
                                    OdInt32& resultColumnIndex,
                                    OdDbFullSubentPathArray* pPaths = NULL,
                                    bool bSupportTextCellOnly = true) const;

  //********************************************************************
  // Overridden methods from OdDbObject
  //********************************************************************
  //

  virtual OdResult dwgInFields (
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields (
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  // virtual OdResult audit(OdDbAuditInfo* pAuditInfo);

  void subClose();
  
  /** \remarks
    This function is an override for OdDbEntity::subSetDatabaseDefaults() to set 
    the dimension style of this entity to the current style for the specified database.
  */
  void subSetDatabaseDefaults(
    OdDbDatabase *pDb,
    bool doSubents);

  // virtual void              objectClosed(const OdDbObjectId objId);
  // virtual void              erased(const OdDbObject* dbObj,
  //                                  bool pErasing = true);
  virtual void modified(
    const OdDbObject* pObj);

  virtual OdResult subGetClassID(
    void* pClsid) const;

  //********************************************************************
  // Overridden methods from OdDbEntity
  //********************************************************************
  //
  /*
  virtual void              list() const;
  */

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  /*
  virtual OdResult getGripPoints(OdGePoint3dArray&, 
                                          OdDbIntArray&,
                                          OdDbIntArray&) const;

  virtual OdResult moveGripPointsAt(const OdDbIntArray&,
                                             const OdGeVector3d&);

  virtual OdResult getStretchPoints(OdGePoint3dArray& stretchPoints)
                                             const;

  virtual OdResult moveStretchPointsAt(const OdDbIntArray& 
                                               indices,
                                               const OdGeVector3d& offset);

  virtual OdResult getOsnapPoints(OdDb::OsnapMode osnapMode,
                                           OdGsMarker   gsSelectionMark,
                                           const OdGePoint3d&  pickPoint,
                                           const OdGePoint3d&  lastPoint,
                                           const OdGeMatrix3d& viewXform,
                                           OdGePoint3dArray&      snapPoints,
                                           OdDbIntArray&   geomIds)
                                           const;
  */

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xfm,
    OdDbEntityPtr& pCopy) const;

  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  /*
  virtual OdResult explode(OdDbVoidPtrArray& entitySet) const;
  */

  //********************************************************************
  // Overridden methods from OdDbBlockReference required for OdDbTable
  //********************************************************************
  //
  //OdGePoint3d            position() const;
  // virtual OdResult setPosition(const OdGePoint3d&);

  //OdGeVector3d           normal() const;
  // virtual OdResult setNormal(const OdGeVector3d& newVal);

  //********************************************************************
  // Methods for internal use only
  //********************************************************************
  //
  // TODO: Temporary method for navigating after editing cells
  /*
  virtual OdResult select_next_cell(int dir,
                                   int& resultRowIndex, 
                                   int& resultColumnIndex,
                                   OdDbFullSubentPathArray* pPaths = 0,
                                   bool bSupportTextCellOnly = true) const;

  virtual void              setRegen();
  virtual void              suppressInvisibleGrid(bool value);
  virtual OdResult getCellExtents(int row, 
                                       int column,
                                       double& cellWidth,
                                       double& cellHeight,
                                       bool bAdjustForMargins) const;
  */
  
  // NEW 2007
  /** \details
    Returns the data type and unit type of the specified row type. 
    
    \param nDataType [out]  Data type.
    \param nUnitType [out]  Unit type.
    \param type [in]  Row type.

    \remarks  
    type must be one of the following:
    
    <table>
    Name                    Value   Description
    OdDb::kTitleRow         1       The top-most or bottom-most row of the table, depending on the flow direction of the table rows (down or up).
    OdDb::kHeaderRow        2       The first row, which is placed after or before the title row.
    OdDb::kDataRow          4       The row, which is neither title row nor header row.  
    </table>
  */
  virtual void getDataType(OdValue::DataType& nDataType,
    OdValue::UnitType& nUnitType,
    OdDb::RowType type) const;
  
  /** \details
    Sets the data type and unit type for all row types. 
    
    \param nDataType [in]  Data type.
    \param nUnitType [in]  Unit type.
  */
  virtual void setDataType(OdValue::DataType nDataType,
    OdValue::UnitType nUnitType);

  /** \details
    Sets the data type and unit type for the specified row types. 
    
    \param nDataType [in]  Data type.
    \param nUnitType [in]  Unit type.
    \param nRowTypes [in]  Row types.
  */
  virtual void setDataType(OdValue::DataType nDataType, 
    OdValue::UnitType nUnitType, 
    int nRowTypes);

  virtual OdString format(OdDb::RowType type) const;

  virtual void setFormat(const OdString& pszFormat);

  virtual void setFormat(const OdString& pszFormat, 
    int nRowTypes);

  /** \details
    Returns the data type and unit type of the specified cell. 
    
    \param row [in]  Row index. Should be more than or equal to 0 and less than the number of rows.
    \param col [in]  Column index. Should be more than or equal to 0 and less than the number of columns.
    \param nDataType [out]  Data type.
    \param nUnitType [out]  Unit type.
  */
  void getDataType(OdUInt32 row, OdUInt32 col,
    OdValue::DataType& nDataType,
    OdValue::UnitType& nUnitType) const;

  /** \details
    Sets the data type and unit type of the specified cell. 
    
    \param row [in]  Row index. Should be more than or equal to 0 and less than the number of rows.
    \param col [in]  Column index. Should be more than or equal to 0 and less than the number of columns.
    \param nDataType [in]  Data type.
    \param nUnitType [in]  Unit type.
  */
  void setDataType(OdUInt32 row, OdUInt32 col, 
    OdValue::DataType nDataType,
    OdValue::UnitType nUnitType);
  
  /** \details
    Returns the content value at the specified content index. 
    
    \param row [in]  Row index. Should be more than or equal to 0 and less than the number of rows.
    \param col [in]  Column index. Should be more than or equal to 0 and less than the number of columns.
  */
  OdValue value(OdUInt32 row, OdUInt32 col) const;

  /** \details
    Sets the value of the first content of a cell. 
    
    \param row [in]  Row index. Should be more than or equal to 0 and less than the number of rows.
    \param col [in]  Column index. Should be more than or equal to 0 and less than the number of columns.
    \param val [in]  Content value.
    
    \returns
    Returns eOk if successful or an appropriate error code.
  */
  void setValue(OdUInt32 row, OdUInt32 col, 
    const OdValue& val);

  /** \details
    Sets the value of the first content of a cell. 
    
    \param row [in]  Row index. Should be more than or equal to 0 and less than the number of rows.
    \param col [in]  Column index. Should be more than or equal to 0 and less than the number of columns.
    \param pszText [in]  Text for converting to set value.
    \param nOption [out]  Parse option.
    
    \returns
    Returns eOk if successful or an appropriate error code.
  */
  void setValue(OdUInt32 row, OdUInt32 col, 
    OdString& pszText,
    OdValue::ParseOption nOption);
  
  /** \details
    Resets the value in a specified cell. 
    
    \param row [in]  Row index.
    \param col [in]  Column index.
    
    \returns
    Returns eOk if successful, or returns eInvalidInput if row and col arguments refer to an invalid cell.
  */
  void resetValue(OdUInt32 row, OdUInt32 col);

  OdString format(OdUInt32 row, OdUInt32 col) const;

  void setFormat(OdUInt32 row, OdUInt32 col, 
    const OdString& pszFormat);

  /** \details
    Returns true if the table break is enabled, or false otherwise.
  */
  bool isBreakEnabled(void) const;
  
  /** \details
    Enables or disables table breaking.

    \param bEnable [in] Value for enabling or disabling the table breaking.
  */
  void enableBreak (bool bEnable);
  
  /** \details
    Returns table break flow direction of this table entity as an OdDb::TableBreakFlowDirection object.
  */
  OdDb::TableBreakFlowDirection breakFlowDirection(void) const;

  /** \details
    Sets table break flow direction of this table entity.

    \param flowDir [in] Table break flow direction.
  */
  void setBreakFlowDirection (OdDb::TableBreakFlowDirection flowDir);

   
  /** \details
    Returns the table break height of the specified table entity as an OdUInt32 value.

    \param index [in] Sub-table index.
    
    \remarks
    index parameter should be more than or equal to 0. 

  */
  double breakHeight (OdUInt32 index) const;
  
  /** \details
    Sets the table break height of the specified table entity.

    \param index [in] Sub-table index.
    \param height [in] Sub-table break height.
    
    \remarks
    index parameter should be more than or equal to 0. 

  */
  void setBreakHeight (OdUInt32 index, 
    double height);

  /** \details
    Returns table break offset of the specified table entity as an OdUInt32 value.

    \param index [in] Sub-table index.
    
    \remarks
    index parameter should be more than or equal to 1. 
  */
  OdGeVector3d breakOffset (OdUInt32 index) const;

  /** \details
    Sets the table break offset of the specified table entity.

    \param index [in] Sun-table index.
    \param vec [in] Vector for table break offset.
    
    \remarks
    index parameter should be more than or equal to 1. 
  */
  void setBreakOffset (OdUInt32 index, 
    const OdGeVector3d& vec);

  /** \details
    Returns the table break option of this table entity as an OdDb::TableBreakOption object.
    
    \remarks
    breakOption() returns one of the following:
    
    <table>
    Name                                    Value       Description
    OdDb::kTableBreakNone                   0           No table break.
    OdDb::kTableBreakEnableBreaking         0x01        Enable table breaking. 
    OdDb::kTableBreakRepeatTopLabels        0x02        Repeat top labels in all tables.  
    OdDb::kTableBreakRepeatBottomLabels     0x04        Repeat bottom labels in all tables.
    OdDb::kTableBreakAllowManualPositions   0x08        Allow manual position for each sub-table. 
    OdDb::kTableBreakAllowManualHeights     0x10        Allow manual height for each sub-table.  
    </table>
  */
  OdDb::TableBreakOption breakOption (void) const;
 
  /** \details
    Sets the table break option of this table entity.

    \param option [in] Table break option.
    
    \remarks
    option should be a combination of one or more of the following:
    
    <table>
    Name                                    Value       Description
    OdDb::kTableBreakNone                   0           No table break.
    OdDb::kTableBreakEnableBreaking         0x01        Enable table breaking. 
    OdDb::kTableBreakRepeatTopLabels        0x02        Repeat top labels in all tables.  
    OdDb::kTableBreakRepeatBottomLabels     0x04        Repeat bottom labels in all tables.
    OdDb::kTableBreakAllowManualPositions   0x08        Allow manual position for each sub-table. 
    OdDb::kTableBreakAllowManualHeights     0x10        Allow manual height for each sub-table.  
    </table>
  */
  void  setBreakOption (OdDb::TableBreakOption option);

  /** \details
    Returns the break spacing for this table entity.
  */
  double breakSpacing (void) const;

  /** \details
    Sets the break spacing for this table entity.

    \param spacing [in]  Spacing value. 

    \remarks
    Method generates the eInvalidInput exception when the spacing is negative.
  */
  void setBreakSpacing (double spacing);

  //********************************************************************
  // New Methods
  //********************************************************************

  /** \details
    Sets the size for this table entity.

    \param rows [in]  Number of rows. 
    \param cols [in]  Number of columns. 

    \remarks
    Method generates the eInvalidInput exception when the number of rows or columns is zero.
  */
  void setSize ( 
    OdInt32 rows, 
    OdInt32 cols);

  /** \details
    Checks if new rows or columns can be inserted into the specified index. Returns true if rows or columns can be inserted.

    \param nIndex [in]  Index. 
    \param bRow [in]  Flag that indicates what to check: "true" for rows or "false" for columns. 

    \remarks
    Method is not implemented yet and always returns true.
  */
  bool canInsert (
    OdInt32 nIndex, 
    bool bRow) const;

  /** \details
    Inserts rows at the specified index.

    \param nIndex [in]  Index. 
    \param nInheritFrom [in]  Row number in the table that the format will be inherited from. 
    \param nNumRows [in]  Number of inserted rows. 
  */
  void insertRowsAndInherit  (
    OdInt32 nIndex, 
    OdInt32 nInheritFrom, 
    OdInt32 nNumRows);

  /** \details
    Inserts columns at the specified index.

    \param col [in]  Index. 
    \param nInheritFrom [in]  Column number in the table that the format will be inherited from. 
    \param nNumCols [in]  Number of inserted columns. 

    \remarks
    Method generates the eInvalidInput exception when the number of rows or columns is zero.
  */
  void insertColumnsAndInherit(
    OdInt32 col, 
    OdInt32 nInheritFrom, 
    OdInt32 nNumCols);

  /** \details
    Checks if rows or columns can be deleted from the specified index. Returns true if rows or columns can be deleted.
 
    \param nCount [in]  Number of rows or columns. 
    \param bRow [in]  Flag that indicates what to check: "true"" for rows or "false" for columns. 

    \remarks
    Method is not implemented yet and always returns true.
  */
  bool canDelete (OdInt32 nIndex, 
    OdInt32 nCount, 
    bool bRow) const;

  /** \details
    Returns "empty flag" cell property. Returns true if cell is empty, or false if it is not.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  bool isEmpty (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Returns merge range of the cell as an object of the OdCellRange class.
    Returns invalid merge range if the cell is not part of the merge range.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
  */
  OdCellRange getMergeRange (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Returns "editing content flag" cell property. Returns true if cell content can be edited or false if it can not.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
  */
  bool isContentEditable (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Returns "editing format flag" cell property. Returns true if cell format can be edited or false if it can not.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
  */
  bool isFormatEditable (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Returns cell state.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Cell state is an enumerator and takes one of the following values:
    
    <table>
    Name                                  Value
    kCellStateNone                        0x00
    kCellStateContentLocked               0x01
    kCellStateContentReadOnly             0x02
    kCellStateLinked                      0x04
    kCellStateContentModifiedAfterUpdate  0x08
    kCellStateFormatLocked                0x10
    kCellStateFormatReadOnly              0x20
    kCellStateFormatModifiedAfterUpdate   0x40
    kAllCellStates                        (kCellStateContentLocked | kCellStateContentReadOnly | kCellStateLinked | kCellStateContentModifiedAfterUpdate | kCellStateFormatLocked | kCellStateFormatReadOnly | kCellStateFormatModifiedAfterUpdate)
    </table>
  */
  OdDb::CellState cellState (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Sets the cell state.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nLock [in]  Cell state.

    \remarks
    Cell state should take one of the following values:
    
    <table>
    Name                                  Value
    kCellStateNone                        0x00
    kCellStateContentLocked               0x01
    kCellStateContentReadOnly             0x02
    kCellStateLinked                      0x04
    kCellStateContentModifiedAfterUpdate  0x08
    kCellStateFormatLocked                0x10
    kCellStateFormatReadOnly              0x20
    kCellStateFormatModifiedAfterUpdate   0x40
    kAllCellStates                        (kCellStateContentLocked | kCellStateContentReadOnly | kCellStateLinked | kCellStateContentModifiedAfterUpdate | kCellStateFormatLocked | kCellStateFormatReadOnly | kCellStateFormatModifiedAfterUpdate)
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  setCellState (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::CellState nLock);

  /** \details
    Returns the number of contents in the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  OdInt32 numContents (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Creates additional content for the cell. Returns index of the created content.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nIndex [in]  Index at which the content will be created. 

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  OdInt32 createContent (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nIndex);

  /** \details
    Moves content in the cell from one position to another.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nFromIndex [in]  Index of content to move.
    \param nToIndex [in]  Target index of the content where it should be moved to.

    \remarks
    Method generates the eNotImplementedYet exception.
  */
  void  moveContent (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nFromIndex, 
    OdInt32 nToIndex);

  /** \details
    Deletes content from the cells.
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  deleteContent (
    OdInt32 row, 
    OdInt32 col);

  /** \details
    Deletes content from the cells.
      
    \param row [in]  Row index of the cell.
    \param column [in]  Column index of the cell.
    \param nIndex [in]  Index of content.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
    */
  void  deleteContent (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nIndex);

  /** \details
    Deletes content from the cells.

    \param range [in]  Range of the cells.

    \remarks
    Method generates the eNotImplementedYet exception.
  */
  void  deleteContent (
    const OdCellRange& range);

  /** \details
    Returns content type of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Content type is an enumerator and takes one of the following values:
    
    <table>
    Name                     Value
    kCellContentTypeUnknown  0x0
    kCellContentTypeValue    0x1
    kCellContentTypeField    0x2
    kCellContentTypeBlock    0x4
    </table>
  */
  OdDb::CellContentType contentType (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Returns content type of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
	\param nIndex [in]  Index of content.

    \remarks
    Content type is an enumerator and takes one of the following values:
    
    <table>
    Name                     Value
    kCellContentTypeUnknown  0x0
    kCellContentTypeValue    0x1
    kCellContentTypeField    0x2
    kCellContentTypeBlock    0x4
    </table>
  */
  OdDb::CellContentType contentType (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nIndex) const;

  /** \details
    Returns the value of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  OdValue value (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Returns the value of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param nOption [in]  Format option of value.

    \remarks
    Format option is an enumerator and takes one of the following values:
    
    <table>
    Name                  Value
    kFormatOptionNone     0x00
    kForEditing           0x01
    kForExpression        0x02
    kUseMaximumPrecision  0x04
    kIgnoreMtextFormat    0x08
    </table>
  */
  OdValue value (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    OdValue::FormatOption nOption) const;

  /** \details
    Sets a value to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param val [in]  Value.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setValue (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdValue& val);

  /** \details
    Sets a value to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param val [in]  Value.
    \param nOption [in]  Parse option of value.

    \remarks
    Parse option is an enumerator and takes one of the following values:
    
    <table>
    Name                    Value
    kParseOptionNone        0x00
    kSetDefaultFormat       0x01
    kPreserveMtextFormat    0x02
    kConvertTextToValue     0x04
    kChangeDataType         0x08
    kParseTextForFieldCode  0x10
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setValue (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdValue& val, 
    OdValue::ParseOption nOption);

  /** \details
    Sets a value to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param sText [in]  Text of the value.
    \param nOption [in]  Parse option of value.

    \remarks
    Parse option is an enumerator and takes one of the following values:
    
    <table>
    Name                    Value
    kParseOptionNone        0x00
    kSetDefaultFormat       0x01
    kPreserveMtextFormat    0x02
    kConvertTextToValue     0x04
    kChangeDataType         0x08
    kParseTextForFieldCode  0x10
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  setValue (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    OdString sText, 
    OdValue::ParseOption nOption);

  /** \details
    Returns data format of the cell as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
  */
  OdString dataFormat (
    OdInt32 row,
    OdInt32 col) const;

  /** \details
    Returns data format of the cell as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  OdString dataFormat (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets the data format of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param sFormat [in]  Data format.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setDataFormat (
    OdInt32 row, 
    OdInt32 col, 
    const OdString& sFormat);

  /** \details
    Sets data format of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param sFormat [in]  Data format.
    \param nContent [in]  Number of content.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setDataFormat (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdString& sFormat);

  /** \details
    Returns text string of the cell as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  OdString textString (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Returns the text string of the cell as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param nOption [in]  Format option of value.

    \remarks
    Format option is an enumerator and takes one of the following values:
    
    <table>
    Name                  Value
    kFormatOptionNone     0x00
    kForEditing           0x01
    kForExpression        0x02
    kUseMaximumPrecision  0x04
    kIgnoreMtextFormat    0x08
    </table>
  */
  OdString textString (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    OdValue::FormatOption nOption) const;

  /** \details
    Returns the text string of the cell as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nOption [in]  Format option of value.

    \remarks
    Format option is an enumerator and takes one of the following values:
    
    <table>
    Name                  Value
    kFormatOptionNone     0x00
    kForEditing           0x01
    kForExpression        0x02
    kUseMaximumPrecision  0x04
    kIgnoreMtextFormat    0x08
    </table>
  */
  OdString textString (
    OdInt32 row, 
    OdInt32 col, 
    OdValue::FormatOption nOption ) const;

  /** \details
    Sets a text string to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param text [in]  Text string.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setTextString (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdString& text);

  /** \details
    Checks if the cell has a formula.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  bool  hasFormula (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Returns the formula of the cell as an object of the OdString class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  OdString getFormula (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets the formula of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param pszFormula [in]  Formula.

    \remarks
    Method generates the eNotImplementedYet exception.
  */
  void  setFormula (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdString& pszFormula);

  /** \details
    Returns the field id of the cell as an object of the OdDbObjectId class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  OdDbObjectId  fieldId (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets a field id to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param fieldId [in]  Field id.
    \param nFlag [in]  Cell option.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setFieldId (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdDbObjectId& fieldId, 
    OdDb::CellOption nFlag);

  /** \details
    Returns the id of the block table record of the cell as an object of the OdDbObjectId class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
  */
  OdDbObjectId  blockTableRecordId (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets the id of the block table record of the cell as an object of the OdDbObjectId class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param blkId [in]  Id of block table record.
    \param autoFit [in]  Flag that indicates whether the block should be auto fit.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setBlockTableRecordId (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdDbObjectId& blkId, 
    bool autoFit);

  /** \details
    Returns the attribute value of the block cell using the specified object id key as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param attdefId [in]  Object id.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  OdString  getBlockAttributeValue (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdDbObjectId& attdefId) const;

  /** \details
    Sets the attribute value of the block cell using the specified object id key as an OdString value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Number of content.
    \param attdefId [in]  Object id.
    \param atrValue [in]  Attribute value.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when content is not editable ( !isContentEditable() ).
  */
  void  setBlockAttributeValue (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdDbObjectId& attdefId, 
    const OdString& atrValue);

  /** \details
    Returns the custom data value of the cell, column, or row as an OdInt32 value.
    Use a valid row index and pass column index -1 to get row data.
    Use a valid column index and pass row index -1 to get column data.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
  */
  OdInt32  getCustomData (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Sets the custom data value of the cell, column, or row.
    Use a valid row index and pass column index -1 to set row data.
    Use a valid column index and pass row index -1 to set column data.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nData [in]  Custom data value. 
  */
  void  setCustomData (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nData);

  /** \details
    Returns the custom data value of the cell, column, or row as an OdInt32 value.
    Use a valid row index and pass column index -1 to get row data.
    Use a valid column index and pass row index -1 to get column data.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param sKey [in]  Key to use for the custom data. 
  */
  OdValue getCustomData (
    OdInt32 row, 
    OdInt32 col, 
    const OdString sKey) const;

  /** \details
    Sets the custom data value of the cell, column, or row.
    Use a valid row index and pass column index -1 to get row data
    Use a valid column index and pass row index -1 to get column data

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param sKey [in]  Key to use for the custom data. 
    \param pData [in]  Custom data value.  
  */
  void  setCustomData (
    OdInt32 row, 
    OdInt32 col, 
    const OdString& sKey, 
    const OdValue* pData);

  /** \details
    Returns the cell style as an OdString value.
    Use a valid row index and pass column index -1 to get style for row.
    Use a valid column index and pass row index -1 to get style for column.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Cell style should be one of the following values:
    
    <table>
    Name
    _TITLE  
    _HEADER
    _DATA
    </table>

  */
  OdString  cellStyle (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Sets the cell style.
    Use a valid row index and pass column index -1 to set style for row.
    Use a valid column index and pass row index -1 to set style for column.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell.   
    \param sCellStyle [in]  Cell style.   

    \remarks
    Cell style should be one of the following values:
    
    <table>
    Name
    _TITLE  
    _HEADER
    _DATA
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  setCellStyle (
    OdInt32 row, 
    OdInt32 col, 
    const OdString& sCellStyle);

  /** \details
    Returns the margin of the cell, row, or column as a double value.
    Use a valid row index and pass column index -1 to get margin for row.
    Use a valid column index and pass row index -1 to get margin for column.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nMargin [in]  Margin type. 

    \remarks
    Margin type should be one of following values:
    
    <table>
    Name                    Value
    kCellMarginTop          0x01
    kCellMarginLeft         0x02
    kCellMarginBottom       0x04
    kCellMarginRight        0x08
    kCellMarginHorzSpacing  0x10
    kCellMarginVertSpacing  0x20
    </table>
  */
  double margin (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::CellMargin nMargin) const;

  /** \details
    Sets the margin to the cell, row, or column.
    Use a valid row index and pass column index -1 to set margin for row.
    Use a valid column index and pass row index -1 to set margin for column.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nMargins [in]  Margin type. 
    \param fMargin [in]  Margin. 

    \remarks
    Margin type should be a combination of one or more following values:
    
    <table>
    Name                    Value
    kCellMarginTop          0x01
    kCellMarginLeft         0x02
    kCellMarginBottom       0x04
    kCellMarginRight        0x08
    kCellMarginHorzSpacing  0x10
    kCellMarginVertSpacing  0x20
    </table>
  */
  void  setMargin (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::CellMargin nMargins, 
    double fMargin);

  /** \details
    Returns the attachment point of the cell as an object of the OdGePoint3d class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param content [in]  Content index. 

    \remarks
    Method generates the eNotImplementedYet exception.
  */
  OdGePoint3d  attachmentPoint (
     OdInt32 row, 
     OdInt32 col, 
     OdInt32 content) const;

  /** \details
    Returns the color of the cell content as an object of the OdCmColor class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index. 

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  OdCmColor  contentColor (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets the color of the cell content.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index. 
    \param color [in]  Color content. 

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
  */
  void  setContentColor (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdCmColor& color);

  /** \details
    Gets the data type of the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index. 
    \param nDataType [out]  Reference to data type. 
    \param nUnitType [out]  Reference to unit type. 

    \remarks
    Data type should take one of following values:
    
    <table>
    Name       Value
    kUnknown   0x00
    kLong      0x01
    kDouble    0x02
    kString    0x04
    kDate      0x08
    kPoint     0x10
    k3dPoint   0x20
    kObjectId  0x40
    kBuffer    0x80
    kResbuf    0x100
    kGeneral   0x200
    kColor     0x400
    </table>

    Unit type should take one of following values:
    
    <table>
    Name         Value
    kUnitless    0x00
    kDistance    0x01
    kAngle       0x02
    kArea        0x04
    kVolume      0x08
    kCurrency    0x10
    kPercentage  0x20
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  getDataType (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    OdValue::DataType& nDataType, 
    OdValue::UnitType& nUnitType) const;

  /** \details
    Sets the data type to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nDataType [in]  Data type. 
    \param nUnitType [in]  Unit type. 

    \remarks
    Data type should take one of following values:
    
    <table>
    Name       Value
    kUnknown   0x00
    kLong      0x01
    kDouble    0x02
    kString    0x04
    kDate      0x08
    kPoint     0x10
    k3dPoint   0x20
    kObjectId  0x40
    kBuffer    0x80
    kResbuf    0x100
    kGeneral   0x200
    kColor     0x400
    </table>

    Unit type should take one of following values:
    
    <table>
    Name         Value
    kUnitless    0x00,
    kDistance    0x01,
    kAngle       0x02,
    kArea        0x04,
    kVolume      0x08,
    kCurrency    0x10,
    kPercentage  0x20
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
  */
  void  setDataType (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    OdValue::DataType nDataType, 
    OdValue::UnitType nUnitType);

  /** \details
    Returns the text style of the cell as an object of the OdDbObjectId class.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.  

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  OdDbObjectId  textStyle (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets the text style to the cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.  
    \param id [in]  Text style id.  

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
  */
  void  setTextStyle (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    const OdDbObjectId& id);

  /** \details
    Returns the text height of the cell as a double value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.  

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  double textHeight (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Returns the text height of the cell as a double value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.
    \param height [in]  Text height.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
  */
  void  setTextHeight (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    double height);

  /** \details
    Returns table rotation in radians as a double value.
  */
  double rotation() const;

  /** \details
    Returns the content rotation angle in radians as a double value.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  double rotation (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets table rotation in radians as a double value.

    \param fAngle [in]  Rotation angle.
  */
  void  setRotation(double fAngle);

  /** \details
    Sets the content rotation angle in radians.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.
    \param fAngle [in]  Rotation angle. 

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
    */
  void  setRotation (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    double fAngle);

  /** \details
    Checks whether data in the cell is auto-scaled or not. Returns the auto-scaled flag as a boolean value.
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.
  */
  bool  isAutoScale (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets the auto-scaled flag to the cell.
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.
    \param autoFit [in]  Auto-scaled flag.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
  */
  void  setAutoScale (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    bool autoFit);

  /** \details
    Returns the scale value of the cell as a double value.
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  double scale (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent) const;

  /** \details
    Sets scale value to the cell.
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nContent [in]  Content index.
    \param scale [in]  Scale value.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  setScale (
    OdInt32 row, 
    OdInt32 col, 
    OdInt32 nContent, 
    double scale);

  /** \details
    Returns content layout of the cell.
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Returning value should be one of following values:
    
    <table>
    Name                                 Value
    kCellContentLayoutFlow               0x1
    kCellContentLayoutStackedHorizontal  0x2
    kCellContentLayoutStackedVertical    0x4
    </table>

  */
  OdDb::CellContentLayout contentLayout (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Sets content layout to the cell.
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nLayout [in]  Content layout.

    \remarks
    Content layout should take one of following values:
    
    <table>
    Name                                 Value
    kCellContentLayoutFlow               0x1
    kCellContentLayoutStackedHorizontal  0x2
    kCellContentLayoutStackedVertical    0x4
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
    Method generates the eIsWriteProtected when format is not editable ( !isFormatEditable() ).
  */
  void  setContentLayout (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::CellContentLayout nLayout);

   /** \details
    Checks whether the merge-all flag is enabled for the cell, row, or column. 
    Returns true if the merge-all flag is enabled or false if it is not.
 
    \param row [in]  Row index. 
    \param col [in]  Column index. 

    \remarks
    Method generates the eNotImplementedYet exception.
  */
   bool  isMergeAllEnabled (
     OdInt32 row, 
     OdInt32 col) const;

  /** \details
    Sets the merge-all flag to enabled for the cell, row, or column. 
 
    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param bEnable [in]  merge-all flag. 

    \remarks
    Method generates the eNotImplementedYet exception.
  */
   void  enableMergeAll (
     OdInt32 row, 
     OdInt32 col, 
     bool bEnable);

  /** \details
    Returns the override of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row property overrides.
    Use a valid column index and pass row index -1 to get column property overrides.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nContent [in]  Content index. 

    \remarks
    Cell property should take one of the next values:
    
    <table>
    Name                        Value
    kCellPropInvalid            0x00000
    kCellPropDataType           0x00001
    kCellPropDataFormat         0x00002
    kCellPropRotation           0x00004
    kCellPropScale              0x00008
    kCellPropAlignment          0x00010
    kCellPropContentColor       0x00020
    kCellPropTextStyle          0x00040
    kCellPropTextHeight         0x00080
    kCellPropAutoScale          0x00100
    kCellPropBackgroundColor    0x00200
    kCellPropMarginLeft         0x00400
    kCellPropMarginTop          0x00800
    kCellPropMarginRight        0x01000
    kCellPropMarginBottom       0x02000
    kCellPropContentLayout      0x04000
    kCellPropMergeAll           0x08000
    kCellPropFlowDirBtoT        0x10000
    kCellPropMarginHorzSpacing  0x20000
    kCellPropMarginVertSpacing  0x40000
    kCellPropDataTypeAndFormat  (kCellPropDataType | kCellPropDataFormat)
    kCellPropContent            (kCellPropDataType | kCellPropDataFormat | kCellPropRotation | kCellPropScale | kCellPropContentColor | kCellPropTextStyle | kCellPropTextHeight | kCellPropAutoScale)
    kCellPropBitProperties      (kCellPropAutoScale | kCellPropMergeAll | kCellPropFlowDirBtoT)
    kCellPropAll                (kCellPropDataType | kCellPropDataFormat | kCellPropRotation | kCellPropScale | kCellPropAlignment | kCellPropContentColor | kCellPropBackgroundColor | kCellPropTextStyle | kCellPropTextHeight | kCellPropMarginLeft | kCellPropMarginTop | kCellPropMarginRight | kCellPropMarginBottom | kCellPropMarginHorzSpacing | kCellPropMarginVertSpacing | kCellPropAutoScale | kCellPropMergeAll | kCellPropFlowDirBtoT | kCellPropContentLayout)
    </table>
  */
   OdDb::CellProperty getOverride (
     OdInt32 row, 
     OdInt32 col, 
     OdInt32 nContent) const;

  /** \details
    Returns the override in the grid line of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row property overrides.
    Use a valid column index and pass row index -1 to get column property overrides.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype. 

    \remarks
    Grid property should take one of the following values:

    <table>
    Name                        Value
    kGridPropInvalid            0x00
    kGridPropLineStyle          0x01
    kGridPropLineWeight         0x02
    kGridPropLinetype           0x04
    kGridPropColor              0x08
    kGridPropVisibility         0x10
    kGridPropDoubleLineSpacing  0x20
    kGridPropAll                (kGridPropLineStyle | kGridPropLineWeight | kGridPropLinetype | kGridPropColor | kGridPropVisibility | kGridPropDoubleLineSpacing)
    </table>

    Grid linetype should take one of the following values:
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
   OdDb::GridProperty getOverride (
     OdInt32 row, 
     OdInt32 col, 
     OdDb::GridLineType nGridLineType) const;

  
  /** \details
    Sets the override in the cell, row, column, content, or grid line in the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row property overrides.
    Use a valid column index and pass row index -1 to set column property overrides.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nContent [in]  Content index. 
    \param nOverride [in]  Override. 

    \remarks
    Cell property should take one of the following values:
    
    <table>
    Name                        Value
    kCellPropInvalid            0x00000
    kCellPropDataType           0x00001
    kCellPropDataFormat         0x00002
    kCellPropRotation           0x00004
    kCellPropScale              0x00008
    kCellPropAlignment          0x00010
    kCellPropContentColor       0x00020
    kCellPropTextStyle          0x00040
    kCellPropTextHeight         0x00080
    kCellPropAutoScale          0x00100
    kCellPropBackgroundColor    0x00200
    kCellPropMarginLeft         0x00400
    kCellPropMarginTop          0x00800
    kCellPropMarginRight        0x01000
    kCellPropMarginBottom       0x02000
    kCellPropContentLayout      0x04000
    kCellPropMergeAll           0x08000
    kCellPropFlowDirBtoT        0x10000
    kCellPropMarginHorzSpacing  0x20000
    kCellPropMarginVertSpacing  0x40000
    kCellPropDataTypeAndFormat  (kCellPropDataType | kCellPropDataFormat)
    kCellPropContent            (kCellPropDataType | kCellPropDataFormat | kCellPropRotation | kCellPropScale | kCellPropContentColor | kCellPropTextStyle | kCellPropTextHeight | kCellPropAutoScale)
    kCellPropBitProperties      (kCellPropAutoScale | kCellPropMergeAll | kCellPropFlowDirBtoT)
    kCellPropAll                (kCellPropDataType | kCellPropDataFormat | kCellPropRotation | kCellPropScale | kCellPropAlignment | kCellPropContentColor | kCellPropBackgroundColor | kCellPropTextStyle | kCellPropTextHeight | kCellPropMarginLeft | kCellPropMarginTop | kCellPropMarginRight | kCellPropMarginBottom | kCellPropMarginHorzSpacing | kCellPropMarginVertSpacing | kCellPropAutoScale | kCellPropMergeAll | kCellPropFlowDirBtoT | kCellPropContentLayout)
    </table>
  */
   void  setOverride (
     OdInt32 row, 
     OdInt32 col, 
     OdInt32 nContent, 
     OdDb::CellProperty nOverride);

  /** \details
    Sets the override in the cell, row, column, content, or grid line in the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row property overrides.
    Use a valid column index and pass row index -1 to set column property overrides.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype.
    \param nOverride [in]  Override. 

    \remarks
    Grid linetype should take one of the next values:
 
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
   void  setOverride (
     OdInt32 row, 
     OdInt32 col, 
     OdDb::GridLineType nGridLineType, 
     OdDb::GridProperty nOverride);

  /** \details
    Removes all overrides in the cell, row, or column. 
    Use a valid row index and pass column index -1 to remove row property overrides.
    Use a valid column index and pass row index -1 to remove column property overrides.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
  */
   void  removeAllOverrides (
     OdInt32 row, 
     OdInt32 col);
  
  /** \details
    Returns the grid line style of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid line style.
    Use a valid column index and pass row index -1 to get column grid line style.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype. 

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid line style should take one of the following values:
    
    <table>
    Name                  Value
    kGridLineStyleSingle  1
    kGridLineStyleDouble  2
    </table>
  */
  OdDb::GridLineStyle gridLineStyle (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid line style to the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row grid line style.
    Use a valid column index and pass row index -1 to set column grid line style.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype. 
    \param nLineStyle [in]  Grid line style. 

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid line style should take one of the following values:
    
    <table>
    Name                  Value
    kGridLineStyleSingle  1
    kGridLineStyleDouble  2
    </table>
  */
  void  setGridLineStyle (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::GridLineStyle nLineStyle);

  /** \details
    Returns the grid lineweight of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid lineweight.
    Use a valid column index and pass row index -1 to get column grid lineweight.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype. 

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid lineweight should take one of the following values:
    
    <table>
    Name              Value
    kLnWt000          0
    kLnWt005          5
    kLnWt009          9
    kLnWt013          13
    kLnWt015          15
    kLnWt018          18
    kLnWt020          20
    kLnWt025          25
    kLnWt030          30
    kLnWt035          35
    kLnWt040          40
    kLnWt050          50
    kLnWt053          53
    kLnWt060          60
    kLnWt070          70
    kLnWt080          80
    kLnWt090          90
    kLnWt100          100
    kLnWt106          106
    kLnWt120          120
    kLnWt140          140
    kLnWt158          158
    kLnWt200          200
    kLnWt211          211
    kLnWtByLayer      -1
    kLnWtByBlock      -2
    kLnWtByLwDefault  -3
    </table>
  */
  OdDb::LineWeight gridLineWeight (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid lineweight of the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row grid lineweight.
    Use a valid column index and pass row index -1 to set column grid lineweight.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype.
    \param nLineWeight [in]  Grid lineweight.

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid lineweight should take one of the following values:
    
    <table>
    Name              Value
    kLnWt000          0
    kLnWt005          5
    kLnWt009          9
    kLnWt013          13
    kLnWt015          15
    kLnWt018          18
    kLnWt020          20
    kLnWt025          25
    kLnWt030          30
    kLnWt035          35
    kLnWt040          40
    kLnWt050          50
    kLnWt053          53
    kLnWt060          60
    kLnWt070          70
    kLnWt080          80
    kLnWt090          90
    kLnWt100          100
    kLnWt106          106
    kLnWt120          120
    kLnWt140          140
    kLnWt158          158
    kLnWt200          200
    kLnWt211          211
    kLnWtByLayer      -1
    kLnWtByBlock      -2
    kLnWtByLwDefault  -3
    </table>
  */
  void  setGridLineWeight (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::LineWeight nLineWeight);

  /** \details
    Returns the grid linetype of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid linetype.
    Use a valid column index and pass row index -1 to get column grid linetype.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetypes.

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  OdDbObjectId    gridLinetype (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid linetype of the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row grid linetype.
    Use a valid column index and pass row index -1 to set column grid linetype.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineTypes [in]  Grid linetypes.
    \param idLinetype [in]  Grid linetype to set.

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  void  setGridLinetype (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineTypes, 
    const OdDbObjectId& idLinetype);

  /** \details
    Returns the grid color of the cell, row, or column as an object of the OdCmColor class. 
    Use a valid row index and pass column index -1 to get row grid line color.
    Use a valid column index and pass row index -1 to get column grid line color.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype.

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  OdCmColor gridColor (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid color of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid line color.
    Use a valid column index and pass row index -1 to get column grid line color.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridlineTypes [in]  Grid linetypes.
    \param color [in]  Grid color.

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  void  setGridColor (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridlineTypes, 
    const OdCmColor& color);

  /** \details
    Returns the grid visibility of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid visibility .
    Use a valid column index and pass row index -1 to get column grid visibility .

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype.

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid visibility should take one of the following values:
    
    <table>
    Name        Value
    kInvisible  1
    kVisible    0
    </table>
  */
  OdDb::Visibility gridVisibility (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid visibility of the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row grid visibility .
    Use a valid column index and pass row index -1 to set column grid visibility .

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineTypes [in]  Grid linetypes.
    \param nVisibility [in]  Grid line visibility.

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>

    Grid visibility should take one of the following values:
    
    <table>
    Name        Value
    kInvisible  1
    kVisible    0
    </table>
  */
  void  setGridVisibility (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::Visibility nVisibility);

  /** \details
    Returns the grid double line spacing of the cell, row, or column as a double value. 
    Use a valid row index and pass column index -1 to get row grid double line spacing.
    Use a valid column index and pass row index -1 to get column grid double line spacing.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype.

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  double gridDoubleLineSpacing (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType) const;

  /** \details
    Sets the grid double line spacing of the cell, row, or column. 
    Use a valid row index and pass column index -1 to set row grid double line spacing.
    Use a valid column index and pass row index -1 to set column grid double line spacing.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineTypes [in]  Grid linetypes.
    \param fSpacing [in]  Grid double line spacing.

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  void  setGridDoubleLineSpacing(
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType 
    nGridLineTypes, 
    double fSpacing);

  /** \details
    Returns the grid line property of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid property.
    Use a valid column index and pass row index -1 to get column grid property.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineType [in]  Grid linetype.
    \param gridProp [out]  Grid line property.

    \remarks
    Grid linetype should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  void  getGridProperty (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineType, 
    OdGridProperty& gridProp) const;

  /** \details
    Sets the grid line property of the cell, row, or column. 
    Use a valid row index and pass column index -1 to get row grid property.
    Use a valid column index and pass row index -1 to get column grid property.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param nGridLineTypes [in]  Grid linetypes.
    \param gridProp [in]  Grid line property.

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  void  setGridProperty (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::GridLineType nGridLineTypes, 
    const OdGridProperty& gridProp);

  /** \details
    Sets the grid line property of the cell, row, or column. 

    \param rangeIn [in]  Cell range, row range or column range.
    \param nGridLineTypes [in]  Grid linetypes.
    \param gridProp [in]  Grid line property.

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  void  setGridProperty  (
    const OdCellRange& rangeIn, 
    OdDb::GridLineType nGridLineTypes, 
    const OdGridProperty& gridProp);

    /** \details
    Checks if the cell is linked to a data source. 
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 

    \remarks
    Grid linetypes should take one of the following values:
    
    <table>
    Name                 Value
    kInvalidGridLine     0x00
    kHorzTop             0x01
    kHorzInside          0x02
    kHorzBottom          0x04
    kVertLeft            0x08
    kVertInside          0x10
    kVertRight           0x20
    kHorzGridLineTypes   kHorzTop|kHorzBottom|kHorzInside
    kVertGridLineTypes   kVertLeft|kVertRight|kVertInside
    kOuterGridLineTypes  kHorzTop|kHorzBottom|kVertLeft|kVertRight
    kInnerGridLineTypes  kHorzInside|kVertInside
    kAllGridLineTypes    kOuterGridLineTypes|kInnerGridLineTypes
    </table>
  */
  bool isLinked (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Returns the data link (OdDbDataLink) for the specified row and column.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell.
  */
  OdDbObjectId getDataLink (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Opens and returns the data link (OdDbDataLink) for the specified row and column.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell.
    \param mode [in]  Open mode.

    \remarks
    Open mode should take one of the following values:
    
    <table>
    Name        Value
    kNotOpen    -1
    kForRead    0
    kForWrite   1
    kForNotify  2
    </table>
  */
  OdDbDataLinkPtr getDataLink (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::OpenMode mode) const;

  /** \details
    Returns the number of data links (OdDbDataLink) for the specified cell range.

    \param pRange [in]  Cell range. 
    \param dataLinkIds [out]  Array of data link IDs.
  */
  OdInt32 getDataLink (
    const OdCellRange& pRange, 
    OdDbObjectIdArray& dataLinkIds) const;

  /** \details
    Sets the data link (OdDbDataLink) for the specified row and column.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell.
    \param idDataLink [in]  Array of data link IDs.
    \param bUpdate [in]  Flag that indicates whether or not the data link should be updated after setting it.

    \remarks
    Method generates the eNotThatKindOfClass exception when idDataLink in not an OdDbDataLink object id.
  */
  void setDataLink (
    OdInt32 row, 
    OdInt32 col, 
    const OdDbObjectId& idDataLink, 
    bool bUpdate);

  /** \details
    Sets the data link (OdDbDataLink) for the specified cell range.

    \param range [in]  Cell range. 
    \param idDataLink [in]  Array of data link IDs.
    \param bUpdate [in]  Flag that indicates whether or not the data link should be updated after setting it.

    \remarks
    Method generates the eNotThatKindOfClass exception when idDataLink in not an OdDbDataLink object id.
  */
  void setDataLink 
    (const OdCellRange& range, 
    const OdDbObjectId& idDataLink, 
    bool bUpdate);

  /** \details
    Returns the data link cell range that includes the specified row and column.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell.
  */
  OdCellRange getDataLinkRange (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Removes the data link at the specified cell.

    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell.

    \remarks
    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  removeDataLink (
    OdInt32 row, 
    OdInt32 col);

  /** \details
    Removes all data links.
  */
  void  removeDataLink (void);
  
  /** \details
    Updates the data link of the cell. 
 
    \param row [in]  Row index of the cell. 
    \param col [in]  Column index of the cell. 
    \param nDir [in]  Direction of update.
    \param nOption [in]  Update option.

    \remarks
    Direction of update should take one of the following values:
    
    <table>
    Name                          Value
    kUpdateDirectionSourceToData  0x1
    kUpdateDirectionDataToSource  0x2
    </table>

    Update option should take one of the following values:
    
    <table>
    Name                                              Value
    kUpdateOptionNone                                 0
    kUpdateOptionSkipFormat                           0x20000
    kUpdateOptionUpdateRowHeight                      0x40000
    kUpdateOptionUpdateColumnWidth                    0x80000
    kUpdateOptionAllowSourceUpdate                    0x100000
    kUpdateOptionForceFullSourceUpdate                0x200000
    kUpdateOptionOverwriteContentModifiedAfterUpdate  0x400000
    kUpdateOptionOverwriteFormatModifiedAfterUpdate   0x800000
    kUpdateOptionForPreview                           0x1000000
    kUpdateOptionIncludeXrefs                         0x2000000
    kUpdateOptionSkipFormatAfterFirstUpdate           0x4000000
    </table>

    Method generates the eInvalidInput exception when input row number and column number are less than 0 or greater than table size.
  */
  void  updateDataLink (
    OdInt32 row, 
    OdInt32 col, 
    OdDb::UpdateDirection nDir, 
    OdDb::UpdateOption nOption);

  /** \details
    Updates the data link in the table. 
 
    \param nDir [in]  Direction of update
    \param nOption [out]  Update option.

    \remarks
    Direction of update should take one of the following values:
    
    <table>
    Name                          Value
    kUpdateDirectionSourceToData  0x1
    kUpdateDirectionDataToSource  0x2
    </table>

    Update option should take one of the following values:
    
    <table>
    Name                                              Value
    kUpdateOptionNone                                 0
    kUpdateOptionSkipFormat                           0x20000
    kUpdateOptionUpdateRowHeight                      0x40000
    kUpdateOptionUpdateColumnWidth                    0x80000
    kUpdateOptionAllowSourceUpdate                    0x100000
    kUpdateOptionForceFullSourceUpdate                0x200000
    kUpdateOptionOverwriteContentModifiedAfterUpdate  0x400000
    kUpdateOptionOverwriteFormatModifiedAfterUpdate   0x800000
    kUpdateOptionForPreview                           0x1000000
    kUpdateOptionIncludeXrefs                         0x2000000
    kUpdateOptionSkipFormatAfterFirstUpdate           0x4000000
    </table>
  */
  void  updateDataLink (
    OdDb::UpdateDirection nDir, 
    OdDb::UpdateOption nOption);

  /** \details
    Returns the column name as an OdString value. 
 
    \param nIndex [in]  Column index.
  */
  OdString  getColumnName (
    OdInt32 nIndex) const;

  /** \details
    Sets the column name. 
 
    \param nIndex [in]  Column index.
    \param sName [in]  Column name.
  */
  void setColumnName (
    OdInt32 nIndex, 
    const OdString& sName);

  /** \details
    Returns the tooltip of the cell as an OdString value. 

    \param row [in]  Row index. 
    \param col [in]  Column index. 
  */
  OdString getToolTip (
    OdInt32 row, 
    OdInt32 col) const;

  /** \details
    Sets the tooltip of the cell.

    \param row [in]  Row index. 
    \param col [in]  Column index. 
    \param sToolTip [in]  Tooltip. 
  */
  void  setToolTip (
    OdInt32 row, 
    OdInt32 col, 
    const OdString sToolTip);
  // void  createTemplate (OdDbTableTemplate*& pTemplate, OdDb::TableCopyOption nCopyOption);
  // void  getIndicatorSize (double& fWidth, double& fHeight) const;

  /** \details
    Copies the content from the source object. 
 
    \param pSource [in]  Source object.  
  */
  virtual void copyFrom(
    const OdRxObject* pSource);

  /** \details
    Copies the content and format from the source table. 
 
    \param pSrc [in]  Source table.  
    \param nOption [in]  Copy option.  
  */
  void copyFrom (
    const OdDbLinkedTableData* pSrc, 
    OdDb::TableCopyOption nOption);

  /** \details
    Copies the content and format from the source table. 
  
    \param pSrc [in]  Source table.  
    \param nOption [in]  Copy option.  
    \param srcRange [in]  Source cell range.  
    \param targetRange [in]  Target cell range.  
    \param pNewTargetRangeOut [out]  Target cell range.  
  */
  void copyFrom (const OdDbLinkedTableData* pSrc, 
    OdDb::TableCopyOption nOption, 
    const OdCellRange& srcRange, 
    const OdCellRange& targetRange,
    OdCellRange* pNewTargetRangeOut);

  /** \details
    Copies the content and format from the source table. 
  
    \param pSrc [in]  Source table.  
    \param nOption [in]  Copy option.  
    \param srcRange [in]  Source cell range.  
    \param targetRange [in]  Target cell range.  
    \param pNewTargetRangeOut [out]  Target cell range.  
  */
  void copyFrom  (const OdDbTable* pSrc, 
    OdDb::TableCopyOption nOption, 
    const OdCellRange& srcRange, 
    const OdCellRange& targetRange,
    OdCellRange* pNewTargetRangeOut);

  /** \details
    Appends this OdDbTable object to the specified owner object.  
  
    \param idPair [in]  ID pair to append.  
    \param pOwnerObject [in]  Pointer to the owner object.  
    \param ownerIdMap [in/out]  Owner's ID map.  
  */  
  void appendToOwner(
    OdDbIdPair& idPair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap);

  /** \details
    Returns true if regeneration of the table block is disabled. Otherwise, returns false.
  */
  bool isRegenerateTableSuppressed() const;

  /** \details
    This method allows the user to disable the regeneration of the table block during property changes. 

    \param bSuppress [in]   Input boolean value to suppress or enable the regeneration of a table object.
  */
  void suppressRegenerateTable(bool bSuppress);

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE;

  void createTemplate(OdDbTableTemplate* target, OdDb::TableCopyOption nCopyOption);

  virtual OdResult subErase(bool erasing);

  virtual void subHighlight(bool bDoIt = true,
    const OdDbFullSubentPath* pSubId = 0, 
    bool highlightAll = false) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbTable object pointers.
*/
typedef OdSmartPtr<OdDbTable> OdDbTablePtr;

DBENT_EXPORT OdDbTablePtr odDbCreateDataLinkAndTable(OdDbDatabase* pDb, OdString linkName, OdString linkDescription, OdString connectionString);

#include "TD_PackPop.h"

#endif
