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
#ifndef __DG_TABLEELEMENT_H__
#define __DG_TABLEELEMENT_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgTextNode.h"

//-------------------------------------------------------------------
// OdDgTableCellSymbology
//-------------------------------------------------------------------

class TG_EXPORT OdDgTableCellSymbology
{ 
public:
  OdDgTableCellSymbology();
//  virtual ~OdDgTableCellSymbology();

  OdUInt32 getColorIndex() const;
  void     setColorIndex( OdUInt32 uColor );

  OdUInt32 getLineStyle() const;
  void     setLineStyle( OdUInt32 uLineStyle );

  OdUInt32 getLineWeight() const;
  void     setLineWeight( OdUInt32 uLineWeight );

  bool     getLineVisibility() const;
  void     setLineVisibility( bool bSet );

  void     updateTableSymbology( OdUInt32 uTableColor, OdUInt32 uLineStyle, OdUInt32 uLineWeight);

  bool     getUseTableColorFlag() const;
  void     setUseTableColorFlag( bool bSet );

  bool     getUseTableLineWeightFlag() const;
  void     setUseTableLineWeightFlag( bool bSet );

  bool     getUseTableLineStyleFlag() const;
  void     setUseTableLineStyleFlag( bool bSet );

private:
  OdUInt32 m_uColorIndex;
  OdUInt32 m_uLineStyleId;
  OdUInt32 m_uLineWeight;
  bool     m_bVisibility;
  bool     m_bUseTableColorIndex;
  bool     m_bUseTableLineWeight;
  bool     m_bUseTableLineStyleId;
};

//-------------------------------------------------------------------
// OdDgTableElementMerge
//-------------------------------------------------------------------

class TG_EXPORT OdDgTableElementMerge
{ 
public:
  OdDgTableElementMerge();
  virtual ~OdDgTableElementMerge() {}

  virtual OdUInt32 getBaseRowIndex() const;
  virtual void     setBaseRowIndex( OdUInt32 uRow );

  virtual OdUInt32 getBaseColumnIndex() const;
  virtual void     setBaseColumnIndex( OdUInt32 uColumn );

  virtual OdUInt32 getRowCount() const;
  virtual void     setRowCount( OdUInt32 nRows );

  virtual OdUInt32 getColumnCount() const;
  virtual void     setColumnCount( OdUInt32 nColumns );

private:
  OdUInt32 m_uRowIndex;
  OdUInt32 m_uColumnIndex;
  OdUInt32 m_nRows;
  OdUInt32 m_nColumns;
};

//-------------------------------------------------------------------
// OdDgTableCellElement
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTableCellElement : public OdRxObject
{
public:

  enum OdDgTableCellElementType
  {
    kBody        = 0,
    kTitleRow    = 1,
    kHeaderRow   = 2,
    kFooterRow   = 3,
    kFirstColumn = 4,
    kLastColumn  = 5
  };

  enum OdDgTableCellElementMarginType
  {
    kNone        = 0,
    kNarrow      = 1,
    kNormal      = 2,
    kWide        = 3
  };

  enum OdDgTableCellElementTextAlignment
  {
    kLeftTop        = 0,
    kLeftMiddle     = 1,
    kLeftBottom     = 2,
    kCenterTop      = 3,
    kCenterMiddle   = 4,
    kCenterBottom   = 5,
    kRightTop       = 6,
    kRightMiddle    = 7,
    kRightBottom    = 8
  };

  enum OdDgTableCellElementTextOrientation
  {
    kHorizontal   = 0,
    kRotateRight  = 1,
    kRotateLeft   = 2,
    kVertical     = 3
  };

public:
  ODRX_DECLARE_MEMBERS( OdDgTableCellElement );

  virtual OdDgTableCellElementType getTableCellType() const = 0;
  virtual OdUInt32 getRowIndex() const = 0;
  virtual OdUInt32 getColumnIndex() const = 0;

  virtual OdDgTableCellSymbology getTopBorder() const = 0;
  virtual void setTopBorder( const OdDgTableCellSymbology& symb ) = 0;

  virtual OdDgTableCellSymbology getBottomBorder() const = 0;
  virtual void setBottomBorder( const OdDgTableCellSymbology& symb ) = 0;

  virtual OdDgTableCellSymbology getLeftBorder() const = 0;
  virtual void setLeftBorder( const OdDgTableCellSymbology& symb ) = 0;

  virtual OdDgTableCellSymbology getRightBorder() const = 0;
  virtual void setRightBorder( const OdDgTableCellSymbology& symb ) = 0;

  virtual OdDgTextNode3dPtr getCellText() const = 0;
  virtual void setCellText( const OdDgTextNode3dPtr& pText3d ) = 0;
  virtual void setCellText( const OdString& strText ) = 0;

  virtual OdDgTableCellElementMarginType getCellMarginType() const = 0;
  virtual void setCellMarginType( const OdDgTableCellElementMarginType& uType ) = 0;

  virtual OdDgTableCellElementTextAlignment getTextAlignment() const = 0;
  virtual void setTextAlignment( const OdDgTableCellElementTextAlignment& uTextAlignment ) = 0;

  virtual OdDgTableCellElementTextOrientation getTextOrientation() const = 0;
  virtual void setTextOrientation( const OdDgTableCellElementTextOrientation& uTextOrientation ) = 0;  

  virtual bool getFillFlag() const = 0;
  virtual void setFillFlag( bool bSet ) = 0;

  virtual bool getShowTextFlag() const = 0;
  virtual void setShowTextFlag( bool bSet ) = 0;

  virtual OdUInt32 getFillColor() const = 0;
  virtual void setFillColor( OdUInt32 uColorIndex ) = 0;

  virtual double       getWidth() const = 0;
  virtual double       getHeight() const = 0;
  virtual OdDgElementId getTableElementId() const = 0;

  virtual void copyFrom(const OdRxObject* pSource) = 0;
};

