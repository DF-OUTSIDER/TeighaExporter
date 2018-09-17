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



#ifndef OD_DBLINKEDTABLEDATA_H
#define OD_DBLINKEDTABLEDATA_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "DbTableStyle.h"
#include "DbLinkedData.h"
#include "DbDataLink.h"
#include "DbField.h"

/** \details
    Work-in-progress. This class represents LinkedTableData objects in an OdDbDatabase instance.
    This enables linking to Excel data.

    \sa
    TD_Db

	<group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbLinkedTableData : public OdDbLinkedData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbLinkedTableData);

  OdDbLinkedTableData();

  virtual void setSize(
    OdInt32 nRows, 
    OdInt32 nCols);

  virtual OdInt32 numColumns() const;

  virtual OdString getColumnName(
    OdInt32 nIndex) const;

  virtual void setColumnName(
    OdInt32 nIndex, 
    const OdString name);

  virtual OdInt32 appendColumn(
    OdInt32 nNumCols);

  virtual OdInt32 insertColumn(
    OdInt32 nIndex, 
    OdInt32 nNumCols);

  virtual void deleteColumn(
    OdInt32 nIndex, 
    OdInt32 nNumColsToDelete);

  virtual OdInt32 numRows() const;

  virtual bool canInsert(
    OdInt32 nIndex, 
    bool bRow) const;

  virtual OdInt32 appendRow(
    OdInt32 nNumRows);

  virtual OdInt32 insertRow(
    OdInt32 nIndex, 
    OdInt32 nNumRows);

  virtual bool canDelete(
    OdInt32 nIndex, 
    OdInt32 nCount, 
    bool bRow) const;

  virtual void deleteRow(
    OdInt32 nIndex, 
    OdInt32 nNumRowsToDelete);

  virtual bool isContentEditable(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdDb::CellState cellState(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual void setCellState(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellState nCellState);

  virtual OdString getToolTip(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual void setToolTip(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdString& sToolTip);

  virtual OdInt32 getCustomData(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual void setCustomData(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nData);

  virtual OdValue getCustomData(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdString& sKey ) const;

  virtual void setCustomData(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdString& sKey, 
    const OdValue* pData);

  virtual OdUInt32 numContents(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdUInt32 createContent(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nIndex);
  
  virtual void moveContent(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nFromIndex, 
    OdInt32 nToIndex);

  virtual void deleteContent(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent);

  virtual void deleteContent(
    OdInt32 nRow, 
    OdInt32 nCol);

  virtual void deleteContent(
    const OdCellRange& range);

  virtual OdDb::CellContentType contentType(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdDb::CellContentType contentType(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent) const;

  virtual void getDataType(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdValue::DataType& nDataType, 
    OdValue::UnitType& nUnitType) const;

  virtual void getDataType(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent,
    OdValue::DataType& nDataType, 
    OdValue::UnitType& nUnitType) const;

  virtual void setDataType(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdValue::DataType nDataType, 
    OdValue::UnitType nUnitType);

  virtual void setDataType(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent,
    OdValue::DataType nDataType, 
    OdValue::UnitType nUnitType);

  virtual OdString dataFormat(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdString dataFormat(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdUInt32 nContent) const;

  virtual void setDataFormat(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdString sFormat);

  virtual void setDataFormat(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent,
    const OdString sFormat);

  virtual OdValue getValue(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdValue getValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    OdValue::FormatOption nOption) const;

  virtual void setValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdValue& value);

  virtual void setValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdValue& value);

  virtual void setValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdValue& value,
    OdValue::ParseOption nOption);

  virtual OdString getText(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdString getText(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent) const;

  virtual OdString getText(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    OdValue::FormatOption nOption) const;

  virtual void setText(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdString& sText);

  virtual void setText(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdString& sText);

  virtual bool hasFormula(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent) const;

  virtual OdString getFormula(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent) const;

  virtual void setFormula(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdString sFormula);

  virtual OdDbObjectId getFieldId(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdDbObjectId getFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent) const;

  virtual void setFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idField);

  virtual void setFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdDbObjectId& idField);

  virtual OdDbFieldPtr getField(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent,
    OdDb::OpenMode mode) const;

  virtual OdDbObjectId getBlockTableRecordId(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdDbObjectId getBlockTableRecordId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent) const;

  virtual void setBlockTableRecordId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idBTR);

  virtual void setBlockTableRecordId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdDbObjectId& idBTR);
 
  virtual OdString getBlockAttributeValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idAttDef) const;
  
  virtual OdString getBlockAttributeValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdDbObjectId& idAttDef) const;
  
  virtual void setBlockAttributeValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idAttDef, 
    const OdString sAttValue);
  
  virtual void setBlockAttributeValue(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdDbObjectId& idAttDef, 
    const OdString sAttValue);

  virtual bool isLinked(
    OdInt32 nRow, 
    OdInt32 nCol) const;
  virtual OdDbObjectId getDataLink(
    OdInt32 nRow, 
    OdInt32 nCol) const;
  virtual OdDbDataLinkPtr getDataLink(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::OpenMode mode) const;
  virtual OdInt32 getDataLink(
    const OdCellRange& pRange, 
    OdDbObjectIdArray& dataLinkIds) const;
  virtual void setDataLink(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idDataLink,
    bool bUpdate);
  virtual void setDataLink(
    const OdCellRange& range, 
    const OdDbObjectId& idDataLink,
    bool bUpdate);
  virtual OdCellRange getDataLinkRange(
    OdInt32 nRow, 
    OdInt32 nCol) const;
  virtual void removeDataLink(
    OdInt32 nRow, 
    OdInt32 nCol);
  virtual void removeDataLink(void);

  virtual void updateDataLink(
    OdInt32 nRow,
    OdInt32 nCol, 
    OdDb::UpdateDirection nDir,
    OdDb::UpdateOption nOption);
  virtual void updateDataLink(
    OdDb::UpdateDirection nDir, 
    OdDb::UpdateOption nOption);

  virtual void clear();
  virtual void copyFrom(
    const OdRxObject* pSource);
  virtual void copyFrom(const OdDbLinkedTableData* pSrc,
    OdDb::TableCopyOption nOption);
  virtual void copyFrom(const OdDbLinkedTableData* pSrc, 
    OdDb::TableCopyOption nOption, 
    const OdCellRange& srcRange, 
    const OdCellRange& targetRange,
    OdCellRange* pNewTargetRange);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbLinkedTableData> OdDbLinkedTableDataPtr;

#include "TD_PackPop.h"

#endif // OD_DBLINKEDTABLEDATA_H
