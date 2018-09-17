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

#ifndef __ODGIFACEEDGEDATATRAITSSAVER_H__
#define __ODGIFACEEDGEDATATRAITSSAVER_H__

#include "Gi/GiGeometry.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiConveyorGeometry.h"

// Uncomment to enable selection marker restore on exit
//#define EX_TRAITS_SELMARK_RESTORE
// Uncomment to enable onTraitsModified call on exit
//#define EX_TRAITS_TRAITSMOD_ONEXIT
// Uncomment to enable traits restoration on exit
#define EX_TRAITS_RESTORE_ONEXIT

#ifdef EX_TRAITS_SELMARK_RESTORE
#include "Gi/GiPathNode.h"
#endif

#ifdef EX_TRAITS_TRAITSMOD_ONEXIT
#define EX_TRAITS_TRAITSMOD_ONEXIT_SET() \
  bModified = true
#else // EX_TRAITS_TRAITSMOD_ONEXIT
#define EX_TRAITS_TRAITSMOD_ONEXIT_SET()
#endif // EX_TRAITS_TRAITSMOD_ONEXIT

#ifdef EX_TRAITS_RESTORE_ONEXIT
#define EX_TRAITS_SET(prevStorage, curStorage, param) \
  prevStorage = curStorage = param
#define EX_TRAITS_DEFINE(type, prevStorage, curStorage) \
  type prevStorage, curStorage
#define EX_TRAITS_DEFINE_PTR(type, prevStorage, curStorage) \
  type prevStorage, *curStorage
#else // EX_TRAITS_RESTORE_ONEXIT
#define EX_TRAITS_SET(prevStorage, curStorage, param) \
  curStorage = param
#define EX_TRAITS_DEFINE(type, prevStorage, curStorage) \
  type curStorage
#define EX_TRAITS_DEFINE_PTR(type, prevStorage, curStorage) \
  type curStorage
#ifdef EX_TRAITS_SELMARK_RESTORE
#undef EX_TRAITS_SELMARK_RESTORE
#endif // EX_TRAITS_SELMARK_RESTORE
#endif

class OdGiFaceEdgeDataTraitsSaverBase
{
  protected:
    enum Components
    {
      // LoWord - FaceTraits
      kFaceColors           = (1 << 0),
      kFaceTrueColors       = (1 << 1),
      kFaceLayers           = (1 << 2),
      kFaceSelectionMarkers = (1 << 3),
      kFaceMaterials        = (1 << 4),
      kFaceMappers          = (1 << 5),
      kFaceTransparencies   = (1 << 6),
      kFaceVisibilities     = (1 << 7),
      kFaceSelMarkAvail     = (1 << 8),
      kFaceSelMarkOnModify  = (1 << 9),
      // HiWord - EdgeTraits
      kEdgeColors           = (1 << 16),
      kEdgeTrueColors       = (1 << 17),
      kEdgeLayers           = (1 << 18),
      kEdgeLinetypes        = (1 << 19),
      kEdgeSelectionMarkers = (1 << 20),
      kEdgeVisibilities     = (1 << 21),
      kEdgeSelMarkAvail     = (1 << 22),
      kEdgeSelMarkOnModify  = (1 << 23),
      // Masks
      kFaceMask             = 0x0000FFFF,
      kEdgeMask             = 0xFFFF0000,
      // Mask used for restore traits state
#ifdef EX_TRAITS_SELMARK_RESTORE
      kFaceRestoreMask      = kFaceMask & ~kFaceVisibilities,
      kEdgeRestoreMask      = kEdgeMask & ~kEdgeVisibilities,
#else
      kFaceRestoreMask      = kFaceMask & ~(kFaceVisibilities | kFaceSelectionMarkers | kFaceSelMarkOnModify),
      kEdgeRestoreMask      = kEdgeMask & ~(kEdgeVisibilities | kEdgeSelectionMarkers | kEdgeSelMarkOnModify),
#endif
      // Mask used for reset traits state
      kFaceSetMask          = kFaceMask & ~kFaceVisibilities,
      kEdgeSetMask          = kEdgeMask & ~kEdgeVisibilities
    };
  protected:
    OdGiSubEntityTraits *m_pTraits;
    OdGiConveyorContext *m_pDrawCtx;
    OdGsView::RenderMode m_renderMode;
    OdUInt32             m_components;
    EX_TRAITS_DEFINE(OdCmEntityColor, m_prevTrueColor, m_curTrueColor);
    EX_TRAITS_DEFINE_PTR(OdDbStub*,   m_prevLayerId,   m_curLayerId);
#ifdef EX_TRAITS_SELMARK_RESTORE
    OdGsMarker           m_prevSelectionMarker;
#endif // EX_TRAITS_SELMARK_RESTORE
  protected:
    OdGiFaceEdgeDataTraitsSaverBase(OdGiSubEntityTraits *pTraits, OdGiConveyorContext *pDrawCtx, OdGsView::RenderMode renderMode)
      : m_pTraits(pTraits), m_pDrawCtx(pDrawCtx), m_renderMode(renderMode), m_components(0)
    {
    }
    // The default constructor will be used for virtual inheritance
    OdGiFaceEdgeDataTraitsSaverBase()
      : m_pTraits(NULL), m_pDrawCtx(NULL), m_renderMode(OdGsView::k2DOptimized), m_components(0)
    {
    }

