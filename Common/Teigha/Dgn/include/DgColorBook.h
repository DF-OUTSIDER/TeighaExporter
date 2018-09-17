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
#ifndef __DG_COLORBOOK_H__
#define __DG_COLORBOOK_H__

#include "DgApplicationData.h"
#include "DgElementIterator.h"


class OdDgColorBook;
typedef OdSmartPtr<OdDgColorBook> OdDgColorBookPtr;

//--------------------------------------------------------------

struct OdDgColorBookItem
{
  OdString   m_strColorName;
  ODCOLORREF m_clrColor;
};

//--------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgColorBook : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgColorBook);

public:

  enum
  {
    kSignature = 0x583C
  };

public:

  virtual OdString getColorBookName() const;
  virtual void     setColorBookName( const OdString& strName );

  virtual OdUInt32 getItemCount() const;

  virtual OdDgColorBookItem getColorBookItem( OdUInt32 uIndex );
  virtual void setColorBookItem( OdUInt32 uIndex, const OdDgColorBookItem& item );
  virtual void addColorBookItem( const OdDgColorBookItem& item );
  virtual void deleteColorBookItem( OdUInt32 uIndex );
};


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgColorBookTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgColorBookTable);
public:

  void add( OdDgElementPtr pColorBook);

  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  OdDgElementId getAt( const OdString& strName );
};

typedef OdSmartPtr<OdDgColorBookTable> OdDgColorBookTablePtr;

#endif // __DG_COLORBOOK_H__
