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
#ifndef _COLLADA_EXPORT_INCLUDED_
#define _COLLADA_EXPORT_INCLUDED_
#ifdef _MSC_VER
#pragma once
#endif
#include "OdaCommon.h"
#include "ColladaExportDef.h"
#include "Gi/GiDrawable.h"
#include "OdStreamBuf.h"
#include "DbBaseDatabase.h"
#include "ColladaExportView.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_COLLADA_EXPORT
{
  /** \details
    Collects the data for Collada.
  */
  OdResult CollectData(OdDbBaseDatabase *pDb, EntityDataArray* m_pEntityDataArr, ColladaMatrialData* pColladaMaterialData, LightExporter* pLightExp, const ODCOLORREF* pPallete, int numColors, const OdGiDrawable* pEntity);

  /** \details
    Exports the data for Collada.
  */
  OdResult exportCollada(OdDbBaseDatabase *pDb, const OdString& pFileName, const ODCOLORREF* pPallete, int numColors, const OdGiDrawable* pEntity = NULL);
};

#endif // _COLLADA_EXPORT_INCLUDED_


