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
#ifndef _COLLADA_IMPORT_INCLUDED_
#define _COLLADA_IMPORT_INCLUDED_

#include "RxModule.h"
#include "RxDictionary.h"
#include "DynamicLinker.h"

class OdDbDatabase;

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT{

  /** \details
    This class implements the Collada importer.
  */
  class OdColladaImport : public OdRxObject
  {
   public:
     enum ImportResult { success, fail, bad_file, bad_database };
     virtual ImportResult import() = 0;

  // Documented properties are:
  // "Database"    - OdDbDatabase object, where dae is imported
  // "ColladaPath" - string, path to the imported dae file

     virtual OdRxDictionaryPtr properties() = 0;
  };

  typedef OdSmartPtr<OdColladaImport> OdColladaImportPtr;


  /** \details
    This class implements the Collada import module.
  */
  class ColladaImportModule : public OdRxModule
  {
   public:
     virtual OdColladaImportPtr create() = 0;
  };

  typedef OdSmartPtr<ColladaImportModule> ColladaImportModulePtr;

  inline OdColladaImportPtr createImporter()
  {
    ColladaImportModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdColladaImportModuleName, false);
    if ( !pModule.isNull() ) return pModule->create ();
    return (OdColladaImport*)0;
  }
}
#endif // _COLLADA_IMPORT_INCLUDED_
