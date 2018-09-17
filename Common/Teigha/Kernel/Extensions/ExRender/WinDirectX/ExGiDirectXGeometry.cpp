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

// to satisfy VC2005, else it refuses to compile WinDirectX project
#if _MSC_VER >= 1400
  #define POINTER_64 __ptr64
#endif

#include "OdaCommon.h"
#include "ExGiDirectXGeometry.h"
#include "CmColorBase.h"
#include "RxObjectImpl.h"
#include "UInt32Array.h"
#include "UInt8Array.h"
#include "SaveState.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiUtils.h"
#include "Gi/GiShellFaceIterator.h"
#include "Ge/GeScale3d.h"
#include "Gs/GsBaseMaterialView.h"
#include "../ExGsHelpers.h"

ODRX_CONS_DEFINE_MEMBERS(ExGiDirectXGeometry, OdRxObject, RXIMPL_CONSTR);

ODCA_HEAP_ALLOCATOR_DEFINE_1_ENH(D3dRecord);
ODCA_HEAP_OPERATORS_DEFINE_0(D3dRecord);

//#define OD_EXDX_TRACERECS

#define OD_D3D_REVERSENORMAL(vert) \
  (vert).normal.x = -(vert).normal.x; \
  (vert).normal.y = -(vert).normal.y; \
  (vert).normal.z = -(vert).normal.z;

#ifdef DEBUG
#define STL_USING_LIMITS
#include "OdaSTL.h"
inline FLOAT dbl2float(double value)
{
  // Test if value denormalized
  if (value < double(std::numeric_limits<float>::min()) && value > -double(std::numeric_limits<float>::min()))
    return 0.0f;
  // Test if value to big
  if (value > double(std::numeric_limits<float>::max()))
    return std::numeric_limits<float>::max();
  // Test if value to small
  if (value < double(-std::numeric_limits<float>::max()))
    return -std::numeric_limits<float>::max();
  return (FLOAT)value;
}
#else
#ifndef dbl2float
#define dbl2float(x) (FLOAT)(x)
#endif
#endif

static void setupDefaultMaterialToDevice(OdD3DStateManager *pSM)
{
  D3DMATERIAL mtrl;
  ZeroMemory( &mtrl, sizeof(D3DMATERIAL) );
  mtrl.Specular.r = 1.0f;
  mtrl.Specular.g = 1.0f;
  mtrl.Specular.b = 1.0f;
  mtrl.Specular.a = 1.0f;
  mtrl.Ambient.r = 0.2f;
  mtrl.Ambient.g = 0.2f;
  mtrl.Ambient.b = 0.2f;
  mtrl.Ambient.a = 1.0f;
  mtrl.Power = 50.0f;
  pSM->SetMaterial( &mtrl );
  pSM->SetAmbientMaterialSourceRenderState(D3DMCS_COLOR1);
  pSM->SetDiffuseMaterialSourceRenderState(D3DMCS_COLOR1);
  pSM->SetSpecularMaterialSourceRenderState(D3DMCS_MATERIAL);
  pSM->SetEmissiveMaterialSourceRenderState(D3DMCS_MATERIAL);
}

template <typename TBase>
const void *applyColorOverride(const void *pInData, OdUInt32 nEntries, const D3dMetafilePlayContext &ctx)
{
  return ctx.makeColorOverride(pInData, OdUInt32(((const OdUInt8*)&((const TBase*)pInData)->color) - ((const OdUInt8*)pInData)),
    sizeof(TBase), nEntries);
}

inline OdUInt32 dataTypeResize(OdUInt32 nData, DWORD dataType)
{
  switch (dataType)
  {
    case D3DPT_LINELIST: return nData * 2;
    case D3DPT_LINESTRIP: return nData + 1;
    case D3DPT_TRIANGLEFAN: return nData + 2;
    case D3DPT_TRIANGLELIST: return nData * 3;
    case D3DPT_TRIANGLESTRIP: return nData + 2;
  }
  return nData;
}

template <typename TBase>
const void *overrideableColorEntry(const void *pInData, OdUInt32 nEntries, const D3dMetafilePlayContext &ctx, DWORD dataType)
{
  if (ctx.isNeedOverride(((const TBase*)pInData)->color))
    return applyColorOverride<TBase>(pInData, dataTypeResize(nEntries, dataType), ctx);
  return pInData;
}

#define CORDATA(type, dataType, pPtr, nEntries, ctx) \
  (const type*)overrideableColorEntry<type>(pPtr, (OdUInt32)(nEntries), ctx, dataType)

#define DRAWPRIMITIVE_CTX(dataType, nEntries, pPtr, type, ctx) \
  (ctx).device()->DrawPrimitiveUP(dataType, (UINT)(nEntries), CORDATA(type, dataType, pPtr, nEntries, ctx), sizeof(type))

#define DRAWPRIMITIVE(dataType, nEntries, pPtr, type) \
  DRAWPRIMITIVE_CTX(dataType, nEntries, pPtr, type, ctx)

OdGsView::RenderMode D3dMetafilePlayContext::renderMode() const
{
  return m_pVectorizer->view().mode();
}

OdGeMatrix3d D3dMetafilePlayContext::eyeToWorld() const
{
  if (m_pMVRedirect)
  { 
    OdGeMatrix3d mtx;
    if (m_pMVRedirect->metafileTransformOverrided())
      mtx = m_pMVRedirect->outputToViewportMatrix();
    else
      mtx = m_pVectorizer->getEyeToWorldTransform();
    m_pMVRedirect->xformByMetafileTransform(mtx);
    return mtx;
  }
  return m_pVectorizer->getEyeToWorldTransform();
}

void D3dMetafilePlayContext::clearPattTextureCache() const
{
  m_linePattTexture.Release();
  m_fillPattTexture.Release();
}

IDirect3DTexture* D3dMetafilePlayContext::linePattTexture(DWORD textureStage, bool bSetChan) const
{
  ODA_ASSERT(textureStage == 0); // Line pattern does not support multitexturing currently
  CComPtr<IDirect3DTexture>& pRes = m_linePattTexture;
  if(!pRes)
  {
    if( SUCCEEDED(m_pDev->CreateTexture(2, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRes)) )
    {
      D3DLOCKED_RECT lockedRect;
      if( SUCCEEDED(pRes->LockRect(0, &lockedRect, NULL, 0)) )
      {
        OdUInt32* pixels = (OdUInt32*)lockedRect.pBits;
        pixels[0] = 0xFFFFFFFF;
        pixels[1] = 0x00000000;
        pRes->UnlockRect(0);
      }
    }
  }

  if (!bSetChan)
    return pRes;
  
  m_pDevSM->SetColorOpTextureStageState(  textureStage, D3DTOP_MODULATE);
  m_pDevSM->SetColorArg1TextureStageState(textureStage, D3DTA_TEXTURE);
  m_pDevSM->SetColorArg2TextureStageState(textureStage, D3DTA_DIFFUSE);
  m_pDevSM->SetMagFilterTextureStageState(textureStage, D3DTEXF_POINT);
  m_pDevSM->SetMinFilterTextureStageState(textureStage, D3DTEXF_POINT);
  m_pDevSM->SetAlphaOpTextureStageState(  textureStage, D3DTOP_SELECTARG1);
  m_pDevSM->SetAlphaArg1TextureStageState(textureStage, D3DTA_TEXTURE);
  m_pDevSM->SetAlphaTestEnableRenderState(TRUE);
  m_pDevSM->SetAlphaFuncRenderState(D3DCMP_GREATER);
  m_pDevSM->SetAlphaRefRenderState(0x7F);

  m_pDevSM->SetAddressUTextureStageState( textureStage, D3DTADDRESS_WRAP);
  m_pDevSM->SetAddressVTextureStageState( textureStage, D3DTADDRESS_WRAP);
  
  m_pDevSM->SetTextureTransformFlagsTextureStageState(textureStage, D3DTTFF_DISABLE);
  m_pDevSM->SetTexCoordIndexTextureStageState(textureStage, D3DTSS_TCI_PASSTHRU);

  m_pDevSM->SetTextureMatrixIdentity(textureStage);

  return pRes;
}

IDirect3DTexture* D3dMetafilePlayContext::lineBkgndTexture(DWORD textureStage, bool bSetChan) const
{
  ODA_ASSERT(textureStage == 0); // Line pattern does not support multitexturing currently
  ODCOLORREF col = view().baseDevice()->getPaletteBackground();
  if (m_pColorConverter && m_bProcessBackgroundColors)
    col = m_pColorConverter->convert(col);
  CComPtr<IDirect3DTexture>& pRes = m_bkgndPattTexture[col];
  if(!pRes)
  {
    if( SUCCEEDED(m_pDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRes)) )
    {
      D3DLOCKED_RECT lockedRect;
      if( SUCCEEDED(pRes->LockRect(0, &lockedRect, NULL, 0)) )
      {
        OdUInt32* pixels = (OdUInt32*)lockedRect.pBits;
        pixels[0] = ExGiDirectXGeometry::getD3DColor(col);
        pRes->UnlockRect(0);
      }
    }
  }

  if (!bSetChan)
    return pRes;

  m_pDevSM->SetColorOpTextureStageState(  textureStage, D3DTOP_SELECTARG1);
  m_pDevSM->SetColorArg1TextureStageState(textureStage, D3DTA_TEXTURE);
  m_pDevSM->SetColorArg2TextureStageState(textureStage, D3DTA_DIFFUSE);
  m_pDevSM->SetMagFilterTextureStageState(textureStage, D3DTEXF_POINT);
  m_pDevSM->SetMinFilterTextureStageState(textureStage, D3DTEXF_POINT);
  m_pDevSM->SetAlphaOpTextureStageState(  textureStage, D3DTOP_SELECTARG1);
  m_pDevSM->SetAlphaArg1TextureStageState(textureStage, D3DTA_TEXTURE);

  m_pDevSM->SetAddressUTextureStageState( textureStage, D3DTADDRESS_WRAP);
  m_pDevSM->SetAddressVTextureStageState( textureStage, D3DTADDRESS_WRAP);

  // Source isn't principle here
  m_pDevSM->SetTextureTransformFlagsTextureStageState(textureStage, D3DTTFF_DISABLE);
  m_pDevSM->SetTexCoordIndexTextureStageState(textureStage, D3DTSS_TCI_PASSTHRU);

  m_pDevSM->SetTextureMatrixIdentity(textureStage);

  return pRes;
}

IDirect3DTexture* D3dMetafilePlayContext::fillPattTexture(DWORD textureStage, bool bSetChan) const
{
  ODA_ASSERT(textureStage <= 1); // Only 0 and 1 texture channels supported
  CComPtr<IDirect3DTexture>& pRes = m_fillPattTexture;
  const DWORD color = 0xFFFFFFFF;
  if(!pRes)
  {
    if( SUCCEEDED(m_pDev->CreateTexture(4, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRes)) )
    {
      D3DLOCKED_RECT lockedRect;
      if( SUCCEEDED(pRes->LockRect(0, &lockedRect, NULL, 0)) )
      {
        const DWORD bgcol = 0x00000000;
        OdUInt32* pixels = (OdUInt32*)lockedRect.pBits;

        pixels[ 0] = color;
        pixels[ 1] = bgcol;
        pixels[ 2] = color;
        pixels[ 3] = bgcol;

        pixels[ 4] = bgcol;
        pixels[ 5] = bgcol;
        pixels[ 6] = bgcol;
        pixels[ 7] = color;

        pixels[ 8] = color;
        pixels[ 9] = bgcol;
        pixels[10] = color;
        pixels[11] = bgcol;

        pixels[12] = bgcol;
        pixels[13] = color;
        pixels[14] = bgcol;
        pixels[15] = bgcol;

        pRes->UnlockRect(0);
      }
    }
  }

  if (!bSetChan)
    return pRes;

  m_pDevSM->SetColorOpTextureStageState(  textureStage, D3DTOP_MODULATE);
  m_pDevSM->SetColorArg1TextureStageState(textureStage, D3DTA_TEXTURE);
  if (textureStage == 0)
    m_pDevSM->SetColorArg2TextureStageState(textureStage, D3DTA_DIFFUSE);
  else
    m_pDevSM->SetColorArg2TextureStageState(textureStage, D3DTA_CURRENT);
  m_pDevSM->SetMagFilterTextureStageState(textureStage, D3DTEXF_POINT);
  m_pDevSM->SetMinFilterTextureStageState(textureStage, D3DTEXF_POINT);
  m_pDevSM->SetAlphaOpTextureStageState(  textureStage, D3DTOP_SELECTARG1);
  m_pDevSM->SetAlphaArg1TextureStageState(textureStage, D3DTA_TEXTURE);
  m_pDevSM->SetAlphaTestEnableRenderState(TRUE);
  m_pDevSM->SetAlphaFuncRenderState(D3DCMP_GREATER);
  m_pDevSM->SetAlphaRefRenderState(0x7F);

  m_pDevSM->SetAddressUTextureStageState( textureStage, D3DTADDRESS_WRAP);
  m_pDevSM->SetAddressVTextureStageState( textureStage, D3DTADDRESS_WRAP);

  m_pDevSM->SetTextureTransformFlagsTextureStageState(textureStage, D3DTTFF_COUNT4 | D3DTTFF_PROJECTED);
  m_pDevSM->SetTexCoordIndexTextureStageState(textureStage, D3DTSS_TCI_CAMERASPACEPOSITION);

  return pRes;
}

void D3dMetafilePlayContext::closePattTextureUsage(DWORD textureStage, bool bSetChan) const
{
  m_pDevSM->SetTexture(textureStage, NULL);
  if (!bSetChan)
    return;
  m_pDevSM->SetAlphaOpTextureStageState(textureStage, D3DTOP_DISABLE);
  m_pDevSM->SetAlphaTestEnableRenderState(FALSE);
}

void D3dMetafilePlayContext::renderLineCap(int iPx, const OdD3DVertexCol *origin, const OdGeVector3d &fwd, const OdGeVector3d &side) const
{
  if (m_lineCaps.isEmpty())
  {
    OdD3DVertexColArray tmpArray;
    m_lineCaps.resize(OD_D3D_MAXLINECAPVERTS, tmpArray);
    m_capRender.resize(OD_D3D_MAXLINECAPVERTS << 1);
  }
  if (iPx < 3) iPx = 3;
  else if (iPx >= OD_D3D_MAXLINECAPVERTS) iPx = OD_D3D_MAXLINECAPVERTS - 1;
  if (m_lineCaps[iPx].isEmpty())
  {
    OdD3DVertexColArray &ca = m_lineCaps[iPx];
    ca.resize(iPx << 1);
    double delta = OdaPI / (iPx - 1);
    int i;
    for (i = 0; i < iPx; i++)
    {
      ca[i].x = FLOAT(sin(delta * i));
      ca[i].y = FLOAT(cos(delta * i));
      ca[i].z = 0.0f;
    }
    for (i = 0; i < iPx; i++)
    {
      ca[i + iPx].x = -ca[i].x;
      ca[i + iPx].y = -ca[i].y;
      ca[i + iPx].z = 0.0f;
    }
  }
  OdD3DVertexCol *vc = m_capRender.asArrayPtr();
  const OdD3DVertexCol *inc = m_lineCaps[iPx].asArrayPtr();
  int i;
  ODCOLORREF color = origin[0].color;
  OdGePoint3d tmp;
  for (i = 0; i < iPx; i++)
  {
    vc[i].x = dbl2float(origin[0].x - fwd.x * inc[i].x + side.x * inc[i].y);
    vc[i].y = dbl2float(origin[0].y - fwd.y * inc[i].x + side.y * inc[i].y);
    vc[i].z = dbl2float(origin[0].z - fwd.z * inc[i].x + side.z * inc[i].y);
    vc[i].color = color;
  }
  for ( ; i < (iPx << 1); i++)
  {
    vc[i].x = dbl2float(origin[1].x - fwd.x * inc[i].x + side.x * inc[i].y);
    vc[i].y = dbl2float(origin[1].y - fwd.y * inc[i].x + side.y * inc[i].y);
    vc[i].z = dbl2float(origin[1].z - fwd.z * inc[i].x + side.z * inc[i].y);
    vc[i].color = color;
  }
  device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, (UINT)(iPx << 1) - 2, vc, sizeof(OdD3DVertexCol));
}

void D3dMetafilePlayContext::clearLineCaps() const
{
  m_lineCaps.setPhysicalLength(0);
  m_capRender.setPhysicalLength(0);
}

IDirect3DTexture *D3dMetafilePlayContext::getTransparencyLevelTexture(OdUInt8 alpha) const
{
  if (m_transpTexture.size() <= alpha)
  {
    m_transpTexture.resize(alpha + 1);
  }
  if (m_transpTexture.at(alpha) == NULL)
  {
    OdGiMaterialTextureEntryPtr pTextureEntry = OdGiMaterialTextureEntry::createObject();
    pTextureEntry->setGiMaterialTexture(stateManager().GetLostStateManager(), ExGiD3dMaterialTextureData::desc(), *(view().userGiContext()), (double)alpha / 255);
    m_transpTexture[alpha] = static_cast<ExGiD3dMaterialTextureData*>(pTextureEntry->textureData().get())->deviceTexture();
  }
  return m_transpTexture.at(alpha);
}

void D3dMetafilePlayContext::clearTransparencyLevels() const
{
  m_transpTexture.clear();
}

void D3dMetafilePlayContext::renderBkgndLine(const OdD3DVertexCol *points, OdUInt32 nPoints, bool bLines) const
{
  //if (renderMode() < OdGsView::kHiddenLine)
  //  return;
  // @@@TODO: Optimize here. Color can be aquired from another place
  OdD3DVertexCol bPts[2];
  ODCOLORREF col = view().baseDevice()->getPaletteBackground();
  if (m_pColorConverter && m_bProcessBackgroundColors)
    col = m_pColorConverter->convert(col);
  DWORD bgcol = ExGiDirectXGeometry::getD3DColor(col);
  if (bLines)
  {
    for (OdUInt32 i = 0; i < nPoints; i += 2)
    {
      bPts[0] = points[i];
      bPts[1] = points[i + 1];
      bPts[0].color = bPts[1].color = bgcol;
      device()->DrawPrimitiveUP(D3DPT_LINELIST, 1, bPts, sizeof(OdD3DVertexCol));
    }
  }
  else
  {
    for (OdUInt32 i = 1; i < nPoints; i++)
    {
      bPts[0] = points[i - 1];
      bPts[1] = points[i];
      bPts[0].color = bPts[1].color = bgcol;
      device()->DrawPrimitiveUP(D3DPT_LINELIST, 1, bPts, sizeof(OdD3DVertexCol));
    }
  }
}

void D3dMetafilePlayContext::renderHighlightedLine(const OdD3DVertexCol *points, OdUInt32 nPoints, bool bBackFill, bool bLines) const
{
  // Unfortunately shader can't help here. In any case we need additional vertex data for calculation and need transforms.
  const OdUInt32 maxMutationVerts = 256; // Should be odd
  if (m_lineHltMutation.isEmpty())
  {
    m_lineHltMutation.setPhysicalLength(maxMutationVerts);
    m_lineHltMutation.resize(maxMutationVerts);
  }
  // data
  OdD3DVertexTexCol *pMutation = m_lineHltMutation.asArrayPtr();
  const double multCft = 0.125;
  double wrapAccum = 0.0;
  OdGePoint3d lastPoint;
  // Prepare transform
  OdGeMatrix3d m3d = view().worldToDeviceMatrix();
  if (materialViewRedirect() != NULL && materialViewRedirect()->metafileTransformOverrided())
    m3d.postMultBy(materialViewRedirect()->metafileToWorldMatrix());
  // @@@TODO: Still doesn't enough for perspective. Perspective correction required.
  DWORD prevFvf = m_pDevSM->GetFVF();
  m_pDevSM->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
  updateTransparencyState(Transparency_LSel, 1);
  while (nPoints)
  {
    OdUInt32 nToRun = odmin(nPoints, maxMutationVerts);
    if (bLines)
    {
      for (OdUInt32 nLine = 0; nLine < nToRun; nLine += 2, points += 2)
      {
        if (lastPoint != OdGePoint3d(points[0].x, points[0].y, points[0].z))
          wrapAccum = 0.0;
        // 1st
        pMutation[nLine].position.x = points[0].x;
        pMutation[nLine].position.y = points[0].y;
        pMutation[nLine].position.z = points[0].z;
        pMutation[nLine].color = points[0].color;
        pMutation[nLine].s = (FLOAT)(wrapAccum * multCft);
        pMutation[nLine].t = 0.0f;
        // calc
        wrapAccum += (OdGePoint3d(points[1].x, points[1].y, points[1].z).transformBy(m3d) -
          OdGePoint3d(points[0].x, points[0].y, points[0].z).transformBy(m3d)).length();
        // 2nd
        pMutation[nLine + 1].position.x = points[1].x;
        pMutation[nLine + 1].position.y = points[1].y;
        pMutation[nLine + 1].position.z = points[1].z;
        pMutation[nLine + 1].color = points[1].color;
        pMutation[nLine + 1].s = (FLOAT)(wrapAccum * multCft);
        pMutation[nLine + 1].t = 0.0f;
        //
        lastPoint.set(points[1].x, points[1].y, points[1].z);
      }
    }
    else
    {
      // LineStrip
      for (OdUInt32 nVertex = 0; nVertex < nToRun; nVertex++, points++)
      {
        // pt
        pMutation[nVertex].position.x = points[0].x;
        pMutation[nVertex].position.y = points[0].y;
        pMutation[nVertex].position.z = points[0].z;
        pMutation[nVertex].color = points[0].color;
        pMutation[nVertex].s = (FLOAT)(wrapAccum * multCft);
        pMutation[nVertex].t = 0.0f;
        // inc
        if (nVertex < nToRun - 1)
        {
          wrapAccum += (OdGePoint3d(points[1].x, points[1].y, points[1].z).transformBy(m3d) -
            OdGePoint3d(points[0].x, points[0].y, points[0].z).transformBy(m3d)).length();
        }
      }
    }
    // dec
    nPoints -= nToRun;
    if (nPoints && !bLines)
    {
      nPoints++;
      points--;
    }
    // Run back
    if (bBackFill)
    {
      m_pDev->SetTexture(0, lineBkgndTexture());
      m_pDev->DrawPrimitiveUP((bLines) ? D3DPT_LINELIST : D3DPT_LINESTRIP, (bLines) ? (nToRun >> 1) : (nToRun - 1), pMutation, sizeof(OdD3DVertexTexCol));
    }
    m_pDev->SetTexture(0, linePattTexture());
    m_pDev->DrawPrimitiveUP((bLines) ? D3DPT_LINELIST : D3DPT_LINESTRIP, (bLines) ? (nToRun >> 1) : (nToRun - 1), pMutation, sizeof(OdD3DVertexTexCol));
  }
  updateTransparencyState(Transparency_LSel, 0);
  m_pDevSM->SetFVF(prevFvf);
}

void D3dMetafilePlayContext::renderHighlightedTriangle(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride, DWORD textureStage) const
{
  // Prepare pattern
  if (renderMode() == OdGsView::kHiddenLine)
  {
    m_pDev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    return;
  }

  updateTransparencyState(Transparency_PSel, 1);
  m_pDev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
  updateTransparencyState(Transparency_PSel, 0);
}

void D3dMetafilePlayContext::updateMaterialTexture(bool bSet, bool bSkipHLT) const
{
  m_bSkipHLT = bSkipHLT;
  if (bSet != m_bTextureEnabled)
  {
    m_bTextureEnabled = bSet;
    if (!m_bTextureEnabled)
      m_pDevSM->SetTexture(1, NULL);
    updateTransparencyState(Transparency_Force, 0);
  }
}

void D3dMetafilePlayContext::updateTransparencyState(TransparencySet nSet, OdUInt8 nVal) const
{
  if (nSet >= Transparency_Force || m_transparency[nSet] != nVal)
  {
    DWORD nChan = (m_bTextureEnabled) ? 1 : 0;
    if (nSet < Transparency_Force)
      m_transparency[nSet] = nVal;
    else if (nSet == Transparency_Reset)
      m_transparency[0] = m_transparency[1] = m_transparency[2] = m_transparency[3] = 0;
    if (m_transparency[Transparency_LSel])
      return;
    bool bSet = m_transparency[Transparency_Material] || m_transparency[Transparency_Color] || (m_transparency[Transparency_PSel] && !m_bSkipHLT);
    if (!bSet)
    {
      closePattTextureUsage(nChan, true/*!nChan*/);
    }
    else
    {
      IDirect3DTexture *pTexture = NULL;
      if (m_transparency[Transparency_PSel] && !m_bSkipHLT)
        pTexture = fillPattTexture(nChan);
      else
      {
        pTexture = getTransparencyLevelTexture(255 - odmax(m_transparency[Transparency_Material], m_transparency[Transparency_Color]));
        // To set channel . . .
        fillPattTexture(nChan);
      }
      //
      stateManager().SetTexture(nChan, pTexture);
#if 0
      FLOAT fac_w = FLOAT(view().eyeToScreenMatrix()[0][0] / 4.);
      FLOAT fac_h = FLOAT(view().eyeToScreenMatrix()[1][1] / 4.);
      D3DXMATRIX mat(
        fac_w, 0.0,   0.0,   0.0,
        0.0,   fac_h, 0.0,   0.0,
        0.0,   0.0,   1.0,   0.0,
        0.0,   0.0,   0.0,   1.0
      );
#else
      OdGeMatrix3d xMtx = view().eyeToScreenMatrix();
      if (m_transparency[Transparency_PSel] && !m_bSkipHLT)
        xMtx.postMultBy(OdGeMatrix3d::scaling(OdGeScale3d(0.25, 0.25, 1.0)));
      else
        xMtx.postMultBy(OdGeMatrix3d::scaling(OdGeScale3d(0.125, 0.125, 1.0)));
      // Add half-texel offset to prevent texture mapping artefacts
      xMtx.postMultBy(OdGeMatrix3d::translation(OdGeVector3d(0.5, 0.5, 0.0)));
      D3DXMATRIX mat(
        (FLOAT)xMtx(0, 0), (FLOAT)xMtx(1, 0), (FLOAT)xMtx(2, 0), (FLOAT)xMtx(3, 0),
        (FLOAT)xMtx(0, 1), (FLOAT)xMtx(1, 1), (FLOAT)xMtx(2, 1), (FLOAT)xMtx(3, 1),
        (FLOAT)xMtx(0, 2), (FLOAT)xMtx(1, 2), (FLOAT)xMtx(2, 2), (FLOAT)xMtx(3, 2),
        (FLOAT)xMtx(0, 3), (FLOAT)xMtx(1, 3), (FLOAT)xMtx(2, 3), (FLOAT)xMtx(3, 3)
      );
#endif
      stateManager().SetTextureMatrix(nChan, &mat);
    }
  }
}

void D3dMetafilePlayContext::resetTextureState() const
{
  OdD3DStateManager &sm = stateManager();
  if (sm.GetTexture(0))
  {
    sm.SetTexture(0, NULL);
    // And we need reset current transparency states to revert them if transparencies doesn't changed
    m_transparency[0] = m_transparency[1] = m_transparency[2] = m_transparency[3] = 0;
  }
}

void D3dMetafilePlayContext::setSelectionMarker(OdGsMarker gsMarker) const
{
  const OdGsHlBranch *pBranch = (m_pMVRedirect) ? m_pMVRedirect->getCurrentHLTBranch() : NULL;
  if (pBranch && !pBranch->markers().isEmpty())
  {
    bool bPrevState = m_pVectorizer->isHighlighted();
    bool bNewState = false;
    if (gsMarker != kNullSubentIndex)
      bNewState = pBranch->hasMarker(gsMarker);
    if (bNewState != bPrevState)
    {
      m_pVectorizer->highlight(bNewState);
    }
  }
}

bool D3dMetafilePlayContext::isSubselectionRequired() const
{
  const OdGsHlBranch *pBranch = (m_pMVRedirect) ? m_pMVRedirect->getCurrentHLTBranch() : NULL;
  return pBranch != NULL;
}

DWORD D3dMetafilePlayContext::makeColorOverride(DWORD color, bool bIfNeed) const
{
  if (!bIfNeed || isNeedOverride(color))
  {
    if (m_bColorOverride)
      color = m_nColorOverride;
    if (m_pColorConverter && (m_bProcessBackgroundColors || !m_bNoColorMod))
      color = ExGiDirectXGeometry::getD3DColor(m_pColorConverter->convert(ExGiDirectXGeometry::getGDIColor(color)));
  }
  return color;
}