    bool setColor(const OdUInt16* pColors, int index)
    {
      OdCmEntityColor trueColor; trueColor.setColorIndex(pColors[index]);
      if (m_curTrueColor != trueColor)
      {
        m_curTrueColor = trueColor;
        m_pTraits->setColor(pColors[index]);
        return true;
      }
      return false;
    }
    bool setTrueColor(const OdCmEntityColor* pTrueColors, int index)
    {
      if (m_curTrueColor != pTrueColors[index])
      {
        m_pTraits->setTrueColor(m_curTrueColor = pTrueColors[index]);
        return true;
      }
      return false;
    }
    bool setLayer(OdDbStub **pLayers, int index)
    {
      if (m_curLayerId != pLayers[index])
      {
        m_pTraits->setLayer(m_curLayerId = pLayers[index]);
        return true;
      }
      return false;
    }

    bool needExit() const { return m_renderMode != OdGsView::k2DOptimized; }
};

class OdGiFaceDataTraitsSaver : protected virtual OdGiFaceEdgeDataTraitsSaverBase
{
  protected:
    const OdGiFaceData  *m_pFaceData;
    EX_TRAITS_DEFINE_PTR(OdDbStub*,         m_prevMaterialId,   m_curMaterialId);
    EX_TRAITS_DEFINE_PTR(const OdGiMapper*, m_prevMapper,       m_curMapper);
    EX_TRAITS_DEFINE(    OdCmTransparency,  m_prevTransparency, m_curTransparency);
  public:
    OdGiFaceDataTraitsSaver(const OdGiFaceData *pFaceData, OdGiSubEntityTraits *pTraits, OdGiConveyorContext *pDrawCtx,
                            OdGsView::RenderMode renderMode, bool bForEdge = false)
      : OdGiFaceEdgeDataTraitsSaverBase(pTraits, pDrawCtx, renderMode), m_pFaceData(pFaceData)
    {
      if (pFaceData && pTraits)
      {
        if (pFaceData->colors())
        {
          EX_TRAITS_SET(m_prevTrueColor, m_curTrueColor, pTraits->trueColor());
          SETBIT_1(m_components, kFaceColors);
        }
        if (pFaceData->trueColors())
        {
          EX_TRAITS_SET(m_prevTrueColor, m_curTrueColor, pTraits->trueColor());
          SETBIT_1(m_components, kFaceTrueColors);
        }
        if (pFaceData->layerIds())
        {
          EX_TRAITS_SET(m_prevLayerId, m_curLayerId, pTraits->layer());
          SETBIT_1(m_components, kFaceLayers);
        }
        if (pFaceData->selectionMarkers())
        {
#ifdef EX_TRAITS_SELMARK_RESTORE
          if (pDrawCtx->currentGiPath())
          {
            m_prevSelectionMarker = pDrawCtx->currentGiPath()->selectionMarker();
            SETBIT_1(m_components, kFaceSelMarkAvail);
          }
#endif // EX_TRAITS_SELMARK_RESTORE
          SETBIT_1(m_components, kFaceSelectionMarkers);
          if (GETBIT(pDrawCtx->drawContextFlags(), OdGiConveyorContext::kForceMarkersOnModified))
            SETBIT_1(m_components, kFaceSelMarkOnModify);
        }
        if (!bForEdge)
        {
          if (pFaceData->materials())
          {
            EX_TRAITS_SET(m_prevMaterialId, m_curMaterialId, pTraits->material());
            SETBIT_1(m_components, kFaceMaterials);
          }
          if (pFaceData->mappers())
          {
            EX_TRAITS_SET(m_prevMapper, m_curMapper, pTraits->mapper());
#ifdef EX_TRAITS_RESTORE_ONEXIT
            // Mapper can point onto internal storage which is similar always, we need store data itself
            if (m_prevMapper) m_prevMapper = new OdGiMapper(*m_curMapper);
#endif // EX_TRAITS_RESTORE_ONEXIT
            SETBIT_1(m_components, kFaceMappers);
          }
        }
        if (pFaceData->transparency())
        {
          EX_TRAITS_SET(m_prevTransparency, m_curTransparency, pTraits->transparency());
          SETBIT_1(m_components, kFaceTransparencies);
        }
        if (pFaceData->visibility())
          SETBIT_1(m_components, kFaceVisibilities);
      }
    }

#ifdef EX_TRAITS_RESTORE_ONEXIT
    bool onExit()
    {
      bool bModified = false;
      if (m_components & kFaceRestoreMask)
      {
        if (GETBIT(m_components, kFaceColors | kFaceTrueColors) && (m_prevTrueColor != m_curTrueColor))
        {
          m_pTraits->setTrueColor(m_prevTrueColor);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
        if (GETBIT(m_components, kFaceLayers) && (m_prevLayerId != m_curLayerId))
        {
          m_pTraits->setLayer(m_prevLayerId);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
#ifdef EX_TRAITS_SELMARK_RESTORE
        if (GETBIT(m_components, kFaceSelMarkAvail))
        {
          m_pTraits->setSelectionMarker(m_prevSelectionMarker);
          if (GETBIT(m_components, kFaceSelMarkOnModify))
            EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
#endif // EX_TRAITS_SELMARK_RESTORE
        if (GETBIT(m_components, kFaceMaterials) && (m_prevMaterialId != m_curMaterialId))
        {
          m_pTraits->setMaterial(m_prevMaterialId);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
        if (GETBIT(m_components, kFaceMappers))
        { // If mappers is specified them will be different always
          m_pTraits->setMapper(m_prevMapper);
          if (m_prevMapper) delete m_prevMapper;
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
        if (GETBIT(m_components, kFaceTransparencies) && (m_prevTransparency != m_curTransparency))
        {
          m_pTraits->setTransparency(m_prevTransparency);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
      }
      return bModified;
    }
    bool needExit() const
    {
      if (!OdGiFaceEdgeDataTraitsSaverBase::needExit())
      {
        if (GETBIT(m_components, kFaceMappers) && m_prevMapper)
          delete m_prevMapper;
        return false;
      }
      return true;
    }
    ~OdGiFaceDataTraitsSaver()
    {
      if (needExit() && onExit())
        m_pDrawCtx->onTraitsModified();
    }
#endif // EX_TRAITS_RESTORE_ONEXIT

    bool setFaceTraits(int faceIndex)
    {
      if (!(m_components & kFaceMask))
        return true;

      if (GETBIT(m_components, kFaceSelectionMarkers))
        m_pTraits->setSelectionMarker(m_pFaceData->selectionMarkers()[faceIndex]);

      if (GETBIT(m_components, kFaceVisibilities) && (m_pFaceData->visibility()[faceIndex] == kOdGiInvisible))
        return false;

      if (m_components & kFaceSetMask)
      {
        bool bModified = GETBIT(m_components, kFaceSelMarkOnModify);
        if (GETBIT(m_components, kFaceColors))
          bModified |= setColor(m_pFaceData->colors(), faceIndex);
        if (GETBIT(m_components, kFaceTrueColors))
          bModified |= setTrueColor(m_pFaceData->trueColors(), faceIndex);
        if (GETBIT(m_components, kFaceLayers))
          bModified |= setLayer(m_pFaceData->layerIds(), faceIndex);
        if (GETBIT(m_components, kFaceMaterials))
        {
          OdDbStub **pMaterials = m_pFaceData->materials();
          if (m_curMaterialId != pMaterials[faceIndex])
          {
            m_pTraits->setMaterial(m_curMaterialId = pMaterials[faceIndex]);
            bModified = true;
          }
        }
        if (GETBIT(m_components, kFaceMappers))
        {
          const OdGiMapper* pMappers = m_pFaceData->mappers();
          m_pTraits->setMapper(m_curMapper = (pMappers + faceIndex));
          bModified = true;
        }
        if (GETBIT(m_components, kFaceTransparencies))
        {
          const OdCmTransparency* pTransparencies = m_pFaceData->transparency();
          if (m_curTransparency != pTransparencies[faceIndex])
          {
            m_pTraits->setTransparency(m_curTransparency = pTransparencies[faceIndex]);
            bModified = true;
          }
        }
        if (m_pDrawCtx->effectivelyVisible())
        {
          if (bModified)
            m_pDrawCtx->onTraitsModified();
          return true;
        }
        else
          return false;
      }

      return m_pDrawCtx->effectivelyVisible();
    }

    bool hasData() const { return (m_components & kFaceMask) != 0; }
};

class OdGiEdgeDataTraitsSaver : protected virtual OdGiFaceEdgeDataTraitsSaverBase
{
  protected:
    const OdGiEdgeData  *m_pEdgeData;
    EX_TRAITS_DEFINE_PTR(OdDbStub*, m_prevLinetypeId, m_curLinetypeId);
  public:
    OdGiEdgeDataTraitsSaver(const OdGiEdgeData *pEdgeData, OdGiSubEntityTraits *pTraits, OdGiConveyorContext *pDrawCtx,
                            OdGsView::RenderMode renderMode, bool bFaceChk = false)
      : OdGiFaceEdgeDataTraitsSaverBase(pTraits, pDrawCtx, renderMode), m_pEdgeData(pEdgeData)
    {
      if (pEdgeData && pTraits)
      {
        if (pEdgeData->colors())
        {
          if (!bFaceChk || !GETBIT(m_components, kFaceColors | kFaceTrueColors))
            EX_TRAITS_SET(m_prevTrueColor, m_curTrueColor, pTraits->trueColor());
          SETBIT_1(m_components, kEdgeColors);
        }
        if (pEdgeData->trueColors())
        {
          if (!bFaceChk || !GETBIT(m_components, kFaceColors | kFaceTrueColors))
            EX_TRAITS_SET(m_prevTrueColor, m_curTrueColor, pTraits->trueColor());
          SETBIT_1(m_components, kEdgeTrueColors);
        }
        if (pEdgeData->layerIds())
        {
          if (!bFaceChk || !GETBIT(m_components, kFaceLayers))
            EX_TRAITS_SET(m_prevLayerId, m_curLayerId, pTraits->layer());
          SETBIT_1(m_components, kEdgeLayers);
        }
        if (pEdgeData->linetypeIds())
        {
          EX_TRAITS_SET(m_prevLinetypeId, m_curLinetypeId, pTraits->lineType());
          SETBIT_1(m_components, kEdgeLinetypes);
        }
        if (pEdgeData->selectionMarkers())
        {
#ifdef EX_TRAITS_SELMARK_RESTORE
          if ((!bFaceChk || !GETBIT(m_components, kFaceSelectionMarkers)) && pDrawCtx->currentGiPath())
          {
            m_prevSelectionMarker = pDrawCtx->currentGiPath()->selectionMarker();
            SETBIT_1(m_components, kEdgeSelMarkAvail);
          }
#endif // EX_TRAITS_SELMARK_RESTORE
          SETBIT_1(m_components, kEdgeSelectionMarkers);
          if (!bFaceChk || !GETBIT(m_components, kFaceSelectionMarkers))
          {
            if (GETBIT(pDrawCtx->drawContextFlags(), OdGiConveyorContext::kForceMarkersOnModified))
              SETBIT_1(m_components, kEdgeSelMarkOnModify);
          }
          else if (GETBIT(m_components, kFaceSelMarkOnModify))
            SETBIT_1(m_components, kEdgeSelMarkOnModify);
        }
        if (pEdgeData->visibility())
          SETBIT_1(m_components, kEdgeVisibilities);
      }
    }

#ifdef EX_TRAITS_RESTORE_ONEXIT
    bool onExit(bool bFaceChk = false)
    {
      bool bModified = false;
      if (m_components & kEdgeRestoreMask)
      {
        if (GETBIT(m_components, kEdgeColors | kEdgeTrueColors) &&
            (!bFaceChk || !GETBIT(m_components, kFaceColors | kFaceTrueColors)) && (m_prevTrueColor != m_curTrueColor))
        {
          m_pTraits->setTrueColor(m_prevTrueColor);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
        if (GETBIT(m_components, kEdgeLayers) &&
            (!bFaceChk || !GETBIT(m_components, kFaceLayers)) && (m_prevLayerId != m_curLayerId))
        {
          m_pTraits->setLayer(m_prevLayerId);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
        if (GETBIT(m_components, kEdgeLinetypes) && (m_prevLinetypeId != m_curLinetypeId))
        {
          m_pTraits->setLineType(m_prevLinetypeId);
          EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
#ifdef EX_TRAITS_SELMARK_RESTORE
        if (GETBIT(m_components, kEdgeSelMarkAvail) &&
            (!bFaceChk || !GETBIT(m_components, kFaceSelectionMarkers)))
        {
          m_pTraits->setSelectionMarker(m_prevSelectionMarker);
          if (GETBIT(m_components, kEdgeSelMarkOnModify))
            EX_TRAITS_TRAITSMOD_ONEXIT_SET();
        }
#endif // EX_TRAITS_SELMARK_RESTORE
      }
      return bModified;
    }
    ~OdGiEdgeDataTraitsSaver()
    {
      if (needExit() && onExit())
        m_pDrawCtx->onTraitsModified();
    }
#endif // EX_TRAITS_RESTORE_ONEXIT

    bool setEdgeTraits(int edgeIndex)
    {
      if (!(m_components & kEdgeMask))
        return true;

      if (GETBIT(m_components, kEdgeSelectionMarkers))
        m_pTraits->setSelectionMarker(m_pEdgeData->selectionMarkers()[edgeIndex]);

      if (GETBIT(m_components, kEdgeVisibilities) && (m_pEdgeData->visibility()[edgeIndex] == kOdGiInvisible))
        return false;

      if (m_components & kEdgeSetMask)
      {
        bool bModified = GETBIT(m_components, kEdgeSelMarkOnModify);
        if (GETBIT(m_components, kEdgeColors))
          bModified |= setColor(m_pEdgeData->colors(), edgeIndex);
        if (GETBIT(m_components, kEdgeTrueColors))
          bModified |= setTrueColor(m_pEdgeData->trueColors(), edgeIndex);
        if (GETBIT(m_components, kEdgeLayers))
          bModified |= setLayer(m_pEdgeData->layerIds(), edgeIndex);
        if (GETBIT(m_components, kEdgeLinetypes))
        {
          OdDbStub **pLinetypes = m_pEdgeData->linetypeIds();
          if (m_curLinetypeId != pLinetypes[edgeIndex])
          {
            m_pTraits->setLineType(m_curLinetypeId = pLinetypes[edgeIndex]);
            bModified = true;
          }
        }
        if (m_pDrawCtx->effectivelyVisible())
        {
          if (bModified)
            m_pDrawCtx->onTraitsModified();
          return true;
        }
        else
          return false;
      }

      return m_pDrawCtx->effectivelyVisible();
    }

    bool hasData() const { return (m_components & kEdgeMask) != 0; }
};

class OdGiFaceEdgeDataTraitsSaver : public OdGiFaceDataTraitsSaver, public OdGiEdgeDataTraitsSaver
{
  public:
    OdGiFaceEdgeDataTraitsSaver(const OdGiFaceData *pFaceData, const OdGiEdgeData *pEdgeData,
                                OdGiSubEntityTraits *pTraits, OdGiConveyorContext *pDrawCtx,
                                OdGsView::RenderMode renderMode)
      : OdGiFaceEdgeDataTraitsSaverBase(pTraits, pDrawCtx, renderMode)
      , OdGiFaceDataTraitsSaver(pFaceData, pTraits, pDrawCtx, renderMode, true)
      , OdGiEdgeDataTraitsSaver(pEdgeData, pTraits, pDrawCtx, renderMode, true)
    {
    }
#ifdef EX_TRAITS_RESTORE_ONEXIT
    ~OdGiFaceEdgeDataTraitsSaver()
    {
      if (OdGiFaceDataTraitsSaver::needExit())
      {
        bool bModified = OdGiFaceDataTraitsSaver::onExit();
        bModified |= OdGiEdgeDataTraitsSaver::onExit(true);
        if (bModified)
          m_pDrawCtx->onTraitsModified();
      }
      m_components = 0; // Avoid redundant operations in base class destructors
    }
#endif

    bool hasData() const { return m_components != 0; }
};

#endif // __ODGIFACEEDGEDATATRAITSSAVER_H__
