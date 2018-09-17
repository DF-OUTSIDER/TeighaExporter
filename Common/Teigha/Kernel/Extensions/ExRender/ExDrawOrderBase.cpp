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
// Basic draw order support for devices based on own display lists

#include "OdaCommon.h"
#include "RxObjectImpl.h"
#include "SaveState.h"
#include "ExDrawOrderBase.h"
#include "Gs/GsContainerNode.h"
#include "Gs/GsEntityNode.h"
#include "Gs/GsLayerNode.h"

// Device

OdDrawOrderBaseDevice::OdDrawOrderBaseDevice()
  : OdGsBaseVectorizeDevice()
{
  disableSpatialIndex(true);
  enableDisplayOffLayers(true);
}

OdDrawOrderBaseDevice::~OdDrawOrderBaseDevice()
{
  if (!m_pMetafileContext.isNull())
    m_pMetafileContext->m_pDevice = NULL;
}

void OdDrawOrderBaseDevice::update(OdGsDCRect* pUpdatedRect)
{
  if (m_pMetafileContext.isNull())
  {
    m_pMetafileContext = OdRxObjectImpl<MetafileContext>::createObject();
    m_pMetafileContext->m_pDevice = this;
  }
  setCheckDrawOrderInheritance( false );
  preprocess();
  OdGsBaseVectorizeDevice::update(pUpdatedRect);
  sync();
  setCheckDrawOrderInheritance( false );
}

void OdDrawOrderBaseDevice::syncView(int id)
{
  OdDrawOrderBaseView *pView = static_cast<OdDrawOrderBaseView*>(viewAt(id));
  pView->sync();
}

void OdDrawOrderBaseDevice::sync()
{
  { // Run synchronization of separate views
    for (int i = 0; i < numViews(); i++)
      syncView(i);
  }
}

void OdDrawOrderBaseDevice::preprocessView(int id)
{
  OdDrawOrderBaseView *pView = static_cast<OdDrawOrderBaseView*>(viewAt(id));
  pView->preprocess();
}

void OdDrawOrderBaseDevice::preprocess()
{
  { // Run preprocessing of separate views
    for (int i = 0; i < numViews(); i++)
      preprocessView(i);
  }
}

// View

OdDrawOrderBaseView::OdDrawOrderBaseView()
  : OdGsBaseVectorizeView()
{
  m_overlayData.substituteAllocator(&m_overlayAlloc);
}

OdDrawOrderBaseView::~OdDrawOrderBaseView()
{
  eraseAll(); // to detach model reactors, since vtbl become incomplete in base classes
  m_overlayData.resetAllocator();
}

OdDrawOrderBaseDevice* OdDrawOrderBaseView::device()
{
  return static_cast<OdDrawOrderBaseDevice*>(OdGsBaseVectorizeView::device());
}

void OdDrawOrderBaseView::drawOrderInvalidate()
{
  for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
  {
    DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
    pOrder->m_metafileOrder.setPhysicalLength(0);
    OdUInt32 nDrawables = pOrder->m_drawableInfo.size();
    DrawableInfo *pDrawables = pOrder->m_drawableInfo.asArrayPtr();
    for (OdUInt32 nDrawable = 0; nDrawable < nDrawables; nDrawable++)
    {
      pDrawables[nDrawable].m_nFrom = pDrawables[nDrawable].m_nLen = 0;
      pDrawables[nDrawable].m_flags = kFullRegeneration;
    }
  }
}

void OdDrawOrderBaseView::clearDrawOrderAccums()
{
  for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
  {
    DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
    pOrder->m_metafileOrderAccum.clear();
  }
}

OdDrawOrderBaseView::ViewInfo::ViewInfo()
  : m_mode(OdGsView::kNone)
  , m_regenType(eOdGiRegenTypeInvalid)
  , m_annoScale(NULL)
{
}

bool OdDrawOrderBaseView::viewChangesAndUpdate()
{
  bool bRVal = false;
  if (m_viewInfo.m_mode != m_renderMode)
  {
    m_viewInfo.m_mode = m_renderMode;
    bRVal = true;
  }
  if (m_viewInfo.m_regenType != m_regenerationType)
  {
    m_viewInfo.m_regenType = m_regenerationType;
    bRVal = true;
  }
  OdDbStub* annoScale = annotationScaleId();
  if (m_viewInfo.m_annoScale != annoScale)
  {
    m_viewInfo.m_annoScale = annoScale;
    bRVal = true;
  }
  return bRVal;
}

