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

#include "ColladaDocumentImporter.h"
#include "ColladaFWLErrorHandler.h"
#include "COLLADASaxFWLLoader.h"
#include "COLLADAFWRoot.h"
#include "ColladaEffectImporter.h"
#include "ColladaImageImporter.h"
#include "ColladaGeometryImporter.h"
#include "ColladaLightImporter.h"
#include "ColladaControllerImporter.h"
#include "ColladaMaterialImporter.h"
#include "ColladaExtraDataHandler.h"
#include "ColladaMaterialCreator.h"
#include "ColladaVisualSceneImporter.h"
#include "ColladaLibraryNodesImporter.h"
#include "ColladaSceneGraphCreator.h"
#include "COLLADAFWFileInfo.h"

void odPrint4Collada(const wchar_t* fmt, ...)
{
  va_list argList;
  va_start(argList, fmt);
  OdString s;
  s.formatV(fmt, argList);
  va_end(argList);
#if defined(OD_HAVE_GETSTDHANDLE_FUNC)
  DWORD dw;
  HANDLE o = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (o != NULL)
  {
    if (::GetFileType(o) == FILE_TYPE_CHAR)
      ::WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
    else
      ::WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
  }
#elif defined(ANDROID)
  printf("%s", (const char*)s);
#elif defined(_WINRT)
  g_printConsoleFunc(L"%ls", s.c_str());
#else
  printf("%ls", s.c_str());
#endif
}

namespace TD_COLLADA_IMPORT
{
  DocumentImporter::DocumentImporter(const OdString &filepath, OdDbDatabase* pDatabase, bool bImportTextures, bool bConsoleOut)
    : m_strImportFilePath(filepath)
    , m_pDatabase(pDatabase)
    , m_bImportTextures(bImportTextures)
    , m_bConsoleOut(bConsoleOut)
  {
    m_ExtraDataHandler = new ExtraDataHandler(this);    
  }

  DocumentImporter::~DocumentImporter()
  {
    delete m_ExtraDataHandler; 
  }

  bool DocumentImporter::createSceneGraph()
  {
    UniqueIdVisualSceneMap::const_iterator it = m_UniqueIdVisualSceneMap.begin();
    if ( it != m_UniqueIdVisualSceneMap.end() )
    {
      SceneGraphCreator sceneGraphCreator(this);
      return sceneGraphCreator.create(it->second);
    }
    return true;
  }

  bool DocumentImporter::import()
  {
    if (m_pDatabase.isNull())
      return false;

    //--------------------------------------------------------------------------------------------------------------
    //Temporary BlockTableRecord for keeping geometry. 
    //It must be removed before storing the result of imports. 
    //It needed for prevent the situation when we have a geometry but there is no any link on it from visual scene.
    OdDbBlockTablePtr pTable  = m_pDatabase->getBlockTableId().safeOpenObject(OdDb::kForWrite);
    m_pTmpGeometryRecord = OdDbBlockTableRecord::createObject();
    m_pTmpGeometryRecord->setName("TemporaryGeometry");
    pTable->add(m_pTmpGeometryRecord);
    //--------------------------------------------------------------------------------------------------------------

    FWLErrorHandler errorHandler;
    COLLADASaxFWL::Loader loader(&errorHandler);
    loader.registerExtraDataCallbackHandler(m_ExtraDataHandler);
    COLLADAFW::Root root(&loader, this);

    COLLADABU::NativeString sPath(m_strImportFilePath.c_str());
    if (!root.loadDocument(sPath.toString()))
    {
      m_pTmpGeometryRecord->erase();
      return false;
    }
      
    if (!createSceneGraph())
    {
      m_pTmpGeometryRecord->erase();
      return false;
    }

    MaterialCreator materialCreator(this);
    if (!materialCreator.create())
    {
      m_pTmpGeometryRecord->erase();
      return false;
    }

    m_pTmpGeometryRecord->erase();

    return true;
  }

  /** This method will be called if an error in the loading process occurred and the loader cannot
	continue to to load. The writer should undo all operations that have been performed.
	@param errorMessage A message containing informations about the error that occurred.
	*/
  void DocumentImporter::cancel(const COLLADAFW::String& errorMessage)
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"Collada import canceled");
  }


	void DocumentImporter::start()
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import started");
  }


	void DocumentImporter::finish()
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import finished");
  }

  bool DocumentImporter::writeGlobalAsset( const COLLADAFW::FileInfo* pAsset )
  {
    m_FileInfo.absoluteFileUri = pAsset->getAbsoluteFileUri();
    //skip all assets
    return true;
  }

  bool DocumentImporter::writeScene ( const COLLADAFW::Scene* scene )
  {
    return true;
  }


	bool DocumentImporter::writeVisualScene ( const COLLADAFW::VisualScene* visualScene )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeVisualScene");
    VisualSceneImporter visualSceneImporter(this);
    return visualSceneImporter.import(visualScene);
  }


	bool DocumentImporter::writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeLibraryNodes");
    LibraryNodesImporter libraryNodesImporter(this);
    return libraryNodesImporter.import(libraryNodes);
  }


	bool DocumentImporter::writeGeometry ( const COLLADAFW::Geometry* geometry )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeGeometry");
    GeometryImporter geometryImporter(this);
    return geometryImporter.import(geometry);
  }


	bool DocumentImporter::writeMaterial( const COLLADAFW::Material* material )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeMaterial");
    MaterialImporter materialImporter(this);
    return materialImporter.import(material);
  }

	bool DocumentImporter:: writeEffect( const COLLADAFW::Effect* effect )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeEffect");
    EffectImporter effectImporter(this);
    return effectImporter.import(effect);
  }

	bool DocumentImporter::writeCamera( const COLLADAFW::Camera* camera )
  {
    return true;
  }

  

	bool DocumentImporter::writeImage( const COLLADAFW::Image* image )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeImage");
    ImageImporter imageImporter(this);
    return imageImporter.import(image);
  }

	bool DocumentImporter::writeLight( const COLLADAFW::Light* pLight )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeLight");
    LightImporter lightImporter(this);
    return lightImporter.import(*pLight);
  }

	bool DocumentImporter::writeAnimation( const COLLADAFW::Animation* animation )
  {
    return true;
  }

	bool DocumentImporter::writeAnimationList( const COLLADAFW::AnimationList* animationList )
  {
    return true;
  }

	bool DocumentImporter::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData )
  {
    return true;
  }

	bool DocumentImporter::writeController( const COLLADAFW::Controller* controller )
  {
    if (m_bConsoleOut)
      odPrint4Collada(L"\n Collada import writeController");
    ControllerImporter controllerImporter(this);
    return controllerImporter.import(*controller);
  }

	bool DocumentImporter::writeFormulas( const COLLADAFW::Formulas* formulas )
  {
    return true;
  }

	bool DocumentImporter::writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene )
  {
    return true;
  }
};