typedef OdSmartPtr<OdDgTableCellElement> OdDgTableCellElementPtr;

//-------------------------------------------------------------------
// OdDgTableRowElement
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTableRowElement : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTableRowElement );

  virtual OdDgTableCellElement::OdDgTableCellElementType getTableRowType() const = 0;

  virtual OdUInt32 getRowIndex() const = 0;

  virtual double getHeight() const = 0;
  virtual void   setHeight( double dHeight ) = 0;

  virtual bool getLockFlag() const = 0;
  virtual void setLockFlag( bool bSet ) = 0;

  virtual double  getWidth() const = 0;

  virtual void copyFrom(const OdRxObject* pSource) = 0;
};

typedef OdSmartPtr<OdDgTableRowElement> OdDgTableRowElementPtr;

//-------------------------------------------------------------------
// OdDgTableColumnElement
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTableColumnElement : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTableColumnElement );

  virtual OdDgTableCellElement::OdDgTableCellElementType getTableColumnType() const = 0;

  virtual OdUInt32 getColumnIndex() const = 0;

  virtual double getWidth() const = 0;
  virtual void   setWidth( double dWidth ) = 0;

  virtual bool getLockFlag() const = 0;
  virtual void setLockFlag( bool bSet ) = 0;

  virtual double getHeight() const = 0;

  virtual void copyFrom(const OdRxObject* pSource) = 0;
};

typedef OdSmartPtr<OdDgTableColumnElement> OdDgTableColumnElementPtr;

