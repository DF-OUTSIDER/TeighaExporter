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
#include "ColladaLightImporter.h"
#include "ColladaDocumentImporter.h"
#include "COLLADAFWLight.h"
#include "DbLight.h"
#include "CmColor.h"

namespace TD_COLLADA_IMPORT
{
	LightImporter::LightImporter( DocumentImporter* documentImporter )
		: ImporterBase(documentImporter)
	{
	}

	LightImporter::~LightImporter()
	{
	}

	bool LightImporter::import(const COLLADAFW::Light& pColladaLight)
	{
    OdDbBlockTableRecordPtr pBTR = this->getDocumentImporter()->getDatabase()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdDbLightPtr pLight = OdDbLight::createObject();
    pLight->setDatabaseDefaults(pBTR->database());
    pBTR->appendOdDbEntity(pLight);

    //pLight->setPosition(ptLight);

    COLLADAFW::Light::LightType lightType = pColladaLight.getLightType();
    switch (lightType)
    {
    case COLLADAFW::Light::AMBIENT_LIGHT:
      pLight->setLightType(OdGiDrawable::kAmbientLight);
      return 0;
    case COLLADAFW::Light::POINT_LIGHT: 
      pLight->setLightType(OdGiDrawable::kPointLight);
      break;
    case COLLADAFW::Light::DIRECTIONAL_LIGHT: 
      pLight->setLightType(OdGiDrawable::kDistantLight);
      break;
    case COLLADAFW::Light::SPOT_LIGHT: 
      pLight->setLightType(OdGiDrawable::kSpotLight);
      break;
    default: 
      return 0;
    }
     
    COLLADAFW::String pStr = pColladaLight.getName();
    OdString odStr(pStr.c_str());
    pLight->setName(odStr);

    if (lightType == COLLADAFW::Light::POINT_LIGHT || lightType == COLLADAFW::Light::SPOT_LIGHT)
    {
      OdGiLightAttenuation atten;
      if ( pColladaLight.getConstantAttenuation() > 0 ) 
      {
        atten.setAttenuationType(OdGiLightAttenuation::kNone);
        atten.setUseLimits(false);
        pLight->setIntensity(pColladaLight.getConstantAttenuation());
      }
      else if ( pColladaLight.getLinearAttenuation() > 0 ) 
      {
        atten.setAttenuationType(OdGiLightAttenuation::kInverseLinear);
        atten.setLimits(1, 0);
        atten.setUseLimits(true);
        pLight->setIntensity(pColladaLight.getLinearAttenuation());
      }
      else if ( pColladaLight.getQuadraticAttenuation() > 0 ) 
      {
        atten.setAttenuationType(OdGiLightAttenuation::kInverseSquare);
        atten.setLimits(1, 0);
        atten.setUseLimits(true);
        pLight->setIntensity(pColladaLight.getQuadraticAttenuation());
      }
      pLight->setLightAttenuation(atten);
    }

    if (lightType == COLLADAFW::Light::SPOT_LIGHT || lightType == COLLADAFW::Light::DIRECTIONAL_LIGHT)
    {
      // Angles are shared between spot and directional lights.
      pLight->setHotspotAndFalloff(0, pColladaLight.getFallOffAngle());
    }

    COLLADAFW::Color pCldCollor = pColladaLight.getColor(); 
    OdCmColor color;
    color.setRGB((OdUInt8)pCldCollor.getRed(), (OdUInt8)pCldCollor.getGreen(), (OdUInt8)pCldCollor.getBlue());
    pLight->setLightColor(color);

    pLight->setOn(true);
		return true;
	}
}
