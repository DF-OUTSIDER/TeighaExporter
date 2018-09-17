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
#ifndef _COLLADA_CONTROLLERIMPORTER_H_
#define _COLLADA_CONTROLLERIMPORTER_H_

#include "ColladaImporterBase.h"
#include "COLLADAFWController.h"


/** \details
<group OdImport_Classes>
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
  This class implements the controller importer.
  */
  class ControllerImporter : public ImporterBase
  {
  public:
    /** Constructor.
    */
    ControllerImporter(DocumentImporter* documentImporter);
    /** Destructor.
    */
    virtual ~ControllerImporter();

    /** Performs the import of the controller.
    @return True on success, false otherwise.
    */
    bool import(const COLLADAFW::Controller & pController);

  private:
    /** Disable default copy ctor.
    */
    ControllerImporter(const ControllerImporter& pre);
    /** Disable default assignment operator.
    */
    const ControllerImporter& operator= (const ControllerImporter& pre);
  };
}
#endif // _COLLADA_CONTROLLERIMPORTER_H_
