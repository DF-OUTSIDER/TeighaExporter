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

#include "ColladaModule.h"
#include "ColladaExportView.h"
#include "ColladaLightTraits.h"
#include "RxRasterServices.h"
#include "DynamicLinker.h"
#include "Ge/GeScale3d.h"

namespace TD_COLLADA_EXPORT
{
  StubVectorizeDevice* OdColladaOut::device() 
  { 
    return static_cast<StubVectorizeDevice*>(OdGsBaseVectorizeView::device()); 
  }


  //----------------------OdColladaOut---------------------------------------
  bool OdColladaOut::doDraw(OdUInt32 i, const OdGiDrawable* pDrawable)
  {
    switch(pDrawable->drawableType())
    {
    case OdGiDrawable::kDistantLight:
      {
        OdGiDistantLightTraitsData m_pLightTraits;
        OdSmartPtr<ColladaDistantLightTraits> pTr = OdRxObjectImpl<ColladaDistantLightTraits>::createObject();
        pTr->setData(m_pLightTraits);
        pDrawable->setAttributes(pTr);
        m_pLightExp->exportLight(++m_iLightCounter, &m_pLightTraits, this->device()->getPalette());
        break;
      }
    case OdGiDrawable::kPointLight:
      {
        OdGiPointLightTraitsData m_pLightTraits;
        OdSmartPtr<ColladaPointLightTraits> pTr = OdRxObjectImpl<ColladaPointLightTraits>::createObject();
        pTr->setData(m_pLightTraits);
        pDrawable->setAttributes(pTr);
        m_pLightExp->exportLight(++m_iLightCounter, &m_pLightTraits, this->device()->getPalette());
        break;
      }
    case OdGiDrawable::kSpotLight:
      {
        OdGiSpotLightTraitsData m_pLightTraits;
        OdSmartPtr<ColladaSpotLightTraits> pTr = OdRxObjectImpl<ColladaSpotLightTraits>::createObject();
        pTr->setData(m_pLightTraits);
        pDrawable->setAttributes(pTr);
        m_pLightExp->exportLight(++m_iLightCounter, &m_pLightTraits, this->device()->getPalette());
        break;
      }
    }

    //Different shells of one object should be one mesh in the .dae file.
    //The exception if they have different materials. One mesh - one material
    if (pDrawable->isPersistent())
    {
      SETBIT(m_flags, kAddEntity, true);
    }
    return OdGsBaseMaterialView::doDraw(i, pDrawable);
  }

  void OdColladaOut::setMode(OdGsView::RenderMode mode)
  {
    OdGsBaseVectorizeView::m_renderMode = kGouraudShaded;
    m_regenerationType = kOdGiRenderCommand;
    OdGiGeometrySimplifier::m_renderMode = OdGsBaseVectorizeView::m_renderMode;
  }