void OdDrawOrderBaseView::preprocess()
{
  bool bViewChanges = viewChangesAndUpdate();
  bool bAllowInheritanceNotification = true;
  if( isDependentGeometryView() )
  {
    bAllowInheritanceNotification = false;
  }
  OdDrawOrderBaseDevice* pBaseDevice = device();
  if( bViewChanges && bAllowInheritanceNotification )
    pBaseDevice->setCheckDrawOrderInheritance( true );
  for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
  {
    DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
    if (pOrder->m_drawableInfo.isEmpty())
    {
      pOrder->m_bMetafileOrderValid = true;
      pOrder->m_nInvalidFrom = pOrder->m_nInvalidLen = 0;
      pOrder->m_nValidFrom = pOrder->m_nValidLen = 0;
    }
    OdUInt32 nDrawablesIn = m_drawables.size(), nDrawableOut = 0;
    const DrawableHolder *pDrawables = m_drawables.getPtr();
    for (OdUInt32 nDrawable = 0; nDrawable < nDrawablesIn; nDrawable++)
    {
      if (device()->gsModelOverlay(pDrawables[nDrawable].m_pGsModel.get()) != oIt.overlayId())
        continue;
      if (pOrder->m_drawableInfo.size() > nDrawableOut)
      {
        DrawableInfo &drawableInfo = pOrder->m_drawableInfo[nDrawableOut];
        if ((drawableInfo.m_drawableId != pDrawables[nDrawable].m_drawableId) ||
            (drawableInfo.m_pDrawable != pDrawables[nDrawable].m_pDrawable.get()) ||
            (drawableInfo.m_pGsModel != pDrawables[nDrawable].m_pGsModel.get()))
        { // List isn't actual any more, require recreation
          pOrder->m_bMetafileOrderValid = false;
          pOrder->m_nInvalidFrom = drawableInfo.m_nFrom;
          pOrder->m_nInvalidLen = pOrder->m_metafileOrder.size() - pOrder->m_nInvalidFrom;
          pOrder->m_nValidFrom = pOrder->m_nInvalidFrom;
          pOrder->m_nValidLen = 0;
          for( OdUInt32 nD = nDrawable; nD < pOrder->m_drawableInfo.size(); ++nD )
          {
            DrawableInfo &drawableInfo = pOrder->m_drawableInfo[nD];
            if( drawableInfo.m_nFrom >= pOrder->m_nInvalidLen ) drawableInfo.m_nFrom -= pOrder->m_nInvalidLen;
          }
          pOrder->m_metafileOrder.resize(pOrder->m_nInvalidFrom);
          pOrder->m_drawableInfo.resize(nDrawableOut);
          onMetafileOrderChanged(oIt.overlayId(), pOrder->m_nInvalidFrom, pOrder->m_nInvalidLen, NULL, 0 );
          if( bAllowInheritanceNotification )
            pBaseDevice->setCheckDrawOrderInheritance( true );
        }
        else if ((drawableInfo.m_type == DrawableInfo::kTransient) &&
                 !pDrawables[nDrawable].m_pGsModel.isNull())
        {
          if (pDrawables[nDrawable].m_pGsRoot && (pDrawables[nDrawable].m_pGsRoot->nodeType() == kContainerNode))
            drawableInfo.m_type = DrawableInfo::kContainer;
          else
            drawableInfo.m_type = DrawableInfo::kPersistent;
        }
      }
      if (pOrder->m_drawableInfo.size() <= nDrawableOut)
      { // New drawable
        DrawableInfo newDrawable;
        newDrawable.m_drawableId = pDrawables[nDrawable].m_drawableId;
        newDrawable.m_pDrawable = pDrawables[nDrawable].m_pDrawable.get();
        newDrawable.m_pGsModel = pDrawables[nDrawable].m_pGsModel.get();
        newDrawable.m_nFrom = newDrawable.m_nLen = 0;
        newDrawable.m_nLink = nDrawable; // link with original array
        newDrawable.m_flags = kFullRegeneration;
        if (pDrawables[nDrawable].m_pGsRoot)
        {
          if (pDrawables[nDrawable].m_pGsRoot->nodeType() == kContainerNode)
            newDrawable.m_type = DrawableInfo::kContainer;
          else
            newDrawable.m_type = DrawableInfo::kPersistent;
        }
        else
          newDrawable.m_type = DrawableInfo::kTransient;
        pOrder->m_drawableInfo.push_back(newDrawable);
        if( bAllowInheritanceNotification )
          pBaseDevice->setCheckDrawOrderInheritance( true );
      }
      else
      {
        DrawableInfo &drawableInfo = pOrder->m_drawableInfo[nDrawableOut];
        switch (drawableInfo.m_type)
        {
          case DrawableInfo::kTransient:
            drawableInfo.m_flags = kFullRegeneration;
            if( bAllowInheritanceNotification )
              pBaseDevice->setCheckDrawOrderInheritance( true );
          break;
          case DrawableInfo::kContainer:
            {
              OdGsContainerNode *pNode = static_cast<OdGsContainerNode*>(pDrawables[nDrawable].m_pGsRoot);
              if (!pNode) // Could be in case if loaded from GsState
                pNode = static_cast<OdGsContainerNode*>(drawableAt(m_drawables[nDrawable])->gsNode());
              const OdUInt32 nVpId = pNode->viewportId(*this);
              if (pDrawables[nDrawable].m_pGsModel->hasChangedLayers() ||
                  !pNode->firstEntityNode(nVpId) || pNode->firstEntityNode(nVpId)->entityUnerased() ||
                  !pNode->entityListValid(nVpId) || (drawableInfo.m_flags == kFullRegeneration))
              {
                drawableInfo.m_flags = kFullRegeneration;
                if( bAllowInheritanceNotification )
                  pBaseDevice->setCheckDrawOrderInheritance( true );
              }
              else if (pNode->childrenRegenDraw(nVpId) ||
                pNode->needRegen(nVpId) || pNode->numberOfChildrenErased(nVpId) ||
                bViewChanges || (drawableInfo.m_flags == kPartialRegeneration))
              {
                drawableInfo.m_flags = kPartialRegeneration;
                if( bAllowInheritanceNotification )
                  pBaseDevice->setCheckDrawOrderInheritance( true );
              }
              else if (m_refModels[(OdGsModel*)pDrawables[nDrawable].m_pGsModel.get()].bHighlighted)
                drawableInfo.m_flags = kHighlightModified;
              if (drawableInfo.m_flags)
                m_refModels[(OdGsModel*)pDrawables[nDrawable].m_pGsModel.get()].bHighlighted = false;
            }
          break;
          case DrawableInfo::kPersistent:
            {
              // willn't work correctly w/o context. context is in sources, no chance to check
              //OdGsEntityNode *pNode = static_cast<OdGsEntityNode*>(getRootNode(m_drawables[nDrawable]));
              //if (!pNode)
                drawableInfo.m_flags = kFullRegeneration;
                if( bAllowInheritanceNotification )
                  pBaseDevice->setCheckDrawOrderInheritance( true );
              //else
              //  if (!pNode->metafile(*this, NULL))
              //    drawableInfo.m_flags = kFullRegeneration;
            }
          break;
        }
      }
      nDrawableOut++;
    }
    if (pOrder->m_drawableInfo.size() > nDrawableOut)
    {
      pOrder->m_bMetafileOrderValid = false;
      pOrder->m_nInvalidFrom = pOrder->m_drawableInfo[nDrawableOut].m_nFrom;
      pOrder->m_nInvalidLen = pOrder->m_metafileOrder.size() - pOrder->m_nInvalidFrom;
      pOrder->m_nValidFrom = pOrder->m_nInvalidFrom;
      pOrder->m_nValidLen = 0;
      pOrder->m_metafileOrder.resize(pOrder->m_nInvalidFrom);
      pOrder->m_drawableInfo.resize(nDrawableOut);
      onMetafileOrderChanged(oIt.overlayId(), pOrder->m_nInvalidFrom, pOrder->m_nInvalidLen, NULL, 0 );
      pBaseDevice->setCheckDrawOrderInheritance( true );
    }
  }
}

