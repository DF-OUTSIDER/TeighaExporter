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



//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////


#include "HoopsExportCommon.h"
#include "HoopsExportView.h"
#include "OdRound.h"

#include "Gi/GiMaterialItem.h"
#include "Gi/GiMapperItem.h"
#include "Gi/GiMaterialTraitsData.h"
#include "Gi/GiLightTraitsData.h"
#include "Gs/GsMaterialNode.h"

namespace TD_HOOPS_COMMON_EXPORT {

#define INVALIDEXTENTS 1.0e20  /* {Secret} */

//====================================================================\\

OdHoopsExportView::OdHoopsExportView()
{
  m_bMaterialCommited = false;
  m_bExportToViewport = false;
  m_bProcessingTTF    = false;
  m_pCurDrawable      = NULL;

  m_pViewportClip = OdGiOrthoClipper::createObject();
  m_pDcToWorld = OdGiXform::createObject();
  m_pWorldToDc = OdGiXform::createObject();
  m_pWorldToDc->input().addSourceNode(output());
  m_pViewportClip->input().addSourceNode( m_pWorldToDc->output() );
  m_pDcToWorld->input().addSourceNode( m_pViewportClip->output() );
  m_pDcToWorld->output().setDestGeometry( *device() );
}

//====================================================================\\

OdHoopsExportView::~OdHoopsExportView()
{

}

//====================================================================\\

OdGsViewPtr OdHoopsExportView::createObject()
{
  return OdRxObjectImpl<OdHoopsExportView, OdGsView>::createObject();
}

//====================================================================\\

OdHoopsExportDevice* OdHoopsExportView::device()
{ 
  return (OdHoopsExportDevice*)OdGsBaseVectorizeView::device();
}

//====================================================================\\

void OdHoopsExportView::onTraitsModified()
{
  m_bMaterialCommited = false;
  OdGsBaseVectorizer::onTraitsModified();
  device()->onTraitsModified(effectiveTraits());

  if( !m_bMaterialCommited )
    processMaterialNode( effectiveTraits().material(), NULL );

}

//====================================================================

void OdHoopsExportView::beginViewVectorization()
{
  OdGsBaseVectorizer::beginViewVectorization();
  m_bExportToViewport = false;
  device()->setDrawContext(drawContext());

  device()->dc_open_segment( device()->m_pHoops->getParams().iStartKey );
  device()->m_pHoops->m_iCurKey = device()->m_pHoops->getParams().iStartKey;

  m_bProcessingTTF = false;
}

//====================================================================

void OdHoopsExportView::endViewVectorization()
{
  OdGsBaseVectorizer::endViewVectorization();
  device()->setDrawContext(0);
  device()->dc_close_segment();

  if( m_bExportToViewport )
    device()->dc_close_segment();
}

//====================================================================

void OdHoopsExportView::xline( const OdGePoint3d& pt1, const OdGePoint3d& pt2 )
{
  device()->setXLineExport( true );
  OdGsBaseVectorizer::xline( pt1, pt2 );
}

//====================================================================

void OdHoopsExportView::ray( const OdGePoint3d& start, const OdGePoint3d&  pt2 )
{
  device()->setRayExport( true );
  OdGsBaseVectorizer::ray( start, pt2 );
}

//====================================================================

void OdHoopsExportView::loadViewport()
{
  if( !device()->m_bInitCamera )
  {
    device()->m_bInitCamera = true;

    HC_Open_Segment_By_Key( device()->m_pHoops->getParams().iStartKey );

    if( device()->m_bSetCamera )
      HoopsSetCamera(getCameraLocation(), getCameraTarget(),getCameraUpVector(), 
      fieldWidth(), fieldHeight(), false, false, 0, 0 );
    else if( device()->m_pHoops->getParams().bSetCamera )
    {
      const OdGsBaseVectorizeView* pRootView = ( OdGsBaseVectorizeView*)(device()->rootView());

      if( pRootView )
        HoopsSetCamera( pRootView->position(), pRootView->target(),
                        pRootView->upVector(), pRootView->fieldWidth(),
                        pRootView->fieldHeight(), false, false, 0, 0 );
      else
        HoopsSetCamera(getCameraLocation(), getCameraTarget(),getCameraUpVector(), 
        fieldWidth(), fieldHeight(), false, false, 0, 0 );
    }

    HC_Close_Segment();
  }

  OdIntArray        iPtsCountArr;
  OdGePoint2dArray  ptsArr;
  viewportClipRegion(iPtsCountArr, ptsArr);

  bool bSetClip = true;

  const OdGsView* pRootView = device()->rootView();

  if( !pRootView || ( pRootView == this ) )
  {
    bSetClip = false;
  }

  if( !iPtsCountArr.size() )
  {
    OdGePoint2d ptMin;
    OdGePoint2d ptMax;
    screenRect(ptMin, ptMax);

    OdGePoint2d ptMinVp;
    OdGePoint2d ptMaxVp;

    getViewport( ptMinVp, ptMaxVp );

    if( OdZero(ptMax.x - ptMin.x + ptMinVp.x - ptMaxVp.x) &&
      OdZero(ptMax.y - ptMin.x + ptMinVp.y - ptMaxVp.y)
      )
    {
      ptMax.x += 0.1;
      ptMax.y += 0.1;
      ptMin.x -= 0.1;
      ptMin.y -= 0.1;
      bSetClip = false;
    }
    
    iPtsCountArr.push_back(5);
    OdGePoint2d tmpPt;
    tmpPt = ptMin;
    ptsArr.push_back(tmpPt);
    tmpPt.x = ptMax.x;
    ptsArr.push_back(tmpPt);
    tmpPt.y = ptMax.y;
    ptsArr.push_back(tmpPt);
    tmpPt.x = ptMin.x;
    ptsArr.push_back(tmpPt);
    tmpPt = ptMin;
    ptsArr.push_back(tmpPt);
  }

  if( bSetClip )
  {
    OdString strViewSegment = device()->defineHoopsObjectName( OdHoopsExportDevice::kHoopsViewport, false );
    HC_KEY iKey = device()->dc_open_segment( strViewSegment );
    device()->m_pHoops->m_lViewKeyArr.push_back( iKey);
    device()->m_pHoops->m_iCurKey = iKey;
    m_bExportToViewport = true;

    OdGeMatrix3d xToWorld;
    OdGeMatrix3d worldToX;
    OdGeMatrix3d xToDc;

    if(!OdGsViewImpl::isPerspective())
    {
      xToWorld = eyeToWorldMatrix();
      setEyeToOutputTransform(xToWorld);

      worldToX = xToWorld;
      worldToX = worldToX.invert();

      xToDc = eyeToScreenMatrix();

      OdGeVector3d zAxis = OdGeVector3d::kZAxis;
      OdGeVector3d dcFront(0.,0.,frontClip());
      OdGeVector3d dcBack(0.,0.,backClip());
      dcFront.transformBy(xToDc);
      dcBack.transformBy(xToDc);
      zAxis.transformBy(xToDc);

      m_pWorldToDc->setTransform( xToDc * worldToX );

      if(zAxis.z > 0.)
        m_pViewportClip->set(ptsArr.size(), ptsArr.asArrayPtr(), isBackClipped(), dcBack.z, isFrontClipped(), dcFront.z);
      else
        m_pViewportClip->set(ptsArr.size(), ptsArr.asArrayPtr(), isFrontClipped(), dcFront.z, isBackClipped(), dcBack.z);

      m_pViewportClip->setDrawContext(OdGiBaseVectorizer::drawContext());
      m_pDcToWorld->setTransform( m_DcToWorldMatrix );
      m_pDcToWorld->output().setDestGeometry( *device() );
    }
    else
    {
      OdGeMatrix3d xToDc = screenMatrix() * projectionMatrix();

      xToWorld = eyeToWorldMatrix();
      setEyeToOutputTransform(xToWorld);

      worldToX = xToWorld;
      worldToX = worldToX.invert();

      bool isBackClp(isBackClipped());

      double frClp(frontClip());
      double bkClp(backClip());

      m_pWorldToDc->setTransform( xToDc * worldToX );
      m_pViewportClip->set(ptsArr.size(), ptsArr.asArrayPtr(), isBackClp, bkClp, true, frClp);
      m_pViewportClip->setDrawContext(OdGiBaseVectorizer::drawContext());
      m_pDcToWorld->setTransform( m_DcToWorldMatrix );
      m_pDcToWorld->output().setDestGeometry( *device() );
    }

    OdGePoint3dArray  ptsClipBoundary;

    for( int i = 0; i < iPtsCountArr[0]; i++ )
    {
      OdGePoint3d ptTmp( ptsArr[i].x, ptsArr[i].y, 0 );
      ptTmp.transformBy( m_DcToWorldMatrix );
      ptsClipBoundary.push_back( ptTmp );
    }

    if( device()->m_bRecording )
    {
      setViewportBorderVisibility(false);
      HoopsSetClipRegion(ptsClipBoundary);
    }
  }
  else
  {
    OdGeMatrix3d xToWorld = eyeToWorldMatrix();
    setEyeToOutputTransform(xToWorld);

    OdGeMatrix3d worldToX = xToWorld;
      worldToX = worldToX.invert();

    OdGeMatrix3d xToDc;

    if(!OdGsViewImpl::isPerspective())
    {
      xToDc = eyeToScreenMatrix();
    }
    else
    {
      xToDc = screenMatrix() * projectionMatrix();
    }

    m_pWorldToDc->setTransform( xToDc * worldToX );
    m_pViewportClip->input().removeSourceNode( m_pWorldToDc->output() );
    m_pDcToWorld->input().removeSourceNode( m_pViewportClip->output() );
    m_pDcToWorld->input().addSourceNode( m_pWorldToDc->output() );
    m_pDcToWorld->setTransform( m_DcToWorldMatrix );
    m_pDcToWorld->output().setDestGeometry( *device() );
  }
}

//====================================================================

void OdHoopsExportView::setView(const OdGePoint3d & position, const OdGePoint3d& target,
                     const OdGeVector3d& upVector, double fieldWidth,
                     double fieldHeight, Projection /*projection*/ )
{
  OdGsBaseVectorizeView::setView(position, target, upVector, fieldWidth, fieldHeight, kParallel);

  const OdGsView* pRootView = device()->rootView();
  OdGeMatrix3d matWToDC = worldToDeviceMatrix();

  if( pRootView )
    matWToDC = pRootView->worldToDeviceMatrix();

  m_WorldToDcMatrix = matWToDC;
  m_DcToWorldMatrix = matWToDC.invert();
}

//====================================================================

void OdHoopsExportView::processMaterialNode( 
  OdDbStub *pMaterialId, OdGsMaterialNode *pNode, 
  const OdGiMaterialTraitsData &materialTraits, 
  const OdGiMapper* pMapper
)
{
  device()->m_MaterialInfo.bTTFProcessing = m_bProcessingTTF;

  if( !device()->m_bRecording )
    return;

  if( device()->m_MaterialInfo.pMaterialId == pMaterialId && device()->m_MaterialInfo.bSetMaterial)
    return;

  OdGiMapperRenderItemPtr pPrevMapItem = device()->m_pCurMapperItem;

  if( mode() <= OdGsView::kHiddenLine || m_bProcessingTTF )
  {
    if( !pPrevMapItem->diffuseMapper().isNull() || 
        !pPrevMapItem->specularMapper().isNull() ||
        !pPrevMapItem->reflectionMapper().isNull() ||
        !pPrevMapItem->bumpMapper().isNull()
      )
    {
      device()->m_pCurMapperItem = OdGiMapperRenderItem::createObject();
    }

    device()->m_MaterialInfo.bSetMaterial = false;
    return;
  }

  if( pMaterialId == NULL && pNode == NULL )
  {
    device()->m_MaterialInfo.bSetMaterial = false;
    return;
  }

  m_bMaterialCommited = true;

  const OdGiMaterialTraitsData& materialData = (pNode == NULL) ? materialTraits : pNode->materialTraits();

  OdGiMaterialColor diffuseColor; 
  OdGiMaterialColor ambientColor; 
  OdGiMaterialColor specularColor;
  double            glossFactor;
  double            opacityPercentage; 
  OdGiMaterialMap   opacityMap;
  OdGiMaterialMap   bumpMap;
  OdGiMaterialMap   specularMap;
  OdGiMaterialMap   diffuseMap;
  OdGiMaterialMap   reflectionMap;
  ODCOLORREF        colorDiffuse(0);
  ODCOLORREF        colorAmbient(0);
  ODCOLORREF        colorSpecular(0);

  materialData.diffuse(diffuseColor, diffuseMap);
  materialData.ambient(ambientColor);
  materialData.specular(specularColor, specularMap, glossFactor);
  materialData.opacity(opacityPercentage, opacityMap);
  materialData.bump(bumpMap);
  materialData.reflection(reflectionMap);

  if( diffuseColor.color().colorMethod() == OdCmEntityColor::kByColor )
    colorDiffuse = ODTOCOLORREF(diffuseColor.color());
  else if( diffuseColor.color().colorMethod() == OdCmEntityColor::kByACI ||
           diffuseColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex
    )
  {
    OdInt16 uIndex = diffuseColor.color().colorIndex();

    if( device()->getPaletteSize() > (OdUInt16)uIndex )
    {
      colorDiffuse = device()->getColor( (OdUInt16)uIndex );
    }
    else
    {
      colorDiffuse = OdCmEntityColor::lookUpRGB((OdUInt8)uIndex);
    }
  }

  if( ambientColor.color().colorMethod() == OdCmEntityColor::kByColor )
    colorAmbient = ODTOCOLORREF(ambientColor.color());
  else if( ambientColor.color().colorMethod() == OdCmEntityColor::kByACI ||
           ambientColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex
         )
  {
    OdInt16 uIndex = diffuseColor.color().colorIndex();

    if( device()->getPaletteSize() > (OdUInt16)uIndex )
    {
      colorAmbient = device()->getColor( (OdUInt16)uIndex );
    }
    else
    {
      colorAmbient = OdCmEntityColor::lookUpRGB((OdUInt8)uIndex);
    }
  }
    
  if( specularColor.color().colorMethod() == OdCmEntityColor::kByColor )
    colorSpecular = ODTOCOLORREF(specularColor.color());
  else if( specularColor.color().colorMethod() == OdCmEntityColor::kByACI ||
           specularColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex
         )
  {
    OdInt16 uIndex = diffuseColor.color().colorIndex();

    if( device()->getPaletteSize() > (OdUInt16)uIndex )
    {
      colorSpecular = device()->getColor( (OdUInt16)uIndex );
    }
    else
    {
      colorSpecular = OdCmEntityColor::lookUpRGB((OdUInt8)uIndex);
    }
  }
    
  device()->m_MaterialInfo.bSetMaterial = true;
  device()->m_MaterialInfo.bTwoSides    = materialData.twoSided();
  device()->m_MaterialInfo.bUseAmbient  = ambientColor.method() == OdGiMaterialColor::kOverride;
  device()->m_MaterialInfo.bUseDiffuse  = diffuseColor.method() == OdGiMaterialColor::kOverride;
  device()->m_MaterialInfo.bUseSpecular = specularColor.method() == OdGiMaterialColor::kOverride;
  device()->m_MaterialInfo.clrAmbient   = colorAmbient;
  device()->m_MaterialInfo.clrDiffuse   = colorDiffuse;
  device()->m_MaterialInfo.clrSpecular  = colorSpecular;
  device()->m_MaterialInfo.dGlossFactor = glossFactor;
  device()->m_MaterialInfo.dTranslucence = 1.0 - opacityPercentage;
  device()->m_MaterialInfo.pMaterialId  = pMaterialId;
  
  if( diffuseMap.source() == OdGiMaterialMap::kFile && diffuseMap.sourceFileName() != "" && 
      GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseDiffuse)
    )
  {
    device()->m_MaterialInfo.strTextureFilename = diffuseMap.sourceFileName();
    device()->m_MaterialInfo.iTextureProjection = diffuseMap.mapper().projection();
    device()->m_MaterialInfo.matTextureTransform = diffuseMap.mapper().transform();
    device()->m_MaterialInfo.uTiling             = diffuseMap.mapper().uTiling();
    device()->m_MaterialInfo.dTextureBrightness  = diffuseMap.blendFactor();
    device()->m_MaterialInfo.iTextureType        = DIFFUSE_TEXTURE;
  } 
  else if( bumpMap.source() == OdGiMaterialMap::kFile && bumpMap.sourceFileName() != "" &&
    GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseBump) )
  {
    device()->m_MaterialInfo.strTextureFilename = bumpMap.sourceFileName();
    device()->m_MaterialInfo.iTextureProjection = bumpMap.mapper().projection();
    device()->m_MaterialInfo.matTextureTransform = bumpMap.mapper().transform();
    device()->m_MaterialInfo.uTiling             = bumpMap.mapper().uTiling();
    device()->m_MaterialInfo.dTextureBrightness  = bumpMap.blendFactor();
    device()->m_MaterialInfo.iTextureType        = BUMP_TEXTURE;
  }
  else if( reflectionMap.source() == OdGiMaterialMap::kFile && reflectionMap.sourceFileName() != "" &&
           GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseReflection) )
  {
    device()->m_MaterialInfo.strTextureFilename = reflectionMap.sourceFileName();
    device()->m_MaterialInfo.iTextureProjection = reflectionMap.mapper().projection();
    device()->m_MaterialInfo.matTextureTransform = reflectionMap.mapper().transform();
    device()->m_MaterialInfo.uTiling             = reflectionMap.mapper().uTiling();
    device()->m_MaterialInfo.dTextureBrightness  = reflectionMap.blendFactor();
    device()->m_MaterialInfo.iTextureType        = REFLECTION_TEXTURE;
  }
  else if( specularMap.source() == OdGiMaterialMap::kFile && specularMap.sourceFileName() != "" &&
           GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseSpecular))
  {
    device()->m_MaterialInfo.strTextureFilename = specularMap.sourceFileName();
    device()->m_MaterialInfo.iTextureProjection = specularMap.mapper().projection();
    device()->m_MaterialInfo.matTextureTransform = specularMap.mapper().transform();
    device()->m_MaterialInfo.uTiling             = specularMap.mapper().uTiling();
    device()->m_MaterialInfo.dTextureBrightness  = specularMap.blendFactor();
    device()->m_MaterialInfo.iTextureType        = SPECULAR_TEXTURE;
  }
  else
  {
    device()->m_MaterialInfo.strTextureFilename.empty();
  }

  if (pMaterialId != NULL && !device()->m_MaterialInfo.strTextureFilename.isEmpty() )
  {
    if( !pPrevMapItem->isLastProcValid(pMapper, pMaterialId) )
    {
      if( pMapper != NULL )
        pPrevMapItem->setMapper(pMapper, materialData, pMaterialId);
      else
        pPrevMapItem->setMapper(materialData, pMaterialId);
    }

    // Reset extents

    pPrevMapItem->setDeviceTransform( eyeToOutputTransform() * getWorldToEyeTransform() * m_DcToWorldMatrix );

    if( pPrevMapItem->isObjectMatrixNeed() )
    {
      OdGeExtents3d curExtents;

      if( GetCurObjectExtents( curExtents ) )
        pPrevMapItem->setObjectTransform(curExtents);
      else
      {
        if( !pPrevMapItem->diffuseMapper().isNull() )
          pPrevMapItem->diffuseMapper()->setInputTransform(OdGeMatrix3d());

        if( !pPrevMapItem->specularMapper().isNull() )
          pPrevMapItem->specularMapper()->setInputTransform(OdGeMatrix3d());

        if( !pPrevMapItem->bumpMapper().isNull() )
          pPrevMapItem->bumpMapper()->setInputTransform(OdGeMatrix3d());

        if( !pPrevMapItem->reflectionMapper().isNull() )
          pPrevMapItem->reflectionMapper()->setInputTransform(OdGeMatrix3d());
      }

    }
    else if( !pPrevMapItem->isEntityMapper() && !pPrevMapItem->isModelMatrixNeed() )
    {
      OdGeExtents3d curExtents;

      if( GetCurObjectExtents( curExtents ) )
        pPrevMapItem->setVertexTransform(curExtents);
      else 
      {
        if( !pPrevMapItem->diffuseMapper().isNull() )
          pPrevMapItem->diffuseMapper()->setInputTransform(OdGeMatrix3d());

        if( !pPrevMapItem->specularMapper().isNull() )
          pPrevMapItem->specularMapper()->setInputTransform(OdGeMatrix3d());

        if( !pPrevMapItem->bumpMapper().isNull() )
          pPrevMapItem->bumpMapper()->setInputTransform(OdGeMatrix3d());

        if( !pPrevMapItem->reflectionMapper().isNull() )
          pPrevMapItem->reflectionMapper()->setInputTransform(OdGeMatrix3d());
      }
    }
    else if( pPrevMapItem->isModelMatrixNeed() )
    {
      // @@@TODO: set model transform
      //   Multiply the mapper transform by the model (block) transform
      // Note: Model matrix setupped to GsModel::transform().
    }
  }
}

