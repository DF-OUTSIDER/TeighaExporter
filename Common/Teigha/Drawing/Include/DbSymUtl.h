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




#ifndef DBSYMUTL_DEFINED
#define DBSYMUTL_DEFINED

#include "TD_PackPush.h"

#include "OdString.h"
#include "DbObjectId.h"
#include "DbSymbolTable.h"
#include "DbSymbolTableRecord.h"
#include "DbTextStyleTableRecord.h"
#include "DbDictionary.h"
#include "DbDatabase.h"

// Predefined names
#define AC_STRING(name, val) \
extern TOOLKIT_EXPORT_GLOBAL const OdConstString name;
#include "StdStrings.h"
#undef AC_STRING

/** \details

    <group TD_Namespaces>
*/
namespace OdDbSymUtil
{
  // --- DbSymbolUtilities Services  ---

  TOOLKIT_EXPORT OdResult repairPreExtendedSymbolName(
    OdString& newName,
    const OdString& oldName,
    const OdDbDatabase* pDb,
    bool allowVerticalBar = false,
    const OdChar symSubst = L'_',
    bool insertPrefix = true);

  /** \details
    If the symbol name was repaired, then newName is a repaired symbol name. 
    newName is empty if oldName did not need any repairing.
    allowVerticalBar is a boolean to indicate if vertical bars are allowed in the symbol name 
    
    \returns
    eInvalidInput      if the specified name is empty
    eOk                in else case 
  */
  TOOLKIT_EXPORT OdResult repairSymbolName(
    OdString& newName,
    const OdString& oldName,
    const OdDbDatabase* pDb,
    bool allowVerticalBar = false,
    const OdChar symSubst = L'_',
    bool insertPrefix = true);
  
  TOOLKIT_EXPORT OdResult validatePreExtendedSymbolName(
    const OdString& name,
    const OdDbDatabase* pDb,
    bool allowVerticalBar = false);

  /** \details
    This method validates name according to the rules for extended (AutoCAD 2000 and later) symbol names.
    allowVerticalBar is a boolean to indicate if vertical bars are allowed in the symbol name 
    
    \returns
    eOk            Symbol name is valid 
    eInvalidInput  Symbol name contains invalid characters 
  */
  TOOLKIT_EXPORT OdResult validateSymbolName(
    const OdString& name,
    const OdDbDatabase* pDb,
    bool allowVerticalBar = false);

  TOOLKIT_EXPORT bool getMaxSymbolNameLength(
    OdUInt32& maxLength,
    OdUInt32& maxSize,
    bool isNewName,
    bool compatibilityMode); // true for extended names

  inline OdString getSymbolName(const OdDbObjectId& objId)
  {
    OdDbSymbolTableRecordPtr pRec
      = OdDbSymbolTableRecord::cast(objId.openObject(OdDb::kForRead, true).get());
    if (!pRec.isNull())
    {
      return pRec->getName();
    }
    return OdString::kEmpty;
  }

  #define DBSYMUTL_MAKE_GETSYMBOLID_FUNC(T_TABLE) \
  inline OdDbObjectId \
  get ## T_TABLE ## Id(const OdString& name, const OdDbDatabase *pDb) \
  { \
    if (!pDb) return OdDbObjectId::kNull; \
    OdDbSymbolTablePtr pTable = pDb->get ## T_TABLE ## TableId().safeOpenObject(); \
    ODA_ASSERT(!pTable->isOdDbObjectIdsInFlux()); \
    return pTable->getAt(name); \
  }
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(Viewport)
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(Block)
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(DimStyle)
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(Layer)
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(Linetype)
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(RegApp)
  //DBSYMUTL_MAKE_GETSYMBOLID_FUNC(TextStyle)