//SEA list cmp
OdDrawOrderBaseView::ActionSequence::ActionSequence()
{
  m_pFirst = NULL;
  m_pLast = NULL;
}

OdDrawOrderBaseView::ActionSequence::~ActionSequence()
{
  clear();
}

void OdDrawOrderBaseView::ActionSequence::clear()
{
  ConvertorAction* pAction = m_pFirst;
  while( pAction )
  {
    ConvertorAction* tmp = pAction->m_pNext;
    pAction->m_pNext = NULL;
    pAction->m_pPrev = NULL;
    delete pAction;
    pAction = tmp;
  }
  m_pFirst = NULL;
  m_pLast = NULL;
}

void OdDrawOrderBaseView::ActionSequence::addAction( DiffAction action, OdUInt32 count )
{
  if( !m_pLast )
  {
    m_pLast = new ConvertorAction;
    m_pLast->m_action = action;
    m_pLast->m_nCount = count;
    m_pFirst = m_pLast;
  }
  else if( m_pLast->m_action == action )
  {
    m_pLast->m_nCount += count;
  }
  else
  {
    ConvertorAction* pAction = new ConvertorAction;
    pAction->m_action = action;
    pAction->m_nCount = count;
    pAction->m_pPrev = m_pLast;
    m_pLast->m_pNext = pAction;
    m_pLast = pAction;
  }
}

OdDrawOrderBaseView::MapBasedComparer::MapBasedComparer()
{
}

OdDrawOrderBaseView::MapBasedComparer::~MapBasedComparer()
{
}

void OdDrawOrderBaseView::MapBasedComparer::setOriginal( const OdUInt64* pList, OdUInt32 size )
{
  m_map.clear();
  if( !pList || size == 0 ) return;
  for( OdUInt32 i = 0; i < size; ++i )
  {
    m_map[ pList[ i ] ] = Options( i );
  }
}

void OdDrawOrderBaseView::MapBasedComparer::diff( const OdUInt64* pList, OdUInt32 size, ActionSequence& resAs )
{
  resAs.clear();
  ActionSequence as;
  if( m_map.empty() )
  {
    resAs.addAction( kAdd, size );
    //setOriginal( pList, size );
    return;
  }
  if( size == 0 )
  {
    resAs.addAction( kRemove, (OdUInt32)m_map.size() );
    //m_map.clear();
    return;
  }
  //1. Parse list
  OdUInt32 prevPos = 0;
  bool bFndFirst = false;
  for( OdUInt32 i = 0; i < size; ++i )
  {
    std::map< OdUInt64, Options >::iterator it = m_map.find( pList[ i ] );
    if( it == m_map.end() )
    {
      as.addAction( kAdd );
    }
    else
    {
      if( !bFndFirst )
      {
        it->second.m_bTouched = true;
        prevPos = it->second.m_pos;
        as.addAction( kNoChanges );
        bFndFirst = true;
        continue;
      }
      if( prevPos < it->second.m_pos )
      {
        if( it->second.m_bTouched )
        {
          //something is wrong, possible non-unique items in list
          ODA_FAIL();
          resAs.addAction( kRemove, (OdUInt32)m_map.size() );
          resAs.addAction( kAdd, size );
          return;
        }
        it->second.m_bTouched = true;
        prevPos = it->second.m_pos;
        as.addAction( kNoChanges );
      }
      else
      {
        as.addAction( kAdd );
      }
    }
  }
  m_removeSet.clear();
  //2. Collect "remove" elements
  std::map< OdUInt64, Options >::iterator mapIt = m_map.begin();
  while( mapIt != m_map.end() )
  {
    if( !mapIt->second.m_bTouched )
    {
      m_removeSet.insert( mapIt->second.m_pos );
    }
    ++mapIt;
  }
  //3. Check "the same lists" case:
  //no removes, no adds, only NoChanges
  if( m_removeSet.empty() && as.top() )
  {
    if( as.top()->m_action == kNoChanges && as.top()->m_pNext == NULL )
    {
      resAs.addAction( kNoChanges, as.top()->m_nCount );
      return;
    }
  }
  //4. Apply difference to map and collect summary action sequence
  std::set< OdUInt32 >::iterator setIt = m_removeSet.begin();
  bool bRun = !(m_removeSet.empty()) || ( as.top() != NULL );
  ActionSequence::ConvertorAction* pAction = as.top();
  OdUInt32 pos = 0;
  while( bRun )
  {
    while( setIt != m_removeSet.end() )
    {
      if( pos == *setIt )
      {
        resAs.addAction( kRemove );
        ++setIt;
        ++pos;
      }
      else
      {
        break;
      }
    }
    if( pAction )
    {
      if( pAction->m_action == kAdd )
      {
        resAs.addAction( kAdd, pAction->m_nCount );
        pAction->m_nCount = 0;
      }
      else
      {
        while( pAction->m_nCount )
        {
          resAs.addAction( kNoChanges );
          ++pos;
          pAction->m_nCount--;
          if( setIt != m_removeSet.end() )
          {
            if( *setIt == pos )
            {
              break;
            }
            if( *setIt < pos )
            {
              break;
            }
          } 
        }
      }
      if( pAction->m_nCount == 0 )
      {
        pAction = pAction->m_pNext;
      }
    }
    bRun = ( setIt != m_removeSet.end() ) && ( pAction != NULL );
  }
  while( setIt != m_removeSet.end() )
  {
    resAs.addAction( kRemove );
    ++setIt;
  }
  while( pAction )
  {
    resAs.addAction( pAction->m_action, pAction->m_nCount );
    pAction = pAction->m_pNext;
  }  
  //setOriginal( pList, size );
}