//====================================================================

void OdHoopsExportView::processMaterialNode( OdDbStub *materialId, OdGsMaterialNode *node )
{
  processMaterialNode(materialId, node, effectiveMaterialTraitsData(), effectiveTraits().mapper() );
}

//====================================================================

void OdHoopsExportView::text(const OdGePoint3d& position,
                                   const OdGeVector3d& normal,
                                   const OdGeVector3d& direction,
                                   const OdChar* msg,
                                   OdInt32 length,
                                   bool raw,
                                   const OdGiTextStyle* pTextStyle)
{
  m_bProcessingTTF = pTextStyle->isTtfFont();
  OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pTextStyle);
  m_bProcessingTTF = false;
}

//====================================================================

bool OdHoopsExportView::doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable)
{
  m_pCurDrawable = pDrawable;
  m_iDoDrawFlag  = drawableFlags;

  bool bRenderDrawableGeometry = true;
  bool bOldSkipLines = false;

  if( device() && (drawableFlags & OdGiDrawable::kDrawableViewDependentViewportDraw) )
  {
    bOldSkipLines = device()->m_bSkipLines;
    device()->m_bSkipLines = true;
    bRenderDrawableGeometry = false;
  }

  bool bRet = OdGsBaseVectorizer::doDraw( drawableFlags, pDrawable );

  if( !bRenderDrawableGeometry )
  {
    device()->m_bSkipLines = bOldSkipLines;
  }

  return bRet;
}

