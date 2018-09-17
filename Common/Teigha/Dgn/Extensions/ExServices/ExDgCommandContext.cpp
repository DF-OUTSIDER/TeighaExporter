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

#include "OdaCommon.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "DgUserIO.h"
#include "DgDatabase.h"
#include "DgHostAppServices.h"
#include "ExDgCommandContext.h"
#include "RxDictionary.h"
#include "DgSubentId.h"

#include "Gi/GiWorldDraw.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiUtils.h"

#include "Ge/GeExtents2d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeLineSeg2d.h"

#include "DgGraphicsElement.h"
#include "DgLevelTableRecord.h"
#include "DgModel.h"
#include "DgReferenceAttach.h"

#include "Ed/EdLispEngine.h"
#include "ResBuf.h"
#include "DynamicLinker.h"
#include "DgUnitsFormatter.h"
#include "ExTrackers.h"
#include "ExKWIndex.h"

/*
#include "DbLine.h"
#include "DbDictionary.h"
#include "DbColor.h"
#include "StaticRxObject.h"
#include "OdDToStr.h"
#include "Gs/Gs.h"
#include "DbViewport.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTableRecord.h"
*/
inline OdString next(OdString& list, const OdChar* delim = (const OdChar*)L",")
{
  OdString res = list.spanExcluding(delim);
  if(res.getLength() != list.getLength())
    list = list.mid(res.getLength()+1);
  else
    list.empty();
  return res;
}

//OdGePoint3d OdEdBaseIO::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
//{
//  throw OdEdOtherInput(getString(prompt, OdEd::kGstDefault, 0));
//}


OdRxDictionaryPtr ExDgCommandContext::arbDataDic() const
{
  if(m_pArbDataDic.isNull())
    m_pArbDataDic = odrxCreateRxDictionary();
  return m_pArbDataDic;
}

void ExDgCommandContext::setParam(OdRxObject* pParamObj)
{
  setArbitraryData(OD_T("ExDgCommandContext_FuncParamObj"), pParamObj);
}

OdRxObjectPtr ExDgCommandContext::param()
{
  return arbitraryData(OD_T("ExDgCommandContext_FuncParamObj"));
}

void ExDgCommandContext::setResult(OdRxObject* pResultObj)
{
  setArbitraryData(OD_T("ExDgCommandContext_FuncResultObj"), pResultObj);
}

OdRxObjectPtr ExDgCommandContext::result()
{
  return arbitraryData(OD_T("ExDgCommandContext_FuncResultObj"));
}

void ExDgCommandContext::setArbitraryData(const OdString& szPathName, OdRxObject* pDataObj)
{
  arbDataDic()->putAt(szPathName, pDataObj);
}

OdRxObjectPtr ExDgCommandContext::arbitraryData(const OdString& szPathName) const
{
  return arbDataDic()->getAt(szPathName);
}

OdDgSelectionSetPtr ExDgCommandContext::pickfirst()
{
  return arbitraryData(OD_T("ExDgCommandContext_pickfirst"));
}

void ExDgCommandContext::setPickfirst(OdDgSelectionSet* pSSet)
{
  setArbitraryData(OD_T("ExDgCommandContext_pickfirst"), pSSet);
}

OdDgSelectionSetPtr ExDgCommandContext::previousSelection()
{
  return arbitraryData(OD_T("ExDgCommandContext_PreviousSSet"));
}

void ExDgCommandContext::setPreviousSelection(OdDgSelectionSet* pSSet)
{
  setArbitraryData(OD_T("ExDgCommandContext_PreviousSSet"), pSSet);
}

OdUnitsFormatter& ExDgCommandContext::baseFormatter()
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(baseDatabase());
  //ODA_ASSERT_ONCE(pDbPE.get());
  if (!pDbPE.get())
    throw OdError(eNoDatabase); 
  return *pDbPE->baseFormatter(baseDatabase());
}
OdDgUnitsFormatter& ExDgCommandContext::formatter()
{
  return m_pDb->formatter();
}

OdDgCommandContextPtr ExDgCommandContext::createObject(OdEdBaseIO* pIOStream, OdDgDatabase* pDb)
{
  OdDgCommandContextPtr pRes = OdRxObjectImpl<ExDgCommandContext, OdDgCommandContext>::createObject();
  ExDgCommandContext* pDbCmdCtx = static_cast<ExDgCommandContext*>(pRes.get());

  pDbCmdCtx->m_pDb = pDb;
  pDbCmdCtx->m_pIoStream = pIOStream;
  return pRes;
}

OdRxObject* ExDgCommandContext::baseDatabase()
{
  return m_pDb;
}

OdDgSelectionSetPtr ExDgCommandContext::box(OdGsView* pGsView, const OdGePoint3d& pt1, OdDbVisualSelection::SubentSelectionMode ssm)
{
  OdGePoint3d pts[2] = {
    pt1,
    dbUserIO()->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|OdEd::kGptRectFrame)
  };

  return OdDgSelectionSet::select(database(), pGsView, 2, pts, OdDbVisualSelection::kBox, ssm);
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

