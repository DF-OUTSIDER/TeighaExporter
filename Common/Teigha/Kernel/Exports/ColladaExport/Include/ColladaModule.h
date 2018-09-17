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

#ifndef _ODCOLLADAMODULEIMPL_INCLUDED_
#define _ODCOLLADAMODULEIMPL_INCLUDED_
#pragma once
#include "OdaCommon.h"
#include "ColladaExportDef.h"
#include "RxDynamicModule.h"
#include "DbBaseDatabase.h"

class OdGiDrawable;
class OdStreamBuf;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_COLLADA_EXPORT
{
  /** \details
    This class implements the Collada export module.
  */
  class COLLADAEXPORT_DLL ColladaModule : public OdRxModule
  {
  protected:
    /** \details
      Initializes the Collada export module.
    */
    void initApp();

    /** \details
      Uninitializes the Collada export module.
    */
    void uninitApp();
  public:
    /** \details
      Exports to the Collada.
    */
    virtual OdResult exportCollada(OdDbBaseDatabase *pDb, const OdString& pFileName, const ODCOLORREF* pPallete, int numColors = 256);
    virtual OdResult exportCollada(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, const OdString& pFileName, const ODCOLORREF* pPallete, int numColors = 256);
  };

  /** \details
    This template class is a specialization of the OdSmartPtr class for STLModule object pointers.
  */
  typedef OdSmartPtr<ColladaModule> ColladaModulePtr;
}
#endif //_ODCOLLADAMODULEIMPL_INCLUDED_
