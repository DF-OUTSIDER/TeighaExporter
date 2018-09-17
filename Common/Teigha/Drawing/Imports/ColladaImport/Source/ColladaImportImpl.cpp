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
#include "Ed/EdCommandContext.h"
#include "ColladaImportImpl.h"

#include "ColladaDocumentImporter.h"
#include "DbCommandContext.h"



namespace TD_COLLADA_IMPORT{

class ColladaImportCommand : public OdEdCommand
{
public:
  const OdString groupName() const { return OD_T("IMPORTS"); }
  const OdString globalName() const { return OD_T("ColladaIn"); };
  void execute(OdEdCommandContext* pCmdCtx);
};


void ColladaImportCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdSmartPtr<OdEdUserIO> pIO = pCmdCtx->userIO();
  OdString path = pIO->getFilePath( OD_T("Enter file name:"), OdEd::kGfpForOpen, OD_T("Collada file to load"),
    OD_T("dae"), OdString::kEmpty, OD_T("Design Web Format (*.dae)|*.dae|"));
  if ( path.isEmpty() )
    return;
 
  OdColladaImportPtr importer = createImporter();
  OdRxDictionaryPtr pProps = importer->properties();
  // Target database
  pProps->putAt( OD_T("Database"), OdDbCommandContextPtr(pCmdCtx)->database() );
  // Source Collada(.dae) file
  pProps->putAt( OD_T("ColladaPath"), OdRxVariantValue(path) );
  pProps->putAt( OD_T("ImportTextures"), OdRxVariantValue( pIO->getKeyword(OD_T("Enable import of textures [Yes/No] <No>:"), OD_T("Yes No"), 1 ) == 0 ) );
  pProps->putAt( OD_T("ConsoleInfo"), OdRxVariantValue( pIO->getKeyword(OD_T("Output information at console  [Yes/No] <Yes>:"), OD_T("Yes No"), 0 ) == 0 ) );
  importer->import();
}


class OdColladaImportModule : public ColladaImportModule
{
  OdStaticRxObject<ColladaImportCommand> m_Command;
public:
  virtual void initApp()
  {
#ifndef TD_NOCOMMANDS
    odedRegCmds()->addCommand( &m_Command );
#endif
  }
  virtual void uninitApp()
  {
#ifndef TD_NOCOMMANDS
    odedRegCmds()->removeCmd( m_Command.groupName(), m_Command.globalName() );
#endif
  }

  virtual OdColladaImportPtr create ()
  {
    return OdRxObjectImpl<OdColladaImportImpl>::createObject();
  }

};
}
using namespace TD_COLLADA_IMPORT;
ODRX_DEFINE_DYNAMIC_MODULE(OdColladaImportModule);



namespace TD_COLLADA_IMPORT{
OdColladaImportImpl::OdColladaImportImpl():
  m_properties( ColladaProperties::createObject() )
{
}

OdColladaImport::ImportResult OdColladaImportImpl::import()
{
  m_database = m_properties->get_Database();
  if ( m_database.isNull() ) 
    return OdColladaImport::bad_database;

  m_database->closeInput();  // some problems (with viewport) in case that db was open partially

  DocumentImporter cldImporter(m_properties->get_ColladaPath(), dynamic_cast<OdDbDatabase*>(m_properties->get_Database().get()), 
    m_properties->get_ImportTextures(), m_properties->get_ConsoleInfo());
  return cldImporter.import() ? OdColladaImport::success : OdColladaImport::fail;
}

OdRxDictionaryPtr OdColladaImportImpl::properties() { return m_properties; }
}