  inline OdDbObjectId getTextStyleId(const OdString& name, const OdDbDatabase *pDb)
  {
    if (!pDb) return OdDbObjectId::kNull;
    OdDbSymbolTablePtr pTable = pDb->getTextStyleTableId().safeOpenObject();
    ODA_ASSERT(!pTable->isOdDbObjectIdsInFlux());
    OdDbTextStyleTableRecordPtr pRec = pTable->getAt(name, OdDb::kForRead);

    if (pRec.get())
      if (!pRec->isShapeFile())
        return pRec->objectId();
    return OdDbObjectId::kNull;
  }

  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(UCS)
  DBSYMUTL_MAKE_GETSYMBOLID_FUNC(View)
  #undef DBSYMUTL_MAKE_GETSYMBOLID_FUNC

  inline OdDbObjectId getPlotstyleId(const OdString& name, const OdDbDatabase *pDb)
  {
    OdDbDictionaryPtr pDic = pDb->getPlotStyleNameDictionaryId(false).openObject();
    if (!pDb) return OdDbObjectId::kNull;
    if (!pDic.isNull())
    {
      ODA_ASSERT(!pDic->isOdDbObjectIdsInFlux());
      return pDic->getAt(name);
    }
    return OdDbObjectId::kNull;
  }

  inline OdDbObjectId getMLStyleId(const OdString& name, const OdDbDatabase *pDb)
  {
    if (!pDb) return OdDbObjectId::kNull;
    OdDbDictionaryPtr pDic = pDb->getMLStyleDictionaryId(false).openObject();
    if (!pDic.isNull())
    {
      ODA_ASSERT(!pDic->isOdDbObjectIdsInFlux());
      return pDic->getAt(name);
    }
    return OdDbObjectId::kNull;
  }

  inline OdDbObjectId getTableStyleId(const OdString& name, const OdDbDatabase *pDb)
  {
    if (!pDb) return OdDbObjectId::kNull;
    OdDbDictionaryPtr pDic = pDb->getTableStyleDictionaryId(false).openObject();
    if (!pDic.isNull())
    {
      ODA_ASSERT(!pDic->isOdDbObjectIdsInFlux());
      return pDic->getAt(name);
    }
    return OdDbObjectId::kNull;
  }

  inline OdDbObjectId getMLeaderStyleId(const OdString& name, const OdDbDatabase *pDb)
  {
    if (!pDb) return OdDbObjectId::kNull;
    OdDbDictionaryPtr pDic = pDb->getMLeaderStyleDictionaryId(false).openObject();
    if (!pDic.isNull())
    {
      ODA_ASSERT(!pDic->isOdDbObjectIdsInFlux());
      return pDic->getAt(name);
    }
    return OdDbObjectId::kNull;
  }

  inline OdDbObjectId getVisualStyleId(const OdString& name, const OdDbDatabase *pDb)
  {
    if (!pDb) return OdDbObjectId::kNull;
    OdDbDictionaryPtr pDic = pDb->getVisualStyleDictionaryId(false).openObject();
    if (!pDic.isNull())
    {
      ODA_ASSERT(!pDic->isOdDbObjectIdsInFlux());
      return pDic->getAt(name);
    }
    return OdDbObjectId::kNull;
  }

  inline const OdString& linetypeByLayerName()
  { return  byLayerNameStr; }

  inline bool isLinetypeByLayerName(const OdString& name)
  { return !odStrICmp(name, linetypeByLayerName()); }

  inline const OdString& linetypeByBlockName()
  { return  byBlockNameStr; }

  inline bool isLinetypeByBlockName(const OdString& name)
  { return !odStrICmp(name, linetypeByBlockName()); }

  inline const OdString& linetypeContinuousName()
  { return  linetypeContinuousNameStr; }

  inline bool isLinetypeContinuousName(const OdString& name)
  { return !odStrICmp(name, linetypeContinuousName()); }

  inline const OdString& layerZeroName()
  { return  layerZeroNameStr; }

  inline bool isLayerZeroName(const OdString& name)
  { return !odStrICmp(name, layerZeroName()); }