static void highlight(OdDgSelectionSet* pSSet, bool bDoIt, bool /*bAllowSubents*/)
{
  OdDgElementPtr pEnt;
  OdDgSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while ( !pIter->done() )
  {
    pEnt = pIter->objectId().safeOpenObject();

    if (pIter->subentCount())
    {
      for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
      {
        OdDgFullSubentPath subEntPath;

        if( pIter->getSubentity(se, subEntPath) )
        {
          pEnt->highlight(bDoIt, &subEntPath);
        }
     }
   }
   else
   {
     pEnt->highlight(bDoIt);
   }

   pIter->next();
 }
}

void merge(OdDgUserIO* pIO,
             OdDgSelectionSet* pRes,
               int options,
                 const OdDgSelectionSet* pSSet,
                   OdEdDgSSetTracker* pTracker)
{
  ODA_ASSERT(pSSet);

  if( !pSSet )
  {
    return;
  }

  OdString s;
  s.format(OD_T("%d found"), pSSet->numEntities());
  pIO->putString(s);
 
  int nNotInCurrentSpace = 0;
  int nObjects = 0;

  bool bAppending = !GETBIT(options, OdEd::kSelRemove);

  OdDgElementId BTRId = pSSet->database()->getActiveModelId();

  OdDgSelectionSetIteratorPtr pIter = pSSet->newIterator();

  while( !pIter->done() )
  {
    bool bMember = pRes->isMember(pIter->objectId());

    if( bMember && GETBIT(options, OdEd::kSelAllowSubents) )
    {
      for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
      {
        OdDgFullSubentPath subEntPath;
        pIter->getSubentity(se, subEntPath);

        if( !pRes->isMember(subEntPath) )
        {
          bMember = false;
          break;
        }
      }
    }

    if( bMember ^ bAppending )
    {
      OdDgElementPtr pEnt = pIter->objectId().safeOpenObject();

      if( !pEnt.isNull() )
      {
        if(!GETBIT(options, OdEd::kSelAllowInactSpaces) && pEnt->ownerId()!=BTRId)
        {
          ++nNotInCurrentSpace;
          pEnt.release();
          pEnt = 0;
        }
        else
        {
          if( pIter->subentCount() )
          {
            for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
            {
              OdDgFullSubentPath subEntPath;

              if( pIter->getSubentity(se, subEntPath) )
              {
                pEnt->highlight(bAppending, &subEntPath);
              }
            }
          }
          else
          {
            pEnt->highlight(bAppending);
          }
        }
      }
      else if( !GETBIT(options, OdEd::kSelAllowObjects) )
      {
        ++nObjects;
        pEnt.release();
        pEnt = 0;
      }

      if( !pEnt.isNull() )
      {
        if( bAppending  )
        {
          if( !pTracker || pTracker->append(pIter->objectId(), pIter->method() ) )
          {
            pRes->append(pIter->objectId(), pIter->method());

            for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
            {
              OdDgFullSubentPath subEntPath;

              if( pIter->getSubentity(se, subEntPath) )
              {
                pRes->append(subEntPath, pIter->method());
              }
            }
          }
        }
        else
        {
          if( !pTracker || pTracker->remove(pIter->objectId(), pIter->method()) )
          {
            if( GETBIT(options, OdEd::kSelAllowSubents) )
            {
              OdDgSelectionSetIteratorPtr pResIter = pRes->newIterator();

              while( !pResIter->done() )
              {
                if( pResIter->objectId() == pIter->objectId() )
                {
                  if( pResIter->subentCount() )
                  {
                    for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
                    {
                      OdDgFullSubentPath subEntPath;

                      if( pIter->getSubentity(se, subEntPath) )
                      {
                        pRes->remove(subEntPath);
                      }
                    }

                    if( !pResIter->subentCount() )
                    {
                      pRes->remove(pIter->objectId());
                    }
                  }

                  break;
                }

                pResIter->next();
              }
            }
            else
            {
              pRes->remove(pIter->objectId());
            }
          }
        }
      }
    }
    else if( bMember && bAppending )
    {
      OdDgElementPtr pEnt = pIter->objectId().safeOpenObject();

      if( !pEnt.isNull() )
      {
        if( pIter->subentCount() )
        {
          for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
          {
            OdDgFullSubentPath subEntPath;

            if( pIter->getSubentity(se, subEntPath) )
            {
              pEnt->highlight(false, &subEntPath);
            }
          }
        }
        else
        {
          pEnt->highlight(false);
        }

        if( !pTracker || pTracker->remove(pIter->objectId(), pIter->method()) )
        {
          if( GETBIT(options, OdEd::kSelAllowSubents) )
          {
            OdDgSelectionSetIteratorPtr pResIter = pRes->newIterator();

            while( !pResIter->done() )
            {
              if( pResIter->objectId() == pIter->objectId() )
              {
                if( pResIter->subentCount() )
                {
                  for( OdUInt32 se = 0; se < pIter->subentCount(); se++ )
                  {
                    OdDgFullSubentPath subEntPath;

                    if( pIter->getSubentity(se, subEntPath) )
                    {
                      pRes->remove(subEntPath);
                    }
                  }

                  if( !pResIter->subentCount() )
                  {
                    pRes->remove(pIter->objectId());
                  }
                }

                break;
              }

              pResIter->next();
            }
          }
          else
          {
            pRes->remove(pIter->objectId());
          }
        }
      }
    }

    pIter->next();
  }

  if( nObjects )
  {
    if( nObjects==1 )
    {
      s = OD_T("1 was not an entity.");
    }
    else
    {
      s.format(OD_T("%d were not entities."), nObjects);
    }

    pIO->putString(s);
  }

  if( nNotInCurrentSpace )
  {
    if( nNotInCurrentSpace == 1 )
    {
      s = OD_T("1 was not in current space.");
    }
    else
    {
      s.format(OD_T("%d were not in current space."), nNotInCurrentSpace);
    }

    pIO->putString(s);
  }
}

