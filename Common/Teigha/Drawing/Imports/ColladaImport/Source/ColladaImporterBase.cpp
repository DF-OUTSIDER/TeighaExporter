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
#include "COLLADAFWUniqueId.h"
#include "COLLADAFWAnimationList.h"
#include "COLLADAFWVisualScene.h"
#include "ColladaImporterBase.h"

namespace TD_COLLADA_IMPORT
{
	ImporterBase::ImporterBase( DocumentImporter* documentImporter)
		:	m_DocumentImporter(documentImporter)
	{
	}

	ImporterBase::~ImporterBase()
	{
	}

  void ImporterBase::addUniqueIdFWImagePair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Image& Image )
  {
    m_DocumentImporter->getUniqueIdFWImageMap().insert(std::pair<COLLADAFW::UniqueId, COLLADAFW::Image>(uniqueId, Image));
  }

  void ImporterBase::addUniqueIdFWEffectPair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Effect& effect )
  {
    m_DocumentImporter->getUniqueIdFWEffectMap().insert(std::pair<COLLADAFW::UniqueId, COLLADAFW::Effect>(uniqueId, effect));
  }

  void ImporterBase::addUniqueIdFWMaterialPair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Material& material )
  {
    m_DocumentImporter->getUniqueIdFWMaterialMap().insert(std::pair<COLLADAFW::UniqueId, COLLADAFW::Material>(uniqueId, material));
  }

  void ImporterBase::addUniqueIdFWNodePair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Node* node )
  {
    m_DocumentImporter->getUniqueIdFWNodeMap()[uniqueId] = node;
  }

  void ImporterBase::addVisualScene( const COLLADAFW::VisualScene* visualScene )
  {
    DocumentImporter::UniqueIdVisualSceneMap& map = m_DocumentImporter->getUniqueIdVisualSceneMap();
    map[ visualScene->getUniqueId() ] = visualScene;
  }

  void ImporterBase::addLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes )
  {
    m_DocumentImporter->getLibraryNodesList().push_back(libraryNodes);
  }

  void ImporterBase::addUniqueIdOdDbObjectIdPair( const COLLADAFW::UniqueId& uniqueId, OdDbObjectId objectId )
  {
    m_DocumentImporter->getUniqueIdOdDbObjectIdMap()[uniqueId] = objectId;
  }

  void ImporterBase::addObjectObjectNamePair( OdDbObjectId objectId, const COLLADAFW::String& name )
  {
    DocumentImporter::ObjectObjectNameMap& objectObjectNameMap = m_DocumentImporter->getObjectObjectNameMap();
    objectObjectNameMap.insert(std::pair<OdDbObjectId, const COLLADAFW::String>(objectId, name));
  }

  void ImporterBase::addObjectMaterialIdPair(OdDbObjectId objectId, const COLLADAFW::MaterialId& matId)
  {
    DocumentImporter::ObjectMaterialIdMap& objectNameMap = m_DocumentImporter->getObjectMaterialIdMap();
    objectNameMap.insert(std::pair<OdDbObjectId, const COLLADAFW::MaterialId>(objectId, matId));
  }

  void ImporterBase::addUniqueIdFWControllerPair(const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Controller& controller)
  {
    m_DocumentImporter->getUniqueIdFWControllerMap().insert(std::pair<COLLADAFW::UniqueId, COLLADAFW::Controller>(uniqueId, controller));
  }

  const COLLADAFW::Node* ImporterBase::getFWNodeByUniqueId( const COLLADAFW::UniqueId& uniqueId )
  {
    const DocumentImporter::UniqueIdFWNodeMap& uniqueIdFWNodeMap = m_DocumentImporter->getUniqueIdFWNodeMap();
    DocumentImporter::UniqueIdFWNodeMap::const_iterator it = uniqueIdFWNodeMap.find(uniqueId);
    if ( it == uniqueIdFWNodeMap.end() )
      return 0;
    else
      return it->second;
  }
  
  OdDbObjectId ImporterBase::getObjectByUniqueId( const COLLADAFW::UniqueId& uniqueId )
  {
    const DocumentImporter::UniqueIdOdDbObjectIdMap& uniqueIdObjectMap = m_DocumentImporter->getUniqueIdOdDbObjectIdMap();
    DocumentImporter::UniqueIdOdDbObjectIdMap::const_iterator it = uniqueIdObjectMap.find(uniqueId);
    if ( it == uniqueIdObjectMap.end() )
      return 0;
    else
      return it->second;
  }

  bool ImporterBase::cloneObjectId(const OdDbObjectId& oldObjId, const OdDbObjectId& newObjId)
  {
    DocumentImporter::ObjectMaterialIdMap& objectMaterialMap = m_DocumentImporter->getObjectMaterialIdMap();
    DocumentImporter::ObjectMaterialIdMap::iterator it = objectMaterialMap.find(oldObjId);
    if (it != objectMaterialMap.end())
    {
      COLLADAFW::MaterialId matId = it->second;
      objectMaterialMap.insert(std::pair<OdDbObjectId, const COLLADAFW::MaterialId>(newObjId, matId));
      return true;
    }
    return false;
  }

  COLLADAFW::MaterialId* ImporterBase::getMaterialIdByObectId(const OdDbObjectId& objId)
  {
    DocumentImporter::ObjectMaterialIdMap& objectMaterialMap = m_DocumentImporter->getObjectMaterialIdMap();
    DocumentImporter::ObjectMaterialIdMap::iterator it = objectMaterialMap.find(objId);
    if (it == objectMaterialMap.end())
      return 0;
    else
      return &it->second;
  }

  void ImporterBase::addOdDbObjectIdUniqueIdPair( OdDbObjectId objectId, const COLLADAFW::UniqueId& uniqueId )
  {
    m_DocumentImporter->getOdDbObjectIdUniqueIdMap()[objectId] = uniqueId;
  }

  const COLLADAFW::Image* ImporterBase::getFWImageByUniqueId( const COLLADAFW::UniqueId& uniqueId )
  {
    const DocumentImporter::UniqueIdFWImageMap& uniqueIdFWImageMap = m_DocumentImporter->getUniqueIdFWImageMap();
    DocumentImporter::UniqueIdFWImageMap::const_iterator it = uniqueIdFWImageMap.find(uniqueId);
    if ( it == uniqueIdFWImageMap.end() )
      return 0;
    else
      return &it->second;
  }
  
  const COLLADAFW::Controller* ImporterBase::getFWControllerByUniqueId(const COLLADAFW::UniqueId& uniqueId)
  {
    const DocumentImporter::UniqueIdFWControllerMap& uniqueIdFWControllerMap = m_DocumentImporter->getUniqueIdFWControllerMap();
    DocumentImporter::UniqueIdFWControllerMap::const_iterator it = uniqueIdFWControllerMap.find(uniqueId);
    if (it == uniqueIdFWControllerMap.end())
      return 0;
    else
      return &it->second;
  }

  const DocumentImporter::FileInfo& ImporterBase::getFileInfo() const 
  { 
    return m_DocumentImporter->getFileInfo(); 
  }





  const DocumentImporter::UniqueIdFWMaterialMap& ImporterBase::getUniqueIdFWMaterialMap()
  {
    return m_DocumentImporter->getUniqueIdFWMaterialMap();
  }

  const DocumentImporter::EntityMaterialBindingsList& ImporterBase::getEntityMaterialBindings()
  {
    return m_DocumentImporter->getEntityMaterialBindingsList();
  }
  
  const COLLADAFW::UniqueId& ImporterBase::getUniqueIdByObjectId( OdDbObjectId& objId )
  {
    const DocumentImporter::OdDbObjectIdUniqueIdMap& objectIDUniqueIdMap = m_DocumentImporter->getOdDbObjectIdUniqueIdMap();
    DocumentImporter::OdDbObjectIdUniqueIdMap::const_iterator it = objectIDUniqueIdMap.find(objId);
    if ( it == objectIDUniqueIdMap.end() )
      return COLLADAFW::UniqueId::INVALID;
    else
      return it->second;
  }

  const COLLADAFW::Material* ImporterBase::getFWMaterialByUniqueId( const COLLADAFW::UniqueId& uniqueId )
  {
    const DocumentImporter::UniqueIdFWMaterialMap& uniqueIdFWMaterialMap = m_DocumentImporter->getUniqueIdFWMaterialMap();
    DocumentImporter::UniqueIdFWMaterialMap::const_iterator it = uniqueIdFWMaterialMap.find(uniqueId);
    if ( it == uniqueIdFWMaterialMap.end() )
      return 0;
    else
      return &it->second;
  }

  const COLLADAFW::Effect* ImporterBase::getFWEffectByUniqueId( const COLLADAFW::UniqueId& uniqueId )
  {
    const DocumentImporter::UniqueIdFWEffectMap& uniqueIdFWEffectMap = m_DocumentImporter->getUniqueIdFWEffectMap();
    DocumentImporter::UniqueIdFWEffectMap::const_iterator it = uniqueIdFWEffectMap.find(uniqueId);
    if ( it == uniqueIdFWEffectMap.end() )
      return 0;
    else
      return &it->second;
  }

  DocumentImporter::EntityMaterialBindingsPair& ImporterBase::createAndAddNodeMaterialBindingsPair( OdDbObjectId objId )
  {
    DocumentImporter::EntityMaterialBindingsPair newPair;
    newPair.objId = objId;
    DocumentImporter::EntityMaterialBindingsList& list =  m_DocumentImporter->getEntityMaterialBindingsList();
    list.push_back(newPair);
    return *list.rbegin();
  }

  const bool ImporterBase::isImportTextures()
  {
    return m_DocumentImporter->m_bImportTextures;
  }
} 
