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

#include "BaseMaterialAndColorHelper.h"
#include "Br/BrEdge.h"
#include "Br/BrFace.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiMaterialTraitsData.h"


// Product specific functions
namespace ProductSpecific
{
  enum ProductFlags
  {
    fFaceHasColor = 1 << 1,// if true return face color to brepbuilder, else face color will be in material
    fFaceHasMaterialMapping = 1 << 2,// if true return material mapping to brepbuilder, else material mapping will be in material
    fEdgeHasColor = 1 << 3,// if true return edge color to brepbuilder, else edge color will be ignored
  };
  enum ProductTypes
  {
    kUnknown = 0,
    kDwg = 1 | fFaceHasColor  | fFaceHasMaterialMapping | fEdgeHasColor,
    kDgn = 1 | 0              | 0                       | 0,
    kPrc = 1 | 0              | 0                       | 0,
    kBim = 1 | 0              | 0                       | 0
  };

  ProductTypes getProduct(const OdDbBaseDatabasePtr& pDb)
  {
    OdString className(pDb->isA()->name());

    if (!className.compare("OdDbDatabase")) return kDwg;
    if (!className.compare("OdDbDatabaseDoc")) return kDwg;
    if (!className.compare("OdDgDatabase")) return kDgn;
    if (!className.compare("OdPrcFile")) return kPrc;
    if (!className.compare("OdBmDatabase")) return kBim;

    return kUnknown;
  }
}


OdBaseMaterialAndColorHelper::OdBaseMaterialAndColorHelper(
  OdDbBaseDatabasePtr pSourceDb,
  OdDbBaseDatabasePtr pDestinationDb,
  const OdMaterialResolver* pMaterialResolver,
  OdDbStub* pDefaultMaterial
)
  : m_pSourceDb(pSourceDb)
  , m_pDestinationDb(pDestinationDb)
  , m_pSourceMaterialResolver(pMaterialResolver)
  , m_pSourceGiContext((OdGiDefaultContext*)NULL)
  , m_pSourceEntityMaterial(pDefaultMaterial)
  , m_hasSourceEntityMaterialMapping(false)
  , m_hasSourceEntityFaceColor(false)
  , m_sourceEntityFaceColor(OdCmEntityColor::kNone)
  , m_hasSourceEntityEdgeColor(false)
  , m_sourceEntityEdgeColor(OdCmEntityColor::kNone)
{
  if (!pSourceDb.isNull())
  {
    OdDbBaseDatabasePEPtr pSourceDbPE = OdDbBaseDatabasePE::cast(pSourceDb);
    ODA_ASSERT(!pSourceDbPE.isNull());
    m_pSourceGiContext = pSourceDbPE->createGiContext(pSourceDb);
  }
}

OdBaseMaterialAndColorHelper::~OdBaseMaterialAndColorHelper()
{
}

OdDbStub * OdBaseMaterialAndColorHelper::findMaterialInUserCache(const MaterialDataStore & matData)
{
  return NULL;
}

void OdBaseMaterialAndColorHelper::addMaterialToUserCache(const MaterialDataStore & matData)
{
}