class DgPolylineSelectionTracker : public OdEdPointDefTracker, public OdGiDrawableImpl<>
{
  protected:
    ODRX_USING_HEAP_OPERATORS(OdEdPointDefTracker);

  const OdGePoint3dArray *m_pts;
  OdGePoint3d m_pt;
  //PolylineSelectionTracker() : m_pts(NULL) { }

  void setValue(const OdGePoint3d &pt) { m_pt = pt; }
  void setValue(const OdGePoint3dArray *pts)
  {
    m_pts = pts;
  }

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const ODRX_OVERRIDE
  {
    OdGiSubEntityTraits* pSETraits = (OdGiSubEntityTraits*)pTraits;
    pSETraits->setTrueColor( OdCmEntityColor( 200, 200, 200) );

    return kDrawableIsAnEntity;
  }

  bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE
  {
    if( m_pts && m_pts->size() > 1 )
    {
      pWd->geometry().polyline((OdInt32)m_pts->size(), m_pts->asArrayPtr());
    }

    if( m_pts && m_pts->size() > 0 )
    {
      OdGePoint3d tailPts[2] =
      {
        m_pts->last(),
        m_pt
      };

      pWd->geometry().polyline(2, tailPts);
    }

    return true;
  }

  OdGePoint3d basePoint() const
  {
    if( m_pts && !m_pts->empty() )
    {
      return m_pts->first();
    }

    return OdGePoint3d::kOrigin;
  }

public:

  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<DgPolylineSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<DgPolylineSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }

  int addDrawables(OdGsView* pView)
  {
    pView->add(this, 0);
    return 1;
  }

  void removeDrawables(OdGsView* pView)
  {
    pView->erase(this);
  }
};

class DgPolygonSelectionTracker : public DgPolylineSelectionTracker
{

protected:

  bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    DgPolylineSelectionTracker::subWorldDraw(pWd);
    // Polygon segments must not intersect. This check must be implemented in selection command.

    if (m_pts && m_pts->size() > 1)
    {
      OdGePoint3d closePts[2] =
      {
         m_pt,
         m_pts->first()
      };

      pWd->geometry().polyline(2, closePts);
    }

    return true;
  }

public:

  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<DgPolygonSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<DgPolygonSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }

  static bool doesPolySegmentsNotIntersects(const OdGePoint3dArray &segments, const OdGePoint3d &newPoint)
  {
    if( segments.size() < 3 )
    {
      return true;
    }

    OdGePoint3dArray poly = segments;
    //poly.push_back(newPoint);
    OdGeVector3d zAxis = ::odgiFaceNormal(poly.size(), poly.asArrayPtr());

    if( OdZero(zAxis.length()) )
    {
      return true; // All segments are parallel
    }

    poly.push_back(newPoint);
    OdUInt32 nSeg;
    OdGeVector3d xAxis;

    for( nSeg = 0; nSeg < poly.size() - 1; nSeg++ )
    {
      xAxis = segments[nSeg + 1] - segments[nSeg];
      OdGe::ErrorCondition error;
      xAxis.normalize(OdGeContext::gZeroTol, error);

      if( error == OdGe::kOk )
      {
        break;
      }
    }

    OdGeVector3d yAxis = zAxis.crossProduct(xAxis).normal();
    OdGePoint2dArray poly2d;
    poly2d.resize(poly.size());

    for( nSeg = 0; nSeg < poly.size(); nSeg++ )
    {
      poly2d[nSeg].set(poly[nSeg].asVector().dotProduct(xAxis), poly[nSeg].asVector().dotProduct(yAxis));
    }

    // 1-st segment with all without last

    for( nSeg = 0; nSeg < poly2d.size() - 3; nSeg++ )
    {
      OdGePoint2d rVal;

      if( OdGeLineSeg2d(poly2d[nSeg], poly2d[nSeg + 1]).intersectWith(OdGeLineSeg2d(poly2d[poly2d.size() - 2], poly2d.last()), rVal) )
      {
        return false;
      }
    }

    // 2-nd segment with all without first

    for( nSeg = 1; nSeg < poly2d.size() - 2; nSeg++ )
    {
      OdGePoint2d rVal;

      if( OdGeLineSeg2d(poly2d[nSeg], poly2d[nSeg + 1]).intersectWith(OdGeLineSeg2d(poly2d.first(), poly2d.last()), rVal) )
      {
        return false;
      }
    }

    return true;
  }
};

