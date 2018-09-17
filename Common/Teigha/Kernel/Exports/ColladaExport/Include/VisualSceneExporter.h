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
#ifndef __VISUALSCENEEXPORTER_H__
#define __VISUALSCENEEXPORTER_H__

#include "OdaCommon.h"
#include "ColladaExportDef.h"
#include "COLLADASWStreamWriter.h"
#include "COLLADASWLibraryVisualScenes.h"
#include "COLLADASWNode.h"
#include "COLLADASWInstanceGeometry.h"
#include "DbBaseDatabase.h"
#include "ColladaExportView.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_COLLADA_EXPORT
{
  /** \details
    This class implements the visual scene exporter for Collada.
  */
  class VisualSceneExporter : public COLLADASW::LibraryVisualScenes
  {
  public:
    /** \details
      Constructor for the visual scene exporter.
    */
    VisualSceneExporter(COLLADASW::StreamWriter * streamWriter);

    /** \details
      Destructor for the visual scene exporter.
    */
    ~VisualSceneExporter();

    /** \details
      Adds the visual scene to be export.
    */
    void addVisualScene(OdDbBaseDatabase *pDb, ColladaEntData* pColladaData);

    /** \details
      Adds the array of lights to be export.
    */
    void addLights(OdStringArray& pLightArr);
  };
}

#endif //__VISUALSCENEEXPORTER_H__
