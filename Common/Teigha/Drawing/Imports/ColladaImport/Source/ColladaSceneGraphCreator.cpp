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
#include "ColladaSceneGraphCreator.h"
#include "COLLADAFWVisualScene.h"
#include "DbEntity.h"
#include "COLLADAFWNode.h"

namespace TD_COLLADA_IMPORT
{
  SceneGraphCreator::SceneGraphCreator( DocumentImporter* documentImporter )
    : ImporterBase(documentImporter)
  {}

  SceneGraphCreator::~SceneGraphCreator()
  {}

  bool SceneGraphCreator::create(const COLLADAFW::VisualScene* pVisualScene)
  {
    if ( !pVisualScene )
      return false;

    InternalNode parentNode;
    importNodes(pVisualScene->getRootNodes(), parentNode);
    return true;
  }


  void SceneGraphCreator::importNode( const COLLADAFW::Node* pNode, InternalNode& parentImportNode )
  {
    size_t iInstances = (pNode->getInstanceGeometries().getCount() /*+ 
                         pNode->getInstanceControllers().getCount() +
                         /*pNode->getInstanceCameras().getCount() + 
                         pNode->getInstanceLights().getCount() */);
    bool bSingleInstance = (iInstances == 1);

    InternalNode importNode(parentImportNode);
    importNode.m_matTransformation = collectTransformation(pNode, importNode.m_matTransformation);
    if ( bSingleInstance )
    {
      importInstanceGeometry( pNode, importNode );
    }
    else
    {
      importInstanceGeometries(pNode->getInstanceGeometries(), importNode);
    }
    importInstanceControllers(pNode->getInstanceControllers(), importNode);
    importNodes(pNode->getChildNodes(), importNode);
    // Append all nodes that are referenced by this node.
    importInstanceNodes(pNode->getInstanceNodes(), importNode);
  }


  //------------------------------
  bool SceneGraphCreator::importInstanceNodes( const COLLADAFW::InstanceNodePointerArray& instanceNodeArray, InternalNode& parentImportNode)
  {
    for ( size_t i = 0, count = instanceNodeArray.getCount(); i < count; ++i)
    {
      COLLADAFW::InstanceNode* instanceNode = instanceNodeArray[i];

      const COLLADAFW::UniqueId& uniqueId = instanceNode->getInstanciatedObjectId();
      OdDbObjectId objectId = getObjectByUniqueId(uniqueId);

      if( objectId.isNull() )
      {
        const COLLADAFW::Node* instanciatedFWNode = getFWNodeByUniqueId( uniqueId );
        // check if the referenced node is in one of the already received library nodes
        if ( instanciatedFWNode )
        {
          importNode( instanciatedFWNode, parentImportNode );
        }
      }
    }
    return true;
  }


  bool SceneGraphCreator::importNodes( const COLLADAFW::NodePointerArray& nodeArray, InternalNode& parentImportNode )
  {
    for ( size_t i = 0, count = nodeArray.getCount(); i < count; ++i)
    {
      importNode(nodeArray[i], parentImportNode);
    }
    return true;
  }

  template<class Instance, const COLLADAFW::PointerArray<Instance>& (COLLADAFW::Node::*getInstances)()const, void (SceneGraphCreator::*postProcess)( OdDbObjectId, Instance* )> 
  OdDbObjectId SceneGraphCreator::importInstance( const COLLADAFW::Node* pNode, InternalNode& parentImportNode )
  {
    const COLLADAFW::PointerArray<Instance>& instances = (pNode->*getInstances)();
    if ( instances.getCount() != 1 )
      return 0;

    Instance* instance = instances[0];
    const COLLADAFW::UniqueId& uniqueId = instance->getInstanciatedObjectId();
    OdDbObjectId objectId = getObjectByUniqueId(uniqueId);
    if(!objectId.isNull())
    {
      //clone object with transformation.
      OdDbObjectId oldObjectId = objectId;
      OdDbEntityPtr pEnt = objectId.safeOpenObject();
      OdDbEntityPtr pClone = pEnt->clone();
      if (pBTR_Tmp.isNull())
      {
        pBTR_Tmp = pEnt->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      }
      objectId = pBTR_Tmp->appendOdDbEntity(pClone);
      pClone->transformBy(parentImportNode.m_matTransformation);
      cloneObjectId(oldObjectId, objectId);
      addOdDbObjectIdUniqueIdPair(objectId, uniqueId);
      
      // post process the creation
      if ( postProcess )
        (this->*postProcess)(objectId, instance);

      return objectId;
    }//if(!objectId.isNull())
    return OdDbObjectId();
  }