void OdDrawOrderBaseView::listCmp( OdUInt64Vector& originalList,  OdUInt64Vector& newList, OdUInt32 nDrawInfo, DrawableInfo* drawInfo, ActionSequence& result  )
{
  result.clear();
  OdUInt32 lenOrig = drawInfo[nDrawInfo].m_nLen;
  OdUInt32 lenNew = newList.size();
  m_mbCmp.setOriginal( originalList.asArrayPtr(), lenOrig );
  m_mbCmp.diff( newList.asArrayPtr(), newList.size(), result );
}

void OdDrawOrderBaseView::sync()
{
  // Means that metafiles modified somehow (added/modified/erased/off/highlight). Query new order for this case.
  // @@@TODO: need filter somehow, if entities only modified or erased other way can be used to modify order,
  //          for example availability of kRegenDraw entity in drawing will cause order query for each update,
  //          but actually only new metafiles must be placed on place where previous metafiles erased.
  VectorizerAutoPtr vect(*this, true);
  DrawableHolderArray drawablesTmp = m_drawables;
  const bool bForPartialUpdate = device()->drawOrderForPartialUpdate();

  if( device()->isCheckDrawOrderInheritance() )
  {
    for( int i = 0; i < device()->numViews(); ++i )
    {
      const OdDrawOrderBaseView* pView = static_cast<const OdDrawOrderBaseView*>(device()->viewAt( i ));
      if( pView == this ) break;
      for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
      {
        DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
        if( pOrder->m_pConnectedView )
        {
          if( !isContentCompatibleOverlay( pOrder->m_pConnectedView, pOrder, oIt.overlayId() )
            || !isMetafilesCompatibleOverlays( oIt.overlayId(), pOrder->m_pConnectedView ) )
          {
            pOrder->m_bBreakInheritance = true;
            pOrder->m_pConnectedView = NULL;
          }
          else
          {
            continue;
          }
        }
        if( isContentCompatibleOverlay( pView, pOrder, oIt.overlayId() ) )
        {
          if( isMetafilesCompatibleOverlays( oIt.overlayId(), pView ) )
          {
            pOrder->m_pConnectedView = pView;
            onMetafileOrderInheritance( oIt.overlayId(), pView );
            pOrder->m_bBreakInheritance = false;
            break;
          }
        }
      }
    }
  }

#define CMP_LIST 1
  for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
  {
    DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
    OdUInt32 nDrawables = pOrder->m_drawableInfo.size();
    DrawableInfo *drawInfo = pOrder->m_drawableInfo.asArrayPtr();
    OdUInt32 nInvalidCtr = 0, nValidCtr = 0;
    for (OdUInt32 nDrawInfo = 0; nDrawInfo < nDrawables; nDrawInfo++)
    {
      if (drawInfo[nDrawInfo].m_flags != 0)
      {
        m_drawables.clear();
        m_drawables.append(drawablesTmp[drawInfo[nDrawInfo].m_nLink]);
        const bool bContainer = (m_drawables.first().m_pGsRoot &&
                                (m_drawables.first().m_pGsRoot->nodeType() == kContainerNode));
        if (m_drawables.first().m_pGsModel.isNull() && m_drawables.first().m_pMetafile.isNull())
          pOrder->m_metafileOrderAccum.clear();
        else
          static_cast<OdDrawOrderBaseVectorizer*>(vect.get())->drawOrderQuery(bContainer);
        OdUInt64Vector &nDrawOrd = pOrder->m_metafileOrderAccum;

        m_drawables = drawablesTmp;

        bool bCheckedValid = false;
        OdUInt32 nInvalidFrom = 0;
        OdUInt32 nInvalidBack = 0;
        if ((drawInfo[nDrawInfo].m_flags != kHighlightModified) && !bForPartialUpdate)
        {
#ifdef CMP_LIST
          listCmp( pOrder->m_metafileOrder, nDrawOrd, nDrawInfo, drawInfo, m_diffActions );
          bCheckedValid = true;
          if( m_diffActions.top() )
          {
            if( m_diffActions.top()->m_pNext == NULL && m_diffActions.top()->m_action == kNoChanges )
            {
              bCheckedValid = true;
            }
            else
            {
              bCheckedValid = false;
            }
          }
          if (bCheckedValid && (nDrawOrd.size() == 0))
          {
            drawInfo[nDrawInfo].m_nFrom = nValidCtr;
          }
#else
          OdUInt32 nMetafilesToCheck = odmin(drawInfo[nDrawInfo].m_nLen, nDrawOrd.size());
          for (OdUInt32 nChk = 0; nChk < nMetafilesToCheck; nChk++)
          {
            if (pOrder->m_metafileOrder.getPtr()[drawInfo[nDrawInfo].m_nFrom + nChk] != nDrawOrd.getPtr()[nChk])
            {
              bCheckedValid = false;
              nInvalidFrom = nChk;
              break;
            }
          }
          if ((drawInfo[nDrawInfo].m_nLen == nDrawOrd.size()) && !bCheckedValid)
          {   
            for (OdUInt32 nChkBk = nMetafilesToCheck; nChkBk > 0;)
            {
              nChkBk--;
              if (pOrder->m_metafileOrder.getPtr()[drawInfo[nDrawInfo].m_nFrom + nChkBk] != nDrawOrd.getPtr()[nChkBk])
              {
                nInvalidBack = nMetafilesToCheck - (nChkBk + 1);
                break;
              }
            }
          }
          if (bCheckedValid && (drawInfo[nDrawInfo].m_nLen != nDrawOrd.size()))
          {
            nInvalidFrom = nMetafilesToCheck;
            bCheckedValid = false;
          }
          if (bCheckedValid && (nDrawOrd.size() == 0))
          {
            drawInfo[nDrawInfo].m_nFrom = nValidCtr;
          }
#endif
        }
        else
          bCheckedValid = true;

        if (!bCheckedValid)
        {
#ifdef CMP_LIST
          pOrder->m_bMetafileOrderValid = false;
          ActionSequence::ConvertorAction* pAction = m_diffActions.top();
          OdUInt32 nOrigPos = 0;
          OdUInt32 nNewPos = 0;
          OdUInt32 nErased = 0;
          //run removes
          while( pAction )
          {
            if( pAction->m_action == kNoChanges )
            {
              nOrigPos += pAction->m_nCount;
            }
            else if( pAction->m_action == kRemove )
            {
              pOrder->m_metafileOrder.erase( pOrder->m_metafileOrder.begin() + nValidCtr + nOrigPos - nErased,
              pOrder->m_metafileOrder.begin() + nValidCtr + nOrigPos - nErased + pAction->m_nCount );
              onMetafileOrderChanged(oIt.overlayId(), nValidCtr + nOrigPos - nErased, pAction->m_nCount, NULL, 0 );
              nErased += pAction->m_nCount;
              nOrigPos += pAction->m_nCount;
              for (OdUInt32 nD = nDrawInfo; nD < nDrawables; nD++)
              {
                if( pOrder->m_drawableInfo[ nD ].m_nFrom >= pAction->m_nCount )
                {
                  pOrder->m_drawableInfo[ nD ].m_nFrom -= pAction->m_nCount;
                }
              }
            }
            pAction = pAction->m_pNext;
          }
          pAction = m_diffActions.top();
          while( pAction )
          {
            if( pAction->m_action == kNoChanges )
            {
              nNewPos += pAction->m_nCount;
            }
            else if( pAction->m_action == kAdd )
            {
              pOrder->m_metafileOrder.insert(pOrder->m_metafileOrder.begin() + nValidCtr + nNewPos,
                nDrawOrd.begin() + nNewPos, nDrawOrd.begin() + nNewPos + pAction->m_nCount );
              onMetafileOrderChanged(oIt.overlayId(), nValidCtr + nNewPos, 0, nDrawOrd.getPtr() + nNewPos , pAction->m_nCount );
              nNewPos += pAction->m_nCount;
              for (OdUInt32 nD = nDrawInfo; nD < nDrawables; nD++)
              {
                pOrder->m_drawableInfo[ nD ].m_nFrom += pAction->m_nCount;
              }
            }
            pAction = pAction->m_pNext;
          }
          nInvalidCtr += drawInfo[nDrawInfo].m_nLen;
          drawInfo[nDrawInfo].m_nFrom = nValidCtr;
          drawInfo[nDrawInfo].m_nLen = nDrawOrd.size();
          nValidCtr += nDrawOrd.size();
#else
          if (pOrder->m_bMetafileOrderValid)
          {
            pOrder->m_bMetafileOrderValid = false;
            pOrder->m_nInvalidFrom = nInvalidCtr + nInvalidFrom;
            pOrder->m_nInvalidLen = drawInfo[nDrawInfo].m_nLen - nInvalidFrom - nInvalidBack;
            pOrder->m_nValidFrom = nValidCtr + nInvalidFrom;
            pOrder->m_nValidLen = nDrawOrd.size() - nInvalidFrom - nInvalidBack;
          }
          else
          {
            if (pOrder->m_nInvalidFrom > nInvalidCtr + nInvalidFrom)
            {
              pOrder->m_nInvalidLen += pOrder->m_nInvalidFrom - (nInvalidCtr + nInvalidFrom);
              pOrder->m_nInvalidFrom = nInvalidCtr + nInvalidFrom;
            }
            if ((nInvalidCtr + nInvalidFrom + (drawInfo[nDrawInfo].m_nLen - nInvalidFrom - nInvalidBack)) >
                (pOrder->m_nInvalidFrom + pOrder->m_nInvalidLen))
              pOrder->m_nInvalidLen -= (pOrder->m_nInvalidFrom + pOrder->m_nInvalidLen) -
                                       (nInvalidCtr + nInvalidFrom + (drawInfo[nDrawInfo].m_nLen - nInvalidFrom - nInvalidBack));
            if (pOrder->m_nValidFrom > nValidCtr + nInvalidFrom)
            {
              pOrder->m_nValidLen += pOrder->m_nValidFrom - (nValidCtr + nInvalidFrom);
              pOrder->m_nValidFrom = nValidCtr + nInvalidFrom;
            }
            if ((nValidCtr + nInvalidFrom + (nDrawOrd.size() - nInvalidFrom - nInvalidBack)) >
                (pOrder->m_nValidFrom + pOrder->m_nValidLen))
              pOrder->m_nValidLen -= (pOrder->m_nValidFrom + pOrder->m_nValidLen) -
                                     (nValidCtr + nInvalidFrom + (nDrawOrd.size() - nInvalidFrom - nInvalidBack));
          }
          if ((drawInfo[nDrawInfo].m_nLen == nDrawOrd.size()) && !nDrawOrd.isEmpty())
          {
            ::memcpy(pOrder->m_metafileOrder.asArrayPtr() + nValidCtr + nInvalidFrom,
                     nDrawOrd.getPtr() + nInvalidFrom, sizeof(OdUInt64) * (drawInfo[nDrawInfo].m_nLen - nInvalidFrom - nInvalidBack));
          }
          else
          {
            if (drawInfo[nDrawInfo].m_nLen - nInvalidFrom - nInvalidBack)
              pOrder->m_metafileOrder.erase(pOrder->m_metafileOrder.begin() + nValidCtr + nInvalidFrom,
                pOrder->m_metafileOrder.begin() + nValidCtr + nInvalidFrom + (drawInfo[nDrawInfo].m_nLen - nInvalidFrom - nInvalidBack));
            pOrder->m_metafileOrder.insert(pOrder->m_metafileOrder.begin() + nValidCtr + nInvalidFrom,
              nDrawOrd.begin() + nInvalidFrom, nDrawOrd.end() - nInvalidBack);
          }
          nInvalidCtr += drawInfo[nDrawInfo].m_nLen;
          drawInfo[nDrawInfo].m_nFrom = nValidCtr;
          drawInfo[nDrawInfo].m_nLen = nDrawOrd.size();
          nValidCtr += nDrawOrd.size();
#endif
        }
        else
        {
          nInvalidCtr += drawInfo[nDrawInfo].m_nLen;
          nValidCtr += drawInfo[nDrawInfo].m_nLen;
        }

        drawInfo[nDrawInfo].m_flags = 0;
        nDrawOrd.clear();
      }
      else
      {
        nInvalidCtr += drawInfo[nDrawInfo].m_nLen;
        nValidCtr += drawInfo[nDrawInfo].m_nLen;
      }
    }
    if (!pOrder->m_bMetafileOrderValid)
    {
      pOrder->m_bMetafileOrderValid = true;
      onMetafileOrderChanged(oIt.overlayId(), pOrder->m_metafileOrder.getPtr(), pOrder->m_metafileOrder.size());
#ifndef CMP_LIST
      onMetafileOrderChanged(oIt.overlayId(), pOrder->m_nInvalidFrom, pOrder->m_nInvalidLen,
                             pOrder->m_metafileOrder.getPtr() + pOrder->m_nValidFrom, pOrder->m_nValidLen);
#endif
    }
    if( pOrder->m_bBreakInheritance )
    {
      onMetafileOrderInheritance( oIt.overlayId(), NULL );
      onMetafileOrderChanged(oIt.overlayId(), 0, 0, pOrder->m_metafileOrder.asArrayPtr(), pOrder->m_metafileOrder.size() );
      pOrder->m_bBreakInheritance = false;
    }
  }
}

