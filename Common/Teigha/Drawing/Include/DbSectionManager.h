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


#ifndef _DbSectionManager_h_Included_
#define _DbSectionManager_h_Included_

#include "DbSection.h"
#include "TD_PackPush.h"


class OdDbSection;
class OdDbSectionManagerIterator;
typedef OdSmartPtr<OdDbSectionManagerIterator> OdDbSectionManagerIteratorPtr;


//*************************************************************************
// OdDbSectionManager
//*************************************************************************

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSectionManager : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSectionManager);

  OdDbObjectId getSection(const OdString& pszName ) const;
  OdDbSectionPtr getSection(const OdString& pszName, OdDb::OpenMode mode) const;
  OdDbObjectId getLiveSection() const;
  OdDbSectionPtr getLiveSection(OdDb::OpenMode mode) const;
  int numSections() const;
  OdDbSectionManagerIteratorPtr newIterator () const;
  OdResult getUniqueSectionName(const OdString& pszBaseName, 
    OdString& pszUniqueName) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;
protected:
  OdDbSectionManager();
};

typedef OdSmartPtr<OdDbSectionManager> OdDbSectionManagerPtr;

//*************************************************************************
// OdDbSectionManagerIterator
//*************************************************************************

class OdDbSectionManagerIteratorImpl;

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSectionManagerIterator : public OdRxObject
{
  OdDbSectionManagerIteratorImpl* m_pImpl;
  friend class OdDbSectionManager;
public:
  ODRX_DECLARE_MEMBERS(OdDbSectionManagerIterator);
  OdDbSectionManagerIterator();
  ~OdDbSectionManagerIterator();

  void  start();
  void  step();
  bool  done() const;

  OdDbObjectId getSection() const;
  OdDbSectionPtr getSection(OdDb::OpenMode openMode) const;
};

#include "TD_PackPop.h"
#endif