  inline const OdString& layerDefpointsName()
  { return  layerDefpointsNameStr; }

  inline bool isLayerDefpointsName(const OdString& name)
  { return !odStrICmp(name, layerDefpointsName()); }

  inline const OdString& textStyleStandardName()
  { return standardStr; }

  inline const OdString& MLineStyleStandardName()
  { return standardStr; }

  inline bool isMLineStandardName(const OdString& name)
  { return !odStrICmp(name, MLineStyleStandardName()); }

  inline const OdString& dimStyleStandardName(OdDb::MeasurementValue measurement)
  { return measurement == OdDb::kEnglish ? standardStr : metricDimStandardStr; }

  inline const OdString& viewportActiveName()
  { return viewportActiveNameStr; }

  inline bool isViewportActiveName(const OdString& name)
  { return !odStrICmp(name, viewportActiveName()); }

  inline bool isTextStyleStandardName(const OdString& name)
  { return !odStrICmp(name, textStyleStandardName()); }

  TOOLKIT_EXPORT const OdDbObjectId& textStyleStandardId(const OdDbDatabase* pDb);
	TOOLKIT_EXPORT const OdDbObjectId& dimStyleStandardId(const OdDbDatabase* pDb);
  TOOLKIT_EXPORT const OdDbObjectId  MLineStyleStandardId(const OdDbDatabase* pDb);
  // Returns layer Id for given name. Creates new layer if it's missing
  TOOLKIT_EXPORT const OdDbObjectId  getLayerId(OdDbDatabase* pDb, const OdString& strLayer);

  inline const OdString& blockModelSpaceName(OdDb::DwgVersion version = OdDb::kDHL_CURRENT)
  { return version <= OdDb::vAC12 ? modelSpaceStr_R12 : modelSpaceStr; }

  inline bool isBlockModelSpaceName(const OdString& pN, OdDb::DwgVersion version = OdDb::kDHL_CURRENT)
  { return odStrICmp(pN, blockModelSpaceName(version)) == 0; }

  inline const OdString& blockPaperSpaceName(OdDb::DwgVersion version = OdDb::kDHL_CURRENT)
  { return version <= OdDb::vAC12 ? paperSpaceStr_R12 : paperSpaceStr; }

  inline bool isBlockPaperSpaceName(const OdString& pN, OdDb::DwgVersion version = OdDb::kDHL_CURRENT)
  { return odStrICmp(pN, blockPaperSpaceName(version)) == 0; }

  inline const OdString& plotStyleNormalName()
  { return plotStyleNormalNameStr;}

  inline const OdString& TableStyleStandardName()
  { return standardStr; }

  inline bool isTableStandardName(const OdString& name)
  { return !odStrICmp(name, TableStyleStandardName()); }

  inline const OdString& MLeaderStyleStandardName()
  { return standardStr; }

  inline bool isMLeaderStandardName(const OdString& name)
  { return !odStrICmp(name, MLeaderStyleStandardName()); }

  inline const OdString& DetailViewStyleStandardName(OdDbDatabase* pDb)
  {
    ODA_ASSERT_ONCE(pDb);
    return (pDb && pDb->getMEASUREMENT() == OdDb::kMetric) ? standardMetricDictStr : standardImperialDictStr;
  }

  inline bool isDetailViewStyleStandardName(OdDbDatabase* pDb, const OdString& name)
  { 
    return !odStrICmp(name, DetailViewStyleStandardName(pDb)); 
  }

  inline const OdString& SectionViewStyleStandardName(OdDbDatabase* pDb)
  {
    return DetailViewStyleStandardName(pDb);
  }

  inline bool isSectionViewStyleStandardName(OdDbDatabase* pDb, const OdString& name)
  { 
    return !odStrICmp(name, SectionViewStyleStandardName(pDb)); 
  }
}
// namespace OdDbSymUtil

#include "TD_PackPop.h"

#endif // DBSYMUTL_DEFINED

