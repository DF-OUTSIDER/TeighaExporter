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

#include <OdaCommon.h>
#include "DgnImportLight.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <DgLight.h>
#include <DbLight.h>
#include "DgShape.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgnPointLightImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLightPoint* pDgnLight = (OdDgLightPoint*)e;

  OdDbLightPtr pDbLight = OdDbLight::createObject();

  pDbLight->setName( pDgnLight->getLightName() );
  pDbLight->setPosition( pDgnLight->getOrigin() );
  pDbLight->setGlyphDisplay( OdDbLight::kGlyphDisplayAuto );
  pDbLight->setHasTarget( false );
  pDbLight->setHotspotAndFalloff( OdaPI, OdaPI );
  pDbLight->setIntensity( pDgnLight->getIntensity() );
  pDbLight->setIsPlottable(false);
  pDbLight->setLightType(OdDbLight::kPointLight);
  pDbLight->setOn( pDgnLight->getOnFlag() );

  OdCmColor lightColor;
  lightColor.setRGB( OdUInt8(pDgnLight->getColorRed()*255),
                     OdUInt8(pDgnLight->getColorGreen()*255),
                     OdUInt8(pDgnLight->getColorBlue()*255) );

  pDbLight->setLightColor( lightColor );

  OdGiLightAttenuation attenuation;

  if( pDgnLight->getAttenuateOnFlag() )
  {
    attenuation.setAttenuationType( OdGiLightAttenuation::kInverseLinear );
    attenuation.setUseLimits(true);
    attenuation.setLimits(0, pDgnLight->getAttenuationDistance() );
  }
  else
  {
    attenuation.setAttenuationType(OdGiLightAttenuation::kNone );
  }

  pDbLight->setLightAttenuation(attenuation);

  OdGiShadowParameters shadowData;
  shadowData.setShadowsOn(pDgnLight->getShadowOnFlag());
  shadowData.setShadowType( OdGiShadowParameters::kShadowMaps );
  shadowData.setShadowMapSize( (OdUInt16)(pDgnLight->getShadowResolution()) );
  pDbLight->setShadowParameters(shadowData);

  owner->appendOdDbEntity(pDbLight);

  OdDgGraphicsElementPtr pLightElm = pDgnLight;

  OdDgElementIteratorPtr pLightCellIter = pDgnLight->createIterator();

  for(; !pLightCellIter->done(); pLightCellIter->step() )
  {
    OdDgElementPtr pElm = pLightCellIter->item().openObject(OdDg::kForRead);

    if( pElm->isKindOf( OdDgGraphicsElement::desc() ) )
    {
      OdDgGraphicsElementPtr pGraphElm = pElm;

      if( !pGraphElm.isNull() && !pGraphElm->getInvisibleFlag() )
      {
        pLightElm = pGraphElm;
        break;
      }
    }
  }

  copyEntityProperties(pLightElm, pDbLight);
 
  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbLight->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pDgnLight->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgnDirectionLightImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLightDistant* pDgnLight = (OdDgLightDistant*)e;

  OdDbLightPtr pDbLight = OdDbLight::createObject();

  pDbLight->setName( pDgnLight->getLightName() );
  pDbLight->setPosition( pDgnLight->getOrigin() );
  pDbLight->setGlyphDisplay( OdDbLight::kGlyphDisplayAuto );
  pDbLight->setHasTarget( false );
  pDbLight->setHotspotAndFalloff( OdaPI, OdaPI );
  pDbLight->setIntensity( pDgnLight->getIntensity() );
  pDbLight->setIsPlottable(false);
  pDbLight->setLightType(OdDbLight::kDistantLight);
  pDbLight->setOn( pDgnLight->getOnFlag() );

  OdGeVector3d vrDirection = OdGeVector3d::kXAxis;
  OdGeMatrix3d matLight = pDgnLight->getTransformation();

  vrDirection.transformBy(matLight);
  vrDirection.normalize();

  pDbLight->setLightDirection( vrDirection );

  OdGiShadowParameters shadowData;
  shadowData.setShadowsOn(pDgnLight->getShadowOnFlag());
  shadowData.setShadowType( OdGiShadowParameters::kShadowMaps );
  shadowData.setShadowMapSize( (OdUInt16)(pDgnLight->getShadowResolution()) );
  pDbLight->setShadowParameters(shadowData);

  OdCmColor lightColor;
  lightColor.setRGB( OdUInt8(pDgnLight->getColorRed()*255),
                     OdUInt8(pDgnLight->getColorGreen()*255),
                     OdUInt8(pDgnLight->getColorBlue()*255) );

  pDbLight->setLightColor( lightColor );

  owner->appendOdDbEntity(pDbLight);

  OdDgGraphicsElementPtr pLightElm = pDgnLight;

  OdDgElementIteratorPtr pLightCellIter = pDgnLight->createIterator();

  for(; !pLightCellIter->done(); pLightCellIter->step() )
  {
    OdDgElementPtr pElm = pLightCellIter->item().openObject(OdDg::kForRead);

    if( pElm->isKindOf( OdDgGraphicsElement::desc() ) )
    {
      OdDgGraphicsElementPtr pGraphElm = pElm;

      if( !pGraphElm.isNull() && !pGraphElm->getInvisibleFlag() )
      {
        pLightElm = pGraphElm;
        break;
      }
    }
  }

  copyEntityProperties(pLightElm, pDbLight);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbLight->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pDgnLight->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgnSpotLightImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLightSpot* pDgnLight = (OdDgLightSpot*)e;

  OdDbLightPtr pDbLight = OdDbLight::createObject();

  pDbLight->setName( pDgnLight->getLightName() );
  pDbLight->setPosition( pDgnLight->getOrigin() );
  pDbLight->setGlyphDisplay( OdDbLight::kGlyphDisplayAuto );
  pDbLight->setHasTarget( false );
  pDbLight->setHotspotAndFalloff( pDgnLight->getConeAngle()*2.0, (pDgnLight->getConeAngle()+pDgnLight->getDeltaAngle())*2.0);
  pDbLight->setIntensity( pDgnLight->getIntensity() );
  pDbLight->setIsPlottable(false);
  pDbLight->setLightType(OdDbLight::kSpotLight);
  pDbLight->setOn( pDgnLight->getOnFlag() );

  OdGeVector3d vrDirection = OdGeVector3d::kXAxis;
  OdGeMatrix3d matLight = pDgnLight->getTransformation();

  vrDirection.transformBy(matLight);
  vrDirection.normalize();

  pDbLight->setTargetLocation( pDgnLight->getOrigin() + vrDirection );

  OdGiShadowParameters shadowData;
  shadowData.setShadowsOn(pDgnLight->getShadowOnFlag());
  shadowData.setShadowType( OdGiShadowParameters::kShadowMaps );
  shadowData.setShadowMapSize( (OdUInt16)(pDgnLight->getShadowResolution()) );
  pDbLight->setShadowParameters(shadowData);

  OdCmColor lightColor;
  lightColor.setRGB( OdUInt8(pDgnLight->getColorRed()*255),
                     OdUInt8(pDgnLight->getColorGreen()*255),
                     OdUInt8(pDgnLight->getColorBlue()*255) );

  pDbLight->setLightColor( lightColor );

  OdGiLightAttenuation attenuation;

  if( pDgnLight->getAttenuateOnFlag() )
  {
    attenuation.setAttenuationType( OdGiLightAttenuation::kInverseLinear );
    attenuation.setUseLimits(true);
    attenuation.setLimits(0, pDgnLight->getAttenuationDistance() );
  }
  else
  {
    attenuation.setAttenuationType(OdGiLightAttenuation::kNone );
  }

  pDbLight->setLightAttenuation(attenuation);

  owner->appendOdDbEntity(pDbLight);

  OdDgGraphicsElementPtr pLightElm = pDgnLight;

  OdDgElementIteratorPtr pLightCellIter = pDgnLight->createIterator();

  for(; !pLightCellIter->done(); pLightCellIter->step() )
  {
    OdDgElementPtr pElm = pLightCellIter->item().openObject(OdDg::kForRead);

    if( pElm->isKindOf( OdDgGraphicsElement::desc() ) )
    {
      OdDgGraphicsElementPtr pGraphElm = pElm;

      if( !pGraphElm.isNull() && !pGraphElm->getInvisibleFlag() )
      {
        pLightElm = pGraphElm;
        break;
      }
    }
  }

  copyEntityProperties(pLightElm, pDbLight);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbLight->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pDgnLight->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgnAreaLightImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLightArea* pDgnLight = (OdDgLightArea*)e;

  OdDbLightPtr pDbLight = OdDbLight::createObject();

  pDbLight->setName( pDgnLight->getLightName() );
  pDbLight->setPosition( pDgnLight->getOrigin() );
  pDbLight->setGlyphDisplay( OdDbLight::kGlyphDisplayOff );
  pDbLight->setHasTarget( false );
  pDbLight->setHotspotAndFalloff( 8.0*OdaPI/9.01, 8.0*OdaPI/9.01 );
  pDbLight->setIntensity( pDgnLight->getIntensity() );
  pDbLight->setIsPlottable(false);
  pDbLight->setLightType(OdDbLight::kSpotLight);
  pDbLight->setOn( pDgnLight->getOnFlag() );

  OdGeVector3d vrDirection = OdGeVector3d::kZAxis;
  OdGeMatrix3d matLight = pDgnLight->getTransformation();

  vrDirection.transformBy(matLight);
  vrDirection.normalize();

  pDbLight->setTargetLocation( pDgnLight->getOrigin() + vrDirection );

  OdDgGraphicsElementPtr pLightElm = pDgnLight;

  double dLightLength = 1.0;
  double dLightWidth = 1.0;

  OdDgElementIteratorPtr pLightCellIter = pDgnLight->createIterator();

  for(; !pLightCellIter->done(); pLightCellIter->step() )
  {
    OdDgElementPtr pElm = pLightCellIter->item().openObject(OdDg::kForRead);

    if( pElm->isKindOf( OdDgShape3d::desc() ) )
    {
      OdDgShape3dPtr pShapeElm = pElm;

      if( !pShapeElm.isNull() )
      {
        pLightElm = pShapeElm;

        ((OdDgnImportPE*)pShapeElm->queryX(OdDgnImportPE::desc()))->subImportElement(pShapeElm, owner);

        if( pShapeElm->getVerticesCount() > 2 )
        {
          dLightLength = pShapeElm->getVertexAt(0).distanceTo( pShapeElm->getVertexAt(1) );
          dLightWidth = pShapeElm->getVertexAt(1).distanceTo( pShapeElm->getVertexAt(2) );
        }

        break;
      }
    }
  }

  OdGiShadowParameters shadowData;
  shadowData.setShadowsOn(pDgnLight->getShadowOnFlag());
  shadowData.setShadowType( OdGiShadowParameters::kShadowMaps );
  shadowData.setExtendedLightShape( OdGiShadowParameters::kRectangle );
  shadowData.setExtendedLightLength( dLightLength );
  shadowData.setExtendedLightWidth( dLightWidth );
  shadowData.setShadowMapSize( (OdUInt16)(pDgnLight->getShadowResolution()) );
  pDbLight->setShadowParameters(shadowData);

  OdCmColor lightColor;
  lightColor.setRGB( OdUInt8(pDgnLight->getColorRed()*255),
    OdUInt8(pDgnLight->getColorGreen()*255),
    OdUInt8(pDgnLight->getColorBlue()*255) );

  pDbLight->setLightColor( lightColor );

  OdGiLightAttenuation attenuation;

  if( pDgnLight->getAttenuateOnFlag() )
  {
    attenuation.setAttenuationType( OdGiLightAttenuation::kInverseLinear );
    attenuation.setUseLimits(true);
    attenuation.setLimits(0, pDgnLight->getAttenuationDistance() );
  }
  else
  {
    attenuation.setAttenuationType(OdGiLightAttenuation::kNone );
  }

  pDbLight->setLightAttenuation(attenuation);

  owner->appendOdDbEntity(pDbLight);

  copyEntityProperties(pLightElm, pDbLight);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbLight->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pDgnLight->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
