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
#ifndef __DG_LINESTYLETABLERECORD_H__
#define __DG_LINESTYLETABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"

/** \details
  
    <group OdDg_Classes>
*/
namespace OdDg
{
  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum LineStyleType
  {
    kLsTypeInternal      = 0,
    kLsTypePointSymbol   = 1,
    kLsTypeCompound      = 2,
    kLsTypeLineCode      = 3,
    kLsTypeLinePoint     = 4,

    kLsTypeUndefined   = -1
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum LineStyleUnitsType
  {
    kLsMasterUnits     = 0,
    kLsUORS            = 1,
    kLsDeviceUnits     = 2
  };
}

/** \details
    This class represents LineStyle records in the OdDgLineStyleTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLineStyleTableRecord);
public:

  // For rsc line styles it's rsc line style def number. 
  OdUInt32 getEntryId() const;

  bool getRefersToElementFlag() const;
  void setRefersToElementFlag(bool val);

  bool getSnappableFlag() const;
  void setSnappableFlag(bool val);

  bool getPhysicalFlag() const;
  void setPhysicalFlag(bool val);

  OdDg::LineStyleUnitsType getUnitsType() const;
  void setUnitsType(OdDg::LineStyleUnitsType val);

  bool getNoSnapFlag() const;
  void setNoSnapFlag(bool val);

  bool getContinuousFlag() const;
  void setContinuousFlag(bool val);

  bool getNoRangeFlag() const;
  void setNoRangeFlag(bool val);

  bool getSharedCellScaleIndependentFlag() const;
  void setSharedCellScaleIndependentFlag(bool val);

  bool getNoWidthFlag() const;
  void setNoWidthFlag(bool val);

  OdDgElementId getRefersToId() const;
  void setRefersToId(const OdDgElementId& val);

  OdDg::LineStyleType getType() const;
  void setType(OdDg::LineStyleType val);

  OdString getRscFileName() const;
  void     setRscFileName( const OdString& strFileName );

  // OdGiDrawable overridden
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  void updateLineStyleData();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLineStyleTableRecord> OdDgLineStyleTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_LINESTYLETABLERECORD_H__