const void* D3dMetafilePlayContext::makeColorOverride(const void *pInData, OdUInt32 nOffsetToClr, OdUInt32 nEntrySize, OdUInt32 nEntries) const
{
  OdUInt32 nFullSize = nEntries * nEntrySize;
  m_colorMutation.resize(nFullSize);
  OdUInt8 *pCopy = m_colorMutation.asArrayPtr();
  if (pCopy)
  {
    CopyMemory(pCopy, pInData, nFullSize);
    OdUInt8 *pOffset = pCopy + nOffsetToClr;
    DWORD prevColorToOverride = *(DWORD*)pOffset, prevOverridenColor = makeColorOverride(prevColorToOverride, false);
    for (OdUInt32 nElem = 0; nElem < nEntries; nElem++, pOffset += nEntrySize)
    {
      if (*(DWORD*)pOffset != prevColorToOverride)
        prevColorToOverride = *(DWORD*)pOffset, prevOverridenColor = makeColorOverride(prevColorToOverride, false);
      *(DWORD*)pOffset = prevOverridenColor;
    }
  }
  return pCopy;
}

#ifdef OD_USE_DIRECT3D9
#include "../lut_helpers.h"
#include "DxPaletteShader.h"

IDirect3DPixelShader9 *D3dMetafilePlayContext::paletteImageShader() const
{
  if (!m_bPaletteShaderState)
  { m_bPaletteShaderState++;
    D3DCAPS devCaps;
    ZeroMemory(&devCaps, sizeof(D3DCAPS));
    m_pDev->GetDeviceCaps(&devCaps);
    if (devCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
    { m_bPaletteShaderState++;
      if (SUCCEEDED(m_pDev->CreatePixelShader((DWORD*)g_ps20_PaletteShader_main, &m_paletteImageShader)))
        m_bPaletteShaderState++;
    }
  }
  return m_paletteImageShader;
}
#endif


bool D3dMetafilePlayContext::isDisplayHighlighted() const
{
  if( m_subSelectionFlags & OdDirectXSelFlag_DontDisplayHighlighted ) return false;
  return true;
}


bool D3dMetafilePlayContext::isDisplayUnhighlighted() const
{
  if( m_subSelectionFlags & OdDirectXSelFlag_DontDisplayUnhighlighted ) return false;
  return true;
}


bool D3dMetafilePlayContext::isSelectHighlighted() const
{
  if( m_subSelectionFlags & OdDirectXSelFlag_DontSelectHighlighted ) return false;
  return true;
}


bool D3dMetafilePlayContext::isSelectUnhighlighted() const
{
  if( m_subSelectionFlags & OdDirectXSelFlag_DontSelectUnhighlighted ) return false;
  return true;
}


bool D3dMetafilePlayContext::isDisplayWithHighlight() const
{
  if( vectorizer()->isHighlighted() && isDisplayHighlighted() ) return true;
  if( !vectorizer()->isHighlighted() && isDisplayUnhighlighted() ) return true;
  return false;
}


bool D3dMetafilePlayContext::isSelectWithHighlight() const
{
  if( vectorizer()->isHighlighted() && isSelectHighlighted() ) return true;
  if( !vectorizer()->isHighlighted() && isSelectUnhighlighted() ) return true;
  return false;
}


void D3dMetafilePlayContext::setMaterialViewRedirect(D3dMaterialViewRedirect *pMVRedirect)
{
  m_pMVRedirect = pMVRedirect;
  m_bDynamicSubEntHlt = pMVRedirect->dynamicSubEntHlt();
  // Update color converter
  m_pColorConverter = pMVRedirect->getColorConverter();
  if (m_pColorConverter)
    m_bProcessBackgroundColors = m_pColorConverter->convertBackgroundColors();
}

void D3dMetafilePlayContext::updateVectorizerContext()
{
  updateMaterialTexture(false);
  // Update transparency
  updateTransparencyState(Transparency_Reset, 0);
  // Update lineweight style settings
  OdGiContext *pCtx = view().userGiContext();
  m_lwdStyle.m_bPointsLwd = pCtx->lineWeightConfiguration(OdGiContext::kPointLineWeight) != 0;
  m_lwdStyle.m_curLwdLes = m_lwdStyle.m_defLwdLes = (OdPs::LineEndStyle)pCtx->lineWeightConfiguration(OdGiContext::kLineCapStyle);
  m_lwdStyle.m_curLwdLjs = m_lwdStyle.m_defLwdLjs = (OdPs::LineJoinStyle)pCtx->lineWeightConfiguration(OdGiContext::kLineJoinStyle);
  m_lwdStyle.m_bCurLwdDef = m_lwdStyle.m_bDefLwdDef = (m_lwdStyle.m_defLwdLes == OdPs::kLesRound) && (m_lwdStyle.m_defLwdLjs == OdPs::kLjsRound);
  m_lwdStyle.m_bCurLwdUsesDef = true;
  // Update vectorization-dependent properties
  m_bLwdOverride = false;
  m_bColorOverride = false;
  m_bNoColorMod = false;
  m_subSelectionFlags = 0;
  m_bContextModified = false;
}

// ExGiDirectXGeometry

ExGiDirectXGeometry::ExGiDirectXGeometry()
: m_color(0)
, m_linetype(OdPs::kLtpSolid)
, m_bTwoSidesMode(true)
, m_bDoubleBufferEnabled(false)
, m_paletteColors(NULL)
, m_numPaletteColors(0)
#ifndef OD_USE_DIRECT3D9
, m_psLinetypeDefs(::odgiGetPsLinetypesManager(OdGiPsLinetypes::kOpenGLDefs).getOpenGLDefinitions())
#else
, m_psLinetypeDefs(NULL)
#endif
, m_bIsVertsNormals(false)
, m_bIsFaceNormals(false)
, m_bIsVertsColors(false)
, m_bProcessingTTF(false)
, m_bShadingDisabled(false)
, m_bFaceNormalsRequired(false)
, m_pD3DSM(NULL)
, m_pixLineWeight(0)
, m_bgColor(0)
, m_bPlayMetafile(false)
, m_pMViewRedirect(NULL)
, m_MaterialWay(eNotInit)
{
  m_mfPlayCtx.setIntMtlPtr(&m_pIntMtl);
}

ExGiDirectXGeometry::~ExGiDirectXGeometry()
{
}

// for internal use only
void ExGiDirectXGeometry::setVectorizer(OdGsBaseVectorizer* pView)
{
  m_mfPlayCtx.setVectorizer(pView);
}

void ExGiDirectXGeometry::setMaterialViewRedirect(D3dMaterialViewRedirect *pMViewRedirect, OdGsBaseVectorizer* pView)
{
  m_mfPlayCtx.setMaterialViewRedirect(m_pMViewRedirect = pMViewRedirect);
  m_rPackager.enableDynamicSubEntHlt(pMViewRedirect->dynamicSubEntHlt());
  m_pIntMtl.release();
  if (pView) m_mfPlayCtx.setVectorizer(pView, false);
}

void ExGiDirectXGeometry::setupIntermediateArrays(bool b4Cache)
{
  m_rPackager.setupArrays(b4Cache);
}

void ExGiDirectXGeometry::setD3DDevice(IDirect3DDevice* pD3Ddevice)
{
  m_pD3Ddevice = pD3Ddevice;
  m_mfPlayCtx.setDevice(pD3Ddevice);
}

void ExGiDirectXGeometry::setD3DStateManager(OdD3DStateManager* pD3DSM)
{
  m_pD3DSM = pD3DSM;
  m_mfPlayCtx.setStateManager(pD3DSM);
}

CComPtr<IDirect3DDevice> ExGiDirectXGeometry::d3dDevice()
{
  return m_pD3Ddevice;
}

OdD3DStateManager &ExGiDirectXGeometry::d3dStateManager()
{
  return *m_pD3DSM;
}

void ExGiDirectXGeometry::setLogicalPalette(const ODCOLORREF* palette, OdUInt16 nCount)
{
  m_paletteColors = palette;
  m_numPaletteColors = nCount;
  m_bgColor = getD3DColor(m_paletteColors[0]);
}

DWORD ExGiDirectXGeometry::getD3DColor(const OdCmEntityColor& col)
{
  DWORD nc;
  if(col.isByACI() || col.isByDgnIndex())
    nc = getD3DColor(m_paletteColors[col.colorIndex()]);
  else
    nc = getD3DColor(ODTOCOLORREF(col));
  nc = (nc & 0x00FFFFFF) | (m_color & 0xFF000000);
  return nc;
}

DWORD ExGiDirectXGeometry::getD3DColor(ODCOLORREF col)
{
  return 0xFF000000 | SWAPBYTES(col);
}

ODCOLORREF ExGiDirectXGeometry::getGDIColor(DWORD col)
{
  return (0xFF000000 & col) | (0x00FFFFFF & SWAPBYTES(col));
}

// traits
void ExGiDirectXGeometry::draw_color(DWORD color)
{
  m_color = color;
}

void ExGiDirectXGeometry::draw_lineweight(OdDb::LineWeight lwLineWeight, int nPixels)
{
  if (m_lineWeight.isForcedLineweight())
    return; // Lineweight was set by primitive
  m_lineWeight.m_type = D3dLwdSetting::kModel;
  m_lineWeight.m_val.m_lwLineWeight = lwLineWeight;
  m_pixLineWeight = nPixels;
}

void ExGiDirectXGeometry::draw_lineweight(double dLineWeight, int nPixels)
{
  if (m_lineWeight.isForcedLineweight())
    return; // Lineweight was set by primitive
  m_lineWeight.m_type = D3dLwdSetting::kPlot;
  m_lineWeight.m_val.m_dLineWeight = dLineWeight;
  m_pixLineWeight = nPixels;
}

void ExGiDirectXGeometry::draw_lineweightOverride(double dLineWeight, int nPixels)
{
  if (m_lineWeight.isForcedLineweight())
    return; // Lineweight was set by primitive
  m_lineWeight.m_type = D3dLwdSetting::kAbsolute;
  m_lineWeight.m_val.m_dLineWeight = dLineWeight;
  m_pixLineWeight = nPixels;
}

void ExGiDirectXGeometry::draw_fillStyle(OdPs::FillStyle /*fillStyle*/)
{
  // not implemented yet -- todo
}

void ExGiDirectXGeometry::draw_linetype(OdPs::LineType lt, bool /*isAdaptive*/, double /*linePatSz*/)
{
  m_linetype = lt;
}

void ExGiDirectXGeometry::draw_transparency(OdUInt8 alpha)
{
  if (m_pMetafile.get())
  {
    m_rPackager.setTransparency(alpha);
  }
  else
  {
    m_mfPlayCtx.updateTransparencyState(D3dMetafilePlayContext::Transparency_Color, alpha);
  }
}

void ExGiDirectXGeometry::draw_lineStyle(bool bUseDef, OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle)
{
  if (m_pMetafile.get())
  {
    m_rPackager.setLwdStyle(bUseDef, lesStyle, ljsStyle);
  }
  else
  {
    D3dLwdStyleRecord *pRec = (bUseDef) ? new D3dLwdStyleRecord() : new D3dLwdStyleRecord(lesStyle, ljsStyle);
    pRec->play(m_mfPlayCtx);
    delete pRec;
  }
}

void ExGiDirectXGeometry::draw_noColorMod(bool bSet)
{
  if (m_pMetafile.get())
  {
    m_rPackager.setNoColorMod(bSet);
  }
  else
  {
    m_mfPlayCtx.setNoColorModificationMode(bSet);
  }
}

void ExGiDirectXGeometry::setTwoSidesMode(bool bTwoSidesMode)
{
  m_bTwoSidesMode = bTwoSidesMode;
}

void ExGiDirectXGeometry::setDoubleBufferEnabled(bool bDoubleBufferEnabled)
{
  m_bDoubleBufferEnabled = bDoubleBufferEnabled;
}

void ExGiDirectXGeometry::present()
{
  if(!m_bDoubleBufferEnabled)
    d3dDevice()->Present( NULL, NULL, NULL, NULL );
}

void ExGiDirectXGeometry::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  D3dRecord* pRec = NULL;
  if (!m_rPackager.isInitialized())
  {
    // Not optimized, GsModel disabled
    if (nPoints == 1 || nPoints == 2 && pPoints[0].isEqualTo(pPoints[1]))
    {
      // Point
      if (!m_mfPlayCtx.lwdStyle().m_bPointsLwd || m_pixLineWeight <= 1)
        pRec = D3dPointRecord::createObject(*pPoints, m_color); // Use as old AutoCAD draw's
      else
      {
        OdGePoint3d pts[2] = { *pPoints, *pPoints };
        pRec = D3dLwdPolylineRecord::createObject(2, pts, m_color, (double)m_pixLineWeight);
      }
    }
    else if (m_pixLineWeight > 1)
    {
      // Lineweight
      pRec = D3dLwdPolylineRecord::createObject(nPoints, pPoints, m_color, (double)m_pixLineWeight);
    }
    else
    {
      // Thin polyline
      pRec = D3dPolylineRecord::createObject(nPoints, pPoints, m_color);
    }
  }
  else
  {
    // Optimized, GsModel enabled, packager work
    if (nPoints == 1 || nPoints == 2 && pPoints[0].isEqualTo(pPoints[1]))
    {
      // Point
      if (!m_rPackager.isNestingMode() && ((!m_mfPlayCtx.lwdStyle().m_bPointsLwd && !m_lineWeight.isForcedLineweight()) || !m_lineWeight.hasLineweight()))
        m_rPackager.addPoint((renderMode() == OdGsView::k2DOptimized) ? D3dMetafilePackager::kNLEntry : D3dMetafilePackager::kLSEntry, *pPoints, m_color);
      else
      {
        OdGePoint3d pts[2] = { *pPoints, *pPoints };
        m_rPackager.addLwdPolyline((renderMode() == OdGsView::k2DOptimized) ? D3dMetafilePackager::kNLEntry : D3dMetafilePackager::kLSEntry, true, 2, pts, m_color, m_lineWeight);
      }
    }
    else if (m_rPackager.isNestingMode() || m_lineWeight.hasLineweight())
    {
      // Lineweight
      m_rPackager.addLwdPolyline((renderMode() == OdGsView::k2DOptimized) ? D3dMetafilePackager::kNLEntry : D3dMetafilePackager::kLSEntry, false, nPoints, pPoints, m_color, m_lineWeight);
    }
    else
    {
      // Thin polyline
      m_rPackager.addPolyline((renderMode() == OdGsView::k2DOptimized) ? D3dMetafilePackager::kNLEntry : D3dMetafilePackager::kLSEntry, nPoints, pPoints, m_color);
    }
  }
  /* if(m_linetype == OdPs::kLtpSolid)
    pRec = D3dPolylineRecord::createObject(nPoints, pPoints, m_color);
  else
    pRec = D3dLtpPolylineRecord::createObject(nPoints, pPoints, m_color, 
    (WORD)m_psLinetypeDefs[m_linetype].m_pattern, (WORD)m_psLinetypeDefs[m_linetype].m_patternLength);*/
  if (m_pMetafile.get())
  {
    if (pRec != NULL)
    {
      if(renderMode() == OdGsView::k2DOptimized)
        m_rPackager.addRecord(D3dMetafilePackager::kNLEntry, D3dMetafilePackager::kPolylineRec, pRec);
      else
        m_rPackager.addRecord(D3dMetafilePackager::kLSEntry, D3dMetafilePackager::kPolylineRec, pRec);
    }
  }
  else
  {
    pRec->play(m_mfPlayCtx);
    delete pRec;
  }
}

void ExGiDirectXGeometry::shellProc(OdInt32 nbVertex,
                                    const OdGePoint3d* pVertexList,
                                    OdInt32 faceListSize,
                                    const OdInt32* pFaceList,
                                    const OdGiEdgeData* pEdgeData,
                                    const OdGiFaceData* pFaceData,
                                    const OdGiVertexData* pVertexData)
{
  //DN: ugly fix for CR 4300,4301
  OdSaveState<OdGsView::RenderMode> save_m_renderMode(m_renderMode);
  if(m_mfPlayCtx.forceFill())
    m_renderMode = OdGsView::k2DOptimized;
  //AM: fix for #17062, #17063
  OdGiDrawFlagsHelper _dh(*m_pTraits, m_mfPlayCtx.forceFill() ? OdGiSubEntityTraits::kDisableFillModeCheck : 0);

  m_MaterialWay = (!isMaterialAvailable() || !currentMaterial()->haveDiffuseTexture()) ? eNotMaterialWay : eMaterialWay;
  OdGiGeometrySimplifier::shellProc(nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  m_MaterialWay = eNotInit;
}

void ExGiDirectXGeometry::prepareShMFacesGeneration(const OdGiFaceData* pFaceData)
{
  OdGsView::RenderMode rm = renderMode();

  if(vertexData() && vertexData()->trueColors())
    m_bIsVertsColors = true;
  else
    m_bIsVertsColors = false;

  if(vertexData() && vertexData()->normals() && 
    (rm == OdGsView::kGouraudShaded || rm == OdGsView::kGouraudShadedWithWireframe))
    m_bIsVertsNormals = true;
  else
    m_bIsVertsNormals = false;

  if(pFaceData && pFaceData->normals())
    m_bIsFaceNormals = true;
  else
    m_bIsFaceNormals = false;  

  if(
    (rm == OdGsView::kFlatShaded) ||
    (rm == OdGsView::kFlatShadedWithWireframe) ||
    ((rm == OdGsView::kGouraudShaded || rm == OdGsView::kGouraudShadedWithWireframe) && (!m_bIsVertsNormals || vertexData()->orientationFlag() == kOdGiNoOrientation))
    )
    m_bFaceNormalsRequired = true;
  else
    m_bFaceNormalsRequired = false;
}

void ExGiDirectXGeometry::generateShellFaces(OdInt32 faceListSize,
                                             const OdInt32* pFaceList,
                                             const OdGiEdgeData* pEdgeData,
                                             const OdGiFaceData* pFaceData)
{
  prepareShMFacesGeneration(pFaceData);
  if (pFaceData && pFaceData->materials())
  { // #CORE-12062 : handle switch between textured and non-textured materials
    m_MaterialWay = eNotInit;
    OdGiGeometrySimplifier::generateShellFaces(faceListSize, pFaceList, pEdgeData, pFaceData);
    m_MaterialWay = eNotMaterialWay;
  }
  else
    OdGiGeometrySimplifier::generateShellFaces(faceListSize, pFaceList, pEdgeData, pFaceData);

  // To make visible when very zoomed out
  OdGsView::RenderMode rm = m_renderMode;
  if((rm==OdGsView::k2DOptimized /*|| (rm==OdGsView::kFlatShaded || rm==OdGsView::kGouraudShaded) && m_bProcessingTTF*/) &&
      !(vertexData() && vertexData()->trueColors()) )
  {
    OdDb::LineWeight lw = m_pTraits->lineWeight();
    if(lw!=OdDb::kLnWt000)
    {
      m_pTraits->setLineWeight(OdDb::kLnWt000);
      m_pDrawCtx->onTraitsModified();
    }
    // edge properties are taken from faces
    generateShellWires(faceListSize, pFaceList, pEdgeData, pFaceData);
    if(lw!=OdDb::kLnWt000)
    {
      m_pTraits->setLineWeight(lw);
      m_pDrawCtx->onTraitsModified();
    }
  }
}

void ExGiDirectXGeometry::generateMeshFaces(OdInt32 rows, OdInt32 columns,
                                            const OdGiFaceData* pFaceData)
{
  prepareShMFacesGeneration(pFaceData);
  if (pFaceData && pFaceData->materials())
  { // #CORE-12062 : handle switch between textured and non-textured materials
    m_MaterialWay = eNotInit;
    OdGiGeometrySimplifier::generateMeshFaces(rows, columns, pFaceData);
    m_MaterialWay = eNotMaterialWay;
  }
  else
    OdGiGeometrySimplifier::generateMeshFaces(rows, columns, pFaceData);

  // To make visible when very zoomed out
  OdGsView::RenderMode rm = m_renderMode;
  if((rm==OdGsView::k2DOptimized /*|| (rm==OdGsView::kFlatShaded || rm==OdGsView::kGouraudShaded) && m_bProcessingTTF*/) &&
      !(vertexData() && vertexData()->trueColors()) )
  {
    // edge properties are taken from faces
    generateMeshWires(rows, columns, 0, pFaceData);
  }
}

void ExGiDirectXGeometry::meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal)
{
  OdGeVector3d normal;
  if(m_bFaceNormalsRequired)
  {
    // correct face normal is necessary
    OdGePoint3d bndPts[4];
    bndPts[0] = vertexDataList()[faceList[0]];
    bndPts[1] = vertexDataList()[faceList[1]];
    bndPts[2] = vertexDataList()[faceList[3]];
    bndPts[3] = vertexDataList()[faceList[2]];
    normal = -::odgiFaceNormal(4, bndPts);
    if(pNormal)
    {
      if(pNormal->dotProduct(normal) < 0.0)
      {
        normal = -*pNormal;
        pNormal = &normal;
      }
    }
    else
    {
      pNormal = &normal;
    }
  } // else correct face normal isn't necessary
  triangleOut(faceList, pNormal);
  OdInt32 vrts[4];
  vrts[0] = faceList[3];
  vrts[1] = faceList[2];
  vrts[2] = faceList[1];
  triangleOut(vrts, pNormal);
}

void ExGiDirectXGeometry::shellFaceOut(OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal)
{
  OdGeVector3d normal;
  // OdGsView::RenderMode rm = renderMode();
  if(m_bFaceNormalsRequired)
  {
    // correct face normal is necessary
    OdGiShellFaceIterator pVertices(vertexDataList(), pFaceList+1);
    normal = -::odgiFaceNormal(*pFaceList, pVertices);
    if(pNormal)
    {
      if(pNormal->dotProduct(normal) < 0.0)
      {
        normal = -*pNormal;
        pNormal = &normal;
      }
    }
    else
    {
      pNormal = &normal;
    }
  } // else correct face normal isn't necessary

  OdGiGeometrySimplifier::shellFaceOut(faceListSize, pFaceList, pNormal);
}

void ExGiDirectXGeometry::triangleOut(const OdInt32* p3Vertices, 
                                      const OdGeVector3d* pNormal)
{
  OdGsView::RenderMode rm = renderMode();
  DWORD sfState = D3DCMP_EQUAL;
  DWORD passState = D3DSTENCILOP_KEEP;
  if (m_pMetafile.get() == NULL && rm == OdGsView::kHiddenLine)
  {
    sfState = m_mfPlayCtx.stateManager().GetStencilFuncRenderState();
    m_mfPlayCtx.stateManager().SetStencilFuncRenderState(D3DCMP_LESSEQUAL);
    passState = m_mfPlayCtx.stateManager().GetStencilPassRenderState();
    m_mfPlayCtx.stateManager().SetStencilPassRenderState(D3DSTENCILOP_INCRSAT);
  }
  if(rm <= OdGsView::kHiddenLine || m_bShadingDisabled)
  {
    OdD3DVertexCol points[3];
    OdUInt32 i;
    for(i = 0; i < 3; i ++)
    {
      OdInt32 vertInd = p3Vertices[i];
      OdGePoint3d curPt = vertexDataList()[vertInd];
      points[i].x = dbl2float(curPt.x);
      points[i].y = dbl2float(curPt.y);
      points[i].z = dbl2float(curPt.z);
      if(rm == OdGsView::kHiddenLine && !m_bProcessingTTF)
        points[i].color = m_bgColor;
      else if(m_bIsVertsColors)
        points[i].color = getD3DColor(vertexData()->trueColors()[vertInd]);
      else
        points[i].color = m_color;
    }
    if (m_rPackager.isInitialized())
    {
      m_rPackager.addTriangle(points);
    }
    else
    {
      D3dTriangleRecord pRec(points);
      m_pD3DSM->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
      pRec.play(m_mfPlayCtx);
    }
  }
  else
  {
    OdD3DVertexNormalCol points[3];
    OdUInt32 i;

    if(m_bIsVertsNormals)
    {
      OdGiOrientationType orientation = vertexData()->orientationFlag();
      for(i = 0; i < 3; i ++)
      {
        OdInt32 vertInd = p3Vertices[i];
        OdGePoint3d curPt = vertexDataList()[vertInd];
        points[i].position.x = dbl2float(curPt.x);
        points[i].position.y = dbl2float(curPt.y);
        points[i].position.z = dbl2float(curPt.z);
        OdGeVector3d normal, candNormal = vertexData()->normals()[vertInd];
        if(orientation == kOdGiCounterClockwise)
          normal = candNormal;
        else if(orientation == kOdGiClockwise)
          normal = -candNormal;
        else if(candNormal.dotProduct(*pNormal) > 0.0) // in case "no orientation" we have to calculate it
          normal = candNormal;
        else
          normal = -candNormal;

        points[i].normal.x = dbl2float(normal.x);
        points[i].normal.y = dbl2float(normal.y);
        points[i].normal.z = dbl2float(normal.z);

        if(m_bIsVertsColors)
          points[i].color = getD3DColor(vertexData()->trueColors()[vertInd]);
        else
          points[i].color = m_color;
      }
    }
    else
    {
      for(i = 0; i < 3; i ++)
      {
        OdInt32 vertInd = p3Vertices[i];
        OdGePoint3d curPt = vertexDataList()[vertInd];
        points[i].position.x = dbl2float(curPt.x);
        points[i].position.y = dbl2float(curPt.y);
        points[i].position.z = dbl2float(curPt.z);

        points[i].normal.x = OdNonZero(pNormal->x, FLT_MIN) ? dbl2float(pNormal->x) : 0.f;
        points[i].normal.y = OdNonZero(pNormal->y, FLT_MIN) ? dbl2float(pNormal->y) : 0.f;
        points[i].normal.z = OdNonZero(pNormal->z, FLT_MIN) ? dbl2float(pNormal->z) : 0.f;
        
        if(rm == OdGsView::kHiddenLine)
          points[i].color = m_bgColor;
        else if(m_bIsVertsColors)
          points[i].color = getD3DColor(vertexData()->trueColors()[vertInd]);
        else
          points[i].color = m_color;
      }
    }

    bool bNotMaterialWay = true;
    if (m_MaterialWay == eNotInit)
      bNotMaterialWay = !isMaterialAvailable() || !currentMaterial()->haveDiffuseTexture();
    else
      bNotMaterialWay = m_MaterialWay == eNotMaterialWay;

    if (bNotMaterialWay)
    {
      if (m_rPackager.isInitialized())
      {
        m_rPackager.addTriangle(m_bTwoSidesMode, points);
      }
      else
      {
        D3dTriangleNormalRecord pRec(points);
        m_pD3DSM->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE);
        pRec.play(m_mfPlayCtx);
        if (m_bTwoSidesMode)
        {
          m_mfPlayCtx.setReverseNormals(true);
          pRec.play(m_mfPlayCtx);
          m_mfPlayCtx.setReverseNormals(false);
        }
      }
    }
    else
    {
      OdD3DVertexNormalTexCol texPts[3];
      const bool bFullMapping = !vertexData() || !vertexData()->mappingCoords(OdGiVertexData::kAllChannels);
      if (bFullMapping)
      {
        OdGiMapperItemEntry::MapInputTriangle inputData;
        OdGiMapperItemEntry::MapOutputCoords outputData;
        for (i = 0; i < 3; i++)
        {
          texPts[i].position = points[i].position;
          texPts[i].normal = points[i].normal;
          texPts[i].color = points[i].color;
          inputData.inPt[i].set(points[i].position.x, points[i].position.y, points[i].position.z);
        }
        currentMapper()->diffuseMapper()->mapCoords(inputData, OdGeVector3d(points->normal.x, points->normal.y, points->normal.z), outputData);
        for (i = 0; i < 3; i++)
        {
          texPts[i].s = dbl2float(outputData.outCoord[i].x);
          texPts[i].t = dbl2float(outputData.outCoord[i].y);
        }
      }
      else
      {
        const OdGePoint3d *pTexCoords = vertexData()->mappingCoords(OdGiVertexData::kAllChannels);
        const OdGePoint3d inTexCoords[3] = { pTexCoords[p3Vertices[0]], pTexCoords[p3Vertices[1]], pTexCoords[p3Vertices[2]] };
        OdGePoint2d outTexCoords[3];
        currentMapper(false)->diffuseMapper()->mapPredefinedCoords(inTexCoords, outTexCoords, 3);
        for (i = 0; i < 3; i++)
        {
          texPts[i].position = points[i].position;
          texPts[i].normal = points[i].normal;
          texPts[i].color = points[i].color;
          texPts[i].s = dbl2float(outTexCoords[i].x);
          texPts[i].t = dbl2float(outTexCoords[i].y);
        }
      }
      if (m_rPackager.isInitialized())
      {
        m_rPackager.addTriangle(m_bTwoSidesMode, texPts);
        if (bFullMapping && isMappingDelayed())
          m_rPackager.addTriangleToDelayMapping(m_bTwoSidesMode);
      }
      else
      {
        D3dTriangleNormalTextureRecord* pRec = new D3dTriangleNormalTextureRecord(texPts);
        m_pD3DSM->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1);
        pRec->play(m_mfPlayCtx);
        if (m_bTwoSidesMode)
        {
          m_mfPlayCtx.setReverseNormals(true);
          pRec->play(m_mfPlayCtx);
          m_mfPlayCtx.setReverseNormals(false);
        }
        delete pRec;
      }
    }
  }
  if (m_pMetafile.get() == NULL && rm == OdGsView::kHiddenLine)
  {
    m_mfPlayCtx.stateManager().SetStencilPassRenderState(passState);
    m_mfPlayCtx.stateManager().SetStencilFuncRenderState(sfState);
  }
}

