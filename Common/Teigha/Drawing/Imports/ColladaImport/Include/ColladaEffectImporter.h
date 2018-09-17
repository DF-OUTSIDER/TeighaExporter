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
#ifndef _COLLADA_EFFECTIMPORTER_H_
#define _COLLADA_EFFECTIMPORTER_H_

#include "ColladaImporterBase.h"
#include "COLLADAFWEffect.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
    This class implements the import of one effect.
  */
  class EffectImporter : public ImporterBase 
  {
  public:
    /** Constructor. 
    */
    EffectImporter(DocumentImporter* documentImporter);
    /** Destructor. 
    */
    virtual ~EffectImporter();

    /** Performs the import of the effect.
        @return True on success, false otherwise.
    */
    bool import(const COLLADAFW::Effect* pEffect);
  private:
    /** Disable default copy ctor. 
    */
    EffectImporter( const EffectImporter& pre );
    /** Disable default assignment operator. 
    */
    const EffectImporter& operator= ( const EffectImporter& pre );
  };
}

#endif // _COLLADA_EFFECTIMPORTER_H_