bool OdDrawOrderBaseView::add(OdGiDrawable* pSceneGraph, OdGsModel* pModel)
{
  if (pModel)
  {
    ModelRef &mRef = m_refModels[pModel];
    if (!mRef.nRefs)
      pModel->addModelReactor(this);
    mRef.nRefs++;
  }
  return OdGsBaseVectorizeView::add(pSceneGraph, pModel);
}

bool OdDrawOrderBaseView::erase(OdGiDrawable* sceneGraph)
{
  {
    for (OdUInt32 n = 0; n < m_drawables.size(); n++)
    {
      DrawableHolder& holder = m_drawables[n];
	    if ((sceneGraph->isPersistent() && (holder.m_drawableId == sceneGraph->id())) || (drawableAt(holder).get() == sceneGraph))
      {
        if (!holder.m_pGsModel.isNull())
        {
          ModelRef &mRef = m_refModels[holder.m_pGsModel.get()];
          if (mRef.nRefs <= 1)
          {
            holder.m_pGsModel->removeModelReactor(this);
            m_refModels.erase(holder.m_pGsModel.get());
          }
          else
            mRef.nRefs--;
        }
        break;
      }
    }
  }
  return OdGsBaseVectorizeView::erase(sceneGraph);
}

void OdDrawOrderBaseView::eraseAll()
{
  {
    for (OdUInt32 n = 0; n < m_drawables.size(); n++)
    {
      DrawableHolder& holder = m_drawables[n];
      if (!holder.m_pGsModel.isNull())
      {
        ModelRef &mRef = m_refModels[holder.m_pGsModel.get()];
        if (mRef.nRefs <= 1)
        {
          holder.m_pGsModel->removeModelReactor(this);
          m_refModels.erase(holder.m_pGsModel.get());
        }
        else
          mRef.nRefs--;
      }
    }
  }
  OdGsBaseVectorizeView::eraseAll();
}