void ExGiDirectXGeometry::initTexture(const OdGePoint3d& origin,
                                      const OdGeVector3d& u,
                                      const OdGeVector3d& v,
                                      const OdGiRasterImage* pImg, // image object
                                      bool transparency,
                                      double brightness,
                                      double contrast,
                                      double fade)
{
  OdGsBaseDeviceMTSyncHelper __sync(*(m_pMViewRedirect->mtDeviceSync()), OdGsBaseDeviceMTHelpers::kSyncRasterCache);

  int srcColDepth = pImg->colorDepth();
  OdUInt32 srcWidth  = pImg->pixelWidth();
  OdUInt32 srcHeight = pImg->pixelHeight();
  OdUInt32 dstWidth  = srcWidth;
  OdUInt32 dstHeight = srcHeight;
  OdGiRasterImage::TransparencyMode srcTransparencyMode = pImg->transparencyMode();
  bool bTransparentOrBitonal = (srcColDepth == 1 && transparency) ||
    (srcColDepth == 32 && transparency && srcTransparencyMode != OdGiRasterImage::kTransparencyOff); // Alpha color available
  bool bSecColor = !transparency && m_pMViewRedirect && m_pMViewRedirect->isSecondaryColorNeed();
  ODCOLORREF secCol = 0;
  if (bSecColor)
    secCol = m_pMViewRedirect->getSecondaryColor();
  D3dMaterialViewRedirect::RasterImageRef texture;
  OdUInt32 findId = (m_pMViewRedirect && pImg->imageSource() == OdGiRasterImage::kFromFile) ? m_pMViewRedirect->findRasterImage(pImg, transparency, m_color, bSecColor, secCol, brightness, contrast, fade) : 0xFFFFFFFF;
  bool bOleSource = pImg->imageSource() == OdGiRasterImage::kFromOleObject;
  if (findId != 0xFFFFFFFF)
  {
    texture = m_pMViewRedirect->incrementRasterImageRef(findId);
  }
  else
  {
    OdGiRasterImagePtr pBitonalImg;
    const OdGiRasterImage *pImgStore = pImg;
    if(srcColDepth == 1)
    {
      OdSmartPtr<OdGiBitonalRasterTransformer> pBitRast = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();
      if (!bSecColor)
        pBitRast->setOriginal(pImg, SWAPBYTES(m_color), SWAPBYTES(m_bgColor), bTransparentOrBitonal);
      else
        pBitRast->setOriginal(pImg, SWAPBYTES(m_color), secCol, bTransparentOrBitonal);
      pBitonalImg = pBitRast;
      pImg = pBitonalImg;
    }
#ifdef OD_USE_DIRECT3D9
    bool bUseLut = (srcColDepth == 1) ? m_pMViewRedirect->useLutPaletteMonochrome()
                                      : m_pMViewRedirect->useLutPaletteColor();
    if (srcColDepth <= 8 && bUseLut && m_mfPlayCtx.paletteImageShader())
    {
      OdUInt8Array tmpScanLines;
      const OdUInt8 *pScanLines = pImg->scanLines();
      if (!pScanLines)
      {
        tmpScanLines.resize(pImg->scanLineSize() * pImg->pixelHeight());
        pImg->scanLines(tmpScanLines.asArrayPtr(), 0, pImg->pixelHeight());
        pScanLines = tmpScanLines.getPtr();
      }
      { // Scale for device.
        D3DCAPS devCaps;
        ZeroMemory(&devCaps, sizeof(D3DCAPS));
        m_pD3Ddevice->GetDeviceCaps(&devCaps);
        DWORD maxTextureWidth = devCaps.MaxTextureWidth;
        DWORD maxTextureHeight = devCaps.MaxTextureHeight;
        if (dstWidth > OdUInt32(maxTextureWidth))
          dstWidth = (OdUInt32)maxTextureWidth;
        if (dstHeight > OdUInt32(maxTextureHeight))
          dstHeight = (OdUInt32)maxTextureHeight;
      }
      m_pD3Ddevice->CreateTexture(dstWidth, dstHeight, 1, 0, D3DFMT_L8, D3DPOOL_MANAGED, &texture.m_pTexture);
      D3DLOCKED_RECT lockedRect;
      texture.m_pTexture->LockRect(0, &lockedRect, NULL, 0);
      ::makeLutTexturePixels(srcWidth, srcHeight, srcColDepth, pImg->scanLineSize(), pScanLines,
                             dstWidth, dstHeight, (OdUInt32)lockedRect.Pitch, (OdUInt8*)lockedRect.pBits);
      texture.m_pTexture->UnlockRect(0);
      texture.m_nTexLut = (int)::computeLutPaletteLength(pImg) / 4;
      m_pD3Ddevice->CreateTexture(texture.m_nTexLut, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.m_pPalette);
      texture.m_pPalette->LockRect(0, &lockedRect, NULL, 0);
      ::makeLutPalettePixels(pImg, (OdUInt8*)lockedRect.pBits, true);
      texture.m_pPalette->UnlockRect(0);
    }
    else
    {
#endif
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
    pDesc->setPixelWidth(srcWidth);
    pDesc->setPixelHeight(srcHeight);
    pDesc->setColorDepth(32);
    if (pImg->scanLinesAlignment() == 4)
      pDesc->setScanLinesAlignment(4);
    pDesc->pixelFormat().setBGRA();
    OdGiRasterImagePtr pCopyImg;
    pCopyImg = pImg->convert(true, brightness, contrast, fade, SWAPBYTES(m_bgColor), false, false, false, pDesc);
    pImg = pCopyImg;
    { // Scale for device (#4979). part I
      D3DCAPS devCaps;
      ZeroMemory(&devCaps, sizeof(D3DCAPS));
      m_pD3Ddevice->GetDeviceCaps(&devCaps);
      DWORD maxTextureWidth = devCaps.MaxTextureWidth;
      DWORD maxTextureHeight = devCaps.MaxTextureHeight;
      if (dstWidth > OdUInt32(maxTextureWidth))
        dstWidth = (OdUInt32)maxTextureWidth;
      if (dstHeight > OdUInt32(maxTextureHeight))
        dstHeight = (OdUInt32)maxTextureHeight;
    }

    // adding record, setting up the texture
    m_pD3Ddevice->CreateTexture(dstWidth, dstHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.m_pTexture);
    D3DLOCKED_RECT lockedRect;
    texture.m_pTexture->LockRect(0, &lockedRect, NULL, 0);
    OdUInt8* pBits = (OdUInt8*)lockedRect.pBits;
    if (dstWidth == srcWidth && dstHeight == srcHeight)
    {
      pImg->scanLines(pBits, 0, srcHeight);
    }
    else
    { // Scale for device (#4979). part II
      bool bTmpBits = false;
      OdUInt8 *pBitsTmp = const_cast<OdUInt8*>(pImg->scanLines());
      if (pBitsTmp == NULL)
      {
        pBitsTmp = new OdUInt8[(srcWidth * srcHeight) << 2];
        bTmpBits = true;
        pImg->scanLines(pBitsTmp, 0, srcHeight);
      }
      double aspectX = double(srcWidth - 1) / (dstWidth - 1),
             aspectY = double(srcHeight - 1) / (dstHeight - 1);
      for (OdUInt32 iy = 0; iy < dstHeight; iy++)
      {
        for (OdUInt32 ix = 0; ix < dstWidth; ix++)
        {
          OdUInt8 *pPtr = pBitsTmp + ((OdUInt32(aspectY * iy) * srcWidth + OdUInt32(aspectX * ix)) << 2);
          *(reinterpret_cast<OdUInt32*>(pBits)) = *(reinterpret_cast<OdUInt32*>(pPtr));
          pBits += 4;
        }
      }
      if (bTmpBits)
        delete []pBitsTmp;
    }
    texture.m_pTexture->UnlockRect(0);
    texture.m_nTexLut = 0;
#ifdef OD_USE_DIRECT3D9
    }
#endif
    if (m_pMViewRedirect)
      m_pMViewRedirect->addRasterImage(texture, pImgStore, transparency, m_color, bSecColor, secCol, brightness, contrast, fade);
    pImg = NULL; // Pointer shouldn't be used after . . .
  }

  OdGeMatrix3d world2uvInvU;
  OdGeVector3d normal = u.crossProduct(v);
  world2uvInvU.setCoordSystem(origin, u, v, normal);
  world2uvInvU.invert();
  OdGeMatrix3d uv2uvNorm;
  uv2uvNorm.setToScaling(OdGeScale3d(1.0 / srcWidth, 1.0 / srcHeight, 1.0));
  D3dInitTextureRecord *pInitRec = new D3dInitTextureRecord(texture, uv2uvNorm * world2uvInvU, 
    bTransparentOrBitonal, srcColDepth == 1, !giCtx().isPlotGeneration() &&
    renderMode() == OdGsView::k2DOptimized && giCtx().imageQuality() == OdGiContext::kImageQualityDraft ||
    bOleSource, (srcColDepth == 32) && (srcTransparencyMode == OdGiRasterImage::kTransparency1Bit),
    (srcColDepth == 32) && (srcTransparencyMode == OdGiRasterImage::kTransparency8Bit),
    m_pMViewRedirect, m_pD3DSM->GetLostStateManager());
  if (m_rPackager.isInitialized())
  {
    m_rPackager.addRecord(D3dMetafilePackager::kNLEntry, D3dMetafilePackager::kInitTextureRec, pInitRec);
  }
  else
  {
    pInitRec->play(m_mfPlayCtx);
    delete pInitRec;
  }
}

void ExGiDirectXGeometry::uninitTexture()
{
  D3dUninitTextureRecord *pUninitRec = new D3dUninitTextureRecord();
  if (m_rPackager.isInitialized())
  {
    m_rPackager.addRecord(D3dMetafilePackager::kNLEntry, D3dMetafilePackager::kUninitTextureRec, pUninitRec);
  }
  else
  {
    pUninitRec->play(m_mfPlayCtx);
    delete pUninitRec;
  }
}

struct D3dMetafilePackagerSaver
{
  D3dMetafilePackager m_rPackager;
  D3dMetafilePackager *m_pRPackager;
  D3dMetafilePackagerSaver(D3dMetafilePackager &rPackager)
    : m_rPackager(rPackager), m_pRPackager(&rPackager) { rPackager.runNestingMode(); }
  void stopOnDemand() { if (m_pRPackager) { m_pRPackager->stopNestingMode(); *m_pRPackager = m_rPackager; m_pRPackager = NULL; } }
  ~D3dMetafilePackagerSaver() { stopOnDemand(); }
};

void ExGiDirectXGeometry::textProc(const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                                   const OdChar* msg, OdInt32 nLength, bool raw, const OdGiTextStyle* pStyle, const OdGeVector3d* pExtrusion)
{
  OdTtfFontsCache::TextInfo ttfInfo;
  if (pExtrusion || !m_rPackager.isInitialized() || !m_pMViewRedirect || !m_pMViewRedirect->getTtfFontsCache() ||
      (renderMode() == OdGsView::k2DOptimized && giCtx().quickTextMode()))
  {
    OdGiGeometrySimplifier::textProc(position, u, v, msg, nLength, raw, pStyle, pExtrusion);
    return;
  }
  OdGiFillType fill;
  { // Emulate GeometrySimplifier behavior
    fill = m_pTraits->fillType();
    m_pTraits->setFillType(giCtx().fillTtf() ? kOdGiFillAlways : kOdGiFillNever);
    m_pTraits->setDrawFlags(m_pTraits->drawFlags() | OdGiSubEntityTraits::kDrawPolygonFill);
    m_pDrawCtx->onTraitsModified();
  }
  // Process text string
  { D3dMetafilePackagerSaver _rss(m_rPackager);
    OdGePoint3d posPt = position;
    OdGeVector3d uVec = u, vVec = v;
    if (m_pMViewRedirect->metafileTransformOverrided())
    {
      const OdGeMatrix3d &xForm = m_pMViewRedirect->metafileToWorldMatrix();
      posPt.transformBy(xForm);
      uVec.transformBy(xForm);
      vVec.transformBy(xForm);
    }
    if (!m_pMViewRedirect->getTtfFontsCache()->processText(posPt, uVec, vVec,
         msg, nLength, raw, pStyle, drawContext(), ttfInfo, m_pMViewRedirect))
    {
      _rss.stopOnDemand(); m_pMetafile = _rss.m_rPackager.metafile();
      OdGiGeometrySimplifier::textProc(position, u, v, msg, nLength, raw, pStyle, pExtrusion);
      return;
    }
    m_pMetafile = _rss.m_rPackager.metafile();
  }
  // Do that generateShellFaces done, to setup all properties once on metafile beginning
  if (renderMode() == OdGsView::k2DOptimized)
    draw_lineweight(OdDb::kLnWt000, 1);
  // Pack text
  m_rPackager.addRecord(D3dMetafilePackager::kNLEntry, D3dMetafilePackager::kTtfTextRec,
    new D3dTtfTextRecord(ttfInfo, m_lineWeight, m_color));
  { // Restore changed traits
    m_pTraits->setFillType(fill);
    m_pTraits->setDrawFlags(m_pTraits->drawFlags() & ~OdGiSubEntityTraits::kDrawPolygonFill);
    m_pDrawCtx->onTraitsModified();
  }
}

void ExGiDirectXGeometry::polypointProc(OdInt32 numPoints,
                                        const OdGePoint3d* vertexList,
                                        const OdCmEntityColor* pColors,
                                        const OdCmTransparency* pTransparency,
                                        const OdGeVector3d* pNormals,
                                        const OdGeVector3d* pExtrusions,
                                        const OdGsMarker* pSubEntMarkers,
                                        OdInt32 nPointSize)
{
  const DWORD prevColor = m_color;
  const OdGsMarker prevMarker = (pSubEntMarkers) ? m_mfPlayCtx.vectorizer()->selectionMarker() : kNullSubentIndex;
  if (nPointSize >= 1)
    m_lineWeight.m_type = D3dLwdSetting::kPoint, m_lineWeight.m_val.m_pixLineWeight = nPointSize;
  for (OdInt32 nPt = 0; nPt < numPoints; nPt++)
  {
    if (pColors || pTransparency)
    {
      DWORD color = (pColors) ? getD3DColor(pColors[nPt]) : prevColor;
      if (pTransparency) color = (color & 0x00FFFFFF) | (((DWORD)pTransparency[nPt].alpha()) << 24);
      draw_color(color);
      draw_transparency(0);
    }
    if (pSubEntMarkers)
      m_pDrawCtx->subEntityTraits().setSelectionMarker(pSubEntMarkers[nPt]);
    if (!pNormals || !pExtrusions)
      polylineProc(1, vertexList + nPt);
    else
      polylineProc(1, vertexList + nPt, pNormals + nPt, pExtrusions + nPt);
  }
  if (nPointSize >= 1) // Lineweight will be reset to actual onto next onTraitsModified call
    m_lineWeight.m_type = D3dLwdSetting::kModel, m_lineWeight.m_val.m_lwLineWeight = OdDb::kLnWt000;
  if (pColors)
    draw_color(prevColor);
  if (pSubEntMarkers)
    m_pDrawCtx->subEntityTraits().setSelectionMarker(prevMarker);
}

OdRxObjectPtr ExGiDirectXGeometry::newGsMetafile()
{
  return (OdRxObject*)ExGiDirectXMetafile::createObject();
}

void ExGiDirectXGeometry::beginMetafile(OdRxObject* pMetafile, OdGsBaseVectorizer *pVectorizer)
{
  // clear cache structures
  m_pMetafile = pMetafile;
  m_pMetafile->begin();
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: beginMetafile()\n");
#endif
  m_rPackager.beginMetafile(pVectorizer, m_pD3Ddevice, pMetafile);
  if (!m_pIntMtl.isNull())
    m_pIntMtl.release();
}

void ExGiDirectXGeometry::endMetafile(OdRxObject*, OdUInt64* pMetafileSize )
{
  if (!m_pIntMtl.isNull())
    m_pIntMtl.release();
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: flush()\n");
#endif
  m_rPackager.endMetafile();
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: endMetafile()\n");
#endif
  if( pMetafileSize )
  {
    *pMetafileSize = m_pMetafile->metafileSize();
  }
  m_pMetafile->end(this);
  m_pMetafile = 0;
}

void ExGiDirectXGeometry::playMetafile(const OdRxObject* pMetafile)
{
  if (!m_bPlayMetafile)
  {
    m_bPlayMetafile = true;
    static_cast<const ExGiDirectXMetafile*>(pMetafile)->play(m_mfPlayCtx);
    m_bPlayMetafile = false;
  }
  else
  { // Nested metafile
    const OdD3DVertexUnionPtr vptr1 = m_mfPlayCtx.vtPtr1();
    const OdD3DVertexUnionPtr vptr2 = m_mfPlayCtx.vtPtr2();
    const OdD3DSelMark *selMarks = m_mfPlayCtx.selMarks();
    static_cast<const ExGiDirectXMetafile*>(pMetafile)->play(m_mfPlayCtx);
    m_mfPlayCtx.setSelMarks(selMarks);
    m_mfPlayCtx.setVPtr2(vptr2);
    m_mfPlayCtx.setVPtr1(vptr1);
  }
}


//SEA
void ExGiDirectXGeometry::play4Select(const OdRxObject* pMetafile, D3dPlay4SelectContext& context )
{
  if( m_mfPlayCtx.isContextModified() )
  {
    //Restore context defaults
    m_mfPlayCtx.setVectorizer( m_mfPlayCtx.vectorizer() );
  }
  if (!m_bPlayMetafile)
  {
    m_bPlayMetafile = true;
    static_cast<const ExGiDirectXMetafile*>(pMetafile)->play4Select(m_mfPlayCtx, context);
    m_bPlayMetafile = false;
  }
  else
  { // Nested metafile
    const OdD3DVertexUnionPtr vptr1 = m_mfPlayCtx.vtPtr1();
    const OdD3DVertexUnionPtr vptr2 = m_mfPlayCtx.vtPtr2();
    const OdD3DSelMark *selMarks = m_mfPlayCtx.selMarks();
    static_cast<const ExGiDirectXMetafile*>(pMetafile)->play4Select(m_mfPlayCtx, context);
    m_mfPlayCtx.setSelMarks(selMarks);
    m_mfPlayCtx.setVPtr2(vptr2);
    m_mfPlayCtx.setVPtr1(vptr1);
  }
}

void ExGiDirectXGeometry::clearMetafile(OdRxObject* pMetafile)
{
  static_cast<ExGiDirectXMetafile*>(pMetafile)->clear();
}

//////////////////////////////// Metafile ///////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS(ExGiDirectXMetafile, OdRxObject, RXIMPL_CONSTR);

ExGiDirectXMetafile::ExGiDirectXMetafile()
{
  m_pNoLightTail = &m_noLightCache;
  m_pLightTail = &m_lightCache;
  m_pLineSegsTail = &m_lineSegsCache;
}

ExGiDirectXMetafile::~ExGiDirectXMetafile()
{
}

void ExGiDirectXMetafile::begin() // notify that metafile was started
{
  clear();
}

void ExGiDirectXMetafile::clear()
{
  m_noLightCache.deleteList();
  m_lightCache.deleteList();
  m_lineSegsCache.deleteList();
  m_pNoLightTail = &m_noLightCache;
  m_pLightTail = &m_lightCache;
  m_pLineSegsTail = &m_lineSegsCache;
}

void ExGiDirectXMetafile::play(D3dMetafilePlayContext& pCtx) const
{
  if (!m_lineSegsCache.tail() && !m_noLightCache.tail() && !m_lightCache.tail())
    return;
  IDirect3DDevice* pDev = pCtx.device();
  OdD3DStateManager *pSM = &(pCtx.stateManager());
  OdGsView::RenderMode rm = pCtx.renderMode();
  if(pCtx.forceFill())
    rm = OdGsView::k2DOptimized;
  DWORD sfState = D3DCMP_EQUAL;
  DWORD passState = D3DSTENCILOP_KEEP;

  if (pCtx.dynamicSubEntHlt())
    pCtx.setSelMarks((pCtx.isSubselectionRequired()) ? m_SelMarks.getPtr() : NULL);

  if(m_lineSegsCache.tail() || m_noLightCache.tail())
  {
    pSM->SetLightingRenderState(FALSE);
    pCtx.resetTextureState();
    pSM->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
    if(m_lineSegsCache.tail())
    {
#ifndef OD_USE_DIRECT3D9
      D3DXMATRIX matr, saveMatr;
      float delta = -(float)(1.0 / 256.0); // how to calculate it ???
      if(rm == OdGsView::kHiddenLine ||
         rm == OdGsView::kFlatShadedWithWireframe ||
         rm == OdGsView::kGouraudShadedWithWireframe) // moving lines to "top"
      {
        pDev->GetTransform(D3DTS_PROJECTION, &matr);
        saveMatr = matr;
        matr(3, 2) += delta;
        pDev->SetTransform(D3DTS_PROJECTION, &matr);
      }
#endif
      OdD3DVertexUnionPtr vPtr;
      vPtr.m_pVC = m_LineSegArray.asArrayPtr();
      pCtx.setVPtr1(vPtr);
      m_lineSegsCache.play(pCtx);
#ifndef OD_USE_DIRECT3D9
      if(rm == OdGsView::kHiddenLine || 
        rm == OdGsView::kFlatShadedWithWireframe || 
        rm == OdGsView::kGouraudShadedWithWireframe)
      {
        pDev->SetTransform(D3DTS_PROJECTION, &saveMatr);
      }
#endif
    }
    //if(m_noLightCache.tail())
    //  m_noLightCache.play(pCtx);
    // Triangles appropriate stencil buffer state enable
    if (m_noLightCache.tail())
    {
      if (rm == OdGsView::kHiddenLine)
      {
        sfState = pSM->GetStencilFuncRenderState();
        pSM->SetStencilFuncRenderState(D3DCMP_LESSEQUAL);
        passState = pSM->GetStencilPassRenderState();
        pSM->SetStencilPassRenderState(D3DSTENCILOP_INCRSAT);
      }
      OdD3DVertexUnionPtr vPtr;
      vPtr.m_pVC = m_NoLightTriArray.asArrayPtr();
      pCtx.setVPtr1(vPtr);
      m_noLightCache.play(pCtx);
      if (rm == OdGsView::kHiddenLine)
      {
        pSM->SetStencilPassRenderState(passState);
        pSM->SetStencilFuncRenderState(sfState);
      }
    }
    //
  }
  
  if(m_lightCache.tail())
  {
    if (rm == OdGsView::kHiddenLine)
    {
      sfState = pSM->GetStencilFuncRenderState();
      pSM->SetStencilFuncRenderState(D3DCMP_LESSEQUAL);
      passState = pSM->GetStencilPassRenderState();
      pSM->SetStencilPassRenderState(D3DSTENCILOP_INCRSAT);
    }

    pSM->SetLightingRenderState(TRUE);
    pCtx.resetTextureState();
    pSM->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE);
    pSM->SetCullModeRenderState(D3DCULL_CW);
    OdD3DVertexUnionPtr vPtr;
    vPtr.m_pVNC = m_LightTriArray.asArrayPtr();
    pCtx.setVPtr1(vPtr);
    vPtr.m_pVNTC = m_TexturedTriArray.asArrayPtr();
    pCtx.setVPtr2(vPtr);
    m_lightCache.play(pCtx);
    pSM->SetCullModeRenderState(D3DCULL_NONE);

    if (rm == OdGsView::kHiddenLine)
    {
      pSM->SetStencilPassRenderState(passState);
      pSM->SetStencilFuncRenderState(sfState);
    }
  }
  // Disable all compound render states
  pSM->SetAlphaTestEnableRenderState(FALSE);
}


OdUInt64 ExGiDirectXMetafile::metafileSize() const
{
  OdUInt64 size = m_lineSegsCache.recordSize();
  size += m_noLightCache.recordSize();
  size += m_lightCache.recordSize();

  size += m_LineSegArray.size() * sizeof( OdD3DVertexCol );
  size += m_NoLightTriArray.size() * sizeof( OdD3DVertexCol );
  size += m_LightTriArray.size() * sizeof( OdD3DVertexNormalCol );
  size += m_TexturedTriArray.size() * sizeof( OdD3DVertexNormalTexCol );
  size += m_SelMarks.size() * sizeof( OdD3DSelMark );

  return size;
}


void ExGiDirectXMetafile::play4Select( D3dMetafilePlayContext& pCtx, D3dPlay4SelectContext& ctx ) const
{
  if (!m_lineSegsCache.tail() && !m_noLightCache.tail() && !m_lightCache.tail())
    return;
  IDirect3DDevice* pDev = pCtx.device();
  OdD3DStateManager *pSM = &(pCtx.stateManager());
  OdGsView::RenderMode rm = pCtx.renderMode();
  if(pCtx.forceFill())
    rm = OdGsView::k2DOptimized;
  DWORD sfState = D3DCMP_EQUAL;
  DWORD passState = D3DSTENCILOP_KEEP;

  if (pCtx.dynamicSubEntHlt())
    pCtx.setSelMarks((pCtx.isSubselectionRequired()) ? m_SelMarks.getPtr() : NULL);

  if(m_lineSegsCache.tail() || m_noLightCache.tail())
  {

    pSM->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
    if(m_lineSegsCache.tail())
    {
      OdD3DVertexUnionPtr vPtr;
      vPtr.m_pVC = m_LineSegArray.asArrayPtr();
      pCtx.setVPtr1(vPtr);
      m_lineSegsCache.play4Select(pCtx, ctx);
    }

    // Triangles appropriate stencil buffer state enable
    if (m_noLightCache.tail())
    {
      OdD3DVertexUnionPtr vPtr;
      vPtr.m_pVC = m_NoLightTriArray.asArrayPtr();
      pCtx.setVPtr1(vPtr);
      m_noLightCache.play4Select(pCtx, ctx);
    }
    //
  }
  
  if(m_lightCache.tail())
  {
    pSM->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE);
    OdD3DVertexUnionPtr vPtr;
    vPtr.m_pVNC = m_LightTriArray.asArrayPtr();
    pCtx.setVPtr1(vPtr);
    vPtr.m_pVNTC = m_TexturedTriArray.asArrayPtr();
    pCtx.setVPtr2(vPtr);
    m_lightCache.play4Select(pCtx, ctx);
  }
}


void ExGiDirectXMetafile::end(ExGiDirectXGeometry* /*pGeom*/)
{
}

/////////////////////// D3dRecord ////////////////////////

void D3dRecord::destroy()
{
  D3dRecord* pCurr = this;
  while(pCurr)
  {
    D3dRecord* pTail = pCurr->m_pTail;
    delete pCurr;
    pCurr = pTail;
  }
}

/////////////////////// D3dHeadRecord ////////////////////////

void D3dHeadRecord::play(const D3dMetafilePlayContext& pCtx) const
{
  const D3dRecord* pCurr = tail();
  while(pCurr)
  {
    const D3dRecord* pTail = pCurr->tail();
    pCurr->play(pCtx);
    pCurr = pTail;
  }
}


OdUInt64 D3dHeadRecord::recordSize() const 
{
  OdUInt64 size = 0;
  const D3dRecord* pCurr = tail();
  while(pCurr)
  {
    const D3dRecord* pTail = pCurr->tail();
    size += pCurr->recordSize();
    pCurr = pTail;
  }
  return size;
}


void D3dHeadRecord::play4Select( const D3dMetafilePlayContext& pCtx, D3dPlay4SelectContext& ctx ) const
{
  const D3dRecord* pCurr = tail();
  while(pCurr)
  {
    const D3dRecord* pTail = pCurr->tail();
    pCurr->play4Select(pCtx, ctx);
    pCurr = pTail;
  }
}