OdResult OdBaseMaterialAndColorHelper::getFaceVisualInfo(const OdBrFace& face,
  OdDbStub*& faceMaterial,
  OdGiMapper& faceMaterialMapping, bool& applyFaceMaterialMapping,
  OdCmEntityColor& faceColor, bool& applyFaceColor)
{
  // init
  faceMaterial = 0;
  faceMaterialMapping = OdGiMapper::kIdentity;
  applyFaceMaterialMapping = false;
  faceColor = OdCmEntityColor::kNone;
  applyFaceColor = false;

  OdResult status = eOk;

  // check db
  if (m_pSourceDb.isNull() || m_pDestinationDb.isNull())
  {
    return eOk;
  }

  OdDbBaseDatabasePEPtr pSourceDbPE(m_pSourceDb);
  ODA_ASSERT(!pSourceDbPE.isNull());
  OdDbBaseDatabasePEPtr pDestinitionDbPE(m_pDestinationDb);
  ODA_ASSERT(!pDestinitionDbPE.isNull());

  ProductSpecific::ProductTypes sourceType = ProductSpecific::getProduct(m_pSourceDb);
  ProductSpecific::ProductTypes destinitionType = ProductSpecific::getProduct(m_pDestinationDb);
  if (ProductSpecific::kUnknown == sourceType || ProductSpecific::kUnknown == destinitionType)
  {
    return eNotImplemented;
  }

  // source info
  OdDbStub* pSourceMaterial = NULL;
  status = getSourceMaterialId(face, pSourceMaterial);
  if (eOk != status)
  {
    return status;
  }
  OdGiMapper sourceMaterialMapping;
  bool hasSourceMaterialMapping = getSourceMaterialMapper(face, sourceMaterialMapping);
  OdCmEntityColor sourceFaceColor = OdCmEntityColor::kNone;
  bool hasSourceFaceColor = getSourceFaceColor(face, sourceFaceColor);
  if (!pSourceMaterial
    && !hasSourceMaterialMapping
    && !hasSourceFaceColor)
  {
    return eOk;
  }

  // same product => use source as out info
  if (sourceType == destinitionType)
  {
    faceMaterial = pSourceMaterial;
    if (GETBIT(destinitionType, ProductSpecific::fFaceHasMaterialMapping)
      && hasSourceMaterialMapping)
    {
      applyFaceMaterialMapping = true;
      faceMaterialMapping = sourceMaterialMapping;
    }
    if (GETBIT(destinitionType, ProductSpecific::fFaceHasColor)
      && hasSourceFaceColor)
    {
      applyFaceColor = true;
      faceColor = sourceFaceColor;
    }

    return eOk;
  }

  // different product => need convertation
  MaterialDataStore materialInfo;
  materialInfo.sourceMaterial = pSourceMaterial;

  OdGiMapper* pMaterialMapper = NULL;
  if (hasSourceMaterialMapping)
  {
    if (GETBIT(destinitionType, ProductSpecific::fFaceHasMaterialMapping))
    {
      applyFaceMaterialMapping = true;
      faceMaterialMapping = sourceMaterialMapping;
    }
    else
    {
      materialInfo.sourceMapper = sourceMaterialMapping;
      pMaterialMapper = &materialInfo.sourceMapper;
    }
  }

  OdCmEntityColor* pMaterialColor = NULL;
  if (hasSourceFaceColor)
  {
    if (GETBIT(destinitionType, ProductSpecific::fFaceHasColor))
    {
      applyFaceColor = true;
      faceColor = sourceFaceColor;
    }
    else
    {
      status = convertColor(sourceFaceColor, materialInfo.sourceColor);
      if (eOk != status)
      {
        return status;
      }
      pMaterialColor = &materialInfo.sourceColor;
    }
  }

  MaterialCache::size_type idx = 0;
  if (m_materialCache.find(materialInfo, idx))
  {
    faceMaterial = m_materialCache[idx].materialId;
    return eOk;
  }
  OdDbStub * userMat = findMaterialInUserCache(materialInfo);
  if (userMat)
  {
    faceMaterial = userMat;
    materialInfo.materialId = faceMaterial;
    m_materialCache.append(materialInfo);
    return eOk;
  }

  // internal material
  OdGiMaterialTraitsTaker materialTraits;
  OdGiMaterialTraitsTaker* pMaterialTraits = NULL;
  if (pSourceMaterial)
  {
    OdGiDrawablePtr pGiMaterial = pSourceDbPE->openObject(pSourceMaterial);
    if (pGiMaterial.isNull())
    {
      return eNullObjectPointer;
    }

    OdUInt32 flags = pGiMaterial->setAttributes(&materialTraits);
    pMaterialTraits = &materialTraits;
  }

  status = eOk;
  if (pMaterialTraits || pMaterialMapper || pMaterialColor)
  {
    status = pDestinitionDbPE->createMaterial(faceMaterial, m_pSourceDb, m_pDestinationDb, pMaterialTraits, pMaterialMapper, pMaterialColor);
    if (eOk == status && faceMaterial)
    {
      materialInfo.materialId = faceMaterial;
      m_materialCache.append(materialInfo);
      addMaterialToUserCache(materialInfo);
    }
  }

  return status;
}