  void OdColladaOut::circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal) {}
  void OdColladaOut::circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint) {}

  void OdColladaOut::circularArc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector, double sweepAngle, OdGiArcType arcType) {}

  void OdColladaOut::circularArc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint, OdGiArcType arcType) {}

  void OdColladaOut::polyline(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal, OdGsMarker baseSubEntMarker) {}

  void OdColladaOut::polygon(OdInt32 numVertices, const OdGePoint3d* vertexList) {}

  void OdColladaOut::pline(const OdGiPolyline& polyline, OdUInt32 fromIndex, OdUInt32 numSegs){}

  void OdColladaOut::xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint) {}
  void OdColladaOut::ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint ) {}

  void OdColladaOut::nurbs(const OdGeNurbCurve3d& nurbsCurve) {}

  void OdColladaOut::ellipArc(const OdGeEllipArc3d& ellipArc, const OdGePoint3d* endPointsOverrides, OdGiArcType arcType) {}

  void OdColladaOut::worldLine(const OdGePoint3d points[2]) {}

  void OdColladaOut::edge(const OdGiEdge2dArray& edges) {}

  void OdColladaOut::polylineOut(OdInt32 , const OdGePoint3d* ) {}

  OdGiMapperItemPtr OdColladaOut::setMapper(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v, const OdGiRasterImage* pImage)
  {
    OdGeMatrix3d world2uvInvU;
    OdGeVector3d normal = u.crossProduct(v);
    world2uvInvU.setCoordSystem(origin, u, v, normal);
    world2uvInvU.invert();
    OdGeMatrix3d uv2uvNorm;
    OdUInt32 iPixelWidth  = pImage->pixelWidth();
    OdUInt32 iPixelHeight = pImage->pixelHeight();
    uv2uvNorm.setToScaling(OdGeScale3d(1.0 / iPixelWidth, 1.0 / iPixelHeight, 1.0));
    OdGiMapperRenderItemPtr pMpr = OdGiMapperRenderItem::createObject();
    OdGiMapper map1;
    map1.setAutoTransform(OdGiMapper::kNone);
    map1.setProjection(OdGiMapper::kPlanar);
    map1.setUTiling(OdGiMapper::kClamp);
    map1.setVTiling(OdGiMapper::kClamp);
    OdGiMaterialTraitsData map2;
    pMpr->setDiffuseMapper(&map1, map2);
    pMpr->diffuseMapper()->setInputTransform(uv2uvNorm * world2uvInvU);
    return pMpr;
  }

  void OdColladaOut::processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *pNode)
  {
    if (GETBIT(m_flags, kRasterImage))
      return;
    OdGsBaseMaterialView::processMaterialNode(materialId, pNode);
  }

  void OdColladaOut::uninitTexture()
  {
    SETBIT(m_flags, kRasterImage,    false);
    SETBIT(m_flags, kRasterImageMat, false);
  }

  void OdColladaOut::initTexture( const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                                  const OdGiRasterImage* pImage, bool transparency, double brightness,
                                  double contrast, double fade)
  {
    SETBIT(m_flags, kRasterImage, true);
    MaterialData& pMDatat = m_pColladaMaterialData->m_matDataArr[m_pCurrentColladaEntData->m_iEntMaterial];

    resetCurrentMapper(setMapper(origin, u, v, pImage));

    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (!pRasSvcs.isNull())
    {
      OdString sImageName;
      sImageName.format(OD_T("Image_ID%i.jpg"), m_iCurMaterial);
      if(pRasSvcs->saveRasterImage( pImage, sImageName ))
      {
        pMDatat.sDiffuseFileSource     = sImageName;
        pMDatat.bDiffuseChannelEnabled = true;
        pMDatat.bDiffuseHasTexture     = true;
      }
    }
  }

  void OdColladaOut::rasterImageDc( const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                                    const OdGiRasterImage* pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts, 
                                    bool transparency, double brightness, double contrast, double fade )
  {
    SETBIT(m_flags, kRasterImageMat, true);
    addColladaEntity();
    OdGsBaseMaterialView::rasterImageDc(origin, u, v, pImage, uvBoundary, numBoundPts, transparency, brightness, contrast, fade);
  }

  void OdColladaOut::image(const OdGiImageBGRA32& img,
                           const OdGePoint3d& origin,
                           const OdGeVector3d& uVec,
                           const OdGeVector3d& vVec,
                           OdGiRasterImage::TransparencyMode trpMode)
  {
    SETBIT(m_flags, kRasterImageMat, true);
    addColladaEntity();
    OdGsBaseMaterialView::image(img, origin, uVec, vVec, trpMode);
  }

  void OdColladaOut::shape(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
                           int shapeNumber, const OdGiTextStyle* pTextStyle)
  {
    addColladaEntity();
    OdGsBaseMaterialView::shape(position, normal, direction, shapeNumber, pTextStyle);
  }

  void OdColladaOut::text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
                          double height, double width, double oblique, const OdString& msg)
  {
    addColladaEntity();
    OdGsBaseMaterialView::text(position, normal, direction, height, width, oblique, msg);
  }

  void OdColladaOut::text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
                          const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle) 
  {
    addColladaEntity();
    OdGsBaseMaterialView::text(position, normal, direction, msg, length, raw, pTextStyle);
  }

  void OdColladaOut::mesh(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, const OdGiEdgeData* pEdgeData,
                          const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData) 
  {
    addColladaEntity();
    OdGsBaseMaterialView::mesh(numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData);
  }


  void OdColladaOut::meshProc(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* pVertexList, const OdGiEdgeData* pEdgeData,
                              const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
  {
    OdUInt32 iTmp = numRows * numColumns;
    m_iPtArrSize = m_pCurrentColladaEntData->m_ptArr.size();
    m_pCurrentColladaEntData->m_ptArr.resize(m_iPtArrSize + iTmp);
    m_pCurrentColladaEntData->m_indPtsArr.reserve(m_pCurrentColladaEntData->m_indPtsArr.size() + iTmp*3);
    m_pCurrentColladaEntData->m_indVtxNormArr.reserve(m_pCurrentColladaEntData->m_indVtxNormArr.size() + iTmp*3);
    m_pCurrentColladaEntData->m_normVtxArr.reserve(m_pCurrentColladaEntData->m_normVtxArr.size() + iTmp*3);
    for (OdUInt32 iInd = 0; iInd < iTmp; ++iInd)
    {
      m_pCurrentColladaEntData->m_ptArr[iInd] = *(pVertexList + iInd);
    }
    OdGiGeometrySimplifier::meshProc(numRows, numColumns, pVertexList, pEdgeData, pFaceData, pVertexData);
  }

  void OdColladaOut::shell( OdInt32 numVerts, const OdGePoint3d* pVertexList,
    OdInt32 faceListSize, const OdInt32* pFaceList, const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
  {
    addColladaEntity();
    OdGsBaseMaterialView::shell(numVerts, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  }

  void OdColladaOut::shellProc( OdInt32 numVerts, const OdGePoint3d* pVertexList,
                                OdInt32 faceListSize, const OdInt32* pFaceList, const OdGiEdgeData* pEdgeData,
                                const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
  {
    m_iPtArrSize = m_pCurrentColladaEntData->m_ptArr.size();
    m_pCurrentColladaEntData->m_ptArr.resize(m_iPtArrSize + numVerts);
    m_pCurrentColladaEntData->m_indPtsArr.reserve(m_pCurrentColladaEntData->m_indPtsArr.size() + numVerts*3);
    m_pCurrentColladaEntData->m_indVtxNormArr.reserve(m_pCurrentColladaEntData->m_indVtxNormArr.size() + numVerts*3);
    m_pCurrentColladaEntData->m_normVtxArr.reserve(m_pCurrentColladaEntData->m_normVtxArr.size() + numVerts*3);
    for (OdInt32 iInd = 0; iInd < numVerts; ++iInd)
    {
      m_pCurrentColladaEntData->m_ptArr[iInd + m_iPtArrSize] = *(pVertexList + iInd);
    }
    OdGiGeometrySimplifier::shellProc(numVerts, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  }

  void OdColladaOut::addColladaEntity()
  {
    if (GETBIT(m_flags, kAddEntity))
    {
      ColladaEntDataArray entData;    
      int ind = m_pEntityDataArr->append(entData);
      m_pCurrentColladaEntDataArr = &m_pEntityDataArr->at(ind);
      SETBIT(m_flags, kAddEntity, false);
      m_pCurrentColladaEntData = NULL;
    }
  }


  void OdColladaOut::addColladaEntData()
  {
    ColladaEntData newData;
    newData.m_iId = ++iColladaEntCounter;
    newData.m_iEntMaterial = m_iCurMaterial;
    int ind = m_pCurrentColladaEntDataArr->append(newData);
    m_pCurrentColladaEntData = &m_pCurrentColladaEntDataArr->at(ind);
  }

  void OdColladaOut::fillMaterialCache(MaterialData& mData, const OdGiMaterialTraitsData &materialData)
  {
    OdGiMaterialColor diffuseColor; OdGiMaterialMap diffuseMap;
    OdGiMaterialColor ambientColor;
    OdGiMaterialColor specularColor; OdGiMaterialMap specularMap; double glossFactor;
    double opacityPercentage; OdGiMaterialMap opacityMap;
    double refrIndex; OdGiMaterialMap refrMap;
    
    materialData.diffuse(diffuseColor, diffuseMap);
    materialData.ambient(ambientColor);
    materialData.specular(specularColor, specularMap, glossFactor);
    materialData.opacity(opacityPercentage, opacityMap);
    materialData.refraction(refrIndex, refrMap);
    
    OdGiMaterialMap bumpMap;
    materialData.bump(bumpMap);
    double bumpPercentage = (bumpMap.blendFactor() - 0.5) * 2;

    ODCOLORREF colorDiffuse(0), colorAmbient(0), colorSpecular(0);
    if (diffuseColor.color().colorMethod() == OdCmEntityColor::kByColor)
    {
      colorDiffuse = ODTOCOLORREF(diffuseColor.color());
    }
    else if (diffuseColor.color().colorMethod() == OdCmEntityColor::kByACI)
    {
      colorDiffuse = OdCmEntityColor::lookUpRGB((OdUInt8)diffuseColor.color().colorIndex());
    }
    if (ambientColor.color().colorMethod() == OdCmEntityColor::kByColor)
    {
      colorAmbient = ODTOCOLORREF(ambientColor.color());
    }
    else if (ambientColor.color().colorMethod() == OdCmEntityColor::kByACI)
    {
      colorAmbient = OdCmEntityColor::lookUpRGB((OdUInt8)ambientColor.color().colorIndex());
    }
    if (specularColor.color().colorMethod() == OdCmEntityColor::kByColor)
    {
      colorSpecular = ODTOCOLORREF(specularColor.color());
    }
    else if (specularColor.color().colorMethod() == OdCmEntityColor::kByACI)
    {
      colorSpecular = OdCmEntityColor::lookUpRGB((OdUInt8)specularColor.color().colorIndex());
    }


    OdCmEntityColor color = fixByACI(this->device()->getPalette(), effectiveTraits().trueColor());

    // ambient
    mData.dAmbientColorFactor = ambientColor.factor();
    mData.bAmbientChannelEnabled = true;
    if (ambientColor.method() == OdGiMaterialColor::kOverride)
      mData.ambientColor.set(ODGETRED(colorAmbient) / 255.0, ODGETGREEN(colorAmbient) / 255.0, ODGETBLUE(colorAmbient) / 255.0, 1.0);
    else
      mData.ambientColor.set(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1.0);

    // diffuse
    if (diffuseColor.method() == OdGiMaterialColor::kOverride)
      mData.diffuseColor.set(ODGETRED(colorDiffuse) / 255.0, ODGETGREEN(colorDiffuse) / 255.0, ODGETBLUE(colorDiffuse) / 255.0, 1.0);
    else
      mData.diffuseColor.set(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1.0);
    mData.dDiffuseColorFactor = diffuseColor.factor();
    mData.bDiffuseChannelEnabled = (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseDiffuse)) ? true : false;
    if (mData.bDiffuseChannelEnabled && diffuseMap.source() == OdGiMaterialMap::kFile && !diffuseMap.sourceFileName().isEmpty() )
    {
      mData.bDiffuseHasTexture = true;
      mData.sDiffuseFileSource = diffuseMap.sourceFileName();
    }

    // specular
    if (specularColor.method() == OdGiMaterialColor::kOverride)
      mData.specularColor.set(ODGETRED(colorSpecular) / 255.0, ODGETGREEN(colorSpecular) / 255.0, ODGETBLUE(colorSpecular) / 255.0, 1.0);
    else
      mData.specularColor.set(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1.0);
    mData.dSpecularColorFactor = specularColor.factor();
    mData.bSpecularChannelEnabled = (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseSpecular)) ? true : false;
    if (mData.bSpecularChannelEnabled && specularMap.source() == OdGiMaterialMap::kFile && !specularMap.sourceFileName().isEmpty() )
    {
      mData.bSpecularHasTexture = true;
      mData.sSpecularFileSource = specularMap.sourceFileName();
    }  
    mData.glossFactor = glossFactor;

    // opacity
    mData.dOpacityPercentage = opacityPercentage;
    mData.bOpacityChannelEnabled = (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseOpacity)) ? true : false;
    if (mData.bOpacityChannelEnabled && opacityMap.source() == OdGiMaterialMap::kFile && !opacityMap.sourceFileName().isEmpty() )
    {
      mData.bOpacityHasTexture = true;
      mData.sOpacityFileSource = opacityMap.sourceFileName();
    }  

    // reflection
    mData.dReflectionPercentage = refrIndex;
    mData.bReflectionChannelEnabled = (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseReflection)) ? true : false;
    if (mData.bReflectionChannelEnabled && refrMap.source() == OdGiMaterialMap::kFile && !refrMap.sourceFileName().isEmpty() )
    {
      mData.bReflectionHasTexture = true;
      mData.sReflectionFileSource = refrMap.sourceFileName();
    }  

    // bump
    mData.dBumpPercentage = bumpPercentage;
    mData.bBumpChannelEnabled = (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseBump)) ? true : false;
    if (mData.bBumpChannelEnabled && bumpMap.source() == OdGiMaterialMap::kFile && !bumpMap.sourceFileName().isEmpty() )
    {
      mData.bBumpHasTexture = true;
      mData.sBumpFileSource = bumpMap.sourceFileName();
    }  

    // emission
    mData.dEmissionPercentage =  materialData.selfIllumination();

    // refraction
    mData.bRefractionChannelEnabled = (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseRefraction)) ? 1 : 0;
    mData.dRefractionIndex          = materialData.reflectivity();

    // transclucence
    mData.dTranslucence             = materialData.translucence();
  }



  void OdColladaOut::fillCache(OdUInt64 matId, const MaterialData& matData, unsigned int iStartFind)
  {
    if ((iStartFind > 0 && iStartFind == m_pColladaMaterialData->m_matIdArr.length()) || 
        !m_pColladaMaterialData->m_matIdArr.find(matId, m_iCurMaterial, iStartFind) ||
        GETBIT(m_flags, kRasterImageMat))
    {
      //material numbers 1,2.....
      m_pColladaMaterialData->m_matDataArr.append(matData);
      m_iCurMaterial = m_pColladaMaterialData->m_matIdArr.append(matId);

      addColladaEntData();//a mesh can have one material. If a material added then new entity should start.
    }
    else
    {
      if (matData == m_pColladaMaterialData->m_matDataArr[m_iCurMaterial])
      {
        if (m_pCurrentColladaEntData)
        {
          if (m_pCurrentColladaEntData->m_iEntMaterial != m_iCurMaterial)
          {
            bool bIsColladaEntNotFound = true;
            size_t iColladaEntArrSize = m_pCurrentColladaEntDataArr->size();
            for (unsigned int i = 0; i < iColladaEntArrSize; ++i)
            {
              if (m_pCurrentColladaEntDataArr->at(i).m_iEntMaterial == m_iCurMaterial)
              {
                m_pCurrentColladaEntData = &m_pCurrentColladaEntDataArr->at(i);
                bIsColladaEntNotFound = false;
                continue;
              }
            }

            if (bIsColladaEntNotFound)
              addColladaEntData();
          }
        }
        else
          addColladaEntData();
      }
      else
        fillCache(matId, matData, m_iCurMaterial + 1);
    }
  }


  OdGiMaterialItemPtr OdColladaOut::fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub* materialId, const OdGiMaterialTraitsData & materialData)
  {
    if (!GETBIT(m_flags, kAddEntity))
    {
      MaterialData matData;
      fillMaterialCache(matData, materialData);

      fillCache((OdUInt64)(OdIntPtr)materialId, matData, 0);
    }
    return OdGiMaterialItemPtr();
  }

  void OdColladaOut::meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal)
  {
    if (!currentMapper()->diffuseMapper().isNull())
    {
      if (m_texCoordsArray.size() < (OdUInt32)vertexDataCount())
        m_texCoordsArray.resize((OdUInt32)vertexDataCount());

      if (!vertexData() || !vertexData()->mappingCoords(OdGiVertexData::kAllChannels))
        currentMapper()->diffuseMapper()->mapCoords((OdUInt32)vertexDataCount(), vertexDataList(), m_texCoordsArray.asArrayPtr(),
          4, faceList, OdGiMapperItemEntry::kPolyIt, pNormal, NULL, (vertexData()) ? vertexData()->normals() : NULL,
          (vertexData()) ? vertexData()->orientationFlag() : kOdGiNoOrientation);
      else
      {
        OdGiMapperItemEntryPtr pEntry = currentMapper(false)->diffuseMapper();
        const OdGePoint3d *pUVW = vertexData()->mappingCoords(OdGiVertexData::kAllChannels);
        OdGePoint2d *pUV = m_texCoordsArray.asArrayPtr();
        for (OdInt32 nPoint = 0; nPoint < 4; nPoint++)
          pEntry->mapPredefinedCoords(pUVW + faceList[nPoint], pUV + faceList[nPoint], 1);
      }
    }

    const OdGiSubEntityTraitsData &effectiveTraits = OdGiGeometrySimplifier::drawContext()->effectiveTraits();    
    OdGiGeometrySimplifier::meshFaceOut(faceList, pNormal);
  }

  void OdColladaOut::shellFaceOut(OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal)
  {
    if (!currentMapper()->diffuseMapper().isNull())
    {
      if (m_texCoordsArray.size() < (OdUInt32)vertexDataCount())
        m_texCoordsArray.resize((OdUInt32)vertexDataCount());

      if (!vertexData() || !vertexData()->mappingCoords(OdGiVertexData::kAllChannels))
        currentMapper()->diffuseMapper()->mapCoords((OdUInt32)vertexDataCount(), vertexDataList(), m_texCoordsArray.asArrayPtr(),
          (OdUInt32)Od_abs(*pFaceList), pFaceList + 1, OdGiMapperItemEntry::kPolyIt, pNormal, NULL, (vertexData()) ? vertexData()->normals() : NULL,
          (vertexData()) ? vertexData()->orientationFlag() : kOdGiNoOrientation);
      else
      {
        OdGiMapperItemEntryPtr pEntry = currentMapper(false)->diffuseMapper();
        OdInt32 nPoints = Od_abs(*pFaceList); const OdGePoint3d *pUVW = vertexData()->mappingCoords(OdGiVertexData::kAllChannels);
        OdGePoint2d *pUV = m_texCoordsArray.asArrayPtr();
        for (OdInt32 nPoint = 1; nPoint <= nPoints; nPoint++)
          pEntry->mapPredefinedCoords(pUVW + pFaceList[nPoint], pUV + pFaceList[nPoint], 1);
      }
    }

    const OdGiSubEntityTraitsData &effectiveTraits = OdGiGeometrySimplifier::drawContext()->effectiveTraits();
    OdGiGeometrySimplifier::shellFaceOut(faceListSize, pFaceList, pNormal);
  }

  //using normals for vertices from vertexData
  void OdColladaOut::triangleOut( const OdInt32* p3Vertices, const OdGeVector3d* pNormal )
  {
    const OdGePoint3d*  pVertexDataList = vertexDataList();
    const OdGeVector3d* pNormals = NULL;

    if ((pVertexDataList + p3Vertices[0]) != (pVertexDataList + p3Vertices[1]) && 
        (pVertexDataList + p3Vertices[0]) != (pVertexDataList + p3Vertices[2]) && 
        (pVertexDataList + p3Vertices[1]) != (pVertexDataList + p3Vertices[2]))
    {
      if(vertexData())
        pNormals = vertexData()->normals();      

      for(int i = 0; i < 3; ++i)
      {
        m_pCurrentColladaEntData->m_indPtsArr.append(m_iPtArrSize + p3Vertices[i]);

        if (pNormals)
        {
          m_pCurrentColladaEntData->m_indVtxNormArr.append(m_pCurrentColladaEntData->m_normVtxArr.append(*(pNormals + p3Vertices[i])));
        }        
        else
        {
          //To compute normal 1 time for a triangle
          if (i == 0)
          {
            if (pNormal)
            {
              iTmpIndex = m_pCurrentColladaEntData->m_normVtxArr.append(*pNormal);
            }
            else
            {
              OdGePoint3d calcArray[3] = { *(pVertexDataList + p3Vertices[0]),*(pVertexDataList + p3Vertices[1]), *(pVertexDataList + p3Vertices[2]) };
              OdGeVector3d vecNorm = -odgiFaceNormal(3, calcArray);
              iTmpIndex = m_pCurrentColladaEntData->m_normVtxArr.append(vecNorm);
            }

            //add normals for 3 points
            m_pCurrentColladaEntData->m_indVtxNormArr.append(iTmpIndex);
            m_pCurrentColladaEntData->m_indVtxNormArr.append(iTmpIndex);
            m_pCurrentColladaEntData->m_indVtxNormArr.append(iTmpIndex);
          }
        }

        if(m_pColladaMaterialData->m_matDataArr.at(m_pCurrentColladaEntData->m_iEntMaterial).bDiffuseChannelEnabled && 
           m_pColladaMaterialData->m_matDataArr.at(m_pCurrentColladaEntData->m_iEntMaterial).bDiffuseHasTexture)
        {
          OdGePoint2d& pPtTex = m_texCoordsArray.at(p3Vertices[i]);
          if (!m_pCurrentColladaEntData->m_pDiffuseMaterialMapperArr.m_ptTextureCoordArr.find(pPtTex, iTmpIndex))
          {
            iTmpIndex = m_pCurrentColladaEntData->m_pDiffuseMaterialMapperArr.m_ptTextureCoordArr.append(pPtTex);
          }
          m_pCurrentColladaEntData->m_pDiffuseMaterialMapperArr.m_indTextureCoordArr.append(iTmpIndex);
        }
      }
    }
  }

  OdColladaOut::OdColladaOut() :
    OdGsBaseMaterialView(),
    m_flags(0),
    m_iLightCounter(0),
    m_pEntityDataArr(0),
    m_pColladaMaterialData(0),
    m_pLightExp(0),
    m_pCurrentColladaEntDataArr(0),
    m_pCurrentColladaEntData(0),
    m_iCurMaterial(0),
    iTmpIndex(0),
    m_iPtArrSize(0)
  {
    setMode(OdGsView::kFlatShaded);
  }

  void OdColladaOut::init(EntityDataArray* pEntityDataArr, ColladaMatrialData* pColladaMaterialData, LightExporter* pLightExp)
  {
    m_pEntityDataArr       = pEntityDataArr;
    m_pColladaMaterialData = pColladaMaterialData;
    m_pLightExp = pLightExp;
  }

  void OdColladaOut::beginViewVectorization()
  {
    OdGsBaseMaterialView::beginViewVectorization();
    setEyeToOutputTransform( getEyeToWorldTransform() );

    OdGiGeometrySimplifier::setDrawContext(OdGsBaseMaterialView::drawContext());
    output().setDestGeometry((OdGiGeometrySimplifier&)*this);
  }

  void OdColladaOut::endViewVectorization()
  {
    OdGsBaseMaterialView::endViewVectorization();
  }
}
