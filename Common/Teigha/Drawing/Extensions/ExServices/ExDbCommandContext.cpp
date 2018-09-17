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

#include "StdAfx.h"
#include "DbUserIO.h"
#include "DbDatabase.h"
#include "DbBaseDatabase.h"
#include "DbHostAppServices.h"
#include "ExDbCommandContext.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeLineSeg2d.h"
#include "DbLine.h"
#include "DbDictionary.h"
#include "DbColor.h"
#include "StaticRxObject.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiUtils.h"
#include "OdDToStr.h"
#include "Gs/Gs.h"
#include "DbViewport.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTableRecord.h"
#include "Ed/EdLispEngine.h"
#include "ExTrackers.h"
#include "ExKWIndex.h"

#include <math.h>

#define DBCC_PAGE_EACH_OBJECT 200

class PointToDistTracker : public OdEdPointTracker
{
  OdGePoint3d       m_basePoint;
  OdEdRealTracker*  m_pTracker;
  int               m_options;
  OdRxObject*       m_pRxDb;
  mutable OdGsView* m_pGsView;
public:
  static OdEdPointTrackerPtr create(const OdGePoint3d& basePoint, int options, OdEdRealTracker* pTracker, 
    OdRxObject* pRxDb)
  {
    OdSmartPtr<PointToDistTracker> pRes = OdRxObjectImpl<PointToDistTracker>::createObject();
    pRes->m_basePoint = basePoint;
    pRes->m_options = options;
    pRes->m_pTracker = pTracker;
    pRes->m_pRxDb = pRxDb;
    pRes->m_pGsView = NULL;
    return pRes;
  }

  double getDistByOptions(const OdGePoint3d& pt) const
  {
    OdDbDatabase* pDb = OdDbDatabase::cast(m_pRxDb).get();

    OdGeVector3d vect = pt - m_basePoint;
    if (GETBIT(m_options, OdEd::kGdsSignedDist) && pDb)
      vect = pDb->formatter().toUCS(pt) - pDb->formatter().toUCS(m_basePoint);
    if (GETBIT(m_options, OdEd::kGds2d))
      vect.z = 0;
    double dist = vect.length();
    if (!pDb || !m_pGsView || !GETBIT(m_options, OdEd::kGdsPerpDist | OdEd::kGdsSignedDist))
      return dist;

    OdGeVector3d vUcsDirection(pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
    OdGePoint3d ptPos = m_pGsView->position(),
      ptTarget = m_pGsView->target();
    OdGeVector3d vDirection = ptPos - ptTarget;

    if (GETBIT(m_options, OdEd::kGdsPerpDist)) // is in perpendicular plane to UCS one
    {
      vUcsDirection.normalize();
      vDirection.normalize();
      double dAng = vDirection.angleTo(vUcsDirection),
        dCoef = tan(dAng);
      if (!OdZero(dCoef))
        dist /= dCoef;
    }

    if (GETBIT(m_options, OdEd::kGdsSignedDist))
    {
      OdGeVector3d vTst = pDb->getUCSXDIR() + pDb->getUCSYDIR();
      if ((vDirection.dotProduct(vTst) < 0.0) == (vect.dotProduct(vTst) < 0.0))
        dist = - dist;
    }
    return dist;
  }

  void setValue(const OdGePoint3d& pt)
  {
    m_pTracker->setValue(getDistByOptions(pt));
  }

  int addDrawables(OdGsView* pView) 
  { 
    m_pGsView = pView;
    return m_pTracker->addDrawables(pView); 
  }
  void removeDrawables(OdGsView* pView) 
  { 
    //m_pGsView = NULL;
    m_pTracker->removeDrawables(pView); 
  }
};

class EnhRectFrame : public RectFrame
{
protected:
  bool m_bApplicable;
  EnhRectFrame() : m_bApplicable(false) {}
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const ODRX_OVERRIDE
  {
    if (m_bApplicable)
    {
      pVd->subEntityTraits().setTransparency(OdCmTransparency(0.5));
      OdGePoint3d pt0 = pVd->viewport().getWorldToEyeTransform() * m_pts[0];
      OdGePoint3d pt2 = pVd->viewport().getWorldToEyeTransform() * m_pts[2];
      if (pt2.x < pt0.x)
        pVd->subEntityTraits().setColor(3);
      else
        pVd->subEntityTraits().setColor(5);
      pVd->subEntityTraits().setFillType(kOdGiFillAlways);
      OdGiDrawFlagsHelper _dfsh(pVd->subEntityTraits(), OdGiSubEntityTraits::kDrawSolidFill | OdGiSubEntityTraits::kDrawPolygonFill);
      RectFrame::subViewportDraw(pVd);
    } else RectFrame::subViewportDraw(pVd);
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3d& base, OdGsModel* pModel)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<EnhRectFrame, OdEdPointTracker>::createObject();
    static_cast<EnhRectFrame*>(pRes.get())->m_pts[0] = base;
    static_cast<EnhRectFrame*>(pRes.get())->setGsModel(pModel);
    return pRes;
  }
  int addDrawables(OdGsView* pView)
  {
    m_bApplicable = (pView && pView->device() && !pView->device()->properties().isNull() &&
                     pView->device()->properties()->has(OD_T("BlendingMode")));
    return RectFrame::addDrawables(pView);
  }
};

inline OdString next(OdString& list, const OdChar* delim = (const OdChar*)L",")
{
  OdString res = list.spanExcluding(delim);
  if(res.getLength() != list.getLength())
    list = list.mid(res.getLength()+1);
  else
    list.empty();
  return res;
}

OdRxDictionaryPtr ExDbCommandContext::arbDataDic() const
{
  if(m_pArbDataDic.isNull())
    m_pArbDataDic = odrxCreateRxDictionary();
  return m_pArbDataDic;
}

void ExDbCommandContext::setParam(OdRxObject* pParamObj)
{
  setArbitraryData(OD_T("ExDbCommandContext_FuncParamObj"), pParamObj);
}

OdRxObjectPtr ExDbCommandContext::param()
{
  return arbitraryData(OD_T("ExDbCommandContext_FuncParamObj"));
}

void ExDbCommandContext::setResult(OdRxObject* pResultObj)
{
  setArbitraryData(OD_T("ExDbCommandContext_FuncResultObj"), pResultObj);
}

OdRxObjectPtr ExDbCommandContext::result()
{
  return arbitraryData(OD_T("ExDbCommandContext_FuncResultObj"));
}

void ExDbCommandContext::setArbitraryData(const OdString& szPathName, OdRxObject* pDataObj)
{
  if (pDataObj)
    arbDataDic()->putAt(szPathName, pDataObj);
  else
    arbDataDic()->remove(szPathName);

}

