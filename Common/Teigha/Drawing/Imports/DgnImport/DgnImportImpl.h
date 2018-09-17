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

#ifndef _DGN_IMPORTIMPL_INCLUDED_ 
#define _DGN_IMPORTIMPL_INCLUDED_

#include "DgnImport.h"
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include <DbHostAppServices.h>
#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgElementId.h>
#include <DgHostAppServices.h>
#include <DgModel.h>
#include "DbMText.h"
#include "DgTextExtendedProperties.h"
#include "DgView.h"
#include "DgnImportLS.h"
#include <DbMaterial.h>
#include "DgMaterialTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include <DbDimStyleTableRecord.h>

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

extern OdDgnImportLineStylePtr g_pDgnImportLS;

//-------------------------------------------------------------------------------------------------------------

class OdDgnImportLineWeightsMapImpl : public OdDgnImportLineWeightsMap
{
public:
  OdDgnImportLineWeightsMapImpl();

  virtual OdDb::LineWeight getLineWeightByDgnIndex( OdUInt32 uIndex ) const;
  virtual OdResult         setLineWeightForDgnIndex( OdUInt32 uIndex, OdDb::LineWeight uLineWeigth );
  virtual void             copyLineWeightsMap( const OdDgnImportLineWeightsMapPtr& pMap );
  virtual void             resetToDefaults();

  static OdDb::LineWeight getDefaultLineWeightByDgnIndex( OdUInt32 uIndex );

private:
  OdDb::LineWeight        m_arrLineWeights[34];
  static OdDb::LineWeight m_arrDefaultLineWeights[34];
};

//-------------------------------------------------------------------------------------------------------------

/** \details
  This class implements the properties for DGN importer.
*/
class DgnProperties : public OdRxDispatchImpl<>
{
  OdString _path;
  OdDbDatabasePtr _db;
  OdSmartPtr<OdDbHostAppServices> _dbSvc;
  OdSmartPtr<OdDgHostAppServices> _dgSvc;
  OdStringArray _MS_SYMBRSRC;
  OdString      _str_ShxFonts_Path;
  OdUInt8 _XRefImportMode; // 0 - Omit, 1 - Retain, 2 - MergeToCell, 3 - Create Dgn Underlay
  bool    _bExplodeTextNodes;
  bool    _bRecomputeDimensionsAfterImport;
  bool    _bImportPaperSpaceModels;
  bool    _bImportActiveModelToModelSpace;
  bool    _bBreakDimensionAssociation;
  bool    _bConvertDgnColorIndicesToTrueColors;
  bool    _bConvertEmptyDataFieldsToSpaces;
  bool    _bEraseUnusedResources;
  OdInt8  _iImportView;
  bool    _bDontImportInvisibleElements;
  OdUInt8 _3dObjectImportMode;  // 0 - OdDgPolyfaceMesh, 1 - OdDb3dSolid / OdDbBody
  OdUInt8 _3dEllipseImportMode; // 0 - OdDbEllipse, 1 - OdDbRegion
  OdUInt8 _2dEllipseImportMode; // 0 - OdDbEllipse, 1 - OdDbRegion
  OdUInt8 _3dShapeImportMode;   // 0 - OdDbPolyline, 1 - OdDbRegion, 2 - OdDbPolyfaceMesh
  OdUInt8 _2dShapeImportMode;   // 0 - OdDbPolyline, 1 - OdDbRegion, 2 - OdDbPolyfaceMesh
  OdUInt8 _3dClosedBSplineCurveImportMode;   // 0 - OdDbSplinePtr, 1 - OdDbRegion
  OdUInt8 _2dClosedBSplineCurveImportMode;   // 0 - OdDbSplinePtr, 1 - OdDbRegion
  OdDgnImportLineWeightsMapPtr _pLineWeightMap;
public:
  DgnProperties()
  {
    _bConvertDgnColorIndicesToTrueColors = false;
    _bExplodeTextNodes = false;
    _bRecomputeDimensionsAfterImport = false;
    _XRefImportMode    = 2;
    _bImportPaperSpaceModels = true;
    _bImportActiveModelToModelSpace = true;
    _bDontImportInvisibleElements = false;
    _bBreakDimensionAssociation = false;
    _bConvertEmptyDataFieldsToSpaces = true;
    _bEraseUnusedResources = false;
    _iImportView = -1;
    _3dEllipseImportMode = 0;
    _2dEllipseImportMode = 0;
    _3dShapeImportMode = 1;
    _2dShapeImportMode = 0;
    _3dObjectImportMode = 0;
    _3dClosedBSplineCurveImportMode = 1;
    _2dClosedBSplineCurveImportMode = 0;
    _pLineWeightMap = OdRxObjectImpl<OdDgnImportLineWeightsMapImpl>::createObject();
  }

