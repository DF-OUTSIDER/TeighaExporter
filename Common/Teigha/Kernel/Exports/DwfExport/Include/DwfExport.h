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


///////////////////////////////////////////////////////////////////////////////
//
// DwfExport.h - Open Design DWF Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DWF_EXPORT_INCLUDED_
#define _DWF_EXPORT_INCLUDED_


#include "RxModule.h"
#include "DwfExportDef.h"
#include "DwfExportParams.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DWF_EXPORT{

/** \details
  This class implements the DWF export.
*/
class OdDwfExport : public OdRxObject
{
public:

  /** \details
    Pure virtual method for DWF export implementation.
  
    \remarks
    Uses an instance of the DwExportParams class as a set of export parameters.
  
    \param pParams [in]  Parameters used by the DWF export procedure.
  */
  virtual void exportDwf(DwExportParams& pParams) = 0;
  
  /** \details
    Pure virtual method for 3D DWF export implementation.
  
    \remarks
    Uses an instance of Dwf3dExportParams class as a set of export parameters.
  
    \param pParams [in]  Parameters used by the 3D DWF export procedure.
  */
  virtual void export3dDwf(Dwf3dExportParams& pParams) = 0;
};

/** \details
  Implements a smart pointer to the DWF export.
*/
typedef OdSmartPtr<OdDwfExport> OdDwfExportPtr;

/** \details
  This class implements the DWF export module.
*/
class DwfExportModule : public OdRxModule
{
public:

/** \details
  Pure virtual method for implementing the creation of the OdDwfExport class instance. 
  
  \remarks
  Returns a smart pointer to the created module.
*/
  virtual OdDwfExportPtr create() = 0;
};

/** \details
 Implements a smart pointer to the DWF export module.
*/
typedef OdSmartPtr<DwfExportModule> OdDwfExportModulePtr;
}

#endif // _DWF_EXPORT_INCLUDED_