OdRxObjectPtr ExDbCommandContext::arbitraryData(const OdString& szPathName) const
{
  return arbDataDic()->getAt(szPathName);
}

OdSelectionSetPtr ExDbCommandContext::pickfirst()
{
  return arbitraryData(OD_T("ExDbCommandContext_pickfirst"));
}

void ExDbCommandContext::setPickfirst(OdSelectionSet* pSSet)
{
  setArbitraryData(OD_T("ExDbCommandContext_pickfirst"), pSSet);
}

OdSelectionSetPtr ExDbCommandContext::previousSelection()
{
  return arbitraryData(OD_T("ExDbCommandContext_PreviousSSet"));
}

void ExDbCommandContext::setPreviousSelection(OdSelectionSet* pSSet)
{
  setArbitraryData(OD_T("ExDbCommandContext_PreviousSSet"), pSSet);
}

OdUnitsFormatter& ExDbCommandContext::baseFormatter()
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(baseDatabase());
  //ODA_ASSERT_ONCE(pDbPE.get());
  if (!pDbPE.get())
    throw OdError(eNoDatabase); 
  return *pDbPE->baseFormatter(baseDatabase());
}

OdDbUnitsFormatter& ExDbCommandContext::formatter()
{
  return database()->formatter();
}

OdDbCommandContextPtr ExDbCommandContext::createObject(OdEdBaseIO* pIOStream, OdDbDatabase* pDb) {
  return ExDbCommandContext::createObject(pIOStream, static_cast<OdRxObject*>(pDb));
}

OdDbCommandContextPtr ExDbCommandContext::createObject(OdEdBaseIO* pIOStream, OdRxObject* pRxDb)
{
  OdDbCommandContextPtr pRes = OdRxObjectImpl<ExDbCommandContext, OdDbCommandContext>::createObject();
  ExDbCommandContext* pDbCmdCtx = static_cast<ExDbCommandContext*>(pRes.get());

  pDbCmdCtx->m_pDb = pRxDb;
  pDbCmdCtx->m_pIoStream = pIOStream;
  pDbCmdCtx->m_pOwner = NULL;
  return pRes;
}

// Cloud TxHost support
void ExDbCommandContext::reset(OdEdBaseIO* pIOStream, OdRxObject* pRxDb)
{
  m_pIoStream = pIOStream;
  m_pDb = pRxDb;
}
OdEdBaseIO* ExDbCommandContext::baseIO()
{
  return m_pIoStream.get();
}
OdEdCommandContextPtr ExDbCommandContext::cloneObject(OdEdBaseIO* pIOStream, // = NULL
                                                      OdRxObject* pRxDb) // = NULL
{
  OdDbCommandContextPtr pRes = OdRxObjectImpl<ExDbCommandContext, OdDbCommandContext>::createObject();
  ExDbCommandContext* pDbCmdCtx = static_cast<ExDbCommandContext*>(pRes.get());
  pDbCmdCtx->m_pDb = pRxDb ? pRxDb : m_pDb;
  pDbCmdCtx->m_pIoStream = pIOStream ? pIOStream : m_pIoStream.get();
  pDbCmdCtx->m_pOwner = this;
  return pRes;
}

OdRxObject* ExDbCommandContext::baseDatabase()
{
  return m_pDb;
}

OdSelectionSetPtr ExDbCommandContext::box(const OdGePoint3d& pt1, OdDbVisualSelection::SubentSelectionMode ssm)
{
  OdGePoint3d pts[2] = {
    pt1,
    dbUserIO()->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|OdEd::kGptRectFrame)
  };

  return OdDbSelectionSet::select(database()->activeViewportId(), 2, pts, OdDbVisualSelection::kBox, ssm);
}

/*
bool pointInsideSeg(const OdGePoint2d& point, const OdGeLineSeg2d& seg)
{
  return !(point.isEqualTo(seg.startPoint()) || point.isEqualTo(seg.endPoint()));
}

void OdExSelectCmd::appendSelectPoint(const OdGePoint2d& point)
{
  if((m_interactionMode != kSelectFence) && (m_selectPoints.size() > 2))
  {
    // self-intersecting countours can't be passed as arguments for window or crossing selection
    OdGePoint2d pt(point.x, point.y);
    OdGeLineSeg2d seg1(OdGePoint2d(m_selectPoints[0].x, m_selectPoints[0].y), pt), 
                  seg2(OdGePoint2d(m_selectPoints.last().x, m_selectPoints.last().y), pt);
    OdUInt32 i;
    bool bIntersFound(false);
    for(i = 0; i < m_selectPoints.size()-1; i ++)
    {
      OdGeLineSeg2d seg(OdGePoint2d(m_selectPoints[i].x, m_selectPoints[i].y), 
                        OdGePoint2d(m_selectPoints[i+1].x, m_selectPoints[i+1].y));
      OdGePoint2d intPt;
      if(seg1.intersectWith(seg, intPt) && pointInsideSeg(intPt, seg1) || 
         seg2.intersectWith(seg, intPt) && pointInsideSeg(intPt, seg2))
      {
        bIntersFound = true;
        break;
      }
    }
    if(bIntersFound)
    {
      dispSelectError();
      return ;
    }
    else
    {
      dispSelectPrompt(); // clear last error
    }
  }
  m_selectPoints.push_back(point);
}
*/

void ExDbCommandContext::highlight(OdSelectionSet* pSSet, 
                                   bool bDoIt) const //  = true
{
  OdSelectionSetIteratorPtr pIter = pSSet->newIterator();
  OdDbDatabase* pDb = NULL;
  OdUInt32 cntr(0);
  for (; !pIter->done(); pIter->next())
  {
    highlight(pIter, bDoIt);
    if ((cntr % DBCC_PAGE_EACH_OBJECT) && (pDb || (pDb = OdDbObjectId(pIter->id()).database())))
    {
      pDb->pageObjects();
      cntr = 0;
    }
    ++cntr;
  }
}

void ExDbCommandContext::highlight(OdSelectionSetIterator* pIter, 
                                   bool bDoIt) const //  = true
{
  OdDbObjectPtr pObj = OdDbObjectId(pIter->id()).safeOpenObject();
  if (!pObj->isKindOf(OdDbEntity::desc()))
    return;
  OdDbEntityPtr pEnt = pObj;
  if (pIter->subentCount())
  {
    for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
    {
      OdDbFullSubentPath subEntPath;
      if (pIter->getSubentity(se, subEntPath))
        pEnt->highlight(bDoIt, &subEntPath);
    }
  }
  else
    pEnt->highlight(bDoIt);
}

bool ExDbCommandContext::interactive() {
  if(m_pIoStream.get())
    return m_pIoStream->interactive();
  throw OdError(eNotInitializedYet);
}

