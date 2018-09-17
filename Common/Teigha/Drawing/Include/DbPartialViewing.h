/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#ifndef _DBVIEWERSTREAMER_API_H_
#define _DBVIEWERSTREAMER_API_H_

#include "RxObject.h"
#include "DbIndex.h"
#include "Si/SiSpatialIndex.h"
#include "Gs/GsSpatialQuery.h"
#include "Gs/Gs.h"

#include "TD_PackPush.h"

#include "DbObjectIterator.h"
#include "DbBlockIterator.h"

/** \details
  Library:  OdDbPartialViewing
  <group OdDb_Classes>
*/
class OdDbPartialViewingIndex : public OdDbIndex {
public:
  struct Entry : OdSiEntity {
    OdGePoint3d  pt1, pt2;
    OdDbObjectId dbentity;
    OdUInt32     dataSize;

    bool extents(OdGeExtents3d& extents) const {
      extents.set(pt1, pt2);
      return extents.isValidExtents();
    }

    static Entry* cast(OdSiEntity *siEntity) {
      return static_cast<Entry*>(siEntity);
    }
  };

private:
  OdArray<Entry>  buffer;
  bool            isPlanar;
  OdSiSpatialIndexPtr si;

  void compute(OdDbBlockTableRecord* pBlock);

  void reset(OdUInt32 n, bool isPlanar = false) {
    buffer.clear();
    buffer.resize(n);
    if (si.isNull() || this->isPlanar != isPlanar)
      si = OdSiSpatialIndex::createObject(isPlanar, n);
    else
      si->clear();
    this->isPlanar = isPlanar;
  }

  void clear() {
    buffer.clear();
    if (si.get())
      si->clear();
    isPlanar = false;
  }
public:
  ODDB_DECLARE_MEMBERS(OdDbPartialViewingIndex);

  OdDbPartialViewingIndex();

  virtual OdDbFilteredBlockIteratorPtr newIterator(const OdDbFilter* pFilter) const;

  virtual void rebuildFull(OdDbIndexUpdateData* pIdxData);
  virtual void rebuildModified(OdDbBlockChangeIterator* iterator);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  void query(const OdSiShape& query, OdSiVisitor& visitor);

  inline bool getWorldExtents(OdGeExtents3d& ext) const {
    return si->extents(ext);
  }
};

typedef OdSmartPtr<OdDbPartialViewingIndex> OdDbPartialViewingIndexPtr;

/** \details
  Library:  OdDbPartialViewing.
  <group OdDb_Classes>
*/
class OdDbPartialViewingFilter : public OdDbFilter {
  OdGsViewPtr view;
public:
  ODDB_DECLARE_MEMBERS(OdDbPartialViewingFilter);

  OdDbPartialViewingFilter();

  virtual OdRxClass* indexClass() const;

  void set(const OdGsView* view);

  const OdGsView* get() const;
};

typedef OdSmartPtr<OdDbPartialViewingFilter> OdDbPartialViewingFilterPtr;


/** \details
  Library:  OdDbPartialViewing
  <group OdDb_Classes>
*/
class OdDbPartialViewingModule : public OdRxModule {
public:
  class DataManagerContext;

  /** \details
    Attaches Db/Gs data manager that filters (unloads/pages/markup/erase) offscreen data
    when entire drawing file is too big to be loaded completely into run-time.

    \returns
    Returns OdGsDevice-based wrapper that implements entity filtering.

    \param gs [in]  Input GS to be managed.
    \param queryFilter [in, optional]  interface object for filtering control.
    \param mgrCtx [in, optional]  hook for custom data management.

    \sa
    *  OdDbPartialViewingFilter
    *  OdDbPartialViewingIndex
    *  OdGsDevice
  */
  virtual OdGsDevicePtr attachDataManagerTo(OdGsDevice* gs, DataManagerContext* mgrCtx) = 0;

  class DataManagerContext : public OdRxObject {
  public:
    virtual OdDbObjectIteratorPtr currentSet() = 0;
    virtual void manageData(OdDbObjectIterator& willBeShown, OdDbObjectIterator& mayBeUnloaded) = 0;
  };
  typedef OdSmartPtr<DataManagerContext> DataManagerContextPtr;

  virtual void filterViewingData(OdGsDevice* filterSettings, 
    DataManagerContext* dataManager, OdArray<OdDbFilterPtr>* pFilters = 0) = 0;

  virtual void addIndex(OdGsDevice* filterSettings) = 0;

  virtual void addIndex(OdDbBlockTableRecord* pBTR) = 0;

  virtual OdDbObjectIteratorPtr getUnloadedObjects(OdDbObjectIterator& objects) = 0;
};

typedef OdSmartPtr<OdDbPartialViewingModule> OdDbPartialViewingModulePtr;


#include "TD_PackPop.h"

#endif // _DBVIEWERSTREAMER_API_H_
