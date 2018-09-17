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
#ifndef _COLLADA_MATERIALCREATOR_H_
#define _COLLADA_MATERIALCREATOR_H_

#include "ColladaImporterBase.h"
#include "Gi/GiMaterial.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** This class creates and assigns all material previously been stored by 
    the material and affect importers.
  */
  class MaterialCreator : public ImporterBase 
  {
    enum createMaterialResults{ errUnknown = -1, errOk, errSetMatState, errNoTexture };
  public:
    /** Constructor. 
    */
    MaterialCreator( DocumentImporter* documentImporter );
    /** Destructor. 
    */
    virtual ~MaterialCreator();

    bool create();

    bool createAndAssingMaterial( const DocumentImporter::EntityMaterialBindingsPair& materialBindingPair );

    createMaterialResults createAndAssingMaterial(OdDbObjectId objId, const COLLADAFW::MaterialBinding& materialBinding);

    const COLLADAFW::Effect* getEffect( const COLLADAFW::Material* pMaterial );

    bool createTeighaMaterial( const COLLADAFW::Effect& effect, const COLLADAFW::String& name, OdDbDictionaryPtr pMatDic );
    createMaterialResults createTeighaMaterial(const COLLADAFW::Effect& effect, const COLLADAFW::String& name, OdDbDictionaryPtr pMatDic, OdDbObjectId& objectId);

    void setTexture( const COLLADAFW::EffectCommon& effectCommon, const COLLADAFW::Texture& texture, OdGiMaterialMap& materialMap );

  private:
    /** Disable default copy ctor. 
    */
    MaterialCreator( const MaterialCreator& pre );
    /** Disable default assignment operator. 
    */
    const MaterialCreator& operator= ( const MaterialCreator& pre );
  };
}

#endif // _COLLADA_MATERIALCREATOR_H_