  ~DgnProperties()
  {
    _pLineWeightMap.release();
  }

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP( DgnProperties );
  static OdRxDictionaryPtr createObject();

  OdRxObjectPtr get_Database() const { return _db.get(); }
  void put_Database( OdRxObject* obj ){ _db = obj; }
  OdRxObjectPtr get_Services() const { return _dbSvc.get(); }
  void put_Services( OdRxObject* obj ){ _dbSvc = obj; }
  OdString get_DgnPath() const { return _path; }
  void put_DgnPath( OdString path ){ _path = path; }
  OdRxObjectPtr get_DgnServices() const { return _dgSvc.get(); }
  void put_DgnServices( OdRxObject* obj ){ _dgSvc = obj; }
  const OdStringArray &get_MS_SYMBRSRC() const { return _MS_SYMBRSRC; }
  void put_MS_SYMBRSRC( const OdStringArray& array ){ _MS_SYMBRSRC = array; }
  const OdUInt8 &get_XRefImportMode() const { return _XRefImportMode; }
  void put_XRefImportMode( const OdUInt8& uMode ){ _XRefImportMode = uMode; }
  const bool &get_ExplodeTextNodes() const { return _bExplodeTextNodes; }
  void put_ExplodeTextNodes( const bool& bSet ){ _bExplodeTextNodes = bSet; } 
  const bool &get_RecomputeDimensionsAfterImport() const { return _bRecomputeDimensionsAfterImport; }
  void put_RecomputeDimensionsAfterImport( const bool& bSet ){ _bRecomputeDimensionsAfterImport = bSet; } 
  const bool &get_ImportPaperSpaceModels() const { return _bImportPaperSpaceModels; }
  void put_ImportPaperSpaceModels( const bool& bSet ){ _bImportPaperSpaceModels = bSet; }
  const bool &get_ImportActiveModelToModelSpace() const { return _bImportActiveModelToModelSpace; }
  void put_ConvertDgnColorIndicesToTrueColors( const bool& bSet ){ _bConvertDgnColorIndicesToTrueColors = bSet; }
  const bool &get_ConvertDgnColorIndicesToTrueColors() const { return _bConvertDgnColorIndicesToTrueColors; }
  void put_ImportActiveModelToModelSpace( const bool& bSet ){ _bImportActiveModelToModelSpace = bSet; }
  const bool &get_DontImportInvisibleElements() const { return _bDontImportInvisibleElements; }
  void put_DontImportInvisibleElements( const bool& bSet ){ _bDontImportInvisibleElements = bSet; }
  const bool &get_ConvertEmptyDataFieldsToSpaces() const { return _bConvertEmptyDataFieldsToSpaces; }
  void put_ConvertEmptyDataFieldsToSpaces( const bool& bSet ){ _bConvertEmptyDataFieldsToSpaces = bSet; }
  OdUInt8 get_ImportViewIndex() const { return _iImportView; }
  void put_ImportViewIndex( const OdUInt8& iImportView ){ _iImportView = iImportView; }
  const OdUInt8 &get_3dEllipseImportMode() const { return _3dEllipseImportMode; }
  void put_3dEllipseImportMode( const OdUInt8& uMode ){ _3dEllipseImportMode = uMode; }
  const OdUInt8 &get_2dEllipseImportMode() const { return _2dEllipseImportMode; }
  void put_2dEllipseImportMode( const OdUInt8& uMode ){ _2dEllipseImportMode = uMode; }
  const OdUInt8 &get_3dShapeImportMode() const { return _3dShapeImportMode; }
  void put_3dShapeImportMode( const OdUInt8& uMode ){ _3dShapeImportMode = uMode; }
  const OdUInt8 &get_2dShapeImportMode() const { return _2dShapeImportMode; }
  void put_2dShapeImportMode( const OdUInt8& uMode ){ _2dShapeImportMode = uMode; }
  const OdUInt8 &get_3dObjectImportMode() const { return _3dObjectImportMode; }
  void put_3dObjectImportMode( const OdUInt8& uMode ){ _3dObjectImportMode = uMode; }
  const OdUInt8 &get_3dClosedBSplineCurveImportMode() const { return _3dClosedBSplineCurveImportMode; }
  void put_3dClosedBSplineCurveImportMode( const OdUInt8& uMode ){ _3dClosedBSplineCurveImportMode = uMode; }
  const OdUInt8 &get_2dClosedBSplineCurveImportMode() const { return _2dClosedBSplineCurveImportMode; }
  void put_2dClosedBSplineCurveImportMode( const OdUInt8& uMode ){ _2dClosedBSplineCurveImportMode = uMode; }
  const OdString &get_shxFontsPath() const { return _str_ShxFonts_Path; }
  void put_shxFontsPath( const OdString& strPath ){ _str_ShxFonts_Path = strPath; }
  const bool &get_BreakDimensionAssociation() const { return _bBreakDimensionAssociation; }
  void put_BreakDimensionAssociation( const bool& bSet ){ _bBreakDimensionAssociation = bSet; }
  const bool &get_EraseUnusedResources() const { return _bEraseUnusedResources; }
  void put_EraseUnusedResources( const bool& bSet ){ _bEraseUnusedResources = bSet; }
  OdRxObjectPtr get_LineStyleImporter() const { return g_pDgnImportLS; }
  void put_LineStyleImporter( OdRxObject* obj ){ throw( OdError(eNotImplemented)); }
  OdRxObjectPtr get_LineWeightsMap() const { return _pLineWeightMap; }
  void put_LineWeightsMap( OdRxObject* obj ){ throw( OdError(eNotImplemented)); }
};