//-------------------------------------------------------------------
// OdDgTableElement
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTableElement : public OdDgGraphicsElement
{
public:

  enum OdDgTableElementBreakType
  {
    kNoBreak = 0,
    kHorizontalBreak = 1
  };

  enum OdDgTableElementSubTablePosition
  {
    kRight  = 0,
    kLeft   = 1,
    kAbove  = 2,
    kBelow  = 3,
    kManual = 4
  };

public:
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTableElement );

  virtual OdUInt32 getRowCount() const;
  virtual OdDgTableRowElementPtr getRow( OdUInt32 uIndex );
  virtual void addRow( OdInt32 uIndexBefore = -1 );
  virtual void deleteRow( OdUInt32 uIndex );

  virtual OdUInt32 getColumnCount() const;
  virtual OdDgTableColumnElementPtr getColumn( OdUInt32 uIndex );
  virtual void addColumn( OdInt32 uIndexBefore = -1 );
  virtual void deleteColumn( OdUInt32 uIndex );

  virtual OdDgTableCellElementPtr getCell( OdUInt32 uRowIndex, OdUInt32 uColumnIndex );

  virtual bool     getAnnotationScaleFlag() const;
  virtual void     setAnnotationScaleFlag( bool bSet );

  virtual double   getAnnotationScale() const;
  virtual void     setAnnotationScale( double dScale );

  virtual OdGePoint3d getOrigin() const;
  virtual void        setOrigin( const OdGePoint3d& ptOrigin );

  virtual OdGeMatrix3d getRotation() const;
  virtual void        setRotation( const OdGeMatrix3d& matRotation );

  virtual double   getWidth() const;
  virtual double   getHeight() const; 

  virtual double   getDefaultRowHeight() const;
  virtual double   getDefaultColumnWidth() const;
  virtual double   getDefaultTextSize() const;
  virtual double   getCellTopMargin() const;
  virtual double   getCellBottomMargin() const;
  virtual double   getCellLeftMargin() const;
  virtual double   getCellRightMargin() const;

  virtual bool     getOddFillColorFlag() const;
  virtual void     setOddFillColorFlag( bool bSet );

  virtual bool     getEvenFillColorFlag() const;
  virtual void     setEvenFillColorFlag( bool bSet );

  virtual OdUInt32 getOddFillColor() const;
  virtual void     setOddFillColor( OdUInt32 uFillColor );

  virtual OdUInt32 getEvenFillColor() const;
  virtual void     setEvenFillColor( OdUInt32 uFillColor );

  virtual OdUInt32 getTitleRowCount() const;
  virtual void     setTitleRowCount( OdUInt32 nRows );

  virtual OdUInt32 getHeaderRowCount() const;
  virtual void     setHeaderRowCount( OdUInt32 nRows );

  virtual OdUInt32 getFooterRowCount() const;
  virtual void     setFooterRowCount( OdUInt32 nRows );

  virtual OdUInt32 getFirstColumnsCount() const;
  virtual void     setFirstColumnsCount( OdUInt32 nColumns );

  virtual OdUInt32 getLastColumnsCount() const;
  virtual void     setLastColumnsCount( OdUInt32 nColumns );

  virtual OdUInt32 getBodyTextStyle() const;
  virtual void     setBodyTextStyle( OdUInt32 uTextStyle );

  virtual OdUInt32 getTitleRowTextStyle() const;
  virtual void     setTitleRowTextStyle( OdUInt32 uTextStyle );

  virtual OdUInt32 getHeaderRowTextStyle() const;
  virtual void     setHeaderRowTextStyle( OdUInt32 uTextStyle );

  virtual OdUInt32 getFooterRowTextStyle() const;
  virtual void     setFooterRowTextStyle( OdUInt32 uTextStyle );

  virtual OdUInt32 getFirstColumnTextStyle() const;
  virtual void     setFirstColumnTextStyle( OdUInt32 uTextStyle );

  virtual OdUInt32 getLastColumnTextStyle() const;
  virtual void     setLastColumnTextStyle( OdUInt32 uTextStyle );

  virtual OdUInt32 getTableColorIndex() const;
  virtual OdUInt32 getTableLineStyle() const;
  virtual OdUInt32 getTableLineWeight() const;
  virtual void     setTableSymbology( OdUInt32 uColor, OdUInt32 uLineStyle, OdUInt32 uLineWeight );

  virtual bool     isDefaultTextColorFromTable() const;
  virtual OdUInt32 getDefaultTextColorIndex() const;
  virtual void     setDefaultTextColorIndex( OdUInt32 uColor, bool bColorFromTable );

  virtual bool     isDefaultTextLineWeightFromTable() const;
  virtual OdUInt32 getDefaultTextLineWeight() const;
  virtual void     setDefaultTextLineWeight( OdUInt32 uLineWeight, bool bLineWeightFromTable );

  virtual OdDgTableElementBreakType getBreakType() const;
  virtual void setBreakType( const OdDgTableElementBreakType& uBreakType );

  virtual OdDgTableElementSubTablePosition getSubTablePosition() const;
  virtual void setSubTablePosition( const OdDgTableElementSubTablePosition& uPosition );

  virtual double getBreakHeight() const;
  virtual void setBreakHeight( double dHeight );

  virtual double getSubTableSpacing() const;
  virtual void setSubTableSpacing( double dSubTableSpacing );

  virtual bool getRepeatHeadersFlag() const;
  virtual void setRepeatHeadersFlag( bool bSet );

  virtual bool getRepeatFootersFlag() const;
  virtual void setRepeatFootersFlag( bool bSet );

  virtual OdUInt32 getMergeCount() const;
  virtual OdDgTableElementMerge getMerge( OdUInt32 uIndex ) const;
  virtual bool setMerge( OdUInt32 uIndex, const OdDgTableElementMerge& mergeData );
  virtual bool addMerge( const OdDgTableElementMerge& mergeData );
  virtual void deleteMerge( OdUInt32 uIndex );

  virtual OdUInt16 getTablePrototypeIndex() const;
  virtual void initTableDefaults();

  void copyFrom(const OdRxObject* pSource);

  //overrides for OdGiDrawable
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr<OdDgTableElement> OdDgTableElementPtr;