void D3dHeadRecord::deleteList()
{
  if(m_pTail)
    m_pTail->destroy();
  m_pTail = 0;
}

D3dHeadRecord::~D3dHeadRecord()
{
  deleteList();
}

/////////////////////// D3dPointRecord ////////////////////////

D3dPointRecord::D3dPointRecord(const OdGePoint3d& pos, DWORD color)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPointRecord()\n");
#endif
  m_pos.x = dbl2float(pos.x);
  m_pos.y = dbl2float(pos.y);
  m_pos.z = dbl2float(pos.z);
  m_pos.color = color;
}

void D3dPointRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( ctx.isDisplayWithHighlight() )
  {
    DRAWPRIMITIVE(D3DPT_POINTLIST, 1, &m_pos, OdD3DVertexCol);
  }
}


void D3dPointRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( pContext.isSelectWithHighlight() )
  {
    OdGePoint3d pt( m_pos.x, m_pos.y, m_pos.z );
    ctx.pGeometry->polylineProc( 1, &pt );
  }
}


D3dPointRecord* D3dPointRecord::createObject(const OdGePoint3d& pos, DWORD color)
{
  return new D3dPointRecord(pos, color);
}

/////////////////////// D3dPackedPointRecord ////////////////////////

D3dPackedPointRecord::D3dPackedPointRecord(OdInt32 nInitialID, OdInt32 numPoints)
  : m_nInitialID(nInitialID)
  , m_nbPoints(numPoints)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedPointRecord(%d, %d)\n", nInitialID, numPoints);
#endif
}

void D3dPackedPointRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( ctx.isDisplayWithHighlight() )
  {
    DRAWPRIMITIVE(D3DPT_POINTLIST, m_nbPoints, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
  }
}


void D3dPackedPointRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( pContext.isSelectWithHighlight() )
  {
    OdGePoint3d OdPt;
    for( OdInt32 i = 0; i < m_nbPoints; ++i )
    {
      OdD3DVertexCol pt = pContext.vtPtr1().m_pVC[ m_nInitialID + i ];
      OdPt.x = pt.x;
      OdPt.y = pt.y;
      OdPt.z = pt.z;
      ctx.pGeometry->polylineProc( 1, &OdPt );
    }
  }
}


D3dPackedPointRecord* D3dPackedPointRecord::createObject(OdInt32 nInitialID, OdInt32 numPoints)
{
  return new D3dPackedPointRecord(nInitialID, numPoints);
}

/////////////////////// D3dPackedPointRecordWSel ////////////////////////

D3dPackedPointRecordWSel::D3dPackedPointRecordWSel(OdInt32 nInitialID, OdInt32 numPoints, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
  : D3dPackedPointRecord(nInitialID, numPoints)
  , m_nInitialSelMark(nInitialSelMark)
  , m_nbSelMarks(nbSelMarks)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedPointRecordWSel(%d, %d)\n", nInitialSelMark, nbSelMarks);
#endif
}

void D3dPackedPointRecordWSel::play(const D3dMetafilePlayContext& ctx) const
{
  if (ctx.selMarks())
  {
    const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbPoints + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( ctx.isDisplayWithHighlight() )
        {
          DRAWPRIMITIVE(D3DPT_POINTLIST, ctDiff, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
        }
      }
      if (nMarks)
      {
        ctx.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  }
  else
    D3dPackedPointRecord::play(ctx);
}


void D3dPackedPointRecordWSel::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( pContext.selMarks() && ctx.bCheckMarkFlag )
  {
    const OdD3DSelMark *pMarks = pContext.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbPoints + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        //DRAWPRIMITIVE(D3DPT_POINTLIST, ctDiff, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
        if( pContext.isSelectWithHighlight() )
        {
          OdGePoint3dArray pts;
          pts.resize( ctDiff );
          for( OdInt32 i = 0; i < ctDiff; ++i )
          {
            OdD3DVertexCol pt = pContext.vtPtr1().m_pVC[ nFirst + i ];
            pts[ i ].x = pt.x;
            pts[ i ].y = pt.y;
            pts[ i ].z = pt.z;
          }
          ctx.pGeometry->polylineProc( ctDiff, pts.asArrayPtr() );
        }
      }
      if (nMarks)
      {
        pContext.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  }
  else
    D3dPackedPointRecord::play4Select(pContext, ctx );
}

D3dPackedPointRecordWSel* D3dPackedPointRecordWSel::createObject(OdInt32 nInitialID, OdInt32 numPoints, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
{
  return new D3dPackedPointRecordWSel(nInitialID, numPoints, nInitialSelMark, nbSelMarks);
}

/////////////////////// D3dPolylineRecord ////////////////////////

D3dPolylineRecord::D3dPolylineRecord(OdInt32 nPoints, const OdGePoint3d* points, DWORD color)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPolylineRecord(%d)\n", nPoints);
#endif
  m_nPoints = nPoints;
  OdD3DVertexCol* pCurPt = m_points;
  ODA_ASSERT(nPoints > 1); // D3dPointRecord should be  created instead
  for(OdInt32 i = 0; i < nPoints; i ++, pCurPt ++, points ++)
  {
    pCurPt->x = dbl2float(points->x);
    pCurPt->y = dbl2float(points->y);
    pCurPt->z = dbl2float(points->z);
    pCurPt->color = color;
  }
}

D3dPolylineRecord* D3dPolylineRecord::createObject(OdInt32 nPoints, 
                                                   const OdGePoint3d* pPoints, 
                                                   DWORD color)
{
  void* pMem = operator new (sizeof(D3dPolylineRecord) + sizeof(OdD3DVertexCol) * (nPoints - 1));
  return ::new (pMem) D3dPolylineRecord(nPoints, pPoints, color);
}


OdUInt64 D3dPolylineRecord::recordSize() const
{
  return sizeof( OdD3DVertexCol ) * m_nPoints;
}

void D3dPolylineRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if (!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_LINESTRIP, m_nPoints - 1, m_points, OdD3DVertexCol);
  }
  else
  {
    ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, m_points, m_nPoints, ctx), m_nPoints, ctx.renderMode() >= OdGsView::kHiddenLine);
  }
}


void D3dPolylineRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  const OdD3DVertexCol* pCurPt = m_points;
  ctx.ptsArray.resize( m_nPoints - 1 );
  for( OdInt32 i = 0; i < m_nPoints - 1; ++i )
  {
    ctx.ptsArray[ i ].x = pCurPt[ i ].x;
    ctx.ptsArray[ i ].y = pCurPt[ i ].y;
    ctx.ptsArray[ i ].z = pCurPt[ i ].z;
  }
  ctx.pGeometry->polylineProc( m_nPoints - 1, ctx.ptsArray.asArrayPtr() );
}

/////////////////////// D3dPackedPolylineRecord ////////////////////////

D3dPackedPolylineRecord::D3dPackedPolylineRecord(OdInt32 nInitialID, OdInt32 numLines)
  : m_nInitialID(nInitialID)
  , m_nbLines(numLines)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedPolylineRecord(%d, %d)\n", nInitialID, numLines);
#endif
}

void D3dPackedPolylineRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if (!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_LINELIST, m_nbLines, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
  }
  else
  {
    const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + m_nInitialID;
    ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, m_nbLines << 1, ctx), m_nbLines << 1, ctx.renderMode() >= OdGsView::kHiddenLine, true);
  }
}


void D3dPackedPolylineRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + m_nInitialID;
  ctx.ptsArray.resize( 2 );
  for( OdInt32 i = 0; i < m_nbLines; ++i )
  {
    ctx.ptsArray[ 0 ].x = pt->x;
    ctx.ptsArray[ 0 ].y = pt->y;
    ctx.ptsArray[ 0 ].z = pt->z;
    pt++;
    ctx.ptsArray[ 1 ].x = pt->x;
    ctx.ptsArray[ 1 ].y = pt->y;
    ctx.ptsArray[ 1 ].z = pt->z;
    pt++;
    ctx.pGeometry->polylineProc( 2, ctx.ptsArray.asArrayPtr() );
  }
}


D3dPackedPolylineRecord* D3dPackedPolylineRecord::createObject(OdInt32 nInitialID, OdInt32 numLines)
{
  return new D3dPackedPolylineRecord(nInitialID, numLines);
}

/////////////////////// D3dPackedPolylineRecordWSel ////////////////////////

D3dPackedPolylineRecordWSel::D3dPackedPolylineRecordWSel(OdInt32 nInitialID, OdInt32 numLines, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
  : D3dPackedPolylineRecord(nInitialID, numLines)
  , m_nInitialSelMark(nInitialSelMark)
  , m_nbSelMarks(nbSelMarks)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedPolylineRecordWSel(%d, %d)\n", nInitialSelMark, nbSelMarks);
#endif
}

void D3dPackedPolylineRecordWSel::play(const D3dMetafilePlayContext& ctx) const
{
  if (ctx.selMarks())
  {
    const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = ((m_nbLines << 1) + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( ctx.isDisplayWithHighlight() )
        {
          if (!ctx.vectorizer()->isHighlighted())
          {
            DRAWPRIMITIVE(D3DPT_LINELIST, ctDiff >> 1, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
          }
          else
          {
            const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + nFirst;
            ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, ctDiff, ctx), (UINT)ctDiff, ctx.renderMode() >= OdGsView::kHiddenLine, true);
          }
        }
      }
      if (nMarks)
      {
        ctx.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedPolylineRecord::play(ctx);
}


void D3dPackedPolylineRecordWSel::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if (pContext.selMarks() && ctx.bCheckMarkFlag)
  {
    const OdD3DSelMark *pMarks = pContext.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = ((m_nbLines << 1) + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        /*if (!ctx.vectorizer()->isHighlighted())
        {
          DRAWPRIMITIVE(D3DPT_LINELIST, ctDiff >> 1, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
        }
        else
        {
          const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + nFirst;
          ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, ctDiff, ctx), (UINT)ctDiff, ctx.renderMode() >= OdGsView::kHiddenLine, true);
        }*/
        if( pContext.isSelectWithHighlight() )
        {
          const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + nFirst;
          ctx.ptsArray.resize( 2 );
          for( OdInt32 i = 0; i < ctDiff >> 1; ++i )
          {
            ctx.ptsArray[ 0 ].x = pt->x;
            ctx.ptsArray[ 0 ].y = pt->y;
            ctx.ptsArray[ 0 ].z = pt->z;
            pt++;
            ctx.ptsArray[ 1 ].x = pt->x;
            ctx.ptsArray[ 1 ].y = pt->y;
            ctx.ptsArray[ 1 ].z = pt->z;
            pt++;
            ctx.pGeometry->polylineProc( 2, ctx.ptsArray.asArrayPtr() );
          }
        }
      }
      if (nMarks)
      {
        pContext.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedPolylineRecord::play4Select( pContext, ctx );
}


D3dPackedPolylineRecordWSel* D3dPackedPolylineRecordWSel::createObject(OdInt32 nInitialID, OdInt32 numLines, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
{
  return new D3dPackedPolylineRecordWSel(nInitialID, numLines, nInitialSelMark, nbSelMarks);
}

/////////////////////// D3dPackedPolylineStripRecord ////////////////////////

D3dPackedPolylineStripRecord::D3dPackedPolylineStripRecord(OdInt32 nInitialID, OdInt32 numVertices)
  : m_nInitialID(nInitialID)
  , m_nbVertices(numVertices)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedPolylineStripRecord(%d, %d)\n", nInitialID, numVertices);
#endif
}

void D3dPackedPolylineStripRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if (!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_LINESTRIP, m_nbVertices - 1, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
  }
  else
  {
    const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + m_nInitialID;
    ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, m_nbVertices, ctx), m_nbVertices, ctx.renderMode() >= OdGsView::kHiddenLine);
  }
}


void D3dPackedPolylineStripRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + m_nInitialID;
  ctx.ptsArray.resize( m_nbVertices - 1 );
  for( OdInt32 i = 0; i < m_nbVertices - 1; ++i )
  {
    ctx.ptsArray[ i ].x = pt[ i ].x;
    ctx.ptsArray[ i ].y = pt[ i ].y;
    ctx.ptsArray[ i ].z = pt[ i ].z;
  }
  ctx.pGeometry->polylineProc( m_nbVertices - 1, ctx.ptsArray.asArrayPtr() );
}


D3dPackedPolylineStripRecord* D3dPackedPolylineStripRecord::createObject(OdInt32 nInitialID, OdInt32 numVertices)
{
  return new D3dPackedPolylineStripRecord(nInitialID, numVertices);
}

/////////////////////// D3dLtpPolylineRecord ////////////////////////

#ifndef OD_USE_DIRECT3D9
D3dLtpPolylineRecord::D3dLtpPolylineRecord(OdInt32 nPoints, const OdGePoint3d* pPoints, 
                                           DWORD color, WORD wLinePattern, WORD wRepeatFactor)
: m_nbPoints(nPoints)
, m_wLinePattern(wLinePattern)
, m_wRepeatFactor(wRepeatFactor)
{
  ODA_ASSERT(nPoints > 1); // D3dPointRecord should be  created instead
  OdD3DVertexCol* pCurPt = m_pPts;
  for(OdInt32 i = 0; i < nPoints; i ++, pCurPt ++, pPoints ++)
  {
    pCurPt->x = dbl2float(pPoints->x);
    pCurPt->y = dbl2float(pPoints->y);
    pCurPt->z = dbl2float(pPoints->z);
    pCurPt->color = color;
  }
}

D3dLtpPolylineRecord* D3dLtpPolylineRecord::createObject(OdInt32 nPoints, const OdGePoint3d* pPoints, 
                                                         DWORD color, WORD wLinePattern, WORD wRepeatFactor)
{
  void* pMem = operator new (sizeof(D3dLtpPolylineRecord) + sizeof(OdD3DVertexCol) * (nPoints - 1));
  return new (pMem) D3dLtpPolylineRecord(nPoints, pPoints, color, wLinePattern, wRepeatFactor);
}

void D3dLtpPolylineRecord::play(const D3dMetafilePlayContext& ctx) const
{
  IDirect3DDevice* pDev = ctx.device();

  D3DLINEPATTERN linePat;
  linePat.wLinePattern  = m_wLinePattern;
  linePat.wRepeatFactor = m_wRepeatFactor;
  pDev->SetRenderState(D3DRS_LINEPATTERN, (DWORD)OdPtrToInt32(&linePat));
  DRAWPRIMITIVE(D3DPT_LINESTRIP, m_nbPoints-1, m_pPts, OdD3DVertexCol);
  linePat.wLinePattern  = 0;
  linePat.wRepeatFactor = 0;
  pDev->SetRenderState(D3DRS_LINEPATTERN, (DWORD)OdPtrToInt32(&linePat));
}
#endif

/////////////////////// D3dLwdPolylineRecord ////////////////////////

D3dLwdPolylineRecord::D3dLwdPolylineRecord(OdInt32 nPoints, const OdGePoint3d* pPoints, 
                                           DWORD color, double lwd)
: m_nbPoints(calculateVerticesCount(nPoints, calculateCapsSize(lwd)))
, m_nbRPoints(nPoints)
, m_lwd(lwd)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dLwdPolylineRecord(%d)\n", nPoints);
#endif
  ODA_ASSERT(nPoints > 0); // Lwd can process one-point lines
  OdD3DVertexCol *pCurPt = m_pPts;
  for (OdInt32 i = 0; i < nPoints; i++, pCurPt++, pPoints++)
  {
    pCurPt->x = dbl2float(pPoints->x);
    pCurPt->y = dbl2float(pPoints->y);
    pCurPt->z = dbl2float(pPoints->z);
    pCurPt->color = color;
  }
  //regeneratePrimitive(nPoints, color, vcn, lwd, nprec);
}

D3dLwdPolylineRecord* D3dLwdPolylineRecord::createObject(OdInt32 nPoints, const OdGePoint3d* pPoints, 
                                                         DWORD color, double lwd)
{
  void* pMem = operator new (sizeof(D3dLwdPolylineRecord) + sizeof(OdD3DVertexCol) * (calculateVerticesCount(nPoints, calculateCapsSize(lwd)) - 1 + nPoints));
  return ::new (pMem) D3dLwdPolylineRecord(nPoints, pPoints, color, lwd);
}

OdUInt64 D3dLwdPolylineRecord::recordSize() const
{
  return sizeof(OdD3DVertexCol) * ( m_nbPoints + m_nbRPoints );
}

OdInt32 D3dLwdPolylineRecord::calculateVerticesCount(OdInt32 numPoints, int nprec)
{
  if (numPoints == 1) return 12 + ((nprec + 1) * 3) * 2;
  return (numPoints - 1) * (12 + ((nprec + 1) * 3) * 2);
}

int D3dLwdPolylineRecord::calculateCapsSize(double lwd)
{
  //return (int)((lwd > 4) ? ((lwd > 30) ? 15 : (lwd / 2)) : lwd);
  return (int)((lwd > 4) ? ((lwd > 15) ? 15 : lwd) : lwd);
}

void D3dLwdPolylineRecord::regeneratePrimitive(OdInt32 numPoints, DWORD color, const OdGeVector3d &vcn, double lwd, int nprec) {
  OdD3DVertexCol* pCurPt = m_pPts + numPoints;
  OdInt32 nperprim = calculateVerticesCount(numPoints, nprec);
  OdInt32 npointsreal = nperprim;
  OdD3DVertexCol* pPoints = m_pPts;
  if (numPoints > 1)
  {
    nperprim /= numPoints - 1;
    for (OdInt32 i = 0; i < npointsreal; i += nperprim, pCurPt += nperprim, pPoints++)
    {
      generateMesh(pCurPt, pPoints, pPoints + 1, color, vcn, lwd, nprec);
    }
  } else {
    generateMesh(pCurPt, pPoints, pPoints, color, vcn, lwd, nprec);
  }
}

void D3dLwdPolylineRecord::generateMesh(OdD3DVertexCol *verts, const OdD3DVertexCol *p1, const OdD3DVertexCol *p2, DWORD color, const OdGeVector3d &vcn, double lwd, int nprec)
{
  // @@@TODO: for optimize in future: pregenerate all-cap-size-lines on vectorizer start, and
  //          draw lines only by setup matrix for already generated primitive.
  OdGePoint3d pv1(p1->x, p1->y, p1->z), pv2(p2->x, p2->y, p2->z);
  OdGeVector3d vn = (pv1 != pv2) ? ((pv2 - pv1).normal()) : (vcn.perpVector()), vu;
  if (vcn.isZeroLength())
  {
    vu.set(-vn.y, vn.x, 0.0);
  }
  else
  {
    if (-vn != vcn)
    {
      vu = vn.crossProduct(vcn.normal());
      if (vu.isZeroLength())
        vu.set(-vn.y, vn.x, 0.0);
      else
        vu = vu.normal();
    }
    else
    {
      vu.set(-vn.y, vn.x, 0.0);
    }
  }

  FLOAT rad = (FLOAT)lwd / 2;
  OdGePoint3d pt;
  // Left line side
  pt = pv1 + vu * rad;
  verts[0].x = dbl2float(pt.x); verts[0].y = dbl2float(pt.y); verts[0].z = dbl2float(pt.z); verts[0].color = color;
  pt = pv2 + vu * rad;
  verts[1].x = dbl2float(pt.x); verts[1].y = dbl2float(pt.y); verts[1].z = dbl2float(pt.z); verts[1].color = color;
  pt = pv1;
  verts[2].x = dbl2float(pt.x); verts[2].y = dbl2float(pt.y); verts[2].z = dbl2float(pt.z); verts[2].color = color;
  verts[3].x = dbl2float(pt.x); verts[3].y = dbl2float(pt.y); verts[3].z = dbl2float(pt.z); verts[3].color = color;
  pt = pv2 + vu * rad;
  verts[4].x = dbl2float(pt.x); verts[4].y = dbl2float(pt.y); verts[4].z = dbl2float(pt.z); verts[4].color = color;
  pt = pv2;
  verts[5].x = dbl2float(pt.x); verts[5].y = dbl2float(pt.y); verts[5].z = dbl2float(pt.z); verts[5].color = color;
  // Right line side
  pt = pv1;
  verts[6].x = dbl2float(pt.x); verts[6].y = dbl2float(pt.y); verts[6].z = dbl2float(pt.z); verts[6].color = color;
  pt = pv2;
  verts[7].x = dbl2float(pt.x); verts[7].y = dbl2float(pt.y); verts[7].z = dbl2float(pt.z); verts[7].color = color;
  pt = pv1 - vu * rad;
  verts[8].x = dbl2float(pt.x); verts[8].y = dbl2float(pt.y); verts[8].z = dbl2float(pt.z); verts[8].color = color;
  verts[9].x = dbl2float(pt.x); verts[9].y = dbl2float(pt.y); verts[9].z = dbl2float(pt.z); verts[9].color = color;
  pt = pv2;
  verts[10].x = dbl2float(pt.x); verts[10].y = dbl2float(pt.y); verts[10].z = dbl2float(pt.z); verts[10].color = color;
  pt = pv2 - vu * rad;
  verts[11].x = dbl2float(pt.x); verts[11].y = dbl2float(pt.y); verts[11].z = dbl2float(pt.z); verts[11].color = color;
  //
  int vertin = 12;

  // Left halfdisk
  int i;
  for (i = 0; i < (nprec + 2); i++)
  {
    double x;
    double y;
    double theta;
    if (i == 0)
    {
      x = 0;
      y = 0;
      theta = 0;
    }
    else
    {
      theta = (double)(i - 1) / (2 * nprec) * Oda2PI + OdaPI2;
      x = cos(theta);
      y = sin(theta);
    }
    OdGePoint3d pt = pv1 + (vn * x + vu * y) * rad;
    if (i == 0) pt = pv1 + vu * rad; else if (i == nprec + 1) pt = pv1 - vu * rad;
    verts[vertin + i].x = dbl2float(pt.x);
    verts[vertin + i].y = dbl2float(pt.y);
    verts[vertin + i].z = dbl2float(pt.z);
    verts[vertin + i].color = color;
    if (i > 0)
    {
      vertin++;
      verts[vertin + i].x = p1->x;
      verts[vertin + i].y = p1->y;
      verts[vertin + i].z = p1->z;
      verts[vertin + i].color = color;
      if (i != nprec + 1)
      {
        vertin++;
        verts[vertin + i] = verts[vertin - 2 + i];
      }
    }
  }
  vertin += i;
  // Right halfdisk
  for (i = 0; i < (nprec + 2); i++)
  {
    double x;
    double y;
    double theta;
    if (i == 0)
    {
      x = 0;
      y = 0;
      theta = 0;
    }
    else
    {
      theta = (double)(i - 1) / (2 * nprec) * Oda2PI - OdaPI2;
      x = cos(theta);
      y = sin(theta);
    }
    OdGePoint3d pt = pv2 + (vn * x + vu * y) * rad;
    if (i == 0) pt = pv2 - vu * rad; else if (i == nprec + 1) pt = pv2 + vu * rad;
    verts[vertin + i].x = dbl2float(pt.x);
    verts[vertin + i].y = dbl2float(pt.y);
    verts[vertin + i].z = dbl2float(pt.z);
    verts[vertin + i].color = color;
    if (i > 0)
    {
      vertin++;
      verts[vertin + i].x = p2->x;
      verts[vertin + i].y = p2->y;
      verts[vertin + i].z = p2->z;
      verts[vertin + i].color = color;
      if (i != nprec + 1)
      {
        vertin++;
        verts[vertin + i] = verts[vertin - 2 + i];
      }
    }
  }
}

class OdExDirectXExtendedLineWeightStyleClient : public OdExExtendedLineWeightStyleClient
{
  const D3dMetafilePlayContext *pCtx;
  DWORD color;
  int nPx;
  public:
    OdExDirectXExtendedLineWeightStyleClient(const OdGeVector3d &axis, double lwd, OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle, const D3dMetafilePlayContext *ctx, DWORD _color, int px)
      : OdExExtendedLineWeightStyleClient(axis, lwd, capStyle, joinStyle), pCtx(ctx), color(_color), nPx(px)
    {
    }
    inline void resetColor(DWORD _color) { color = _color; }
    virtual bool elsPolygonOut(const OdGePoint3d *pPoints, OdUInt32 nPoints)
    {
      OdD3DVertexCol vtxs[4];
      for (OdUInt32 n = 0; n < nPoints; n++)
      {
        vtxs[n].x = dbl2float(pPoints[n].x);
        vtxs[n].y = dbl2float(pPoints[n].y);
        vtxs[n].z = dbl2float(pPoints[n].z);
        vtxs[n].color = color;
      }
      pCtx->device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, nPoints - 2, vtxs, sizeof(OdD3DVertexCol));
      return true;
    }
    virtual bool elsCircleOut(const OdGePoint3d &pPoint, const OdGeVector3d &normal, double radius)
    {
      OdD3DVertexCol vtxs[2];
      vtxs[0].x = vtxs[1].x = dbl2float(pPoint.x);
      vtxs[0].y = vtxs[1].y = dbl2float(pPoint.y);
      vtxs[0].z = vtxs[1].z = dbl2float(pPoint.z);
      vtxs[0].color = vtxs[1].color = color;
      OdGeVector3d xVec = normal.perpVector(), yVec;
      yVec = normal.crossProduct(xVec);
      pCtx->renderLineCap(nPx, vtxs, yVec * radius, xVec * radius);
      return true;
    }
};

inline bool operator ==(const OdD3DVertexCol &v1, const OdD3DVertexCol &v2)
{
  return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}
inline bool operator !=(const OdD3DVertexCol &v1, const OdD3DVertexCol &v2)
{
  return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
}

void D3dLwdPolylineRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  isNeedRebuild(ctx);
  if (ctx.lwdStyle().m_bCurLwdDef || (m_nbRPoints == 2 && m_pPts[0] == m_pPts[1]))
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, m_nbPoints / 3, m_pPts + m_nbRPoints, OdD3DVertexCol);
  }
  else
  {
    OdGeVector3d vcn(ctx.view().target() - ctx.view().position());
    if (ctx.materialViewRedirect() != NULL && ctx.materialViewRedirect()->metafileTransformOverrided())
    {
      vcn.transformBy(ctx.materialViewRedirect()->worldToMetafileMatrix());
    }
    const OdD3DVertexCol *pVerts = m_pPts + m_nbRPoints;
    OdExDirectXExtendedLineWeightStyleClient _exproc(vcn.normal(),
      OdGeVector3d(pVerts[8].x - pVerts[0].x, pVerts[8].y - pVerts[0].y, pVerts[8].z - pVerts[0].z).length(),
      ctx.lwdStyle().m_curLwdLes, ctx.lwdStyle().m_curLwdLjs, &ctx, ctx.makeColorOverride(pVerts->color), (int)m_lwd);
    OdGePoint3dArray &verts = ctx.lwdBuf();
    verts.resize(m_nbRPoints);
    OdGePoint3d *pOutVerts = verts.asArrayPtr();
    for (OdInt32 n = 0; n < m_nbRPoints; n++)
      pOutVerts[n].set(m_pPts[n].x, m_pPts[n].y, m_pPts[n].z);
    _exproc.process(pOutVerts, m_nbRPoints);
    verts.clear();
    if (verts.physicalLength() > OD_D3D_LWDBUFKEEPLIMIT)
      verts.setPhysicalLength(OD_D3D_LWDBUFKEEPLIMIT);
  }
  if (ctx.vectorizer()->isHighlighted())
  {
    ctx.renderHighlightedLine(m_pPts, m_nbRPoints, true);
  }
}


void D3dLwdPolylineRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( pContext.isSelectWithHighlight() ) return;
  ctx.ptsArray.resize( m_nbPoints );
  const OdD3DVertexCol *pVerts = m_pPts + m_nbRPoints;
  for( OdInt32 i = 0; i < m_nbPoints; ++i )
  {
    ctx.ptsArray[ i ].x = pVerts[ i ].x;
    ctx.ptsArray[ i ].y = pVerts[ i ].y;
    ctx.ptsArray[ i ].z = pVerts[ i ].z;
  }
  ctx.pGeometry->polylineProc( m_nbPoints, ctx.ptsArray.asArrayPtr() );

}


