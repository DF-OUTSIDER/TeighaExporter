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
#ifndef __DG_FONTTABLERECORD_H__
#define __DG_FONTTABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"
#include "OdCodePage.h"
#include "OdFont.h"

/** \details
    This class represents Font records in the OdDgFontTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgFontTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgFontTableRecord);
public:

  OdUInt32 getNumber() const;

  /** \details
    Sets a font type.

    \param type [in]  the type of font used. Use: kFontTypeTrueType for TrueType fonts,
                                         kFontTypeShape for Shapefile fonts.

    \remarks
    Must be called before adding the record to the Font table.
    It's unnecessary to add RSC (MicroStation Recource file) font.
  */
  virtual void setType(OdFontType type);
  /** \details
    Gets a font type.

    Returns the type of font used.
  */
  virtual OdFontType getType() const;

  virtual OdString getAlternateName() const;
  // TODO
  //virtual void setAlternateName(const OdString& name);
  //virtual OdCodePageId getCodePageId() const;
  //virtual void setCodePageId(OdCodePageId);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgFontTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgFontTableRecord> OdDgFontTableRecordPtr;

/** \details
This class holds the matches between EntryId of fonts and their names.
It is useful for V7 files that do not store any font tables (OdCharMapper holds the matches based on the names of fonts).

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgV7FontsMapper
{
public:
  static OdResult addFontName( OdUInt8 entryId, const OdString& name );
  static OdString getFontName( OdUInt8 entryId );
  static void removeAllMatches();
};

#include "TD_PackPop.h"

#endif // __DG_FONTTABLERECORD_H__
