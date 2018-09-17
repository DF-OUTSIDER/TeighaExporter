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


#ifndef _ODTHREEJSJSONMODULEIMPL_INCLUDED_
#define _ODTHREEJSJSONMODULEIMPL_INCLUDED_

#include "ThreejsJSONExportDef.h"
#include "RxDynamicModule.h"
#include "DbBaseDatabase.h"

class OdGiDrawable;
class OdStreamBuf;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_THREEJSJSON_EXPORT
{
  /** \details
   This class implements the module for ThreejsJSON export.
  */
  class THREEJSJSONEXPORT_DLL ThreejsJSONModule : public OdRxModule
  {
  protected:
    /** \details
      Initializes the ThreejsJSON export module.
    */
    void initApp();

    /** \details
      Uninitializes the ThreejsJSON export module.
    */
    void uninitApp();
  public:
    /** \details
      Exports to the ThreejsJSON.
    */
    virtual OdResult exportThreejsJSON(OdDbBaseDatabase *pDb, OdStreamBuf *pOutStream, const ODCOLORREF &background, bool bFacesEnabled = false);
  };

  /** \details
    This template class is a specialization of the OdSmartPtr class for STLModule object pointers.
  */
  typedef OdSmartPtr<ThreejsJSONModule> ThreejsJSONModulePtr;
}
#endif //_ODTHREEJSJSONMODULEIMPL_INCLUDED_
