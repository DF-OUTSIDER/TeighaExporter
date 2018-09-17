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
#ifndef _COLLADA_NODEIMPORTER_H_
#define _COLLADA_NODEIMPORTER_H_

#include "ColladaImporterBase.h"
#include "COLLADAFWNode.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
    This class implements the node importer.
  */
  class NodeImporter : public ImporterBase
  {
  public:
    /** Constructor. 
    */
    NodeImporter(DocumentImporter* documentImporter);
    /** Destructor. 
    */
    virtual ~NodeImporter();

    /** Importes the nodes. 
    */
    bool importNodes( const COLLADAFW::NodePointerArray& nodes );

    /** Importes an one node. 
    */
    bool importNode( const COLLADAFW::Node* node );

  private:
    /** Disable default copy ctor. 
    */
    NodeImporter( const NodeImporter& pre );
    /** Disable default assignment operator. 
    */
    const NodeImporter& operator= ( const NodeImporter& pre );
  };
}

#endif // _COLLADA_NODEIMPORTER_H_
