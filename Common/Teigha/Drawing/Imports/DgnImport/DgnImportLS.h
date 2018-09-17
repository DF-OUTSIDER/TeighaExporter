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

#ifndef _DGN_IMPORTLS_INCLUDED_
#define _DGN_IMPORTLS_INCLUDED_

#include <OdaCommon.h>
#include <DbLinetypeTable.h>
#include <DbLinetypeTableRecord.h>
#include <OdVector.h>

#define STL_USING_MAP
#include <OdaSTL.h>

#include <DgnLS/DbLSComponent.h>

class OdDgDatabase;
class OdDgLineStyleTableRecord;
class OdDgLineStyleDefTableRecord;
class OdDgLineStyleResource;
class OdDgLineCodeResource;
class OdDgLinePointResourceSymInfo;

class OdDbDatabase;
class OdDbBlockTableRecord;

/** \details
  <group OdDg_Classes>
  This class implements the import of DGN line style to .dwg database.  
  Corresponding C++ library: TG_DwgDb  
*/
class OdDgnImportLineStyle : public OdRxObject
{
public:
  virtual OdDbObjectId importLineStyle( OdDgDatabase* pDgnDb, OdDbDatabase* pDwgDb, const OdString& strLineStyleName ) = 0;
};

typedef OdSmartPtr<OdDgnImportLineStyle> OdDgnImportLineStylePtr;

// Dgn Line Styles importer

/** \details
  <group OdDg_Classes>
  This class implements the .dgn linestyle importer.
  Corresponding C++ library: TG_DwgDb
*/
class DgnLSImporter
{
  protected:
    // Globals (set once)
    OdUInt8 m_curUID[16];
    OdUInt32 m_compCounters[kLSInternalComponent];
    OdDgDatabase* m_pSrcDb;
    OdDbDatabase* m_pDstDb;
    OdDbLinetypeTablePtr m_pLTTable;
    typedef std::map<OdDgLineStyleResource *, OdDbObjectId> CompMap;
    CompMap m_componentsMap;
    // Elements (set for each)
    OdDgLineStyleTableRecord *m_pLS;
    double m_fUORPerMaster;
    double m_fUORPerStorage;
  public:
    DgnLSImporter(OdDgDatabase *pSrcDb, OdDbDatabase *pDstDb);

    OdDbLinetypeTableRecordPtr copyLinetype(OdDgLineStyleTableRecord *pLS);

    static void prepareLS(OdDgDatabase* pDb);
  protected:
    bool processLineStyle(OdDbLinetypeTableRecord *pRec, OdDgLineStyleDefTableRecord *pDef, bool bIsElement, bool bIsInternal);

    bool processPrimitiveRepresentation(OdDbLinetypeTableRecord *pRec, OdDgLineStyleDefTableRecord *pDef, bool bIsInternal);

    bool processPrimitive_Internal(OdDbLinetypeTableRecord *pRec);
    bool processPrimitive_Stroke(OdDbLinetypeTableRecord *pRec, OdDgLineCodeResource *pRes);

    bool processDgnLSRepresentation(OdDbLinetypeTableRecord *pRec, OdDgLineStyleDefTableRecord *pDef, bool bIsElement, bool bIsInternal);

    OdDbLSComponentPtr processDgnLS_Component(const OdDgDatabase* pDgnDb, OdDgLineStyleResource *pRes, OdUInt64 handleId, bool bFromRscFile);

    OdDbLSComponentPtr processDgnLS_Internal(OdUInt32 lineCode, bool bFromBase);
    //OdDbLSComponentPtr processDgnLS_Internal(OdDgLineStyleResource *pRes);
    OdDbLSComponentPtr processDgnLS_Stroke(OdDgLineStyleResource *pRes, OdUInt64 handleId);
    OdDbLSComponentPtr processDgnLS_Compound(const OdDgDatabase* pDgnDb, OdDgLineStyleResource *pRes, OdUInt64 handleId, bool bFromRscFile);
    OdDbLSComponentPtr processDgnLS_Point(const OdDgDatabase* pDgnDb, OdDgLineStyleResource *pRes, OdUInt64 handleId, bool bFromRscFile);
    OdDbLSComponentPtr processDgnLS_Symbol(OdDgLineStyleResource *pRes, OdUInt64 handleId, OdDgLinePointResourceSymInfo &symInfo,
                                           double scaleX = 1.0, double scaleY = 1.0,
                                           double offsetX = 0.0, double offsetY = 0.0);

    const OdUInt8 *getUIDIncrement();
    void transformBy(OdDbBlockTableRecord *pBTR, const OdGeMatrix3d &xfm) const;
    void clearColor(OdDbBlockTableRecord *pBTR) const;
    OdString generateDictKey(OdDgLineStyleResource *pRes, OdUInt64 handleId, OdDgLinePointResourceSymInfo *symInfo = NULL);

    static void prepareLS_Compound(OdDgDatabase* pDb, OdDgLineStyleResource *pRes, OdVector<OdDgLineStyleResource*> &pNesting, bool bStyleFromRscFile);
    static void prepareLS_Point(OdDgDatabase* pDb, OdDgLineStyleResource *pRes, bool bStyleFromRscFile );
    static void prepareLS_Symbol(OdDgDatabase* pDb, OdDgLineStyleResource *pRes, OdDgLinePointResourceSymInfo &symInfo);
};

#endif // _DGN_IMPORTLS_INCLUDED_