bool D3dLwdPolylineRecord::isNeedRebuild(const D3dMetafilePlayContext &pContext) const
{
  double lwCft = 100.0;
  OdGeVector3d vcn;
  bool regen = true;
  if (pContext.vectorizer() != NULL)
  {
    regen = (m_vcn[0] != pContext.view().target() || m_vcn[1] != pContext.view().position());
    if (regen)
    {
      vcn = pContext.view().target() - pContext.view().position();
      { // Lineweights recalculation coefficient
        OdGeMatrix3d m3d = pContext.view().worldToDeviceMatrix();
        OdGePoint3d pt1 = OdGePoint3d(m_pPts->x, m_pPts->y, m_pPts->z), pt2 = pt1 + pContext.view().upVector();
        pt1.transformBy(m3d); pt2.transformBy(m3d);
        lwCft = (pt2 - pt1).length();
        /* @@@ This calculation may be needed for all points in perspective mode */
      }
      m_vcn[0] = (const OdGePoint3d&)pContext.view().target();
      m_vcn[1] = pContext.view().position();
    }
  }

  if (regen)
  {
    if (pContext.materialViewRedirect() != NULL && pContext.materialViewRedirect()->metafileTransformOverrided())
    {
      vcn.transformBy(pContext.materialViewRedirect()->worldToMetafileMatrix());
    }
    const_cast<D3dLwdPolylineRecord*>(this)->regeneratePrimitive(m_nbRPoints, m_pPts->color, vcn, (OdZero(lwCft)) ? (1.0) : ((double)m_lwd / lwCft), calculateCapsSize(m_lwd));
  }
  return regen;
}

/////////////////////// D3dPackedLwdPolylineRecord ////////////////////////

D3dPackedLwdPolylineRecord::D3dPackedLwdPolylineRecord(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight)
  : m_bPoints(bPoints)
  , m_nInitialID(nInitialID)
  , m_nbLines(numLines)
  , m_lineWeight(lineWeight)
  , m_nPxSize(-1)
  , m_nView(0)
  , m_lwScaleCoef(1.0)
  , m_lwCoef(0.0)
  , m_flags(0)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedLwdPolylineRecord(%d, %d)\n", nInitialID, numLines);
#endif
}

void D3dPackedLwdPolylineRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if (ctx.isLineweightOverride())
  {
    D3dPackedLwdPolylineRecord *pThis = const_cast<D3dPackedLwdPolylineRecord*>(this);
    if (ctx.applyLineweightOverride(pThis->m_lineWeight))
      m_flags = 0;
  }
  OdInt32 pxSize = computePxSize(ctx);
  if (pxSize < 2)
  {
    if (!m_bPoints)
    {
      if (!ctx.vectorizer()->isHighlighted())
      {
        DRAWPRIMITIVE(D3DPT_LINELIST, m_nbLines, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
      }
      else
      {
        const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + m_nInitialID;
        ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, m_nbLines << 1, ctx), m_nbLines << 1, ctx.renderMode() >= OdGsView::kHiddenLine, true);
      }
    }
    else
    {
      DRAWPRIMITIVE(D3DPT_POINTLIST, m_nbLines * 2, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
    }
  }
  else
  {
    // Lineweight
    if (isNeedCacheRebuilding(ctx))
    {
      regenCache(ctx);
    }
    // Render
    if (ctx.lwdStyle().m_bCurLwdDef)
    {
      const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + m_nInitialID;
      for (OdInt32 i = 0; i < m_nbLines; i++, points += 2/*, ptCache += 6*/)
      {
        ctx.renderLineCap(pxSize, CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, 2, ctx), m_pCache[i].dir, m_pCache[i].side);
      }
    }
    else
    {
      // Render with caps&joints
      OdGeVector3d vcn(m_viewDirection);
      if (ctx.materialViewRedirect() != NULL && ctx.materialViewRedirect()->metafileTransformOverrided())
      {
        vcn.transformBy(ctx.materialViewRedirect()->worldToMetafileMatrix());
      }
      OdExDirectXExtendedLineWeightStyleClient _exproc(vcn.normal(), m_dSize2,
        ctx.lwdStyle().m_curLwdLes, ctx.lwdStyle().m_curLwdLjs, &ctx, 0, (int)pxSize);
      OdUInt32 counter = 0;
      const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + m_nInitialID;
      OdGePoint3dArray &buffer = ctx.lwdBuf();
      for (OdInt32 i = 0; i < m_nbLines; i++, points += 2)
      {
        if (points[0] == points[1])
        {
          // Draw point
          ctx.renderLineCap(pxSize, CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, 2, ctx), m_pCache[i].dir, m_pCache[i].side);
        }
        else if (counter == 0)
        {
          _exproc.resetColor(ctx.makeColorOverride(points[0].color));
          buffer.push_back(OdGePoint3d(points[0].x, points[0].y, points[0].z));
          buffer.push_back(OdGePoint3d(points[1].x, points[1].y, points[1].z));
          counter += 2;
        }
        else if (points[-1] == points[0] && points[-1].color == points[0].color)
        {
          buffer.push_back(OdGePoint3d(points[1].x, points[1].y, points[1].z));
          counter++;
        }
        else
        {
          _exproc.process(buffer.getPtr(), counter);
          buffer.clear();
          counter = 0;
          i--; points -= 2; // restart loop to check point
        }
      }
      // flush
      if (!buffer.empty())
        _exproc.process(buffer.getPtr(), counter);
      buffer.clear();
      if (buffer.physicalLength() > OD_D3D_LWDBUFKEEPLIMIT)
        buffer.setPhysicalLength(OD_D3D_LWDBUFKEEPLIMIT);
    }
    if (ctx.vectorizer()->isHighlighted())
    {
      const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + m_nInitialID;
      ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, m_nbLines << 1, ctx), m_nbLines << 1, true, true);
    }
  }
}


void D3dPackedLwdPolylineRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isDisplayWithHighlight() ) return;
  if (!m_bPoints)
  {
    const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + m_nInitialID;
    ctx.ptsArray.resize( 2 );
    for( OdInt32 i = 0; i < m_nbLines; ++i )
    {
      ctx.ptsArray[ 0 ].x = pt->x;
      ctx.ptsArray[ 0 ].y = pt->y;
      ctx.ptsArray[ 0 ].z = pt->z;
      pt++;
      ctx.ptsArray[ 1 ].x = pt->x;
      ctx.ptsArray[ 1 ].y = pt->y;
      ctx.ptsArray[ 1 ].z = pt->z;
      pt++;
      ctx.pGeometry->polylineProc( 2, ctx.ptsArray.asArrayPtr() );
    }
  }
  else
  {
    //DRAWPRIMITIVE(D3DPT_POINTLIST, m_nbLines * 2, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
    OdGePoint3d OdPt;
    for( OdInt32 i = 0; i < m_nbLines * 2; ++i )
    {
      OdD3DVertexCol pt = pContext.vtPtr1().m_pVC[ m_nInitialID + i ];
      OdPt.x = pt.x;
      OdPt.y = pt.y;
      OdPt.z = pt.z;
      ctx.pGeometry->polylineProc( 1, &OdPt );
    }
  }
}


OdInt32 D3dPackedLwdPolylineRecord::computePxSize(const D3dMetafilePlayContext& ctx) const
{
  OdInt32 pxSize = m_lineWeight.toPixels(ctx.view());
  // m_flags:
  // 1 - PS
  // 2 - PxSize changed
  // 4 - Lwd scale coef changed
  // 8 - Target changed
  // 16 - lwCoef changed
  // 128 - Cache is not calculated
  if ((m_flags & 1) == 1)
    m_flags = 1;
  else
    m_flags = 0;
  if (pxSize != m_nPxSize)
  {
    m_flags |= 2;
    if (m_nPxSize == -1)
      m_flags |= 128;
    if (pxSize >= 2)
      m_nPxSize = pxSize;
  }
  return pxSize;
}

bool D3dPackedLwdPolylineRecord::isNeedCacheRebuilding(const D3dMetafilePlayContext& ctx) const
{
  OdGeVector3d target = ctx.view().target() - ctx.view().position();
#ifdef ODGS_LOCAL_IDS
  OdUInt32 nView = ctx.view().localViewportId(ctx.vectorizer()->gsWriter().gsModel());
#else
  OdUInt32 nView = ctx.view().viewportId();
#endif
  double scaleCoef = ctx.view().lineweightToDcScale();
  if (scaleCoef != m_lwScaleCoef)
  {
    m_flags |= 4;
    m_lwScaleCoef = scaleCoef;
  }
  if (target != m_viewDirection)
  {
    m_flags |= 8;
    m_viewDirection = target;
  }
  if ((m_flags & (2 | 4)) == (2 | 4))
  {
    m_flags |= 1;
  }
  if (nView != m_nView)
  {
    m_nView = nView;
    return true; // Need rebuild since cache reused by another view
  }
  if ((m_flags & 1) == 1 && (m_flags & 8) == 0) // PS
    return false;
  if ((m_flags & (2 | 4)) == 0 || (m_flags & 128) == 128)
    return true; // MS
  return false;
}

void D3dPackedLwdPolylineRecord::regenCache(const D3dMetafilePlayContext& ctx) const
{
  OdGeMatrix3d m3d = ctx.view().worldToDeviceMatrix();
  const OdD3DVertexCol *pPt = ctx.vtPtr1().m_pVC + m_nInitialID;
  OdGePoint3d pt1 = OdGePoint3d(pPt->x, pPt->y, pPt->z), pt2 = pt1 + ctx.view().upVector();
  pt1.transformBy(m3d); pt2.transformBy(m3d);

  double lwCft = (pt2 - pt1).length();
  if (lwCft != m_lwCoef)
  {
    m_flags |= 16;
    m_lwCoef = lwCft;
  }
  if ((m_flags & (8 | 16)) == 0)
    return; // Nothing to regen
  lwCft = double(m_nPxSize) / lwCft;
  m_dSize2 = lwCft;
  m_dSize = m_dSize2 * 0.5;
  OdGeVector3d vcn = m_viewDirection;

  if (ctx.materialViewRedirect() != NULL && ctx.materialViewRedirect()->metafileTransformOverrided())
  {
    vcn.transformBy(ctx.materialViewRedirect()->worldToMetafileMatrix());
  }
  for (OdInt32 i = 0; i < m_nbLines; i++)
  {
    regenLine(m_pCache + i, ctx.vtPtr1().m_pVC + m_nInitialID + (i << 1), vcn);
    if (ctx.materialViewRedirect() != NULL)
    {
      ctx.materialViewRedirect()->xformByMetafileTransform(m_pCache[i].dir);
      ctx.materialViewRedirect()->xformByMetafileTransform(m_pCache[i].side);
    }
  }
}

void D3dPackedLwdPolylineRecord::regenLine(D3dPackedLwdPolylineRecord::LwdCache *outCache, const OdD3DVertexCol *inVerts, const OdGeVector3d &vcn) const
{
  OdGePoint3d pv1(inVerts[0].x, inVerts[0].y, inVerts[0].z), pv2(inVerts[1].x, inVerts[1].y, inVerts[1].z);
  OdGeVector3d vn = (pv1 != pv2) ? ((pv2 - pv1).normal()) : (vcn.perpVector()), vu;
  if (vcn.isZeroLength())
  {
    vu.set(-vn.y, vn.x, 0.0);
  }
  else
  {
    if (-vn != vcn)
    {
      vu = vn.crossProduct(vcn.normal());
      if (vu.isZeroLength())
        vu.set(-vn.y, vn.x, 0.0);
      else
        vu = vu.normal();
    }
    else
    {
      vu.set(-vn.y, vn.x, 0.0);
    }
  }
  outCache->dir = vn * m_dSize;
  outCache->side = vu * m_dSize;
}

D3dPackedLwdPolylineRecord* D3dPackedLwdPolylineRecord::createObject(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight)
{
  void* pMem = operator new (sizeof(D3dPackedLwdPolylineRecord) + sizeof(LwdCache) * (numLines - 1));
  return ::new (pMem) D3dPackedLwdPolylineRecord(bPoints, nInitialID, numLines, lineWeight);
}

/////////////////////// D3dPackedLwdPolylineRecordWSel ////////////////////////

D3dPackedLwdPolylineRecordWSel::D3dPackedLwdPolylineRecordWSel(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
  : D3dPackedLwdPolylineRecord(bPoints, nInitialID, numLines, lineWeight)
  , m_nInitialSelMark(nInitialSelMark)
  , m_nbSelMarks(nbSelMarks)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedLwdPolylineRecordWSel(%d, %d)\n", nInitialSelMark, nbSelMarks);
#endif
}

void D3dPackedLwdPolylineRecordWSel::play(const D3dMetafilePlayContext& ctx) const
{
  if (ctx.selMarks())
  {
    OdInt32 pxSize = computePxSize(ctx);
    if (pxSize < 2)
    {
      if (m_bPoints)
      {
        D3dPackedLwdPolylineRecord::play(ctx);
        return;
      }
      const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
      OdInt32 nMarks = m_nbSelMarks;
      OdInt32 nFirst = m_nInitialID;
      OdInt32 ctDiff;
      do
      {
        if (nMarks)
        {
          ctDiff = pMarks->uFrom - nFirst;
          if (ctDiff < 0)
            ctDiff = 0;
        }
        else
          ctDiff = ((m_nbLines << 1) + m_nInitialID) - nFirst;
        if (ctDiff)
        {
          if( ctx.isDisplayWithHighlight() )
          {
            if (!ctx.vectorizer()->isHighlighted())
            {
              DRAWPRIMITIVE(D3DPT_LINELIST, ctDiff >> 1, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
            }
            else
            {
              const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + nFirst;
              ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, ctDiff, ctx), (OdUInt32)ctDiff, ctx.renderMode() >= OdGsView::kHiddenLine, true);
            }
          }
        }
        if (nMarks)
        {
          ctx.setSelectionMarker(pMarks->gsMark);
          nFirst += ctDiff;
          pMarks++;
        }
        nMarks--;
      }
      while (nMarks != -1);
    }
    else
    {
      // Lineweight
      if (isNeedCacheRebuilding(ctx))
      {
        regenCache(ctx);
      }
      const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
      OdInt32 nMarks = m_nbSelMarks;
      OdInt32 nFirst = m_nInitialID;
      OdInt32 ctDiff;
      do
      {
        if (nMarks)
        {
          ctDiff = pMarks->uFrom - nFirst;
          if (ctDiff < 0)
            ctDiff = 0;
        }
        else
          ctDiff = ((m_nbLines << 1) + m_nInitialID) - nFirst;
        if (ctDiff)
        {
          if( ctx.isDisplayWithHighlight() )
          {
            const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + nFirst;
            {
              for (OdInt32 i = 0; i < (ctDiff >> 1); i++, points += 2)
              {
                OdInt32 cacheId = i + ((nFirst - m_nInitialID) >> 1);
                ctx.renderLineCap(pxSize, CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, 2, ctx), m_pCache[cacheId].dir, m_pCache[cacheId].side);
              }
            }
            if (ctx.vectorizer()->isHighlighted())
            {
              const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + nFirst;
              ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, ctDiff, ctx), (OdUInt32)ctDiff, true, true);
            }
          }
        }
        if (nMarks)
        {
          ctx.setSelectionMarker(pMarks->gsMark);
          nFirst += ctDiff;
          pMarks++;
        }
        nMarks--;
      }
      while (nMarks != -1);
    }
  } else
    D3dPackedLwdPolylineRecord::play(ctx);
}


void D3dPackedLwdPolylineRecordWSel::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  if (pContext.selMarks() && ctx.bCheckMarkFlag)
  {
    if (m_bPoints)
    {
      D3dPackedLwdPolylineRecord::play4Select( pContext, ctx );
      return;
    }
    const OdD3DSelMark *pMarks = pContext.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = ((m_nbLines << 1) + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        /*if (!pContext.vectorizer()->isHighlighted())
        {
          DRAWPRIMITIVE(D3DPT_LINELIST, ctDiff >> 1, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
        }
        else
        {
          const OdD3DVertexCol *points = ctx.vtPtr1().m_pVC + nFirst;
          ctx.renderHighlightedLine(CORDATA(OdD3DVertexCol, D3DPT_POINTLIST, points, ctDiff, ctx), (OdUInt32)ctDiff, ctx.renderMode() >= OdGsView::kHiddenLine, true);
        }*/
        const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + nFirst;
        ctx.ptsArray.resize( 2 );
        for( OdInt32 i = 0; i < ctDiff >> 1; ++i )
        {
          ctx.ptsArray[ 0 ].x = pt->x;
          ctx.ptsArray[ 0 ].y = pt->y;
          ctx.ptsArray[ 0 ].z = pt->z;
          pt++;
          ctx.ptsArray[ 1 ].x = pt->x;
          ctx.ptsArray[ 1 ].y = pt->y;
          ctx.ptsArray[ 1 ].z = pt->z;
          pt++;
          ctx.pGeometry->polylineProc( 2, ctx.ptsArray.asArrayPtr() );
        }
      }
      if (nMarks)
      {
        pContext.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  }
  else
    D3dPackedLwdPolylineRecord::play4Select( pContext, ctx );
}


D3dPackedLwdPolylineRecordWSel* D3dPackedLwdPolylineRecordWSel::createObject(bool bPoints, OdInt32 nInitialID, OdInt32 numLines, const D3dLwdSetting &lineWeight, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
{
  void* pMem = operator new (sizeof(D3dPackedLwdPolylineRecordWSel) + sizeof(LwdCache) * (numLines - 1));
  return ::new (pMem) D3dPackedLwdPolylineRecordWSel(bPoints, nInitialID, numLines, lineWeight, nInitialSelMark, nbSelMarks);
}

/////////////////////// D3dTriangleRecord ////////////////////////

D3dTriangleRecord::D3dTriangleRecord(const OdD3DVertexCol* points)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTriangleRecord\n");
#endif
  memcpy(m_points, points, 3 * sizeof(OdD3DVertexCol));
}

// NB: this function must be rarely called
void D3dTriangleRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if(!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, 1, m_points, OdD3DVertexCol);
  }
  else
  {
    ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, 1, CORDATA(OdD3DVertexCol, D3DPT_TRIANGLELIST, m_points, 1, ctx), sizeof(OdD3DVertexCol));
  }
}


void D3dTriangleRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  ctx.ptsArray.resize( 3 );
  for( OdInt32 i = 0; i < 3; ++i )
  {
    ctx.ptsArray[ i ].x = m_points[ i ].x;
    ctx.ptsArray[ i ].y = m_points[ i ].y;
    ctx.ptsArray[ i ].z = m_points[ i ].z;
  }
  ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
}


/////////////////////// D3dTriangleNormalRecord ////////////////////////

D3dTriangleNormalRecord::D3dTriangleNormalRecord(OdD3DVertexNormalCol* pPts)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTriangleNormalRecord\n");
#endif
  memcpy(points, pPts, 3 * sizeof(OdD3DVertexNormalCol));
}

void D3dTriangleNormalRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if(ctx.reverseNormals())
  {
    OD_D3D_REVERSENORMAL(points[0]);
    OD_D3D_REVERSENORMAL(points[1]);
    OD_D3D_REVERSENORMAL(points[2]);
  }

  if(!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, 1, points, OdD3DVertexNormalCol);
  }
  else
  {
    ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, 1, CORDATA(OdD3DVertexNormalCol, D3DPT_TRIANGLELIST, points, 1, ctx), sizeof(OdD3DVertexNormalCol));
  }

  if(ctx.reverseNormals())
  {
    OD_D3D_REVERSENORMAL(points[0]);
    OD_D3D_REVERSENORMAL(points[1]);
    OD_D3D_REVERSENORMAL(points[2]);
  }
}


void D3dTriangleNormalRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  ctx.ptsArray.resize( 3 );
  for( OdInt32 i = 0; i < 3; ++i )
  {
    ctx.ptsArray[ i ].x = points[ i ].position.x;
    ctx.ptsArray[ i ].y = points[ i ].position.y;
    ctx.ptsArray[ i ].z = points[ i ].position.z;
  }
  ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
}


/////////////////////// D3dTriangleNormalTextureRecord ////////////////////////

D3dTriangleNormalTextureRecord::D3dTriangleNormalTextureRecord(OdD3DVertexNormalTexCol* pPts)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTriangleNormalTextureRecord()\n");
#endif
  memcpy(points, pPts, 3 * sizeof(OdD3DVertexNormalTexCol));
}

void D3dTriangleNormalTextureRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if(ctx.reverseNormals())
  {
    OD_D3D_REVERSENORMAL(points[0]);
    OD_D3D_REVERSENORMAL(points[1]);
    OD_D3D_REVERSENORMAL(points[2]);
  }
  DWORD prevShader = ctx.stateManager().GetFVF();
  ctx.stateManager().SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1);
  if (!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, 1, points, OdD3DVertexNormalTexCol);
  }
  else
  {
    ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, 1, CORDATA(OdD3DVertexNormalTexCol, D3DPT_TRIANGLELIST, points, 1, ctx), sizeof(OdD3DVertexNormalTexCol), 1);
  }
  ctx.stateManager().SetFVF(prevShader);
  if(ctx.reverseNormals())
  {
    OD_D3D_REVERSENORMAL(points[0]);
    OD_D3D_REVERSENORMAL(points[1]);
    OD_D3D_REVERSENORMAL(points[2]);
  }
}


void D3dTriangleNormalTextureRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  ctx.ptsArray.resize( 3 );
  for( OdInt32 i = 0; i < 3; ++i )
  {
    ctx.ptsArray[ i ].x = points[ i ].position.x;
    ctx.ptsArray[ i ].y = points[ i ].position.y;
    ctx.ptsArray[ i ].z = points[ i ].position.z;
  }
  ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
}

/////////////////////// D3dPackedTriangleRecord ////////////////////////

D3dPackedTriangleRecord::D3dPackedTriangleRecord(OdInt32 nInitialID, OdInt32 numTriangles)
  : m_nInitialID(nInitialID)
  , m_nbTriangles(numTriangles)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedTriangleRecord(%d, %d)\n", nInitialID, numTriangles);
#endif
}

void D3dPackedTriangleRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if(!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, m_nbTriangles, ctx.vtPtr1().m_pVC + m_nInitialID, OdD3DVertexCol);
  }
  else
  {
    ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, (UINT)m_nbTriangles, CORDATA(OdD3DVertexCol, D3DPT_TRIANGLELIST, ctx.vtPtr1().m_pVC + m_nInitialID, m_nbTriangles, ctx), sizeof(OdD3DVertexCol));
  }
}


void D3dPackedTriangleRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + m_nInitialID;
  ctx.ptsArray.resize( 3 );
  for( OdInt32 i = 0; i < m_nbTriangles; ++i )
  {
    for( OdInt32 j = 0; j < 3; j++ )
    {
      ctx.ptsArray[ j ].x = pt[ 3*i + j ].x;
      ctx.ptsArray[ j ].y = pt[ 3*i + j ].y;
      ctx.ptsArray[ j ].z = pt[ 3*i + j ].z;
    }
    ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
  }
}


D3dPackedTriangleRecord* D3dPackedTriangleRecord::createObject(OdInt32 nInitialID, OdInt32 numTriangles)
{
  return new D3dPackedTriangleRecord(nInitialID, numTriangles);
}

/////////////////////// D3dPackedTriangleRecordWSel ////////////////////////

D3dPackedTriangleRecordWSel::D3dPackedTriangleRecordWSel(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
  : D3dPackedTriangleRecord(nInitialID, numTriangles)
  , m_nInitialSelMark(nInitialSelMark)
  , m_nbSelMarks(nbSelMarks)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedTriangleRecordWSel(%d, %d)\n", nInitialSelMark, nbSelMarks);
#endif
}

void D3dPackedTriangleRecordWSel::play(const D3dMetafilePlayContext& ctx) const
{
  if (ctx.selMarks() != NULL)
  {
    const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbTriangles * 3 + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( ctx.isDisplayWithHighlight() )
        {
          if(!ctx.vectorizer()->isHighlighted())
          {
            DRAWPRIMITIVE(D3DPT_TRIANGLELIST, ctDiff / 3, ctx.vtPtr1().m_pVC + nFirst, OdD3DVertexCol);
          }
          else
          {
            ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, (UINT)(ctDiff / 3), CORDATA(OdD3DVertexCol, D3DPT_TRIANGLELIST, ctx.vtPtr1().m_pVC + nFirst, ctDiff / 3, ctx), sizeof(OdD3DVertexCol));
          }
        }
      }
      if (nMarks)
      {
        ctx.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedTriangleRecord::play(ctx);
}


void D3dPackedTriangleRecordWSel::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if (pContext.selMarks() != NULL && ctx.bCheckMarkFlag)
  {
    const OdD3DSelMark *pMarks = pContext.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbTriangles * 3 + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( pContext.isSelectWithHighlight() )
        {
          const OdD3DVertexCol* pt = pContext.vtPtr1().m_pVC + nFirst;
          ctx.ptsArray.resize( 3 );
          for( OdInt32 i = 0; i < ctDiff / 3; ++i )
          {
            for( OdInt32 j = 0; j < 3; j++ )
            {
              ctx.ptsArray[ j ].x = pt[ 3*i + j ].x;
              ctx.ptsArray[ j ].y = pt[ 3*i + j ].y;
              ctx.ptsArray[ j ].z = pt[ 3*i + j ].z;
            }
            ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
          }
        }
      }
      if (nMarks)
      {
        pContext.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedTriangleRecord::play4Select(pContext, ctx);
}


D3dPackedTriangleRecordWSel* D3dPackedTriangleRecordWSel::createObject(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
{
  return new D3dPackedTriangleRecordWSel(nInitialID, numTriangles, nInitialSelMark, nbSelMarks);
}

/////////////////////// D3dPackedTriangleNormalRecord ////////////////////////

D3dPackedTriangleNormalRecord::D3dPackedTriangleNormalRecord(OdInt32 nInitialID, OdInt32 numTriangles)
  : m_nInitialID(nInitialID)
  , m_nbTriangles(numTriangles)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedTriangleNormalRecord(%d, %d)\n", nInitialID, numTriangles);
#endif
}

void D3dPackedTriangleNormalRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  if(!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, m_nbTriangles, ctx.vtPtr1().m_pVNC + m_nInitialID, OdD3DVertexNormalCol);
  }
  else
  {
    ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, (UINT)m_nbTriangles, CORDATA(OdD3DVertexNormalCol, D3DPT_TRIANGLELIST, ctx.vtPtr1().m_pVNC + m_nInitialID, m_nbTriangles, ctx), sizeof(OdD3DVertexNormalCol));
  }
}


void D3dPackedTriangleNormalRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  const OdD3DVertexNormalCol* pt = pContext.vtPtr1().m_pVNC + m_nInitialID;
  ctx.ptsArray.resize( 3 );
  for( OdInt32 i = 0; i < m_nbTriangles; ++i )
  {
    for( OdInt32 j = 0; j < 3; j++ )
    {
      ctx.ptsArray[ j ].x = pt[ 3*i + j ].position.x;
      ctx.ptsArray[ j ].y = pt[ 3*i + j ].position.y;
      ctx.ptsArray[ j ].z = pt[ 3*i + j ].position.z;
    }
    ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
  }
}


D3dPackedTriangleNormalRecord* D3dPackedTriangleNormalRecord::createObject(OdInt32 nInitialID, OdInt32 numTriangles)
{
  return new D3dPackedTriangleNormalRecord(nInitialID, numTriangles);
}

/////////////////////// D3dPackedTriangleNormalRecordWSel ////////////////////////

D3dPackedTriangleNormalRecordWSel::D3dPackedTriangleNormalRecordWSel(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
  : D3dPackedTriangleNormalRecord(nInitialID, numTriangles)
  , m_nInitialSelMark(nInitialSelMark)
  , m_nbSelMarks(nbSelMarks)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedTriangleNormalRecordWSel(%d, %d)\n", nInitialSelMark, nbSelMarks);
#endif
}

void D3dPackedTriangleNormalRecordWSel::play(const D3dMetafilePlayContext& ctx) const
{
  if (ctx.selMarks() != NULL)
  {
    const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbTriangles * 3 + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( ctx.isDisplayWithHighlight() )
        {
          if(!ctx.vectorizer()->isHighlighted())
          {
            DRAWPRIMITIVE(D3DPT_TRIANGLELIST, ctDiff / 3, ctx.vtPtr1().m_pVNC + nFirst, OdD3DVertexNormalCol);
          }
          else
          {
            ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, (UINT)(ctDiff / 3), CORDATA(OdD3DVertexNormalCol, D3DPT_TRIANGLELIST, ctx.vtPtr1().m_pVNC + nFirst, ctDiff / 3, ctx), sizeof(OdD3DVertexNormalCol));
          }
        }
      }
      if (nMarks)
      {
        ctx.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedTriangleNormalRecord::play(ctx);
}


