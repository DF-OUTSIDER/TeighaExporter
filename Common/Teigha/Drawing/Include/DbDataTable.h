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


#ifndef OD_DBDATATABLE_H
#define OD_DBDATATABLE_H

#include "TD_PackPush.h"
#include "RxObject.h"
#include "DbObject.h"

class OdDbDataCellImpl;
/** \details
    Corresponding C++ library: TD_Db 
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDataCell : public OdRxObject
{
public:
  enum CellType {
      kUnknown      = 0,
      kInteger      = 1,
      kDouble       = 2,
      kCharPtr      = 3,
      kPoint        = 4,
      kObjectId     = 5,
      kHardOwnerId  = 6,
      kSoftOwnerId  = 7,
      kHardPtrId    = 8,
      kSoftPtrId    = 9,
      kBool         = 10,
      kVector       = 11
  };
  ODDB_DECLARE_MEMBERS(OdDbDataCell);

  // Constructors
  
  OdDbDataCell();

  /*
  OdDbDataCell(const OdDbDataCell& rCell);

  OdDbDataCell(bool b);
  OdDbDataCell(int i);
  OdDbDataCell(double d);
  OdDbDataCell(const OdString& str);
  OdDbDataCell(const OdGePoint3d& point);
  OdDbDataCell(const OdGeVector3d& vec);
  OdDbDataCell(const OdDbObjectId& id);
  OdDbDataCell(const OdDbHardOwnershipId& id);
  OdDbDataCell(const OdDbSoftOwnershipId& id);
  OdDbDataCell(const OdDbHardPointerId&   id);
  OdDbDataCell(const OdDbSoftPointerId&   id);

  virtual ~OdDbDataCell();

  */
  
  virtual void init();

  virtual void set(const OdDbDataCell& cell);
  virtual void set(bool b);
  virtual void set(OdInt32 i);
  virtual void set(double d);
  virtual void set(const OdString& str);
  virtual void set(const OdGePoint3d& pt);
  virtual void set(const OdGeVector3d& vec);
  virtual void set(const OdDbObjectId& id);
  virtual void set(const OdDbHardOwnershipId& id);
  virtual void set(const OdDbSoftOwnershipId& id);
  virtual void set(const OdDbHardPointerId& id);
  virtual void set(const OdDbSoftPointerId& id);

  virtual void get(OdDbDataCell& cell);
  virtual void get(bool& b);
  virtual void get(OdInt32& i);
  virtual void get(double& d);
  virtual void get(OdString& str);
  virtual void get(OdGePoint3d& pt);
  virtual void get(OdGeVector3d& vec);
  virtual void get(OdDbObjectId& id);
  virtual void get(OdDbHardOwnershipId& id);
  virtual void get(OdDbSoftOwnershipId& id);
  virtual void get(OdDbHardPointerId& id);
  virtual void get(OdDbSoftPointerId& id);

/*
  virtual bool operator == (const OdDbDataCell &cell) const;
  virtual bool operator != (const OdDbDataCell &cell) const;
*/

  virtual OdDbDataCell::CellType type() const;
  OdDbDataCell(OdDbDataCellImpl* pValImpl);

protected:
  friend class OdDbSystemInternals;
  OdDbDataCellImpl* m_pImpl;
};

typedef OdSmartPtr<OdDbDataCell> OdDbDataCellPtr;

typedef OdArray<OdDbDataCellPtr> OdDbDataCellArray; 

class OdDbDataColumnImpl;

