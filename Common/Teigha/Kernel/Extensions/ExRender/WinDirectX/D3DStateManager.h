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

#ifndef __OD_EX_DIRECT_3D_STATEMANAGER__
#define __OD_EX_DIRECT_3D_STATEMANAGER__

#include "TD_PackPush.h"

/*
  OdD3DStateManager
  - - -
  This class manages Direct3D render states and texture stages on client side. First point of
implementation is optimizing the state changing calls. DirectX interfaces takes some time on
this kind of calls, but if state not changed this time will be lost. Second point - porting to
newer versions of Direct3D interfaces. Some render states and functions may be removed/added
from version to version. This class wraps main part of D3D functionality and can be used in
future porting to DX9/10.
*/

#include "D3DInclude.h"

/* State managing macro */
#define OD_D3DSM_RS_DECLARE(stateFlag, stateName) \
  private: \
    DWORD m_dw##stateName; \
  public: \
    void Set##stateName##RenderState(DWORD value) \
    { \
      if (m_dw##stateName != value) \
      { \
        m_dw##stateName = value; \
        m_pD3DDevice->SetRenderState(stateFlag, value); \
      } \
    } \
    DWORD Get##stateName##RenderState() const \
    { \
      return m_dw##stateName; \
    }
#ifndef OD_USE_DIRECT3D9
#define OD_D3DSM_RS_DECLARE9(stateFlag, stateName) \
  public: \
    void Set##stateName##RenderState(DWORD) \
    { \
    } \
    DWORD Get##stateName##RenderState() const \
    { \
      return 0; \
    }
#else
#define OD_D3DSM_RS_DECLARE9(stateFlag, stateName) \
  OD_D3DSM_RS_DECLARE(stateFlag, stateName)
#endif

#define OD_D3DSM_RS_INIT(stateFlag, stateName) \
      m_dw##stateName = 0; \
      m_pD3DDevice->GetRenderState(stateFlag, &m_dw##stateName);
#ifndef OD_USE_DIRECT3D9
#define OD_D3DSM_RS_INIT9(stateFlag, stateName)
#else
#define OD_D3DSM_RS_INIT9(stateFlag, stateName) \
  OD_D3DSM_RS_INIT(stateFlag, stateName)
#endif

/* Texture stage managing macro */
#define OD_D3DSM_TS_NUMTS   4 // Number of processable texture stages

#define OD_D3DSM_TS_DECLARE_FUNC(funcName, nStages, stateFlag, stateName) \
  private: \
    DWORD m_dwTS##stateName[nStages]; \
  public: \
    void Set##stateName##TextureStageState(DWORD stage, DWORD value) \
    { \
      if (m_dwTS##stateName[stage] != value) \
      { \
        m_dwTS##stateName[stage] = value; \
        m_pD3DDevice->funcName(stage, stateFlag, value); \
      } \
    } \
    DWORD Get##stateName##TextureStageState(DWORD stage) const \
    { \
      return m_dwTS##stateName[stage]; \
    }
#define OD_D3DSM_TS_DECLARE(nStages, stateFlag, stateName) \
  OD_D3DSM_TS_DECLARE_FUNC(SetTextureStageState, nStages, stateFlag, stateName)
#ifndef OD_USE_DIRECT3D9
#define OD_D3DSM_TS_DECLARE9(nStages, tsStateFlag, sampStateFlag, stateName) \
  OD_D3DSM_TS_DECLARE(nStages, tsStateFlag, stateName)
#else
#define OD_D3DSM_TS_DECLARE9(nStages, tsStateFlag, sampStateFlag, stateName) \
  OD_D3DSM_TS_DECLARE_FUNC(SetSamplerState, nStages, sampStateFlag, stateName)
#endif

#define OD_D3DSM_TS_INIT_FUNC(funcName, nStages, stateFlag, stateName) \
      for (itState = 0; itState < nStages; itState++) \
      { \
        m_dwTS##stateName[itState] = 0; \
        m_pD3DDevice->funcName(itState, stateFlag, &(m_dwTS##stateName[itState])); \
      }
#define OD_D3DSM_TS_INIT(nStages, stateFlag, stateName) \
  OD_D3DSM_TS_INIT_FUNC(GetTextureStageState, nStages, stateFlag, stateName)
#ifndef OD_USE_DIRECT3D9
#define OD_D3DSM_TS_INIT9(nStages, tsStateFlag, sampStateFlag, stateName) \
  OD_D3DSM_TS_INIT(nStages, tsStateFlag, stateName)
#else
#define OD_D3DSM_TS_INIT9(nStages, tsStateFlag, sampStateFlag, stateName) \
  OD_D3DSM_TS_INIT_FUNC(GetSamplerState, nStages, sampStateFlag, stateName)
#endif

class D3dLostStateManager;

/** \details
  Class manages Direct3D render states and texture stages on client side.

  <group ExRender_Windows_Classes> 
*/
class OdD3DStateManager {
  protected:
    IDirect3DDevice *m_pD3DDevice;
    D3dLostStateManager *m_pLSManager;
  public:
    OdD3DStateManager()
      : m_pD3DDevice(NULL)
      , m_pLSManager(NULL)
    {
    }

    void SetDevice(IDirect3DDevice *pDev)
    {
      m_pD3DDevice = pDev;
    }
    IDirect3DDevice *GetDevice() const
    {
      return m_pD3DDevice;
    }
    bool IsDevice() const
    {
      return m_pD3DDevice != NULL;
    }

    void SetLostStateManager(D3dLostStateManager *pLSManager)
    {
      m_pLSManager = pLSManager;
    }
    D3dLostStateManager *GetLostStateManager() const
    {
      return m_pLSManager;
    }

    // Render states

    OD_D3DSM_RS_DECLARE(D3DRS_CULLMODE, CullMode);
    OD_D3DSM_RS_DECLARE(D3DRS_LIGHTING, Lighting);
    OD_D3DSM_RS_DECLARE(D3DRS_ZFUNC, ZFunc);
    OD_D3DSM_RS_DECLARE(D3DRS_ZENABLE, ZEnable);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILENABLE, StencilEnable);
    OD_D3DSM_RS_DECLARE(D3DRS_SRCBLEND, SrcBlend);
    OD_D3DSM_RS_DECLARE(D3DRS_DESTBLEND, DestBlend);
    OD_D3DSM_RS_DECLARE(D3DRS_ALPHATESTENABLE, AlphaTestEnable);
    OD_D3DSM_RS_DECLARE(D3DRS_ZWRITEENABLE, ZWriteEnable);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILFUNC, StencilFunc);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILREF, StencilRef);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILMASK, StencilMask);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILWRITEMASK, StencilWriteMask);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILZFAIL, StencilZFail);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILFAIL, StencilFail);
    OD_D3DSM_RS_DECLARE(D3DRS_STENCILPASS, StencilPass);
    OD_D3DSM_RS_DECLARE(D3DRS_ALPHABLENDENABLE, AlphaBlendEnable);
    OD_D3DSM_RS_DECLARE(D3DRS_NORMALIZENORMALS, NormalizeNormals);
    OD_D3DSM_RS_DECLARE(D3DRS_AMBIENT, Ambient);
    OD_D3DSM_RS_DECLARE(D3DRS_SPECULARENABLE, SpecularEnable);
    OD_D3DSM_RS_DECLARE(D3DRS_LOCALVIEWER, LocalViewer);
    OD_D3DSM_RS_DECLARE(D3DRS_AMBIENTMATERIALSOURCE, AmbientMaterialSource);
    OD_D3DSM_RS_DECLARE(D3DRS_DIFFUSEMATERIALSOURCE, DiffuseMaterialSource);
    OD_D3DSM_RS_DECLARE(D3DRS_SPECULARMATERIALSOURCE, SpecularMaterialSource);
    OD_D3DSM_RS_DECLARE(D3DRS_EMISSIVEMATERIALSOURCE, EmissiveMaterialSource);
    OD_D3DSM_RS_DECLARE(D3DRS_ALPHAFUNC, AlphaFunc);
    OD_D3DSM_RS_DECLARE(D3DRS_ALPHAREF, AlphaRef);
    OD_D3DSM_RS_DECLARE(D3DRS_TEXTUREFACTOR, TextureFactor);
    OD_D3DSM_RS_DECLARE9(D3DRS_SLOPESCALEDEPTHBIAS, SlopeScaleDepthBias);
    OD_D3DSM_RS_DECLARE9(D3DRS_DEPTHBIAS, DepthBias);
    OD_D3DSM_RS_DECLARE9(D3DRS_ANTIALIASEDLINEENABLE, AntiAliasedLineEnable);

  protected:
    void InitRenderStates()
    {
      OD_D3DSM_RS_INIT(D3DRS_CULLMODE, CullMode);
      OD_D3DSM_RS_INIT(D3DRS_LIGHTING, Lighting);
      OD_D3DSM_RS_INIT(D3DRS_ZFUNC, ZFunc);
      OD_D3DSM_RS_INIT(D3DRS_ZENABLE, ZEnable);
      OD_D3DSM_RS_INIT(D3DRS_STENCILENABLE, StencilEnable);
      OD_D3DSM_RS_INIT(D3DRS_SRCBLEND, SrcBlend);
      OD_D3DSM_RS_INIT(D3DRS_DESTBLEND, DestBlend);
      OD_D3DSM_RS_INIT(D3DRS_ALPHATESTENABLE, AlphaTestEnable);
      OD_D3DSM_RS_INIT(D3DRS_ZWRITEENABLE, ZWriteEnable);
      OD_D3DSM_RS_INIT(D3DRS_ZFUNC, ZFunc);
      OD_D3DSM_RS_INIT(D3DRS_STENCILFUNC, StencilFunc);
      OD_D3DSM_RS_INIT(D3DRS_STENCILREF, StencilRef);
      OD_D3DSM_RS_INIT(D3DRS_STENCILMASK, StencilMask);
      OD_D3DSM_RS_INIT(D3DRS_STENCILWRITEMASK, StencilWriteMask);
      OD_D3DSM_RS_INIT(D3DRS_STENCILZFAIL, StencilZFail);
      OD_D3DSM_RS_INIT(D3DRS_STENCILFAIL, StencilFail);
      OD_D3DSM_RS_INIT(D3DRS_STENCILPASS, StencilPass);
      OD_D3DSM_RS_INIT(D3DRS_ALPHABLENDENABLE, AlphaBlendEnable);
      OD_D3DSM_RS_INIT(D3DRS_NORMALIZENORMALS, NormalizeNormals);
      OD_D3DSM_RS_INIT(D3DRS_AMBIENT, Ambient);
      OD_D3DSM_RS_INIT(D3DRS_SPECULARENABLE, SpecularEnable);
      OD_D3DSM_RS_INIT(D3DRS_LOCALVIEWER, LocalViewer);
      OD_D3DSM_RS_INIT(D3DRS_AMBIENTMATERIALSOURCE, AmbientMaterialSource);
      OD_D3DSM_RS_INIT(D3DRS_DIFFUSEMATERIALSOURCE, DiffuseMaterialSource);
      OD_D3DSM_RS_INIT(D3DRS_SPECULARMATERIALSOURCE, SpecularMaterialSource);
      OD_D3DSM_RS_INIT(D3DRS_EMISSIVEMATERIALSOURCE, EmissiveMaterialSource);
      OD_D3DSM_RS_INIT(D3DRS_ALPHAFUNC, AlphaFunc);
      OD_D3DSM_RS_INIT(D3DRS_ALPHAREF, AlphaRef);
      OD_D3DSM_RS_INIT(D3DRS_TEXTUREFACTOR, TextureFactor);
      OD_D3DSM_RS_INIT9(D3DRS_SLOPESCALEDEPTHBIAS, SlopeScaleDepthBias);
      OD_D3DSM_RS_INIT9(D3DRS_DEPTHBIAS, DepthBias);
      OD_D3DSM_RS_INIT9(D3DRS_ANTIALIASEDLINEENABLE, AntiAliasedLineEnable);
    }

    // Texture stage states

    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_COLOROP, ColorOp);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_COLORARG1, ColorArg1);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_COLORARG2, ColorArg2);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_ALPHAOP, AlphaOp);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_ALPHAARG1, AlphaArg1);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_ALPHAARG2, AlphaArg2);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_TEXCOORDINDEX, TexCoordIndex);
    OD_D3DSM_TS_DECLARE9(OD_D3DSM_TS_NUMTS, D3DTSS_ADDRESSU, D3DSAMP_ADDRESSU, AddressU);
    OD_D3DSM_TS_DECLARE9(OD_D3DSM_TS_NUMTS, D3DTSS_ADDRESSV, D3DSAMP_ADDRESSV, AddressV);
    OD_D3DSM_TS_DECLARE9(OD_D3DSM_TS_NUMTS, D3DTSS_BORDERCOLOR, D3DSAMP_BORDERCOLOR, BorderColor);
    OD_D3DSM_TS_DECLARE9(OD_D3DSM_TS_NUMTS, D3DTSS_MAGFILTER, D3DSAMP_MAGFILTER, MagFilter);
    OD_D3DSM_TS_DECLARE9(OD_D3DSM_TS_NUMTS, D3DTSS_MINFILTER, D3DSAMP_MINFILTER, MinFilter);
    OD_D3DSM_TS_DECLARE(OD_D3DSM_TS_NUMTS, D3DTSS_TEXTURETRANSFORMFLAGS, TextureTransformFlags);

  protected:
    void InitTextureStageStates()
    {
      DWORD itState;
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_COLOROP, ColorOp);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_COLORARG1, ColorArg1);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_COLORARG2, ColorArg2);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_ALPHAOP, AlphaOp);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_ALPHAARG1, AlphaArg1);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_ALPHAARG2, AlphaArg2);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_TEXCOORDINDEX, TexCoordIndex);
      OD_D3DSM_TS_INIT9(OD_D3DSM_TS_NUMTS, D3DTSS_ADDRESSU, D3DSAMP_ADDRESSU, AddressU);
      OD_D3DSM_TS_INIT9(OD_D3DSM_TS_NUMTS, D3DTSS_ADDRESSV, D3DSAMP_ADDRESSV, AddressV);
      OD_D3DSM_TS_INIT9(OD_D3DSM_TS_NUMTS, D3DTSS_BORDERCOLOR, D3DSAMP_BORDERCOLOR, BorderColor);
      OD_D3DSM_TS_INIT9(OD_D3DSM_TS_NUMTS, D3DTSS_MAGFILTER, D3DSAMP_MAGFILTER, MagFilter);
      OD_D3DSM_TS_INIT9(OD_D3DSM_TS_NUMTS, D3DTSS_MINFILTER, D3DSAMP_MINFILTER, MinFilter);
      OD_D3DSM_TS_INIT(OD_D3DSM_TS_NUMTS, D3DTSS_TEXTURETRANSFORMFLAGS, TextureTransformFlags);
    }

    // Textures

    private:
      IDirect3DTexture *m_pTexture[OD_D3DSM_TS_NUMTS];
    public:
      void SetTexture(DWORD stage, IDirect3DTexture *pTexture)
      {
        if (m_pTexture[stage] != pTexture)
        {
          m_pTexture[stage] = pTexture;
          m_pD3DDevice->SetTexture(stage, pTexture);
        }
      }
      IDirect3DTexture *GetTexture(DWORD stage) const
      {
        return m_pTexture[stage];
      }

    // Vertex shader

    private:
      DWORD m_dwFVF;
    public:
      void SetFVF(DWORD value)
      {
        if (m_dwFVF != value)
        {
          m_dwFVF = value;
#ifndef OD_USE_DIRECT3D9
          m_pD3DDevice->SetVertexShader(value);
#else
          m_pD3DDevice->SetFVF(value);
#endif
        }
      }
      DWORD GetFVF() const
      {
        return m_dwFVF;
      }

    // Material

    private:
      bool m_bMaterialInitialized;
      D3DMATERIAL m_material;
    public:
      void SetMaterial(const D3DMATERIAL *material)
      {
        if (!m_bMaterialInitialized ||
            m_material.Ambient.r != material->Ambient.r ||
            m_material.Ambient.g != material->Ambient.g ||
            m_material.Ambient.b != material->Ambient.b ||
            m_material.Ambient.a != material->Ambient.a ||
            m_material.Diffuse.r != material->Diffuse.r ||
            m_material.Diffuse.g != material->Diffuse.g ||
            m_material.Diffuse.b != material->Diffuse.b ||
            m_material.Diffuse.a != material->Diffuse.a ||
            m_material.Specular.r != material->Specular.r ||
            m_material.Specular.g != material->Specular.g ||
            m_material.Specular.b != material->Specular.b ||
            m_material.Specular.a != material->Specular.a ||
            m_material.Emissive.r != material->Emissive.r ||
            m_material.Emissive.g != material->Emissive.g ||
            m_material.Emissive.b != material->Emissive.b ||
            m_material.Emissive.a != material->Emissive.a ||
            m_material.Power != material->Power) // Takes some time
        {
          m_bMaterialInitialized = true;
          m_material = *material;
          m_pD3DDevice->SetMaterial(material);
        }
      }
      const D3DMATERIAL &GetMaterial() const
      {
        return m_material;
      }

    private:
      bool m_bTexMatIdentity[OD_D3DSM_TS_NUMTS];
    public:
      void SetTextureMatrix(DWORD stage, const D3DMATRIX *pMat)
      {
        m_pD3DDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage), pMat);
        m_bTexMatIdentity[stage] = false;
      }
      void SetTextureMatrixIdentity(DWORD stage)
      {
        if (!m_bTexMatIdentity[stage])
        {
          m_bTexMatIdentity[stage] = true;
          D3DXMATRIX d3dM(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
          m_pD3DDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage), &d3dM);
        }
      }

      void Init()
      {
        DWORD it;
        InitRenderStates();
        InitTextureStageStates();
        for (it = 0; it < OD_D3DSM_TS_NUMTS; it++)
          m_pTexture[it] = NULL;
#ifndef OD_USE_DIRECT3D9
        m_pD3DDevice->GetVertexShader(&m_dwFVF);
#else
        m_pD3DDevice->GetFVF(&m_dwFVF);
#endif
        m_bMaterialInitialized = false;
        for (it = 0; it < OD_D3DSM_TS_NUMTS; it++)
          m_bTexMatIdentity[it] = true;
      }

    // Not supported currently:
    // - - -
    // SetLight
    // LightEnable
};

#include "TD_PackPop.h"

#endif __OD_EX_DIRECT_3D_STATEMANAGER__