void D3dPackedTriangleNormalRecordWSel::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if (pContext.selMarks() != NULL && ctx.bCheckMarkFlag )
  {
    const OdD3DSelMark *pMarks = pContext.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbTriangles * 3 + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( pContext.isSelectWithHighlight() )
        {
          const OdD3DVertexNormalCol* pt = pContext.vtPtr1().m_pVNC + nFirst;
          ctx.ptsArray.resize( 3 );
          for( OdInt32 i = 0; i < ctDiff / 3; ++i )
          {
            for( OdInt32 j = 0; j < 3; j++ )
            {
              ctx.ptsArray[ j ].x = pt[ 3*i + j ].position.x;
              ctx.ptsArray[ j ].y = pt[ 3*i + j ].position.y;
              ctx.ptsArray[ j ].z = pt[ 3*i + j ].position.z;
            }
            ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
          }
        }
      }
      if (nMarks)
      {
        pContext.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedTriangleNormalRecord::play4Select(pContext, ctx);
}


D3dPackedTriangleNormalRecordWSel* D3dPackedTriangleNormalRecordWSel::createObject(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
{
  return new D3dPackedTriangleNormalRecordWSel(nInitialID, numTriangles, nInitialSelMark, nbSelMarks);
}

/////////////////////// D3dPackedTriangleNormalTextureRecord ////////////////////////

D3dPackedTriangleNormalTextureRecord::D3dPackedTriangleNormalTextureRecord(OdInt32 nInitialID, OdInt32 numTriangles)
  : m_nbTriangles(numTriangles)
  , m_nInitialID(nInitialID)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedTriangleNormalTextureRecord(%d, %d)\n", nInitialID, numTriangles);
#endif
}

void D3dPackedTriangleNormalTextureRecord::play(const D3dMetafilePlayContext& ctx) const
{
  if( !ctx.isDisplayWithHighlight() ) return;
  DWORD prevShader = ctx.stateManager().GetFVF();
  ctx.stateManager().SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1);
  if (!ctx.vectorizer()->isHighlighted())
  {
    DRAWPRIMITIVE(D3DPT_TRIANGLELIST, m_nbTriangles, ctx.vtPtr2().m_pVNTC + m_nInitialID, OdD3DVertexNormalTexCol);
  }
  else
  {
    ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, (UINT)m_nbTriangles, CORDATA(OdD3DVertexNormalTexCol, D3DPT_TRIANGLELIST, ctx.vtPtr2().m_pVNTC + m_nInitialID, m_nbTriangles, ctx), sizeof(OdD3DVertexNormalTexCol), 1);
  }
  ctx.stateManager().SetFVF(prevShader);
}


void D3dPackedTriangleNormalTextureRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( !pContext.isSelectWithHighlight() ) return;
  const OdD3DVertexNormalTexCol* pt = pContext.vtPtr2().m_pVNTC + m_nInitialID;
  ctx.ptsArray.resize( 3 );
  for( OdInt32 i = 0; i < m_nbTriangles; ++i )
  {
    for( OdInt32 j = 0; j < 3; j++ )
    {
      ctx.ptsArray[ j ].x = pt[ 3*i + j ].position.x;
      ctx.ptsArray[ j ].y = pt[ 3*i + j ].position.y;
      ctx.ptsArray[ j ].z = pt[ 3*i + j ].position.z;
    }
    ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
  }
}


D3dPackedTriangleNormalTextureRecord* D3dPackedTriangleNormalTextureRecord::createObject(OdInt32 nInitialID, OdInt32 numTriangles)
{
  return new D3dPackedTriangleNormalTextureRecord(nInitialID, numTriangles);
}

/////////////////////// D3dPackedTriangleNormalTextureRecordWSel ////////////////////////

D3dPackedTriangleNormalTextureRecordWSel::D3dPackedTriangleNormalTextureRecordWSel(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
  : D3dPackedTriangleNormalTextureRecord(nInitialID, numTriangles)
  , m_nInitialSelMark(nInitialSelMark)
  , m_nbSelMarks(nbSelMarks)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dPackedTriangleNormalTextureRecordWSel(%d, %d)\n", nInitialSelMark, nbSelMarks);
#endif
}

void D3dPackedTriangleNormalTextureRecordWSel::play(const D3dMetafilePlayContext& ctx) const
{
  if (ctx.selMarks() != NULL)
  {
    const OdD3DSelMark *pMarks = ctx.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbTriangles * 3 + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( ctx.isDisplayWithHighlight() )
        {
          DWORD prevShader = ctx.stateManager().GetFVF();
          ctx.stateManager().SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1);
          if (!ctx.vectorizer()->isHighlighted())
          {
            DRAWPRIMITIVE(D3DPT_TRIANGLELIST, ctDiff / 3, ctx.vtPtr2().m_pVNTC + nFirst, OdD3DVertexNormalTexCol);
          }
          else
          {
            ctx.renderHighlightedTriangle(D3DPT_TRIANGLELIST, (UINT)(ctDiff / 3), CORDATA(OdD3DVertexNormalTexCol, D3DPT_TRIANGLELIST, ctx.vtPtr2().m_pVNTC + nFirst, ctDiff / 3, ctx), sizeof(OdD3DVertexNormalTexCol), 1);
          }
          ctx.stateManager().SetFVF(prevShader);
        }
      }
      if (nMarks)
      {
        ctx.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedTriangleNormalTextureRecord::play(ctx);
}


void D3dPackedTriangleNormalTextureRecordWSel::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if (pContext.selMarks() != NULL && ctx.bCheckMarkFlag )
  {
    const OdD3DSelMark *pMarks = pContext.selMarks() + m_nInitialSelMark;
    OdInt32 nMarks = m_nbSelMarks;
    OdInt32 nFirst = m_nInitialID;
    OdInt32 ctDiff;
    do
    {
      if (nMarks)
      {
        ctDiff = pMarks->uFrom - nFirst;
        if (ctDiff < 0)
          ctDiff = 0;
      }
      else
        ctDiff = (m_nbTriangles * 3 + m_nInitialID) - nFirst;
      if (ctDiff)
      {
        if( pContext.isSelectWithHighlight() )
        {
          const OdD3DVertexNormalTexCol* pt = pContext.vtPtr2().m_pVNTC + nFirst;
          ctx.ptsArray.resize( 3 );
          for( OdInt32 i = 0; i < ctDiff / 3; ++i )
          {
            for( OdInt32 j = 0; j < 3; j++ )
            {
              ctx.ptsArray[ j ].x = pt[ 3*i + j ].position.x;
              ctx.ptsArray[ j ].y = pt[ 3*i + j ].position.y;
              ctx.ptsArray[ j ].z = pt[ 3*i + j ].position.z;
            }
            ctx.pGeometry->polygonProc( 3, ctx.ptsArray.asArrayPtr() );
          }
        }
      }
      if (nMarks)
      {
        pContext.setSelectionMarker(pMarks->gsMark);
        nFirst += ctDiff;
        pMarks++;
      }
      nMarks--;
    }
    while (nMarks != -1);
  } else
    D3dPackedTriangleNormalTextureRecord::play4Select( pContext, ctx );
}


D3dPackedTriangleNormalTextureRecordWSel* D3dPackedTriangleNormalTextureRecordWSel::createObject(OdInt32 nInitialID, OdInt32 numTriangles, OdInt32 nInitialSelMark, OdInt32 nbSelMarks)
{
  return new D3dPackedTriangleNormalTextureRecordWSel(nInitialID, numTriangles, nInitialSelMark, nbSelMarks);
}

//////////////////// ExGsDelayedMappingEntry ///////////////////////

struct ExGsDelayedMappingEntry : public OdGsBaseMaterialView::DelayCacheEntry
{
  OdUInt32 m_uStartID;
  OdUInt32 m_uLength;
  ExGiDirectXMetafilePtr m_pMetafile;
};

/////////////////////// D3dMetafilePackager ////////////////////////

const OdInt32 D3dMetafilePackager::MaxPolylineNoStrip = 12;
const OdInt32 D3dMetafilePackager::MaxPolylineNoStripPack = 32;

/** Pending metafile packager optimizations after several drawings tests:
  - 2D mode:
    - Some entities in 2D mode has following packed stack after processing:
      TraceRecs: D3dPackedTriangleRecord(0, 2)
      TraceRecs: D3dPackedPolylineRecord(6, 4)
      TraceRecs: D3dPackedTriangleRecord(14, 2)
      TraceRecs: D3dPackedPolylineRecord(20, 4)
      TraceRecs: D3dPackedTriangleRecord(28, 2)
      TraceRecs: D3dPackedPolylineRecord(34, 4)
      TraceRecs: D3dPackedTriangleRecord(42, 2)
      TraceRecs: D3dPackedPolylineRecord(48, 4)
      ...
      , typically if contain some shells/polygones. Rendering order in 2D mode is important, but if color
      of lines and triangles is not changed in that call's, packed triangles and polylines can be mixed in
      two records, before next color change.
  - 3D mode:
    - 3D solids use shell's with big count of triangles/vertices in one call. Currently each triangle packed as
      3 vertices of appropriate type. I think more effective pack big shells with large vertices count as
      indexed primitives (which can be packed too). Example: 282 vertices in shell with 396 indices in faces
      array as result contain 1188 vertices in triangles packet.
      One problem is face normals. Need duplication of vertices with various normals or summ normals in
      vertices. Flat shading can be enable on D3Ddevice for flat shaded modes.
**/

D3dMetafilePackager::D3dMetafilePackager()
  : m_bInitialized(false)
  , m_bDynamicSubEntHlt(false)
  , m_bNestingMode(false)
  , m_pVectorizer(NULL)
  , m_d3dDevice(NULL)
  , m_pCurDelMapEntry(NULL)
  , m_selectionFlag( 0 )
{
  clear();
}

D3dMetafilePackager::~D3dMetafilePackager()
{
  clear();
}

void D3dMetafilePackager::clear()
{
  for (OdInt32 nPackEntry = kLSEntry; nPackEntry <= kLTEntry; nPackEntry++)
  {
    m_packEntry[nPackEntry].m_arrayType = kUnassignedArray;
    m_packEntry[nPackEntry].m_packPrimitive = kRecordPrim;
    m_packEntry[nPackEntry].m_nInitialID = m_packEntry[nPackEntry].m_nVertices = 0;
    m_packEntry[nPackEntry].m_bLastHLTState = false;
    m_packEntry[nPackEntry].m_gsPrevMarker = kNullSubentIndex;
    m_packEntry[nPackEntry].m_transparency = 0;
    m_packEntry[nPackEntry].m_lwdStyle.m_bCurLwdUsesDef = true;
    m_packEntry[nPackEntry].m_bNoColorMod = false;
    m_packEntry[nPackEntry].m_uMtfCount = 0;
    m_packEntry[nPackEntry].m_selectionFlag = 0;
  }
  //m_LSArray.clear();
  m_LSArray.setPhysicalLength(0);
  //m_VCArray.clear();
  m_VCArray.setPhysicalLength(0);
  //m_VNCArray.clear();
  m_VNCArray.setPhysicalLength(0);
  //m_VNTCArray.clear();
  m_VNTCArray.setPhysicalLength(0);
}

void D3dMetafilePackager::beginMetafile(OdGsBaseVectorizer* pVectorizer, IDirect3DDevice *d3dDevice, ExGiDirectXMetafilePtr pMetafile)
{
  //ODA_ASSERT(!m_bInitialized); // Correct situation
  if (m_bInitialized)
  {
    endMetafile();
  }
  m_pVectorizer = pVectorizer;
  m_d3dDevice = d3dDevice;
  m_pMetafile = pMetafile;
  ODA_ASSERT(!m_pMetafile.isNull());

  if (!m_bNestingMode)
  {
    if (!m_bDynamicSubEntHlt)
    {
      m_bControlHLT = !pVectorizer->isHighlighted();
      m_bLastHLT = !m_bControlHLT;
      if (m_bLastHLT)
      {
        if (m_bLastHLT && pVectorizer->currentHighlightBranch() && !pVectorizer->currentHighlightBranch()->markers().isEmpty())
        {
          m_bControlHLT = !m_bControlHLT;
          m_bLastHLT = !m_bLastHLT;
        }
        else
        {
          for (OdUInt32 i = 0; i < kLastPackEntry; i++)
          {
            m_packEntry[i].m_bLastHLTState = true;
          }
        }
      }
    }
    else
      m_gsLastMarker = kNullSubentIndex;

    m_transparency = 0;
    m_lwdStyle.m_bCurLwdUsesDef = true;
    m_bNoColorMod = false;
  }
  m_bInitialized = true;
}

void D3dMetafilePackager::endMetafile()
{
  ODA_ASSERT(m_bInitialized);
  flushDelayMapping();
  flush(kLSEntry);
  flush(kNLEntry);
  flush(kLTEntry);

  if (!m_bNestingMode)
  {
    if (!m_bDynamicSubEntHlt)
    {
      // Restore highlight states
      PackEntryType lastEntry;
      if (lastDataEntry(lastEntry))
      {
        if (m_bControlHLT)
        {
          for (OdUInt32 i = 0; i < (OdUInt32)lastEntry; i++)
          {
            if (m_packEntry[i].m_bLastHLTState)
            {
              addRecord((PackEntryType)i, kEndHighlightingRec, new D3dEndHighlightingRecord());
            }
          }
        }
        else
        {
          for (OdUInt32 i = 0; i < (OdUInt32)lastEntry; i++)
          {
            if (!m_packEntry[i].m_bLastHLTState)
            {
              addRecord((PackEntryType)i, kBeginHighlightingRec, new D3dBeginHighlightingRecord());
            }
          }
        }
        if (m_packEntry[lastEntry].m_bLastHLTState != m_bLastHLT)
        {
          if (m_bLastHLT)
          {
            addRecord(lastEntry, kBeginHighlightingRec, new D3dBeginHighlightingRecord());
          }
          else
          {
            addRecord(lastEntry, kEndHighlightingRec, new D3dBeginHighlightingRecord());
          }
        }
      }
    }

    if (m_packEntry[kLSEntry].m_bNoColorMod)
      addRecord(kLSEntry, kEndNoColorModRec, new D3dEndNoColorModRecord());
    if (m_packEntry[kNLEntry].m_bNoColorMod)
      addRecord(kNLEntry, kEndNoColorModRec, new D3dEndNoColorModRecord());
    if (m_packEntry[kLTEntry].m_bNoColorMod)
      addRecord(kLTEntry, kEndNoColorModRec, new D3dEndNoColorModRecord());

    if (m_packEntry[kLSEntry].m_transparency != 0)
      addRecord(kLSEntry, kTransparencyRec, new D3dTransparencyRecord(0));
    if (m_packEntry[kNLEntry].m_transparency != 0)
      addRecord(kNLEntry, kTransparencyRec, new D3dTransparencyRecord(0));
    if (m_packEntry[kLTEntry].m_transparency != 0)
      addRecord(kLTEntry, kTransparencyRec, new D3dTransparencyRecord(0));

    if (!m_packEntry[kLSEntry].m_lwdStyle.m_bCurLwdUsesDef)
      addRecord(kLSEntry, kLwdStyleRec, new D3dLwdStyleRecord());
    if (!m_packEntry[kNLEntry].m_lwdStyle.m_bCurLwdUsesDef)
      addRecord(kNLEntry, kLwdStyleRec, new D3dLwdStyleRecord());
    if (!m_packEntry[kLTEntry].m_lwdStyle.m_bCurLwdUsesDef)
      addRecord(kLTEntry, kLwdStyleRec, new D3dLwdStyleRecord());
  }

  if( m_packEntry[kLSEntry].m_selectionFlag != 0 )
    addRecord(kLSEntry, kSelectionModeRec, new D3dSelectionModeRecord( 0 ) );
  if( m_packEntry[kNLEntry].m_selectionFlag != 0 )
    addRecord(kNLEntry, kSelectionModeRec, new D3dSelectionModeRecord( 0 ) );
  if( m_packEntry[kLTEntry].m_selectionFlag != 0 )
    addRecord(kLTEntry, kSelectionModeRec, new D3dSelectionModeRecord( 0 ) );

  if (!m_LSArray.empty())
  {
    m_LSArray.setPhysicalLength(m_LSArray.size());
    m_pMetafile->setLineSegArray(m_LSArray);
  }
  if (!m_VCArray.empty())
  {
    m_VCArray.setPhysicalLength(m_VCArray.size());
    m_pMetafile->setNoLightTriArray(m_VCArray);
  }
  if (!m_VNCArray.empty())
  {
    m_VNCArray.setPhysicalLength(m_VNCArray.size());
    m_pMetafile->setLightTriArray(m_VNCArray);
  }
  if (!m_VNTCArray.empty())
  {
    m_VNTCArray.setPhysicalLength(m_VNTCArray.size());
    m_pMetafile->setTexturedTriArray(m_VNTCArray);
  }
  if (m_bDynamicSubEntHlt && !m_pMetafile->getSelMarksArray().empty() && !m_bNestingMode)
  {
    m_pMetafile->getSelMarksArray().setPhysicalLength(m_pMetafile->getSelMarksArray().size());
  }
  m_bInitialized = false;
  clear();
}

void D3dMetafilePackager::runNestingMode()
{
  m_bNestingMode = true;
  m_bInitialized = false;
  clear();
}

void D3dMetafilePackager::stopNestingMode()
{
  m_bNestingMode = false;
}

void D3dMetafilePackager::flush(PackEntryType entryType)
{
  ODA_ASSERT(m_bInitialized);
  PackEntry *entry = m_packEntry + entryType;
  D3dRecord *pRec = NULL;
  if (entry->m_arrayType != kUnassignedArray && entry->m_packPrimitive != kRecordPrim)
  {
    bool bSelMarks = false;
    if (m_bDynamicSubEntHlt && !entry->m_SelMarks.empty())
      bSelMarks = true;
    if (entry->m_packPrimitive == kPolylinePrim)
    {
      if (bSelMarks)
        pRec = new D3dPackedPolylineRecordWSel(entry->m_nInitialID, entry->m_nVertices >> 1, m_pMetafile->getSelMarksArray().size(), entry->m_SelMarks.size());
      else
        pRec = new D3dPackedPolylineRecord(entry->m_nInitialID, entry->m_nVertices >> 1);
    }
    else if (entry->m_packPrimitive == kTrianglePrim)
    {
      ODA_ASSERT(entry->m_arrayType != kLSArray);
      switch (entry->m_arrayType)
      {
        case kVCArray:
          if (bSelMarks)
            pRec = new D3dPackedTriangleRecordWSel(entry->m_nInitialID, entry->m_nVertices / 3, m_pMetafile->getSelMarksArray().size(), entry->m_SelMarks.size());
          else
            pRec = new D3dPackedTriangleRecord(entry->m_nInitialID, entry->m_nVertices / 3);
        break;
        case kVNCArray:
          if (bSelMarks)
            pRec = new D3dPackedTriangleNormalRecordWSel(entry->m_nInitialID, entry->m_nVertices / 3, m_pMetafile->getSelMarksArray().size(), entry->m_SelMarks.size());
          else
            pRec = new D3dPackedTriangleNormalRecord(entry->m_nInitialID, entry->m_nVertices / 3);
        break;
        case kVNTCArray:
          if (bSelMarks)
            pRec = new D3dPackedTriangleNormalTextureRecordWSel(entry->m_nInitialID, entry->m_nVertices / 3, m_pMetafile->getSelMarksArray().size(), entry->m_SelMarks.size());
          else
            pRec = new D3dPackedTriangleNormalTextureRecord(entry->m_nInitialID, entry->m_nVertices / 3);
        break;
      }
    }
    else if (entry->m_packPrimitive == kPointPrim)
    {
      if (bSelMarks)
        pRec = new D3dPackedPointRecordWSel(entry->m_nInitialID, entry->m_nVertices, m_pMetafile->getSelMarksArray().size(), entry->m_SelMarks.size());
      else
        pRec = new D3dPackedPointRecord(entry->m_nInitialID, entry->m_nVertices);
    }
    else if ((entry->m_packPrimitive == kLwdPointPrim) || (entry->m_packPrimitive == kLwdPolylinePrim))
    {
      if (bSelMarks)
        pRec = D3dPackedLwdPolylineRecordWSel::createObject(entry->m_packPrimitive == kLwdPointPrim, entry->m_nInitialID, entry->m_nVertices >> 1, entry->m_lineWeight, m_pMetafile->getSelMarksArray().size(), entry->m_SelMarks.size());
      else
        pRec = D3dPackedLwdPolylineRecord::createObject(entry->m_packPrimitive == kLwdPointPrim, entry->m_nInitialID, entry->m_nVertices >> 1, entry->m_lineWeight);
    }
  }
  if (pRec != NULL)
  {
    switch (entryType)
    {
      case kLSEntry:
        m_pMetafile->appendLineSegsRecord(pRec);
      break;
      case kNLEntry:
        m_pMetafile->appendNoLightRecord(pRec);
      break;
      case kLTEntry:
        m_pMetafile->appendLightRecord(pRec);
      break;
    }
    entry->m_arrayType = kUnassignedArray;
    entry->m_packPrimitive = kRecordPrim;
    entry->m_nInitialID = entry->m_nVertices = 0;
    entry->m_uMtfCount++;
    if (m_bDynamicSubEntHlt && !entry->m_SelMarks.empty())
    {
      m_pMetafile->getSelMarksArray().append(entry->m_SelMarks);
      entry->m_SelMarks.resize(0);
    }
  }
}

void D3dMetafilePackager::addRecord(PackEntryType entryType, RecordType recType, D3dRecord* pRecord)
{
  ODA_ASSERT(m_bInitialized);
  if ((!m_bDynamicSubEntHlt && (recType != kBeginHighlightingRec && recType != kEndHighlightingRec)) ||
       (m_bDynamicSubEntHlt && (recType != kSelectionMarkerRec)))
    testHLTState(entryType);
  PackEntry *entry = m_packEntry + entryType;
  flush(entryType);
  switch (entryType)
  {
    case kLSEntry:
      m_pMetafile->appendLineSegsRecord(pRecord);
    break;
    case kNLEntry:
      m_pMetafile->appendNoLightRecord(pRecord);
    break;
    case kLTEntry:
      m_pMetafile->appendLightRecord(pRecord);
    break;
  }
  entry->m_arrayType = kUnassignedArray;
  entry->m_packPrimitive = kRecordPrim;
  entry->m_uMtfCount++;
}

void D3dMetafilePackager::addPoint(PackEntryType entryType, const OdGePoint3d& pos, DWORD color)
{
  ODA_ASSERT(m_bInitialized && entryType != kLTEntry);
  testHLTState(entryType);
  PackEntry *entry = m_packEntry + entryType;
  // Don't stop polyline packaging on point
  /*if (entry->m_packPrimitive == kPolylinePrim)
  {
    const OdGePoint3d polyPts[2] = { pos, pos };
    addPolyline(entryType, 2, polyPts, color);
    return;
  }*/
  //
  ArrayType aType = (entryType == kLSEntry) ? kLSArray : kVCArray;
  OdD3DVertexColArray *recArray = (entryType == kLSEntry) ? (&m_LSArray) : (&m_VCArray);
  bool appendData = (entry->m_arrayType == aType) && (entry->m_packPrimitive == kPointPrim);
  if (!appendData)
  {
    flush(entryType);
    entry->m_arrayType = aType;
    entry->m_packPrimitive = kPointPrim;
    entry->m_nInitialID = (OdInt32)recArray->size();
    entry->m_nVertices = 0;
  }
  OdD3DVertexCol vert;
  vert.x = dbl2float(pos.x);
  vert.y = dbl2float(pos.y);
  vert.z = dbl2float(pos.z);
  vert.color = color;
  recArray->append(vert);
  entry->m_nVertices++;
}

void D3dMetafilePackager::addPolyline(PackEntryType entryType, OdInt32 nPoints, const OdGePoint3d* points, DWORD color)
{
  ODA_ASSERT(m_bInitialized && entryType != kLTEntry);
  testHLTState(entryType);
  PackEntry *entry = m_packEntry + entryType;
  bool noStrip = (entry->m_packPrimitive != kPolylinePrim) ? (nPoints <= MaxPolylineNoStrip) : (nPoints <= MaxPolylineNoStripPack);
  if (!noStrip)
  {
    // Polyline record with stripping
    flush(entryType);
    OdD3DVertexColArray *recArray = (entryType == kLSEntry) ? (&m_LSArray) : (&m_VCArray);
    OdInt32 nInitialID = (OdInt32)recArray->size();
    recArray->resize(recArray->size() + nPoints);
    for (OdInt32 i = 0; i < nPoints; i++)
    {
      OdD3DVertexCol &outVert = recArray->at(nInitialID + i);
      const OdGePoint3d *inVert = points + i;
      outVert.x = dbl2float(inVert->x);
      outVert.y = dbl2float(inVert->y);
      outVert.z = dbl2float(inVert->z);
      outVert.color = color;
    }
    D3dRecord *pRec = new D3dPackedPolylineStripRecord(nInitialID, nPoints);;
    if (entryType == kLSEntry)
    {
      m_pMetafile->appendLineSegsRecord(pRec);
    }
    else
    {
      m_pMetafile->appendNoLightRecord(pRec);
    }
    entry->m_uMtfCount++;
  }
  else
  {
    ArrayType aType = (entryType == kLSEntry) ? kLSArray : kVCArray;
    OdD3DVertexColArray *recArray = (entryType == kLSEntry) ? (&m_LSArray) : (&m_VCArray);
    // Test is non-stripped polyline initialized
    bool appendData = (entry->m_arrayType == aType) && (entry->m_packPrimitive == kPolylinePrim);
    if (!appendData)
    {
      flush(entryType);
      entry->m_arrayType = aType;
      entry->m_packPrimitive = kPolylinePrim;
      entry->m_nInitialID = (OdInt32)recArray->size();
      entry->m_nVertices = 0;
    }
    // Now pack each polyline segment as separated line segment
    OdInt32 nNewPts = (nPoints << 1) - 2;
    OdInt32 nLines = nNewPts >> 1;
    OdInt32 nInitialID = (OdInt32)recArray->size();
    recArray->resize(recArray->size() + nNewPts);
    for (OdInt32 i = 0; i < nLines; i++)
    {
      OdD3DVertexCol &outVert1 = recArray->at(nInitialID + (i << 1));
      OdD3DVertexCol &outVert2 = recArray->at(nInitialID + (i << 1) + 1);
      const OdGePoint3d *inVert1 = points + i;
      const OdGePoint3d *inVert2 = points + i + 1;
      outVert1.x = dbl2float(inVert1->x);
      outVert1.y = dbl2float(inVert1->y);
      outVert1.z = dbl2float(inVert1->z);
      outVert2.x = dbl2float(inVert2->x);
      outVert2.y = dbl2float(inVert2->y);
      outVert2.z = dbl2float(inVert2->z);
      outVert2.color = outVert1.color = color;
    }
    entry->m_nVertices += nNewPts;
  }
}

void D3dMetafilePackager::addTriangle(const OdD3DVertexCol *pVertices)
{
  ODA_ASSERT(m_bInitialized);
  testHLTState(kNLEntry);
  PackEntry *entry = m_packEntry + kNLEntry;
  bool appendData = (entry->m_arrayType == kVCArray) && (entry->m_packPrimitive == kTrianglePrim);
  if (!appendData)
  {
    flush(kNLEntry);
    entry->m_arrayType = kVCArray;
    entry->m_packPrimitive = kTrianglePrim;
    entry->m_nInitialID = (OdInt32)m_VCArray.size();
    entry->m_nVertices = 0;
  }
  OdUInt32 nInitialID = m_VCArray.size();
  m_VCArray.resize(m_VCArray.size() + 3);
  memcpy(m_VCArray.asArrayPtr() + nInitialID, pVertices, sizeof(OdD3DVertexCol) * 3);
  entry->m_nVertices += 3;
}

void D3dMetafilePackager::addTriangle(bool bTwoSidesMode, const OdD3DVertexNormalCol *pVertices)
{
  ODA_ASSERT(m_bInitialized);
  testHLTState(kLTEntry);
  PackEntry *entry = m_packEntry + kLTEntry;
  bool appendData = (entry->m_arrayType == kVNCArray) && (entry->m_packPrimitive == kTrianglePrim);
  if (!appendData)
  {
    flush(kLTEntry);
    entry->m_arrayType = kVNCArray;
    entry->m_packPrimitive = kTrianglePrim;
    entry->m_nInitialID = (OdInt32)m_VNCArray.size();
    entry->m_nVertices = 0;
  }
  OdUInt32 nInitialID = m_VNCArray.size();
  m_VNCArray.resize(m_VNCArray.size() + ((bTwoSidesMode) ? 6 : 3));
  memcpy(m_VNCArray.asArrayPtr() + nInitialID, pVertices, sizeof(OdD3DVertexNormalCol) * 3);
  if (bTwoSidesMode)
  {
    OdD3DVertexNormalCol *pOutput = m_VNCArray.asArrayPtr() + nInitialID + 3;
    pOutput[0] = pVertices[2];
    OD_D3D_REVERSENORMAL(pOutput[0]);
    pOutput[1] = pVertices[1];
    OD_D3D_REVERSENORMAL(pOutput[1]);
    pOutput[2] = pVertices[0];
    OD_D3D_REVERSENORMAL(pOutput[2]);
    entry->m_nVertices += 3;
  }
  entry->m_nVertices += 3;
}

