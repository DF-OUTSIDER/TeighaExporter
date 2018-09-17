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

#ifndef _DWFIMPORT_INCLUDED_
#define _DWFIMPORT_INCLUDED_

#include "RxModule.h"
#include "RxDictionary.h"
#include "DynamicLinker.h"

//////////////////////////////////////////////////////////////////////////

#include "OdPlatformSettings.h"
class OdDbDatabase;


#ifdef DWFIMPORT_EXPORTS
  #define DWFIMPORT_TOOLKIT         OD_TOOLKIT_EXPORT
  #define DWFIMPORT_TOOLKIT_STATIC  OD_STATIC_EXPORT
#else                               
  #define DWFIMPORT_TOOLKIT         OD_TOOLKIT_IMPORT
  #define DWFIMPORT_TOOLKIT_STATIC  OD_STATIC_IMPORT
#endif

//////////////////////////////////////////////////////////////////////////

//DD:EXPORT_ON


/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {

/** \details
  This class implements the DWF import.
*/
class OdDwfImport : public OdRxObject
{
public:
  // "encrypted_file" file means kzMIMEType_W3D_S stream
  // see here: http://dwf.blogs.com/beyond_the_paper/2006/08/how_to_detect_a.html

  enum ImportResult { success, fail, bad_password, bad_file, bad_database, encrypted_file };
  virtual ImportResult import() = 0;

  // Documented properties are:
  // "Database"    - OdDbDatabase object, where dwf is imported
  // "DwfPath"     - string, path to the imported dwf file
  // "Password"    - string, (default empty), password to dwf if required
  // "PaperWidth"  - double, (default 297),   paper width in millimeters
  // "PaperHeight" - double, (default 210),   paper height in millimeters
  // "Background"  - ODCOLORREF, (defaulf 0xffffff), (output parameter), paper color

  virtual OdRxDictionaryPtr properties() = 0;
};

typedef OdSmartPtr<OdDwfImport> OdDwfImportPtr;

/** \details
  This class implements the DWF import module.
*/
class OdDwfImportModule : public OdRxModule
{
public:
    virtual OdDwfImportPtr create () = 0;
    
};

typedef OdSmartPtr<OdDwfImportModule> OdDwfImportModulePtr;

inline OdDwfImportPtr createImporter()
{
     OdDwfImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ImportModuleName);
     if ( !pModule.isNull() ) return pModule->create ();
     return (OdDwfImport*)0;
}

//DD:EXPORT_OFF
}
#endif // _DWFIMPORT_INCLUDED_
