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

#ifndef _EX_DGNELEMENTDUMPERPE_H_
#define _EX_DGNELEMENTDUMPERPE_H_

#include "OdaCommon.h"
#include "Dg3DObject.h"
#include "DgModel.h"
#include "DgBSplineCurve.h"
#include "DgBSplineSurface.h"
#include "DgCellHeader.h"
#include "DgComplexShape.h"
#include "DgComplexString.h"
#include "DgTextNode.h"
#include "DgSharedCellDefinition.h"
#include "RxObjectImpl.h"
#include "DgDatabase.h"
#include "DgTableRecord.h"
#include "ExDgnDumper.h"
#include "DgNonModelElementCollection.h"
#include "DgLevelFilterTableRecord.h"
#include "DgElementTemplate.h"
#include "ExDgnDumperModule.h"
#include "DgDetailingSymbol.h"
#include "DgEnvironmentMap.h"
#include "DgDisplayStyle.h"
#include "DgLevelMask.h"
/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRxObjectDumperPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS( OdDgRxObjectDumperPE );

  OdDgRxObjectDumperPE();
  virtual ~OdDgRxObjectDumperPE();
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;

  void addRef(){ ++m_references;}
  void release(){ --m_references; }
  long numRefs() const{ return m_references; }
private:
  long m_references;
};