bool OdDrawOrderBaseView::onHighlight(OdGsModel *pModel, const OdGiPathNode& /*path*/, bool /*bDoIt*/, const OdGsView* /*pView*/)
{
  m_refModels[pModel].bHighlighted = true;
  return true;
}

bool OdDrawOrderBaseView::onErased(OdGsModel *pModel, OdGiDrawable *pErased, OdGiDrawable *pParent)
{
  if (pParent)
    onErased(pModel, pErased, pParent->id());
  return true;
}

bool OdDrawOrderBaseView::onErased(OdGsModel *pModel, OdGiDrawable *pErased, OdDbStub *parentID)
{
  if (parentID)
  {
    for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
    {
      DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
      OdUInt32 nDrawables = pOrder->m_drawableInfo.size();
      DrawableInfo *drawInfo = pOrder->m_drawableInfo.asArrayPtr();
      for (OdUInt32 nDrawable = 0; nDrawable < nDrawables; nDrawable++)
      {
        if (drawInfo[nDrawable].m_drawableId == parentID)
        {
          if (drawInfo[nDrawable].m_flags < kPartialRegeneration)
            drawInfo[nDrawable].m_flags = kPartialRegeneration;
          break;
        }
      }
    }
  }
  return true;
}

void OdDrawOrderBaseView::extractLights()
{
  VectorizerAutoPtr vect(*this, true);
  static_cast<OdDrawOrderBaseVectorizer*>(vect.get())->displayLights();
}