class ExDgSelectionMethods
{
protected:

     // Returns true if entity is on visible layer

   static bool isEntityVisible(OdDgElementPtr pEntity)
   {
     OdDgGraphicsElementPtr pGraphEntity = pEntity->cast( OdDgGraphicsElement::desc() );

     if( !pGraphEntity.isNull() && !pGraphEntity->getInvisibleFlag() )
     {
       if( !pGraphEntity->getLevelId().isNull() )
       {
         OdDgLevelTableRecordPtr pLevel = OdDgLevelTableRecord::cast(pGraphEntity->getLevelId().openObject(OdDg::kForRead));

         if( !pLevel->getIsFrozenFlag() && pLevel->getIsDisplayedFlag())
         {
           return true;
         }
       }
     }

     return false;
   }

   // Return object Id of current selection block

   static OdDgElementId selectionBlockId(const OdDgElementId& vpId)
   {
     return vpId;
   }

 public:

   static void selectLast(OdDgDatabase *pDb, OdDgElementIdArray &ids)
   {
     OdDgElementId blockId = pDb->getActiveModelId();

     OdDgModelPtr pBlockRec = OdDgModel::cast(blockId.openObject(OdDg::kForRead));

     OdDgElementIteratorPtr pIt = pBlockRec->createGraphicsElementsIterator();

     OdUInt32 nItems = ids.size();

     while( !pIt->done() )
     {
       OdDgElementPtr pEntity = pIt->item().openObject(OdDg::kForRead);

       if( isEntityVisible(pEntity) )
       {
         ids.append(pEntity->id());
         break;
       }

       pIt->step(false);
     }

     if( ids.size() == nItems )
     {
       pIt = pBlockRec->createControlElementsIterator();

       while( !pIt->done() )
       {
         OdDgElementPtr pEntity = pIt->item().openObject(OdDg::kForRead);

         if( pEntity->getElementType() == OdDgElement::kTypeReferenceAttachmentHeader )
         {
           OdDgReferenceAttachmentHeaderPtr pXRef = pEntity;

           if( pXRef->getDisplayFlag() )
           {
             ids.append(pEntity->id());
             break;
           }
         }

         pIt->step(false);
       }
     }
   }

   static void selectAll(OdDgDatabase *pDb, OdDgElementIdArray &ids)
   {
     OdDgElementId blockId = pDb->getActiveModelId();
     OdDgModelPtr pBlockRec = OdDgModel::cast(blockId.openObject(OdDg::kForRead));

     OdDgElementIteratorPtr pIt = pBlockRec->createGraphicsElementsIterator();

     while( !pIt->done() )
     {
       OdDgElementPtr pEntity = pIt->item().openObject(OdDg::kForRead);

       if( isEntityVisible(pEntity) )
       {
         ids.append(pEntity->id());
       }

       pIt->step(false);
     }

     pIt = pBlockRec->createControlElementsIterator();

     while( !pIt->done() )
     {
       OdDgElementPtr pEntity = pIt->item().openObject(OdDg::kForRead);

       if( pEntity->getElementType() == OdDgElement::kTypeReferenceAttachmentHeader )
       {
         OdDgReferenceAttachmentHeaderPtr pXRef = pEntity;

         if( pXRef->getDisplayFlag() )
         {
           ids.append(pEntity->id());
         }
       }

       pIt->step(false);
     }
   }

