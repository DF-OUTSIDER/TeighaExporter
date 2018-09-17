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

#ifndef _ODCOLLADADOCUMENTIMPORT_INCLUDED_
#define _ODCOLLADADOCUMENTIMPORT_INCLUDED_
#pragma once
#include "OdaCommon.h"
#include "ColladaImportDef.h"
#include "COLLADABU.h"
#include "COLLADAFWIWriter.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWEffect.h"
#include "COLLADAFWColor.h"
#include "COLLADAFWImage.h"
#include "COLLADAFWController.h"
#include "COLLADAFWNode.h"
#include "ColladaErrorHandler.h"
#include "DbDatabase.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "COLLADAFWMaterialBinding.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  class ExtraDataHandler;

  /** \details
    This class implements the document importer.
  */
  class DocumentImporter : COLLADAFW::IWriter
  {
    bool m_bConsoleOut;
    bool m_bImportTextures;
  public:
    /** Maps unique ids of framework images to the corresponding framework image.
    */
    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Image> UniqueIdFWImageMap;

    /** Maps unique ids of framework effects to the corresponding framework material.
    */
    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Effect> UniqueIdFWEffectMap;

    /** Maps unique ids of framework materials to the corresponding framework material.
    */
    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Material> UniqueIdFWMaterialMap;

    /** Maps Unique id to framework nodes.
    */
    typedef std::map<COLLADAFW::UniqueId, const COLLADAFW::Node*> UniqueIdFWNodeMap;

    /** Vector of material bindings.
    */
    typedef std::vector<COLLADAFW::MaterialBinding> MaterialBindingVector;

    /** Maps objects id to material id.
    */
    typedef std::map<OdDbObjectId, COLLADAFW::MaterialId > ObjectMaterialIdMap;

    /** List of library nodes.
    */
    typedef std::list<const COLLADAFW::LibraryNodes*> LibraryNodesList;

    /** Maps objects to Unique id to.
    */
    typedef std::map<COLLADAFW::UniqueId, OdDbObjectId> UniqueIdOdDbObjectIdMap;

    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Controller> UniqueIdFWControllerMap;

    typedef std::map< OdDbObjectId, COLLADAFW::UniqueId> OdDbObjectIdUniqueIdMap;

    typedef std::map<COLLADAFW::UniqueId, const COLLADAFW::VisualScene*> UniqueIdVisualSceneMap;

    typedef std::map<OdDbObjectId, COLLADAFW::String > ObjectObjectNameMap;


    /** Pair of nodes an the instantiated materials.
    */
    struct EntityMaterialBindingsPair
    {
      OdDbObjectId objId;
      MaterialBindingVector materialBindings;
    };

    /** \details
      This stucture implements the file information block.
    */
    struct FileInfo
    {
      COLLADABU::URI absoluteFileUri;
      float unitScale;
    };

    /** List of all max nodes that reference a material and their material bindings.
    */
    typedef std::list<EntityMaterialBindingsPair> EntityMaterialBindingsList;

  private:
    ExtraDataHandler*          m_ExtraDataHandler;
    OdString                   m_strImportFilePath;
    OdDbDatabasePtr            m_pDatabase;
    
    UniqueIdFWImageMap         m_UniqueIdFWImageMap;
    UniqueIdFWEffectMap        m_UniqueIdFWEffectMap;
    UniqueIdFWMaterialMap      m_UniqueIdFWMaterialMap;
    UniqueIdFWNodeMap          m_UniqueIdFWNodeMap;
    UniqueIdOdDbObjectIdMap    m_UniqueIdOdDbObjectIdMap;
    UniqueIdFWControllerMap    m_UniqueIdFWControllerMap;
    UniqueIdVisualSceneMap     m_UniqueIdVisualSceneMap;
    LibraryNodesList           m_LibraryNodesList;
    ObjectObjectNameMap        m_ObjectObjectNameMap;
    ObjectMaterialIdMap        m_ObjectMaterialIdMap;
    FileInfo                   m_FileInfo;


    EntityMaterialBindingsList m_EntityMaterialBindingsList;
    OdDbObjectIdUniqueIdMap    m_OdDbObjectIdUniqueIdMap;

    OdDbBlockTableRecordPtr    m_pTmpGeometryRecord;

    /** Disable default copy ctor. 
    */
    DocumentImporter( const DocumentImporter& pre );
    /** Disable default assignment operator. 
    */
    const DocumentImporter& operator= ( const DocumentImporter& pre );

  public:
    DocumentImporter(const OdString &filepath, OdDbDatabase* m_pDatabase, bool bImportTextures, bool bConsoleOut);

    virtual ~DocumentImporter();

    bool import();

    bool createSceneGraph();

    OdDbDatabasePtr getDatabase() {return m_pDatabase;}

    OdDbBlockTableRecordPtr getGeometryTmpBTR() { return m_pTmpGeometryRecord; }

   	/** This method will be called if an error in the loading process occurred and the loader cannot
		continue to to load. The writer should undo all operations that have been performed.
		@param errorMessage A message containing informations about the error that occurred.
   	*/
    virtual void cancel(const COLLADAFW::String& errorMessage);

		/** This is the method called. The writer hast to prepare to receive data.
		*/
		virtual void start();

		/** This method is called after the last write* method. No other methods will be called after this.
		*/
		virtual void finish();

    /** When this method is called, the writer must write the global document asset.
    @return The writer should return true, if writing succeeded, false otherwise.
    */
    virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* pAsset );

    /** When this method is called, the writer must write the scene.
    @return The writer should return true, if writing succeeded, false otherwise.
    */
    virtual bool writeScene ( const COLLADAFW::Scene* scene );

		/** When this method is called, the writer must write the entire visual scene.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );

		/** When this method is called, the writer must handle all nodes contained in the 
		library nodes.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes );

		/** When this method is called, the writer must write the geometry.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry );

		/** When this method is called, the writer must write the material.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeMaterial( const COLLADAFW::Material* material );

		/** When this method is called, the writer must write the effect.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeEffect( const COLLADAFW::Effect* effect );

		/** When this method is called, the writer must write the camera.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeCamera( const COLLADAFW::Camera* camera );

		/** When this method is called, the writer must write the image.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeImage( const COLLADAFW::Image* image );

		/** When this method is called, the writer must write the light.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeLight( const COLLADAFW::Light* light );

		/** When this method is called, the writer must write the Animation.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeAnimation( const COLLADAFW::Animation* animation );

		/** When this method is called, the writer must write the AnimationList.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList );

		/** When this method is called, the writer must write the skin controller data.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData );

		/** When this method is called, the writer must write the controller.
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeController( const COLLADAFW::Controller* controller );

                /** When this method is called, the writer must write the formulas. All the 
                formulas of the entire COLLADA file are contained in formulas. The writer should 
                return true, if writing succeeded, false otherwise.
                */
                virtual bool writeFormulas( const COLLADAFW::Formulas* formulas );

		/** When this method is called, the writer must write the kinematics scene. 
		@return The writer should return true, if writing succeeded, false otherwise.
		*/
		virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene );

  private:
    /** Returns the UniqueIdFWImageMap.
    */
    UniqueIdFWImageMap& getUniqueIdFWImageMap() { return m_UniqueIdFWImageMap; }

    /** Returns the UniqueIdFWEffectMap.
    */
    UniqueIdFWEffectMap& getUniqueIdFWEffectMap() { return m_UniqueIdFWEffectMap; }

    /** Returns the UniqueIdFWMaterialMap.
    */
    UniqueIdFWMaterialMap& getUniqueIdFWMaterialMap() { return m_UniqueIdFWMaterialMap; }

    UniqueIdFWNodeMap& getUniqueIdFWNodeMap() { return m_UniqueIdFWNodeMap; }

    /** Returns the ObjectMaterialBindingsList.
    */
    EntityMaterialBindingsList& getEntityMaterialBindingsList() { return m_EntityMaterialBindingsList; }

    OdDbObjectIdUniqueIdMap& getOdDbObjectIdUniqueIdMap(){ return m_OdDbObjectIdUniqueIdMap; }

    UniqueIdFWControllerMap& getUniqueIdFWControllerMap() { return m_UniqueIdFWControllerMap; }

    UniqueIdOdDbObjectIdMap& getUniqueIdOdDbObjectIdMap(){ return m_UniqueIdOdDbObjectIdMap; }

    UniqueIdVisualSceneMap& getUniqueIdVisualSceneMap(){ return m_UniqueIdVisualSceneMap; }

    LibraryNodesList& getLibraryNodesList(){ return m_LibraryNodesList; }

    ObjectObjectNameMap& getObjectObjectNameMap() { return m_ObjectObjectNameMap; }

    ObjectMaterialIdMap& getObjectMaterialIdMap() { return m_ObjectMaterialIdMap; }

    const FileInfo& getFileInfo() const { return m_FileInfo; }

    friend class ImporterBase;
  };
};

#endif //_ODCOLLADADOCUMENTIMPORT_INCLUDED_
