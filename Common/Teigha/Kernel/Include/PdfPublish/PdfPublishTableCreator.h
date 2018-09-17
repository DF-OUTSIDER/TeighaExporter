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


///////////////////////////////////////////////////////////////////////////////
//
// OdTableCreator.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_TABLE_CREATOR_INCLUDED_
#define _PDF_PUBLISH_TABLE_CREATOR_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishTable.h"
#include "UInt32Array.h"
#include "DoubleArray.h"
#define STL_USING_MAP
#include "OdaSTL.h"


namespace OdPdfPublish{

/** \details
  This class implements the PDF publish.
*/
  class OdTableCreatorImpl;

class PDFPUBLISH_TOOLKIT OdTableCreator : public OdRxObject
{

  friend class OdTableCreatorImpl;
  OdTableCreatorImpl* m_pImpl;
protected:
  OdTableCreator();

public:
  ODRX_DECLARE_MEMBERS(OdTableCreator);
  ODRX_HEAP_OPERATORS();

  bool isEmpty() const;
  void clear();
  bool isValid() const;


  virtual ~OdTableCreator();

  void setColumnCount(const OdUInt32 column_count);
  void setRowCount(const OdUInt32 row_count);
  void setParticularColumnWidth(const OdUInt32 col, const OdUInt32 column_width);
  void setParticularRowHeight(const OdUInt32 row, const OdUInt32 row_height);
  void setCellFillColor(const OdUInt32 row, const OdUInt32 column, const ODCOLORREF fill_color);
  void setCellBorderColor(const OdUInt32 row, const OdUInt32 column, const ODCOLORREF border_color);
  void setCellBorderThickness(const OdUInt32 row, const OdUInt32 column, const Border::Thickness border_thickness);


  void getColumnCount(OdUInt32& column_count) const;
  void getRowCount(OdUInt32& row_count) const;
  bool getParticularColumnWidth(const OdUInt32 col, OdUInt32& column_width) const;
  bool getParticularRowHeight(const OdUInt32 row, OdUInt32& row_height) const;
  void getCellFillColor(const OdUInt32 row, const OdUInt32 column, ODCOLORREF& fill_color) const;
  void getCellBorderColor(const OdUInt32 row, const OdUInt32 column, ODCOLORREF& border_color) const;
  void getCellBorderThickness(const OdUInt32 row, const OdUInt32 column, Border::Thickness& border_thickness) const;

  OdTablePtr createTable() const;

};

/** \details
  Implements the smart pointer to the PDF publish.
*/
SMARTPTR(OdTableCreator);

}
#endif // _PDF_PUBLISH_TABLE_CREATOR_INCLUDED_

