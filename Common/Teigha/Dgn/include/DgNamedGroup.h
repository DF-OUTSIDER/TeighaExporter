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
#ifndef __DG_NAMEDGROUP_H__
#define __DG_NAMEDGROUP_H__


#include "TD_PackPush.h"
#include "DgElement.h"

/** \details
  Common interface for named group element.

  Corresponding C++ library: TG_Db 
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgNamedGroupItem
{
public:

  enum OdDgNamedGroupPropagateType
  {
    kApplyIfGroupLockOn = 0,
    kApplyNever         = 1,
    kApplyAlways        = 2
  };

public:

  OdDgNamedGroupItem();
  virtual ~OdDgNamedGroupItem();

  OdUInt32 getFlags() const;
  void setFlags( OdUInt32 uFlags );

  OdDgNamedGroupPropagateType getForwardPropagate()  const;
  void setForwardPropagate( OdDgNamedGroupPropagateType iType );

  OdDgNamedGroupPropagateType getBackwardPropagate() const;
  void setBackwardPropagate( OdDgNamedGroupPropagateType iType );

  OdDgNamedGroupPropagateType getGroupPropagate()    const;
  void setGroupPropagate( OdDgNamedGroupPropagateType iType );

  bool isForwardPropagateDefault() const;
  bool isBackwardPropagateDefault() const;
  bool isGroupPropagateDefault() const;

  OdUInt32 getPathLength() const;

  void     getPath( OdArray<OdUInt64>& arrIdPath ) const;
  void     setPath( const OdArray<OdUInt64>& arrIdPath );

private:

  OdArray<OdUInt64> m_arrIdPath;
  OdUInt32      m_uFlags;
  bool          m_bDefaultForwardPropagate;
  bool          m_bDefaultBackwardPropagate;
  bool          m_bDefaultGroupPropagate;

};

// Named group header

/** \details

  Corresponding C++ library: TG_Db 
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgNamedGroupHeader : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgNamedGroupHeader );

public:

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdDgNamedGroupItem::OdDgNamedGroupPropagateType getDefaultForwardPropagate()  const;
  virtual void setDefaultForwardPropagate( OdDgNamedGroupItem::OdDgNamedGroupPropagateType iType );

  virtual OdDgNamedGroupItem::OdDgNamedGroupPropagateType getDefaultBackwardPropagate() const;
  virtual void setDefaultBackwardPropagate( OdDgNamedGroupItem::OdDgNamedGroupPropagateType iType );

  virtual OdDgNamedGroupItem::OdDgNamedGroupPropagateType getDefaultGroupPropagate()    const;
  virtual void setDefaultGroupPropagate( OdDgNamedGroupItem::OdDgNamedGroupPropagateType iType );

  virtual bool getAllowDuplicatesFlag() const;
  virtual void setAllowDuplicatesFlag( bool bSet );

  virtual bool getExclusiveMembersFlag() const;
  virtual void setExclusiveMembersFlag( bool bSet );

  virtual bool getAllowFarReferencesFlag() const;
  virtual void setAllowFarReferencesFlag( bool bSet );

  virtual bool getClosedGroupFlag() const;
  virtual void setClosedGroupFlag( bool bSet );

  virtual bool getSelectMembersFlag() const;
  virtual void setSelectMembersFlag( bool bSet );

  virtual bool getAnonimusFlag() const;
  virtual void setAnonimusFlag( bool bSet );

  virtual bool getHasOtherGroupFlag() const;
  virtual void setHasOtherGroupFlag( bool bSet);

  virtual bool getHasAlwaysPropagateMembersFlag() const;
  virtual void setHasAlwaysPropagateMembersFlag( bool bSet);

  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  virtual OdString getDescription() const;
  virtual void     setDescription( const OdString& strDescription );

  virtual OdUInt32 getNamedGroupItemCount() const;

  virtual bool addNamedGroupItem( const OdDgNamedGroupItem& item );
  virtual OdDgNamedGroupItem getNamedGroupItem( OdUInt32 uIndex ) const;
  virtual bool setNamedGroupItem( const OdDgNamedGroupItem& item,  OdUInt32 uIndex );
  virtual bool deleteNamedGroupItem( OdUInt32 uIndex );
};

typedef OdSmartPtr<OdDgNamedGroupHeader> OdDgNamedGroupHeaderPtr;

#include "TD_PackPop.h"

#endif // __DG_NAMEDGROUP_H__