void OdDrawOrderBaseView::displayLights(OdGsBaseVectorizer &vec)
{
  const OdUInt32 nDrawables = m_drawables.size();
  DrawableHolder *pDrawHolder = m_drawables.asArrayPtr();
  for (OdUInt32 nDrawable = 0; nDrawable < nDrawables; nDrawable++)
  {
    if (pDrawHolder[nDrawable].m_pGsRoot && (pDrawHolder[nDrawable].m_pGsRoot->nodeType() == kContainerNode))
    {
      OdGsContainerNode *pNode = static_cast<OdGsContainerNode*>(pDrawHolder[nDrawable].m_pGsRoot);
      pNode->turnOnLights(vec);
    }
  }
}

bool OdDrawOrderBaseView::isContentCompatibleOverlay( const OdDrawOrderBaseView* pView, const DrawOrderOverlayData* pOverlayData, OdGsOverlayId overId ) const
{
  if( !pView || !pOverlayData ) return false;
  OdUInt32 sz = pOverlayData->m_drawableInfo.size();
  const DrawableInfo *drawInfo = pOverlayData->m_drawableInfo.asArrayPtr();
  for (GsViewOverlayDataContainer::Iterator oIt = pView->m_overlayData.newIterator(); !oIt.done(); oIt.next())
  {
    if( overId != oIt.overlayId() ) continue;
    DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
    if( pOrder->m_drawableInfo.size() != sz ) continue;
    const DrawableInfo* viewDrawInfo = pOrder->m_drawableInfo.asArrayPtr();
    bool bFind = true;
    for (OdUInt32 nDrawInfo = 0; nDrawInfo < sz; nDrawInfo++)
    {
      if( drawInfo[ nDrawInfo ].m_drawableId != viewDrawInfo[ nDrawInfo ].m_drawableId )
      {
        bFind = false;
        break;
      }
    }
    if( bFind )
    {
      return true;
    }
  }
  return false;
}

bool OdDrawOrderBaseView::isMetafilesCompatibleOverlays( OdGsOverlayId overId1, const OdDrawOrderBaseView* pView2 )
{
  if( !pView2 ) return false;
  for( OdUInt32 i = 0; i < m_drawables.size(); ++i )
  {
    if( device()->gsModelOverlay(m_drawables[i].m_pGsModel.get()) != overId1 ) continue;
    if( !m_drawables[ i ].m_pGsRoot ) continue;
    if( !m_drawables[ i ].m_pGsRoot->isMetafilesCompatible( this, pView2, NULL ) )
    {
      return false;
    }
  }
  return true;
}

void OdDrawOrderBaseView::breakOverlayCompatibility()
{
  for( int i = 0; i < device()->numViews(); ++i )
  {
    OdDrawOrderBaseView* pView = static_cast<OdDrawOrderBaseView*>(device()->viewAt( i ));
    bool unregMe = ( pView == this );
    for (GsViewOverlayDataContainer::Iterator oIt = pView->m_overlayData.newIterator(); !oIt.done(); oIt.next())
    {
      DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
      if( !unregMe )
      {
        if( pOrder->m_pConnectedView == this )
        {
          pView->onMetafileOrderInheritance( oIt.overlayId(), NULL );
          pOrder->m_pConnectedView = NULL;
          pView->onMetafileOrderChanged(oIt.overlayId(), 0, 0, pOrder->m_metafileOrder.asArrayPtr(), pOrder->m_metafileOrder.size() );
        }
      }
      else
      {
        if( pOrder->m_pConnectedView )
        {
          onMetafileOrderInheritance( oIt.overlayId(), NULL );
          pOrder->m_pConnectedView = NULL;
        }
      }
    }
  }
}

void OdDrawOrderBaseView::breakOverlayCompatibility( OdGsOverlayId nOverlay )
{
  for (GsViewOverlayDataContainer::Iterator oIt = m_overlayData.newIterator(); !oIt.done(); oIt.next())
  {
    if( oIt.overlayId() != nOverlay ) continue;
    DrawOrderOverlayData* pOrder = static_cast<DrawOrderOverlayData*>(oIt.overlayData());
    if( pOrder->m_pConnectedView )
    {
      onMetafileOrderInheritance( nOverlay, NULL );
      pOrder->m_pConnectedView = NULL;
    }
  }
}

// Vectorizer

OdDrawOrderBaseVectorizer::OdDrawOrderBaseVectorizer()
  : OdGsBaseMaterialVectorizer()
  , m_bDisableNesting(false)
  , m_bDisableOffLayersVisibility(false)
{
}

OdDrawOrderBaseVectorizer::~OdDrawOrderBaseVectorizer()
{
}

OdDrawOrderBaseDevice* OdDrawOrderBaseVectorizer::device()
{
  return static_cast<OdDrawOrderBaseDevice*>(view().device());
}

OdDrawOrderBaseView& OdDrawOrderBaseVectorizer::view() const
{
  return static_cast<OdDrawOrderBaseView&>(OdGiBaseVectorizerImpl::view());
}

void OdDrawOrderBaseVectorizer::display(bool /*update*/)
{
  //OdGsBaseVectorizer::display(update); // Do not run display since rendition will do all this job for us
}

void OdDrawOrderBaseVectorizer::switchOverlay(OdGsOverlayId overlayId)
{
  OdGsBaseMaterialVectorizer::switchOverlay(overlayId);
  m_pCurDrawOrderAccum = view().drawOrderAccum(overlayId);
}

