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
#ifndef __DG_LEVELFILTERTABLERECORD_H__
#define __DG_LEVELFILTERTABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"
#include "DgTable.h"
#include "DgLevelTableRecord.h"

/** \details
This class represents Level Filter records in the OdDgLevelFilterTable in an OdDgDatabase instance.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLevelFilterTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLevelFilterTableRecord);
public:

  enum OdDgLevelFilterType
  {
    kSystem = 1,
    kUser   = 2
  };

public:

  virtual void setOwnerId( OdDgElementId ownerId );
  virtual OdUInt32 getEntryId() const;

  virtual OdUInt32 getParentId() const;
  virtual void setParentId(OdUInt32 uId);

  virtual OdDgLevelFilterType getFilterType() const;
  virtual void setFilterType( OdDgLevelFilterType uType );

  virtual bool getComposeOrFlag() const;
  virtual void setComposeOrFlag( bool bSet );

  virtual OdUInt32 getFilterMemberCount() const;
  virtual OdString getFilterMemberName( OdUInt32 uIndex ) const;
  virtual OdDgLevelFilterTable::OdDgFilterMemberType getFilterMemberType( OdUInt32 uIndex ) const;
  virtual OdString getFilterMemberExpression( OdUInt32 uIndex ) const;
  virtual OdString getFilterMemberExpression( const OdString& strMemberName ) const;
  virtual bool     setFilterMemberExpression( OdUInt32 uIndex, const OdString& strExpression );
  virtual bool     setFilterMemberExpression( const OdString& strMemberName, const OdString& strExpression );

  virtual bool     filter( const OdDgLevelTableRecordPtr pLevel ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLevelFilterTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLevelFilterTableRecord> OdDgLevelFilterTableRecordPtr;

/** \details
This class represents Level Filter that check level group and level name filter members.

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLevelNameAndGroupFilter : public OdDgLevelFilterTableRecord
{

public:
  OdDgLevelNameAndGroupFilter();

// Support of layer name filter
  bool      isNameFilterEnable() const;
  void      clearNameFilter();
  OdString  getNameFilterExpression() const; 
  void      setNameFilterExpression( const OdString& strFilter );

// Support of layer group filter
  bool      isGroupFilterEnable() const;
  void      clearGroupFilter();
  OdUInt32  getLevelCount() const;
  OdDgLevelTableRecordPtr  getLevelFromGroup( OdUInt32 uIndex, OdDg::OpenMode openMode ) const;
  void      addLevelToGroup( OdDgLevelTableRecordPtr pLevel );
  void      deleteLevelFromGroup( OdUInt32 uIndex );
  void      deleteLevelFromGroup( OdDgLevelTableRecordPtr pLevel );

  virtual bool filter( const OdDgLevelTableRecordPtr pLevel ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLevelFilterTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLevelNameAndGroupFilter> OdDgLevelNameAndGroupFilterPtr;

#include "TD_PackPop.h"

#endif // __DG_LEVELFILTERTABLERECORD_H__
