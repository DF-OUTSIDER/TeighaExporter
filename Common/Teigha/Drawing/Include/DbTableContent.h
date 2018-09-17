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




#ifndef OD_DBTABLECONTENT_H
#define OD_DBTABLECONTENT_H

#include "DbObject.h"
#include "DbFormattedTableData.h"

#include "TD_PackPush.h"

/** \details
  Work-in-progress. This class represents TableContent objects in an OdDbDatabase instance.

  \sa
  TD_Db

	<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbTableContent : public OdDbFormattedTableData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbTableContent);

  OdDbTableContent();

  /** \details
    Returns the object ID of the OdDbTableStyle.
  */
  virtual OdDbObjectId tableStyleId() const;

  /** \details
    Sets the object ID of the OdDbTableStyle.

    \param idTableStyle [in]  Object ID of the table style.
  */
  virtual void setTableStyleId(
    const OdDbObjectId& idTableStyle);

  /** \details
    Returns the cell style as an OdString value.
    Use a valid row index and pass column index -1 to get the style for a row.
    Use a valid column index and pass row index -1 to get the style for a column.

    \param nRow [in]  Row index of the cell. 
    \param nCol [in]  Column index of the cell. 

    \remarks
    Cell style should be one of the following values:
    
    <table>
    Name
    _TITLE  
    _HEADER
    _DATA
    </table>
  */
  virtual const OdString cellStyle(
    OdInt32 nRow, 
    OdInt32 nCol) const;

  /** \details
    Sets the cell style.
    Use a valid row index and pass column index -1 to set the style for a row.
    Use a valid column index and pass row index -1 to set the style for a column.

    \param nRow [in]  Row index of the cell. 
    \param nCol [in]  Column index of the cell.   
    \param pszCellStyle [in]  Cell style.   

    \remarks
    Cell style should be one of the following values:
    
    <table>
    Name
    _TITLE  
    _HEADER
    _DATA
    </table>
  */
  virtual void setCellStyle(
    OdInt32 nRow, 
    OdInt32 nCol, 
    const OdString& pszCellStyle);

  /** \details
    Returns the height of the specified row.
    
    \param nRow [in]  Row index.
  */
  virtual double rowHeight(
    OdInt32 nRow) const;

  /** \details
    Sets the height of the specified row.

    \param nRow [in]  Row index.
    \param fHeight [in]  Row height.

    \remarks
    Method generates the eInvalidIndex exception when there is no row with specified index in the table.
  */    
  virtual void setRowHeight(
    OdInt32 nRow, 
    double fHeight);

  /** \details
    Returns the width of the specified column.
    
    \param nCol [in]  Column index. 
  */
  virtual double columnWidth(
    OdInt32 nCol) const;

  /** \details
    Sets the width of the specified column.

    \param nCol [in]  Column index. 
    \param fWidth [in]  Column width.

    \remarks
    Method generates the eInvalidIndex exception when there is no column with specified index in the table.
  */
  virtual void setColumnWidth(
    OdInt32 nCol, 
    double fWidth);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class template for OdDbTableContent object pointers.
*/
typedef OdSmartPtr<OdDbTableContent> OdDbTableContentPtr;

#include "TD_PackPop.h"

#endif // OD_DBTABLECONTENT_H
