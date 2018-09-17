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

#ifndef _DGNIMPORT_INCLUDED_
#define _DGNIMPORT_INCLUDED_

#include "RxModule.h"
#include "RxDictionary.h"
#include "DynamicLinker.h"
#include "OdPlatformSettings.h"

class OdDbDatabase;

//DD:EXPORT_ON

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DGN_IMPORT {

/** \details
  This class implements the DGN importer.
*/
class OdDgnImport : public OdRxObject
{
public:
  enum ImportResult { success, fail, bad_password, bad_file, bad_database, encrypted_file};
  virtual ImportResult import() = 0;

  /** \details
    Passes parameters for Dgn importing.
    
    \remarks
  
    See Teigha Developer's Guide, DGN Input Parameters for the list of possible parameters.
  */

  virtual OdRxDictionaryPtr properties() = 0;
};

typedef OdSmartPtr<OdDgnImport> OdDgnImportPtr;

/** \details
  This class implements the DGN import module.
*/
class OdDgnImportModule : public OdRxModule
{
public:
    virtual OdDgnImportPtr create () = 0;
    
};

typedef OdSmartPtr<OdDgnImportModule> OdDgnImportModulePtr;

inline OdDgnImportPtr createDgnImporter()
{
     OdDgnImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDgnImportModuleName);
     if ( !pModule.isNull() ) return pModule->create ();
     return (OdDgnImport*)0;
}

/** \details
  This class implements the DGN import line weights map.
*/
class OdDgnImportLineWeightsMap;
typedef OdSmartPtr<OdDgnImportLineWeightsMap> OdDgnImportLineWeightsMapPtr;

class OdDgnImportLineWeightsMap : public OdRxObject
{
public:
  virtual OdResult         setLineWeightForDgnIndex( OdUInt32 uIndex, OdDb::LineWeight uLineWeigth ) = 0;
  virtual void             copyLineWeightsMap( const OdDgnImportLineWeightsMapPtr& pMap ) = 0;
  virtual OdDb::LineWeight getLineWeightByDgnIndex( OdUInt32 uIndex ) const = 0;
  virtual void             resetToDefaults() = 0;
};

}
//DD:EXPORT_OFF

#endif // _DGNIMPORT_INCLUDED_
