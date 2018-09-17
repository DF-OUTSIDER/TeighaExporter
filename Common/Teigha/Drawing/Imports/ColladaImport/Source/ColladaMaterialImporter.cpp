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
#include "ColladaMaterialImporter.h"
#include "COLLADAFWMaterial.h"


namespace TD_COLLADA_IMPORT
{

	MaterialImporter::MaterialImporter( DocumentImporter* documentImporter )
		: ImporterBase(documentImporter)
	{
	}

	MaterialImporter::~MaterialImporter()
	{
	}

	bool MaterialImporter::import(const COLLADAFW::Material* pMaterial)
	{
		const COLLADAFW::UniqueId& materialUniqueId = pMaterial->getUniqueId();
		if ( materialUniqueId.isValid() )
			addUniqueIdFWMaterialPair(materialUniqueId, *pMaterial);
		return true;
	}
}