void D3dMetafilePackager::addTriangle(bool bTwoSidesMode, const OdD3DVertexNormalTexCol *pVertices)
{
  ODA_ASSERT(m_bInitialized);
  testHLTState(kLTEntry);
  PackEntry *entry = m_packEntry + kLTEntry;
  bool appendData = (entry->m_arrayType == kVNTCArray) && (entry->m_packPrimitive == kTrianglePrim);
  if (!appendData)
  {
    flush(kLTEntry);
    entry->m_arrayType = kVNTCArray;
    entry->m_packPrimitive = kTrianglePrim;
    entry->m_nInitialID = (OdInt32)m_VNTCArray.size();
    entry->m_nVertices = 0;
  }
  OdUInt32 nInitialID = m_VNTCArray.size();
  m_VNTCArray.resize(m_VNTCArray.size() + ((bTwoSidesMode) ? 6 : 3));
  memcpy(m_VNTCArray.asArrayPtr() + nInitialID, pVertices, sizeof(OdD3DVertexNormalTexCol) * 3);
  if (bTwoSidesMode)
  {
    OdD3DVertexNormalTexCol *pOutput = m_VNTCArray.asArrayPtr() + nInitialID + 3;
    pOutput[0] = pVertices[2];
    OD_D3D_REVERSENORMAL(pOutput[0]);
    pOutput[1] = pVertices[1];
    OD_D3D_REVERSENORMAL(pOutput[1]);
    pOutput[2] = pVertices[0];
    OD_D3D_REVERSENORMAL(pOutput[2]);
    entry->m_nVertices += 3;
  }
  entry->m_nVertices += 3;
}

void D3dMetafilePackager::addLwdPolyline(PackEntryType entryType, bool bPoints, OdInt32 nPoints, const OdGePoint3d* points, DWORD color, const D3dLwdSetting &lineWeight)
{
  ODA_ASSERT(m_bInitialized && entryType != kLTEntry);
  testHLTState(entryType);
  PackEntry *entry = m_packEntry + entryType;
  ArrayType aType = (entryType == kLSEntry) ? kLSArray : kVCArray;
  OdD3DVertexColArray *recArray = (entryType == kLSEntry) ? (&m_LSArray) : (&m_VCArray);
  // Test is lwd polyline initialized
  const PackPrimitive nPrim = (bPoints) ? kLwdPointPrim : kLwdPolylinePrim;
  bool appendData = (entry->m_arrayType == aType) && (entry->m_packPrimitive == nPrim) && (entry->m_lineWeight == lineWeight);
  if (!appendData)
  {
    flush(entryType);
    entry->m_arrayType = aType;
    entry->m_packPrimitive = nPrim;
    entry->m_lineWeight = lineWeight;
    entry->m_nInitialID = (OdInt32)recArray->size();
    entry->m_nVertices = 0;
  }
  // Now pack each polyline segment as separated line segment
  OdInt32 nNewPts = (nPoints << 1) - 2;
  OdInt32 nLines = nNewPts >> 1;
  OdInt32 nInitialID = (OdInt32)recArray->size();
  recArray->resize(recArray->size() + nNewPts);
  for (OdInt32 i = 0; i < nLines; i++)
  {
    OdD3DVertexCol &outVert1 = recArray->at(nInitialID + (i << 1));
    OdD3DVertexCol &outVert2 = recArray->at(nInitialID + (i << 1) + 1);
    const OdGePoint3d *inVert1 = points + i;
    const OdGePoint3d *inVert2 = points + i + 1;
    outVert1.x = dbl2float(inVert1->x);
    outVert1.y = dbl2float(inVert1->y);
    outVert1.z = dbl2float(inVert1->z);
    outVert2.x = dbl2float(inVert2->x);
    outVert2.y = dbl2float(inVert2->y);
    outVert2.z = dbl2float(inVert2->z);
    outVert2.color = outVert1.color = color;
  }
  entry->m_nVertices += nNewPts;
}

bool D3dMetafilePackager::curHLTState(PackEntryType entryType) const
{
  return m_packEntry[entryType].m_bLastHLTState;
}

OdGsMarker D3dMetafilePackager::curSelectionMarker(PackEntryType entryType) const
{
  return m_packEntry[entryType].m_gsPrevMarker;
}

bool D3dMetafilePackager::isLastDataEntry(PackEntryType entryType) const
{
  for (OdUInt32 j = (OdUInt32)entryType + 1; j < (OdUInt32)kLastPackEntry; j++)
  {
    if (m_packEntry[j].m_uMtfCount > 0)
      return false;
  }
  return true;
}

bool D3dMetafilePackager::lastDataEntry(PackEntryType &entryType) const
{
  for (OdUInt32 j = (OdUInt32)kLastPackEntry; j > 0; j--)
  {
    if (m_packEntry[j - 1].m_uMtfCount > 0)
    {
      entryType = (PackEntryType)(j - 1);
      return true;
    }
  }
  return false;
}

const OdGsBaseVectorizer *D3dMetafilePackager::metafileVectorizer() const
{
  return m_pVectorizer;
}

void D3dMetafilePackager::startDelayMapping(void *pEntry)
{
  flushDelayMapping();
  ODA_ASSERT(m_bInitialized);
  m_pCurDelMapEntry = (ExGsDelayedMappingEntry*)pEntry;
  m_pCurDelMapEntry->m_uStartID = m_VNTCArray.size();
  m_pCurDelMapEntry->m_uLength = 0;
  m_pCurDelMapEntry->m_pMetafile = m_pMetafile;
}

void D3dMetafilePackager::addTriangleToDelayMapping(bool bTwoSidesMode)
{
  if (!m_pCurDelMapEntry) return;
  ODA_ASSERT(m_pCurDelMapEntry != NULL);
  m_pCurDelMapEntry->m_uLength += (bTwoSidesMode) ? 6 : 3;
}

void D3dMetafilePackager::flushDelayMapping()
{
  m_pCurDelMapEntry = NULL;
  //if (m_pCurDelMapEntry)
  //{
  //  m_pCurDelMapEntry->m_uLength = m_VNTCArray.size() - m_pCurDelMapEntry->m_uStartID;
  //  m_pCurDelMapEntry = NULL;
  //}
}

bool D3dMetafilePackager::isInitialized() const
{
  return m_bInitialized;
}

void D3dMetafilePackager::testHLTState(PackEntryType entryType)
{
  PackEntry *entry = m_packEntry + entryType;
  if (!m_bDynamicSubEntHlt)
  {
    m_bLastHLT = m_pVectorizer->isHighlighted();
    if (entry->m_bLastHLTState != m_bLastHLT)
    {
      entry->m_bLastHLTState = m_pVectorizer->isHighlighted();
      D3dRecord *pRec;
      if (entry->m_bLastHLTState)
        pRec = new D3dBeginHighlightingRecord();
      else
        pRec = new D3dEndHighlightingRecord();
      addRecord(entryType, (entry->m_bLastHLTState) ? kBeginHighlightingRec : kEndHighlightingRec, pRec);
      entry->m_uMtfCount++;
    }
  }
  else
  {
    if (!m_pVectorizer->gsWriter().gsModel())
      return;
    OdGsMarker gsMarker = m_pVectorizer->selectionMarker();
    if (m_gsLastMarker != gsMarker)
    {
      if (isNestedPersistent(m_pVectorizer->currentGiPath()) && !m_pVectorizer->view().baseDevice()->supportBlocks())
      {
        gsMarker = kNullSubentIndex;
      }
    }
    m_gsLastMarker = gsMarker;
    if (entry->m_gsPrevMarker != gsMarker)
    {
      entry->m_gsPrevMarker = gsMarker;
      bool noDrop = false;
      int xx;
      for (xx = 0; xx < kLastPackEntry; xx++)
      {
        if (m_packEntry[xx].m_packPrimitive != kRecordPrim)
        {
          noDrop = true;
          break;
        }
      }
      if (!noDrop)
      {
        D3dRecord *pRec = new D3dSelectionMarkerRecord(entry->m_gsPrevMarker);
        addRecord(entryType, kSelectionMarkerRec, pRec);
        entry->m_uMtfCount++;
      }
      {
        for (xx = 0; xx < kLastPackEntry; xx++)
        {
          OdD3DSelMark selMark;
          selMark.gsMark = gsMarker;
          switch (m_packEntry[xx].m_arrayType)
          {
            case kLSArray:
              selMark.uFrom = m_LSArray.size();
            break;
            case kVCArray:
              selMark.uFrom = m_VCArray.size();
            break;
            case kVNCArray:
              selMark.uFrom = m_VNCArray.size();
            break;
            case kVNTCArray:
              selMark.uFrom = m_VNTCArray.size();
            break;
            default:
              selMark.uFrom = 0;
          }
          if (m_packEntry[xx].m_SelMarks.empty() ||
              m_packEntry[xx].m_SelMarks.last().uFrom != selMark.uFrom)
              m_packEntry[xx].m_SelMarks.append(selMark);
            else
              m_packEntry[xx].m_SelMarks.last() = selMark;
        }
      }
    }
  }
  if (entry->m_bNoColorMod != m_bNoColorMod)
  {
    entry->m_bNoColorMod = m_bNoColorMod;
    D3dRecord *pRec;
    if (m_bNoColorMod) pRec = new D3dBeginNoColorModRecord();
    else pRec = new D3dEndNoColorModRecord();
    addRecord(entryType, (m_bNoColorMod) ? kBeginNoColorModRec : kEndNoColorModRec, pRec);
    entry->m_uMtfCount++;
  }
  if (entry->m_transparency != m_transparency && entryType != kLSEntry)
  {
    entry->m_transparency = m_transparency;
    D3dRecord *pRec = new D3dTransparencyRecord(entry->m_transparency);
    addRecord(entryType, kTransparencyRec, pRec);
    entry->m_uMtfCount++;
  }
  if (entry->m_lwdStyle.m_bCurLwdUsesDef != m_lwdStyle.m_bCurLwdUsesDef ||
      (!m_lwdStyle.m_bCurLwdUsesDef &&
      (m_lwdStyle.m_curLwdLes != entry->m_lwdStyle.m_curLwdLes || m_lwdStyle.m_curLwdLjs != entry->m_lwdStyle.m_curLwdLjs)))
  {
    entry->m_lwdStyle.m_bCurLwdUsesDef = m_lwdStyle.m_bCurLwdUsesDef;
    entry->m_lwdStyle.m_curLwdLes = m_lwdStyle.m_curLwdLes;
    entry->m_lwdStyle.m_curLwdLjs = m_lwdStyle.m_curLwdLjs;
    D3dRecord *pRec = (m_lwdStyle.m_bCurLwdUsesDef) ? new D3dLwdStyleRecord() : new D3dLwdStyleRecord(m_lwdStyle.m_curLwdLes, m_lwdStyle.m_curLwdLjs);
    addRecord(entryType, kLwdStyleRec, pRec);
    entry->m_uMtfCount++;
  }
  if( entry->m_selectionFlag != m_selectionFlag )
  {
    entry->m_selectionFlag = m_selectionFlag;
    addRecord(entryType, kSelectionModeRec, new D3dSelectionModeRecord( m_selectionFlag ) );
  }
}

#ifdef DEBUG

D3dMetafilePackager::Stats D3dMetafilePackager::statInfo() const
{
  Stats stats;
  stats.m_nLSVertices = m_LSArray.size();
  stats.m_nVCVertices = m_VCArray.size();
  stats.m_nVNCVertices = m_VNCArray.size();
  stats.m_nVNTCVertices = m_VNTCArray.size();
  stats.m_nLSMetafiles = m_packEntry[kLSEntry].m_uMtfCount;
  stats.m_nNLMetafiles = m_packEntry[kNLEntry].m_uMtfCount;
  stats.m_nLTMetafiles = m_packEntry[kLTEntry].m_uMtfCount;
  return stats;
}

#endif

void D3dMetafilePackager::setupArrays(bool b4Cache)
{
  if (!b4Cache)
  {
    const OdInt32 nGrow = 02000000 / 4;
    m_LSArray.setGrowLength(nGrow / sizeof(OdD3DVertexCol));
    m_VCArray.setGrowLength(nGrow / sizeof(OdD3DVertexCol));
    m_VNCArray.setGrowLength(nGrow / sizeof(OdD3DVertexNormalCol));
    m_VNTCArray.setGrowLength(nGrow / sizeof(OdD3DVertexNormalTexCol));
  }
  else
  {
    m_LSArray.setGrowLength(-100);
    m_VCArray.setGrowLength(-100);
    m_VNCArray.setGrowLength(-100);
    m_VNTCArray.setGrowLength(-100);
  }
}

/////////////////////// D3dLostStateManager ////////////////////////

D3dLostStateManager::D3dLostStateManager()
  : m_bEnableManager(false)
{
}

D3dLostStateManager::~D3dLostStateManager()
{
  clearEntriesList();
}

void D3dLostStateManager::clearEntriesList()
{
  OdList<D3dLostStateEntry*>::iterator it = m_entries.begin();
  for ( ; it != m_entries.end( ); it++)
  {
    (*it)->onManagerDestroy(this);
  }
  m_entries.clear();
}

bool D3dLostStateManager::isManagerEnabled() const
{
  return m_bEnableManager;
}

void D3dLostStateManager::enableManager(bool bEnable)
{
  m_bEnableManager = bEnable;
}

void D3dLostStateManager::setD3DDevice(IDirect3DDevice* pD3Ddevice)
{
  m_pD3Ddevice = pD3Ddevice;
}

IDirect3DDevice* D3dLostStateManager::d3dDevice() const
{
  return m_pD3Ddevice;
}

void D3dLostStateManager::appendEntry(D3dLostStateEntry *pNewEntry)
{
  m_entries.insert(m_entries.end(), pNewEntry);
}

void D3dLostStateManager::removeEntry(D3dLostStateEntry *pEntry)
{
  m_entries.remove(pEntry);
}

void D3dLostStateManager::executeRecreation()
{
  OdList<D3dLostStateEntry*>::iterator it = m_entries.begin();
  for ( ; it != m_entries.end( ); it++)
  {
    (*it)->recreate(this);
  }
}

//////////////// D3dInitTextureRecord
D3dInitTextureRecord::D3dInitTextureRecord(D3dMaterialViewRedirect::RasterImageRef &pTexture, 
                                           const OdGeMatrix3d& w2TxXform,
                                           bool bTransparency,
                                           bool bBitonal,
                                           bool bDraft,
                                           bool bAlpha1Bit, bool bAlpha8Bit,
                                           D3dMaterialViewRedirect *pUninitIface,
                                           D3dLostStateManager *pLSManager)
: m_texture(pTexture)
, m_w2TxXform(w2TxXform)
, m_uFlags(0)
, m_pUninitIface(pUninitIface)
, m_pLSManager(pLSManager)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dInitTextureRecord()\n");
#endif
  SETBIT(m_uFlags, kTransp , bTransparency);
  SETBIT(m_uFlags, kBitonal, bBitonal);
  SETBIT(m_uFlags, kDraft  , bDraft);
  SETBIT(m_uFlags, kAlpha1 , bAlpha1Bit);
  SETBIT(m_uFlags, kAlpha8 , bAlpha8Bit);
  if (m_pLSManager)
  {
    OdGsBaseDeviceMTSyncHelper _sh(*(pUninitIface->mtDeviceSync()), OdGsBaseDeviceMTHelpers::kSyncRasterCache);
    m_pLSManager->appendEntry(this);
  }
}

D3dInitTextureRecord::~D3dInitTextureRecord()
{
  if (m_pUninitIface)
  {
    OdGsBaseDeviceMTSyncHelper _sh(*(m_pUninitIface->mtDeviceSync()), OdGsBaseDeviceMTHelpers::kSyncRasterCache);
    m_pUninitIface->decrementRasterImageRef(m_texture.m_pTexture);
    m_pLSManager->removeEntry(this);
  }
}

inline IDirect3DDevice *getDevice(IDirect3DTexture *pTex)
{
  IDirect3DDevice *pRVal = NULL;
  if (pTex)
    pTex->GetDevice(&pRVal);
  if (pRVal)
    pRVal->Release(); // Safe enough to use after. This is necessary to prevent interface leaks.
  return pRVal;
}

void D3dInitTextureRecord::play(const D3dMetafilePlayContext& ctx) const
{
  OdD3DStateManager *pSM = &(ctx.stateManager());
  if (getDevice(m_texture.m_pTexture) == ctx.device())
    pSM->SetTexture(0, m_texture.m_pTexture);
  else
  {
    // Get shared resource
    IDirect3DTexture *pTex = getShared(ctx.device());
    if (!pTex)
      pTex = createShared(ctx.device(), m_texture.m_pTexture, ctx.materialViewRedirect()->sharingProvider());
    pSM->SetTexture(0, pTex);
  }
  pSM->SetColorOpTextureStageState(0, D3DTOP_SELECTARG1);
  pSM->SetColorArg1TextureStageState(0, D3DTA_TEXTURE);
  pSM->SetColorArg2TextureStageState(0, D3DTA_DIFFUSE);

  if(GETBIT(m_uFlags, kBitonal | kDraft) || m_texture.m_nTexLut)
  {
    pSM->SetMagFilterTextureStageState(0, D3DTEXF_POINT);
    pSM->SetMinFilterTextureStageState(0, D3DTEXF_POINT);
  }
  else
  {
    pSM->SetMagFilterTextureStageState(0, D3DTEXF_LINEAR);
    pSM->SetMinFilterTextureStageState(0, D3DTEXF_LINEAR);
  }

  pSM->SetAddressUTextureStageState(0, D3DTADDRESS_CLAMP);
  pSM->SetAddressVTextureStageState(0, D3DTADDRESS_CLAMP);

  pSM->SetAlphaOpTextureStageState(0, D3DTOP_MODULATE);
  if(GETBIT(m_uFlags, kTransp))
  {
    if (!GETBIT(m_uFlags, kAlpha8))
    {
      pSM->SetAlphaTestEnableRenderState(TRUE);
      pSM->SetAlphaFuncRenderState(D3DCMP_GREATER);
      if (GETBIT(m_uFlags, kBitonal))
        pSM->SetAlphaRefRenderState(0x7F);
      else if (GETBIT(m_uFlags, kAlpha1))
      {
        pSM->SetAlphaRefRenderState(0xFF);
        pSM->SetAlphaFuncRenderState(D3DCMP_EQUAL);
      }
      else
        pSM->SetAlphaRefRenderState(0);
    }
    pSM->SetAlphaBlendEnableRenderState(TRUE);
    pSM->SetSrcBlendRenderState(D3DBLEND_SRCALPHA);
    pSM->SetDestBlendRenderState(D3DBLEND_INVSRCALPHA);
  }
  else
  {
    pSM->SetAlphaTestEnableRenderState(FALSE);
    pSM->SetAlphaBlendEnableRenderState(FALSE);
  }

  OdGeMatrix3d textTransform = m_w2TxXform * ctx.eyeToWorld();

  D3DXMATRIX mat;
  for(int i = 0; i < 4; i ++)
  {
    for(int j = 0; j < 4; j ++)
    {
      mat(i, j) = (float)textTransform[j][i];
    }
  }

  pSM->SetTextureMatrix(0, &mat);
  pSM->SetTextureTransformFlagsTextureStageState(0, D3DTTFF_COUNT2);
  pSM->SetTexCoordIndexTextureStageState(0, D3DTSS_TCI_CAMERASPACEPOSITION);

  ctx.updateMaterialTexture(true, true);
#ifdef OD_USE_DIRECT3D9
  if (m_texture.m_nTexLut)
  {
    IDirect3DPixelShader9 *pShader = ctx.paletteImageShader();
    if (getDevice(m_texture.m_pPalette) == ctx.device())
      pSM->SetTexture(2, m_texture.m_pPalette);
    else
    {
      // Get shared resource
      IDirect3DTexture *pPal = m_palRef.getShared(ctx.device());
      if (!pPal)
        pPal = m_palRef.createShared(ctx.device(), m_texture.m_pPalette, ctx.materialViewRedirect()->sharingProvider());
      pSM->SetTexture(2, pPal);
    }

    pSM->SetColorOpTextureStageState(2, D3DTOP_SELECTARG1);
    pSM->SetColorArg1TextureStageState(2, D3DTA_TEXTURE);
    pSM->SetColorArg2TextureStageState(2, D3DTA_DIFFUSE);
    pSM->SetMagFilterTextureStageState(2, D3DTEXF_POINT);
    pSM->SetMinFilterTextureStageState(2, D3DTEXF_POINT);
    pSM->SetAddressUTextureStageState(2, D3DTADDRESS_CLAMP);
    pSM->SetAddressVTextureStageState(2, D3DTADDRESS_CLAMP);
    pSM->SetAlphaOpTextureStageState(2, D3DTOP_SELECTARG1);
    pSM->SetTextureTransformFlagsTextureStageState(2, D3DTTFF_DISABLE);
    pSM->SetTexCoordIndexTextureStageState(2, D3DTSS_TCI_PASSTHRU);

    ctx.device()->SetPixelShader(pShader);
    FLOAT lut[4] = { (float)m_texture.m_nTexLut };
    ctx.device()->SetPixelShaderConstantF(0, lut, 1);
    lut[0] = (float)(pSM->GetTexture(1) != NULL);
    ctx.device()->SetPixelShaderConstantF(1, lut, 1);
  }
#endif
}

OdUInt64 D3dInitTextureRecord::recordSize() const
{
  OdUInt64 size = (OdUInt64)sizeof(*this);
  return size;
}


void D3dInitTextureRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}

void D3dInitTextureRecord::recreate(D3dLostStateManager * /*pManager*/)
{
}

void D3dInitTextureRecord::onManagerDestroy(D3dLostStateManager * /*pManager*/)
{
  m_pLSManager = NULL;
  m_pUninitIface = NULL;
}

/////////////// D3dUninitTextureRecord
D3dUninitTextureRecord::D3dUninitTextureRecord()
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dUninitTextureRecord()\n");
#endif
}

void D3dUninitTextureRecord::play(const D3dMetafilePlayContext& ctx) const
{
#ifdef OD_USE_DIRECT3D9
  ctx.device()->SetPixelShader(NULL);
  ctx.stateManager().SetTexture(2, NULL);
#endif
  ctx.stateManager().SetTexture(0, NULL);
  ctx.stateManager().SetAlphaTestEnableRenderState(FALSE);
  ctx.stateManager().SetAlphaBlendEnableRenderState(TRUE);
  if (ctx.renderMode() != OdGsView::k2DOptimized)
  {
    ctx.stateManager().SetSrcBlendRenderState(D3DBLEND_ONE);
    ctx.stateManager().SetDestBlendRenderState(D3DBLEND_ZERO);
  }
  ctx.updateMaterialTexture(false);
}


void D3dUninitTextureRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}

/////////////// D3dBaseBeginOrEndRecord
D3dBaseBeginOrEndRecord::D3dBaseBeginOrEndRecord()
{
}

/////////////// D3dBaseHighlightingRecord
D3dBaseHighlightingRecord::D3dBaseHighlightingRecord()
  : D3dBaseBeginOrEndRecord()
{
}

void D3dBaseHighlightingRecord::play(const D3dMetafilePlayContext& pContext) const
{
  pContext.vectorizer()->highlight(isBegin());
}


void D3dBaseHighlightingRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}

/////////////// D3dBeginHighlightingRecord
D3dBeginHighlightingRecord::D3dBeginHighlightingRecord()
  : D3dBaseHighlightingRecord()
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dBeginHighlightingRecord()\n");
#endif
}

bool D3dBeginHighlightingRecord::isBegin() const
{
  return true;
}

/////////////// D3dEndHighlightingRecord
D3dEndHighlightingRecord::D3dEndHighlightingRecord()
  : D3dBaseHighlightingRecord()
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dEndHighlightingRecord()\n");
#endif
}

bool D3dEndHighlightingRecord::isBegin() const
{
  return false;
}

/////////////// D3dSelectionMarkerRecord
D3dSelectionMarkerRecord::D3dSelectionMarkerRecord(OdGsMarker gsMarker)
  : m_gsMarker(gsMarker)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dSelectionMarkerRecord()\n");
#endif
}

void D3dSelectionMarkerRecord::play(const D3dMetafilePlayContext& pContext) const
{
  if (pContext.selMarks())
    pContext.setSelectionMarker(m_gsMarker);
}


void D3dSelectionMarkerRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& /*ctx*/ ) const
{
  if (pContext.selMarks())
    pContext.setSelectionMarker(m_gsMarker);
}


/////////////// D3dTransparencyRecord
D3dTransparencyRecord::D3dTransparencyRecord(OdUInt8 invAlpha)
  : m_transparency(invAlpha)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTransparencyRecord()\n");
#endif
}

void D3dTransparencyRecord::play(const D3dMetafilePlayContext& pContext) const
{
  pContext.updateTransparencyState(D3dMetafilePlayContext::Transparency_Color, m_transparency);
}


void D3dTransparencyRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}


/////////////// D3dLwdStyleRecord
D3dLwdStyleRecord::D3dLwdStyleRecord()
{
  m_lwdStyle[0] = 1;
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTransparencyRecord()\n");
#endif
}

D3dLwdStyleRecord::D3dLwdStyleRecord(OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle)
{
  m_lwdStyle[0] = 0;
  m_lwdStyle[1] = (OdUInt8)lesStyle;
  m_lwdStyle[2] = (OdUInt8)ljsStyle;
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTransparencyRecord()\n");
#endif
}

void D3dLwdStyleRecord::play(const D3dMetafilePlayContext& pContext) const
{
  D3dLwdStyle &lwdStyle = pContext.lwdStyle();
  if (m_lwdStyle[0])
  {
    if (!lwdStyle.m_bCurLwdUsesDef)
    {
      lwdStyle.m_bCurLwdDef = lwdStyle.m_bDefLwdDef;
      lwdStyle.m_curLwdLes = lwdStyle.m_defLwdLes;
      lwdStyle.m_curLwdLjs = lwdStyle.m_defLwdLjs;
      lwdStyle.m_bCurLwdUsesDef = true;
    }
  }
  else
  {
    OdPs::LineEndStyle lesStyle = (OdPs::LineEndStyle)m_lwdStyle[1];
    OdPs::LineJoinStyle ljsStyle = (OdPs::LineJoinStyle)m_lwdStyle[2];
    lwdStyle.m_bCurLwdUsesDef = (lesStyle == OdPs::kLesUseObject) && (ljsStyle == OdPs::kLjsUseObject);
    lwdStyle.m_curLwdLes = (lesStyle != OdPs::kLesUseObject) ? lesStyle : lwdStyle.m_defLwdLes;
    lwdStyle.m_curLwdLjs = (ljsStyle != OdPs::kLjsUseObject) ? ljsStyle : lwdStyle.m_defLwdLjs;
    lwdStyle.m_bCurLwdDef = (lwdStyle.m_curLwdLes == OdPs::kLesRound) && (lwdStyle.m_curLwdLjs == OdPs::kLjsRound);
  }
}


void D3dLwdStyleRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}


/////////////// D3dBaseNoColorModRecord
D3dBaseNoColorModRecord::D3dBaseNoColorModRecord()
  : D3dBaseBeginOrEndRecord()
{
}

void D3dBaseNoColorModRecord::play(const D3dMetafilePlayContext& pContext) const
{
  pContext.setNoColorModificationMode(isBegin());
}


void D3dBaseNoColorModRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}

/////////////// D3dBeginNoColorModRecord
D3dBeginNoColorModRecord::D3dBeginNoColorModRecord()
  : D3dBaseNoColorModRecord()
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dBeginNoColorModRecord()\n");
#endif
}

bool D3dBeginNoColorModRecord::isBegin() const
{
  return true;
}

/////////////// D3dEndNoColorModRecord
D3dEndNoColorModRecord::D3dEndNoColorModRecord()
  : D3dBaseNoColorModRecord()
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dEndNoColorModRecord()\n");
#endif
}

bool D3dEndNoColorModRecord::isBegin() const
{
  return false;
}

