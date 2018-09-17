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
// TrGL2 renderer local client

#include "OdaCommon.h"
#include "Gi/GiPlotStyleDefs.h"
#include "Gs/GsHighlightData.h"
#include "ExGsGLES2LocalSharingProvider.h"
#include "ExGsGLES2LocalRenditionGsClient.h"

// OdTrGL2HlBranchGsClient

const void *OdTrGL2HlBranchGsClient::hlBranchGetPtr(OdTrVisHlBranchId hlBranchId) const
{
  return OdTrVisIdToPtr(void *, hlBranchId);
}

bool OdTrGL2HlBranchGsClient::hlBranchIsEmpty(const void *pHlBranchIn) const
{ const OdGsHlBranch *pHlBranch = reinterpret_cast<const OdGsHlBranch*>(pHlBranchIn);
  return pHlBranch->isEmpty();
}

bool OdTrGL2HlBranchGsClient::hlBranchHasMarkers(const void *pHlBranchIn) const
{ const OdGsHlBranch *pHlBranch = reinterpret_cast<const OdGsHlBranch*>(pHlBranchIn);
  return !pHlBranch->markers().isEmpty();
}

const void *OdTrGL2HlBranchGsClient::hlBranchGetChild(const void *pHlBranchIn, OdDbStub *pChildId) const
{ const OdGsHlBranch *pHlBranch = reinterpret_cast<const OdGsHlBranch*>(pHlBranchIn);
  return const_cast<OdGsHlBranch*>(pHlBranch)->findChild(pChildId);
}

bool OdTrGL2HlBranchGsClient::hlBranchHasMarker(const void *pHlBranchIn, OdTrVisGsMarker gsMarker) const
{ const OdGsHlBranch *pHlBranch = reinterpret_cast<const OdGsHlBranch*>(pHlBranchIn);
  return pHlBranch->hasMarker((OdGsMarker)gsMarker);
}

OdTrGL2HlBranchClientPtr OdTrGL2HlBranchGsClient::createObject()
{
  return OdRxObjectImpl<OdTrGL2HlBranchGsClient, OdTrGL2HlBranchClient>::createObject();
}

// OdTrGL2LocalRenditionGsClient

OdTrGL2LocalContextPtr OdTrGL2LocalRenditionGsClient::createLocalContext(OdTrVisRenderClient *pDevice)
{
  return OdTrGL2LocalContext::createLocalContext(pDevice);
}

OdTrGL2HlBranchClientPtr OdTrGL2LocalRenditionGsClient::hlBranchClient()
{
  if (m_pHlBranchClient.isNull())
    m_pHlBranchClient = OdTrGL2HlBranchGsClient::createObject();
  return m_pHlBranchClient;
}

OdTrVisSharingProviderClientPtr OdTrGL2LocalRenditionGsClient::createSharingProvider()
{
  return OdRxObjectImpl<OdTrGL2LocalSharingProvider, OdTrVisSharingProviderClient>::createObject();
}

bool OdTrGL2LocalRenditionGsClient::generateLinetypeTexture(OdPs::LineType ltpId, OdUInt8Array& outPixels, OdUInt8 forePixVal) const
{
  outPixels = ::odgiGetPsLinetypesManager().rasterizeLinetype(ltpId, 2, forePixVal);
  return true;
}

bool OdTrGL2LocalRenditionGsClient::generateFilltypeTexture(OdPs::FillStyle fsId, OdUInt8Array& outPixels, OdUInt32 dimX, OdUInt32 dimY, OdUInt8 forePixVal) const
{
  outPixels = ::odgiGetPsFillstylesManager().rasterizeFillstyle(fsId, 0, 0, dimX, dimY, forePixVal);
  return true;
}

OdTrGL2LocalRenditionClientPtr OdTrGL2LocalRenditionGsClient::createObject()
{
  return OdRxObjectImpl<OdTrGL2LocalRenditionGsClient, OdTrGL2LocalRenditionClient>::createObject();
}

//