  //------------------------------
  template<class Instance, void (SceneGraphCreator::*postProcess)( OdDbObjectId, Instance* )>
  bool SceneGraphCreator::importInstances( const COLLADAFW::PointerArray<Instance>& instanceArray, InternalNode& parentImportNode )
  {
    for ( size_t i = 0, count = instanceArray.getCount(); i < count; ++i)
    {
      Instance* instance = instanceArray[i];
      const COLLADAFW::UniqueId& uniqueId = instance->getInstanciatedObjectId();
      OdDbObjectId objectId = getObjectByUniqueId(uniqueId);
      const COLLADAFW::Controller* pContorller = getFWControllerByUniqueId(uniqueId);
      if (pContorller)
      {
        COLLADAFW::UniqueId prevId(pContorller->getSource());
        COLLADAFW::UniqueId currId(uniqueId);
        while (pContorller->getSource().getClassId() == COLLADAFW::COLLADA_TYPE::CONTROLLER && prevId != currId)
        {
          prevId = pContorller->getSource();
          pContorller = getFWControllerByUniqueId(prevId);
          currId = pContorller->getSource();
        }
        objectId = getObjectByUniqueId(pContorller->getSource());
      }     
      if(!objectId.isNull())
      {
        //clone object with transformation.
        OdDbObjectId oldObjectId = objectId;
        OdDbEntityPtr pEnt = objectId.safeOpenObject();
        OdDbEntityPtr pClone = pEnt->clone();
        if (pBTR_Tmp.isNull())
        {
          pBTR_Tmp = pEnt->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
        }
        objectId = pBTR_Tmp->appendOdDbEntity(pClone);
        pClone->transformBy(parentImportNode.m_matTransformation);
        cloneObjectId(oldObjectId, objectId);
        addOdDbObjectIdUniqueIdPair(objectId, uniqueId);
        
        // post process the creation
        if ( postProcess )
          (this->*postProcess)(objectId, instance);
      }
    }
    return true;
  }

  bool SceneGraphCreator::importInstanceGeometries( const COLLADAFW::InstanceGeometryPointerArray& instanceGeometryArray, InternalNode& parentImportNode )
  {
    return importInstances<COLLADAFW::InstanceGeometry, &SceneGraphCreator::storeMaterialBindings<COLLADAFW::COLLADA_TYPE::INSTANCE_GEOMETRY> >(instanceGeometryArray, parentImportNode);
  }

  OdDbObjectId SceneGraphCreator::importInstanceGeometry( const COLLADAFW::Node* pNode, InternalNode& parentImportNode )
  {
    return importInstance<COLLADAFW::InstanceGeometry, &COLLADAFW::Node::getInstanceGeometries, &SceneGraphCreator::storeMaterialBindings<COLLADAFW::COLLADA_TYPE::INSTANCE_GEOMETRY> >(pNode, parentImportNode);
  }

  bool SceneGraphCreator::importInstanceControllers(const COLLADAFW::InstanceControllerPointerArray& instanceControllerArray, InternalNode& parentImportNode)
  {
    return importInstances<COLLADAFW::InstanceController, &SceneGraphCreator::storeMaterialBindings<COLLADAFW::COLLADA_TYPE::INSTANCE_CONTROLLER> >(instanceControllerArray, parentImportNode);
  }

  OdGeMatrix3d SceneGraphCreator::collectTransformation( const COLLADAFW::Node* pNode, OdGeMatrix3d& matTransformation)
  {
    COLLADABU::Math::Matrix4 transformationMatrix;
    pNode->getTransformationMatrix(transformationMatrix);
    OdGeMatrix3d mat = OdGeMatrix3d::kIdentity;
    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
      {
        mat[i][j] = transformationMatrix[i][j];
      }
    }
    return matTransformation * mat;
  }
}