inline bool isOverallVp(const OdDbEntity* pObj)
{
  OdDbViewportPtr pVp = OdDbViewport::cast(pObj);
  if(pVp.get() && pVp->number()==1)
    return true;
  return false;
}

bool ExDbCommandContext::isPickadd() const
{
  return true;
}

static OdUInt32 getGripObjLimit(const OdRxObject* cpRxDb)
{
  static OdUInt32 limitGripObj = 0;
  if (limitGripObj)
    return limitGripObj;

  limitGripObj = 100; // GRIPOBJLIMIT
  OdDbBaseHostAppServices* pBaseAppServices = NULL; 
  OdRxObject* pRxDb = const_cast<OdRxObject*>(cpRxDb);
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPE.get())
    pBaseAppServices =  pDbPE->appServices(pRxDb);
  ODA_ASSERT_ONCE(pDbPE.get() && pBaseAppServices);
  //TODO v3.9 add vitual getGRIPOBJLIMIT ... to OdDbBaseHostAppServices
  // see
  // TD vars: PICKFIRST PICKBOX GRIPSIZE GRIPHOT GRIPCOLOR GRIPHOVER GRIPOBJLIMIT
  // TG vars: PICKFIRST PICKBOX DG_GRIPSIZE DG_GRIPHOT DG_GRIPCOLOR DG_GRIPHOVER DG_GRIPOBJLIMIT
  //
  // TODO // limitGripObj = (OdUInt32) pBaseAppServices->getGRIPOBJLIMIT();
  if (pBaseAppServices)
  {
    OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(pBaseAppServices).get();
    if (pAppServices)
      limitGripObj = (OdUInt32) pAppServices->getGRIPOBJLIMIT();
  }
  return limitGripObj;
}

bool checkCurrentSpaceForNestedBlocks(OdDbEntityPtr pEnt, OdDbObjectId BTRId)
{
  OdDbBlockTableRecordPtr pBlockTRec = OdDbBlockTableRecord::cast(pEnt->ownerId().openObject());
  bool notInCurrentSpace = pEnt->ownerId() != BTRId;

  if (!pBlockTRec.isNull())// TODO AlexeyTyurin
  {
    OdDbObjectIdArray idsblk;
    pBlockTRec->getBlockReferenceIds(idsblk);

    for (unsigned int i = 0; i < idsblk.length(); i++)
    {
      OdDbEntityPtr blkRefer = OdDbEntity::cast(idsblk[i].openObject());
      if (!blkRefer.isNull())
      {
        notInCurrentSpace = checkCurrentSpaceForNestedBlocks(blkRefer, BTRId);
        break;
      }
    }
  }

  return notInCurrentSpace;
}
void ExDbCommandContext::merge(OdSelectionSet* pRes,
                               int options,
                               const OdSelectionSet* pSSet,
                               OdSSetTracker* pTracker,
                               OdEdUserIO* pIO) const //  = NULL
{
  ODA_ASSERT(pSSet && pRes && pRes->baseDatabase() == pSSet->baseDatabase());

  if (!pSSet)
    return;
  OdString s;
  if (pIO)
  {
    s.format(OD_T("%d found"), pSSet->numEntities());
    pIO->putString(s);
  }
  
  int nOnLockedLayer = 0;
  int nNotInCurrentSpace = 0;
  int nObjects = 0;
  bool bPsVp = false;
  OdUInt32 numResEntities = pRes->numEntities(),
           numSSetEntities = pSSet->numEntities(),
           limitGripObj = getGripObjLimit(pRes->baseDatabase());

  if (!isPickadd() && numResEntities && numSSetEntities)
  {
    if (numResEntities < limitGripObj && numSSetEntities < limitGripObj)
      highlight(pRes, false);
    pRes->clear();
  }

  bool bAppending = !GETBIT(options, OdEd::kSelRemove);
  OdDbObjectId BTRId;
  OdDbDatabase* pDb = OdDbDatabase::cast(pRes->baseDatabase()).get();
  if (pDb && !GETBIT(options, OdEd::kSelAllowInactSpaces))
  {
    OdDbObjectId idMS = pDb->getModelSpaceId();
    BTRId = pDb->getActiveLayoutBTRId();
    if (BTRId != idMS)
    { // Paper Space Layout
      OdDbLayoutPtr pLayout = pDb->currentLayoutId().safeOpenObject();
      if (pLayout->activeViewportId() != pLayout->overallVportId())
      {
        BTRId = idMS;
      }
    }
  }
  OdSelectionSetIteratorPtr pIter;

  OdUInt32 cntr(0);
  for (pIter = pSSet->newIterator(); !pIter->done(); pIter->next())
  {
    OdDbStub* id = pIter->id();
    ODA_ASSERT_ONCE(id && pSSet->baseDatabase() == baseDatabaseBy(id));

    bool bMember = pRes->isMember(id);
    if (bMember && GETBIT(options, OdEd::kSelAllowSubents))
    {
      for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
      {
        OdDbBaseFullSubentPath subEntPath;
        pIter->getSubentity(se, subEntPath);
        if (!pRes->isMember(subEntPath))
        {
          bMember = false;
          break;
        }
      }
    }
    if (!(bMember ^ bAppending))
      continue;
    if (pDb)
    {
      OdDbEntityPtr pEnt = OdDbEntity::cast(OdDbObjectId(id).openObject());

      if (pEnt.get())
      {
        bool notInCurrentSpace = checkCurrentSpaceForNestedBlocks(pEnt, BTRId);
        OdDbLayerTableRecordPtr l = pEnt->layerId().openObject();
        if (!GETBIT(options, OdEd::kSelAllowLocked) && !l.isNull() && l->isLocked())
        {
          ++nOnLockedLayer;
          continue;
        }
        else if(!GETBIT(options, OdEd::kSelAllowInactSpaces) && notInCurrentSpace)
        {
          ++nNotInCurrentSpace;
          continue;
        }
        else if(!bPsVp && !GETBIT(options, OdEd::kSelAllowPSVP) && ::isOverallVp(pEnt))
        {
          bPsVp = true;
          continue;
        }
      }
      else if (!GETBIT(options, OdEd::kSelAllowObjects))
      {
        ++nObjects;
        continue;
      }
    }
      
    if (numResEntities < limitGripObj && numSSetEntities < limitGripObj)
      highlight(pIter, bAppending);
    if (bAppending)
    {
      if (!pTracker || pTracker->append(id, pIter->method()))
      {
        pRes->append(id, pIter->method());
        for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
        {
          OdDbBaseFullSubentPath subEntPath;
          if (pIter->getSubentity(se, subEntPath))
            pRes->append(subEntPath, pIter->method());
        }
      }
    }
    else
    {
      if (!pTracker || pTracker->remove(id, pIter->method()))
      {
        if (GETBIT(options, OdEd::kSelAllowSubents) || GETBIT(options, OdEd::kSelAllowSubentsAlways))
        {
          for (OdSelectionSetIteratorPtr pResIter = pRes->newIterator();
               !pResIter->done(); pResIter->next())
          {
            if (pResIter->id() != id)
              continue;
            
            if (pResIter->subentCount())
            {
              for (OdUInt32 se = 0; se < pIter->subentCount(); se++)
              {
                OdDbBaseFullSubentPath subEntPath;
                if (pIter->getSubentity(se, subEntPath))
                  pRes->remove(subEntPath);
              }
              if (!pResIter->subentCount())
                pRes->remove(id);
            }
            break;
          } // end for
        }
        else
          pRes->remove(id);
      }
    }
    if (pDb && (cntr % DBCC_PAGE_EACH_OBJECT))
    {
      pDb->pageObjects();
      cntr = 0;
    }
    ++cntr;
  } // end for

  if (pIO && nObjects)
  {
    if (nObjects == 1)
      s = OD_T("1 was not an entity.");
    else
      s.format(OD_T("%d were not entities."), nObjects);
    pIO->putString(s);
  }
  if (pIO && nNotInCurrentSpace)
  {
    if (nNotInCurrentSpace == 1)
      s = OD_T("1 was not in current space.");
    else
      s.format(OD_T("%d were not in current space."), nNotInCurrentSpace);
    pIO->putString(s);
  }
  if (pIO && nOnLockedLayer)
  {
    if (nOnLockedLayer == 1)
      s = L"1 was on a locked layer.";
    else
      s.format(L"%d were on a locked layer.", nOnLockedLayer);
    pIO->putString(s);
  }
  if (pIO && bPsVp)
  {
    pIO->putString(OD_T("1 was the paper space viewport."));
  }
}

