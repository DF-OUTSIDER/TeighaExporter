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
#include "ColladaExport.h"
#include "AssetExporter.h"
#include "EffectExporter.h"
#include "GeometryExporter.h"
#include "ImageExporter.h"
#include "MaterialExporter.h"
#include "VisualSceneExporter.h"

#include "COLLADASWStreamWriter.h"
#include "COLLADASWScene.h"
#include "COLLADASWException.h"

namespace TD_COLLADA_EXPORT
{
  class StubDeviceModuleText : public OdGsBaseModule
  {
  private:
    EntityDataArray*    m_pEntityDataArr;
    ColladaMatrialData* m_pColladaMaterialData;
    LightExporter*      m_pLightExp;

  public:
    void init(EntityDataArray* pEntityDataArr, ColladaMatrialData* pColladaMaterialData, LightExporter* pLightExp)
    {
      m_pEntityDataArr       = pEntityDataArr;
      m_pColladaMaterialData = pColladaMaterialData;
      m_pLightExp            = pLightExp;
    }
  protected:
    OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
    {
      return OdRxObjectImpl<StubVectorizeDevice, OdGsBaseVectorizeDevice>::createObject();
    }
    OdSmartPtr<OdGsViewImpl> createViewObject()
    {
      OdSmartPtr<OdGsViewImpl> pP = OdRxObjectImpl<OdColladaOut, OdGsViewImpl>::createObject();
      ((OdColladaOut*)pP.get())->init(m_pEntityDataArr, m_pColladaMaterialData, m_pLightExp);
      return pP;
    }
    OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
    {
      return OdSmartPtr<OdGsBaseVectorizeDevice>();
    }
    OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
    {
      return OdSmartPtr<OdGsViewImpl>();
    }
  };
  ODRX_DEFINE_PSEUDO_STATIC_MODULE(StubDeviceModuleText);



  OdResult CollectData(OdDbBaseDatabase *pDb, EntityDataArray* pEntityDataArr, ColladaMatrialData* pColladaMaterialData, LightExporter* pLightExp, const ODCOLORREF* pPallete, int numColors, const OdGiDrawable* pEntity)
  {
    OdResult ret = eOk;
    odgsInitialize();
    try
    {
      OdGsModulePtr pGsModule = ODRX_STATIC_MODULE_ENTRY_POINT(StubDeviceModuleText)(OD_T("StubDeviceModuleText"));

      ((StubDeviceModuleText*)pGsModule.get())->init(pEntityDataArr, pColladaMaterialData, pLightExp);


      OdDbBaseDatabasePEPtr pDbPE(pDb);
      OdGiDefaultContextPtr pContext = pDbPE->createGiContext(pDb);
      {
        OdGsDevicePtr pDevice = pGsModule->createDevice();
        pDevice->setLogicalPalette( pPallete, numColors );

        if(pEntity)
        {
          //for one entity
          OdGsViewPtr pNewView = pDevice->createView();
          pNewView->setMode(OdGsView::kFlatShaded);
          pDevice->addView( pNewView );
          pNewView->add((OdGiDrawable*)pEntity, 0);
        }
        else
        {
          OdGsDevicePtr pHlpDevice = pDbPE->setupActiveLayoutViews(pDevice, pContext);
        }
        pDevice->setUserGiContext( pContext );

        OdGsDCRect screenRect(OdGsDCPoint(0, 1000), OdGsDCPoint(1000, 0));
        pDevice->onSize(screenRect);
        pDevice->update();
      }

      pContext.release();
      pGsModule.release();
    }
    catch (OdError& e)
    {
      ret = e.code();
    }
    catch(...)
    {
      ret = eExtendedError;
    }
    odgsUninitialize();
    return ret;
  }


  OdResult exportCollada(OdDbBaseDatabase *pDb, const OdString& pFileName, const ODCOLORREF* pPallete, int numColors, const OdGiDrawable* pEntity)
  {
    OdResult ret = eOk;
    OdUInt32 iCurEntData = 0;
    try
    {
      EntityDataArray      m_entityDataArr;
      ColladaMatrialData   m_pColladaMaterialData;

      //OdString sFilePath(pFileName);
      //sFilePath.replace(L'/', L'\\');
      //OdColladaOut::m_sFilePath = sFilePath.left(sFilePath.reverseFind('\\') + 1);

      COLLADASW::NativeString sName(pFileName.c_str());
      COLLADASW::StreamWriter mStreamWriter(sName);
      mStreamWriter.startDocument();
      OdString buffer;

      AssetExporter exAsset(&mStreamWriter, pDb);

      OdStringArray lightArr;
      {
        LightExporter exportLights(&mStreamWriter, lightArr);
        ret = CollectData(pDb, &m_entityDataArr, &m_pColladaMaterialData, &exportLights, pPallete, numColors, pEntity);
      }

      if (eOk == ret)
      {      
        OdUInt32 i = 0;
        //export materials
        {
          //import images
          ImageExporter exportImages(&mStreamWriter);
          exportImages.exportImages(m_pColladaMaterialData.m_matDataArr, pFileName);

          //create effects
          OdUInt32 intDataSize = m_pColladaMaterialData.m_matDataArr.size();
          {
            EffectExporter exportEffect(&mStreamWriter);
            for (i = 0; i < intDataSize; ++i)
            {
              //effect numbers 1,2....
              buffer.format(OD_T("%i"), i + 1); 
              COLLADASW::NativeString idStr(buffer.c_str());
              exportEffect.exportEffects(pDb, &m_pColladaMaterialData.m_matDataArr[i], idStr);
            }
          }

          MaterialExporter exportMaterial(&mStreamWriter);
          for (i = 0; i < intDataSize; ++i)
          {
            //material numbers 1,2....
            buffer.format(OD_T("%i"), i + 1); 
            COLLADASW::NativeString idStr(buffer.c_str());
            exportMaterial.exportMaterials(pDb, idStr);
          }
        }

        OdUInt32 iEntDataArrSize = m_entityDataArr.size();
        //export geometry
        {
          GeometryExporter exportGeometry(&mStreamWriter);
          for (iCurEntData = 0; iCurEntData < iEntDataArrSize; ++iCurEntData)
          {
            OdUInt32 intDataSize = m_entityDataArr[iCurEntData].size();
            for (i = 0; i < intDataSize; ++i)
            {
              exportGeometry.exportGeometry(pDb, &m_entityDataArr[iCurEntData][i], m_pColladaMaterialData);
            }
          }
        }

        //create visual scene
        {
          VisualSceneExporter vscene(&mStreamWriter);
          vscene.addLights(lightArr);
          for (iCurEntData = 0; iCurEntData < iEntDataArrSize; ++iCurEntData)
          {
            OdUInt32 intDataSize = m_entityDataArr[iCurEntData].size();
            for (i = 0; i < intDataSize; ++i)
              vscene.addVisualScene(pDb, &m_entityDataArr[iCurEntData][i]);
          }
        }

        COLLADASW::Scene scene ( &mStreamWriter, COLLADASW::URI ( "#VisualSceneNode" ) );        
        scene.add();

        mStreamWriter.endDocument();
      }
    }
    /*catch (COLLADABU::Exception* e)
    {
      ret = eExtendedError;
    }*/
    catch(...)
    {
      ret = eExtendedError;
    }
    return ret;
  }
};