//====================================================================

bool OdHoopsExportView::GetCurObjectExtents( OdGeExtents3d& extents )
{
  bool bFindExtents = device()->getSavedExtents( m_pCurDrawable, this, extents );

  if( !bFindExtents && m_pCurDrawable )
  {
    OdGeExtents3d curExtents;

    device()->enableRecording(false);
    m_pCurDrawable->getGeomExtents(curExtents);
    device()->enableRecording(true);

    if(!OdZero(curExtents.minPoint().x - INVALIDEXTENTS) &&
       !OdZero(curExtents.minPoint().y - INVALIDEXTENTS) &&
       !OdZero(curExtents.minPoint().z - INVALIDEXTENTS) &&
       !OdZero(curExtents.maxPoint().x + INVALIDEXTENTS) &&
       !OdZero(curExtents.maxPoint().y + INVALIDEXTENTS) &&
       !OdZero(curExtents.maxPoint().z + INVALIDEXTENTS)
      )
    {
      extents = curExtents;
      device()->saveExtents( m_pCurDrawable, this, extents );
      bFindExtents = true;
    }
  }

  return bFindExtents;
}

//====================================================================

void OdHoopsExportView::addPointLight(const OdGiPointLightTraitsData& LightData)
{
  OdGePoint3d     ptLightPos = LightData.position();
  bool            bUseLight  = LightData.isOn();
  OdCmEntityColor clrLight   = LightData.color();
  double          dIntensity = LightData.intensity();

  OdString strLightSegment = device()->defineHoopsObjectName( OdHoopsExportDevice::kHoolsLight );

  device()->addHoopsPointLight( strLightSegment, ptLightPos, clrLight, dIntensity, bUseLight );
}

