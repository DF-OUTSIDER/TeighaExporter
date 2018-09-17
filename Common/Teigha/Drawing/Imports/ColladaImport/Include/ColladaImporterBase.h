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
#ifndef __COLLADA_IMPORTERBASE_H__
#define __COLLADA_IMPORTERBASE_H__

#include "OdaCommon.h"
#include "COLLADAFWLight.h"
#include "ColladaDocumentImporter.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
        /** \details
          This is the base class for Collada importer.
        */
  class ImporterBase
  {
  private:
    /** The collada importer the importer belongs to.
    */
    DocumentImporter* m_DocumentImporter;

  protected:
    /** \details 
      Constructor. 
      \param colladaImporter [in] The collada importer the importer belongs to.
    */
    ImporterBase(DocumentImporter* colladaImporter);

    /** \details
      Destructor. 
    */
    virtual ~ImporterBase();
 
    DocumentImporter* getDocumentImporter() { return m_DocumentImporter; }
 
    void addUniqueIdFWImagePair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Image& image );
 
    void addUniqueIdFWEffectPair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Effect& effect );
 
    void addUniqueIdFWMaterialPair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Material& material );

    void addUniqueIdFWNodePair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Node* node );

    void addVisualScene( const COLLADAFW::VisualScene* visualScene );

    void addLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes );

    void addUniqueIdOdDbObjectIdPair( const COLLADAFW::UniqueId& uniqueId, OdDbObjectId objectId );

    template<class FWObject>
    bool handleObjectReferences( FWObject* pFWobject, OdDbObjectId objectId );

    void addObjectObjectNamePair( OdDbObjectId objectId, const COLLADAFW::String& name);

    void addObjectMaterialIdPair(OdDbObjectId objectId, const COLLADAFW::MaterialId& matId);

    void addUniqueIdFWControllerPair(const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Controller& controller);

    const COLLADAFW::Node* getFWNodeByUniqueId( const COLLADAFW::UniqueId& uniqueId );

    OdDbObjectId getObjectByUniqueId( const COLLADAFW::UniqueId& uniqueId );

    bool cloneObjectId(const OdDbObjectId& oldObjId, const OdDbObjectId& newObjId);

    COLLADAFW::MaterialId* getMaterialIdByObectId(const OdDbObjectId& oldObjId);

    DocumentImporter::EntityMaterialBindingsPair& createAndAddNodeMaterialBindingsPair( OdDbObjectId objId );

    void addOdDbObjectIdUniqueIdPair( OdDbObjectId objectId, const COLLADAFW::UniqueId& uniqueId );

    const COLLADAFW::Image* getFWImageByUniqueId( const COLLADAFW::UniqueId& uniqueId );

    const COLLADAFW::Controller* getFWControllerByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    const DocumentImporter::FileInfo& getFileInfo() const;

    const bool isImportTextures();

    const DocumentImporter::UniqueIdFWMaterialMap& getUniqueIdFWMaterialMap();

    const COLLADAFW::UniqueId& getUniqueIdByObjectId( OdDbObjectId& objId );

    const DocumentImporter::EntityMaterialBindingsList& getEntityMaterialBindings();

    const COLLADAFW::Material* getFWMaterialByUniqueId( const COLLADAFW::UniqueId& uniqueId );

    const COLLADAFW::Effect* getFWEffectByUniqueId( const COLLADAFW::UniqueId& uniqueId );
  private:
    /** Disable default copy ctor. 
    */
    ImporterBase( const ImporterBase& pre );
    /** Disable default assignment operator. 
    */
    const ImporterBase& operator= ( const ImporterBase& pre );
  };


  template<class FWObject>
  bool ImporterBase::handleObjectReferences( FWObject* pFWobject, OdDbObjectId objectId )
  {
    const COLLADAFW::UniqueId& uniqueID = pFWobject->getUniqueId();

    const COLLADAFW::String& objectName = pFWobject->getName();
    addUniqueIdOdDbObjectIdPair(uniqueID, objectId);
    if ( !objectName.empty() )
      addObjectObjectNamePair(objectId, objectName);

    return true;
  }
}

#endif // __COLLADA_IMPORTERBASE_H__