void OdDrawOrderBaseVectorizer::drawOrderQuery(bool /*bContainer*/, bool bDisableNested)
{
  view().clearDrawOrderAccums();
  switchOverlay(activeOverlay());
  OdSaveState<bool> _ss(m_bDisableNesting, bDisableNested);
  OdGsBaseVectorizer::display(false);
  m_pCurDrawOrderAccum = NULL;
}

void OdDrawOrderBaseVectorizer::displayLights()
{
  view().displayLights(*this);
}

// Metafile

OdDrawOrderBaseMetafile::OdDrawOrderBaseMetafile()
  : m_pLayer(NULL)
{
  m_visualId = (OdUInt64)(OdIntPtr)this;
}

OdDrawOrderBaseMetafile::~OdDrawOrderBaseMetafile()
{
  if (!m_pContext.isNull() && m_pContext->m_pDevice && isFilled())
    m_pContext->m_pDevice->onDeleteRequest(onMetafileDeleteRequestProc, this);
}

void OdDrawOrderBaseMetafile::onMetafileDeleteRequestProc(OdDrawOrderBaseDevice *pDevice, void *pData)
{
  pDevice->onMetafileDeleted(reinterpret_cast<OdDrawOrderBaseMetafile*>(pData));
}

OdRxObjectPtr OdDrawOrderBaseMetafile::createObject(OdDrawOrderBaseDevice::MetafileContext *pContext)
{
  OdRxObjectPtr pMf(OdRxObjectImpl<OdDrawOrderBaseMetafile>::createObject());
  static_cast<OdDrawOrderBaseMetafile*>(pMf.get())->m_pContext = pContext;
  return pMf;
}

// Vectorizer

OdRxObjectPtr OdDrawOrderBaseVectorizer::newGsMetafile()
{
  return OdDrawOrderBaseMetafile::createObject(device()->metafileContext());
}

void OdDrawOrderBaseVectorizer::beginMetafile(OdRxObject* pMetafile)
{
  OdGsBaseMaterialVectorizer::beginMetafile(pMetafile);
  OdDrawOrderBaseMetafile *pMf = static_cast<OdDrawOrderBaseMetafile*>(pMetafile);
  OdGsBaseModel *pModel = gsWriter().gsModel();
  if (m_effectiveLayerId && pModel)
    pMf->m_pLayer = gsLayerNode(m_effectiveLayerId, pModel);
  else
    pMf->m_pLayer = NULL;
  pMf->setVisible(true);
  if (!m_bDisableOffLayersVisibility && pMf->m_pLayer)
  {
#ifndef ODGS_LOCAL_IDS
    OdUInt32 nLayerFlags = pMf->m_pLayer->layerTraits(viewportId()).flags();
#else
    OdUInt32 nLayerFlags = pMf->m_pLayer->layerTraits(view().localViewportId(pMf->m_pLayer->baseModel())).flags();
#endif
    pMf->setVisible(!GETBIT(nLayerFlags, OdGiLayerTraits::kOff));
  }
  pMf->setHighlighted(isHighlighted() && !currentHighlightBranch());
  pMf->setFaded(isFaded());

  m_curRecordingMetafile = pMetafile;
}

void OdDrawOrderBaseVectorizer::endMetafile(OdRxObject* pMetafile)
{
  OdDrawOrderBaseMetafile *pMf = static_cast<OdDrawOrderBaseMetafile*>(pMetafile);
  pMf->setFilled();
  onMetafileAdded(pMetafile, pMf->isVisible(), pMf->isHighlighted());
  OdGsBaseMaterialVectorizer::endMetafile(pMetafile);

  m_curRecordingMetafile.release();
}

void OdDrawOrderBaseVectorizer::playMetafile(const OdRxObject* pMetafile)
{
  OdDrawOrderBaseMetafile *pMf = static_cast<OdDrawOrderBaseMetafile*>(const_cast<OdRxObject*>(pMetafile));
  if (!m_bDisableOffLayersVisibility && pMf->m_pLayer)
  {
#ifndef ODGS_LOCAL_IDS
    OdUInt32 nLayerFlags = pMf->m_pLayer->layerTraits(viewportId()).flags();
#else
    OdUInt32 nLayerFlags = pMf->m_pLayer->layerTraits(view().localViewportId(pMf->m_pLayer->baseModel())).flags();
#endif
    if (pMf->isVisible() != !GETBIT(nLayerFlags, OdGiLayerTraits::kOff))
    {
      const bool bFlag = !pMf->isVisible();
      pMf->setVisible(bFlag);
      device()->onMetafileVisibilityChanged(pMf, bFlag);
    }
  }
  if (pMf->isFaded() != GETBIT(m_fadingFlags, kFfRefEdit))
  {
    const bool bFlag = !pMf->isFaded();
    pMf->setFaded(bFlag);
    device()->onMetafileFadingChanged(pMf, bFlag);
  }
  const bool bNested = pMf->isNested();
  if (!bNested)
  { // Don't process highlighting for nested metafiles
    if (pMf->isHighlighted() != isHighlighted())
    {
      const bool bFlag = !pMf->isHighlighted();
      pMf->setHighlighted(bFlag);
      device()->onMetafileHighlightingChanged(pMf, bFlag);
    }
  }
  if ((!bNested || !m_bDisableNesting) && m_pCurDrawOrderAccum)
    m_pCurDrawOrderAccum->push_back(pMf->m_visualId);
}

void OdDrawOrderBaseVectorizer::onMetafileAdded(OdRxObject *pMetafile, bool bVisible, bool bHighlighted)
{
  device()->onMetafileAdded(pMetafile, bVisible, bHighlighted);
}

//