/** \details
    Corresponding C++ library: TD_Db 
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDataColumn : public OdRxObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDataColumn);
  OdDbDataColumn();
  OdDbDataColumn(const OdDbDataColumn& rCol);
//  OdDbDataColumn(const OdDbDataCell::CellType type,
//       const OdString pColName = NULL);

//  virtual ~OdDbDataColumn();

  virtual OdUInt32 physicalLength() const;
  virtual OdUInt32 growLength() const;

  virtual void setPhysicalLength(OdUInt32 n);
  virtual void setGrowLength(OdUInt32 n);

//  virtual OdDbDataColumn& operator = (const OdDbDataColumn& col);

  virtual void setColumnType(OdDbDataCell::CellType type);
  virtual OdDbDataCell::CellType columnType() const;

  virtual void setColumnName(const OdString& pName);
  virtual OdString columnName() const;

  virtual OdDbDataCellPtr getCellAt(OdUInt32 index) const;
  virtual void setCellAt(OdUInt32 index, const OdDbDataCellPtr cell);

  virtual void appendCell(const OdDbDataCellPtr cell);

  virtual void insertCellAt(OdUInt32 index, const OdDbDataCellPtr& cell);
  virtual void   removeCellAt(OdUInt32 index);
  virtual OdInt32  getIndexAtCell(const OdDbDataCellPtr& cell) const;

  virtual OdUInt32 numCells() const;

  OdDbDataColumn(OdDbDataColumnImpl* m_pImpl);

protected:
  friend class OdDbSystemInternals;
  OdDbDataColumnImpl* m_pImpl;
};

typedef OdSmartPtr<OdDbDataColumn> OdDbDataColumnPtr;

class OdDbDataTableImpl;

/** \details
    Corresponding C++ library: TD_Db 
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDataTable : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDataTable);
  
  OdDbDataTable(void);
  
//  virtual OdDbDataTable& operator = (const OdDbDataTable& table);

  virtual OdUInt32 numColumns() const;
  virtual OdUInt32 numRows() const;

  virtual void setNumRowsPhysicalSize(OdUInt32 n);
  virtual void setNumRowsGrowSize(OdUInt32 n);

  virtual OdUInt32 numRowsPhysicalSize() const;
  virtual OdUInt32 numRowsGrowSize() const;

  virtual void setNumColsPhysicalSize(OdUInt32 n);
  virtual void setNumColsGrowSize(OdUInt32 n);

  virtual OdUInt32 numColsPhysicalSize() const;
  virtual OdUInt32 numColsGrowSize() const;
  
  virtual OdString tableName() const;
  virtual void setTableName(const OdString& pName);

  virtual OdDbDataColumnPtr getColumnAt(OdUInt32 index) const;
  virtual OdString columnNameAt(OdUInt32 index) const;

  virtual  OdDbDataCell::CellType columnTypeAt(OdUInt32 index) const;
  virtual  OdInt32 columnIndexAtName (const OdString& name) const;

  virtual  void appendColumn(
      OdDbDataCell::CellType type, const OdString& colName);
  virtual  void insertColumnAt(OdUInt32 index, 
      OdDbDataCell::CellType type, const OdString& colName);
  virtual  void removeColumnAt(OdUInt32 index);

  virtual  void getRowAt(OdUInt32 index, 
      OdDbDataCellArray& outRow) const;
  virtual  void setRowAt(OdUInt32 index,
      const OdDbDataCellArray& row, bool bValidate = true);

  virtual  void appendRow(
      const OdDbDataCellArray& row, bool bValidate = true);

  virtual  void insertRowAt(OdUInt32 index,
      const OdDbDataCellArray& row, bool bValidate = true);

  virtual  void removeRowAt(OdUInt32 index);

  virtual  OdDbDataCellPtr getCellAt(OdUInt32 row, OdUInt32 col) const;
  virtual  void setCellAt(OdUInt32 row, OdUInt32 col,
      const OdDbDataCellPtr& cell);

  //********************************************************************
  // Overridden methods from OdDbObject
  //********************************************************************
  //

  virtual OdResult  dwgInFields(
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  virtual OdResult  dxfInFields(
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  // virtual   audit(OdDbAuditInfo* pAuditInfo);
};

typedef OdSmartPtr<OdDbDataTable> OdDbDataTablePtr;

#include "TD_PackPop.h"

#endif
