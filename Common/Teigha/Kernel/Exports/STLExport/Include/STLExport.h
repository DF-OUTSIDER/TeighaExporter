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
// STLExport.h - Open Design STL Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _STL_EXPORT_INCLUDED_
#define _STL_EXPORT_INCLUDED_

#include "STLExportDef.h"
#include "Gi/GiDrawable.h"
#include "OdStreamBuf.h"
#include "DbBaseDatabase.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_STL_EXPORT
{

  /** \details
     Exports an element to STL file
     
     Input : pEntity - element to export
             bTextMode - if true, export to ASCII STL format, else to binary STL format.
     Output: pOutStream - output stream (file stream, memory stream)
    
     Return : eOk is ok
              or OdResult error code
  */
  OdResult exportSTL(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, OdStreamBuf &pOutStream, bool bTextMode, double dDeviation);

  /** \details
     Exports an element to STL file with checking of solid topology
     
     Input : pEntity - element to export
             bTextMode - if true, export to ASCII STL format, else to binary STL format.
     Output: pOutStream - output stream (file stream, memory stream)
    
     Return : eOk is ok
              or OdResult error code
  */
  OdResult exportSTLEx(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, OdStreamBuf &pOutStream, bool bTextMode, double dDeviation);

};

#endif // _STL_EXPORT_INCLUDED_

