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

#ifndef _DGN_IMPORTCOMMON_INCLUDED_ 
#define _DGN_IMPORTCOMMON_INCLUDED_

#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgElementId.h>
#include <DgHostAppServices.h>
#include <DgModel.h>
#include <DgLevelMask.h>
#include <DgAttributeLinkage.h>
#include <DbPolyline.h>
#include "DgText.h"

class OdDgDatabase;
class OdDbDatabase;
class OdDgLevelTableRecord;
class OdDbLayerTableRecord;
class OdDgElement;
class OdDgLineStyleResource;
class OdDgModel;


/** \details
  <group OdImport_Classes> 
*/
namespace TD_DGN_IMPORT {

/*!DOM*/

void copyEntityProperties(OdDgGraphicsElement* src, OdDbEntity* dst);
OdDbObjectId getDwgMaterialByDgnMaterial( const OdDgElementId& idDgnMaterial, OdDbDatabase* pDwgDb );
void applyLevelSymbologyOverrides( const OdDgElementId& idLevel, OdUInt32& uColorIndex, OdUInt32& uLineWeight, 
                                   OdInt32& iLineStyleId );
OdUInt32 codePageToCharset( OdUInt32 uCodePage );
OdDgElementId odDgnImportGetMaterialId(OdDgGraphicsElement* src, OdRxObjectPtrArray& pLinkages);
bool odDgnImportLineStyleModifiers(const OdDgLineStyleModificationLinkage *pLStyleModLinkage, OdDgElementId ownerId,
                                   OdGiDgLinetypeModifiers &lineTypeModifiers, double &modScale);
void odDgnImportSetLSModXData(OdDbEntity *pEntity, const OdGiDgLinetypeModifiers &lsMod, double lsScale);
OdDb::LineWeight odDgnImportLineweightMapping(OdUInt32 mappingVal);
static double globalLinetypeScale(OdDgGraphicsElement* src);
void setDwgColorByDgnIndex( OdDgDatabase* pDb, OdDbEntity* dst, OdUInt32 uColorIndex );
void setDwgColorByDgnIndex( OdDgDatabase* pDb, OdCmColor& dstClr, OdUInt32 uColorIndex );
void setDwgLineTypeIdByDgnLineTypeIndex(OdDbDatabase* pDb, OdDgDatabase* pDgnDb, 
                                        OdUInt32 uDgnLineStyle, OdDbObjectId& idDwgLineType );
void getInternalGraphicsElement( OdDgElementIteratorPtr& pIter, OdDgGraphicsElementPtr& pInternalGraphics );

bool createVertexPolyline( OdGePoint3dArray arrVertices, OdDbPolylinePtr& pPolyline, double dThickness );
OdUInt32 getLineStyleEntryId( const OdDgGraphicsElementPtr& pElm );

bool createPolylineFromComplexElement( OdDgGraphicsElement* e, OdDgElementIteratorPtr& pIter, 
                                       OdDbBlockTableRecord* owner, OdDbPolylinePtr& pPolyline, 
                                       bool bForceCloseElement = false );
bool prepareMatrixToBlockTransform( OdGeMatrix3d& matTransform, bool bUniScaleTransform = false );
OdString convertEmptyDataFieldsTo_( const OdString& strTextData, const OdDgTextEditField& textField );

}
#endif // _DGN_IMPORTCOMMON_INCLUDED_