class ExSelectionMethods
{
  protected:
    // Returns true if entity is on visible layer
    static bool isEntityVisible(OdDbEntityPtr pEntity)
    {
      if (!pEntity.isNull() && pEntity->visibility() == OdDb::kVisible)
      {
        OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::cast(pEntity->layerId().openObject(OdDb::kForRead));
        if (!pLayer->isFrozen() && !pLayer->isOff())
          return true;
      }
      return false;
    }
    // Return object Id of cirrent selection block
    static OdDbObjectId selectionBlockId(const OdDbObjectId& vpId)
    {
      OdDbDatabase* pDb = vpId.database();
      OdDbViewportPtr pVp = OdDbViewport::cast(vpId.safeOpenObject());
      if (!pVp.isNull())
        return (pVp->number() == 1) ? pDb->getPaperSpaceId() : pDb->getModelSpaceId();
      return pDb->getModelSpaceId();
    }
    // Invalidates tracker
    static void invalidateTracker(OdEdPointTracker *pTracker)
    {
      static_cast<GraphTrackerBase*>(pTracker)->invalidateGsModel();
    }
  public:
    static void selectLast(OdDbDatabase *pDb, OdDbObjectIdArray &ids)
    {
      OdDbObjectId blockId = selectionBlockId(pDb->activeViewportId());
      OdDbBlockTableRecordPtr pBlockRec = OdDbBlockTableRecord::cast(blockId.openObject(OdDb::kForRead));
      OdDbObjectIteratorPtr pIt = pBlockRec->newIterator(false);
      OdUInt32 cntr(0);
      while (!pIt->done())
      {
        OdDbEntityPtr pEntity = pIt->entity(OdDb::kForRead);
        if (isEntityVisible(pEntity))
        {
          ids.append(pEntity->id());
          break;
        }
        pIt->step(false);
        if (pDb && (cntr % DBCC_PAGE_EACH_OBJECT))
        {
          pDb->pageObjects();
          cntr = 0;
        }
        ++cntr;
      }
    }
    static void selectAll(OdDbDatabase *pDb, OdDbObjectIdArray &ids)
    {
      OdDbObjectId blockId = selectionBlockId(pDb->activeViewportId());
      OdDbBlockTableRecordPtr pBlockRec = OdDbBlockTableRecord::cast(blockId.openObject(OdDb::kForRead));
      OdDbObjectIteratorPtr pIt = pBlockRec->newIterator();
      OdUInt32 cntr(0);
      while (!pIt->done())
      {
        OdDbEntityPtr pEntity = pIt->entity(OdDb::kForRead);
        if (isEntityVisible(pEntity))
        {
          ids.append(pEntity->id());
        }
        pIt->step();
        if (pDb && (cntr % DBCC_PAGE_EACH_OBJECT))
        {
          pDb->pageObjects();
          cntr = 0;
        }
        ++cntr;
      }
    }
    static void selectRect(OdSmartPtr<OdEdBaseIO> pIoStream, OdDbUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
    {
      pts.resize(2);
      pts[0] = pIO->getPoint(OD_T("Specify first corner:"),    OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);
      if ((pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
        ssm = OdDbVisualSelection::kDisableSubents;
      pts[1] = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|OdEd::kGptRectFrame);
    }
    static void selectPolyline(OdSmartPtr<OdEdBaseIO> pIoStream, OdDbUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
    {
      pts.clear();
      OdEdPointTrackerPtr pTracker = PolylineSelectionTracker::create(&pts);
      for (;;)
      {
        if (pts.size() == 0)
        {
          pts.resize(1);
          pts[0] = pIO->getPoint(OD_T("Specify first fence point:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);
        }
        if ((pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
          ssm = OdDbVisualSelection::kDisableSubents;
        try
        {
          pts.append(pIO->getPoint(OD_T("Specify next fence point:"),
                                   OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|/*OdEd::kGptRubberBand|*/
                                   OdEd::kInpThrowEmpty/*|OdEd::kInpThrowOther*/,
                                   0, OD_T("Undo"), pTracker));
          invalidateTracker(pTracker);
        }
        catch(const OdEdKeyword& kw)
        {
          if (kw.keywordIndex() == 0)
          {
            pts.removeLast();
            invalidateTracker(pTracker);
          }
        }
        catch (const OdEdEmptyInput)
        {
          break;
        }
      }
    }
    static void selectPolygon(OdSmartPtr<OdEdBaseIO> pIoStream, OdDbUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
    {
      pts.clear();
      OdEdPointTrackerPtr pTracker = PolygonSelectionTracker::create(&pts);
      for (;;)
      {
        if (pts.size() == 0)
        {
          pts.resize(1);
          pts[0] = pIO->getPoint(OD_T("First polygon point:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);
        }
        if ((pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
          ssm = OdDbVisualSelection::kDisableSubents;
        try
        {
          OdGePoint3d newPoint = pIO->getPoint(OD_T("Specify endpoint of line:"),
                                               OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|
                                               OdEd::kInpThrowEmpty,
                                               0, OD_T("Undo"), pTracker);
          if (PolygonSelectionTracker::doesPolySegmentsNotIntersects(pts, newPoint))
          {
            pts.append(newPoint);
            invalidateTracker(pTracker);
          }
          else
          {
            pIO->putError(OD_T("Invalid point, polygon segments cannot intersect."));
          }
        }
        catch(const OdEdKeyword& kw)
        {
          if (kw.keywordIndex() == 0)
          {
            pts.removeLast();
            invalidateTracker(pTracker);
          }
        }
        catch (const OdEdEmptyInput)
        {
          break;
        }
      }
    }
};

OdSelectionSetPtr ExDbCommandContext::select(const OdString& sPrompt,
                                             int options,
                                             const OdSelectionSet* pDefVal,
                                             const OdString& sKeywords,
                                             OdSSetTracker* pBaseTracker,
                                             OdGePoint3dArray* ptsPointer)
{
  OdEdSSetTracker* pTracker = OdEdSSetTracker::cast(pBaseTracker).get();
  OdDbUserIO* pIO = dbUserIO();
  OdDbDatabase* pDb = database();
  OdDbSelectionSetPtr pRes = OdDbSelectionSet::createObject(pDb);
  
  OdString sMsg;
  OdDbSelectionSetPtr pSSet = pickfirst();
  setPickfirst(0);

  if( pSSet.get() && pSSet->numEntities() && !pDefVal && !GETBIT(options, OdEd::kSelIgnorePickFirst) )
  {
    merge(pRes, (options | OdEd::kSelRemove) & ~OdEd::kSelRemove, pSSet, pTracker, pIO);
    return pRes;
  }
  pSSet.release();

  OdString prompt(sPrompt);
  if (prompt.isEmpty())
    prompt = OD_T("Select objects:");

  OdString keywords(sKeywords);
  if (keywords.isEmpty())
    keywords = OD_T("Window Last Crossing BOX ALL Fence WPolygon CPolygon Group Add Remove Multiple Previous Undo Auto Single");

  if (pDefVal)
    merge(pRes, (options | OdEd::kSelRemove) & ~OdEd::kSelRemove, pDefVal, pTracker, pIO);

  OdGePoint3dArray pts;
  do
  {
selection_loop_start:
    OdDbVisualSelection::SubentSelectionMode ssm = (GETBIT(options, OdEd::kSelAllowSubents) || GETBIT(options, OdEd::kSelAllowSubentsAlways))? OdDbVisualSelection::kEnableSubents : OdDbVisualSelection::kDisableSubents;
    int nKword = -1;
    pts.resize(1);
    try
    {
      try
      {
        if(GETBIT(options, OdEd::kSelPickLastPoint))
        {
          SETBIT(options, OdEd::kSelPickLastPoint, false);
          pts[0] = pIO->getLASTPOINT();
        }
        else
        {
          pts[0] = pIO->getPoint(prompt, 
            OdEd::kGptNoUCS|
            OdEd::kGptNoLimCheck|
            OdEd::kInpThrowEmpty|
            OdEd::kInpThrowOther,
            0, keywords);
        }

        if (ptsPointer != NULL)
        {
          ptsPointer->append(pts[0]); // TODO more complicated for nKword != -1 
        }

        if ((m_pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0 && !GETBIT(options, OdEd::kSelAllowSubentsAlways))
          ssm = OdDbVisualSelection::kDisableSubents;

        pSSet = OdDbSelectionSet::select(database()->activeViewportId(), 1, &pts[0], OdDbVisualSelection::kPoint, ssm);
        if(pSSet->numEntities()==0 && !GETBIT(options, OdEd::kSelSingleEntity))
        {
          // try again if selected entity hasn't subentity
          if (ssm == OdDbVisualSelection::kEnableSubents && GETBIT(options, OdEd::kSelAllowSubentsAlways))
          {
            ssm = OdDbVisualSelection::kDisableSubents;
            pSSet = OdDbSelectionSet::select(database()->activeViewportId(), 1, &pts[0], OdDbVisualSelection::kPoint, ssm);
          }
          
          if (pSSet->numEntities() == 0)
            pSSet = box(pts[0], ssm);
        }
      }
      catch(const OdEdOtherInput& inp)
      {
        if(try_lisp(inp.string()))
        {
          pSSet = OdDbSelectionSet::createObject(pDb);
          for (OdResBufPtr pRb(result()); pRb.get(); pRb = pRb->next())
          {
            pSSet->append(pRb->getObjectId(pDb));
          }
        }
        else
          throw;
      }
    }
    catch(const OdEdKeyword& kw)
    {
      if (!sKeywords.isEmpty()) throw;
      nKword = kw.keywordIndex();
    }
    catch(const OdEdEmptyInput)
    {
      if(pRes->numEntities() || GETBIT(options, OdEd::kSelAllowEmpty))
        break;

      if(GETBIT(options, OdEd::kInpThrowEmpty))
        throw;

      continue;
    }
    catch(const OdEdOtherInput& )
    {
      if(GETBIT(options, OdEd::kInpThrowOther))
        throw;

      continue;
    }

    bool bNYI = false;
    switch(nKword)
    {
    case -1: // no op
      break;
    case  0: // Window
      ExSelectionMethods::selectRect(m_pIoStream, pIO, ssm, pts);
      pSSet = OdDbSelectionSet::select(database()->activeViewportId(), 2, pts.getPtr(), OdDbVisualSelection::kWindow, ssm);
      break;
    case  1: // Last
      {
        OdDbObjectIdArray ids;
        pSSet = OdDbSelectionSet::createObject(database());
        ExSelectionMethods::selectLast(database(), ids);
        pSSet->append(ids);
      }
      break;
    case  2: // Crossing
      ExSelectionMethods::selectRect(m_pIoStream, pIO, ssm, pts);
      pSSet = OdDbSelectionSet::select(database()->activeViewportId(), 2, pts.getPtr(), OdDbVisualSelection::kCrossing, ssm);
      break;
    case  3: // BOX
      pts[0] = pIO->getPoint(L"Specify first corner:", OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);
      if ((m_pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0)
        ssm = OdDbVisualSelection::kDisableSubents;
      pSSet = box(pts[0], ssm);
      break;
    case  4: // ALL
      {
        OdDbObjectIdArray ids;
        pSSet = OdDbSelectionSet::createObject(database());
        ExSelectionMethods::selectAll(database(), ids);
        pSSet->append(ids);
      }
      break;
    case  5: // Fence
      ExSelectionMethods::selectPolyline(m_pIoStream, pIO, ssm, pts);
      if (pts.size() > 1)
        pSSet = OdDbSelectionSet::select(database()->activeViewportId(), (int)pts.size(), pts.getPtr(), OdDbVisualSelection::kFence, ssm);
      break;
    case  6: // WPolygon
      ExSelectionMethods::selectPolygon(m_pIoStream, pIO, ssm, pts);
      if (pts.size() > 2)
        pSSet = OdDbSelectionSet::select(database()->activeViewportId(), (int)pts.size(), pts.getPtr(), OdDbVisualSelection::kWPoly, ssm);
      break;
    case  7: // CPolygon
      ExSelectionMethods::selectPolygon(m_pIoStream, pIO, ssm, pts);
      if (pts.size() > 2)
        pSSet = OdDbSelectionSet::select(database()->activeViewportId(), (int)pts.size(), pts.getPtr(), OdDbVisualSelection::kCPoly, ssm);
      break;
    case  8: // Group
      bNYI = true;
      break;
    case  9: // Add
      SETBIT(options, OdEd::kSelRemove, false);
      break;
    case 10: // Remove
      SETBIT(options, OdEd::kSelRemove, true);
      break;
    case 11: // Multiple
      SETBIT(options, OdEd::kSelSinglePass, false);
      break;
    case 12: // Previous
      pSSet = previousSelection();
      break;
    case 13: // Undo
      bNYI = true;
      break;
    case 14: // AUto
      SETBIT(options, OdEd::kSelRemove, false); // Same as 'Add'
      SETBIT(options, OdEd::kSelSinglePass, false); // Same as 'Multiple'
      break;
    case 15: // SIngle
      SETBIT(options, OdEd::kSelSinglePass, true);
      goto selection_loop_start;
    }
    if (!pSSet.isNull())
      merge(pRes, options, pSSet, pTracker, pIO);
    else if (bNYI)
      pIO->putError(OD_T("Not Yet Implemented"));
  }
  while(!GETBIT(options, OdEd::kSelSinglePass) && !GETBIT(options, OdEd::kSelAllowEmpty)); // TODO Alexeytyurin

  if (   !GETBIT(options, OdEd::kSelLeaveHighlighted) 
      && pRes->numEntities() < getGripObjLimit(pRes->baseDatabase()))
    highlight(pRes, false);
  setPreviousSelection(pRes);

  return pRes;
}

OdString ExDbCommandContext::getStringInternal(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  return m_pIoStream->getString(prompt, options, pTracker);
}

OdGePoint3d ExDbCommandContext::getPointOrReal(const OdString& prompt,
                                               int options,
                                               const OdString& keywords,
                                               OdResBuf::ValueType realType,
                                               OdEdPointTracker* pPtTracker)
{
  for(;;)
  {
    try
    {
      return getPoint(prompt, options|OdEd::kInpThrowOther,
                      0, keywords, pPtTracker);
    }
    catch(const OdEdOtherInput& other)
    {
      const OdChar* reprompt = NULL;
      RealInput real = {0};
      try
      {
        switch(realType)
        {
        case OdResBuf::kDxfXdDist:
          if(pPtTracker)
          {
            if(!keywords.isEmpty())
              reprompt = OD_T("Requires numeric distance, second point, or option keyword.");
            else
              reprompt = OD_T("Requires numeric distance or second point.");
          }
          else
          {
            if(!keywords.isEmpty())
              reprompt = OD_T("Requires numeric distance, two points, or option keyword.");
            else
              reprompt = OD_T("Requires numeric distance or two points.");
          }
          real.val = baseFormatter().unformatLinear(other.string());
          break;
        case OdResBuf::kRtOrient:
          if(pPtTracker)
          {
            if(!keywords.isEmpty())
              reprompt = OD_T("Requires valid numeric angle, second point, or option keyword.");
            else
              reprompt = OD_T("Requires valid numeric angle or second point.");
          }
          else
          {
            if(!keywords.isEmpty())
              reprompt = OD_T("Requires valid numeric angle, two points, or option keyword.");
            else
              reprompt = OD_T("Requires valid numeric angle or two points.");
          }
          real.val = baseFormatter().unformatAngle(other.string());
          break;
        default:
          break;
        }
      }
      catch(const OdError& e)
      {
        if(e.code()!=eInvalidInput)
          throw;
        if(GETBIT(options, OdEd::kInpThrowOther))
          throw other;
        m_pIoStream->putError(reprompt);
        continue;
      }
      throw real;
    }
  }
}


double ExDbCommandContext::getReal(const OdString& sPrompt,
                                   int options,
                                   double defVal,
                                   OdResBuf::ValueType vt,
                                   const OdString& sKeywords,
                                   OdEdRealTracker* pTracker)
{
  OdString prompt ( sPrompt );
  if(prompt.isEmpty())
    prompt = OD_T("Enter a distance:");

  double resDist;
  OdGePoint3d basePt;
  if (!GETBIT(options, OdEd::kGdsFromLastPoint))
  {
    try
    {
      basePt = getPointOrReal(prompt, options, sKeywords, vt, 0);
    }
    catch(const RealInput& dist)
    {
      if(pTracker)
        pTracker->setValue(dist.val);
      return dist.val;
    }
    prompt = OD_T("Specify second point:");
  }
  else
  {
    basePt = dbUserIO()->getLASTPOINT();
  }
  
  OdEdPointTrackerPtr pPtTracker;
  if(pTracker)
  {
    if(vt==OdResBuf::kDxfXdDist)
      pPtTracker = PointToDistTracker::create(basePt, options, pTracker, m_pDb);
    else
      pPtTracker = PointToOrientTracker::create(basePt, pTracker);
  }

  try
  {
    OdGePoint3d pt = getPointOrReal(prompt, options | OdEd::kInpThrowEmpty, sKeywords, vt, pPtTracker);
    resDist = (pt - basePt).length();
    if (pTracker && vt == OdResBuf::kDxfXdDist && GETBIT(options, OdEd::kGdsSignedDist))
      resDist = ((PointToDistTracker*) pPtTracker.get())->getDistByOptions(pt);
  }
  catch(const RealInput& dist)
  {
    resDist = dist.val;
  }
  catch(const OdEdEmptyInput& )
  {
    if (GETBIT(options, OdEd::kInpThrowEmpty))
      throw;
    resDist = defVal;
  }
  if (pTracker)
    pTracker->setValue(resDist);
  return resDist;
}

double ExDbCommandContext::getDist(const OdString& sPrompt,
                                   int options,
                                   double defVal,
                                   const OdString& sKeywords,
                                   OdEdRealTracker* pTracker)
{
  OdString prompt( sPrompt );
  if(prompt.isEmpty())
    prompt = OD_T("Enter a distance:");

  return getReal(prompt, options, defVal, OdResBuf::kDxfXdDist, sKeywords, pTracker);
}

double ExDbCommandContext::getAngle(const OdString& sPrompt,
                                    int options,
                                    double defVal,
                                    const OdString& sKeywords,
                                    OdEdRealTracker* pTracker)
{
  OdString prompt(sPrompt);

  if (prompt.isEmpty())
    prompt = OD_T("Specify an angle:");

  double ANGBASE = 0.0;
  //OdEdRealTrackerPtr pTrck; // TODO define angle via points
  if (GETBIT(options, OdEd::kGanNoAngBase))
  {
    if (OdDbDatabase::cast(baseDatabase()).get())
      ANGBASE = database()->getANGBASE();
    //if (pTracker)
    //  pTrck = ANGBASE_Tracker::create(pTracker, ANGBASE);
  }
  // #4907 //return getReal(prompt, options, defVal + ANGBASE, sKeywords, pTrck) - ANGBASE;
  for (;;)
  {
    OdString sInput = ExDbCommandContext::getString(prompt, options, OdString::kEmpty, sKeywords);
    if (sInput.isEmpty())
      return defVal;
    try
    {
      double dAngle = baseFormatter().unformatAngle(sInput);
      dAngle -= ANGBASE;
      return dAngle;
    }
    catch (const OdError& e)
    {
      if (e.code() != eInvalidInput)
        throw;
      if (GETBIT(options, OdEd::kInpThrowOther))
        throw OdEdOtherInput(sInput);
      putError(sKeywords.find(L' ') < 0 ? L"Requires valid numeric angle."
                                        : L"Requires valid numeric angle or optional keyword.");
    }
  }
}

OdEdPointDefTrackerPtr ExDbCommandContext::createRubberBand(const OdGePoint3d& base,
                                                            OdGsModel* pModel) const // = NULL
{
  return RubberBand::create(base, pModel);
}

OdEdPointDefTrackerPtr ExDbCommandContext::createRectFrame(const OdGePoint3d& base,
                                                           OdGsModel* pModel) const // = NULL
{
  { // Create enhanced rect frame if requested
    OdRxObjectPtr pArbData = arbitraryData(OD_T("ExDbCommandContext_EnhRectFrame"));
    if (!pArbData.isNull())
      return EnhRectFrame::create(base, pModel);
  }
  return RectFrame::create(base, pModel);
}

OdGePoint3d ExDbCommandContext::getLASTPOINT() const
{
  return m_LASTPOINT;
}

void ExDbCommandContext::setLASTPOINT(const OdGePoint3d& pt)
{
  m_LASTPOINT = pt;
}

OdGePoint3d ExDbCommandContext::getPoint(const OdString& sPrompt,
                                         int options,
                                         const OdGePoint3d* pDefVal,
                                         const OdString& keywords,
                                         OdEdPointTracker* pTracker)
{
  OdString prompt(sPrompt);

  if(prompt.isEmpty())
    prompt = OD_T("Enter a point:");

  OdEdPointTrackerPtr pPredef;
  if(!pTracker)
  {
    if(GETBIT(options, OdEd::kGptRectFrame))
    {
      pTracker = pPredef = createRectFrame(dbUserIO()->getLASTPOINT());
    }
    else if(GETBIT(options, OdEd::kGptRubberBand))
    {
      pTracker = pPredef = createRubberBand(dbUserIO()->getLASTPOINT());
    }
  }

  for(;;)
  {
    OdGePoint3d res;
    try
    {
      res = m_pIoStream->getPoint( prompt, options, pTracker );
    }
    catch(const OdEdEmptyInput&)
    {
      if(GETBIT(options, OdEd::kInpThrowEmpty))
        throw;
    }
    catch(const OdEdOtherInput& other)
    {
      if(!other.string().isEmpty())
      {
        if(!keywords.isEmpty())
          KWIndex(keywords).check(other.string());
        try
        {
          res = OdGePoint3d::kOrigin;
          OdString sInput(other.string());
          int i = 0;
          if (sInput.find(L',') > 0) // prevent exception via unformatLinear
          {
            for (; i < 3 && !sInput.isEmpty(); ++i)
            {
              OdString sCoord = ::next(sInput);
              res[i] = baseFormatter().unformatLinear(sCoord);
            }
          }
          if (i < 2)
            throw;
        }
        catch(const OdError& e)
        {
          if(e.code()!=eInvalidInput)
            throw;
          
          if(GETBIT(options, OdEd::kInpThrowOther))
            throw other;
          
          dbUserIO()->putError(keywords.isEmpty() ? OD_T("Invalid point.") : OD_T("Point or option keyword required."));
          continue;
        }
      }
      else if (GETBIT(options, OdEd::kInpThrowEmpty))
      {
        throw OdEdEmptyInput();
      }
      else if(pDefVal) {
        res = *pDefVal;
      }
      else
      {
        continue;
      }
    }
    if(!GETBIT(options, OdEd::kGptNoLimCheck) && database()->getLIMCHECK())
    {
      OdGeExtents2d ext(database()->getLIMMIN(), database()->getLIMMAX());
      if(!ext.contains((const OdGePoint2d&)res))
      {
        putError(OD_T("**Outside limits"));
        continue;
      }
    }
    
    dbUserIO()->setLASTPOINT(res);
    return res;
  }
}

int ExDbCommandContext::getInt(const OdString& sPrompt,
                               int options,
                               int defVal,
                               const OdString& sKeywords,
                               OdEdIntegerTracker* pTracker)
{
  OdString prompt(sPrompt);

  if(prompt.isEmpty())
    prompt = OD_T("Enter an integer value:");
  OdString sInput = ExDbCommandContext::getString(prompt, options, OdString::kEmpty, sKeywords);
  if (sInput.isEmpty())
    return defVal;
  return ::odStrToInt(sInput);
}

int ExDbCommandContext::getKeyword(const OdString& sPrompt,
                                   const OdString& sKeywords,
                                   int defVal,
                                   int options,
                                   OdEdIntegerTracker* pTracker)
{
  OdString prompt(sPrompt);

  KWIndex kwIndex(sKeywords);
  if(prompt.isEmpty())
  {
    prompt = OD_T("Enter one of keywords [");
    for(OdUInt32 i=0; i<kwIndex.size(); ++i)
    {
      prompt += kwIndex.at(i).keyword();
      prompt += OD_T("/");
    }
    prompt += OD_T("]");
  }

  bool bDef = (OdUInt32(defVal) < kwIndex.size());
  if(bDef)
  {
    OdString defValSuf = L"<" + kwIndex.at(defVal).keyword() + L">";
    if (prompt.find(defValSuf) < 0) //skip if already present
      prompt += defValSuf;
  }
  prompt += OD_T(":");

  for(;;)
  {
    SETBIT(options, OdEd::kGstNoEmpty, !bDef);
    OdString sInput = ExDbCommandContext::getString(prompt, options);
    if(bDef && sInput.isEmpty())
      return defVal;
    OdUInt32 n = kwIndex.find(sInput);
    if(n < kwIndex.size())
      return n;
    putString(OD_T(""));
    putError(OD_T("Invalid option keyword."));
  }
}

double ExDbCommandContext::getReal(const OdString& sPrompt, 
                                   int options,
                                   double defVal,
                                   const OdString& sKeywords,
                                   OdEdRealTracker* pTracker)
{
  OdString prompt(sPrompt);

  if(prompt.isEmpty())
    prompt = OD_T("Enter a real value:");
  for(;;)
  {
    OdString sInput = ExDbCommandContext::getString(prompt, options, OdString::kEmpty, sKeywords);
    if (sInput.isEmpty())
      return defVal;
    try
    {
      return baseFormatter().unformatLinear(sInput);
    }
    catch(const OdError& e)
    {
      if(e.code()!=eInvalidInput)
        throw;
      if(GETBIT(options, OdEd::kInpThrowOther))
        throw OdEdOtherInput(sInput);
      putError(sKeywords.find(L' ') < 0 ? L"Requires real value." 
                                        : L"Requires real value or optional keyword.");
    }
  }
}

OdString ExDbCommandContext::getString(const OdString& sPrompt, 
                                       int options, 
                                       const OdString& pDefVal,
                                       const OdString& sKeywords,
                                       OdEdStringTracker* pTracker)
{
  OdString res = getStringInternal(sPrompt, options, pTracker);

  if(res.isEmpty())
  {
    if(GETBIT(options, OdEd::kInpThrowEmpty))
      throw OdEdEmptyInput();

    if(!pDefVal.isEmpty())
      return pDefVal;

    if(GETBIT(options, OdEd::kGstNoEmpty))
    {
      do
      {
        putError(OD_T("Enter non-empty value"));
        res = getStringInternal(sPrompt, options, pTracker);
      }
      while(res.isEmpty());
    }
    else
      return res;
  }

  if(!sKeywords.isEmpty())
    KWIndex(sKeywords).check(res);

  return res;
}

OdString ExDbCommandContext::getFilePath(const OdString& prompt, 
                                         int options,
                                         const OdString& dialogCaption, 
                                         const OdString& defExt,
                                         const OdString& fileName,
                                         const OdString& filter,
                                         const OdString& keywords,
                                         OdEdStringTracker* pTracker)
{
  OdString sInput;
  OdDbDatabasePtr db = OdDbDatabase::cast(baseDatabase());
  OdDbHostAppServices* hostAppSvcs = db.isNull() ? 0 : db->appServices();
  if(hostAppSvcs) {
    if(!hostAppSvcs->getFILEDIA()) {
      sInput = OdDbUserIO::getFilePath(prompt, options, dialogCaption, defExt, fileName, filter, keywords, pTracker);
      if(sInput != OD_T("~"))
      {
        sInput.trimLeft(' ');
        sInput.trimRight(' ');
        return sInput;
      }
    }
    sInput = hostAppSvcs->fileDialog(options & 3, dialogCaption, defExt, fileName, filter);
    if( sInput != OD_T("*unsupported*") )
      return sInput;
  }
  return OdDbUserIO::getFilePath(prompt, options, dialogCaption, defExt, fileName, filter, keywords, pTracker);
}

void ExDbCommandContext::putString(const OdString& string)
{
  m_pIoStream->putString(string);
}

OdCmColor ExDbCommandContext::getColor(const OdString& sPrompt,
                                       int options,
                                       const OdCmColor* pDefVal,
                                       const OdString& sKeywords,
                                       OdEdColorTracker* pTracker)
{
  database(); // throw OdError(eNoDatabase) if it is not DWG
  OdSharedPtr<OdCmColorBase> color = getCmColor(sPrompt, options, pDefVal, sKeywords, pTracker);
  OdCmColor* pColor = dynamic_cast<OdCmColor*>(color.get());
  ODA_ASSERT_ONCE(pColor);
  if (!pColor)
    throw OdError(eInvalidInput);
  return *pColor;
}

OdSharedPtr<OdCmColorBase> ExDbCommandContext::getCmColor(const OdString& sPrompt,
                                                          int options,
                                                          const OdCmColorBase* pDefVal,
                                                          const OdString& sKeywords,
                                                          OdEdColorTracker* pTracker)
{
  OdString prompt(sPrompt);

  if(prompt.isEmpty())
    prompt = OD_T("Specify a color:");

  for(;;)
  {
    OdString sInput = ExDbCommandContext::getString(
                          prompt, options,
                          pDefVal ? (const OdString&)baseFormatter().formatCmColor(*pDefVal) 
                                  : OdString::kEmpty, sKeywords);
    try
    {
      return baseFormatter().unformatCmColor(sInput);
    }
    catch(const OdError& e)
    {
      if(e.code()!=eInvalidInput)
        throw;
      if(GETBIT(options, OdEd::kInpThrowOther))
        throw OdEdOtherInput(sInput);
      putError(sKeywords.find(L' ') < 0 ? L"Invalid color value."
                                        : L"Requires color value or optional keyword.");
    }
  }
}

bool ExDbCommandContext::try_lisp(const OdString& input)
{
  OdString s(input);
  if (s[0]==L'(')
  {
    OdEdLispModulePtr lspMod = odrxDynamicLinker()->loadApp(OdLspModuleName);
    if (lspMod.isNull())
    {
      s.trimLeft(OD_T("( \r\n\t"));
      if(s.left(7).makeLower() == OD_T("handent"))
      {
        s = s.right(s.getLength() - 7);
        OdResBufPtr pRb, pRbLast;
        OdUInt64 handle = 0;
        while (odSScanf(s.trimLeft(L" \r\n\t\","), PRIx64W, &handle))
        {
          s.trimLeft(OD_T("0123456789ABCDEFabcdef"));
          s.trimLeft(OD_T(" \r\n\t\""));
          if (s[0] != L')' && s[0] != L',')
            break;
          OdResBufPtr pRbNext = OdResBuf::newRb(OdResBuf::kRtHandle);
          pRbNext->setHandle(OdDbHandle(handle));
          if (pRbLast.get())
            pRbLast->setNext(pRbNext);
          else
            setResult(pRb = pRbNext);
          pRbLast = pRbNext;
          if (s[0] == L')')
            return true;
        }
      }
    }
    else
    {
      lspMod->createLispEngine()->execute(this, s);
    }
  }
  return false;
}

#undef DBCC_PAGE_EACH_OBJECT