OdResult OdBaseMaterialAndColorHelper::getEdgeVisualInfo(const OdBrEdge& edge, OdCmEntityColor& edgeColor, bool& applyEdgeColor)
{
  edgeColor = OdCmEntityColor::kNone;
  applyEdgeColor = false;

  // check db
  if (m_pSourceDb.isNull() || m_pDestinationDb.isNull())
  {
    return eOk;
  }

  OdDbBaseDatabasePEPtr pSourceDbPE(m_pSourceDb);
  ODA_ASSERT(!pSourceDbPE.isNull());
  OdDbBaseDatabasePEPtr pDestinitionDbPE(m_pDestinationDb);
  ODA_ASSERT(!pDestinitionDbPE.isNull());

  ProductSpecific::ProductTypes sourceType = ProductSpecific::getProduct(m_pSourceDb);
  ProductSpecific::ProductTypes destinitionType = ProductSpecific::getProduct(m_pDestinationDb);
  if (ProductSpecific::kUnknown == sourceType || ProductSpecific::kUnknown == destinitionType)
  {
    return eNotImplemented;
  }

  if (!GETBIT(destinitionType, ProductSpecific::fEdgeHasColor))
  {
    return eOk;
  }

  OdCmEntityColor sourceEdgeColor;
  applyEdgeColor = getSourceEdgeColor(edge, sourceEdgeColor);
  if (!applyEdgeColor)
  {
    return eOk;
  }

  // same product => use same color
  if (sourceType == destinitionType)
  {
    edgeColor = sourceEdgeColor;
    return eOk;
  }

  // different product => convert
  return convertColor(sourceEdgeColor, edgeColor);
}

OdResult OdBaseMaterialAndColorHelper::convertColor(
  const OdCmEntityColor& sourceDbColor,
  OdCmEntityColor& destinationDbColor
)
{
  return eNotImplemented;
}


// Getter of source visual info:

OdResult OdBaseMaterialAndColorHelper::getSourceMaterialId(const OdBrFace& face, OdDbStub*& material) const
{
  material = m_pSourceEntityMaterial;

  if (m_pSourceMaterialResolver)
  {
    OdUInt64 matId = 0;
    if (face.getMaterialID(matId))
    {
      return m_pSourceMaterialResolver->resolveMaterial(matId, material);
    }
  }

  if (!m_pSourceGiContext.isNull())
  {
    OdString matName;
    if (face.getMaterialString(matName))
    {
      material = m_pSourceGiContext->getStubByMatName(matName);
      return eOk;
    }
  }

  return eOk;
}

bool OdBaseMaterialAndColorHelper::getSourceMaterialMapper(const OdBrFace& face, OdGiMapper& materialMapper) const
{
  OdGeMatrix3d mx;
  OdBrFace::Projection projection;
  OdBrFace::Tiling tiling;
  OdBrFace::AutoTransform autoTransform;

  bool rc = face.getMaterialMapper(mx, projection, tiling, autoTransform);

  if (rc)
  {
    materialMapper = OdGiMapper();
    materialMapper.setProjection(OdGiMapper::Projection(projection));
    materialMapper.setUTiling(OdGiMapper::Tiling(tiling));
    materialMapper.setVTiling(OdGiMapper::Tiling(tiling));
    materialMapper.setAutoTransform(OdGiMapper::AutoTransform(autoTransform));
    materialMapper.transform() = mx;
  }
  else if (m_hasSourceEntityMaterialMapping)
  {
    materialMapper = m_sourceEntityMaterialMapping;
    rc = true;
  }

  return rc;
}

bool OdBaseMaterialAndColorHelper::getSourceFaceColor(const OdBrFace& face, OdCmEntityColor& color) const
{
  bool rc = face.getColor(color);
  if (!rc && m_hasSourceEntityFaceColor)
  {
    color = m_sourceEntityFaceColor;
    rc = true;
  }

  return rc;
}

bool OdBaseMaterialAndColorHelper::getSourceEdgeColor(const OdBrEdge& edge, OdCmEntityColor& color) const
{
  bool rc = edge.getColor(color);
  if (!rc && m_hasSourceEntityEdgeColor)
  {
    color = m_sourceEntityEdgeColor;
    rc = true;
  }

  return rc;
}
