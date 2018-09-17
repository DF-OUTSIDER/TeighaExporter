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



#ifndef OD_DBFORMATTEDTABLEDATA_H
#define OD_DBFORMATTEDTABLEDATA_H

#include "DbObject.h"
#include "DbLinkedTableData.h"

#include "TD_PackPush.h"

	/** \details
    Work-in-progress. This class represents FormattedTableData objects in an OdDbDatabase instance.

    \sa
    TD_Db

	<group OdDb_Classes>
	*/
class DBENT_EXPORT OdDbFormattedTableData : public OdDbLinkedTableData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbFormattedTableData);

  OdDbFormattedTableData();

  virtual OdInt32 insertRowAndInherit(
    OdInt32 nIndex, 
    OdInt32 nInheritFrom,
    OdInt32 nNumRows);

  virtual OdInt32 insertColumnAndInherit(
    OdInt32 nIndex, 
    OdInt32 nInheritFrom,
    OdInt32 nNumCols);

  virtual void setFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idField, 
    OdDb::CellOption nFlag);

  virtual void setFieldId(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdUInt32 nContent, 
    const OdDbObjectId& idField, 
    OdDb::CellOption nFlag);

  virtual void merge(
    const OdCellRange& range);

  virtual void unmerge(
    const OdCellRange& range);

  virtual bool isMerged(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdCellRange getMergeRange(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual bool isMergeAllEnabled(
    OdInt32 nRow, 
    OdInt32 nCol) const;
  
  virtual void enableMergeAll(
    OdInt32 nRow, 
    OdInt32 nCol,
    bool bEnable);

  virtual bool isFormatEditable(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual double rotation(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual double rotation(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  virtual void setRotation(
    OdInt32 nRow, 
    OdInt32 nCol, 
    double fRotation);

  virtual void setRotation(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    double fRotation);

  virtual double scale(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual double scale(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  virtual void setScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    double fScale);

  virtual void setScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    double fScale);

  virtual bool isAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual bool isAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  virtual void setAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    bool bAutoScale);

  virtual void setAutoScale(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    bool bAutoScale);

  virtual OdDb::CellAlignment alignment(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual void setAlignment(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellAlignment nAlignment);

  virtual OdCmColor contentColor(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdCmColor contentColor(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  virtual void setContentColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdCmColor& color);

  virtual void setContentColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    const OdCmColor& color);

  virtual OdDbObjectId textStyle(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual OdDbObjectId textStyle(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  virtual void setTextStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdDbObjectId& idTextStyle);

  virtual void setTextStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent,
    const OdDbObjectId& idTextStyle);

  virtual double textHeight(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual double textHeight(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent) const;

  virtual void setTextHeight(
    OdInt32 nRow, 
    OdInt32 nCol, 
    double fTextHeight);

  virtual void setTextHeight(
    OdInt32 nRow, 
    OdInt32 nCol,
    OdInt32 nContent,
    double fTextHeight);

  virtual OdCmColor backgroundColor(OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual void setBackgroundColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdCmColor& color);

  virtual OdDb::CellContentLayout contentLayout(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  virtual void setContentLayout(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellContentLayout nLayout);

  virtual OdDb::FlowDirection flowDirection() const;
  
  virtual void setFlowDirection(
    OdDb::FlowDirection nDir);

  virtual double margin(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellMargin nMargin) const;

  virtual void setMargin(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::CellMargin nMargins, 
    double fMargin);

  virtual OdDb::GridLineStyle gridLineStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setGridLineStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes,
    OdDb::GridLineStyle nLineStyle);

  virtual OdDb::LineWeight gridLineWeight(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setGridLineWeight(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::LineWeight nLineWeight);

  virtual OdDbObjectId gridLinetype(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setGridLinetype(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    const OdDbObjectId& idLinetype);

  virtual OdCmColor gridColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setGridColor(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    const OdCmColor& color);

  virtual OdDb::Visibility gridVisibility(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setGridVisibility(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    OdDb::Visibility nVisibility);

  virtual double gridDoubleLineSpacing(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setGridDoubleLineSpacing(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    double fSpacing);

  virtual void getGridProperty(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType, 
    OdGridProperty& gridProp) const;

  virtual void setGridProperty(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineTypes, 
    const OdGridProperty& gridProp);

  virtual void setGridProperty(
    const OdCellRange& range,
    OdDb::GridLineType nGridLineTypes, 
    const OdGridProperty& gridProp);

  virtual OdDb::CellProperty getOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent) const;

  virtual OdDb::GridProperty getOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType) const;

  virtual void setOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdInt32 nContent, 
    OdDb::CellProperty nOverride);
  
  virtual void setOverride(
    OdInt32 nRow, 
    OdInt32 nCol, 
    OdDb::GridLineType nGridLineType, 
    OdDb::GridProperty nOverride);

  virtual void removeAllOverrides(
    OdInt32 nRow, 
    OdInt32 nCol);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbFormattedTableData> OdDbFormattedTableDataPtr;

#include "TD_PackPop.h"

#endif // OD_DBFORMATTEDTABLEDATA_H