//-------------------------------------------------------------------
// OdDgTablePrototypeElement
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgPrototypeSchemaItem
{
public:
  enum OdDgPrototypeSchemaItemType
  {
    kGroup            = 0,
    kExpressionDesc   = 1,
    kBoolean          = 2,
    kDateTime         = 3,
    kDouble           = 4,
    kInt32            = 5,
    kPoint3d          = 8,
    kString           = 9,
    kBooleanArray     = 12,
    kDateTimeArray    = 13,
    kDoubleArray      = 14,
    kInt32Array       = 15,
    kPoint3dArray     = 18,
    kStringArray      = 19,
    kFlags            = 100
  };

public:
  OdDgPrototypeSchemaItem();
  virtual ~OdDgPrototypeSchemaItem();

  OdDgPrototypeSchemaItemType getItemDataType() const;
  void     setItemDataType( const OdDgPrototypeSchemaItemType& uDataType );

  OdUInt32 getParentSchemaItemIndex() const;
  void     setParentSchemaItemIndex( OdUInt32 uParentIndex );

  bool     getItemTypeBasedDataFlag() const;
  void     setItemTypeBasedDataFlag( bool bSet );

  OdString getItemName() const;
  void     setItemName( const OdString& strName );

  bool     getProxyDataFlag() const;
  void     setProxyDataFlag( bool bSet );

  OdBinaryData getProxyData() const;
  void         setProxyData( const OdBinaryData& binData );

private:
  OdDgPrototypeSchemaItemType m_uDataType;
  OdUInt32      m_uParentIndex;
  OdUInt32      m_uDataOffset;
  OdUInt32      m_uDataFlags;
  OdUInt32      m_uBitwiseAndData;
  OdString      m_strData;
  bool          m_bItemTypeData;
  bool          m_bProxy;
  OdBinaryData  m_binProxyData;
};

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgPrototypeSchema
{
  public:
    OdDgPrototypeSchema();
    virtual ~OdDgPrototypeSchema();

    OdUInt16 getSchemaIndex() const;
    void     setSchemaIndex( OdUInt16 uIndex );

    OdString  getSchemaName() const;
    void      setSchemaName( const OdString& strName );

    OdUInt32                 getSchemaItemCount() const;
    OdDgPrototypeSchemaItem  getSchemaItem( OdUInt32 uIndex ) const;
    void  setSchemaItem( OdUInt32 uIndex, const OdDgPrototypeSchemaItem& item );
    void  addSchemaItem( const OdDgPrototypeSchemaItem& item );
    void  addSchemaItem( const OdString& strName, const OdDgPrototypeSchemaItem::OdDgPrototypeSchemaItemType& uType, OdUInt32 uParentIndex = 0, bool bItemTypeBasedItem = false );
    void  deleteSchemaItem( OdUInt32 uIndex );

  private:
    OdUInt16 m_uSchemaIndex;
    OdString m_strSchemaName;
    OdArray<OdDgPrototypeSchemaItem> m_arrSchemaItems;
};

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgTablePrototypeElement : public OdDgElement
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTablePrototypeElement );

  virtual OdUInt16 getPrototypeIndex() const;
  virtual OdString getPrototypeName() const;

  virtual OdUInt32 getPrototypeVersion() const;
  virtual void     setPrototypeVersion( OdUInt32 uVersion);

  virtual OdUInt32 getPrototypeSubVersion() const;
  virtual void     setPrototypeSubVersion( OdUInt32 uSubVersion);

  void      setPrototypeIndex( OdUInt16 uIndex );
  void      setPrototypeName( const OdString& strName );

  OdBinaryData getPrototypeHeader() const;
  OdBinaryData getDefaultPrototypeHeader() const;
  void         setPrototypeHeader( const OdBinaryData& binHeaderData );

  OdUInt32                 getSchemaCount() const;
  OdDgPrototypeSchema      getSchema( OdUInt32 uIndex ) const;

  OdDgPrototypeSchema      getSchemaByIndex( OdUInt16 uSchemaIndex ) const;
  bool                     hasSchema( OdUInt16 uSchemaIndex ) const;
  void                     setSchema( OdDgPrototypeSchema& schema );
  void                     deleteSchemaByIndex( OdUInt16 uSchemaIndex );
};

typedef OdSmartPtr<OdDgTablePrototypeElement> OdDgTablePrototypeElementPtr;

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/

class TG_EXPORT OdDgPrototypeElementTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgPrototypeElementTable);
public:

  void add( OdDgElementPtr pPrototypeElement);

  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  OdDgElementId getAt( const OdString& strPrototypeName );
  OdDgElementId getAt( OdUInt16 uPrototypeIndex );
};

typedef OdSmartPtr<OdDgPrototypeElementTable> OdDgPrototypeElementTablePtr;

#include "TD_PackPop.h"

#endif // __DG_TABLEELEMENT_H__
