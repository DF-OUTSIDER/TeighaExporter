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

#include "VisualSceneExporter.h"
#include "GeometryExporter.h"
#include "COLLADASWInstanceLight.h"

#include "Gs/GsBaseVectorizer.h"

namespace TD_COLLADA_EXPORT
{

  VisualSceneExporter::VisualSceneExporter(COLLADASW::StreamWriter * streamWriter):COLLADASW::LibraryVisualScenes ( streamWriter ) 
  {
    openVisualScene( COLLADASW::String("VisualSceneNode"), COLLADASW::String("VisualScene") );
  }


  VisualSceneExporter::~VisualSceneExporter()
  {
    closeLibrary();
  }

  void VisualSceneExporter::addLights(OdStringArray& pLightArr)
  {
    int iSize = pLightArr.size();
    for(int i = 0; i < iSize; i++)
    {
      COLLADASW::Node colladaNode( mSW );
      colladaNode.setType( COLLADASW::Node::NODE );
      COLLADASW::NativeString str(pLightArr[i].c_str());
      colladaNode.setNodeId(str.toString());
      colladaNode.start();
      
      COLLADASW::InstanceLight instanceLight ( mSW, "#" + str);
      instanceLight.add();

      colladaNode.end();
    }
  }

  void VisualSceneExporter::addVisualScene(OdDbBaseDatabase *pDb, ColladaEntData* pColladaData)
  {
    OdString buffer;
    buffer.format(OD_T("%i"), pColladaData->m_iId); 
    COLLADASW::NativeString idStr(buffer.c_str());

    COLLADASW::Node colladaNode( mSW );
    colladaNode.setType( COLLADASW::Node::NODE );
    colladaNode.setNodeName( COLLADASW::String("node") );
    colladaNode.setNodeId( COLLADASW::String("node") + idStr.toString());
    colladaNode.start();

    {
      buffer.format(OD_T("%i"), pColladaData->m_iEntMaterial + 1); 
      COLLADASW::NativeString matIDStr(buffer.c_str());
      COLLADASW::InstanceGeometry instanceGeometry ( mSW );
      instanceGeometry.setUrl( "#shape" + idStr + "-lib" ); 
      
      instanceGeometry.getBindMaterial().getInstanceMaterialList().push_back(COLLADASW::InstanceMaterial ( "Material" + idStr, "#ID" + matIDStr + "-material" ));
      

      //for material with texture
      //if(pColladaData->m_entMaterial.m_bMaterialHasTexture)
      {
        COLLADASW::InstanceMaterial& instanceMaterial = instanceGeometry.getBindMaterial().getInstanceMaterialList().back();
        const COLLADASW::String & inputSemantic = COLLADASW::InputList::getSemanticString(COLLADASW::InputSemantic::TEXCOORD);
        instanceMaterial.push_back( COLLADASW::BindVertexInput( "UVSET0", inputSemantic, 0) );
      }

      instanceGeometry.add();
    }

    colladaNode.end();
  }
}