typedef OdSmartPtr< OdDgRxObjectDumperPE > OdDgRxObjectDumperPEPtr;

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgElementDumperPE : public OdDgRxObjectDumperPE
{
public:
  ODRX_DECLARE_MEMBERS( OdDgElementDumperPE );

  OdDgElementDumperPE();
  virtual ~OdDgElementDumperPE();

  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;

  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const;

  virtual OdString getName( OdRxObjectPtr pObj ) const;

  virtual bool hasSubElements( OdDgElementPtr pElm ) const;

protected:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual void dumpLinkages( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual void dumpXAttributes( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual void dumpReactors( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual void dumpBinaryData( const OdBinaryData& binData, OdExDgnDumper* pDumper ) const;
  virtual void dumpBinaryDataBytes( const OdBinaryData& binData, OdExDgnDumper* pDumper ) const;
};

typedef OdSmartPtr< OdDgElementDumperPE > OdDgElementDumperPEPtr;

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgGraphicsElementDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDatabaseDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual void dumpLinkages( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual void dumpXAttributes( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgModelTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgModelTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgModelDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgModelPtr pComplElm = pElm;
    return pComplElm->createGraphicsElementsIterator( atBeginning, skipDeleted );
    //TODO: add support of createControlElementsIterator()
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgModelPtr pModel = pObj;
    OdString s;
    s.format(L"%ls", pModel->getName().c_str());
    return s;
  }

  OdDgElementIteratorPtr createControlElementsIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgModelPtr pComplElm = pElm;
    return pComplElm->createControlElementsIterator( atBeginning, skipDeleted );
  }

  bool hasSubElements( OdDgElementPtr pElm ) const
  {
    OdDgElementIteratorPtr pIt = createIterator( pElm );

    bool bRet = false;

    if( pIt.isNull() || pIt->done() )
    {
      OdDgModelPtr pComplElm = pElm;
      pIt = pComplElm->createControlElementsIterator();

      bRet = (!pIt.isNull() && !pIt->done());
    }
    else
    {
      bRet = true;
    }

    return bRet;
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLevelFilterTableDumperPE : public OdDgTableDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgFontTableDumperPE : public OdDgTableDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLineStyleTableDumperPE : public OdDgTableDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMaterialTableDumperPE : public OdDgTableDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};


/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgEnvironmentMapTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  void dumpEnvironmentLayer( OdExDgnDumper* pDumper, OdDgEnvMapLayer layer, bool bMainLayer ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDisplayStyleTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgDisplayStyleTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDisplayStyleTableRecordDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgDisplayStyleTableRecordPtr pRec = pObj;
    OdString s;
    s.format(L"%ls", pRec->getName().c_str());
    return s;
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgPrintStyleTableRecordDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgPrintStyleTableRecordPtr pRec = pObj;
    OdString s;
    s.format(L"%ls", pRec->getName().c_str());
    return s;
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgViewGroupTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgViewGroupTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgNamedViewTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgNamedViewTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSharedCellDefinitionTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgSharedCellDefinitionTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTagDefinitionSetTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgTagDefinitionSetTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgNonModelElementCollectionPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgNonModelElementCollectionPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgPrototypeElementTablePE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgPrototypeElementTablePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTablePrototypeElementPE : public OdDgElementDumperPE
{
public:
  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgTablePrototypeElementPtr pRec = pObj;
    OdString s;
    s.format(L"%ls (%d)", pRec->getPrototypeName().c_str(), pRec->getPrototypeIndex() );
    return s;
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgVariablesListPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgVariablesListPtr pListElm = pElm;
    return pListElm->createVariableValuesSetIterator( atBeginning, skipDeleted );
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgVariableValuesSetPE : public OdDgElementDumperPE
{
public:

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgVariableValuesSetPtr pRec = pObj;
    OdString strName = pRec->getName();

    if( !pRec->getDescription().isEmpty() )
      strName += L" (" + pRec->getDescription() + L")";

    return strName;
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgElementTemplateTablePE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgElementTemplateTablePtr pElmTempTable = pElm;
    return pElmTempTable->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLinkSetTablePE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgLinkSetTablePtr pLinkSetTable = pElm;
    return pLinkSetTable->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgPrintStyleTablePE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgPrintStyleTablePtr pTable = pElm;
    return pTable->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDictionaryTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgDictionaryTablePtr pTable = pElm;
    return pTable->createIterator( atBeginning, skipDeleted );
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgColorBookTablePE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgColorBookTablePtr pColorBookTable = pElm;
    return pColorBookTable->createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTableRecordDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgTableRecordPtr pRec = pObj;
    OdString s;
    s.format(L"%ls", pRec->getName().c_str());
    return s;
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgViewGroupDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgViewGroupPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSharedCellDefinitionDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgSharedCellDefinitionPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgSharedCellDefinitionPtr pSharedCell = pObj;

    OdString s;
    s.format(L"%ls", pSharedCell->getName().c_str());
    return s;
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRasterAttachmentHeaderDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgReferenceAttachmentHeaderDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgReferenceAttachmentHeaderPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgReferenceOverrideDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgReferenceOverridePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgReferenceOrderDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLevelTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLevelFilterTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgFontTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTextStyleTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDimStyleTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMaterialTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMultilineStyleTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLineStyleTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLinearAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgIntersectAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgArcAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMultilineAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgBSplineCurveAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgProjectionAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgOriginAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgIntersect2AssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMeshVertexAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMeshEdgeAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgBSplineSurfaceAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgUnknownAssociationDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLineStyleDefTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgTableRecordPtr pRec = pObj;
    OdString s;
    s.format(L"%ls", pRec->getName().c_str());

    if( s.isEmpty() )
    {
      s.format(L"<Empty>");
    }

    return s;
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDictionaryTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDictionaryDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgDictionaryPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgDictionaryPtr pElm = pObj;

    if( pElm->isRootDictionary() )
      return OD_T("Root Dictionary");

    return pElm->getName();
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRegAppTableRecordDumperPE : public OdDgTableRecordDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgViewDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgViewPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgReferenceAttachmentLevelMaskGroupPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgReferenceAttachmentLevelMaskGroupPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLevelMaskDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSharedCellReferenceDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgACSDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTagDefinitionSetDumperPE : public OdDgElementDumperPE
{
public:

  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgTagDefinitionSetPtr pTagSetElm = pElm;

    return pTagSetElm->createIterator( atBeginning, skipDeleted );
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgXMLCustomSchemaContainerDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgItemTypeLibraryDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTagDefinitionDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgColorTableDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgApplicationDataDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

//!DOM graphics elements

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTerrainGraphicsDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLine2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLine3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLineString2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLineString3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRasterFrameDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgText2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgText3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgShape2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgShape3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgCurve2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgCurve3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgEllipse2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgEllipse3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgArc2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgArc3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgConeDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgPointString2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgPointString3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDimensionDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgNamedGroupDumperPE : public OdDgElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMultilineDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRasterComponentDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTagElementDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

//!DOM complex graphics elements

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgBSplineCurve2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgBSplineCurve3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgBSplineSurfaceDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgCellHeader2dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgCellHeader2dPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgCellHeader3dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgCellHeader3dPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgOle2Frame2dDumperPE : public OdDgCellHeader2dDumperPE
{
public:

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgOle2Frame3dDumperPE : public OdDgCellHeader3dDumperPE
{
public:

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDistantLightDumperPE : public OdDgCellHeader3dDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgPointLightDumperPE : public OdDgCellHeader3dDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSpotLightDumperPE : public OdDgCellHeader3dDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgAreaLightDumperPE : public OdDgCellHeader3dDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSkyOpeningLightDumperPE : public OdDgCellHeader3dDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgComplexShapeDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgComplexShapePtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgComplexStringDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgComplexStringPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRasterHeader2dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgRasterHeader2dPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgRasterHeader3dDumperPE : public OdDgGraphicsElementDumperPE 
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgRasterHeader3dPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTextNode2dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgTextNode2dPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTextNode3dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgTextNode3dPtr pComplElm = pElm;
    return pComplElm->createIterator( atBeginning, skipDeleted );
  }
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTitleText2dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgTitleText3dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSectionMarker2dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSectionMarker3dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgArrowMarker2dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgArrowMarker3dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDetailMarker2dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgDetailMarker3dDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSectionClipDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgElementTemplateDumperPE : public OdDgElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDgElementTemplatePtr pTemplElm = pElm;

    if( pTemplElm->isTemplateGroup() )
    {
      return pTemplElm->createIterator( atBeginning, skipDeleted );
    }
    else
    {
      return OdDgElementTemplatePtr();
    }
  }

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgElementTemplatePtr pTemplElm = pObj;

    return pTemplElm->getTemplateName();
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLinkSetDumperPE : public OdDgElementDumperPE
{
public:

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgLinkSetPtr pLinkSetElm = pObj;

    return pLinkSetElm->getName();
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgColorBookDumperPE : public OdDgElementDumperPE
{
public:

  virtual OdString getName( OdRxObjectPtr pObj ) const
  {
    OdDgColorBookPtr pColorBook = pObj;

    return pColorBook->getColorBookName();
  }

  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSolidDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;

  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDg3dObjectHelper helper( pElm );
    return helper.createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgMeshFaceLoopsDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgSurfaceDumperPE : public OdDgGraphicsElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const
  {
    OdDg3dObjectHelper helper( pElm );
    return helper.createIterator( atBeginning, skipDeleted );
  }
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgProxyElementDumperPE : public OdDgElementDumperPE
{
public:
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgComplexProxyElementDumperPE : public OdDgProxyElementDumperPE
{
public:
  virtual OdDgElementIteratorPtr createIterator( OdDgElementPtr pElm, bool atBeginning = true, bool skipDeleted = true ) const;
  virtual void dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

//!DOM Line Style Definition resource data

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgCompoundLineStyleResourceDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLineCodeResourceDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT OdDgLinePointResourceDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

/**
  <group Dg_Extension_Classes>
*/
class TG_EXPORT  OdDgPointSymbolResourceDumperPE : public OdDgRxObjectDumperPE
{
public:
  virtual void dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const;
};

#endif // _EX_DGNELEMENTDUMPERPE_H_
