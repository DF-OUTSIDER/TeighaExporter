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
#ifndef _COLLADA_VISUALSCENEIMPORTER_H_
#define _COLLADA_VISUALSCENEIMPORTER_H_
#include "COLLADAFWNode.h"
#include "COLLADAFWVisualScene.h"
#include "ColladaNodeImporter.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
    This class implements the visual scene importer.
  */
  class VisualSceneImporter : public NodeImporter
  {
  public:
    /** Constructor. 
    */
    VisualSceneImporter(DocumentImporter* documentImporter);
    
    /** Destructor. 
    */
    virtual ~VisualSceneImporter();

    /** Imports the scene. 
    */
    bool import(const COLLADAFW::VisualScene* visualScene);

  private:
    /** Disable default copy ctor. 
    */
    VisualSceneImporter( const VisualSceneImporter& pre );

    /** Disable default assignment operator. 
    */
    const VisualSceneImporter& operator= ( const VisualSceneImporter& pre );
  };
}

#endif // _COLLADA_VISUALSCENEIMPORTER_H_