/////////////// D3dTtfTextRecord
D3dTtfTextRecord::D3dTtfTextRecord(const OdTtfFontsCache::TextInfo &textInfo, const D3dLwdSetting &lineWeight, DWORD color)
  : m_textInfo(textInfo), m_lineWeight(lineWeight), m_nColor(color)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dTtfTextRecord()\n");
#endif
}

void D3dTtfTextRecord::play(const D3dMetafilePlayContext& pContext) const
{
  if (pContext.materialViewRedirect())
  {
    OdTtfFontsCache *pCache = pContext.materialViewRedirect()->getTtfFontsCache();
    pContext.setLineweightOverride(m_lineWeight);
    pContext.setColorOverride(m_nColor);
    pCache->playText(m_textInfo, pContext.materialViewRedirect());
    pContext.setLineweightOverride(false);
    pContext.setColorOverride(false);
  }
}


void D3dTtfTextRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if (pContext.materialViewRedirect())
  {
    OdTtfFontsCache *pCache = pContext.materialViewRedirect()->getTtfFontsCache();
    pContext.setLineweightOverride(m_lineWeight);
    pContext.setColorOverride(m_nColor);
    pCache->playText(m_textInfo, pContext.materialViewRedirect());
    pContext.setLineweightOverride(false);
    pContext.setColorOverride(false);
  }
}


//#include "Gi/GiVisualStyle.h"
void ExGiDirectXGeometry::draw_visualStyle(const OdGiVisualStyle &/*visualStyle*/)
{
  // Example for visual style accessing in GiGeometry:
  //const OdGiDisplayStyle &ds = visualStyle.displayStyle();
  //ODA_TRACE(L"ds.displaySettings -> %u\n", ds.displaySettings());
}

ODRX_CONS_DEFINE_MEMBERS(ExD3dMaterialCacheEntry, OdRxObject, RXIMPL_CONSTR);

bool ExGiDirectXGeometry::skipMaterialProcess(OdDbStub *materialId)
{
  return (renderMode() < OdGsView::kFlatShaded) || m_mfPlayCtx.forceFill();
}

OdGiMaterialItemPtr ExGiDirectXGeometry::fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData)
{
  if (m_pD3DSM == NULL) return OdGiMaterialItemPtr();

  OdGiMaterialItemPtr pMatItem = OdGiMaterialItem::createObject();
  if (materialId == NULL)
    return pMatItem;
  ExD3dMaterialCacheEntryPtr mcache;

  OdGiMaterialColor diffuseColor; OdGiMaterialMap diffuseMap;
  OdGiMaterialColor ambientColor; OdGiMaterialMap ambientMap;
  OdGiMaterialColor specularColor; OdGiMaterialMap specularMap; double glossFactor;
  double opacityPercentage; OdGiMaterialMap opacityMap; OdGiMaterialColor emissionColor;
  ODCOLORREF colorDiffuse(0), colorAmbient(0), colorSpecular(0), colorEmission(0);

  materialData.diffuse(diffuseColor, diffuseMap);
  materialData.ambient(ambientColor);
  materialData.specular(specularColor, specularMap, glossFactor);
  materialData.opacity(opacityPercentage, opacityMap);
  if (diffuseColor.color().colorMethod() == OdCmEntityColor::kByColor)
  {
    colorDiffuse = ODTOCOLORREF(diffuseColor.color());
  }
  else if (diffuseColor.color().colorMethod() == OdCmEntityColor::kByACI
        || diffuseColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    colorDiffuse = OdCmEntityColor::lookUpRGB((OdUInt8)diffuseColor.color().colorIndex());
  }
  else
  {
    ODA_ASSERT_ONCE(false);
  }
  if (ambientColor.color().colorMethod() == OdCmEntityColor::kByColor)
  {
    colorAmbient = ODTOCOLORREF(ambientColor.color());
  }
  else if (ambientColor.color().colorMethod() == OdCmEntityColor::kByACI
        || ambientColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    colorAmbient = OdCmEntityColor::lookUpRGB((OdUInt8)ambientColor.color().colorIndex());
  }
  else
  {
    ODA_ASSERT_ONCE(false);
  }
  if (specularColor.color().colorMethod() == OdCmEntityColor::kByColor)
  {
    colorSpecular = ODTOCOLORREF(specularColor.color());
  }
  else if (specularColor.color().colorMethod() == OdCmEntityColor::kByACI
        || specularColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    colorSpecular = OdCmEntityColor::lookUpRGB((OdUInt8)specularColor.color().colorIndex());
  }
  else
  {
    ODA_ASSERT_ONCE(false);
  }
  if (materialData.luminanceMode() == OdGiMaterialTraits::kEmissionColor)
  {
    OdGiMaterialMap emissionMap;
    materialData.emission(emissionColor, emissionMap);
    if (emissionColor.color().colorMethod() == OdCmEntityColor::kByColor)
    {
      colorEmission = ODTOCOLORREF(emissionColor.color());
    }
    else if (emissionColor.color().colorMethod() == OdCmEntityColor::kByACI
             || emissionColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
    {
      colorEmission = OdCmEntityColor::lookUpRGB((OdUInt8)emissionColor.color().colorIndex());
    }
    else
    {
      ODA_ASSERT_ONCE(false);
    }
  }

  D3dMaterialRecordData mtrlData;
  D3DMATERIAL &mtrl = mtrlData.m_d3dMat;
  ZeroMemory( &mtrl, sizeof(D3DMATERIAL) );
  mtrl.Specular.r = float((double)ODGETRED(colorSpecular) / 255 * specularColor.factor());
  mtrl.Specular.g = float((double)ODGETGREEN(colorSpecular) / 255 * specularColor.factor());
  mtrl.Specular.b = float((double)ODGETBLUE(colorSpecular) / 255 * specularColor.factor());
  mtrl.Specular.a = 1.0f;
  mtrl.Power = (1.0f - float(glossFactor)) * 100;
  mtrl.Ambient.r = float((double)ODGETRED(colorAmbient) / 255 * ambientColor.factor());
  mtrl.Ambient.g = float((double)ODGETGREEN(colorAmbient) / 255 * ambientColor.factor());
  mtrl.Ambient.b = float((double)ODGETBLUE(colorAmbient) / 255 * ambientColor.factor());
  mtrl.Ambient.a = 1.0f;
  mtrl.Diffuse.r = float((double)ODGETRED(colorDiffuse) / 255 * diffuseColor.factor());
  mtrl.Diffuse.g = float((double)ODGETGREEN(colorDiffuse) / 255 * diffuseColor.factor());
  mtrl.Diffuse.b = float((double)ODGETBLUE(colorDiffuse) / 255 * diffuseColor.factor());
  //mtrl.Diffuse.a = float(opacityPercentage); // !!! not / 100
  mtrl.Diffuse.a = 1.0f;
  mtrl.Emissive.r = float((double)ODGETRED(colorEmission) / 255 * emissionColor.factor());
  mtrl.Emissive.g = float((double)ODGETGREEN(colorEmission) / 255 * emissionColor.factor());
  mtrl.Emissive.b = float((double)ODGETBLUE(colorEmission) / 255 * emissionColor.factor());
  mtrl.Emissive.a = 1.0f;

  //
  mtrlData.m_bUseDiffuseMat = diffuseColor.method() == OdGiMaterialColor::kOverride;
  mtrlData.m_bUseAmbientMat = ambientColor.method() == OdGiMaterialColor::kOverride;
  mtrlData.m_bUseSpecularMat = specularColor.method() == OdGiMaterialColor::kOverride;
  mtrlData.m_fOpacityPercentage = opacityPercentage;

  // #15298 : fill completely black surfaces by specular highlighting
  // This is correct for "Shaded" visual styles, but invalid for "Realistic".
  // So, make more analysis to keep old behavior in cases when this is possible.
  if (OdZero(mtrl.Specular.r + mtrl.Specular.g + mtrl.Specular.b) &&
      (!mtrlData.m_bUseDiffuseMat || OdZero(mtrl.Diffuse.r + mtrl.Diffuse.g + mtrl.Diffuse.b)) &&
      isLightColor(*m_paletteColors))
    mtrl.Specular.r = mtrl.Specular.g = mtrl.Specular.b = 1.0f, mtrl.Power = 10.0f, mtrlData.m_bUseSpecularMat = true;
  //

  mcache = ExD3dMaterialCacheEntry::createObject();
  mcache->m_mat = mtrlData;

  mcache->m_mat.m_fDiffuseBlendFactor = 0.0;
  struct WrappedSync {
    D3dMaterialViewRedirect *m_pViewRedir;
    WrappedSync(D3dMaterialViewRedirect *pViewRedir) : m_pViewRedir(pViewRedir) { }
    bool call_setGiMaterialTexture(OdGiMaterialItem *pMatItem,
        OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, const OdGiMaterialMap &matMap, OdGiMaterialTextureManager *pManager = NULL) {
      OdGsBaseDeviceMTSyncHelper __sync(*m_pViewRedir->mtDeviceSync(), OdGsBaseDeviceMTHelpers::kSyncTextureCache);
      return pMatItem->createDiffuseTexture()->setGiMaterialTexture(pDeviceInfo, pTexDataImpl, giCtx, matMap, pManager);
    }
  } _wrappedSync(m_pMViewRedirect);
  // Texture data, if available
  if (!GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseDiffuse) ||
      !_wrappedSync.call_setGiMaterialTexture(pMatItem, m_pD3DSM->GetLostStateManager(), ExGiD3dMaterialTextureData::desc(), giCtx(), diffuseMap, materialTextureManager()))
  {
    pMatItem->removeDiffuseTexture();
  }
  else
  {
    mcache->m_mat.m_fDiffuseBlendFactor = diffuseMap.blendFactor();
    mcache->m_mat.m_uTiling = currentMapper(false)->diffuseMapper()->mapper().uTiling();
    mcache->m_mat.m_vTiling = currentMapper(false)->diffuseMapper()->mapper().vTiling();
  }
  pMatItem->setCachedData((OdRxObjectPtr)mcache);
  return pMatItem;
}

void ExGiDirectXGeometry::renderMaterial(OdGiMaterialItemPtr pCache, OdDbStub * /*materialId*/)
{
  if (m_bPlayMetafile || m_pD3DSM == NULL)
    return;

  if (pCache.isNull())
  {
    setupDefaultMaterialToDevice(&d3dStateManager());
    m_mfPlayCtx.updateMaterialTexture(false);
    m_mfPlayCtx.updateTransparencyState(D3dMetafilePlayContext::Transparency_Material, 0);
    return;
  }

  D3dRecord *pMatRec = NULL;
  pMatRec = D3dMaterialRecord::createObject(pCache);
  ODA_ASSERT(pMatRec != NULL);
  if (m_rPackager.isInitialized())
  {
    ODA_ASSERT(renderMode() > OdGsView::kHiddenLine);
    m_rPackager.addRecord(D3dMetafilePackager::kLTEntry, D3dMetafilePackager::kMaterialRec, pMatRec);
  }
  else
  {
    pMatRec->play(m_mfPlayCtx);
    delete pMatRec;
  }
}

void ExGiDirectXGeometry::playDelayCacheEntry(const void *pEntry)
{
  ExGsDelayedMappingEntry *pMapEntry = const_cast<ExGsDelayedMappingEntry*>((const ExGsDelayedMappingEntry*)pEntry);
  int i;
  OdD3DVertexNormalTexColArray &vertArray = (pMapEntry->m_pMetafile->getTexturedTriArray().isEmpty()) ? m_rPackager.getVNTCArray() : pMapEntry->m_pMetafile->getTexturedTriArray();
  for (OdUInt32 id = pMapEntry->m_uStartID; id < pMapEntry->m_uStartID + pMapEntry->m_uLength; id += 3)
  {
    OdGiMapperItemEntry::MapInputTriangle inputData;
    OdGiMapperItemEntry::MapOutputCoords outputData;
    for (i = 0; i < 3; i++)
    {
      inputData.inPt[i].set(vertArray.at(id + i).position.x, vertArray.at(id + i).position.y, vertArray.at(id + i).position.z);
    }
    currentMapper()->diffuseMapper()->mapCoords(inputData, OdGeVector3d(vertArray.at(id + 1).normal.x, vertArray.at(id + 1).normal.y, vertArray.at(id + 1).normal.z), outputData);
    for (i = 0; i < 3; i++)
    {
      vertArray.at(id + i).s = dbl2float(outputData.outCoord[i].x);
      vertArray.at(id + i).t = dbl2float(outputData.outCoord[i].y);
    }
  }
}

void ExGiDirectXGeometry::mapperChangedForDelayCache()
{
  if (m_rPackager.isInitialized())
  {
    ExGsDelayedMappingEntry *pMapEntry = new ExGsDelayedMappingEntry();
    appendDelayCacheEntry(pMapEntry);
    m_rPackager.startDelayMapping(pMapEntry);
  }
}

OdGiMaterialItemPtr ExGiDirectXGeometry::currentMaterial() const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryCurrentMaterial();
  return OdGiMaterialItemPtr();
}

void ExGiDirectXGeometry::resetCurrentMaterial(OdGiMaterialItemPtr pMaterial)
{
  if (m_pMViewRedirect)
    m_pMViewRedirect->geometryResetCurrentMaterial(pMaterial);
}

bool ExGiDirectXGeometry::isMaterialEnabled() const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryIsMaterialEnabled();
  return false;
}

bool ExGiDirectXGeometry::isMaterialAvailable() const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryIsMaterialAvailable();
  return false;
}

void ExGiDirectXGeometry::resetCurrentMaterial()
{
  if (m_pMViewRedirect)
    m_pMViewRedirect->geometryResetCurrentMaterial();
}

OdGiMapperItemPtr ExGiDirectXGeometry::currentMapper(bool bForCoords) const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryCurrentMapper(bForCoords);
  return OdGiMapperItemPtr();
}

void ExGiDirectXGeometry::resetCurrentMapper(OdGiMapperItemPtr pMapper)
{
  if (m_pMViewRedirect)
    m_pMViewRedirect->geometryResetCurrentMapper(pMapper);
}

bool ExGiDirectXGeometry::isMapperEnabled() const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryIsMapperEnabled();
  return false;
}

bool ExGiDirectXGeometry::isMapperAvailable() const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryIsMapperAvailable();
  return false;
}

bool ExGiDirectXGeometry::isMappingDelayed() const
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->geometryIsMappingDelayed();
  return false;
}

void ExGiDirectXGeometry::appendDelayCacheEntry(void *pEntry)
{
  if (m_pMViewRedirect)
    m_pMViewRedirect->geometryAppendDelayCacheEntry(pEntry);
}

OdGiMaterialTextureManager *ExGiDirectXGeometry::materialTextureManager()
{
  if (m_pMViewRedirect)
    return m_pMViewRedirect->materialTextureManager();
  return NULL;
}

// ExGiD3dMaterialTextureData

ODRX_CONS_DEFINE_MEMBERS(ExGiD3dMaterialTextureData, OdGiMaterialTextureData, RXIMPL_CONSTR);

ExGiD3dMaterialTextureData::ExGiD3dMaterialTextureData()
  : m_uLSWidth(0)
  , m_uLSHeight(0)
{
}

ExGiD3dMaterialTextureData::~ExGiD3dMaterialTextureData()
{
  m_pTexture = 0;
  if (!m_pLSBits.empty() && m_pLSManager)
  {
    m_pLSManager->removeEntry(this);
  }
}

// Input: RGBA array of colors
void ExGiD3dMaterialTextureData::setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image)
{
  const D3DPOOL poolType = D3DPOOL_MANAGED;
  D3dLostStateManager *pLSManager = reinterpret_cast<D3dLostStateManager*>(pDeviceInfo.getPtr());
  IDirect3DDevice *pD3Ddevice = pLSManager->d3dDevice();
  ODA_ASSERT(pD3Ddevice != NULL);
  D3DCAPS devCaps;
  ZeroMemory(&devCaps, sizeof(D3DCAPS));
  pD3Ddevice->GetDeviceCaps(&devCaps);
  DWORD maxTextureWidth = devCaps.MaxTextureWidth;
  DWORD maxTextureHeight = devCaps.MaxTextureHeight;
  DWORD srcWidth = (DWORD)image.width();
  DWORD srcHeight = (DWORD)image.height();
  DWORD destWidth = 1;
  while (destWidth < srcWidth)
  {
    destWidth <<= 1;
    if (destWidth == maxTextureWidth)
    {
      break;
    }
  }
  DWORD destHeight = 1;
  while (destHeight < srcHeight)
  {
    destHeight <<= 1;
    if (destHeight == maxTextureHeight)
    {
      break;
    }
  }
  if (destWidth == srcWidth && destHeight == srcHeight)
  {
    pD3Ddevice->CreateTexture(srcWidth, srcHeight, 1, 0, D3DFMT_A8R8G8B8, poolType, &m_pTexture);
    D3DLOCKED_RECT lockedRect;
    m_pTexture->LockRect(0, &lockedRect, NULL, 0);
    OdUInt32 *pBits = (OdUInt32*)lockedRect.pBits;
    memcpy(pBits, image.image(), sizeof(OdUInt32) * srcWidth * srcHeight);
    m_pTexture->UnlockRect(0);
  }
  else
  {
    pD3Ddevice->CreateTexture(destWidth, destHeight, 1, 0, D3DFMT_A8R8G8B8, poolType, &m_pTexture);
    D3DLOCKED_RECT lockedRect;
    m_pTexture->LockRect(0, &lockedRect, NULL, 0);
    OdUInt32 *pBits = (OdUInt32*)lockedRect.pBits;
    double aspectX = double(srcWidth - 1) / (destWidth - 1),
           aspectY = double(srcHeight - 1) / (destHeight - 1);
    for (OdUInt32 iy = 0; iy < destHeight; iy++)
    {
      for (OdUInt32 ix = 0; ix < destWidth; ix++)
      {
        OdGiPixelBGRA32 *pPtr = image.image() + (OdUInt32(aspectY * iy) * srcWidth + OdUInt32(aspectX * ix));
        *pBits = pPtr->getBGRA();
        pBits++;
      }
    }
    m_pTexture->UnlockRect(0);
  }
  if (pLSManager->isManagerEnabled() && poolType != D3DPOOL_MANAGED)
  {
    m_pLSBits.resize(image.width() * image.height());
    CopyMemory(m_pLSBits.asArrayPtr(), image.image(), sizeof(OdGiPixelBGRA32) * image.width() * image.height());
    m_uLSWidth = image.width(); m_uLSHeight = image.height();
    pLSManager->appendEntry(this);
    m_pLSManager = pLSManager;
  }
}

void ExGiD3dMaterialTextureData::textureData(OdGiPixelBGRA32Array &data, OdUInt32 &width, OdUInt32 &height) const
{
  if (!m_pLSBits.empty())
  {
    data = m_pLSBits;
    width = m_uLSWidth;
    height = m_uLSHeight;
  }
  else
  {
    if (m_pTexture == 0) return;
    D3DLOCKED_RECT lockedRect;
    m_pTexture->LockRect(0, &lockedRect, NULL, 0);
    D3DSURFACE_DESC surfDesc;
    m_pTexture->GetLevelDesc(0, &surfDesc);
    width = (OdUInt32)surfDesc.Width;
    height = (OdUInt32)surfDesc.Height;
    data.resize(width * height);
    for (OdUInt32 i = 0; i < data.size(); i++)
    {
      data[i].setBGRA(((OdUInt32*)lockedRect.pBits)[i]);
    }
    m_pTexture->UnlockRect(0);
  }
}

bool ExGiD3dMaterialTextureData::haveData() const
{
  return m_pTexture != 0 || !m_pLSBits.empty();
}

CComPtr<IDirect3DTexture> ExGiD3dMaterialTextureData::deviceTexture() const
{
  return m_pTexture;
}

CComPtr<IDirect3DTexture> ExGiD3dMaterialTextureData::deviceTexture(const D3dMetafilePlayContext &ctx) const
{
  if (getDevice(m_pTexture) == ctx.device())
    return m_pTexture;
  else
  {
    // Get shared resource
    IDirect3DTexture *pTex = getShared(ctx.device());
    if (!pTex)
      pTex = createShared(ctx.device(), m_pTexture, ctx.materialViewRedirect()->sharingProvider());
    return pTex;
  }
}

void ExGiD3dMaterialTextureData::recreate(D3dLostStateManager *pManager)
{
  if (!m_pLSBits.empty())
  {
    pManager->removeEntry(this);
    OdGiImageBGRA32 image(m_uLSWidth, m_uLSHeight, m_pLSBits.asArrayPtr());
    m_pTexture.Release();
    setTextureData(pManager, image);
  }
}

void ExGiD3dMaterialTextureData::onManagerDestroy(D3dLostStateManager * /*pManager*/)
{
  m_pLSManager = NULL;
}

//

D3dMaterialRecord::D3dMaterialRecord(OdGiMaterialItemPtr pMatItem)
{
#ifdef OD_EXDX_TRACERECS
  ODA_TRACE(L"TraceRecs: D3dMaterialRecord()\n");
#endif
  m_pMatItem = pMatItem;
}

void D3dMaterialRecord::play(const D3dMetafilePlayContext& ctx) const
{
  bool bad = true;
  if (!m_pMatItem.isNull() && m_pMatItem->materialId())
  {
    ExD3dMaterialCacheEntryPtr mcache = ExD3dMaterialCacheEntry::cast(m_pMatItem->cachedData());

    OdGiMaterialItemPtr *pIntMtlPtr = ctx.intMtlPtr();
    if (!pIntMtlPtr->isNull() && m_pMatItem->materialId() == (*pIntMtlPtr)->materialId() &&
        !m_pMatItem->haveDiffuseTexture() && (mcache.isNull() || !(mcache->m_mat.m_fOpacityPercentage < 1.0)))
    {
      // @@@TODO: optimize for texture state also.
      return;
    }
    *pIntMtlPtr = m_pMatItem;
    if (!mcache.isNull())
    {
      ctx.stateManager().SetMaterial(&mcache->m_mat.m_d3dMat);
      ctx.stateManager().SetAmbientMaterialSourceRenderState((mcache->m_mat.m_bUseAmbientMat) ? D3DMCS_MATERIAL : D3DMCS_COLOR1);
      ctx.stateManager().SetDiffuseMaterialSourceRenderState((mcache->m_mat.m_bUseDiffuseMat) ? D3DMCS_MATERIAL : D3DMCS_COLOR1);
      ctx.stateManager().SetSpecularMaterialSourceRenderState((mcache->m_mat.m_bUseSpecularMat) ? D3DMCS_MATERIAL : D3DMCS_COLOR1);
      ctx.stateManager().SetEmissiveMaterialSourceRenderState(D3DMCS_MATERIAL);
      //ctx.device()->SetRenderState(D3DRS_ALPHABLENDENABLE, (mcache->m_mat.m_d3dMat.Diffuse.a != 1.0) ? TRUE : FALSE);
      //if (mcache->m_mat.m_d3dMat.Diffuse.a < 1.0)
      //{
      //  ctx.device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
      //  ctx.device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
      //}
      // Process texture record
      if (m_pMatItem->haveDiffuseTexture() && mcache->m_mat.m_fDiffuseBlendFactor > 0.0)
      {
        ctx.stateManager().SetTexture(0, ((ExGiD3dMaterialTextureDataPtr)m_pMatItem->diffuseTexture()->textureData())->deviceTexture(ctx));
        ctx.updateMaterialTexture(true);
        if (mcache->m_mat.m_fDiffuseBlendFactor < 1.0)
        {
          DWORD dw = DWORD(mcache->m_mat.m_fDiffuseBlendFactor * 255);
          ctx.stateManager().SetTextureFactorRenderState((dw << 24) | (dw << 16) | (dw << 8) | dw);
          ctx.stateManager().SetColorOpTextureStageState(0, D3DTOP_BLENDFACTORALPHA);
        }
        else
        {
          ctx.stateManager().SetColorOpTextureStageState(0, D3DTOP_SELECTARG1);
        }
        ctx.stateManager().SetColorArg1TextureStageState(0, D3DTA_TEXTURE);
        ctx.stateManager().SetColorArg2TextureStageState(0, D3DTA_DIFFUSE);
        ctx.stateManager().SetMagFilterTextureStageState(0, D3DTEXF_LINEAR);
        ctx.stateManager().SetMinFilterTextureStageState(0, D3DTEXF_LINEAR);

        ctx.stateManager().SetTextureMatrixIdentity(0);
        ctx.stateManager().SetTextureTransformFlagsTextureStageState(0, D3DTTFF_COUNT2);
        ctx.stateManager().SetTexCoordIndexTextureStageState(0, D3DTSS_TCI_PASSTHRU);
        switch (mcache->m_mat.m_uTiling)
        {
          case OdGiMapper::kCrop:
            ctx.stateManager().SetAddressUTextureStageState(0, D3DTADDRESS_BORDER);
            ctx.stateManager().SetBorderColorTextureStageState(0, 0); // transparent border
          break;
          case OdGiMapper::kClamp:
            ctx.stateManager().SetAddressUTextureStageState(0, D3DTADDRESS_CLAMP);
          break;
          case OdGiMapper::kMirror:
            ctx.stateManager().SetAddressUTextureStageState(0, D3DTADDRESS_MIRROR);
          break;
          default:
            ctx.stateManager().SetAddressUTextureStageState(0, D3DTADDRESS_WRAP);
        }
        switch (mcache->m_mat.m_vTiling)
        {
          case OdGiMapper::kCrop:
            ctx.stateManager().SetAddressVTextureStageState(0, D3DTADDRESS_BORDER);
            ctx.stateManager().SetBorderColorTextureStageState(0, 0); // transparent border
          break;
          case OdGiMapper::kClamp:
            ctx.stateManager().SetAddressVTextureStageState(0, D3DTADDRESS_CLAMP);
          break;
          case OdGiMapper::kMirror:
            ctx.stateManager().SetAddressVTextureStageState(0, D3DTADDRESS_MIRROR);
          break;
          default:
            ctx.stateManager().SetAddressVTextureStageState(0, D3DTADDRESS_WRAP);
        }
      }
      else
      {
        ctx.updateMaterialTexture(false);
      }
      if (mcache->m_mat.m_fOpacityPercentage < 1.0)
      {
        ctx.updateTransparencyState(D3dMetafilePlayContext::Transparency_Material, 255 - (OdUInt8)(mcache->m_mat.m_fOpacityPercentage * 255));
      }
      else
      {
        ctx.updateTransparencyState(D3dMetafilePlayContext::Transparency_Material, 0);
      }
      bad = false;
    }
  }
  if (bad)
  {
    ctx.intMtlPtr()->release();
    setupDefaultMaterialToDevice(&(ctx.stateManager()));
    ctx.updateMaterialTexture(false);
    ctx.updateTransparencyState(D3dMetafilePlayContext::Transparency_Material, 0);
  }
}


void D3dMaterialRecord::play4Select( const D3dMetafilePlayContext& /*pContext*/, D3dPlay4SelectContext& /*ctx*/ ) const
{
  //Nothing
}


D3dMaterialRecord* D3dMaterialRecord::createObject(OdGiMaterialItemPtr pMatItem)
{
  return new D3dMaterialRecord(pMatItem);
}


//Selection Mode record
D3dSelectionModeRecord::D3dSelectionModeRecord( OdUInt8 flags )
{
  m_flags = flags;
}


void D3dSelectionModeRecord::play(const D3dMetafilePlayContext& pContext) const
{
  if( pContext.getSubselectionFlags() != m_flags )
  {
    pContext.setSubselectionFlags( m_flags );
  }
}


void D3dSelectionModeRecord::play4Select( const D3dMetafilePlayContext& pContext, D3dPlay4SelectContext& ctx ) const
{
  if( pContext.getSubselectionFlags() != m_flags )
  {
    pContext.setSubselectionFlags( m_flags );
  }
}


D3dSelectionModeRecord* D3dSelectionModeRecord::createObject( OdUInt8 flags )
{
  return new D3dSelectionModeRecord( flags );
}


void ExGiDirectXGeometry::d3dSelectionFlags(OdUInt8 selFlags)
{
//  D3dRecord *pSelModeRec = NULL;
//  pSelModeRec = D3dSelectionModeRecord::createObject(selFlags);
//  ODA_ASSERT(pSelModeRec != NULL);
  m_mfPlayCtx.setContextModified( true );
  if (m_rPackager.isInitialized())
  {
    m_rPackager.setSelectionFlag( selFlags );
  }
  else
  {
    D3dRecord *pSelModeRec = NULL;
    pSelModeRec = D3dSelectionModeRecord::createObject(selFlags);
    ODA_ASSERT(pSelModeRec != NULL);
    pSelModeRec->play(m_mfPlayCtx);
    delete pSelModeRec;
  }
}
//