   static void selectRect(OdSmartPtr<OdEdBaseIO> pIoStream, OdDgUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
   {
     pts.resize(2);
     pts[0] = pIO->getPoint(OD_T("Specify first corner:"),    OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);

     if( (pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0 )
     {
       ssm = OdDbVisualSelection::kDisableSubents;
     }

     pts[1] = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|OdEd::kGptRectFrame);
   }

   static void selectPolyline(OdSmartPtr<OdEdBaseIO> pIoStream, OdDgUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
   {
     pts.clear();
     OdEdPointTrackerPtr pTracker = DgPolylineSelectionTracker::create(&pts);

     for(;;)
     {
       if( pts.size() == 0 )
       {
         pts.resize(1);
         pts[0] = pIO->getPoint(OD_T("Specify first fence point:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);
       }

       if( (pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0 )
       {
         ssm = OdDbVisualSelection::kDisableSubents;
       }

       try
       {
         pts.append( pIO->getPoint(OD_T("Specify next fence point:"),
                                  OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|/*OdEd::kGptRubberBand|*/
                                  OdEd::kInpThrowEmpty/*|OdEd::kInpThrowOther*/,
                                  0, OD_T("Undo"), pTracker) );
       }
       catch(const OdEdKeyword& kw)
       {
         if( kw.keywordIndex() == 0 )
         {
           pts.removeLast();
         }
       }
       catch (const OdEdEmptyInput)
       {
         break;
       }
     }
   }

   static void selectPolygon(OdSmartPtr<OdEdBaseIO> pIoStream, OdDgUserIO* pIO, OdDbVisualSelection::SubentSelectionMode &ssm, OdGePoint3dArray &pts)
   {
     pts.clear();
     OdEdPointTrackerPtr pTracker = DgPolygonSelectionTracker::create(&pts);

     for(;;)
     {
       if( pts.size() == 0 )
       {
         pts.resize(1);
         pts[0] = pIO->getPoint(OD_T("First polygon point:"), OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);
       }

       if( (pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0 )
       {
         ssm = OdDbVisualSelection::kDisableSubents;
       }

       try
       {
         OdGePoint3d newPoint = pIO->getPoint(OD_T("Specify endpoint of line:"),
                                              OdEd::kGptNoLimCheck|OdEd::kGptNoUCS|
                                              OdEd::kInpThrowEmpty,
                                              0, OD_T("Undo"), pTracker);

         if( DgPolygonSelectionTracker::doesPolySegmentsNotIntersects(pts, newPoint) )
         {
           pts.append(newPoint);
         }
         else
         {
           pIO->putError(OD_T("Invalid point, polygon segments cannot intersect."));
         }
       }
       catch(const OdEdKeyword& kw)
       {
         if( kw.keywordIndex() == 0 )
         {
           pts.removeLast();
         }
       }
       catch (const OdEdEmptyInput)
       {
         break;
       }
     }
   }
};

OdDgSelectionSetPtr ExDgCommandContext::select(OdGsView* pGsView,
                                                 const OdString& sPrompt,
                                                   int options,
                                                     const OdDgSelectionSet* pDefVal,
                                                       const OdString& sKeywords,
                                                      OdEdDgSSetTracker* pTracker)
{
  OdDgUserIO* pIO = dbUserIO();
  OdDgDatabase* pDb = database();
  OdDgSelectionSetPtr pRes = OdDgSelectionSet::createObject(database());

  OdString sMsg;
  OdDgSelectionSetPtr pSSet = pickfirst();
  setPickfirst(0);
  OdDgSelectionSetIteratorPtr pIter;

  if( pSSet.get() && pSSet->numEntities() && !pDefVal && !GETBIT(options, OdEd::kSelIgnorePickFirst) )
  {
    merge(pIO, pRes, (options | OdEd::kSelRemove) & ~OdEd::kSelRemove, pSSet, pTracker);
    return pRes;
  }
  pSSet.release();

  OdString prompt(sPrompt);

  if(prompt.isEmpty())
  {
    prompt = OD_T("Select objects:");
  }

  OdString keywords(sKeywords);

  if(keywords.isEmpty())
  {
    keywords = OD_T("Window Last Crossing BOX ALL Fence WPolygon CPolygon Group Add Remove Multiple Previous Undo Auto Single");
  }

  if( pDefVal )
  {
    merge(pIO, pRes, (options | OdEd::kSelRemove) & ~OdEd::kSelRemove, pDefVal, pTracker);
  }

  OdGePoint3dArray pts;

  do
  {
    selection_loop_start:

    OdDbVisualSelection::SubentSelectionMode ssm = GETBIT(options, OdEd::kSelAllowSubents)? OdDbVisualSelection::kEnableSubents : OdDbVisualSelection::kDisableSubents;

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

        if( (m_pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0 )
        {
          ssm = OdDbVisualSelection::kDisableSubents;
        }

        pSSet = OdDgSelectionSet::select(database(), pGsView, 1, &pts[0], OdDbVisualSelection::kPoint, ssm);

        if( !pSSet.isNull() && pSSet->numEntities() == 0 && !GETBIT(options, OdEd::kSelSingleEntity) )
        {
          pSSet = box(pGsView, pts[0], ssm);
        }
      }
      catch(const OdEdOtherInput& inp)
      {
        if(try_lisp(inp.string()))
        {
          pSSet = OdDgSelectionSet::createObject(pDb);

          OdRxObjectPtr pObj = result();

          OdDgElementPtr pElm;

          if( pObj->isKindOf( OdDgElement::desc() ) )
          {
            pElm = pObj;

            OdDgElementId idElement = pElm->elementId();

            pSSet->append(idElement);
          }
        }
        else
          throw;
      }
    }
    catch(const OdEdKeyword& kw)
    {
      nKword = kw.keywordIndex();
    }
    catch(const OdEdEmptyInput)
    {
      if(pRes->numEntities() || GETBIT(options, OdEd::kSelAllowEmpty))
      {
        break;
      }

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

    switch( nKword )
    {
      case -1: // no op
      {

      }
      break;

      case  0: // Window
      {
        ExDgSelectionMethods::selectRect(m_pIoStream, pIO, ssm, pts);
        pSSet = OdDgSelectionSet::select(database(), pGsView, 2, pts.getPtr(), OdDbVisualSelection::kWindow, ssm);
      }
      break;

      case  1: // Last
      {
        OdDgElementIdArray ids;
        pSSet = OdDgSelectionSet::createObject(database());
        ExDgSelectionMethods::selectLast(database(), ids);
        pSSet->append(ids);
      } 
      break;

      case  2: // Crossing
      {
        ExDgSelectionMethods::selectRect(m_pIoStream, pIO, ssm, pts);
        pSSet = OdDgSelectionSet::select(database(), pGsView, 2, pts.getPtr(), OdDbVisualSelection::kCrossing, ssm);
      } 
      break;

      case  3: // BOX
      {
        pts[0] = pIO->getPoint(L"Specify first corner:", OdEd::kGptNoLimCheck|OdEd::kGptNoUCS);

        if( (m_pIoStream->getKeyState() & OdEdBaseIO::kControlIsDown) == 0 )
        {
          ssm = OdDbVisualSelection::kDisableSubents;
        }

        pSSet = box(pGsView, pts[0], ssm);
      } 
      break;

      case  4: // ALL
      {
        OdDgElementIdArray ids;
        pSSet = OdDgSelectionSet::createObject(database());
        ExDgSelectionMethods::selectAll(database(), ids);
        pSSet->append(ids);
      }
      break;

      case  5: // Fence
      {
        ExDgSelectionMethods::selectPolyline(m_pIoStream, pIO, ssm, pts);

        if( pts.size() > 1 )
        {
          pSSet = OdDgSelectionSet::select(database(), pGsView, (int)pts.size(), pts.getPtr(), OdDbVisualSelection::kFence, ssm);
        }
      } 
      break;

      case  6: // WPolygon
      {
        ExDgSelectionMethods::selectPolygon(m_pIoStream, pIO, ssm, pts);
        if( pts.size() > 2 )
        {
          pSSet = OdDgSelectionSet::select(database(), pGsView, (int)pts.size(), pts.getPtr(), OdDbVisualSelection::kWPoly, ssm);
        }
      }
      break;
      case  7: // CPolygon
      {
        ExDgSelectionMethods::selectPolygon(m_pIoStream, pIO, ssm, pts);

        if( pts.size() > 2 )
        {
          pSSet = OdDgSelectionSet::select(database(), pGsView, (int)pts.size(), pts.getPtr(), OdDbVisualSelection::kCPoly, ssm);
        }
      }
      break;

      case  8: // Group
      {
        bNYI = true;
      }
      break;

      case  9: // Add
      {
        SETBIT(options, OdEd::kSelRemove, false);
      }
      break;

      case 10: // Remove
      {
        SETBIT(options, OdEd::kSelRemove, true);
      }
      break;

      case 11: // Multiple
      {
        SETBIT(options, OdEd::kSelSinglePass, false);
      }
      break;

      case 12: // Previous
      {
        pSSet = previousSelection();
      }
      break;

      case 13: // Undo
      {
        bNYI = true;
      }
      break;

      case 14: // AUto
      {
        SETBIT(options, OdEd::kSelRemove, false); // Same as 'Add'
        SETBIT(options, OdEd::kSelSinglePass, false); // Same as 'Multiple'
      }
      break;

      case 15: // SIngle
      {
        SETBIT(options, OdEd::kSelSinglePass, true);
        goto selection_loop_start;
      }
    }

    if( !pSSet.isNull() )
    {
      merge(pIO, pRes, options, pSSet, pTracker);
    }
    else if( bNYI )
    {
      pIO->putError(OD_T("Not Yet Implemented"));
    }
  }
  while(!GETBIT(options, OdEd::kSelSinglePass));

  if( !GETBIT(options, OdEd::kSelLeaveHighlighted) )
  {
    ::highlight(pRes, false, GETBIT(options, OdEd::kSelAllowSubents));
  }

  setPreviousSelection(pRes);
  return pRes;
}

OdString ExDgCommandContext::getStringInternal(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  return m_pIoStream->getString(prompt, options, pTracker);
}

OdGePoint3d ExDgCommandContext::getPointOrReal(const OdString& prompt,
                                               int options,
                                               const OdString& keywords,
                                               OdResBuf::ValueType realType,
                                               OdEdPointTracker* pPtTracker)
{
  for(;;)
  {
    try
    {
      return getPoint(prompt, options|OdEd::kInpThrowOther| OdEd::kInpThrowEmpty,
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
          real.val = formatter().unformatLinear(other.string());
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
          real.val = formatter().unformatAngle(other.string());
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

class PointToDistTracker : public OdEdPointTracker
{
  OdGePoint3d       m_basePoint;
  OdEdRealTracker*  m_pTracker;
public:
  static OdEdPointTrackerPtr create(const OdGePoint3d& basePoint, OdEdRealTracker* pTracker)
  {
    OdSmartPtr<PointToDistTracker> pRes = OdRxObjectImpl<PointToDistTracker>::createObject();
    pRes->m_basePoint = basePoint;
    pRes->m_pTracker = pTracker;
    return pRes;
  }
  void setValue(const OdGePoint3d& pt)
  {
    m_pTracker->setValue((pt-m_basePoint).length());
  }

  int addDrawables(OdGsView* pView) { return m_pTracker->addDrawables(pView); }
  void removeDrawables(OdGsView* pView) { m_pTracker->removeDrawables(pView); }
};

double ExDgCommandContext::getReal(const OdString& sPrompt,
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
  if (!GETBIT(options, 128))
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
      pPtTracker = PointToDistTracker::create(basePt, pTracker);
    else
      pPtTracker = PointToOrientTracker::create(basePt, pTracker);
  }

  try
  {
    resDist = (getPointOrReal(prompt, options, sKeywords, vt, pPtTracker) - basePt).length();
  }
  catch(const RealInput& dist)
  {
    resDist = dist.val;
  }
  catch (const OdEdEmptyInput&)
  {
	  if (GETBIT(options, OdEd::kInpThrowEmpty))
		  throw;
	  resDist = defVal;
  }
  catch (const OdEdCancel& e) {
		  throw e;
  }
  if(pTracker)
    pTracker->setValue(resDist);
  return resDist;
}

double ExDgCommandContext::getDist(const OdString& sPrompt,
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

//class ANGBASE_Tracker : public OdEdRealTracker
//{
//  OdEdRealTracker*  m_pAngleTracker;
//  double            m_ANGBASE;
//protected:
//  ANGBASE_Tracker() {}
//public:
//  static OdEdRealTrackerPtr create(OdEdRealTracker* pAngleTracker, double ANGBASE)
//  {
//    OdSmartPtr<ANGBASE_Tracker> pRes = OdRxObjectImpl<ANGBASE_Tracker>::createObject();
//    pRes->m_pAngleTracker = pAngleTracker;
//    pRes->m_ANGBASE = ANGBASE;
//    return pRes;
//  }
//  void setValue(double orient)
//  {
//    m_pAngleTracker->setValue(orient - m_ANGBASE);
//  }
//
//  int addDrawables(OdGsView* pView) { return m_pAngleTracker->addDrawables(pView); }
//  void removeDrawables(OdGsView* pView) { m_pAngleTracker->removeDrawables(pView); }
//};

#pragma MARKMESSAGE(TODO: 3.5 - ExDgCommandContext::getAngle() does not implemented)
double ExDgCommandContext::getAngle(const OdString& sPrompt,
                                    int options,
                                    double defVal,
                                    const OdString& sKeywords,
                                    OdEdRealTracker* pTracker)
{
  //return 0;
  OdString prompt(sPrompt);

 if(prompt.isEmpty())
    prompt = OD_T("Specify an angle:");

  double ANGBASE;
  OdEdRealTrackerPtr pTrck;
  if(!GETBIT(options, OdEd::kGanNoAngBase))
  {
    ANGBASE = 0;
  }
  else
  {
    ANGBASE = m_pDb->getAngleReadoutBaseAngle();
    if(pTracker)
      pTrck = ANGBASE_Tracker::create(pTracker, ANGBASE);
  }
  return getReal(prompt, options, defVal + ANGBASE, sKeywords, pTrck) - ANGBASE;
}

OdEdPointDefTrackerPtr ExDgCommandContext::createRubberBand(const OdGePoint3d& base, OdGsModel* pModel) const
{
  return RubberBand::create(base, pModel);
}

OdEdPointDefTrackerPtr ExDgCommandContext::createRectFrame(const OdGePoint3d& base, OdGsModel* pModel) const
{
  return RectFrame::create(base, pModel);
}

OdGePoint3d ExDgCommandContext::getLASTPOINT() const
{
  return m_LASTPOINT;
}

void ExDgCommandContext::setLASTPOINT(const OdGePoint3d& pt)
{
  m_LASTPOINT = pt;
}

OdGePoint3d ExDgCommandContext::getPoint(const OdString& sPrompt,
                                         int options,
                                         const OdGePoint3d* pDefVal,
                                         const OdString& keywords,
                                         OdEdPointTracker* pTracker)
{
  OdGePoint3d res;

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
          for (; i < 3 && !sInput.isEmpty(); ++i)
          {
            OdString sCoord = ::next(sInput);
            res[i] = baseFormatter().unformatLinear(sCoord);
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
          
          m_pIoStream->putError(keywords.isEmpty() ? OD_T("Invalid point.") : OD_T("Point or option keyword required."));
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
    //if(!GETBIT(options, OdEd::kGptNoLimCheck) && database()->getLIMCHECK())
    //{
    //  OdGeExtents2d ext(database()->getLIMMIN(), database()->getLIMMAX());
    //  if(!ext.contains((const OdGePoint2d&)res))
    //  {
    //    putError(OD_T("**Outside limits"));
    //    continue;
    //  }
    //}
    
    dbUserIO()->setLASTPOINT(res);
    return res;
  }
}

int ExDgCommandContext::getInt(const OdString& sPrompt,
                               int options,
                               int defVal,
                               const OdString& sKeywords,
                               OdEdIntegerTracker* pTracker)
{
  OdString prompt(sPrompt);

  if(prompt.isEmpty())
    prompt = OD_T("Enter an integer value:");
  OdString sInput = ExDgCommandContext::getString(prompt, 0, OdString::kEmpty, sKeywords);
  if (sInput.isEmpty())
    return defVal;
  return ::odStrToInt(sInput);
}

int ExDgCommandContext::getKeyword(const OdString& sPrompt,
                                   const OdString& sKeywords,
                                   int defVal,
                                   int options,
                                   OdEdIntegerTracker* pTracker)
{
  OdString prompt(sPrompt);

  KWIndex kwIndex(sKeywords);
  OdString pmt;
  if(prompt.isEmpty())
  {
    pmt = OD_T("Enter one of keywords [");
    for(OdUInt32 i=0; i<kwIndex.size(); ++i)
    {
      pmt += kwIndex.at(i).keyword();
      pmt += OD_T("/");
    }
    pmt += OD_T("]");
    if(!GETBIT(options, OdEd::kInpThrowEmpty))
    {
      pmt += OD_T("<");
      pmt += kwIndex.at(defVal).keyword();
      pmt += OD_T(">");
    }
    pmt += OD_T(":");
    prompt = pmt;
  }

  bool bDef = (OdUInt32(defVal) < kwIndex.size());
  for(;;)
  {
    SETBIT(options, OdEd::kGstNoEmpty, !bDef);
    OdString sInput = ExDgCommandContext::getString(prompt, options);
    if(bDef && sInput.isEmpty())
      return defVal;
    OdUInt32 n = kwIndex.find(sInput);
    if(n < kwIndex.size())
      return n;
    putString(OD_T(""));
    putError(OD_T("Invalid option keyword."));
  }
}

double ExDgCommandContext::getReal(const OdString& sPrompt, 
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
    OdString sInput = ExDgCommandContext::getString(prompt, options, OdString::kEmpty, sKeywords);
    if (sInput.isEmpty())
      return defVal;
    try
    {
      return formatter().unformatL(sInput);
    }
    catch(const OdError& e)
    {
      if(e.code()!=eInvalidInput)
        throw;
      if(GETBIT(options, OdEd::kInpThrowOther))
        throw OdEdOtherInput(sInput);
      putError(sKeywords.isEmpty() ? OD_T("Requires real value.") : OD_T("Requires real value or optional keyword."));
    }
  }
}

OdString ExDgCommandContext::getString(const OdString& sPrompt, 
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

OdString ExDgCommandContext::getFilePath(const OdString& prompt, 
                                         int options,
                                         const OdString& dialogCaption, 
                                         const OdString& defExt,
                                         const OdString& fileName,
                                         const OdString& filter,
                                         const OdString& keywords,
                                         OdEdStringTracker* pTracker)
{
  OdString sInput;

#pragma MARKMESSAGE(TODO: 3.5 - ExDgCommandContext::getFilePath() does not implemented)
/*
  if(!database()->appServices()->getFILEDIA())
  {
    sInput = OdDgUserIO::getFilePath(prompt, options, dialogCaption, defExt, fileName, filter, keywords, pTracker);
    if(sInput != OD_T("~"))
    {
      sInput.trimLeft(' ');
      sInput.trimRight(' ');
      return sInput;
    }
  }
*/
  sInput = database()->appServices()->fileDialog(options & 3, dialogCaption, defExt, fileName, filter);
  if( sInput == OD_T("*unsupported*") )
    sInput = OdDgUserIO::getFilePath(prompt, options, dialogCaption, defExt, fileName, filter, keywords, pTracker);

  return sInput;
}

void ExDgCommandContext::putString(const OdString& string)
{
  m_pIoStream->putString(string);
}

OdDgCmColor ExDgCommandContext::getColor(const OdString& sPrompt,
                                       int options,
                                       const OdDgCmColor* pDefVal,
                                       const OdString& sKeywords,
                                       OdEdColorTracker* pTracker)
{
  return OdDgCmColor();
#pragma MARKMESSAGE(TODO: 3.5 - ExDgCommandContext::getFilePath() does not implemented)
/*
  OdString prompt(sPrompt);

  if(prompt.isEmpty())
    prompt = OD_T("Specify a color:");

  for(;;)
  {
    OdString sInput = ExDgCommandContext::getString(prompt, options,
                        pDefVal ? (const OdString&)formatter().formatColor(*pDefVal) : OdString::kEmpty, sKeywords);
    try
    {
      return formatter().unformatColor(sInput);
    }
    catch(const OdError& e)
    {
      if(e.code()!=eInvalidInput)
        throw;
      if(GETBIT(options, OdEd::kInpThrowOther))
        throw OdEdOtherInput(sInput);
      putError(sKeywords.isEmpty() ? OD_T("Invalid color value.") : OD_T("Requires color value or optional keyword."));
    }
  }
*/
}

bool ExDgCommandContext::try_lisp(const OdString& input)
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
        s.trimLeft(OD_T(" \r\n\t\""));
        OdUInt64 handle = 0;

        if( odSScanf(s, PRIx64W, &handle) )
        {
          s.trimLeft(OD_T("0123456789ABCDEFabcdef"));
          s.trimLeft(OD_T(" \r\n\t\""));
          if(s[0]==L')')
          {
            OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtHandle);
            pRb->setHandle(OdDbHandle(handle));
            setResult(pRb);
            return true;
          }
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