typedef OdSmartPtr<DgnProperties> DgnPropertiesPtr;

//-------------------------------------------------------------------------------------------------------------

struct OdDgnImportPathToDwgObject
{
  OdDbFullSubentPath m_idPath;
  bool               m_bExists;
  OdUInt32           m_uStartPt;
  OdUInt32           m_uEndPt;
};

//-------------------------------------------------------------------------------------------------------------

/** \details
  This class implements the DGN OdDgnImportLineStyle.
*/

class OdDgnImportLineStyleImpl : public OdDgnImportLineStyle
{
public:
  OdDgnImportLineStyleImpl(){};
  virtual OdDbObjectId importLineStyle( OdDgDatabase* pDgnDb, OdDbDatabase* pDwgDb, const OdString& strLineStyleName );
};

//-------------------------------------------------------------------------------------------------------------

/** \details
  This class implements the DGN importer.
*/
class DgnImporter : public OdDgnImport
{
  typedef std::map<OdDgElementId, OdDgnImportPathToDwgObject> IdMap;
  typedef std::map<OdDbObjectId, bool> IdResourceUsage;
  IdMap _idMap;
  OdArray<IdMap> m_arrIdMapStack;
  IdResourceUsage _dwgResourceUsage;
  bool bReloadRecomputeDim;
  OdDgDatabase* m_pDgnDb;
  OdDbHostAppServices* m_pHostAppServices;