//====================================================================

void OdHoopsExportView::addSpotLight(const OdGiSpotLightTraitsData& LightData)
{
  OdGePoint3d     ptLightPos    = LightData.position();
  OdGePoint3d     ptLightTarget = LightData.target();
  double          dHotSpot      = LightData.hotspot();
  double          dFallOff      = LightData.falloff();
  bool            bUseLight     = LightData.isOn();
  OdCmEntityColor clrLight      = LightData.color();
  double          dIntensity    = LightData.intensity();

  OdString strLightSegment = device()->defineHoopsObjectName( OdHoopsExportDevice::kHoolsLight );

  device()->addHoopsSpotLight( strLightSegment, ptLightPos, ptLightTarget, dHotSpot, dFallOff, clrLight, dIntensity, bUseLight );
}

//====================================================================

void OdHoopsExportView::addDistantLight(const OdGiDistantLightTraitsData& LightData)
{
  OdGeVector3d    vrLightDir    = LightData.direction();
  bool            bUseLight     = LightData.isOn();
  OdCmEntityColor clrLight      = LightData.color();
  double          dIntensity    = LightData.intensity();

  OdString strLightSegment = device()->defineHoopsObjectName( OdHoopsExportDevice::kHoolsLight );

  device()->addHoopsDistantLight( strLightSegment, vrLightDir, clrLight, dIntensity, bUseLight );
}
}
//====================================================================

#undef INVALIDEXTENTS