  DgnPropertiesPtr _properties;
  void copyLayerProperties(OdDgLevelTableRecord* l, OdDbLayerTableRecord* l_d);
  void copyDimStyleProperties( const OdDgDimStyleTableRecordPtr& pDgDimStyle, 
    OdDbDimStyleTableRecordPtr& pDbDimStyle, OdDbDatabase* pDb );
  void copyModelSpace(OdDgModel* m, OdDbDatabase* pDb);
  void copyLayout(OdDgModel* m, OdDbDatabase* pDb);
  void copyRegApps(OdDgDatabase* pDg, OdDbDatabase* pDb);
  void copyViewports(OdDgDatabase* pDg, OdDbDatabase* pDb, const OdDgModelPtr& pBaseModel);
  void copyDimStyles(OdDgDatabase* pDg, OdDbDatabase* pDb);
  void copyMLineStyles(OdDgDatabase* pDg, OdDbDatabase* pDb, const OdDgModelPtr& pBaseModel );
  void copyNamedViews(OdDgDatabase* pDg, OdDbDatabase* pDb);
  void copyDisplayStyles(OdDgDatabase* pDg, OdDbDatabase* pDb);
  void registerElementLoaders( OdDbHostAppServices* pHostAppServices );
  void unregisterElementLoaders( OdDbHostAppServices* pHostAppServices );
  static void setModelLevelMask( OdDgModel* m, OdInt8 iViewIndex = -1 );
  static void removeModelLevelMask();
  static bool setActiveModelView( OdDgModel* m, OdInt8 iViewIndex );
public:
  void copyLayers(OdDgDatabase* src, OdDbDatabase* dst, bool bRenameLayers = false );
  void copyTextStyles(OdDgDatabase* pDg, OdDbDatabase* pDb );
  void copyLineStyles(OdDgDatabase* src, OdDbDatabase* dst );
  void copyMaterials(OdDgDatabase* pDg, OdDbDatabase* pDb );
  void copyBlocks(OdDgDatabase* pDg, OdDbDatabase* pDb );
  void setDimensionAssociation( OdDgDatabase* pDg, OdDbDatabase* pDb );
  void createShxFontFromRscFont( const OdString& strFontName );

  bool setTextNodeToMTextImportPE();
  void setTextNodeToEntitySetImportPE();

  static void setImportPEToElements( bool bExplodeTextNode );
  static void removeImportPEFromElements();
  static bool copyMaterialProperties( const OdDgMaterialTableRecordPtr& pDgMaterial, 
    OdDbMaterialPtr& pDbMaterial, const OdDbDictionaryPtr& pMatDictionary );

  OdDb::LineWeight getLineWeightByDgnIndex( OdUInt32 uIndex );

  OdDbObjectId getObjectId( const OdDgElementId& idDgnElement ) const;
  bool getObjectPath( const OdDgElementId& idDgnElement, OdDgnImportPathToDwgObject& dwgPath );
  void addObjectPath( const OdDgElementId& idDgnElement, const OdDgnImportPathToDwgObject& dwgPath );
  void pushIdMap();
  void popIdMap();
  void addResourceId( const OdDbObjectId& idResource );
  void setResourceUsage( const OdDbObjectId& idResource );
  void removeUnusedResources();
public:
  static void copySpaceBlock(OdDgModel* m, OdDbBlockTableRecord* ms, bool bIgnoreTransformToOffset = false, 
                             OdInt8 iViewIndex = -1, bool bModelSpace = false );
  static void copyNamedGroup(OdDgModel* m, const OdString& strGroupName, OdDbBlockTableRecord* ms, OdInt8 iViewIndex = -1 );
public:
  DgnImporter();
  virtual OdRxDictionaryPtr properties() ODRX_OVERRIDE;
  virtual ImportResult import() ODRX_OVERRIDE;
};

}
#endif // _DGN_IMPORTIMPL_INCLUDED_
